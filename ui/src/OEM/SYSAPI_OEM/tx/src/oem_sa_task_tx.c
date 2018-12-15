#include "SYSAPI.h"
#include "OEM_SA_Task.h"
#include "tx_api.h"


#define MAX_OEM_THREAD_NUM 32

#ifndef LPVOID
typedef void* LPVOID;
#endif

typedef struct __TThread {
	atTCHAR szTaskName[32];
	TX_THREAD handle;
	//HANDLE event;
	DWORD id;
	int state;
	DWORD wait_signal;
	DWORD cur_signal;
	void (*callback)(SYS_TASK, void*);
	void *param;
} _TThread;

typedef struct __TEvent {
	TX_EVENT_FLAGS_GROUP event;
	DWORD wait_signal;
	DWORD cur_signal;
//	_TThread wait_threads[MAX_OEM_THREAD_NUM];
//	int wait_thread_cnt;	
} _TEvent;

static _TThread *thread[MAX_OEM_THREAD_NUM];


atUINT g_mutex_count = 0;



/*
static unsigned long g_mutex_count = 0;
static DWORD WINAPI _threadfunc( LPVOID vv )
{
	_TThread *tt = (_TThread *)vv;

	tt->callback( (atHANDLE)tt, tt->param );

	tt->state = SYS_TASKSTATE_TERMINATED;
	
//	tt->cur_signal |= SYS_EVENT_TERMINATE;
//	SetEvent( tt->event );
	
	return 0;
}
*/

atBOOL  	    OEM_SYSAPI_InitTask( atLONG lSysInst, atLONG lInitData1, atLONG lInitData2 )
{
/*
	int i;
	_TThread *tt;
	
	for( i = 0; i < MAX_OEM_THREAD_NUM; i++ ){
		thread[i] = NULL;
	}

	tt = (_TThread *)SYSAPI_AllocMem( sizeof(_TThread) );
	SYSAPI_memset(tt, 0, sizeof(_TThread));
	SYSAPI_tcsncpy( tt->szTaskName, _AT("MainTask"), 30 );
//	tt->event = CreateEvent( NULL, FALSE, FALSE, NULL );;
	tt->param = NULL;
	tt->callback = NULL;
	tt->handle = GetCurrentThread();
	tt->id = GetCurrentThreadId();
	tt->state = SYS_TASKSTATE_RUNNING;
	thread[0] = tt;
*/

	return atTRUE;
}

atVOID  	    OEM_SYSAPI_FinalTask(void)
{
/*
	int i;

	for( i = 1; i < MAX_OEM_THREAD_NUM; i++ ){
		if( thread[i] != NULL ){
			OEM_SYSAPI_TerminateTask( (OEM_SYS_TASK)thread[i] );
			thread[i] = NULL;
		}
	}	

//	CloseHandle( thread[0]->event );
	SYSAPI_FreeMem( thread[0] );
	thread[0] = NULL;
*/	
}


OEM_SYS_TASK         OEM_SYSAPI_CreateTask( atLPTSTR szTaskName, void (*callback)(atHANDLE,void*), void *param, atBOOL bStart, atLONG lStackSize )
{
	int idx;
//	DWORD ThreadId;
	_TThread *tt;

/*
	idx = find_free_handle();
	if( idx < 0 ) return atNULL; // 32개를 다 채웠다.
		
//	tt = __NEW( _TThread );	
	tt = (_TThread *)SYSAPI_AllocMem( sizeof(_TThread) );
	SYSAPI_memset(tt, 0, sizeof(_TThread));
	SYSAPI_tcsncpy( tt->szTaskName, szTaskName, 30 );
//	tt->event = CreateEvent( NULL, FALSE, FALSE, NULL );
	tt->param = param;
	tt->callback = callback;

	tt->handle = CreateThread( NULL, lStackSize, _threadfunc, (void *)tt, (bStart)?0:CREATE_SUSPENDED, &ThreadId );
	tt->id = ThreadId;
	tt->state = (bStart)? SYS_TASKSTATE_RUNNING : SYS_TASKSTATE_SUSPEND;
	//tt->event = CreateEvent( NULL, FALSE, FALSE, NULL );
	//tt->id = ThreadId;
	//tt->param = param;
	//tt->callback = callback;

	thread[idx] = tt;
*/	
	
	return (OEM_SYS_TASK)tt;
}

atVOID          OEM_SYSAPI_DestroyTask( OEM_SYS_TASK hOemTask )
{
/*
	int idx;
	_TThread *tt = (_TThread *)hOemTask;

	idx = find_match_thread( tt->handle );
	if( idx < 0 ) return; // 무언가 문제가 있다면...
	
	if( tt->state < SYS_TASKSTATE_TERMINATING ){
		OEM_SYSAPI_TerminateTask( hOemTask );
		OEM_SYSAPI_WaitForTerminate( hOemTask, SYS_WAIT_INFINITE );
	}

//	CloseHandle( tt->event );
	CloseHandle( tt->handle );

//	__DELETE( tt );
	SYSAPI_FreeMem( tt );

	if( idx >= 0 ) thread[idx] = NULL;
*/	
}


atBOOL	OEM_SYSAPI_GetTaskInfo( OEM_SYS_TASK hOemTask, SYS_TASK_INFO *lpTaskInfo )
{
/*
	_TThread *tt = (_TThread *)hOemTask;
	if( tt == atNULL ) return atFALSE;

	SYSAPI_tcsncpy( lpTaskInfo->szTaskName, tt->szTaskName, 30 );
	lpTaskInfo->fnTaskFunc = tt->callback;
	lpTaskInfo->nState = OEM_SYSAPI_GetTaskState(hOemTask);
	lpTaskInfo->lpParam = tt->param;
*/

	return atTRUE;
}

atINT         OEM_SYSAPI_GetTaskState( OEM_SYS_TASK hOemTask )
{
	_TThread *tt = (_TThread *)hOemTask;
	return tt->state;
}

atVOID		OEM_SYSAPI_TerminateTask( OEM_SYS_TASK hOemTask )
{
	_TThread *tt = (_TThread *)hOemTask;
	if( tt->state >= SYS_TASKSTATE_TERMINATING ) return;
	
	tt->state = SYS_TASKSTATE_TERMINATING;

//	tt->cur_signal |= SYS_EVENT_TERMINATE;

//	SetEvent( tt->event );
}

atBOOL    OEM_SYSAPI_WaitForTerminate( OEM_SYS_TASK hOemTask, atLONG lMilliSec )
{
	_TThread *tt = (_TThread *)hOemTask;

/*
	if( tt->state == SYS_TASKSTATE_TERMINATED ) return atTRUE;
	if( tt->id == GetCurrentThreadId() ) return atFALSE;
	
//	tt->wait_signal = SYS_EVENT_TERMINATE;
	
	while( tt->state != SYS_TASKSTATE_TERMINATED ){
		WaitForSingleObject( tt->handle, (lMilliSec==SYS_WAIT_INFINITE)?1000:lMilliSec );
		if( lMilliSec != SYS_WAIT_INFINITE ) break;
	}
*/

	return (tt->state == SYS_TASKSTATE_TERMINATED);
}

atVOID     	    OEM_SYSAPI_SuspendTask( OEM_SYS_TASK hOemTask )
{
	_TThread *tt = (_TThread *)hOemTask;

	if( tt->state != SYS_TASKSTATE_RUNNING ) return;
	tt->state = SYS_TASKSTATE_SUSPEND;
	
//	SuspendThread( tt->handle );	
}

atVOID          OEM_SYSAPI_ResumeTask( OEM_SYS_TASK hOemTask )
{
	_TThread *tt = (_TThread *)hOemTask;

	if( tt->state != SYS_TASKSTATE_SUSPEND ) return;
	tt->state = SYS_TASKSTATE_RUNNING;
	
//	ResumeThread( tt->handle );	
}


OEM_SYS_TASK		OEM_SYSAPI_GetCurrentTask( NOARGS )
{
/*
	int idx;
	DWORD task_id;
	
	task_id = GetCurrentThreadId();

	idx = find_match_thread_id( task_id );

	return (idx >= 0)? (OEM_SYS_TASK)thread[idx] : atNULL;
*/
return atNULL;
}

OEM_SYS_TASKSIGNAL    OEM_SYSAPI_WaitEvents( OEM_SYS_EVENT hOemEvents, atBOOL bIsAll, atLONG lMilliSec )
{
	_TEvent *evt = (_TEvent *)hOemEvents;
//	int old_state;
	atDWORD cur_signal;

//	if( (tt->state == SYS_TASKSTATE_TERMINATING) || (tt->state == SYS_TASKSTATE_TERMINATED) ) return SYS_EVENT_TERMINATE;
	
	if( bIsAll && ((evt->cur_signal & evt->wait_signal) == evt->wait_signal) ) return evt->cur_signal;
	if( !bIsAll && ((evt->cur_signal & evt->wait_signal) != 0x00) ) return evt->cur_signal;
	
//	evt->cur_signal = 0;
//	evt->wait_signal |= SYS_EVENT_TERMINATE;

//	old_state = tt->state;
//	tt->state = SYS_TASKSTATE_SUSPEND;
//	while( evt->state < SYS_TASKSTATE_TERMINATING ){
	while( !(evt->cur_signal & SYS_EVENT_TERMINATE) ){
		tx_event_flags_get( &evt->event, evt->wait_signal, TX_AND_CLEAR, &cur_signal, (lMilliSec==SYS_WAIT_INFINITE)?1000:lMilliSec );
		if( lMilliSec != SYS_WAIT_INFINITE ) break;
		if( bIsAll ){
			if( (cur_signal & evt->wait_signal) == evt->wait_signal ) break;
		} else {
			if( cur_signal & evt->wait_signal ) break;
		}
	}
	//WaitForSingleObject( tt->event, (lMilliSec==SYS_WAIT_INFINITE)?INFINITE:lMilliSec );
//	if( tt->state == SYS_TASKSTATE_SUSPEND ) tt->state = old_state;

	return evt->cur_signal;
}

atVOID          OEM_SYSAPI_Sleep( atDWORD dwMilliSec )
{
//	int old_state;
//	_TThread *tt = (_TThread *)OEM_SYSAPI_GetCurrentTask();
//	if(!tt) return;

//	if( tt->state != SYS_TASKSTATE_RUNNING ) return;
	
//	old_state = tt->state;
//	tt->state = SYS_TASKSTATE_SUSPEND;
	tx_thread_sleep( dwMilliSec );
//	tt->state = old_state;
}

OEM_SYS_EVENT 	OEM_SYSAPI_CreateEventGroup( NOARGS )
{
	_TEvent *evt;
	TX_EVENT_FLAGS_GROUP event;
	UINT status;
	
	status = tx_event_flags_create( &event, "xxxxxxxxxx" );
	if( status != TX_SUCCESS ){
		return atNULL;
	}
	
	evt = (_TEvent *)SYSAPI_AllocMem( sizeof(_TEvent) );
	SYSAPI_memset(evt, 0, sizeof(_TEvent));

	evt->event = event;
	evt->wait_signal = 0x00;
	evt->cur_signal = 0x00;

	return (OEM_SYS_EVENT)evt;
}

atVOID		OEM_SYSAPI_DestroyEventGroup( OEM_SYS_EVENT hOemEvents )
{
	_TEvent *evt = (_TEvent *)hOemEvents;
	if( evt == atNULL ) return;

	
	/*if(evt->event)*/	tx_event_flags_delete( &evt->event );
	

	SYSAPI_FreeMem( evt );
}

OEM_SYS_TASKSIGNAL	OEM_SYSAPI_SetWaitSignal( OEM_SYS_EVENT hOemEvents, OEM_SYS_TASKSIGNAL dwSignal )
{
	_TEvent *evt = (_TEvent *)hOemEvents;
	if( evt == atNULL ) return (OEM_SYS_TASKSIGNAL)0x00;

	evt->wait_signal = dwSignal;

	return (OEM_SYS_TASKSIGNAL)evt->wait_signal;
}

OEM_SYS_TASKSIGNAL	OEM_SYSAPI_GetWaitSignal( OEM_SYS_EVENT hOemEvents )
{
	_TEvent *evt = (_TEvent *)hOemEvents;
	if( evt == atNULL ) return (OEM_SYS_TASKSIGNAL)0x00;

	return (OEM_SYS_TASKSIGNAL)evt->wait_signal;
}

OEM_SYS_TASKSIGNAL	OEM_SYSAPI_GetCurrentSignal( OEM_SYS_EVENT hOemEvents )
{
	_TEvent *evt = (_TEvent *)hOemEvents;
	if( evt == atNULL ) return (OEM_SYS_TASKSIGNAL)0x00;

	return (OEM_SYS_TASKSIGNAL)evt->cur_signal;
}

OEM_SYS_TASKSIGNAL		OEM_SYSAPI_SendSignal( OEM_SYS_EVENT hOemEvents, OEM_SYS_TASKSIGNAL dwSignal )
{
	_TEvent *evt = (_TEvent *)hOemEvents;
	if( evt == atNULL ) return (OEM_SYS_TASKSIGNAL)0x00;

	evt->cur_signal |= dwSignal;

	if( evt->cur_signal & evt->wait_signal ){
		tx_event_flags_set( &evt->event, dwSignal, TX_OR );
//		SetEvent( evt->event );
	}
	
	return evt->cur_signal;
}

OEM_SYS_TASKSIGNAL		OEM_SYSAPI_ClearSignal( OEM_SYS_EVENT hOemEvents, OEM_SYS_TASKSIGNAL dwSignal )
{
	_TEvent *evt = (_TEvent *)hOemEvents;
	if( evt == atNULL ) return (OEM_SYS_TASKSIGNAL)0x00;

	evt->cur_signal &= ~dwSignal;

	if( !(evt->cur_signal & evt->wait_signal) ){
//		ResetEvent( evt->event );
	}
	
	return evt->cur_signal;
}


OEM_SYS_CRSECTION    OEM_SYSAPI_CreateCriticalSection(void)
{
	TX_MUTEX* pMutex;
	UINT status;
	char mname[36];
	pMutex = (TX_MUTEX*)SYSAPI_AllocMem( sizeof(TX_MUTEX));
	if(!pMutex) return NULL;

	g_mutex_count++;

	SYSAPI_memset(pMutex, 0, sizeof(TX_MUTEX));

	sprintf(mname, "MTX_%d", g_mutex_count);

	status = tx_mutex_create(pMutex, mname, TX_NO_INHERIT);
	if(status != TX_SUCCESS) return NULL;

	return (OEM_SYS_CRSECTION)pMutex;
}

atVOID          OEM_SYSAPI_DestroyCriticalSection( OEM_SYS_CRSECTION hCrSection )
{
	TX_MUTEX* pMutex;

	pMutex = (TX_MUTEX*)hCrSection;
	if(!pMutex) return;

	tx_mutex_delete(pMutex);

	SYSAPI_FreeMem(pMutex);
}

atVOID          OEM_SYSAPI_EnterCriticalSection( OEM_SYS_CRSECTION hCrSection )
{
	TX_MUTEX* pMutex;

	pMutex = (TX_MUTEX*)hCrSection;
	if(!pMutex) return;

	tx_mutex_get(pMutex, TX_WAIT_FOREVER);
}

atVOID          OEM_SYSAPI_LeaveCriticalSection( OEM_SYS_CRSECTION hCrSection )
{
	TX_MUTEX* pMutex;

	pMutex = (TX_MUTEX*)hCrSection;
	if(!pMutex) return;

	tx_mutex_put(pMutex);
}

