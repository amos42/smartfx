#include "SYSAPI.h"
#include "OEM_SA_Task.h"
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define MAX_OEM_THREAD_NUM 32

typedef struct __TThread {
	atTCHAR szTaskName[32];
	pthread_t pthread;
	pthread_mutex_t mutex;
	pthread_cond_t cond;
	int flag;
	int state;
	void (*callback)(SYS_TASK, void*);
	void *param;
} _TThread;

typedef struct __TEvent {
	sem_t semaphore;
	atDWORD wait_signal;
	atDWORD cur_signal;
} _TEvent;

typedef struct __CRS {
	pthread_mutex_t m_csec;
	pthread_t m_pthread;
	int m_cnt;
} _CRS;

static _TThread *thread[MAX_OEM_THREAD_NUM];

static int find_free_handle(void)
{
	int i;

	for( i = 0; i < MAX_OEM_THREAD_NUM; i++ ){
		if( thread[i] == NULL ) return i;
	}

	return -1;
}

static int find_match_thread( pthread_t pthread )
{
	int i;

	for( i = 0; i < MAX_OEM_THREAD_NUM; i++ ){
		if( thread[i] && thread[i]->pthread == pthread ) return i;
	}

	return -1;
}

/*
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
*/

static void *_threadfunc( void *vv )
{
	_TThread *tt = (_TThread *)vv;

	pthread_mutex_lock(&tt->mutex);
	while(tt->flag <= 0)
	{
		pthread_cond_wait(&tt->cond, &tt->mutex);
	}
	pthread_mutex_unlock(&tt->mutex);

	tt->callback( (atHANDLE)tt, tt->param );
	tt->state = SYS_TASKSTATE_TERMINATED;

	return atNULL;
}


atBOOL  	    OEM_SYSAPI_InitTask( atLONG lSysInst, atLONG lInitData1, atLONG lInitData2 )
{
	int i;
	_TThread *tt;

	for( i = 0; i < MAX_OEM_THREAD_NUM; i++ ) {
		thread[i] = NULL;
	}

	tt = (_TThread *)SYSAPI_AllocMem( sizeof(_TThread) );
	SYSAPI_memset(tt, 0, sizeof(_TThread));
	SYSAPI_tcsncpy( tt->szTaskName, _AT("MainTask"), 30 );
	tt->param = NULL;
	tt->callback = NULL;
	pthread_mutex_init(&tt->mutex, NULL);
	pthread_cond_init(&tt->cond, NULL);
	tt->flag = 0;
	tt->state = SYS_TASKSTATE_RUNNING;
	tt->pthread = pthread_self();

	thread[0] = tt;
	return atTRUE;
}

atVOID  	    OEM_SYSAPI_FinalTask(void)
{
	int i;

	for( i = 1; i < MAX_OEM_THREAD_NUM; i++ ){
		if( thread[i] != NULL ){
			OEM_SYSAPI_TerminateTask( (OEM_SYS_TASK)thread[i] );
			thread[i] = NULL;
		}
	}

	if( thread[0] )
	{
		pthread_cond_destroy(&thread[0]->cond);
		pthread_mutex_destroy(&thread[0]->mutex);
		SYSAPI_FreeMem( thread[0] );
	}
	thread[0] = NULL;
}


OEM_SYS_TASK         OEM_SYSAPI_CreateTask( atLPTSTR szTaskName, void (*callback)(atHANDLE,void*), void *param, atBOOL bStart, atLONG lStackSize )
{
	int idx;
	_TThread *tt;

	idx = find_free_handle();
	if( idx < 0 ) return atNULL; // 32???? ?? a???.

	tt = (_TThread *)SYSAPI_AllocMem( sizeof(_TThread) );
	SYSAPI_memset(tt, 0, sizeof(_TThread));
	SYSAPI_tcsncpy( tt->szTaskName, szTaskName, 30 );
	tt->param = param;
	tt->callback = callback;
	pthread_mutex_init(&tt->mutex, NULL);
	pthread_cond_init(&tt->cond, NULL);
	tt->flag = 0;

	pthread_create( &tt->pthread, NULL, _threadfunc, (void *)tt );
	pthread_mutex_lock(&tt->mutex);
	if( bStart )
	{
		tt->flag++;
		pthread_cond_signal(&tt->cond);
		tt->state = SYS_TASKSTATE_RUNNING;
	}
	else
	{
		tt->state = SYS_TASKSTATE_SUSPEND;
	}
	pthread_mutex_unlock(&tt->mutex);

	thread[idx] = tt;
	return (OEM_SYS_TASK)tt;
}

atVOID          OEM_SYSAPI_DestroyTask( OEM_SYS_TASK hOemTask )
{
	int idx;
	_TThread *tt = (_TThread *)hOemTask;

	idx = find_match_thread( tt->pthread );
	if( idx < 0 ) return; // ???? ??f?? ????...

	if( tt->state < SYS_TASKSTATE_TERMINATING ) {
		OEM_SYSAPI_TerminateTask( hOemTask );
		OEM_SYSAPI_WaitForTerminate( hOemTask, SYS_WAIT_INFINITE );
	}

	SYSAPI_FreeMem( tt );
	if( idx >= 0 ) thread[idx] = NULL;
}


atBOOL	OEM_SYSAPI_GetTaskInfo( OEM_SYS_TASK hOemTask, SYS_TASK_INFO *lpTaskInfo )
{
	_TThread *tt = (_TThread *)hOemTask;
	if( tt == atNULL ) return atFALSE;

	SYSAPI_tcsncpy( lpTaskInfo->szTaskName, tt->szTaskName, 30 );
	lpTaskInfo->fnTaskFunc = tt->callback;
	lpTaskInfo->nState = OEM_SYSAPI_GetTaskState(hOemTask);
	lpTaskInfo->lpParam = tt->param;

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

	pthread_cond_destroy(&tt->cond);
	pthread_mutex_destroy(&tt->mutex);
}

atBOOL    OEM_SYSAPI_WaitForTerminate( OEM_SYS_TASK hOemTask, atLONG lMilliSec )
{
	int status;
	_TThread *tt = (_TThread *)hOemTask;

	if( tt->state == SYS_TASKSTATE_TERMINATED ) return atTRUE;
	if( tt->pthread == pthread_self() ) return atFALSE;

//	tt->wait_signal = SYS_EVENT_TERMINATE;

	pthread_join( tt->pthread, (void **)&status );
 	return (tt->state == SYS_TASKSTATE_TERMINATED);
}

atVOID     	    OEM_SYSAPI_SuspendTask( OEM_SYS_TASK hOemTask )
{
	_TThread *tt = (_TThread *)hOemTask;
	if( tt->state != SYS_TASKSTATE_RUNNING ) return;
	tt->state = SYS_TASKSTATE_SUSPEND;

	// Suspend Thread
	pthread_mutex_lock(&tt->mutex);
	tt->flag--;
	pthread_mutex_unlock(&tt->mutex);
}

atVOID          OEM_SYSAPI_ResumeTask( OEM_SYS_TASK hOemTask )
{
	_TThread *tt = (_TThread *)hOemTask;
	if( tt->state != SYS_TASKSTATE_SUSPEND ) return;
	tt->state = SYS_TASKSTATE_RUNNING;

	// Resume Thread
	pthread_mutex_lock(&tt->mutex);
	tt->flag++;
	pthread_cond_signal(&tt->cond);
	pthread_mutex_unlock(&tt->mutex);
}


OEM_SYS_TASK		OEM_SYSAPI_GetCurrentTask( NOARGS )
{
	pthread_t pthread = pthread_self();
	int idx = find_match_thread(pthread);
	return (idx >= 0) ? (OEM_SYS_TASK)thread[idx] : atNULL;
}

OEM_SYS_TASKSIGNAL    OEM_SYSAPI_WaitEvents( OEM_SYS_EVENT hOemEvents, atBOOL bIsAll, atLONG lMilliSec )
{
	_TEvent *evt = (_TEvent *)hOemEvents;
	if( evt == atNULL ) return (OEM_SYS_TASKSIGNAL)0x00;

	while( !(evt->cur_signal & SYS_EVENT_TERMINATE) ){
		struct timespec ts;
		ts.tv_sec = (lMilliSec == SYS_WAIT_INFINITE ? 1000 : lMilliSec) / 1000;
		ts.tv_nsec = (lMilliSec == SYS_WAIT_INFINITE ? 1000 : lMilliSec) * 1000;
		sem_timedwait(&evt->semaphore, &ts);

		if( lMilliSec != SYS_WAIT_INFINITE ) break;

		if( bIsAll ){
			if( (evt->cur_signal & evt->wait_signal) == evt->wait_signal ) break;
		} else {
			if( evt->cur_signal & evt->wait_signal ) break;
		}
	}

	return evt->cur_signal;
}

atVOID          OEM_SYSAPI_Sleep( atDWORD dwMilliSec )
{
#if 1
	usleep(dwMilliSec * 1000);
#else
	int old_state;
	_TThread *tt = (_TThread *)OEM_SYSAPI_GetCurrentTask();
	if(!tt) return;
	if( tt->state != SYS_TASKSTATE_RUNNING ) return;

	old_state = tt->state;
	tt->state = SYS_TASKSTATE_SUSPEND;
	usleep( dwMilliSec * 1000 );
	tt->state = old_state;
#endif
}

OEM_SYS_EVENT 	OEM_SYSAPI_CreateEventGroup( NOARGS )
{
	_TEvent *evt;

	evt = (_TEvent *)SYSAPI_AllocMem( sizeof(_TEvent) );
	SYSAPI_memset(evt, 0, sizeof(_TEvent));

	sem_init(&evt->semaphore, 0, 0);
	evt->wait_signal = 0x00;
	evt->cur_signal = 0x00;

	return (OEM_SYS_EVENT)evt;
}

atVOID		OEM_SYSAPI_DestroyEventGroup( OEM_SYS_EVENT hOemEvents )
{
	_TEvent *evt = (_TEvent *)hOemEvents;
	if( evt == atNULL ) return;

	sem_destroy(&evt->semaphore);
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
		sem_post(&evt->semaphore);
	}

	return evt->cur_signal;
}

OEM_SYS_TASKSIGNAL		OEM_SYSAPI_ClearSignal( OEM_SYS_EVENT hOemEvents, OEM_SYS_TASKSIGNAL dwSignal )
{
	_TEvent *evt = (_TEvent *)hOemEvents;
	if( evt == atNULL ) return (OEM_SYS_TASKSIGNAL)0x00;

	evt->cur_signal &= ~dwSignal;

	if( !(evt->cur_signal & evt->wait_signal) ){
		//sem_post(&evt->semaphore);
	}

	return evt->cur_signal;
}

OEM_SYS_CRSECTION    OEM_SYSAPI_CreateCriticalSection(void)
{
	_CRS *cr = (_CRS *)SYSAPI_AllocMem( sizeof(_CRS) );
	if( cr == atNULL ) return atNULL;

	pthread_mutex_init(&cr->m_csec, NULL);
	cr->m_pthread = pthread_self();
	cr->m_cnt = 0;

	return (OEM_SYS_CRSECTION)cr;
}

atVOID          OEM_SYSAPI_DestroyCriticalSection( OEM_SYS_CRSECTION hCrSection )
{
	_CRS *cr = (_CRS *)hCrSection;
	if( cr == atNULL ) return;

	pthread_mutex_destroy(&cr->m_csec);
	SYSAPI_FreeMem(cr);
}

atVOID          OEM_SYSAPI_EnterCriticalSection( OEM_SYS_CRSECTION hCrSection )
{
	int state = 0;
	_CRS *cr = (_CRS *)hCrSection;
//	if( cr == atNULL ) return;

	if (cr->m_pthread != pthread_self()) {
		state = pthread_mutex_lock(&cr->m_csec);
		cr->m_pthread = pthread_self();
		cr->m_cnt = 1;
	} else {
	   cr->m_cnt++;
	}
}

atVOID          OEM_SYSAPI_LeaveCriticalSection( OEM_SYS_CRSECTION hCrSection )
{
	int state = 0;
	_CRS *cr = (_CRS *)hCrSection;
//	if( cr == atNULL ) return;

	if(cr->m_pthread == pthread_self()){
		if (cr->m_cnt == 1)
		{
			cr->m_cnt = 0;
			cr->m_pthread = 0;
			state = pthread_mutex_unlock(&cr->m_csec);
		} else {
			cr->m_cnt--;
		}
	} else {
		if( cr->m_pthread == 0 )
			return;

		cr->m_cnt = 0;
		cr->m_pthread = 0;
		state = pthread_mutex_unlock(&cr->m_csec);
	}
}
