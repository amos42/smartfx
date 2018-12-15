#include "SYSAPI.h"
#include "OEM_SA_Timer.h"
#include "tx_api.h"


//static TInt g_nanokernel_tick_period;
static UINT g_btn_no = 0;


atBOOL  OEM_SYSAPI_InitTimer( atLONG lSysInst, atLONG lInitData1, atLONG lInitData2 )
{
	return atTRUE;
}


atVOID  OEM_SYSAPI_FinalTimer( NOARGS )
{

}


atULONG	OEM_SYSAPI_GetCurrTime( NOARGS )
{
 	return tx_time_get();
}



//typedef VOID xTTimerProc( HWND hwnd, UINT uMsg, UINT idEvent, DWORD dwTime );

typedef struct 
{
	TX_TIMER txTimer;
	int 	is_enabled;
	int 	bRepeat;
	void (*callback)(OEM_SYS_TIMER,void*);
	void *param;
	
} xTTimer;


static VOID _OemTimerProc(ULONG param)
{
	xTTimer *pOemTimer = (xTTimer *)param;

	if( !pOemTimer->bRepeat ){
		OEM_SYSAPI_EnableTimer( (OEM_SYS_TIMER)pOemTimer, atFALSE );
	}

	//SYSAPI_DebugPrintf(_AT("_OemTimerProc\n"));

	pOemTimer->callback( (OEM_SYS_TIMER)pOemTimer, pOemTimer->param );
}


OEM_SYS_TIMER    OEM_SYSAPI_AddTimer(atLONG atINTerval, atVOID (*callback)(OEM_SYS_TIMER,atVOID*), atVOID *param, atBOOL bRepeat, atBOOL bEnable )
{
	char timer_name[100];
	UINT status;

	SYSAPI_DebugPrintf(_AT("SYSAPI_AddTimer\n"));

	sprintf(timer_name, "BTN_%d", g_btn_no);
	g_btn_no++;

	xTTimer *pOemTimer = (xTTimer*)SYSAPI_AllocMem( sizeof(xTTimer) );
	if(!pOemTimer) return atNULL;

	SYSAPI_memset(pOemTimer, 0, sizeof(xTTimer));

	pOemTimer->is_enabled = bEnable;
	pOemTimer->bRepeat    = bRepeat;
	pOemTimer->param      = param;
	pOemTimer->callback   = callback;

	status = tx_timer_create(&pOemTimer->txTimer, timer_name, _OemTimerProc, (ULONG)pOemTimer, atINTerval, atINTerval, bEnable ? TX_AUTO_ACTIVATE : TX_NO_ACTIVATE);
	if(status != TX_SUCCESS)
	{
		SYSAPI_DebugPrintf(_AT("Error tx_timer_create failed!!\n"));
		SYSAPI_FreeMem(pOemTimer);
		return atNULL;
	}

	return (OEM_SYS_TIMER)pOemTimer;

}

atVOID      OEM_SYSAPI_RemoveTimer( OEM_SYS_TIMER hTimer )
{
	xTTimer* pTimer = (xTTimer*)hTimer;
	if(!pTimer) return;

	tx_timer_delete(&pTimer->txTimer);

	SYSAPI_FreeMem(pTimer);
}

atVOID      OEM_SYSAPI_SetTimerParam( OEM_SYS_TIMER hTimer, atVOID *lpParam )
{
#if 0
	CMTimer* tmr = (CMTimer*)hTimer;
	if( tmr == atNULL ) return;
    
	tmr->SetParam( lpParam );
#endif
}

atBOOL      OEM_SYSAPI_IsTimerEnabled( OEM_SYS_TIMER hTimer )
{
	xTTimer* pTimer;
	CHAR* timer_name;
	UINT active;
	UINT status;
	ULONG remaining_ticks;
	ULONG reschedule_ticks;
	TX_TIMER* pNextTimer;
	
	pTimer = (xTTimer*)hTimer;
	if(!pTimer) return atFALSE;

	active = 0;

	status = tx_timer_info_get(&pTimer->txTimer, &timer_name, &active, &remaining_ticks, &reschedule_ticks, &pNextTimer);
	if(status == TX_SUCCESS && active)
		return atTRUE;
	else
		return atFALSE;

}

atVOID      OEM_SYSAPI_EnableTimer( OEM_SYS_TIMER hTimer, atBOOL bEnable )
{
	xTTimer* pTimer;
	
	pTimer = (xTTimer*)hTimer;
	if(!pTimer) return;

	if(bEnable)
		tx_timer_activate(&pTimer->txTimer);
	else
		tx_timer_deactivate(&pTimer->txTimer);
}

atVOID      OEM_SYSAPI_SetTimerInterval( OEM_SYS_TIMER hTimer, atLONG lInterval )
{
	xTTimer*  pTimer;
	CHAR*     timer_name;
	UINT      active;
	UINT      status;
	ULONG     remaining_ticks;
	ULONG     reschedule_ticks;
	TX_TIMER* pNextTimer;
	
		
	pTimer = (xTTimer*)hTimer;
	if(!pTimer) return;

	status = tx_timer_info_get(&pTimer->txTimer,&timer_name, &active, &remaining_ticks, &reschedule_ticks, &pNextTimer);
	if(status != TX_SUCCESS) return;

	if(reschedule_ticks == (ULONG)lInterval) return;

	tx_timer_change(&pTimer->txTimer, 0, lInterval);

}

atVOID      OEM_SYSAPI_ResetTimer( OEM_SYS_TIMER hTimer )
{
	xTTimer*  pTimer;
	CHAR*     timer_name;
	UINT      active;
	UINT      status;
	ULONG     remaining_ticks;
	ULONG     reschedule_ticks;
	TX_TIMER* pNextTimer;

	pTimer = (xTTimer*)hTimer;
	if(!pTimer) return;

	status = tx_timer_info_get(&pTimer->txTimer,&timer_name, &active, &remaining_ticks, &reschedule_ticks, &pNextTimer);
	if(status != TX_SUCCESS) return;

	tx_timer_change(&pTimer->txTimer, 0, reschedule_ticks);
}

atVOID     	OEM_SYSAPI_SuspendTimer( OEM_SYS_TIMER hTimer )
{
	xTTimer* pTimer = (xTTimer*)hTimer;
		
	if(!pTimer) return;

	tx_timer_deactivate(&pTimer->txTimer);
}

atVOID      OEM_SYSAPI_ResumeTimer( OEM_SYS_TIMER hTimer )
{
	xTTimer* pTimer = (xTTimer*)hTimer;
		
	if(!pTimer) return;

	tx_timer_activate(&pTimer->txTimer);
}
