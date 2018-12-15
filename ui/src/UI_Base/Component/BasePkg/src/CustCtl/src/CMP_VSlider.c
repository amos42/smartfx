#include "A_UIENG.h"
#include "UI_Util.h"
#include "CMP_VSlider.h"


typedef struct _tagatVSLIDER {
	int direction;
	
	int nMaxPos;
	int nPosition;

	int nCursorTopMargin, nCursorBottomMargin, nCursorX;

	int nIncSize;
	
	int nMouseCurX, nMouseCurY;
	atBOOL bMouseDown;
} atVSLIDER,  *atLPVSLIDER;

static int VSliderProc( atHCONTROL hControl, int nMsg, long lParam1, long lParam2 );


atHCONTROL atVSLIDER_Create( int nIndex, int nX, int nY, int nWidth, int nHeight, int direction,
								 int nCursorTopMargin, int nCursorBottomMargin, int nCursorX )
{
	atHCONTROL		hControl;
	atLPCONTROL		lpControl;

//	lpControl	= atCONTROL_CreateBaseControl( hParentWnd, nIndex, nX, nY, nWidth, nHeight, CTL_PARENTTYPE_WINDOW, atNULL );
	hControl = atCONTROL_Create( nIndex, nX, nY, nWidth, nHeight, CTL_PARENTTYPE_WINDOW, 0, atNULL, atNULL );
	lpControl = atCONTROL_GetPtr(hControl);
	
	if ( lpControl != atNULL ){
		atLPVSLIDER	lpVSlider;

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

		lpVSlider->nCursorTopMargin = nCursorTopMargin;
		lpVSlider->nCursorBottomMargin = nCursorBottomMargin;
		lpVSlider->nCursorX = nCursorX;

		lpVSlider->bMouseDown = atFALSE;
		
		if ( nWidth <= 0 ){
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
	int posY, height;
	int size = lpVSlider->nMaxPos;
	int pos = lpVSlider->nPosition;
	
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

static void atVSLIDER_OnDraw( atHGDC hCanvas, atHCONTROL hControl, atBOOL bFocused )
{
	atLPVSLIDER	lpVSlider = (atLPVSLIDER)atCONTROL_GetDataPtr(hControl);
	int size = lpVSlider->nMaxPos;
	atRECT rect;
	atRECT cur_rect;
	atCOLOR TickColor;

	atCONTROL_GetControlRect( hControl, &rect );

	if ( bFocused )	{
		TickColor = UI_COLOR_RED;
	} else {
		TickColor = UI_COLOR_GRAY;
	}

	atUIAPI_FillRect(hCanvas, 0, 0, RECT_WIDTH(rect), RECT_HEIGHT(rect), UI_COLOR_WHITE);

	atUIAPI_DrawRect(hCanvas, RECT_WIDTH(rect)/2-2, 0, 4, RECT_HEIGHT(rect), UI_COLOR_GRAY, 1);
	
	if( size > 0 ){
		GetCursorRect( lpVSlider, bFocused, &rect, &cur_rect );
		atUIAPI_FillRect(hCanvas, cur_rect.nStartX, cur_rect.nStartY, RECT_WIDTH(cur_rect), RECT_HEIGHT(cur_rect), TickColor );	
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
			atVSLIDER_OnDraw( atCONTROL_GetGDC(hControl), hControl, (atBOOL)lParam2 );
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
					if( lpVSlider->nPosition < lpVSlider->nMaxPos ){
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
				atVSLIDER_SetPosition( hControl, pos );
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
	}

	return r;
}




atINT atVSLIDER_GetMaxPos( atHCONTROL hControl )
{
	atLPVSLIDER	lpVSlider = (atLPVSLIDER)atCONTROL_GetDataPtr(hControl);
	return lpVSlider->nMaxPos;
}

atVOID atVSLIDER_SetMaxPos( atHCONTROL hControl, atINT nMaxPos )
{
	atLPVSLIDER	lpVSlider = (atLPVSLIDER)atCONTROL_GetDataPtr(hControl);
	lpVSlider->nMaxPos = nMaxPos;
	if( lpVSlider->nMaxPos < 0 ) lpVSlider->nMaxPos = 0;
	if( lpVSlider->nPosition > lpVSlider->nMaxPos ) lpVSlider->nPosition = lpVSlider->nMaxPos;
	if( lpVSlider->nPosition < 0 ) lpVSlider->nPosition = 0;
}

atINT atVSLIDER_GetPosition( atHCONTROL hControl )
{
	atLPVSLIDER	lpVSlider = (atLPVSLIDER)atCONTROL_GetDataPtr(hControl);
	return lpVSlider->nPosition;
}

atINT atVSLIDER_SetPosition( atHCONTROL hControl, atINT nPos )
{
	atLPVSLIDER	lpVSlider = (atLPVSLIDER)atCONTROL_GetDataPtr(hControl);
	lpVSlider->nPosition = nPos;
	if( lpVSlider->nPosition > lpVSlider->nMaxPos ) lpVSlider->nPosition = lpVSlider->nMaxPos;
	if( lpVSlider->nPosition < 0 ) lpVSlider->nPosition = 0;

	atCONTROL_PostMessageToParent( hControl, WNDMSG_CHANGE, (long)atCONTROL_GetID(hControl), (long)lpVSlider->nPosition );

	return lpVSlider->nPosition;
}

