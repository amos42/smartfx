//
// MainWnd.h
//
///////////////////////////////////////////////////////////////////////

#ifndef _MAINWND_H_INCLUDED
#define _MAINWND_H_INCLUDED

#include "A_UIENG.h"

#ifdef __cplusplus
extern "C" {
#endif


typedef struct _MAINWNDDATA {
	atHCONTROL hListBox;
//    atHBITMAP hBm[2];
//    int idx;
} MAINWNDDATA, * LPMAINWNDDATA;


int MainWndOnEvent(atHWINDOW hWnd, int nMsg, long nParam1, long nParam2);


#ifdef __cplusplus
};
#endif


#endif // _MainWnd_H_INCLUDED

