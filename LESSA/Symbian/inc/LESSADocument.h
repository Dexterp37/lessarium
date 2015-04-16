/*
============================================================================
 Name        : CLESSADocument from LESSADocument.h
 Author      : 
 Version     :
 Copyright   : Your copyright notice
 Description : Declares document for application.
============================================================================
*/

#ifndef LESSADOCUMENT_H
#define LESSADOCUMENT_H

// INCLUDES
#include <akndoc.h>
   
// CONSTANTS

// FORWARD DECLARATIONS
class  CEikAppUi;

// CLASS DECLARATION

/**
*  CLESSADocument application class.
*/
class CLESSADocument : public CAknDocument
    {
    public: // Constructors and destructor
        /**
        * Two-phased constructor.
        */
        static CLESSADocument* NewL(CEikApplication& aApp);

        /**
        * Destructor.
        */
        virtual ~CLESSADocument();

    public: // New functions

    public: // Functions from base classes
    protected:  // New functions

    protected:  // Functions from base classes

    private:

        /**
        * EPOC default constructor.
        */
        CLESSADocument(CEikApplication& aApp);
        void ConstructL();

    private:

        /**
        * From CEikDocument, create CLESSAAppUi "App UI" object.
        */
        CEikAppUi* CreateAppUiL();
    };

#endif



