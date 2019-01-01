/**
  @file OEM_UI_Debug.h
  @brief Debug에 관한 포팅 OEM API 선언
  @remark Debug에 관한 포팅 OEM API 선언
  @author Amos
  @date 2010-04-19 오전 11:41:00
  @see UI_Debug.h
*/
#ifndef __OEM_UI_DEBUG_H_
#define __OEM_UI_DEBUG_H_

#include "AT_Types.h"


#ifdef __cplusplus
  extern "C" {
#endif


typedef atBOOL  FN_OEM_UI_InitDebug( atLONG lSysInst, atLONG lInitData1, atLONG lInitData2 );
typedef atVOID  FN_OEM_UI_FinalDebug(void);

typedef atBOOL	FN_OEM_UI_ActivateDebug( atBOOL bActivate );	
typedef atVOID	FN_OEM_UI_Alert( const atLPTSTR szMsg, atINT nType );
typedef atINT	FN_OEM_UI_DebugPrint( const atLPTSTR str );


typedef struct tagOEM_UI_DEBUG {
	FN_OEM_UI_InitDebug		*fnInitDebug;
	FN_OEM_UI_FinalDebug	*fnFinalDebug;

	FN_OEM_UI_ActivateDebug *fnActivateDebug;
	FN_OEM_UI_Alert			*fnAlert;
	FN_OEM_UI_DebugPrint	*fnDebugPrint;
} OEM_UI_DEBUG_T;


atVOID	OEM_UI_Debug_SetOemFn( OEM_UI_DEBUG_T *fnOem );
atVOID	OEM_UI_Debug_GetOemFn( OEM_UI_DEBUG_T *fnOem );


#ifdef __cplusplus
}
#endif


#endif //  __OEM_DEBUG_H_
