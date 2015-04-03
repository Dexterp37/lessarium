/**
	License: zlib/libpng
	Author: Alessio Pierluigi Placitelli
	Company:

	Description: This class holds Symbian FSS core functionalities implementation

	Revision:
		04/04/2006: Exists and 

	Todo:
*/

#ifdef COMPILE_FOR_SYMBIAN

#include "PDO/FSS/SymbianfsCore.h"
#include "PDO/String/SymbianstCore.h"

//#include <aknnotewrappers.h>	// To use CAknInformationNote
#include <coeutils.h>
#include <f32file.h>
#include <bautils.h> 
#include <eikenv.h> 
#include <eikappui.h> 
#include <eikapp.h>

namespace PDO
{
	namespace FSS
	{
		Core * Core::createCore()
		{
			return new SymbianCore();
		}

		LBool SymbianCore::Exists(const String *sFileName)
		{
			if(!sFileName || sFileName->IsEmpty())
				return LFalse;

			PDO::BaseString::SymbianCore *pStr = (PDO::BaseString::SymbianCore *)sFileName;

			// Dexter: we don't have to check for NULL pointers since IsEmpty() does that.
			return ConeUtils::FileExists(*(pStr->GetRawData()));
		}
/*
		LInt  SymbianCore::GetAttributes(LString sFileName)
		{
			LUint iAttrBits;

			if(CEikonEnv::Static()->FsSession().Att(*sFileName, iAttrBits) != KErrNone)
				return 0;

			return 0;	// Todo: 
		}
*/

		LBool SymbianCore::IsFile(const String *sFileName)
		{
			if(!sFileName || sFileName->IsEmpty())
				return LFalse;

			PDO::BaseString::SymbianCore *pStr = (PDO::BaseString::SymbianCore *)sFileName;

			LBool bResult = LFalse;

			// Todo: Implement exceptions?
			if(BaflUtils::IsFolder(CEikonEnv::Static()->FsSession(), *(pStr->GetRawData()), bResult) != KErrNone)
				return LFalse;

			return !bResult;
		}

		LBool SymbianCore::IsDirectory(const String *sFileName)
		{
			if(!sFileName || sFileName->IsEmpty())
				return LFalse;

			PDO::BaseString::SymbianCore *pStr = (PDO::BaseString::SymbianCore *)sFileName;

			LBool bResult = LFalse;

			// Todo: Implement exceptions?
			if(BaflUtils::IsFolder(CEikonEnv::Static()->FsSession(), *(pStr->GetRawData()), bResult) != KErrNone)
				return LFalse;

			return bResult;
		}

		LBool SymbianCore::Rename(const String *sSource, const String *sDest)
		{
			if(!sSource|| !sDest || sSource->IsEmpty() || !sDest->IsEmpty())
				return LFalse;

			PDO::BaseString::SymbianCore *pSrc = (PDO::BaseString::SymbianCore *)sSource;
			PDO::BaseString::SymbianCore *pDest = (PDO::BaseString::SymbianCore *)sDest;

			// Todo: use IsValidName 
			return (CEikonEnv::Static()->FsSession().Rename(*(pSrc->GetRawData()), *(pDest->GetRawData())) == KErrNone ) ? LTrue : LFalse;
		}

		LBool SymbianCore::Move(const String *sSource, const String *sDest)
		{
			// Todo: is Move a specialized Rename?
			if(!sSource|| !sDest || sSource->IsEmpty() || !sDest->IsEmpty())
				return LFalse;

			PDO::BaseString::SymbianCore *pSrc = (PDO::BaseString::SymbianCore *)sSource;
			PDO::BaseString::SymbianCore *pDest = (PDO::BaseString::SymbianCore *)sDest;

			// Todo: use IsValidName 
			return (CEikonEnv::Static()->FsSession().Rename(*(pSrc->GetRawData()), *(pDest->GetRawData())) == KErrNone ) ? LTrue : LFalse;
		}

		//LDate SymbianCore::GetLastModified(const String *sSource);	// TODO: LDate type...

		LBool SymbianCore::MakeDir(const String *sFileName)
		{
			if(!sFileName || sFileName->IsEmpty())
				return LFalse;

			PDO::BaseString::SymbianCore *pStr = (PDO::BaseString::SymbianCore *)sFileName;

			TRAPD(err, ConeUtils::EnsurePathExistsL(*(pStr->GetRawData())));
			
			// Todo: error logging?
			return (err == KErrNone);
		}

		LBool SymbianCore::SetAttributes(const String *sFileName, const LInt iAttributes)
		{
			return LFalse; // Todo
		}

		String *Core::GetLESSAPath() 
		{
			MakeString(retPath);
			TFileName syAppName = BaflUtils::DriveAndPathFromFullName(CEikonEnv::Static()->EikAppUi()->Application()->AppFullName());
			
			((PDO::BaseString::SymbianCore*)retPath)->SetFromTDes(syAppName);

			return retPath;
		}

/*
		// Test
		LVoid	SymbianCore::ShowIWork()
		{
			TBuf<0x100> buf;
			buf.Format( _L("Core::ShowIWork is working!"));

			CAknInformationNote* informationNote;
            informationNote = new (ELeave) CAknInformationNote;
            informationNote->ExecuteLD(buf);

		}*/
	}
}

#endif // COMPILE_FOR_SYMBIAN