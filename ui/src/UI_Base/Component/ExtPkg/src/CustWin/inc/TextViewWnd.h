//
// TextViewWnd.h
//
///////////////////////////////////////////////////////////////////////

#ifndef __TEXTVIEWWND_H_
#define __TEXTVIEWWND_H_

#ifndef __A_UIENG_H_
#include "A_UIENG.h"
#endif


#ifdef __cplusplus
extern "C" {
#endif


#define EXTPKG_CUSTWIN_TEXTVIEW     _AT("ExtPkg.CustWin.TextViewWnd")


atBOOL      RegistTextViewWndClass(void);
atHWINDOW	CreateTextViewWnd(atHWINDOW hParentWnd, int id, atLPTSTR szTitle, atLPTSTR szText );


#ifdef __cplusplus
}
#endif


#endif // __TEXTVIEWWND_H_
