#include "SYSAPI.h"
#include "OEM_SA_Task.h"


atBOOL  	    OEM_SYSAPI_InitTask( atLONG lSysInst, atLONG lInitData1, atLONG lInitData2 )
{
	return atTRUE;
}

atVOID  	    OEM_SYSAPI_FinalTask(void)
{
}


OEM_SYS_TASK         OEM_SYSAPI_CreateTask( atLPTSTR szTaskName, void (*callback)(atHANDLE,void*), void *param, atBOOL bStart, atLONG lStackSize )
{
	return (OEM_SYS_TASK)atNULL;
}

atVOID          OEM_SYSAPI_DestroyTask( OEM_SYS_TASK hOemTask )
{
}


atBOOL	OEM_SYSAPI_GetTaskInfo( OEM_SYS_TASK hOemTask, SYS_TASK_INFO *lpTaskInfo )
{
	return atFALSE;
}

atINT         OEM_SYSAPI_GetTaskState( OEM_SYS_TASK hOemTask )
{
//	return SYS_TASKSTATE_NULL;
	return 0;
}

atVOID		OEM_SYSAPI_TerminateTask( OEM_SYS_TASK hOemTask )
{
}

atBOOL    OEM_SYSAPI_WaitForTerminate( OEM_SYS_TASK hOemTask, atLONG lMilliSec )
{
	return atFALSE;
}

atVOID     	    OEM_SYSAPI_SuspendTask( OEM_SYS_TASK hOemTask )
{
}	

atVOID          OEM_SYSAPI_ResumeTask( OEM_SYS_TASK hOemTask )
{
}


OEM_SYS_TASK		OEM_SYSAPI_GetCurrentTask( NOARGS )
{
	return atNULL;
}

OEM_SYS_TASKSIGNAL    OEM_SYSAPI_WaitEvents( OEM_SYS_EVENT hOemEvents, atBOOL bIsAll, atLONG lMilliSec )
{
	return 0;
}

atVOID          OEM_SYSAPI_Sleep( atDWORD dwMilliSec )
{
}

OEM_SYS_EVENT 	OEM_SYSAPI_CreateEventGroup( NOARGS )
{
	return atNULL;
}

atVOID		OEM_SYSAPI_DestroyEventGroup( OEM_SYS_EVENT hOemEvents )
{
}

OEM_SYS_TASKSIGNAL	OEM_SYSAPI_SetWaitSignal( OEM_SYS_EVENT hOemEvents, OEM_SYS_TASKSIGNAL dwSignal )
{
	return 0;
}

OEM_SYS_TASKSIGNAL	OEM_SYSAPI_GetWaitSignal( OEM_SYS_EVENT hOemEvents )
{
	return 0;
}

OEM_SYS_TASKSIGNAL	OEM_SYSAPI_GetCurrentSignal( OEM_SYS_EVENT hOemEvents )
{
	return 0;
}

OEM_SYS_TASKSIGNAL		OEM_SYSAPI_SendSignal( OEM_SYS_EVENT hOemEvents, OEM_SYS_TASKSIGNAL dwSignal )
{
	return 0;
}

OEM_SYS_TASKSIGNAL		OEM_SYSAPI_ClearSignal( OEM_SYS_EVENT hOemEvents, OEM_SYS_TASKSIGNAL dwSignal )
{
	return 0;
}

OEM_SYS_CRSECTION    OEM_SYSAPI_CreateCriticalSection(void)
{
	return atNULL;
}

atVOID          OEM_SYSAPI_DestroyCriticalSection( OEM_SYS_CRSECTION hCrSection )
{
}

atVOID          OEM_SYSAPI_EnterCriticalSection( OEM_SYS_CRSECTION hCrSection )
{
}

atVOID          OEM_SYSAPI_LeaveCriticalSection( OEM_SYS_CRSECTION hCrSection )
{
}

