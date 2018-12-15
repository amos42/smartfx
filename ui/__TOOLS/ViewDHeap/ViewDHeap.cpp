// AEE_test.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "resource.h"
#include "APP_Entry.h"


#define _SCR_RESOLUTION_WIDTH		320
#define _SCR_RESOLUTION_HEIGHT	400


#define MAX_LOADSTRING 100

// Global Variables:
static HINSTANCE hInst;								// current instance
static TCHAR szTitle[MAX_LOADSTRING];								// The title bar text
static TCHAR szWindowClass[MAX_LOADSTRING];								// The title bar text
static BOOL g_bStayOnTop = TRUE;

// Foward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK	About(HWND, UINT, WPARAM, LPARAM);



static void OnCommand(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	HMENU hMenu;

	switch(wParam)
	{
	case IDM_STAYONTOP:
		hMenu = GetMenu(hWnd);
		g_bStayOnTop = !g_bStayOnTop;
		if( g_bStayOnTop )
			CheckMenuItem( hMenu, IDM_STAYONTOP, MF_CHECKED );
		else
			CheckMenuItem( hMenu, IDM_STAYONTOP, MF_UNCHECKED );
		SetWindowPos(hWnd, (g_bStayOnTop)? HWND_TOPMOST : HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE );
		break;
	}
}



int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR     lpCmdLine,
                     int       nCmdShow)
{
 	// TODO: Place code here.
	MSG msg;
	HACCEL hAccelTable;

	// Initialize global strings
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_VIEWDHEAP, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance (hInstance, nCmdShow)) 
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, (LPCTSTR)IDC_VIEWDHEAP);

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
}



//
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
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX); 

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= (WNDPROC)WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, (LPCTSTR)IDC_VIEWDHEAP);
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= (LPCTSTR)IDC_VIEWDHEAP;
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

	RECT rc = {0, 0, _SCR_RESOLUTION_WIDTH, _SCR_RESOLUTION_HEIGHT};
	AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, TRUE);

   hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, CW_USEDEFAULT, rc.right-rc.left, rc.bottom-rc.top, NULL, NULL, hInstance, NULL);

   if (!hWnd)
   {
      return FALSE;
   }

   SetWindowPos(hWnd, HWND_TOPMOST, 0, 0, rc.right-rc.left, rc.bottom-rc.top, 0 );
   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

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
	PAINTSTRUCT ps;
	RECT rect;

	switch (message) 
	{
		case WM_CREATE:
			APP_Entry_Init((void *)hInst, (void *)hWnd, _SCR_RESOLUTION_WIDTH, _SCR_RESOLUTION_HEIGHT);
			break;

//		case WM_SHOWWINDOW:
//			APP_Entry_Enter();
//			break;

		case WM_TIMER:
			APP_Entry_Timer( wParam );
			break;

		case WM_PAINT:
			BeginPaint(hWnd, &ps);
			APP_Entry_Draw( (void *)ps.hdc );
			EndPaint(hWnd, &ps);
			break;

		case WM_SIZE:
			GetClientRect(hWnd, &rect);
//			atUIENG_OnResize( rect.right-rect.left, rect.bottom-rect.top ); // 임시로 막음.	
//			atUIAPI_FlushScreen();
			break;

		case WM_CLOSE:
			APP_Entry_Exit();
			DestroyWindow(hWnd);
			break;

		case WM_DESTROY:
			APP_Entry_Final();
			PostQuitMessage(0);
			break;

		case WM_COMMAND:
			OnCommand( hWnd, wParam, lParam );
			break;

		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
   }

   return 0;
}
