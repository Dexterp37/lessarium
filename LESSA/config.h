/** 
 *	\file config.h
 *	License: zlib/libpng
 *
 *	The core configuration file
 *
 *	\author Alessio Pierluigi Placitelli
 *	\version
 *	\todo
 *			- split types in a different file? types.h?
 *	\note Using preprocessor, we can "hide" platform dependent code. 
 *	Just let's not try to make everything rely on preprocessor, oh?
 *	Otherwise code would like UGLY...
 */

/**
 *	\mainpage LESSA Online Documentation
 *
 *	This is stupid
 *
 */

#ifndef _CONFIG_H_
#define _CONFIG_H_

/* 
	*** SYMBIAN ***
*/
#ifdef COMPILE_FOR_SYMBIAN	

#include <e32def.h> 
#include <e32std.h>
#include <apparc.h>

#define LNull	NULL

typedef TInt	LInt;
typedef TUint	LUint;
typedef TBool	LBool;
//typedef ETrue	LTrue;
//typedef EFalse	LFalse;
#define LTrue	ETrue
#define LFalse	EFalse
typedef TAny	LVoid;
typedef HBufC*	LString;
typedef TChar	LChar;
typedef TUint32	LUlong;
typedef TUint16 LUshort;
//typedef TInt32	LDate;	// Be sure it is 32bit long (Check LDate_Format.txt)

/* 
	*** WINDOWS ***
*/
#elif COMPILE_FOR_WINDOWS

#include <windows.h>

#define LNull	NULL

typedef int		LInt;
typedef unsigned int LUint;
typedef bool	LBool;
//typedef true	LTrue;
//typedef false   LFalse;
#define LTrue	true
#define LFalse	false
typedef void	LVoid;
typedef char*	LString;
typedef char	LChar;
typedef DWORD	LUlong;
typedef WORD	LUshort;

/* 
	*** LINUX ***
*/
#elif COMPILE_FOR_LINUX	

#define LNull	NULL

typedef int		LInt;
typedef unsigned int LUint;
typedef bool	LBool;
//typedef true	LTrue;
//typedef false   LFalse;
#define LTrue	true
#define LFalse	false
typedef void	LVoid;
typedef char*	LString;
typedef char	LChar;
typedef unsigned long	LUlong;
typedef unsigned short	LUshort;
#endif


/* 
	*** GENERIC ***
*/
typedef unsigned char	LUChar;
typedef char			LChar8Bit;
typedef struct {
	LUshort m_uYear;	///< Year (0 - 65535)
	LUshort m_uMonth;	///< Month (1 - 12)
	LUshort m_uDay;		///< Day (1 - 31)
	LUshort m_uHour;	///< Hour (0 - 23)
	LUshort m_uMinute;	///< Minute (0 - 59)
	LUshort m_uSecond;	///< Second (0 - 59)
}LDate;

#endif //_CONFIG_H_