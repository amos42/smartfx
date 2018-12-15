//
// MsgBoxList.h
// 
// ���� : MSGBOXLIST_KIND ������ �˾��� �׸��� ȣ���ϴ� �Լ��̴�
//
///////////////////////////////////////////////////////////////////////


#ifndef __LISTBOXWND_H_
#define __LISTBOXWND_H_


#ifndef __A_UIENG_H_
#include "A_UIENG.h"
#endif


#ifdef __cplusplus
extern "C" {
#endif


#define EXTPKG_CUSTWIN_LISTBOX      _AT("ExtPkg.CustWin.ListBoxWnd")


atBOOL      RegistListBoxWndClass(void);
atHWINDOW	CreateListBoxWnd_0(atHWINDOW hParentWnd, int id, atLPTSTR szTitle, void (*add_func)(atHCONTROL,void*,int), void *data, int max_cnt, int sel);
atHWINDOW	CreateListBoxWnd(atHWINDOW hParentWnd, int id, atLPTSTR szTitle, atTCHAR (*szInputMsg)[32], int max_cnt, int sel);


#ifdef __cplusplus
}
#endif


#endif // _MSGBOXLIST_H_INCLUDED
