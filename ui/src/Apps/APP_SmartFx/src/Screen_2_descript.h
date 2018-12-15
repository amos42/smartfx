//
// MainWnd.h
//
///////////////////////////////////////////////////////////////////////

#ifndef _SCREEN_2_DESCRIPT_H_INCLUDED
#define _SCREEN_2_DESCRIPT_H_INCLUDED

#include "A_UIENG.h"

#ifdef __cplusplus
extern "C" {
#endif


typedef struct _SCR_2_DESC_WNDDATA {
	atHBITMAP hBm_bg;
	atHBITMAP hBm_title;
	atHBITMAP hBm_btn_ok_n;
	atHBITMAP hBm_btn_ok_p;

	atINT16 nType;

} SCR_2_DESC_WNDDATA, * LPSCR_2_DESC_WNDDATA;


int Scr_2_Desc_WndOnEvent(atHWINDOW hWnd, int nMsg, long nParam1, long nParam2);


#ifdef __cplusplus
};
#endif


#endif // _MainWnd_H_INCLUDED

