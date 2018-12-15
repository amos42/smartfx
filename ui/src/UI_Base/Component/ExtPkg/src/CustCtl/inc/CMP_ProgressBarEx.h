#ifndef __CMP_PROGRESSBAREX_H_
#define __CMP_PROGRESSBAREX_H_


#ifndef __A_UIENG_H_
#include "A_UIENG.h"
#endif


#ifdef __cplusplus
extern "C" {
#endif


atHCONTROL atPROGRESSBAR_EXT_Create( int nIndex, int nX, int nY, int nWidth, int nHeight, int style, 
					  			 atHBITMAP lpProgressBarLeftImage, atHBITMAP lpProgressBarCenterImage, atHBITMAP lpProgressBarRightImage, 
					  			 atHBITMAP lpProgressMeasureLeftImage, atHBITMAP lpProgressMeasureCenterImage, atHBITMAP lpProgressMeasureRightImage,
								 int nMesureLeftMargin, int nMesureRightMargin, int nMesureY );

int atPROGRESSBAR_EXT_GetSize( atHCONTROL hControl );
void atPROGRESSBAR_EXT_SetSize( atHCONTROL hControl, int size );
int atPROGRESSBAR_EXT_GetPosition( atHCONTROL hControl );
int atPROGRESSBAR_EXT_SetPosition( atHCONTROL hControl, int pos );
void	atPROGRESSBAR_EXT_SetBitmap( atHCONTROL hControl,
					  	atHBITMAP hImgProgressLeft, atHBITMAP hImgProgressCenter, atHBITMAP hImgProgressRight,
						  atHBITMAP hImgMeasureLeft, atHBITMAP hImgMeasureCenter, atHBITMAP hImgMeasureRight,
						  int nMesureLeftMargin, int nMesureRightMargin, int nMesureY );


#ifdef __cplusplus
}
#endif


#endif // _MYCONTROL_H_INCLUDED
