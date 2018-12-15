//
// PaintBox.h
//
///////////////////////////////////////////////////////////////////////

#ifndef __CMP_PAINTBOX_H_
#define __CMP_PAINTBOX_H_

#ifndef __A_UIENG_H_
#include "A_UIENG.h"
#endif


#ifdef __cplusplus
extern "C" {
#endif


typedef void (atPAINTBOX_RENDERER)( atHCONTROL hControl, atHGDC hCanvas, atBOOL bFocused );
typedef atPAINTBOX_RENDERER* atLPPAINTBOX_RENDERER;


atHCONTROL atPAINTBOX_Create( int nIndex, int nX, int nY, int nWidth, int nHeight, atDWORD dwAttr, atLPPAINTBOX_RENDERER lpRenderer );

atBOOL	atPAINTBOX_SetRenderer( atHCONTROL hControl, atLPPAINTBOX_RENDERER lpRenderer );


#ifdef __cplusplus
}
#endif


#endif // __PANNEL_H_
