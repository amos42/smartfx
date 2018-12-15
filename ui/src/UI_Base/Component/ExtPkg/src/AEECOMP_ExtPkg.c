#include "AEECOMP_ExtPkg.h"


#include "CustRes/ExtPkg_ResDump.h"
//#include "User_ResDump.h"


typedef struct _GLOBALCTLIMG {
	atHBITMAP		lpNormalBtnL;
	atHBITMAP		lpNormalBtnC;
	atHBITMAP		lpNormalBtnR;
	atHBITMAP		lpFocusedBtnL;
	atHBITMAP		lpFocusedBtnC;
	atHBITMAP		lpFocusedBtnR;
	atHBITMAP		lpDisabledBtnL;
	atHBITMAP		lpDisabledBtnC;
	atHBITMAP		lpDisabledBtnR;

    atHBITMAP		hImgListItemNormalLeft;
	atHBITMAP		hImgListItemNormalCenter;
	atHBITMAP		hImgListItemNormalRight;
    atHBITMAP		hImgListItemSelectLeft;
	atHBITMAP		hImgListItemSelectCenter;
	atHBITMAP		hImgListItemSelectRight;
    atHBITMAP		hImgListItemDisableLeft;
	atHBITMAP		hImgListItemDisableCenter;
	atHBITMAP		hImgListItemDisableRight;

	atHBITMAP		lpScrollBarTop;
	atHBITMAP		lpScrollBar;
	atHBITMAP		lpScrollBarBottom;
	//atHBITMAP		lpScrollCursor;
	atHBITMAP		lpScrollCursorTop;
	atHBITMAP		lpScrollCursorMid;
	atHBITMAP		lpScrollCursorBot;

	atHBITMAP		lpSpinStartXOFF;
	atHBITMAP		lpSpinStartXON;
	atHBITMAP		lpSpinEndXOFF;
	atHBITMAP		lpSpinEndXON;
	atHBITMAP		lpSpinNormalBG;
	atHBITMAP		lpSpinFocusBG;


	atHBITMAP		lpCheckBoxOFF;
	atHBITMAP		lpCheckBoxON;

	atHBITMAP		lpRadioOff;
	atHBITMAP		lpRadioOn;	

	atHBITMAP		lpNormalEdtL;
	atHBITMAP		lpNormalEdtC;
	atHBITMAP		lpNormalEdtR;
	atHBITMAP		lpFocusedEdtL;
	atHBITMAP		lpFocusedEdtC;
	atHBITMAP		lpFocusedEdtR;
	atHBITMAP		lpDisabledEdtL;
	atHBITMAP		lpDisabledEdtC;
	atHBITMAP		lpDisabledEdtR;
	
	atHBITMAP		lpProgressBarLeft;
	atHBITMAP		lpProgressBarCenter;
	atHBITMAP		lpProgressBarRight;
	atHBITMAP		lpProgressMeasureLeft;
	atHBITMAP		lpProgressMeasureCenter;
	atHBITMAP		lpProgressMeasureRight;

	atHBITMAP		lpVProgressBarTop;
	atHBITMAP		lpVProgressBarMiddle;
	atHBITMAP		lpVProgressBarBottom;
	atHBITMAP		lpVProgressMeasureTop;
	atHBITMAP		lpVProgressMeasureMiddle;
	atHBITMAP		lpVProgressMeasureBottom;

	atHBITMAP		lpMenuArrowOFF;
	atHBITMAP		lpMenuArrowON;

    atHBITMAP      lpHSliderLeft;
    atHBITMAP      lpHSliderCenter;
    atHBITMAP      lpHSliderRight;
    atHBITMAP      lpHSliderTick;
    atHBITMAP      lpHSliderTickFocus;
    atHBITMAP      lpVSliderTop;
    atHBITMAP      lpVSliderMiddle;
    atHBITMAP      lpVSliderBottom;
    atHBITMAP      lpVSliderTick;
    atHBITMAP      lpVSliderTickFocus;
	
    atHBITMAP      lpTab3On;
    atHBITMAP      lpTab3Off;    
    atHBITMAP      lpTabNext;
    atHBITMAP      lpTabPrevious;
    atHBITMAP      lpBullet;
    
	  atLAYOUTBITMAP   lpPanelLayout;
} GLOBALCTLIMG;


typedef struct _GLOBALWINIMG {
	atLAYOUTBITMAP   lpWinLayout[2];
	atLAYOUTBITMAP   lpPopupLayout[4];
} GLOBALWINIMG;

static GLOBALCTLIMG		g_CtlImages;  
static GLOBALWINIMG		g_WinImages;



atHCONTROL ExtPkg_CreateRadioButton(atHWINDOW hWnd, int nIndex, int nX, int nY, int nWidth, int nHeight, int nGroupID, atLPTSTR szText ) 
{
    atHCONTROL hControl = atRADIOBUTTON_EXT_Create( nIndex, nX, nY, nWidth, nHeight, nGroupID, szText,
                                                g_CtlImages.lpRadioOff, g_CtlImages.lpRadioOn );
    if( hWnd ) atWINDOW_AddControl( hWnd, hControl );

    return hControl;
}

atHCONTROL ExtPkg_CreateButton(atHWINDOW hWnd, int nIndex, int nX, int nY, int nWidth, int nHeight, atLPTSTR szText)
{
    atHCONTROL hControl = atBUTTON_EXT_Create( nIndex, nX, nY, nWidth, nHeight, szText, 
											g_CtlImages.lpNormalBtnL, g_CtlImages.lpNormalBtnC, g_CtlImages.lpNormalBtnR,
											atNULL, atNULL, atNULL,
											g_CtlImages.lpFocusedBtnL, g_CtlImages.lpFocusedBtnC, g_CtlImages.lpFocusedBtnR,
											g_CtlImages.lpDisabledBtnL, g_CtlImages.lpDisabledBtnC, g_CtlImages.lpDisabledBtnR );
    if( hWnd ) atWINDOW_AddControl( hWnd, hControl );

    return hControl;
}

atHCONTROL ExtPkg_CreateButton2(atHWINDOW hWnd, int nIndex, int nBY, int nWidth, int nHeight, atLPTSTR szText)
{
	atRECT rect;
	int x, y;

	atWINDOW_GetClientRect(hWnd, &rect);
	x = (RECT_WIDTH(rect) - nWidth) / 2;
	if( nBY >= 0 ) 
		y = nBY;
	else
		y = RECT_HEIGHT(rect) - nHeight + nBY;
	
    return ExtPkg_CreateButton( hWnd, nIndex, x, y, nWidth, nHeight, szText );
}


atHCONTROL ExtPkg_CreateImageButton(atHWINDOW hWnd, int nIndex, int nX, int nY, int nWidth, int nHeight, atHBITMAP lpNormalImage, atHBITMAP lpFocusedImage, atHBITMAP lpDisableImage )
{
    atHCONTROL hControl = atIMAGEBUTTON_Create( nIndex, nX, nY, nWidth, nHeight, 
												0, 0, 0, 0,
												lpNormalImage, atNULL, lpFocusedImage, lpDisableImage );
    if( hWnd ) atWINDOW_AddControl( hWnd, hControl );

    return hControl;
}


atHCONTROL ExtPkg_CreateEditBox(atHWINDOW hWnd, int nIndex, int nX, int nY, int nWidth, int nHeight, atLPTSTR szText, atEDITBOX_EXT_INPUTMODE lstInputMode[], atINT nInputModeCount, atINT nCurInputMode )
{
    atHCONTROL hControl = atEDITBOX_EXT_Create( nIndex, nX, nY, nWidth, nHeight, 
										    szText,
										    atFALSE,
											lstInputMode, nInputModeCount, nCurInputMode,
                                            g_CtlImages.lpNormalEdtL, g_CtlImages.lpNormalEdtC, g_CtlImages.lpNormalEdtR,
										    g_CtlImages.lpFocusedEdtL, g_CtlImages.lpFocusedEdtC, g_CtlImages.lpFocusedEdtR,
										    g_CtlImages.lpDisabledEdtL, g_CtlImages.lpDisabledEdtC, g_CtlImages.lpDisabledEdtR );
    if( hWnd ) atWINDOW_AddControl( hWnd, hControl );

    return hControl;
}

atHCONTROL ExtPkg_CreateOICEditBox(atHWINDOW hWnd, int nIndex, int nX, int nY, int nWidth, int nHeight, atLPTSTR szText, atEDITBOX_EXT_INPUTMODE lstInputMode[], atINT nInputModeCount, atINT nCurInputMode )
{
    atHCONTROL hControl = atEDITBOX_EXT_Create( nIndex, nX, nY, nWidth, nHeight, 
										    szText,
										    atTRUE,
											lstInputMode, nInputModeCount, nCurInputMode,
                                            g_CtlImages.lpNormalEdtL, g_CtlImages.lpNormalEdtC, g_CtlImages.lpNormalEdtR,
										    g_CtlImages.lpFocusedEdtL, g_CtlImages.lpFocusedEdtC, g_CtlImages.lpFocusedEdtR,
										    g_CtlImages.lpDisabledEdtL, g_CtlImages.lpDisabledEdtC, g_CtlImages.lpDisabledEdtR );
    if( hWnd ) atWINDOW_AddControl( hWnd, hControl );

    return hControl;
}

atHCONTROL  ExtPkg_CreateSpinBox(atHWINDOW hWnd, int nIndex, int nX, int nY, int nWidth, int nHeight)
{
    atHCONTROL hControl = atSPINBOX_EXT_Create( nIndex, nX, nY, nWidth, nHeight, 
                                            g_CtlImages.lpSpinStartXOFF, g_CtlImages.lpSpinEndXOFF, g_CtlImages.lpSpinNormalBG,
                                            g_CtlImages.lpSpinStartXON, g_CtlImages.lpSpinEndXON, g_CtlImages.lpSpinFocusBG);
    if( hWnd ) atWINDOW_AddControl( hWnd, hControl );

    return hControl;
}

atHCONTROL  ExtPkg_CreateSpinBox2(atHWINDOW hWnd, int nIndex, int nX, int nY, int nWidth, int nHeight, atTCHAR (*szText)[32], int nMaxCount)
{
    int i;
    atHCONTROL hControl = ExtPkg_CreateSpinBox(hWnd, nIndex, nX, nY, nWidth, nHeight);
    
    for (i=0; i<nMaxCount; i++)
    {
        atSPINBOX_EXT_AddItem(hControl, i, szText[i]);
    }

    return hControl;
}

/*
atHCONTROL  ExtPkg_CreateEditBox(atHWINDOW hWnd, int nIndex, int nX, int nY, int nWidth, int nHeight, INPUTMODESET *lpInputModeSet)
{
    atHCONTROL hControl = atEDITBOX_Create( hWnd, nIndex, nX, nY, nWidth, nHeight, lpInputModeSet );

    if( hWnd ) atWINDOW_AddControl( hWnd, hControl );

    return hControl;
}
*/

atHCONTROL  ExtPkg_CreateCheckBox(atHWINDOW hWnd, int nIndex, int nX, int nY, int nWidth, int nHeight, atLPTSTR szText)
{
    atHCONTROL hControl = atCHECKBOX_EXT_Create( nIndex, nX, nY, nWidth, nHeight, szText, atFALSE,
                                            g_CtlImages.lpCheckBoxOFF, g_CtlImages.lpCheckBoxON );
    if( hWnd ) atWINDOW_AddControl( hWnd, hControl );
                
    return hControl;
}

atHCONTROL  ExtPkg_CreateTickerBox( atHWINDOW hWnd, int nIndex, int nX, int nY, int nWidth, int nHeight, 
								 atLPTSTR szText, atINT nLen, atINT nTickerInterval, atINT nTickerInc, atINT nTickerInitTick )
{
    atHCONTROL hControl = atTICKERBOX_Create( nIndex, nX, nY, nWidth, nHeight, szText, nLen, nTickerInterval, nTickerInc, nTickerInitTick, 1, 1, atNULL, atNULL, atNULL );
	
    if( hWnd ) atWINDOW_AddControl( hWnd, hControl );

    return hControl;
}

atHCONTROL ExtPkg_CreateListBox( atHWINDOW hWnd, int nIndex, int nX, int nY, int nWidth, int nHeight, int nItemHeight, atBOOL bFirstNO, atINT nCheckType,  atBOOL bTicker )
{
    atHCONTROL  hControl;
	
    hControl = atLISTBOX_EXT_Create( nIndex, nX, nY, nWidth, nHeight,
                                				nItemHeight, 4, 4, 4,
                                				bFirstNO, nCheckType, bTicker, 
                                				g_CtlImages.hImgListItemNormalLeft, g_CtlImages.hImgListItemNormalCenter, g_CtlImages.hImgListItemNormalRight,
                                				g_CtlImages.hImgListItemSelectLeft, g_CtlImages.hImgListItemSelectCenter, g_CtlImages.hImgListItemSelectRight,
                                				g_CtlImages.hImgListItemDisableLeft, g_CtlImages.hImgListItemDisableCenter, g_CtlImages.hImgListItemDisableRight,
                                				g_CtlImages.lpCheckBoxOFF, g_CtlImages.lpCheckBoxON,
												g_CtlImages.lpScrollBarTop, g_CtlImages.lpScrollBar, g_CtlImages.lpScrollBarBottom, 
												//g_CtlImages.lpScrollCursor );
												g_CtlImages.lpScrollCursorTop, g_CtlImages.lpScrollCursorMid, g_CtlImages.lpScrollCursorBot );
										
    if( hWnd ) atWINDOW_AddControl( hWnd, hControl );

    return hControl;
}

atHCONTROL ExtPkg_CreateListBox2( atHWINDOW hWnd, int nIndex, int nX, int nY, int nWidth, int nHeight, int nItemHeight, atBOOL bFirstNO, atINT nCheckType, atBOOL bTicker, atTCHAR (*szListMsg)[32], int nMaxCount, int nSelIndex )
{
    int i;
    atHCONTROL  hControl = ExtPkg_CreateListBox( hWnd, nIndex, nX, nY, nWidth, nHeight,
                                            nItemHeight, bFirstNO, nCheckType, bTicker );

//	atLPLISTBOX_EXT lpListBox = (atLPLISTBOX_EXT)atCONTROL_GetDataPtr( hControl );
//	if (!lpListBox) return atNULL;

    for( i = 0; i < nMaxCount; i ++ )
    {
        atLISTBOX_EXT_Start_AddItem(hControl);
        if(bFirstNO) atLISTBOX_EXT_AddItem_NO(hControl, i+1);
        atLISTBOX_EXT_AddItem_String(hControl, szListMsg[i], 0 );
        atLISTBOX_EXT_Update_AddItem(hControl);

//		if (nSelIndex == i)
//			lpListBox->nSelectIndex = nSelIndex;
  }

	atLISTBOX_EXT_SetSelectIndex( hControl, nSelIndex );
    
    return hControl;
}

atHCONTROL ExtPkg_CreateScrollBar( atHWINDOW hWnd, int nIndex, int nX, int nY, int nWidth, int nHeight, int style)
{
    atHCONTROL  hControl = atSCROLLBAR_EXT_Create( nIndex, nX, nY, nWidth, nHeight, (atSCROLLBAR_EXT_TYPE)style,
                                            0, 0, 0, 0, 
                                            //g_CtlImages.lpScrollBarTop, g_CtlImages.lpScrollBar, g_CtlImages.lpScrollBarBottom, g_CtlImages.lpScrollCursor );
											g_CtlImages.lpScrollBarTop, g_CtlImages.lpScrollBar, g_CtlImages.lpScrollBarBottom,
											g_CtlImages.lpScrollCursorTop, g_CtlImages.lpScrollCursorMid, g_CtlImages.lpScrollCursorBot );
    if( hWnd ) atWINDOW_AddControl( hWnd, hControl );

    return hControl;
}

atHCONTROL ExtPkg_CreateRadioGroup(atHWINDOW hWnd, int nIndex, int nX, int nY, int nWidth, int nHeight, 
                            atTCHAR (*szButtonMsg)[32], int ButtonCnt, int init_idx, int item_height )
{
    atHCONTROL hControl = atRADIOGROUP_EXT_Create( nIndex, nX, nY, nWidth, nHeight, szButtonMsg, ButtonCnt, init_idx, item_height,
							1, 2, 2,
                                                g_CtlImages.lpRadioOff, g_CtlImages.lpRadioOn,
						      //g_CtlImages.lpScrollBarTop, g_CtlImages.lpScrollBar, g_CtlImages.lpScrollBarBottom, g_CtlImages.lpScrollCursor );
							  g_CtlImages.lpScrollBarTop, g_CtlImages.lpScrollBar, g_CtlImages.lpScrollBarBottom,
							  g_CtlImages.lpScrollCursorTop, g_CtlImages.lpScrollCursorMid, g_CtlImages.lpScrollCursorBot );
    if( hWnd ) atWINDOW_AddControl( hWnd, hControl );

    return hControl;
}


atHCONTROL ExtPkg_CreateProgressBar(atHWINDOW hWnd, int nIndex, int nX, int nY, int nWidth, int nHeight, int nSize, int isText ) 
{
    atHCONTROL hControl = atPROGRESSBAR_EXT_Create( nIndex, nX, nY, nWidth, nHeight, isText,
                                                g_CtlImages.lpProgressBarLeft, g_CtlImages.lpProgressBarCenter, g_CtlImages.lpProgressBarRight,
                                                g_CtlImages.lpProgressMeasureLeft, g_CtlImages.lpProgressMeasureCenter, g_CtlImages.lpProgressMeasureRight,
                                                2, 2, 0 );
    atPROGRESSBAR_EXT_SetSize( hControl, nSize );

    if( hWnd ) atWINDOW_AddControl( hWnd, hControl );

    return hControl;
}

atHCONTROL ExtPkg_CreateVProgressBar(atHWINDOW hWnd, int nIndex, int nX, int nY, int nWidth, int nHeight, int nSize, int isText ) 
{
    atHCONTROL hControl = atVPROGRESSBAR_EXT_Create( nIndex, nX, nY, nWidth, nHeight, isText, 
                                                g_CtlImages.lpVProgressBarTop, g_CtlImages.lpVProgressBarMiddle, g_CtlImages.lpVProgressBarBottom,
                                                g_CtlImages.lpVProgressMeasureTop, g_CtlImages.lpVProgressMeasureMiddle, g_CtlImages.lpVProgressMeasureBottom,
                                                2, 2, 0 );
    atVPROGRESSBAR_EXT_SetSize( hControl, nSize );

    if( hWnd ) atWINDOW_AddControl( hWnd, hControl );

    return hControl;
}

atHCONTROL  ExtPkg_CreateScrollText(atHWINDOW  hWnd, int nIndex, int nX, int nY, int nWidth, int nHeight, int attr)
{
    atHCONTROL hControl = atSCROLLTEXT_Create( nIndex, nX, nY, nWidth, nHeight, attr, 
											  //g_CtlImages.lpScrollBarTop, g_CtlImages.lpScrollBar, g_CtlImages.lpScrollBarBottom, g_CtlImages.lpScrollCursor);
											  g_CtlImages.lpScrollBarTop, g_CtlImages.lpScrollBar, g_CtlImages.lpScrollBarBottom,
												g_CtlImages.lpScrollCursorTop, g_CtlImages.lpScrollCursorMid, g_CtlImages.lpScrollCursorBot );
    if( hWnd ) atWINDOW_AddControl( hWnd, hControl );

    return hControl;
}

atHCONTROL ExtPkg_CreateHSlider( atHWINDOW hWnd, int nIndex, int nX, int nY, int nWidth, int nHeight, int max_pos, int pos )
{
	atHCONTROL hControl = atHSLIDER_EXT_Create( nIndex, nX, nY, nWidth, nHeight, 
									 		g_CtlImages.lpHSliderLeft, g_CtlImages.lpHSliderCenter, g_CtlImages.lpHSliderRight, 
									 		g_CtlImages.lpHSliderTick, g_CtlImages.lpHSliderTickFocus, g_CtlImages.lpHSliderTick, 
									 		0, 0, 0 );
    atHSLIDER_EXT_SetMaxPos( hControl, max_pos );
    atHSLIDER_EXT_SetPosition( hControl, pos );
	
    if( hWnd ) atWINDOW_AddControl( hWnd, hControl );

    return hControl;
}

atHCONTROL ExtPkg_CreateVSlider( atHWINDOW hWnd, int nIndex, int nX, int nY, int nWidth, int nHeight, int direction, int max_pos, int pos )
{
	atHCONTROL hControl = atVSLIDER_EXT_Create( nIndex, nX, nY, nWidth, nHeight, direction,
									 		g_CtlImages.lpVSliderTop, g_CtlImages.lpVSliderMiddle, g_CtlImages.lpVSliderBottom, 
									 		g_CtlImages.lpVSliderTick, g_CtlImages.lpVSliderTickFocus, g_CtlImages.lpVSliderTick, 
									 		0, 0, 0 );
    atVSLIDER_EXT_SetMaxPos( hControl, max_pos );
    atVSLIDER_EXT_SetPosition( hControl, pos );
	
    if( hWnd ) atWINDOW_AddControl( hWnd, hControl );

    return hControl;
}


atHCONTROL ExtPkg_CreatePanel(atHWINDOW hWnd, int nIndex, int nX, int nY, int nWidth, int nHeight, atDWORD dwAttr ) 
{
    atHCONTROL hControl = atIMAGEPANEL_Create( nIndex, nX, nY, nWidth, nHeight, dwAttr,
    																					 g_CtlImages.lpPanelLayout.bmTopLeft, g_CtlImages.lpPanelLayout.bmTopCenter, g_CtlImages.lpPanelLayout.bmTopRight,
    																					 g_CtlImages.lpPanelLayout.bmMiddleLeft, g_CtlImages.lpPanelLayout.bmMiddleCenter, g_CtlImages.lpPanelLayout.bmMiddleRight,
    																					 g_CtlImages.lpPanelLayout.bmBottomLeft, g_CtlImages.lpPanelLayout.bmBottomCenter, g_CtlImages.lpPanelLayout.bmBottomRight );
    if( hWnd ) atWINDOW_AddControl( hWnd, hControl );

    return hControl;
}

atHCONTROL ExtPkg_CreateTransPanel(atHWINDOW hWnd, int nIndex, int nX, int nY, int nWidth, int nHeight, atDWORD dwAttr, atREAL rTransparent ) 
{
    atHCONTROL hControl = atTRANSPANEL_Create( nIndex, nX, nY, nWidth, nHeight, dwAttr, rTransparent );
    if( hWnd ) atWINDOW_AddControl( hWnd, hControl );

    return hControl;
}

atHCONTROL ExtPkg_CreateScrollPanel(atHWINDOW hWnd, int nIndex, int nX, int nY, int nWidth, int nHeight, int nClientWidth, int nClientHeight, atDWORD dwAttr ) 
{
    atHCONTROL hControl = atSCROLLPANEL_Create( nIndex, nX, nY, nWidth, nHeight, nClientWidth, nClientHeight, dwAttr,
												g_CtlImages.lpScrollBarTop, g_CtlImages.lpScrollBar, g_CtlImages.lpScrollBarBottom, 
												//g_CtlImages.lpScrollCursor );
												g_CtlImages.lpScrollCursorTop, g_CtlImages.lpScrollCursorMid, g_CtlImages.lpScrollCursorBot );
    if( hWnd ) atWINDOW_AddControl( hWnd, hControl );

    return hControl;
}


atHWINDOW	ExtPkg_CreateListBoxWnd( atHWINDOW hParentWnd, int id, atLPTSTR szTitle, atTCHAR (*szInputMsg)[32], int max_cnt, int sel )
{
	return CreateListBoxWnd( hParentWnd, id, szTitle, szInputMsg, max_cnt, sel );
}

atHWINDOW ExtPkg_CreateFileListWnd( atHWINDOW hParentWnd, int id, atLPTSTR szTitle, atLPTSTR szPath, atLPTSTR szFilter, int max_cnt, atBOOL bCheckbox, atLPTSTR szOutput )
{
	return CreateFileListWnd( hParentWnd, id, szTitle, szPath, szFilter, max_cnt, bCheckbox, szOutput );
}

atHWINDOW ExtPkg_CreateHSliderWnd( atHWINDOW hParentWnd, int id, atLPTSTR szTitle, void *data, int max_pos, int pos )
{
	return CreateHSliderWnd( hParentWnd, id, szTitle, data, max_pos, pos);
}

atHWINDOW ExtPkg_CreateMsgBoxEx( atHWINDOW hParentWnd, int id, atLPTSTR szWindowText, atLPTSTR szMsg, int nMsgBoxType )
{
	return CreateMsgBoxEx( hParentWnd, id, szWindowText, szMsg, nMsgBoxType );
}

atHWINDOW ExtPkg_CreateMsgBoxEx2( atHWINDOW hParentWnd, int id, atLPTSTR szWindowText, atLPTSTR szMsg, int nMsgBoxType, long lWaitTime )
{
	return CreateMsgBoxEx2( hParentWnd, id, szWindowText, szMsg, nMsgBoxType, lWaitTime );
}

atHWINDOW ExtPkg_CreatePopupMenuWnd( atHWINDOW hParentWnd, int id, int x, int y, int width, int height, atTCHAR (*szInputMsg)[32], int menu_count, int view_count, int align )
{
	return CreatePopupMenuWnd( hParentWnd, id, x, y, width, height, 20, szInputMsg, menu_count, view_count, -1, align, atNULL );
}

atHWINDOW ExtPkg_CreateProgressWnd( atHWINDOW hParentWnd, int id, atLPTSTR szTitle, int size, int pos, atLPTSTR text, atLPTSTR button_caption )
{
	return CreateProgressWnd( hParentWnd, id, szTitle, size, pos, text, button_caption );
}

atHWINDOW ExtPkg_CreateRadioGroupWnd(atHWINDOW hParentWnd, int id, atLPTSTR szTitle, atTCHAR (*szInputMsg)[32], int max_cnt, int init_value)
{
	return CreateRadioGroupWnd( hParentWnd, id, szTitle, szInputMsg, max_cnt, init_value);
}

atHWINDOW	ExtPkg_CreateEditBoxWnd(atHWINDOW hParentWnd, int id, atLPTSTR szTitle, atLPTSTR szMsg, atLPTSTR szText, atBOOL bUseOIC )
{
	return CreateEditBoxExWnd( hParentWnd, id, szTitle, szMsg, szText, bUseOIC );
}

atHWINDOW ExtPkg_CreateTextViewWnd(atHWINDOW hParentWnd, int id, atLPTSTR szTitle, atLPTSTR szText )
{
	return CreateTextViewWnd( hParentWnd, id, szTitle, szText );
}


static atHBITMAP loadimg2( atHRESSRC hResSrc, atLPCTSTR filename, atBOOL isTrans )
{
	atHBITMAP hbm = atUIAPI_CreateBitmapFromRes( hResSrc, filename );
	if( hbm == atNULL ) return atNULL;
	if(isTrans) atUIAPI_SetBitmapTransColor( hbm, atUI_COLOR_TRANS );
	return hbm;
}

static atHBITMAP loadimg( atHRESSRC hResSrc, atLPCTSTR filename )
{
	return loadimg2( hResSrc, filename, atFALSE );
}

static void  MyControl_Init( atHRESSRC hResSrc )
{
    atUIAPI_memset( &g_CtlImages, 0, sizeof(g_CtlImages) );

    g_CtlImages.lpScrollBarTop         = loadimg( hResSrc, _AT("scroll_bar.img") );
    g_CtlImages.lpScrollBar         = loadimg( hResSrc, _AT("scroll_bar.img") );
    g_CtlImages.lpScrollBarBottom         = loadimg( hResSrc, _AT("scroll_bar.img") );
    //g_CtlImages.lpScrollCursor      = loadimg( hResSrc, _AT("scroll_cursor.img") );    

    g_CtlImages.lpNormalBtnL         = loadimg( hResSrc, _AT("btn_off_l.img") );
    g_CtlImages.lpNormalBtnC         = loadimg( hResSrc, _AT("btn_off_c.img") );
    g_CtlImages.lpNormalBtnR         = loadimg( hResSrc, _AT("btn_off_r.img") );
    g_CtlImages.lpFocusedBtnL        = loadimg( hResSrc, _AT("btn_on_l.img") );
    g_CtlImages.lpFocusedBtnC        = loadimg( hResSrc, _AT("btn_on_c.img") );
    g_CtlImages.lpFocusedBtnR        = loadimg( hResSrc, _AT("btn_on_r.img") );
    g_CtlImages.lpDisabledBtnL        = loadimg( hResSrc, _AT("btn_dis_l.img") );
    g_CtlImages.lpDisabledBtnC        = loadimg( hResSrc, _AT("btn_dis_c.img") );
    g_CtlImages.lpDisabledBtnR        = loadimg( hResSrc, _AT("btn_dis_r.img") );

    g_CtlImages.hImgListItemNormalLeft = atNULL;
    g_CtlImages.hImgListItemNormalCenter = atNULL;
    g_CtlImages.hImgListItemNormalRight = atNULL;
    g_CtlImages.hImgListItemSelectLeft = atNULL;
    g_CtlImages.hImgListItemSelectCenter = atNULL;
    g_CtlImages.hImgListItemSelectRight = atNULL;
    g_CtlImages.hImgListItemDisableLeft = atNULL;
    g_CtlImages.hImgListItemDisableCenter = atNULL;
    g_CtlImages.hImgListItemDisableRight = atNULL;
	
    g_CtlImages.lpNormalEdtL         = atNULL;
    g_CtlImages.lpNormalEdtC         = atNULL;
    g_CtlImages.lpNormalEdtR         = atNULL;
    g_CtlImages.lpFocusedEdtL        = atNULL;
    g_CtlImages.lpFocusedEdtC        = atNULL;
    g_CtlImages.lpFocusedEdtR        = atNULL;
    g_CtlImages.lpDisabledEdtL        = atNULL;
    g_CtlImages.lpDisabledEdtC        = atNULL;
    g_CtlImages.lpDisabledEdtR        = atNULL;

    g_CtlImages.lpSpinStartXOFF       = loadimg2( hResSrc, _AT("spin_left_off.img"), atTRUE );    
    g_CtlImages.lpSpinStartXON        = loadimg2( hResSrc, _AT("spin_left_on.img"), atTRUE ); 
    g_CtlImages.lpSpinEndXOFF      = loadimg2( hResSrc, _AT("spin_right_off.img"), atTRUE );   
    g_CtlImages.lpSpinEndXON       = loadimg2( hResSrc, _AT("spin_right_on.img"), atTRUE );    

    g_CtlImages.lpCheckBoxOFF       = loadimg( hResSrc, _AT("checkbox_off.img") );
    g_CtlImages.lpCheckBoxON        = loadimg( hResSrc, _AT("checkbox_on.img") );

    g_CtlImages.lpRadioOff          = loadimg2( hResSrc, _AT("radio_off.img"), atTRUE );    
    g_CtlImages.lpRadioOn           = loadimg2( hResSrc, _AT("radio_on.img"), atTRUE ); 

    g_CtlImages.lpMenuArrowOFF      = loadimg2( hResSrc, _AT("menu_arrow_off.img"), atTRUE );    
    g_CtlImages.lpMenuArrowON       = loadimg2( hResSrc, _AT("menu_arrow_on.img"), atTRUE ); 

    g_CtlImages.lpProgressBarLeft       = loadimg( hResSrc, _AT("progress_l.img") );
    g_CtlImages.lpProgressBarCenter       = loadimg( hResSrc, _AT("progress_c.img") );
    g_CtlImages.lpProgressBarRight       = loadimg( hResSrc, _AT("progress_r.img") );
    g_CtlImages.lpProgressMeasureLeft   = loadimg( hResSrc, _AT("progress_measure.img") );
    g_CtlImages.lpProgressMeasureCenter   = loadimg( hResSrc, _AT("progress_measure.img") );
    g_CtlImages.lpProgressMeasureRight   = loadimg( hResSrc, _AT("progress_measure.img") );

    g_CtlImages.lpVProgressBarTop       = loadimg( hResSrc, _AT("vprogress_t.img") );
    g_CtlImages.lpVProgressBarMiddle       = loadimg( hResSrc, _AT("vprogress_m.img") );
    g_CtlImages.lpVProgressBarBottom       = loadimg( hResSrc, _AT("vprogress_b.img") );
    g_CtlImages.lpVProgressMeasureTop   = loadimg( hResSrc, _AT("vprogress_measure.img") );
    g_CtlImages.lpVProgressMeasureMiddle   = loadimg( hResSrc, _AT("vprogress_measure.img") );
    g_CtlImages.lpVProgressMeasureBottom   = loadimg( hResSrc, _AT("vprogress_measure.img") );

	g_CtlImages.lpHSliderLeft			= loadimg2( hResSrc, _AT("hslider_l.img"), atTRUE );
	g_CtlImages.lpHSliderCenter		= loadimg2( hResSrc, _AT("hslider_c.img"), atTRUE );
	g_CtlImages.lpHSliderRight			= loadimg2( hResSrc, _AT("hslider_r.img"), atTRUE );
	g_CtlImages.lpHSliderTick			= loadimg2( hResSrc, _AT("hslider_tick.img"), atTRUE );
	g_CtlImages.lpHSliderTickFocus		= loadimg2( hResSrc, _AT("hslider_tick_f.img"), atTRUE );

	g_CtlImages.lpVSliderTop			= loadimg2( hResSrc, _AT("vslider_t.img"), atTRUE );
	g_CtlImages.lpVSliderMiddle		= loadimg2( hResSrc, _AT("vslider_m.img"), atTRUE );
	g_CtlImages.lpVSliderBottom 		= loadimg2( hResSrc, _AT("vslider_b.img"), atTRUE );
	g_CtlImages.lpVSliderTick			= loadimg2( hResSrc, _AT("vslider_tick.img"), atTRUE );
	g_CtlImages.lpVSliderTickFocus 	= loadimg2( hResSrc, _AT("vslider_tick_f.img"), atTRUE );
	
    g_CtlImages.lpPanelLayout.bmTopLeft      = loadimg( hResSrc, _AT("pop_lst_tl.img") );
    g_CtlImages.lpPanelLayout.bmTopCenter    = loadimg( hResSrc, _AT("pop_lst_tc.img") );
    g_CtlImages.lpPanelLayout.bmTopRight     = loadimg( hResSrc, _AT("pop_lst_tr.img") );
    g_CtlImages.lpPanelLayout.bmMiddleLeft   = loadimg( hResSrc, _AT("pop_lst_ml.img") );
    g_CtlImages.lpPanelLayout.bmMiddleCenter = loadimg( hResSrc, _AT("pop_lst_mc.img") );
    g_CtlImages.lpPanelLayout.bmMiddleRight  = loadimg( hResSrc, _AT("pop_lst_mr.img") );
    g_CtlImages.lpPanelLayout.bmBottomLeft   = loadimg( hResSrc, _AT("pop_lst_bl.img") );
    g_CtlImages.lpPanelLayout.bmBottomCenter = loadimg( hResSrc, _AT("pop_lst_bc.img") );
    g_CtlImages.lpPanelLayout.bmBottomRight  = loadimg( hResSrc, _AT("pop_lst_br.img") );
}

static void    MyControl_Final(void)
{
    atUIAPI_DestroyBitmap(g_CtlImages.lpNormalBtnL);
    atUIAPI_DestroyBitmap(g_CtlImages.lpNormalBtnC);
    atUIAPI_DestroyBitmap(g_CtlImages.lpNormalBtnR);
    atUIAPI_DestroyBitmap(g_CtlImages.lpFocusedBtnL);
    atUIAPI_DestroyBitmap(g_CtlImages.lpFocusedBtnC);
    atUIAPI_DestroyBitmap(g_CtlImages.lpFocusedBtnR);
    atUIAPI_DestroyBitmap(g_CtlImages.lpDisabledBtnL);
    atUIAPI_DestroyBitmap(g_CtlImages.lpDisabledBtnC);
    atUIAPI_DestroyBitmap(g_CtlImages.lpDisabledBtnR);

    atUIAPI_DestroyBitmap(g_CtlImages.lpScrollBarTop);
    atUIAPI_DestroyBitmap(g_CtlImages.lpScrollBar);
    atUIAPI_DestroyBitmap(g_CtlImages.lpScrollBarBottom);
    //atUIAPI_DestroyBitmap(g_CtlImages.lpScrollCursor);

    atUIAPI_DestroyBitmap(g_CtlImages.hImgListItemNormalLeft);
    atUIAPI_DestroyBitmap(g_CtlImages.hImgListItemNormalCenter);
    atUIAPI_DestroyBitmap(g_CtlImages.hImgListItemNormalRight);
    atUIAPI_DestroyBitmap(g_CtlImages.hImgListItemSelectLeft);
    atUIAPI_DestroyBitmap(g_CtlImages.hImgListItemSelectCenter);
    atUIAPI_DestroyBitmap(g_CtlImages.hImgListItemSelectRight);
    atUIAPI_DestroyBitmap(g_CtlImages.hImgListItemDisableLeft);
    atUIAPI_DestroyBitmap(g_CtlImages.hImgListItemDisableCenter);
    atUIAPI_DestroyBitmap(g_CtlImages.hImgListItemDisableRight);

    //atUIAPI_DestroyBitmap(g_CtlImages.lpNormalEdtL);
    //atUIAPI_DestroyBitmap(g_CtlImages.lpNormalEdtC);
    //atUIAPI_DestroyBitmap(g_CtlImages.lpNormalEdtR);
    //atUIAPI_DestroyBitmap(g_CtlImages.lpFocusedEdtL);
    //atUIAPI_DestroyBitmap(g_CtlImages.lpFocusedEdtC);
    //atUIAPI_DestroyBitmap(g_CtlImages.lpFocusedEdtR);
    //atUIAPI_DestroyBitmap(g_CtlImages.lpDisabledEdtL);
    //atUIAPI_DestroyBitmap(g_CtlImages.lpDisabledEdtC);
    //atUIAPI_DestroyBitmap(g_CtlImages.lpDisabledEdtR);

    atUIAPI_DestroyBitmap(g_CtlImages.lpSpinStartXOFF);
    atUIAPI_DestroyBitmap(g_CtlImages.lpSpinStartXON);
    atUIAPI_DestroyBitmap(g_CtlImages.lpSpinEndXOFF);
    atUIAPI_DestroyBitmap(g_CtlImages.lpSpinEndXON);

    atUIAPI_DestroyBitmap(g_CtlImages.lpCheckBoxOFF);
    atUIAPI_DestroyBitmap(g_CtlImages.lpCheckBoxON);

    atUIAPI_DestroyBitmap(g_CtlImages.lpRadioOff);
    atUIAPI_DestroyBitmap(g_CtlImages.lpRadioOn);   

	atUIAPI_DestroyBitmap(g_CtlImages.lpProgressBarLeft);
	atUIAPI_DestroyBitmap(g_CtlImages.lpProgressBarCenter);
	atUIAPI_DestroyBitmap(g_CtlImages.lpProgressBarRight);
	atUIAPI_DestroyBitmap(g_CtlImages.lpProgressMeasureLeft);
	atUIAPI_DestroyBitmap(g_CtlImages.lpProgressMeasureCenter);
	atUIAPI_DestroyBitmap(g_CtlImages.lpProgressMeasureRight);

	atUIAPI_DestroyBitmap(g_CtlImages.lpVProgressBarTop);
	atUIAPI_DestroyBitmap(g_CtlImages.lpVProgressBarMiddle);
	atUIAPI_DestroyBitmap(g_CtlImages.lpVProgressBarBottom);
	atUIAPI_DestroyBitmap(g_CtlImages.lpVProgressMeasureTop);
	atUIAPI_DestroyBitmap(g_CtlImages.lpVProgressMeasureMiddle);
	atUIAPI_DestroyBitmap(g_CtlImages.lpVProgressMeasureBottom);

	atUIAPI_DestroyBitmap(g_CtlImages.lpMenuArrowON);
	atUIAPI_DestroyBitmap(g_CtlImages.lpMenuArrowOFF);

	atUIAPI_DestroyBitmap(g_CtlImages.lpHSliderLeft);
	atUIAPI_DestroyBitmap(g_CtlImages.lpHSliderCenter);
	atUIAPI_DestroyBitmap(g_CtlImages.lpHSliderRight);
	atUIAPI_DestroyBitmap(g_CtlImages.lpHSliderTick);
	atUIAPI_DestroyBitmap(g_CtlImages.lpHSliderTickFocus);
	
	atUIAPI_DestroyBitmap(g_CtlImages.lpVSliderTop);
	atUIAPI_DestroyBitmap(g_CtlImages.lpVSliderMiddle);
	atUIAPI_DestroyBitmap(g_CtlImages.lpVSliderBottom);
	atUIAPI_DestroyBitmap(g_CtlImages.lpVSliderTick);	
	atUIAPI_DestroyBitmap(g_CtlImages.lpVSliderTickFocus);	
	
	atUIAPI_DestroyBitmap(g_CtlImages.lpPanelLayout.bmTopLeft);
	atUIAPI_DestroyBitmap(g_CtlImages.lpPanelLayout.bmTopCenter);
	atUIAPI_DestroyBitmap(g_CtlImages.lpPanelLayout.bmTopRight);
	atUIAPI_DestroyBitmap(g_CtlImages.lpPanelLayout.bmMiddleLeft);
	atUIAPI_DestroyBitmap(g_CtlImages.lpPanelLayout.bmMiddleCenter);
	atUIAPI_DestroyBitmap(g_CtlImages.lpPanelLayout.bmMiddleRight);
	atUIAPI_DestroyBitmap(g_CtlImages.lpPanelLayout.bmBottomLeft);
	atUIAPI_DestroyBitmap(g_CtlImages.lpPanelLayout.bmBottomCenter);
	atUIAPI_DestroyBitmap(g_CtlImages.lpPanelLayout.bmBottomRight);
}

static void  MyWindow_Init( atHRESSRC hResSrc )
{
	atMARGIN margin;

    atUIAPI_memset( &g_WinImages, 0, sizeof(g_WinImages) );

    g_WinImages.lpWinLayout[0].bmTopLeft      = loadimg2( hResSrc, _AT("win_tl.img"), atTRUE );
    g_WinImages.lpWinLayout[0].bmTopCenter    = loadimg( hResSrc, _AT("win_tc.img") );
    g_WinImages.lpWinLayout[0].bmTopRight     = loadimg2( hResSrc, _AT("win_tr.img"), atTRUE );
    g_WinImages.lpWinLayout[0].bmMiddleLeft   = loadimg( hResSrc, _AT("win_ml.img") );
    g_WinImages.lpWinLayout[0].bmMiddleCenter = loadimg( hResSrc, _AT("win_mc.img") );
    g_WinImages.lpWinLayout[0].bmMiddleRight  = loadimg( hResSrc, _AT("win_mr.img") );
    g_WinImages.lpWinLayout[0].bmBottomLeft   = loadimg2( hResSrc, _AT("win_bl.img"), atTRUE );
    g_WinImages.lpWinLayout[0].bmBottomCenter = loadimg( hResSrc, _AT("win_bc.img") );
    g_WinImages.lpWinLayout[0].bmBottomRight  = loadimg2( hResSrc, _AT("win_br.img"), atTRUE );

    g_WinImages.lpWinLayout[1].bmTopLeft      = loadimg2( hResSrc, _AT("win_ch_tl.img"), atTRUE );
    g_WinImages.lpWinLayout[1].bmTopCenter    = loadimg( hResSrc, _AT("win_ch_tc.img") );
    g_WinImages.lpWinLayout[1].bmTopRight     = loadimg2( hResSrc, _AT("win_ch_tr.img"), atTRUE );
    g_WinImages.lpWinLayout[1].bmMiddleLeft   = loadimg( hResSrc, _AT("win_ml.img") );
    g_WinImages.lpWinLayout[1].bmMiddleCenter = loadimg( hResSrc, _AT("win_mc.img") );
    g_WinImages.lpWinLayout[1].bmMiddleRight  = loadimg( hResSrc, _AT("win_mr.img") );
    g_WinImages.lpWinLayout[1].bmBottomLeft   = loadimg2( hResSrc, _AT("win_bl.img"), atTRUE );
    g_WinImages.lpWinLayout[1].bmBottomCenter = loadimg( hResSrc, _AT("win_bc.img") );
    g_WinImages.lpWinLayout[1].bmBottomRight  = loadimg2( hResSrc, _AT("win_br.img"), atTRUE );

    g_WinImages.lpPopupLayout[0].bmTopLeft      = loadimg2( hResSrc, _AT("pop_lst_tl.img"), atTRUE );
    g_WinImages.lpPopupLayout[0].bmTopCenter    = loadimg( hResSrc, _AT("pop_lst_tc.img") );
    g_WinImages.lpPopupLayout[0].bmTopRight     = loadimg2( hResSrc, _AT("pop_lst_tr.img"), atTRUE );
    g_WinImages.lpPopupLayout[0].bmMiddleLeft   = loadimg( hResSrc, _AT("pop_lst_ml.img") );
    g_WinImages.lpPopupLayout[0].bmMiddleCenter = loadimg( hResSrc, _AT("pop_lst_mc.img") );
    g_WinImages.lpPopupLayout[0].bmMiddleRight  = loadimg( hResSrc, _AT("pop_lst_mr.img") );
    g_WinImages.lpPopupLayout[0].bmBottomLeft   = loadimg2( hResSrc, _AT("pop_lst_bl.img"), atTRUE );
    g_WinImages.lpPopupLayout[0].bmBottomCenter = loadimg( hResSrc, _AT("pop_lst_bc.img") );
    g_WinImages.lpPopupLayout[0].bmBottomRight  = loadimg2( hResSrc, _AT("pop_lst_br.img"), atTRUE );

    g_WinImages.lpPopupLayout[1].bmTopLeft      = loadimg2( hResSrc, _AT("pop_msg_tl.img"), atTRUE );
    g_WinImages.lpPopupLayout[1].bmTopCenter    = loadimg( hResSrc, _AT("pop_msg_tc.img") );
    g_WinImages.lpPopupLayout[1].bmTopRight     = loadimg2( hResSrc, _AT("pop_msg_tr.img"), atTRUE );
    g_WinImages.lpPopupLayout[1].bmMiddleLeft   = loadimg( hResSrc, _AT("pop_msg_ml.img") );
    g_WinImages.lpPopupLayout[1].bmMiddleCenter = loadimg( hResSrc, _AT("pop_msg_mc.img") );
    g_WinImages.lpPopupLayout[1].bmMiddleRight  = loadimg( hResSrc, _AT("pop_msg_mr.img") );
    g_WinImages.lpPopupLayout[1].bmBottomLeft   = loadimg2( hResSrc, _AT("pop_msg_bl.img"), atTRUE );
    g_WinImages.lpPopupLayout[1].bmBottomCenter = loadimg( hResSrc, _AT("pop_msg_bc.img") );
    g_WinImages.lpPopupLayout[1].bmBottomRight  = loadimg2( hResSrc, _AT("pop_msg_br.img"), atTRUE );

    g_WinImages.lpPopupLayout[2].bmTopLeft      = loadimg2( hResSrc, _AT("pop_lst_tl.img"), atTRUE );
    g_WinImages.lpPopupLayout[2].bmTopCenter    = loadimg( hResSrc, _AT("pop_lst_tc.img") );
    g_WinImages.lpPopupLayout[2].bmTopRight     = loadimg2( hResSrc, _AT("pop_lst_tr.img"), atTRUE );
    g_WinImages.lpPopupLayout[2].bmMiddleLeft   = loadimg( hResSrc, _AT("pop_lst_ml.img") );
    g_WinImages.lpPopupLayout[2].bmMiddleCenter = loadimg( hResSrc, _AT("pop_lst_mc.img") );
    g_WinImages.lpPopupLayout[2].bmMiddleRight  = loadimg( hResSrc, _AT("pop_lst_mr.img") );
    g_WinImages.lpPopupLayout[2].bmBottomLeft   = loadimg2( hResSrc, _AT("pop_lst_bl.img"), atTRUE );
    g_WinImages.lpPopupLayout[2].bmBottomCenter = loadimg( hResSrc, _AT("pop_lst_bc.img") );
    g_WinImages.lpPopupLayout[2].bmBottomRight  = loadimg2( hResSrc, _AT("pop_lst_br.img"), atTRUE );

    g_WinImages.lpPopupLayout[3].bmTopLeft      = loadimg2( hResSrc, _AT("pop_lst_tl.img"), atTRUE );
    g_WinImages.lpPopupLayout[3].bmTopCenter    = loadimg( hResSrc, _AT("pop_lst_tc.img") );
    g_WinImages.lpPopupLayout[3].bmTopRight     = loadimg2( hResSrc, _AT("pop_lst_tr.img"), atTRUE );
    g_WinImages.lpPopupLayout[3].bmMiddleLeft   = loadimg( hResSrc, _AT("pop_lst_ml.img") );
    g_WinImages.lpPopupLayout[3].bmMiddleCenter = loadimg( hResSrc, _AT("pop_lst_mc.img") );
    g_WinImages.lpPopupLayout[3].bmMiddleRight  = loadimg( hResSrc, _AT("pop_lst_mr.img") );
    g_WinImages.lpPopupLayout[3].bmBottomLeft   = loadimg2( hResSrc, _AT("pop_lst_bl.img"), atTRUE );
    g_WinImages.lpPopupLayout[3].bmBottomCenter = loadimg( hResSrc, _AT("pop_lst_bc.img") );
    g_WinImages.lpPopupLayout[3].bmBottomRight  = loadimg2( hResSrc, _AT("pop_lst_br.img"), atTRUE );

	SET_MARGIN( margin, 26, 3, 3, 3 );
	atAPP_SetDefaultLayout( atAPPMNG_GetCurApp(), 0, &g_WinImages.lpWinLayout[0], &margin );
	SET_MARGIN( margin, 26, 3, 3, 3 );
	atAPP_SetDefaultLayout( atAPPMNG_GetCurApp(), 1, &g_WinImages.lpWinLayout[1], &margin );
	SET_MARGIN( margin, 24, 6, 6, 6 );
	atAPP_SetDefaultLayout( atAPPMNG_GetCurApp(), 2, &g_WinImages.lpPopupLayout[0], &margin );
}


static void MyWindow_Final(void)
{
    int i;

    for( i = 0; i < 2; i++ ){
        atUIAPI_DestroyBitmap(g_WinImages.lpWinLayout[i].bmTopLeft );
        atUIAPI_DestroyBitmap(g_WinImages.lpWinLayout[i].bmTopCenter );
        atUIAPI_DestroyBitmap(g_WinImages.lpWinLayout[i].bmTopRight );
        atUIAPI_DestroyBitmap(g_WinImages.lpWinLayout[i].bmMiddleLeft );
        atUIAPI_DestroyBitmap(g_WinImages.lpWinLayout[i].bmMiddleCenter );
        atUIAPI_DestroyBitmap(g_WinImages.lpWinLayout[i].bmMiddleRight );
        atUIAPI_DestroyBitmap(g_WinImages.lpWinLayout[i].bmBottomLeft );
        atUIAPI_DestroyBitmap(g_WinImages.lpWinLayout[i].bmBottomCenter );
        atUIAPI_DestroyBitmap(g_WinImages.lpWinLayout[i].bmBottomRight );

        g_WinImages.lpWinLayout[i].bmTopLeft   = atNULL;
        g_WinImages.lpWinLayout[i].bmTopCenter = atNULL;
        g_WinImages.lpWinLayout[i].bmTopRight  = atNULL;
        g_WinImages.lpWinLayout[i].bmMiddleLeft   = atNULL;
        g_WinImages.lpWinLayout[i].bmMiddleCenter = atNULL;
        g_WinImages.lpWinLayout[i].bmMiddleRight  = atNULL;
        g_WinImages.lpWinLayout[i].bmBottomLeft   = atNULL;
        g_WinImages.lpWinLayout[i].bmBottomCenter = atNULL;
        g_WinImages.lpWinLayout[i].bmBottomRight  = atNULL;
    }

    for( i = 0; i < 4; i++ ){
        atUIAPI_DestroyBitmap(g_WinImages.lpPopupLayout[i].bmTopLeft );
        atUIAPI_DestroyBitmap(g_WinImages.lpPopupLayout[i].bmTopCenter );
        atUIAPI_DestroyBitmap(g_WinImages.lpPopupLayout[i].bmTopRight );
        atUIAPI_DestroyBitmap(g_WinImages.lpPopupLayout[i].bmMiddleLeft );
        atUIAPI_DestroyBitmap(g_WinImages.lpPopupLayout[i].bmMiddleCenter );
        atUIAPI_DestroyBitmap(g_WinImages.lpPopupLayout[i].bmMiddleRight );
        atUIAPI_DestroyBitmap(g_WinImages.lpPopupLayout[i].bmBottomLeft );
        atUIAPI_DestroyBitmap(g_WinImages.lpPopupLayout[i].bmBottomCenter );
        atUIAPI_DestroyBitmap(g_WinImages.lpPopupLayout[i].bmBottomRight );

        g_WinImages.lpPopupLayout[i].bmTopLeft    = atNULL;
        g_WinImages.lpPopupLayout[i].bmTopCenter  = atNULL;
        g_WinImages.lpPopupLayout[i].bmTopRight   = atNULL;
        g_WinImages.lpPopupLayout[i].bmMiddleLeft   = atNULL;
        g_WinImages.lpPopupLayout[i].bmMiddleCenter = atNULL;
        g_WinImages.lpPopupLayout[i].bmMiddleRight  = atNULL;
        g_WinImages.lpPopupLayout[i].bmBottomLeft   = atNULL;
        g_WinImages.lpPopupLayout[i].bmBottomCenter  = atNULL;
        g_WinImages.lpPopupLayout[i].bmBottomRight   = atNULL;
    }
}

void    AEECOMP_ExtPkg_Initialize(void)
{
	atHRESSRC hResSrc = atUIAPI_AddResSource( _AT("ExtPkg"), atUI_RES_SRC_RESSTREAM, (const atLPVOID)pExtPkg_ResData, sizeof(pExtPkg_ResData) );
//	atUIAPI_SetCurRes( _AT("ExtPkg") );

    MyControl_Init(hResSrc);
    MyWindow_Init(hResSrc);
    
	// 사용할 윈도우들을 모두 등록한다.

    RegistMsgBoxExWndClass(hResSrc);
    RegistListBoxWndClass();
    RegistRadioGroupWndClass();
    RegistProgressWndClass();
    RegistHSliderWndClass();
    RegistTextViewWndClass();
    RegistEditBoxExWndClass();
	RegistPopupMenuWndClass(hResSrc);
	RegistFileListWndClass();

//	atUIAPI_SetCurRes( atNULL );
	atUIAPI_RemoveResSource( _AT("ExtPkg") );
}


void    AEECOMP_ExtPkg_Finalize(void)
{
  UnregistEditBoxExWndClass();
	UnregistPopupMenuWndClass();
	UnregistMsgBoxExWndClass();

    MyWindow_Final();
    MyControl_Final();
}

