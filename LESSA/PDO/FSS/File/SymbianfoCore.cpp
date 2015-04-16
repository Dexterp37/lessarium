/**
	License: zlib/libpng
	Author: Alessio Pierluigi Placitelli
	Company:

	Description: This class holds Symbian File class implementation, needed to read/write from files

	Revision:
		17/04/2006: Initial draft
		03/05/2006: ASCII to UTF conversion (we always want to write/read 8bit ASCII, to keep
					things compatible with other platforms)
		29/06/2006: Fixed a bug in SymbianCore::Open, which made app crash if file didn't exist

	Todo:
			- Check how to write binary /text blocks of VOID 
*/

#ifdef COMPILE_FOR_SYMBIAN


#include "PDO/FSS/File/SymbianfoCore.h"
#include "PDO/String/SymbianstCore.h"

#include <eikenv.h> 
#include <coeutils.h>
#include <s32file.h>
#include <utf.h>	// 

// DEBUG*
#include "PDO/String/SymbianstCore.h"
#include <aknnotewrappers.h>	// To use CAknInformationNote
// DEBUG

#pragma warning(disable:4800)

namespace PDO
{
	namespace FSS
	{
		namespace File
		{
			Core *Core::createCore()
			{
				return new SymbianCore();
			}

			LBool SymbianCore::Open(const String *path, LInt iOptions)
			{
				if(!path || path->IsEmpty())
					return LFalse;

				PDO::BaseString::SymbianCore *pPath = (PDO::BaseString::SymbianCore*)path;

				// Dexter: build CreateFile parameteres
				TUint uDesiredAccess = ((iOptions & LFILE_READ) ? EFileRead : EFileWrite) 
										| ((iOptions & LFILE_WRITE) ? EFileWrite : EFileRead);

				uDesiredAccess |= (iOptions & LFILE_TEXT) ? EFileStreamText : EFileStream;

				// Workaround: that's needed because epoc won't let you write to Z:
				#ifdef __WINS__
				pPath->Replace("Z:", "C:");
				ConeUtils::EnsurePathExistsL(*(pPath->GetRawData()));
				#endif

				TInt iErr = KErrNone;

				if((iErr = m_rFile.Open(CEikonEnv::Static()->FsSession(), *(pPath->GetRawData()), uDesiredAccess)) != KErrNone)
				{
					if(!(iOptions & LFILE_OVERWRITE_OR_CREATE) || (iErr = m_rFile.Create(CEikonEnv::Static()->FsSession(), *(pPath->GetRawData()), uDesiredAccess)) != KErrNone)
						return LFalse;
				}

				m_iFlags = m_iFlags | 0x4;

				return LTrue;
			}

			LBool SymbianCore::Close()
			{
				if(!(m_iFlags & 0x4))
					return LTrue;

				TRAPD(err, m_rFile.Flush());

				m_rFile.Close();

				return (err == KErrNone);
			}

			LVoid *SymbianCore::Read(const LInt iToRead)
			{
				if(!iToRead)
					return LNull;	// Paranoid

				
				TBuf8<512> tDes;	// More?
				if(m_rFile.Read(tDes, iToRead) != KErrNone)
				{
					return LNull;
				}

				LVoid *pRet = TDes8ToLVoid(tDes);
				
				return pRet;
			}

			String *SymbianCore::Read()
			{
				TBuf8<256>	buf8Bit;
				TBuf<256>	bufUnicode;
				TInt		iTotalRead = 0;
				TInt		iErr = KErrNone;
				MakeString(retStr);

				PDO::BaseString::SymbianCore *pSymStr = (PDO::BaseString::SymbianCore*)retStr;

				do
				{
					// Read small chunks
					if((iErr = m_rFile.Read(buf8Bit, 255)) != KErrNone)
							break;

					if(iErr == KErrNone && buf8Bit.Length() == 0)
					{
						m_iFlags = m_iFlags | 0x2;
						break;
					}

					// How much data we read?
					iTotalRead += buf8Bit.Length();

					TInt iPos = buf8Bit.Locate(TChar('\n'));
					if(iPos != KErrNotFound)
					{
						// We found end of line
						CnvUtfConverter::ConvertToUnicodeFromUtf8(bufUnicode, buf8Bit.Left(iPos));
						pSymStr->AppendTDes(bufUnicode);     

						// Rewind to the \n
						TInt iRewAmount = iTotalRead - (retStr->Length() + 1);
						iRewAmount = -iRewAmount;
					/*	// Debug purposes.
	   					CAknInformationNote* informationNote;
						informationNote = new (ELeave) CAknInformationNote(LTrue);
						informationNote->ExecuteLD(*(pSymStr->GetRawData()));
					*/
						m_rFile.Seek(ESeekCurrent, iRewAmount);

						return retStr;
					}

					CnvUtfConverter::ConvertToUnicodeFromUtf8(bufUnicode, buf8Bit);
					pSymStr->AppendTDes(bufUnicode);        

				}while(1);

				if(!retStr->IsEmpty())
					return retStr;

				// Free if empty
				delete retStr;

				return LNull;
			}

			LInt SymbianCore::Write(const LVoid *pBuf, const LInt iBufSize)
			{
				//m_rFile.Write(

				return -1;
			}

			LInt SymbianCore::Write(const String *pStr)
			{
				if(!pStr)// || !pStr->Length())
					return 0;

				LUint uLen = pStr->Length();
				if(uLen == 0)
					return 0;

				TInt iErr = KErrNone;
				TFileText txtFile;

				txtFile.Set(m_rFile);
				if((iErr = txtFile.Write(*((PDO::BaseString::SymbianCore*)pStr)->GetRawData())) != KErrNone)  
				{
					return 0;
				}
				
				return pStr->Length();				
			}
			
			LInt SymbianCore::Seek(const LInt iMode, const LInt iMoveOf)
			{
				if(!iMoveOf)
					return 0;

				TSeek aMode;

				switch(iMode)
				{
				case LFILE_SEEK_START:
					aMode = ESeekStart;
					break;
				case LFILE_SEEK_CURRENT:
					aMode = ESeekCurrent;
					break;
				case LFILE_SEEK_END:
					aMode = ESeekEnd;
					break;
				}

				LInt iTmp = iMoveOf;
				m_rFile.Seek(aMode, iTmp);

				return 1; // Return how much we moved of?
			}
			
			/*

			Core& Win32Core::operator << (const String &sTxt)
			{
				Write(&sTxt);

				return *this;
			}

			Core &Win32Core::operator << (cCore &rCore, const String *pTxt)
			{
				Write(pTxt);

				return *this;
			}*/

			// Both return number of bytes converted
			LVoid *SymbianCore::TDes8ToLVoid(const TDes8 &rSrc)
			{
				LInt iLen = rSrc.Length();
				if(!iLen)
					return LNull;

				char *pBuff = new char[iLen];
				if(!pBuff)
					return LNull;

				for(LInt k = 0; k < iLen; k++)
					pBuff[k] = rSrc[k];

				return (LVoid*)pBuff;
			}

			HBufC8 *SymbianCore::LVoidToHBufC8(const LVoid *pSrc, const LInt iBufSize)
			{
				HBufC8 *pBuf = HBufC8::New(iBufSize);
				if(!pBuf)
				{
					return LNull;
				}

				char *pCharBuf = (char*)pSrc;
				
				TPtr8 ptr = pBuf->Des();
				for(LInt k = 0; k < iBufSize; k++)
					ptr.Append(pCharBuf[k]);
				
				return pBuf;	
			}
		}

	}
}

#endif // COMPILE_FOR_SYMBIAN