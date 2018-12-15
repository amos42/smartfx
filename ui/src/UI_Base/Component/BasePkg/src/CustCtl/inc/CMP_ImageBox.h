//
// ImageBox.h
//
///////////////////////////////////////////////////////////////////////

#ifndef __CMP_IMAGEBOX_H_
#define __CMP_IMAGEBOX_H_

#ifndef __A_UIENG_H_
#include "A_UIENG.h"
#endif


#ifdef __cplusplus
extern "C" {
#endif


#define atIMAGEBOXATTR_OUTLINE	0x8000
#define atIMAGEBOXATTR_OPAQUE	0x4000


atHCONTROL atIMAGEBOX_Create( int nIndex, int nX, int nY, int nWidth, int nHeight, atHBITMAP lpImage, atBOOL bOwnImage ); 
atU32 atIMAGEBOX_GetAttr(atHCONTROL hControl );
void atIMAGEBOX_SetAttr(atHCONTROL hControl, atU32 uAttr );
void atIMAGEBOX_SetAlign(atHCONTROL hControl, int align );



#ifdef __cplusplus
}
#endif


#endif // __IMAGEBOX_H_
