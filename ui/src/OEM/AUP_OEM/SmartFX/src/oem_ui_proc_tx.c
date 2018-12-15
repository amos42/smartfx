#include "UI_MNG.h"
#include "UI_API/UI_API.h"
#include "OEM_UI_Proc.h"
#include "SYSAPI.h"

#include <stdio.h>

#define DIRECT_PROC 0
#define THREAD_PROC 1
#define THREAD_PROC_2 2
#define TIMER_PROC 3

//================================================================
//#define PROC_IMPL DIRECT_PROC
#define PROC_IMPL THREAD_PROC
//#define PROC_IMPL THREAD_PROC_2
//#define PROC_IMPL TIMER_PROC
//================================================================




//================================================================
#if PROC_IMPL == DIRECT_PROC
//================================================================

typedef struct tagProcess {
	atTCHAR szProcName[32];
	int state;  // -1 - invalide, 0 - ready, 1 - busy, 2 - suspend
	void *param;
	atBOOL (*callback)(OEM_UI_PROCESSOR, void *);
} TProcess;


atBOOL  	    OEM_UI_InitProcessor( atLONG lSysInst, atLONG lInitData1, atLONG lInitData2 )
{
	return atTRUE;
}

atVOID  	    OEM_UI_FinalProcessor(void)
{
}

OEM_UI_PROCESSOR    OEM_UI_CreateProcessor( atLPTSTR szProcName, atINT nProcType, atBOOL (*callback)(OEM_UI_PROCESSOR, void*), void *param )
{
	TProcess *p;

	p =	(TProcess *)atUIAPI_AllocMem( sizeof(TProcess) );
	SYSAPI_memset(p, 0, sizeof(TProcess));
	SYSAPI_tcsncpy( p->szProcName, szProcName, 30 );
	p->state = 0;
	p->callback = callback;
	p->param = param;

    return (OEM_UI_PROCESSOR)p;
}

atVOID          OEM_UI_DestroyProcessor( OEM_UI_PROCESSOR OEM_Processor )
{
	TProcess *p = (TProcess *)OEM_Processor; 
	p->state = -1;

	atUIAPI_FreeMem( p );
}

atVOID          OEM_UI_SuspendProcessor( OEM_UI_PROCESSOR OEM_Processor )
{
	TProcess *p = (TProcess *)OEM_Processor; 
	p->state = 2;
}

atVOID          OEM_UI_ResumeProcessor( OEM_UI_PROCESSOR OEM_Processor )
{
	TProcess *p = (TProcess *)OEM_Processor; 
	if( p->state != 2 ) return;

	p->state = 0;
	OEM_UI_WakeProcessor( OEM_Processor );
}

atVOID          OEM_UI_WakeProcessor( OEM_UI_PROCESSOR OEM_Processor )
{
	TProcess *p = (TProcess *)OEM_Processor; 
	if( p == atNULL ) return;
	
	if( p->state != 0 ) return;

	p->state = 1;
	p->callback( (OEM_UI_PROCESSOR)p, p->param );
	p->state = 0;
}

atBOOL          OEM_UI_IsActiveProcessor( OEM_UI_PROCESSOR OEM_Processor )
{
	TProcess *p = (TProcess *)OEM_Processor; 
	return p->state == 1;
}

OEM_UI_CRSECTION    OEM_UI_CreateCriticalSection(void)
{
//	return (OEM_UI_CRSECTION)SYSAPI_CreateCriticalSection();
}

atVOID          OEM_UI_DestroyCriticalSection( OEM_UI_CRSECTION OEM_CrSection )
{
//	SYSAPI_DestroyCriticalSection( (SYS_CRSECTION)OEM_CrSection );
}

atVOID          OEM_UI_EnterCriticalSection( OEM_UI_CRSECTION OEM_CrSection )
{
//	SYSAPI_EnterCriticalSection( (SYS_CRSECTION)OEM_CrSection );
}

atVOID          OEM_UI_LeaveCriticalSection( OEM_UI_CRSECTION OEM_CrSection )
{
//	SYSAPI_LeaveCriticalSection( (SYS_CRSECTION)OEM_CrSection );
}

atLONG          OEM_UI_MessageLoop( atLONG lParam1, atLONG lParam2 )
{
	return 0;
}

//================================================================
#elif (PROC_IMPL == THREAD_PROC) || (PROC_IMPL == THREAD_PROC_2)
//================================================================


#define SYS_EVENT_WAKEUP 0x0001

typedef struct tagProcess {
	atTCHAR szProcName[32];
	int state;
	void *param;
	atBOOL (*callback)(OEM_UI_PROCESSOR, void *);
	SYS_TASK thread;
	SYS_EVENT events;	
} TProcess;


#if PROC_IMPL == THREAD_PROC
static void cUIThread(SYS_TASK hTask, void* pUserData)
{
	TProcess *p = (TProcess *)pUserData; 
	atBOOL r ;
	
	SYSAPI_SetWaitSignal( p->events, SYS_EVENT_WAKEUP );
	
	while( SYSAPI_IsTaskActivate(hTask) ){
		SYS_SIGNAL sig = SYSAPI_WaitEvents( p->events, atFALSE, SYS_WAIT_INFINITE );
		if( sig & SYS_EVENT_WAKEUP ){
			p->state = 1;
			r = p->callback( (OEM_UI_PROCESSOR)p, p->param );
			p->state = 0;

			if(!r) SYSAPI_ClearSignal( p->events, SYS_EVENT_WAKEUP );
		}
	}
	
	//	if( r ) OEM_UI_SuspendProcessor( (OEM_UI_PROCESSOR)p );
}
#else
static void cUIThread(SYS_TASK hTask, void* pUserData)
{
	TProcess *p = (TProcess *)pUserData; 
	atBOOL r ;
	
	SYSAPI_SetWaitSignal( p->events, SYS_EVENT_WAKEUP );
	
	while( SYSAPI_IsTaskActivate(hTask) ){
		//		SYS_SIGNAL sig = SYSAPI_WaitEvents( p->events, atFALSE, SYS_WAIT_INFINITE );
		//		if( sig & SYS_EVENT_WAKEUP ){
		//			SYSAPI_ClearSignal( p->events, SYS_EVENT_WAKEUP );
		p->state = 1;
		r = p->callback( (OEM_UI_PROCESSOR)p, p->param );
		p->state = 0;
		//		}

		SYSAPI_Sleep(5);
	}

	//	if( r ) OEM_UI_SuspendProcessor( (OEM_UI_PROCESSOR)p );
}
#endif

atBOOL  	    OEM_UI_InitProcessor( atLONG lSysInst, atLONG lInitData1, atLONG lInitData2 )
{
	return atTRUE;
}

atVOID  	    OEM_UI_FinalProcessor(void)
{
}

OEM_UI_PROCESSOR    OEM_UI_CreateProcessor( atLPTSTR szProcName, atINT nProcType, atBOOL (*callback)(OEM_UI_PROCESSOR, void*), void *param )
{
	TProcess *p;
	atTCHAR eventName[100];

	p =	(TProcess *)atUIAPI_AllocMem( sizeof(TProcess) );
	SYSAPI_memset(p, 0, sizeof(TProcess));
	SYSAPI_tcsncpy( p->szProcName, szProcName, 30 );
	p->state = 0;

	SYSAPI_tcscpy(eventName, szProcName);
	SYSAPI_tcscat(eventName,_AT("_event"));
	
	p->callback = callback;
	p->param = param;
	p->events = SYSAPI_CreateEventGroup(eventName);
	p->thread = SYSAPI_CreateTask(szProcName, cUIThread, p, atTRUE, 900 *1024);

	SYSAPI_SetWaitSignal( p->events, SYS_EVENT_WAKEUP);
		
    return (OEM_UI_PROCESSOR)p;
}

atVOID          OEM_UI_DestroyProcessor( OEM_UI_PROCESSOR OEM_Processor )
{
	TProcess *p = (TProcess *)OEM_Processor; 
	p->state = -1;
	//SYSAPI_TerminateTask( p->thread );
	SYSAPI_RequestTerminate( p->thread );
	SYSAPI_WaitForTerminate( p->thread, SYS_WAIT_INFINITE );
	SYSAPI_DestroyTask( p->thread );
	SYSAPI_DestroyEventGroup( p->events );

	atUIAPI_FreeMem( p );
}

atVOID          OEM_UI_SuspendProcessor( OEM_UI_PROCESSOR OEM_Processor )
{
	TProcess *p = (TProcess *)OEM_Processor; 
	SYSAPI_SuspendTask( p->thread );
}

atVOID          OEM_UI_ResumeProcessor( OEM_UI_PROCESSOR OEM_Processor )
{
	TProcess *p = (TProcess *)OEM_Processor; 
	SYSAPI_ResumeTask( p->thread );
}

atVOID          OEM_UI_WakeProcessor( OEM_UI_PROCESSOR OEM_Processor )
{
	TProcess *p = (TProcess *)OEM_Processor; 
	SYSAPI_SendSignal( p->events, SYS_EVENT_WAKEUP );
}

atBOOL          OEM_UI_IsActiveProcessor( OEM_UI_PROCESSOR OEM_Processor )
{
	TProcess *p = (TProcess *)OEM_Processor; 
	//return SYSAPI_GetTaskState(p->thread) == SYS_TASKSTATE_RUNNING;
	return p->state == 1;
}

OEM_UI_CRSECTION    OEM_UI_CreateCriticalSection(void)
{
	return (OEM_UI_CRSECTION)SYSAPI_CreateCriticalSection();
}

atVOID          OEM_UI_DestroyCriticalSection( OEM_UI_CRSECTION OEM_CrSection )
{
	SYSAPI_DestroyCriticalSection( (SYS_CRSECTION)OEM_CrSection );
}

atVOID          OEM_UI_EnterCriticalSection( OEM_UI_CRSECTION OEM_CrSection )
{
	SYSAPI_EnterCriticalSection( (SYS_CRSECTION)OEM_CrSection );
}

atVOID          OEM_UI_LeaveCriticalSection( OEM_UI_CRSECTION OEM_CrSection )
{
	SYSAPI_LeaveCriticalSection( (SYS_CRSECTION)OEM_CrSection );
}

atLONG          OEM_UI_MessageLoop( atLONG lParam1, atLONG lParam2 )
{
	return 0;
}

//================================================================
#elif PROC_IMPL == TIMER_PROC
//================================================================


#include "OEM_UI_Timer.h"


typedef struct tagProcess {
	atTCHAR szProcName[32];
	int state;
	void *param;
	atBOOL (*callback)(OEM_UI_PROCESSOR, void *);
	atHTIMER timer;
} TProcess;


static void cUITimer(atHTIMER hTimer, void *param)
{
	TProcess *p = (TProcess *)param; 
	if(p)
	{
	 	atBOOL r = p->callback( (OEM_UI_PROCESSOR)p, p->param );

		if( r ) OEM_UI_SuspendProcessor( (OEM_UI_PROCESSOR)p );
	}
}


atBOOL  	    OEM_UI_InitProcessor( atLONG lSysInst, atLONG lInitData1, atLONG lInitData2 )
{
	return atTRUE;
}

atVOID  	    OEM_UI_FinalProcessor(void)
{
}

OEM_UI_PROCESSOR    OEM_UI_CreateProcessor( atLPTSTR szProcName, atINT nProcType, atBOOL (*callback)(OEM_UI_PROCESSOR, void*), void *param )
{
	TProcess *p;

	p =	(TProcess *)atUIAPI_AllocMem( sizeof(TProcess) );
	if(!p) return 0;
	SYSAPI_memset(p, 0, sizeof(TProcess));
	SYSAPI_tcsncpy( p->szProcName, szProcName, 30 );
	p->state    =0 ;
	p->callback = callback;
	p->param    = param;
	p->timer    = atUIAPI_AddTimer( 200, cUITimer, p, atTRUE, atFALSE );      

    return (OEM_UI_PROCESSOR)p;
}

atVOID          OEM_UI_DestroyProcessor( OEM_UI_PROCESSOR OEM_Processor )
{
	TProcess *p = (TProcess *)OEM_Processor; 
    atUIAPI_RemoveTimer( p->timer );
	p->state = -1;

	atUIAPI_FreeMem( p );
}

atVOID          OEM_UI_SuspendProcessor( OEM_UI_PROCESSOR OEM_Processor )
{
	TProcess *p = (TProcess *)OEM_Processor; 
    atUIAPI_EnableTimer( p->timer, atFALSE );
}

atVOID          OEM_UI_ResumeProcessor( OEM_UI_PROCESSOR OEM_Processor )
{
	TProcess *p = (TProcess *)OEM_Processor; 
    atUIAPI_EnableTimer( p->timer, atTRUE );
}

atVOID          OEM_UI_WakeProcessor( OEM_UI_PROCESSOR OEM_Processor )
{
	TProcess *p = (TProcess *)OEM_Processor; 
    atUIAPI_EnableTimer( p->timer, atTRUE );
}

atBOOL          OEM_UI_IsActiveProcessor( OEM_UI_PROCESSOR OEM_Processor )
{
	TProcess *p = (TProcess *)OEM_Processor; 
    return atUIAPI_IsTimerEnabled( p->timer );
	return atFALSE;
}


OEM_UI_CRSECTION    OEM_UI_CreateCriticalSection(void)
{
	int *v = (int *)atUIAPI_AllocMem( sizeof(int) );
	*v = 0;
	return (OEM_UI_CRSECTION)v;
}

atVOID          OEM_UI_DestroyCriticalSection( OEM_UI_CRSECTION OEM_CrSection )
{
	atUIAPI_FreeMem( (void *)OEM_CrSection );
}

atVOID          OEM_UI_EnterCriticalSection( OEM_UI_CRSECTION OEM_CrSection )
{
	int *v = (int *)OEM_CrSection;
	//while( *v > 0 ) Sleep(1);
	*v++;
}

atVOID          OEM_UI_LeaveCriticalSection( OEM_UI_CRSECTION OEM_CrSection )
{
	int *v = (int *)OEM_CrSection;
	
	*v--;
}

atLONG          OEM_UI_MessageLoop( atLONG lParam1, atLONG lParam2 )
{
	return 0;
}

//================================================================
#elif PROC_IMPL == THREAD_TIMER_PROC
//================================================================


#include "OEM_UI_Timer.h"


typedef struct tagProcess {
	atTCHAR szProcName[32];
	int state;
	void *param;
	atBOOL (*callback)(OEM_UI_PROCESSOR, void *);
	atHTIMER timer;
} TProcess;


static void cUITimer(atHTIMER hTimer, void *param)
{
	TProcess *p = (TProcess *)param; 
	if(p)
	{
	 	atBOOL r = p->callback( (OEM_UI_PROCESSOR)p, p->param );

		if( r ) OEM_UI_SuspendProcessor( (OEM_UI_PROCESSOR)p );
	}
}


atBOOL  	    OEM_UI_InitProcessor( atLONG lSysInst, atLONG lInitData1, atLONG lInitData2 )
{
	return atTRUE;
}

atVOID  	    OEM_UI_FinalProcessor(void)
{
}

OEM_UI_PROCESSOR    OEM_UI_CreateProcessor( atLPTSTR szProcName, atINT nProcType, atBOOL (*callback)(OEM_UI_PROCESSOR, void*), void *param )
{
	TProcess *p;

	p =	(TProcess *)atUIAPI_AllocMem( sizeof(TProcess) );
	if(!p) return 0;
	SYSAPI_memset(p, 0, sizeof(TProcess));
	SYSAPI_tcsncpy( p->szProcName, szProcName, 30 );
	p->state    =0 ;
	p->callback = callback;
	p->param    = param;
	p->timer    = atUIAPI_AddTimer( 200, cUITimer, p, atTRUE, atFALSE );

    return (OEM_UI_PROCESSOR)p;
}

atVOID          OEM_UI_DestroyProcessor( OEM_UI_PROCESSOR OEM_Processor )
{
	TProcess *p = (TProcess *)OEM_Processor; 
    atUIAPI_RemoveTimer( p->timer );
	p->state = -1;

	atUIAPI_FreeMem( p );
}

atVOID          OEM_UI_SuspendProcessor( OEM_UI_PROCESSOR OEM_Processor )
{
	TProcess *p = (TProcess *)OEM_Processor; 
    atUIAPI_EnableTimer( p->timer, atFALSE );
}

atVOID          OEM_UI_ResumeProcessor( OEM_UI_PROCESSOR OEM_Processor )
{
	TProcess *p = (TProcess *)OEM_Processor; 
    atUIAPI_EnableTimer( p->timer, atTRUE );
}

atVOID          OEM_UI_WakeProcessor( OEM_UI_PROCESSOR OEM_Processor )
{
	TProcess *p = (TProcess *)OEM_Processor; 
    atUIAPI_EnableTimer( p->timer, atTRUE );
}

atBOOL          OEM_UI_IsActiveProcessor( OEM_UI_PROCESSOR OEM_Processor )
{
	TProcess *p = (TProcess *)OEM_Processor; 
    return atUIAPI_IsTimerEnabled( p->timer );
	return atFALSE;
}


OEM_UI_CRSECTION    OEM_UI_CreateCriticalSection(void)
{
	int *v = (int *)atUIAPI_AllocMem( sizeof(int) );
	*v = 0;
	return (OEM_UI_CRSECTION)v;
}

atVOID          OEM_UI_DestroyCriticalSection( OEM_UI_CRSECTION OEM_CrSection )
{
	atUIAPI_FreeMem( (void *)OEM_CrSection );
}

atVOID          OEM_UI_EnterCriticalSection( OEM_UI_CRSECTION OEM_CrSection )
{
	int *v = (int *)OEM_CrSection;
	//while( *v > 0 ) Sleep(1);
	*v++;
}

atVOID          OEM_UI_LeaveCriticalSection( OEM_UI_CRSECTION OEM_CrSection )
{
	int *v = (int *)OEM_CrSection;
	
	*v--;
}

atLONG          OEM_UI_MessageLoop( atLONG lParam1, atLONG lParam2 )
{
	return 0;
}

//================================================================
#endif



void init_uiapi_proc_oem() 
{
	OEM_UI_PROC_T oem_funcs = {
		OEM_UI_InitProcessor,
		OEM_UI_FinalProcessor,

		OEM_UI_CreateProcessor,
		OEM_UI_DestroyProcessor,

		OEM_UI_SuspendProcessor,
		OEM_UI_ResumeProcessor,
		OEM_UI_WakeProcessor,
		OEM_UI_IsActiveProcessor,

		OEM_UI_CreateCriticalSection,
		OEM_UI_DestroyCriticalSection,
		OEM_UI_EnterCriticalSection,
		OEM_UI_LeaveCriticalSection,

		OEM_UI_MessageLoop
	};

	OEM_UI_Proc_SetOemFn( &oem_funcs );
}
