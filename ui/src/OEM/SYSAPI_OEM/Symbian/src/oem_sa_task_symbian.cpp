#include "SYSAPI.h"
#include "OEM_SA_Task.h"

#include <e32svr.h>
#include <e32cmn.h>


static int _TSK_Index;

typedef struct __TSK {
	atTCHAR szTaskName[32];
	RThread			m_rThread;
//	RSemaphore		m_semaphore;	
//	TRequestStatus	m_Status;
	int state;
	TInt id;
	void *param;
//	TUint32 wait_signal;
//	TUint32 cur_signal;
	void (*callback)(SYS_TASK, void*);
} _TSK;

typedef struct __TEvent {
//	TRequestStatus	m_Status;
	RSemaphore		m_semaphore;	
	TUint32 wait_signal;
	TUint32 cur_signal;
} _TEvent;

typedef struct __CRS {
	RCriticalSection m_csec;
	TInt m_cnt;
	TThreadId m_curId;	
} _CRS;

class CActiveController : public CActive
{
public:
	_TSK* m_curThread;
	CActiveController(_TSK* aThread) :
	 CActive(EPriorityStandard)
	{
		m_curThread=aThread;
		iStatus=KRequestPending;
		CActiveScheduler::Add(this);
		SetActive();
	}
	~CActiveController(){};
	
	void Start()
	{
		TRequestStatus* request_status=&iStatus;
		User::RequestComplete(request_status,KErrNone);
	}
	
private:
	void RunL()
	{
		m_curThread->callback( (SYS_TASK)m_curThread, m_curThread->param );
		CActiveScheduler::Stop();
	}
	
	void DoCancel()
	{
	}
};



#define MAX_OEM_SYS_TASK_NUM  32

static _TSK *thread[MAX_OEM_SYS_TASK_NUM];

void init_tsk_struct(_TSK *tsk)
{
	if(!tsk) return;
	
	SYSAPI_memset(tsk->szTaskName,0, sizeof(tsk->szTaskName));
	tsk->id          = 0;
	tsk->state       = 0;
	tsk->param       = NULL;
//	tsk->wait_signal = 0;
//	tsk->cur_signal  = 0;
	tsk->callback    = NULL;
}


static int find_free_handle(void)
{
	int i;
	
	for( i = 0; i < MAX_OEM_SYS_TASK_NUM; i++ ){
		if( thread[i] == NULL ) return i;
	}

	return -1;
}

static int find_match_thread( _TSK *tt )
{
	int i;
	
	for( i = 0; i < MAX_OEM_SYS_TASK_NUM; i++ ){
		if( thread[i] == tt ) return i;
	}

	return -1;
}


static int find_match_thread_id( atDWORD task_id )
{
	int i;
	
	for( i = 0; i < MAX_OEM_SYS_TASK_NUM; i++ ){
		if( (thread[i] != atNULL) && (thread[i]->id == task_id) ) return i;
	}

	return -1;
}

static TInt _procThread(TAny* pData)
{
	_TSK *tsk = (_TSK *)pData;

	CTrapCleanup* cleanup = CTrapCleanup::New();
	CActiveScheduler *active_scheduler = new CActiveScheduler();
	CActiveScheduler::Install( active_scheduler );
	    
    CActiveController* ActiveCon = new(ELeave) CActiveController(tsk);
		
	ActiveCon->Start();
	CActiveScheduler::Start();
			
	//TRequestStatus* request_status=&iStatus;
	//User::RequestComplete(request_status,KErrNone);
	
	tsk->state = SYS_TASKSTATE_TERMINATED;
	
	delete ActiveCon;
	delete active_scheduler;
	delete cleanup;

	return 0;
}


atBOOL  	    OEM_SYSAPI_InitTask( atLONG lSysInst, atLONG lInitData1, atLONG lInitData2 )
{
	int i;
	_TSK *tsk;
	RThread cur_thread;
	
	for( i = 0; i < MAX_OEM_SYS_TASK_NUM; i++ ){
		thread[i] = atNULL;
	}

	tsk = (_TSK *)SYSAPI_AllocMem( sizeof(_TSK) );
	SYSAPI_memset(tsk, 0, sizeof(_TSK));
	SYSAPI_tcsncpy( tsk->szTaskName, _AT("MainTask"), 30 );
//	tsk->m_semaphore.CreateLocal(1);
	tsk->param = NULL;
	tsk->callback = NULL;
	tsk->m_rThread = cur_thread;
	tsk->id = cur_thread.Id();
	tsk->state = SYS_TASKSTATE_RUNNING;
	thread[0] = tsk;
	
	return atTRUE;
}

atVOID  	    OEM_SYSAPI_FinalTask(void)
{
	int i;

	for( i = 1; i < MAX_OEM_SYS_TASK_NUM; i++ ){
		if( thread[i] != NULL ){
			OEM_SYSAPI_TerminateTask( (OEM_SYS_TASK)thread[i] );
			thread[i] = NULL;
		}
	}	

//	thread[0]->m_semaphore.Close();
	SYSAPI_FreeMem( thread[0] );
}


OEM_SYS_TASK         OEM_SYSAPI_CreateTask( atLPTSTR szTaskName, void (*callback)(SYS_TASK,void*), void *param, atBOOL bStart, atLONG lStackSize )
{
	int idx;
//	int dwStackSize = 40 * 1024;
//	int dwHeapMinSize = 8*1024; 
//	int dwHeapMaxSize = 32*1024*1024;
	//atUCS_CHAR *szThreadName = L"Thread"; 
	TPtrC ThreadName((TUint16*)szTaskName);
	
	idx = find_free_handle();
	if( idx < 0 ) return atNULL; // 32개를 다 채웠다.
		
	_TSK *tsk = new _TSK;
	if(!tsk) return atNULL;
	//SYSAPI_memset(tsk, 0, sizeof(_TSK));
	init_tsk_struct(tsk);

	SYSAPI_tcsncpy( tsk->szTaskName, szTaskName, 30 );
	
//	TInt result = tsk->m_rThread.Create( ThreadName, _procThread, lStackSize, dwHeapMinSize, dwHeapMaxSize, (void *)tsk );
	TInt result = tsk->m_rThread.Create( ThreadName, _procThread, (TInt)lStackSize, atNULL, (void *)tsk, EOwnerProcess );
	if(result != KErrNone)
	{
		delete tsk;
		return atNULL;
	}
	tsk->id       = tsk->m_rThread.Id();
	tsk->param    = param;
	tsk->callback = callback;
//	tsk->m_semaphore.CreateLocal(1);
	
	if( bStart ){
		tsk->m_rThread.Resume();
		tsk->state = SYS_TASKSTATE_RUNNING;
	} else {
		tsk->state = SYS_TASKSTATE_SUSPEND;
	}
	
	thread[idx] = tsk;
	
	return (OEM_SYS_TASK)tsk;
}

atVOID          OEM_SYSAPI_DestroyTask( OEM_SYS_TASK hOemTask )
{
	int idx;
	_TSK *tsk = (_TSK *)hOemTask;

	idx = find_match_thread( tsk );
	if( idx < 0 ) return; // 무언가 문제가 있다면...
		
	tsk->m_rThread.Terminate(0);

	tsk->m_rThread.Close();
//	tsk->m_semaphore.Close();

	delete tsk;

	if( idx >= 0 ) thread[idx] = NULL;
}


atBOOL	OEM_SYSAPI_GetTaskInfo( OEM_SYS_TASK hOemTask, SYS_TASK_INFO *lpTaskInfo )
{
	_TSK *tsk = (_TSK *)hOemTask;
	if( tsk == atNULL ) return atFALSE;

	SYSAPI_tcsncpy( lpTaskInfo->szTaskName, tsk->szTaskName, 30 );
	lpTaskInfo->fnTaskFunc = tsk->callback;
	lpTaskInfo->nState = OEM_SYSAPI_GetTaskState(hOemTask);
	lpTaskInfo->lpParam = tsk->param;

	return atTRUE;
}

atINT         OEM_SYSAPI_GetTaskState( OEM_SYS_TASK hOemTask )
{
	_TSK *tsk = (_TSK *)hOemTask;
	if( tsk == atNULL ) return SYS_TASKSTATE_NONE;
	
	return (atINT)tsk->state;
}

atVOID		OEM_SYSAPI_TerminateTask( OEM_SYS_TASK hOemTask )
{
	_TSK *tsk = (_TSK *)hOemTask;

	tsk->state = SYS_TASKSTATE_TERMINATING;
	
//	tsk->cur_signal |= SYS_EVENT_TERMINATE;
}

atBOOL		OEM_SYSAPI_IsTerminated( OEM_SYS_TASK hOemTask )
{
	_TSK *tsk = (_TSK *)hOemTask;
	if( tsk == atNULL ) return atTRUE;
	
	return (tsk->state == SYS_TASKSTATE_TERMINATED)? atTRUE : atFALSE;
}

atBOOL    OEM_SYSAPI_WaitForTerminate( OEM_SYS_TASK hOemTask, atLONG lMilliSec )
{
	_TSK *tsk = (_TSK *)hOemTask;
	
	if( tsk->state == SYS_TASKSTATE_TERMINATED ) return atTRUE;
	
	RThread cur_thread;
	if( tsk->id == cur_thread.Id() ) return atFALSE;
	
//	tsk->wait_signal = SYS_EVENT_TERMINATE;
	
	while( tsk->state != SYS_TASKSTATE_TERMINATED ){
//		tsk->m_semaphore.Wait( lMilliSec * 1000 );
		OEM_SYSAPI_Sleep(100);
		if( lMilliSec != SYS_WAIT_INFINITE ) break;
	}
	
	return (tsk->state == SYS_TASKSTATE_TERMINATED);
}

atVOID     	    OEM_SYSAPI_SuspendTask( OEM_SYS_TASK hOemTask )
{
	_TSK *tsk = (_TSK *)hOemTask;
	
	if( tsk->state != SYS_TASKSTATE_RUNNING ) return;
	tsk->state = SYS_TASKSTATE_SUSPEND;
	
	tsk->m_rThread.Suspend();	
}

atVOID          OEM_SYSAPI_ResumeTask( OEM_SYS_TASK hOemTask )
{
	_TSK *tsk = (_TSK *)hOemTask;
	
	if( tsk->state != SYS_TASKSTATE_SUSPEND ) return;
	tsk->state = SYS_TASKSTATE_RUNNING;
	
	tsk->m_rThread.Resume();
}


OEM_SYS_TASK		OEM_SYSAPI_GetCurrentTask( NOARGS )
{
	RThread cur_thread;
	int idx;
	
	idx = find_match_thread_id( cur_thread.Id() );

	return (idx >= 0)? (OEM_SYS_TASK)thread[idx] : atNULL;
}

OEM_SYS_TASKSIGNAL    OEM_SYSAPI_WaitEvents( OEM_SYS_EVENT hOemEvents, atBOOL bIsAll, atLONG lMilliSec )
{
	_TEvent *evt = (_TEvent *)hOemEvents;
	if( evt == atNULL ) return (OEM_SYS_TASKSIGNAL)0x00;

	if( evt->wait_signal == 0 ) return 0;
	if( bIsAll && ((evt->cur_signal & evt->wait_signal) == evt->wait_signal) ) return evt->cur_signal;
	if( !bIsAll && ((evt->cur_signal & evt->wait_signal) != 0x00) ) return evt->cur_signal;
	
	if( lMilliSec == SYS_WAIT_INFINITE ){
		evt->m_semaphore.Wait();
	} else {
		evt->m_semaphore.Wait( lMilliSec * 1000 );
	}

	return (OEM_SYS_TASKSIGNAL)evt->cur_signal;
}

atVOID          OEM_SYSAPI_Sleep( atDWORD dwMilliSec )
{
	User::After(dwMilliSec*1000);
}

OEM_SYS_EVENT 	OEM_SYSAPI_CreateEventGroup( NOARGS )
{
	_TEvent *evt;
	
	evt = (_TEvent *)SYSAPI_AllocMem( sizeof(_TEvent) );
	SYSAPI_memset(evt, 0, sizeof(_TEvent));

	evt->m_semaphore.CreateLocal(0);
	evt->wait_signal = 0x00;
	evt->cur_signal = 0x00;

	return (OEM_SYS_EVENT)evt;
}

atVOID		OEM_SYSAPI_DestroyEventGroup( OEM_SYS_EVENT hOemEvents )
{
	_TEvent *evt = (_TEvent *)hOemEvents;
	if( evt == atNULL ) return;

	evt->m_semaphore.Close();

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
		evt->m_semaphore.Signal();
	}
	
	return evt->cur_signal;
}

OEM_SYS_TASKSIGNAL		OEM_SYSAPI_ClearSignal( OEM_SYS_EVENT hOemEvents, OEM_SYS_TASKSIGNAL dwSignal )
{
	_TEvent *evt = (_TEvent *)hOemEvents;
	if( evt == atNULL ) return (OEM_SYS_TASKSIGNAL)0x00;

	evt->cur_signal &= ~dwSignal;

	if( !(evt->cur_signal & evt->wait_signal) ){
//		evt->m_semaphore.Signal();
	}
	
	return evt->cur_signal;
}


OEM_SYS_CRSECTION    OEM_SYSAPI_CreateCriticalSection(void)
{
	_CRS *cr = new _CRS;
	if( cr == atNULL ) return atNULL;
	
//	cr->m_csec.CreateLocal(EOwnerThread);
	cr->m_csec.CreateLocal();
	cr->m_curId = 0;
	cr->m_cnt = 0;
	
	return (OEM_SYS_CRSECTION)cr;
}

atVOID          OEM_SYSAPI_DestroyCriticalSection( OEM_SYS_CRSECTION hCrSection )
{
	_CRS *cr = (_CRS *)hCrSection;
	if( cr == atNULL ) return;
	
	cr->m_csec.Close();
	delete cr;
}


/*
atVOID          OEM_SYSAPI_EnterCriticalSection( OEM_SYS_CRSECTION hCrSection )
{
	_CRS *cr = (_CRS *)hCrSection;
	if( cr == atNULL ) return;

	RThread cur_thread;
	
//	if( cr->m_csec.IsBlocked() && (cr->m_curId == cur_thread.Id()) ){
	if( (cr->m_curId == cur_thread.Id()) && (cr->m_cnt > 0) ){
       cr->m_cnt++;
	} else {
       cr->m_curId = cur_thread.Id();            
       cr->m_cnt = 1;
       cr->m_csec.Wait();
	}
}

atVOID          OEM_SYSAPI_LeaveCriticalSection( OEM_SYS_CRSECTION hCrSection )
{
	_CRS *cr = (_CRS *)hCrSection;
	if( cr == atNULL ) return;
	
	RThread cur_thread;
	atBOOL is_release = atFALSE;
	
//	if( cr->m_csec.IsBlocked() && (cr->m_curId == cur_thread.Id()) ){
	if( (cr->m_curId == cur_thread.Id()) && (cr->m_cnt > 1) ){
//      if( --cr->m_cnt <= 0 ) is_release = atTRUE;
	} else {
	  is_release = atTRUE;
	}
	
//	if( is_release ){
	if( --cr->m_cnt <= 0 ){
		cr->m_cnt = 0;
		cr->m_curId = 0;
		cr->m_csec.Signal();
    }
}
*/
atVOID          OEM_SYSAPI_EnterCriticalSection( OEM_SYS_CRSECTION hCrSection )
{
	_CRS *cr = (_CRS *)hCrSection;
//	if( cr == atNULL ) return;
	
        if (cr->m_curId != RThread().Id()) {
            cr->m_csec.Wait();
    		cr->m_curId = RThread().Id();            
    		cr->m_cnt = 1;
        } else {
           cr->m_cnt++;
        }
}

atVOID          OEM_SYSAPI_LeaveCriticalSection( OEM_SYS_CRSECTION hCrSection )
{
	_CRS *cr = (_CRS *)hCrSection;
//	if( cr == atNULL ) return;
	
       if(cr->m_curId == RThread().Id()){
            if (cr->m_cnt == 1)
            {
            	cr->m_cnt = 0;
            	cr->m_curId = 0;
               cr->m_csec.Signal();
            } else {
               cr->m_cnt--;
            }
        } else {
if( cr->m_curId.Id() == 0 ){
return;
}
    	cr->m_cnt = 0;
    	cr->m_curId = 0;
       cr->m_csec.Signal();
        }
}
