/**
	License: zlib/libpng
	Author: Alessio Pierluigi Placitelli
	Company:

	Description: This class holds Win32 File class implementation, needed to read/write from files

	Revision:
		14/04/2006: Initial draft
		30/04/2006: Using stdlib instead of windows api.. they suit our needs better!
		13/05/2006: Fixed a little bug in String *Win32Core::Read() which returning empty 
					strings after first call

	Todo:
*/

#ifdef COMPILE_FOR_WINDOWS


#include "PDO/FSS/File/Win32foCore.h"
#include "PDO/String/Win32stCore.h"	

#pragma warning(disable:4800)

namespace PDO
{
	namespace FSS
	{
		namespace File
		{
			Core *Core::createCore()
			{
				return new Win32Core();
			}

			LBool Win32Core::Open(const String *path, LInt iOptions)
			{
				m_iFlags = iOptions;

				PDO::BaseString::Win32Core *pPath = (PDO::BaseString::Win32Core*)path;

				SECURITY_ATTRIBUTES attr = { sizeof(SECURITY_ATTRIBUTES), NULL, TRUE, };

				// Dexter: build CreateFile parameteres
				DWORD dwDesiredAccess = (iOptions & LFILE_READ) ? GENERIC_READ : GENERIC_WRITE;
				if(iOptions & LFILE_WRITE)
					dwDesiredAccess = dwDesiredAccess | GENERIC_WRITE;

				// Call CreateFile
				m_hFile = CreateFile(pPath->GetRawData(), dwDesiredAccess, /*FILE_SHARE_READ*/0, &attr,
									 (iOptions & LFILE_OVERWRITE_OR_CREATE) ? CREATE_ALWAYS : OPEN_EXISTING,
									 FILE_ATTRIBUTE_NORMAL, NULL);

				return (m_hFile == INVALID_HANDLE_VALUE) ? LFalse : LTrue;
			}

			LBool Win32Core::Close()
			{
				if(m_hFile == INVALID_HANDLE_VALUE)
					return LTrue;

				if(CloseHandle(m_hFile))
				{
					m_hFile = INVALID_HANDLE_VALUE;
					return LTrue;
				}

				return LFalse;
			}

			LVoid *Win32Core::Read(const LInt iToRead)
			{
				if(!iToRead)
					return LNull;

				LVoid *pBuf = malloc(iToRead);
				if(!pBuf)
					return LNull;

                DWORD dwBytesRead = 0;
				LBool bRes = ReadFile(m_hFile, pBuf, iToRead, &dwBytesRead, NULL);
				if(bRes && (dwBytesRead == 0))
				{
					m_bEof = LTrue;
				}

				return pBuf;
			}

			String *Win32Core::Read()	// Not efficient at all...
			{
				if(!(m_iFlags & LFILE_TEXT))
					return LNull;

				DWORD dwBytesRead = 0;
				char szTemp[256];
				LInt iToRead = 255;
				LONG lTotalRead = 0; 

				MakeString(szToRet);

				do
				{
					// Read small chunks of data... and look for \r\n
					LBool bRes = ReadFile(m_hFile, szTemp, iToRead, &dwBytesRead, NULL);
					if(bRes && (dwBytesRead == 0))
					{
						m_bEof = LTrue;
						break;
					}

					// Count how much we read (if we did it multiple times)
					lTotalRead += dwBytesRead;

					// Look for \r\n in szTemp
					for(LInt i = 0; i < 255; i++)
					{
						if(szTemp[i] == '\r' && szTemp[i + 1] == '\n')
						{
                            char szTemp2[256];
							strncpy(szTemp2, szTemp, i);
							szTemp2[i] = '\0';
							*szToRet += szTemp2;

							// Now rewind to last End of Line (don't forget carriage return..)
							LONG iMoveOf = lTotalRead - (szToRet->Length() + 2);	
							SetFilePointer(m_hFile, -iMoveOf, NULL, FILE_CURRENT);

							return szToRet;
						}
					}

					*szToRet += szTemp;

				}while(1);	// Wohoo.. dangerous...

				if(szToRet->IsEmpty())
				{
					delete szToRet;
					szToRet = NULL;
				}

				return szToRet;
			}

			LInt Win32Core::Write(const LVoid *pBuf, const LInt iBufSize)
			{
				DWORD dwBytesWritten = 0;

				// Dexter: should we check for errors here?
				WriteFile(m_hFile, pBuf, iBufSize, &dwBytesWritten, NULL);
				
				return dwBytesWritten;
			}

			LInt Win32Core::Write(const String *pStr)
			{
				if(!pStr)// || !pStr->Length())
					return 0;

				LUint uLen = pStr->Length();
				if(uLen == 0)
					return 0;

				DWORD dwBytesWritten = 0;
				LVoid *pBuf = (((PDO::BaseString::Win32Core*)pStr)->GetRawData());	// Dirty 

				// Dexter: should we check for errors here?
				WriteFile(m_hFile, pBuf, uLen, &dwBytesWritten, NULL);
				
				return dwBytesWritten;				
			}
			
			LInt Win32Core::Seek(const LInt iMode, const LInt iMoveOf)
			{
				if(!iMoveOf)
					return 0;

				DWORD dwMode = 0L;
				switch(iMode)
				{
				case LFILE_SEEK_START:
					dwMode = FILE_BEGIN;
					break;
				case LFILE_SEEK_CURRENT:
					dwMode = FILE_CURRENT;
					break;
				case LFILE_SEEK_END:
					dwMode = FILE_END;
					break;
				default:
					return 0;
				}

				if(!SetFilePointer(m_hFile, iMoveOf, NULL, dwMode))
					return 0;

				return 1;
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
		}

	}
}

#endif // COMPILE_FOR_WINDOWS