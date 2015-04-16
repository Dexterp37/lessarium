/** 
 *	\file foCore.h
 *	\class PDO::FSS::File::Core foCore.h "PDO/FSS/File/foCore.h"
 *	License: zlib/libpng
 *
 *	This class holds File class, needed to read/write from files
 *
 *	\author Alessio Pierluigi Placitelli
 *	\version
 *	\todo
 *		- Change Seek return type to LBool?
 *		- Call Close() from File::Core and not from sys. dependent implementation?
 */
//$log$ 30/04/2006 Fixed bitmasks

#ifndef _FILECORE_H_
#define _FILECORE_H_

#include "PDO/String/stCore.h"

// Flags
#define LFILE_OVERWRITE_OR_CREATE		0x00000001	///< Overwrite file if already exists, create it if not
#define LFILE_READ						0x00000002	///< Open for Read operations (can be used with LFILE_WRITE)
#define LFILE_WRITE						0x00000004  ///< Open for Write operations (can be used with LFILE_READ)
#define LFILE_BINARY					0x00000008	///< Specify that file should be opened in binary mode
#define LFILE_TEXT						0x00000010  ///< Specify that file should be opened in text mode

// Seek modes
#define LFILE_SEEK_START				0	///< Move file pointer starting from the beginning of the file. Negative values may produce errors
#define LFILE_SEEK_CURRENT				1	///< Move file pointer starting from current position in the file. Positive values move forward, negative backward
#define LFILE_SEEK_END					2	///< Move file pointer starting from the end of the file.

namespace PDO
{
	namespace FSS
	{
		namespace File
		{
			class Core
			{
			protected:
				Core();

			public:
				inline virtual ~Core() {};
				
				static Core *createCore();
				
				/**
					Opens the file located by path parameter using iOption 
					options

					\param[in] path A String parameter file path
					\param[in] iOptions A LInt parameter which specify modes
					\return Returns LTrue if file was opened correctly, LFalse otherwise
				*/
				virtual LBool Open(const String *path, LInt iOptions) = 0;

				/**
					Closes a previously opened file handle

					\return Returns LTrue if file was closed correctly, LFalse otherwise
					\warning On some platforms may always return LTrue
				*/
				virtual LBool Close() = 0;

				/**
					Reads from a previously opened file iToRead number of bytes

					\param[in] iToRead  A LInt parameter which holds number of bytes to read
					\return Returns a LVoid pointer to an heap allocated array which holds read data
					\attention The function is not guaranteed to read iToRead bytes. It may fail 
					because it reached the end of file. Check IsEOF
					\see IsEOF
				*/
				virtual LVoid  *Read(const LInt iToRead) = 0;

				/**
					Reads from a previously opened file to next end of line character

					\return Returns a String which holds read data, LNull if read has failed.
					\attention The function is not guaranteed to return a valid string. It may fail 
					because it reached the end of file. Check IsEOF and PDO::BaseString::Core::IsEmpty
					\see IsEOF
					\see PDO::BaseString::Core::IsEmpty
				*/
				virtual String *Read() = 0;

				/**
					Writes iBufSize bytes from pBuf to the file

					\param[in] pBuf A LVoid parameter containing data to write
					\param[in] iBufSize A LInt parameter holding number of bytes to write
					\return Returns number of bytes written to file
				*/
				virtual LInt Write(const LVoid *pBuf, const LInt iBufSize) = 0;

				/**
					Writes pStr to the file

					\param[in] pStr A String parameter containing data to write
					\return Returns number of characters written to file
				*/
				virtual LInt Write(const String *pStr) = 0;

				/**
					Moves file pointer position

					\param[in] iMode A LInt specifying seek mode to use (LFILE_SEEK_START, LFILE_SEEK_CURRENT or LFILE_SEEK_END)
					\param[in] iMoveOf A LInt parameter holding the amount of movement (can accept negative values)
					\return Returns 1 if move succeded, 0 otherwise
				*/
				virtual LInt Seek(const LInt iMode, const LInt iMoveOf) = 0;

				/**
					Checks if end of file has been reached

					\return Returns LTrue if end of file has been reached, LFalse otherwise
				*/
				virtual LBool IsEOF() = 0;

				// Dexter: Experiments..
				//virtual Core & operator << (const String &sTxt) = 0;
//				friend Core* operator << (Core *plCore, const String *pTxt);
				//Core & operator << (const String *pTxt);
			};
		}
	}
}

#endif // _FILECORE_H_