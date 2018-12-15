#include "SYSAPI.h"
#include "OEM_SA_Timer.h"
#include <windows.h>


extern HWND g_SYS_hWindow;
extern LRESULT (CALLBACK *g_SYS_WndProc_Timer)(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);


static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message) {
		case WM_TIMER:
			break;
    }

    return 0;
}


static atBOOL  OEM_SYSAPI_InitTimer( atLONG lSysInst, atLONG lInitData1, atLONG lInitData2 )
{
	g_SYS_WndProc_Timer = WndProc;

	return atTRUE;
}


static atVOID  OEM_SYSAPI_FinalTimer(void)
{
	g_SYS_WndProc_Timer = atNULL;
}


static atULONG	OEM_SYSAPI_GetCurrTime(void)
{
	return GetTickCount();
}


static atVOID      OEM_SYSAPI_EnableTimer( OEM_SYS_TIMER hTimer, atBOOL bEnable );

typedef VOID CALLBACK xTTimerProc( HWND hwnd, UINT uMsg, UINT idEvent, DWORD dwTime );

typedef struct {
	int interval;
	int is_enabled;
	int bRepeat;
	void (*callback)(OEM_SYS_TIMER,void*);
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

	if( !p->bRepeat ){
		OEM_SYSAPI_EnableTimer( (OEM_SYS_TIMER)p, atFALSE );
	}

	p->callback( (OEM_SYS_TIMER)p, p->param );
}

 
static OEM_SYS_TIMER    OEM_SYSAPI_AddTimer( atLONG interval, void (*callback)(OEM_SYS_TIMER,void*), void *param, atBOOL bRepeat, atBOOL bEnable )
{
	xTTimer *p = SYSAPI_AllocMem( sizeof(xTTimer) );

	p->interval = interval;
	p->is_enabled = bEnable;
	p->bRepeat = bRepeat;
	p->param = param;
	p->callback = callback;

	if( p->is_enabled ){
		SetTimer( g_SYS_hWindow, (UINT)p, interval, _TimerProc );
	}

	return (OEM_SYS_TIMER)p;
}

static atVOID      OEM_SYSAPI_RemoveTimer( OEM_SYS_TIMER hTimer )
{
	if( hTimer == atNULL ) return;

	SYSAPI_FreeMem( hTimer );
}

/*
static atVOID      OEM_SYSAPI_SetTimerParam( OEM_SYS_TIMER hTimer, atVOID *lpParam )
{
	xTTimer *p = (xTTimer *)hTimer;
	if( hTimer == atNULL ) return;

	p->param = lpParam;
}
*/

static atBOOL      OEM_SYSAPI_IsTimerEnabled( OEM_SYS_TIMER hTimer )
{
	xTTimer *p = (xTTimer *)hTimer;
	if( hTimer == atNULL ) return atFALSE;

	return p->is_enabled;
}

static atVOID      OEM_SYSAPI_EnableTimer( OEM_SYS_TIMER hTimer, atBOOL bEnable )
{
	xTTimer *p = (xTTimer *)hTimer;
	if( hTimer == atNULL ) return;

	if( p->is_enabled == bEnable ) return;

	p->is_enabled = bEnable;
	if( bEnable ){
		SetTimer( g_SYS_hWindow, (UINT)hTimer, p->interval, _TimerProc );
	} else {
		KillTimer( g_SYS_hWindow, (UINT)hTimer );
	}
}

static atVOID      OEM_SYSAPI_SetTimerInterval( OEM_SYS_TIMER hTimer, atLONG lInterval )
{
	xTTimer *p = (xTTimer *)hTimer;
	if( hTimer == atNULL ) return;

	if( p->interval == lInterval ) return;
	p->interval = lInterval;

	if( p->is_enabled ){
		KillTimer( g_SYS_hWindow, (UINT)hTimer );
		SetTimer( g_SYS_hWindow, (UINT)hTimer, p->interval, _TimerProc );
	}
}

static atVOID      OEM_SYSAPI_ResetTimer( OEM_SYS_TIMER hTimer )
{
	xTTimer *p = (xTTimer *)hTimer;
	if( hTimer == atNULL ) return;

	if( p->is_enabled ){
		KillTimer( g_SYS_hWindow, (UINT)hTimer );
		SetTimer( g_SYS_hWindow, (UINT)hTimer, p->interval, _TimerProc );
	}
}

static atVOID     	OEM_SYSAPI_SuspendTimer( OEM_SYS_TIMER hTimer )
{
	xTTimer *p = (xTTimer *)hTimer;
	if( hTimer == atNULL ) return;

}

static atVOID      OEM_SYSAPI_ResumeTimer( OEM_SYS_TIMER hTimer )
{
	xTTimer *p = (xTTimer *)hTimer;
	if( hTimer == atNULL ) return;

}



void init_sysapi_timer_oem() 
{
	OEM_SYSAPI_TIMER_T oem_funcs = {
		OEM_SYSAPI_InitTimer,          
		OEM_SYSAPI_FinalTimer,         
		OEM_SYSAPI_GetCurrTime,        
		OEM_SYSAPI_AddTimer,           
		OEM_SYSAPI_RemoveTimer,        
		OEM_SYSAPI_IsTimerEnabled,     
		OEM_SYSAPI_EnableTimer,        
		OEM_SYSAPI_SetTimerInterval,   
		OEM_SYSAPI_ResetTimer,
		OEM_SYSAPI_SuspendTimer,       
		OEM_SYSAPI_ResumeTimer
	};

	OEM_SYSAPI_TIMER_SetOemFn( &oem_funcs );
}
