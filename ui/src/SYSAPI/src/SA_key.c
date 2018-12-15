#include "SYSAPI.h"
#include "OEM_SA_Key.h"


#ifdef __USES_SYSAPI_KEYPAD_


static atBOOL g_bSysInputIntitialized = atFALSE;


static OEM_SYSAPI_KEY_T tOemFn;


atVOID OEM_SYSAPI_KEY_SetOemFn(OEM_SYSAPI_KEY_T *fnOem)
{
	tOemFn = *fnOem;
}


atVOID OEM_SYSAPI_KEY_GetOemFn(OEM_SYSAPI_KEY_T *fnOem)
{
	*fnOem = tOemFn;
}


atBOOL SYSAPI_InitKeypad( atLONG lSysInst, atLONG lInitData1, atLONG lInitData2 )
{
	if( g_bSysInputIntitialized ) return atTRUE;
	
	if( tOemFn.fnInitKeypad != atNULL ){
		g_bSysInputIntitialized =  tOemFn.fnInitKeypad( lSysInst, lInitData1, lInitData2 );
	}

	return g_bSysInputIntitialized;    
}


void SYSAPI_FinalKeypad( NOARGS )
{
	if( !g_bSysInputIntitialized ) return;
	
	if( tOemFn.fnSetKeypadNotifyFunc != atNULL ){
    	tOemFn.fnSetKeypadNotifyFunc(atNULL);
	}
	if( tOemFn.fnFinalKeypad != atNULL ){
    	tOemFn.fnFinalKeypad();
	}
		
	g_bSysInputIntitialized = atFALSE;
}


void	SYSAPI_SetKeypadNotifyFunc( SYS_FUNC_KEYPADNOTIFY *fnInputFunc )
{
	if( tOemFn.fnSetKeypadNotifyFunc != atNULL ){
		tOemFn.fnSetKeypadNotifyFunc( fnInputFunc );
	}
}


int SYSAPI_ConvOEMKey2VKey( int OEMKey )
{
	if( (OEMKey >= SYS_VKEY_USERDEF_START) && (OEMKey <= SYS_VKEY_USERDEF_END) ){
		return OEMKey;
	}

	if( tOemFn.fnConvOEMKey2VKey != atNULL ){
		return tOemFn.fnConvOEMKey2VKey( OEMKey );
	}

	return 0;
}


int SYSAPI_ConvVKey2OEMKey( int VKey )
{
	if( (VKey >= SYS_VKEY_USERDEF_START) && (VKey <= SYS_VKEY_USERDEF_END) ){
		return VKey;
	}

	if( tOemFn.fnConvVKey2OEMKey != atNULL ){
		return tOemFn.fnConvVKey2OEMKey( VKey );
	}

	return 0;
}


#endif

