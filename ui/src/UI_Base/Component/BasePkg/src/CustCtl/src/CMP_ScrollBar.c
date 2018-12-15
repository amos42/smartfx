//
// MenuWnd.c
//
///////////////////////////////////////////////////////////////////////

#include "A_UIENG.h"
#include "UI_Util.h"
#include "CMP_ScrollBar.h"


typedef struct _atSCROLLBAR {
	int style;
	
	int	nSize;
	int nPosition;

	int nCursorX, nCursorY;
	int nCursorLimit;

	atRECT rtCursorRect;
	atBOOL	bActionValidate;
} atSCROLLBAR,  *atLPSCROLLBAR;


static int ScrollBarProc( atHCONTROL hControl, int nMsg, long lParam1, long lParam2 );



atHCONTROL atSCROLLBAR_Create( int nIndex, int nX, int nY, int nWidth, int nHeight, 
								 int style, 	int nCursorX, int nCursorY, int nCursorLimit )
{
	atHCONTROL		hControl;
	atLPCONTROL		lpControl;

//	lpControl	= atCONTROL_CreateBaseControl( hParentWnd, nIndex, nX, nY, nWidth, nHeight, CTL_PARENTTYPE_WINDOW, atNULL );
	hControl = atCONTROL_Create( nIndex, nX, nY, nWidth, nHeight, CTL_PARENTTYPE_WINDOW, 0, atNULL, atNULL );
	lpControl = atCONTROL_GetPtr(hControl);
	
	if ( lpControl != atNULL ){
		atLPSCROLLBAR	lpScrollBar;
//		atHBITMAP				lpBM;

//		lpControl->nControlTypeID	= CTLID_SCROLLBAR;
		atCONTROL_SetFocusable( hControl, atFALSE );		// ScrollBox는 기본적으로 포커스를 가지지 않는다.

		lpScrollBar = (atLPSCROLLBAR)UI_MEMALLOC(sizeof(atSCROLLBAR));
		if ( lpScrollBar == atNULL ){
			UI_MEMFREE(lpControl);
			return atNULL;
		}
		atUIAPI_memset(lpScrollBar, 0, sizeof(atSCROLLBAR));

		lpScrollBar->style = style;
		
		lpScrollBar->nSize = 0;
		lpScrollBar->nPosition = 0;		
		SET_RECT( lpScrollBar->rtCursorRect, 0, 0, 0, 0 );
		lpScrollBar->bActionValidate = atFALSE;

		lpScrollBar->nCursorX = nCursorX;
		lpScrollBar->nCursorY = nCursorY;

		if( nWidth <= 0 ){
			lpControl->Base.rect.nEndX		= nX + 8 - 1;
		}
//		lpBM = lpScrollBar->lpScrollcursorImage;
//		if ( nHeight <= 0 ){
//			lpControl->Base.rect.nEndY		= nY + atUIAPI_GetBitmapHeight(lpBM);
//		}

		lpScrollBar->nCursorLimit = nCursorLimit;
			
		lpControl->Base.nObjDataSize = sizeof(atSCROLLBAR);
		lpControl->Base.lpObjData = lpScrollBar;
		lpControl->Base.fnObjProc = ScrollBarProc;
	}

//	return atCONTROL_GetHandle(lpControl);
	return hControl;
}

static void atSCROLLBAR_OnDestroy( atHCONTROL hControl )
{
//	atLPCONTROL lpControl = atCONTROL_GetPtr(hControl);
//	atLPSCROLLBAR lpScrollBar = (atLPSCROLLBAR)atCONTROL_GetDataPtr( hControl );

//	if( lpScrollBar->lpScrollbarJBM ) ReleaseImage(g_lpImageMG, &lpScrollBar->lpScrollbarJBM);
//	if( lpScrollBar->lpScrollCursorJBM ) ReleaseImage(g_lpImageMG, &lpScrollBar->lpScrollCursorJBM);
}


static void atSCROLLBAR_OnDraw( atHGDC hCanvas, atHCONTROL hControl, int nX, int nY, atBOOL bFocused )
{
	atLPCONTROL lpControl = atCONTROL_GetPtr(hControl);
	atLPSCROLLBAR	lpScrollBar = (atLPSCROLLBAR)atCONTROL_GetDataPtr(hControl);
//	int height;
	int size = lpScrollBar->nSize;
	int pos = lpScrollBar->nPosition;
//	int posY;
	atRECT rect;
	int sx, sy;
//	atLPCANVAS lpDC = atGDC_GetPtr(hCanvas);

//	if( lpDC == atNULL ) return;

	atCONTROL_GetControlRect(hControl, &rect);
//	rect.nStartX += lpDC->Rgn.nX;
//	rect.nEndX += lpDC->Rgn.nX;
//	rect.nStartY += lpDC->Rgn.nY;
//	rect.nEndY += lpDC->Rgn.nY;
	sx = nX + lpScrollBar->nCursorX;
	sy = nY + lpScrollBar->nCursorY;
//	sx += lpDC->Rgn.nX;
//	sy += lpDC->Rgn.nY;

//	if ( bFocused )
//	{
		atUIAPI_FillRect(hCanvas, nX+1, nY+1, RECT_WIDTH(rect)-2, RECT_HEIGHT(rect)-2, UI_COLOR_WHITE);
		atUIAPI_DrawRect(hCanvas, nX, nY, RECT_WIDTH(rect), RECT_HEIGHT(rect), UI_COLOR_GRAY, 1);

		if( atCONTROL_IsEnable(hControl) ){
			if( size > 1 ){
				atUIAPI_DrawRect( hCanvas, lpScrollBar->rtCursorRect.nStartX, lpScrollBar->rtCursorRect.nStartY, RECT_WIDTH(lpScrollBar->rtCursorRect), RECT_HEIGHT(lpScrollBar->rtCursorRect), UI_COLOR_BLACK, 1);
			} else {
			}
		}
//	}
//	else
//	{
//		atUIAPI_DrawRect(rect.nStartX, rect.nStartY, rect.nEndX-rect.nStartX+1, rect.nEndY-rect.nStartY+1, UI_COLOR_GRAY);
//	}
}

static int ScrollBarProc( atHCONTROL hControl, int nMsg, long lParam1, long lParam2 )
{
	atLPCONTROL lpControl = atCONTROL_GetPtr(hControl);
	atLPSCROLLBAR	lpScrollBar = (atLPSCROLLBAR)atCONTROL_GetDataPtr(hControl);
	int r = 0;
	int x, y;

	switch( nMsg )
	{
		case CTLMSG_INIT:
			break;
		case CTLMSG_RELEASE:
			atSCROLLBAR_OnDestroy( hControl );
			r = 1;
			break;
		case CTLMSG_RESIZE:
			r = 1;
			break;
		case CTLMSG_PAINT:
			atSCROLLBAR_OnDraw( atCONTROL_GetGDC(hControl), hControl, 0, 0, (atBOOL)lParam2 );
			r = 1;
			break;
		case CTLMSG_KEYDOWN:
			switch( lParam1 ){
				case atVKEY_UP:
//				case atVKEY_LEFT:
					atSCROLLBAR_Scroll( hControl, -1 );
					r = 1;
					break;
				case atVKEY_DOWN:
//				case atVKEY_RIGHT:
					atSCROLLBAR_Scroll( hControl, 1 );
					r = 1;
					break;
			}
			break;
		case CTLMSG_MOUSEDOWN:
			if( lpScrollBar->nSize > 1 ){
				x = lParam2 >> 16;
				y = lParam2 & 0xffff;
				if( x >= lpScrollBar->rtCursorRect.nStartX && x <= lpScrollBar->rtCursorRect.nEndX &&
					y < lpScrollBar->rtCursorRect.nStartY ){
					atSCROLLBAR_Scroll( hControl, -3 );
					atCONTROL_PostMessageToParent( hControl, CTLMSG_CHANGE, (long)lpControl->Base.nID, (long)lpScrollBar->nPosition );
				} else if( x >= lpScrollBar->rtCursorRect.nStartX && x <= lpScrollBar->rtCursorRect.nEndX &&
					y > lpScrollBar->rtCursorRect.nEndY ){
					atSCROLLBAR_Scroll( hControl, 3 );
					atCONTROL_PostMessageToParent( hControl, CTLMSG_CHANGE, (long)lpControl->Base.nID, (long)lpScrollBar->nPosition );
				} else if( x >= lpScrollBar->rtCursorRect.nStartX && x <= lpScrollBar->rtCursorRect.nEndX &&
					y >= lpScrollBar->rtCursorRect.nStartY && y <= lpScrollBar->rtCursorRect.nEndY ){
					lpScrollBar->bActionValidate = atTRUE;
//					atCONTROL_Draw(hControl, atTRUE);
					atCONTROL_Refresh( hControl );
				}
			}
			r = 1;
			break;

		case CTLMSG_MOUSEMOVE:
			if( lpScrollBar->bActionValidate ){
				x = lParam2 >> 16;
				y = lParam2 & 0xffff;
				if( y < lpScrollBar->rtCursorRect.nStartY ){
					atSCROLLBAR_Scroll( hControl, -1 );
					atCONTROL_PostMessageToParent( hControl, CTLMSG_CHANGE, (long)lpControl->Base.nID, (long)lpScrollBar->nPosition );
				} else if( y > lpScrollBar->rtCursorRect.nEndY ){
					atSCROLLBAR_Scroll( hControl, 1 );
					atCONTROL_PostMessageToParent( hControl, CTLMSG_CHANGE, (long)lpControl->Base.nID, (long)lpScrollBar->nPosition );
				}
			}
			r = 1;
			break;

		case CTLMSG_MOUSEUP:
			lpScrollBar->bActionValidate = atFALSE;
//			atCONTROL_Draw(hControl, atTRUE);
			atCONTROL_Refresh( hControl );
			r = 1;
			break;
	}

	return r;
}



static void recalc_cursor_rect( atHCONTROL hControl, atLPSCROLLBAR lpScrollBar )
{
	int height;
	int size = lpScrollBar->nSize;
	int pos = lpScrollBar->nPosition;
	int posY;
	atRECT rect;
	int sx, sy;

	atCONTROL_GetControlRect(hControl, &rect);
//	rect.nStartX += lpDC->Rgn.nX;
//	rect.nEndX += lpDC->Rgn.nX;
//	rect.nStartY += lpDC->Rgn.nY;
//	rect.nEndY += lpDC->Rgn.nY;
	sx = lpScrollBar->nCursorX;
	sy = lpScrollBar->nCursorY;

	if( size > 1 ){
		if( lpScrollBar->nCursorLimit > 0 )
			height = lpScrollBar->nCursorLimit - lpScrollBar->nCursorY - 30;
		else
			height = RECT_HEIGHT(rect) - lpScrollBar->nCursorY - 30;
		posY = height * pos / (size-1);		
		SET_RECT( lpScrollBar->rtCursorRect, sx+1, sy+posY, sx+RECT_WIDTH(rect)-2-1, sy+posY+30-1 );	
	} else {
	}
}


int atSCROLLBAR_GetSize( atHCONTROL hControl )
{
	atLPSCROLLBAR	lpScrollBar = (atLPSCROLLBAR)atCONTROL_GetDataPtr(hControl);
	return lpScrollBar->nSize;
}

void atSCROLLBAR_SetSize( atHCONTROL hControl, int size )
{
	atLPSCROLLBAR	lpScrollBar = (atLPSCROLLBAR)atCONTROL_GetDataPtr(hControl);
	lpScrollBar->nSize = size;
	if( lpScrollBar->nSize < 0 ) lpScrollBar->nSize = 0;
	if( lpScrollBar->nPosition >= lpScrollBar->nSize ) lpScrollBar->nPosition = lpScrollBar->nSize - 1;
	if( lpScrollBar->nPosition < 0 ) lpScrollBar->nPosition = 0;

	recalc_cursor_rect( hControl, lpScrollBar );
}

int atSCROLLBAR_GetPosition( atHCONTROL hControl )
{
	atLPSCROLLBAR	lpScrollBar = (atLPSCROLLBAR)atCONTROL_GetDataPtr(hControl);
	return lpScrollBar->nPosition;
}

int atSCROLLBAR_SetPosition( atHCONTROL hControl, int pos )
{
//	atLPCONTROL lpControl = atCONTROL_GetPtr(hControl);
	atLPSCROLLBAR	lpScrollBar = (atLPSCROLLBAR)atCONTROL_GetDataPtr(hControl);
	atUIAPI_DebugPrintf(_AT("atSCROLLBAR_SetPosition : %d\r\n"), pos );

	lpScrollBar->nPosition = pos;
	if( lpScrollBar->nPosition >= lpScrollBar->nSize ) lpScrollBar->nPosition = lpScrollBar->nSize - 1;
	if( lpScrollBar->nPosition < 0 ) lpScrollBar->nPosition = 0;

//	atWINOBJ_ProcessObjProc( atCONTROL_GetParentWindow(hControl), OBJMSG_CHANGE, lpControl->nControlID, lpScrollBar->nPosition );
//	atCONTROL_Draw(hControl, atTRUE);
	recalc_cursor_rect( hControl, lpScrollBar );

	return lpScrollBar->nPosition;
}

int atSCROLLBAR_Scroll( atHCONTROL hControl, int value )
{
	atUIAPI_DebugPrintf(_AT("atSCROLLBAR_Scroll : %d\r\n"), value );
	return atSCROLLBAR_SetPosition( hControl, atSCROLLBAR_GetPosition( hControl ) + value );
}

