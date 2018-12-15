// OEM_SYSAPI_SYSTEM.h
//
////////////////////////////////////////////////////////////////

#ifndef __OEM_SA_TIMER_H_
#define __OEM_SA_TIMER_H_


#include "SA_Types.h"


#ifdef __cplusplus
extern "C" {
#endif


typedef void *OEM_SYS_TIMER;


typedef atBOOL          FN_OEM_SYSAPI_InitTimer( atLONG lSysInst, atLONG lInitData1, atLONG lInitData2 );
typedef atVOID          FN_OEM_SYSAPI_FinalTimer(void);
typedef atULONG	        FN_OEM_SYSAPI_GetCurrTime(void);
typedef OEM_SYS_TIMER   FN_OEM_SYSAPI_AddTimer( atLONG interval, void (*callback)(OEM_SYS_TIMER,void*), void *param, atBOOL bRepeat, atBOOL bEnable );
typedef atVOID          FN_OEM_SYSAPI_RemoveTimer( OEM_SYS_TIMER hTimer );
typedef atBOOL          FN_OEM_SYSAPI_IsTimerEnabled( OEM_SYS_TIMER hTimer );
typedef atVOID          FN_OEM_SYSAPI_EnableTimer( OEM_SYS_TIMER hTimer, atBOOL bEnable );
typedef atVOID          FN_OEM_SYSAPI_SetTimerInterval( OEM_SYS_TIMER hTimer, atLONG lInterval );
typedef atVOID          FN_OEM_SYSAPI_ResetTimer( OEM_SYS_TIMER hTimer );
typedef atVOID     	    FN_OEM_SYSAPI_SuspendTimer( OEM_SYS_TIMER hTimer );
typedef atVOID          FN_OEM_SYSAPI_ResumeTimer( OEM_SYS_TIMER hTimer );


typedef struct tagOEM_SYSAPI_TIMER_T {
    FN_OEM_SYSAPI_InitTimer          *fnInitTimer;          
    FN_OEM_SYSAPI_FinalTimer         *fnFinalTimer;         
    FN_OEM_SYSAPI_GetCurrTime        *fnGetCurrTime;        
    FN_OEM_SYSAPI_AddTimer           *fnAddTimer;           
    FN_OEM_SYSAPI_RemoveTimer        *fnRemoveTimer;        
    FN_OEM_SYSAPI_IsTimerEnabled     *fnIsTimerEnabled;     
    FN_OEM_SYSAPI_EnableTimer        *fnEnableTimer;        
    FN_OEM_SYSAPI_SetTimerInterval   *fnSetTimerInterval;   
    FN_OEM_SYSAPI_ResetTimer         *fnResetTimer;         
    FN_OEM_SYSAPI_SuspendTimer       *fnSuspendTimer;       
    FN_OEM_SYSAPI_ResumeTimer        *fnResumeTimer;        
} OEM_SYSAPI_TIMER_T;


atVOID	OEM_SYSAPI_TIMER_SetOemFn( OEM_SYSAPI_TIMER_T *fnOem );
atVOID	OEM_SYSAPI_TIMER_GetOemFn( OEM_SYSAPI_TIMER_T *fnOem );


#ifdef __cplusplus
}
#endif


#endif // __OEM_SA_SYSTEM_H_
