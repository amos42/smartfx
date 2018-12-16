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


#define MAX_SOFT_TIMER (64)
#define MINIMUM_TIMER_INTERVAL (10)


typedef struct _TSoftTimer {
	atBOOL	activate;
	int		index;
	int 	is_enabled;
	int 	bRepeat;
	long	interval;
	void (*callback)(atHTIMER, void*);
	void *param;
	int	 is_suspend;
	long call_time;
} TSoftTimer;


static TSoftTimer g_timers[MAX_SOFT_TIMER];
static int g_timer_cnt = 0;
static OEM_TIMER g_timer_handle = atNULL;


static int _get_empty_timer()
{
	int i, idx = -1;
	for (i = 0; i < MAX_SOFT_TIMER; i++) {
		if (!g_timers[i].activate) {
			idx = i;
			break;
		}
	}
	return idx;
}

static atVOID _TimerProc(OEM_TIMER hOemTimer, void *param)
{
	int i, cnt;
	TSoftTimer *p;
	long tm;

	tm = atUIAPI_GetCurrTime();

	cnt = 0;
	for (i = 0; i < MAX_SOFT_TIMER; i++) {
		p = &g_timers[i];
		if (!p->activate) continue;

		if (p->is_enabled && !p->is_suspend) {
			if ((tm - p->call_time) >= p->interval) {
				p->call_time = tm - (tm - p->call_time - p->interval);
				if (!p->bRepeat) p->is_enabled = atFALSE;
				if (p->callback) p->callback((atHTIMER)p, p->param);
				tm = atUIAPI_GetCurrTime();
			}
		}

		if (++cnt >= g_timer_cnt) break;
	}
}


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
	int i;
	atBOOL r;

	g_timer_handle = atNULL;

	g_timer_cnt = 0;
	for (i = 0; i < MAX_SOFT_TIMER; i++) g_timers[i].activate = atFALSE;

	if( tOemFn.fnInitTimer != atNULL ){
	   r = tOemFn.fnInitTimer( lSysInst, lInitData1, lInitData2 );
	   if (!r) return atFALSE;
	}

	if (tOemFn.fnAddTimer != atNULL) {
		g_timer_handle = tOemFn.fnAddTimer(MINIMUM_TIMER_INTERVAL, _TimerProc, 0, atTRUE, atTRUE);
		if (g_timer_handle == atNULL) return atFALSE;
		if (tOemFn.fnEnableTimer != atNULL) {
			tOemFn.fnEnableTimer(g_timer_handle, atTRUE);
		}
	}

	return atTRUE;
}


/** 
 @brief 
 
 ......

 @param 
 @return 없음
*/
atVOID  atUIAPI_FinalTimer(void)
{
	int i;

	if (g_timer_handle != atNULL && tOemFn.fnRemoveTimer != atNULL) {
		tOemFn.fnRemoveTimer(g_timer_handle);
		g_timer_handle = atNULL;
	}

	g_timer_cnt = 0;
	for (i = 0; i < MAX_SOFT_TIMER; i++) g_timers[i].activate = atFALSE;

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
	TSoftTimer *p;
	int idx = _get_empty_timer();
	if (idx < 0) return atNULL;

	p = &g_timers[idx];
	p->index = idx;
	p->interval = interval;
	p->is_enabled = bEnable;
	p->bRepeat = bRepeat;
	p->param = param;
	p->callback = callback;

	p->is_suspend = atFALSE;
	p->call_time = atUIAPI_GetCurrTime();

	p->activate = atTRUE;

	g_timer_cnt++;

	/*
	if( tOemFn.fnAddTimer != atNULL ){
		return (atHTIMER)tOemFn.fnAddTimer( interval, callback, param, bRepeat, bEnable );
	}
	return (atHTIMER)atNULL;
	*/

	return (atHTIMER)p;
}


/** 
 @brief 
 
 ......

 @param 
 @return 없음
*/
atVOID  atUIAPI_RemoveTimer( atHTIMER hTimer )
{
	TSoftTimer *p = (TSoftTimer *)hTimer;
	if (hTimer == atNULL) return;

	p->activate = atFALSE;

	g_timer_cnt--;

	/*
	if( atUIAPI_IsTimerEnabled(hTimer) ) atUIAPI_EnableTimer( hTimer, atFALSE );
	
	if( tOemFn.fnRemoveTimer != atNULL ){
		tOemFn.fnRemoveTimer( (OEM_TIMER)hTimer );
	}
	*/
}


/** 
 @brief 
 
 ......

 @param 
 @return 없음
*/
atVOID  atUIAPI_SetTimerParam( atHTIMER hTimer, void *param )
{
	TSoftTimer *p = (TSoftTimer *)hTimer;
	if (hTimer == atNULL) return;

	p->param = param;
	/*
	if( tOemFn.fnSetTimerParam != atNULL ){
		tOemFn.fnSetTimerParam( hTimer, param );
	}
	*/
}


/** 
 @brief 
 
 ......

 @param 
 @return 없음
*/
atBOOL  atUIAPI_IsTimerEnabled( atHTIMER hTimer )
{
	TSoftTimer *p = (TSoftTimer *)hTimer;
	if (hTimer == atNULL) return atFALSE;

	return p->is_enabled;

	/*
	if( tOemFn.fnIsTimerEnabled != atNULL ){
		return tOemFn.fnIsTimerEnabled( (OEM_TIMER)hTimer );
	}

	return atFALSE;
	*/
}


/** 
 @brief 
 
 ......

 @param 
 @return 없음
*/
atVOID  atUIAPI_EnableTimer( atHTIMER hTimer, atBOOL bEnable )
{
	TSoftTimer *p = (TSoftTimer *)hTimer;
	if (hTimer == atNULL) return;

	if (p->is_enabled == bEnable) return;

	p->is_enabled = bEnable;
	if (bEnable) {
		p->call_time = atUIAPI_GetCurrTime();
	}
	else {
		;
	}

	/*
	if( tOemFn.fnEnableTimer != atNULL ){
		tOemFn.fnEnableTimer( (OEM_TIMER)hTimer, bEnable );
	}
	*/
}


/** 
 @brief 
 
 ......

 @param 
 @return 없음
*/
atVOID  atUIAPI_SetTimerInterval( atHTIMER hTimer, atLONG lInterval )
{
	TSoftTimer *p = (TSoftTimer *)hTimer;
	if (hTimer == atNULL) return;

	if (p->interval == lInterval) return;

	p->interval = lInterval;
	if (p->is_enabled) {
		p->call_time = atUIAPI_GetCurrTime();
	}
	else {
		;
	}

	/*
	if( tOemFn.fnSetTimerInterval != atNULL ){
		tOemFn.fnSetTimerInterval( (OEM_TIMER)hTimer, lInterval );
	}
	*/
}


/** 
 @brief 
 
 ......

 @param 
 @return 없음
*/
atVOID  atUIAPI_ResetTimer( atHTIMER hTimer )
{
	TSoftTimer *p = (TSoftTimer *)hTimer;
	if (hTimer == atNULL) return;

	if (p->is_enabled) {
		p->call_time = atUIAPI_GetCurrTime();
	}
	else {
		;
	}

	/*
	if( tOemFn.fnResetTimer != atNULL ){
		tOemFn.fnResetTimer( (OEM_TIMER)hTimer );
	}
	*/
}


/** 
 @brief 
 
 ......

 @param 
 @return 없음
*/
atVOID  atUIAPI_SuspendTimer( atHTIMER hTimer )
{
	TSoftTimer *p = (TSoftTimer *)hTimer;
	if (hTimer == atNULL) return;

	if (p->is_suspend) return;

	p->is_suspend = atTRUE;

	/*
	if( tOemFn.fnSuspendTimer != atNULL ){
		tOemFn.fnSuspendTimer( (OEM_TIMER)hTimer );
	}
	*/
}


/** 
 @brief 
 
 ......

 @param 
 @return 없음
*/
atVOID  atUIAPI_ResumeTimer( atHTIMER hTimer )
{
	TSoftTimer *p = (TSoftTimer *)hTimer;
	if (hTimer == atNULL) return;

	if (!p->is_suspend) return;

	p->is_suspend = atFALSE;

	/*
	if( tOemFn.fnResumeTimer != atNULL ){
		tOemFn.fnResumeTimer( (OEM_TIMER)hTimer );
	}
	*/
}


/**
 @brief

 ......

 @param
 @return 없음
*/
atVOID  atUIAPI_TickTimer( atLONG nMilliSec )
{

}

