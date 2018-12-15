#ifndef __CMP_SCROLLBAREX_H_
#define __CMP_SCROLLBAREX_H_


#ifndef __A_UIENG_H_
#include "A_UIENG.h"
#endif


#ifdef __cplusplus
extern "C" {
#endif


typedef enum {
	atSCROLLBAR_EXT_TYPE_NORMAL,
	atSCROLLBAR_EXT_TYPE_WINDOW,
	atSCROLLBAR_EXT_TYPE_WINDOW_FRICTION,
	atSCROLLBAR_EXT_TYPE_WINDOW_FRICTION_STICK
} atSCROLLBAR_EXT_TYPE;

atHCONTROL atSCROLLBAR_EXT_Create( int nIndex, int nX, int nY, int nWidth, int nHeight, atSCROLLBAR_EXT_TYPE style, 	
								 int nCursorX, int nCursorTopMargin, int nCursorBottomMargin, int nCursorMinSize,
					  			 atHBITMAP lpScrollbarImgTop, atHBITMAP lpScrollbarImage, atHBITMAP lpScrollbarImgBottom, 
					  			 atHBITMAP lpScrollcursorImgTop, atHBITMAP lpScrollcursorImgMid, atHBITMAP lpScrollcursorImgBot );
int atSCROLLBAR_EXT_GetSize( atHCONTROL hControl );
void atSCROLLBAR_EXT_SetSize( atHCONTROL hControl, int size );
int atSCROLLBAR_EXT_GetPosition( atHCONTROL hControl );
int atSCROLLBAR_EXT_SetPosition( atHCONTROL hControl, int pos );
int atSCROLLBAR_EXT_GetMaxPosition( atHCONTROL hControl );
int atSCROLLBAR_EXT_Scroll( atHCONTROL hControl, int value );
//void atSCROLLBAR_EXT_SetBitmap(atHCONTROL hControl, atHBITMAP lpScrollbarImgTop, atHBITMAP lpScrollbarImage, atHBITMAP lpScrollbarImgBottom, atHBITMAP lpScrollcursorImage );
void atSCROLLBAR_EXT_SetBitmap(atHCONTROL hControl, atHBITMAP lpScrollbarImgTop, atHBITMAP lpScrollbarImage, atHBITMAP lpScrollbarImgBottom,
							   atHBITMAP lpScrollCursorImgTop, atHBITMAP lpScrollCursorImgMid, atHBITMAP lpScrollCursorImgBot );
void atSCROLLBAR_EXT_SetUseTouch(atHCONTROL hControl, atBOOL bUsed);	// Touch 사용설정 적용 API
int atSCROLLBAR_EXT_GetUseTouch(atHCONTROL hControl);					// Touch 사용설정 확인 API

void atSCROLLBAR_EXT_StartGestureScroll( atHCONTROL hControl, atINT nStartY );
void atSCROLLBAR_EXT_ProcessGestureScroll( atHCONTROL hControl, atINT nY );
void atSCROLLBAR_EXT_EndGestureScroll( atHCONTROL hControl, atINT nEndY );
void atSCROLLBAR_EXT_StopAndResetGestureScroll( atHCONTROL hControl );
atBOOL atSCROLLBAR_EXT_IsProcessGestureScroll( atHCONTROL hControl );


#ifdef __cplusplus
}
#endif


#endif // __SCROLLBAR_H_
