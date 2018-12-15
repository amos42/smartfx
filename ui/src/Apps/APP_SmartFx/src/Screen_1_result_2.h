//
// MainWnd.h
//
///////////////////////////////////////////////////////////////////////

#ifndef _SCREEN_1_RESULT_2_H_INCLUDED
#define _SCREEN_1_REsULT_2_H_INCLUDED

#include "A_UIENG.h"

#ifdef __cplusplus
extern "C" {
#endif


typedef struct _SCR_1_RESULT_2_WNDDATA {
	atHBITMAP	hBm_bg;
	atHBITMAP	hBm_title;
	atHBITMAP	hBm_btn_ok_n;
	atHBITMAP	hBm_btn_ok_p;

	atHFONT		hFnt_main;

	atINT16		nType;

	int test_value;
} SCR_1_RESULT_2_WNDDATA, * LPSCR_1_RESULT_2_WNDDATAA;


int Scr_1_Result_2_WndOnEvent(atHWINDOW hWnd, int nMsg, long nParam1, long nParam2);


#ifdef __cplusplus
};
#endif


#endif // _SCREEN_1_REsULT_2_H_INCLUDED

