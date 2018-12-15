#include "SYSAPI.h"
#include "OEM_SA_Timer.h"
#include <linux/sched.h>
#include <time.h>

#include <gtk/gtk.h>

static GTimer *g_hTimer = atNULL;

atBOOL  OEM_SYSAPI_InitTimer( atLONG lSysInst, atLONG lInitData1, atLONG lInitData2 )
{
	// 타이머 인스턴스 생성
	g_hTimer = g_timer_new();
	if(g_hTimer == atNULL) return atFALSE;

	// 타이머 작동
	g_timer_start(g_hTimer);
	return atTRUE;
}

atVOID  OEM_SYSAPI_FinalTimer(void)
{
	if(g_hTimer == atNULL) return;

	// 타미어 작동중지
	g_timer_stop(g_hTimer);
	// 타이머 인스턴스 시스템반환
	g_timer_destroy(g_hTimer);
	g_hTimer = atNULL;
}

atULONG	OEM_SYSAPI_GetCurrTime(void)
{
	if(g_hTimer)
	{
		gdouble sec;
		gulong microsec;

		// 타이머 작동 이후의 경과된 시간을 구한다. (초 단위는 리턴되고, 마이크로 단위는 레퍼런스로 구함)
		sec = g_timer_elapsed(g_hTimer, &microsec);
		return (atULONG)((glong)sec * 1000 + microsec / 1000);	// 소수 곱셈 X
		//return (atULONG)((glong)(sec * 1000) + microsec / 1000); // 소수 곰셉 O
	}

	return 0;
}

typedef struct {
	int interval;
	int is_enabled;
	int bRepeat;
	int timer_id;
	void (*callback)(OEM_SYS_TIMER,void*);
	void *param;
} xTTimer;

static void _KillTimer(xTTimer *p)
{
	if(p == atNULL || p->timer_id == 0) return;

	// GTimer 에 등록된 Callback 함수를 제거한다.
	gtk_timeout_remove(p->timer_id);
	p->timer_id = 0;
}

static void _TimerProc(gpointer param)
{
	xTTimer *p = (xTTimer *)param;
	if(p == atNULL) return;

	// 실제로 Callback 되어질 함수를 콜한다.
	if(p->callback) p->callback(p, p->param);
	// 반복성 Timer 가 아니라면 GTimer 에 등록된 Callback 함수를 제거한다.
	if(!p->bRepeat) _KillTimer(p);
}

static void _SetTimer(xTTimer *p)
{
	if(p == atNULL) return;
	// GTimer 에 Callback 함수를 등록시킨다.
	p->timer_id = gtk_timeout_add(p->interval, (GtkFunction)_TimerProc, p);
}

OEM_SYS_TIMER    OEM_SYSAPI_AddTimer( atLONG interval, void (*callback)(OEM_SYS_TIMER,void*), void *param, atBOOL bRepeat, atBOOL bEnable )
{
	// OEM_SYS_TIMER 를 메모리 할당한다.
	xTTimer *p = SYSAPI_AllocMem( sizeof(xTTimer) );
	if(p == atNULL) return NULL;

	p->interval = interval;
	p->is_enabled = bEnable;
	p->bRepeat = bRepeat;
	p->param = param;
	p->callback = callback;
	p->timer_id = 0;

	// 활성화 Timer 이라면 GTimer 에 Callback 함수를 등록시킨다.
	if( p->is_enabled ) _SetTimer(p);

	return (OEM_SYS_TIMER)p;
}

atVOID      OEM_SYSAPI_RemoveTimer( OEM_SYS_TIMER hTimer )
{
	xTTimer *p = (xTTimer *)hTimer;
	if(p == atNULL) return;

	// GTimer 에 등록된 Callback 함수를 제거한다.
	_KillTimer(p);
	// 할당된 OEM_SYS_TIMER 를 시스템에 반환한다.
	SYSAPI_FreeMem( p );
}

atBOOL      OEM_SYSAPI_IsTimerEnabled( OEM_SYS_TIMER hTimer )
{
	xTTimer *p = (xTTimer *)hTimer;
	if( hTimer == atNULL ) return atFALSE;

	return p->is_enabled;
}

atVOID      OEM_SYSAPI_EnableTimer( OEM_SYS_TIMER hTimer, atBOOL bEnable )
{
	xTTimer *p = (xTTimer *)hTimer;
	if( hTimer == atNULL ) return;

	// 현재 활성화 상태 여부가 동일하면 패스한다.
	if( p->is_enabled == bEnable) return;
	p->is_enabled = bEnable;
	if( bEnable ){		// 활성화 여부가 True 이면 GTimer 에 Callback 함수를 등록한다.
		_SetTimer(p);
	} else {				// 활성화 여부가 False 이면 GTimer 에 등록된 Callback 함수를 제거한다.
		_KillTimer(p);
	}
}

atVOID      OEM_SYSAPI_SetTimerInterval( OEM_SYS_TIMER hTimer, atLONG lInterval )
{
	xTTimer *p = (xTTimer *)hTimer;
	if( hTimer == atNULL ) return;

	// 인터벌이 동일하면 패스한다.
	if( p->interval == lInterval ) return;
	p->interval = lInterval;

	// 활성화 Timer 이라면 GTimer 에 등록된 Callback 함수를 제거하고 재등록한다.
	if( p->is_enabled ){
		_KillTimer(p);
		_SetTimer(p);
	}
}

atVOID      OEM_SYSAPI_ResetTimer( OEM_SYS_TIMER hTimer )
{
	xTTimer *p = (xTTimer *)hTimer;
	if( hTimer == atNULL ) return;

	// 활성화 Timer 이라면 GTimer 에 등록된 Callback 함수를 제거하고 재등록한다.
	if( p->is_enabled ){
		_KillTimer(p);
		_SetTimer(p);
	}
}

atVOID     	OEM_SYSAPI_SuspendTimer( OEM_SYS_TIMER hTimer )
{
	/*
	xTTimer *p = (xTTimer *)hTimer;
	if( hTimer == atNULL ) return;
	*/
}

atVOID      OEM_SYSAPI_ResumeTimer( OEM_SYS_TIMER hTimer )
{
	/*
	xTTimer *p = (xTTimer *)hTimer;
	if( hTimer == atNULL ) return;
	*/
}

