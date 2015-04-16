/*
============================================================================
 Name        : CLESSAView2 from LESSAView2.h
 Author      : 
 Version     :
 Copyright   : Your copyright notice
 Description : CLESSAView2 implementation
============================================================================
*/

// INCLUDE FILES
#include  <aknviewappui.h>
#include  <avkon.hrh>
#include  <LESSA.rsg>
#include  "LESSAView2.h"
#include  "LESSAContainer2.h" 

// ================= MEMBER FUNCTIONS =======================

// ---------------------------------------------------------
// CLESSAView2::ConstructL(const TRect& aRect)
// EPOC two-phased constructor
// ---------------------------------------------------------
//
void CLESSAView2::ConstructL()
    {
    BaseConstructL( R_LESSA_VIEW2 );
    }

// ---------------------------------------------------------
// CLESSAView2::~CLESSAView2()
// destructor
// ---------------------------------------------------------
//
CLESSAView2::~CLESSAView2()
    {
    if ( iContainer )
        {
        AppUi()->RemoveFromViewStack( *this, iContainer );
        }

    delete iContainer;
    }

// ---------------------------------------------------------
// TUid CLESSAView2::Id()
// 
// ---------------------------------------------------------
//
TUid CLESSAView2::Id() const
    {
    return KView2Id;
    }

// ---------------------------------------------------------
// CLESSAView2::HandleCommandL(TInt aCommand)
// takes care of view command handling
// ---------------------------------------------------------
//
void CLESSAView2::HandleCommandL(TInt aCommand)
    {   
    switch ( aCommand )
        {
        case EAknSoftkeyOk:
            {
            iEikonEnv->InfoMsg( _L("view2 ok") );
            break;
            }
        case EAknSoftkeyBack:
            {
            AppUi()->HandleCommandL(EEikCmdExit);
            break;
            }
        default:
            {
            AppUi()->HandleCommandL( aCommand );
            break;
            }
        }
    }

// ---------------------------------------------------------
// CLESSAView2::HandleClientRectChange()
// ---------------------------------------------------------
//
void CLESSAView2::HandleClientRectChange()
    {
    if ( iContainer )
        {
        iContainer->SetRect( ClientRect() );
        }
    }

// ---------------------------------------------------------
// CLESSAView2::DoActivateL(...)
// 
// ---------------------------------------------------------
//
void CLESSAView2::DoActivateL(
   const TVwsViewId& /*aPrevViewId*/,TUid /*aCustomMessageId*/,
   const TDesC8& /*aCustomMessage*/)
    {
    if (!iContainer)
        {
        iContainer = new (ELeave) CLESSAContainer2;
        iContainer->SetMopParent(this);
        iContainer->ConstructL( ClientRect() );
        AppUi()->AddToStackL( *this, iContainer );
        }
   }

// ---------------------------------------------------------
// CLESSAView2::DoDeactivate()
// 
// ---------------------------------------------------------
//
void CLESSAView2::DoDeactivate()
    {
    if ( iContainer )
        {
        AppUi()->RemoveFromViewStack( *this, iContainer );
        }
    
    delete iContainer;
    iContainer = NULL;
    }



