/**
  @file OEM_UI_Proc.h
  @brief Processor에 관한 포팅 OEM API 선언
  @remark Processor에 관한 포팅 OEM API 선언
  @author Amos
  @date 2010-04-19 오전 11:41:00
  @see UI_Proc.h
*/
#ifndef __OEM_UI_PROC_H_
#define __OEM_UI_PROC_H_

#include "UI_Types.h"


#ifdef __cplusplus
extern "C" {
#endif
	

typedef void *OEM_UI_PROCESSOR;
typedef void *OEM_UI_CRSECTION;


typedef atBOOL  			FN_OEM_UI_InitProcessor( atLONG lSysInst, atLONG lInitData1, atLONG lInitData2 );
typedef atVOID  			FN_OEM_UI_FinalProcessor(void);

typedef OEM_UI_PROCESSOR	FN_OEM_UI_CreateProcessor( atLPTSTR szProcName, atINT nProcType, atBOOL (*callback)(OEM_UI_PROCESSOR, void*), void *param );
typedef atVOID				FN_OEM_UI_DestroyProcessor( OEM_UI_PROCESSOR OEM_Processor );

typedef atVOID				FN_OEM_UI_SuspendProcessor( OEM_UI_PROCESSOR OEM_Processor );
typedef atVOID				FN_OEM_UI_ResumeProcessor( OEM_UI_PROCESSOR OEM_Processor );
typedef atVOID				FN_OEM_UI_WakeProcessor( OEM_UI_PROCESSOR OEM_Processor );
typedef atBOOL				FN_OEM_UI_IsActiveProcessor( OEM_UI_PROCESSOR OEM_Processor );

typedef OEM_UI_CRSECTION    FN_OEM_UI_CreateCriticalSection(void);
typedef atVOID				FN_OEM_UI_DestroyCriticalSection( OEM_UI_CRSECTION OEM_CrSection );
typedef atVOID				FN_OEM_UI_EnterCriticalSection( OEM_UI_CRSECTION OEM_CrSection );
typedef atVOID				FN_OEM_UI_LeaveCriticalSection( OEM_UI_CRSECTION OEM_CrSection );

typedef atLONG				FN_OEM_UI_MessageLoop( atLONG lParam1, atLONG lParam2 );


typedef struct tagOEM_UI_PROC {
	FN_OEM_UI_InitProcessor				*fnInitProcessor;
	FN_OEM_UI_FinalProcessor			*fnFinalProcessor;

	FN_OEM_UI_CreateProcessor			*fnCreateProcessor;
	FN_OEM_UI_DestroyProcessor			*fnDestroyProcessor;

	FN_OEM_UI_SuspendProcessor			*fnSuspendProcessor;
	FN_OEM_UI_ResumeProcessor			*fnResumeProcessor;
	FN_OEM_UI_WakeProcessor				*fnWakeProcessor;
	FN_OEM_UI_IsActiveProcessor			*fnIsActiveProcessor;

	FN_OEM_UI_CreateCriticalSection		*fnCreateCriticalSection;
	FN_OEM_UI_DestroyCriticalSection	*fnDestroyCriticalSection;
	FN_OEM_UI_EnterCriticalSection		*fnEnterCriticalSection;
	FN_OEM_UI_LeaveCriticalSection		*fnLeaveCriticalSection;

	FN_OEM_UI_MessageLoop				*fnMessageLoop;
} OEM_UI_PROC_T;


atVOID	OEM_UI_Proc_SetOemFn( OEM_UI_PROC_T *fnOem );
atVOID	OEM_UI_Proc_GetOemFn( OEM_UI_PROC_T *fnOem );


#ifdef __cplusplus
}
#endif


#endif

