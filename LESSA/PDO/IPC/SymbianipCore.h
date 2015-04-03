/**
	License: zlib/libpng
	Author: Alessio Pierluigi Placitelli
	Company:

	Description: This class holds Symbian IPC core functionalities implementation

	Revision:

	Todo:
*/

#ifdef COMPILE_FOR_SYMBIAN

#ifndef _SYMBIAN_IPCORE_H_
#define _SYMBIAN_IPCORE_H_

#include "PDO/IPC/ipCore.h"
#include <e32base.h>

namespace PDO
{
	namespace IPC
	{
		class SymbianCore : public Core
		{
		public:
			inline SymbianCore() {};
			inline virtual ~SymbianCore(){};

			LBool StartAndListen();

		private:
			class CIPCSymbianServer : public CServer2
			{
			public:
				CSession2* NewSessionL(const TVersion& aVersion,const RMessage2& aMessage) const;

				static TInt ThreadFunction(TAny* aStarted);

				SymbianCore *m_pCore;	// PDO::IPC::Core pointer 

			protected:
				CIPCSymbianServer(CActive::TPriority aActiveObjectPriority);
				~CIPCSymbianServer() {m_pCore = LNull;}

			friend class SymbianCore;
			};

			/*
				This represents a single Client-Server session. 
				All messages from client pass to DispatchMessageL and are redirected
				to HandleMessage in PDO::IPC::Core
			*/
			class CIPCSymbianSession : public CSession2
			{
			public:
				CIPCSymbianSession(){};

				void ServiceL(const RMessage2& aMessage);
				void DispatchMessageL(const RMessage2& aMessage);
/*
				//services available to initialize/increase/decrease/reset and return the counter value.
				void SetFromStringL(const RMessage2& aMessage);
				void Increase();
				void Decrease();
				void IncreaseBy(const RMessage2& aMessage);
				void DecreaseBy(const RMessage2& aMessage);
				void CounterValue(const RMessage2& aMessage);
				void Reset();

			protected:
				// panic the client
				void PanicClient(const RMessage2& aMessage,TInt aPanic) const;
					
			private:
				TInt iCount;*/
			};

			RThread		m_rServerThread;

		};
	}
}

#endif // _SYMBIAN_IPCORE_H_

#endif // COMPILE_FOR_SYMBIAN