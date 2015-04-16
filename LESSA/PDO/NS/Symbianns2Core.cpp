/**
	License: zlib/libpng
	Author: Alessio Pierluigi Placitelli
	Company:

	Description: This class holds Symbian NS core functionalities implementation

	Revision:

	Todo:
		- for some strange reason, first time you try to download, it goes timeout! investigate!
*/

#ifdef COMPILE_FOR_SYMBIAN

#include "PDO/NS/Symbianns2Core.h"
#include "PDO/String/SymbianstCore.h"	
#include <commdbconnpref.h>	
#include <commdb.h>
#include <cdblen.h>
#include <utf.h>

#include "LESSAApp.h"
//extern const TUid KUidLESSA;

namespace PDO
{
	namespace NS
	{
		Core * Core::createCore()
		{
			return new SymbianCore();
		}

		Core::SNS_REQUEST_DESC *SymbianCore::RequestResource(const String *pURL, const String *pLocalDest)
		{
			if(!pURL || pURL->IsEmpty() || !pLocalDest || pLocalDest->IsEmpty())
				return LNull;

			if(!m_bIsOnline)
			{
				GetFavouriteIap();

				m_rDownloadMgr.ConnectL(KUidLESSA, *this, ETrue );
				m_rDownloadMgr.SetIntAttribute(EDlMgrIap, m_uIapID );		// TODO : Check for errors
				m_rDownloadMgr.SetIntAttribute(EDlMgrExitAction, EExitPause);
				m_rDownloadMgr.DeleteAll();

				m_bIsOnline = LTrue;
			}

			// Convert string to TDesC8
			TBool bResult = LFalse;

			PDO::BaseString::SymbianCore *pSymURL = (PDO::BaseString::SymbianCore *)pURL;
			HBufC8* buf8Bit = HBufC8::NewLC( pURL->Length() );
			buf8Bit->Des().Copy( *(pSymURL->GetRawData()) );

			// Add download to download manager..
			RHttpDownload& download = m_rDownloadMgr.CreateDownloadL( *buf8Bit , bResult );
			CleanupStack::PopAndDestroy( buf8Bit );		// Free memory ASAP

			PDO::BaseString::SymbianCore *pSymDest = (PDO::BaseString::SymbianCore *)pLocalDest;
#ifdef __WINS__
			pSymDest->Replace("Z:", "C:");
#endif
			download.SetStringAttribute(EDlAttrDestFilename, *(pSymDest->GetRawData()));
			download.SetBoolAttribute(EDlAttrContinue, ETrue);		// Let download manager handle download
			download.SetBoolAttribute(EDlAttrHidden, ETrue);		// Hide download to the user
			//download.SetIntAttribute(EDlAttrAction, EDoNothing);


			SNS_REQUEST_DESC *pReq = new SNS_REQUEST_DESC;
			if(!pReq)
			{
				download.Delete();
				return LNull;
			}

			//pReq->
			// TODO! WILL CRASH!

			download.Start();


			return pReq;
		}

		LUint SymbianCore::RequestResourceSize(const SNS_REQUEST_DESC *pRequest)
		{
			if(!pRequest)
				return 0;

			return 0;
		}

		LBool SymbianCore::RequestResourceLastMod(const SNS_REQUEST_DESC *pRequest, LDate &rDateOut)
		{
			return LTrue;
		}

		void  SymbianCore::PauseAll()
		{

		}

		void  SymbianCore::Pause(const SNS_REQUEST_DESC *pRequest)
		{

		}

		LBool SymbianCore::SetProxyServer(const String *pProxyAddress)
		{
			if(!pProxyAddress || pProxyAddress->IsEmpty())
				return LFalse;

			return LFalse;
		}

		void SymbianCore::HandleDMgrEventL(RHttpDownload &aDownload, THttpDownloadEvent aEvent)
		{
			// Start download again if content-type is acceptable
			// and UiLib is not installed
			if( EHttpContentTypeReceived == aEvent.iProgressState )
			{
				User::LeaveIfError( aDownload.Start() );
			}

			switch ( aEvent.iDownloadState )
			{
			case EHttpDlCreated:
				{
					// Add new download to the list
//					NewDownloadCreatedL( aDownload );
					return;
					break;
				}
			case EHttpDlInprogress:
				{
					// Update progress info
//					UpdateProgressInfoL( aDownload, aEvent );
					TBool bVal = EFalse;
					//aDownload.GetBoolAttribute(EDlAttrContinue, bVal);
					return;
					break;
				}
			case EHttpDlPaused:
				{
//					DownloadPausedL( aDownload, aEvent );
					return;
					break;
				}
			case EHttpDlCompleted:
				{
//					DownloadCompletedL( aDownload, aEvent );
					TInt iErr = aDownload.Move();
					if(iErr != KErrNone)
					{
						/*if(iErr == KErrNotFound)
						{

						}*/

						// That happens when file has been downloaded
						// but then manually moved or deleted. Simply
						// delete download from download manager.
						iErr = aDownload.Delete();
						return;
					}

					break;
				}
			case EHttpDlFailed:
				{
//					DownloadFailedL( aDownload, aEvent );
					TInt32 iVal = 0L;	// Returns 6 (EDlAttrGlobalErrorId), or 4 (EDiskFull)
					TInt32 iRetVal = aDownload.GetIntAttribute(EDlAttrErrorId, iVal);

					TInt32 iGenVal = 0L;	// Returns -33 (KErrTimedOut), or -26 ()
					aDownload.GetIntAttribute(EDlAttrGlobalErrorId, iGenVal);

					//aDownload.Start();
					return;

					break;
				}
			case EHttpDlMoved:
				{
					// Download moved to this session.
					// Add it to the list.
					return;
					break;
				}
			case EHttpDlMediaRemoved:
				{
					// Remove the download from the list.
					return;
					break;
				}
			case EHttpDlMediaInserted:
				{
					// Add new download to the list.
					return;
					break;
				}
			case EHttpDlPausable:
			case EHttpDlNonPausable:
				{
					// Update the download UI data.
					return;
					break;
				}
			case EHttpDlDeleted:
				{
					// Download is deleted from another client instance.
					return;
					break;
				}
			default:
				break;
			}
		}

		void SymbianCore::GetFavouriteIap()
		{
			CCommsDatabase* cdb = CCommsDatabase::NewL(EDatabaseTypeIAP);
			CleanupStack::PushL(cdb);

			CCommsDbConnectionPrefTableView* commDBView = cdb->OpenConnectionPrefTableInRankOrderLC(ECommDbConnectionDirectionOutgoing);
			if(commDBView->GotoFirstRecord() == KErrNone)
			{
				CCommsDbConnectionPrefTableView::TCommDbIapConnectionPref pref;
				commDBView->ReadConnectionPreferenceL(pref);
				m_uIapID = pref.iBearer.iIapId;
			}

			CleanupStack::PopAndDestroy(commDBView);
			CleanupStack::PopAndDestroy(cdb);

		}
	}
}

#endif // COMPILE_FOR_SYMBIAN