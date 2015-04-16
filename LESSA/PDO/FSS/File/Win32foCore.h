/**
	License: zlib/libpng
	Author: Alessio Pierluigi Placitelli
	Company:

	Description: This class holds File class, needed to read/write from files

	Revision:

	Todo:
*/

#ifdef COMPILE_FOR_WINDOWS

#ifndef _WIN32_FILECORE_H_
#define _WIN32_FILECORE_H_

#include "foCore.h"

namespace PDO
{
	namespace FSS
	{
		namespace File
		{
			class Win32Core : public Core
			{
			public:
				inline Win32Core() : m_iFlags(LNull), m_hFile(INVALID_HANDLE_VALUE), m_bEof(LFalse) {};
				inline virtual ~Win32Core() { Close(); };

				LBool Open(const String *path, LInt iOptions);
				LBool Close();
				
				LVoid  *Read(const LInt iToRead);
				String *Read();
				LInt  Write(const LVoid *pBuf, const LInt iBufSize);
				LInt  Write(const String *pStr);

				LInt Seek(const LInt iMode, const LInt iMoveOf);

				inline LBool IsEOF() {return m_bEof; };

				//Core & operator << (const String &sTxt);
				//Core & operator << (Core &rCore, const String *pTxt);

			private:
				LInt	m_iFlags;		// Flags	
				LBool	m_bEof;			// End Of File 
				HANDLE	m_hFile;		// File handle
			};
		}
	}
}

#endif // _WIN32_FILECORE_H_
#endif // COMPILE_FOR_WINDOWS