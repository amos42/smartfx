//
// MsgBoxList.h
// 
// ���� : MSGBOXLIST_KIND ������ �˾��� �׸��� ȣ���ϴ� �Լ��̴�
//
///////////////////////////////////////////////////////////////////////

#ifndef __HSLIDERWND_H_
#define __HSLIDERWND_H_


#ifndef __A_UIENG_H_
#include "A_UIENG.h"
#endif


#ifdef __cplusplus
extern "C" {
#endif


#define EXTPKG_CUSTWIN_HSLIDER      _AT("ExtPkg.CustWin.HSliderWnd")


atBOOL      RegistHSliderWndClass(void);
atHWINDOW	CreateHSliderWnd(atHWINDOW hParentWnd, int id, atLPTSTR szTitle, void *data, int size, int pos);


#ifdef __cplusplus
}
#endif


#endif // __HSLIDERWND_H_
