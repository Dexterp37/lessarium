/**
	License: zlib/libpng
	Author: Alessio Pierluigi Placitelli
	Company:

	Description: This class holds Win32 NS core functionalities implementation

	Revision:
				15/04/2006	Dexter

	Todo:
*/

#ifdef COMPILE_FOR_WINDOWS

#ifndef _WIN32_FSSCORE_H_
#define _WIN32_FSSCORE_H_

#include "fsCore.h"

namespace PDO
{
	namespace FSS
	{
		class Win32Core : public Core
		{
		public:
			inline Win32Core() {};
			inline virtual ~Win32Core() {};
			
			// Operations
			LBool Exists(const String *sFileName);
			//LInt  GetAttributes(const String *sFileName);
			//String* GetDefaultDrive	// Linux has no "Drive" :D
			LBool IsFile(const String *sFileName);
			LBool IsDirectory(const String *sFileName);
			LBool Rename(const String *sSource, const String *sDest);
			LBool Move(const String *sSource, const String *sDest);
			//LDate GetLastModified(const String *sSource);	// TODO: LDate type...
			LBool MakeDir(const String *sFileName);
			LBool SetAttributes(const String *sFileName, const LInt iAttributes);

			// Test
			//LVoid	ShowIWork();
		};
	}
}

#endif // _WIN32_FSSCORE_H_

#endif // COMPILE_FOR_WINDOWS