#ifndef __CMP_ANIBUTTON_H_
#define __CMP_ANIBUTTON_H_


#ifndef __A_UIENG_H_
#include "A_UIENG.h"
#endif


#ifdef __cplusplus
extern "C" {
#endif


atHCONTROL atANIBUTTON_Create( int nIndex, int nX, int nY, int nWidth, int nHeight,
						       atINT nTimerInteval, atBOOL bRepeat, atINT nImageCount, ... );


#ifdef __cplusplus
}
#endif


#endif // __BUTTON_H_
