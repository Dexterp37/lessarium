/*
============================================================================
 Name        : CLESSAApp from LESSAApp.h
 Author      : 
 Version     :
 Copyright   : Your copyright notice
 Description : Declares main application class.
============================================================================
*/

#ifndef LESSAAPP_H
#define LESSAAPP_H

// INCLUDES
#include <aknapp.h>

// CONSTANTS
// UID of the application
const TUid KUidLESSA = { 0x05926b6a };

// CLASS DECLARATION

/**
* CLESSAApp application class.
* Provides factory to create concrete document object.
* 
*/
class CLESSAApp : public CAknApplication
    {
    
    public: // Functions from base classes
    private:

        /**
        * From CApaApplication, creates CLESSADocument document object.
        * @return A pointer to the created document object.
        */
        CApaDocument* CreateDocumentL();
        
        /**
        * From CApaApplication, returns application's UID (KUidLESSA).
        * @return The value of KUidLESSA.
        */
        TUid AppDllUid() const;
    };

#endif



