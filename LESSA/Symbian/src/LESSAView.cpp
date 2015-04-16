/*
============================================================================
 Name        : CLESSAView from LESSAView.h
 Author      : 
 Version     :
 Copyright   : Your copyright notice
 Description : CLESSAView implementation
============================================================================
*/

// INCLUDE FILES
#include  <aknviewappui.h>
#include  <avkon.hrh>
#include  <LESSA.rsg>
#include  "LESSAView.h"
#include  "LESSAContainer.h" 


// ================= MEMBER FUNCTIONS =======================

// ---------------------------------------------------------
// CLESSAView::ConstructL(const TRect& aRect)
// EPOC two-phased constructor
// ---------------------------------------------------------
//
void CLESSAView::ConstructL()
    {
    BaseConstructL( R_LESSA_VIEW1 );
    }

// ---------------------------------------------------------
// CLESSAView::~CLESSAView()
// destructor
// ---------------------------------------------------------
//
CLESSAView::~CLESSAView()
    {
    if ( iContainer )
        {
        AppUi()->RemoveFromViewStack( *this, iContainer );
        }

    delete iContainer;
    }

// ---------------------------------------------------------
// TUid CLESSAView::Id()
//
// ---------------------------------------------------------
//
TUid CLESSAView::Id() const
    {
    return KViewId;
    }

// ---------------------------------------------------------
// CLESSAView::HandleCommandL(TInt aCommand)
// takes care of view command handling
// ---------------------------------------------------------
//
void CLESSAView::HandleCommandL(TInt aCommand)
    {   
    switch ( aCommand )
        {
        case EAknSoftkeyOk:
            {
            iEikonEnv->InfoMsg( _L("view1 ok") );

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
// CLESSAView::HandleClientRectChange()
// ---------------------------------------------------------
//
void CLESSAView::HandleClientRectChange()
    {
    if ( iContainer )
        {
        iContainer->SetRect( ClientRect() );
        }
    }

// ---------------------------------------------------------
// CLESSAView::DoActivateL(...)
// 
// ---------------------------------------------------------
//
void CLESSAView::DoActivateL(
   const TVwsViewId& /*aPrevViewId*/,TUid /*aCustomMessageId*/,
   const TDesC8& /*aCustomMessage*/)
    {
    if (!iContainer)
        {
        iContainer = new (ELeave) CLESSAContainer;
        iContainer->SetMopParent(this);
        iContainer->ConstructL( ClientRect() );
        AppUi()->AddToStackL( *this, iContainer );
        } 
   }

// ---------------------------------------------------------
// CLESSAView::DoDeactivate()
// 
// ---------------------------------------------------------
//
void CLESSAView::DoDeactivate()
    {
    if ( iContainer )
        {
        AppUi()->RemoveFromViewStack( *this, iContainer );
        }
    
    delete iContainer;
    iContainer = NULL;
    }



