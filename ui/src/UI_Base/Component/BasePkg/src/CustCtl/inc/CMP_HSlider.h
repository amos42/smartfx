//
// HSlider.h
//
///////////////////////////////////////////////////////////////////////

#ifndef __CMP_HSLIDER_H_
#define __CMP_HSLIDER_H_


#ifndef __A_UIENG_H_
#include "A_UIENG.h"
#endif


#ifdef __cplusplus
extern "C" {
#endif


atHCONTROL atHSLIDER_Create( int nIndex, int nX, int nY, int nWidth, int nHeight, 
								 int nCursorLeftMargin, int nCursorRightMargin, int nCursorY );
atINT	atHSLIDER_GetMaxPos( atHCONTROL hControl );
atVOID	atHSLIDER_SetMaxPos( atHCONTROL hControl, atINT nMaxPos );
atINT 	atHSLIDER_GetPosition( atHCONTROL hControl );
atINT 	atHSLIDER_SetPosition( atHCONTROL hControl, atINT nPos );


#ifdef __cplusplus
}
#endif


#endif // 

