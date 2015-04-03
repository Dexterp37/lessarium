/** 
 *	\file arCore.h 
 *	\class PIO::AR::Core arCore.h "PIO/AR/arCore.h"
 *	\enum PIO::AR::EARSTATE arCore.h "PIO/AR/arCore.h"
 *	\class PIO::AR::CONTENT_ENTRY arCore.h "PIO/AR/arCore.h"
 *	License: zlib/libpng
 *
 *	This class holds AR core functionalities, which is the
 *	class designed to keep track of installed application
 *
 *	\author Alessio Pierluigi Placitelli
 *	\version
 *	\todo
 *		- Load more information from XML
 *		- When modifying the XML file, invalidate the skimmed line numbers
 *		
 */
#ifndef _ARCORE_H_
#define _ARCORE_H_

#include "PIO/XML/xmCore.h"
#include "PIO/Utils/llist.h"

namespace PIO
{
	namespace AR
	{
		typedef enum {
			AR_ERROR = -5,
			AR_RESET = 0,
			AR_CONTENT_START,
			AR_CONTENT_END,
			AR_CONTENT_NAME,
			AR_CONTENT_UNKNOWN
		}EARSTATE;

		class CONTENT_ENTRY {
		public:
			CONTENT_ENTRY() : m_pName(LNull), m_uLineNumber(0) {};
			~CONTENT_ENTRY() 
			{ 
				delete m_pName; 
				m_pName = LNull; 
			};

			String	*m_pName;			///< Content short name
			LUint	m_uLineNumber;		///< Line in the xml file (cached to easily load info from file)
		};

		class Core
		{
		public:
			Core() : m_xmlParser(LNull), m_eState(AR_RESET) {};
			inline virtual ~Core() { delete m_xmlParser; m_xmlParser = LNull; };

			/**
				Superficially read the XML application registry, extracting few information

				\note This function saves information content name and line number to m_contentList
				\see m_contentList
			*/	
			LBool	SkimRead(/*const String *pFileName*/);	

			LBool	GetFirstEntry(const String *pName);

			/**
				Called by the parser. When element name is "content", append a new element to list tail

				\see m_contentList PIO::XML::Parser::Core
			*/	
			LInt StartElementCB(const String *pTagName, const PIO::XML::Parser::STOXSTATE *pParserState);
			LInt AttributeNameCB(const String *pAttrName, const PIO::XML::Parser::STOXSTATE *pParserState, LVoid *pUserData);
			LInt AttributeValueCB(const String *pAttrValue, const PIO::XML::Parser::STOXSTATE *pParserState, LVoid *pUserData);

			static LInt XML_StartElementCB(const String *pTagName, const PIO::XML::Parser::STOXSTATE *pParserState, LVoid *pUserData);
			static LInt XML_AttributeNameCB(const String *pTagName, const PIO::XML::Parser::STOXSTATE *pParserState, LVoid *pUserData);
			static LInt XML_AttributeValueCB(const String *pTagName, const PIO::XML::Parser::STOXSTATE *pParserState, LVoid *pUserData);

		private:
			PIO::Utils::CSingleLinkedList<CONTENT_ENTRY>	m_contentList;	///< Single linked list filled with content 
			PIO::XML::Parser::Core	*m_xmlParser;							///< An instance of the XML parser
			EARSTATE				m_eState;								///< AR FSM state (needed since XML parser is event based)
		};
	}
}

#endif // _ARCORE_H_