#include "SYSAPI.h"
#include "OEM_SA_Task.h"
#include "stm32f7xx_hal.h"
#include "cmsis_os.h"
#include <stdint.h>


#define MAX_TASK 3
#define MAX_MUTEX 5

// for CMSIS 1.02
typedef struct tagTASK {
	atBOOL bActive;
	void (*callback)(atHANDLE,void*);
	void *param;
	osThreadDef_t ostmr0;
	osThreadDef_t *ostmr;
	osThreadId tmrid;
	atBOOL bEnable;
} TASK;

typedef struct tagEVENTGRP {
  OEM_SYS_TASKSIGNAL wait_signal, cur_signal;
	osThreadId tmrid;
} EVENTGRP;

// for CMSIS 1.02
typedef struct tagMUTEX {
	atBOOL bActive;
	
	uint32_t os_mutex_cb[4];
	osMutexDef_t osmtx0; 
	
	osMutexDef_t *osmtx;
	osMutexId mtxid;
} MUTEX;


void __sa_task_0(void const *argument);
void __sa_task_1(void const *argument);
void __sa_task_2(void const *argument);

osThreadDef (__sa_task_0, osPriorityNormal, 1, 0); 
osThreadDef (__sa_task_1, osPriorityNormal, 1, 0); 
osThreadDef (__sa_task_2, osPriorityNormal, 1, 0); 



/// Define a Mutex.
/// \param         name          name of the mutex object.
#if defined (osObjectsExternal)  // object is external
#define osMutexDef2(name)  \
extern const osMutexDef_t os_mutex_def_##name
#else                            // define the object
#define osMutexDef2(name)  \
uint32_t os_mutex_cb_##name[4] = { 0 }; \
const osMutexDef_t os_mutex_def_##name = { (os_mutex_cb_##name) }
#endif

/// Access a Mutex definition.
/// \param         name          name of the mutex object.
#define osMutex2(name)  \
&os_mutex_def_##name


osMutexDef2(__sa_mutex_0); 
osMutexDef2(__sa_mutex_1); 
osMutexDef2(__sa_mutex_2); 
osMutexDef2(__sa_mutex_3); 
//osMutexDef2(__sa_mutex_4); 

uint32_t os_mutex_cb___sa_mutex_4[4] = { 0 };
//osMutexDef_t os_mutex_def___sa_mutex_4 = { os_mutex_cb___sa_mutex_4 };
osMutexDef_t os_mutex_def___sa_mutex_4;

osMutexDef2(__sa_suspend_mutex); 
osMutexId g_suspend_mutexid;


void __sa_task_0(void const *argument)
{
	TASK *tmr = (TASK *)argument;
	if(tmr == NULL) return;
	
	if(tmr->callback) tmr->callback((atHANDLE)tmr, tmr->param);
}

void __sa_task_1(void const *argument)
{
	TASK *tmr = (TASK *)argument;
	if(tmr == NULL) return;
	
	if(tmr->callback) tmr->callback((atHANDLE)tmr, tmr->param);
}

void __sa_task_2(void const *argument)
{
	TASK *tmr = (TASK *)argument;
	if(tmr == NULL) return;
	
	if(tmr->callback) tmr->callback((atHANDLE)tmr, tmr->param);
}


static TASK g_tmr[MAX_TASK] = {0, };
static MUTEX g_mtx[MAX_MUTEX] = {0, };


atBOOL  	    OEM_SYSAPI_InitTask( atLONG lSysInst, atLONG lInitData1, atLONG lInitData2 )
{
	g_tmr[0].ostmr = (osThreadDef_t *)osThread(__sa_task_0);
	g_tmr[1].ostmr = (osThreadDef_t *)osThread(__sa_task_1);
	g_tmr[2].ostmr = (osThreadDef_t *)osThread(__sa_task_2);

 	g_mtx[0].osmtx = (osMutexDef_t *)osMutex2(__sa_mutex_0);
 	g_mtx[1].osmtx = (osMutexDef_t *)osMutex2(__sa_mutex_1);
 	g_mtx[2].osmtx = (osMutexDef_t *)osMutex2(__sa_mutex_2);
 	g_mtx[3].osmtx = (osMutexDef_t *)osMutex2(__sa_mutex_3);
	
  os_mutex_def___sa_mutex_4.mutex = (void *)os_mutex_cb___sa_mutex_4 ;
 	g_mtx[4].osmtx = &os_mutex_def___sa_mutex_4; //(osMutexDef_t *)osMutex2(__sa_mutex_4);
  
  g_suspend_mutexid = osMutexCreate (osMutex2(__sa_suspend_mutex));

	return atTRUE;
}

atVOID  	    OEM_SYSAPI_FinalTask(void)
{
  osMutexDelete (g_suspend_mutexid);
}


OEM_SYS_TASK         OEM_SYSAPI_CreateTask( atLPTSTR szTaskName, void (*callback)(atHANDLE,void*), void *param, atBOOL bStart, atLONG lStackSize )
{
	TASK *tmr = atNULL;
	int i;
	for(i = 0; i < MAX_TASK; i++) {
		if(!g_tmr[i].bActive) {
			tmr = &g_tmr[i];
			break;
		}
	}
	if(tmr == atNULL) {
	    return atNULL;
	}

	tmr = (TASK *)SYSAPI_AllocMem( sizeof(TASK) );
	
	tmr->ostmr0.pthread = __sa_task_0;
	tmr->ostmr0.tpriority = osPriorityNormal;
  tmr->ostmr0.instances = 100;
	tmr->ostmr0.stacksize = lStackSize;
	tmr->ostmr = &tmr->ostmr0;

	tmr->callback = callback;
	tmr->param = param;

	tmr->tmrid =  osThreadCreate (tmr->ostmr, tmr);
	
   printf("Create thread : %p\r\n", tmr->tmrid );
	if(tmr->tmrid == NULL) {
	    return atNULL;
	}
	
	tmr->bActive = atTRUE;
	
	return (OEM_SYS_TASK)tmr;
}

atVOID    OEM_SYSAPI_DestroyTask( OEM_SYS_TASK hOemTask )
{
	TASK *tmr = (TASK *)hOemTask;
	if(tmr == NULL) return;

	osThreadTerminate 	( 	tmr->tmrid	); 	
	
	tmr->bActive = atFALSE;
	
	SYSAPI_FreeMem( tmr );
}


atBOOL	OEM_SYSAPI_GetTaskInfo( OEM_SYS_TASK hOemTask, SYS_TASK_INFO *lpTaskInfo )
{
	TASK *tmr = (TASK *)hOemTask;
	if(tmr == NULL) return atFALSE;

	return atFALSE;
}

atINT     OEM_SYSAPI_GetTaskState( OEM_SYS_TASK hOemTask )
{
	return 0;
}

atVOID		OEM_SYSAPI_TerminateTask( OEM_SYS_TASK hOemTask )
{
}

atVOID		OEM_SYSAPI_RequestTerminate( OEM_SYS_TASK hOemTask )
{
}

atBOOL    OEM_SYSAPI_WaitForTerminate( OEM_SYS_TASK hOemTask, atLONG lMilliSec )
{
	return atFALSE;
}

atVOID    OEM_SYSAPI_SuspendTask( OEM_SYS_TASK hOemTask )
{
}	

atVOID    OEM_SYSAPI_ResumeTask( OEM_SYS_TASK hOemTask )
{
}


OEM_SYS_TASK		OEM_SYSAPI_GetCurrentTask( NOARGS )
{
	return osThreadGetId();
}

OEM_SYS_TASKSIGNAL    OEM_SYSAPI_WaitEvents( OEM_SYS_EVENT hOemEvents, atBOOL bIsAll, atLONG lMilliSec )
{
	EVENTGRP *evt = (EVENTGRP *)hOemEvents;
	if(evt == NULL) return 0;
  
 	if( evt->wait_signal == 0 ) return 0;
  
	if( bIsAll && ((evt->cur_signal & evt->wait_signal) == evt->wait_signal) ) return evt->cur_signal;
	if( !bIsAll && ((evt->cur_signal & evt->wait_signal) != 0x00) ) return evt->cur_signal;
	
	if( lMilliSec == SYS_WAIT_INFINITE ){
    lMilliSec = osWaitForever;
	}

   TASK *tsk = (TASK *)OEM_SYSAPI_GetCurrentTask();
   evt->tmrid = osThreadGetId();
                  
	 osEvent osEvt = osSignalWait (0x1000, lMilliSec);
  
		if (osEvt.status == osEventSignal)  {
			if (osEvt.value.signals & 0x1000) {
				osSignalClear (evt->tmrid, 0x1000);
     }
  }
  
  return evt->cur_signal;
}

atVOID          OEM_SYSAPI_Sleep( atDWORD dwMilliSec )
{
	if(dwMilliSec == 0){
		osThreadYield();
	} else {
		osDelay( dwMilliSec );
	}
}

OEM_SYS_EVENT 	OEM_SYSAPI_CreateEventGroup(atLPTSTR szGroupName)
{
  EVENTGRP *evt = (EVENTGRP *)SYSAPI_AllocMem(sizeof(EVENTGRP));
  
  evt->cur_signal = evt->wait_signal = 0x00;
  evt->tmrid = atNULL;
  
	return (OEM_SYS_EVENT)evt;
}

atVOID		OEM_SYSAPI_DestroyEventGroup( OEM_SYS_EVENT hOemEvents )
{
	EVENTGRP *evt = (EVENTGRP *)hOemEvents;
	if(evt == NULL) return;
  
  SYSAPI_FreeMem(evt);
}

OEM_SYS_TASKSIGNAL	OEM_SYSAPI_SetWaitSignal( OEM_SYS_EVENT hOemEvents, OEM_SYS_TASKSIGNAL dwSignal )
{
	EVENTGRP *evt = (EVENTGRP *)hOemEvents;
	if(evt == NULL) return atNULL;
  
  evt->wait_signal = dwSignal;
  
	return dwSignal;
}

OEM_SYS_TASKSIGNAL	OEM_SYSAPI_GetWaitSignal( OEM_SYS_EVENT hOemEvents )
{
	EVENTGRP *evt = (EVENTGRP *)hOemEvents;
	if(evt == NULL) return atNULL;
  
 	return evt->wait_signal;
}

OEM_SYS_TASKSIGNAL	OEM_SYSAPI_GetCurrentSignal( OEM_SYS_EVENT hOemEvents )
{
	EVENTGRP *evt = (EVENTGRP *)hOemEvents;
	if(evt == NULL) return atNULL;
  
 	return evt->cur_signal;
}

OEM_SYS_TASKSIGNAL		OEM_SYSAPI_SendSignal( OEM_SYS_EVENT hOemEvents, OEM_SYS_TASKSIGNAL dwSignal )
{
	EVENTGRP *evt = (EVENTGRP *)hOemEvents;
	if(evt == NULL) return atNULL;

	evt->cur_signal |= dwSignal;

	if( evt->cur_signal & evt->wait_signal ){
		osSignalSet (evt->tmrid, 0x1000);
	}
	
	return evt->cur_signal;  
}

OEM_SYS_TASKSIGNAL		OEM_SYSAPI_ClearSignal( OEM_SYS_EVENT hOemEvents, OEM_SYS_TASKSIGNAL dwSignal )
{
	EVENTGRP *evt = (EVENTGRP *)hOemEvents;
	if(evt == NULL) return atNULL;
  
	evt->cur_signal &= ~dwSignal;

	if( !(evt->cur_signal & evt->wait_signal) ){
		 //evt->m_semaphore.Signal();
	}
	
	return evt->cur_signal;  
}

OEM_SYS_CRSECTION    OEM_SYSAPI_CreateCriticalSection(void)
{
	MUTEX *mtx = atNULL;
	int i;
	for(i = 0; i < MAX_MUTEX; i++) {
		if(!g_mtx[i].bActive) {
			mtx = &g_mtx[i];
			break;
		}
	}
	if(mtx == atNULL) {
	    return atNULL;
	}

	mtx = (MUTEX *)SYSAPI_AllocMem(sizeof(MUTEX));
	
	SYSAPI_memset((void *)mtx->os_mutex_cb, (char)0, sizeof(uint32_t[4]));
  mtx->osmtx0.mutex = (void *)mtx->os_mutex_cb;
	mtx->osmtx = &mtx->osmtx0;
  mtx->mtxid = osMutexCreate (mtx->osmtx);
	
  printf("Create criticalsection : %p\r\n", mtx->mtxid );
	if(mtx->mtxid == NULL) {
	    return atNULL;
	}
	
	mtx->bActive = atTRUE;

	return (OEM_SYS_CRSECTION)mtx;
}

atVOID          OEM_SYSAPI_DestroyCriticalSection( OEM_SYS_CRSECTION hCrSection )
{
	MUTEX *mtx = (MUTEX *)hCrSection;
	if(mtx == NULL) return;

  osMutexDelete (mtx->mtxid);
  
  mtx->bActive = atFALSE;
	
	SYSAPI_FreeMem( mtx );
}

atVOID          OEM_SYSAPI_EnterCriticalSection( OEM_SYS_CRSECTION hCrSection )
{
	MUTEX *mtx = (MUTEX *)hCrSection;
	if(mtx == NULL) return;

  osMutexWait (mtx->mtxid, 0);
}

atVOID          OEM_SYSAPI_LeaveCriticalSection( OEM_SYS_CRSECTION hCrSection )
{
	MUTEX *mtx = (MUTEX *)hCrSection;
	if(mtx == NULL) return;

  osMutexRelease (mtx->mtxid);
}

