/**
  @file UI_Debug.c
  @brief Debug에 관한 API
  @remark Debug에 관한 API 모음
  @author Amos
  @date 2010-04-19 오전 11:41:00
  @see UI_Debug.h
*/
#include "UI_API/UI_API.h"
#include "OEM_UI_Debug.h"


#ifdef __USES_UI_DEBUG_

#include <stdarg.h>


static OEM_UI_DEBUG_T tOemFn;

atVOID	OEM_UI_Debug_SetOemFn( OEM_UI_DEBUG_T *fnOem )
{
	tOemFn = *fnOem;
}


atVOID	OEM_UI_Debug_GetOemFn( OEM_UI_DEBUG_T *fnOem )
{
	*fnOem = tOemFn;
}


/** 
 @brief Debug 시스템을 초기화 한다. (내부용)
 
 Debug 시스템을 초기화 한다.

 @param lSysInst [in] : UI엔진의 인스턴스 (OEM 전달용. 내부용)
 @param lInitData1, lInitData2 [in] : 초기화 파라미터 (OEM 전달용. 내부용)
 @return 성공하면 atTRUE, 실패하면 atFALSE
 @remark 내부용으로 사용된다.
*/
atVOID atUIAPI_Alert( atLPTSTR szMsg, atINT nType )
{
	if( tOemFn.fnAlert != atNULL ){
		tOemFn.fnAlert(szMsg, nType);
	}
}


/** 
 @brief 
 
 ......

 @param 
 @return 없음
*/
atBOOL  atUIAPI_InitDebug( atLONG lSysInst, atLONG lInitData1, atLONG lInitData2 )
{
	if( tOemFn.fnInitDebug != atNULL ){
		return tOemFn.fnInitDebug( lSysInst, lInitData1, lInitData2 );
	}

	return atFALSE;
}


/** 
 @brief 
 
 ......

 @param 
 @return 없음
*/
atVOID  atUIAPI_FinalDebug(void)
{
	if( tOemFn.fnFinalDebug != atNULL ){
		tOemFn.fnFinalDebug();
	}
}


/** 
 @brief 
 
 ......

 @param 
 @return 없음
*/
atBOOL atUIAPI_ActivateDebug( atBOOL bActivate )
{
	if( tOemFn.fnActivateDebug != atNULL ){
		return tOemFn.fnActivateDebug( bActivate );
	}

	return atFALSE;
}


/** 
 @brief 
 
 ......

 @param 
 @return 없음
*/
atINT  atUIAPI_DebugPrint( const atLPTSTR str )
{
	if( tOemFn.fnDebugPrint != atNULL ){
		return tOemFn.fnDebugPrint( str );
	}

	return 0;
}


/** 
 @brief 
 
 ......

 @param 
 @return 없음
*/
atINT  atUIAPI_DebugPrintf( const atLPTSTR fmt, ... )
{
	atTCHAR str[256];
    va_list ap;
    int len;

    va_start(ap, fmt);
    len = atUIAPI_vsprintf( str, fmt, ap );
    va_end(ap);

	return atUIAPI_DebugPrint( str );
}

#endif

