//
// MsgBox.h
//
///////////////////////////////////////////////////////////////////////

#ifndef __MSGBOXEXWND_H_
#define __MSGBOXEXWND_H_


#ifndef __A_UIENG_H_
#include "A_UIENG.h"
#endif


#ifdef __cplusplus
extern "C" {
#endif


#define EXTPKG_CUSTWIN_MSGBOX       _AT("ExtPkg.CustWin.MsgBoxWnd")


typedef enum {
	atMBT_EX_NONE,        
	atMBT_EX_ALERT		= 100,  // ! (취소)
	atMBT_EX_CONFIRM,			// v (확인)
	atMBT_EX_QUESTION,			// ? (예, 아니오)
	atMBT_EX_ERROR,				// x (확인)
	atMBT_EX_QUESTION_CANCEL,		// ? (예, 아니오)
	atMBT_EX_QUESTION_CHECK,		// ? (예, 아니오, Check)
	atMBT_EX_CUSTOM_1BTN,			// 
	atMBT_EX_CUSTOM_2BTN,			// 
	atMBT_EX_CUSTOM_3BTN,			// 
//	atMBT_EX_WAIT        
	atMBT_EX_MAX
} atMSGBOXEX_TYPEID;

typedef enum {
	atMB_EX_OK		= 10000,
	atMB_EX_CANCEL,
	atMB_EX_YES,
	atMB_EX_NO,
	atMB_EX_YES_CHECK,
	atMB_EX_CUSTOM_1,
	atMB_EX_CUSTOM_2,
	atMB_EX_CUSTOM_3
} atMSGBOXEX_BUTTONID;


atBOOL      RegistMsgBoxExWndClass( atHRESSRC hResSrc );
atVOID      UnregistMsgBoxExWndClass(void);
atHWINDOW	CreateMsgBoxEx( atHWINDOW hParentWnd, int id, atLPTSTR szWindowText, atLPTSTR szMsg, int nMsgBoxType );
atHWINDOW	CreateMsgBoxEx2( atHWINDOW hParentWnd, int id, atLPTSTR szWindowText, atLPTSTR szMsg, int nMsgBoxType, long lWaitTime );


#ifdef __cplusplus
}
#endif


#endif // _MSGBOXWND_H_INCLUDED
