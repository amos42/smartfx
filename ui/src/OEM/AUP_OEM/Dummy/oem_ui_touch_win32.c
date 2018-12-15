//
// Key.c
//
////////////////////////////////////////////////////////////////

#include "UI_MNG.h"
#include "UI_API/UI_API.h"
#include "OEM_UI_Touch.h"
#include "SYSAPI.h"


static int func( int msg, long param1, long param2 )
{
	int vkey;

	switch (msg) 
	{
		case SYS_MSG_TOUCHDOWN:
 			atUIENG_OnMouseDown( (param1)? 2 : 1, param2 >> 16, param2 & 0xffff );
			break;

		case SYS_MSG_TOUCHMOVE:
			atUIENG_OnMouseMove( param1, param2 >> 16, param2 & 0xffff );
			break;

		case SYS_MSG_TOUCHUP:
			atUIENG_OnMouseUp( param1, param2 >> 16, param2 & 0xffff );
			break;
			
		case SYS_MSG_LONGCLICK:
			atUIENG_OnMouseLongClick( param1, param2 >> 16, param2 & 0xffff );
			break;
	}

	return 0;
}


atBOOL OEM_UI_InitTouchDev(  atLONG lSysInst, atLONG lInitData1, atLONG lInitData2 )
{
// Memory, String, File, Debug, Task, Timer, Input, Sound, GPS는 시스템에서 초기화를 관리한다.
//	SYSAPI_InitInputDev( lSysInst, lInitData1, lInitData2 );

	SYSAPI_SetTouchNotifyFunc( func );

	return atTRUE;
}


void OEM_UI_FinalTouchDev(void)
{
// Memory, String, File, Debug, Task, Timer, Input, Sound, GPS는 시스템에서 초기화를 관리한다.
//	SYSAPI_FinalInputDev();
	SYSAPI_SetTouchNotifyFunc( atNULL );
}


atBOOL OEM_UI_GetMousePos( int *x, int *y )
{
	return SYSAPI_GetMousePos( x, y );
}
	

int OEM_UI_ConvOEMPtr2VPtr( int *x, int *y )
{
	return SYSAPI_ConvOEMPtr2VPtr( x, y );
}


int OEM_UI_ConvVPtr2OemPtr( int *x, int *y )
{
	return SYSAPI_ConvVPtr2OemPtr( x, y );
}

