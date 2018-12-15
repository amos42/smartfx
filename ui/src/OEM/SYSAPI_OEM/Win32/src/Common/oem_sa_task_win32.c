#include "SYSAPI.h"
#include "OEM_SA_Task.h"
#include <windows.h>

#define MAX_OEM_THREAD_NUM 32

typedef struct __TThread {
	atTCHAR szTaskName[32];
//	HANDLE handle, event;
	HANDLE handle;
	DWORD id;
	int state;
//	DWORD wait_signal;
//	DWORD cur_signal;
	void (*callback)(SYS_TASK, void*);
	void *param;
} _TThread;

typedef struct __TEvent {
	HANDLE event;
	DWORD wait_signal;
	DWORD cur_signal;
} _TEvent;


static _TThread *thread[MAX_OEM_THREAD_NUM];


static int find_free_handle(void)
{
	int i;
	
	for( i = 0; i < MAX_OEM_THREAD_NUM; i++ ){
		if( thread[i] == NULL ) return i;
	}

	return -1;
}

static int find_match_thread( HANDLE tt )
{
	int i;
	
	for( i = 0; i < MAX_OEM_THREAD_NUM; i++ )
	{
		if(!thread[i]) continue;
		if( thread[i]->handle == tt ) return i;
	}

	return -1;
}

static int find_match_thread_id( DWORD task_id )
{
	int i;
	
	for( i = 0; i < MAX_OEM_THREAD_NUM; i++ ){
		if( thread[i] == NULL ) continue;
		if( thread[i]->id == task_id ) return i;
	}

	return -1;
}

static int get_active_thread_num()
{
	
	int i;
	int cnt = 0;
	
	for( i = 0; i < MAX_OEM_THREAD_NUM; i++ ){
		if( thread[i]) cnt++;
	}

	return cnt;
}

static DWORD WINAPI _threadfunc( LPVOID vv )
{
    DWORD exitCode;
	_TThread *tt = (_TThread *)vv;

	SYSAPI_DebugPrintf(_AT("thread %s\n"), tt->szTaskName);

	tt->callback( (atHANDLE)tt, tt->param );

	tt->state = SYS_TASKSTATE_TERMINATED;

	SYSAPI_DebugPrintf(_AT("thread %s -done\n"), tt->szTaskName);
	
//	tt->cur_signal |= SYS_EVENT_TERMINATE;
//	SetEvent( tt->event );

	// _beginthreadex로 생성한 코드 안전 종료
    GetExitCodeThread( tt->handle, &exitCode );
    //_endthreadex(exitCode);	
	ExitThread(exitCode);
		
	return 0;
}


static atBOOL  	    OEM_SYSAPI_InitTask( atLONG lSysInst, atLONG lInitData1, atLONG lInitData2 )
{
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
	
	return atTRUE;
}

static atVOID		OEM_SYSAPI_TerminateTask( OEM_SYS_TASK hOemTask );

static atVOID  	    OEM_SYSAPI_FinalTask(void)
{
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
}


static OEM_SYS_TASK         OEM_SYSAPI_CreateTask( atLPTSTR szTaskName, void (*callback)(atHANDLE,void*), void *param, atBOOL bStart, atLONG lStackSize )
{
	int idx;
	DWORD ThreadId;
	_TThread *tt;

	idx = find_free_handle();
	if( idx < 0 ) return atNULL; // 32개를 다 채웠다.
		
//	tt = __NEW( _TThread );	
	tt = (_TThread *)SYSAPI_AllocMem( sizeof(_TThread) );
	SYSAPI_memset(tt, 0, sizeof(_TThread));
	SYSAPI_tcsncpy( tt->szTaskName, szTaskName, 30 );
//	tt->event = CreateEvent( NULL, FALSE, FALSE, NULL );
	tt->param = param;
	tt->callback = callback;

	// WinPC의 경우, CreateThread보다 _beginthreadex가 좀 더 안전함. WinCE는 지원 안 함.
	tt->handle = CreateThread( NULL, lStackSize, _threadfunc, (void *)tt, (bStart)?0:CREATE_SUSPENDED, &ThreadId );
//	tt->handle = _beginthreadex( NULL, lStackSize, _threadfunc, (void *)tt, (bStart)?0:CREATE_SUSPENDED, &ThreadId );
	tt->id = ThreadId;
	tt->state = (bStart)? SYS_TASKSTATE_RUNNING : SYS_TASKSTATE_SUSPEND;
	//tt->event = CreateEvent( NULL, FALSE, FALSE, NULL );
	//tt->id = ThreadId;
	//tt->param = param;
	//tt->callback = callback;

	thread[idx] = tt;

	return (OEM_SYS_TASK)tt;
}

static atVOID          OEM_SYSAPI_DestroyTask( OEM_SYS_TASK hOemTask )
{
	int idx;
	_TThread *tt = (_TThread *)hOemTask;

	SYSAPI_DebugPrintf(_AT("OEM_SYSAPI_DestroyTask %s\n"), tt->szTaskName);

	idx = find_match_thread( tt->handle );
	if( idx < 0 )
	{
		//SYSAPI_DebugPrintf(_AT("ERROR Destroy Task 0x%x\n"), tt->handle);
		return; // 무언가 문제가 있다면...
	}
	
	if( tt->state < SYS_TASKSTATE_TERMINATING ){
		OEM_SYSAPI_TerminateTask( hOemTask );
		OEM_SYSAPI_WaitForTerminate( hOemTask, SYS_WAIT_INFINITE );
	}

//	CloseHandle( tt->event );
	//_endthreadex( tt->handle );
	CloseHandle( tt->handle );

//	__DELETE( tt );
	SYSAPI_FreeMem( tt );

	if( idx >= 0 ) thread[idx] = NULL;

	SYSAPI_DebugPrintf(_AT("OEM_SYSAPI_DestroyTask -done\n"), tt->szTaskName);
}


static atBOOL	OEM_SYSAPI_GetTaskInfo( OEM_SYS_TASK hOemTask, SYS_TASK_INFO *lpTaskInfo )
{
	_TThread *tt = (_TThread *)hOemTask;
	if( tt == atNULL ) return atFALSE;

	SYSAPI_tcsncpy( lpTaskInfo->szTaskName, tt->szTaskName, 30 );
	lpTaskInfo->fnTaskFunc = tt->callback;
	lpTaskInfo->nState = OEM_SYSAPI_GetTaskState(hOemTask);
	lpTaskInfo->lpParam = tt->param;

	return atTRUE;
}

static atINT         OEM_SYSAPI_GetTaskState( OEM_SYS_TASK hOemTask )
{
	_TThread *tt = (_TThread *)hOemTask;
	return tt->state;
}

static atVOID		OEM_SYSAPI_TerminateTask( OEM_SYS_TASK hOemTask )
{
	_TThread *tt = (_TThread *)hOemTask;
	if( tt->state >= SYS_TASKSTATE_TERMINATING ) return;
	
	tt->state = SYS_TASKSTATE_TERMINATING;

//	tt->cur_signal |= SYS_EVENT_TERMINATE;

//	SetEvent( tt->event );
}


static atVOID		OEM_SYSAPI_RequestTerminate( OEM_SYS_TASK hOemTask )
{
	_TThread *tt = (_TThread *)hOemTask;
	if( tt->state >= SYS_TASKSTATE_TERMINATING ) return;
	
	tt->state = SYS_TASKSTATE_TERMINATING;

//	tt->cur_signal |= SYS_EVENT_TERMINATE;

//	SetEvent( tt->event );
}


static atBOOL    OEM_SYSAPI_WaitForTerminate( OEM_SYS_TASK hOemTask, atLONG lMilliSec )
{
	_TThread *tt = (_TThread *)hOemTask;

	if( tt->state == SYS_TASKSTATE_TERMINATED ) return atTRUE;
	if( tt->id == GetCurrentThreadId() ) return atFALSE;
	
//	tt->wait_signal = SYS_EVENT_TERMINATE;
	
	while( tt->state != SYS_TASKSTATE_TERMINATED ){
		WaitForSingleObject( tt->handle, (lMilliSec==SYS_WAIT_INFINITE)?1000:lMilliSec );
		if( lMilliSec != SYS_WAIT_INFINITE ) break;
	}
	
	return (tt->state == SYS_TASKSTATE_TERMINATED);
}

static atVOID     	    OEM_SYSAPI_SuspendTask( OEM_SYS_TASK hOemTask )
{
	_TThread *tt = (_TThread *)hOemTask;

	if( tt->state != SYS_TASKSTATE_RUNNING ) return;
	tt->state = SYS_TASKSTATE_SUSPEND;
	
	SuspendThread( tt->handle );	
}

static atVOID          OEM_SYSAPI_ResumeTask( OEM_SYS_TASK hOemTask )
{
	_TThread *tt = (_TThread *)hOemTask;

	if( tt->state != SYS_TASKSTATE_SUSPEND && tt->state != SYS_TASKSTATE_TERMINATED ) return;
	tt->state = SYS_TASKSTATE_RUNNING;
	
	ResumeThread( tt->handle );	
}


static OEM_SYS_TASK		OEM_SYSAPI_GetCurrentTask( NOARGS )
{
	int idx;
	DWORD task_id;
	
	task_id = GetCurrentThreadId();

	idx = find_match_thread_id( task_id );

	return (idx >= 0)? (OEM_SYS_TASK)thread[idx] : atNULL;
}

static OEM_SYS_TASKSIGNAL    OEM_SYSAPI_WaitEvents( OEM_SYS_EVENT hOemEvents, atBOOL bIsAll, atLONG lMilliSec )
{
	_TEvent *evt = (_TEvent *)hOemEvents;
	if( evt == atNULL ) return (OEM_SYS_TASKSIGNAL)0x00;

	while( !(evt->cur_signal & SYS_EVENT_TERMINATE) ){
		if( bIsAll ){
			if( (evt->cur_signal & evt->wait_signal) == evt->wait_signal ) break;
		} else {
			if( evt->cur_signal & evt->wait_signal ) break;
		}

		WaitForSingleObject( evt->event, (lMilliSec==SYS_WAIT_INFINITE)?1000:lMilliSec );
		if( lMilliSec != SYS_WAIT_INFINITE ) break;
	}

	return evt->cur_signal;
}

static atVOID          OEM_SYSAPI_Sleep( atDWORD dwMilliSec )
{
//	int old_state;
	_TThread *tt = (_TThread *)OEM_SYSAPI_GetCurrentTask();
	if(!tt) return;

	if( tt->state != SYS_TASKSTATE_RUNNING ) return;
	
//	old_state = tt->state;
//	tt->state = SYS_TASKSTATE_SUSPEND;
	Sleep( dwMilliSec );
//	tt->state = old_state;
}

static OEM_SYS_EVENT 	OEM_SYSAPI_CreateEventGroup(atLPTSTR szGroupName)
{
	_TEvent *evt;
	
	evt = (_TEvent *)SYSAPI_AllocMem( sizeof(_TEvent) );
	SYSAPI_memset(evt, 0, sizeof(_TEvent));

	evt->event = CreateEvent( NULL, FALSE, FALSE, NULL );
	evt->wait_signal = 0x00;
	evt->cur_signal = 0x00;

	return (OEM_SYS_EVENT)evt;
}

static atVOID		OEM_SYSAPI_DestroyEventGroup( OEM_SYS_EVENT hOemEvents )
{
	_TEvent *evt = (_TEvent *)hOemEvents;
	if( evt == atNULL ) return;

	if( evt->event ) CloseHandle( evt->event );

	SYSAPI_FreeMem( evt );
}

static OEM_SYS_TASKSIGNAL	OEM_SYSAPI_SetWaitSignal( OEM_SYS_EVENT hOemEvents, OEM_SYS_TASKSIGNAL dwSignal )
{
	_TEvent *evt = (_TEvent *)hOemEvents;
	if( evt == atNULL ) return (OEM_SYS_TASKSIGNAL)0x00;

	evt->wait_signal = dwSignal;

	return (OEM_SYS_TASKSIGNAL)evt->wait_signal;
}

static OEM_SYS_TASKSIGNAL	OEM_SYSAPI_GetWaitSignal( OEM_SYS_EVENT hOemEvents )
{
	_TEvent *evt = (_TEvent *)hOemEvents;
	if( evt == atNULL ) return (OEM_SYS_TASKSIGNAL)0x00;

	return (OEM_SYS_TASKSIGNAL)evt->wait_signal;
}

static OEM_SYS_TASKSIGNAL	OEM_SYSAPI_GetCurrentSignal( OEM_SYS_EVENT hOemEvents )
{
	_TEvent *evt = (_TEvent *)hOemEvents;
	if( evt == atNULL ) return (OEM_SYS_TASKSIGNAL)0x00;

	return (OEM_SYS_TASKSIGNAL)evt->cur_signal;
}

static OEM_SYS_TASKSIGNAL		OEM_SYSAPI_SendSignal( OEM_SYS_EVENT hOemEvents, OEM_SYS_TASKSIGNAL dwSignal )
{
	_TEvent *evt = (_TEvent *)hOemEvents;
	if( evt == atNULL ) return (OEM_SYS_TASKSIGNAL)0x00;

	evt->cur_signal |= dwSignal;

	if( evt->cur_signal & evt->wait_signal ){
		SetEvent( evt->event );
	}
	
	return evt->cur_signal;
}

static OEM_SYS_TASKSIGNAL		OEM_SYSAPI_ClearSignal( OEM_SYS_EVENT hOemEvents, OEM_SYS_TASKSIGNAL dwSignal )
{
	_TEvent *evt = (_TEvent *)hOemEvents;
	if( evt == atNULL ) return (OEM_SYS_TASKSIGNAL)0x00;

	evt->cur_signal &= ~dwSignal;

	if( !(evt->cur_signal & evt->wait_signal) ){
		ResetEvent( evt->event );
	}
	
	return evt->cur_signal;
}

static OEM_SYS_CRSECTION    OEM_SYSAPI_CreateCriticalSection(void)
{
	LPCRITICAL_SECTION cr;
	cr = (LPCRITICAL_SECTION)SYSAPI_AllocMem( sizeof(CRITICAL_SECTION) );
	
	InitializeCriticalSection( cr );

	return (OEM_SYS_CRSECTION)cr;
}

static atVOID          OEM_SYSAPI_DestroyCriticalSection( OEM_SYS_CRSECTION hCrSection )
{
	DeleteCriticalSection( (LPCRITICAL_SECTION)hCrSection );
	SYSAPI_FreeMem( (void *)hCrSection );
}

static atVOID          OEM_SYSAPI_EnterCriticalSection( OEM_SYS_CRSECTION hCrSection )
{
	EnterCriticalSection( (LPCRITICAL_SECTION)hCrSection );
}

static atVOID          OEM_SYSAPI_LeaveCriticalSection( OEM_SYS_CRSECTION hCrSection )
{
	LeaveCriticalSection( (LPCRITICAL_SECTION)hCrSection );
}



void init_sysapi_task_oem() 
{
	OEM_SYSAPI_TASK_T oem_funcs = {
		OEM_SYSAPI_InitTask,
		OEM_SYSAPI_FinalTask,

		OEM_SYSAPI_CreateTask,
		OEM_SYSAPI_DestroyTask,

		OEM_SYSAPI_GetTaskInfo,
		OEM_SYSAPI_GetTaskState,

		OEM_SYSAPI_TerminateTask,
		OEM_SYSAPI_RequestTerminate,

		OEM_SYSAPI_WaitForTerminate,

		OEM_SYSAPI_SuspendTask,
		OEM_SYSAPI_ResumeTask,

		//OEM_SYSAPI_GetCurrentTask,

		OEM_SYSAPI_CreateEventGroup,
		OEM_SYSAPI_DestroyEventGroup,
		OEM_SYSAPI_SetWaitSignal,
		OEM_SYSAPI_GetWaitSignal,
		OEM_SYSAPI_GetCurrentSignal,
		OEM_SYSAPI_SendSignal,
		OEM_SYSAPI_ClearSignal,

		OEM_SYSAPI_WaitEvents,
		OEM_SYSAPI_Sleep,

		OEM_SYSAPI_CreateCriticalSection,
		OEM_SYSAPI_DestroyCriticalSection,
		OEM_SYSAPI_EnterCriticalSection,
		OEM_SYSAPI_LeaveCriticalSection
	};

	OEM_SYSAPI_TASK_SetOemFn( &oem_funcs );
}

