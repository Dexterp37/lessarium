/*
============================================================================
 Name        : CLESSAApp from LESSAApp.cpp
 Author      : 
 Version     :
 Copyright   : Your copyright notice
 Description : Main application class
============================================================================
*/

// INCLUDE FILES
#include    "LESSAApp.h"
#include    "LESSADocument.h"

// ================= MEMBER FUNCTIONS =======================

// ---------------------------------------------------------
// CLESSAApp::AppDllUid()
// Returns application UID
// ---------------------------------------------------------
//
TUid CLESSAApp::AppDllUid() const
    {
    return KUidLESSA;
    }

   
// ---------------------------------------------------------
// CLESSAApp::CreateDocumentL()
// Creates CLESSADocument object
// ---------------------------------------------------------
//
CApaDocument* CLESSAApp::CreateDocumentL()
    {
    return CLESSADocument::NewL( *this );
    }

// ================= OTHER EXPORTED FUNCTIONS ==============
//
// ---------------------------------------------------------
// NewApplication() 
// Constructs CLESSAApp
// Returns: created application object
// ---------------------------------------------------------
//
EXPORT_C CApaApplication* NewApplication()
    {
    return new CLESSAApp;
    }

// ---------------------------------------------------------
// E32Dll(TDllReason) 
// Entry point function for EPOC Apps
// Returns: KErrNone: No error
// ---------------------------------------------------------
//
GLDEF_C TInt E32Dll( TDllReason )
    {
    return KErrNone;
    }

  

