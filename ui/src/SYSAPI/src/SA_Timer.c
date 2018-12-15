#include "SYSAPI.h"
#include "OEM_SA_Timer.h"


#ifdef __USES_SYSAPI_TIMER_


static atBOOL g_bSysTimerIntitialized = atFALSE;


typedef struct tagTSysTimer {
	OEM_SYS_TIMER oem_timer;
	int interval;
	int is_enabled;
	int bRepeat;
	void (*callback)(SYS_TIMER,void*);
	void *param;
} TSysTimer;

static atVOID _TimerProc( OEM_SYS_TIMER oem_timer, void *param )
{
	TSysTimer *p = (TSysTimer *)param;

	p->callback( (SYS_TIMER)p, p->param );
}

static TSysTimer *_AllocTimer(void)
{
	return (TSysTimer *)SYSAPI_AllocMem( sizeof(TSysTimer) );
}

static void _FreeTimer( TSysTimer *tmr )
{
	SYSAPI_FreeMem( (void *)tmr );
}

 
static OEM_SYSAPI_TIMER_T tOemFn;


atVOID OEM_SYSAPI_TIMER_SetOemFn(OEM_SYSAPI_TIMER_T *fnOem)
{
	tOemFn = *fnOem;
}


atVOID OEM_SYSAPI_TIMER_GetOemFn(OEM_SYSAPI_TIMER_T *fnOem)
{
	*fnOem = tOemFn;
}


atBOOL  SYSAPI_InitTimer( atLONG lSysInst, atLONG lInitData1, atLONG lInitData2 )
{
	if( g_bSysTimerIntitialized ) return atTRUE;
	
	if( tOemFn.fnInitTimer != atNULL ){
		g_bSysTimerIntitialized = tOemFn.fnInitTimer( lSysInst, lInitData1, lInitData2 );
	}
	
	return g_bSysTimerIntitialized;
}


atVOID  SYSAPI_FinalTimer( NOARGS )
{
	if( !g_bSysTimerIntitialized ) return;
	
	if( tOemFn.fnFinalTimer != atNULL ){
		tOemFn.fnFinalTimer();
	}
	
	g_bSysTimerIntitialized = atFALSE;
}


atULONG	SYSAPI_GetCurrTime( NOARGS )
{
	if( tOemFn.fnGetCurrTime != atNULL ){
		return tOemFn.fnGetCurrTime();
	}

	return 0;
}


SYS_TIMER    SYSAPI_AddTimer( atLONG interval, void (*callback)(SYS_TIMER,void*), void *param, atBOOL bRepeat, atBOOL bEnable )
{
	TSysTimer *p = _AllocTimer();

	p->interval = interval;
	p->is_enabled = bEnable;
	p->bRepeat = bRepeat;
	p->param = param;
	p->callback = callback;

	if( tOemFn.fnAddTimer != atNULL ){
		p->oem_timer = (void *)tOemFn.fnAddTimer( interval, _TimerProc, (void *)p, bRepeat, bEnable );
	} else {
		p->oem_timer = atNULL;
	}

	return (SYS_TIMER)p;
}

atVOID      SYSAPI_RemoveTimer( SYS_TIMER hTimer )
{
	TSysTimer *p = (TSysTimer *)hTimer;
	if( p == atNULL ) return;
		
	if( SYSAPI_IsTimerEnabled(hTimer) ) SYSAPI_EnableTimer( hTimer, atFALSE );
	
	if( tOemFn.fnRemoveTimer != atNULL ){
		tOemFn.fnRemoveTimer( p->oem_timer );
	}

	_FreeTimer( p );
}

atVOID    SYSAPI_SetTimerParam( SYS_TIMER hTimer, void *param )
{
	TSysTimer *p = (TSysTimer *)hTimer;
	if( p == atNULL ) return;

	p->param = param;
}

atBOOL      SYSAPI_IsTimerEnabled( SYS_TIMER hTimer )
{
	TSysTimer *p = (TSysTimer *)hTimer;
	if( p == atNULL ) return atFALSE;
		
	if( tOemFn.fnIsTimerEnabled != atNULL ){
		return tOemFn.fnIsTimerEnabled( p->oem_timer );
	}

	return atFALSE;
}

atVOID      SYSAPI_EnableTimer( SYS_TIMER hTimer, atBOOL bEnable )
{
	TSysTimer *p = (TSysTimer *)hTimer;
	if( p == atNULL ) return;
		
	if( tOemFn.fnEnableTimer != atNULL ){
		tOemFn.fnEnableTimer( p->oem_timer, bEnable );
	}
}

atVOID      SYSAPI_SetTimerInterval( SYS_TIMER hTimer, atLONG lInterval )
{
	TSysTimer *p = (TSysTimer *)hTimer;
	if( p == atNULL ) return;
		
	if( tOemFn.fnSetTimerInterval != atNULL ){
		tOemFn.fnSetTimerInterval( p->oem_timer, lInterval );
	}
}

atVOID      SYSAPI_ResetTimer( SYS_TIMER hTimer )
{
	TSysTimer *p = (TSysTimer *)hTimer;
	if( p == atNULL ) return;
		
	if( tOemFn.fnResetTimer != atNULL ){
		tOemFn.fnResetTimer( p->oem_timer );
	}
}

atVOID     	SYSAPI_SuspendTimer( SYS_TIMER hTimer )
{
	TSysTimer *p = (TSysTimer *)hTimer;
	if( p == atNULL ) return;
		
	if( tOemFn.fnSuspendTimer != atNULL ){
		tOemFn.fnSuspendTimer( p->oem_timer );
	}
}

atVOID      SYSAPI_ResumeTimer( SYS_TIMER hTimer )
{
	TSysTimer *p = (TSysTimer *)hTimer;
	if( p == atNULL ) return;
		
	if( tOemFn.fnResumeTimer != atNULL ){
		tOemFn.fnResumeTimer( p->oem_timer );
	}
}


#endif
