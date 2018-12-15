//
// MainWnd.h
//
///////////////////////////////////////////////////////////////////////

#ifndef _SCREEN_1_RESULT_H_INCLUDED
#define _SCREEN_1_RESULT_H_INCLUDED

#include "A_UIENG.h"

#ifdef __cplusplus
extern "C" {
#endif


typedef struct _SCR_1_RESULT_WNDDATA {
	atHBITMAP hBm_bg;
	//atHBITMAP hBm_content_ok;
	//atHBITMAP hBm_content_no;

	atHBITMAP	hBm_title;

	atHBITMAP hBm_btn_home_n;
	atHBITMAP hBm_btn_check_n;
	atHBITMAP hBm_btn_home_p;
	atHBITMAP hBm_btn_check_p;

	atHFONT hFnt_main;

	atINT16 nType;
} SCR_1_RESULT_WNDDATA, * LPSCR_1_RESULT_WNDDATA;


int Scr_1_Result_WndOnEvent(atHWINDOW hWnd, int nMsg, long nParam1, long nParam2);


#ifdef __cplusplus
};
#endif


#endif // _MainWnd_H_INCLUDED

