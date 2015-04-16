/**
	License: zlib/libpng
	Author: Alessio Pierluigi Placitelli
	Company:

	Description: This class holds Win32 String core functionalities implementation

	Revision:
		04/04/2006: Initial draft
		14/05/2006:	Fixed a but in operator += (char cSingle) which cause PDO to crash
		29/06/2006: Fixed missing ! in Win32Core::operator ==

	Todo:
*/

#ifdef COMPILE_FOR_WINDOWS

#include "PDO/String/Win32stCore.h"
#include <string>

namespace PDO
{
	namespace BaseString
	{
		Core * Core::createCore()
		{
			return new Win32Core();
		}

		LUint Win32Core::Length() const
		{
			return (m_pData) ? (LUint)strlen(m_pData) : 0;
		}

		LBool Win32Core::IsEmpty() const
		{
			return Length() ? LFalse : LTrue;
		}

		LInt Win32Core::Replace(const Core *pWhat, const Core *pWith)
		{
			return -1;
		}

		LInt Win32Core::Replace(const char *pWhat, const char *pWith)
		{
			if(!pWhat || !pWith)
				return -1;
/*
			char *pStr = NULL;
			LInt iWhatLen = strlen(pWhat);

			while((pStr = strstr(m_pData, pWhat)) != NULL)
			{

			}*/

			return -1;
		}

		void Win32Core::operator = (const char *pContent)
		{
			if(!pContent)
				return;

			delete [] m_pData;	// Dexter: since that's an array..

			// Allocate space and copy string
			LUint uStrLen = (LUint)strlen(pContent);
			m_pData = new char[uStrLen + 1];
			strcpy(m_pData, pContent);
		}

		void Win32Core::operator = (const Core &rCopy)
		{
			delete [] m_pData;	// Dexter: since that's an array..

			// Allocate space and copy string
			LUint uStrLen = rCopy.Length();
			m_pData = new char[uStrLen + 1];
			for(LUint u = 0; u < uStrLen; u++)
				m_pData[u] = rCopy[u];

			m_pData[uStrLen + 1] = '\0';	// Append a null character
		}

		Core& Win32Core::operator += (const char *pSuffix)
		{
			if(!pSuffix || !strlen(pSuffix))
				return *this;

			char *pTmpData = m_pData;	// Backup old data

			// Allocate space and concatenate strings
			LUint uNewLen = Length() + (LUint)strlen(pSuffix);
			m_pData = new char[uNewLen + 1];

			if(pTmpData)
			{
				strcpy(m_pData, pTmpData);
				strcat(m_pData, pSuffix);
			}else
			{
				strcpy(m_pData, pSuffix);
			}

			delete pTmpData;	// Free old data

			return *this;
		}

		Core& Win32Core::operator += (char cSingle)
		{
			char *pTmpData = m_pData;	// Backup old data

			// Allocate space and concatenate strings
			LUint uNewLen = Length() + 1;
			m_pData = new char[uNewLen + 1];

			if(pTmpData)
			{
				strcpy(m_pData, pTmpData);
				m_pData[uNewLen - 1] = cSingle;
				m_pData[uNewLen] = '\0';
			}else
			{
				m_pData[uNewLen - 1] = cSingle;
				m_pData[uNewLen] = '\0';
			}

			delete pTmpData;	// Free old data

			return *this;
		}

		Core& Win32Core::operator += (Core &rSuffix)
		{
			LUint uSuffLen = rSuffix.Length();
			if(!uSuffLen)
				return *this;

			char *pTmpData = m_pData;

			// Allocate space and concatenate strings
			LUint uNewLen = Length() + uSuffLen;
			m_pData = new char[uNewLen];

			if(pTmpData)
			{
				strcpy(m_pData, pTmpData);
				strcat(m_pData, ((Win32Core*)&rSuffix)->GetRawData());
			}else
			{
				strcpy(m_pData, ((Win32Core*)&rSuffix)->GetRawData());
			}

			return *this;
		}

		LBool Win32Core::operator == (const Core &rCompare) const
		{
			PDO::BaseString::Win32Core *pRaw = (PDO::BaseString::Win32Core *)&rCompare;

			return (strcmp(m_pData, pRaw->GetRawData()) == 0);
		}

		LBool Win32Core::operator == (const char *pCompare) const
		{
			if(!pCompare || !m_pData)
				return LFalse;

			return (strcmp(m_pData, pCompare) == 0);
		}

		LBool Win32Core::operator < (const Core &rCompare) const
		{
			PDO::BaseString::Win32Core *pRaw = (PDO::BaseString::Win32Core *)&rCompare;

			return (strcmp(m_pData, pRaw->GetRawData()) < 0);
		}

		LBool Win32Core::operator > (const Core &rCompare) const
		{
			PDO::BaseString::Win32Core *pRaw = (PDO::BaseString::Win32Core *)&rCompare;

			return (strcmp(m_pData, pRaw->GetRawData()) > 0);
		}

		char  Win32Core::operator [] (LUint n) const
		{
			return (n < Length()) ? m_pData[n] : 0;
		}

	}
}

#endif // COMPILE_FOR_WINDOWS