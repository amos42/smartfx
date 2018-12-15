#ifndef __FILELISTWND_H_
#define __FILELISTWND_H_

#ifndef __A_UIENG_H_
#include "A_UIENG.h"
#endif


#ifdef __cplusplus
extern "C" {
#endif


#define EXTPKG_CUSTWIN_FILELIST      _AT("ExtPkg.CustWin.FileListWnd")


atBOOL      RegistFileListWndClass(void);
atHWINDOW	CreateFileListWnd(atHWINDOW hParentWnd, int id, atLPTSTR szTitle, atLPTSTR szPath, atLPTSTR szFilter, int max_cnt, atBOOL bCheckbox, atLPTSTR szOutput );


#ifdef __cplusplus
}
#endif


#endif // _MSGBOXLIST_H_INCLUDED
