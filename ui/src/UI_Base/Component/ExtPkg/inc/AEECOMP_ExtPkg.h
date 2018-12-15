#ifndef __AEECOMP_EXTPKG_H_
#define __AEECOMP_EXTPKG_H_


#ifndef __UI_TYPES_H_
#include "UI_Types.h"
#endif
#include "ExtPkg_Ctl.h"
#include "ExtPkg_Win.h"


#ifdef __cplusplus
extern "C" {
#endif


void AEECOMP_ExtPkg_Initialize(void);
void AEECOMP_ExtPkg_Finalize(void);

void AEECOMP_ExtPkg_User_Initialize(void);
void AEECOMP_ExtPkg_User_Finalize(void);


atHCONTROL ExtPkg_CreateTickerBox( atHWINDOW hWnd, int nIndex, int nX, int nY, int nWidth, int nHeight, 
								 atLPTSTR szText, atINT nLen, atINT nTickerInterval, atINT nTickerInc, atINT nTickerInitTick );
atHCONTROL ExtPkg_CreateCheckBox(atHWINDOW hWnd, int nIndex, int nX, int nY, int nWidth, int nHeight, atLPTSTR szText );
atHCONTROL ExtPkg_CreateButton(atHWINDOW hWnd, int nIndex, int nX, int nY, int nWidth, int nHeight, atLPTSTR szText);
atHCONTROL ExtPkg_CreateButton2(atHWINDOW hWnd, int nIndex, int nBY, int nWidth, int nHeight, atLPTSTR szText);
atHCONTROL ExtPkg_CreateImageButton(atHWINDOW hWnd, int nIndex, int nX, int nY, int nWidth, int nHeight, atHBITMAP lpNormalImage, atHBITMAP lpFocusedImage, atHBITMAP lpDisableImage );
atHCONTROL ExtPkg_CreateEditBox(atHWINDOW hWnd, int nIndex, int nX, int nY, int nWidth, int nHeight, atLPTSTR szText, atEDITBOX_EXT_INPUTMODE lstInputMode[], atINT nInputModeCount, atINT nCurInputMode );
atHCONTROL ExtPkg_CreateOICEditBox(atHWINDOW hWnd, int nIndex, int nX, int nY, int nWidth, int nHeight, atLPTSTR szText, atEDITBOX_EXT_INPUTMODE lstInputMode[], atINT nInputModeCount, atINT nCurInputMode );
atHCONTROL ExtPkg_CreateListBox( atHWINDOW hWnd, int nIndex, int nX, int nY, int nWidth, int nHeight, int nItemHeight, atBOOL bFirstNO, atINT nCheckType, atBOOL bTicker );
atHCONTROL ExtPkg_CreateListBox2( atHWINDOW hWnd, int nIndex, int nX, int nY, int nWidth, int nHeight, int nItemHeight, atBOOL bFirstNO, atINT nCheckType, atBOOL bTicker, atTCHAR (*szListMsg)[32], int nMaxCount, int nSelIndex );
atHCONTROL ExtPkg_CreateRadioButton(atHWINDOW hWnd, int nIndex, int nX, int nY, int nWidth, int nHeight, int nGroupID, atLPTSTR szText);
atHCONTROL ExtPkg_CreateRadioGroup(atHWINDOW hWnd, int nIndex, int nX, int nY, int nWidth, int nHeight, atTCHAR (*szButtonMsg)[32], int ButtonCnt, int init_idx, int item_height);							
atHCONTROL ExtPkg_CreateSpinBox(atHWINDOW hWnd, int nIndex, int nX, int nY, int nWidth, int nHeight);
atHCONTROL ExtPkg_CreateSpinBox2(atHWINDOW hWnd, int nIndex, int nX, int nY, int nWidth, int nHeight, atTCHAR (*szText)[32], int nMaxCount);
atHCONTROL ExtPkg_CreateScrollBar( atHWINDOW hWnd, int nIndex, int nX, int nY, int nWidth, int nHeight, int style);
atHCONTROL ExtPkg_CreateProgressBar(atHWINDOW hWnd, int nIndex, int nX, int nY, int nWidth, int nHeight, int nSize, int isText );
atHCONTROL ExtPkg_CreateVProgressBar(atHWINDOW hWnd, int nIndex, int nX, int nY, int nWidth, int nHeight, int nSize, int isText );
atHCONTROL ExtPkg_CreateScrollText(atHWINDOW	hWnd, int nIndex, int nX, int nY, int nWidth, int nHeight, int attr);
atHCONTROL ExtPkg_CreateHSlider( atHWINDOW hWnd, int nIndex, int nX, int nY, int nWidth, int nHeight, int max_pos, int pos);
atHCONTROL ExtPkg_CreateVSlider( atHWINDOW hWnd, int nIndex, int nX, int nY, int nWidth, int nHeight, int direction, int max_pos, int pos);
atHCONTROL ExtPkg_CreatePanel(atHWINDOW hWnd, int nIndex, int nX, int nY, int nWidth, int nHeight, atDWORD dwAttr );
atHCONTROL ExtPkg_CreateTransPanel(atHWINDOW hWnd, int nIndex, int nX, int nY, int nWidth, int nHeight, atDWORD dwAttr, atREAL rTransparent );
atHCONTROL ExtPkg_CreateScrollPanel(atHWINDOW hWnd, int nIndex, int nX, int nY, int nWidth, int nHeight, int nClientWidth, int nClientHeight, atDWORD dwAttr );

atHWINDOW ExtPkg_CreateListBoxWnd(atHWINDOW hParentWnd, int id, atLPTSTR szTitle, atTCHAR (*szInputMsg)[32], int max_cnt, int sel);
atHWINDOW ExtPkg_CreateFileListWnd(atHWINDOW hParentWnd, int id, atLPTSTR szTitle, atLPTSTR szPath, atLPTSTR szFilter, int max_cnt, atBOOL bCheckbox, atLPTSTR szOutput );
atHWINDOW ExtPkg_CreateHSliderWnd(atHWINDOW hParentWnd, int id, atLPTSTR szTitle, void *data, int max_pos, int pos);
atHWINDOW ExtPkg_CreateMsgBoxEx( atHWINDOW hParentWnd, int id, atLPTSTR szWindowText, atLPTSTR szMsg, int nMsgBoxType );
atHWINDOW ExtPkg_CreateMsgBoxEx2( atHWINDOW hParentWnd, int id, atLPTSTR szWindowText, atLPTSTR szMsg, int nMsgBoxType, long lWaitTime );
atHWINDOW ExtPkg_CreatePopupMenuWnd( atHWINDOW hParentWnd, int id, int x, int y, int width, int height, atTCHAR (*szInputMsg)[32], int menu_count, int view_count, int align );
atHWINDOW ExtPkg_CreateProgressWnd( atHWINDOW hParentWnd, int id, atLPTSTR szTitle, int size, int pos, atLPTSTR text, atLPTSTR button_caption );
atHWINDOW ExtPkg_CreateRadioGroupWnd(atHWINDOW hParentWnd, int id, atLPTSTR szTitle, atTCHAR (*szInputMsg)[32], int max_cnt, int init_value);
atHWINDOW ExtPkg_CreateTextViewWnd(atHWINDOW hParentWnd, int id, atLPTSTR szTitle, atLPTSTR szText );


#ifdef __cplusplus
}
#endif


#endif
