//
// MyControl.h
//
///////////////////////////////////////////////////////////////////////

#ifndef __CMP_SPINBOX_H_
#define __CMP_SPINBOX_H_


#ifndef __A_UIENG_H_
#include "A_UIENG.h"
#endif


#ifdef __cplusplus
extern "C" {
#endif


typedef struct _tagmtSPINDATA {
	int			nIndex;
	atTCHAR		szText[32];
	void *	data;
} atSPINITEM, * atLPSPINITEM;


atHCONTROL atSPINBOX_Create( int nIndex, int nX, int nY, int nWidth, int nHeight );
void atSPINBOX_Destroy( atHCONTROL hControl );
void atSPINBOX_Draw( atHGDC hCanvas, atHCONTROL hControl, atBOOL bFocused );

// 이미 존재하는 nIndex의 경우 내용만 바뀌게 됨(데이터를 메모리 생성하지 않음).
// 1: 새로생성, 0: 기존거에 업데이트
int atSPINBOX_AddItem(atHCONTROL hControl, int nIndex, atLPTSTR szText);

atBOOL atSPINBOX_SetCurrItem(atHCONTROL hControl, int nSpinBoxItemIndex);
int atSPINBOX_GetCurrItem( atHCONTROL hControl );

// added by jin, 2005-12-23
atLPTSTR atSPINBOX_GetCurrItemText(atHCONTROL hControl);

atLPSPINITEM atSPINBOX_FindItem(atHCONTROL hControl, int nIndex);

void atSPINBOX_ClearAllItem(atHCONTROL hControl);

void * atSPINBOX_GetCurrItemData(atHCONTROL hControl);
int atSPINBOX_AddItemWithData(atHCONTROL hControl, int nIndex, atLPTSTR szText, void *data);


#ifdef __cplusplus
}
#endif


#endif // __SPINBOX_H_
