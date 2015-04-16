/**
	License: zlib/libpng
	Author: Alessio Pierluigi Placitelli
	Company:

	Description: This class holds Symbian FSS core functionalities implementation

	Revision:

	Todo:
*/

#ifdef COMPILE_FOR_SYMBIAN

#ifndef _SYMBIAN_FSSCORE_H_
#define _SYMBIAN_FSSCORE_H_

#include "fsCore.h"

namespace PDO
{
	namespace FSS
	{
		class SymbianCore : public Core
		{
		public:
			inline SymbianCore() {};
			inline virtual ~SymbianCore() {};

			// Operations
			LBool Exists(const String  *sFileName);
			//LInt  GetAttributes(const String *sFileName);	// Do we need it?
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

#endif // _SYMBIAN_FSSCORE_H_

#endif // COMPILE_FOR_SYMBIAN