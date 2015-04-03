// LESSA.cpp : Defines the entry point for the application.
//

#include "stdafx.h"

//#include "config.h"
#include "PDO/FSS/fsCore.h"			// PDO::FSS
#include "PDO/FSS/File/foCore.h"	// PDO::FSS::File
#include "PDO/String/stCore.h"		// PDO::FSS::String aka String
#include "PDO/NS/nsCore.h"			// PDO::NS
#include "PIO/XML/xmCore.h"			// PIO::XML::Core
#include "PIO/AR/arCore.h"			// PIO::AR::Core
#include "PIO/VC/vcCore.h"			// PIO::VC::Core

PDO::NS::Core *pNS = LNull;			// We need it global to test

#define MAX_LOADSTRING 100
/*
LInt StartElementHandler(const String *pTagName, const PIO::XML::Parser::STOXSTATE *pPaBrserState, LVoid *pUserData)
{

	return 1;
}

LInt AttributeNameHandler(const String *pAttrName, const PIO::XML::Parser::STOXSTATE *pParserState, LVoid *pUserData)
{

	return 1;
}

LInt AttributeValueHandler(const String *pValue, const PIO::XML::Parser::STOXSTATE *pParserState, LVoid *pUserData)
{

	return 1;
}

LInt WordEndHandler(const String *pWordEnd, const PIO::XML::Parser::STOXSTATE *pParserState, LVoid *pUserData)
{

	return 1;
}

LInt WhiteSpaceHandler(const String *pWS, const PIO::XML::Parser::STOXSTATE *pParserState, LVoid *pUserData)
{

	return 1;
}

LInt EntityHandler(const String *pEntity, const PIO::XML::Parser::STOXSTATE *pParserState, LVoid *pUserData)
{

	return 1;
}
*/
// Global Variables:
HINSTANCE hInst;								// current instance
TCHAR szTitle[MAX_LOADSTRING];					// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];			// the main window class name

// Forward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
 	// TODO: Place code here.
	MSG msg;
	HACCEL hAccelTable;

	// Initialize global strings
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_LESSA, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance (hInstance, nCmdShow)) 
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, (LPCTSTR)IDC_LESSA);

	// Main message loop:
	while (GetMessage(&msg, NULL, 0, 0)) 
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg)) 
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int) msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
//  COMMENTS:
//
//    This function and its usage are only necessary if you want this code
//    to be compatible with Win32 systems prior to the 'RegisterClassEx'
//    function that was added to Windows 95. It is important to call this function
//    so that the application will get 'well formed' small icons associated
//    with it.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX); 

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= (WNDPROC)WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, (LPCTSTR)IDI_LESSA);
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= (LPCTSTR)IDC_LESSA;
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, (LPCTSTR)IDI_SMALL);

	return RegisterClassEx(&wcex);
}

//
//   FUNCTION: InitInstance(HANDLE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd;

   hInst = hInstance; // Store instance handle in our global variable

   hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 400, CW_USEDEFAULT, 200, NULL, NULL, hInstance, NULL);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);
/*
    // Platform independent code	(String Test)
   //PDO::BaseString::Core *pstrTemp = PDO::BaseString::Core::createCore();	// PDO::BaseString::Core is typedef'd to String for semplicity
   String *pstrTemp = String::createCore();
   if(pstrTemp)
   {
	*pstrTemp = "string:I'm a test string. ";
	*pstrTemp += "Today, I'm a test string AGAIN";

	pstrTemp->Replace("string", "idiot");

	delete pstrTemp;
   }
   // End platform independent code
/*
   // Platform independent code
   PDO::FSS::Core *pFS = PDO::FSS::Core::createCore();
   if(pFS)
   {
	   // Get application path
	   String *pRealPath = PDO::FSS::Core::GetLESSAPath();

	   // Stupid test: check if application path exists :D
	   LBool bRes = pFS->Exists(pRealPath);

 	   *pRealPath += "lessa_tst.txt";

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
   // End platform independent code
*//*
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
   }
*/
/*
   // AR Core test
   PIO::AR::Core *pAR = new PIO::AR::Core();
   if(pAR)
   {
		LBool bRes = pAR->SkimRead();

		delete pAR;
   }
*/

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

   // NS Core test
   pNS = PDO::NS::Core::createCore();
   if(pNS)
   {
	    // Resource 1
		MakeString(strRes1);
		*strRes1 = "http://www.giuseppenapolitano.it/files/nulla2.jpg";

	    String *pLocalRes1 = PDO::FSS::Core::GetLESSAPath();
 	    *pLocalRes1 += "nulla_dl.jpg";

	    // Resource 2
		MakeString(strRes2);
		*strRes2 = "http://www.giuseppenapolitano.it/files/ep_090403.jpg";

	    String *pLocalRes2 = PDO::FSS::Core::GetLESSAPath();
 	    *pLocalRes2 += "esqape_dl.jpg";

	    // Resource 3
		MakeString(strRes3);
		*strRes3 = "http://www.giuseppenapolitano.it/files/banco_dexter.jpg";

	    String *pLocalRes3 = PDO::FSS::Core::GetLESSAPath();
 	    *pLocalRes3 += "banco_dl.jpg";

		pNS->RequestResource(strRes1, pLocalRes1);
		pNS->RequestResource(strRes2, pLocalRes2);
		pNS->RequestResource(strRes3, pLocalRes3);

		delete strRes1, strRes2, strRes3;
		delete pLocalRes1, pLocalRes2, pLocalRes3;
   }

   return TRUE;
}

//
//  FUNCTION: WndProc(HWND, unsigned, WORD, LONG)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND	- process the application menu
//  WM_PAINT	- Paint the main window
//  WM_DESTROY	- post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;

	switch (message) 
	{
	case WM_COMMAND:
		wmId    = LOWORD(wParam); 
		wmEvent = HIWORD(wParam); 
		// Parse the menu selections:
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(hInst, (LPCTSTR)IDD_ABOUTBOX, hWnd, (DLGPROC)About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		// TODO: Add any drawing code here...
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		{
			delete pNS;
			pNS = LNull;

			PostQuitMessage(0);
		}break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// Message handler for about box.
LRESULT CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_INITDIALOG:
		return TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL) 
		{
			EndDialog(hDlg, LOWORD(wParam));
			return TRUE;
		}
		break;
	}
	return FALSE;
}
