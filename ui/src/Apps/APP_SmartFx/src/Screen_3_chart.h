//
// MainWnd.h
//
///////////////////////////////////////////////////////////////////////

#ifndef _SCREEN_3_CAHRT_H_INCLUDED
#define _SCREEN_3_CHART_H_INCLUDED

#include "A_UIENG.h"

#ifdef __cplusplus
extern "C" {
#endif


typedef struct _SCR_3_CHART_WNDDATA {
	atHBITMAP hBm_bg;
	atHBITMAP hBm_title;

	atHBITMAP hBm_btn_01_n;
	atHBITMAP hBm_btn_02_n;
	atHBITMAP hBm_btn_03_n;
	atHBITMAP hBm_btn_04_n;
	atHBITMAP hBm_btn_05_n;
	atHBITMAP hBm_btn_06_n;
	atHBITMAP hBm_btn_07_n;
	atHBITMAP hBm_btn_exit_n;

	atHBITMAP hBm_btn_01_p;
	atHBITMAP hBm_btn_02_p;
	atHBITMAP hBm_btn_03_p;
	atHBITMAP hBm_btn_04_p;
	atHBITMAP hBm_btn_05_p;
	atHBITMAP hBm_btn_06_p;
	atHBITMAP hBm_btn_07_p;
	atHBITMAP hBm_btn_exit_p;

	atINT16 nType;

} SCR_3_CHART_WNDDATA, * LPSCR_3_CHART_WNDDATA;


int Scr_3_Chart_WndOnEvent(atHWINDOW hWnd, int nMsg, long nParam1, long nParam2);


#ifdef __cplusplus
};
#endif


#endif // _MainWnd_H_INCLUDED

