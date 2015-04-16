/*
============================================================================
 Name        : CLESSAContainer from LESSAContainer.h
 Author      : 
 Version     :
 Copyright   : Your copyright notice
 Description : Container control implementation
============================================================================
*/

// INCLUDE FILES
#include "LESSAContainer.h"

#include <eiklabel.h>  // for example label control
#include <aknnotewrappers.h>	// To use CAknInformationNote
#include  "PDO/NS/nsCore.h"
#include  "PDO/FSS/fsCore.h"
#include  "PDO/String/stCore.h"
#include  "PDO/String/SymbianstCore.h"	// Just for test purposes
#include  "PDO/IPC/ipCore.h"		// PDO::IPC::Core
#include  "PIO/XML/xmCore.h"
#include  "PIO/AR/arCore.h"			// PIO::AR::Core
#include  "PIO/VC/vcCore.h"			// PIO::VC::Core

LInt StartElementHandler(const String *pTagName, const PIO::XML::Parser::STOXSTATE *pParserState)
{
	CAknInformationNote* informationNote;
	informationNote = new (ELeave) CAknInformationNote(LTrue);
	informationNote->ExecuteLD(*(((PDO::BaseString::SymbianCore*)pTagName)->GetRawData()));

	return 1;
}

LInt AttributeNameHandler(const String *pAttrName, const PIO::XML::Parser::STOXSTATE *pParserState)
{
	CAknInformationNote* informationNote;
	informationNote = new (ELeave) CAknInformationNote(LTrue);
	informationNote->ExecuteLD(*(((PDO::BaseString::SymbianCore*)pAttrName)->GetRawData()));

	return 1;
}

LInt AttributeValueHandler(const String *pValue, const PIO::XML::Parser::STOXSTATE *pParserState)
{
	CAknInformationNote* informationNote;
	informationNote = new (ELeave) CAknInformationNote(LTrue);
	informationNote->ExecuteLD(*(((PDO::BaseString::SymbianCore*)pValue)->GetRawData()));

	return 1;
}

LInt WordEndHandler(const String *pWordEnd, const PIO::XML::Parser::STOXSTATE *pParserState)
{
	CAknInformationNote* informationNote;
	informationNote = new (ELeave) CAknInformationNote(LTrue);
	informationNote->ExecuteLD(*(((PDO::BaseString::SymbianCore*)pWordEnd)->GetRawData()));

	return 1;
}

LInt WhiteSpaceHandler(const String *pWS, const PIO::XML::Parser::STOXSTATE *pParserState)
{

	return 1;
}

LInt EntityHandler(const String *pEntity, const PIO::XML::Parser::STOXSTATE *pParserState)
{
	CAknInformationNote* informationNote;
	informationNote = new (ELeave) CAknInformationNote(LTrue);
	informationNote->ExecuteLD(*(((PDO::BaseString::SymbianCore*)pEntity)->GetRawData()));

	return 1;
}

// ================= MEMBER FUNCTIONS =======================

// ---------------------------------------------------------
// CLESSAContainer::ConstructL(const TRect& aRect)
// EPOC two phased constructor
// ---------------------------------------------------------
//
void CLESSAContainer::ConstructL(const TRect& aRect)
{
    CreateWindowL();

    iLabel = new (ELeave) CEikLabel;
    iLabel->SetContainerWindowL( *this );
    iLabel->SetTextL( _L("Example View") );

    iToDoLabel = new (ELeave) CEikLabel;
    iToDoLabel->SetContainerWindowL( *this );
    iToDoLabel->SetTextL( _L("Add Your controls\n here") );

    SetRect(aRect);
    ActivateL();

/*

   // Platform independent code	(String Test)
   PDO::BaseString::Core *pstrTemp = PDO::BaseString::Core::createCore();	// PDO::BaseString::Core is typedef'd to String for semplicity
   if(pstrTemp)
   {
	*pstrTemp = "string:I'm a test string. ";
	*pstrTemp += "Today, I'm a test string AGAIN";

	pstrTemp->Replace("string", "idiot");

	delete pstrTemp;
   }
   // End platform independent code

   // Platform independent code
   PDO::FSS::Core *pFS = PDO::FSS::Core::createCore();
   if(pFS)
   {
	   // Get application path
	   String *pRealPath = PDO::FSS::Core::GetLESSAPath();

	   // Stupid test: check if application path exists :D
	   LBool bRes = pFS->Exists(pRealPath);

 	   *pRealPath += "lessa_tst.txt";

	   	//CAknInformationNote* informationNote;
		//informationNote = new (ELeave) CAknInformationNote(LTrue);
		//informationNote->ExecuteLD(*(((PDO::BaseString::SymbianCore*)pRealPath)->GetRawData()));

	   PDO::FSS::File::Core *pFile = PDO::FSS::File::Core::createCore();
	   if(pFile->Open(pRealPath, LFILE_READ | LFILE_WRITE | LFILE_OVERWRITE_OR_CREATE))
	   {
			pFile->Write(pRealPath); 
			pFile->Close();
	   }

	   delete pRealPath;
	   delete pFile;
	   delete pFS;
   }
   // End platform independent code */
/*
   // XML Settings ... Test
   PIO::XML::Parser::Core *pXML = new PIO::XML::Parser::Core();
   if(pXML)
   {
	   PIO::XML::Parser::SXMLHANDLERS xmlHandlers = { 
		   (PIO::XML::Parser::START_ELEMENT_HANDLER)&StartElementHandler,
			&AttributeNameHandler,
			&AttributeValueHandler,
			&WordEndHandler,
			&WhiteSpaceHandler,
			&EntityHandler
	   };

	   pXML->SetHandlers(&xmlHandlers);

	   // Get application path
	   String *pRealPath = PDO::FSS::Core::GetLESSAPath();
 	   *pRealPath += "soap_rq.xml";

	   LInt iRes = pXML->ParseFile(pRealPath);

	   delete pRealPath;
	   delete pXML;
   }*/

/*
   // AR Core test
   PIO::AR::Core *pAR = new PIO::AR::Core();
   if(pAR)
   {
		LBool bRes = pAR->SkimRead();

		delete pAR;
   }*/
/*
   // VC Core test
   LDate date1, date2;

   date1.m_uYear = 2006;
   date1.m_uMonth = 7;
   date1.m_uDay = 3;
   date1.m_uHour = 12;
   date1.m_uMinute = 30;
   date1.m_uSecond = 25;

   date2.m_uYear = 2006;
   date2.m_uMonth = 7;
   date2.m_uDay = 2;
   date2.m_uHour = 12;
   date2.m_uMinute = 30;
   date2.m_uSecond = 25;

   LBool bMoreRecent = PIO::VC::Core::CheckTimestamps(&date1, &date2);
   if(bMoreRecent)
   {
	// Woha, you won. date1 is more recent than 2
   }
*/
/*
   // NC core test
   pNS = PDO::NS::Core::createCore();
   if(pNS)
   {
	    // Resource 1
		MakeString(strRes1);
		*strRes1 = "http://www.giuseppenapolitano.it/files/nulla2.jpg";

	    String *pLocalRes1 = PDO::FSS::Core::GetLESSAPath();
 	    *pLocalRes1 += "nulla_dl.jpg";

		pNS->RequestResource(strRes1, pLocalRes1);
        
		delete strRes1;
		delete pLocalRes1;
   }*/

   pIPCServer = PDO::IPC::Core::createCore();
   if(pIPCServer)
   {
		LBool bRes = pIPCServer->StartAndListen();	
   }
}

// Destructor
CLESSAContainer::~CLESSAContainer()
    {
    delete iLabel;
    delete iToDoLabel;

	delete pNS;
	pNS = NULL;

	delete pIPCServer;
	pIPCServer = NULL;
    }

// ---------------------------------------------------------
// CLESSAContainer::SizeChanged()
// Called by framework when the view size is changed
// ---------------------------------------------------------
//
void CLESSAContainer::SizeChanged()
    {
    // TODO: Add here control resize code etc.
    iLabel->SetExtent( TPoint(10,10), iLabel->MinimumSize() );
    iToDoLabel->SetExtent( TPoint(10,100), iToDoLabel->MinimumSize() );
    }

// ---------------------------------------------------------
// CLESSAContainer::CountComponentControls() const
// ---------------------------------------------------------
//
TInt CLESSAContainer::CountComponentControls() const
    {
    return 2; // return nbr of controls inside this container
    }

// ---------------------------------------------------------
// CLESSAContainer::ComponentControl(TInt aIndex) const
// ---------------------------------------------------------
//
CCoeControl* CLESSAContainer::ComponentControl(TInt aIndex) const
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
// CLESSAContainer::Draw(const TRect& aRect) const
// ---------------------------------------------------------
//
void CLESSAContainer::Draw(const TRect& aRect) const
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
// CLESSAContainer::HandleControlEventL(
//     CCoeControl* aControl,TCoeEvent aEventType)
// ---------------------------------------------------------
//
void CLESSAContainer::HandleControlEventL(
    CCoeControl* /*aControl*/,TCoeEvent /*aEventType*/)
    {
    // TODO: Add your control event handler code here
    }


  
