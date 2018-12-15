#ifndef __CMP_HSLIDEREX_H_
#define __CMP_HSLIDEREX_H_


#ifndef __A_UIENG_H_
#include "A_UIENG.h"
#endif


#ifdef __cplusplus
extern "C" {
#endif


atHCONTROL atHSLIDER_EXT_Create( int nIndex, int nX, int nY, int nWidth, int nHeight, 
					  			 atHBITMAP lpHSliderLeft, atHBITMAP lpHSliderCenter, atHBITMAP lpHSliderRight,
								 atHBITMAP lpHSliderTickNormal, atHBITMAP lpHSliderTickFocus, atHBITMAP lpHSliderTickDisable,
								 int nCursorLeftMargin, int nCursorRightMargin, int nCursorY );
atINT	atHSLIDER_EXT_GetMaxPos( atHCONTROL hControl );
atVOID	atHSLIDER_EXT_SetMaxPos( atHCONTROL hControl, atINT nMaxPos );
atINT 	atHSLIDER_EXT_GetPosition( atHCONTROL hControl );
atINT 	atHSLIDER_EXT_SetPosition( atHCONTROL hControl, atINT nPos );


#ifdef __cplusplus
}
#endif


#endif // 

