//
// MainWnd.h
//
///////////////////////////////////////////////////////////////////////

#ifndef _SCREEN_INFORMATION_H_INCLUDED
#define _SCREEN_INFORMATION_H_INCLUDED

#include "A_UIENG.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct _SCR_INFORMATION_WNDDATA {
	atHBITMAP hBm_bg;
	atHBITMAP hBm_title;

	atHBITMAP hBm_btn_back_n;
	atHBITMAP hBm_btn_left_n;
	atHBITMAP hBm_btn_right_n;
	
	atHBITMAP hBm_btn_back_p;

	atINT16		nType;

	int test_value;
} SCR_INFORMATION_WNDDATA, * LPSCR_INFORMATION_WNDDATA;


int Scr_inforamtion_WndOnEvent(atHWINDOW hWnd, int nMsg, long nParam1, long nParam2);


#ifdef __cplusplus
};
#endif


#endif // _MainWnd_H_INCLUDED

