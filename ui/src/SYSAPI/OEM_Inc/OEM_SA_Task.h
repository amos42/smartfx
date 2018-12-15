#ifndef __OEM_SA_TASK_H_
#define __OEM_SA_TASK_H_

#include "SA_Types.h"
#include "SYSAPI.h"


#ifdef __cplusplus
extern "C" {
#endif


typedef atHANDLE    OEM_SYS_TASK;
typedef atHANDLE    OEM_SYS_EVENT;
typedef atHANDLE    OEM_SYS_CRSECTION;
typedef atDWORD     OEM_SYS_TASKSIGNAL;


typedef atBOOL  			FN_OEM_SYSAPI_InitTask( atLONG lSysInst, atLONG lInitData1, atLONG lInitData2 );
typedef atVOID  			FN_OEM_SYSAPI_FinalTask( NOARGS );

typedef OEM_SYS_TASK		FN_OEM_SYSAPI_CreateTask( atLPTSTR szTaskName, void (*callback)(atHANDLE,void*), void *param, atBOOL bStart, atLONG lStackSize );
typedef atVOID				FN_OEM_SYSAPI_DestroyTask( OEM_SYS_TASK hOemTask );

typedef atBOOL				FN_OEM_SYSAPI_GetTaskInfo( OEM_SYS_TASK hOemTask, SYS_TASK_INFO *lpTaskInfo );
typedef atINT				FN_OEM_SYSAPI_GetTaskState( OEM_SYS_TASK hOemTask );

typedef atVOID				FN_OEM_SYSAPI_TerminateTask( OEM_SYS_TASK hOemTask );
typedef atVOID				FN_OEM_SYSAPI_RequestTerminate( OEM_SYS_TASK hOemTask );

typedef atBOOL   			FN_OEM_SYSAPI_WaitForTerminate( OEM_SYS_TASK hOemTask, atLONG lMilliSec );

typedef atVOID     			FN_OEM_SYSAPI_SuspendTask( OEM_SYS_TASK hOemTask );
typedef atVOID				FN_OEM_SYSAPI_ResumeTask( OEM_SYS_TASK hOemTask );

//typedef OEM_SYS_TASK		FN_OEM_SYSAPI_GetCurrentTask( NOARGS );

typedef OEM_SYS_EVENT 		FN_OEM_SYSAPI_CreateEventGroup(atLPTSTR szGroupName);
typedef atVOID				FN_OEM_SYSAPI_DestroyEventGroup( OEM_SYS_EVENT hOemEvents );
typedef OEM_SYS_TASKSIGNAL	FN_OEM_SYSAPI_SetWaitSignal( OEM_SYS_EVENT hOemEvents, OEM_SYS_TASKSIGNAL dwSignal );
typedef OEM_SYS_TASKSIGNAL	FN_OEM_SYSAPI_GetWaitSignal( OEM_SYS_EVENT hOemEvents );
typedef OEM_SYS_TASKSIGNAL	FN_OEM_SYSAPI_GetCurrentSignal( OEM_SYS_EVENT hOemEvents );
typedef OEM_SYS_TASKSIGNAL	FN_OEM_SYSAPI_SendSignal( OEM_SYS_EVENT hOemEvents, OEM_SYS_TASKSIGNAL dwSignal );
typedef OEM_SYS_TASKSIGNAL	FN_OEM_SYSAPI_ClearSignal( OEM_SYS_EVENT hOemEvents, OEM_SYS_TASKSIGNAL dwSignal );

typedef OEM_SYS_TASKSIGNAL	FN_OEM_SYSAPI_WaitEvents( OEM_SYS_EVENT hOemEvent, atBOOL bIsAll, atLONG lMilliSec );
typedef atVOID				FN_OEM_SYSAPI_Sleep( atDWORD dwMilliSec );

typedef OEM_SYS_CRSECTION   FN_OEM_SYSAPI_CreateCriticalSection( NOARGS );
typedef atVOID				FN_OEM_SYSAPI_DestroyCriticalSection( OEM_SYS_CRSECTION hCrSection );
typedef atVOID				FN_OEM_SYSAPI_EnterCriticalSection( OEM_SYS_CRSECTION hCrSection );
typedef atVOID				FN_OEM_SYSAPI_LeaveCriticalSection( OEM_SYS_CRSECTION hCrSection );


typedef struct tagOEM_SYSAPI_TASK_T {
	FN_OEM_SYSAPI_InitTask					*fnInitTask;
	FN_OEM_SYSAPI_FinalTask					*fnFinalTask;

	FN_OEM_SYSAPI_CreateTask				*fnCreateTask;
	FN_OEM_SYSAPI_DestroyTask				*fnDestroyTask;

	FN_OEM_SYSAPI_GetTaskInfo				*fnGetTaskInfo;
	FN_OEM_SYSAPI_GetTaskState				*fnGetTaskState;

	FN_OEM_SYSAPI_TerminateTask				*fnTerminateTask;
	FN_OEM_SYSAPI_RequestTerminate			*fnRequestTerminate;

	FN_OEM_SYSAPI_WaitForTerminate			*fnWaitForTerminate;

	FN_OEM_SYSAPI_SuspendTask				*fnSuspendTask;
	FN_OEM_SYSAPI_ResumeTask				*fnResumeTask;

	//FN_OEM_SYSAPI_GetCurrentTask			*fnGetCurrentTask;

	FN_OEM_SYSAPI_CreateEventGroup			*fnCreateEventGroup;
	FN_OEM_SYSAPI_DestroyEventGroup			*fnDestroyEventGroup;
	FN_OEM_SYSAPI_SetWaitSignal				*fnSetWaitSignal;
	FN_OEM_SYSAPI_GetWaitSignal				*fnGetWaitSignal;
	FN_OEM_SYSAPI_GetCurrentSignal			*fnGetCurrentSignal;
	FN_OEM_SYSAPI_SendSignal				*fnSendSignal;
	FN_OEM_SYSAPI_ClearSignal				*fnClearSignal;

	FN_OEM_SYSAPI_WaitEvents				*fnWaitEvents;
	FN_OEM_SYSAPI_Sleep						*fnSleep;

	FN_OEM_SYSAPI_CreateCriticalSection		*fnCreateCriticalSection;
	FN_OEM_SYSAPI_DestroyCriticalSection	*fnDestroyCriticalSection;
	FN_OEM_SYSAPI_EnterCriticalSection		*fnEnterCriticalSection;
	FN_OEM_SYSAPI_LeaveCriticalSection		*fnLeaveCriticalSection;
} OEM_SYSAPI_TASK_T;


atVOID	OEM_SYSAPI_TASK_SetOemFn( OEM_SYSAPI_TASK_T *fnOem );
atVOID	OEM_SYSAPI_TASK_GetOemFn( OEM_SYSAPI_TASK_T *fnOem );


#ifdef __cplusplus
}
#endif


#endif

