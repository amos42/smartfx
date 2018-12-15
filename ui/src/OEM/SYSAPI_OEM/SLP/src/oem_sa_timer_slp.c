#include "SYSAPI.h"
#include "OEM_SA_Timer.h"
#include <linux/sched.h>
#include <time.h>

#include <gtk/gtk.h>

static GTimer *g_hTimer = atNULL;

atBOOL  OEM_SYSAPI_InitTimer( atLONG lSysInst, atLONG lInitData1, atLONG lInitData2 )
{
	// Ÿ�̸� �ν��Ͻ� ����
	g_hTimer = g_timer_new();
	if(g_hTimer == atNULL) return atFALSE;

	// Ÿ�̸� �۵�
	g_timer_start(g_hTimer);
	return atTRUE;
}

atVOID  OEM_SYSAPI_FinalTimer(void)
{
	if(g_hTimer == atNULL) return;

	// Ÿ�̾� �۵�����
	g_timer_stop(g_hTimer);
	// Ÿ�̸� �ν��Ͻ� �ý��۹�ȯ
	g_timer_destroy(g_hTimer);
	g_hTimer = atNULL;
}

atULONG	OEM_SYSAPI_GetCurrTime(void)
{
	if(g_hTimer)
	{
		gdouble sec;
		gulong microsec;

		// Ÿ�̸� �۵� ������ ����� �ð��� ���Ѵ�. (�� ������ ���ϵǰ�, ����ũ�� ������ ���۷����� ����)
		sec = g_timer_elapsed(g_hTimer, &microsec);
		return (atULONG)((glong)sec * 1000 + microsec / 1000);	// �Ҽ� ���� X
		//return (atULONG)((glong)(sec * 1000) + microsec / 1000); // �Ҽ� ���� O
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

	// GTimer �� ��ϵ� Callback �Լ��� �����Ѵ�.
	gtk_timeout_remove(p->timer_id);
	p->timer_id = 0;
}

static void _TimerProc(gpointer param)
{
	xTTimer *p = (xTTimer *)param;
	if(p == atNULL) return;

	// ������ Callback �Ǿ��� �Լ��� ���Ѵ�.
	if(p->callback) p->callback(p, p->param);
	// �ݺ��� Timer �� �ƴ϶�� GTimer �� ��ϵ� Callback �Լ��� �����Ѵ�.
	if(!p->bRepeat) _KillTimer(p);
}

static void _SetTimer(xTTimer *p)
{
	if(p == atNULL) return;
	// GTimer �� Callback �Լ��� ��Ͻ�Ų��.
	p->timer_id = gtk_timeout_add(p->interval, (GtkFunction)_TimerProc, p);
}

OEM_SYS_TIMER    OEM_SYSAPI_AddTimer( atLONG interval, void (*callback)(OEM_SYS_TIMER,void*), void *param, atBOOL bRepeat, atBOOL bEnable )
{
	// OEM_SYS_TIMER �� �޸� �Ҵ��Ѵ�.
	xTTimer *p = SYSAPI_AllocMem( sizeof(xTTimer) );
	if(p == atNULL) return NULL;

	p->interval = interval;
	p->is_enabled = bEnable;
	p->bRepeat = bRepeat;
	p->param = param;
	p->callback = callback;
	p->timer_id = 0;

	// Ȱ��ȭ Timer �̶�� GTimer �� Callback �Լ��� ��Ͻ�Ų��.
	if( p->is_enabled ) _SetTimer(p);

	return (OEM_SYS_TIMER)p;
}

atVOID      OEM_SYSAPI_RemoveTimer( OEM_SYS_TIMER hTimer )
{
	xTTimer *p = (xTTimer *)hTimer;
	if(p == atNULL) return;

	// GTimer �� ��ϵ� Callback �Լ��� �����Ѵ�.
	_KillTimer(p);
	// �Ҵ�� OEM_SYS_TIMER �� �ý��ۿ� ��ȯ�Ѵ�.
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

	// ���� Ȱ��ȭ ���� ���ΰ� �����ϸ� �н��Ѵ�.
	if( p->is_enabled == bEnable) return;
	p->is_enabled = bEnable;
	if( bEnable ){		// Ȱ��ȭ ���ΰ� True �̸� GTimer �� Callback �Լ��� ����Ѵ�.
		_SetTimer(p);
	} else {				// Ȱ��ȭ ���ΰ� False �̸� GTimer �� ��ϵ� Callback �Լ��� �����Ѵ�.
		_KillTimer(p);
	}
}

atVOID      OEM_SYSAPI_SetTimerInterval( OEM_SYS_TIMER hTimer, atLONG lInterval )
{
	xTTimer *p = (xTTimer *)hTimer;
	if( hTimer == atNULL ) return;

	// ���͹��� �����ϸ� �н��Ѵ�.
	if( p->interval == lInterval ) return;
	p->interval = lInterval;

	// Ȱ��ȭ Timer �̶�� GTimer �� ��ϵ� Callback �Լ��� �����ϰ� �����Ѵ�.
	if( p->is_enabled ){
		_KillTimer(p);
		_SetTimer(p);
	}
}

atVOID      OEM_SYSAPI_ResetTimer( OEM_SYS_TIMER hTimer )
{
	xTTimer *p = (xTTimer *)hTimer;
	if( hTimer == atNULL ) return;

	// Ȱ��ȭ Timer �̶�� GTimer �� ��ϵ� Callback �Լ��� �����ϰ� �����Ѵ�.
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

