
#ifndef __EDITBOXWND_H_
#define __EDITBOXWND_H_


#ifndef __A_UIENG_H_
#include "A_UIENG.h"
#endif


#ifdef __cplusplus
extern "C" {
#endif


#define BASEPKG_CUSTWIN_EDITBOX     _AT("BasePkg.CustWin.EditBoxWnd")


atBOOL      RegistEditBoxWndClass(void);
void        UnregistEditBoxWndClass(void);

atHWINDOW	CreateEditBoxWnd(atHWINDOW hParentWnd, int id, atLPTSTR szTitle, atLPTSTR szMsg, atLPTSTR szText );


#ifdef __cplusplus
}
#endif


#endif // __EDITBOXWND_H_
