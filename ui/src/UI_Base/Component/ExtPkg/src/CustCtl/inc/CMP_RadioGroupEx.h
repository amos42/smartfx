#ifndef __CMP_RADIOGROUPEX_H_
#define __CMP_RADIOGROUPEX_H_


#ifndef __A_UIENG_H_
#include "A_UIENG.h"
#endif


#ifdef __cplusplus
extern "C" {
#endif


atHCONTROL atRADIOGROUP_EXT_Create( int nIndex, int nX, int nY, int nWidth, int nHeight, 
									atTCHAR (*szButtonMsg)[32], int ButtonCnt, int init_idx, int item_height,							
									int horiz_item_count, int horiz_gap, int line_gap,
									atHBITMAP lpNormalImage, atHBITMAP lpSelectImage,
						  			//atHBITMAP lpScrollbarImgTop, atHBITMAP lpScrollbarImage, atHBITMAP lpScrollbarImgBottom, atHBITMAP lpScrollcursorImage );
									atHBITMAP lpScrollbarImgTop, atHBITMAP lpScrollbarImage, atHBITMAP lpScrollbarImgBottom,
									atHBITMAP lpScrollCursorImgTop, atHBITMAP lpScrollCursorImgMid, atHBITMAP lpScrollCursorImgBot );

int atRADIOGROUP_EXT_GetItemCount( atHCONTROL hControl );
int atRADIOGROUP_EXT_AddItem( atHCONTROL hControl, atLPTSTR itm_str );
int atRADIOGROUP_EXT_GetSelectIndex( atHCONTROL hControl );
void atRADIOGROUP_EXT_SetSelectIndex( atHCONTROL hControl, int idx );


#ifdef __cplusplus
}
#endif


#endif // __RADIOBUTTON_H_
