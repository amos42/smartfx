//
// MainWnd.h
//
///////////////////////////////////////////////////////////////////////

#ifndef _SCREEN_3_IMAGE_H_INCLUDED
#define _SCREEN_3_IMAGE_H_INCLUDED

#include "A_UIENG.h"

#ifdef __cplusplus
extern "C" {
#endif


typedef struct _SCR_3_IMAGE_WNDDATA {
	atHBITMAP hBm_bg;
	atHBITMAP hBm_title;

	atHCONTROL hBtn_center;

	atHBITMAP hBm_btn_center_n;
	atHBITMAP hBm_btn_info_n;
	atHBITMAP hBm_btn_info_p;

	atHBITMAP hBm_btn_back_n;
	atHBITMAP hBm_btn_back_p;

	atHFONT hFnt_main;

	atINT16 nType;

} SCR_3_IMAGE_WNDDATA, * LPSCR_3_IMAGE_WNDDATA;


int Scr_3_Image_WndOnEvent(atHWINDOW hWnd, int nMsg, long nParam1, long nParam2);


#ifdef __cplusplus
};
#endif


#endif // _MainWnd_H_INCLUDED

