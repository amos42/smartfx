//
// TransPannel.h
//
///////////////////////////////////////////////////////////////////////

#ifndef __CMP_TRANSPANEL_H_
#define __CMP_TRANSPANEL_H_

#ifndef __A_UIENG_H_
#include "A_UIENG.h"
#endif


#ifdef __cplusplus
extern "C" {
#endif


atHCONTROL atTRANSPANEL_Create( int nIndex, int nX, int nY, int nWidth, int nHeight, atDWORD dwAttr, atREAL rTransparent );

atBOOL	atTRANSPANEL_AddControl( atHCONTROL hControl, atHCONTROL hChildControl );
atVOID	atTRANSPANEL_SetControlFocus( atHCONTROL hControl, atHCONTROL hChildControl );
atHCONTROL atTRANSPANEL_GetControlFocus( atHCONTROL hControl );


#ifdef __cplusplus
}
#endif


#endif // __PANNEL_H_
