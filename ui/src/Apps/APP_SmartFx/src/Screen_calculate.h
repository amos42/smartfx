//
// MainWnd.h
//
///////////////////////////////////////////////////////////////////////

#ifndef _SCREEN_CALCULATE_H_INCLUDED
#define _SCREEN_CALCULATE_H_INCLUDED

#include "A_UIENG.h"

#ifdef __cplusplus
extern "C" {
#endif


typedef struct _SCR_CALCULATE_WNDDATA 
{
	atHBITMAP hBm_btn_img;
	atHBITMAP hBm_btn_ok;

	atHFONT hFnt_main;

	atINT16		nVoltage;

	atINT16		nType;
} SCR_CALCULATE_WNDDATA, * LPSCR_CALCULATE_WNDDATA;


int Scr_Calculate_WndOnEvent(atHWINDOW hWnd, int nMsg, long nParam1, long nParam2);


#ifdef __cplusplus
};
#endif


#endif // _SCREEN_CALCULATE_H_INCLUDED

