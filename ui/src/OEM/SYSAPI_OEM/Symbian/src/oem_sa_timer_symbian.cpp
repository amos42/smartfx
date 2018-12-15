#include "SYSAPI.h"
#include "OEM_SA_Timer.h"
//#include "A_VD.h"
//#include "AEEEmulAPI.h"
#include "OSAL/_MPTimer.h"
#include <hal.h>


static TInt g_nanokernel_tick_period;


atBOOL  OEM_SYSAPI_InitTimer( atLONG lSysInst, atLONG lInitData1, atLONG lInitData2 )
{
	HAL::Get(HAL::ENanoTickPeriod, g_nanokernel_tick_period);

	return atTRUE;
}


atVOID  OEM_SYSAPI_FinalTimer( NOARGS )
{

}


atULONG	OEM_SYSAPI_GetCurrTime( NOARGS )
{
//	return User::NTickCount() * g_nanokernel_tick_period / 1000000L;
	return User::NTickCount() * g_nanokernel_tick_period / 1000L;
}

OEM_SYS_TIMER    OEM_SYSAPI_AddTimer( atLONG atINTerval, atVOID (*callback)(OEM_SYS_TIMER,atVOID*), atVOID *param, atBOOL bRepeat, atBOOL bEnable )
{
	CMTimer* tmr = atNULL;
	
    tmr = CMTimer::NewL( atINTerval, callback, param, bRepeat );  // 최소 주기 10ms
    
    if( tmr )
    	tmr->EnableTimer( bEnable );
    else
    	return atNULL;

	return (OEM_SYS_TIMER)tmr;

}

atVOID      OEM_SYSAPI_RemoveTimer( OEM_SYS_TIMER hTimer )
{
	CMTimer* tmr = (CMTimer*)hTimer;
	if( tmr == atNULL ) return;
    
	CMTimer::Delete( tmr );

}

/*
atVOID      OEM_SYSAPI_SetTimerParam( OEM_SYS_TIMER hTimer, atVOID *lpParam )
{
	CMTimer* tmr = (CMTimer*)hTimer;
	if( tmr == atNULL ) return;
    
	tmr->SetParam( lpParam );
}
*/

atBOOL      OEM_SYSAPI_IsTimerEnabled( OEM_SYS_TIMER hTimer )
{
	CMTimer* tmr = (CMTimer*)hTimer;
	if( tmr == atNULL ) return atFALSE;

	return (atBOOL)tmr->GetTimerStatus();
	
}

atVOID      OEM_SYSAPI_EnableTimer( OEM_SYS_TIMER hTimer, atBOOL bEnable )
{
	CMTimer* tmr = (CMTimer*)hTimer;
	if( tmr == atNULL ) return;

	tmr->EnableTimer( bEnable );
	
}

atVOID      OEM_SYSAPI_SetTimerInterval( OEM_SYS_TIMER hTimer, atLONG lInterval )
{
	CMTimer* tmr = (CMTimer*)hTimer;
	if( tmr == atNULL ) return;
	
	tmr->StartTimer( lInterval );

}

atVOID      OEM_SYSAPI_ResetTimer( OEM_SYS_TIMER hTimer )
{
	CMTimer* tmr = (CMTimer*)hTimer;
	if( tmr == atNULL ) return;

	tmr->ResetTimer();

}

atVOID     	OEM_SYSAPI_SuspendTimer( OEM_SYS_TIMER hTimer )
{
	CMTimer* tmr = (CMTimer*)hTimer;
	if( tmr == atNULL ) return;

	tmr->EnableTimer( atFALSE );
	
}

atVOID      OEM_SYSAPI_ResumeTimer( OEM_SYS_TIMER hTimer )
{
	CMTimer* tmr = (CMTimer*)hTimer;
	if( tmr == atNULL ) return;

	tmr->EnableTimer( atTRUE );

}

