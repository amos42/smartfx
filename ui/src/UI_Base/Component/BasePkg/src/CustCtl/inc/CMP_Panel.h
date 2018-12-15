//
// Pannel.h
//
///////////////////////////////////////////////////////////////////////

#ifndef __CMP_PANEL_H_
#define __CMP_PANEL_H_

#ifndef __A_UIENG_H_
#include "A_UIENG.h"
#endif


#ifdef __cplusplus
extern "C" {
#endif


atHCONTROL atPANEL_Create( int nIndex, int nX, int nY, int nWidth, int nHeight, atDWORD dwAttr, int nBorderWidth );

atBOOL	atPANEL_AddControl( atHCONTROL hControl, atHCONTROL hChildControl );
atVOID	atPANEL_SetControlFocus( atHCONTROL hControl, atHCONTROL hChildControl );
atHCONTROL atPANEL_GetControlFocus( atHCONTROL hControl );


#ifdef __cplusplus
}
#endif


#endif // __PANNEL_H_
