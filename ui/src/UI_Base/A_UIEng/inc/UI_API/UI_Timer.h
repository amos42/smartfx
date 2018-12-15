/**
  @file UI_Timer.h
  @brief Timer에 관한 API
  @remark Timer에 관한 API 모음
  @author Amos
  @date 2010-04-19 오전 11:41:00
  @see UI_Timer.c
*/
#ifndef __UI_TIMER_H_
#define __UI_TIMER_H_

#include "UI_Types.h"


#ifdef __cplusplus
extern "C" {
#endif
	

typedef atHANDLE atHTIMER;


atBOOL  	atUIAPI_InitTimer( atLONG lSysInst, atLONG lInitData1, atLONG lInitData2 );
atVOID  	atUIAPI_FinalTimer(void);
atULONG		atUIAPI_GetCurrTime(void);

atHTIMER    atUIAPI_AddTimer( long interval, void (*callback)(atHTIMER,void*), void *param, atBOOL bRepeat, atBOOL bEnable );
atVOID      atUIAPI_RemoveTimer( atHTIMER hTimer );
atVOID    atUIAPI_SetTimerParam( atHTIMER hTimer, void *param );
atBOOL      atUIAPI_IsTimerEnabled( atHTIMER hTimer );
atVOID      atUIAPI_EnableTimer( atHTIMER hTimer, atBOOL bEnable );
atVOID      atUIAPI_SetTimerInterval( atHTIMER hTimer, atLONG lInterval );
atVOID      atUIAPI_ResetTimer( atHTIMER hTimer );

atVOID     	atUIAPI_SuspendTimer( atHTIMER hTimer );
atVOID      atUIAPI_ResumeTimer( atHTIMER hTimer );


#ifdef __cplusplus
}
#endif


#endif

