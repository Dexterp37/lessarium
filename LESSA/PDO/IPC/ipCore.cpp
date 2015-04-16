/** 
 *	\file ipCore.cpp
 *	License: zlib/libpng
 *
 *	This class holds IPC core constructor implementation
 *
 *	\author Alessio Pierluigi Placitelli
 */
//$log$ 01/08/2006 Initial draft

#include "PDO/IPC/ipCore.h"

namespace PDO
{
	namespace IPC
	{
		Core::Core() : m_pListener(LNull)
		{

		}

		LVoid Core::HandleMessages()
		{
			if(m_pListener && (m_pListener->HandleMessages() < 0))
				return;
		}
	}
}