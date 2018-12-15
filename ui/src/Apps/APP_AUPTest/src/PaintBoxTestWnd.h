//
// MainWnd.h
//
///////////////////////////////////////////////////////////////////////

#ifndef _PAINTBOXTESTWND_H_INCLUDED
#define _PAINTBOXTESTWND_H_INCLUDED

#include "A_UIENG.h"

#ifdef __cplusplus
extern "C" {
#endif


typedef struct _PAINTBOXTESTWNDDATA {
	int dummy;
} PAINTBOXTESTWNDDATA, * LPPAINTBOXTESTWNDDATA;


int PaintBoxTestWndOnEvent(atHWINDOW hWnd, int nMsg, long nParam1, long nParam2);


#ifdef __cplusplus
}
#endif


#endif // _MainWnd_H_INCLUDED

