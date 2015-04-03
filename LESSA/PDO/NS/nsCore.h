/** 
 *	\file nsCore.h 
 *	\class PDO::NS::Core nsCore.h "PDO/NS/nsCore.h"
 *	\struct PDO::NS::Core::SNS_REQUEST_DESC nsCore.h "PDO/NS/nsCore.h"
 *	License: zlib/libpng
 *
 *	This class holds NS core functionalities, enabling
 *	application to download multiple file streams
 *
 *	\author Alessio Pierluigi Placitelli
 *	\version
 *	\todo
 *		- Handle callbacks and errors in system specific
 *		- Check Win32Core in RequestResource, since it crashes 
 *		  on like InternetOpenUrl when last parameter is non Null
 *		- Check Win32Core: has many DWORD to unsigned int..
 *		- Create something like LDate RequestResourceTimestamp(const String *pURL);
 *		- Complete Symbianns2Core RequestResourceLength
 *		- Add a download queue which elements will hold remote files infor
 *		- Win32 Resume functionality in the new thread (check for file existence, 
 *		  read size, and just download remaining part)
 */

#ifndef _NSCORE_H_
#define _NSCORE_H_

#include "config.h"
#include "PDO/String/stCore.h"
#include "PIO/Utils/llist.h"

namespace PDO
{
	namespace NS
	{
		class Core
		{
		protected:
			Core();

		public:
			typedef struct
			{
				LUlong	m_pRequestID;	///< Resource request ID
				LVoid	*m_lpUserData;	///< Custom user data
			}SNS_REQUEST_DESC;


			inline virtual ~Core() {};
			
			static Core *createCore();

			/**
				Requests NS to download the resource pointed by pURL. NS
				can handle multiple requests on every platform, thus
				making it easy for developer to download multiple files.

				\param[in] pURL A String parameter containing resource URL
				\param[in] pLocalDest A String parameter describing where to save the file
				\return Returns LTrue if request has been accepted, LFalse otherwise
				\note The function returns right after request has been made. That 
				doesn't mean that file was downloaded correctly.
			*/
			virtual SNS_REQUEST_DESC *RequestResource(const String *pURL, const String *pLocalDest){ return LNull; };

			/**
				Requests NS to find out remote resource size

				\param[in] pReqDesc A SNS_REQUEST_DESC parameter containing requested resource descriptor
				\return Returns the size in bytes of requested resource, 0 if no size is available or it failed
				\see SNS_REQUEST_DESC RequestResource
			*/
			virtual LUint RequestResourceSize(const SNS_REQUEST_DESC *pReqDesc){ return 0; };

			/**
				Requests NS to find out remote resource last modified date

				\param[in] pReqDesc A SNS_REQUEST_DESC parameter containing requested resource descriptor
				\param[out] rDateOut A LDate reference which will hold resource last mod. date
				\return Returns LTrue if last modification date was found, LFalse otherwise
				\see SNS_REQUEST_DESC RequestResource LNULL_DATE LDate
			*/
			virtual LBool RequestResourceLastMod(const SNS_REQUEST_DESC *pReqDesc, LDate &rDateOut){ return LFalse; };

			/**
				Stop any pending resource request

				\note Does not disconnect
			*/
			virtual void PauseAll(){ };

			/**
				Stop any pending resource request

				\note Does not disconnect
				\see SNS_REQUEST_DESC RequestResource
			*/
			virtual void Pause(const SNS_REQUEST_DESC *pRequest){ };

			/**
				Makes NS use a proxy server. 

				\param[in] pProxyAddress A String parameter containing proxy address
				\return Returns LTrue if proxy was set, LFalse otherwise
				\note If proxy is not specified, a direct internet connection is assumed
			*/
			virtual LBool SetProxyServer(const String *pProxyAddress){ return LFalse; };

		protected:

			PIO::Utils::CSingleLinkedList<SNS_REQUEST_DESC>		m_reqList;		///< Single linked list holding pending resource request descriptors
		};
	}
}

#endif // _NSCORE_H_