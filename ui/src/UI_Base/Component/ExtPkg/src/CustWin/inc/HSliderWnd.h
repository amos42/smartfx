//
// MsgBoxList.h
// 
// 내용 : MSGBOXLIST_KIND 종류의 팝업을 그리고 호출하는 함수이다
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
