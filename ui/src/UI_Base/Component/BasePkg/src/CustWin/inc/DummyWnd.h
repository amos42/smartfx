// DummyWnd.h
// Custom Window 개발시 참고용

#ifndef __DUMMYWND_H_
#define __DUMMYWND_H_


#ifndef __A_UIENG_H_
#include "A_UIENG.h"
#endif


#ifdef __cplusplus
extern "C" {
#endif


#define BASEPKG_CUSTWIN_DUMMY   _AT("BasePkg.CustWin.DummyWnd")


atBOOL      RegistDummyWndClass(void);
atVOID		UnregistDummyWndClass(void);
atHWINDOW	CreateDummyWnd( atHWINDOW hParentWnd );


#ifdef __cplusplus
}
#endif


#endif // __DUMMYWND_H_
