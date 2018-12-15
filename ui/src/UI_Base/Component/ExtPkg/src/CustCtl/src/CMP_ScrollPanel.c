//
// ScrollPannel.c
//
///////////////////////////////////////////////////////////////////////

#include "A_UIENG.h"
#include "CMP_ScrollPanel.h"
#include "CMP_ScrollBarEx.h"
#include "AEECOMP_BasePkg.h"


typedef struct _atSCROLLPANEL {
	int dummy;
	int nClientWidth, nClientHeight;
	atHBITMAP hScrollbarImgTop, hScrollbarImage, hScrollbarImgBottom, hScrollcursorImage;
	atHCONTROL hClientCtl;
	atHCONTROL hScrollBar;

	atSCROLLPANEL_FUNCS fnFuncs;
	atLPVOID			lpFuncParam;

	atLPCTRLPROC		lpOrgScrollPanelClientProc;

	int pos;
	atBOOL	bActionValidate;
	atINT	nFirstMouseY;
	atINT	nFirstViewTopY;
	atBOOL	bUsedTouch;		// 터치 사용여부
	atINT	nHeight;
} atSCROLLPANEL,  *atLPSCROLLPANEL;


static int ScrollPanelProc( atHCONTROL hControl, int nMsg, long lParam1, long lParam2 );

static int ScrollPanelClientProc( atHCONTROL hControl, int nMsg, long lParam1, long lParam2 )
{
	int r = 0;
	atHGDC hClientGDC;
//	int x, y;
	int msg = -1;
	atLPSCROLLPANEL lpPanel = (atLPSCROLLPANEL)atCONTROL_GetUserDataPtr( hControl );
	if( lpPanel == atNULL ) return 0;

	switch( nMsg ){
		case CTLMSG_PAINT:
			if( lpPanel->fnFuncs.fnDrawClient ){
				hClientGDC = atCONTROL_GetGDC( hControl );
				if( atUIAPI_IsCanvasActivate(hClientGDC) ){
					lpPanel->fnFuncs.fnDrawClient( hControl, hClientGDC, 
												   lpPanel->nClientWidth, lpPanel->nClientHeight, 
												   (atBOOL)lParam2/*atCONTROL_IsFocus(hControl)*/,
												   0, lpPanel->pos, 
												   lpPanel->lpFuncParam );
				}
				r = 1;
			}
			break;
/*
		case CTLMSG_MOUSEDOWN:
			x = (short)(lParam2 >> 16);
			y = (short)(lParam2 & 0xffff);
			msg = 0;
			if( lpPanel->fnFuncs.fnTouchClient ){
				r = lpPanel->fnFuncs.fnTouchClient( hControl,  msg, x, y,  lpPanel->nClientWidth, lpPanel->nClientHeight, 
											   lpPanel->lpFuncParam );
			}
			if( r != 0 ) break;
			lpPanel->bActionValidate = atTRUE;
			atSCROLLBAR_EXT_StartGestureScroll( lpPanel->hScrollBar, y );
			break;
		case CTLMSG_MOUSEMOVE:
			x = (short)(lParam2 >> 16);
			y = (short)(lParam2 & 0xffff);
			msg = 1;
			if( lpPanel->fnFuncs.fnTouchClient ){
				r = lpPanel->fnFuncs.fnTouchClient( hControl,  msg, x, y,  lpPanel->nClientWidth, lpPanel->nClientHeight, 
											   lpPanel->lpFuncParam );
			}
			if( r != 0 ) break;
			if( lpPanel->bActionValidate ){
				atSCROLLBAR_EXT_ProcessGestureScroll( lpPanel->hScrollBar, y );
			}
			break;
		case CTLMSG_MOUSEUP:
			x = (short)(lParam2 >> 16);
			y = (short)(lParam2 & 0xffff);
			msg = 2;
			if( lpPanel->fnFuncs.fnTouchClient ){
				r = lpPanel->fnFuncs.fnTouchClient( hControl,  msg, x, y,  lpPanel->nClientWidth, lpPanel->nClientHeight, 
											   lpPanel->lpFuncParam );
			}
			if( r != 0 ) break;
			if( lpPanel->bActionValidate ){
				atSCROLLBAR_EXT_EndGestureScroll( lpPanel->hScrollBar, y );
				lpPanel->bActionValidate = atFALSE;
			}
			break;
*/
		default:
			if( lpPanel->lpOrgScrollPanelClientProc )
				r = atCONTROL_CallControlProc( lpPanel->lpOrgScrollPanelClientProc, hControl, nMsg, lParam1, lParam2 );
			break;
	}

	return r;
}


static void _RecalcSize( atHCONTROL hControl )
{
	atLPSCROLLPANEL lpPanel = (atLPSCROLLPANEL)atCONTROL_GetDataPtr( hControl );
	atREGION rgn;
	int scroll_width = 0;

	atCONTROL_GetControlRegion( hControl, &rgn );
	if( rgn.nHeight <= 0 ){
		atSCROLLBAR_EXT_SetSize( lpPanel->hScrollBar, 0 );
		atSCROLLPANEL_SetScrollPos( hControl, 0 );
		return;
	}
	
	if( lpPanel->nClientHeight >= rgn.nHeight  ){
		atREGION rgn2;
//		int new_size = lpPanel->nClientHeight - rgn.nHeight;
		int new_size = lpPanel->nClientHeight;
		atSCROLLBAR_EXT_SetSize( lpPanel->hScrollBar, new_size );
		atCONTROL_ShowChildControl( hControl, lpPanel->hScrollBar, atTRUE );
		atCONTROL_GetControlRegion( lpPanel->hScrollBar, &rgn2 );
		scroll_width = rgn2.nWidth;
		if( lpPanel->pos > new_size ) atSCROLLPANEL_SetScrollPos( hControl, new_size );
	} else {
		atSCROLLPANEL_SetScrollPos( hControl, 0 );
		atSCROLLBAR_EXT_SetSize( lpPanel->hScrollBar, 0 );
		atCONTROL_ShowChildControl( hControl, lpPanel->hScrollBar, atFALSE );
	}
	
	atCONTROL_SetSize( lpPanel->hClientCtl, rgn.nWidth - scroll_width, lpPanel->nClientHeight );
}



struct _CreateParam {
	int nClientWidth, nClientHeight;
	//atHBITMAP hScrollbarImgTop, hScrollbarImage, hScrollbarImgBottom, hScrollcursorImage;
	atHBITMAP hScrollbarImgTop, hScrollbarImage, hScrollbarImgBottom;
	atHBITMAP hScrollCursorImgTop, hScrollCursorImgMid, hScrollCursorImgBot;
};


atHCONTROL atSCROLLPANEL_Create( int nIndex, int nX, int nY, int nWidth, int nHeight, int nClientWidth, int nClientHeight, atDWORD dwAttr,
 								 //atHBITMAP hScrollbarImgTop, atHBITMAP hScrollbarImage, atHBITMAP hScrollbarImgBottom, atHBITMAP hScrollcursorImage )
								 atHBITMAP hScrollbarImgTop, atHBITMAP hScrollbarImage, atHBITMAP hScrollbarImgBottom,
								 atHBITMAP hScrollCursorImgTop, atHBITMAP hScrollCursorImgMid, atHBITMAP hScrollCursorImgBot )
{
	atHCONTROL		hControl;
	struct _CreateParam param;

	param.nClientWidth = nClientWidth;
	param.nClientHeight = nClientHeight;
	param.hScrollbarImgTop	= hScrollbarImgTop;
	param.hScrollbarImage	= hScrollbarImage;
	param.hScrollbarImgBottom	= hScrollbarImgBottom;
	//param.hScrollcursorImage	= hScrollcursorImage;
	param.hScrollCursorImgTop	= hScrollCursorImgTop;
	param.hScrollCursorImgMid	= hScrollCursorImgMid;
	param.hScrollCursorImgBot	= hScrollCursorImgBot;

	hControl = atCONTROL_Create( nIndex, nX, nY, nWidth, nHeight, CTL_PARENTTYPE_WINDOW, sizeof(atSCROLLPANEL), ScrollPanelProc, &param );

	return hControl;
}

static void atSCROLLPANEL_OnInit( atHCONTROL hControl, struct _CreateParam *param )
{
	atLPSCROLLPANEL lpPanel = (atLPSCROLLPANEL)atCONTROL_GetDataPtr( hControl );
//	atCONTROL_COLOR cl;
	int sw;

	lpPanel->nClientWidth = param->nClientWidth;
	lpPanel->nClientHeight = param->nClientHeight;
	
	lpPanel->lpOrgScrollPanelClientProc = atNULL;
//	atUIAPI_memset( &lpPanel->fnFuncs, 0, sizeof(atSCROLLPANEL_FUNCS) );

	lpPanel->hClientCtl = atDUMMYCTL_Create( 100, 0, 0, param->nClientWidth, param->nClientHeight );
	atCONTROL_SetAttr( lpPanel->hClientCtl, atCONTROL_GetAttr(lpPanel->hClientCtl) | atOBJATTR_TRANSPARENT | CTL_PARENTTYPE_CONTROL );
	atCONTROL_SetAttr( lpPanel->hClientCtl, atCONTROL_GetAttr(lpPanel->hClientCtl) & ~atOBJATTR_EVENTTARGET );
//	atCONTROL_SetAnchor( lpPanel->hClientCtl, 0, 0, 0, 8 );
	atCONTROL_SetUserDataPtr( lpPanel->hClientCtl, (void *)lpPanel );
	lpPanel->lpOrgScrollPanelClientProc = atCONTROL_SetControlProc( lpPanel->hClientCtl, ScrollPanelClientProc );
	atCONTROL_AddChildControl( hControl, lpPanel->hClientCtl );	

	if( param->hScrollbarImgTop )
		sw = atUIAPI_GetBitmapWidth(param->hScrollbarImgTop);
	else if( param->hScrollCursorImgTop )
		sw = atUIAPI_GetBitmapWidth(param->hScrollCursorImgTop);
	else
		sw = 8;
	lpPanel->hScrollBar = atSCROLLBAR_EXT_Create( 100, 0, 0, sw, 100, atSCROLLBAR_EXT_TYPE_WINDOW_FRICTION_STICK, 0, 0, 0, 0,
						  						   param->hScrollbarImgTop, param->hScrollbarImage, param->hScrollbarImgBottom, 
						  						   //param->hScrollcursorImage );
												   param->hScrollCursorImgTop, param->hScrollCursorImgMid, param->hScrollCursorImgBot );
	//atSCROLLBAR_EXT_SetSize( lpPanel->hScrollBar, param->nClientHeight );
	
	atCONTROL_SetAttr( lpPanel->hScrollBar, atCONTROL_GetAttr(lpPanel->hScrollBar) | CTL_PARENTTYPE_CONTROL );
	atCONTROL_SetAnchor( lpPanel->hScrollBar, 0, 0, atANCHOR_IGNORE, 0 );
//	atCONTROL_GetControlColor( hControl, &cl );
//	atCONTROL_SetControlColor( lpPanel->hScrollBar, &cl );
	atCONTROL_SetFocusable(lpPanel->hScrollBar, atFALSE);
//	atCONTROL_SetVisible( lpPanel->hScrollBar, atFALSE );
	atCONTROL_AddChildControl( hControl, lpPanel->hScrollBar );	

	_RecalcSize(hControl);
	
	lpPanel->pos = 0;
}

static void atSCROLLPANEL_OnRelease( atHCONTROL hControl )
{
//	atSCROLLPANEL_SetScrollPos( hControl, 0 );
}

static void atSCROLLPANEL_OnDraw( atHGDC hCanvas, atHCONTROL hControl, atBOOL bFocused )
{
/*
	atREGION rgn;

	atCONTROL_GetClientRegion( hControl, &rgn );
		
	atUIAPI_FillRect( hCanvas, rgn.nX, rgn.nY, rgn.nWidth, rgn.nHeight, atUI_COLOR_BLUE );
	atUIAPI_DrawRect( hCanvas, rgn.nX, rgn.nY, rgn.nWidth, rgn.nHeight, atUI_COLOR_WHITE, 1 );
	atUIAPI_DrawLine( hCanvas, rgn.nX, rgn.nY, rgn.nWidth-1, rgn.nHeight-1, atUI_COLOR_WHITE, 1, atFALSE );
	atUIAPI_DrawLine( hCanvas, rgn.nWidth-1, rgn.nY, rgn.nX, rgn.nHeight-1, atUI_COLOR_WHITE, 1, atFALSE );
*/	
}

static int atSCROLLPANEL_OnKeyDown( atHCONTROL hControl, atINT nKeyCode )
{
	return 0;
}

static int atSCROLLPANEL_OnMouseDown( atHCONTROL hControl, atINT nButton, atINT nX, atINT nY )
{
	int r = 0;
	atLPSCROLLPANEL lpPanel = (atLPSCROLLPANEL)atCONTROL_GetDataPtr( hControl );
	
	if( lpPanel->fnFuncs.fnTouchClient ){
		r = lpPanel->fnFuncs.fnTouchClient( hControl, 0, nX, nY+lpPanel->pos, lpPanel->nClientWidth, lpPanel->nClientHeight, 
									   lpPanel->lpFuncParam );
	}
	if( r != 0 ) return r;

	if( atCONTROL_IsVisible(lpPanel->hScrollBar) ) {
		if( atSCROLLBAR_EXT_IsProcessGestureScroll(lpPanel->hScrollBar) ){
			atSCROLLBAR_EXT_StopAndResetGestureScroll(lpPanel->hScrollBar);
			lpPanel->bActionValidate = atFALSE;
		}

		lpPanel->nFirstViewTopY = atSCROLLBAR_EXT_GetPosition(lpPanel->hScrollBar);
		lpPanel->nFirstMouseY = nY;
		lpPanel->bActionValidate = atTRUE;
		
		atSCROLLBAR_EXT_StartGestureScroll( lpPanel->hScrollBar, lpPanel->nFirstMouseY );
	} else {
		lpPanel->bActionValidate = atFALSE;
	}
	
	return 0;
}

static int atSCROLLPANEL_OnMouseMove( atHCONTROL hControl, atINT nButton, atINT nX, atINT nY )
{
	int r = 0;
	atLPSCROLLPANEL lpPanel = (atLPSCROLLPANEL)atCONTROL_GetDataPtr( hControl );
	
	if( !lpPanel->bActionValidate ){
	if( lpPanel->fnFuncs.fnTouchClient ){
		r = lpPanel->fnFuncs.fnTouchClient( hControl, 1, nX, nY+lpPanel->pos, lpPanel->nClientWidth, lpPanel->nClientHeight, 
									   lpPanel->lpFuncParam );
	}
	if( r != 0 ) return r;
	}

	if( lpPanel->bActionValidate ){
//		atSCROLLPANEL_SetScrollPos( hControl, lpPanel->nFirstViewTopY + (lpPanel->nFirstMouseY - nY) ); // 현재 이벤트 구조에 문제가 있는지 오동작한다.
//		atSCROLLPANEL_SetScrollPos( hControl, (lpPanel->nFirstMouseY - nY) );
//		atCONTROL_Refresh(hControl);

		atSCROLLBAR_EXT_ProcessGestureScroll( lpPanel->hScrollBar, nY );
	}
	
	return 0;
}

static int atSCROLLPANEL_OnMouseUp( atHCONTROL hControl, atINT nButton, atINT nX, atINT nY )
{
	int r = 0;
	atLPSCROLLPANEL lpPanel = (atLPSCROLLPANEL)atCONTROL_GetDataPtr( hControl );
	
	if( !lpPanel->bActionValidate ){
		if( lpPanel->fnFuncs.fnTouchClient ){
			r = lpPanel->fnFuncs.fnTouchClient( hControl, 2, nX, nY+lpPanel->pos, lpPanel->nClientWidth, lpPanel->nClientHeight, 
										   lpPanel->lpFuncParam );
		}
		if( r != 0 ) return r;
	}

	if( lpPanel->bActionValidate ){
		atSCROLLBAR_EXT_EndGestureScroll( lpPanel->hScrollBar, nY );
		lpPanel->bActionValidate = atFALSE;
	}
	
	return 0;
}


static void atSCROLLPANEL_OnResize( atHCONTROL hControl )
{
	/*
	atLPSCROLLPANEL lpPanel = (atLPSCROLLPANEL)atCONTROL_GetDataPtr( hControl );
	atREGION rgn;
	int scroll_width = 0;

	atCONTROL_GetControlRegion( hControl, &rgn );
	
	if( lpPanel->nClientHeight >= rgn.nHeight  ){
		atREGION rgn2;
		int new_size = lpPanel->nClientHeight - rgn.nHeight;
		atSCROLLBAR_EXT_SetSize( lpPanel->hScrollBar, new_size );
		atCONTROL_ShowChildControl( hControl, lpPanel->hScrollBar, atTRUE );
		atCONTROL_GetControlRegion( lpPanel->hScrollBar, &rgn2 );
		scroll_width = rgn2.nWidth;
		if( lpPanel->pos > new_size ) atSCROLLPANEL_SetScrollPos( hControl, new_size );
	} else {
		atSCROLLPANEL_SetScrollPos( hControl, 0 );
		atSCROLLBAR_EXT_SetSize( lpPanel->hScrollBar, 0 );
		atCONTROL_ShowChildControl( hControl, lpPanel->hScrollBar, atFALSE );
	}
	
	atCONTROL_SetSize( lpPanel->hClientCtl, rgn.nWidth - scroll_width, lpPanel->nClientHeight );
	*/
	_RecalcSize(hControl);
}

static void atSCROLLPANEL_OnChangeControlFocus( atHCONTROL hControl,  atHCONTROL hNewChildCtl, atHCONTROL hOldChildCtl )
{
	atLPSCROLLPANEL lpPanel = (atLPSCROLLPANEL)atCONTROL_GetDataPtr( hControl );
	atREGION parentrgn, ctlrgn;
	atINT iSetScroll;
	int item_s_y, item_e_y, v_item_s_y, v_item_e_y;

	if( hNewChildCtl == atNULL ) return;
	atCONTROL_GetControlRegion( hControl, &parentrgn );
	atCONTROL_GetControlRegion( hNewChildCtl, &ctlrgn );

	iSetScroll = lpPanel->pos;

	item_s_y = ctlrgn.nY;
	item_e_y = item_s_y + ctlrgn.nHeight;
	
	v_item_s_y = item_s_y - iSetScroll;
	v_item_e_y = item_e_y - iSetScroll;

	if( v_item_s_y < 0 )
		iSetScroll = item_s_y;
	if( v_item_e_y > parentrgn.nHeight )
		iSetScroll = item_e_y - parentrgn.nHeight;

	if( iSetScroll != lpPanel->pos ){
		atSCROLLPANEL_SetScrollPos(hControl, iSetScroll);
		atCONTROL_Refresh(hControl);
	}
}


static int ScrollPanelProc( atHCONTROL hControl, int nMsg, long lParam1, long lParam2 )
{
	atLPCONTROL lpControl = atCONTROL_GetPtr(hControl);
	atLPSCROLLPANEL lpPanel = (atLPSCROLLPANEL)atCONTROL_GetDataPtr( hControl );
	int x, y;
	atCONTROL_COLOR cl;
	int ret = 0;

	switch( nMsg )
	{
		case CTLMSG_INIT:
			atSCROLLPANEL_OnInit( hControl, (struct _CreateParam *)lParam1 );
			break;
		case CTLMSG_RELEASE:
			atSCROLLPANEL_OnRelease( hControl );
			break;
		case CTLMSG_CHANGE:
			if( lParam1 == 100 ) {
				atSCROLLPANEL_SetScrollPos(hControl, lParam2);
				atCONTROL_Refresh( hControl );
				ret = 1;
			}
			break;
		case CTLMSG_PAINT:
			atSCROLLPANEL_OnDraw( atCONTROL_GetGDC(hControl), hControl, (atBOOL)lParam2 );
			ret = 1;
			break;
		case CTLMSG_RESIZE:
			atSCROLLPANEL_OnResize( hControl );
			break;
		case CTLMSG_KEYDOWN:
		    return atSCROLLPANEL_OnKeyDown( hControl, lParam1 );
		case CTLMSG_CHANGECONTROLFOCUS:
			atSCROLLPANEL_OnChangeControlFocus( hControl, (atHCONTROL)lParam1, (atHCONTROL)lParam2 );
			break;
		case CTLMSG_MOUSEDOWN:
			x = (short)(lParam2 >> 16);
			y = (short)(lParam2 & 0xffff);
			atSCROLLPANEL_OnMouseDown( hControl, lParam1, x, y );
			break;
		case CTLMSG_MOUSEMOVE:
			x = (short)(lParam2 >> 16);
			y = (short)(lParam2 & 0xffff);
			atSCROLLPANEL_OnMouseMove( hControl, lParam1, x, y );
			break;
		case CTLMSG_MOUSEUP:
			x = (short)(lParam2 >> 16);
			y = (short)(lParam2 & 0xffff);
			atSCROLLPANEL_OnMouseUp( hControl, lParam1, x, y );
			break;
		case CTLMSG_CHANGELAYOUT:
			atCONTROL_GetControlColor( hControl, &cl );
			atCONTROL_SetControlColor( lpPanel->hScrollBar, &cl );
			break;
	}

	return ret;
}


//===================================================

void	atSCROLLPANEL_GetFuncs( atHCONTROL hControl, atLPSCROLLPANEL_FUNCS lpFuncs, atLPVOID *lppFuncParam )
{
	atLPSCROLLPANEL lpPanel = (atLPSCROLLPANEL)atCONTROL_GetDataPtr( hControl );
	if( lpPanel == atNULL || lpFuncs == atNULL ) return;

	*lpFuncs = lpPanel->fnFuncs;
	if( lppFuncParam ) *lppFuncParam = lpPanel->lpFuncParam;
}

void atSCROLLPANEL_SetupFuncs( atHCONTROL hControl, atLPSCROLLPANEL_FUNCS lpFuncs, atLPVOID lpFuncParam )
{
	atLPSCROLLPANEL lpPanel = (atLPSCROLLPANEL)atCONTROL_GetDataPtr( hControl );
	if( lpPanel == atNULL || lpFuncs == atNULL ) return;

	lpPanel->fnFuncs = *lpFuncs;
	lpPanel->lpFuncParam = lpFuncParam;

	atSCROLLPANEL_SetScrollPos( hControl, 0 );
	atCONTROL_Refresh( hControl );
}

atHCONTROL atSCROLLPANEL_GetPanelControl( atHCONTROL hControl )
{
	atLPSCROLLPANEL lpPanel = (atLPSCROLLPANEL)atCONTROL_GetDataPtr( hControl );
	if( lpPanel == atNULL ) return atNULL;

	return lpPanel->hClientCtl;
}

// Kyle Start [Add 2009/06/22-14:25]
atHCONTROL atSCROLLPANEL_GetScrollBar( atHCONTROL hControl )
{
	atLPSCROLLPANEL lpPanel = (atLPSCROLLPANEL)atCONTROL_GetDataPtr( hControl );
	if( lpPanel == atNULL ) return atNULL;

	return lpPanel->hScrollBar;
}
// Kyle [Add 2009/06/22-14:25] End

atBOOL	atSCROLLPANEL_AddControl( atHCONTROL hControl, atHCONTROL hChildControl )
{
	atLPSCROLLPANEL lpPanel = (atLPSCROLLPANEL)atCONTROL_GetDataPtr( hControl );
	if( lpPanel == atNULL ) return atFALSE;

	atCONTROL_AddChildControl( lpPanel->hClientCtl, hChildControl );

	return atTRUE;
}

atVOID	atSCROLLPANEL_SetControlFocus( atHCONTROL hControl, atHCONTROL hChildControl )
{
	atLPSCROLLPANEL lpPanel = (atLPSCROLLPANEL)atCONTROL_GetDataPtr( hControl );
	if( lpPanel == atNULL ) return;

	atCONTROL_SetCurChildControl( lpPanel->hClientCtl, hChildControl );
}

atHCONTROL atSCROLLPANEL_GetControlFocus( atHCONTROL hControl )
{
	atLPSCROLLPANEL lpPanel = (atLPSCROLLPANEL)atCONTROL_GetDataPtr( hControl );
	if( lpPanel == atNULL ) return atNULL;

	return atCONTROL_GetCurChildControl( lpPanel->hClientCtl );
}

atBOOL	atSCROLLPANEL_SetScrollPos( atHCONTROL hControl, int pos )
{
	atLPSCROLLPANEL lpPanel = (atLPSCROLLPANEL)atCONTROL_GetDataPtr( hControl );
	if( lpPanel == atNULL ) return atFALSE;

//	if( pos < 0 || pos > atSCROLLBAR_EXT_GetSize(lpPanel->hScrollBar) ){
//		return atFALSE;
//	}

	lpPanel->pos = pos;
	
	atCONTROL_SetPosition( lpPanel->hClientCtl, 0, -pos );
	atSCROLLBAR_EXT_SetPosition( lpPanel->hScrollBar, pos );

	return atTRUE;
}

atINT	atSCROLLPANEL_GetScrollPos( atHCONTROL hControl )
{
	atLPSCROLLPANEL lpPanel = (atLPSCROLLPANEL)atCONTROL_GetDataPtr( hControl );
	if( lpPanel == atNULL ) return -1;

	return lpPanel->pos;
}

atBOOL	atSCROLLPANEL_SetClientSize(atHCONTROL hControl, int nClientWidth, int nClientHeight)
{
	atLPSCROLLPANEL lpPanel = (atLPSCROLLPANEL)atCONTROL_GetDataPtr( hControl );
	if( lpPanel == atNULL ) return atFALSE;

	lpPanel->nClientWidth  = nClientWidth;
	lpPanel->nClientHeight = nClientHeight;

	_RecalcSize(hControl);
	//atCONTROL_Refresh(hControl);

	return atTRUE;
}

atBOOL	atSCROLLPANEL_GetClientSize(atHCONTROL hControl, int* nClientWidth, int* nClientHeight)
{
	atLPSCROLLPANEL lpPanel = (atLPSCROLLPANEL)atCONTROL_GetDataPtr( hControl );
	if( lpPanel == atNULL ) return atFALSE;
	if( !nClientWidth && !nClientHeight)	return atFALSE;

	if(nClientWidth) *nClientWidth   = lpPanel->nClientWidth;
	if(nClientHeight) *nClientHeight = lpPanel->nClientHeight;

	return atTRUE;
}