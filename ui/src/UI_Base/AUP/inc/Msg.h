/**
  @file Msg.h
  @brief Window Message �� ���� API
  @remark Window Message �� ���� API ����
  @author Amos
  @date 2010-04-19 ���� 11:41:00
  @see Msg.c
*/
#ifndef __MSG_H_
#define __MSG_H_

#include "UI_Types.h"
#include "UI_Event.h"
#include "WinObj.h"


#ifdef __cplusplus
extern "C" {
#endif
    

typedef atHEVENTPROC atHMSGPROC;


atHMSGPROC 	atMSG_CreateMsgProc( void );
void 		atMSG_DestroyMsgProc( atHMSGPROC hMsgProc );
atINT		atMSG_SendMessage( atHMSGPROC hMsgProc, atHWINOBJ hWnd, atINT nMsg, atLONG lParam1, atLONG lParam2 );
atINT		atMSG_PostMessage( atHMSGPROC hMsgProc, atHWINOBJ hWnd, atINT nMsg, atLONG lParam1, atLONG lParam2 );
atVOID		atMSG_SendExitCode( atHMSGPROC hMsgProc, atINT nExitCode );
atINT		atMSG_ProcessEvents( atHMSGPROC hMsgProc );
atINT 		atMSG_AddMessageHooker( atHMSGPROC hMsgProc, atLPWINOBJPROC fnCallback );
void 		atMSG_RemoveMessageHooker( atHMSGPROC hMsgProc, atINT nHookerID );


#ifdef __cplusplus
}
#endif


#endif
