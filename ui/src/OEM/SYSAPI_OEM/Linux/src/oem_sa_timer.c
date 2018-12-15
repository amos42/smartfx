#include "SYSAPI.h"
#include "OEM_SA_Timer.h"
#include "stm32f7xx_hal.h"
#include "cmsis_os.h"


#define MAX_TIMER 3


typedef struct tagTMR {
	atBOOL bActive;
	int inter;
	void (*callback)(OEM_SYS_TIMER,void*);
	void *param;
	osTimerDef_t *ostmr;
	osTimerId tmrid;
	atBOOL bEnable;
} TMR;


void __sa_timer_0(void const *argument);
void __sa_timer_1(void const *argument);
void __sa_timer_2(void const *argument);

osTimerDef (__sa_tim0, __sa_timer_0); 
osTimerDef (__sa_tim1, __sa_timer_1); 
osTimerDef (__sa_tim2, __sa_timer_2); 

void __sa_timer_0(void const *argument)
{
	TMR *tmr = (TMR *)argument;
	if(tmr == NULL) return;
	
	if(tmr->callback) tmr->callback((OEM_SYS_TIMER)tmr, tmr->param);
}

void __sa_timer_1(void const *argument)
{
	TMR *tmr = (TMR *)argument;
	if(tmr == NULL) return;
	
	if(tmr->callback) tmr->callback((OEM_SYS_TIMER)tmr, tmr->param);
}

void __sa_timer_2(void const *argument)
{
	TMR *tmr = (TMR *)argument;
	if(tmr == NULL) return;
	
	if(tmr->callback) tmr->callback((OEM_SYS_TIMER)tmr, tmr->param);
}


static TMR g_tmr[MAX_TIMER] = {0, };


atBOOL  OEM_SYSAPI_InitTimer( atLONG lSysInst, atLONG lInitData1, atLONG lInitData2 )
{
	g_tmr[0].ostmr = (osTimerDef_t *)osTimer(__sa_tim0);
	g_tmr[1].ostmr = (osTimerDef_t *)osTimer(__sa_tim1);
	g_tmr[2].ostmr = (osTimerDef_t *)osTimer(__sa_tim2);
	
	return atTRUE;
}

atVOID  OEM_SYSAPI_FinalTimer(void)
{
}

atULONG	OEM_SYSAPI_GetCurrTime(void)
{
	return osKernelSysTick();
}

OEM_SYS_TIMER    OEM_SYSAPI_AddTimer( atLONG interval, void (*callback)(OEM_SYS_TIMER,void*), void *param, atBOOL bRepeat, atBOOL bEnable )
{
	TMR *tmr = atNULL;
	int i;
	for(i = 0; i < MAX_TIMER; i++) {
		if(!g_tmr[i].bActive) {
			tmr = &g_tmr[i];
			break;
		}
	}
	if(tmr == atNULL) {
	    return atNULL;
	}
	
	if(bRepeat){
		tmr->tmrid =  osTimerCreate (tmr->ostmr, osTimerPeriodic, tmr);
	} else {
		tmr->tmrid =  osTimerCreate (tmr->ostmr, osTimerOnce, tmr);
	}
	
  //SYSAPI_DebugPrintf(_AT("Create timer : %p\r\n"), tmr->tmrid );
	
	if(tmr->tmrid == NULL) {	
	    return atNULL;
	}
	
	if(interval < 10)  interval = 10;
	tmr->inter = interval;
	tmr->callback = callback;
	tmr->param = param;
	tmr->bEnable = bEnable;			
	tmr->bActive = atTRUE;
	
	if(bEnable) {
		osTimerStart (tmr->tmrid, tmr->inter); 
	}

	return (OEM_SYS_TIMER)tmr;
}

atVOID      OEM_SYSAPI_RemoveTimer( OEM_SYS_TIMER hTimer )
{
	TMR *tmr = (TMR *)hTimer;
	if(tmr == NULL) return;

	osTimerDelete 	( 	tmr->tmrid	); 	
	
	tmr->bActive = atFALSE;
}

atBOOL      OEM_SYSAPI_IsTimerEnabled( OEM_SYS_TIMER hTimer )
{
	TMR *tmr = (TMR *)hTimer;
	if(tmr == NULL) return atFALSE;
	
	return tmr->bEnable;
}

atVOID      OEM_SYSAPI_EnableTimer( OEM_SYS_TIMER hTimer, atBOOL bEnable )
{
	TMR *tmr = (TMR *)hTimer;
	if(tmr == NULL) return;
	
	if(bEnable){
		osTimerStart (tmr->tmrid, tmr->inter); 
	} else {
		osTimerStop 	(tmr->tmrid); 	
	}
	
	tmr->bEnable = bEnable;
}

atVOID      OEM_SYSAPI_SetTimerInterval( OEM_SYS_TIMER hTimer, atLONG lInterval )
{
	TMR *tmr = (TMR *)hTimer;
	if(tmr == NULL) return;

	tmr->inter = lInterval;
}

atVOID      OEM_SYSAPI_ResetTimer( OEM_SYS_TIMER hTimer )
{
	TMR *tmr = (TMR *)hTimer;
	if(tmr == NULL) return;
	
	if(tmr->bEnable) {
		osTimerStop 	(tmr->tmrid); 	
		osTimerStart (tmr->tmrid, tmr->inter); 
	}
}

atVOID     	OEM_SYSAPI_SuspendTimer( OEM_SYS_TIMER hTimer )
{
	TMR *tmr = (TMR *)hTimer;
	if(tmr == NULL) return;
	
	if(tmr->bEnable) {
		osTimerStop 	(tmr->tmrid); 	
	}
}

atVOID      OEM_SYSAPI_ResumeTimer( OEM_SYS_TIMER hTimer )
{
	TMR *tmr = (TMR *)hTimer;
	if(tmr == NULL) return;
	
	if(tmr->bEnable) {
		osTimerStart (tmr->tmrid, tmr->inter); 
	}
}

