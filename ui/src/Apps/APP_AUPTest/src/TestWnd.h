//
// MainWnd.h
//
///////////////////////////////////////////////////////////////////////

#ifndef _TESTWND_H_INCLUDED
#define _TESTWND_H_INCLUDED

#include "A_UIENG.h"

#ifdef __cplusplus
extern "C" {
#endif


typedef struct _TESTWNDDATA {
	atHCONTROL hListBox;
	atHCONTROL hEditBox;

	atHCONTROL hCtl1, hCtl2, hCtl3;
//    atHBITMAP hBm[2];
//    int idx;
} TESTWNDDATA, * LPTESTWNDDATA;


int TestWndOnEvent(atHWINDOW hWnd, int nMsg, long nParam1, long nParam2);


#ifdef __cplusplus
};
#endif


#endif // _MainWnd_H_INCLUDED

