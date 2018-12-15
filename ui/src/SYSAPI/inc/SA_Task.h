#ifndef __SA_TASK_H_
#define __SA_TASK_H_

#include "SA_Types.h"


#ifdef __cplusplus
extern "C" {
#endif


typedef atHANDLE    SYS_TASK;
typedef atHANDLE    SYS_CRSECTION;
typedef atHANDLE    SYS_EVENT;
typedef atDWORD     SYS_SIGNAL;

typedef void (SYS_TASK_FUNC)(SYS_TASK, void*);


typedef enum {
	SYS_TASKSTATE_NONE,
	SYS_TASKSTATE_READY,
	SYS_TASKSTATE_RUNNING,
	SYS_TASKSTATE_SUSPEND,
	SYS_TASKSTATE_TERMINATING,
	SYS_TASKSTATE_TERMINATED
} SYS_TASKSTATE;

#define SYS_WAIT_INFINITE (-1)


#define SYS_EVENT_TERMINATE		0x80000000


typedef struct _tagSYS_TASK_INFO {
	atTCHAR szTaskName[32];
	atINT 	nState;
	SYS_TASK_FUNC *fnTaskFunc;
	void *lpParam;
} SYS_TASK_INFO;


atBOOL  	    SYSAPI_InitTask( atLONG lSysInst, atLONG lInitData1, atLONG lInitData2 );
atVOID  	    SYSAPI_FinalTask(void);

SYS_TASK         SYSAPI_CreateTask( atLPTSTR szTaskName, SYS_TASK_FUNC *callback, void *param, atBOOL bStart, atLONG lStackSize );
atVOID          SYSAPI_DestroyTask( SYS_TASK hTask );

atBOOL      	SYSAPI_GetTaskInfo( SYS_TASK hTask, SYS_TASK_INFO *lpTaskInfo );
SYS_TASKSTATE  SYSAPI_GetTaskState( SYS_TASK hTask );

atBOOL		SYSAPI_IsTaskActivate( SYS_TASK hTask );
atVOID		SYSAPI_RequestTerminate( SYS_TASK hTask );
atVOID		SYSAPI_TerminateTask( SYS_TASK hTask );
atBOOL		SYSAPI_WaitForTerminate( SYS_TASK hTask, atLONG lMilliSec );

atVOID     	    SYSAPI_SuspendTask( SYS_TASK hTask );
atVOID          SYSAPI_ResumeTask( SYS_TASK hTask );


// SYSAPI_GetCurrentTask()  ; ¡íc¢¯e¡¾YAo : ThreadX¢¯¢®¨ù¡© ¡¾¢¬Co¨úE¥ìC¨úi AOA¨ö.
//SYS_TASK	SYSAPI_GetCurrentTask( NOARGS );  

SYS_EVENT 	SYSAPI_CreateEventGroup( atLPTSTR szGroupName);
atVOID		SYSAPI_DestroyEventGroup( SYS_EVENT hEvents );
SYS_SIGNAL	SYSAPI_SetWaitSignal( SYS_EVENT hEvents, SYS_SIGNAL dwSignal );
SYS_SIGNAL	SYSAPI_GetWaitSignal( SYS_EVENT hEvents );
SYS_SIGNAL	SYSAPI_GetCurrentSignal( SYS_EVENT hEvents );

SYS_SIGNAL	SYSAPI_SendSignal( SYS_EVENT hEvents, SYS_SIGNAL dwSignal );
SYS_SIGNAL	SYSAPI_ClearSignal( SYS_EVENT hEvents, SYS_SIGNAL dwSignal );

SYS_SIGNAL	SYSAPI_WaitEvents( SYS_EVENT hEvents, atBOOL bIsAll, atLONG lMilliSec );
atVOID		SYSAPI_Sleep( atDWORD dwMilliSec );

SYS_CRSECTION    SYSAPI_CreateCriticalSection( NOARGS );
atVOID          SYSAPI_DestroyCriticalSection( SYS_CRSECTION hCrSection );
atVOID          SYSAPI_EnterCriticalSection( SYS_CRSECTION hCrSection );
atVOID          SYSAPI_LeaveCriticalSection( SYS_CRSECTION hCrSection );


#ifdef __cplusplus
}
#endif


#endif

