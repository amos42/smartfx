//
// HSlider.c
//
///////////////////////////////////////////////////////////////////////

#include "A_UIENG.h"
#include "UI_Util.h"
#include "CMP_HSliderEx.h"
#include <string.h>


typedef struct _tagatHSLIDER {
	atHBITMAP	lpHSliderLeftImg, lpHSliderCenterImg, lpHSliderRightImg;
	atHBITMAP	lpHSliderTickNormalImg, lpHSliderTickFocusImg, lpHSliderTickDisableImg;

	int nMaxPos;
	int nPosition;

	int nIncSize;

	int nCursorLeftMargin, nCursorRightMargin, nCursorY;

	int nMouseCurX, nMouseCurY;
	int nMouseCurGap;
	atBOOL bMouseDown;
} atHSLIDER,  *atLPHSLIDER;

static int HSliderProc( atHCONTROL hControl, int nMsg, long lParam1, long lParam2 );


atHCONTROL atHSLIDER_EXT_Create( int nIndex, int nX, int nY, int nWidth, int nHeight, 
					  			 atHBITMAP lpHSliderLeft, atHBITMAP lpHSliderCenter, atHBITMAP lpHSliderRight,
								 atHBITMAP lpHSliderTickNormal, atHBITMAP lpHSliderTickFocus, atHBITMAP lpHSliderTickDisable,
								 int nCursorLeftMargin, int nCursorRightMargin, int nCursorY )
{
	atHCONTROL		hControl;
	atLPCONTROL		lpControl;

//	lpControl	= atCONTROL_CreateBaseControl( hParentWnd, nIndex, nX, nY, nWidth, nHeight, CTL_PARENTTYPE_WINDOW, atNULL );
	hControl = atCONTROL_Create( nIndex, nX, nY, nWidth, nHeight, CTL_PARENTTYPE_WINDOW, 0, atNULL, atNULL );
	lpControl = atCONTROL_GetPtr(hControl);
	
	if ( lpControl != atNULL ){
		atLPHSLIDER	lpHSlider;
		atHBITMAP				lpBM;

//		lpControl->nControlTypeID	= CTLID_HSLIDER;

		lpHSlider = (atLPHSLIDER)UI_MEMALLOC(sizeof(atHSLIDER));
		if ( lpHSlider == atNULL )
		{
			UI_MEMFREE(lpControl);
			return atNULL;
		}
		atUIAPI_memset(lpHSlider, 0, sizeof(atHSLIDER));

		lpHSlider->lpHSliderLeftImg			= lpHSliderLeft;
		lpHSlider->lpHSliderCenterImg		= lpHSliderCenter;
		lpHSlider->lpHSliderRightImg		= lpHSliderRight;
		lpHSlider->lpHSliderTickNormalImg			= lpHSliderTickNormal;
		lpHSlider->lpHSliderTickFocusImg	= lpHSliderTickFocus;
		lpHSlider->lpHSliderTickDisableImg	= lpHSliderTickDisable;

		lpHSlider->nMaxPos = 100;
		lpHSlider->nPosition = 0;		

		lpHSlider->nCursorLeftMargin = nCursorLeftMargin;
		lpHSlider->nCursorRightMargin = nCursorRightMargin;
		lpHSlider->nCursorY = nCursorY;

		lpHSlider->bMouseDown = atFALSE;

		lpBM = lpHSlider->lpHSliderTickNormalImg;

		if ( nHeight <= 0 ){
			if( lpBM )
				lpControl->Base.rect.nEndY		= nY + atUIAPI_GetBitmapHeight(lpBM);
			else
				lpControl->Base.rect.nEndY		= nY + 20;
		}
		
		lpControl->Base.nObjDataSize = sizeof(atHSLIDER);
		lpControl->Base.lpObjData = lpHSlider;
		lpControl->Base.fnObjProc = HSliderProc;
	}

//	return atCONTROL_GetHandle(lpControl);
	return hControl;
}


static void GetCursorRect( atLPHSLIDER lpHSlider, atBOOL bFocused, atRECT *lpCtlArea, atRECT *lpRect )
{
	atHBITMAP lpBMTick;
	int posX, width;
	int size = lpHSlider->nMaxPos;
	int pos = lpHSlider->nPosition;
	
	if ( bFocused )	{
		lpBMTick = lpHSlider->lpHSliderTickFocusImg;
	} else {
		lpBMTick = lpHSlider->lpHSliderTickNormalImg;
	}

	if( lpBMTick ){
		width = RECT_WIDTH(*lpCtlArea) - (lpHSlider->nCursorLeftMargin + lpHSlider->nCursorRightMargin) - atUIAPI_GetBitmapWidth(lpBMTick);
		posX = width * pos / (size) - atUIAPI_GetBitmapWidth(lpBMTick)/2;

		lpRect->nStartX = posX + atUIAPI_GetBitmapWidth(lpBMTick)/2;
		lpRect->nStartY = lpHSlider->nCursorY; 
		lpRect->nEndX = lpRect->nStartX + atUIAPI_GetBitmapWidth(lpBMTick) - 1;
		lpRect->nEndY = lpRect->nStartY + RECT_HEIGHT(*lpCtlArea) - 1 - lpHSlider->nCursorY;
	} else {
		width = RECT_WIDTH(*lpCtlArea) - (2 + 4);
		posX = width * pos / (size);

		lpRect->nStartX = posX;
		lpRect->nStartY = lpHSlider->nCursorY; 
		lpRect->nEndX = posX + 4;
		lpRect->nEndY = RECT_HEIGHT(*lpCtlArea) - 1 - lpHSlider->nCursorY;
	}
}

static void atHSLIDER_EXT_OnDraw( atHGDC hCanvas, atHCONTROL hControl, atBOOL bFocused )
{
	atLPHSLIDER	lpHSlider = (atLPHSLIDER)atCONTROL_GetDataPtr(hControl);
	int size = lpHSlider->nMaxPos;
	int pos = lpHSlider->nPosition;
	atRECT rect;
	atRECT cur_rect;
	atHBITMAP lpBMTick;
	atCOLOR TickColor;

	atCONTROL_GetControlRect( hControl, &rect );

	if( !atCONTROL_IsEnable( hControl) ) {
		lpBMTick = lpHSlider->lpHSliderTickDisableImg;
		TickColor = UI_COLOR_DARKGRAY;
	} else if ( bFocused )	{
		lpBMTick = lpHSlider->lpHSliderTickFocusImg;
		TickColor = UI_COLOR_RED;
	} else {
		lpBMTick = lpHSlider->lpHSliderTickNormalImg;
		TickColor = UI_COLOR_GRAY;
	}

	//atUIAPI_FillRect(hCanvas, 0, 0, RECT_WIDTH(rect), RECT_HEIGHT(rect), UI_COLOR_WHITE);

	if( lpHSlider->lpHSliderLeftImg && lpHSlider->lpHSliderCenterImg && lpHSlider->lpHSliderRightImg ){
		atUIAPI_DrawClient1LineStretch(hCanvas,  0, 0, RECT_WIDTH(rect), RECT_HEIGHT(rect), 
									lpHSlider->lpHSliderLeftImg, lpHSlider->lpHSliderCenterImg, lpHSlider->lpHSliderRightImg );
	} else {
		atUIAPI_DrawRect(hCanvas, 0, RECT_HEIGHT(rect)/2-2, RECT_WIDTH(rect), 4, UI_COLOR_GRAY, 1);
	}
	
	if( size > 0 ){
		if( lpBMTick ){
			GetCursorRect( lpHSlider, bFocused, &rect, &cur_rect );
			atUIAPI_DrawImageStretch(hCanvas, cur_rect.nStartX, cur_rect.nStartY, lpBMTick, RECT_WIDTH(cur_rect), RECT_HEIGHT(cur_rect) );	
		} else {
			GetCursorRect( lpHSlider, bFocused, &rect, &cur_rect );
			atUIAPI_FillRect(hCanvas, cur_rect.nStartX, cur_rect.nStartY, RECT_WIDTH(cur_rect), RECT_HEIGHT(cur_rect), TickColor );	
		}
	}
}


static int HSliderProc( atHCONTROL hControl, int nMsg, long lParam1, long lParam2 )
{
	atLPCONTROL lpControl = atCONTROL_GetPtr(hControl);
	atLPHSLIDER	lpHSlider = (atLPHSLIDER)atCONTROL_GetDataPtr(hControl);
	int x, y, pos;
	atRECT cur_rect;
	int r = 0;

	switch( nMsg )
	{
		case CTLMSG_INIT:
			lpHSlider->nIncSize = 1;
			break;
		case CTLMSG_RELEASE:
			break;
		case CTLMSG_PAINT:
			atHSLIDER_EXT_OnDraw( atCONTROL_GetGDC(hControl), hControl, (atBOOL)lParam2 );
			r = 1;
			break;
		case CTLMSG_KEYDOWN:
			switch( lParam1 ){
				case atVKEY_LEFT:
				case atVKEY_RIGHT:
					lpHSlider->nIncSize = 1;
					r = 1;
					break;
				case atVKEY_SELECT:
					if( lpHSlider->nPosition >= 0 ){
						atCONTROL_PostMessageToParent( hControl, WNDMSG_COMMAND, (long)lpControl->Base.nID, (long)lpHSlider->nPosition );
					}
					r = 1;
					break;
			}
			break;
		case CTLMSG_KEYUP:
			switch( lParam1 ){
				case atVKEY_LEFT:
				case atVKEY_RIGHT:
					lpHSlider->nIncSize = 1;
					break;
			}
			break;
		case CTLMSG_KEYPRESS:
			switch( lParam1 ){
				case atVKEY_LEFT:
					if( lpHSlider->nPosition > 0 ){
						lpHSlider->nPosition -= lpHSlider->nIncSize;
						if( lpHSlider->nPosition < 0 ) lpHSlider->nPosition = 0;
						if( lpHSlider->nIncSize < 10 ) lpHSlider->nIncSize++;
//						atCONTROL_ProcessControlProc( hControl, CTLMSG_PAINT, 0, 0 );
//						atCONTROL_Flush(hControl);
						atCONTROL_Refresh( hControl );
						atCONTROL_PostMessageToParent( hControl, WNDMSG_CHANGE, (long)lpControl->Base.nID, (long)lpHSlider->nPosition );
					}
					r = 1;
                    			break;
				case atVKEY_RIGHT:
					if( lpHSlider->nPosition < lpHSlider->nMaxPos ){
						lpHSlider->nPosition += lpHSlider->nIncSize;
						if( lpHSlider->nPosition > lpHSlider->nMaxPos ) lpHSlider->nPosition = lpHSlider->nMaxPos;
						if( lpHSlider->nIncSize < 10 ) lpHSlider->nIncSize++;
//						atCONTROL_ProcessControlProc( hControl, CTLMSG_PAINT, 0, 0 );
//						atCONTROL_Flush(hControl);
						atCONTROL_Refresh( hControl );
						atCONTROL_PostMessageToParent( hControl, WNDMSG_CHANGE, (long)lpControl->Base.nID, (long)lpHSlider->nPosition );
					}
					r = 1;
					break;
			}
			break;
		case CTLMSG_MOUSEDOWN:
			x = lParam2 >> 16;
			y = lParam2 & 0xffff;
			GetCursorRect( lpHSlider, atTRUE/*atCONTROL_IsFocus(hControl)*/, &lpControl->Base.rect, &cur_rect );
//			if( x >= cur_rect.nStartX && x <= cur_rect.nEndX && y >= cur_rect.nStartY && y <= cur_rect.nEndY ){
				lpHSlider->nMouseCurX = x;
				lpHSlider->nMouseCurY = y;
				lpHSlider->nMouseCurGap = x - cur_rect.nStartX;
				lpHSlider->bMouseDown = atTRUE;

				x -= RECT_WIDTH(cur_rect)/2;
				pos = (x * (lpHSlider->nMaxPos+1)) / (RECT_WIDTH(lpControl->Base.rect) - RECT_WIDTH(cur_rect));
				atHSLIDER_EXT_SetPosition( hControl, pos );
//			}


			r = 1;
			break;
		case CTLMSG_MOUSEMOVE:
 			if( lpHSlider->bMouseDown ){
				x = lParam2 >> 16;
				y = lParam2 & 0xffff;

				GetCursorRect( lpHSlider, atTRUE/*atCONTROL_IsFocus(hControl)*/, &lpControl->Base.rect, &cur_rect );
				
				x -= RECT_WIDTH(cur_rect)/2;
				
				pos = (x * (lpHSlider->nMaxPos+1)) / (RECT_WIDTH(lpControl->Base.rect) - RECT_WIDTH(cur_rect));
				
				atHSLIDER_EXT_SetPosition( hControl, pos );
				
				lpHSlider->nMouseCurX = x;
 				lpHSlider->nMouseCurY = y;
			}
			r = 1;
			break;
		case CTLMSG_MOUSEUP:
			if(lpHSlider->bMouseDown)
				lpHSlider->bMouseDown = atFALSE;
			r = 1;
			break;
	}

	return r;
}



atINT atHSLIDER_EXT_GetMaxPos( atHCONTROL hControl )
{
	atLPHSLIDER	lpHSlider = (atLPHSLIDER)atCONTROL_GetDataPtr(hControl);
	return lpHSlider->nMaxPos;
}

atVOID atHSLIDER_EXT_SetMaxPos( atHCONTROL hControl, atINT nMaxPos )
{
	atLPHSLIDER	lpHSlider = (atLPHSLIDER)atCONTROL_GetDataPtr(hControl);
	lpHSlider->nMaxPos = nMaxPos;
	if( lpHSlider->nMaxPos < 0 ) lpHSlider->nMaxPos = 0;
	if( lpHSlider->nPosition > lpHSlider->nMaxPos ) lpHSlider->nPosition = lpHSlider->nMaxPos;
	if( lpHSlider->nPosition < 0 ) lpHSlider->nPosition = 0;
}

atINT atHSLIDER_EXT_GetPosition( atHCONTROL hControl )
{
	atLPHSLIDER	lpHSlider = (atLPHSLIDER)atCONTROL_GetDataPtr(hControl);
	return lpHSlider->nPosition;
}

atINT 	atHSLIDER_EXT_SetPosition( atHCONTROL hControl, atINT nPos )
{
	atLPHSLIDER	lpHSlider = (atLPHSLIDER)atCONTROL_GetDataPtr(hControl);
	lpHSlider->nPosition = nPos;
	if( lpHSlider->nPosition > lpHSlider->nMaxPos ) lpHSlider->nPosition = lpHSlider->nMaxPos;
	if( lpHSlider->nPosition < 0 ) lpHSlider->nPosition = 0;

	atCONTROL_PostMessageToParent( hControl, WNDMSG_CHANGE, (long)atCONTROL_GetID(hControl), (long)lpHSlider->nPosition );

	return lpHSlider->nPosition;
}

