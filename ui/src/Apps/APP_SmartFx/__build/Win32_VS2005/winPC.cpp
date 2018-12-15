// winPC.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "resource.h"
#include <stdio.h>
#include <time.h>
#include <stdlib.h>

#include "BUP.h"
#include "SYSAPI.h"
#include "APP_Entry.h"


#define _SCR_RESOLUTION_WIDTH	(240)
#define _SCR_RESOLUTION_HEIGHT	(320)

#define MAX_LOADSTRING 100

// Global Variables:
static HINSTANCE hInst;								// current instance

static TCHAR szTitle[]={'B','U','P','_','T','e','s','t',};				// The title bar text
static TCHAR szWindowClass[]={'B','U','P','_','T','e','s','t',};		// window class name

static int g_scale = 1;
static int cur_screen_x, cur_screen_y;
static int g_debug_console = 0;

// Foward declarations of functions included in this code module:
static ATOM				MyRegisterClass(HINSTANCE hInstance);
static BOOL				InitInstance(HINSTANCE, int);
static LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);


int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR     lpCmdLine,
                     int       nCmdShow)

{
	// TODO: Place code here.
	MSG msg;
	HACCEL hAccelTable;

	MyRegisterClass(hInstance);
	
	// Perform application initialization:
	if (!InitInstance (hInstance, nCmdShow)) 
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, (LPCTSTR)IDC_BUP_TEST);

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
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX); 

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= (WNDPROC)WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, (LPCTSTR)IDI_BUP_TEST);
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= (LPCTSTR)IDC_BUP_TEST;
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
static BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	HWND hWnd;

	hInst = hInstance; // Store instance handle in our global variable

	RECT rc = {0, 0, _SCR_RESOLUTION_WIDTH, _SCR_RESOLUTION_HEIGHT};
	AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, TRUE); // 풀다운 메뉴 영역 포함한 윈도우 크기
//	AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE); // 풀다운 메뉴 영역 제외한 윈도우 크기

	hWnd = CreateWindow(szWindowClass, szTitle, 
   						WS_OVERLAPPEDWINDOW,
      					CW_USEDEFAULT, CW_USEDEFAULT, 
						rc.right-rc.left, rc.bottom-rc.top,
      					NULL, NULL, hInstance, NULL);

   if (!hWnd) return FALSE;

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}



static void SetWindowClientSize(HWND hWnd, int cx, int cy)
{
	RECT rc, rc_win;
	int  width, height;

	SYSAPI_ExtGraphicMethod( 0x8001, g_scale, 0, 0, 0, 0, 0 );
	
	GetWindowRect(hWnd, &rc_win);
	rc.left   = 0; 
	rc.right  = cx;
	rc.top    = 0;
	rc.bottom = cy;

	AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, TRUE); 

	width  = rc.right - rc.left;
	height = rc.bottom - rc.top;

	if(rc_win.top < 0)
		rc_win.top = 10;
	if(rc_win.left < 0) 
		rc_win.left = 10;
	
	MoveWindow(hWnd, rc_win.left, rc_win.top, width, height, TRUE);
}


static LRESULT OnCommand(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	HMENU hMenu;

	switch(wParam)
	{
	case IDM_SCREEN_240x320 :
		SetWindowClientSize(hWnd, 240*g_scale, 320*g_scale); break;
	case IDM_SCREEN_320x240 :
		SetWindowClientSize(hWnd, 320*g_scale, 240*g_scale); break;
	case IDM_SCREEN_240x400 :
		SetWindowClientSize(hWnd, 240*g_scale, 400*g_scale); break;
	case IDM_SCREEN_400x240 :
		SetWindowClientSize(hWnd, 400*g_scale, 240*g_scale); break;
	case IDM_SCREEN_320x320 :
		SetWindowClientSize(hWnd, 320*g_scale, 320*g_scale); break;
	case IDM_SCREEN_480x272 :
		SetWindowClientSize(hWnd, 480*g_scale, 272*g_scale); break;
	case IDM_SCREEN_800x480 :
		SetWindowClientSize(hWnd, 800*g_scale, 480*g_scale); break;
	case IDM_SCREEN_SCALE_X1 :
		if( g_scale != 1 ){
			hMenu = GetMenu(hWnd);
			CheckMenuItem( hMenu, IDM_SCREEN_SCALE_X1, MF_CHECKED );
			CheckMenuItem( hMenu, IDM_SCREEN_SCALE_X2, MF_UNCHECKED );

			cur_screen_x = cur_screen_x / g_scale * 1;
			cur_screen_y = cur_screen_y / g_scale * 1;
			g_scale = 1;
			SetWindowClientSize(hWnd, cur_screen_x, cur_screen_y);
		}
		break;
	case IDM_SCREEN_SCALE_X2 :
		if( g_scale != 2 ){
			hMenu = GetMenu(hWnd);
			CheckMenuItem( hMenu, IDM_SCREEN_SCALE_X2, MF_CHECKED );
			CheckMenuItem( hMenu, IDM_SCREEN_SCALE_X1, MF_UNCHECKED );

			cur_screen_x = cur_screen_x / g_scale * 2;
			cur_screen_y = cur_screen_y / g_scale * 2;
			g_scale = 2;
			SetWindowClientSize(hWnd, cur_screen_x, cur_screen_y);
		}
		break;
	case IDM_VIEW_DEBUG:
		hMenu = GetMenu(hWnd);
		g_debug_console = !g_debug_console;
		if( g_debug_console )
			CheckMenuItem( hMenu, IDM_VIEW_DEBUG, MF_CHECKED );
		else
			CheckMenuItem( hMenu, IDM_VIEW_DEBUG, MF_UNCHECKED );
		atUIAPI_ActivateDebug( g_debug_console );
		break;
	}

	return 0;
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
	RECT rect;

	switch (message) 
	{
		case WM_CREATE:
			cur_screen_x = _SCR_RESOLUTION_WIDTH;
			cur_screen_y = _SCR_RESOLUTION_HEIGHT;
			
			APP_Entry_Init((void *)hInst, (void *)hWnd, _SCR_RESOLUTION_WIDTH, _SCR_RESOLUTION_HEIGHT);
			break;

		case WM_SHOWWINDOW:
			APP_Entry_Enter();
			break;

		case WM_PAINT:
			APP_Entry_Draw();
			break;

		case WM_SIZE:
			GetClientRect(hWnd, &rect);
			cur_screen_x = rect.right-rect.left;
			cur_screen_y = rect.bottom-rect.top;
			atUIENG_OnResize( cur_screen_x/g_scale, cur_screen_y/g_scale ); 
			atUIAPI_FlushScreen();
			break;

		case WM_CLOSE:
			APP_Entry_Exit();
			DestroyWindow(hWnd);
			break;

		case WM_DESTROY:
			APP_Entry_Final();
			PostQuitMessage(0);
			break;

		case WM_KEYDOWN:
			break;
			
		case WM_COMMAND:
			OnCommand(hWnd, wParam, lParam);
			break;

		default:
			//	atAPP_PostMessage(atAPPMNG_GetCurApp(), hWnd, message, wParam, lParam);
			return DefWindowProc(hWnd, message, wParam, lParam);
   }

   return 0;
}


