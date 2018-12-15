//
// MainWnd.h
//
///////////////////////////////////////////////////////////////////////

#ifndef _SCREEN_SETTING_H_INCLUDED
#define _SCREEN_SETTING_H_INCLUDED

#include "A_UIENG.h"

#ifdef __cplusplus
extern "C" {
#endif


typedef struct _SCR_SETTING_WNDDATA {
	atHBITMAP hBm_bg;
	atHBITMAP hBm_title;

	atHBITMAP hBm_bright_1;
	atHBITMAP hBm_bright_2;
	atHBITMAP hBm_bright_3;
	atHBITMAP hBm_bright_4;
	atHBITMAP hBm_bright_5;

	atHBITMAP hBm_btn_dist_l;
	atHBITMAP hBm_btn_dist_r;

	atHBITMAP hBm_btn_battery_low;
	atHBITMAP hBm_btn_battery_high;

	atHBITMAP hBm_btn_msg_on;
	atHBITMAP hBm_btn_msg_off;

	atHBITMAP hBm_btn_home_n;
	atHBITMAP hBm_btn_home_p;
	atHBITMAP hBm_btn_check_n;
	atHBITMAP hBm_btn_check_p;

	atHFONT hFnt_main;
	atHFONT hFnt_sub;

	atINT16		nType;

	atBOOL		bInit;
	atINT16		nIndexDist;
	atINT16		nTempBattery;

} SCR_SETTING_WNDDATA, * LPSCR_SETTING_WNDDATA;


int Scr_Setting_WndOnEvent(atHWINDOW hWnd, int nMsg, long nParam1, long nParam2);


#ifdef __cplusplus
};
#endif


#endif // _MainWnd_H_INCLUDED

