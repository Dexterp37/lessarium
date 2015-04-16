/** 
 *	\file stCore.h
 *	\class PDO::BaseString::Core stCore.h "PDO/String/stCore.h"
 *	License: zlib/libpng
 *
 *	This class attempts to overcome some system's (Symbian? shh...) newbie unfriendly
 *	string handling by providing an unified and CString (MFC) like class.
 *
 *	\author Alessio Pierluigi Placitelli
 *	\version
 *	\todo
 *	\note Use the String keyword to pass string values between functions.
 *	In order to declare a string value, use the MakeString(varName)
 *	macro and a varName string pointer will be instantiated. 
 */
#ifndef _LSTRINGCORE_H_
#define _LSTRINGCORE_H_

#include "config.h"

namespace PDO
{
	namespace BaseString
	{
		class Core
		{
		protected:
			Core();	///< Prevents user from directly allocate it. Must use createCore()

		public:
			inline virtual ~Core() {};
			
			static Core *createCore();

			/**
				Returns the length of the string instance

				\return Returns a LUint holding the length of the string
			*/
			virtual	LUint	Length() const = 0;

			/**
				Checks if string is empty

				\return Returns LTrue if string is empty, LFalse otherwise
				\note Simply calls Length to determine if string is empty.
			*/
			virtual LBool	IsEmpty() const = 0;

			/**
				Looks for pWhat in this string, and replaces it with pWith

				\param[in] pWhat A String parameter which contains the string to look for
				\param[in] pWith A String parameter which contains the string to sobstitute pWhat with
				\return Returns number of times pWhat was found
			*/
			virtual LInt	Replace(const Core *pWhat, const Core *pWith) = 0;

			/**
				Looks for pWhat in this string, and replaces it with pWith

				\param[in] pWhat A String parameter which contains the string to look for
				\param[in] pWith A String parameter which contains the string to sobstitute pWhat with
				\return Returns number of times pWhat was found
			*/
			virtual LInt	Replace(const char *pWhat, const char *pWith) = 0;

			/**
				Assigns r-value to this string

				\param[in] pContent A String parameter which is implicitly passed to the function when
				the operator is found in a valid expression.
			*/
			virtual LVoid	operator = (const char *pContent) = 0;

			/**
				Assigns r-value to this string

				\param[in] rCopy A String parameter which is implicitly passed to the function when
				the operator is found in a valid expression.
			*/
			virtual LVoid	operator = (const Core &rCopy) = 0;

			/**
				Appends pSuffix string to this string and returns a reference to
				this.

				\param[in] pSuffix A String parameter which contains the string to append
				\return Returns a reference to this pointer
			*/
			virtual Core&	operator += (const char *pSuffix) = 0;

			/**
				Appends cSingle character to this string and returns a reference to
				this.

				\param[in] cSingle A char parameter which contains the string to append
				\return Returns a reference to this pointer
			*/
			virtual Core&	operator += (char cSingle) = 0;

			/**
				Appends rSuffix string to this string and returns a reference to
				this.

				\param[in] rSuffix A String parameter which contains the string to append
				\return Returns a reference to this pointer
			*/
			virtual Core&	operator += (Core &rSuffix) = 0;

			/**
				Tests if this string is equal to r-value

				\param[in] rCompare A String parameter which is implicitly passed to the function
				\return Returns LTrue if strings are same (case sensitive), LFalse otherwise
			*/
			virtual LBool	operator == (const Core &rCompare) const = 0;

			/**
				Tests if this string is equal to r-value

				\param[in] pCompare A String parameter which is implicitly passed to the function
				\return Returns LTrue if strings are same (case sensitive), LFalse otherwise
			*/
			virtual LBool	operator == (const char *pCompare) const = 0;

			/**
				Compares this string with r-value

				\param[in] rCompare A String parameter which is implicitly passed to the function
				\return Returns LTrue if this string is less than rCompare, LFalse otherwise
			*/
			virtual LBool	operator < (const Core &rCompare) const = 0;

			/**
				Compares this string with r-value

				\param[in] rCompare A String parameter which is implicitly passed to the function
				\return Returns LTrue if this string is more than rCompare, LFalse otherwise
			*/
			virtual LBool	operator > (const Core &rCompare) const = 0;

			/**
				Returns a single character

				\param[in] n A LUint parameter which is the index of wanted character
				\return Returns the n-th character of the string
			*/
			virtual char	operator [] (LUint n) const = 0;
		};
	}
}

typedef PDO::BaseString::Core String;
#define MakeString(x) PDO::BaseString::Core * x = PDO::BaseString::Core::createCore()

#endif // _LSTRINGCORE_H_