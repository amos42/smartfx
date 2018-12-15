//
// sound_wipic.c
//
////////////////////////////////////////////////////////////////

#include "OEM_SA_system.h"
#include <string.h>
#include <windows.h>


#define OVER_WM5

#ifdef OVER_WM5			
#include <phone.h>
#pragma comment(lib, "phone.lib")
#endif


#define WM_INVOKE_SYSGUI (WM_USER+12345)


HINSTANCE g_SYS_hInstance;
HWND g_SYS_hWindow;


typedef LRESULT (CALLBACK FAR TWndProc)(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
static TWndProc *g_WndProc_Original = atNULL;

TWndProc *g_SYS_WndProc_Timer = atNULL;
TWndProc *g_SYS_WndProc_Key = atNULL;
TWndProc *g_SYS_WndProc_Mouse = atNULL;
TWndProc *g_SYS_WndProc_Paint = atNULL;
TWndProc *g_SYS_WndProc_Resize = atNULL;


static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	LRESULT r = -1;

	switch (message) {
//		case WM_SYSKEYDOWN:
//		case WM_SYSKEYUP:
		case WM_KEYDOWN:
		case WM_KEYUP:
		case WM_CHAR:
			if( g_SYS_WndProc_Key ) r = g_SYS_WndProc_Key( hWnd, message, wParam, lParam );
			break;

		case WM_LBUTTONDOWN:
		case WM_RBUTTONDOWN:
		case WM_MOUSEMOVE:
		case WM_LBUTTONUP:
		case WM_RBUTTONUP:
		//case WM_NOTIFY:
		case WM_CONTEXTMENU:
			if( g_SYS_WndProc_Mouse ) r = g_SYS_WndProc_Mouse( hWnd, message, wParam, lParam );
			break;

		case WM_TIMER:
			if( g_SYS_WndProc_Timer ) r = g_SYS_WndProc_Timer( hWnd, message, wParam, lParam );
			break;

		case WM_PAINT:
			if( g_SYS_WndProc_Paint ) r = g_SYS_WndProc_Paint( hWnd, message, wParam, lParam );
			break;
			
		case WM_SIZE:
			if( g_SYS_WndProc_Resize ) r = g_SYS_WndProc_Resize( hWnd, message, wParam, lParam );
			break;

		case WM_INVOKE_SYSGUI:
			{
			SYS_FUNC_INVOKE_SYSGUI *fnFunc = (SYS_FUNC_INVOKE_SYSGUI *)wParam;
			return fnFunc( lParam );
			}
	}

	if( r != 0 )
		return CallWindowProc( g_WndProc_Original, hWnd, message, wParam, lParam );
	else
		return r;
}


atLONG OEM_SYSAPI_InitSystem( atLONG lSysInst, atLONG lInitData1, atLONG lInitData2 )
{
	g_SYS_hInstance = (HINSTANCE)lSysInst;
	g_SYS_hWindow = (HWND)lInitData1;
	if( g_SYS_hWindow == atNULL ) return -1;

	g_WndProc_Original = (TWndProc *)GetWindowLong( g_SYS_hWindow, GWL_WNDPROC );
	SetWindowLong( g_SYS_hWindow, GWL_WNDPROC, (LONG)WndProc );
	
	return 0;
}


void OEM_SYSAPI_FinalSystem(void)
{
	SetWindowLong( g_SYS_hWindow, GWL_WNDPROC, (LONG)g_WndProc_Original );
	g_WndProc_Original = atNULL;
	
	g_SYS_hInstance = atNULL;
	g_SYS_hWindow = atNULL;
}


atLONG OEM_SYSAPI_GetSystemVar(SYSAPI_SYSVAR sysvar)
{
	long ret = 0;

	switch(sysvar) {
	case SYSAPI_SYSVAR_BACKLIGHT :
		ret = 100;
		break;
	case SYSAPI_SYSVAR_BATTERY :
		ret = 100;
		break;
	}

	return ret;
}

atLONG OEM_SYSAPI_SetSystemVar(SYSAPI_SYSVAR sysvar, atLONG value)
{
	return 0;
}

atINT OEM_SYSAPI_SystemOff(atINT ExitCode)
{
	PostMessage(g_SYS_hWindow, WM_CLOSE, ExitCode, 0);
	return ExitCode;
}


atINT  OEM_SYSAPI_InvokeSysGUI( SYS_FUNC_INVOKE_SYSGUI *fnFunc, atLONG lParam )
{
	return SendMessage( g_SYS_hWindow, WM_INVOKE_SYSGUI, (long)fnFunc, (long)lParam );
}


atINT OEM_SYSAPI_SysCommand( SYSAPI_SYSCMD cmdCode, atLONG param1, atLONG param2 )
{
	int result = 0;

	switch(cmdCode) {
#ifdef OVER_WM5
	case SYSAPI_SYSCMD_MAKECALL : {
			atLPCTSTR szPhoneNumber = (atLPCTSTR)param1;
			atLPCTSTR szIDName = (atLPCTSTR)param2;
			PHONEMAKECALLINFO pmci;
			ZeroMemory(&pmci, sizeof(PHONEMAKECALLINFO));
			pmci.dwFlags = PMCF_DEFAULT;
			pmci.pszDestAddress = szPhoneNumber;
			pmci.pszCalledParty = szIDName;
			pmci.cbSize = sizeof(pmci);

			PhoneMakeCall(&pmci);
			result = 1;
		}
#endif
	default :
		result = 0;
	}

	return result;
}



void init_sysapi_system_oem(void) 
{
	OEM_SYSAPI_SYSTEM_T oem_funcs = {
		OEM_SYSAPI_InitSystem,
		OEM_SYSAPI_FinalSystem,

		OEM_SYSAPI_GetSystemVar,
		OEM_SYSAPI_SetSystemVar,

		OEM_SYSAPI_SystemOff,

		OEM_SYSAPI_InvokeSysGUI,

		OEM_SYSAPI_SysCommand
	};

	OEM_SYSAPI_SYSTEM_SetOemFn( &oem_funcs );
}
