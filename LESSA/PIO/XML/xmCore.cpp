/*-------------------------------------------------------------------------
 * tox - an XML tokenizer
 *
 * Copyright (c) 2000 Eckhart Köppen
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	 See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 *-----------------------------------------------------------------------*/
/** 
 *	\file xmCore.cpp
 *	License: LGPL
 *
 *	This class holds XML core constructor implementation, based
 *  on Eckhart Köppen's Tox XML Tokenizer (http://www.40hz.org/tox/)
 *
 *	\author Eckhart Köppen adapted by Alessio Pierluigi Placitelli
 *	\version
 *	\todo
 *		- Make ParseLine return LFalse depending on DoAction value
 *		- Allow DoAction to return values depending on action and result
 *		  (it seek an application and then block the parsing), hooking
 *		  up data.
 *		- rip m_pUserData?
 */
//$log$ 30/04/2006 Initial draft
//$log$ 13/05/2006 First working Tox port
#include "PIO/XML/xmCore.h"

namespace PIO
{
	namespace XML
	{
		namespace Parser
		{
			LBool Core::ParseFile(const String *path)
			{
				if(m_pFile)
					delete m_pFile;		// Just if file was already opened..

				m_pFile = PDO::FSS::File::Core::createCore();
				if(!m_pFile->Open(path, LFILE_READ | LFILE_TEXT))	// Just open file if it exists
					return LFalse;

				LBool bStop = LFalse;
				PDO::BaseString::Core *pLine = LNull;

				while(1)
				{
					pLine = m_pFile->Read();
					if(!pLine)
					{
						break;
					}

					LInt iRetVal = ParseLine(pLine);

					delete pLine;

					if(iRetVal == LFalse)
						return LFalse;
				};

				return LTrue;
			}

			/// Not absolutely working. Experimental
			LBool Core::ContinueFileParsing()
			{
				if(!m_pFile)
					return LFalse;

				PDO::BaseString::Core *pLine = LNull;

				while(1)
				{
					pLine = m_pFile->Read();
					if(!pLine)
					{
						break;
					}

					LInt iRetVal = ParseLine(pLine);

					delete pLine;

					if(iRetVal == LFalse)
						return LFalse;	// Error or Eof?
				};

				return LTrue;
			}

			LInt Core::ParseLine(const String *line)
			{
//#ifdef _DEBUG		// Dexter: wipe out checks in Release?
				if(!line || line->IsEmpty())
					return LFalse;
//#endif
				++m_iLineNumber;

				EXMLPARSERSTATUS eOldState;
				LInt iLen = line->Length();
				m_iParserPos = 0;
				LInt iTransition = 0, i;

				while((m_eState != STATE_ERROR) 
					  && (m_eState != STATE_END) 
					  && (m_iParserPos < iLen))
				{
					LChar8Bit c = (*line)[m_iParserPos];	// Pick the character

					eOldState = m_eState;		// Save old state
					iTransition = 0;			// Reset iTransition

					for (i = 0; !iTransition && m_eState != STATE_ERROR; i++) 
					{
						switch (state_table[m_eState][i].accept_type) 
						{
						case TYPE_EXACT:
#ifndef HTML                        
						case TYPE_CHAR:
#endif                        
							if(state_table[m_eState][i].accept_value == c) 
							{
								iTransition = 1;
								DoAction(i);
								if (m_eState == eOldState) 
								{
									m_eState = state_table[m_eState][i].next_state;
								}
							}
							break;
#ifdef HTML                        
						case TYPE_CHAR:		// Dexter's note: 'a' & 0xDF = 'A'
							if(state_table[m_eState][i].accept_value == (c & 0xDF)) 
							{
								iTransition = 1;
								DoAction(i);
								if (m_eState == eOldState) 
								{
									m_eState = state_table[m_eState][i].next_state;
								}
							}
							break;
#endif                                        
						case TYPE_RANGE:
							if (IsInRange((LInt)state_table[m_eState][i].accept_value, c)) 
							{
								iTransition = 1;
								DoAction(i);
								if (m_eState == eOldState) 
								{
									m_eState = state_table[m_eState][i].next_state;
								}
							}
							break;
						case TYPE_SPECIAL:
							switch ((LInt)state_table[m_eState][i].accept_value) 
							{
								case SPECIAL_SKIP_DTD:
//									skip_dtd (parser);
									iTransition = 1;
									if(m_iAngleBracketLevel == -1) 
									{
										m_eState = state_table[m_eState][i].next_state;
										m_isQuoteLevel = 0;
										m_idQuoteLevel = 0;
										m_iAngleBracketLevel = 0;
									}
									break;
								case SPECIAL_FIND_STRING:
//									find_string (parser);
									iTransition = 1;
//									if (parser->find_string == 0) 
									{
										m_eState = state_table[m_eState][i].next_state;
									}
									break;

							}
							break;
						case TYPE_END:
							m_eState = STATE_ERROR;
							break;
						}
					}
					// Advance
					m_iParserPos = m_iParserPos + 1;

					// Copy to inner buffer, if needed
					if(m_bAcquiring)
					{
						if(!m_sBuffer)
							m_sBuffer = PDO::BaseString::Core::createCore();

						*m_sBuffer += c;
					}	
				}

				return LTrue;
			}

			void Core::SetHandlers(const SXMLHANDLERS *pHandlers)
			{	
				// Unusued fields should be set to LNull when initializing SSAXDRIVER
				if(pHandlers->pStartElementHandler != LNull)
					StartElementHandler = pHandlers->pStartElementHandler;

				if(pHandlers->pAttrNameHandler != LNull)
					AttributeNameHandler = pHandlers->pAttrNameHandler;

				if(pHandlers->pAttrValHandler != LNull)
					AttributeValueHandler = pHandlers->pAttrValHandler;

				if(pHandlers->pWordEndHandler != LNull)
					WordEndHandler = pHandlers->pWordEndHandler;

				if(pHandlers->pWSHandler != LNull)
					WhiteSpaceHandler = pHandlers->pWSHandler;

				if(pHandlers->pEntityHandler != LNull)
					EntityHandler = pHandlers->pEntityHandler;
			}

			void Core::GetParserState(STOXSTATE * const pState)
			{
				if(!pState)
					return;

				pState->m_bAcquiring = m_bAcquiring;
				pState->m_eState = m_eState;
				pState->m_iAngleBracketLevel = m_iAngleBracketLevel;
				pState->m_idQuoteLevel = m_idQuoteLevel;
				pState->m_iParserPos = m_iParserPos;
				pState->m_isQuoteLevel = m_isQuoteLevel;
				pState->m_iLineNumber = m_iLineNumber;
			}

			void Core::SetCBUserData(LVoid	*pUserData)
			{
				m_pUserData = pUserData;
			}

			void Core::DoAction(LInt iVariant)
			{
				//@todo: use a swith instead of the "action & "? action isn't a bitfield, right?
				unsigned int action = state_table[m_eState][iVariant].action;

				if (action & ACTION_TAGNAME_END)
				{
					m_bAcquiring = LFalse;

					if(StartElementHandler) 
					{
						STOXSTATE ParserState;
						GetParserState(&ParserState);
						(*StartElementHandler)(m_sBuffer, &ParserState, m_pUserData);	
					}

					// We don't need m_sBuffer anymore.. free!
					delete m_sBuffer;
					m_sBuffer = LNull;

					return;	// Return?
				}

				if (action & ACTION_ATTRNAME_END) 
				{
					m_bAcquiring = LFalse;

					if(AttributeNameHandler)
					{
						STOXSTATE ParserState;
						GetParserState(&ParserState);
						(*AttributeNameHandler)(m_sBuffer, &ParserState, m_pUserData);
					}

					delete m_sBuffer;
					m_sBuffer = LNull;

					return;
				}

				if (action & ACTION_ATTRVAL_END) 
				{
					m_bAcquiring = LFalse;

					if(AttributeValueHandler)
					{
						STOXSTATE ParserState;
						GetParserState(&ParserState);
						(*AttributeValueHandler)(m_sBuffer, &ParserState, m_pUserData);
					}

					delete m_sBuffer;
					m_sBuffer = LNull;

					return;
				}

				if (action & ACTION_WORD_END) 			
				{
					m_bAcquiring = LFalse;

					if(WordEndHandler)
					{
						STOXSTATE ParserState;
						GetParserState(&ParserState);
						(*WordEndHandler)(m_sBuffer, &ParserState, m_pUserData);
					}

					delete m_sBuffer;
					m_sBuffer = LNull;

					return;
				}

				if (action & ACTION_WS_END) 		
				{
					m_bAcquiring = LFalse;

					if(WhiteSpaceHandler)
					{
						STOXSTATE ParserState;
						GetParserState(&ParserState);
						(*WhiteSpaceHandler)(m_sBuffer, &ParserState, m_pUserData);
					}

					delete m_sBuffer;
					m_sBuffer = LNull;

					return;
				}

				if (action & ACTION_ENTITY_END) 		
				{
					m_bAcquiring = LFalse;

					if(EntityHandler)
					{
						STOXSTATE ParserState;
						GetParserState(&ParserState);
						(*EntityHandler)(m_sBuffer, &ParserState, m_pUserData);
					}

					delete m_sBuffer;
					m_sBuffer = LNull;

					return;
				}

				if (action & ACTION_PUMP_START) 
					m_bAcquiring = LTrue;
			}

			LBool Core::IsInRange(LInt iRange, LChar8Bit c)
			{
				for(LInt i = 0; range_table[iRange][i].start > 0; i++) 
				{
					if(((LUChar)range_table[iRange][i].start) <= c
						&& c <= ((LUChar)range_table[iRange][i].end)) 
					{
						return LTrue;
					}
				}

				return LFalse;
			}
		}
	}
}