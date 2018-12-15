//
// MyControl.h
//
///////////////////////////////////////////////////////////////////////

#ifndef __CMP_RADIOBUTTON_H_
#define __CMP_RADIOBUTTON_H_


#ifndef __A_UIENG_H_
#include "A_UIENG.h"
#endif


#ifdef __cplusplus
extern "C" {
#endif


atHCONTROL atRADIOBUTTON_Create( int nIndex, int nX, int nY, int nWidth, int nHeight, 
							int nGroupID, atLPTSTR szText );
/*
void atRADIOBUTTON_Destroy( atHCONTROL hControl );
void atRADIOBUTTON_Draw( atHGDC hCanvas, atHCONTROL hControl, atBOOL bFocused );
*/

atBOOL atRADIOBUTTON_IsSelected( atHCONTROL hControl );
void atRADIOBUTTON_SetSelect( atHCONTROL hControl, atBOOL bEnable );


#ifdef __cplusplus
}
#endif


#endif // __RADIOBUTTON_H_
