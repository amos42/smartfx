//
// MyControl.h
//
///////////////////////////////////////////////////////////////////////

#ifndef __CMP_SPINBOXEX_H_
#define __CMP_SPINBOXEX_H_


#ifndef __A_UIENG_H_
#include "A_UIENG.h"
#endif


#ifdef __cplusplus
extern "C" {
#endif


typedef struct _tagatSPIN_EXT_ITEM {
	int			nIndex;
	atTCHAR		szText[32];
	void *	data;
} atSPIN_EXT_ITEM, * atLPSPIN_EXT_ITEM;


atHCONTROL atSPINBOX_EXT_Create( int nIndex, int nX, int nY, int nWidth, int nHeight, 
								atHBITMAP lpNormalLeftImage, atHBITMAP lpNormalRightImage, atHBITMAP lpNormalBGImage,
								atHBITMAP lpFocusedLeftImage, atHBITMAP lpFocusedRightImage, atHBITMAP lpFocusBGImage);

// 이미 존재하는 nIndex의 경우 내용만 바뀌게 됨(데이터를 메모리 생성하지 않음).
// 1: 새로생성, 0: 기존거에 업데이트
int atSPINBOX_EXT_AddItem(atHCONTROL hControl, int nIndex, atLPTSTR szText);

atBOOL atSPINBOX_EXT_SetCurrItem(atHCONTROL hControl, int nSpinBoxItemIndex);
int atSPINBOX_EXT_GetCurrItem( atHCONTROL hControl );

// added by jin, 2005-12-23
atLPTSTR atSPINBOX_EXT_GetCurrItemText(atHCONTROL hControl);

atLPSPIN_EXT_ITEM atSPINBOX_EXT_FindItem(atHCONTROL hControl, int nIndex);

void atSPINBOX_EXT_ClearAllItem(atHCONTROL hControl);

void * atSPINBOX_EXT_GetCurrItemData(atHCONTROL hControl);
int atSPINBOX_EXT_AddItemWithData(atHCONTROL hControl, int nIndex, atLPTSTR szText, void *data);


#ifdef __cplusplus
}
#endif


#endif // __SPINBOX_H_
