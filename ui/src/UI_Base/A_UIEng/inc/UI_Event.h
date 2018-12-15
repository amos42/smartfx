/**
  @file UI_Event.h
  @brief UI�������� ����ϴ� �̺�Ʈ �Ŵ���
  @remark UI�������� ����ϴ� �̺�Ʈ �Ŵ���
  @author Amos
  @date 2010-04-19 ���� 11:41:00
  @see UI_Event.c
*/
#ifndef __UI_EVENT_H_
#define __UI_EVENT_H_


#include "UI_Types.h"


#ifdef __cplusplus
extern "C" {
#endif
	

typedef atHANDLE  atHEVENTPROC;
typedef int (atUIEVENTPROC)( atHEVENTPROC, void * );
typedef atUIEVENTPROC * atLPUIEVENTPROC;


atBOOL  	    atUIAPI_InitEventProcessor(void);
atVOID  	    atUIAPI_FinalEventProcessor(void);

atHEVENTPROC	atUIAPI_CreateEventProcessor( atLPTSTR szProcName, atINT nProcType, atLPUIEVENTPROC eventproc, atLPUIEVENTPROC postproc, int queue_size, int item_size );
void            atUIAPI_DestroyEventProcessor( atHEVENTPROC hEvProc );
atBOOL			atUIAPI_EventTick( atHEVENTPROC hEvProc );
atLONG 			atUIAPI_SetEventProcTag( atHEVENTPROC hEvProc, atLONG lTag );
atLONG 			atUIAPI_GetEventProcTag( atHEVENTPROC hEvProc );
int             atUIAPI_CallEventProc( atHEVENTPROC hEvProc, void *event );
int             atUIAPI_ProcessEvents( atHEVENTPROC hEvProc );
atBOOL          atUIAPI_IsEmptyEvent( atHEVENTPROC hEvProc );
atBOOL          atUIAPI_RegistEvent( atHEVENTPROC hEvProc, void *event );


#ifdef __cplusplus
}
#endif


#endif
