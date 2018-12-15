//
// Key.c
//
////////////////////////////////////////////////////////////////

#include "SYSAPI.h"
#include "OEM_SA_key.h"


//#include "../Common/OEM_SA_key_win32.c.h"

#include <windows.h>

#define OVER_WM5

#ifdef OVER_WM5			
#include <aygshell.h>
#pragma comment(lib, "aygshell.lib")
#endif


extern LRESULT (CALLBACK *g_SYS_WndProc_Key)(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
extern LRESULT (CALLBACK *g_SYS_WndProc_Mouse)(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

static SYS_FUNC_INPUTNOTIFY *g_fnInputFunc;
static int g_btn = 0;

extern HWND g_SYS_hWindow;


static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int x, y;
	int vkey, scan_code;
//	short delta;
#ifdef OVER_WM5			
  	SHRGINFO shrgi;
	RECT rect;
#endif
//	PNMRGINFO lpNoti;
	int ret = 0;

	switch (message) 
	{
//		case WM_SYSKEYDOWN:
		case WM_KEYDOWN:
			if( !(lParam & (1 << 30)) ){ // 키 누르는 처음 한번만 호출
				vkey = OEM_SYSAPI_ConvOEMKey2VKey( wParam );
				if( vkey == 0 ) return 1;
				if( g_fnInputFunc ) g_fnInputFunc( SYS_MSG_KEYDOWN, vkey, 0 );
			}
			break;

//		case WM_SYSKEYUP:
		case WM_KEYUP:
			vkey = OEM_SYSAPI_ConvOEMKey2VKey( wParam );
			if( vkey == 0 ) return 1;
			if( g_fnInputFunc ) g_fnInputFunc( SYS_MSG_KEYUP, vkey, 0 );
			break;

		case WM_CHAR:
			scan_code = (lParam >> 16) & 0xFF;
			if( scan_code == 28 ) break;					// Enter 키는 제외한다.
			if( scan_code == 14 ) break;					// Back Space 키는 제외한다.
			if( scan_code >= 71 && scan_code <= 82 ) break;	// 숫자 키패드는 제외한다.
			if( g_fnInputFunc ) g_fnInputFunc( SYS_MSG_CHAR, wParam, 0 );
			break;

		case WM_LBUTTONDOWN:
			x = LOWORD(lParam);
			y = HIWORD(lParam);
			if( g_fnInputFunc ) g_fnInputFunc( SYS_MSG_TOUCHDOWN, 0, x << 16 | y );
			SetCapture(hWnd);
			g_btn = 1;
#ifdef OVER_WM5			
			shrgi.cbSize        = sizeof(SHRGINFO);
			shrgi.hwndClient    = hWnd;
			shrgi.ptDown.x      = x;
			shrgi.ptDown.y      = y;
			shrgi.dwFlags       = SHRG_NOTIFYPARENT;//SHRG_RETURNCMD;
			SHRecognizeGesture(&shrgi);
#endif
			break;

		case WM_MOUSEMOVE:
			if( !g_btn ) break;
			x = LOWORD(lParam);
			y = HIWORD(lParam);
 			if( g_fnInputFunc ) g_fnInputFunc( SYS_MSG_TOUCHMOVE, 0, x << 16 | y );
			break;

		case WM_LBUTTONUP:
			ReleaseCapture();
			x = LOWORD(lParam);
			y = HIWORD(lParam);
 			if( g_fnInputFunc ) g_fnInputFunc( SYS_MSG_TOUCHUP, 0, x << 16 | y );
			g_btn = 0;
			break;

#ifdef OVER_WM5			
		//case WM_NOTIFY:
		//	lpNoti = (PNMRGINFO)lParam;
		//	if( g_fnInputFunc ) g_fnInputFunc( SYS_MSG_LONGCLICK, 0, lpNoti->ptAction.x << 16 | lpNoti->ptAction.y );			
		//	break;
		case WM_CONTEXTMENU:
			x = LOWORD(lParam);
			y = HIWORD(lParam);
			rect.left = rect.right = x;
			rect.top = rect.bottom = y;
			ScreenToClient (hWnd, &rect);
			if( g_fnInputFunc ) g_fnInputFunc( SYS_MSG_LONGCLICK, 0, rect.left << 16 | rect.top );			
			break;
#endif			
	}

	return 0;
}


atBOOL OEM_SYSAPI_InitInputDev(  atLONG lSysInst, atLONG lInitData1, atLONG lInitData2 )
{
	g_SYS_WndProc_Key = WndProc;
	g_SYS_WndProc_Mouse = WndProc;

	g_fnInputFunc = atNULL;

	return atTRUE;
}


void OEM_SYSAPI_FinalInputDev(void)
{
	g_fnInputFunc = atNULL;

	g_SYS_WndProc_Key = atNULL;
	g_SYS_WndProc_Mouse = atNULL;
}


void	OEM_SYSAPI_SetInputNotifyFunc( SYS_FUNC_INPUTNOTIFY *fnInputFunc )
{
	g_fnInputFunc	= fnInputFunc;
}
	

atBOOL OEM_SYSAPI_GetMousePos( int *x, int *y )
{
	POINT ptr;
	BOOL r;
 
	GetCursorPos( &ptr );

	r = ScreenToClient( g_SYS_hWindow, &ptr );
	if( !r ) return atFALSE;
	
	if( x ) *x = ptr.x;
	if( y ) *y = ptr.y;

	return atTRUE;
}
	
/*
int OEM_SYSAPI_ConvOEMKey2VKey( int OEMKey )
{
    int vk = SYS_VKEY_NULL;

    switch( OEMKey ){
		default:
			vk = atVKEY_NULL;
			break;
    }

    return vk;
}


int OEM_SYSAPI_ConvVKey2OEMKey( int VKey )
{
    int ok = 0;

	ok = VKey;

    return ok;
}
*/


int OEM_SYSAPI_ConvOEMPtr2VPtr( int *x, int *y )
{
	return 0;
}


int OEM_SYSAPI_ConvVPtr2OemPtr( int *x, int *y )
{
	return 0;
}


int OEM_SYSAPI_ConvOEMKey2VKey( int OEMKey )
{
    int vk = SYS_VKEY_NULL;

    switch( OEMKey ){
        case VK_HOME			:
        case VK_RETURN			:
        case VK_F23				: vk = SYS_VKEY_SELECT;        break;			// Mirage
	 case VK_END			: 
	 case VK_BACK			: vk = SYS_VKEY_CLEAR;		break;
        case VK_UP				: vk = SYS_VKEY_UP;        	break;
        case VK_DOWN			: vk = SYS_VKEY_DOWN;        	break;
        case VK_LEFT			: vk = SYS_VKEY_LEFT;        	break;
        case VK_RIGHT			: vk = SYS_VKEY_RIGHT;        	break;
        case VK_NUMPAD0		: vk = SYS_VKEY_0;        		break;
        case VK_NUMPAD1		: vk = SYS_VKEY_1;        		break;
        case VK_NUMPAD2		: vk = SYS_VKEY_2;        		break;
        case VK_NUMPAD3		: vk = SYS_VKEY_3;        		break;
        case VK_NUMPAD4		: vk = SYS_VKEY_4;        		break;
        case VK_NUMPAD5		: vk = SYS_VKEY_5;        		break;
        case VK_NUMPAD6		: vk = SYS_VKEY_6;        		break;
        case VK_NUMPAD7		: vk = SYS_VKEY_7;        		break;
        case VK_NUMPAD8		: vk = SYS_VKEY_8;        		break;
        case VK_NUMPAD9		: vk = SYS_VKEY_9;        		break;
		case VK_ADD				: vk = SYS_VKEY_STAR;		break;
		case VK_SUBTRACT		: vk = SYS_VKEY_POUND;	   	break;
		case VK_F1			: vk = SYS_VKEY_SOFT1;		break;			// Mirage
		case VK_F2			: vk = SYS_VKEY_SOFT2;		break;			// Mirage
        case VK_F3			: vk = SYS_VKEY_SEND;        	break;
        case VK_F4			: vk = SYS_VKEY_END;        	break;			// Mirage

		case VK_F11				: vk = SYS_VKEY_DEBUG1;	   	break;
    }

    return vk;
}


int OEM_SYSAPI_ConvVKey2OEMKey( int VKey )
{
    int ok = 0;

ok = VKey;

    return ok;
}


void init_sysapi_key_oem() 
{
	OEM_SYSAPI_KEY_T oem_funcs = {
		OEM_SYSAPI_InitKeypad,
		OEM_SYSAPI_FinalKeypad,

		OEM_SYSAPI_SetKeypadNotifyFunc,

		OEM_SYSAPI_ConvOEMKey2VKey,
		OEM_SYSAPI_ConvVKey2OEMKey
	};

	OEM_SYSAPI_KEY_SetOemFn( &oem_funcs );
}

