//
// MemoryInfoWnd.h
//
///////////////////////////////////////////////////////////////////////

#ifndef __MEMORYINFOWND_H_
#define __MEMORYINFOWND_H_


#ifndef __A_UIENG_H_
#include "A_UIENG.h"
#endif


#ifdef __cplusplus
extern "C" {
#endif


#define BASEPKG_CUSTWIN_MEMORYINFO   _AT("BasePkg.CustWin.MemoryInfoWnd")


atBOOL      RegistMemoryInfoWndClass(void);
atVOID		UnregistMemoryInfoWndClass(void);
atHWINDOW	CreateMemoryInfoWnd( atHWINDOW hParentWnd );


#ifdef __cplusplus
}
#endif


#endif // __MEMORYINFOWND_H_
