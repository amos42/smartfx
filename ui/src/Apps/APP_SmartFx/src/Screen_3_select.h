//
// MainWnd.h
//
///////////////////////////////////////////////////////////////////////

#ifndef _SCREEN_3_SELECT_H_INCLUDED
#define _SCREEN_3_SELECT_H_INCLUDED

#include "A_UIENG.h"

#ifdef __cplusplus
extern "C" {
#endif


typedef struct _SCR_3_SELECT_WNDDATA {
	atHBITMAP hBm_bg;

	atHBITMAP hBm_btn_sp_n;
	atHBITMAP hBm_btn_vi_n;
	atHBITMAP hBm_btn_home_n;

	atHBITMAP hBm_btn_sp_p;
	atHBITMAP hBm_btn_vi_p;
	atHBITMAP hBm_btn_home_p;

	atINT16 nType;
} SCR_3_SELECT_WNDDATA, * LPSCR_3_SELECT_WNDDATA;

int Scr_3_Select_WndOnEvent(atHWINDOW hWnd, int nMsg, long nParam1, long nParam2);

#ifdef __cplusplus
};
#endif

#endif // _SCREEN_3_SELECT_H_INCLUDED

