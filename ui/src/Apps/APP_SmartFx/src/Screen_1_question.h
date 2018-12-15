//
// MainWnd.h
//
///////////////////////////////////////////////////////////////////////

#ifndef _SCREEN_1_QUESTION_H_INCLUDED
#define _SCREEN_1_QUESTION_H_INCLUDED

#include "A_UIENG.h"

#ifdef __cplusplus
extern "C" {
#endif


typedef struct _SCR_1_QUESTION_WNDDATA {
	atHBITMAP hBm_bg;
	atHBITMAP hBm_title;
	atHBITMAP hBm_content;

	atHBITMAP hBm_btn_yes_n;
	atHBITMAP hBm_btn_no_n;
	atHBITMAP hBm_btn_yes_p;
	atHBITMAP hBm_btn_no_p;

	atINT16		nType;
} SCR_1_QUESTION_WNDDATA, * LPSCR_1_QUESTION_WNDDATA;


int Scr_1_Question_WndOnEvent(atHWINDOW hWnd, int nMsg, long nParam1, long nParam2);


#ifdef __cplusplus
};
#endif


#endif // _MainWnd_H_INCLUDED

