//
// TestWnd.c 
//
//
//   
//
///////////////////////////////////////////////////////////////////////
 
#include "A_UIENG.h"
#include "UI_Util.h"
#include "Global.h"
//#include "MyControl.h"
#include "AEECOMP_BasePkg.h"
#include "AEECOMP_ExtPkg.h"
#include "TestWnd.h"
#include <string.h>


static atTCHAR xx[][32] = {
	_AT("Add Item"),
	_AT("Remove Item"),
	_AT("Close")
};

static atTCHAR xxyy[100];
static atHCONTROL ctl;
static int xx1 = 10, yy1 = 10, xx2 = 50, yy2 = 50;


static atVOID xyz( atHCONTROL hControl, atHGDC hCanvas, atINT nWidth, atINT nHeight, atBOOL bFocused, atINT nScrollPosX, atINT nScrollPosY, atLPVOID lpParam )
{
	atREGION rgn;

	atCONTROL_GetControlRegion( hControl, &rgn );
	rgn.nX = 0;
	rgn.nY = 0;
//	rgn.nWidth = nWidth;
	rgn.nHeight = nHeight;
		
atUIAPI_FillRect( hCanvas, rgn.nX, rgn.nY, rgn.nWidth, rgn.nHeight, atUI_COLOR_RED );
atUIAPI_DrawRect( hCanvas, rgn.nX, rgn.nY, rgn.nWidth, rgn.nHeight, atUI_COLOR_WHITE, 1 );
atUIAPI_DrawLine( hCanvas, rgn.nX, rgn.nY, rgn.nWidth-1, rgn.nHeight-1, atUI_COLOR_WHITE, 1, atFALSE );
atUIAPI_DrawLine( hCanvas, rgn.nWidth-1, rgn.nY, rgn.nX, rgn.nHeight-1, atUI_COLOR_WHITE, 1, atFALSE );

}


static void TestWndOnCommand(atHWINDOW hWnd, long nParam1, long nParam2)
{
    LPTESTWNDDATA lpData = (LPTESTWNDDATA)atWINDOW_GetDataPtr(hWnd);
//    LPCONFUSERINFO lpConfUserInfo = lpData->lpConfUserInfo;    

    switch(nParam1)
    {
        case 100 :
			atAPP_DestroyWindow( atAPPMNG_GetCurApp(), hWnd );
			break;

        case 2000 : // 메인 메뉴리스트 
            switch( nParam2 ){
				case 0 :
					ExtPkg_CreateButton( hWnd, 100, xx1, yy1, 150, 18, _AT("XXXXXXXX") );
//					atWINDOW_Draw( hWnd, atTRUE );
					atWINDOW_Refresh( hWnd );
					break;
				case 1 :
					atWINDOW_RemoveControl( hWnd, atWINDOW_GetCurControl(hWnd) );
//					atWINDOW_Draw( hWnd, atTRUE );
					atWINDOW_Refresh( hWnd );
					break;
				case 2 :
					atAPP_DestroyWindow( atAPPMNG_GetCurApp(), hWnd );
					break;
			}
			break;
	  default:
	  	if( nParam1 / 1000 == 1 ){
			atTCHAR xx[100];
			atUIAPI_sprintf( xx, _AT("button id = %d"), nParam1 );
			ExtPkg_CreateMsgBoxEx( hWnd, 10, _AT("Notify"), xx, atMBT_CONFIRM );
	  	}
		break;
	}
}


static void TestWndOnChange(atHWINDOW hWnd, long nParam1, long nParam2)
{
    LPTESTWNDDATA lpData = (LPTESTWNDDATA)atWINDOW_GetDataPtr(hWnd);
//    LPCONFUSERINFO lpConfUserInfo = lpData->lpConfUserInfo;    

    switch(nParam1)
    {
        case 103 :
			atVPROGRESSBAR_EXT_SetPosition( lpData->hCtl1, nParam2 );
//			atCONTROL_Draw( lpData->hCtl1, atTRUE );
			atCONTROL_Refresh( lpData->hCtl1 );
			break;
	}
}


//#include "TnPkg_EditBox.h"
static atHBITMAP hbm;

static atINT xxx( atLISTBOX_EXT *lpListBox, atLPVOID lpParam )
{
	return 8;
}

static atLPLISTBOX_EXT_ITEM yyy( atLISTBOX_EXT *lpListBox, atLPVOID lpParam, atINT nIndex )
{
	static atLISTBOX_EXT_ITEM ll;

	atUIAPI_sprintf( ll.szItemName[0], _AT("xxxxxxxxxx yyyyy item %d"), nIndex );
	ll.bEnable = atTRUE;
	ll.clItemColor[1] = atUI_COLOR_WHITE;
	ll.imgItemImage[0] = hbm;
	ll.imgItemImageRegion[0].nX = 0;
	ll.imgItemImageRegion[0].nY = 0;
	ll.imgItemImageRegion[0].nWidth = atUIAPI_GetBitmapWidth(hbm);
	ll.imgItemImageRegion[0].nHeight = atUIAPI_GetBitmapHeight(hbm);
	
	return &ll;
}

static void TestWndOnInit(atHWINDOW hWnd)
{
	atSCROLLPANEL_FUNCS funcs;
    LPTESTWNDDATA	lpData = (LPTESTWNDDATA)atWINDOW_GetDataPtr(hWnd);

//int i;
//for( i = 0; i < 12; i++ ){
//	ExtPkg_CreateButton( hWnd, 100+i, 10, 5+i*20, 50, 18, _AT("xxx") );
//	CreateButton( hWnd, 100, 10, 10, 50, 20, _AT("xxxxxxxxxx") );
//}
//	ctl = ExtPkg_CreateButton( hWnd, 100, x1, y1, 150, 18, _AT("XXXXXXXX") );
//	ctl = ExtPkg_CreateButton( hWnd, 101, 0, 0, 200, 200, _AT("XXXXXXXX") );
//	ctl = ExtPkg_CreateButton( hWnd, 102, x1, y1, 150, 18, _AT("XXXXXXXX") );

	//ctl = BasePkg_CreateEditBox( hWnd, 102, x1, y1, 150, 22, _AT("") );
	//ctl = BasePkg_CreateEditBox( hWnd, 103, x1, y1+40, 150, 22, _AT("ABC1234567XYZ") );
	//ctl = BasePkg_CreateEditBox( hWnd, 102, x1, y1+80, 150, 22, _AT("") );

//atTCHAR str[100];
//atUIAPI_sprintf( str, _AT("%%x = %7d\nXXXXXXXX\nYYYYYYYY\nZZZZZZZZ"), -10 );
//	ctl = BasePkg_CreateStaticBox( hWnd, 102, x1, y1, 150, 100, str );

//	atWINDOW_AddControl( hWnd, TN_EDITBOX_Create( hWnd, 102, x1, y1, 150, 57, _AT("xxxx"), atNULL, atNULL ) );
//	lpData->hEditBox = BasePkg_CreateEditBox( hWnd, 100, 10, 10, 100, 22, _AT("x") );
//	atEDITBOX_SetAlign( lpData->hEditBox, ALIGN_CENTER_MIDDLE );

//	lpData->hCtl1 = ExtPkg_CreateVProgressBar( hWnd, 102, x1, y1, 25, 120, 100, 0 );
//	atVPROGRESSBAR_EXT_SetPosition( lpData->hCtl1, 20 );

//	lpData->hCtl2 = ExtPkg_CreateVSlider( hWnd, 103, x1+30, y1, 18, 120, 100, 20 );
//	lpData->hCtl2 = ExtPkg_CreateHSlider( hWnd, 103, x1+30, y1, 120, 18, 100, 20 );
//	ctl = atVPROGRESSBAR_EXT_SetPosition( ctl, 20 );

//	ctl = ExtPkg_CreateVProgressBar( hWnd, 102, x1, y1, 20, 100, 100, 0 );
//	ctl = atVPROGRESSBAR_EXT_SetPosition( ctl, 20 );

/*
	hbm = atUIAPI_CreateBitmapFromFile( _AT("tick_icon.img") );

	ctl = ExtPkg_CreateListBox( hWnd, 1000, 5, 5, 80, 80, 20, atFALSE, 0, atTRUE );
	atCONTROL_SetAnchor( ctl, 5, 5, 5, 5 );
	atLISTBOX_EXT_SetAttr( ctl, atLISTBOX_EXT_ATTR_MOVE_WRAP );
	atLISTBOX_EXT_SetupFunc( ctl, xxx, yyy, atNULL );
	atLISTBOX_EXT_RealizeItem( ctl );

	atWINDOW_SetSoftKey( hWnd, _AT("Menu"), atNULL, atNULL );
*/	
	int i;
	atHCONTROL ctlx;

//	atWINDOW_SetAnchor( hWnd, 5, 5, 5, 5 );

	ctl = ExtPkg_CreateScrollPanel( hWnd, 200, 10, 10, 160, 160, 150, 410, 0 );
	atCONTROL_SetAnchor( ctl, 10, 10, 10, 10 );
	atUIAPI_memset( &funcs, 0, sizeof(funcs) );
	funcs.fnDrawClient = xyz;
	atSCROLLPANEL_SetupFuncs( ctl, &funcs, atNULL );

/*
	ctlx = ExtPkg_CreateListBox( hWnd, 1000, 5, 5, 80, 80, 20, atFALSE, 0, atTRUE );
	atLISTBOX_EXT_SetupFunc( ctlx, xxx, yyy, atNULL );
	atLISTBOX_EXT_RealizeItem( ctlx );
*/

	for( i = 0; i < 10; i ++ ){
		atTCHAR str[100];
		
		atUIAPI_sprintf( str, _AT("*Title #%d"), i );
		ctlx = BasePkg_CreateStaticBox( atNULL, 0, 10, 2 + 40*i, 100, 18, str );
		atSTATICBOX_SetAttr( ctlx, atSTATICBOX_GetAttr(ctlx) & ~atSTATICBOXATTR_OPAQUE );
		atSCROLLPANEL_AddControl( ctl, ctlx );
		
		atUIAPI_sprintf( str, _AT("Button #%d"), i );
		ctlx = ExtPkg_CreateButton( atNULL, 1000+i, 10, 2 + 40*i + 20, 50, 18, str );
		atCONTROL_SetAnchor( ctlx, 2 + 40*i + 20, atANCHOR_IGNORE, 10, 10 );
		atSCROLLPANEL_AddControl( ctl, ctlx );
	}
}



static void TestWndOnRelease(atHWINDOW hWnd)
{
//    LPTESTWNDDATA	lpData = (LPTESTWNDDATA)atWINDOW_GetDataPtr(hWnd);

}


static int TestWndOnKeyDown(atHWINDOW hWnd, int nKeyCode)
{
    LPTESTWNDDATA lpData = (LPTESTWNDDATA)atWINDOW_GetDataPtr(hWnd);
atHWINDOW hPopup;

    switch( nKeyCode )
    {
			
        case atVKEY_SOFT1:
hPopup = ExtPkg_CreatePopupMenuWnd( hWnd, 2000, 0, 0, 200, 100, xx, 3, 3, MENU_ALIGN_LEFT | MENU_ALIGN_BOTTOM );
SetPopupMenuKey( hPopup, atVKEY_SOFT1, atVKEY_SOFT2, atVKEY_SOFT2 );
EnablePopupMenuItem( hPopup, 0, atFALSE );
EnablePopupMenuItem( hPopup, 1, atFALSE );
atWINDOW_Refresh(hPopup);
            break;
			
        case atVKEY_SOFT2:
/*			
{		
atRECT rect;
SET_RECT( rect, 10, 10, 80, 80 );
atUIAPI_Clear( atUIAPI_GetScreenCanvas(), atUI_COLOR_RED );
			atAPP_RedrawAllWindowArea( atAPPMNG_GetCurApp(), &rect );
//			atWINDOW_Draw( hWnd, atTRUE );
atUIAPI_FlushScreen();
}
*/
{
atTCHAR str[100];
atEDITBOX_GetFullText(lpData->hEditBox, str);
CreateMsgBox( hWnd,  0, _AT(""), str, atMBT_CONFIRM );
}
            break;

        case atVKEY_CLEAR:
		atAPP_DestroyWindow( atAPPMNG_GetCurApp(), hWnd );
            break;
			
        default:
            return 0;
    }

    return 0; // WindowOnEvent 가 호출되지 않도록 0을 리턴한다.
}


#define DELTA 5

static int TestWndOnKeyPress(atHWINDOW hWnd, int nKeyCode)
{
    LPTESTWNDDATA lpData = (LPTESTWNDDATA)atWINDOW_GetDataPtr(hWnd);
//	atHWINDOW hPopup;

    switch( nKeyCode )
    {
        case atVKEY_LEFT:	
			xx1 -= DELTA; xx2 -= DELTA;
//			atCONTROL_SetPosition( ctl, xx1, yy1 );
			atWINDOW_Refresh( hWnd );
            break;

        case atVKEY_RIGHT:	
			xx1 += DELTA; xx2 += DELTA;
//			atCONTROL_SetPosition( ctl, xx1, yy1 );
			atWINDOW_Refresh( hWnd );
            break;

        case atVKEY_UP:	
			yy1 -= DELTA; yy2 -= DELTA;
//			atCONTROL_SetPosition( ctl, xx1, yy1 );
//atSCROLLPANEL_SetScrollPos( ctl, yy1 );
yy1 = atSCROLLPANEL_GetScrollPos( ctl );
			atWINDOW_Refresh( hWnd );
            break;

        case atVKEY_DOWN:	
			yy1 += DELTA; yy2 += DELTA;
//			atCONTROL_SetPosition( ctl, xx1, yy1 );
//atSCROLLPANEL_SetScrollPos( ctl, yy1 );
yy1 = atSCROLLPANEL_GetScrollPos( ctl );
			atWINDOW_Refresh( hWnd );
            break;
			
        default:
            return 0;
    }

    return 1; // WindowOnEvent 가 호출되지 않도록 0을 리턴한다.
}


static void TestWndOnPaint(atHWINDOW hWnd)
{
/*
	atHGDC hCanvas = atWINDOW_GetClientGDC(hWnd);
	atREGION rgn;

	atWINDOW_GetClientRegion( hWnd, &rgn );
		
atUIAPI_FillRect( hCanvas, rgn.nX, rgn.nY, rgn.nWidth, rgn.nHeight, atUI_COLOR_RED );
atUIAPI_DrawRect( hCanvas, rgn.nX, rgn.nY, rgn.nWidth, rgn.nHeight, atUI_COLOR_WHITE, 1 );
atUIAPI_DrawLine( hCanvas, rgn.nX, rgn.nY, rgn.nWidth-1, rgn.nHeight-1, atUI_COLOR_WHITE, 1, atFALSE );
atUIAPI_DrawLine( hCanvas, rgn.nWidth-1, rgn.nY, rgn.nX, rgn.nHeight-1, atUI_COLOR_WHITE, 1, atFALSE );
*/
//	int i;
//	atREGION rgn;

/*
	atSRECT rect1, rect2;

//	SET_RECT( rect1, 10, 10, 50, 50 );
	SET_RECT( rect1, x1, y1, x2, y2 );
	SET_RECT( rect2, 40, 20, 80, 100 );

	atUIAPI_FillRect( gdc, rect1.nStartX, rect1.nStartY, RECT_WIDTH(rect1), RECT_HEIGHT(rect1), atUI_COLOR_RED );
	atUIAPI_FillRect( gdc, rect2.nStartX, rect2.nStartY, RECT_WIDTH(rect2), RECT_HEIGHT(rect2), atUI_COLOR_BLUE );

	if( atUIAPI_IntersectRect( &rect1, &rect1, &rect2 ) )
		atUIAPI_FillRect( gdc, rect1.nStartX, rect1.nStartY, RECT_WIDTH(rect1), RECT_HEIGHT(rect1), atUI_COLOR_GREEN );
*/

/*
	atUIAPI_SetFont( gdc, atUIAPI_GetStockFont(0) );
//	atUIAPI_SetFont( gdc, atUIAPI_GetStockFont(2) );

	atUIAPI_DrawStringEx( gdc, 10, 10, _AT("AB*CDEFG12345abcdWXY[]*%!-=.,"), -1, atUI_COLOR_BLUE ); 
	atUIAPI_DrawStringEx( gdc, 10, 30, _AT("AB*CDEFG12345abcdWXY[]*%!-=.,"), -1, atUI_COLOR_BLUE ); 

	atUIAPI_GetGDCArea( gdc, &rgn );

//	atUIAPI_DrawStringRotateAlign( gdc, rgn.nWidth/2, rgn.nHeight/2, _AT("ABCabc123[]*%!-=.,"), -1, atUI_COLOR_RED, INTTOREAL(10), ALIGN_CENTER_MIDDLE ); 

	for( i = 0; i < 360; i += 45 ){
//		atUIAPI_DrawStringRotate( gdc, rgn.nWidth/2, rgn.nHeight/2, _AT("ABCabc123[]*%!-=.,"), -1, atUI_COLOR_RED, INTTOREAL(i) ); 
//		atUIAPI_DrawStringRotate( gdc, rgn.nWidth/2, rgn.nHeight/2, _AT("A-B"), -1, atUI_COLOR_RED, INTTOREAL(i) ); 
		atUIAPI_DrawStringRotateAlign( gdc, rgn.nWidth/2, rgn.nHeight/2, _AT("ABCabc123[]*%!-=.,"), -1, atUI_COLOR_RED, INTTOREAL(i), ALIGN_CENTER_MIDDLE ); 
	}


	atUIAPI_DrawCircle( gdc, rgn.nWidth/2, rgn.nHeight/2, 2, atUI_COLOR_GREEN, atFALSE );
*/


/*
{
atTCHAR str[100];
atSIZE sz;
int cnt;
//	atUIAPI_SetFont( gdc, atUIAPI_GetStockFont(0) );
atUIAPI_sprintf( str, _AT("%%x = %7d\nXXXXXXXX\nYYYYYYYY\nZZZZZZZZ"), -10 );
cnt = atUIAPI_GetStringMetric( atUIAPI_GetFont(gdc), str, -1, &sz );
atUIAPI_FillRect( gdc, 10, 10, sz.nWidth, sz.nHeight, atUI_COLOR_BLUE );
atUIAPI_DrawString( gdc, 10, 10, str, atUI_COLOR_RED );
}
*/

}


int TestWndOnEvent(atHWINDOW hWnd, int nMsg, long nParam1, long nParam2)
{
	int ret = 0;

	switch( nMsg )
	{
		case WNDMSG_INIT:
			TestWndOnInit( hWnd );
			break;
		case WNDMSG_RELEASE:
			TestWndOnRelease( hWnd );
			break;
		case WNDMSG_PAINT:
			TestWndOnPaint( hWnd );
			break;
		case WNDMSG_KEYDOWN:
			return TestWndOnKeyDown( hWnd, nParam1 );
			break;
		case WNDMSG_KEYPRESS:
			return TestWndOnKeyPress( hWnd, nParam1 );
			break;
		case WNDMSG_CHANGE:
			TestWndOnChange( hWnd, nParam1, nParam2 );
			break;
		case WNDMSG_COMMAND:
			TestWndOnCommand( hWnd, nParam1, nParam2 );
			break;
		default:
			return 0;
			break;
	}

	return ret;
}
