/**
  @file UI_Proc.h
  @brief Processor에 관한 API
  @remark Processor에 관한 API 모음
  @author Amos
  @date 2010-04-19 오전 11:41:00
  @see UI_Proc.c
*/
#ifndef __UI_PROC_H_
#define __UI_PROC_H_

#include "UI_Types.h"


#ifdef __cplusplus
extern "C" {
#endif
	

typedef atHANDLE atHPROCESSOR;
typedef atHANDLE atHCRSECTION;

enum {
	atUI_PROCTYPE_DEFAULT = 0x0000,
	atUI_PROCTYPE_EVENTPROC,
	atUI_PROCTYPE_DISPPROC,
	atUI_PROCTYPE_USERDEF = 0x1000
};

atBOOL  	    atUIAPI_InitProcessor( atLONG lSysInst, atLONG lInitData1, atLONG lInitData2 );
atVOID  	    atUIAPI_FinalProcessor(void);

atHPROCESSOR    atUIAPI_CreateProcessor( atLPTSTR szProcName, atINT nProcType, atBOOL (*callback)(atHPROCESSOR,void*), void *param );
atVOID          atUIAPI_DestroyProcessor( atHPROCESSOR hProcessor );

atVOID          atUIAPI_SuspendProcessor( atHPROCESSOR hProcessor );
atVOID          atUIAPI_ResumeProcessor( atHPROCESSOR hProcessor );
atVOID          atUIAPI_WakeProcessor( atHPROCESSOR hProcessor );
atBOOL          atUIAPI_IsActiveProcessor( atHPROCESSOR hProcessor );

atVOID			atUIAPI_StartDisplay(void);
atVOID			atUIAPI_StopDisplay(void);


atHCRSECTION    atUIAPI_CreateCriticalSection(void);
atVOID          atUIAPI_DestroyCriticalSection( atHCRSECTION hCrSection );
atVOID          atUIAPI_EnterCriticalSection( atHCRSECTION hCrSection );
atVOID          atUIAPI_LeaveCriticalSection( atHCRSECTION hCrSection );


#ifdef __cplusplus
}
#endif


#endif

