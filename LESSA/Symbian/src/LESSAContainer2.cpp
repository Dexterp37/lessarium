/*
============================================================================
 Name        : CLESSAContainer2 from LESSAContainer2.cpp
 Author      : 
 Version     :
 Copyright   : Your copyright notice
 Description : Container control implementation
============================================================================
*/

// INCLUDE FILES
#include "LESSAContainer2.h"

#include <eiklabel.h>  // for example label control


// ================= MEMBER FUNCTIONS =======================

// ---------------------------------------------------------
// CLESSAContainer2::ConstructL(const TRect& aRect)
// EPOC two phased constructor
// ---------------------------------------------------------
//
void CLESSAContainer2::ConstructL(const TRect& aRect)
    {
    CreateWindowL();

    iLabel = new (ELeave) CEikLabel;
    iLabel->SetContainerWindowL( *this );
    iLabel->SetTextL( _L("Example View 2") );

    iToDoLabel = new (ELeave) CEikLabel;
    iToDoLabel->SetContainerWindowL( *this );
    iToDoLabel->SetTextL( _L("Add Your controls\n here") );

    SetRect(aRect);
    ActivateL();
    }

// Destructor
CLESSAContainer2::~CLESSAContainer2()
    {
    delete iLabel;
    delete iToDoLabel;
    }

// ---------------------------------------------------------
// CLESSAContainer2::SizeChanged()
// Called by framework when the view size is changed
// ---------------------------------------------------------
//
void CLESSAContainer2::SizeChanged()
    {
    // TODO: Add here control resize code etc.
    iLabel->SetExtent( TPoint(10,10), iLabel->MinimumSize() );
    iToDoLabel->SetExtent( TPoint(10,100), iToDoLabel->MinimumSize() );
    }

// ---------------------------------------------------------
// CLESSAContainer2::CountComponentControls() const
// ---------------------------------------------------------
//
TInt CLESSAContainer2::CountComponentControls() const
    {
    return 2; // return nbr of controls inside this container
    }

// ---------------------------------------------------------
// CLESSAContainer2::ComponentControl(TInt aIndex) const
// ---------------------------------------------------------
//
CCoeControl* CLESSAContainer2::ComponentControl(TInt aIndex) const
    {
    switch ( aIndex )
        {
        case 0:
            return iLabel;
        case 1:
            return iToDoLabel;
        default:
            return NULL;
        }
    }

// ---------------------------------------------------------
// CLESSAContainer2::Draw(const TRect& aRect) const
// ---------------------------------------------------------
//
void CLESSAContainer2::Draw(const TRect& aRect) const
    {
    CWindowGc& gc = SystemGc();
    // TODO: Add your drawing code here
    // example code...
    gc.SetPenStyle( CGraphicsContext::ENullPen );
    gc.SetBrushColor( KRgbGray );
    gc.SetBrushStyle( CGraphicsContext::ESolidBrush );
    gc.DrawRect( aRect );
    }

// ---------------------------------------------------------
// CLESSAContainer2::HandleControlEventL(
//     CCoeControl* aControl,TCoeEvent aEventType)
// ---------------------------------------------------------
//
void CLESSAContainer2::HandleControlEventL(
    CCoeControl* /*aControl*/,TCoeEvent /*aEventType*/)
    {
    // TODO: Add your control event handler code here
    }


  
