//
// RadioGroupWnd.h
// 
// ³»¿ë : 
//
///////////////////////////////////////////////////////////////////////

#ifndef __RADIOGROUPWND_H_
#define __RADIOGROUPWND_H_


#ifndef __A_UIENG_H_
#include "A_UIENG.h"
#endif


#ifdef __cplusplus
extern "C" {
#endif


#define EXTPKG_CUSTWIN_RADIOGROUP   _AT("ExtPkg.CustWin.RadioGroupWnd")


//typedef (RADIOGROUPWND_CALLBACK)(atHWINDOW hWnd, int idx);
//typedef RADIOGROUPWND_CALLBACK * LPRADIOGROUP_CALLBACK;

atBOOL      RegistRadioGroupWndClass(void);
atHWINDOW	CreateRadioGroupWnd_0(atHWINDOW hParentWnd, int id, atLPTSTR szTitle,
										void (*add_func)(atHCONTROL,void *,int), void *data, int max_cnt, int init_value);
atHWINDOW	CreateRadioGroupWnd(atHWINDOW hParentWnd, int id, atLPTSTR szTitle,
										atTCHAR (*szInputMsg)[32], int max_cnt, int init_value);


#ifdef __cplusplus
}
#endif


#endif // _MSGBOXLIST_H_INCLUDED
__RADIOGROUPWND_H_
