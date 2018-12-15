#include "AT_Types.h"
#include "SYSAPI.h"
#include "OEM_SA_System.h"


static atBOOL g_bSystemIntitialized = atFALSE;

static OEM_SYSAPI_SYSTEM_T tOemFn;


atVOID OEM_SYSAPI_SYSTEM_SetOemFn(OEM_SYSAPI_SYSTEM_T *fnOem)
{
	tOemFn = *fnOem;
}


atVOID OEM_SYSAPI_SYSTEM_GetOemFn(OEM_SYSAPI_SYSTEM_T *fnOem)
{
	*fnOem = tOemFn;
}


atLONG SYSAPI_InitSystem( atLONG lSysInst, atLONG lInitData1, atLONG lInitData2 )
{
	long oemInst = 0;

	if( g_bSystemIntitialized ) return atTRUE;

	if (tOemFn.fnInitSystem != atNULL) {
		oemInst = tOemFn.fnInitSystem(lSysInst, lInitData1, lInitData2);
	}

	return oemInst;
}


atVOID SYSAPI_FinalSystem( NOARGS )
{
	if( !g_bSystemIntitialized ) return;

	if (tOemFn.fnFinalSystem != atNULL) {
		tOemFn.fnFinalSystem();
	}

	g_bSystemIntitialized = atFALSE;
}


atLONG SYSAPI_GetSystemVar( SYSAPI_SYSVAR sysvar )
{
	if(sysvar == SYSAPI_SYSVAR_NONE) {
		return 0;
	}

	if( tOemFn.fnGetSystemVar != atNULL ){
		return tOemFn.fnGetSystemVar( sysvar );
	}

	return 0;
}

atLONG SYSAPI_SetSystemVar( SYSAPI_SYSVAR sysvar, atLONG value )
{
	if(sysvar == SYSAPI_SYSVAR_NONE) {
		return 0;
	}

	if( tOemFn.fnSetSystemVar != atNULL ){
		return tOemFn.fnSetSystemVar( sysvar, value );
	}

	return 0;
}

atINT SYSAPI_SystemOff( atINT ExitCode )
{
	if( tOemFn.fnSystemOff != atNULL ){
		return tOemFn.fnSystemOff(ExitCode );
	}

	return 0;
}


atINT  SYSAPI_InvokeSysGUI( SYS_FUNC_INVOKE_SYSGUI *fnFunc, atLONG lParam )
{
	if( tOemFn.fnInvokeSysGUI != atNULL ){
		return tOemFn.fnInvokeSysGUI( fnFunc, lParam );
	}

	return 0;
}

atINT   SYSAPI_SysCommand( SYSAPI_SYSCMD cmdCode, atLONG param1, atLONG param2 )
{
	if(cmdCode == SYSAPI_SYSCMD_NONE) {
		return 0;
	}

	if( tOemFn.fnSysCommand != atNULL ){
		return tOemFn.fnSysCommand(cmdCode, param1, param2);
	}

	return 0;
}

