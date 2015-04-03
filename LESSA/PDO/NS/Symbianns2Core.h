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

#include <downloadmgrclient.h>

namespace PDO
{
	namespace NS
	{
		class SymbianCore : public Core, public MHttpDownloadMgrObserver
		{
		public:
			inline SymbianCore() : m_bIsOnline(LFalse), m_uIapID(0) {};
			inline virtual ~SymbianCore() 
			{
				if(m_bIsOnline)
				{
					//m_rDownloadMgr.PauseAll();
					m_rDownloadMgr.Disconnect(); // Disconnect HTTP and pause download
					m_rDownloadMgr.Close();
				}
			};

			SNS_REQUEST_DESC *RequestResource(const String *pURL, const String *pLocalDest);
			LUint RequestResourceSize(const SNS_REQUEST_DESC *pRequest);
			LBool RequestResourceLastMod(const SNS_REQUEST_DESC *pRequest, LDate &rDateOut);
			void  PauseAll();
			void  Pause(const SNS_REQUEST_DESC *pRequest);
			LBool SetProxyServer(const String *pProxyAddress);

			void  HandleDMgrEventL(RHttpDownload &aDownload, THttpDownloadEvent aEvent);

		private:
			TUint32				m_uIapID;
			LBool				m_bIsOnline;
			RHttpDownloadMgr	m_rDownloadMgr;


			void	GetFavouriteIap();	// Get default connection ID
		};
	}
}

#endif // _SYMBIAN_NSCORE_H_

#endif // COMPILE_FOR_SYMBIAN