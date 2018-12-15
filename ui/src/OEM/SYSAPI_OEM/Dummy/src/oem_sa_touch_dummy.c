//
// Key.c
//
////////////////////////////////////////////////////////////////

#include "SYSAPI.h"
#include "OEM_SA_Touch.h"


atBOOL OEM_SYSAPI_InitTouchDev(  atLONG lSysInst, atLONG lInitData1, atLONG lInitData2 )
{
	return atFALSE;
}


void OEM_SYSAPI_FinalTouchDev(void)
{
}


void	OEM_SYSAPI_SetTouchNotifyFunc( SYS_FUNC_TOUCHNOTIFY *fnInputFunc )
{
}
	

int OEM_SYSAPI_ConvOEMPtr2VPtr( int *x, int *y )
{
	return 0;
}


int OEM_SYSAPI_ConvVPtr2OemPtr( int *x, int *y )
{
	return 0;
}

