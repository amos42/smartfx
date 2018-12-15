/**
  @file UI_Timer.c
  @brief Timer에 관한 API
  @remark Timer에 관한 API 모음
  @author Amos
  @date 2010-04-19 오전 11:41:00
  @see UI_Timer.h
*/
#include "UI_API/UI_API.h"
#include "OEM_UI_Timer.h"
#include "UI_Util.h"


static OEM_UI_TIMER_T tOemFn;

atVOID	OEM_UI_Timer_SetOemFn( OEM_UI_TIMER_T *fnOem )
{
	tOemFn = *fnOem;
}


atVOID	OEM_UI_Timer_GetOemFn( OEM_UI_TIMER_T *fnOem )
{
	*fnOem = tOemFn;
}


/** 
 @brief 
 
 ......

 @param 
 @return 없음
*/
atBOOL  atUIAPI_InitTimer( atLONG lSysInst, atLONG lInitData1, atLONG lInitData2 )
{
	if( tOemFn.fnInitTimer != atNULL ){
	   return tOemFn.fnInitTimer( lSysInst, lInitData1, lInitData2 );
	}

	return atFALSE;
}


/** 
 @brief 
 
 ......

 @param 
 @return 없음
*/
atVOID  atUIAPI_FinalTimer(void)
{
	if( tOemFn.fnFinalTimer != atNULL ){
		tOemFn.fnFinalTimer();
	}
}


/** 
 @brief 
 
 ......

 @param 
 @return 없음
*/
atULONG	 atUIAPI_GetCurrTime(void)
{
	if( tOemFn.fnGetCurrTime != atNULL ){
		return tOemFn.fnGetCurrTime();
	}

	return 0;
}


/** 
 @brief 
 
 ......

 @param 
 @return 없음
*/
atHTIMER  atUIAPI_AddTimer( atLONG interval, void (*callback)(atHTIMER,void*), void *param, atBOOL bRepeat, atBOOL bEnable )
{
	if( tOemFn.fnAddTimer != atNULL ){
		return (atHTIMER)tOemFn.fnAddTimer( interval, callback, param, bRepeat, bEnable );
	}

	return (atHTIMER)atNULL;
}


/** 
 @brief 
 
 ......

 @param 
 @return 없음
*/
atVOID  atUIAPI_RemoveTimer( atHTIMER hTimer )
{
	if( atUIAPI_IsTimerEnabled(hTimer) ) atUIAPI_EnableTimer( hTimer, atFALSE );
	
	if( tOemFn.fnRemoveTimer != atNULL ){
		tOemFn.fnRemoveTimer( (OEM_TIMER)hTimer );
	}
}


/** 
 @brief 
 
 ......

 @param 
 @return 없음
*/
atVOID  atUIAPI_SetTimerParam( atHTIMER hTimer, void *param )
{
	if( tOemFn.fnSetTimerParam != atNULL ){
		tOemFn.fnSetTimerParam( hTimer, param );
	}
}


/** 
 @brief 
 
 ......

 @param 
 @return 없음
*/
atBOOL  atUIAPI_IsTimerEnabled( atHTIMER hTimer )
{
	if( tOemFn.fnIsTimerEnabled != atNULL ){
		return tOemFn.fnIsTimerEnabled( (OEM_TIMER)hTimer );
	}

	return atFALSE;
}


/** 
 @brief 
 
 ......

 @param 
 @return 없음
*/
atVOID  atUIAPI_EnableTimer( atHTIMER hTimer, atBOOL bEnable )
{
	if( tOemFn.fnEnableTimer != atNULL ){
		tOemFn.fnEnableTimer( (OEM_TIMER)hTimer, bEnable );
	}
}


/** 
 @brief 
 
 ......

 @param 
 @return 없음
*/
atVOID  atUIAPI_SetTimerInterval( atHTIMER hTimer, atLONG lInterval )
{
	if( tOemFn.fnSetTimerInterval != atNULL ){
		tOemFn.fnSetTimerInterval( (OEM_TIMER)hTimer, lInterval );
	}
}


/** 
 @brief 
 
 ......

 @param 
 @return 없음
*/
atVOID  atUIAPI_ResetTimer( atHTIMER hTimer )
{
	if( tOemFn.fnResetTimer != atNULL ){
		tOemFn.fnResetTimer( (OEM_TIMER)hTimer );
	}
}


/** 
 @brief 
 
 ......

 @param 
 @return 없음
*/
atVOID  atUIAPI_SuspendTimer( atHTIMER hTimer )
{
	if( tOemFn.fnSuspendTimer != atNULL ){
		tOemFn.fnSuspendTimer( (OEM_TIMER)hTimer );
	}
}


/** 
 @brief 
 
 ......

 @param 
 @return 없음
*/
atVOID  atUIAPI_ResumeTimer( atHTIMER hTimer )
{
	if( tOemFn.fnResumeTimer != atNULL ){
		tOemFn.fnResumeTimer( (OEM_TIMER)hTimer );
	}
}

