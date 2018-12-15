/**
  @file UI_Event.c
  @brief Event Processor�� ���� API
  @remark Event Processor�� ���� API ����
  @author Amos
  @date 2010-04-19 ���� 11:41:00
  @see UI_Event.h
*/
#include "AT_Types.h"
#include "A_Queue.h"
#include "Msg.h"
//#include "A_VD.h"
#include "UI_API/UI_API.h"


/** @brief �̺�Ʈ ���μ��� ��ü. (���ο�) */
typedef struct _tagatEVENTPROCESSOR {
    atQUEUE *g_queue;
    
    int process_flag;
    atHPROCESSOR hProcess;    
    
    atLPUIEVENTPROC lpEvProc;
    atLPUIEVENTPROC lpPostProc;

	long tag;
} atEVENTPROCESSOR;



/** 
 @brief �̺�Ʈ ���μ��� �ʱ�ȭ
 
 �̺�Ʈ ���μ��� ����

 @return ������ atTRUE, ���н� atFALSE
*/
atBOOL atUIAPI_InitEventProcessor(void)
{
    return atTRUE;
}


/** 
 @brief �̺�Ʈ ���μ��� ����
 
 �̺�Ʈ ���μ��� ����

 @return ����
*/
atVOID atUIAPI_FinalEventProcessor(void)
{
}    


/** 
 @brief �̺�Ʈ ���μ��� �ܰ� ����
 
 �̺�Ʈ ���μ����� �����Ѵ�.

 @param param [in] : �̺�Ʈ �ڵ鷯
 @return �̺�Ʈ ó�� ���
*/
atBOOL atUIAPI_EventTick( atHEVENTPROC hEvProc )
{
    if( hEvProc == atNULL ) return atFALSE;
	
//	while( !atUIAPI_IsEmptyEvent(hEvProc) ){
		atUIAPI_ProcessEvents( hEvProc );
//	}
	
	return atUIAPI_IsEmptyEvent(hEvProc)? atFALSE : atTRUE;
}


static atBOOL cUIProc( atHPROCESSOR hProcessor, void *param )
{
	return atUIAPI_EventTick( (atHEVENTPROC)param );
}

/** 
 @brief �̺�Ʈ ���μ��� ����
 
 �̺�Ʈ ���μ����� �����Ѵ�.

 @param szProcName [in] : ���μ��� �̸�
 @param nProcType [in] : ���μ��� Ÿ��
 @param eventproc [in] : ���μ��� �ڵ鷯
 @param postproc [in] : ���μ��� Post �ڵ鷯
 @param queue_size [in] : �̺�Ʈ ť�� ����
 @param item_size [in] : �̺�Ʈ �������� ũ��
 @return �̺�Ʈ ���μ��� �ڵ�
*/
atHEVENTPROC  atUIAPI_CreateEventProcessor( atLPTSTR szProcName, atINT nProcType, atLPUIEVENTPROC eventproc, atLPUIEVENTPROC postproc, int queue_size, int item_size )
{
	atQUEUEFUNCS funcs;

    atEVENTPROCESSOR *lpEvProc = (atEVENTPROCESSOR *)atUIAPI_AllocMem( sizeof(atEVENTPROCESSOR) );
    if( lpEvProc == atNULL ) return atNULL;
    
    lpEvProc->lpEvProc = eventproc;
    lpEvProc->lpPostProc = postproc;
    
	funcs.lpData = atNULL;
	funcs.fnAllocMem = atUIAPI_AllocMem;
	funcs.fnFreeMem = (atLPCALLBACKFREE)atUIAPI_FreeMem;
    lpEvProc->g_queue = atQUEUE_Create( &funcs, queue_size, item_size );

    lpEvProc->hProcess = atUIAPI_CreateProcessor( szProcName, nProcType, cUIProc, (atHEVENTPROC)lpEvProc );

    lpEvProc->process_flag = atFALSE;
	lpEvProc->tag = 0;

	return (atHEVENTPROC)lpEvProc;
}


/** 
 @brief �̺�Ʈ ���μ��� �Ҹ�
 
 �̺�Ʈ ���μ����� �Ҹ��Ų��.

 @param hEvProc [in] : �̺�Ʈ ���μ����� �ڵ�
 @return ����
*/
void atUIAPI_DestroyEventProcessor( atHEVENTPROC hEvProc )
{
	atEVENTPROCESSOR *lpEvProc = (atEVENTPROCESSOR *)hEvProc;
	
	if( lpEvProc == atNULL ) return;

    if( lpEvProc->hProcess ){
        atUIAPI_DestroyProcessor( lpEvProc->hProcess );
	    lpEvProc->hProcess = atNULL;
	}

    atQUEUE_Destroy( lpEvProc->g_queue );
	lpEvProc->g_queue = atNULL;
    
    atUIAPI_FreeMem( lpEvProc );
}


/** 
 @brief �̺�Ʈ ���μ����� Tag ����
 
 �̺�Ʈ ���μ����� Tag�� �����Ѵ�.\n
 ���������� �������� ������, ����� ���ǵ� �ڷ�μ� Ȱ��ȴ�.

 @param hEvProc [in] : �̺�Ʈ ���μ����� �ڵ�
 @param lTag [in] : Tag ��
 @return ���� Tag ��
*/
atLONG atUIAPI_SetEventProcTag( atHEVENTPROC hEvProc, atLONG lTag )
{
	atEVENTPROCESSOR *lpEvProc = (atEVENTPROCESSOR *)hEvProc;
	long old_tag;

	old_tag = lpEvProc->tag;
	lpEvProc->tag = lTag;

	return old_tag;
}


/** 
 @brief �̺�Ʈ ���μ����� Tag�� ��´�.
 
 �̺�Ʈ ���μ����� Tag�� ��´�.

 @param hEvProc [in] : �̺�Ʈ ���μ����� �ڵ�
 @return Tag ��
*/
atLONG atUIAPI_GetEventProcTag( atHEVENTPROC hEvProc )
{
	atEVENTPROCESSOR *lpEvProc = (atEVENTPROCESSOR *)hEvProc;

	return lpEvProc->tag;
}


/** 
 @brief �̺�Ʈ�� ȣ���Ѵ�.
 
 �̺�Ʈ�� ȣ���Ѵ�.

 @param hEvProc [in] : �̺�Ʈ ���μ����� �ڵ�
 @param event [in] : �̺�Ʈ ������
 @return �̺�Ʈ ���� �����
*/
int atUIAPI_CallEventProc( atHEVENTPROC hEvProc, void *event )
{
	atEVENTPROCESSOR *lpEvProc = (atEVENTPROCESSOR *)hEvProc;

    return lpEvProc->lpEvProc( hEvProc, event );
}


/** 
 @brief �̺�Ʈ ���μ����� �����Ų��.
 
 �̺�Ʈ ���μ����� �����Ų��.

 @param hEvProc [in] : �̺�Ʈ ���μ����� �ڵ�
 @return �̺�Ʈ ���� �����
*/
int atUIAPI_ProcessEvents( atHEVENTPROC hEvProc )
{
	atVOID *pev;//, ev;
	atEVENTPROCESSOR *lpEvProc = (atEVENTPROCESSOR *)hEvProc;

	if( lpEvProc == atNULL ) return -1;
	if( lpEvProc->process_flag ) return 1;
	lpEvProc->process_flag = atTRUE;

	pev = (void *)atQUEUE_Dequeue( lpEvProc->g_queue, atNULL/*&ev*/ );
	
	while( pev != atNULL ){
		atUIAPI_CallEventProc( hEvProc, pev );

	    pev = atQUEUE_Dequeue( lpEvProc->g_queue, atNULL/*&ev*/ );
	}

	lpEvProc->process_flag = atFALSE;

	if( lpEvProc->lpPostProc ) lpEvProc->lpPostProc( hEvProc, atNULL );
	
	return 0;
}


/** 
 @brief �̺�Ʈ ���μ��� ť�� ����°� �Ǵ��Ѵ�. 
 
 �̺�Ʈ ���μ��� ť�� ����°� �Ǵ��Ѵ�.

 @param hEvProc [in] : �̺�Ʈ ���μ����� �ڵ�
 @return ť�� ��� ������ atTRUE, ������� ������ atFALSE
*/
atBOOL atUIAPI_IsEmptyEvent( atHEVENTPROC hEvProc )
{
	atEVENTPROCESSOR *lpEvProc = (atEVENTPROCESSOR *)hEvProc;

	if( lpEvProc == atNULL ) return atTRUE;
    return atQUEUE_GetCount( lpEvProc->g_queue ) == 0;
}


/** 
 @brief �̺�Ʈ ť�� �̺�Ʈ�� �߰��Ѵ�.
 
 �̺�Ʈ ť�� �̺�Ʈ�� �߰��Ѵ�.

 @param hEvProc [in] : �̺�Ʈ ���μ����� �ڵ�
 @param event [in] : �̺�Ʈ ������
 @return ������ atTRUE, ���н� atFALSE
*/
atBOOL atUIAPI_RegistEvent( atHEVENTPROC hEvProc, void *event )
{
	atBOOL r;
	atEVENTPROCESSOR *lpEvProc = (atEVENTPROCESSOR *)hEvProc;

	if( lpEvProc == atNULL ) return -1;
	if( event == atNULL ) return -1;

	r = atQUEUE_Enqueue( lpEvProc->g_queue, event, atFALSE );

	if( !atUIAPI_IsActiveProcessor(lpEvProc->hProcess) )
		atUIAPI_WakeProcessor( lpEvProc->hProcess );
    
    return r;
}

