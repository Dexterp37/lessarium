/**
	License: zlib/libpng
	Author: Alessio Pierluigi Placitelli
	Company:

	Description: This class holds Symbian FSS core functionalities implementation

	Revision:

	Todo:
*/

#ifdef COMPILE_FOR_SYMBIAN

#ifndef _SYMBIAN_NSCORE_H_
#define _SYMBIAN_NSCORE_H_

#include "PDO/NS/nsCore.h"

#include <http/rhttpsession.h>
#include <es_sock.h>
#include <CommDbConnPref.h>

namespace PDO
{
	namespace NS
	{
		class ISymConnMonitor	
		{
		public:
			virtual void ConnectionCreated(const TInt& aError) = 0;		// Pure virtual, implemented in PDO::NS::SymbianCore
		};

		class SymbianCore : public Core, ISymConnMonitor
		{
		public:
			inline SymbianCore() {};
			inline virtual ~SymbianCore() {};

			LBool RequestResource(const String *pURL, const String *pLocalDest);
			LUint RequestResourceSize(const String *pURL);
			LBool SetProxyServer(const String *pProxyAddress);

		protected:
			class CSymConnOpener : public CActive
			{
			public: 
				static CSymConnOpener* NewL(RConnection& aConnection, ISymConnMonitor& aConnObserver);
				~CSymConnOpener();
				
				void OpenConnection(TCommDbConnPref& aPrefs);
				void CloseConnection();

			private:
				// Symbian OS default constructor
				CSymConnOpener(RConnection& aConnection, ISymConnMonitor& aConnObserver);
				void ConstructL();

				// From CActive
				void RunL();
				void DoCancel();

				RConnection& iConnection;				// Connection reference
				ISymConnMonitor& iConnObserver;			// Object to which message notifications are destined
				
			};
/*
			class CSymDownThread : public CActive
			{
			public:
				CSymDownThread() {};
				~CSymDownThread() {};
			};
*/

			// 
			void InitOnce();

			// Callbacks
			void ConnectionCreated(const TInt &aError); 

			RConnection		m_rConnection;
			RHTTPSession	m_rHttpSession;
			RSocketServ		m_rSockServ;
		};
	}
}

#endif // _SYMBIAN_NSCORE_H_

#endif // COMPILE_FOR_SYMBIAN