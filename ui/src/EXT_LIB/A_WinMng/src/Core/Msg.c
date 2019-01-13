/**
  @file Msg.c
  @brief Message Processor에 관한 API
  @remark Message Processor에 관한 API 모음
  @author Amos
  @date 2010-04-19 오전 11:41:00
  @see Msg.h
*/
#include "UI_Types.h"
#include "Msg.h"
//#include "Window.h"
#include "UI_Event.h"


#define MSG_QUEUE_SIZE 1000


/** @brief Message 이벤트 아이템의 구조 (내부용) */
typedef struct tag_myEVENT {
    atHWINOBJ hwin;
	int msg;
	int parm1, parm2;
} myEVENT;


static atBOOL g_bExitFlag;
static atINT	g_nExitCode;


static int _eventproc( atHEVENTPROC hEvtProc, void *data )
{
	myEVENT *pev = (myEVENT *)data;
	atUILIST lpHookerChain = (atUILIST)atUIAPI_GetEventProcTag(hEvtProc);

	if( lpHookerChain ){
		int i, cnt, r;
		atLPWINOBJPROC fnHookerProc;

		cnt = atUIAPI_GetListCount(lpHookerChain);
		for( i = 0; i < cnt; i++ ){
			fnHookerProc = (atLPWINOBJPROC)atUIAPI_GetListAt(lpHookerChain, i);
			r = fnHookerProc( pev->hwin, pev->msg, pev->parm1, pev->parm2 );
			if( r != 0 ) return r;
		}
	}
	
	return atWINDOW_ProcessWindowProc( pev->hwin, pev->msg, pev->parm1, pev->parm2 );
}

static int _postproc( atHEVENTPROC hEvtProc, void *data )
{
	if( g_bExitFlag ){
		//atUIAPI_SystemOff(g_nExitCode);
		g_bExitFlag = atFALSE;
	}

	return 0;
}


/** 
 @brief 메시지 프로세서 생성
 
 메시지 프로세서 생성

 @return 메시지 프로세서 핸들
*/
atHMSGPROC 	atMSG_CreateMsgProc( void )
{
	g_bExitFlag = atFALSE;
	g_nExitCode = 0;

	return (atHMSGPROC)atUIAPI_CreateEventProcessor( _AT("MsgProc"), 0, _eventproc, _postproc, MSG_QUEUE_SIZE, sizeof(myEVENT) );
}


/** 
 @brief 메시지 프로세서 소멸
 
 메시지 프로세서 소멸

 @param hMsgProc [in] : 메시지 프로세서 핸들
 @return 없음
*/
void atMSG_DestroyMsgProc( atHMSGPROC hMsgProc )
{
	atUILIST lpHookerChain = (atUILIST)atUIAPI_GetEventProcTag((atHEVENTPROC)hMsgProc);
	
	if( lpHookerChain != atNULL ){
		atUIAPI_DestroyList( lpHookerChain, atFALSE );
	}	

	atUIAPI_DestroyEventProcessor( (atHEVENTPROC)hMsgProc );
}


/** 
 @brief 종료 코드를 보낸다.
 
 종료 코드를 보낸다.\n
 이후 안전하게 종료 절차로 들어간다.

 @param hMsgProc [in] : 메시지 프로세서 핸들
 @param nExitCode [in] : 종료코드
 @return 없음
*/
atVOID atMSG_SendExitCode( atHMSGPROC hMsgProc, atINT nExitCode )
{
	g_nExitCode = nExitCode;
	g_bExitFlag = atTRUE;
}


/** 
 @brief 메시지 프로세서를 진행시킨다.
 
 메시지 프로세서를 진행시킨다.

 @param hMsgProc [in] : 메시지 프로세서의 핸들
 @return 메시지 동작 결과값
*/
atINT atMSG_ProcessEvents( atHMSGPROC hMsgProc )
{
	atINT r = atUIAPI_ProcessEvents( hMsgProc );
	
	return r;
}


/** 
 @brief Window에 메시지를 보낸다.
 
 Window에 메시지를 보낸다.

 @param hMsgProc [in] : 메시지 프로세서 핸들
 @param hWnd [in] : Window의 핸들
 @param nMsg [in] : 전송할 메시지
 @param lParam1, lParam2 [in] : 메시지의 파라미터
 @return 메시지 핸들러의 결과값
 @remark 메시지 처리가 핸들러의 직접적인 Call을 통해 즉각적으로 일어난다.
*/
atINT atMSG_SendMessage( atHMSGPROC hMsgProc, atHWINOBJ hWnd, atINT nMsg, atLONG lParam1, atLONG lParam2 )
{
	int r = -1;

	if( g_bExitFlag ) return -1;
	
	if( hWnd == atNULL ) return -1;

	r = atWINOBJ_ProcessObjProc( hWnd, nMsg, lParam1, lParam2 );
	atMSG_ProcessEvents(hMsgProc);  // 이후 처리를 한다.

	return r;
}


/** 
 @brief Window에 메시지를 보낸다.
 
 Window에 메시지를 보낸다.

 @param hWnd [in] : Window의 핸들
 @param nMsg [in] : 전송할 메시지
 @param lParam1, lParam2 [in] : 메시지의 파라미터
 @return 메시지 처리 요청 결과.
 @remark 메시지 처리가 메시지 Queue를 통해 일어나기 때문에, 비동기적으로 일어난다.
*/
atINT atMSG_PostMessage( atHMSGPROC hMsgProc, atHWINOBJ hWnd, atINT nMsg, atLONG lParam1, atLONG lParam2 )
{
	myEVENT ev;
	int r;

	if( g_bExitFlag ) return -1;
	
	if( hWnd == atNULL ) return -1;

	ev.hwin = hWnd;
	ev.msg = nMsg;
	ev.parm1 = lParam1;
	ev.parm2 = lParam2;
	
	r = atUIAPI_RegistEvent( hMsgProc, &ev );
//	atMSG_ProcessEvents(hMsgProc);  // 이후 처리를 한다.

	return r;
}


/** 
 @brief 메시지 후커를 추가한다.
 
 메시지 후커를 추가한다.

 @param hMsgProc [in] : 메시지 프로세서 핸들
 @param fnCallback [in] : 후킹 핸들러
 @return 추가된 후커의 ID
*/
atINT atMSG_AddMessageHooker( atHMSGPROC hMsgProc, atLPWINOBJPROC fnCallback )
{
	atUILIST lpHookerChain = (atUILIST)atUIAPI_GetEventProcTag((atHEVENTPROC)hMsgProc);
	
	if( lpHookerChain == atNULL ){
		lpHookerChain = atUIAPI_CreateList();
		atUIAPI_SetEventProcTag( (atHEVENTPROC)hMsgProc, (atLONG)lpHookerChain );
	}
	
	return atUIAPI_AddListTail( lpHookerChain, (void *)fnCallback );
}


/** 
 @brief 메시지 후커를 소멸시킨다.
 
 메시지 후커를 소멸시킨다.

 @param hMsgProc [in] : 메시지 프로세서 핸들
 @param nHookerID [in] : 후커의 ID
 @return 없음
*/
atVOID atMSG_RemoveMessageHooker( atHMSGPROC hMsgProc, atINT nHookerID )
{
	atUILIST lpHookerChain = (atUILIST)atUIAPI_GetEventProcTag((atHEVENTPROC)hMsgProc);
	
	if( lpHookerChain == atNULL ) return;
	
	atUIAPI_RemoveListAt( lpHookerChain, nHookerID, atFALSE );
	if( atUIAPI_GetListCount(lpHookerChain) <= 0 ){
		atUIAPI_DestroyList( lpHookerChain, atFALSE );
		lpHookerChain = atNULL;
		atUIAPI_SetEventProcTag( (atHEVENTPROC)hMsgProc, (atLONG)lpHookerChain );
	}
}

