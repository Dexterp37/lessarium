/*
============================================================================
 Name        : CLESSADocument from LESSADocument.h
 Author      : 
 Version     :
 Copyright   : Your copyright notice
 Description : CLESSADocument implementation
============================================================================
*/

// INCLUDE FILES
#include "LESSADocument.h"
#include "LESSAAppui.h"

// ================= MEMBER FUNCTIONS =======================

// constructor
CLESSADocument::CLESSADocument(CEikApplication& aApp)
: CAknDocument(aApp)    
    {
    }

// destructor
CLESSADocument::~CLESSADocument()
    {
    }

// EPOC default constructor can leave.
void CLESSADocument::ConstructL()
    {
    }

// Two-phased constructor.
CLESSADocument* CLESSADocument::NewL(
        CEikApplication& aApp)     // CLESSAApp reference
    {
    CLESSADocument* self = new (ELeave) CLESSADocument( aApp );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop();

    return self;
    }
    
// ----------------------------------------------------
// CLESSADocument::CreateAppUiL()
// constructs CLESSAAppUi
// ----------------------------------------------------
//
CEikAppUi* CLESSADocument::CreateAppUiL()
    {
    return new (ELeave) CLESSAAppUi;
    }

  
