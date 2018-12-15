//
// MsgBox.h
//
///////////////////////////////////////////////////////////////////////

#ifndef __MSGBOXWND_H_
#define __MSGBOXWND_H_


#ifndef __A_UIENG_H_
#include "A_UIENG.h"
#endif


#ifdef __cplusplus
extern "C" {
#endif


#define BASEPKG_CUSTWIN_MSGBOX       _AT("BasePkg.CustWin.MsgBoxWnd")


typedef enum {
	atMBT_NONE,        
	atMBT_ALERT		= 100,  // ! (취소)
	atMBT_CONFIRM,			// v (확인)
	atMBT_QUESTION,			// ? (예, 아니오)
	atMBT_ERROR,				// x (확인)
	atMBT_QUESTION_CANCEL,		// ? (예, 아니오)
	atMBT_QUESTION_CHECK,		// ? (예, 아니오, Check)
	atMBT_CUSTOM_1BTN,			// 
	atMBT_CUSTOM_2BTN,			// 
	atMBT_CUSTOM_3BTN,			// 
//	atMBT_WAIT        
	atMBT_MAX
} atMSGBOXTYPEID;

typedef enum {
	atMB_OK		= 10000,
	atMB_CANCEL,
	atMB_YES,
	atMB_NO,
	atMB_YES_CHECK,
	atMB_CLOSE, // 강제적으로 메세지 박스 닫을경우. 
	atMB_CUSTOM_1,
	atMB_CUSTOM_2,
	atMB_CUSTOM_3
} atMSGBOXBUTTONID;


atBOOL      RegistMsgBoxWndClass(void);
void		UnregistMsgBoxWndClass(void);
atHWINDOW	CreateMsgBox( atHWINDOW hParentWnd, int id, atLPTSTR szWindowText, atLPTSTR szMsg, int nMsgBoxType );
atHWINDOW	CreateMsgBox2( atHWINDOW hParentWnd, int id, atLPTSTR szWindowText, atLPTSTR szMsg, int nMsgBoxType, long lWaitTime );


#ifdef __cplusplus
}
#endif


#endif // _MSGBOXWND_H_INCLUDED
