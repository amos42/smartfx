#include "SYSAPI.h"
#include "OEM_SA_Touch.h"


#ifdef __USES_SYSAPI_TOUCH_


static atBOOL g_bSysInputIntitialized = atFALSE;


static OEM_SYSAPI_TOUCH_T tOemFn;


atVOID OEM_SYSAPI_TOUCH_SetOemFn(OEM_SYSAPI_TOUCH_T *fnOem)
{
	tOemFn = *fnOem;
}


atVOID OEM_SYSAPI_TOUCH_GetOemFn(OEM_SYSAPI_TOUCH_T *fnOem)
{
	*fnOem = tOemFn;
}


atBOOL SYSAPI_InitTouchDev( atLONG lSysInst, atLONG lInitData1, atLONG lInitData2 )
{
	if( g_bSysInputIntitialized ) return atTRUE;
	
	if( tOemFn.fnInitTouchDev != atNULL ){
		g_bSysInputIntitialized =  tOemFn.fnInitTouchDev( lSysInst, lInitData1, lInitData2 );
	}

	return g_bSysInputIntitialized;    
}


void SYSAPI_FinalTouchDev( NOARGS )
{
	if( !g_bSysInputIntitialized ) return;
	
	if( tOemFn.fnSetTouchNotifyFunc != atNULL ){
    	tOemFn.fnSetTouchNotifyFunc(atNULL);
	}
	if( tOemFn.fnFinalTouchDev != atNULL ){
    	tOemFn.fnFinalTouchDev();
	}
		
	g_bSysInputIntitialized = atFALSE;
}


void	SYSAPI_SetTouchNotifyFunc( SYS_FUNC_TOUCHNOTIFY *fnInputFunc )
{
	if( tOemFn.fnSetTouchNotifyFunc != atNULL ){
		tOemFn.fnSetTouchNotifyFunc( fnInputFunc );
	}
}

atBOOL SYSAPI_GetMousePos( int *x, int *y )
{
	if( tOemFn.fnGetMousePos != atNULL ){
		return tOemFn.fnGetMousePos( x, y );
	}

	return atFALSE;
}

int SYSAPI_ConvOEMPtr2VPtr( int *x, int *y )
{
	if( tOemFn.fnConvOEMPtr2VPtr != atNULL ){
		return tOemFn.fnConvOEMPtr2VPtr( x, y );
	}

	return 0;
}


int SYSAPI_ConvVPtr2OemPtr( int *x, int *y )
{
	if( tOemFn.fnConvVPtr2OemPtr != atNULL ){
		return tOemFn.fnConvVPtr2OemPtr( x, y );
	}

	return 0;
}


#endif

