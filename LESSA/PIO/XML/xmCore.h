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
 *	\file xmCore.h
 *	\class PIO::XML::Parser::Core xmCore.h "PIO/XML/xmCore.h"
 *	\enum PIO::XML::Parser::EXMLPARSERSTATUS xmCore.h "PIO/XML/xmCore.h"
 *	\struct PIO::XML::Parser::STOXSTATE xmCore.h "PIO/XML/xmCore.h"
 *	\struct PIO::XML::Parser::SXMLHANDLERS xmCore.h "PIO/XML/xmCore.h"
 *	\struct PIO::XML::Parser::LChar8Range xmCore.h "PIO/XML/xmCore.h"
 *
 *	License: LGPL
 *
 *	This class holds XML Parser class, based on Eckhart Köppen's 
 *	Tox XML Tokenizer (http://www.40hz.org/tox/) 
 *
 *	\author Eckhart Köppen adapted by Alessio Pierluigi Placitelli
 *	\version
 *	\todo
 *		- m_pFile is just used in ParseFile, why is it a class member then?
 *		- rename EXMLPARSERSTATUS to EXMLPARSERSTATE ?
 *		- enable callbacks return values to influence Tox parsing
 *		- enable caller to disable data acquisition until some event is fired 
 *		(should make things faster)
 */
//$log$ 14/05/2006 added pragmas to disable truncation warning 4309
#ifndef _XMLCORE_H_
#define _XMLCORE_H_

#include "PDO/String/stCore.h"
#include "PDO/FSS/File/foCore.h"

#define SPECIAL_SKIP_DTD 	0
#define SPECIAL_FIND_STRING 1

#define RANGE_LETTER     0
#define RANGE_CHAR       1
#define RANGE_WS         2
#define RANGE_NAMECHAR   3
#define RANGE_HEX        4
#define RANGE_DEC        5

#define ACTION_PUMP_START        1
#define ACTION_TAGNAME_END       2
#define ACTION_ATTRNAME_END      4
#define ACTION_ATTRVAL_END       8
#define ACTION_WORD_END         16
#define ACTION_WS_END           32
#define ACTION_ENTITY_END	   128

namespace PIO
{
	namespace XML
	{
		namespace Parser
		{
			typedef enum {
				STATE_ERROR = -2,
				STATE_END = -1,
				STATE_CONTENT = 0,
				STATE_STARTTAG_START = 1,
				STATE_REF_START = 2,
				STATE_CHARDATA = 3,
				STATE_ENDTAG_START = 4,
				STATE_STARTTAG_NAME = 5,
				STATE_REF_NAME = 6,
				STATE_ENDTAG_NAME = 7,
				STATE_STARTTAG_NAMEEND = 8,
				STATE_ENDTAG_NAMEEND = 9,
				STATE_ATTR_NAME = 10,
				STATE_ATTR_NAMEEND = 11,
				STATE_ATTR_VALSTART = 12,
				STATE_ATTR_VALDQUOT = 13,
				STATE_EMPTYTAG_END = 14,
				STATE_ATTR_VALDQUOT_REF = 15,
				STATE_ATTR_VDQ_REFNAME = 16,
				STATE_ATTR_VALSQUOT = 17,
				STATE_ATTR_VALSQUOT_REF = 18,
				STATE_ATTR_VSQ_REFNAME = 19,
				STATE_DTD_START = 20,
				STATE_MARKUPDECL_START = 21,
				STATE_CDATA = 22,
				STATE_CDATA_1ST_BRACKET = 23,
				STATE_CDATA_2ND_BRACKET = 24,
				STATE_PI = 25,
				STATE_PI_END_QMARK = 26,
				STATE_COMMENT = 27,
				STATE_COMMENT_1ST_DASH = 28,
				STATE_COMMENT_2ND_DASH = 29,
				STATE_REF_NUMBER = 30,
				STATE_REF_HEX_NUMBER_1 = 31,
				STATE_REF_HEX_NUMBER = 32,
				STATE_REF_DEC_NUMBER = 33,
				STATE_WS_CONTENT = 34,
				STATE_WORD_CONTENT = 35,
				STATE_COMMENT_START_1ST = 36,
				STATE_DOCTYPE_D = 37,
				STATE_DOCTYPE_O = 38,
				STATE_DOCTYPE_C = 39,
				STATE_DOCTYPE_T = 40,
				STATE_DOCTYPE_Y = 41,
				STATE_DOCTYPE_P = 42,
				STATE_CDATA_BRACKET = 43,
				STATE_CDATA_C = 44,
				STATE_CDATA_D = 45,
				STATE_CDATA_A = 46,
				STATE_CDATA_T = 47,
				STATE_CDATA_A2 = 48,
				STATE_ATTR_VALTOKEN = 49
			}EXMLPARSERSTATUS;

			typedef struct {
				EXMLPARSERSTATUS	m_eState;		///< Automa State
				LBool				m_bAcquiring;	///< Parser is saving some information to m_sBuffer
				LInt				m_iParserPos;	///< Parser position in current string parsing
				LInt				m_isQuoteLevel;
				LInt				m_idQuoteLevel;
				LInt				m_iAngleBracketLevel;
				LInt				m_iLineNumber;	///< What line is parser parsing?
			}STOXSTATE;

			typedef LInt (*START_ELEMENT_HANDLER)(const String *pTagName, const STOXSTATE *pParserState, LVoid *pUserData);
			typedef LInt (*ATTRIBUTE_NAME_HANDLER)(const String *pAttrName, const STOXSTATE *pParserState, LVoid *pUserData);
			typedef LInt (*ATTRIBUTE_VALUE_HANDLER)(const String *pAttrVal, const STOXSTATE *pParserState, LVoid *pUserData);
			typedef LInt (*WORD_END_HANDLER)(const String *pWord, const STOXSTATE *pParserState, LVoid *pUserData);
			typedef LInt (*WHITESPACE_HANDLER)(const String *pWS, const STOXSTATE *pParserState, LVoid *pUserData);
			typedef LInt (*ENTITY_HANDLER)(const String *pEntity, const STOXSTATE *pParserState, LVoid *pUserData);
			
			typedef struct {
				START_ELEMENT_HANDLER	pStartElementHandler;
				ATTRIBUTE_NAME_HANDLER	pAttrNameHandler;
				ATTRIBUTE_VALUE_HANDLER pAttrValHandler;
				WORD_END_HANDLER		pWordEndHandler;
				WHITESPACE_HANDLER		pWSHandler;
				ENTITY_HANDLER			pEntityHandler;
			}SXMLHANDLERS;

			typedef struct
			{
				LChar8Bit start;
				LChar8Bit end;
			} LChar8Range;

#pragma warning(disable:4309)
			const LChar8Range range_table[][12] = {
				/* letter */
				{
					{0x0041, 0x005A},
					{0x0061, 0x007A},
					{0x00C0, 0x00D6},
					{0x00D8, 0x00F6},
					{0x00F8, 0x00FF}
				},
					/* character */
				{
					{0x0009, 0x0009},
					{0x000A, 0x000A},
					{0x000D, 0x000D},
					{0x0020, 0x00FF}
				},
					/* whitespace */
				{
					{0x0009, 0x0009},
					{0x000A, 0x000A},
					{0x000D, 0x000D},
					{0x0020, 0x0020},
				},
				/* name character */
				{
					{'.', '.'},
					{'-', '-'},
					{'_', '_'},
					{':', ':'},
					{0x0030, 0x0039},
					{0x0041, 0x005A},
					{0x0061, 0x007A},
					{0x00B7, 0x00B7},
					{0x00C0, 0x00D6},
					{0x00D8, 0x00F6},
					{0x00F8, 0x00FF}
				},
				/* hex numbers */
				{
					{'0', '9'},
					{'a', 'f'},
					{'A', 'F'}
				},
					/* digits */
				{
					{'0', '9'}
				},
			};
#pragma warning(default:4309)

			typedef enum {
				TYPE_RANGE,
				TYPE_EXACT,
				TYPE_CHAR,
				TYPE_SPECIAL,
				TYPE_END
			}EXMLPARSER_ACCEPTTYPES;

			typedef struct
			{
				EXMLPARSER_ACCEPTTYPES accept_type;
				LChar8Bit accept_value;
				PIO::XML::Parser::EXMLPARSERSTATUS next_state;
				LUChar action;
			} state_def;

			const state_def state_table[][9] = {
				//  0: STATE_CONTENT 
				{
					{TYPE_EXACT, (LChar8Bit) '<', STATE_STARTTAG_START, 0},
					{TYPE_EXACT, (LChar8Bit) '&', STATE_REF_START, ACTION_PUMP_START},
					{TYPE_RANGE, (LChar8Bit) RANGE_WS, STATE_WS_CONTENT, ACTION_PUMP_START},
					{TYPE_RANGE, (LChar8Bit) RANGE_CHAR, STATE_WORD_CONTENT, ACTION_PUMP_START},
					{TYPE_END, 0, STATE_ERROR, 0}
				},
					//  1: STATE_STARTTAG_START 
				{
					{TYPE_EXACT, (LChar8Bit) '/', STATE_ENDTAG_START, ACTION_PUMP_START},
					{TYPE_EXACT, (LChar8Bit) '_', STATE_STARTTAG_NAME, ACTION_PUMP_START},
					{TYPE_EXACT, (LChar8Bit) ':', STATE_STARTTAG_NAME, ACTION_PUMP_START},
					{TYPE_EXACT, (LChar8Bit) '?', STATE_PI, 0},
					{TYPE_EXACT, (LChar8Bit) '!', STATE_MARKUPDECL_START, 0},
#ifdef HTML
					{TYPE_RANGE, (LChar8Bit) RANGE_CHAR, STATE_STARTTAG_NAME, ACTION_PUMP_START},
#else
					{TYPE_RANGE, (LChar8Bit) RANGE_LETTER, STATE_STARTTAG_NAME, ACTION_PUMP_START},
#endif		
					{TYPE_END, 0, STATE_ERROR, 0}
				},
					//  2: STATE_REF_START 
				{
					{TYPE_EXACT, (LChar8Bit) '_', STATE_REF_NAME, 0},
					{TYPE_EXACT, (LChar8Bit) ':', STATE_REF_NAME, 0},
					{TYPE_EXACT, (LChar8Bit) '#', STATE_REF_NUMBER, 0},
					{TYPE_RANGE, (LChar8Bit) RANGE_LETTER, STATE_REF_NAME, 0},
#ifdef HTML         
					{TYPE_RANGE, (LChar8Bit) RANGE_WS, STATE_WS_CONTENT, ACTION_PUMP_START},
#endif         
					{TYPE_END, 0, STATE_ERROR, 0}
				},
					//  3: STATE_CHARDATA 
				{
					{TYPE_END, 0, STATE_ERROR, 0}
				},
					//  4: STATE_ENDTAG_START 
				{
					{TYPE_EXACT, (LChar8Bit) '_', STATE_ENDTAG_NAME, 0},
					{TYPE_EXACT, (LChar8Bit) ':', STATE_ENDTAG_NAME, 0},
					{TYPE_RANGE, (LChar8Bit) RANGE_LETTER, STATE_ENDTAG_NAME, 0},
					{TYPE_END, 0, STATE_ERROR, 0}
				},
					//  5: STATE_STARTTAG_NAME 
				{
					{TYPE_RANGE, (LChar8Bit) RANGE_NAMECHAR, STATE_STARTTAG_NAME, 0},
					{TYPE_RANGE, (LChar8Bit) RANGE_WS, STATE_STARTTAG_NAMEEND, ACTION_TAGNAME_END},
					{TYPE_EXACT, (LChar8Bit) '/', STATE_EMPTYTAG_END, 0},
					{TYPE_EXACT, (LChar8Bit) '>', STATE_CONTENT, ACTION_TAGNAME_END},
					{TYPE_END, 0, STATE_ERROR, 0}
				},
					//  6: STATE_REF_NAME 
				{
					{TYPE_EXACT, (LChar8Bit) ';', STATE_CONTENT, ACTION_ENTITY_END},
#ifdef HTML         
					{TYPE_EXACT, (LChar8Bit) '&', STATE_CONTENT, ACTION_ENTITY_END},
					{TYPE_RANGE, (LChar8Bit) RANGE_WS, STATE_CONTENT, ACTION_ENTITY_END},
					{TYPE_RANGE, (LChar8Bit) RANGE_CHAR, STATE_REF_NAME, 0},
#else
					{TYPE_RANGE, (LChar8Bit) RANGE_NAMECHAR, STATE_REF_NAME, 0},
#endif         
					{TYPE_END, 0, STATE_ERROR, 0}
				},
					//  7: STATE_ENDTAG_NAME 
				{
					{TYPE_RANGE, (LChar8Bit) RANGE_NAMECHAR, STATE_ENDTAG_NAME, 0},
					{TYPE_RANGE, (LChar8Bit) RANGE_WS, STATE_ENDTAG_NAMEEND, ACTION_TAGNAME_END},
					//   {TYPE_EXACT, (LChar8Bit) '/', STATE_EMPTYTAG_END, 0}, ??? 
					{TYPE_EXACT, (LChar8Bit) '>', STATE_CONTENT, ACTION_TAGNAME_END},
					{TYPE_END, 0, STATE_ERROR, 0}
				},
					//  8: STATE_STARTTAG_NAMEEND 
				{
					{TYPE_RANGE, (LChar8Bit) RANGE_WS, STATE_STARTTAG_NAMEEND,  0},
					{TYPE_EXACT, (LChar8Bit) '>', STATE_CONTENT,  0},
					{TYPE_EXACT, (LChar8Bit) '/', STATE_EMPTYTAG_END, 0},
					{TYPE_EXACT, (LChar8Bit) '_', STATE_ATTR_NAME, ACTION_PUMP_START},
					{TYPE_EXACT, (LChar8Bit) ':', STATE_ATTR_NAME, ACTION_PUMP_START},
					{TYPE_RANGE, (LChar8Bit) RANGE_LETTER, STATE_ATTR_NAME, ACTION_PUMP_START},
#ifdef HTML
					{TYPE_EXACT, (LChar8Bit) '<', STATE_STARTTAG_START,  0}, // Hack 
					{TYPE_RANGE, (LChar8Bit) RANGE_CHAR, STATE_STARTTAG_NAMEEND,  0},
#endif         
					{TYPE_END, 0, STATE_ERROR, 0}
				},
					//  9: STATE_ENDTAG_NAMEEND 
				{
					{TYPE_RANGE, (LChar8Bit) RANGE_WS, STATE_ENDTAG_NAMEEND, 0},
					{TYPE_EXACT, (LChar8Bit) '>', STATE_CONTENT, 0},
					{TYPE_END, 0, STATE_ERROR, 0}
				},
					// 10: STATE_ATTR_NAME 
				{
					{TYPE_RANGE, (LChar8Bit) RANGE_WS, STATE_ATTR_NAMEEND, ACTION_ATTRNAME_END},
					{TYPE_RANGE, (LChar8Bit) RANGE_NAMECHAR, STATE_ATTR_NAME, 0},
					{TYPE_EXACT, (LChar8Bit) '=', STATE_ATTR_VALSTART, ACTION_ATTRNAME_END},
#ifdef HTML
					{TYPE_EXACT, (LChar8Bit) '/', STATE_EMPTYTAG_END, ACTION_ATTRNAME_END},
					{TYPE_EXACT, (LChar8Bit) '>', STATE_CONTENT, ACTION_ATTRNAME_END},
#endif         
					{TYPE_END, 0, STATE_ERROR, 0}
				},
					// 11: STATE_ATTR_NAMEEND 
				{
					{TYPE_RANGE, (LChar8Bit) RANGE_WS, STATE_ATTR_NAMEEND, 0},
					{TYPE_EXACT, (LChar8Bit) '=', STATE_ATTR_VALSTART, 0},
#ifdef HTML
					{TYPE_RANGE, (LChar8Bit) RANGE_NAMECHAR, STATE_ATTR_NAME, ACTION_PUMP_START},
#endif         
					{TYPE_END, 0, STATE_ERROR, 0}
				},
					// 12: STATE_ATTR_VALSTART 
				{
					{TYPE_RANGE, (LChar8Bit) RANGE_WS, STATE_ATTR_VALSTART, 0},
					{TYPE_EXACT, (LChar8Bit) '"', STATE_ATTR_VALDQUOT, 0},
					{TYPE_EXACT, (LChar8Bit) '\'', STATE_ATTR_VALSQUOT, 0},
#ifdef HTML
					{TYPE_RANGE, (LChar8Bit) RANGE_CHAR, STATE_ATTR_VALTOKEN, 0},
#endif         
					{TYPE_END, 0, STATE_ERROR, 0}
				},
					// 13: STATE_ATTR_VALDQUOT 
				{
					{TYPE_EXACT, (LChar8Bit) '"', STATE_STARTTAG_NAMEEND, ACTION_ATTRVAL_END},
#ifndef HTML         
					{TYPE_EXACT, (LChar8Bit) '<', STATE_ERROR, 0},
					{TYPE_EXACT, (LChar8Bit) '&', STATE_ATTR_VALDQUOT_REF, ACTION_PUMP_START},
#endif         
					{TYPE_RANGE, (LChar8Bit) RANGE_CHAR, STATE_ATTR_VALDQUOT, ACTION_PUMP_START},
					{TYPE_END, 0, STATE_ERROR, 0}
				},
					// 14: STATE_EMPTYTAG_END 
				{
					{TYPE_EXACT, (LChar8Bit) '>', STATE_CONTENT, 0},
#ifdef HTML         
					{TYPE_RANGE, (LChar8Bit) RANGE_CHAR, STATE_CONTENT, 0},
#endif         
					{TYPE_END, 0, STATE_ERROR, 0}
				},
					// 15: STATE_ATTR_VALDQUOT_REF 
				{
					{TYPE_EXACT, (LChar8Bit) '_', STATE_ATTR_VDQ_REFNAME, 0},
					{TYPE_EXACT, (LChar8Bit) ':', STATE_ATTR_VDQ_REFNAME, 0},
					{TYPE_RANGE, (LChar8Bit) RANGE_LETTER, STATE_ATTR_VDQ_REFNAME, 0},
					{TYPE_END, 0, STATE_ERROR, 0}
				},
					// 16: STATE_ATTR_VDQ_REFNAME 
				{
					{TYPE_RANGE, (LChar8Bit) RANGE_NAMECHAR, STATE_ATTR_VDQ_REFNAME, 0},
					{TYPE_EXACT, (LChar8Bit) ';', STATE_ATTR_VALDQUOT, ACTION_ENTITY_END},
					{TYPE_END, 0, STATE_ERROR, 0}
				},
					// 17: STATE_ATTR_VALSQUOT 
				{
					{TYPE_EXACT, (LChar8Bit) '\'', STATE_STARTTAG_NAMEEND, ACTION_ATTRVAL_END},
#ifndef HTML         
					{TYPE_EXACT, (LChar8Bit) '<', STATE_ERROR, 0},
					{TYPE_EXACT, (LChar8Bit) '&', STATE_ATTR_VALSQUOT_REF, ACTION_PUMP_START},
#endif         
					{TYPE_RANGE, (LChar8Bit) RANGE_CHAR, STATE_ATTR_VALSQUOT, ACTION_PUMP_START},
					{TYPE_END, 0, STATE_ERROR, 0}
				},
					// 18: STATE_ATTR_VALSQUOT_REF 
				{
					{TYPE_EXACT, (LChar8Bit) '_', STATE_ATTR_VSQ_REFNAME, 0},
					{TYPE_EXACT, (LChar8Bit) ':', STATE_ATTR_VSQ_REFNAME, 0},
					{TYPE_RANGE, (LChar8Bit) RANGE_LETTER, STATE_ATTR_VSQ_REFNAME, 0},
#ifdef HTML
					{TYPE_RANGE, (LChar8Bit) RANGE_WS, STATE_ATTR_VALSQUOT, 0},
#endif         
					{TYPE_END, 0, STATE_ERROR, 0}
				},
					// 19: STATE_ATTR_VSQ_REFNAME 
				{
					{TYPE_RANGE, (LChar8Bit) RANGE_NAMECHAR, STATE_ATTR_VSQ_REFNAME, 0},
					{TYPE_EXACT, (LChar8Bit) ';', STATE_ATTR_VALSQUOT, ACTION_ENTITY_END},
					{TYPE_END, 0, STATE_ERROR, 0}
				},
					// 20: STATE_DTD_START 
				{
					{TYPE_SPECIAL, (LChar8Bit) SPECIAL_SKIP_DTD, STATE_CONTENT, 0},
					{TYPE_END, 0, STATE_ERROR, 0}
				},
					// 21: STATE_MARKUPDECL_START 
				{
					{TYPE_EXACT, (LChar8Bit) '-', STATE_COMMENT_START_1ST, 0},
#ifdef HTML
					{TYPE_CHAR, (LChar8Bit) 'D', STATE_DOCTYPE_D, 0},
#else
					{TYPE_EXACT, (LChar8Bit) 'D', STATE_DOCTYPE_D, 0},
#endif         
					{TYPE_EXACT, (LChar8Bit) '[', STATE_CDATA_BRACKET, 0},
					{TYPE_END, 0, STATE_ERROR, 0}
				},
					// 22: STATE_CDATA 
				{
					{TYPE_EXACT, (LChar8Bit) ']', STATE_CDATA_1ST_BRACKET, 0},
					{TYPE_RANGE, (LChar8Bit) RANGE_CHAR, STATE_CDATA, 0},
					{TYPE_END, 0, STATE_ERROR, 0}
				},
					// 23: STATE_CDATA_1ST_BRACKET 
				{
					{TYPE_EXACT, (LChar8Bit) ']', STATE_CDATA_2ND_BRACKET, 0},
					{TYPE_RANGE, (LChar8Bit) RANGE_CHAR, STATE_CDATA, 0},
					{TYPE_END, 0, STATE_ERROR, 0}
				},
					// 24: STATE_CDATA_2ND_BRACKET 
				{
					{TYPE_EXACT, (LChar8Bit) '>', STATE_CONTENT, 0},
					{TYPE_EXACT, (LChar8Bit) ']', STATE_CDATA_2ND_BRACKET, 0},
					{TYPE_RANGE, (LChar8Bit) RANGE_CHAR, STATE_CDATA, 0},
					{TYPE_END, 0, STATE_ERROR, 0}
				},
					// 25: STATE_PI 
				{
					{TYPE_EXACT, (LChar8Bit) '?', STATE_PI_END_QMARK, 0},
					{TYPE_RANGE, (LChar8Bit) RANGE_CHAR, STATE_PI, 0},
					{TYPE_END, 0, STATE_ERROR, 0}
				},
					// 26: STATE_PI_END_QMARK 
				{
					{TYPE_EXACT, (LChar8Bit) '>', STATE_CONTENT, 0},
					{TYPE_EXACT, (LChar8Bit) '?', STATE_PI_END_QMARK, 0},
					{TYPE_RANGE, (LChar8Bit) RANGE_CHAR, STATE_PI, 0},
					{TYPE_END, 0, STATE_ERROR, 0}
				},
					//27: STATE_COMMENT 
				{
					{TYPE_EXACT, (LChar8Bit) '-', STATE_COMMENT_1ST_DASH, 0},
					{TYPE_RANGE, (LChar8Bit) RANGE_CHAR, STATE_COMMENT, 0},
					{TYPE_END, 0, STATE_ERROR, 0}
				},
					// 28: STATE_COMMENT_1ST_DASH 
				{
					{TYPE_EXACT, (LChar8Bit) '-', STATE_COMMENT_2ND_DASH, 0},
					{TYPE_RANGE, (LChar8Bit) RANGE_CHAR, STATE_COMMENT, 0},
					{TYPE_END, 0, STATE_ERROR, 0}
				},
					// 29: STATE_COMMENT_2ND_DASH 
				{
					{TYPE_EXACT, (LChar8Bit) '>', STATE_CONTENT, 0},
#ifdef HTML         
					{TYPE_RANGE, (LChar8Bit) RANGE_CHAR, STATE_COMMENT_2ND_DASH, 0},
#endif         
					{TYPE_END, 0, STATE_ERROR, 0}
				},
					// 30: STATE_REF_NUMBER 
				{
					{TYPE_EXACT, (LChar8Bit) 'x', STATE_REF_HEX_NUMBER_1, 0},
					{TYPE_RANGE, (LChar8Bit) RANGE_DEC, STATE_REF_DEC_NUMBER, 0},
					{TYPE_END, 0, STATE_ERROR, 0}
				},
					// 31: STATE_REF_HEX_NUMBER_1 
				{
					{TYPE_RANGE, (LChar8Bit) RANGE_HEX, STATE_REF_HEX_NUMBER, 0},
					{TYPE_END, 0, STATE_ERROR, 0}
				},
					// 32: STATE_REF_HEX_NUMBER 
				{
					{TYPE_RANGE, (LChar8Bit) RANGE_HEX, STATE_REF_HEX_NUMBER, 0},
					{TYPE_EXACT, (LChar8Bit) ';', STATE_CONTENT, ACTION_ENTITY_END},
					{TYPE_END, 0, STATE_ERROR, 0}
				},
					// 33: STATE_REF_DEC_NUMBER 
				{
					{TYPE_RANGE, (LChar8Bit) RANGE_DEC, STATE_REF_DEC_NUMBER, 0},
					{TYPE_EXACT, (LChar8Bit) ';', STATE_CONTENT, ACTION_ENTITY_END},
					{TYPE_END, 0, STATE_ERROR, 0}
				},
					// 34: STATE_WS_CONTENT 
				{
					{TYPE_EXACT, (LChar8Bit) '<', STATE_STARTTAG_START, ACTION_WS_END},
					{TYPE_EXACT, (LChar8Bit) '&', STATE_REF_START, ACTION_PUMP_START | ACTION_WS_END},
					{TYPE_RANGE, (LChar8Bit) RANGE_WS, STATE_WS_CONTENT, 0},
					{TYPE_RANGE, (LChar8Bit) RANGE_CHAR, STATE_WORD_CONTENT, ACTION_PUMP_START | ACTION_WS_END},
					{TYPE_END, 0, STATE_ERROR, 0}
				},
					// 35: STATE_WORD_CONTENT 
				{
					{TYPE_EXACT, (LChar8Bit) '<', STATE_STARTTAG_START, ACTION_WORD_END},
					{TYPE_EXACT, (LChar8Bit) '&', STATE_REF_START, ACTION_PUMP_START | ACTION_WORD_END},
					{TYPE_RANGE, (LChar8Bit) RANGE_WS, STATE_WS_CONTENT, ACTION_PUMP_START | ACTION_WORD_END},
					{TYPE_RANGE, (LChar8Bit) RANGE_CHAR, STATE_WORD_CONTENT, 0},
					{TYPE_END, 0, STATE_ERROR, 0}
				},
					// 36: STATE_COMMENT_START_1ST 
				{
					{TYPE_EXACT, (LChar8Bit) '-', STATE_COMMENT, 0},
					{TYPE_END, 0, STATE_ERROR, 0}
				},
					// 37: STATE_DOCTYPE_D 
				{
					{TYPE_CHAR, (LChar8Bit) 'O', STATE_DOCTYPE_O, 0},
					{TYPE_END, 0, STATE_ERROR, 0}
				},
					// 38: STATE_DOCTYPE_O 
				{
					{TYPE_CHAR, (LChar8Bit) 'C', STATE_DOCTYPE_C, 0},
					{TYPE_END, 0, STATE_ERROR, 0}
				},
					// 39: STATE_DOCTYPE_C 
				{
					{TYPE_CHAR, (LChar8Bit) 'T', STATE_DOCTYPE_T, 0},
					{TYPE_END, 0, STATE_ERROR, 0}
				},
					// 40: STATE_DOCTYPE_T 
				{
					{TYPE_CHAR, (LChar8Bit) 'Y', STATE_DOCTYPE_Y, 0},
					{TYPE_END, 0, STATE_ERROR, 0}
				},
					// 41: STATE_DOCTYPE_Y 
				{
					{TYPE_CHAR, (LChar8Bit) 'P', STATE_DOCTYPE_P, 0},
					{TYPE_END, 0, STATE_ERROR, 0}
				},
					// 42: STATE_DOCTYPE_P 
				{
					{TYPE_CHAR, (LChar8Bit) 'E', STATE_DTD_START, 0},
					{TYPE_END, 0, STATE_ERROR, 0}
				},
					// 43: STATE_CDATA_BRACKET 
				{
					{TYPE_CHAR, (LChar8Bit) 'C', STATE_CDATA_C, 0},
					{TYPE_END, 0, STATE_ERROR, 0}
				},
					// 44: STATE_CDATA_C 
				{
					{TYPE_CHAR, (LChar8Bit) 'D', STATE_CDATA_D, 0},
					{TYPE_END, 0, STATE_ERROR, 0}
				},
					// 45: STATE_CDATA_D 
				{
					{TYPE_CHAR, (LChar8Bit) 'A', STATE_CDATA_A, 0},
					{TYPE_END, 0, STATE_ERROR, 0}
				},
					// 46: STATE_CDATA_A 
				{
					{TYPE_CHAR, (LChar8Bit) 'T', STATE_CDATA_T, 0},
					{TYPE_END, 0, STATE_ERROR, 0}
				},
					// 47: STATE_CDATA_T 
				{
					{TYPE_CHAR, (LChar8Bit) 'A', STATE_CDATA_A2, 0},
					{TYPE_END, 0, STATE_ERROR, 0}
				},
					// 48: STATE_CDATA_A2 
				{
					{TYPE_EXACT, (LChar8Bit) '[', STATE_CDATA, 0},
					{TYPE_END, 0, STATE_ERROR, 0}
				},
					// 49: STATE_ATTR_VALTOKEN 
				{
					{TYPE_EXACT, (LChar8Bit) '>', STATE_CONTENT, ACTION_ATTRVAL_END},
					{TYPE_RANGE, (LChar8Bit) RANGE_WS, STATE_STARTTAG_NAMEEND, ACTION_ATTRVAL_END},
					{TYPE_RANGE, (LChar8Bit) RANGE_CHAR, STATE_ATTR_VALTOKEN, ACTION_PUMP_START},
					{TYPE_END, 0, STATE_ERROR, 0}
				},
#ifdef HTML	
					// 50: STATE_SKIP_TAG 
				{
					{TYPE_SPECIAL, (LChar8Bit) SPECIAL_FIND_STRING, STATE_CONTENT, 0},
				},
#endif	
			};

			class Core
			{
			public:
				Core() : m_pFile(LNull), m_eState(STATE_CONTENT), m_sBuffer(LNull), 
						 m_bAcquiring(LFalse), m_iParserPos(LNull), m_isQuoteLevel(LNull), 
						 m_idQuoteLevel(LNull), m_iAngleBracketLevel(LNull),
						 m_pUserData(LNull),
						 m_iLineNumber(-1),
						 StartElementHandler(LNull),
						 AttributeNameHandler(LNull),
						 AttributeValueHandler(LNull),
						 WordEndHandler(LNull),
						 WhiteSpaceHandler(LNull),
						 EntityHandler(LNull) {};

				inline virtual ~Core() {delete m_pFile; m_pFile = LNull; delete m_sBuffer; m_sBuffer = LNull; m_pUserData = LNull;};

				/**
					Opens and parses the file located at path. File gets closed once parsing is done.

					\param[in] path A String parameter which contains the location of the file to parse
					\return Returns LTrue if file has been parsed correctly, LFalse otherwise
					\note This function simply calls ParseLine on each file line. This function may return
					depending on callback return values
					\see ParseLine
				*/
				LBool	ParseFile(const String *path);
				
				/**
					Continues parsing started with ParseFile function

					\return Returns LTrue if file has been parsed correctly, LFalse otherwise
					\note This function simply calls ParseLine on each file line. This function may return
					depending on callback return values
					\see ParseLine ParseFile
				*/
				LBool	ContinueFileParsing();
				

				/**
					Parses a single XML string, raising when necessary events.

					\param[in] line A String parameter which contains the string to parse
					\return Returns LTrue if file has been parsed correctly, LFalse otherwise
					\attention To get events fired, callback pointers should be initialized by calling SetHandlers
					\see SetHandlers
				*/				
				LInt	ParseLine(const String *line);

				/**
					Let parser know which functions to call when events are raised.

					\param[in] pHandlers A SXMLHANDLERS pointer which contains information about callback functions
					which get fired when events are raised
					\see SXMLHANDLERS
				*/	
				LVoid	SetHandlers(const SXMLHANDLERS *pHandlers);

				/**
					Gets information about current parser state.

					\param[out] pState A STOXSTATE pointer which will be initialized to parser state values
					\see STOXSTATE
				*/	
				void	GetParserState(STOXSTATE * const pState);

				/**
					Sets user data to be passed as parameter to callback events for pUserData parameter

					\param[in] pUserData A LVoid 32bit pointer holding user data.
					\note This is an UGLY hack to allow class member functions as callbacks. This allows
					to pass static class member which will, internally, call class member function.
				*/	
				void	SetCBUserData(LVoid	*pUserData);

				START_ELEMENT_HANDLER		StartElementHandler;	///< This function gets called when element name is found (i.e. <a_name [...]>)
				ATTRIBUTE_NAME_HANDLER		AttributeNameHandler;	///< This function gets called when an attribute name is found (i.e. <[...] attr_name=[...]>)
				ATTRIBUTE_VALUE_HANDLER		AttributeValueHandler;	///< This function gets called when an attribute value is found (i.e. <[...]="value"[...]>)
				WORD_END_HANDLER			WordEndHandler;			///< This function gets called for every word between tags separated by whitespaces (i.e. </[...]>Word<[...]>)
				WHITESPACE_HANDLER			WhiteSpaceHandler;		///< This function gets called when whitespaces, line feeds and tabulators are encountered 
				ENTITY_HANDLER				EntityHandler;			///< Unknown? Examine code :)
			
			private:
				PDO::FSS::File::Core	*m_pFile;		///< XML File descriptor
				EXMLPARSERSTATUS		m_eState;		///< XML Parser Finite Machine State
				LBool					m_bAcquiring;	///< Tells if parser is saving data to m_sBuffer
				LInt					m_iParserPos;	///< Parser position in current string parsing
				LInt					m_isQuoteLevel;
				LInt					m_idQuoteLevel;
				LInt					m_iAngleBracketLevel;
				LInt					m_iLineNumber;	///< What line number is parser on?
				LVoid					*m_pUserData;	///< Set by SetUserDataCB to be passed to even CBs. Doesn't get deleted.

				String					*m_sBuffer;		///< Internal string buffer

				/**
					Gets called by ParseLine when an action must be taken

					\param[in] iVariant A LInt variable which indicates which variation of state_table entry 
					for current m_eState must be considered
					\see state_def
					\see EXMLPARSERSTATUS
				*/	
				LVoid	DoAction(LInt iVariant);

				/**
					Checks if character c is within the range iRange of range_table

					\param[in] iRange A LInt parameter which tells which range must be considered
					\param[in] c A LChar8Bit parameter which holds the character to check
					\return Returns LTrue if c is in the considered range, LFalse otherwise
					\see range_table
					\see LChar8Range
				*/	
				LBool	IsInRange(LInt iRange, LChar8Bit c);	
			};

		}
	}
}
#endif // _XMLCORE_H_