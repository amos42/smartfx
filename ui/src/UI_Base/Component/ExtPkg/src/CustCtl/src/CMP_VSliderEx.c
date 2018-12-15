#include "A_UIENG.h"
#include "UI_Util.h"
#include "CMP_VSliderEx.h"


typedef struct _tagatVSLIDER {
	int direction;
	
	int nMaxPos;
	int nPosition;

	atHBITMAP	lpVSliderTopImg, lpVSliderMiddleImg, lpVSliderBottomImg;
	atHBITMAP	lpVSliderTickNormalImg, lpVSliderTickFocusImg, lpVSliderTickDisableImg;

	int nCursorTopMargin, nCursorBottomMargin, nCursorX;

	int nIncSize;

	int nMouseCurX, nMouseCurY;
	atBOOL bMouseDown;
} atVSLIDER,  *atLPVSLIDER;

static int VSliderProc( atHCONTROL hControl, int nMsg, long lParam1, long lParam2 );


atHCONTROL atVSLIDER_EXT_Create( int nIndex, int nX, int nY, int nWidth, int nHeight, int direction,
					  			 atHBITMAP lpVSliderTop, atHBITMAP lpVSliderMiddle, atHBITMAP lpVSliderBottom,
								 atHBITMAP lpVSliderTickNormal, atHBITMAP lpVSliderTickFocus, atHBITMAP lpVSliderTickDisable,
								 int nCursorTopMargin, int nCursorBottomMargin, int nCursorX )
{
	atHCONTROL		hControl;
	atLPCONTROL		lpControl;

//	lpControl	= atCONTROL_CreateBaseControl( hParentWnd, nIndex, nX, nY, nWidth, nHeight, CTL_PARENTTYPE_WINDOW, atNULL );
	hControl = atCONTROL_Create( nIndex, nX, nY, nWidth, nHeight, CTL_PARENTTYPE_WINDOW, 0, atNULL, atNULL );
	lpControl = atCONTROL_GetPtr(hControl);
	
	if ( lpControl != atNULL )
	{
		atLPVSLIDER	lpVSlider;
		atHBITMAP				lpBM;

//		lpControl->nControlTypeID	= CTLID_VSLIDER;

		lpVSlider = (atLPVSLIDER)UI_MEMALLOC(sizeof(atVSLIDER));
		if ( lpVSlider == atNULL )
		{
			UI_MEMFREE(lpControl);
			return atNULL;
		}
		atUIAPI_memset(lpVSlider, 0, sizeof(atVSLIDER));

		lpVSlider->direction = direction;
		lpVSlider->nMaxPos = 100;
		lpVSlider->nPosition = 0;		

		lpVSlider->lpVSliderTopImg			= lpVSliderTop;
		lpVSlider->lpVSliderMiddleImg		= lpVSliderMiddle;
		lpVSlider->lpVSliderBottomImg		= lpVSliderBottom;
		lpVSlider->lpVSliderTickNormalImg	= lpVSliderTickNormal;
		lpVSlider->lpVSliderTickFocusImg	= lpVSliderTickFocus;
		lpVSlider->lpVSliderTickDisableImg	= lpVSliderTickDisable;

		lpVSlider->nCursorTopMargin = nCursorTopMargin;
		lpVSlider->nCursorBottomMargin = nCursorBottomMargin;
		lpVSlider->nCursorX = nCursorX;

		lpVSlider->bMouseDown = atFALSE;

		lpBM = lpVSlider->lpVSliderTickNormalImg;
		if ( nWidth <= 0 ){
			if( lpBM )
				lpControl->Base.rect.nEndX		= nX + atUIAPI_GetBitmapWidth(lpBM);
			else
				lpControl->Base.rect.nEndX		= nX + 20;
		}
		
		lpControl->Base.nObjDataSize = sizeof(atVSLIDER);
		lpControl->Base.lpObjData = lpVSlider;
		lpControl->Base.fnObjProc = VSliderProc;
	}

//	return atCONTROL_GetHandle(lpControl);
	return hControl;
}

static void GetCursorRect( atLPVSLIDER lpVSlider, atBOOL bFocused, atRECT *lpCtlArea, atRECT *lpRect )
{
	atHBITMAP lpBMTick;
	int posY, height;
	int size = lpVSlider->nMaxPos;
	int pos = lpVSlider->nPosition;
	
	if ( bFocused )	{
		lpBMTick = lpVSlider->lpVSliderTickFocusImg;
	} else {
		lpBMTick = lpVSlider->lpVSliderTickNormalImg;
	}

	if( lpBMTick ){
		height = RECT_HEIGHT(*lpCtlArea) - (lpVSlider->nCursorTopMargin + lpVSlider->nCursorBottomMargin) - atUIAPI_GetBitmapHeight(lpBMTick);
		if( lpVSlider->direction == 0 )
			posY = height * pos / (size) - atUIAPI_GetBitmapHeight(lpBMTick)/2;
		else
			posY = height - height * pos / (size) - atUIAPI_GetBitmapHeight(lpBMTick)/2;

		lpRect->nStartX = lpVSlider->nCursorX; 
		lpRect->nStartY = posY + atUIAPI_GetBitmapHeight(lpBMTick)/2;;
		lpRect->nEndX = RECT_WIDTH(*lpCtlArea) - 1 - lpVSlider->nCursorX;
		lpRect->nEndY = lpRect->nStartY + atUIAPI_GetBitmapHeight(lpBMTick) - 1;
	} else {
		height = RECT_HEIGHT(*lpCtlArea) - 2 - 4;
		if( lpVSlider->direction == 0 )
			posY = height * pos / (size);
		else
			posY = height - height * pos / (size);

		lpRect->nStartX = lpVSlider->nCursorX; 
		lpRect->nStartY = posY;
		lpRect->nEndX = lpCtlArea->nEndX - lpVSlider->nCursorX;
		lpRect->nEndY = posY + 4;
	}
}

static void atVSLIDER_EXT_OnDraw( atHGDC hCanvas, atHCONTROL hControl, atBOOL bFocused )
{
	atLPVSLIDER	lpVSlider = (atLPVSLIDER)atCONTROL_GetDataPtr(hControl);
	int size = lpVSlider->nMaxPos;
	int pos = lpVSlider->nPosition;
	atRECT rect;
	atHBITMAP lpBMTick;
	atCOLOR TickColor;
	atRECT cur_rect;

	atCONTROL_PrepareGDC( hControl, hCanvas, bFocused );

	atCONTROL_GetControlRect( hControl, &rect );

	if( !atCONTROL_IsEnable( hControl) ) {
		lpBMTick = lpVSlider->lpVSliderTickDisableImg;
		TickColor = UI_COLOR_DARKGRAY;
	} else if ( bFocused )	{
		lpBMTick = lpVSlider->lpVSliderTickFocusImg;
		TickColor = UI_COLOR_RED;
	} else {
		lpBMTick = lpVSlider->lpVSliderTickNormalImg;
		TickColor = UI_COLOR_GRAY;
	}

	atUIAPI_FillRect(hCanvas, 0, 0, RECT_WIDTH(rect), RECT_HEIGHT(rect), UI_COLOR_WHITE);

	if( lpVSlider->lpVSliderTopImg && lpVSlider->lpVSliderMiddleImg && lpVSlider->lpVSliderBottomImg ){
		atUIAPI_DrawClient1LineVertStretch(hCanvas,  0, 0, RECT_WIDTH(rect),  RECT_HEIGHT(rect), 
									lpVSlider->lpVSliderTopImg, lpVSlider->lpVSliderMiddleImg, lpVSlider->lpVSliderBottomImg );
	} else {
		atUIAPI_DrawRect(hCanvas, RECT_WIDTH(rect)/2-2, 0, 4, RECT_HEIGHT(rect), UI_COLOR_GRAY, 1);
	}
	
	if( size > 0 ){
		if( lpBMTick ){
			GetCursorRect( lpVSlider, bFocused, &rect, &cur_rect );
			atUIAPI_DrawImageStretch(hCanvas, cur_rect.nStartX, cur_rect.nStartY, lpBMTick, RECT_WIDTH(cur_rect), RECT_HEIGHT(cur_rect) );	
		} else {
			GetCursorRect( lpVSlider, bFocused, &rect, &cur_rect );
			atUIAPI_FillRect(hCanvas, cur_rect.nStartX, cur_rect.nStartY, RECT_WIDTH(cur_rect), RECT_HEIGHT(cur_rect), TickColor );	
		}
	}
}


static int VSliderProc( atHCONTROL hControl, int nMsg, long lParam1, long lParam2 )
{
	atLPCONTROL lpControl = atCONTROL_GetPtr(hControl);
	atLPVSLIDER	lpVSlider = (atLPVSLIDER)atCONTROL_GetDataPtr(hControl);
	int x, y, pos;
	atRECT cur_rect;
	int r = 0;

	switch( nMsg )
	{
		case CTLMSG_INIT:
			lpVSlider->nIncSize = 1;
			break;
		case CTLMSG_RELEASE:
			break;
		case CTLMSG_PAINT:
			atVSLIDER_EXT_OnDraw( atCONTROL_GetGDC(hControl), hControl, (atBOOL)lParam2 );
			r = 1;
			break;
		case CTLMSG_KEYDOWN:
			switch( lParam1 ){
				case atVKEY_DOWN:
				case atVKEY_UP:
					lpVSlider->nIncSize = 1;
					r = 1;
					break;
				case atVKEY_SELECT:
					if( lpVSlider->nPosition >= 0 ){
						atCONTROL_PostMessageToParent( hControl, WNDMSG_COMMAND, (long)lpControl->Base.nID, (long)lpVSlider->nPosition );
					}
					r = 1;
					break;
			}
			break;
		case CTLMSG_KEYUP:
			switch( lParam1 ){
				case atVKEY_DOWN:
				case atVKEY_UP:
					lpVSlider->nIncSize = 1;
					break;
			}
			break;
		case CTLMSG_KEYPRESS:
			switch( lParam1 ){
				case atVKEY_DOWN:
					if( lpVSlider->nPosition > 0 ){
						lpVSlider->nPosition -= lpVSlider->nIncSize;
						if( lpVSlider->nPosition < 0 ) lpVSlider->nPosition = 0;
						if( lpVSlider->nIncSize < 10 ) lpVSlider->nIncSize++;
//						atCONTROL_ProcessControlProc( hControl, CTLMSG_PAINT, 0, 0 );
//						atCONTROL_Flush(hControl);
						atCONTROL_Refresh( hControl );
						atCONTROL_PostMessageToParent( hControl, WNDMSG_CHANGE, (long)lpControl->Base.nID, (long)lpVSlider->nPosition );
					}
					r = 1;
                    			break;
				case atVKEY_UP:
					if( lpVSlider->nPosition < lpVSlider->nMaxPos){
						lpVSlider->nPosition += lpVSlider->nIncSize;
						if( lpVSlider->nPosition > lpVSlider->nMaxPos ) lpVSlider->nPosition = lpVSlider->nMaxPos;
						if( lpVSlider->nIncSize < 10 ) lpVSlider->nIncSize++;
//						atCONTROL_ProcessControlProc( hControl, CTLMSG_PAINT, 0, 0 );
//						atCONTROL_Flush(hControl);
						atCONTROL_Refresh( hControl );
						atCONTROL_PostMessageToParent( hControl, WNDMSG_CHANGE, (long)lpControl->Base.nID, (long)lpVSlider->nPosition );
					}
					r = 1;
					break;
			}
			break;			
		case CTLMSG_MOUSEDOWN:
			x = lParam2 >> 16;
			y = lParam2 & 0xffff;
			GetCursorRect( lpVSlider, atTRUE/*atCONTROL_IsFocus(hControl)*/, &lpControl->Base.rect, &cur_rect );
			if( x >= cur_rect.nStartX && x <= cur_rect.nEndX && y >= cur_rect.nStartY && y <= cur_rect.nEndY ){
				lpVSlider->nMouseCurX = x;
				lpVSlider->nMouseCurY = y;
				lpVSlider->bMouseDown = atTRUE;
			}
			r = 1;
			break;
		case CTLMSG_MOUSEMOVE:
			if( lpVSlider->bMouseDown ){
				x = lParam2 >> 16;
				y = lParam2 & 0xffff;

				pos = lpVSlider->nPosition + (lpVSlider->nMouseCurY - y);					
				atVSLIDER_EXT_SetPosition( hControl, pos );
//				atCONTROL_Draw( hControl, atTRUE );
				atCONTROL_Refresh( hControl );

				lpVSlider->nMouseCurX = x;
				lpVSlider->nMouseCurY = y;				
			}
			r = 1;
			break;
		case CTLMSG_MOUSEUP:
			lpVSlider->bMouseDown = atFALSE;
			r = 1;
			break;
	}

	return r;
}


int atVSLIDER_EXT_GetMaxPos( atHCONTROL hControl )
{
	atLPVSLIDER	lpVSlider = (atLPVSLIDER)atCONTROL_GetDataPtr(hControl);
	if( lpVSlider == atNULL ) return 0;

	return lpVSlider->nMaxPos;
}

void atVSLIDER_EXT_SetMaxPos( atHCONTROL hControl, atINT nMaxPos )
{
	atLPVSLIDER	lpVSlider = (atLPVSLIDER)atCONTROL_GetDataPtr(hControl);
	if( lpVSlider == atNULL ) return;

	lpVSlider->nMaxPos = nMaxPos;
	if( lpVSlider->nMaxPos < 0 ) lpVSlider->nMaxPos = 0;
	if( lpVSlider->nPosition > lpVSlider->nMaxPos ) lpVSlider->nPosition = lpVSlider->nMaxPos;
	if( lpVSlider->nPosition < 0 ) lpVSlider->nPosition = 0;
}

int atVSLIDER_EXT_GetPosition( atHCONTROL hControl )
{
	atLPVSLIDER	lpVSlider = (atLPVSLIDER)atCONTROL_GetDataPtr(hControl);
	if( lpVSlider == atNULL ) return 0;

	return lpVSlider->nPosition;
}

int atVSLIDER_EXT_SetPosition( atHCONTROL hControl, atINT nPos )
{
	atLPVSLIDER	lpVSlider = (atLPVSLIDER)atCONTROL_GetDataPtr(hControl);
	if( lpVSlider == atNULL ) return 0;

	lpVSlider->nPosition = nPos;
	if( lpVSlider->nPosition > lpVSlider->nMaxPos ) lpVSlider->nPosition = lpVSlider->nMaxPos;
	if( lpVSlider->nPosition < 0 ) lpVSlider->nPosition = 0;

	atCONTROL_PostMessageToParent( hControl, WNDMSG_CHANGE, (long)atCONTROL_GetID(hControl), (long)lpVSlider->nPosition );

	return lpVSlider->nPosition;
}

