/**
	License: zlib/libpng
	Author: Alessio Pierluigi Placitelli
	Company:

	Description: This class holds Win32 NS core functionalities implementation

	Revision:

	Todo:
*/

#ifdef COMPILE_FOR_WINDOWS

#ifndef _WIN32_NSCORE_H_
#define _WIN32_NSCORE_H_

#include "PDO/NS/nsCore.h"

#include <windows.h>
#include <wininet.h>

#define WIN32NS_REMOTESIZE_AVAILABLE	0x1

namespace PDO
{
	namespace NS
	{
		class Win32Core : public Core
		{
		public:
			inline Win32Core() : m_hSession(LNull) {};
			inline virtual ~Win32Core() 
			{ 
				if(m_hSession) 
				{
					InternetSetStatusCallback(m_hSession, LNull); 
					InternetCloseHandle(m_hSession); 
				}
				
				m_hSession = LNull; 
			};

			SNS_REQUEST_DESC * RequestResource(const String *pURL, const String *pLocalDest);
			LUint RequestResourceSize(const SNS_REQUEST_DESC *pReqDesc);
			LBool RequestResourceLastMod(const SNS_REQUEST_DESC *pReqDesc, LDate &rDateOut);
			void  PauseAll();
			void  Pause(const SNS_REQUEST_DESC *pRequest);
			LBool SetProxyServer(const String *pProxyAddress);

		private:
			typedef struct {
				LPVOID		m_pCaller;
				HINTERNET	m_hResourceUrl;
				String		*m_pSaveTo;
				LBool		m_bStop;
			}SNSRESRQ_PARAMS;

			HINTERNET m_hSession;	///< Internet Session Handle
			
			// Callback function and static adaptor to pass it as parameter
			static void InternetStatus_PassMe(HINTERNET hInternet, DWORD_PTR dwContext, DWORD dwInternetStatus, LPVOID lpvStatusInformation, DWORD dwStatusInformationLength); 
			void InternetStatusCallback(HINTERNET hInternet, DWORD_PTR dwContext, DWORD dwInternetStatus, LPVOID lpvStatusInformation, DWORD dwStatusInformationLength); 
		
			// Thread Routine
			static DWORD	ResourceGrabThread_PassMe(LPVOID pParam);
			DWORD			ResourceGrabThread(SNSRESRQ_PARAMS *pParam);
			LARGE_INTEGER	CheckForResume(const String *pLocalFile);
		};
	}
}

#endif // _WIN32_NSCORE_H_

#endif // COMPILE_FOR_WINDOWS