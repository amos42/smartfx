#include "SYSAPI.h"
#include "OEM_SA_Timer.h"


atBOOL  OEM_SYSAPI_InitTimer( atLONG lSysInst, atLONG lInitData1, atLONG lInitData2 )
{
	return atFALSE;
}

atVOID  OEM_SYSAPI_FinalTimer(void)
{
}

atULONG	OEM_SYSAPI_GetCurrTime(void)
{
	return 0;
}

OEM_SYS_TIMER    OEM_SYSAPI_AddTimer( atLONG interval, void (*callback)(OEM_SYS_TIMER,void*), void *param, atBOOL bRepeat, atBOOL bEnable )
{
	return (OEM_SYS_TIMER)atNULL;
}

atVOID      OEM_SYSAPI_RemoveTimer( OEM_SYS_TIMER hTimer )
{
}

atBOOL      OEM_SYSAPI_IsTimerEnabled( OEM_SYS_TIMER hTimer )
{
	return atFALSE;
}

atVOID      OEM_SYSAPI_EnableTimer( OEM_SYS_TIMER hTimer, atBOOL bEnable )
{
}

atVOID      OEM_SYSAPI_SetTimerInterval( OEM_SYS_TIMER hTimer, atLONG lInterval )
{
}

atVOID      OEM_SYSAPI_ResetTimer( OEM_SYS_TIMER hTimer )
{
}

atVOID     	OEM_SYSAPI_SuspendTimer( OEM_SYS_TIMER hTimer )
{
}

atVOID      OEM_SYSAPI_ResumeTimer( OEM_SYS_TIMER hTimer )
{
}

