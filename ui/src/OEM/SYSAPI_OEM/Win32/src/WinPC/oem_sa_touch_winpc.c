//
// Key.c
//
////////////////////////////////////////////////////////////////

#include "SYSAPI.h"
#include "OEM_SA_Touch.h"
#include <windows.h>


extern LRESULT (CALLBACK *g_SYS_WndProc_Mouse)(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

static SYS_FUNC_TOUCHNOTIFY *g_fnInputFunc = atNULL;
static int g_btn = 0;

extern HWND g_SYS_hWindow;
extern int		g_SYS_nScale; // 화면 스케일링을 위해...


static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int x, y;
	int vkey, scan_code;
	short delta;
	int ret = 0;
	
	switch (message) 
	{
		case WM_LBUTTONDOWN:
			x = LOWORD(lParam);
			y = HIWORD(lParam);
			if( g_fnInputFunc ) g_fnInputFunc( SYS_MSG_TOUCHDOWN, 0, x << 16 | y );
			SetCapture(hWnd);
			g_btn = 1;
			break;
			
		case WM_RBUTTONDOWN:
			x = LOWORD(lParam);
			y = HIWORD(lParam);
			if( g_fnInputFunc ) g_fnInputFunc( SYS_MSG_TOUCHDOWN, 0, x << 16 | y );
			SetCapture(hWnd);
			g_btn = 1;
			g_fnInputFunc( SYS_MSG_LONGCLICK, 0, x << 16 | y );
			break;

		case WM_MOUSEMOVE:
			if( !g_btn ) break;
			x = LOWORD(lParam);
			y = HIWORD(lParam);
 			if( g_fnInputFunc ) g_fnInputFunc( SYS_MSG_TOUCHMOVE, 0, x << 16 | y );
			break;

		case WM_LBUTTONUP:
		case WM_RBUTTONUP:
			ReleaseCapture();
			x = LOWORD(lParam);
			y = HIWORD(lParam);
 			if( g_fnInputFunc ) g_fnInputFunc( SYS_MSG_TOUCHUP, 0, x << 16 | y );
			g_btn = 0;
			break;
	}

	return 0;
}


atBOOL OEM_SYSAPI_InitTouchDev(  atLONG lSysInst, atLONG lInitData1, atLONG lInitData2 )
{
	g_SYS_WndProc_Mouse = WndProc;

	g_fnInputFunc = atNULL;

	return atTRUE;
}


void OEM_SYSAPI_FinalTouchDev(void)
{
	g_fnInputFunc = atNULL;

	g_SYS_WndProc_Mouse = atNULL;
}


void	OEM_SYSAPI_SetTouchNotifyFunc( SYS_FUNC_TOUCHNOTIFY *fnInputFunc )
{
	g_fnInputFunc = fnInputFunc;
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
	

int OEM_SYSAPI_ConvOEMPtr2VPtr( int *x, int *y )
{
//	*x = *x / g_SYS_nScale;
//	*y = *y / g_SYS_nScale;
	return 0;
}


int OEM_SYSAPI_ConvVPtr2OemPtr( int *x, int *y )
{
//	*x = *x * g_SYS_nScale;
//	*y = *y * g_SYS_nScale;
	return 0;
}



void init_sysapi_touch_oem() 
{
	OEM_SYSAPI_TOUCH_T oem_funcs = {
		OEM_SYSAPI_InitTouchDev,
		OEM_SYSAPI_FinalTouchDev,

		OEM_SYSAPI_SetTouchNotifyFunc,

		OEM_SYSAPI_GetMousePos,

		OEM_SYSAPI_ConvOEMPtr2VPtr,
		OEM_SYSAPI_ConvVPtr2OemPtr
	};

	OEM_SYSAPI_TOUCH_SetOemFn( &oem_funcs );
}
