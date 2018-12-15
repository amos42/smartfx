#include "A_UIENG.h"
#include "UI_API/UI_API.h"
#include "OEM_UI_Timer.h"
#include "SYSAPI.h"


static atBOOL  OEM_UI_InitTimer( atLONG lSysInst, atLONG lInitData1, atLONG lInitData2 )
{
// Memory, String, File, Debug, Task, Timer, Input, Sound, GPS는 시스템에서 초기화를 관리한다.
//	return SYSAPI_InitTimer( lSysInst, lInitData1, lInitData2 );
	return atTRUE;
}


static atVOID  OEM_UI_FinalTimer( NOARGS )
{
// Memory, String, File, Debug, Task, Timer, Input, Sound, GPS는 시스템에서 초기화를 관리한다.
//	SYSAPI_FinalTimer();
}


static atULONG	OEM_UI_GetCurrTime( NOARGS )
{
	return SYSAPI_GetCurrTime();
}

static OEM_TIMER    OEM_UI_AddTimer( atLONG interval, atVOID (*callback)(OEM_TIMER,atVOID*), atVOID *param, atBOOL bRepeat, atBOOL bEnable )
{
	return (OEM_TIMER)SYSAPI_AddTimer( interval, callback, param, bRepeat, bEnable );
}

static atVOID      OEM_UI_RemoveTimer( OEM_TIMER hTimer )
{
	SYSAPI_RemoveTimer( (SYS_TIMER)hTimer );
}

static atVOID      OEM_UI_SetTimerParam( OEM_TIMER hTimer, atVOID *lpParam )
{
	SYSAPI_SetTimerParam( (SYS_TIMER)hTimer, lpParam );
}

static atBOOL      OEM_UI_IsTimerEnabled( OEM_TIMER hTimer )
{
	return SYSAPI_IsTimerEnabled((SYS_TIMER)hTimer);
}

static atVOID      OEM_UI_EnableTimer( OEM_TIMER hTimer, atBOOL bEnable )
{
	SYSAPI_EnableTimer( hTimer, bEnable );
}

static atVOID      OEM_UI_SetTimerInterval( OEM_TIMER hTimer, atLONG lInterval )
{
	SYSAPI_SetTimerInterval( (SYS_TIMER)hTimer, lInterval );
}

static atVOID      OEM_UI_ResetTimer( OEM_TIMER hTimer )
{
	SYSAPI_ResetTimer( (SYS_TIMER)hTimer );
}

static atVOID     	OEM_UI_SuspendTimer( OEM_TIMER hTimer )
{
	SYSAPI_SuspendTimer( (SYS_TIMER)hTimer );
}

static atVOID      OEM_UI_ResumeTimer( OEM_TIMER hTimer )
{
	SYSAPI_ResumeTimer( (SYS_TIMER)hTimer );
}


void init_uiapi_timer_oem() 
{
	OEM_UI_TIMER_T oem_funcs = {
		OEM_UI_InitTimer,
		OEM_UI_FinalTimer,
		OEM_UI_GetCurrTime,
		OEM_UI_AddTimer,
		OEM_UI_RemoveTimer,
		OEM_UI_SetTimerParam,
		OEM_UI_IsTimerEnabled,
		OEM_UI_EnableTimer,
		OEM_UI_SetTimerInterval,
		OEM_UI_ResetTimer,
		OEM_UI_SuspendTimer,
		OEM_UI_ResumeTimer
	};

	OEM_UI_Timer_SetOemFn( &oem_funcs );
}
