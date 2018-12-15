//
// sound_wipic.c
//
////////////////////////////////////////////////////////////////

#include "OEM_SA_System.h"


atLONG OEM_SYSAPI_Initialize( SYSAPI_INIT_DATA *lpInitData )
{
	return 0;
}


void OEM_SYSAPI_Finalize(void)
{
}

void OEM_SYSAPI_GetSystemDateTime(SYS_DATETIME *date)
{
}

atLONG OEM_SYSAPI_GetAntennaInfo(void)
{
	return 0;
}

atLONG OEM_SYSAPI_GetBatteryInfo(void)
{
	return 0;
}

atINT OEM_SYSAPI_SystemOff(atINT ExitCode)
{
	return 0;
}


atINT  OEM_SYSAPI_InvokeSysGUI( SYS_FUNC_INVOKE_SYSGUI *fnFunc, atLONG lParam )
{
	return 0;
}


atINT OEM_SYSAPI_SysCommand(int cmdCode, atLONG param1, atLONG param2)
{
	return 0;
}


