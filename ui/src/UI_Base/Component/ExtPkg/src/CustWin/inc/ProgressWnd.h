
#ifndef __PROGRESSRWND_H_
#define __PROGRESSRWND_H_


#ifndef __A_UIENG_H_
#include "A_UIENG.h"
#endif


#ifdef __cplusplus
extern "C" {
#endif


#define EXTPKG_CUSTWIN_PROGRESS     _AT("ExtPkg.CustWin.ProgressWnd")


atBOOL      RegistProgressWndClass(void);
atHWINDOW	CreateProgressWnd( atHWINDOW hParentWnd, int id, atLPTSTR szTitle, int size, int pos, atLPTSTR text, atLPTSTR button_caption );
void        ChangeProgressWnd( atHWINDOW hWnd, int pos, atLPTSTR text );


#ifdef __cplusplus
}
#endif


#endif // __PROGRESSRWND_H_
