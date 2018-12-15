//
// MenuWnd.c
//
///////////////////////////////////////////////////////////////////////

#include "A_UIENG.h"
#include "UI_Util.h"
#include "CMP_ImageButton.h"
#include <string.h>


//#define FONT_HEIGHT_GAP			1

typedef struct _MYIMAGEBUTTON {
	atDWORD			dwAttr;
	atHBITMAP		lpNormalImage;
	atHBITMAP		lpFocusedImage;
	atHBITMAP		lpSelectedImage;
	atHBITMAP		lpDisableImage;
	int             nNormalX, nNormalY;
	int             nFocusedX, nFocusedY;
	atIMAGEBUTTON_DRAW_FUNC *fnDrawItem;
	atLPVOID		lpFuncParam;
} MYIMAGEBUTTON, * LPMYIMAGEBUTTON;



struct _CreateParam {
	atHBITMAP		lpNormalImage;
	atHBITMAP		lpFocusedImage;
	atHBITMAP		lpSelectedImage;
	atHBITMAP		lpDisableImage;
	int             nNormalX, nNormalY;
	int             nFocusedX, nFocusedY; 
};

static int ButtonProc( atHCONTROL hControl, int nMsg, long lParam1, long lParam2 );


atHCONTROL atIMAGEBUTTON_Create( int nIndex, int nX, int nY, int nWidth, int nHeight,
                         int nNormalX, int nNormalY, int nFocusedX, int nFocusedY, 
						 atHBITMAP lpNormalImage, atHBITMAP lpFocusedImage,
						 atHBITMAP lpSelectedImage, atHBITMAP lpDisableImage )
{
	atHCONTROL		hControl;
	struct _CreateParam param;

	param.lpNormalImage = lpNormalImage;
	param.lpFocusedImage = lpFocusedImage;
	param.lpSelectedImage = lpSelectedImage;
	param.lpDisableImage = lpDisableImage;
	param.nNormalX = nNormalX; param.nNormalY = nNormalY;
	param.nFocusedX = nFocusedX; param.nFocusedY = nFocusedY;
	
	hControl = atCONTROL_Create( nIndex, nX, nY, nWidth, nHeight, CTL_PARENTTYPE_WINDOW, sizeof(MYIMAGEBUTTON), ButtonProc, &param );

	return hControl;
}


atHCONTROL atIMAGEBUTTON_GetNormalImage( atHCONTROL hControl)
{
	//atLPCONTROL 	lpControl = atCONTROL_GetPtr(hControl);
	LPMYIMAGEBUTTON	lpButton = (LPMYIMAGEBUTTON)atCONTROL_GetDataPtr(hControl);

	return lpButton->lpNormalImage;
}


atVOID atIMAGEBUTTON_SetImages( atHCONTROL hControl, atHBITMAP hBmpNoraml, atHBITMAP hBmpFocused, atHBITMAP hBmpSel, atHBITMAP hBmpDisable)
{
	//atLPCONTROL 	lpControl = atCONTROL_GetPtr(hControl);
	LPMYIMAGEBUTTON	lpButton = (LPMYIMAGEBUTTON)atCONTROL_GetDataPtr(hControl);

	lpButton->lpNormalImage		= hBmpNoraml;
	lpButton->lpDisableImage	= hBmpDisable;
	lpButton->lpSelectedImage	= hBmpSel;
	lpButton->lpFocusedImage	= hBmpFocused;
}



static void atIMAGEBUTTON_OnInit( atHCONTROL hControl, struct _CreateParam *param )
{
	atLPCONTROL 	lpControl = atCONTROL_GetPtr(hControl);
	LPMYIMAGEBUTTON	lpButton = (LPMYIMAGEBUTTON)atCONTROL_GetDataPtr(hControl);
	atHBITMAP		lpBM;
	int nX, nY, nWidth, nHeight;

	lpButton->dwAttr = 0x00;
	
	lpButton->lpNormalImage		= param->lpNormalImage;
	lpButton->lpDisableImage	= param->lpDisableImage;
	lpButton->lpSelectedImage	= param->lpSelectedImage;
	lpButton->lpFocusedImage	= param->lpFocusedImage;

	lpBM = lpButton->lpNormalImage;

	nX = lpControl->Base.rect.nStartX;
	nWidth = RECT_WIDTH(lpControl->Base.rect);
	if ( nWidth <= 0 ){
		if( lpBM )
		  lpControl->Base.rect.nEndX		= nX + atUIAPI_GetBitmapWidth(lpBM);
		else
		  lpControl->Base.rect.nEndX		= nX + 20;
	}
	nY = lpControl->Base.rect.nStartY;
	nHeight = RECT_HEIGHT(lpControl->Base.rect);
	if ( nHeight <= 0 ){
		if( lpBM )
		  lpControl->Base.rect.nEndY		= nY + atUIAPI_GetBitmapHeight(lpBM);
		else
		  lpControl->Base.rect.nEndY		= nY + 20;
	}

	// 변경하지 말하야할 상황에서 강제 셋팅을 해버림 때문에 추가
	//if (lpControl->Base.bShow)
		//atCONTROL_SetSoftKey( hControl, atNULL, _AT("OK"), atNULL );

	lpButton->fnDrawItem  = NULL;
	lpButton->lpFuncParam = NULL;
}


static void atIMAGEBUTTON_OnDestroy( atHCONTROL hControl )
{
//	atLPCONTROL 	lpControl = atCONTROL_GetPtr(hControl);
//	LPMYIMAGEBUTTON	lpButton = (LPMYIMAGEBUTTON)atCONTROL_GetDataPtr(hControl);

//	ReleaseImage(g_lpImageMG, &lpButton->lpNormalImage	);
//	ReleaseImage(g_lpImageMG, &lpButton->lpFocusedImage );
}

static void atIMAGEBUTTON_OnDraw( atHGDC hCanvas, atHCONTROL hControl, atBOOL bFocused )
{
	atLPCONTROL lpControl = atCONTROL_GetPtr(hControl);
	LPMYIMAGEBUTTON	lpButton;
	atHBITMAP			lpBM;
	atRECT rect;

	if ( atCONTROL_IsVisible(hControl) == atFALSE ) return;

	atCONTROL_GetControlRect(hControl, &rect);
		
	lpButton = (LPMYIMAGEBUTTON)atCONTROL_GetDataPtr( hControl );

	if( lpButton->dwAttr & atIMAGEBUTTONATTR_OPAQUE ){		
		atUIAPI_Clear( hCanvas, (bFocused)? lpControl->tControlColor.clFocusedBgColor : lpControl->tControlColor.clNormalBgColor );
	}

	if ( !atCONTROL_IsEnable(hControl) ){
		lpBM = ( lpButton->lpDisableImage != atNULL ) ? lpButton->lpDisableImage : lpButton->lpNormalImage;
	} else if( lpControl->Base.bMouseDown ) {
		lpBM = lpButton->lpSelectedImage;
	} else {
		lpBM = ( bFocused ) ? lpButton->lpFocusedImage : lpButton->lpNormalImage;
	}

	if( !lpBM )
		lpBM = lpButton->lpNormalImage;

	if( lpBM ){
		atUIAPI_DrawImage(hCanvas, 0, 0, lpBM);
	} else {
		atUIAPI_FillRect(hCanvas, 0, 0, RECT_WIDTH(rect), RECT_HEIGHT(rect), (bFocused)? atUIAPI_GetRGB(255,128,0) : atUIAPI_GetRGB(0,230,0) );
	}
}


static int ButtonProc( atHCONTROL hControl, int nMsg, long lParam1, long lParam2 )
{
	atLPCONTROL lpControl = atCONTROL_GetPtr(hControl);
	LPMYIMAGEBUTTON lpButton = (LPMYIMAGEBUTTON)atCONTROL_GetDataPtr( hControl );
//	int x=0, y=0;
	int r = 0;

	switch( nMsg )
	{
		case CTLMSG_INIT:
			atIMAGEBUTTON_OnInit( hControl, (struct _CreateParam *)lParam1 );
			break;
		case CTLMSG_RELEASE:
			atIMAGEBUTTON_OnDestroy( hControl );
			r = 1;
			break;
		case CTLMSG_PAINT:
			if( lpButton && lpButton->fnDrawItem )
				lpButton->fnDrawItem( atCONTROL_GetGDC(hControl), RECT_WIDTH(lpControl->Base.rect), RECT_HEIGHT(lpControl->Base.rect), lpControl->Base.bMouseDown, lpButton->lpFuncParam );
			else
				atIMAGEBUTTON_OnDraw( atCONTROL_GetGDC(hControl), hControl, (atBOOL)lParam2 );
			r = 1;
			break;
		case CTLMSG_KEYDOWN:
			switch( lParam1 )
			{
				case atVKEY_UP:
				case atVKEY_DOWN:
				case atVKEY_LEFT:
				case atVKEY_RIGHT:
				case atVKEY_FF:
				case atVKEY_REW:
					return atCONTROL_DefControlProc( hControl, nMsg, lParam1, lParam2 );
					break;
				case atVKEY_SELECT:
					atCONTROL_PostMessageToParent( hControl, WNDMSG_COMMAND, (long)lpControl->Base.nID, 0 );
					r = 1;
					break;
				default:
//					return atCONTROL_DefControlProc( hControl, nMsg, lParam1, lParam2 );
					break;
			}
			break;
/*
		case CTLMSG_MOUSEUP:
			if(lpControl->Base.bMouseDown)
			{
				lpControl->Base.bMouseDown = atFALSE;
				
				x = lParam2 >> 16;
				y = (short)(lParam2 & 0xffff);
				
				x += lpControl->Base.rect.nStartX;
				y += lpControl->Base.rect.nStartY;
				
				if( atUIAPI_IsPtInRect(x, y, &lpControl->Base.rect) )
					atCONTROL_ProcessControlProc( hControl, CTLMSG_KEYDOWN, atVKEY_SELECT, 0 );
				
//				atCONTROL_Refresh( hControl );
			}

//			atCONTROL_ProcessControlProc( hControl, CTLMSG_KEYDOWN, atVKEY_SELECT, 0 );
			r = 1;
			break;
*/
		case CTLMSG_MOUSEDOWN:
			atCONTROL_Refresh(hControl);
			break;
		case CTLMSG_MOUSEUP:
			atCONTROL_Refresh(hControl);
			break;
			
		case CTLMSG_MOUSECLICK:
			atCONTROL_ProcessControlProc( hControl, CTLMSG_KEYDOWN, atVKEY_SELECT, 0 );
			r = 1;
			break;
	}

	return r;
}


atDWORD atIMAGEBUTTON_GetAttr(atHCONTROL hControl )
{
	LPMYIMAGEBUTTON	lpButton = (LPMYIMAGEBUTTON)atCONTROL_GetDataPtr(hControl);
	if( lpButton == atNULL ) return 0;

	return lpButton->dwAttr;
}

void	atIMAGEBUTTON_SetAttr(atHCONTROL hControl, atDWORD dwAttr )
{
	LPMYIMAGEBUTTON	lpButton = (LPMYIMAGEBUTTON)atCONTROL_GetDataPtr(hControl);
	if( lpButton == atNULL ) return;

	lpButton->dwAttr = dwAttr;
}

void atIMAGEBUTTON_SetDrawFunc(atHCONTROL hControl, atIMAGEBUTTON_DRAW_FUNC* fnDrawItem, atLPVOID lpFuncParam )
{
	LPMYIMAGEBUTTON	lpButton = (LPMYIMAGEBUTTON)atCONTROL_GetDataPtr(hControl);
	if( lpButton == atNULL ) return;
	if( !fnDrawItem || !lpFuncParam )	return;

	lpButton->fnDrawItem  = fnDrawItem;
	lpButton->lpFuncParam = lpFuncParam;
}
