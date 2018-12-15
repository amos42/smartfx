#ifndef __CMP_VSLIDEREX_H_
#define __CMP_VSLIDEREX_H_


#ifndef __A_UIENG_H_
#include "A_UIENG.h"
#endif


#ifdef __cplusplus
extern "C" {
#endif


atHCONTROL atVSLIDER_EXT_Create( int nIndex, int nX, int nY, int nWidth, int nHeight, int direction,
					  			 atHBITMAP lpVSliderTop, atHBITMAP lpVSliderMiddle, atHBITMAP lpVSliderBottom,
								 atHBITMAP lpVSliderTickNormal, atHBITMAP lpVSliderTickFocus, atHBITMAP lpVSliderTickDisable,
								 int nCursorTopMargin, int nCursorBottomMargin, int nCursorX );
atINT	atVSLIDER_EXT_GetMaxPos( atHCONTROL hControl );
atVOID	atVSLIDER_EXT_SetMaxPos( atHCONTROL hControl, atINT nMaxPos );
atINT 	atVSLIDER_EXT_GetPosition( atHCONTROL hControl );
atINT 	atVSLIDER_EXT_SetPosition( atHCONTROL hControl, atINT nPos );


#ifdef __cplusplus
}
#endif


#endif // 

