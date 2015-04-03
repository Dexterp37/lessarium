/**
	License: zlib/libpng
	Author: Alessio Pierluigi Placitelli
	Company:

	Description: This class holds Windows NS core functionalities implementation

	Revision:

	Todo:
		- Implement resume feature
*/

#ifdef COMPILE_FOR_WINDOWS

#include "PDO/NS/Win32nsCore.h"
#include "PDO/String/Win32stCore.h"	
#ifdef _DEBUG
#include <stdio.h>
#endif

namespace PDO
{
	namespace NS
	{
		Core * Core::createCore()
		{
			return new Win32Core();
		}

		Core::SNS_REQUEST_DESC *Win32Core::RequestResource(const String *pURL, const String *pLocalDest)
		{
			if(!pURL || pURL->IsEmpty() || !pLocalDest || pLocalDest->IsEmpty())
				return LFalse;

			if(m_hSession == LNull)
			{
				// That's the first time we try to download something, init wininet
				m_hSession = InternetOpen("LESSA", INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, NULL);
				if(!m_hSession)
				{
					// GetLastError() ?
					return LFalse;
				}	

				InternetSetStatusCallback(m_hSession, (INTERNET_STATUS_CALLBACK)InternetStatus_PassMe);
			}

			PDO::BaseString::Win32Core *pWinUrl = (PDO::BaseString::Win32Core*)pURL;

			// Check if file was already partially downloaded. If yes, resume
			char *pszHeaders = NULL;
			LARGE_INTEGER i64ResumeAt = CheckForResume(pLocalDest);
			if(i64ResumeAt.QuadPart)
			{
				pszHeaders = new char[128];		// Waste of space.. i know.
				if(pszHeaders)
				{
				 sprintf(pszHeaders, "Range: bytes=%I64d-", i64ResumeAt.QuadPart);
				}
			}


			// Should create a new thread here
			HINTERNET hUrl = InternetOpenUrl(m_hSession, pWinUrl->GetRawData(), pszHeaders, (pszHeaders) ? strlen(pszHeaders) : 0, INTERNET_FLAG_RELOAD | INTERNET_FLAG_PRAGMA_NOCACHE | INTERNET_FLAG_NO_CACHE_WRITE, NULL);//(LPARAM)this );
			if(!hUrl)
			{
				if(pszHeaders)
					delete [] pszHeaders;

				// GetLastError
				return LFalse;
			}

			if(pszHeaders)	// Cleanup headers
				delete [] pszHeaders;

			Core::SNS_REQUEST_DESC *pReq = new Core::SNS_REQUEST_DESC;
			if(!pReq)
			{
				InternetCloseHandle(hUrl);
				return LNull;
			}

			DWORD dwThreadId = 0L;
			SNSRESRQ_PARAMS *pThreadParams = new SNSRESRQ_PARAMS;
			if(pThreadParams)
			{
				pThreadParams->m_pCaller = this;
				pThreadParams->m_hResourceUrl = hUrl;
				pThreadParams->m_pSaveTo = (String*)pLocalDest;
				pThreadParams->m_bStop = LFalse;

				CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)ResourceGrabThread_PassMe, pThreadParams, 0, &dwThreadId);

				pReq->m_pRequestID = dwThreadId;	// Request id is thread id, in windows
				pReq->m_lpUserData = pThreadParams; // Thread params are user data here..

				if(m_reqList.AddHead(pReq))
					return pReq;

				// Else cleanup...
				pThreadParams->m_pCaller = LNull;
				pThreadParams->m_hResourceUrl = LNull;
				pThreadParams->m_pSaveTo = LNull;	// Make Win32nsCore own a string to local path?
			}

			InternetCloseHandle(hUrl);
			delete pReq;

			return LNull;
		}

		LUint Win32Core::RequestResourceSize(const SNS_REQUEST_DESC *pReqDesc)
		{
			if(!pReqDesc)
				return 0;

			SNSRESRQ_PARAMS *pParams = (SNSRESRQ_PARAMS*)(pReqDesc->m_lpUserData);

			// TODO: Use a Mutex to access ?
			char szBuffer[32];
			DWORD dwBufferSize = sizeof(szBuffer); 

			BOOL bQuery = HttpQueryInfo(pParams->m_hResourceUrl, HTTP_QUERY_CONTENT_LENGTH, szBuffer, &dwBufferSize, NULL);
			if(!bQuery)
			{
				return 0;
			}

			return atol(szBuffer);
		}

		LBool Win32Core::RequestResourceLastMod(const SNS_REQUEST_DESC *pReqDesc, LDate &rDateOut)
		{
			if(!pReqDesc)
				return 0;

			SNSRESRQ_PARAMS *pParams = (SNSRESRQ_PARAMS*)(pReqDesc->m_lpUserData);

			// TODO: Use a Mutex to access ?
			SYSTEMTIME remoteFileTime;
			DWORD dwBufferSize = sizeof(SYSTEMTIME); 

			BOOL bQuery = HttpQueryInfo(pParams->m_hResourceUrl, HTTP_QUERY_LAST_MODIFIED, &remoteFileTime, &dwBufferSize, NULL);
			if(!bQuery)
			{
				return 0;
			}

			rDateOut.m_uYear = remoteFileTime.wYear;
			rDateOut.m_uMonth = remoteFileTime.wMonth;
			rDateOut.m_uDay = remoteFileTime.wDay;
			rDateOut.m_uMonth = remoteFileTime.wHour;
			rDateOut.m_uMinute = remoteFileTime.wMinute;
			rDateOut.m_uSecond = remoteFileTime.wSecond;

			return LFalse;
		}

		void  Win32Core::PauseAll()
		{
			PIO::Utils::CSingleLinkedList<PDO::NS::Core::SNS_REQUEST_DESC>::_LLIST_NODE *pNode = m_reqList.GetHead();
			while(pNode)
			{
				Pause(pNode->m_pElement);

				pNode = pNode->m_pNext;
			}
		}

		void Win32Core::Pause(const SNS_REQUEST_DESC *pRequest)
		{
			SNSRESRQ_PARAMS *pParams = (SNSRESRQ_PARAMS *)(pRequest->m_lpUserData);
			if(pParams)
				pParams->m_bStop = LTrue;	// There is no need to use i Mutex, i think
		}

		LBool Win32Core::SetProxyServer(const String *pProxyAddress)
		{
			return LFalse;
		}

		void Win32Core::InternetStatus_PassMe(HINTERNET hInternet, DWORD_PTR dwContext, DWORD dwInternetStatus, LPVOID lpvStatusInformation, DWORD dwStatusInformationLength)
		{
			if(dwContext)
			{
				((Win32Core*)dwContext)->InternetStatusCallback(hInternet, dwContext, dwInternetStatus, lpvStatusInformation, dwStatusInformationLength);
			}
		}

		void Win32Core::InternetStatusCallback(HINTERNET hInternet, DWORD_PTR dwContext, DWORD dwInternetStatus, LPVOID lpvStatusInformation, DWORD dwStatusInformationLength)
		{
			return;
		}

		DWORD Win32Core::ResourceGrabThread_PassMe(LPVOID pParam)
		{
			SNSRESRQ_PARAMS *pNsParams = (SNSRESRQ_PARAMS*)pParam;

			if(pNsParams && pNsParams->m_pCaller)
			{
				return ((Win32Core*)pNsParams->m_pCaller)->ResourceGrabThread(pNsParams);
			}

            return 0;
		}

		// That's the downloader thread
		DWORD Win32Core::ResourceGrabThread(SNSRESRQ_PARAMS *pParam)
		{
			char szBuffer[512];
			DWORD dwBufferSize = sizeof(szBuffer); 

			// Find out the length of our file
			BOOL bQuery = HttpQueryInfo(pParam->m_hResourceUrl, HTTP_QUERY_CONTENT_LENGTH, szBuffer, &dwBufferSize, NULL);
			if(!bQuery)
			{
				// Cleanup
				InternetCloseHandle(pParam->m_hResourceUrl);

				pParam->m_hResourceUrl = NULL;
				pParam->m_pCaller = NULL;
				pParam->m_pSaveTo = NULL;
				//pParam->m_bStop   = LFalse;

				delete pParam;

				// Signal error ?

				return 0;
			}

			// Convert text length to DWORD
			DWORD dwFileSize = atol(szBuffer);
			DWORD dwBytesRead = 0L;
			dwBufferSize = sizeof(szBuffer);
			BOOL bRet = false;

			PDO::BaseString::Win32Core *pWinUrl = (PDO::BaseString::Win32Core*)pParam->m_pSaveTo;

			FILE * p = fopen(pWinUrl->GetRawData(), "wb");
			// Read file chunk by chunk into our buffer and save to disk
			do
			{
				bRet = InternetReadFile(pParam->m_hResourceUrl, &szBuffer, dwBufferSize, &dwBytesRead);
				if(bRet)
				{
					fwrite(szBuffer, dwBytesRead, 1, p);

#ifdef _DEBUG
					fflush(p);
#endif
				}

			}while(bRet && dwBytesRead && !(pParam->m_bStop));

			fclose(p);

			// Cleanup
			InternetCloseHandle(pParam->m_hResourceUrl);

			pParam->m_hResourceUrl = NULL;
			pParam->m_pCaller = NULL;
			pParam->m_pSaveTo = NULL;

			delete pParam;


			return 1;
		}

		LARGE_INTEGER Win32Core::CheckForResume(const String *pLocalFile)
		{
			LARGE_INTEGER i64FileSize;
			PDO::BaseString::Win32Core *pWinUrl = (PDO::BaseString::Win32Core*)pLocalFile;

			HANDLE hFile = CreateFile(pWinUrl->GetRawData(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
			if(hFile == INVALID_HANDLE_VALUE) 
			{
				i64FileSize.QuadPart = 0;
				return i64FileSize;
			}

			if(!GetFileSizeEx(hFile, &i64FileSize))
				i64FileSize.QuadPart = 0;

			CloseHandle(hFile);

			return i64FileSize;
		}
	}
}

#endif // COMPILE_FOR_WINDOWS