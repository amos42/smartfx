//
// Key.c
//
////////////////////////////////////////////////////////////////

#include "SYSAPI.h"
#include "OEM_SA_Key.h"


atBOOL OEM_SYSAPI_InitKeypad(  atLONG lSysInst, atLONG lInitData1, atLONG lInitData2 )
{
	return atFALSE;
}


void OEM_SYSAPI_FinalKeypad(void)
{
}


void	OEM_SYSAPI_SetKeypadNotifyFunc( SYS_FUNC_KEYPADNOTIFY *fnInputFunc )
{
}
	

int OEM_SYSAPI_ConvOEMKey2VKey( int OEMKey )
{
    return SYS_VKEY_NULL;
}


int OEM_SYSAPI_ConvVKey2OEMKey( int VKey )
{
    return 0;
}

