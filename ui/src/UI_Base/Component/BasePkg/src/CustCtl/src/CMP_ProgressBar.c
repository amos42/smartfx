//
// ProgressBar.c
//
///////////////////////////////////////////////////////////////////////

#include "A_UIENG.h"
#include "UI_Util.h"
#include "CMP_ProgressBar.h"


typedef struct _atPROGRESSBAR {
	int style;
	
	int	nSize;
	int nPosition;

	int nCursorX, nCursorY;
	int nCursorLimit;
} atPROGRESSBAR,  *atLPPROGRESSBAR;


static void atPROGRESSBAR_OnDestroy( atHCONTROL hControl );
static void atPROGRESSBAR_OnDraw( atHGDC hCanvas, atHCONTROL hControl, atBOOL bFocused );

static int ProgressBarProc( atHCONTROL hControl, int nMsg, long lParam1, long lParam2 )
{
	atLPCONTROL lpControl = atCONTROL_GetPtr(hControl);
//	atLPPROGRESSBAR	lpProgressBar = (atLPPROGRESSBAR)atCONTROL_GetDataPtr(hControl);
	int r = 0;

	switch( nMsg )
	{
		case CTLMSG_INIT:
			break;
		case CTLMSG_RELEASE:
			atPROGRESSBAR_OnDestroy( hControl );
			r = 1;
			break;
		case CTLMSG_PAINT:
			atPROGRESSBAR_OnDraw( atCONTROL_GetGDC(hControl), hControl, (atBOOL)lParam2 );
			r = 1;
			break;
	}

	return r;
}



atHCONTROL atPROGRESSBAR_Create( int nIndex, int nX, int nY, int nWidth, int nHeight, 
								 int style, 	int nCursorX, int nCursorY, int nCursorLimit )
{
	atHCONTROL		hControl;
	atLPCONTROL		lpControl;

//	lpControl	= atCONTROL_CreateBaseControl( hParentWnd, nIndex, nX, nY, nWidth, nHeight, CTL_PARENTTYPE_WINDOW, atNULL );
	hControl = atCONTROL_Create( nIndex, nX, nY, nWidth, nHeight, CTL_PARENTTYPE_WINDOW, 0, atNULL, atNULL );
	lpControl = atCONTROL_GetPtr(hControl);
	
	if ( lpControl != atNULL )
	{
		atLPPROGRESSBAR	lpProgressBar;

//		lpControl->nControlTypeID	= CTLID_PROGRESSBAR;
		atCONTROL_SetAttr( hControl, atCONTROL_GetAttr(hControl) & ~atOBJATTR_EVENTTARGET );
		atCONTROL_SetFocusable( hControl, atFALSE );

		lpProgressBar = (atLPPROGRESSBAR)UI_MEMALLOC(sizeof(atPROGRESSBAR));
		if ( lpProgressBar == atNULL )
		{
			UI_MEMFREE(lpControl);
			return atNULL;
		}
		atUIAPI_memset(lpProgressBar, 0, sizeof(atPROGRESSBAR));

		lpProgressBar->style = style;
		
		lpProgressBar->nSize = 100;
		lpProgressBar->nPosition = 0;		

		lpProgressBar->nCursorX = nCursorX;
		lpProgressBar->nCursorY = nCursorY;

		if ( nHeight <= 0 ){
			lpControl->Base.rect.nEndY		= nY + 20;
		}
		lpProgressBar->nCursorLimit = nCursorLimit;
		if ( nCursorLimit <= 0 ){
			lpProgressBar->nCursorLimit = RECT_WIDTH(lpControl->Base.rect) - nCursorX;
		}
		
		lpControl->Base.nObjDataSize = sizeof(atPROGRESSBAR);
		lpControl->Base.lpObjData = lpProgressBar;
		lpControl->Base.fnObjProc = ProgressBarProc;
	}

//	return atCONTROL_GetHandle(lpControl);
	return hControl;
}

static void atPROGRESSBAR_OnDestroy( atHCONTROL hControl )
{
	atLPCONTROL lpControl = atCONTROL_GetPtr(hControl);
	atLPPROGRESSBAR lpProgressBar = (atLPPROGRESSBAR)atCONTROL_GetDataPtr( hControl );

//	if( lpProgressBar->lpProgressBarJBM ) ReleaseImage(g_lpImageMG, &lpProgressBar->lpProgressBarJBM);
//	if( lpProgressBar->lpProgressCursorJBM ) ReleaseImage(g_lpImageMG, &lpProgressBar->lpProgressCursorJBM);
}


static void atPROGRESSBAR_OnDraw( atHGDC hCanvas, atHCONTROL hControl, atBOOL bFocused )
{
	atLPCONTROL lpControl = atCONTROL_GetPtr(hControl);
	atLPPROGRESSBAR	lpProgressBar = (atLPPROGRESSBAR)atCONTROL_GetDataPtr(hControl);
	int width;
	int size = lpProgressBar->nSize;
	int pos = lpProgressBar->nPosition;
	int posY;
	atRECT rect;
	int sx, sy;

	rect = lpControl->Base.rect;
//	atWINDOW_ClientToScreen( atCONTROL_GetParentWindow(hControl), &rect.nStartX, &rect.nStartY );
//	atWINDOW_ClientToScreen( atCONTROL_GetParentWindow(hControl), &rect.nEndX, &rect.nEndY );
	sx = lpProgressBar->nCursorX;
	sy = lpProgressBar->nCursorY;
//	atWINDOW_ClientToScreen( atCONTROL_GetParentWindow(hControl), &sx, &sy );

//	if ( bFocused )
//	{
		atUIAPI_DrawRect(hCanvas, 0, 0, RECT_WIDTH(rect), RECT_HEIGHT(rect), UI_COLOR_GRAY, 1);
		
		if( (size > 0) && (pos > 0) ){
			width = RECT_WIDTH(rect) - sx*2;
			posY = width * pos / size;

			atUIAPI_FillRect(hCanvas, sx, sy, posY, RECT_HEIGHT(rect)-sy*2, atUIAPI_GetRGB(0,0,255) );	
		}

		if( lpProgressBar->style == 1 ){
			atTCHAR str[10];
			UI_SPRINTF( str, _AT("%d/%d"), lpProgressBar->nPosition, lpProgressBar->nSize );
			atUIAPI_DrawStringAlign( hCanvas, 0 + RECT_WIDTH(rect)/2, 0 + RECT_HEIGHT(rect)/2, str, ALIGN_CENTER_MIDDLE, UI_COLOR_BLACK);
		} else if( lpProgressBar->style == 2 ){
			atTCHAR str[10];
			UI_SPRINTF( str, _AT("%d%%"), lpProgressBar->nPosition / lpProgressBar->nSize );
			atUIAPI_DrawStringAlign( hCanvas, 0 + RECT_WIDTH(rect)/2, 0 + RECT_HEIGHT(rect)/2, str, ALIGN_CENTER_MIDDLE, UI_COLOR_BLACK);
		}
//	}
//	else
//	{
//		atUIAPI_DrawRect(rect.nStartX, rect.nStartY, rect.nEndX-rect.nStartX+1, rect.nEndY-rect.nStartY+1, UI_COLOR_GRAY);
//	}
}

int atPROGRESSBAR_GetSize( atHCONTROL hControl )
{
	atLPPROGRESSBAR	lpProgressBar = (atLPPROGRESSBAR)atCONTROL_GetDataPtr(hControl);
	return lpProgressBar->nSize;
}

void atPROGRESSBAR_SetSize( atHCONTROL hControl, int size )
{
	atLPPROGRESSBAR	lpProgressBar = (atLPPROGRESSBAR)atCONTROL_GetDataPtr(hControl);
	lpProgressBar->nSize = size;
	if( lpProgressBar->nSize < 0 ) lpProgressBar->nSize = 0;
	if( lpProgressBar->nPosition > lpProgressBar->nSize ) lpProgressBar->nPosition = lpProgressBar->nSize;
	if( lpProgressBar->nPosition < 0 ) lpProgressBar->nPosition = 0;
}

int atPROGRESSBAR_GetPosition( atHCONTROL hControl )
{
	atLPPROGRESSBAR	lpProgressBar = (atLPPROGRESSBAR)atCONTROL_GetDataPtr(hControl);
	return lpProgressBar->nPosition;
}

int atPROGRESSBAR_SetPosition( atHCONTROL hControl, int pos )
{
	atLPPROGRESSBAR	lpProgressBar = (atLPPROGRESSBAR)atCONTROL_GetDataPtr(hControl);
	lpProgressBar->nPosition = pos;
	if( lpProgressBar->nPosition > lpProgressBar->nSize ) lpProgressBar->nPosition = lpProgressBar->nSize;
	if( lpProgressBar->nPosition < 0 ) lpProgressBar->nPosition = 0;

//	atCONTROL_Draw(hControl, atTRUE);

	return lpProgressBar->nPosition;
}
