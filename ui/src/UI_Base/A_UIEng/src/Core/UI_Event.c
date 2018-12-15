/**
  @file UI_Event.c
  @brief Event Processor에 관한 API
  @remark Event Processor에 관한 API 모음
  @author Amos
  @date 2010-04-19 오전 11:41:00
  @see UI_Event.h
*/
#include "AT_Types.h"
#include "A_Queue.h"
#include "Msg.h"
//#include "A_VD.h"
#include "UI_API/UI_API.h"


/** @brief 이벤트 프로세서 개체. (내부용) */
typedef struct _tagatEVENTPROCESSOR {
    atQUEUE *g_queue;
    
    int process_flag;
    atHPROCESSOR hProcess;    
    
    atLPUIEVENTPROC lpEvProc;
    atLPUIEVENTPROC lpPostProc;

	long tag;
} atEVENTPROCESSOR;



/** 
 @brief 이벤트 프로세서 초기화
 
 이벤트 프로세서 시작

 @return 성공시 atTRUE, 실패시 atFALSE
*/
atBOOL atUIAPI_InitEventProcessor(void)
{
    return atTRUE;
}


/** 
 @brief 이벤트 프로세서 종료
 
 이벤트 프로세서 시작

 @return 없음
*/
atVOID atUIAPI_FinalEventProcessor(void)
{
}    


/** 
 @brief 이벤트 프로세서 단계 실행
 
 이벤트 프로세서를 생성한다.

 @param param [in] : 이벤트 핸들러
 @return 이벤트 처리 결과
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
 @brief 이벤트 프로세서 생성
 
 이벤트 프로세서를 생성한다.

 @param szProcName [in] : 프로세서 이름
 @param nProcType [in] : 프로세서 타입
 @param eventproc [in] : 프로세서 핸들러
 @param postproc [in] : 프로세서 Post 핸들러
 @param queue_size [in] : 이벤트 큐의 갯수
 @param item_size [in] : 이벤트 아이템의 크기
 @return 이벤트 프로세서 핸들
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
 @brief 이벤트 프로세서 소멸
 
 이벤트 프로세서를 소멸시킨다.

 @param hEvProc [in] : 이벤트 프로세서의 핸들
 @return 없음
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
 @brief 이벤트 프로세서의 Tag 설정
 
 이벤트 프로세서의 Tag를 설정한다.\n
 직접적으로 사용되지는 않으나, 사용자 정의된 자료로서 활용된다.

 @param hEvProc [in] : 이벤트 프로세서의 핸들
 @param lTag [in] : Tag 값
 @return 이전 Tag 값
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
 @brief 이벤트 프로세서의 Tag를 얻는다.
 
 이벤트 프로세서의 Tag를 얻는다.

 @param hEvProc [in] : 이벤트 프로세서의 핸들
 @return Tag 값
*/
atLONG atUIAPI_GetEventProcTag( atHEVENTPROC hEvProc )
{
	atEVENTPROCESSOR *lpEvProc = (atEVENTPROCESSOR *)hEvProc;

	return lpEvProc->tag;
}


/** 
 @brief 이벤트를 호출한다.
 
 이벤트를 호출한다.

 @param hEvProc [in] : 이벤트 프로세서의 핸들
 @param event [in] : 이벤트 포인터
 @return 이벤트 동작 결과값
*/
int atUIAPI_CallEventProc( atHEVENTPROC hEvProc, void *event )
{
	atEVENTPROCESSOR *lpEvProc = (atEVENTPROCESSOR *)hEvProc;

    return lpEvProc->lpEvProc( hEvProc, event );
}


/** 
 @brief 이벤트 프로세서를 진행시킨다.
 
 이벤트 프로세서를 진행시킨다.

 @param hEvProc [in] : 이벤트 프로세서의 핸들
 @return 이벤트 동작 결과값
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
 @brief 이벤트 프로세서 큐가 비었는가 판단한다. 
 
 이벤트 프로세서 큐가 비었는가 판단한다.

 @param hEvProc [in] : 이벤트 프로세서의 핸들
 @return 큐가 비어 있으면 atTRUE, 비어있지 않으면 atFALSE
*/
atBOOL atUIAPI_IsEmptyEvent( atHEVENTPROC hEvProc )
{
	atEVENTPROCESSOR *lpEvProc = (atEVENTPROCESSOR *)hEvProc;

	if( lpEvProc == atNULL ) return atTRUE;
    return atQUEUE_GetCount( lpEvProc->g_queue ) == 0;
}


/** 
 @brief 이벤트 큐에 이벤트를 추가한다.
 
 이벤트 큐에 이벤트를 추가한다.

 @param hEvProc [in] : 이벤트 프로세서의 핸들
 @param event [in] : 이벤트 포인터
 @return 성공시 atTRUE, 실패시 atFALSE
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

