//
// Key.c
//
////////////////////////////////////////////////////////////////

#include "SYSAPI.h"
#include "OEM_SA_Key.h"


static SYS_FUNC_KEYPADNOTIFY *g_fnInputFunc = atNULL;


void _keycall(int keycode, int status) 
{
	keycode = SYS_VKEY_USERDEF + 100;
	
	if(status == 0) {
			if( g_fnInputFunc ) g_fnInputFunc( SYS_MSG_KEYDOWN, keycode, 0 );
	} else {
			if( g_fnInputFunc ) g_fnInputFunc( SYS_MSG_KEYUP, keycode, 0 );
	}
}


atBOOL OEM_SYSAPI_InitKeypad(  atLONG lSysInst, atLONG lInitData1, atLONG lInitData2 )
{
	return atTRUE;
}


void OEM_SYSAPI_FinalKeypad(void)
{
}


void	OEM_SYSAPI_SetKeypadNotifyFunc( SYS_FUNC_KEYPADNOTIFY *fnInputFunc )
{
	g_fnInputFunc = fnInputFunc;
}
	

int OEM_SYSAPI_ConvOEMKey2VKey( int OEMKey )
{
    return OEMKey;
}


int OEM_SYSAPI_ConvVKey2OEMKey( int VKey )
{
    return VKey;
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

