//
// MainWnd.h
//
///////////////////////////////////////////////////////////////////////

#ifndef _SCREEN_3_RESULT_H_INCLUDED
#define _SCREEN_3_RESULT_H_INCLUDED

#include "A_UIENG.h"

#ifdef __cplusplus
extern "C" {
#endif


typedef struct _SCR_3_RESULT_WNDDATA 
{
	atHBITMAP hBm_bg;
	atHBITMAP hBm_title;
	atHBITMAP hBm_btn_back_n;
	atHBITMAP hBm_btn_back_p;

	atINT16		nType;
} SCR_3_RESULT_WNDDATA, * LPSCR_3_RESULT_WNDDATA;


int Scr_3_Result_WndOnEvent(atHWINDOW hWnd, int nMsg, long nParam1, long nParam2);


#ifdef __cplusplus
};
#endif


#endif // _SCREEN_3_RESULT_H_INCLUDED

