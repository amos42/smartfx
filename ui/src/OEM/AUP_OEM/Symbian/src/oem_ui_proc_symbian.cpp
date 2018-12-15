#include "UI_MNG.h"
#include "UI_API/UI_API.h"
#include "OEM_UI_Proc.h"
#include "OEM_UI_Timer.h"
#include "SYSAPI.h"




#define THREAD_PROC 0


#if THREAD_PROC
//================================================================


typedef struct tagProcess {
	atTCHAR szProcName[32];
	int state;
	void *param;
	atBOOL (*callback)(OEM_UI_PROCESSOR, void *);
	SYS_TASK thread;
} TProcess;


static void cUIThread(SYS_TASK hTask, void* pUserData)
{
	TProcess *p = (TProcess *)pUserData; 
	atBOOL r ;

	while( SYSAPI_IsTaskActivate(hTask) ){
		SYSAPI_WaitEvent( 1, atFALSE, SYS_WAIT_INFINITE );
		SYSAPI_ClearSignal( hTask, 1 );

		r = p->callback( (OEM_UI_PROCESSOR)p, p->param );
	}
	
//	if( r ) OEM_UI_SuspendProcessor( (OEM_UI_PROCESSOR)p );
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
	SYSAPI_memset(p, 0, sizeof(TProcess));
	SYSAPI_tcsncpy( p->szProcName, szProcName, 30 );
	p->state = 0;
	p->callback = callback;
	p->param = param;
	p->thread = SYSAPI_CreateTask(szProcName, cUIThread, p, atTRUE, 40*1024);

    return (OEM_UI_PROCESSOR)p;
}

atVOID          OEM_UI_DestroyProcessor( OEM_UI_PROCESSOR OEM_Processor )
{
	TProcess *p = (TProcess *)OEM_Processor; 
	p->state = -1;
	SYSAPI_TerminateTask( p->thread );
	SYSAPI_WaitForTerminate( p->thread, SYS_WAIT_INFINITE );
	SYSAPI_DestroyTask( p->thread );

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
	SYSAPI_SendSignal( p->thread, SYS_EVENT_WAKEUP );
}

atBOOL          OEM_UI_IsActiveProcessor( OEM_UI_PROCESSOR OEM_Processor )
{
	TProcess *p = (TProcess *)OEM_Processor; 
	return SYSAPI_GetTaskState(p->thread) == SYS_TASKSTATE_RUNNING;
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


//================================================================
#else
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

	atBOOL r = p->callback( (OEM_UI_PROCESSOR)p, p->param );

	if( r ) OEM_UI_SuspendProcessor( (OEM_UI_PROCESSOR)p );
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
	SYSAPI_memset(p, 0, sizeof(TProcess));
	SYSAPI_tcsncpy( p->szProcName, szProcName, 30 );
	p->state = 0;
	p->callback = callback;
	p->param = param;
	p->timer = atUIAPI_AddTimer( 10, cUITimer, p, atTRUE, atFALSE );      

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
	
	// fix-me 
	// symbian에서 timer enable/disable이 반복될 경우 문제가 발생하여
	// disable시키지 않도록 변경함.
    //atUIAPI_EnableTimer( p->timer, atFALSE );
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


//================================================================
#endif












/*
#include "MPThread.h"
#include "S60/SymThread.h"

#define _USE_TIMER_PROC  1 

typedef struct tagProcess {
	atTCHAR szProcName[32];
	int state;
	void *param;
	atBOOL (*callback)(OEM_UI_PROCESSOR, void *);
#if _USE_TIMER_PROC	
	atHTIMER timer;
#else
	CSymThread *thread;
#endif
} TProcess;

 

#if _USE_TIMER_PROC
static void cUITimer(atHTIMER hTimer, void *param)
{
	TProcess *p = (TProcess *)param; 

	atBOOL r = p->callback( (OEM_UI_PROCESSOR)p, p->param );

	// fix-me
	//if( r ) OEM_UI_SuspendProcessor( (OEM_UI_PROCESSOR)p );
}
#else

static DWORD cUIThread(MPThread* pMPThread, void* pUserData)
{
	TProcess *p = (TProcess *)pUserData; 
	atBOOL r ;

	while( p->state != -1 ){
		pMPThread->WaitForWakeup();
		r = p->callback( (OEM_UI_PROCESSOR)p, p->param );
	}
	
//	if( r ) OEM_UI_SuspendProcessor( (OEM_UI_PROCESSOR)p );

	return 0;
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

	p =	(TProcess *)atUIAPI_AllocMem( sizeof(TProcess) );
	SYSAPI_memset(p, 0, sizeof(TProcess));
	SYSAPI_tcsncpy( p->szProcName, szProcName, 30 );
	p->state = 0;
	p->callback = callback;
	p->param = param;
#if _USE_TIMER_PROC	
	p->timer = atUIAPI_AddTimer( 10, cUITimer, p, atTRUE, atFALSE );
#else
	p->thread = new CSymThread(_AT("EventQueue"));
	p->thread->Start( atNULL, 0x40000, cUIThread, p );
#endif
    return (OEM_UI_PROCESSOR)p;
}

atVOID          OEM_UI_DestroyProcessor( OEM_UI_PROCESSOR OEM_Processor )
{
	TProcess *p = (TProcess *)OEM_Processor;
#if _USE_TIMER_PROC
    atUIAPI_RemoveTimer( p->timer );
#else
	p->state = -1;
//	p->thread->Wakeup();
	p->thread->Stop();
	delete p->thread;
#endif
	atUIAPI_FreeMem( p );
	
}

atVOID          OEM_UI_SuspendProcessor( OEM_UI_PROCESSOR OEM_Processor )
{
	TProcess *p = (TProcess *)OEM_Processor;
#if _USE_TIMER_PROC
    atUIAPI_EnableTimer( p->timer, atFALSE );
#endif
//	p->thread->Stop();
}

atVOID          OEM_UI_ResumeProcessor( OEM_UI_PROCESSOR OEM_Processor )
{
	TProcess *p = (TProcess *)OEM_Processor;
#if _USE_TIMER_PROC
    atUIAPI_EnableTimer( p->timer, atTRUE );
#endif
//	p->thread->Stop();
}

atVOID          OEM_UI_WakeProcessor( OEM_UI_PROCESSOR OEM_Processor )
{
	TProcess *p = (TProcess *)OEM_Processor;
#if _USE_TIMER_PROC	
    atUIAPI_EnableTimer( p->timer, atTRUE );
#else
	p->thread->Wakeup();
#endif
}

atBOOL          OEM_UI_IsActiveProcessor( OEM_UI_PROCESSOR OEM_Processor )
{
	TProcess *p = (TProcess *)OEM_Processor;
#if _USE_TIMER_PROC	
    return atUIAPI_IsTimerEnabled( p->timer );
#else
//	return !p->thread->isWakeup();

	return atFALSE;
#endif
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

*/




/*

typedef struct tagProcess {
	int state;
	void *param;
	atBOOL (*callback)(OEM_UI_PROCESSOR, void *);
//	atHTIMER timer;
	SYS_TASK thread;
} TProcess;


static void cUIThread(SYS_TASK hTask, void* pUserData)
{
	TProcess *p = (TProcess *)pUserData; 
	atBOOL r ;

	while( p->state != -1 ){
		SYSAPI_WaitEvent( hTask, 1, SYS_WAIT_INFINITE );
		SYSAPI_ClearSignal( hTask, 1 );

		r = p->callback( (OEM_UI_PROCESSOR)p, p->param );
	}
	
//	if( r ) OEM_UI_SuspendProcessor( (OEM_UI_PROCESSOR)p );
}



atBOOL  	    OEM_UI_InitProcessor( atLONG lSysInst, atLONG lInitData1, atLONG lInitData2 )
{
	return atTRUE;
}

atVOID  	    OEM_UI_FinalProcessor(void)
{
}

OEM_UI_PROCESSOR    OEM_UI_CreateProcessor( atINT nProcType, atBOOL (*callback)(OEM_UI_PROCESSOR, void*), void *param )
{
	TProcess *p;

	p =	(TProcess *)atUIAPI_AllocMem( sizeof(TProcess) );
	p->state = 0;
	p->callback = callback;
	p->param = param;
//	p->timer = atUIAPI_AddTimer( 10, cUITimer, p, atTRUE, atFALSE );      
	p->thread = SYSAPI_CreateTask(cUIThread, p, atTRUE);

    return (OEM_UI_PROCESSOR)p;
}

atVOID          OEM_UI_DestroyProcessor( OEM_UI_PROCESSOR OEM_Processor )
{
	TProcess *p = (TProcess *)OEM_Processor; 
//    atUIAPI_RemoveTimer( p->timer );
	p->state = -1;
//	p->thread->Wakeup();
//	p->thread->Stop();
	SYSAPI_DestroyTask( p->thread );

	atUIAPI_FreeMem( p );
}

atVOID          OEM_UI_SuspendProcessor( OEM_UI_PROCESSOR OEM_Processor )
{
	TProcess *p = (TProcess *)OEM_Processor; 
//    atUIAPI_EnableTimer( p->timer, atFALSE );
//	p->thread->Stop();
	SYSAPI_SuspendTask( p->thread );
}

atVOID          OEM_UI_ResumeProcessor( OEM_UI_PROCESSOR OEM_Processor )
{
	TProcess *p = (TProcess *)OEM_Processor; 
//    atUIAPI_EnableTimer( p->timer, atFALSE );
//	p->thread->Stop();
	SYSAPI_ResumeTask( p->thread );
}

atVOID          OEM_UI_WakeProcessor( OEM_UI_PROCESSOR OEM_Processor )
{
	TProcess *p = (TProcess *)OEM_Processor; 
//    atUIAPI_EnableTimer( p->timer, atTRUE );
//	p->thread->WaitForWakeup();
	SYSAPI_SendSignal( p->thread, SYS_EVENT_WAKEUP );
}

atBOOL          OEM_UI_IsActiveProcessor( OEM_UI_PROCESSOR OEM_Processor )
{
	TProcess *p = (TProcess *)OEM_Processor; 
//    return atUIAPI_IsTimerEnabled( p->timer );
//	return !p->thread->isWakeup();
	//return SYSAPI_GetTaskState(p->thread);
	return atFALSE;
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

*/
