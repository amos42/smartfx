/**
  @file UI_Debug.h
  @brief Debug에 관한 API
  @remark Debug에 관한 API 모음
  @author Amos
  @date 2010-04-19 오전 11:41:00
  @see UI_Debug.c
*/
#ifndef __UI_DEBUG_H_
#define __UI_DEBUG_H_

#include "UI_Types.h"


#ifdef __cplusplus
extern "C" {
#endif


atBOOL  	    atUIAPI_InitDebug( atLONG lSysInst, atLONG lInitData1, atLONG lInitData2 );
atVOID  	    atUIAPI_FinalDebug(void);

atVOID	atUIAPI_Alert( atLPTSTR szMsg, atINT nType );

atBOOL   atUIAPI_ActivateDebug( atBOOL bActivate );

atINT  	atUIAPI_DebugPrint( const atLPTSTR str );
atINT  	atUIAPI_DebugPrintf( const atLPTSTR fmt, ... );


#ifdef __cplusplus
}
#endif


#endif //  __DEBUG_H_
