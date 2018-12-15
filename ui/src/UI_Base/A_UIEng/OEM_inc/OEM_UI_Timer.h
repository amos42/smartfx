/**
  @file OEM_UI_Timer.h
  @brief Timer에 관한 포팅 OEM API 선언
  @remark Timer에 관한 포팅 OEM API 선언
  @author Amos
  @date 2010-04-19 오전 11:41:00
  @see UI_Timer.h
*/
#ifndef __OEM_UI_TIMER_H_
#define __OEM_UI_TIMER_H_

#include "UI_Types.h"


#ifdef __cplusplus
extern "C" {
#endif


typedef void *OEM_TIMER;


typedef atBOOL      FN_OEM_UI_InitTimer( atLONG lSysInst, atLONG lInitData1, atLONG lInitData2 );
typedef atVOID      FN_OEM_UI_FinalTimer(void);
typedef atULONG	    FN_OEM_UI_GetCurrTime(void);
typedef OEM_TIMER   FN_OEM_UI_AddTimer( atLONG interval, void (*callback)(OEM_TIMER,void*), void *param, atBOOL bRepeat, atBOOL bEnable );
typedef atVOID      FN_OEM_UI_RemoveTimer( OEM_TIMER hTimer );
typedef atVOID		FN_OEM_UI_SetTimerParam( OEM_TIMER hTimer, atVOID *param );
typedef atBOOL      FN_OEM_UI_IsTimerEnabled( OEM_TIMER hTimer );
typedef atVOID      FN_OEM_UI_EnableTimer( OEM_TIMER hTimer, atBOOL bEnable );
typedef atVOID      FN_OEM_UI_SetTimerInterval( OEM_TIMER hTimer, atLONG lInterval );
typedef atVOID      FN_OEM_UI_ResetTimer( OEM_TIMER hTimer );
typedef atVOID     	FN_OEM_UI_SuspendTimer( OEM_TIMER hTimer );
typedef atVOID      FN_OEM_UI_ResumeTimer( OEM_TIMER hTimer );


typedef struct tagOEM_UI_TIMER {
	FN_OEM_UI_InitTimer				*fnInitTimer;
	FN_OEM_UI_FinalTimer			*fnFinalTimer;
	FN_OEM_UI_GetCurrTime			*fnGetCurrTime;
	FN_OEM_UI_AddTimer				*fnAddTimer;
	FN_OEM_UI_RemoveTimer			*fnRemoveTimer;
	FN_OEM_UI_SetTimerParam			*fnSetTimerParam;
	FN_OEM_UI_IsTimerEnabled		*fnIsTimerEnabled;
	FN_OEM_UI_EnableTimer			*fnEnableTimer;
	FN_OEM_UI_SetTimerInterval		*fnSetTimerInterval;
	FN_OEM_UI_ResetTimer			*fnResetTimer;
	FN_OEM_UI_SuspendTimer			*fnSuspendTimer;
	FN_OEM_UI_ResumeTimer			*fnResumeTimer;
} OEM_UI_TIMER_T;


atVOID	OEM_UI_Timer_SetOemFn( OEM_UI_TIMER_T *fnOem );
atVOID	OEM_UI_Timer_GetOemFn( OEM_UI_TIMER_T *fnOem );


#ifdef __cplusplus
}
#endif


#endif 
