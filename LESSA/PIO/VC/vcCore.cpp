/** 
 *	\file vcCore.cpp
 *	License: zlib/libpng
 *
 *	This class holds VC core implementation
 *
 *	\author Alessio Pierluigi Placitelli
 *	\version
 *	\todo
 *		- Probably it could be done in a more efficient way
 */

#include "PIO/VC/vcCore.h"

namespace PIO
{
	namespace VC
	{
		LBool Core::CheckTimestamps(const LDate *pLocal, const LDate *pRemote)
		{
			if(pRemote->m_uYear > pLocal->m_uYear)
				return LTrue;
			else if(pRemote->m_uYear < pLocal->m_uYear)
				return LFalse;

			if(pRemote->m_uMonth > pLocal->m_uMonth)
				return LTrue;
			else if(pRemote->m_uMonth < pLocal->m_uMonth)
				return LFalse;

			if(pRemote->m_uDay > pLocal->m_uDay)
				return LTrue;
			else if(pRemote->m_uDay < pLocal->m_uDay)
				return LFalse;

			if(pRemote->m_uHour > pLocal->m_uHour)
				return LTrue;
			else if(pRemote->m_uHour < pLocal->m_uHour)
				return LFalse;

			if(pRemote->m_uMinute > pLocal->m_uMinute)
				return LTrue;
			else if(pRemote->m_uMinute < pLocal->m_uMinute)
				return LFalse;

			if(pRemote->m_uSecond > pLocal->m_uSecond)
				return LTrue;
			else if(pRemote->m_uSecond < pLocal->m_uSecond)
				return LFalse;

			return LFalse;
		}
	}
}