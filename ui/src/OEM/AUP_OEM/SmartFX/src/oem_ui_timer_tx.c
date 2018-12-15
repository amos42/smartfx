#include "A_UIENG.h"
#include "UI_API/UI_API.h"
#include "OEM_UI_Timer.h"
#include "SYSAPI.h"


//extern HWND g_hWindow;

/*
extern LRESULT (CALLBACK *WndProc_Timer)(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message) {
		case WM_TIMER:
			break;
    }

    return 0;
}
*/


atBOOL  OEM_UI_InitTimer( atLONG lSysInst, atLONG lInitData1, atLONG lInitData2 )
{
//	WndProc_Timer = WndProc;
	return SYSAPI_InitTimer( lSysInst, lInitData1, lInitData2 );
}


atVOID  OEM_UI_FinalTimer(void)
{
//	WndProc_Timer = atNULL;
	SYSAPI_FinalTimer();
}


atULONG	OEM_UI_GetCurrTime(void)
{
	return SYSAPI_GetCurrTime();
}

/*
typedef VOID CALLBACK xTTimerProc( HWND hwnd, UINT uMsg, UINT idEvent, DWORD dwTime );

typedef struct {
	int is_active;
	int interval;
	int is_enabled;
	int bRepeat;
	void (*callback)(OEM_TIMER,void*);
	void *param;
} xTTimer;

static VOID CALLBACK _TimerProc(
    HWND hwnd,	// handle of window for timer messages 
    UINT uMsg,	// WM_TIMER message
    UINT idEvent,	// timer identifier
    DWORD dwTime 	// current system time
 )
{
	xTTimer *p = (xTTimer *)idEvent;
	if( p == atNULL ) return;

	if( !p->bRepeat ){
		OEM_UI_EnableTimer( (OEM_TIMER)p, atFALSE );
	}

	p->callback( (OEM_TIMER)p, p->param );
}
*/
 
OEM_TIMER    OEM_UI_AddTimer( atLONG interval, void (*callback)(OEM_TIMER,void*), void *param, atBOOL bRepeat, atBOOL bEnable )
{
/*
	xTTimer *p = atUIAPI_AllocMem( sizeof(xTTimer) );

	p->interval = interval;
	p->is_enabled = bEnable;
	p->bRepeat = bRepeat;
	p->param = param;
	p->callback = callback;

	if( p->is_enabled ){
		SetTimer( g_hWindow, (UINT)p, interval, _TimerProc );
		p->is_active = atTRUE;
	} else {
		p->is_active = atFALSE;
	}

	return (OEM_TIMER)p;
*/
	return (OEM_TIMER)SYSAPI_AddTimer( interval, callback, param, bRepeat, bEnable );
}

atVOID      OEM_UI_RemoveTimer( OEM_TIMER hTimer )
{
/*
	xTTimer *p = (xTTimer *)hTimer;
	if( hTimer == atNULL ) return;

	if( p->is_active ){
		KillTimer( g_hWindow, (UINT)hTimer );
		p->is_active = atFALSE;
	}
	
	atUIAPI_FreeMem( hTimer );
*/
	SYSAPI_RemoveTimer( hTimer );
}

atVOID      OEM_UI_SetTimerParam( OEM_TIMER hTimer, atVOID *lpParam )
{
/*
	xTTimer *p = (xTTimer *)hTimer;
	if( hTimer == atNULL ) return;

	p->param = lpParam;
*/
	SYSAPI_SetTimerParam( hTimer, lpParam );
}

atBOOL      OEM_UI_IsTimerEnabled( OEM_TIMER hTimer )
{
/*
	xTTimer *p = (xTTimer *)hTimer;
	if( hTimer == atNULL ) return atFALSE;

	return p->is_enabled;
*/
	return SYSAPI_IsTimerEnabled(hTimer);
}

atVOID      OEM_UI_EnableTimer( OEM_TIMER hTimer, atBOOL bEnable )
{
/*
	xTTimer *p = (xTTimer *)hTimer;
	if( hTimer == atNULL ) return;

	if( p->is_enabled == bEnable ) return;

	p->is_enabled = bEnable;
	if( bEnable ){
		SetTimer( g_hWindow, (UINT)hTimer, p->interval, _TimerProc );
		p->is_active = atTRUE;
	} else {
		KillTimer( g_hWindow, (UINT)hTimer );
		p->is_active = atFALSE;
	}
*/
	SYSAPI_EnableTimer( hTimer, bEnable );
}

atVOID      OEM_UI_SetTimerInterval( OEM_TIMER hTimer, atLONG lInterval )
{
/*
	xTTimer *p = (xTTimer *)hTimer;
	if( hTimer == atNULL ) return;

	if( p->interval == lInterval ) return;
	p->interval = lInterval;

	if( p->is_enabled ){
		if( p->is_active ){
			KillTimer( g_hWindow, (UINT)hTimer );
			SetTimer( g_hWindow, (UINT)hTimer, p->interval, _TimerProc );
		}
	}
*/
	SYSAPI_SetTimerInterval( hTimer, lInterval );
}

atVOID      OEM_UI_ResetTimer( OEM_TIMER hTimer )
{
/*
	xTTimer *p = (xTTimer *)hTimer;
	if( hTimer == atNULL ) return;

	if( p->is_enabled ){
		if( p->is_active ){
			KillTimer( g_hWindow, (UINT)hTimer );
			SetTimer( g_hWindow, (UINT)hTimer, p->interval, _TimerProc );
		}
	}
*/
	SYSAPI_ResetTimer( hTimer );
}

atVOID     	OEM_UI_SuspendTimer( OEM_TIMER hTimer )
{
/*
	xTTimer *p = (xTTimer *)hTimer;
	if( hTimer == atNULL ) return;

	if( p->is_enabled ){
		KillTimer( g_hWindow, (UINT)hTimer );
		p->is_active = atFALSE;
	}
*/
	SYSAPI_SuspendTimer( hTimer );
}

atVOID      OEM_UI_ResumeTimer( OEM_TIMER hTimer )
{
/*
	xTTimer *p = (xTTimer *)hTimer;
	if( hTimer == atNULL ) return;

	if( p->is_enabled ){
		SetTimer( g_hWindow, (UINT)hTimer, p->interval, _TimerProc );
		p->is_active = atTRUE;
	}
*/
	SYSAPI_ResumeTimer( hTimer );
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
