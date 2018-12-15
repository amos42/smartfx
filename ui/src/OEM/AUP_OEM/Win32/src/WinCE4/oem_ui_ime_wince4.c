#include "A_UIENG.h"
#include "OEM_UI_IME.h"
#include <windows.h>

#include <aygshell.h>                // Palm-size PC shell includes
#include <sip.h>                     // SIP includes
#include <keybd.h>                   // Keyboard flag includes
#pragma comment(lib, "aygshell.lib")


#define BUP_IME_CLASSNAME _T("BUP_IME_CLASS")


#ifdef __cplusplus
extern "C" {
#endif

extern HINSTANCE g_UI_hInstance;
extern HWND g_UI_hWindow;

void OEM_UI_EndIME(void);


#ifdef __cplusplus
}
#endif



/*
static atBOOL OEM_UI_InitIME( atLONG lSysInst, atLONG lInitData1, atLONG lInitData2 )
{
	if( (void *)lSysInst == atNULL ) return atFALSE;

	return atTRUE;
}


void OEM_UI_FinalIME(void)
{
}
*/


static int OEM_UI_GetIMEMethodClassStockCount()
{
	return 0;
}

int OEM_UI_GetIMEMethodClassStock( atINT nIndex, atUI_IMEMETHODCLASS *lpImeClass )
{
	return 0;
}



typedef LRESULT (CALLBACK FAR TWndProc)( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam );
static TWndProc *g_WndProc_Original = atNULL;


atTCHAR OEM_UI_IME_Buffer[256];
atLPUI_IME_NOTIFY_FUNC fnImeMethodFunc;


#define IDC_EDIT   1001
#define IDC_BUTTON 1002

static HWND hImeWnd;
static HWND hEditBox, hButton;


static void show_sip( atBOOL bShow )
{
	if( bShow )
    		SHSipPreference( hImeWnd, SIP_UP );
	else
    		SHSipPreference( hImeWnd, SIP_DOWN );
}


static LRESULT CALLBACK ParentWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	RECT rect;

	switch (message) 
	{
		case WM_SIZE:
			GetClientRect(hWnd, &rect);
			MoveWindow( hImeWnd, 0, 0, rect.right-rect.left, rect.bottom-rect.top, TRUE );
			break;
 	}

	return CallWindowProc( g_WndProc_Original, hWnd, message, wParam, lParam );
}


static LRESULT CALLBACK ImeWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	RECT rect;
	SIPINFO sinfo;
	
	switch( message ){
    	case WM_CREATE:
    	    hEditBox = CreateWindow( _T("edit"), _T(""), WS_CHILD | WS_VISIBLE | WS_BORDER | ES_MULTILINE | WS_VSCROLL | WS_HSCROLL,  10, 10, 100, 40, hWnd, (HMENU)IDC_EDIT, GetModuleHandle(0), 0 );
            SendMessage( hEditBox, EM_SETMARGINS, EC_LEFTMARGIN | EC_RIGHTMARGIN, MAKELONG( 2, 2 ) );
    	    hButton = CreateWindow( _T("button"), _T("OK"), WS_CHILD | WS_VISIBLE | WS_BORDER, 50, 70, 80, 30, hWnd, (HMENU)IDC_BUTTON, GetModuleHandle(0), 0 );
			SetFocus( hEditBox );
    		break;
    
    	case WM_SHOWWINDOW:
       	    show_sip( atTRUE );
    		break;

		case WM_CLOSE:
       	    show_sip( atFALSE );
			break;
    
    	case WM_DESTROY:
    	    hEditBox = NULL;
			hButton = NULL;
    		PostQuitMessage(0);
    		break;

		case WM_TIMER:
         		memset( &sinfo, 0, sizeof( sinfo ) );
         		sinfo.cbSize = sizeof( sinfo );
         		if ( SHSipInfo(SPI_GETSIPINFO, 0, &sinfo, 0) ){
         		    if ( sinfo.fdwFlags & SIPF_ON ){
				KillTimer( hWnd, 111 );
				break;
         		    }
         		}
       	    	show_sip( atTRUE );
			break;

    	case WM_SIZE:
			GetClientRect(hWnd, &rect);
//			SetFocus( hButton );
            MoveWindow( hEditBox, rect.left+10, rect.top+10, (rect.right-rect.left)-20, 40, TRUE );
            MoveWindow( hButton,  rect.left + ((rect.right-rect.left)-100)/2, rect.top+10+40+10, 100, 30, TRUE );
		if( IsWindowVisible(hWnd) ){
//			SetFocus( hEditBox );
       	      	show_sip( atTRUE );
			SetTimer( hWnd, 111, 500, NULL );
		}
    		break;
    
        case WM_COMMAND:
            switch(LOWORD(wParam)){
                case IDC_BUTTON:
SendMessage( hEditBox, WM_GETTEXT, SendMessage(hEditBox,WM_GETTEXTLENGTH,0,0)+1, (LPARAM)OEM_UI_IME_Buffer );
		       		show_sip( atFALSE );
					OEM_UI_EndIME();
if( fnImeMethodFunc ) fnImeMethodFunc( NULL, 10, 0, 0 );
fnImeMethodFunc = NULL;	
					break;
                case IDC_EDIT:
                    switch(HIWORD(wParam)){
                        case EN_CHANGE:
                            break;
                        case EN_SETFOCUS:
       	    		show_sip( atTRUE );
                            break;
                        case EN_KILLFOCUS:
       	    		show_sip( atFALSE );
                            break;
                    }
                    break;
            }
            break;
    
    	default:
    		return DefWindowProc(hWnd, message, wParam, lParam);
   }

   return 0;
}


static atBOOL OEM_UI_InitIME( atLONG lSysInst, atLONG lInitData1, atLONG lInitData2 )
{
	WNDCLASS	wc;
	int screen_width, screen_height;

	if( (void *)lSysInst == NULL ) return atFALSE;

    wc.style			= CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc		= (WNDPROC)ImeWndProc;
    wc.cbClsExtra		= 0;
    wc.cbWndExtra		= 0;
    wc.hInstance		= g_UI_hInstance;
    wc.hIcon			= NULL;
    wc.hCursor			= NULL;
    wc.hbrBackground	= (HBRUSH) GetStockObject(BLACK_BRUSH);
    wc.lpszMenuName		= 0;
    wc.lpszClassName	= BUP_IME_CLASSNAME;
	RegisterClass(&wc);

	screen_width = GetSystemMetrics(SM_CXSCREEN);
	screen_height = GetSystemMetrics(SM_CYSCREEN);
	hImeWnd = CreateWindow(wc.lpszClassName, _T("BUP_IME"), 
						   WS_CHILD,
						   0, 0, screen_width, screen_height,
      					          g_UI_hWindow, NULL, g_UI_hInstance, NULL);
    if( hImeWnd == NULL ) return atFALSE;
	ShowWindow( hImeWnd, FALSE );

    //SHFullScreen( hImeWnd, SHFS_HIDETASKBAR | SHFS_SHOWSIPBUTTON | SHFS_HIDESTARTICON );

	// Main 윈도우의 Resize를 가로채기 위해서...
	g_WndProc_Original = (TWndProc *)GetWindowLong( g_UI_hWindow, GWL_WNDPROC );
	SetWindowLong( g_UI_hWindow, GWL_WNDPROC, (LONG)ParentWndProc );
	
	return atTRUE;
}


static void OEM_UI_FinalIME(void)
{
	SetWindowLong( g_UI_hWindow, GWL_WNDPROC, (LONG)g_WndProc_Original );
	g_WndProc_Original = atNULL;
	
    if( hImeWnd ){
        DestroyWindow( hImeWnd );
        hImeWnd = NULL;
    }        
}


static void OEM_UI_StartIME(void)
{
//OEM_UI_IME_Buffer[0] = _ATC('\0');
//SendMessage( hEditBox, WM_SETTEXT, 0, (LPARAM)_T("") );
	SendMessage( hEditBox, WM_SETTEXT, atUIAPI_strlen(OEM_UI_IME_Buffer), (LPARAM)OEM_UI_IME_Buffer );
	
//	ShowWindow( hImeWnd, SW_SHOWMAXIMIZED );
	ShowWindow( hImeWnd, SW_SHOW );
	SetFocus( hEditBox );
    UpdateWindow( hImeWnd );

//    SHFullScreen( hImeWnd, SHFS_HIDETASKBAR | SHFS_SHOWSIPBUTTON | SHFS_HIDESTARTICON );

    show_sip( atTRUE );
}


static void OEM_UI_EndIME(void)
{
	ShowWindow( hImeWnd, SW_HIDE );
	
	SetFocus( g_UI_hWindow );
}


void init_uiapi_ime_oem() 
{
	OEM_UI_IME_T oem_funcs = {
		OEM_UI_InitIME,
		OEM_UI_FinalIME,

		OEM_UI_GetIMEMethodClassStockCount,
		OEM_UI_GetIMEMethodClassStock,

		OME_UI_IME_CreateIME,
		OME_UI_IME_DestroyIME,

		OEM_UI_IME_Start,
		OEM_UI_IME_Input,
		OEM_UI_IME_GetComplete,
		OEM_UI_IME_GetComposite,
		OEM_UI_IME_Flush,
		OEM_UI_IME_Stop	
	};

	OEM_UI_IME_SetOemFn( &oem_funcs );
}
