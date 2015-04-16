/**
	License: zlib/libpng
	Author: Alessio Pierluigi Placitelli
	Company:

	Description: This class holds Symbian FSS core functionalities implementation

	Revision:

	Todo:
*/

#ifdef COMPILE_FOR_WINDOWS

#include "PDO/FSS/Win32fsCore.h"
#include "PDO/String/Win32stCore.h"

#include <windows.h>	// To use CAknInformationNote
#include <sys/stat.h>
#include <stdio.h>
#include <direct.h>

namespace PDO
{
	namespace FSS
	{
		Core * Core::createCore()
		{
			return new Win32Core();
		}

		LBool Win32Core::Exists(const String *sFileName)
		{
			if(!sFileName)
				return LFalse;

			char *pStr = ((PDO::BaseString::Win32Core*)sFileName)->GetRawData();

			return (GetFileAttributes(pStr) != INVALID_FILE_ATTRIBUTES);	
		}
/*
		LInt  Win32Core::GetAttributes(const String *sFileName)
		{
			return 1;
		}
*/
		LBool Win32Core::IsFile(const String *sFileName)
		{
			if(!sFileName || sFileName->IsEmpty())
				return LFalse;

			PDO::BaseString::Win32Core *pStr = (PDO::BaseString::Win32Core *)sFileName;

			struct _stati64 sbuf;

			if (_stati64(pStr->GetRawData(), &sbuf) == -1) 
				return false;

			return ((sbuf.st_mode & _S_IFDIR) == 0) &&	((sbuf.st_mode & _S_IFREG) != 0);
		}

		LBool Win32Core::IsDirectory(const String *sFileName)
		{
			if(!sFileName || sFileName->IsEmpty())
				return LFalse;

			PDO::BaseString::Win32Core *pStr = (PDO::BaseString::Win32Core *)sFileName;

			struct _stati64 sbuf;

			if (_stati64(pStr->GetRawData(), &sbuf) == -1) 
				return false;

			return ((sbuf.st_mode & _S_IFDIR) != 0);
		}

		LBool Win32Core::Rename(const String *sSource, const String *sDest)
		{
			if(!sSource|| !sDest || sSource->IsEmpty() || !sDest->IsEmpty())
				return LFalse;

			PDO::BaseString::Win32Core *pSrc = (PDO::BaseString::Win32Core *)sSource;
			PDO::BaseString::Win32Core *pDest = (PDO::BaseString::Win32Core *)sDest;

			return (rename(pSrc->GetRawData(), pDest->GetRawData()) == 0) ? LTrue : LFalse;
		}

		LBool Win32Core::Move(const String *sSource, const String *sDest)
		{
			if(!sSource|| !sDest || sSource->IsEmpty() || !sDest->IsEmpty())
				return LFalse;

			PDO::BaseString::Win32Core *pSrc = (PDO::BaseString::Win32Core *)sSource;
			PDO::BaseString::Win32Core *pDest = (PDO::BaseString::Win32Core *)sDest;

			return (MoveFile(pSrc->GetRawData(), pDest->GetRawData()) == 0) ? LFalse : LTrue;
		}

		//LDate Win32Core::GetLastModified(const String *sSource);	// TODO: LDate type...

		LBool Win32Core::MakeDir(const String *sFileName)
		{
			if(!sFileName || sFileName->IsEmpty())
				return LFalse;

			PDO::BaseString::Win32Core *pStr = (PDO::BaseString::Win32Core *)sFileName;

			return (mkdir(pStr->GetRawData()) == 0) ? LTrue : LFalse;			
		}

		LBool Win32Core::SetAttributes(const String *sFileName, const LInt iAttributes)
		{
			return LFalse; // Todo
		}

		String *Core::GetLESSAPath()
		{
			char szTemp[_MAX_PATH];
			GetCurrentDirectory(_MAX_PATH, szTemp);

			MakeString(sRetPath);
			*sRetPath = szTemp;

			*sRetPath += "\\";

			return sRetPath;
		}

/*
		LVoid	Win32Core::ShowIWork()
		{
			MessageBox(NULL, "Core::ShowIWork is working!", "ShowIWork", MB_OK|MB_ICONEXCLAMATION);
		}

		LBool	System::FileExistsS(LString sFileName)
		{
			return  GetFileAttributes(sFileName) != INVALID_FILE_ATTRIBUTES;
		}*/
	}
}

#endif // COMPILE_FOR_WINDOWS