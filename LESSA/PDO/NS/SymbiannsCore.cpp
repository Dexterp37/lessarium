/**
	License: zlib/libpng
	Author: Alessio Pierluigi Placitelli
	Company:

	Description: This class holds Symbian NS core functionalities implementation

	Revision:

	Todo:
*/

#ifdef COMPILE_FOR_SYMBIAN

#include "PDO/NS/SymbiannsCore.h"
#include "PDO/String/SymbianstCore.h"	

#include <http/rhttpconnectioninfo.h>
#include <HttpStringConstants.h>

namespace PDO
{
	namespace NS
	{
		Core * Core::createCore()
		{
			return new SymbianCore();
		}

		LBool SymbianCore::RequestResource(const String *pURL, const String *pLocalDest)
		{
			if(!pURL || pURL->IsEmpty() || !pLocalDest || pLocalDest->IsEmpty())
				return LFalse;

			return LFalse;
		}

		LUint SymbianCore::RequestResourceSize(const String *pURL)
		{
			if(!pURL || pURL->IsEmpty())
				return 0;

			return 0;
		}

		LBool SymbianCore::SetProxyServer(const String *pProxyAddress)
		{
			if(!pProxyAddress || pProxyAddress->IsEmpty())
				return LFalse;

			return LFalse;
		}

		void SymbianCore::InitOnce()
		{

		}

		void SymbianCore::ConnectionCreated(const TInt &aError)
		{
			if(aError != KErrNone)
			{
				//TRAPD(error, m_TransactionObserver.FailedL(aError))
				//if(error != KErrNone)
				//	User::Panic(KApplicationName, error);
			}

			//m_Status = kCONNECTED;

			//Set properties for the HTTP session
			TRAPD(error,
				RStringPool strP = m_rHttpSession.StringPool();
				RHTTPConnectionInfo connInfo = m_rHttpSession.ConnectionInfo();
				connInfo.SetPropertyL ( strP.StringF(HTTP::EHttpSocketServ, RHTTPSession::GetTable() ), THTTPHdrVal (m_SockServ.Handle()) );
				TInt connPtr = REINTERPRET_CAST(TInt, &m_rConnection);
				connInfo.SetPropertyL ( strP.StringF(HTTP::EHttpSocketConnection, RHTTPSession::GetTable() ), THTTPHdrVal (connPtr) );
				)
			
		//	if(error != KErrNone)
//				User::Panic(KApplicationName, error);

		}

	}
}

#endif // COMPILE_FOR_SYMBIAN