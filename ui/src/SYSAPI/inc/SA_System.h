//
// SYSTEM.h
//
////////////////////////////////////////////////////////////////

#ifndef __SA_SYSTEM_H_
#define __SA_SYSTEM_H_

#include "SA_Types.h"


#ifdef __cplusplus
extern "C" {
#endif


typedef enum {
	SYSAPI_SYSVAR_NONE = 0,

	SYSAPI_SYSVAR_ANTENA,
	SYSAPI_SYSVAR_BATTERY,
	SYSAPI_SYSVAR_BACKLIGHT,

	SYSAPI_SYSVAR_USERDEF_START = 0x100,
	SYSAPI_SYSVAR_USERDEF = SYSAPI_SYSVAR_USERDEF_START,
	SYSAPI_SYSVAR_USERDEF_END = 0x1FF,
	
	SYSAPI_SYSVAR_MAX
} SYSAPI_SYSVAR;

typedef enum {
	SYSAPI_SYSCMD_NONE = 0,

	SYSAPI_SYSCMD_MAKECALL,

	SYSAPI_SYSCMD_USERDEF = 0x100
} SYSAPI_SYSCMD;

typedef int (SYS_FUNC_INVOKE_SYSGUI)( long lParam );


atLONG	SYSAPI_InitSystem( atLONG lSysInst, atLONG lInitData1, atLONG lInitData2 );
atVOID	SYSAPI_FinalSystem( NOARGS );

atLONG  SYSAPI_GetSystemVar( SYSAPI_SYSVAR sysvar );
atLONG  SYSAPI_SetSystemVar( SYSAPI_SYSVAR sysvar, atLONG value );

atINT   SYSAPI_SysCommand( SYSAPI_SYSCMD cmdCode, atLONG param1, atLONG param2 );
atINT 	SYSAPI_SystemOff( atINT ExitCode );

atINT   SYSAPI_InvokeSysGUI( SYS_FUNC_INVOKE_SYSGUI *fnFunc, atLONG lParam );


#ifdef __cplusplus
}
#endif


#endif //  __SA_SYSTEM_H_
