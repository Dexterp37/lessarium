/*
============================================================================
 Name        : CLESSAAppUi from LESSAAppui.cpp
 Author      : 
 Version     :
 Copyright   : Your copyright notice
 Description : CLESSAAppUi implementation
============================================================================
*/

// INCLUDE FILES
#include "LESSAAppui.h"
#include "LESSAView.h"
#include "LESSAView2.h"
#include <LESSA.rsg>
#include "LESSA.hrh"

#include <avkon.hrh>

// ================= MEMBER FUNCTIONS =======================
//
// ----------------------------------------------------------
// CLESSAAppUi::ConstructL()
// 
// ----------------------------------------------------------
//
void CLESSAAppUi::ConstructL()
    {
    BaseConstructL();


    // Show tabs for main views from resources
    CEikStatusPane* sp = StatusPane();

    // Fetch pointer to the default navi pane control
    iNaviPane = (CAknNavigationControlContainer*)sp->ControlL( 
        TUid::Uid(EEikStatusPaneUidNavi));

    // Tabgroup has been read from resource and it were pushed to the navi pane. 
    // Get pointer to the navigation decorator with the ResourceDecorator() function. 
    // Application owns the decorator and it has responsibility to delete the object.
    iDecoratedTabGroup = iNaviPane->ResourceDecorator();
    if (iDecoratedTabGroup)
        {
        iTabGroup = (CAknTabGroup*) iDecoratedTabGroup->DecoratedControl();
		}

    CLESSAView* view1 = new (ELeave) CLESSAView;

    CleanupStack::PushL( view1 );
    view1->ConstructL();
    AddViewL( view1 );      // transfer ownership to CAknViewAppUi
    CleanupStack::Pop();    // view1

    CLESSAView2* view2 = new (ELeave) CLESSAView2;

    CleanupStack::PushL( view2 );
    view2->ConstructL();
    AddViewL( view2 );      // transfer ownership to CAknViewAppUi
    CleanupStack::Pop();    // view2

    SetDefaultViewL(*view1);

    }

// ----------------------------------------------------
// CLESSAAppUi::~CLESSAAppUi()
// Destructor
// Frees reserved resources
// ----------------------------------------------------
//
CLESSAAppUi::~CLESSAAppUi()
    {
    delete iDecoratedTabGroup;
   }

// ------------------------------------------------------------------------------
// CLESSAAppUi::DynInitMenuPaneL(TInt aResourceId,CEikMenuPane* aMenuPane)
//  This function is called by the EIKON framework just before it displays
//  a menu pane. Its default implementation is empty, and by overriding it,
//  the application can set the state of menu items dynamically according
//  to the state of application data.
// ------------------------------------------------------------------------------
//
void CLESSAAppUi::DynInitMenuPaneL(
    TInt /*aResourceId*/,CEikMenuPane* /*aMenuPane*/)
    {
    }

// ----------------------------------------------------
// CLESSAAppUi::HandleKeyEventL(
//     const TKeyEvent& aKeyEvent,TEventCode /*aType*/)
// takes care of key event handling
// ----------------------------------------------------
//
TKeyResponse CLESSAAppUi::HandleKeyEventL(
    const TKeyEvent& aKeyEvent,TEventCode aType)
    {
    if ( iTabGroup == NULL )
        {
        return EKeyWasNotConsumed;
        }

    TInt active = iTabGroup->ActiveTabIndex();
    TInt count = iTabGroup->TabCount();

    switch ( aKeyEvent.iCode )
        {
        case EKeyLeftArrow:
            if ( active > 0 )
                {
                active--;
                iTabGroup->SetActiveTabByIndex( active );
                ActivateLocalViewL(TUid::Uid(iTabGroup->TabIdFromIndex(active)));
                }
            break;
        case EKeyRightArrow:
            if( (active + 1) < count )
                {
                active++;
                iTabGroup->SetActiveTabByIndex( active );
                ActivateLocalViewL(TUid::Uid(iTabGroup->TabIdFromIndex(active)));
                }
            break;
        default:
            return EKeyWasNotConsumed;
            break;
        }

    return EKeyWasConsumed;
	}

// ----------------------------------------------------
// CLESSAAppUi::HandleCommandL(TInt aCommand)
// takes care of command handling
// ----------------------------------------------------
//
void CLESSAAppUi::HandleCommandL(TInt aCommand)
    {
    switch ( aCommand )
        {
        case EEikCmdExit:
            {
            Exit();
            break;
            }
        case ELESSACmdAppTest:
            {
            iEikonEnv->InfoMsg(_L("test"));
            break;
            }
        // TODO: Add Your command handling code here

        default:
            break;      
        }
    }

