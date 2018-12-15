//
// MenuWnd.c
//
///////////////////////////////////////////////////////////////////////

#include "A_UIENG.h"
#include "UI_Util.h"
#include "CMP_RadioGroupEx.h"
#include "CMP_ScrollBarEx.h"
#include <string.h>


#define PREALLOC_RADIOGROUP_ITEM_COUNT 10


// Grouping by nGroupID
typedef struct _MYRADIOGROUP
{
    int			nItemHeight;
	int			nOneScreenCount;

	atHBITMAP		lpNormalImage;
	atHBITMAP		lpSelectImage;

//	char		szItems[MAX_RADIOGROUP_ITEMS_COUNT][32];
	atTCHAR		(*szItems)[32];
	int			nAllocItemCount;
	int			nItemCount;
	int			horiz_item_count;
	int			horiz_gap, line_gap;
		
 	int			nSelectIndex;
 	int			nCursorIndex;

	atHCONTROL	hScrollBar;

	int			nOrgSelectIndex;
	atBOOL		bMouseDown;
} MYRADIOGROUP, * LPMYRADIOGROUP;

static int RadioGroupProc( atHCONTROL hControl, int nMsg, long lParam1, long lParam2 );



typedef struct {
	int nItemHeight;
	int horiz_item_count;
	int horiz_gap, line_gap;
	atHBITMAP lpNormalImage, lpSelectImage;
	//atHBITMAP lpScrollbarImgTop, lpScrollbarImage, lpScrollbarImgBottom, lpScrollcursorImage;
	atHBITMAP lpScrollbarImgTop, lpScrollbarImage, lpScrollbarImgBottom;
	atHBITMAP lpScrollCursorImgTop, lpScrollCursorImgMid, lpScrollCursorImgBot;
} _CreateParam;

#define COLOR_SELECTBAR (atUIAPI_GetRGB(0,120,200))


atHCONTROL atRADIOGROUP_EXT_Create( int nIndex, int nX, int nY, int nWidth, int nHeight, 
									atTCHAR (*szButtonMsg)[32], int ButtonCnt, int init_idx, int nItemHeight,
									int horiz_item_count, int horiz_gap, int line_gap,
									atHBITMAP lpNormalImage, atHBITMAP lpSelectImage,
						  			//atHBITMAP lpScrollbarImgTop, atHBITMAP lpScrollbarImage, atHBITMAP lpScrollbarImgBottom, atHBITMAP lpScrollcursorImage )
									atHBITMAP lpScrollbarImgTop, atHBITMAP lpScrollbarImage, atHBITMAP lpScrollbarImgBottom,
									atHBITMAP lpScrollCursorImgTop, atHBITMAP lpScrollCursorImgMid, atHBITMAP lpScrollCursorImgBot )
{
	atHCONTROL		hControl;
	_CreateParam param;

	param.nItemHeight = nItemHeight;
	param.horiz_item_count = horiz_item_count;
	param.horiz_gap = horiz_gap;
	param.line_gap = line_gap;
	param.lpNormalImage	= lpNormalImage;
	param.lpSelectImage	= lpSelectImage;
    param.nItemHeight = nItemHeight;
	param.lpScrollbarImgTop	= lpScrollbarImgTop;
	param.lpScrollbarImage	= lpScrollbarImage;
	param.lpScrollbarImgBottom	= lpScrollbarImgBottom;
	//param.lpScrollcursorImage	= lpScrollcursorImage;
	param.lpScrollCursorImgTop = lpScrollCursorImgTop;
	param.lpScrollCursorImgMid = lpScrollCursorImgMid;
	param.lpScrollCursorImgBot = lpScrollCursorImgBot;

	hControl = atCONTROL_Create( nIndex, nX, nY, nWidth, nHeight, CTL_PARENTTYPE_WINDOW, sizeof(MYRADIOGROUP), RadioGroupProc, &param );
	if( hControl == atNULL ) return atNULL;

	{
//		atLPCONTROL 	lpControl = atCONTROL_GetPtr(hControl);
		LPMYRADIOGROUP	lpRadioGrp = atCONTROL_GetDataPtr( hControl );
		
		if( init_idx >= ButtonCnt ) init_idx = -1;
	 	lpRadioGrp->nSelectIndex = init_idx;
		lpRadioGrp->nCursorIndex = (init_idx >= 0)? init_idx : 0;

		if( (szButtonMsg != atNULL) && (ButtonCnt > 0) ){
			int i;

			lpRadioGrp->szItems = (atTCHAR (*)[32])UI_MEMALLOC(ButtonCnt * 32 * sizeof(atTCHAR));
			lpRadioGrp->nAllocItemCount = ButtonCnt;
			
			for( i = 0; i < ButtonCnt; i ++ ){
				UI_STRCPY( lpRadioGrp->szItems[i], szButtonMsg[i] );
			}
			lpRadioGrp->nItemCount = ButtonCnt;
		} 
	}
	
	return hControl;
}


static void atRADIOGROUP_Init( atHCONTROL hControl, _CreateParam *param )
{
	LPMYRADIOGROUP	lpRadioGrp = (LPMYRADIOGROUP)atCONTROL_GetDataPtr(hControl);
	atRECT rect;
	int nWidth, nHeight;

	if ( lpRadioGrp == atNULL ) return;

//	lpRadioGrp->lpItemList = atUIAPI_CreateList();
	lpRadioGrp->nItemCount = 0;

	lpRadioGrp->nItemHeight = param->nItemHeight;
	lpRadioGrp->horiz_item_count = param->horiz_item_count;
	lpRadioGrp->horiz_gap = param->horiz_gap;
	lpRadioGrp->line_gap = param->line_gap;

	lpRadioGrp->lpNormalImage	= param->lpNormalImage;
	lpRadioGrp->lpSelectImage	= param->lpSelectImage;

	atCONTROL_GetControlRect( hControl, &rect );
	nWidth = RECT_WIDTH(rect);
	nHeight = RECT_HEIGHT(rect);
	
	lpRadioGrp->nOneScreenCount	= nHeight / lpRadioGrp->nItemHeight;		
	lpRadioGrp->nSelectIndex = -1;
	lpRadioGrp->nCursorIndex = -1;
	
	lpRadioGrp->szItems = atNULL;
	lpRadioGrp->nAllocItemCount = 0;
	lpRadioGrp->nItemCount = 0;

//	lpRadioGrp->hScrollBar = CreateScrollBar( hControl, 0, nWidth-1-8, 0, 8, nHeight, 0, 0, 0, 0, atFALSE );
	lpRadioGrp->hScrollBar = atSCROLLBAR_EXT_Create( 0, nWidth-1-8, 0, 8, nHeight, 
                              						atSCROLLBAR_EXT_TYPE_WINDOW_FRICTION_STICK, 0, 0, 0, 0,
			  						      param->lpScrollbarImgTop, param->lpScrollbarImage, param->lpScrollbarImgBottom, 
			  						      //param->lpScrollcursorImage );
										  param->lpScrollCursorImgTop, param->lpScrollCursorImgMid, param->lpScrollCursorImgBot );
	atCONTROL_SetAnchor( lpRadioGrp->hScrollBar, 0, 0, atANCHOR_IGNORE, 0 );
	atCONTROL_SetAttr( lpRadioGrp->hScrollBar, atCONTROL_GetAttr(lpRadioGrp->hScrollBar) | CTL_PARENTTYPE_CONTROL );
	atCONTROL_SetFocusable(lpRadioGrp->hScrollBar, atFALSE);
	atCONTROL_SetVisible( lpRadioGrp->hScrollBar, atFALSE );	
	atCONTROL_AddChildControl( hControl, lpRadioGrp->hScrollBar );
	
	atCONTROL_SetSoftKey( hControl, _AT(""), _AT("OK"), _AT("Select") );

	lpRadioGrp->bMouseDown = atFALSE;
}

static void atRADIOGROUP_Destroy( atHCONTROL hControl )
{
	atLPCONTROL 	lpControl = atCONTROL_GetPtr(hControl);
	LPMYRADIOGROUP	lpRadioGroup = atCONTROL_GetDataPtr( hControl );

	atCONTROL_Destroy( lpRadioGroup->hScrollBar );

	if( lpRadioGroup->szItems ) UI_MEMFREE( lpRadioGroup->szItems );
}

static void atRADIOGROUP_Draw( atHGDC hCanvas, atHCONTROL hControl, atBOOL bFocused )
{
	atLPCONTROL lpControl = atCONTROL_GetPtr(hControl);
	LPMYRADIOGROUP	lpRadioGroup = (LPMYRADIOGROUP)atCONTROL_GetDataPtr(hControl);
	atLPCONTROL		lpControl2;
	int				/*nAscent, */nHeight;
	atHBITMAP			lpBM;
	int i, idx;
	int nCurPage, nCurIndex;
	int x, y, w, h;
	int posY, hh;
	atCOLOR txt_color;

//	atUIAPI_SetFont( hCanvas, lpControl->font );
	
//	nAscent = MC_grpGetFontAscent(lpControl->font);
	nHeight = atUIAPI_GetFontHeight( atUIAPI_GetFont(hCanvas) );

	x = 0;
	y = 0; 
	w = RECT_WIDTH(lpControl->Base.rect);
	h = RECT_HEIGHT(lpControl->Base.rect);
	if ( atCONTROL_IsVisible(lpRadioGroup->hScrollBar) )
	{
		lpControl2 = atCONTROL_GetPtr(lpRadioGroup->hScrollBar);
		w -= RECT_WIDTH(lpControl2->Base.rect);
	}

	atUIAPI_FillRect(hCanvas, x, y, w, h, UI_COLOR_WHITE);  // 배경을 지운다.

	if( lpRadioGroup->nOneScreenCount <= 0 ) return;

	hh = lpRadioGroup->nItemHeight;

	if(lpRadioGroup->nCursorIndex >= 0){
		nCurPage = lpRadioGroup->nCursorIndex / lpRadioGroup->nOneScreenCount;
		nCurIndex = lpRadioGroup->nCursorIndex % lpRadioGroup->nOneScreenCount;
	} else {
		nCurPage = 0;
		nCurIndex = -1;
	}
	 
	if( lpRadioGroup->nCursorIndex != -1 ){
		// Select Bar Draw
		posY = y + 1 + hh * nCurIndex;
		atUIAPI_FillRect(hCanvas, x + 1, posY, w - 2, lpRadioGroup->nItemHeight, (bFocused)? COLOR_SELECTBAR : atUI_COLOR_GRAY);
	}

	idx = 0;
	for( i = 0; i < lpRadioGroup->nOneScreenCount; i ++ ){
		int idx = nCurPage * lpRadioGroup->nOneScreenCount + i;
		int ww;

		//if( idx >= lpRadioGroup->nItemCount ) break;
			
		if ( idx == lpRadioGroup->nSelectIndex  )
			lpBM = lpRadioGroup->lpSelectImage;
		else
			lpBM = lpRadioGroup->lpNormalImage;
		if ( idx == lpRadioGroup->nCursorIndex  )
			txt_color = UI_COLOR_WHITE;
		else
			txt_color = UI_COLOR_BLACK;

		if( lpBM ){
			atUIAPI_DrawImage(hCanvas, x + 2, y + 1 + i*hh, lpBM);
			ww = atUIAPI_GetBitmapWidth(lpBM);
		} else {
			int ratio = (hh-2)/2;
			ww = hh;
			atUIAPI_DrawCircle(hCanvas, x + 2 + ratio, y + 1 + i*hh + ratio, ratio, UI_COLOR_BLACK, 1);
			if ( idx == lpRadioGroup->nSelectIndex	)
			  atUIAPI_FillCircle(hCanvas, x + 2 + ratio, y + 1 + i*hh + ratio, 5, UI_COLOR_BLACK);
		}
	
		atUIAPI_DrawStringAlign(hCanvas,  x + 2 + ww + 2, y + 2 + i * hh + (hh>>1),
					lpRadioGroup->szItems[idx], ALIGN_LEFT_MIDDLE, txt_color);
	}
	
//	if(atCONTROL_IsVisible(lpRadioGroup->hScrollBar) == atTRUE)
//		atCONTROL_Draw( lpRadioGroup->hScrollBar, atFALSE );
}

static int atRADIOGROUP_EXT_OnMouseDown( atHCONTROL hControl, int x, int y )
{
	LPMYRADIOGROUP	lpRadioGroup = (LPMYRADIOGROUP)atCONTROL_GetDataPtr( hControl );

	lpRadioGroup->nOrgSelectIndex = lpRadioGroup->nSelectIndex;
	lpRadioGroup->bMouseDown = atTRUE;

	atRADIOGROUP_EXT_OnMouseMove( hControl, x, y );

	return 1;
}

static int atRADIOGROUP_EXT_OnMouseMove( atHCONTROL hControl, int x, int y )
{
	LPMYRADIOGROUP	lpRadioGroup = (LPMYRADIOGROUP)atCONTROL_GetDataPtr( hControl );

	if(lpRadioGroup->bMouseDown ){
		atRECT rect;
		atCONTROL_GetClientRect( hControl, &rect );
		
		if(atUIAPI_IsPtInRect( x, y, &rect )){ 
			int idx = atSCROLLBAR_EXT_GetPosition( lpRadioGroup->hScrollBar );
			idx += y / lpRadioGroup->nItemHeight;
			if(idx >= 0 && idx < lpRadioGroup->nItemCount ){
				lpRadioGroup->nSelectIndex = lpRadioGroup->nCursorIndex = idx;
			} else {
				lpRadioGroup->nCursorIndex = -1;
			}
		} else {
			lpRadioGroup->nCursorIndex = -1;
			lpRadioGroup->nSelectIndex = lpRadioGroup->nOrgSelectIndex;
		}

		atCONTROL_Refresh( hControl );
	}

	return 1;
}

static int atRADIOGROUP_EXT_OnMouseUp( atHCONTROL hControl, int x, int y )
{
	LPMYRADIOGROUP	lpRadioGroup = (LPMYRADIOGROUP)atCONTROL_GetDataPtr( hControl );

	if(lpRadioGroup->bMouseDown){
		atRECT rect;
		atCONTROL_GetClientRect( hControl, &rect );
		
		if(atUIAPI_IsPtInRect( x, y, &rect )){ 
			atCONTROL_ProcessControlProc( hControl, CTLMSG_KEYDOWN, atVKEY_SELECT, 0 );
		}

		lpRadioGroup->bMouseDown = atFALSE;
	}

	return 1;
}

static int RadioGroupProc( atHCONTROL hControl, int nMsg, long lParam1, long lParam2 )
{
	atLPCONTROL lpControl = atCONTROL_GetPtr(hControl);
	LPMYRADIOGROUP	lpRadioGroup = (LPMYRADIOGROUP)atCONTROL_GetDataPtr( hControl );
	atRECT rect;
	int x, y;
	int r = 0;

	switch( nMsg ){
		case CTLMSG_INIT:
			atRADIOGROUP_Init( hControl, (_CreateParam *)lParam1 );
			break;
		case CTLMSG_RELEASE:
			atRADIOGROUP_Destroy( hControl );
			r = 1;
			break;
		case CTLMSG_RESIZE:
			atCONTROL_GetControlRect( hControl, &rect );
			lpRadioGroup->nOneScreenCount = RECT_HEIGHT(rect) / lpRadioGroup->nItemHeight;
			if( lpRadioGroup->nItemCount > lpRadioGroup->nOneScreenCount ){
				atCONTROL_SetControlRegion( lpRadioGroup->hScrollBar, 0+RECT_WIDTH(rect)-8, 0, 8, RECT_HEIGHT(rect) );
				atCONTROL_SetVisible( lpRadioGroup->hScrollBar, atTRUE );
			} else
				atCONTROL_SetVisible( lpRadioGroup->hScrollBar, atFALSE );
			r = 1;
			break;
		case CTLMSG_PAINT:
			atRADIOGROUP_Draw( atCONTROL_GetGDC(hControl), hControl, (atBOOL)lParam2 );
			r = 1;
			break;
		case CTLMSG_GOTFOCUS:
//			SetSoftKey( atCONTROL_GetParentWindow(hControl), SOFTKEY_NOCHANGE, SOFTKEY_OK, SOFTKEY_CHECK );
			r = 1;
			break;
		case CTLMSG_KEYDOWN:
			switch( lParam1 )
			{
				case atVKEY_UP:
				case atVKEY_REW:
					lpRadioGroup->nCursorIndex --;
					if( lpRadioGroup->nCursorIndex < 0 )	{
						if( 1 ){
							lpRadioGroup->nCursorIndex = lpRadioGroup->nItemCount - 1;
						} else {
							lpRadioGroup->nCursorIndex = 0;
							return 0;
						}
					}
					atSCROLLBAR_EXT_SetPosition( lpRadioGroup->hScrollBar, lpRadioGroup->nCursorIndex );
//					atCONTROL_Draw( hControl, atTRUE );
					atCONTROL_Refresh( hControl );
					r = 1;
					break;
				case atVKEY_DOWN:
				case atVKEY_FF:
					lpRadioGroup->nCursorIndex ++;
					if( lpRadioGroup->nCursorIndex >= lpRadioGroup->nItemCount ){
						if( 1 ){
							lpRadioGroup->nCursorIndex = 0;
						} else {
							lpRadioGroup->nCursorIndex = lpRadioGroup->nItemCount -1;
							return 0;
						}
					}
					atSCROLLBAR_EXT_SetPosition( lpRadioGroup->hScrollBar, lpRadioGroup->nCursorIndex );
//					atCONTROL_Draw( hControl, atTRUE );
					atCONTROL_Refresh( hControl );
					r = 1;
					break;
				case atVKEY_SOFT2:
					lpRadioGroup->nSelectIndex = lpRadioGroup->nCursorIndex;
//					atCONTROL_Draw( hControl, atTRUE );
					atCONTROL_Refresh( hControl );
					r = 1;
					break;
				case atVKEY_SELECT:
					lpRadioGroup->nSelectIndex = lpRadioGroup->nCursorIndex;
					atCONTROL_PostMessageToParent( hControl, WNDMSG_COMMAND, (long)lpControl->Base.nID, (long)lpRadioGroup->nSelectIndex );
					r = 1;
					break;
			}
			break;
		case CTLMSG_MOUSEDOWN:
			x = (short)(lParam2 >> 16);
			y = (short)(lParam2 & 0xffff);
			r = atRADIOGROUP_EXT_OnMouseDown( hControl, x, y );
			break;

		case CTLMSG_MOUSEMOVE:
			x = (short)(lParam2 >> 16);
			y = (short)(lParam2 & 0xffff);
			r = atRADIOGROUP_EXT_OnMouseMove( hControl, x, y );
			break;
			
		case CTLMSG_MOUSEUP:
			// Kyle Start [Add 2009/06/22-14:25]
			x = (short)(lParam2 >> 16);
			y = (short)(lParam2 & 0xffff);
			r = atRADIOGROUP_EXT_OnMouseUp( hControl, x, y );
			break;
	}

	return r;
}


//=====================================================================


int atRADIOGROUP_EXT_GetItemCount( atHCONTROL hControl )
{
	LPMYRADIOGROUP	lpRadioGroup = (LPMYRADIOGROUP)atCONTROL_GetDataPtr(hControl);

	return lpRadioGroup->nItemCount;
}

int atRADIOGROUP_EXT_AddItem( atHCONTROL hControl, atLPTSTR itm_str )
{
	LPMYRADIOGROUP	lpRadioGroup = (LPMYRADIOGROUP)atCONTROL_GetDataPtr(hControl);

//	if( lpRadioGroup->nItemCount >= PREALLOC_RADIOGROUP_ITEM_COUNT ) return 0;
	if( lpRadioGroup->nItemCount >= lpRadioGroup->nAllocItemCount )
	{
		int size = lpRadioGroup->nAllocItemCount + PREALLOC_RADIOGROUP_ITEM_COUNT;
		atTCHAR (*itms)[32] = (atTCHAR (*)[32])UI_MEMALLOC( size * 32 * sizeof(atTCHAR) );

		if( lpRadioGroup->szItems )
		{
			memcpy( itms, lpRadioGroup->szItems, lpRadioGroup->nAllocItemCount * 32 * sizeof(atTCHAR) );
			UI_MEMFREE( lpRadioGroup->szItems );
		}

		lpRadioGroup->szItems = itms;
		lpRadioGroup->nAllocItemCount = size;
	}
	
	UI_STRCPY( lpRadioGroup->szItems[lpRadioGroup->nItemCount], itm_str );
	
	lpRadioGroup->nItemCount ++;
	
	atSCROLLBAR_EXT_SetSize( lpRadioGroup->hScrollBar, lpRadioGroup->nItemCount );
	if(lpRadioGroup->nItemCount > lpRadioGroup->nOneScreenCount )
		atCONTROL_SetVisible( lpRadioGroup->hScrollBar, atTRUE );
	else
		atCONTROL_SetVisible( lpRadioGroup->hScrollBar, atFALSE );

	return lpRadioGroup->nItemCount;	
}

int atRADIOGROUP_EXT_GetSelectIndex( atHCONTROL hControl )
{
	LPMYRADIOGROUP	lpRadioGroup = (LPMYRADIOGROUP)atCONTROL_GetDataPtr(hControl);

	if ( lpRadioGroup == atNULL )	return -1;

	return lpRadioGroup->nSelectIndex;	
}

void atRADIOGROUP_EXT_SetSelectIndex( atHCONTROL hControl, int idx )
{
	LPMYRADIOGROUP	lpRadioGroup = (LPMYRADIOGROUP)atCONTROL_GetDataPtr(hControl);

	if ( lpRadioGroup == atNULL )	return;

	lpRadioGroup->nSelectIndex = idx;	
	lpRadioGroup->nCursorIndex = idx;
}

