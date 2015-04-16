/**
	License: zlib/libpng
	Author: Alessio Pierluigi Placitelli
	Company:

	Description: This class holds Win32 String core functionalities implementation

	Revision:

	Todo:
*/

#ifdef COMPILE_FOR_WINDOWS

#ifndef _WIN32_STRINGCORE_H_
#define _WIN32_STRINGCORE_H_

#include "PDO/String/stCore.h"

namespace PDO
{
	namespace BaseString
	{
		class Win32Core : public Core
		{
		public:
			inline Win32Core() : m_pData(LNull) {};
			inline virtual ~Win32Core() { delete m_pData; m_pData = LNull; };

			// Services
			LUint	Length() const;
			LBool	IsEmpty() const;

			LInt	Replace(const Core *pWhat, const Core *pWith);
			LInt	Replace(const char *pWhat, const char *pWith);

			// Operators
			void	operator = (const char *pContent);
			void	operator = (const Core &rCopy);

			Core&	operator += (const char *pSuffix);
			Core&	operator += (char cSingle);
			Core&	operator += (Core &rSuffix);

			LBool	operator == (const Core &rCompare) const;
			LBool	operator == (const char *pCompare) const;
			LBool	operator < (const Core &rCompare) const;
			LBool	operator > (const Core &rCompare) const;

			char	operator [] (LUint n) const;

			// Raw data access (needed for PDO::FSS::File::Win32Core)
			char *	GetRawData() { return m_pData; }
		protected:
			char	*m_pData;
		};
	}
}

#endif // _WIN32_STRINGCORE_H_

#endif // COMPILE_FOR_WINDOWS