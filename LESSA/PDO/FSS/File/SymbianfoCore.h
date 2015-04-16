/**
	License: zlib/libpng
	Author: Alessio Pierluigi Placitelli
	Company:

	Description: This class holds File class, needed to read/write from files

	Revision:

	Todo:
*/

#ifdef COMPILE_FOR_SYMBIAN

#ifndef _SYMBIAN_FILECORE_H_
#define _SYMBIAN_FILECORE_H_

#include "foCore.h"

namespace PDO
{
	namespace FSS
	{
		namespace File
		{
			class SymbianCore : public Core
			{
			public:
				inline SymbianCore() : m_iFlags(0) {};
				inline virtual ~SymbianCore() { Close(); };		// Todo: check if RFile is valid and Close here?

				LBool Open(const String *path, LInt iOptions);
				LBool Close();
				
				LVoid  *Read(const LInt iToRead);
				String *Read();
				LInt  Write(const LVoid *pBuf, const LInt iBufSize);
				LInt  Write(const String *pStr);

				LInt Seek(const LInt iMode, const LInt iMoveOf);

				inline LBool IsEOF() {return (m_iFlags & 0x2); };

				//Core & operator << (const String &sTxt);
				//Core & operator << (Core &rCore, const String *pTxt);

			protected:
				// Both return number of bytes converted
				static LVoid *TDes8ToLVoid(const TDes8 &rSrc);
				static HBufC8 *LVoidToHBufC8(const LVoid *pSrc, const LInt iBufSize);

			private:
				LInt	m_iFlags;		// End Of File (0x2), File Opened (0x4)..
				RFile	m_rFile;		// RFile instance
			};
		}
	}
}

#endif // _SYMBIAN_FILECORE_H_
#endif // COMPILE_FOR_SYMBIAN