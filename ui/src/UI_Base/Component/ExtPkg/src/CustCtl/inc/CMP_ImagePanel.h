//
// ImagePannel.h
//
///////////////////////////////////////////////////////////////////////

#ifndef __CMP_IMAGEPANEL_H_
#define __CMP_IMAGEPANEL_H_

#ifndef __A_UIENG_H_
#include "A_UIENG.h"
#endif


#ifdef __cplusplus
extern "C" {
#endif


atHCONTROL atIMAGEPANEL_Create( int nIndex, int nX, int nY, int nWidth, int nHeight, atDWORD dwAttr, 
    														atHBITMAP bmTopLeft, atHBITMAP bmTopCenter, atHBITMAP bmTopRight,
    														atHBITMAP bmMiddleLeft, atHBITMAP bmMiddleCenter, atHBITMAP bmMiddleRight,
    														atHBITMAP bmBottomLeft, atHBITMAP bmBottomCenter, atHBITMAP bmBottomRight );

atBOOL	atIMAGEPANEL_AddControl( atHCONTROL hControl, atHCONTROL hChildControl );
atVOID	atIMAGEPANEL_SetControlFocus( atHCONTROL hControl, atHCONTROL hChildControl );
atHCONTROL atIMAGEPANEL_GetControlFocus( atHCONTROL hControl );


#ifdef __cplusplus
}
#endif


#endif // __PANNEL_H_
