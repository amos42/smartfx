//
// EqSetWnd.h
//
///////////////////////////////////////////////////////////////////////

#ifndef __EQSETWND_H_
#define __EQSETWND_H_

#include "A_UIENG.h"

#ifdef __cplusplus
extern "C" {
#endif


typedef struct _EQSETWNDDATA {
	atHCONTROL hEq[6];

	atHTIMER hTmr;

	atBOOL bOSDflag;
} EQSETWNDDATA, * LPEQSETWNDDATA;


int EqSetWndOnEvent(atHWINDOW hWnd, int nMsg, long nParam1, long nParam2);


#ifdef __cplusplus
}
#endif


#endif // __EQSETWND_H_
