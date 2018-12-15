//
// Key.c
//
////////////////////////////////////////////////////////////////

#include "SYSAPI.h"
#include "OEM_SA_Touch.h"


static SYS_FUNC_TOUCHNOTIFY *g_fnInputFunc = atNULL;

void _touchcall(int up_down, int x, int y) 
{
	if (up_down == 0) {
		if( g_fnInputFunc ) g_fnInputFunc( SYS_MSG_TOUCHDOWN, 0, x << 16 | y );
	} else if (up_down == 1) {
		if( g_fnInputFunc ) g_fnInputFunc( SYS_MSG_TOUCHUP, 0, x << 16 | y );
	}
}


atBOOL OEM_SYSAPI_InitTouchDev(  atLONG lSysInst, atLONG lInitData1, atLONG lInitData2 )
{
	return atTRUE;
}


void OEM_SYSAPI_FinalTouchDev(void)
{
}


void	OEM_SYSAPI_SetTouchNotifyFunc( SYS_FUNC_TOUCHNOTIFY *fnInputFunc )
{
	g_fnInputFunc = fnInputFunc;
}

atBOOL OEM_SYSAPI_GetMousePos( int *x, int *y )
{
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
