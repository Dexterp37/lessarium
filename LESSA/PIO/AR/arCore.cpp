/** 
 *	\file arCore.cpp
 *	License: zlib/libpng
 *
 *	This class holds AR core implementation
 *
 *	\author Alessio Pierluigi Placitelli
 *	\version
 *	\todo
 *		- Fix the memory leak which occours when uncommenting line 125
 */

#include "PIO/AR/arCore.h"
#include "PDO/FSS/fsCore.h"			// PDO::FSS

namespace PIO
{
	namespace AR
	{
		LBool Core::SkimRead()
		{
			PIO::XML::Parser::Core *pXML = new PIO::XML::Parser::Core();
			if(pXML)
			{
				PIO::XML::Parser::SXMLHANDLERS xmlHandlers = { 
						(PIO::XML::Parser::START_ELEMENT_HANDLER)&XML_StartElementCB,
						(PIO::XML::Parser::ATTRIBUTE_NAME_HANDLER)&XML_AttributeNameCB,
						(PIO::XML::Parser::ATTRIBUTE_VALUE_HANDLER)&XML_AttributeValueCB,
						NULL,
						NULL,
						NULL
				};

				pXML->SetHandlers(&xmlHandlers);
				pXML->SetCBUserData(this);

				// Get application path
				String *pRealPath = PDO::FSS::Core::GetLESSAPath();
 				*pRealPath += "contents.xml";

				LInt iRes = pXML->ParseFile(pRealPath);

				delete pRealPath;
				delete pXML;

				return iRes;
			}

			return LFalse;
		}

		LBool Core::GetFirstEntry(const String *pName)
		{
			PIO::XML::Parser::Core *pXML = new PIO::XML::Parser::Core();
			if(pXML)
			{
				PIO::XML::Parser::SXMLHANDLERS xmlHandlers = { 
					(PIO::XML::Parser::START_ELEMENT_HANDLER)&XML_StartElementCB,
						NULL,
						NULL,
						NULL,
						NULL,
						NULL
				};

				pXML->SetHandlers(&xmlHandlers);

				// Get application path
				String *pRealPath = PDO::FSS::Core::GetLESSAPath();
 				*pRealPath += "soap_rq.xml";

				LInt iRes = pXML->ParseFile(pRealPath);

				delete pRealPath;
				delete pXML;
			}

			return LFalse;
		}

		LInt Core::StartElementCB(const String *pTagName, const PIO::XML::Parser::STOXSTATE *pParserState)
		{
			if(*pTagName == "content" && m_eState != AR_CONTENT_START)	// || pTagName == "CONTENT"
			{
				CONTENT_ENTRY *pEntry = new CONTENT_ENTRY();
				if(!pEntry)
					return 0;

				pEntry->m_uLineNumber = pParserState->m_iLineNumber;

				if(!m_contentList.AddTail(pEntry))
					return 0;

				m_eState = AR_CONTENT_START;
			}else if(*pTagName == "/content")
			{
				m_eState = AR_CONTENT_END;
			}

			return 1;
		}

		LInt Core::AttributeNameCB(const String *pAttrName, const PIO::XML::Parser::STOXSTATE *pParserState, LVoid *pUserData)
		{
			if(m_eState == AR_CONTENT_START && *pAttrName == "name")	// == "NAME"?
			{
				m_eState = AR_CONTENT_NAME;
				
				return 1;
			}

			m_eState = AR_CONTENT_UNKNOWN;

			return 1;
		}

		LInt Core::AttributeValueCB(const String *pAttrValue, const PIO::XML::Parser::STOXSTATE *pParserState, LVoid *pUserData)
		{
			if(m_eState == AR_CONTENT_NAME)
			{
				PIO::Utils::CSingleLinkedList<CONTENT_ENTRY>::_LLIST_NODE *pLast = m_contentList.GetTail();
				if(pLast)
				{
					pLast->m_pElement->m_pName = PDO::BaseString::Core::createCore();
					*(pLast->m_pElement->m_pName) = *pAttrValue;
				}

			}else if(m_eState == AR_CONTENT_UNKNOWN)
			{
				m_eState = AR_CONTENT_START;
			}

			return 1;
		}


		LInt Core::XML_StartElementCB(const String *pTagName, const PIO::XML::Parser::STOXSTATE *pParserState, LVoid *pUserData)
		{
			return ((Core*)pUserData)->StartElementCB(pTagName, pParserState);
		}

		LInt Core::XML_AttributeNameCB(const String *pTagName, const PIO::XML::Parser::STOXSTATE *pParserState, LVoid *pUserData)
		{
			return ((Core*)pUserData)->AttributeNameCB(pTagName, pParserState, pUserData);
		}

		LInt Core::XML_AttributeValueCB(const String *pTagName, const PIO::XML::Parser::STOXSTATE *pParserState, LVoid *pUserData)
		{
			return ((Core*)pUserData)->AttributeValueCB(pTagName, pParserState, pUserData);
		}

	}
}