#ifndef __CMP_BUTTONEX_H_
#define __CMP_BUTTONEX_H_


#ifndef __A_UIENG_H_
#include "A_UIENG.h"
#endif


#ifdef __cplusplus
extern "C" {
#endif


atHCONTROL atBUTTON_EXT_Create( int nIndex, int nX, int nY, int nWidth, int nHeight, 
	                     atLPTSTR szText, 
						 atHBITMAP lpNormalImg_Left, atHBITMAP lpNormalImg_Center, atHBITMAP lpNormalImg_Right,
						 atHBITMAP lpFocusedImg_Left, atHBITMAP lpFocusedImg_Center, atHBITMAP lpFocusedImg_Right,
						 atHBITMAP lpSelectedImg_Left, atHBITMAP lpSelectedImg_Center, atHBITMAP lpSelectedImg_Right,
						 atHBITMAP lpDisabledImg_Left, atHBITMAP lpDisabledImg_Center, atHBITMAP lpDisabledImg_Right );

atVOID	atBUTTON_EXT_SetText( atHCONTROL hControl, atLPTSTR szText );
atVOID	atBUTTON_EXT_GetText( atHCONTROL hControl, atLPTSTR szText, atINT nMaxLen );
atVOID	atBUTTON_EXT_SetBitmap( atHCONTROL hControl, atHBITMAP lpNormalImg_Left, atHBITMAP lpNormalImg_Center, atHBITMAP lpNormalImg_Right,
							   atHBITMAP lpFocusedImg_Left, atHBITMAP lpFocusedImg_Center, atHBITMAP lpFocusedImg_Right,
							   atHBITMAP lpSelectedImg_Left, atHBITMAP lpSelectedImg_Center, atHBITMAP lpSelectedImg_Right,
							   atHBITMAP lpDisabledImg_Left, atHBITMAP lpDisabledImg_Center, atHBITMAP lpDisabledImg_Right );


#ifdef __cplusplus
}
#endif


#endif // __BUTTON_H_
