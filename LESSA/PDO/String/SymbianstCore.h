/**
	License: zlib/libpng
	Author: Alessio Pierluigi Placitelli
	Company:

	Description: This class holds Symbian String core functionalities implementation

	Revision:

	Todo:
*/

#ifdef COMPILE_FOR_SYMBIAN

#ifndef _SYMBIAN_STRINGCORE_H_
#define _SYMBIAN_STRINGCORE_H_

#include "PDO/String/stCore.h"

namespace PDO
{
	namespace BaseString
	{
		class SymbianCore : public Core
		{
		public:
			inline SymbianCore() : m_pData(LNull) {};
			inline virtual ~SymbianCore() { delete m_pData; m_pData = NULL;};

			// Services
			LUint	Length() const;
			LBool	IsEmpty() const;

			LInt	Replace(const Core *pWhat, const Core *pWith);
			LInt	Replace(const char *pWhat, const char *pWith);

			// Operators
			LVoid	operator = (const char *pContent);
			LVoid	operator = (const Core &rCopy);

			Core&	operator += (const char *pSuffix);
			Core&	operator += (char cSingle);
			Core&	operator += (Core &rSuffix);

			LBool	operator == (const Core &rCompare) const;
			LBool	operator == (const char *pCompare) const;
			LBool	operator < (const Core &rCompare) const;
			LBool	operator > (const Core &rCompare) const;

			char	operator [] (LUint n) const;

			// Raw data access (needed for PDO::FSS::File::Win32Core)
			HBufC	*GetRawData() { return m_pData; }
			LBool	SetFromTDes(const TDesC &rDes);
			void	AppendTDes(const TDesC &rDes);
		protected:

			static TUint strlen(const char *pString);

		private:
			HBufC	*m_pData;
		};
	}
}

#endif // _SYMBIAN_STRINGCORE_H_

#endif // COMPILE_FOR_SYMBIAN