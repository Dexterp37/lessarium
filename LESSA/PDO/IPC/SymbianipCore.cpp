/**
	License: zlib/libpng
	Author: Alessio Pierluigi Placitelli
	Company:

	Description: This class holds Symbian IPC core functionalities implementation

	Revision:

	Todo:
*/

#ifdef COMPILE_FOR_SYMBIAN

#include "PDO/IPC/SymbianipCore.h"
#include <e32svr.h>
#include <e32uid.h>

namespace PDO
{
	namespace IPC
	{
		_LIT(KServerName,"LESSAGeneric");

		Core * Core::createCore()
		{
			return new SymbianCore();
		}

		LBool SymbianCore::StartAndListen()
		{
			TInt res = KErrNone;
			
			TFindServer findCountServer(KServerName);
			TFullName   name;
			
			// Need to check that the server exists.
			if(findCountServer.Next(name) != KErrNone)
			{
				// Create the thread for the server.
				res = m_rServerThread.Create(KServerName,
					CIPCSymbianServer::ThreadFunction,
					KDefaultStackSize,
					0x10000,
					0x10000,
					(TAny*)this);

				// The thread has been created OK so get it started - however
				// we need to make sure that it has started before we continue.
				if(res == KErrNone)
				{
					TRequestStatus rendezvousStatus;
					
					m_rServerThread.SetPriority(EPriorityNormal);
					m_rServerThread.Rendezvous(rendezvousStatus);
					m_rServerThread.Resume();
					User::WaitForRequest(rendezvousStatus);
				}else
				{
					m_rServerThread.Close();
				}
			}
			
			return (res == KErrNone) ? LTrue : LFalse;
		}

		/***********************
			CIPCSymbianServer
		************************/
		SymbianCore::CIPCSymbianServer::CIPCSymbianServer(CActive::TPriority aActiveObjectPriority)
			: CServer2(aActiveObjectPriority), m_pCore(LNull)
		{
		
		}

		CSession2* SymbianCore::CIPCSymbianServer::NewSessionL(const TVersion& aVersion, const RMessage2& /*aMessage*/) const
		{
			TVersion v(0, 6, 0);	// TODO: this is the server protocol version. Hardcode it somewhere else?
			if(!User::QueryVersionSupported(v, aVersion))
				return NULL; //User::Leave(KErrNotSupported);	
			
			// CAN USE THE aMessage argument to check client's security and identity
			// can make use of this later but for now ignore. AH 4/5/05
			// the connect message is delivered via the RMessage2 object passed. 
			
			// do something with this later (and move it to the start of the function?)
			PDO::IPC::Core::SIPC_PHANDLE *pHandle = new PDO::IPC::Core::SIPC_PHANDLE;
			if(pHandle)
			{
				pHandle->m_pProcID = 0;
				pHandle->m_lpUserData = (LVoid*) new CIPCSymbianSession();
				if(!pHandle->m_lpUserData)
				{
					delete pHandle;
					return LNull;
				}

				m_pCore->m_caList.AddHead((PDO::IPC::Core::SIPC_PHANDLE*)pHandle);

                return (CSession2*)(pHandle->m_lpUserData);
			}
			
			// Create the session.
			return NULL;  //new (ELeave) CCountServSession;
		}

		TInt SymbianCore::CIPCSymbianServer::ThreadFunction(TAny *aStarted)
		{
			// get clean-up stack
			CTrapCleanup* cleanup=CTrapCleanup::New();
			if (cleanup == NULL)
			{
				//CCountServServer::PanicServer(ECreateTrapCleanup);
				return 0; // Correct?
			}
			
			// create an active scheduler and server
			CActiveScheduler *pA = new CActiveScheduler;
			if(!pA)
			{
				delete cleanup;
				return 0;
			}

			SymbianCore::CIPCSymbianServer *pS = new SymbianCore::CIPCSymbianServer(EPriorityStandard);
			if(!pS)
			{
				delete pA;
				delete cleanup;
				return 0;
			}	

			// Save PDO::IPC::Core pointer 
			pS->m_pCore = (SymbianCore*)aStarted;

			//Install the active scheduler
			CActiveScheduler::Install(pA);
				
			// Start the server
			TInt err = pS->Start(KServerName);
			if (err != KErrNone)
			{
				//CCountServServer::PanicServer(ESvrStartServer);
				return 0;
			}
			
			// Let everyone know that we are ready to
			// deal with requests.
			RThread::Rendezvous(KErrNone);
			
			// And start fielding requests from client(s).
			CActiveScheduler::Start();

			// Tidy up... 	
			delete pS;
			delete pA;
			delete cleanup; 
			
			// ...although we should never get here!
			return(KErrNone);
		}

		/***********************
			CIPCSymbianSession
		************************/
		void SymbianCore::CIPCSymbianSession::ServiceL(const RMessage2& aMessage)
		{
			TRAPD(err,DispatchMessageL(aMessage));
			aMessage.Complete(err);
		}

		void SymbianCore::CIPCSymbianSession::DispatchMessageL(const RMessage2& aMessage)
		{
			// This will handle messages sent by client
			switch (aMessage.Function())
			{
/*			case ECountServSetFromString:
				SetFromStringL(aMessage);
				return;
			case ECountServIncrease:
				Increase();
				return;
			case ECountServIncreaseBy:
				IncreaseBy(aMessage);
				return;
			case ECountServDecrease:
				Decrease();
				return;
			case ECountServDecreaseBy:
				DecreaseBy(aMessage);
				return;
			case ECountServReset:
				Reset();
				return;
			case ECountServValue:
				CounterValue(aMessage);
				return;
			  
			// This is an example of a request that we know about, but don't support.
			// We cause KErrNotSupported to be returned to the client.
			case ECountServUnsupportedRequest:
				User::Leave(KErrNotSupported);
				
			//  Requests that we don't understand at all are a different matter.
			//  This is considered a client programming error, so we panic the 
			//  client - this also completes the message.*/
			default:
				//PanicClient(aMessage,EBadRequest);
				return;
			}
		}

		/**
		Initialize the counter with the numeric equivalent of the descriptor contents
		This function is here to demonstrate reading from the client address space.
		Note that in this example, the client and the server are part of the same process,
		*/
/*		void CCountServSession::SetFromStringL(const RMessage2& aMessage)
			{
			
			// length of passed descriptor (1st parameter passed from client)
			TInt deslen = aMessage.GetDesLength(0);
			
			// Passed data will be saved in this descriptor.
			RBuf buffer;
		      
			// Max length set to the value of "deslen", but current length is zero
			buffer.CreateL(deslen);
		      
			// Do the right cleanup if anything subsequently goes wrong
			buffer.CleanupClosePushL();
		    
			// Copy the client's descriptor data into our buffer.
			aMessage.ReadL(0,buffer,0);
		    
			// Now do a validation to make sure that the string only has digits
			if (buffer.Length() == 0)
				{
    			User::Leave(ENonNumericString);
				}
		    
			TLex16 lexer;
		    
			lexer.Assign(buffer);
			while (!lexer.Eos())
				{
				TChar thechar;
		        
				thechar = lexer.Peek();
				if (!thechar.IsDigit())
					{
        			User::Leave(ENonNumericString);
					}
				lexer.Inc();
				}
		       
			// Convert to a simple TInt value. 
			lexer.Assign(buffer);           
			if (lexer.Val(iCount))
				{
    			User::Leave(ENonNumericString);
				}
		    	
			// Clean up the memory acquired by the RBuf variable "buffer"
			CleanupStack::PopAndDestroy();
			}


		void CCountServSession::Increase()
			{
			iCount++;
			}



		void CCountServSession::IncreaseBy(const RMessage2& aMessage)
			{
			iCount = iCount + aMessage.Int0();
			}


		void CCountServSession::Decrease()
			{
			iCount--;
			}


		void CCountServSession::DecreaseBy(const RMessage2& aMessage)
			{
			iCount = iCount - aMessage.Int0();	
			}


	
		void CCountServSession::Reset()
			{
			iCount=0;
			}



		void CCountServSession::CounterValue(const RMessage2& aMessage)
			{
			TPckgBuf<TInt> p(iCount);
			aMessage.WriteL(0,p);
			}


	
		void CCountServSession::PanicClient(const RMessage2& aMessage,TInt aPanic) const
			{
			_LIT(KTxtServer,"CountServ server");
			aMessage.Panic(KTxtServer,aPanic);
			}*/
	}
}

#endif // COMPILE_FOR_SYMBIAN