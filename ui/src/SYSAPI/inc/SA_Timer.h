
#ifndef __SA_TIMER_H_
#define __SA_TIMER_H_

#include "SA_Types.h"


#ifdef __cplusplus
extern "C" {
#endif
	

typedef atHANDLE SYS_TIMER;


atBOOL  	SYSAPI_InitTimer( atLONG lSysInst, atLONG lInitData1, atLONG lInitData2 );
atVOID  	SYSAPI_FinalTimer( NOARGS );
atULONG		SYSAPI_GetCurrTime( NOARGS );

SYS_TIMER    SYSAPI_AddTimer(atLONG interval, void (*callback)(SYS_TIMER,void*), void *param, atBOOL bRepeat, atBOOL bEnable );
atVOID      SYSAPI_RemoveTimer( SYS_TIMER hTimer );
atVOID    SYSAPI_SetTimerParam( SYS_TIMER hTimer, void *param );
atBOOL      SYSAPI_IsTimerEnabled( SYS_TIMER hTimer );
atVOID      SYSAPI_EnableTimer( SYS_TIMER hTimer, atBOOL bEnable );
atVOID      SYSAPI_SetTimerInterval( SYS_TIMER hTimer, atLONG lInterval );
atVOID      SYSAPI_ResetTimer( SYS_TIMER hTimer );

atVOID     	SYSAPI_SuspendTimer( SYS_TIMER hTimer );
atVOID      SYSAPI_ResumeTimer( SYS_TIMER hTimer );


#ifdef __cplusplus
}
#endif


#endif

