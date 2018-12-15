#ifndef __CMP_RADIOBUTTONEX_H_
#define __CMP_RADIOBUTTONEX_H_


#ifndef __A_UIENG_H_
#include "A_UIENG.h"
#endif


#ifdef __cplusplus
extern "C" {
#endif


atHCONTROL atRADIOBUTTON_EXT_Create( int nIndex, int nX, int nY, int nWidth, int nHeight, 
							int nGroupID, atLPTSTR szText, 
							atHBITMAP lpNormalImage, atHBITMAP lpSelectImage);

atBOOL atRADIOBUTTON_EXT_IsSelected( atHCONTROL hControl );
void atRADIOBUTTON_EXT_SetSelect( atHCONTROL hControl, atBOOL bEnable );


#ifdef __cplusplus
}
#endif


#endif // __RADIOBUTTON_H_
