//
// SettingWnd.h
//
///////////////////////////////////////////////////////////////////////

#ifndef _SETTINGWND_H_INCLUDED
#define _SETTINGWND_H_INCLUDED

#include "Window.h"

#ifdef __cplusplus
extern "C" {
#endif


typedef struct _SETTINGWNDDATA
{
	atHCONTROL hListBox;
//    atHBITMAP hBm[2];
//    int idx;
} SETTINGWNDDATA, * LPSETTINGWNDDATA;


int SettingWndOnEvent(atHWINDOW hWnd, int nMsg, long nParam1, long nParam2);


#ifdef __cplusplus
};
#endif


#endif // _SETTINGWND_H_INCLUDED

