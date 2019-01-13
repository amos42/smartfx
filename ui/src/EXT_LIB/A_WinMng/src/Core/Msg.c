/**
  @file Msg.c
  @brief Message Processor�� ���� API
  @remark Message Processor�� ���� API ����
  @author Amos
  @date 2010-04-19 ���� 11:41:00
  @see Msg.h
*/
#include "UI_Types.h"
#include "Msg.h"
//#include "Window.h"
#include "UI_Event.h"


#define MSG_QUEUE_SIZE 1000


/** @brief Message �̺�Ʈ �������� ���� (���ο�) */
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
 @brief �޽��� ���μ��� ����
 
 �޽��� ���μ��� ����

 @return �޽��� ���μ��� �ڵ�
*/
atHMSGPROC 	atMSG_CreateMsgProc( void )
{
	g_bExitFlag = atFALSE;
	g_nExitCode = 0;

	return (atHMSGPROC)atUIAPI_CreateEventProcessor( _AT("MsgProc"), 0, _eventproc, _postproc, MSG_QUEUE_SIZE, sizeof(myEVENT) );
}


/** 
 @brief �޽��� ���μ��� �Ҹ�
 
 �޽��� ���μ��� �Ҹ�

 @param hMsgProc [in] : �޽��� ���μ��� �ڵ�
 @return ����
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
 @brief ���� �ڵ带 ������.
 
 ���� �ڵ带 ������.\n
 ���� �����ϰ� ���� ������ ����.

 @param hMsgProc [in] : �޽��� ���μ��� �ڵ�
 @param nExitCode [in] : �����ڵ�
 @return ����
*/
atVOID atMSG_SendExitCode( atHMSGPROC hMsgProc, atINT nExitCode )
{
	g_nExitCode = nExitCode;
	g_bExitFlag = atTRUE;
}


/** 
 @brief �޽��� ���μ����� �����Ų��.
 
 �޽��� ���μ����� �����Ų��.

 @param hMsgProc [in] : �޽��� ���μ����� �ڵ�
 @return �޽��� ���� �����
*/
atINT atMSG_ProcessEvents( atHMSGPROC hMsgProc )
{
	atINT r = atUIAPI_ProcessEvents( hMsgProc );
	
	return r;
}


/** 
 @brief Window�� �޽����� ������.
 
 Window�� �޽����� ������.

 @param hMsgProc [in] : �޽��� ���μ��� �ڵ�
 @param hWnd [in] : Window�� �ڵ�
 @param nMsg [in] : ������ �޽���
 @param lParam1, lParam2 [in] : �޽����� �Ķ����
 @return �޽��� �ڵ鷯�� �����
 @remark �޽��� ó���� �ڵ鷯�� �������� Call�� ���� �ﰢ������ �Ͼ��.
*/
atINT atMSG_SendMessage( atHMSGPROC hMsgProc, atHWINOBJ hWnd, atINT nMsg, atLONG lParam1, atLONG lParam2 )
{
	int r = -1;

	if( g_bExitFlag ) return -1;
	
	if( hWnd == atNULL ) return -1;

	r = atWINOBJ_ProcessObjProc( hWnd, nMsg, lParam1, lParam2 );
	atMSG_ProcessEvents(hMsgProc);  // ���� ó���� �Ѵ�.

	return r;
}


/** 
 @brief Window�� �޽����� ������.
 
 Window�� �޽����� ������.

 @param hWnd [in] : Window�� �ڵ�
 @param nMsg [in] : ������ �޽���
 @param lParam1, lParam2 [in] : �޽����� �Ķ����
 @return �޽��� ó�� ��û ���.
 @remark �޽��� ó���� �޽��� Queue�� ���� �Ͼ�� ������, �񵿱������� �Ͼ��.
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
//	atMSG_ProcessEvents(hMsgProc);  // ���� ó���� �Ѵ�.

	return r;
}


/** 
 @brief �޽��� ��Ŀ�� �߰��Ѵ�.
 
 �޽��� ��Ŀ�� �߰��Ѵ�.

 @param hMsgProc [in] : �޽��� ���μ��� �ڵ�
 @param fnCallback [in] : ��ŷ �ڵ鷯
 @return �߰��� ��Ŀ�� ID
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
 @brief �޽��� ��Ŀ�� �Ҹ��Ų��.
 
 �޽��� ��Ŀ�� �Ҹ��Ų��.

 @param hMsgProc [in] : �޽��� ���μ��� �ڵ�
 @param nHookerID [in] : ��Ŀ�� ID
 @return ����
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

