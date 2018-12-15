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


typedef struct _MAINWNDDATA 
{
	atHCONTROL ctlPanel;
	atHCONTROL ctlBattery;

    atHBITMAP hBm_bg;
	atHBITMAP hBm_battery;

	atHBITMAP hBm_btn_01_n;
	atHBITMAP hBm_btn_01_p;
	atHBITMAP hBm_btn_02_n;
	atHBITMAP hBm_btn_02_p;
	atHBITMAP hBm_btn_03_n;
	atHBITMAP hBm_btn_03_p;

	atHBITMAP hBm_btn_01_res_n;
	atHBITMAP hBm_btn_01_res_p;
	atHBITMAP hBm_btn_02_res_n;
	atHBITMAP hBm_btn_02_res_p;

	atHBITMAP hBm_btn_setting;

    int idx;
	int nCount;
} MAINWNDDATA, * LPMAINWNDDATA;


int MainWndOnEvent(atHWINDOW hWnd, int nMsg, long nParam1, long nParam2);


#ifdef __cplusplus
};
#endif


#endif // _MainWnd_H_INCLUDED

