//
// Key.c
//
////////////////////////////////////////////////////////////////

#include "SYSAPI.h"
#include "OEM_SA_key.h"

#pragma comment(linker, "/subsystem:windows")
#define _WIN32_WINNT 0x0501
#define WINVER   0x0501
#include <windows.h>


extern LRESULT (CALLBACK *g_SYS_WndProc_Key)(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

static SYS_FUNC_KEYPADNOTIFY *g_fnInputFunc = atNULL;

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

		case WM_MOUSEWHEEL:
		   	delta = HIWORD( wParam );
		   	delta = ( delta / WHEEL_DELTA ); // WHEEL_DELTA : 120
			if( g_fnInputFunc ) g_fnInputFunc( SYS_MSG_SCROLL, delta, 0 );
		   	break;
	}

	return 0;
}


static atBOOL OEM_SYSAPI_InitKeypad(  atLONG lSysInst, atLONG lInitData1, atLONG lInitData2 )
{
	g_SYS_WndProc_Key = WndProc;

	g_fnInputFunc = atNULL;

	return atTRUE;
}


static void OEM_SYSAPI_FinalKeypad(void)
{
	g_fnInputFunc = atNULL;

	g_SYS_WndProc_Key = atNULL;
}


static void	OEM_SYSAPI_SetKeypadNotifyFunc( SYS_FUNC_KEYPADNOTIFY *fnInputFunc )
{
	g_fnInputFunc = fnInputFunc;
}
	

static int OEM_SYSAPI_ConvOEMKey2VKey( int OEMKey )
{
    int vk = SYS_VKEY_NULL;

   switch( OEMKey ){
        case VK_HOME			:
        case VK_RETURN			: vk = SYS_VKEY_SELECT;        break;
	 case VK_END			: 
	 case VK_BACK			: vk = SYS_VKEY_CLEAR;		break;
        case VK_UP				: vk = SYS_VKEY_UP;        	break;
        case VK_DOWN			: vk = SYS_VKEY_DOWN;        	break;
        case VK_LEFT			: vk = SYS_VKEY_LEFT;        	break;
        case VK_RIGHT			: vk = SYS_VKEY_RIGHT;        	break;
        case VK_DELETE			: vk = SYS_VKEY_SEND;        	break;
        case VK_NEXT			: vk = SYS_VKEY_END;        	break;
        case VK_INSERT			: vk = SYS_VKEY_SOFT1;        	break;
		case VK_ESCAPE :
        case VK_PRIOR			: vk = SYS_VKEY_SOFT2;        	break;
//		case '0' :
        case VK_NUMPAD0		: vk = SYS_VKEY_0;        		break;
//		case '1' :
        case VK_NUMPAD1		: vk = SYS_VKEY_1;        		break;
//		case '2' :
        case VK_NUMPAD2		: vk = SYS_VKEY_2;        		break;
//		case '3' :
        case VK_NUMPAD3		: vk = SYS_VKEY_3;        		break;
//		case '4' :
        case VK_NUMPAD4		: vk = SYS_VKEY_4;        		break;
//		case '5' :
        case VK_NUMPAD5		: vk = SYS_VKEY_5;        		break;
//		case '6' :
        case VK_NUMPAD6		: vk = SYS_VKEY_6;        		break;
//		case '7' :
        case VK_NUMPAD7		: vk = SYS_VKEY_7;        		break;
//		case '8' :
        case VK_NUMPAD8		: vk = SYS_VKEY_8;        		break;
//		case '9' :
        case VK_NUMPAD9		: vk = SYS_VKEY_9;        		break;
		case VK_F5:
		case VK_ADD				: vk = SYS_VKEY_STAR;		break;
		case VK_F6:
		case VK_SUBTRACT		: vk = SYS_VKEY_POUND;	   	break;
		
		case VK_F11				: vk = SYS_VKEY_DEBUG1;	   	break;
    }

    return vk;
}


static int OEM_SYSAPI_ConvVKey2OEMKey( int VKey )
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

