/** 
 *	\file vcCore.h 
 *	\class PIO::VC::Core vcCore.h "PIO/VC/vcCore.h"
 *	License: zlib/libpng
 *
 *	This class is needed to compare version timestamps
 *	between local and remote content
 *
 *	\author Alessio Pierluigi Placitelli
 *	\version
 *	\todo
 *		
 */
#ifndef _VCCORE_H_
#define _VCCORE_H_

#include "config.h"

namespace PIO
{
	namespace VC
	{
		class Core
		{
		public:
			Core() {};
			inline virtual ~Core() { };

			/**
				Superficially read the XML application registry, extracting few information

				\param[in] pLocal A LDate parameter containing resource URL
				\param[in] pRemote A LDate parameter describing where to save the file
				\return Returns LTrue if remote content is more recent than local, LFalse otherwise
				\note Simply compares dates
				\see LDate
			*/	
			static LBool CheckTimestamps(const LDate *pLocal, const LDate *pRemote);	
		};
	}
}

#endif // _ARCORE_H_