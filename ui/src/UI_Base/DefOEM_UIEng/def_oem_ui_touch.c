//
// Key.c
//
////////////////////////////////////////////////////////////////

#include "UI_API/UI_API.h"
#include "OEM_UI_Touch.h"


atBOOL OEM_UI_InitTouchDev(  atLONG lSysInst, atLONG lInitData1, atLONG lInitData2 )
{
// Memory, String, File, Debug, Task, Timer, Input, Sound, GPS�� �ý��ۿ��� �ʱ�ȭ�� �����Ѵ�.
//	SYSAPI_InitInputDev( lSysInst, lInitData1, lInitData2 );

	//SYSAPI_SetTouchNotifyFunc( func );

	return atTRUE;
}


void OEM_UI_FinalTouchDev(void)
{
// Memory, String, File, Debug, Task, Timer, Input, Sound, GPS�� �ý��ۿ��� �ʱ�ȭ�� �����Ѵ�.
//	SYSAPI_FinalInputDev();
}


atBOOL OEM_UI_GetMousePos( int *x, int *y )
{
	return atFALSE;
}
	

int OEM_UI_ConvOEMPtr2VPtr( int *x, int *y )
{
	return 0;
}


int OEM_UI_ConvVPtr2OemPtr( int *x, int *y )
{
	return 0;
}

