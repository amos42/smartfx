//
// OEM_SYSAPI_SYSTEM.h
//
////////////////////////////////////////////////////////////////

#ifndef __OEM_SA_SYSTEM_H_
#define __OEM_SA_SYSTEM_H_


#include "SA_Types.h"
#include "SYSAPI.h"


#ifdef __cplusplus
extern "C" {
#endif


typedef atLONG	FN_OEM_SYSAPI_InitSystem( atLONG lSysInst, atLONG lInitData1, atLONG lInitData2 );
typedef void	FN_OEM_SYSAPI_FinalSystem(void);

typedef atLONG	FN_OEM_SYSAPI_GetSystemVar(SYSAPI_SYSVAR sysvar);
typedef atLONG	FN_OEM_SYSAPI_SetSystemVar(SYSAPI_SYSVAR sysvar, atLONG value);

typedef atINT	FN_OEM_SYSAPI_SystemOff(atINT ExitCode);

typedef atINT   FN_OEM_SYSAPI_InvokeSysGUI( SYS_FUNC_INVOKE_SYSGUI *fnFunc, atLONG lParam );

typedef atINT	FN_OEM_SYSAPI_SysCommand(SYSAPI_SYSCMD cmdCode, atLONG param1, atLONG param2);


typedef struct tagOEM_SYSAPI_SYSTEM_T {
	FN_OEM_SYSAPI_InitSystem			*fnInitSystem;
	FN_OEM_SYSAPI_FinalSystem			*fnFinalSystem;

	FN_OEM_SYSAPI_GetSystemVar			*fnGetSystemVar;
	FN_OEM_SYSAPI_SetSystemVar			*fnSetSystemVar;

	FN_OEM_SYSAPI_SystemOff				*fnSystemOff;

	FN_OEM_SYSAPI_InvokeSysGUI			*fnInvokeSysGUI;

	FN_OEM_SYSAPI_SysCommand			*fnSysCommand;
} OEM_SYSAPI_SYSTEM_T;


atVOID	OEM_SYSAPI_SYSTEM_SetOemFn( OEM_SYSAPI_SYSTEM_T *fnOem );
atVOID	OEM_SYSAPI_SYSTEM_GetOemFn( OEM_SYSAPI_SYSTEM_T *fnOem );


#ifdef __cplusplus
}
#endif


#endif // __OEM_SA_SYSTEM_H_
