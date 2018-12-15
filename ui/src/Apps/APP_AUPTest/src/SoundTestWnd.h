//
// MainWnd.h
//
///////////////////////////////////////////////////////////////////////

#ifndef _SOUNDTESTWND_H_INCLUDED
#define _SOUNDTESTWND_H_INCLUDED

#include "A_UIENG.h"

#ifdef __cplusplus
extern "C" {
#endif


typedef struct _SOUNDTESTWNDDATA {
	atHCONTROL hListBox;
	atHCONTROL hEditBox;

	atHCONTROL hCtl1, hCtl2, hCtl3;
//    atHBITMAP hBm[2];
//    int idx;
} SOUNDTESTWNDDATA, *LPSOUNDTESTWNDDATA;


int SoundTestWndOnEvent(atHWINDOW hWnd, int nMsg, long nParam1, long nParam2);


#ifdef __cplusplus
};
#endif


#endif // _MainWnd_H_INCLUDED

