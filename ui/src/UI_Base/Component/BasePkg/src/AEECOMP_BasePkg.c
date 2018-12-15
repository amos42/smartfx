#include "AEECOMP_BasePkg.h"



atHCONTROL BasePkg_CreateDummyCtl(atHWINDOW hWnd, int nIndex, int nX, int nY, int nWidth, int nHeight)
{
    atHCONTROL hControl = atDUMMYCTL_Create( nIndex, nX, nY, nWidth, nHeight );
    if( hWnd ) atWINDOW_AddControl( hWnd, hControl );

    return hControl;
}

atHCONTROL BasePkg_CreateButton(atHWINDOW hWnd, int nIndex, int nX, int nY, int nWidth, int nHeight, atLPTSTR szText)
{
    atHCONTROL hControl = atBUTTON_Create( nIndex, nX, nY, nWidth, nHeight, szText );
    if( hWnd ) atWINDOW_AddControl( hWnd, hControl );

    return hControl;
}

atHCONTROL BasePkg_CreateButton2(atHWINDOW hWnd, int nIndex, int nBY, int nWidth, int nHeight, atLPTSTR szText)
{
	atRECT rect;
	int x, y;

	atWINDOW_GetClientRect(hWnd, &rect);
	x = (RECT_WIDTH(rect) - nWidth) / 2;
	if( nBY >= 0 ) 
		y = nBY;
	else
		y = RECT_HEIGHT(rect) - nHeight + nBY;
	
    return BasePkg_CreateButton( hWnd, nIndex, x, y, nWidth, nHeight, szText );
}

atHCONTROL  BasePkg_CreateCheckBox(atHWINDOW hWnd, int nIndex, int nX, int nY, int nWidth, int nHeight, atLPTSTR szText)
{
    atHCONTROL hControl = atCHECKBOX_Create( nIndex, nX, nY, nWidth, nHeight, szText, atFALSE );
    if( hWnd ) atWINDOW_AddControl( hWnd, hControl );
                
    return hControl;
}

atHCONTROL BasePkg_CreateListBox( atHWINDOW hWnd, int nIndex, int nX, int nY, int nWidth, int nHeight, int nItemHeight, atBOOL bFirstNO, atINT nCheckType )
{
    atHCONTROL  hControl;

    hControl = atLISTBOX_Create( nIndex, nX, nY, nWidth, nHeight, nItemHeight, bFirstNO, nCheckType );
    if( hWnd ) atWINDOW_AddControl( hWnd, hControl );

    return hControl;
}

atHCONTROL BasePkg_CreateListBox2( atHWINDOW hWnd, int nIndex, int nX, int nY, int nWidth, int nHeight, int nItemHeight, atBOOL bFirstNO, atINT nCheckType, atTCHAR (*szListMsg)[32], int nMaxCount)
{
    int i;
    atHCONTROL  hControl = BasePkg_CreateListBox( hWnd, nIndex, nX, nY, nWidth, nHeight,
                                            nItemHeight, bFirstNO, nCheckType);

    for( i = 0; i < nMaxCount; i ++ )
    {
        atLISTBOX_Start_AddItem(hControl);
        if(bFirstNO) atLISTBOX_AddItem_NO(hControl, i+1);
        atLISTBOX_AddItem_String(hControl, szListMsg[i], 0 );
        atLISTBOX_Update_AddItem(hControl);
    }
    
    return hControl;
}

atHCONTROL BasePkg_CreateScrollBar( atHWINDOW hWnd, int nIndex, int nX, int nY, int nWidth, int nHeight, int style, int nCursorX, int nCursorY, int nCursorLimit )
{
    atHCONTROL  hControl = atSCROLLBAR_Create( nIndex, nX, nY, nWidth, nHeight, style,
                                            nCursorX, nCursorY, nCursorLimit );
    if( hWnd ) atWINDOW_AddControl( hWnd, hControl );

    return hControl;
}



atHCONTROL BasePkg_CreateRadioButton(atHWINDOW hWnd, int nIndex, int nX, int nY, int nWidth, int nHeight, int nGroupID, atLPTSTR szText ) 
{
    atHCONTROL hControl = atRADIOBUTTON_Create( nIndex, nX, nY, nWidth, nHeight, nGroupID, szText );
    if( hWnd ) atWINDOW_AddControl( hWnd, hControl );

    return hControl;
}

atHCONTROL  BasePkg_CreateSpinBox(atHWINDOW hWnd, int nIndex, int nX, int nY, int nWidth, int nHeight)
{
    atHCONTROL hControl = atSPINBOX_Create( nIndex, nX, nY, nWidth, nHeight );
    if( hWnd ) atWINDOW_AddControl( hWnd, hControl );

    return hControl;
}

atHCONTROL  BasePkg_CreateSpinBox2(atHWINDOW hWnd, int nIndex, int nX, int nY, int nWidth, int nHeight, atTCHAR (*szText)[32], int nMaxCount)
{
    int i;
    atHCONTROL hControl = BasePkg_CreateSpinBox(hWnd, nIndex, nX, nY, nWidth, nHeight);
    
    for (i=0; i<nMaxCount; i++){
        atSPINBOX_AddItem(hControl, i, szText[i]);
    }

    return hControl;
}

atHCONTROL  BasePkg_CreateEditBox(atHWINDOW hWnd, int nIndex, int nX, int nY, int nWidth, int nHeight, atLPTSTR szText, atEDITBOX_INPUTMODE lstInputMode[], atINT nInputModeCount, atINT nCurInputMode )
{
    atHCONTROL hControl = atEDITBOX_Create( nIndex, nX, nY, nWidth, nHeight, szText, lstInputMode, nInputModeCount, nCurInputMode );
    if( hWnd ) atWINDOW_AddControl( hWnd, hControl );

    return hControl;
}

atHCONTROL  BasePkg_CreateStaticBox(atHWINDOW   hWnd, int nIndex, int nX, int nY, int nWidth, int nHeight, atLPTSTR szText )
{
    atHCONTROL hControl = atSTATICBOX_Create( nIndex, nX, nY, nWidth, nHeight, szText );
	
    if( hWnd ) atWINDOW_AddControl( hWnd, hControl );

    return hControl;
}


atHCONTROL  BasePkg_CreateStaticBoxIDS(atHWINDOW   hWnd, int nIndex, int nX, int nY, int nWidth, int nHeight, atULONG nTextIDS)
{
    atHCONTROL hControl = atSTATICBOX_CreateIDS( nIndex, nX, nY, nWidth, nHeight, nTextIDS);
	
    if( hWnd ) atWINDOW_AddControl( hWnd, hControl );

    return hControl;
}


atHCONTROL  BasePkg_CreateImageBox(atHWINDOW    hWnd, int nIndex, int nX, int nY, int nWidth, int nHeight, atHBITMAP lpBM, atBOOL bOwnImage )
{
    atHCONTROL hControl = atIMAGEBOX_Create( nIndex, nX, nY, nWidth, nHeight, lpBM, bOwnImage );
    if( hWnd ) atWINDOW_AddControl( hWnd, hControl );

    return hControl;
}

atHCONTROL  BasePkg_CreateImageBox2(atHWINDOW   hWnd, int nIndex, int nX, int nY, int nWidth, int nHeight, atLPTSTR filename )
{
    atHBITMAP lpBM = atUIAPI_CreateBitmapFromFile(filename);
	
    return  BasePkg_CreateImageBox( hWnd, nIndex, nX, nY, nWidth, nHeight, lpBM, atTRUE );
}

atHCONTROL BasePkg_CreateRadioGroup(atHWINDOW hWnd, int nIndex, int nX, int nY, int nWidth, int nHeight, 
                            atTCHAR (*szButtonMsg)[32], int ButtonCnt, int init_idx, int item_height )
{
    atHCONTROL hControl = atRADIOGROUP_Create( nIndex, nX, nY, nWidth, nHeight, szButtonMsg, ButtonCnt, init_idx, item_height );
    if( hWnd ) atWINDOW_AddControl( hWnd, hControl );

    return hControl;
}


atHCONTROL BasePkg_CreateProgressBar(atHWINDOW hWnd, int nIndex, int nX, int nY, int nWidth, int nHeight, int nSize, int isText ) 
{
    atHCONTROL hControl = atPROGRESSBAR_Create( nIndex, nX, nY, nWidth, nHeight, isText, 2, 2, 0 );
    atPROGRESSBAR_SetSize( hControl, nSize );

    if( hWnd ) atWINDOW_AddControl( hWnd, hControl );

    return hControl;
}


atHCONTROL BasePkg_CreateVProgressBar(atHWINDOW hWnd, int nIndex, int nX, int nY, int nWidth, int nHeight, int nSize, int isText ) 
{
    atHCONTROL hControl = atVPROGRESSBAR_Create( nIndex, nX, nY, nWidth, nHeight, isText, 2, 2, 0 );
    atVPROGRESSBAR_SetSize( hControl, nSize );

    if( hWnd ) atWINDOW_AddControl( hWnd, hControl );

    return hControl;
}


atHCONTROL BasePkg_CreateHSlider( atHWINDOW hWnd, int nIndex, int nX, int nY, int nWidth, int nHeight, int max_pos, int pos )
{
	atHCONTROL hControl = atHSLIDER_Create( nIndex, nX, nY, nWidth, nHeight, 0, 0, 0 );

    atHSLIDER_SetMaxPos( hControl, max_pos );
    atHSLIDER_SetPosition( hControl, pos );
	
    if( hWnd ) atWINDOW_AddControl( hWnd, hControl );
	
    return hControl;
}


atHCONTROL BasePkg_CreateVSlider( atHWINDOW hWnd, int nIndex, int nX, int nY, int nWidth, int nHeight, int direction, int max_pos, int pos )
{
	atHCONTROL hControl = atVSLIDER_Create( nIndex, nX, nY, nWidth, nHeight, direction, 0, 0, 0 );

    atVSLIDER_SetMaxPos( hControl, max_pos );
    atVSLIDER_SetPosition( hControl, pos );
	
    if( hWnd ) atWINDOW_AddControl( hWnd, hControl );
	
    return hControl;
}


atHCONTROL BasePkg_CreatePanel( atHWINDOW hWnd, int nIndex, int nX, int nY, int nWidth, int nHeight, int nBorderWidth )
{
	atHCONTROL hControl = atPANEL_Create( nIndex, nX, nY, nWidth, nHeight, 0, nBorderWidth );

    if( hWnd ) atWINDOW_AddControl( hWnd, hControl );
	
    return hControl;
}


atHCONTROL BasePkg_CreatePaintBox( atHWINDOW hWnd, int nIndex, int nX, int nY, int nWidth, int nHeight, atLPPAINTBOX_RENDERER lpRenderer )
{
	atHCONTROL hControl = atPAINTBOX_Create( nIndex, nX, nY, nWidth, nHeight, 0, lpRenderer );

    if( hWnd ) atWINDOW_AddControl( hWnd, hControl );
	
    return hControl;
}


atHWINDOW	BasePkg_CreateMemoryInfoWnd( atHWINDOW hParentWnd )
{
	return CreateMemoryInfoWnd( hParentWnd );
}


atHWINDOW	BasePkg_CreateWindowInfoWnd( atHWINDOW hParentWnd )
{
	return CreateWindowInfoWnd( hParentWnd );
}


atHWINDOW	BasePkg_CreateMsgBox( atHWINDOW hParentWnd, int id, atLPTSTR szWindowText, atLPTSTR szMsg, int nMsgBoxType )
{
	return CreateMsgBox( hParentWnd, id, szWindowText, szMsg, nMsgBoxType );
}


atHWINDOW	BasePkg_CreateMsgBox2( atHWINDOW hParentWnd, int id, atLPTSTR szWindowText, atLPTSTR szMsg, int nMsgBoxType, long lWaitTime )
{
	return CreateMsgBox2( hParentWnd, id, szWindowText, szMsg, nMsgBoxType, lWaitTime );
}

atHWINDOW	BasePkg_CreateEditBoxWnd(atHWINDOW hParentWnd, int id, atLPTSTR szTitle, atLPTSTR szMsg, atLPTSTR szText )
{
	return CreateEditBoxWnd( hParentWnd, id, szTitle, szMsg, szText );
}



static void  MyControl_Init(void)
{
}

static void    MyControl_Final(void)
{
}

static void  MyWindow_Init(void)
{
	atMARGIN margin;

	SET_MARGIN( margin, 26, 3, 3, 3 );
	atAPP_SetDefaultLayout( atAPPMNG_GetCurApp(), 0, atNULL, &margin );
	SET_MARGIN( margin, 26, 3, 3, 3 );
	atAPP_SetDefaultLayout( atAPPMNG_GetCurApp(), 1, atNULL, &margin );
	SET_MARGIN( margin, 24, 6, 6, 6 );
	atAPP_SetDefaultLayout( atAPPMNG_GetCurApp(), 2, atNULL, &margin );
}


static void MyWindow_Final(void)
{
}


void    AEECOMP_BasePkg_Initialize(void)
{
    MyControl_Init();
    MyWindow_Init();
    
	// 사용할 윈도우들을 모두 등록한다.
    RegistDummyWndClass();
    RegistMemoryInfoWndClass();
    RegistWindowInfoWndClass();
    RegistMsgBoxWndClass();
    RegistEditBoxWndClass();
}


void    AEECOMP_BasePkg_Finalize(void)
{
	UnregistEditBoxWndClass();
	UnregistMsgBoxWndClass();
	UnregistWindowInfoWndClass();
	UnregistMemoryInfoWndClass();
   	UnregistDummyWndClass();

    MyWindow_Final();
    MyControl_Final();
}

