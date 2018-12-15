// winCE.cpp : Defines the entry point for the application.
//

#include "resource.h"

#include "../inc/EntryPtr.h"

#include <shellsdk.h>
#pragma comment(lib, "aygshell.lib")


#include "pm.h"

#define _SCR_RESOLUTION_WIDTH	SCR_RESOLUTION_WIDTH
#define _SCR_RESOLUTION_HEIGHT	SCR_RESOLUTION_HEIGHT
//#define _SCR_RESOLUTION_WIDTH		320
//#define _SCR_RESOLUTION_HEIGHT	320

#define MAX_LOADSTRING 100

// Global Variables:
static HINSTANCE hInst;								// current instance

static TCHAR szTitle[256];				// The title bar text
static TCHAR szWindowClass[MAX_LOADSTRING] = _T("BUP_Launcher");								// The title bar text

//TCHAR szTitle[MAX_LOADSTRING];								// The title bar text
//TCHAR szWindowClass[MAX_LOADSTRING];								// The title bar text

static int g_screen_width, g_screen_height;

// Foward declarations of functions included in this code module:
static ATOM				MyRegisterClass(HINSTANCE hInstance);
static BOOL				InitInstance(HINSTANCE, int);
static LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);


//////////////////////////////////////////////////////////////////////////

static void* g_iPwrReq = 0;
static void BrightTurnOn ()
{
	g_iPwrReq = SetPowerRequirement(TEXT("BKL1:"), D0, POWER_NAME, NULL, 0);
}

static void BrightTurnOff()
{
	ReleasePowerRequirement(g_iPwrReq);
}


int WINAPI WinMain(	HINSTANCE hInstance,
					HINSTANCE hPrevInstance,
					LPTSTR    lpCmdLine,
					int       nCmdShow)
{
	HWND  hWnd = FindWindow(szWindowClass, szTitle);
    if( hWnd ){
   		SHFullScreen( hWnd, SHFS_HIDETASKBAR | SHFS_HIDESIPBUTTON | SHFS_HIDESTARTICON );
		//ShowWindow( hWnd, SW_SHOW );
		SetForegroundWindow( hWnd );
		//BringWindowToTop( hWnd );
		//SetActiveWindow( hWnd );
		PostQuitMessage(0);
        return FALSE;
    }

/*	
	//RegisterNavixWindowMsg();

	if(wcsstr (lpCmdLine, _T("dev_run")) )
	{
		// 기존에 실행되고 있다면 종료시킨다.
		HWND  hWnd = ::FindWindow(_T(APP_CLASS), 0);
		if (hWnd)
		{
			SendMessage (hWnd, WM_CLOSE,0,0);
#if (_LOG_DEBUG_)
			__LOG(DZ_TEMP, "Exit running tornado...");
#endif
			return 1;
		}
	}
*/

	// TODO: Place code here.
	MSG msg;
	HACCEL hAccelTable;

	// Initialize global strings
//	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
//	LoadString(hInstance, IDC_AEE_TEST, szWindowClass, MAX_LOADSTRING);
//_stprintf(&szTitle,_T("TEST_1"));
//_stprintf(&szWindowClass,_T("AEE_TEST"));

	//__LOG (DZ_TEMP, "stating");

	MyRegisterClass(hInstance);
	
	//__LOG (DZ_TEMP, "done MyRegisterClass");
	// Perform application initialization:
	if (!InitInstance (hInstance, nCmdShow)) 
	{
		return FALSE;
	}
	//__LOG (DZ_TEMP, "done InitInstance");

	hAccelTable = LoadAccelerators(hInstance, (LPCTSTR)IDC_BUP_LAUNCHER);

	// Main message loop:
	while (GetMessage(&msg, NULL, 0, 0)) 
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg)) 
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
	
	return msg.wParam;
	//return 0;
}


//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
//  COMMENTS:
//
//    This function and its usage is only necessary if you want this code
//    to be compatible with Win32 systems prior to the 'RegisterClassEx'
//    function that was added to Windows 95. It is important to call this function
//    so that the application will get 'well formed' small icons associated
//    with it.
//
static ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASS	wc;

    wc.style			= CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc		= (WNDPROC) WndProc;
    wc.cbClsExtra		= 0;
    wc.cbWndExtra		= 0;
    wc.hInstance		= hInstance;
    wc.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_BUP_LAUNCHER));
    wc.hCursor			= LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground	= (HBRUSH) GetStockObject(WHITE_BRUSH);
    wc.lpszMenuName		= 0;
    wc.lpszClassName	= szWindowClass;

	return RegisterClass(&wc);
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
static BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	HWND hWnd;

	hInst = hInstance; // Store instance handle in our global variable

#ifdef __WCE_EMULATOR_
	RECT rc = {0, 0, 240, 320};
	//AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, TRUE);
	g_screen_width = rc.right-rc.left;
	g_screen_height = rc.bottom-rc.top;
#else
	g_screen_width = GetSystemMetrics(SM_CXSCREEN);
	g_screen_height = GetSystemMetrics(SM_CYSCREEN);
#endif

	hWnd = CreateWindow(szWindowClass, szTitle, 
   						WS_VISIBLE,
						0, 0, g_screen_width, g_screen_height,
      					HWND_DESKTOP, NULL, hInstance, NULL);

   if (!hWnd) return FALSE;

//   ShowWindow(hWnd, /*nCmdShow*/ SW_MAXIMIZE);
   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

#ifndef __WCE_EMULATOR_
   SHFullScreen( hWnd, SHFS_HIDETASKBAR | SHFS_HIDESIPBUTTON | SHFS_HIDESTARTICON );
#endif

   SetForegroundWindow( hWnd );

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
static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;

	switch (message) 
	{
	case WM_CREATE:
		{
			BrightTurnOn ();

			void *appui = (void *)hInst;
			void *appview = (void *)hWnd;

//			InitBUP( g_screen_width, g_screen_height, appview, appui );
			if( !ENTRYPTR_Create( appui, appview, g_screen_width, g_screen_height ) )
			{
				PostMessage(hWnd, WM_CLOSE, 0,0);
			}

			SetFocus( hWnd );
		}
		break;

	case WM_SHOWWINDOW:
		ENTRYPTR_Enter();
		break;

	case WM_CLOSE:
		ENTRYPTR_Exit();
		DestroyWindow(hWnd);
	break;

	case WM_DESTROY:
		BrightTurnOff ();

		ENTRYPTR_Destroy();
		PostQuitMessage(0);
		break;

	case WM_PAINT:
		BeginPaint( hWnd, &ps );
		ENTRYPTR_Draw();
		EndPaint( hWnd, &ps );
		break;

	case WM_SIZE:
		{
			RECT rect;
			GetClientRect(hWnd, &rect);
//			atUIENG_OnResize( rect.right-rect.left, rect.bottom-rect.top ); // 임시로 막음.	
			ENTRYPTR_Resize( rect.right-rect.left, rect.bottom-rect.top );
		}
		break;

	default:
		//	atAPP_PostMessage(atAPPMNG_GetCurApp(), hWnd, message, wParam, lParam);
		return DefWindowProc(hWnd, message, wParam, lParam);
   }

   return 0;
}
