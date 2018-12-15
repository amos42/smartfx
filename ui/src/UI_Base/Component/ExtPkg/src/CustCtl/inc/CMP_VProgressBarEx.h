#ifndef __CMP_VPROGRESSBAREX_H_
#define __CMP_VPROGRESSBAREX_H_


#ifndef __A_UIENG_H_
#include "A_UIENG.h"
#endif


#ifdef __cplusplus
extern "C" {
#endif


atHCONTROL atVPROGRESSBAR_EXT_Create( int nIndex, int nX, int nY, int nWidth, int nHeight, int style,
					  			 atHBITMAP lpProgressBarTopImage, atHBITMAP lpProgressBarMiddleImage, atHBITMAP lpProgressBarBottomImage, 
					  			 atHBITMAP lpProgressMeasureTopImage, atHBITMAP lpProgressMeasureMiddleImage, atHBITMAP lpProgressMeasureBottomImage,
								 int nMesureTopMargin, int nMesureBottomMargin, int nMesureX );

int atVPROGRESSBAR_EXT_GetSize( atHCONTROL hControl );
void atVPROGRESSBAR_EXT_SetSize( atHCONTROL hControl, int size );
int atVPROGRESSBAR_EXT_GetPosition( atHCONTROL hControl );
int atVPROGRESSBAR_EXT_SetPosition( atHCONTROL hControl, int pos );
void	atVPROGRESSBAR_EXT_SetBitmap( atHCONTROL hControl,
					  	atHBITMAP hImgProgressTop, atHBITMAP hImgProgressMiddle, atHBITMAP hImgProgressBottom,
						  atHBITMAP hImgMeasureTop, atHBITMAP hImgMeasureMiddle, atHBITMAP hImgMeasureBottom,
						  int nMesureTopMargin, int nMesureBottomMargin, int nMesureX );


#ifdef __cplusplus
}
#endif


#endif // _MYCONTROL_H_INCLUDED
