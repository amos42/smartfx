//
// WindowInfoWnd.h
//
///////////////////////////////////////////////////////////////////////

#ifndef __WINDOWINFOWND_H_
#define __WINDOWINFOWND_H_


#ifndef __A_UIENG_H_
#include "A_UIENG.h"
#endif


#ifdef __cplusplus
extern "C" {
#endif


#define BASEPKG_CUSTWIN_WINDOWINFO   _AT("BasePkg.CustWin.WindowInfoWnd")


atBOOL      RegistWindowInfoWndClass(void);
atVOID      UnregistWindowInfoWndClass(void);
atHWINDOW	CreateWindowInfoWnd( atHWINDOW hParentWnd );


#ifdef __cplusplus
}
#endif


#endif // __WINDOWINFOWND_H_
