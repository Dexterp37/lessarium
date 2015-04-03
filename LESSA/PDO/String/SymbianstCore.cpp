/**
	License: zlib/libpng
	Author: Alessio Pierluigi Placitelli
	Company:

	Description: This class holds Symbian String core functionalities implementation

	Revision:
		09/04/2006: Initial draft

	Todo:
*/

#ifdef COMPILE_FOR_SYMBIAN

#include "PDO/String/SymbianstCore.h"

namespace PDO
{
	namespace BaseString
	{
		Core * Core::createCore()
		{
			return new SymbianCore();
		}

		LUint SymbianCore::Length() const
		{
			return (m_pData) ? m_pData->Length() : 0;
		}

		LBool SymbianCore::IsEmpty() const
		{
			return Length() ? LFalse : LTrue;
		}

		LInt SymbianCore::Replace(const Core *pWhat, const Core *pWith)
		{
			if(!pWhat || !pWith || pWhat->IsEmpty() || pWith->IsEmpty())
				return -1;

			TDesC &rWhat = *(((PDO::BaseString::SymbianCore*)pWhat)->GetRawData());
			TDesC &rWith = *(((PDO::BaseString::SymbianCore*)pWith)->GetRawData());

			TInt iHowMany = 0;
			TInt iWhatLen = rWhat.Length();
			TInt iFoundOffset = KErrNotFound;
			TPtr pStr = m_pData->Des();

			// Look for pWhat in the string
			while((iFoundOffset = pStr.Find(rWhat)) != KErrNotFound)
			{
				pStr.Delete(iFoundOffset, iWhatLen);
				pStr.Insert(iFoundOffset, rWith);

				++iHowMany;
			}

			return iHowMany;
		}

		LInt SymbianCore::Replace(const char *pWhat, const char *pWith)
		{
			MakeString(strWhat);
			MakeString(strWith);

			*strWhat = pWhat;
			*strWith = pWith;
			
			LInt iRet = Replace(strWhat, strWith);

			// Clean and avoid memory leaks
			delete strWhat;
			delete strWith;

			return iRet;
		}

		LVoid SymbianCore::operator = (const char *pContent)
		{
			delete m_pData;						// delete already checks for NULLs
			LInt iLength = strlen(pContent);
			m_pData = HBufC::NewL(iLength);

			TPtr pStr = m_pData->Des();
			for(LInt i = 0; i < iLength; i++)
				pStr.Append(TChar(pContent[i]));
		}

		LVoid SymbianCore::operator = (const Core &rCopy)
		{
			delete m_pData;						// delete already checks for NULLs
			LUint uLength = rCopy.Length();
			m_pData = HBufC::NewL(uLength);

			TPtr pStr = m_pData->Des();
			for(LUint i = 0; i < uLength; i++)
				pStr.Append(rCopy[i]);
		}

		Core& SymbianCore::operator += (const char *pSuffix)
		{
			// Note: should we check for NULL m_pData also here?
			HBufC *pTmpData = m_pData;		// Dexter: Backup our old string

			TUint uSuffLen = strlen(pSuffix); 
			TUint uLen = Length() + uSuffLen;	// Get new length
			m_pData = HBufC::NewL(uLen);

			// Append suffix
			TPtr pStr = m_pData->Des();

			if(pTmpData)
				pStr.Copy(pTmpData->Des());		// Copy old string to new one
			
			for(LUint i = 0; i < uSuffLen; i++)
				pStr.Append(TChar(pSuffix[i]));

			delete pTmpData;

			return *this;
		}

		Core& SymbianCore::operator += (char cSingle)
		{
			HBufC *pTmpData = m_pData;		// Dexter: Backup our old string

			TInt iLen = Length() + 1;		// Get new length
			m_pData = HBufC::NewL(iLen);

			// Append suffix
			TPtr pStr = m_pData->Des();

			if(pTmpData)
				pStr.Copy(pTmpData->Des());		// Copy old string to new one
			
			pStr.Append(TChar(cSingle));

			delete pTmpData;

			return *this;
		}

		Core& SymbianCore::operator += (Core &rSuffix)
		{
			HBufC *pTmpData = m_pData;				// Dexter: Backup our old string

			TInt iSuffLen = rSuffix.Length();
			TInt iLen = Length() + iSuffLen;		// Get new length

			m_pData = HBufC::NewL(iLen);

			TPtr pStr = m_pData->Des();

			if(pTmpData)
				pStr.Copy(pTmpData->Des());
			
			for(LInt i = 0; i < iSuffLen; i++)
				pStr.Append(rSuffix[i]);

			delete pTmpData;

			return *this;
		}

		LBool SymbianCore::operator == (const Core &rCompare) const
		{
			// Dexter: if lengths differs, then string is different.
			LUint iLength = Length();

			if(iLength != rCompare.Length())
				return LFalse;

			// Check to see if strings are the same
			for(LUint u = 0; u < iLength; u++)
				if((*this)[u] != rCompare[u])
					return LFalse;
			

			return LTrue;
		}

		LBool SymbianCore::operator == (const char *pCompare) const
		{
			// Dexter: if lengths differs, then string is different.
			LUint iLength = Length();
			LUint iCompLen = strlen(pCompare);

			if(iLength != iCompLen)
				return LFalse;

			// Check to see if strings are the same
			for(LUint u = 0; u < iLength; u++)
				if((*this)[u] != pCompare[u])
					return LFalse;
			

			return LTrue;
		}

		LBool SymbianCore::operator < (const Core &rCompare) const
		{
			return LFalse;
		}

		LBool SymbianCore::operator > (const Core &rCompare) const
		{
			return LFalse;
		}

		char SymbianCore::operator [] (LUint n) const
		{
			return (m_pData) ? (m_pData->Des()[n]) : LNull;
		}

		LBool SymbianCore::SetFromTDes(const TDesC &rDes)
		{
			delete m_pData;						// delete already checks for NULLs
			
			LInt iLength = rDes.Length();
			m_pData = HBufC::NewL(iLength);

			TPtr pStr = m_pData->Des();
			pStr.Copy(rDes);

			return LTrue;
		}

		void SymbianCore::AppendTDes(const TDesC &rDes)
		{
			// Note: should we check for NULL m_pData also here?
			HBufC *pTmpData = m_pData;		// Dexter: Backup our old string

			TUint uSuffLen = rDes.Length();
			if(!uSuffLen)
				return;

			TUint uLen = Length() + uSuffLen;	// Get new length
			m_pData = HBufC::NewL(uLen);

			// Append suffix
			TPtr pStr = m_pData->Des();

			if(pTmpData)
			{
				pStr.Copy(pTmpData->Des());		// Copy old string to new one
				pStr.Append(rDes);
			}else
			{
				pStr.Copy(rDes);
			}

			delete pTmpData;
		}

		TUint SymbianCore::strlen(const char *pString)
		{
			TUint uSize = 0;

			while(pString[uSize] != '\0')
				uSize++;
			
			return uSize;
		}
	}
}

#endif // COMPILE_FOR_SYMBIAN