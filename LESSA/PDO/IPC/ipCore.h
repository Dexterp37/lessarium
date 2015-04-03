/** 
 *	\file ipCore.h 
 *	\class PDO::IPC::Core ipCore.h "PDO/IPC/ipCore.h"
 *	\class PDO::IPC::IIPCListener ipCore.h "PDO/IPC/ipCore.h" 
 *	License: zlib/libpng
 *
 *	This class holds IPC core functionalities, needed mechanism
 *  for inter process communication
 *
 *	\author Alessio Pierluigi Placitelli
 *	\version
 *	\todo
 *		- Allow multiple IIPCListener(s)
 *		- Add a function like AcceptConnection(xx) which decides if a CA could 
 *		  communicate with LESSA
 */

#ifndef _IPCCORE_H_
#define _IPCCORE_H_

#include "config.h"
#include "PIO/Utils/llist.h"

namespace PDO
{
	namespace IPC
	{
		class IIPCListener {
		public:
			/**
				Starts up IPC server and listens for other "Lessa Compliant" 
				processes

				\return Returns < 0 if messages were not handled or there
				where errors, 0 if messages are handled and no more listeners
				should receive them, > 0 if messages were handled but not "consumed"
			*/
			virtual LInt HandleMessages() = 0;	
		};

		class Core
		{
		protected:
			Core();

		public:
			typedef struct
			{
				LUlong	m_pProcID;		///< Process ID
				LVoid	*m_lpUserData;	///< Custom user data
			}SIPC_PHANDLE;

			inline virtual ~Core() 
			{
				delete m_pListener;
				m_pListener = LNull;
			};
			
			static Core *createCore();

			/**
				Starts up IPC server and listens for other "Lessa Compliant" 
				processes

				\return Returns LTrue if no error occoured, LFalse otherwise
			*/
			virtual LBool StartAndListen() = 0;
		
		private:
			/**
				Provides default implementation for incoming Compliant Application 
				messages, if listening class isn't handling them

				\see IIPCListener
			*/
			LVoid	HandleMessages();

		protected:
			PIO::Utils::CSingleLinkedList<PDO::IPC::Core::SIPC_PHANDLE>		m_caList;		///< Single linked list holding "Compliant Application" descriptors
			IIPCListener	*m_pListener;	///< A single listener class, which handles CA messages sent to IPC
		};
	}
}

#endif // _IPCCORE_H_