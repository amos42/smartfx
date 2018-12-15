//
// OEM_SYSAPI_Debug.h
//
////////////////////////////////////////////////////////////////

#ifndef __OEM_SA_DEBUG_H_
#define __OEM_SA_DEBUG_H_

#include "AT_Types.h"


#ifdef __cplusplus
  extern "C" {
#endif


typedef atBOOL  FN_OEM_SYSAPI_InitDebug( atLONG lSysInst, atLONG lInitData1, atLONG lInitData2 );
typedef atVOID  FN_OEM_SYSAPI_FinalDebug(void);

typedef atVOID FN_OEM_SYSAPI_Alert( const atLPTSTR szMsg );

typedef atBOOL FN_OEM_SYSAPI_ActivateDebug( atBOOL bActivate );

typedef atINT  FN_OEM_SYSAPI_DebugPrint( const atLPTSTR str );


typedef struct tagOEM_SYSAPI_DEBUG_T {
	FN_OEM_SYSAPI_InitDebug			*fnInitDebug;
	FN_OEM_SYSAPI_FinalDebug		*fnFinalDebug;

	FN_OEM_SYSAPI_Alert				*fnAlert;

	FN_OEM_SYSAPI_ActivateDebug		*fnActivateDebug;

	FN_OEM_SYSAPI_DebugPrint		*fnDebugPrint;
} OEM_SYSAPI_DEBUG_T;


atVOID	OEM_SYSAPI_DEBUG_SetOemFn( OEM_SYSAPI_DEBUG_T *fnOem );
atVOID	OEM_SYSAPI_DEBUG_GetOemFn( OEM_SYSAPI_DEBUG_T *fnOem );


#ifdef __cplusplus
}
#endif


#endif //  __OEM_SYSAPI_DEBUG_H_
