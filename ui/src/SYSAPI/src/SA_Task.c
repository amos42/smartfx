#include "SYSAPI.h"
#include "OEM_SA_Task.h"


#ifdef __USES_SYSAPI_TASK_


static atBOOL g_bSysTaskIntitialized = atFALSE;

static OEM_SYSAPI_TASK_T tOemFn;


atVOID OEM_SYSAPI_TASK_SetOemFn(OEM_SYSAPI_TASK_T *fnOem)
{
	tOemFn = *fnOem;
}


atVOID OEM_SYSAPI_TASK_GetOemFn(OEM_SYSAPI_TASK_T *fnOem)
{
	*fnOem = tOemFn;
}


atBOOL  	    SYSAPI_InitTask( atLONG lSysInst, atLONG lInitData1, atLONG lInitData2 )
{
	if( g_bSysTaskIntitialized ) return atTRUE;
	
	if( tOemFn.fnInitTask != atNULL ){
		g_bSysTaskIntitialized = tOemFn.fnInitTask( lSysInst, lInitData1, lInitData2 );
	}
	
	return g_bSysTaskIntitialized;
}

atVOID  	    SYSAPI_FinalTask( NOARGS )
{
	if( !g_bSysTaskIntitialized ) return;
	
	if( tOemFn.fnFinalTask != atNULL ){
		tOemFn.fnFinalTask();
	}
	
	g_bSysTaskIntitialized = atFALSE;
}


SYS_TASK         SYSAPI_CreateTask( atLPTSTR szTaskName, SYS_TASK_FUNC *callback, void *param, atBOOL bStart, atLONG lStackSize )
{
	if( tOemFn.fnCreateTask != atNULL ){
		return (SYS_TASK)tOemFn.fnCreateTask( szTaskName, callback, param, bStart, lStackSize );
	}

	return (SYS_TASK)atNULL;
}

atVOID          SYSAPI_DestroyTask( SYS_TASK hTask )
{
	if( tOemFn.fnDestroyTask != atNULL ){
		tOemFn.fnDestroyTask( (OEM_SYS_TASK)hTask );
	}
}


atBOOL      	SYSAPI_GetTaskInfo( SYS_TASK hTask, SYS_TASK_INFO *lpTaskInfo )
{
	if( tOemFn.fnGetTaskInfo != atNULL ){
		return tOemFn.fnGetTaskInfo( (OEM_SYS_TASK)hTask, lpTaskInfo );
	}

	return atFALSE;
}

SYS_TASKSTATE  SYSAPI_GetTaskState( SYS_TASK hTask )
{
	if( tOemFn.fnGetTaskState != atNULL ){
		return (SYS_TASKSTATE)tOemFn.fnGetTaskState( (OEM_SYS_TASK)hTask );
	}

	return (SYS_TASKSTATE)atNULL;
}

atBOOL		SYSAPI_IsTaskActivate( SYS_TASK hTask )
{
	int state = SYS_TASKSTATE_NONE;

	if( tOemFn.fnGetTaskState != atNULL ){
		state = tOemFn.fnGetTaskState( (OEM_SYS_TASK)hTask );
	}

	return (state != SYS_TASKSTATE_TERMINATING) && (state != SYS_TASKSTATE_TERMINATED);
}

atVOID		SYSAPI_TerminateTask( SYS_TASK hTask )
{
	if( tOemFn.fnTerminateTask != atNULL ){
		tOemFn.fnTerminateTask( (OEM_SYS_TASK)hTask );
	}
}


atVOID		SYSAPI_RequestTerminate( SYS_TASK hTask )
{
	if( tOemFn.fnRequestTerminate != atNULL ){
		tOemFn.fnRequestTerminate( (OEM_SYS_TASK)hTask );
	}
}


atBOOL    	SYSAPI_WaitForTerminate( SYS_TASK hTask, atLONG lMilliSec )
{
	if( tOemFn.fnWaitForTerminate != atNULL ){
		return tOemFn.fnWaitForTerminate( (OEM_SYS_TASK)hTask, lMilliSec );
	}

	return atFALSE;
}

atVOID     	    SYSAPI_SuspendTask( SYS_TASK hTask )
{
	if( tOemFn.fnSuspendTask != atNULL ){
		tOemFn.fnSuspendTask( (OEM_SYS_TASK)hTask );
	}
}

atVOID          SYSAPI_ResumeTask( SYS_TASK hTask )
{
	if( tOemFn.fnResumeTask != atNULL ){
		tOemFn.fnResumeTask( (OEM_SYS_TASK)hTask );
	}
}


/*  ¡íc¢¯e¡¾YAo : ThreadX¢¯¢®¨ù¡© ¡¾¢¬Co¨úE¥ìC¨úi AOA¨ö.
SYS_TASK	SYSAPI_GetCurrentTask(atVOID)
{
	if( tOemFn.fnGetCurrentTask != atNULL ){
		return (SYS_TASK)tOemFn.fnGetCurrentTask();
	}

	return (SYS_TASK)atNULL;
}
*/

SYS_EVENT 	SYSAPI_CreateEventGroup( atLPTSTR szGroupName)
{
	if( tOemFn.fnCreateEventGroup != atNULL ){
		return (SYS_EVENT)tOemFn.fnCreateEventGroup(szGroupName);
	}

	return (SYS_EVENT)atNULL;
}

atVOID		SYSAPI_DestroyEventGroup( SYS_EVENT hEvents )
{
	if( tOemFn.fnDestroyEventGroup != atNULL ){
		tOemFn.fnDestroyEventGroup( (OEM_SYS_EVENT)hEvents );
	}
}

SYS_SIGNAL	SYSAPI_SetWaitSignal( SYS_EVENT hEvents, SYS_SIGNAL dwSignal )
{
	if( tOemFn.fnSetWaitSignal != atNULL ){
		return (SYS_SIGNAL)tOemFn.fnSetWaitSignal( (OEM_SYS_EVENT)hEvents, (OEM_SYS_TASKSIGNAL)dwSignal );
	}

	return (SYS_SIGNAL)atNULL;
}

SYS_SIGNAL	SYSAPI_GetWaitSignal( SYS_EVENT hEvents )
{
	if( tOemFn.fnGetWaitSignal != atNULL ){
		return (SYS_SIGNAL)tOemFn.fnGetWaitSignal( (OEM_SYS_EVENT)hEvents );
	}

	return (SYS_SIGNAL)atNULL;
}

SYS_SIGNAL		SYSAPI_GetCurrentSignal( SYS_EVENT hEvents )
{
	if( tOemFn.fnGetCurrentSignal != atNULL ){
		return (SYS_SIGNAL)tOemFn.fnGetCurrentSignal( (OEM_SYS_EVENT)hEvents );
	}

	return (SYS_SIGNAL)atNULL;
}

SYS_SIGNAL    SYSAPI_WaitEvents( SYS_EVENT hEvents, atBOOL bIsAll, atLONG lMilliSec )
{
	if( tOemFn.fnWaitEvents != atNULL ){
		return (SYS_SIGNAL)tOemFn.fnWaitEvents( (OEM_SYS_EVENT)hEvents, bIsAll, lMilliSec );
	}

	return (SYS_SIGNAL)atNULL;
}

atVOID          SYSAPI_Sleep( atDWORD dwMilliSec )
{
	if( tOemFn.fnSleep != atNULL ){
		tOemFn.fnSleep( dwMilliSec );
	}
}

SYS_SIGNAL		SYSAPI_SendSignal( SYS_EVENT hEvents, SYS_SIGNAL dwSignal )
{
	if( tOemFn.fnSendSignal != atNULL ){
		return (SYS_SIGNAL)tOemFn.fnSendSignal( (OEM_SYS_EVENT)hEvents, (OEM_SYS_TASKSIGNAL)dwSignal );
	}

	return (SYS_SIGNAL)atNULL;
}

SYS_SIGNAL		SYSAPI_ClearSignal( SYS_EVENT hEvents, SYS_SIGNAL dwSignal )
{
	if( tOemFn.fnClearSignal != atNULL ){
		return (SYS_SIGNAL)tOemFn.fnClearSignal( (OEM_SYS_EVENT)hEvents, (OEM_SYS_TASKSIGNAL)dwSignal );
	}

	return (SYS_SIGNAL)atNULL;
}

SYS_CRSECTION    SYSAPI_CreateCriticalSection( NOARGS )
{
	if( tOemFn.fnCreateCriticalSection != atNULL ){
		return (SYS_CRSECTION)tOemFn.fnCreateCriticalSection();
	}

	return (SYS_CRSECTION)atNULL;
}

atVOID          SYSAPI_DestroyCriticalSection( SYS_CRSECTION hCrSection )
{
	if( tOemFn.fnDestroyCriticalSection != atNULL ){
		tOemFn.fnDestroyCriticalSection( (OEM_SYS_CRSECTION)hCrSection );
	}
}

atVOID          SYSAPI_EnterCriticalSection( SYS_CRSECTION hCrSection )
{
	if( tOemFn.fnEnterCriticalSection != atNULL ){
		tOemFn.fnEnterCriticalSection( (OEM_SYS_CRSECTION)hCrSection );
	}
}

atVOID          SYSAPI_LeaveCriticalSection( SYS_CRSECTION hCrSection )
{
	if( tOemFn.fnLeaveCriticalSection != atNULL ){
		tOemFn.fnLeaveCriticalSection( (OEM_SYS_CRSECTION)hCrSection );
	}
}


#endif
