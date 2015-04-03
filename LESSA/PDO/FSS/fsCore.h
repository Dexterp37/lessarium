/** 
 *	\file fsCore.h 
 *	\class PDO::FSS::Core fsCore.h "PDO/FSS/fsCore.h"
 *	License: zlib/libpng
 *
 *	This class holds FSS core functionalities, about everything that 
 *	regards file system (except file read/write which is handled by
 *	File class).
 *
 *	\author Alessio Pierluigi Placitelli
 *	\version
 *	\todo
 *			- Get last write time
 *			- Get installation path
 *			- Enumerate Mount Points /Drives
 *			- Change GetLESSAPath function name to something more general?	
 */

#ifndef _FSSCORE_H_
#define _FSSCORE_H_

#include "PDO/FSS/File/foCore.h"

namespace PDO
{
	namespace FSS
	{
		class Core
		{
		protected:
			Core();

		public:
			inline virtual ~Core() {};
			
			static Core *createCore();

			/**
				Takes a string variable pointing to a file or a directory path
				and checks if that path exists.

				\param[in] sFileName A String parameter containing a path
				\return Returns LTrue if file or path exists, LFalse otherwise
			*/
			virtual LBool Exists(const String *sFileName) = 0;
			//virtual LInt  GetAttributes(const String *sFileName) = 0;	// Do we need it?
			//virtual String* GetDefaultDrive	// Linux has no "Drive" :D

			/**
				Takes a string variable pointing to a path and checks if
				it is a file rather than a directory.

				\param[in] sFileName A String parameter containing a path
				\return Returns LTrue if path points to a file, LFalse if isn't a file or
				doesn't exist.
			*/
			virtual LBool IsFile(const String *sFileName) = 0;

			/**
				Takes a string variable pointing to a path and checks if
				it is a directory rather than a file.

				\param[in] sFileName A String parameter containing a path
				\return Returns LTrue if path points to a directory, LFalse if isn't a directory or
				doesn't exist.
			*/
			virtual LBool IsDirectory(const String *sFileName) = 0;

			/**
				Renames sSource to sDest.

				\param[in] sSource A String parameter containing the path to the source location
				\param[in] sDest A String parameter containing the path to the destination
				\return Returns LTrue if renaming succeded, LFalse otherwise
			*/
			virtual LBool Rename(const String *sSource, const String *sDest) = 0;

			/**
				Moves sSource to sDest.

				\param[in] sSource A String parameter containing the path to the source location
				\param[in] sDest A String parameter containing the path to the destination
				\return Returns LTrue if renaming succeded, LFalse otherwise
			*/
			virtual LBool Move(const String *sSource, const String *sDest) = 0;
			//virtual LDate GetLastModified(const String *sSource) = 0;	// TODO: LDate type...

			/**
				Creates the path pointed by sFileName

				\param[in] sFileName A String containing the path to create
				\return Returns LTrue if path was created, LFalse otherwise
			*/
			virtual LBool MakeDir(const String *sFileName) = 0;

			/**
				Sets file/directory attributes

				\param[in] sFileName A String which holds path to the file 
				to change attributes of
				\param[in] iAttributes A LInt bitfield which tells how to 
				modify attributes
				\return Returns LTrue if attributes were changed, LFalse otherwise
			*/
			virtual LBool SetAttributes(const String *sFileName, const LInt iAttributes) = 0;

			/**
				Gets LESSA system path

				\return Returns a newly allocated string holding application path
				\remarks Returned path already contains ending '\\'. Developer should 
				delete returned pointed when it is no longer needed, otherwise 
				it will be orphaned on the heap.
			*/
			static String * GetLESSAPath();
		};
	}
}

#endif // _FSSCORE_H_