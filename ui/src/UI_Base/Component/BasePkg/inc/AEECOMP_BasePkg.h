#ifndef __BASEPKG_H_
#define __BASEPKG_H_


#ifndef __UI_TYPES_H_
#include "UI_Types.h"
#endif
#include "BasePkg_Ctl.h"
#include "BasePkg_Win.h"


#ifdef __cplusplus
extern "C" {
#endif


void AEECOMP_BasePkg_Initialize(void);
void AEECOMP_BasePkg_Finalize(void);


atHCONTROL  BasePkg_CreateDummyCtl(atHWINDOW hWnd, int nIndex, int nX, int nY, int nWidth, int nHeight);

atHCONTROL  BasePkg_CreateStaticBox(atHWINDOW	hWnd, int index, int nX, int nY, int nWidth, int nHeight, atLPTSTR szText );
atHCONTROL  BasePkg_CreateStaticBoxIDS(atHWINDOW	hWnd, int index, int nX, int nY, int nWidth, int nHeight, atULONG nTextIDS);
atHCONTROL  BasePkg_CreateTickerBox( atHWINDOW hWnd, int nIndex, int nX, int nY, int nWidth, int nHeight, 
								 atLPTSTR szText, atINT nLen, atINT nTickerInterval, atINT nTickerInc, atINT nTickerInitTick );
atHCONTROL	BasePkg_CreateImageBox(atHWINDOW	hWnd, int nIndex, int nX, int nY, int nWidth, int nHeight, atHBITMAP lpBM, atBOOL bOwnImage );
atHCONTROL	BasePkg_CreateImageBox2(atHWINDOW	hWnd, int nIndex, int nX, int nY, int nWidth, int nHeight, atLPTSTR filename );
atHCONTROL  BasePkg_CreateEditBox(atHWINDOW hWnd, int nIndex, int nX, int nY, int nWidth, int nHeight, atLPTSTR szText, atEDITBOX_INPUTMODE lstInputMode[], atINT nInputModeCount, atINT nCurInputMode );
atHCONTROL	BasePkg_CreateCheckBox(atHWINDOW hWnd, int nIndex, int nX, int nY, int nWidth, int nHeight, atLPTSTR szText);
atHCONTROL  BasePkg_CreateButton(atHWINDOW hWnd, int nIndex, int nX, int nY, int nWidth, int nHeight, atLPTSTR szText);
atHCONTROL  BasePkg_CreateButton2(atHWINDOW hWnd, int nIndex, int nBY, int nWidth, int nHeight, atLPTSTR szText);
atHCONTROL  BasePkg_CreateListBox( atHWINDOW hWnd, int nIndex, int nX, int nY, int nWidth, int nHeight, int nItemHeight, atBOOL bFirstNO, atINT nCheckType);
atHCONTROL  BasePkg_CreateListBox2( atHWINDOW hWnd, int nIndex, int nX, int nY, int nWidth, int nHeight, int nItemHeight, atBOOL bFirstNO, atINT nCheckType, atTCHAR (*szListMsg)[32], int nMaxCount);
atHCONTROL  BasePkg_CreateRadioButton(atHWINDOW hWnd, int nIndex, int nX, int nY, int nWidth, int nHeight, int nGroupID, atLPTSTR szText);
atHCONTROL  BasePkg_CreateRadioGroup(atHWINDOW hWnd, int nIndex, int nX, int nY, int nWidth, int nHeight, atTCHAR (*szButtonMsg)[32], int ButtonCnt, int init_idx, int item_height);							
atHCONTROL  BasePkg_CreateSpinBox(atHWINDOW hWnd, int nIndex, int nX, int nY, int nWidth, int nHeight);
atHCONTROL  BasePkg_CreateSpinBox2(atHWINDOW hWnd, int nIndex, int nX, int nY, int nWidth, int nHeight, atTCHAR (*szText)[32], int nMaxCount);
atHCONTROL  BasePkg_CreateScrollBar( atHWINDOW hWnd, int nIndex, int nX, int nY, int nWidth, int nHeight, int style, int nCursorX, int nCursorY, int nCursorLimit );
atHCONTROL  BasePkg_CreateProgressBar(atHWINDOW hWnd, int nIndex, int nX, int nY, int nWidth, int nHeight, int nSize, int isText );
atHCONTROL  BasePkg_CreateVProgressBar(atHWINDOW hWnd, int nIndex, int nX, int nY, int nWidth, int nHeight, int nSize, int isText );
atHCONTROL  BasePkg_CreateHSlider( atHWINDOW hWnd, int nIndex, int nX, int nY, int nWidth, int nHeight, int max_pos, int pos);
atHCONTROL  BasePkg_CreateVSlider( atHWINDOW hWnd, int nIndex, int nX, int nY, int nWidth, int nHeight, int direction, int max_pos, int pos);
atHCONTROL  BasePkg_CreatePanel( atHWINDOW hWnd, int nIndex, int nX, int nY, int nWidth, int nHeight, int nBorderWidth );
atHCONTROL  BasePkg_CreatePaintBox( atHWINDOW hWnd, int nIndex, int nX, int nY, int nWidth, int nHeight, atLPPAINTBOX_RENDERER lpRenderer );

atHWINDOW	BasePkg_CreateMemoryInfoWnd( atHWINDOW hParentWnd );
atHWINDOW	BasePkg_CreateWindowInfoWnd( atHWINDOW hParentWnd );
atHWINDOW	BasePkg_CreateMsgBox( atHWINDOW hParentWnd, int id, atLPTSTR szWindowText, atLPTSTR szMsg, int nMsgBoxType );
atHWINDOW	BasePkg_CreateMsgBox2( atHWINDOW hParentWnd, int id, atLPTSTR szWindowText, atLPTSTR szMsg, int nMsgBoxType, long lWaitTime );
atHWINDOW	BasePkg_CreateEditBoxWnd(atHWINDOW hParentWnd, int id, atLPTSTR szTitle, atLPTSTR szMsg, atLPTSTR szText );


#ifdef __cplusplus
}
#endif


#endif
