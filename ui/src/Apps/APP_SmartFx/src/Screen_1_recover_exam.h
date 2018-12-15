//
// MainWnd.h
//
///////////////////////////////////////////////////////////////////////

#ifndef _SCREEN_1_DIVID_RECOVER_H_INCLUDED
#define _SCREEN_1_DIVID_RECOVER_H_INCLUDED

#include "A_UIENG.h"

#ifdef __cplusplus
extern "C" {
#endif


typedef struct _SCR_1_RECOVER_EXAM_WNDDATA {
	atHBITMAP	hBm_bg;
	//atHBITMAP	hBm_title;

	atHBITMAP	hBm_btn_info_n;
	atHBITMAP	hBm_btn_home_n;
	atHBITMAP	hBm_btn_check_n;

	atHBITMAP	hBm_btn_info_p;
	atHBITMAP	hBm_btn_home_p;
	atHBITMAP	hBm_btn_check_p;

	atHBITMAP	hBm_btn_calcu_n;
	atHBITMAP	hBm_btn_calcu_p;

	atHFONT		hFnt_sub;

	atUINT		nType;
} SCR_1_RECOVER_EXAM_WNDDATA, * LPSCR_1_RECOVER_EXAM_WNDDATA;


int Scr_1_Recover_Exam_WndOnEvent(atHWINDOW hWnd, int nMsg, long nParam1, long nParam2);


#ifdef __cplusplus
};
#endif


#endif // _MainWnd_H_INCLUDED

