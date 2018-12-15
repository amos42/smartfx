/**
  @file UI_Debug.c
  @brief Debug�� ���� API
  @remark Debug�� ���� API ����
  @author Amos
  @date 2010-04-19 ���� 11:41:00
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
 @brief Debug �ý����� �ʱ�ȭ �Ѵ�. (���ο�)
 
 Debug �ý����� �ʱ�ȭ �Ѵ�.

 @param lSysInst [in] : UI������ �ν��Ͻ� (OEM ���޿�. ���ο�)
 @param lInitData1, lInitData2 [in] : �ʱ�ȭ �Ķ���� (OEM ���޿�. ���ο�)
 @return �����ϸ� atTRUE, �����ϸ� atFALSE
 @remark ���ο����� ���ȴ�.
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
 @return ����
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
 @return ����
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
 @return ����
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
 @return ����
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
 @return ����
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

