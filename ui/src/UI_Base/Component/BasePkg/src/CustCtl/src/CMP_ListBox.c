#include "A_UIENG.h"
#include "UI_Util.h"
#include "CMP_ListBox.h"
#include "CMP_ScrollBar.h"


typedef struct _tagatLISTBOX {
	atDWORD      dwAttr;
	
	atLP_LISTBOX_DRAWITEM_FUNC fnDraw;
	atLP_LISTBOX_CHANGEITEM_FUNC fnChange;

	int			nSelectIndex;
	 
     	int			nItemHeight, nItemGap;
	int			nWidthMargin, nHeightMargin;
	
     	int			nOneScreenCount;

	 atBOOL			bFirstNO;
	 atINT			nCheckType;

	 atBOOL			bImageVision;
	
	 atUILIST		lpListBoxList;
	 atLPLISTBOXITEM	lpAddItemData;  // 아이템 추가 버퍼
	 atINT		nAddItemIndex;

	atHCONTROL		hScrollBar;

	atINT      		nCurCheckedIndex;
	atBOOL		bActionValidate;  // 마우스 액션을 위해.
} atLISTBOX,  *atLPLISTBOX;

static int ListBoxProc( atHCONTROL hControl, int nMsg, long lParam1, long lParam2 );


#define 	COLOR_SELECTBAR 		(atUIAPI_GetRGB(0,120,200))
#define		TICKER_INTERVAL		50
#define		IMAGE_WIDTH			16


struct _CreateParam{
	int nItemHeight;
	atBOOL bFirstNO;
	int nCheckType;
};


static atVOID _defDrawItem( atHGDC hCanvas, atLPCONTROL lpControl, atINT nIndex, atINT nX, atINT nY, atINT nWidth, atINT nHeight, atBOOL bFirstNo, atINT nCheckType, atLPLISTBOXITEM lpItem, atBOOL bSelected, atBOOL bFocused )
{
	atLPTSTR disp_str;
	int StrWidth;
	atTCHAR szBuff[atLISTBOX_STR_LEN];
	atCOLOR nTxtColor, nFrColor, nBgColor;
	int img_height;
	
	if( bSelected ){
		if( bFocused ){
			nBgColor = lpControl->tControlColor.clFocusedBgColor;
			nTxtColor = lpControl->tControlColor.clFocusedTxtColor;
			nFrColor = lpControl->tControlColor.clFocusedFrColor; 
		} else {
			nBgColor = lpControl->tControlColor.clDisableBgColor;
			nTxtColor = lpControl->tControlColor.clDisableTxtColor;
			nFrColor = lpControl->tControlColor.clDisableFrColor;
		}
	} else {
		nBgColor = lpControl->tControlColor.clNormalBgColor;
		nTxtColor = lpControl->tControlColor.clNormalTxtColor;
		nFrColor = lpControl->tControlColor.clNormalFrColor;
	}

	atUIAPI_FillRect(hCanvas, nX, nY, nWidth, nHeight, nBgColor );

	if( lpItem == atNULL ) return;
	
	nX += 2;
	
	// 체크박스 찍을 위치
	if ( nCheckType == 1 ){
		int len = nHeight - 2*2;
		if(lpItem->bChecked){
			atUIAPI_DrawRect( hCanvas, nX, nY+2, len, len, nFrColor, 1 );
			atUIAPI_DrawLine( hCanvas, nX, nY+2, nX+len-1, nY+2+len-1, nFrColor, 1, atFALSE );
			atUIAPI_DrawLine( hCanvas, nX, nY+2+len-1, nX+len-1, nY+2, nFrColor, 1, atFALSE );
		} else {
			atUIAPI_DrawRect( hCanvas, nX, nY+2, len, len, nFrColor, 1 );
		}
		nX += nHeight + 2;
	} else if ( nCheckType == 2 ){
		int ratio = (nHeight - 2*2) / 2;
		if(lpItem->bChecked){
			atUIAPI_DrawCircle( hCanvas, nX+ratio, nY+nHeight/2, ratio, nFrColor, 1 );
			atUIAPI_FillCircle( hCanvas, nX+ratio, nY+nHeight/2, 5, nFrColor );
		} else {
			atUIAPI_DrawCircle( hCanvas, nX+ratio, nY+nHeight/2, ratio, nFrColor, 1 );
		}
		nX += nHeight + 2;
	}

	// 숫자 찍을 위치
	if( bFirstNo ){
		UI_SPRINTF(szBuff, _AT("%d."), lpItem->nNO);
		atUIAPI_DrawStringAlign(hCanvas, nX, nY+nHeight/2, szBuff, ALIGN_LEFT_MIDDLE, nTxtColor);
		nX += atUIAPI_GetStringWidth(atUIAPI_GetFont(hCanvas), szBuff, -1) + 2;
	}

	// 첫 이미지 찍을 위치
	if( bSelected && (lpItem->imgItemImage[atLISTBOX_IMG_POST_SELECT] != atNULL) ){
		img_height = lpItem->imgItemImageRegion[atLISTBOX_IMG_POST_SELECT].nHeight;
		if( img_height > nHeight ) img_height = nHeight;
		atUIAPI_DrawImageAlignPart(hCanvas, nX, nY+nHeight/2, 
									lpItem->imgItemImage[atLISTBOX_IMG_POST_SELECT], 
									lpItem->imgItemImageRegion[atLISTBOX_IMG_POST_SELECT].nX, lpItem->imgItemImageRegion[atLISTBOX_IMG_POST_SELECT].nY, lpItem->imgItemImageRegion[atLISTBOX_IMG_POST_SELECT].nWidth, img_height,
									ALIGN_LEFT_MIDDLE );
	} else if( lpItem->imgItemImage[atLISTBOX_IMG_POST] != atNULL ){
		img_height = lpItem->imgItemImageRegion[atLISTBOX_IMG_POST].nHeight;
		if( img_height > nHeight ) img_height = nHeight;
		atUIAPI_DrawImageAlignPart(hCanvas, nX, nY+nHeight/2, 
									lpItem->imgItemImage[atLISTBOX_IMG_POST], 
									lpItem->imgItemImageRegion[atLISTBOX_IMG_POST].nX, lpItem->imgItemImageRegion[atLISTBOX_IMG_POST].nY, lpItem->imgItemImageRegion[atLISTBOX_IMG_POST].nWidth, img_height,
									ALIGN_LEFT_MIDDLE );
	}

	if(bSelected){	// List Item Select
		disp_str = lpItem->szItemName[1];
		if( UI_STRLEN(disp_str) <= 0 ) disp_str = lpItem->szItemName[0];
	} else { // List Item nonSelect
		disp_str = lpItem->szItemName[0];
	}
	
	StrWidth = nWidth - nX;
   	myStringCut( hCanvas, disp_str, szBuff, StrWidth, atTRUE);	
	atUIAPI_DrawStringAlign(hCanvas, nX, nY+nHeight/2, szBuff, ALIGN_LEFT_MIDDLE, nTxtColor);
}


atHCONTROL atLISTBOX_Create( int nIndex, int nX, int nY, int nWidth, int nHeight,
						  int nItemHeight, atBOOL bFirstNO, atINT nCheckType )
{
	atHCONTROL hControl;
	struct _CreateParam param;

	param.nItemHeight 	= nItemHeight;
	param.bFirstNO		= bFirstNO;
	param.nCheckType	= nCheckType;

	hControl = atCONTROL_Create( nIndex, nX, nY, nWidth, nHeight, CTL_PARENTTYPE_WINDOW, sizeof(atLISTBOX), ListBoxProc, &param );
//	if( hControl == atNULL ) return atNULL;

	return hControl;
}

void	atLISTBOX_SetFunc( atHCONTROL hControl, atLP_LISTBOX_DRAWITEM_FUNC fnDraw, atLP_LISTBOX_CHANGEITEM_FUNC fnChange )
{
	atLPLISTBOX lpListBox = (atLPLISTBOX)atCONTROL_GetDataPtr( hControl );
	if( lpListBox == atNULL ) return;
	
	lpListBox->fnDraw = fnDraw;
	lpListBox->fnChange = fnChange;
}


static void atLISTBOX_OnInit( atHCONTROL hControl, struct _CreateParam *param )
{
	atLPLISTBOX lpListBox = (atLPLISTBOX)atCONTROL_GetDataPtr( hControl );
	atRECT rect;
	int nWidth, nHeight;
	//	atLPCONTROL lpControl;
	
	lpListBox->lpListBoxList = atUIAPI_CreateList();
	lpListBox->lpAddItemData = atNULL;
	lpListBox->nAddItemIndex = -1;

	lpListBox->nSelectIndex 		=	-1;
//	lpListBox->nCount				=	0;//nMaxCount;
	lpListBox->nItemHeight			=	param->nItemHeight;

	lpListBox->bFirstNO 			=	param->bFirstNO;
	lpListBox->nCheckType			=	param->nCheckType;

	atCONTROL_GetControlRect( hControl, &rect );
	nWidth = RECT_WIDTH(rect);
	nHeight = RECT_HEIGHT(rect);
	
	lpListBox->nOneScreenCount	=	nHeight / param->nItemHeight;		

	lpListBox->nCurCheckedIndex = -1;
	lpListBox->bActionValidate = atFALSE;

	lpListBox->fnDraw = _defDrawItem;
	
		
//	lpListBox->hScrollBar = CreateScrollBar( hControl, 0, 0+nWidth-8, 0, 8, nHeight, 0, 0, 0, 0, atFALSE );
	lpListBox->hScrollBar = atSCROLLBAR_Create( 100, 0+nWidth-8, 0, 8, nHeight, 0, 0, 0, 0 );
	atCONTROL_SetAnchor( lpListBox->hScrollBar, 0, 0, atANCHOR_IGNORE, 0 );
	atCONTROL_SetAttr( lpListBox->hScrollBar, atCONTROL_GetAttr(lpListBox->hScrollBar) | CTL_PARENTTYPE_CONTROL );
	atCONTROL_SetFocusable(lpListBox->hScrollBar, atFALSE);
	atCONTROL_SetVisible( lpListBox->hScrollBar, atFALSE );
	atCONTROL_AddChildControl( hControl, lpListBox->hScrollBar );

	atCONTROL_SetSoftKey( hControl, _AT(""), _AT("Select"), _AT("") );
}

static void atLISTBOX_OnRelease( atHCONTROL hControl )
{	
	atLPLISTBOX lpListBox = (atLPLISTBOX)atCONTROL_GetDataPtr( hControl );
	if( lpListBox == atNULL ) return;

	if( lpListBox->lpAddItemData ){
		UI_MEMFREE( lpListBox->lpAddItemData );
		lpListBox->lpAddItemData = atNULL;
	}

	atUIAPI_DestroyList(lpListBox->lpListBoxList, atTRUE);
	lpListBox->lpListBoxList = atNULL;

	if( lpListBox->hScrollBar ){
		atCONTROL_Destroy( lpListBox->hScrollBar );
		lpListBox->hScrollBar = atNULL;
	}
}

static void atLISTBOX_OnDraw( atHGDC hCanvas, atHCONTROL hControl, atBOOL bFocused )
{
	atLPCONTROL 	lpControl = atCONTROL_GetPtr(hControl);
	atLPLISTBOX lpListBox = (atLPLISTBOX)atCONTROL_GetDataPtr(hControl);
	int i;	
	int x, y, w, h;
	atLPLISTBOXITEM	lpItem		= atNULL;
	int nCurPage = 0, nCurIndex = 0, nTotalPage = 1;
	atLPCONTROL		lpControl2;
	
	// 표시 좌표 보정
	x = 0;//lpControl->Base.rect.nStartX;
	y = 0;//lpControl->Base.rect.nStartY;
	w = RECT_WIDTH(lpControl->Base.rect);
	h = RECT_HEIGHT(lpControl->Base.rect);
	
	if ( atCONTROL_IsVisible(lpListBox->hScrollBar) ){
		lpControl2 = atCONTROL_GetPtr(lpListBox->hScrollBar);
		w -= RECT_WIDTH(lpControl2->Base.rect);
	}

//	atUIAPI_FillRect( hCanvas, x, y, w, h, lpControl->clNormalBgColor );  // 배경을 지운다.
	
	// 아이템이 하나도 없으면 여기서 중지.
	if( lpListBox->lpListBoxList == atNULL ) return;
	if( atUIAPI_GetListCount(lpListBox->lpListBoxList) <= 0 ) return;
	if( lpListBox->nOneScreenCount <= 0 ) return;

	if( lpListBox->fnDraw == atNULL ) return;
	
	lpItem	= (atLPLISTBOXITEM)atUIAPI_GetListHead( lpListBox->lpListBoxList, atNULL );
	if( lpItem == atNULL ) return;

	nCurPage = lpListBox->nSelectIndex / lpListBox->nOneScreenCount;
	nCurIndex = lpListBox->nSelectIndex % lpListBox->nOneScreenCount;

	for( i = 0; i < lpListBox->nOneScreenCount; i++ ){
		int idx = nCurPage * lpListBox->nOneScreenCount + i;
		int nTempHeight = y + lpListBox->nItemHeight*i;//-nCurPage);
		
		lpItem = (atLPLISTBOXITEM)atUIAPI_GetListAt(lpListBox->lpListBoxList, idx);
//		if (lpItem == atNULL) break;

		if( lpItem ){
			if(idx == lpListBox->nSelectIndex){	// List Item Select
				// Select Bar Draw
				lpListBox->fnDraw(hCanvas, lpControl, idx, x, nTempHeight, w, lpListBox->nItemHeight, lpListBox->bFirstNO, lpListBox->nCheckType, lpItem, atTRUE, bFocused );
			} else {
				lpListBox->fnDraw(hCanvas, lpControl, idx, x, nTempHeight, w, lpListBox->nItemHeight, lpListBox->bFirstNO, lpListBox->nCheckType, lpItem, atFALSE, bFocused );
			}
		} else {
			lpListBox->fnDraw(hCanvas, lpControl, idx, x, nTempHeight, w, lpListBox->nItemHeight, lpListBox->bFirstNO, lpListBox->nCheckType, atNULL, atFALSE, bFocused );
		}
	}
	
//	if(atCONTROL_IsVisible(lpListBox->hScrollBar) == atTRUE)	{
//		atCONTROL_Draw( lpListBox->hScrollBar, atFALSE );
//	}
}

static int atLISTBOX_OnKeyDown( atHCONTROL hControl, int nKeyCode )
{
	atLPLISTBOX lpListBox	= (atLPLISTBOX)atCONTROL_GetDataPtr( hControl );
	int r = 0;
	atLPLISTBOXITEM lpItem;

	switch( nKeyCode ){
		case atVKEY_UP:
		case atVKEY_REW:
			if( lpListBox->nSelectIndex <= 0 ){
				if( lpListBox->dwAttr & atLISTBOX_ATTR_MOVE_NEXTCONTROL ){
					return 0;
				}
			}
			return 1;
			break;
		case atVKEY_DOWN:
		case atVKEY_FF:
			if( lpListBox->nSelectIndex >= atLISTBOX_GetCount(hControl)-1 ){
				if( lpListBox->dwAttr & atLISTBOX_ATTR_MOVE_NEXTCONTROL ){
					return 0;
				}
			}
			return 1;
			break;

		case atVKEY_SOFT2:
			if( lpListBox->nCheckType == 1 ){
				lpItem = atLISTBOX_GetSelectItem(hControl);
				if( lpItem ){
					atBOOL bChecked = atLISTBOX_ToggleSelectItemChecked(hControl);
//					atCONTROL_Draw(hControl, atTRUE);
					atCONTROL_Refresh( hControl );
					if( bChecked )
						atCONTROL_SetSoftKey(hControl, atNULL, atNULL, _AT("UnChk"));
					else
						atCONTROL_SetSoftKey(hControl, atNULL, atNULL, _AT("Check"));
					atSOFTKEY_RefreshSoftKey();
					r = 1;
				}
			} if( lpListBox->nCheckType == 2 ){
				lpItem = atLISTBOX_GetSelectItem(hControl);
				if( lpItem ){
					atLISTBOX_SetItemCheck(hControl, atLISTBOX_GetSelectIndex(hControl), atTRUE);
//					atCONTROL_Draw(hControl, atTRUE);
					atCONTROL_Refresh( hControl );
					atCONTROL_SetSoftKey(hControl, atNULL, atNULL, _AT(""));
					atSOFTKEY_RefreshSoftKey();
					r = 1;
				}
			}
			break;
		case atVKEY_SELECT:
			if( lpListBox->nSelectIndex >= 0 ){
				if( lpListBox->nCheckType == 2 ){
					lpItem = atLISTBOX_GetSelectItem(hControl);
					if( lpItem ){
						atLISTBOX_SetItemCheck(hControl, atLISTBOX_GetSelectIndex(hControl), atTRUE);
//						atCONTROL_Draw(hControl, atTRUE);
						atCONTROL_Refresh( hControl );
						atCONTROL_SetSoftKey(hControl, atNULL, atNULL, _AT(""));
						atSOFTKEY_RefreshSoftKey();
					}
				}
				if( lpListBox->bFirstNO ){
					lpItem = atLISTBOX_GetSelectItem(hControl);
					atCONTROL_PostMessageToParent( hControl, WNDMSG_COMMAND, (long)atCONTROL_GetID(hControl), (long)lpItem->nNO );
				} else {
					atCONTROL_PostMessageToParent( hControl, WNDMSG_COMMAND, (long)atCONTROL_GetID(hControl), (long)lpListBox->nSelectIndex );
				}
				r = 1;
			}
			break;
	}

    return r;
}

static int atLISTBOX_OnKeyPress( atHCONTROL hControl, int nKeyCode )
{
	atLPLISTBOX lpListBox	= (atLPLISTBOX)atCONTROL_GetDataPtr( hControl );
	int r = 0;
	atLPLISTBOXITEM lpItem;

	switch( nKeyCode ){
		case atVKEY_UP:
		case atVKEY_REW:
			atLISTBOX_MoveSelect(hControl, -1);
//			atCONTROL_Draw(hControl, atTRUE);
			if( lpListBox->nCheckType == 1 ){
				lpItem = atLISTBOX_GetSelectItem(hControl);
				if( lpItem ){
					if( lpItem->bChecked )
						atCONTROL_SetSoftKey(hControl, atNULL, atNULL, _AT("UnChk"));
					else
						atCONTROL_SetSoftKey(hControl, atNULL, atNULL, _AT("Check"));
					atSOFTKEY_RefreshSoftKey();
				}
			} else if( lpListBox->nCheckType == 2 ){
				lpItem = atLISTBOX_GetSelectItem(hControl);
				if( lpItem ){
					if( lpItem->bChecked )
						atCONTROL_SetSoftKey(hControl, atNULL, atNULL, _AT(""));
					else
						atCONTROL_SetSoftKey(hControl, atNULL, atNULL, _AT("Select"));
					atSOFTKEY_RefreshSoftKey();
				}
			}
			atCONTROL_Refresh( hControl );
			r = 1;
			break;
		case atVKEY_DOWN:
		case atVKEY_FF:
			atLISTBOX_MoveSelect(hControl, 1);
//			atCONTROL_Draw(hControl, atTRUE);
			if( lpListBox->nCheckType == 1 ){
				lpItem = atLISTBOX_GetSelectItem(hControl);
				if( lpItem ){
					if( lpItem->bChecked )
						atCONTROL_SetSoftKey(hControl, atNULL, atNULL, _AT("UnChk"));
					else
						atCONTROL_SetSoftKey(hControl, atNULL, atNULL, _AT("Check"));
				}
			} else if( lpListBox->nCheckType == 2 ){
				lpItem = atLISTBOX_GetSelectItem(hControl);
				if( lpItem ){
					if( lpItem->bChecked )
						atCONTROL_SetSoftKey(hControl, atNULL, atNULL, _AT(""));
					else
						atCONTROL_SetSoftKey(hControl, atNULL, atNULL, _AT("Select"));
				}
			}
			atCONTROL_Refresh( hControl );
			r = 1;
			break;
	}

	return r;
}

static void _Realloc_Items( atHCONTROL hControl )
{
	atLPCONTROL	lpControl = atCONTROL_GetPtr(hControl);
	atLPLISTBOX lpListBox = (atLPLISTBOX)atCONTROL_GetDataPtr(hControl);
//	atLPTSTR szSelStr;
	atRECT rect;
//int xxx, yyy;
int nHeight;
int cnt;
int show_sb = 0;

	atCONTROL_GetControlRect( hControl, &rect );

	nHeight = RECT_HEIGHT(lpControl->Base.rect);


//	if(RECT_HEIGHT(lpControl->Base.rect) < atLISTBOX_GetCount(lpListBox) * lpListBox->nItemHeight) 
	{		
		lpListBox->nOneScreenCount			=	nHeight / lpListBox->nItemHeight;		
//		lpListBox->hScrollBar			=	atLISTBOX_GetCount(hControl)-lpListBox->nOneScreenCount;
//		lpListBox->ScrollBar.nCount			=	atLISTBOX_GetCount(hControl)-lpListBox->nOneScreenCount;
	}
//	else 
	{			
//		lpListBox->nOneScreenCount			=	atLISTBOX_GetCount(hControl);
	}	

	
	cnt = atUIAPI_GetListCount(lpListBox->lpListBoxList);
	if( cnt <= 0 ){
		if( lpListBox->hScrollBar != atNULL )
			atCONTROL_SetVisible( lpListBox->hScrollBar, atFALSE );
		return;
	}

	if( cnt > lpListBox->nOneScreenCount ){
		if( !atCONTROL_IsVisible(lpListBox->hScrollBar) ){
//			atRECT r;
//			atCONTROL_GetControlRect( lpListBox->hScrollBar, &r );
//			atCONTROL_SetControlRegion( lpListBox->hScrollBar, RECT_WIDTH(rect)-RECT_WIDTH(r), 0, RECT_WIDTH(r), RECT_HEIGHT(rect) );
			atCONTROL_SetVisible( lpListBox->hScrollBar, atTRUE );
		}
show_sb = 1;
	}

}


static int ListBoxProc( atHCONTROL hControl, int nMsg, long lParam1, long lParam2 )
{
	atLPCONTROL lpControl = atCONTROL_GetPtr(hControl);
	atLPLISTBOX lpListBox	= (atLPLISTBOX)atCONTROL_GetDataPtr( hControl );
	atLPLISTBOXITEM lpItem;
//	atRECT rect;
//	int oldidx, newidx;
	int x, y;
	int nFirstIdx;
	int r = 0;

	switch( nMsg ){
		case CTLMSG_INIT:
			atLISTBOX_OnInit( hControl, (struct _CreateParam *)lParam1 );
			r = 1;
			break;
		case CTLMSG_RELEASE:
			atLISTBOX_OnRelease( hControl );
			r = 1;
			break;
		case CTLMSG_CHANGE:
			if( lParam1 == 100 ){
				atLISTBOX_SetSelectIndex( hControl, lParam2 );
//				atCONTROL_Draw(hControl, atTRUE);
				atCONTROL_Refresh( hControl );
				r = 1;
			}
			break;
		case CTLMSG_RESIZE:
			_Realloc_Items(hControl);
			r = 1;
			break;
		case CTLMSG_PAINT:
			atLISTBOX_OnDraw( atCONTROL_GetGDC(hControl), hControl, (atBOOL)lParam2 );
			r = 1;
			break;
		case CTLMSG_GOTFOCUS:
			if( lpListBox->nCheckType ){
				lpItem = atLISTBOX_GetSelectItem(hControl);
				if( lpItem ){
					if( lpItem->bChecked )
						atCONTROL_SetSoftKey(hControl, atNULL, atNULL, _AT("UnChk"));
					else
						atCONTROL_SetSoftKey(hControl, atNULL, atNULL, _AT("Check"));
					atSOFTKEY_RefreshSoftKey();
					r = 1;
				}
			}
			break;
		case CTLMSG_LOSTFOCUS:
			break;
			
		case CTLMSG_KEYDOWN:
			r = atLISTBOX_OnKeyDown( hControl, lParam1 );
			break;
		case CTLMSG_KEYPRESS:
			r = atLISTBOX_OnKeyPress( hControl, lParam1 );
			break;
					
		case CTLMSG_MOUSEDOWN:
			x = lParam2 >> 16;
			y = lParam2 & 0xffff;
			if( atCONTROL_IsVisible(lpListBox->hScrollBar) ){
				atRECT rect;
				atCONTROL_GetControlRect(lpListBox->hScrollBar, &rect);
				if( x >= rect.nStartX && x <= rect.nEndX && y >= rect.nStartY && y <= rect.nEndY ){
					atCONTROL_ProcessControlProc( lpListBox->hScrollBar, CTLMSG_MOUSEDOWN, lParam1, ((x-rect.nStartX) << 16) | (y-rect.nStartY) );
					return 1; // 일단은... 패스... --;
				}
			}
			nFirstIdx = lpListBox->nSelectIndex / lpListBox->nOneScreenCount * lpListBox->nOneScreenCount;
			lpListBox->bActionValidate = atLISTBOX_SetSelectIndex( hControl, nFirstIdx + y / lpListBox->nItemHeight );
			if( lpListBox->bActionValidate ){
//				atCONTROL_Draw(hControl, atTRUE);
				atCONTROL_Refresh( hControl );
			}
			r = 1;
			break;

		case CTLMSG_MOUSEUP:
			if( lpListBox->bActionValidate ){
				x = lParam2 >> 16;
				y = lParam2 & 0xffff;
				lpListBox->bActionValidate = atFALSE;
				atCONTROL_ProcessControlProc( hControl, CTLMSG_KEYDOWN, atVKEY_SELECT, 0 );
			}
			r = 1;
			break;
	}

	return r;
}


//==============================================================================================


void atLISTBOX_ClearAllItem( atHCONTROL hControl )
{
	atLPCONTROL 	lpControl = atCONTROL_GetPtr(hControl);
	atLPLISTBOX lpListBox = (atLPLISTBOX)atCONTROL_GetDataPtr( hControl );

	atUIAPI_RemoveListAll(lpListBox->lpListBoxList, atTRUE);

	lpListBox->nSelectIndex = -1;
	atCONTROL_SetVisible( lpListBox->hScrollBar, atFALSE );
}

atBOOL atLISTBOX_SetSelectIndex(atHCONTROL hControl, int index)
{
	atLPCONTROL	lpControl = atCONTROL_GetPtr(hControl);
	atLPLISTBOX lpListBox = (atLPLISTBOX)atCONTROL_GetDataPtr(hControl);
	atLPLISTBOXITEM		itm;

	if( index < 0 ) return atFALSE;
	if( index >= atUIAPI_GetListCount(lpListBox->lpListBoxList) ) return atFALSE;

	if( lpListBox->nSelectIndex == index ) return atTRUE;
	
	itm = atLISTBOX_GetItem(hControl, index);
	if( !itm->bEnable ) return atFALSE;

	lpListBox->nSelectIndex		= index;

	atSCROLLBAR_SetPosition( lpListBox->hScrollBar, lpListBox->nSelectIndex );

	_Realloc_Items( hControl );

	atCONTROL_PostMessageToParent( hControl, WNDMSG_CHANGE, (long)lpControl->Base.nID, (long)lpListBox->nSelectIndex );

	return atTRUE;
}

atBOOL atLISTBOX_SetSelectIndexByPage(atHCONTROL hControl, int page)
{
	atLPLISTBOX lpListBox = (atLPLISTBOX)atCONTROL_GetDataPtr(hControl);
	int index;

	index = page * lpListBox->nOneScreenCount;

	return atLISTBOX_SetSelectIndex( hControl, index );
}

int atLISTBOX_MoveSelect(atHCONTROL hControl, int nDirect)
{
	atLPCONTROL	lpControl = atCONTROL_GetPtr(hControl);
	atLPLISTBOX lpListBox = (atLPLISTBOX)atCONTROL_GetDataPtr(hControl);
	atLPLISTBOXITEM		lpItem;
	int stidx, maxidx, curidx;
//	atLPTSTR szSelStr;
	
	maxidx = atUIAPI_GetListCount(lpListBox->lpListBoxList)-1;
	if( maxidx < 0 ) return -1;

	stidx = lpListBox->nSelectIndex;
	if( stidx == -1 )
	{
		stidx = (nDirect > 0 )? 0 : maxidx;
	}

	curidx = stidx;
	do {
		curidx += nDirect;
		
		if( nDirect < 0 && curidx < 0) 
			curidx = maxidx;
		if( nDirect > 0 && curidx > maxidx) 
			curidx = 0;
		
		lpItem = atUIAPI_GetListAt(lpListBox->lpListBoxList, curidx);
		
		if( lpItem && lpItem->bEnable ) break;
	} while( curidx != stidx );

	if( curidx == stidx )
	{
		lpItem = atUIAPI_GetListAt(lpListBox->lpListBoxList, curidx);
		if( lpItem && !lpItem->bEnable ) curidx = -1;
	}

	atLISTBOX_SetSelectIndex( hControl, curidx );

	return lpListBox->nSelectIndex;
}

atBOOL	atLISTBOX_ToggleSelectItemChecked(atHCONTROL hControl)
{
	atLPLISTBOX lpListBox = (atLPLISTBOX)atCONTROL_GetDataPtr( hControl );
	atLPLISTBOXITEM itm;

	if( lpListBox->nCheckType != 1 ) return atFALSE;

	itm = atLISTBOX_GetSelectItem(hControl);
	itm->bChecked = !itm->bChecked;

	if( itm->bChecked ){
		lpListBox->nCurCheckedIndex = atLISTBOX_GetSelectIndex(hControl);
	}
	
	return itm->bChecked;
}

void	atLISTBOX_SetItemCheck(atHCONTROL hControl, int nIndex, atBOOL bCheck)
{
	atLPLISTBOX lpListBox = (atLPLISTBOX)atCONTROL_GetDataPtr( hControl );
	int pos;
	atLPLISTBOXITEM itm, itm2;

	if( lpListBox == atNULL ) return;
	if( (lpListBox->nCheckType != 1) && (lpListBox->nCheckType != 2) ) return;

	itm = atLISTBOX_GetItem(hControl, nIndex);
	if( itm == atNULL ) return;
	if( itm->bChecked == bCheck ) return;

	if( lpListBox->nCheckType == 1 ){
		itm->bChecked = bCheck;
	} else if( lpListBox->nCheckType == 2 ){
		if( !bCheck ) return;
        itm2 = (atLPLISTBOXITEM)atUIAPI_GetListHead( lpListBox->lpListBoxList, &pos );
		while( itm2 ){
		   if( itm != itm2 ) itm2->bChecked = atFALSE;
	       itm2 = (atLPLISTBOXITEM)atUIAPI_GetListNext( lpListBox->lpListBoxList, &pos );
		}
		itm->bChecked = atTRUE;
	}
	
	if( bCheck ) lpListBox->nCurCheckedIndex = nIndex;
}

atBOOL atLISTBOX_GetItemChecked(atHCONTROL hControl, int nIndex)
{
	atLPLISTBOX lpListBox = (atLPLISTBOX)atCONTROL_GetDataPtr( hControl );
	atLPLISTBOXITEM itm;

	if( lpListBox == atNULL ) return atFALSE;
	if( lpListBox->nCheckType != 1 ) return atFALSE;

	itm = atLISTBOX_GetItem(hControl, nIndex);

	return (itm)? itm->bChecked : atFALSE;	
}

atINT atLISTBOX_GetCurCheckedIndex(atHCONTROL hControl)
{
	atLPLISTBOX lpListBox = (atLPLISTBOX)atCONTROL_GetDataPtr( hControl );

	if( lpListBox == atNULL ) return -1;
	if( (lpListBox->nCheckType != 1) && (lpListBox->nCheckType != 2) ) return -1;

	return lpListBox->nCurCheckedIndex;
}

void	atLISTBOX_SetItemEnabled(atHCONTROL hControl, int nIndex, atBOOL bEnable)
{
	atLPLISTBOX lpListBox = (atLPLISTBOX)atCONTROL_GetDataPtr( hControl );
	atLPLISTBOXITEM itm;

	itm = atLISTBOX_GetItem(hControl, nIndex);
	
	if( itm ) itm->bEnable = bEnable;
	if( !bEnable ){
		if( nIndex == atLISTBOX_GetSelectIndex(hControl) )
			atLISTBOX_MoveSelect( hControl, 1 );
	}
}

atBOOL atLISTBOX_GetItemEnabled(atHCONTROL hControl, int nIndex)
{
	atLPLISTBOX lpListBox = (atLPLISTBOX)atCONTROL_GetDataPtr( hControl );
	atLPLISTBOXITEM itm;

	itm = atLISTBOX_GetItem(hControl, nIndex);

	return (itm)? itm->bEnable : atFALSE;
	
}

/*
void atLISTBOX_Resize( atHCONTROL hControl, int nX, int nY, int nWidte, int nHeight )
{
	atLPCONTROL lpControl = atCONTROL_GetPtr(hControl);
	atLPLISTBOX lpListBox = (atLPLISTBOX)atCONTROL_GetDataPtr( hControl );

	lpControl->Base.rect.nStartX	=	nX;
	lpControl->Base.rect.nStartY	=	nY;	
	lpControl->Base.rect.nEndX	= nX + nWidte;
	lpControl->Base.rect.nEndY = nY + nHeight;

	_Realloc_Items(hControl);
}
*/


/******아이템 추가 함수*********/
void atLISTBOX_Start_AddItem(atHCONTROL hControl)
{	
	atLISTBOX_Start_AddItemEx( hControl, -1 );
}

void atLISTBOX_Start_AddItemEx(atHCONTROL hControl, atINT nIndex)
{	
	atLPLISTBOX lpListBox = (atLPLISTBOX)atCONTROL_GetDataPtr( hControl );
	atLPLISTBOXITEM		lpData	= atNULL;
	
	lpListBox->nAddItemIndex = nIndex;
	lpListBox->lpAddItemData = (atLPLISTBOXITEM)UI_MEMALLOC(sizeof(atLISTBOXITEM));
	lpData	= lpListBox->lpAddItemData;
	atUIAPI_memset(lpData, 0, sizeof(atLISTBOXITEM));
	lpData->bEnable = atTRUE;
	lpData->nNO = 0;
	lpData->data = atNULL;
}

void atLISTBOX_AddItem_Enabled(atHCONTROL hControl, atBOOL bEnabled)
{
	atLPLISTBOX lpListBox = (atLPLISTBOX)atCONTROL_GetDataPtr( hControl );
	atLPLISTBOXITEM		lpData	= lpListBox->lpAddItemData;

	lpData->bEnable	= bEnabled;
}

void atLISTBOX_AddItem_Checked(atHCONTROL hControl, atBOOL bChecked)
{
	atLPLISTBOX lpListBox = (atLPLISTBOX)atCONTROL_GetDataPtr( hControl );
	atLPLISTBOXITEM		lpData	= lpListBox->lpAddItemData;

	lpData->bChecked	= bChecked;
}

void atLISTBOX_AddItem_NO(atHCONTROL hControl, int nNO)
{
	atLPLISTBOX lpListBox = (atLPLISTBOX)atCONTROL_GetDataPtr( hControl );
	atLPLISTBOXITEM		lpData	= lpListBox->lpAddItemData;

	lpData->nNO	= nNO;
}

void atLISTBOX_AddItem_String(atHCONTROL hControl, atLPTSTR szItemName, int nIndex) 
{
	atLPLISTBOX lpListBox = (atLPLISTBOX)atCONTROL_GetDataPtr( hControl );
	atLPLISTBOXITEM		lpData	= lpListBox->lpAddItemData;

	UI_STRNCPY(lpData->szItemName[nIndex], szItemName, atLISTBOX_STR_LEN-1);
	lpData->szItemName[nIndex][atLISTBOX_STR_LEN-1] = _ATC('\0');
}

void atLISTBOX_AddItem_Data(atHCONTROL hControl, atVOID*data) 
{
	atLPLISTBOX lpListBox = (atLPLISTBOX)atCONTROL_GetDataPtr( hControl );
	atLPLISTBOXITEM		lpData	= lpListBox->lpAddItemData;

	lpData->data = data;
}

void atLISTBOX_AddItem_Image(atHCONTROL hControl, atHBITMAP imgListImage, int nIndex)
{
	atLPLISTBOX lpListBox = (atLPLISTBOX)atCONTROL_GetDataPtr( hControl );
	atLPLISTBOXITEM		lpData	= lpListBox->lpAddItemData;

	lpData->imgItemImage[nIndex]		= imgListImage;
	if( imgListImage == atNULL ) return;
	lpData->imgItemImageRegion[nIndex].nX = 0;
	lpData->imgItemImageRegion[nIndex].nY = 0;
	lpData->imgItemImageRegion[nIndex].nWidth = atUIAPI_GetBitmapWidth(imgListImage);
	lpData->imgItemImageRegion[nIndex].nHeight = atUIAPI_GetBitmapHeight(imgListImage);
}

void atLISTBOX_AddItem_Image2(atHCONTROL hControl, atHBITMAP imgListImage, int x, int y, int width, int height, int nIndex)
{
	atLPLISTBOX lpListBox = (atLPLISTBOX)atCONTROL_GetDataPtr( hControl );
	atLPLISTBOXITEM		lpData	= lpListBox->lpAddItemData;

	lpData->imgItemImage[nIndex]		= imgListImage;
	if( imgListImage == atNULL ) return;
	lpData->imgItemImageRegion[nIndex].nX = x;
	lpData->imgItemImageRegion[nIndex].nY = y;
	lpData->imgItemImageRegion[nIndex].nWidth = (width>=0)? width : atUIAPI_GetBitmapWidth(imgListImage);
	lpData->imgItemImageRegion[nIndex].nHeight = (height>=0)? height : atUIAPI_GetBitmapHeight(imgListImage);
}

void atLISTBOX_Update_AddItem(atHCONTROL hControl)
{
	int idx;
	atLPLISTBOX lpListBox = (atLPLISTBOX)atCONTROL_GetDataPtr( hControl );

	if( lpListBox->nAddItemIndex >= 0 ){
		idx = atUIAPI_InsertListBefore(lpListBox->lpListBoxList, lpListBox->nAddItemIndex, lpListBox->lpAddItemData);
	} else {
		idx = atUIAPI_AddListTail(lpListBox->lpListBoxList, lpListBox->lpAddItemData);
	}

	if( lpListBox->nSelectIndex == -1 ){
		if( lpListBox->lpAddItemData->bEnable )
		  atLISTBOX_SetSelectIndex( hControl, idx );
	}

	if(  lpListBox->lpAddItemData->bChecked ){
		if( lpListBox->nCurCheckedIndex == -1 ) 
			lpListBox->nCurCheckedIndex = idx;
	}

	atSCROLLBAR_SetSize( lpListBox->hScrollBar, atUIAPI_GetListCount(lpListBox->lpListBoxList) );

	lpListBox->lpAddItemData = atNULL;

	_Realloc_Items(hControl);
}


void    atLISTBOX_DeleteItem(atHCONTROL hControl, int nIndex)
{   
	atLPLISTBOX lpListBox = (atLPLISTBOX)atCONTROL_GetDataPtr( hControl );
	int cnt;

    atUIAPI_RemoveListAt(lpListBox->lpListBoxList, nIndex/*atUIAPI_GetListAt(lpListBox->lpListBoxList, nIndex)*/, atTRUE);

	cnt = atUIAPI_GetListCount(lpListBox->lpListBoxList);

	if( lpListBox->nSelectIndex >= cnt ) lpListBox->nSelectIndex = cnt - 1;

	if( nIndex == lpListBox->nCurCheckedIndex ){
		lpListBox->nCurCheckedIndex = -1;
	}
	
	_Realloc_Items(hControl);
}

int atLISTBOX_GetCount(atHCONTROL hControl)
{
	atLPCONTROL lpControl = atCONTROL_GetPtr(hControl);
	atLPLISTBOX lpListBox = (atLPLISTBOX)atCONTROL_GetDataPtr( hControl );

	return atUIAPI_GetListCount(lpListBox->lpListBoxList);
}

int atLISTBOX_GetSelectIndex(atHCONTROL hControl)
{
	atLPCONTROL lpControl = atCONTROL_GetPtr(hControl);
	atLPLISTBOX lpListBox = (atLPLISTBOX)atCONTROL_GetDataPtr( hControl );

	return lpListBox->nSelectIndex;
}

atLPLISTBOXITEM atLISTBOX_GetItem(atHCONTROL hControl, int nIndex)
{
	atLPCONTROL lpControl = atCONTROL_GetPtr(hControl);
	atLPLISTBOX lpListBox = (atLPLISTBOX)atCONTROL_GetDataPtr( hControl );
	return (atLPLISTBOXITEM)atUIAPI_GetListAt( lpListBox->lpListBoxList, nIndex );
}

atLPLISTBOXITEM atLISTBOX_GetSelectItem(atHCONTROL hControl)
{
	return atLISTBOX_GetItem( hControl, atLISTBOX_GetSelectIndex(hControl) );
}

atVOID atLISTBOX_RealizeItem(atHCONTROL hControl)
{
	_Realloc_Items(hControl);
}

atLPTSTR atLISTBOX_GetItemString(atHCONTROL hControl, int nIndex, int nStringIndex)
{
	atLPLISTBOX lpListBox = (atLPLISTBOX)atCONTROL_GetDataPtr( hControl );
	atLPLISTBOXITEM lpListBoxItem= atUIAPI_GetListAt( lpListBox->lpListBoxList, nIndex );
	
	return lpListBoxItem->szItemName[nStringIndex];
}

atVOID* atLISTBOX_GetItemData(atHCONTROL hControl, int nIndex)
{
	atLPLISTBOX lpListBox = (atLPLISTBOX)atCONTROL_GetDataPtr( hControl );
	atLPLISTBOXITEM lpListBoxItem= atUIAPI_GetListAt( lpListBox->lpListBoxList, nIndex );

	if( lpListBoxItem )
		return lpListBoxItem->data;
	else
		return atNULL;
}

atVOID* atLISTBOX_GetCurItemData(atHCONTROL hControl)
{
	return atLISTBOX_GetItemData(hControl, atLISTBOX_GetSelectIndex(hControl));
}

