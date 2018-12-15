#ifndef __CMP_IMAGEBUTTON_H_
#define __CMP_IMAGEBUTTON_H_


#ifndef __A_UIENG_H_
#include "A_UIENG.h"
#endif


#ifdef __cplusplus
extern "C" {
#endif


#define atIMAGEBUTTONATTR_OUTLINE		0x8000
#define atIMAGEBUTTONATTR_OPAQUE		0x4000

typedef void (atIMAGEBUTTON_DRAW_FUNC)( atHGDC hDC, atINT nWidth, atINT nHeight, atBOOL bSelect, atLPVOID lpParam );

atHCONTROL atIMAGEBUTTON_Create( int nIndex, int nX, int nY, int nWidth, int nHeight,
                         int nNormalX, int nNormalY, int nFocusedX, int nFocusedY, 
						 atHBITMAP lpNormalImage, atHBITMAP lpFocusedImage,
						 atHBITMAP lpSelectedImage, atHBITMAP lpDisableImage );

atHCONTROL atIMAGEBUTTON_GetNormalImage( atHCONTROL hControl);
atVOID atIMAGEBUTTON_SetImages( atHCONTROL hControl, atHBITMAP hBmpNoraml, atHBITMAP hBmpFocused, atHBITMAP hBmpSel, atHBITMAP hBmpDisable);

atDWORD	atIMAGEBUTTON_GetAttr(atHCONTROL hControl );
void	atIMAGEBUTTON_SetAttr(atHCONTROL hControl, atDWORD dwAttr );

void	atIMAGEBUTTON_SetDrawFunc(atHCONTROL hControl, atIMAGEBUTTON_DRAW_FUNC* fnDrawItem, atLPVOID lpFuncParam );


#ifdef __cplusplus
}
#endif


#endif // __IMAGEBUTTON_H_
