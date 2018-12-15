//
// VSlider.h
//
///////////////////////////////////////////////////////////////////////

#ifndef __CMP_VSLIDER_H_
#define __CMP_VSLIDER_H_


#ifndef __A_UIENG_H_
#include "A_UIENG.h"
#endif


#ifdef __cplusplus
extern "C" {
#endif


atHCONTROL atVSLIDER_Create( int nIndex, int nX, int nY, int nWidth, int nHeight, int direction,
								 int nCursorTopMargin, int nCursorBottomMargin, int nCursorX );
atINT	atVSLIDER_GetMaxPos( atHCONTROL hControl );
atVOID	atVSLIDER_SetMaxPos( atHCONTROL hControl, atINT nMaxPos );
atINT 	atVSLIDER_GetPosition( atHCONTROL hControl );
atINT 	atVSLIDER_SetPosition( atHCONTROL hControl, atINT nPos );


#ifdef __cplusplus
}
#endif


#endif // 

