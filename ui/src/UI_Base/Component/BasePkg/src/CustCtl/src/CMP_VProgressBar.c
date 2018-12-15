//
// VPROGRESSBAR.c
//
///////////////////////////////////////////////////////////////////////

#include "A_UIENG.h"
#include "UI_Util.h"
#include "CMP_VProgressBar.h"


typedef struct _atVPROGRESSBAR {
	int style;
	
	int	nSize;
	int nPosition;

	int nCursorX, nCursorY;
	int nCursorLimit;
} atVPROGRESSBAR,  *atlpVProgressBar;


static void atVPROGRESSBAR_OnDestroy( atHCONTROL hControl );
static void atVPROGRESSBAR_OnDraw( atHGDC hCanvas, atHCONTROL hControl, atBOOL bFocused );

static int VProgressBarProc( atHCONTROL hControl, int nMsg, long lParam1, long lParam2 )
{
	atLPCONTROL lpControl = atCONTROL_GetPtr(hControl);
//	atlpVProgressBar	lpVProgressBar = (atlpVProgressBar)atCONTROL_GetDataPtr(hControl);
	int r = 0;

	switch( nMsg )
	{
		case CTLMSG_INIT:
			break;
		case CTLMSG_RELEASE:
			atVPROGRESSBAR_OnDestroy( hControl );
			r = 1;
			break;
		case CTLMSG_PAINT:
			atVPROGRESSBAR_OnDraw( atCONTROL_GetGDC(hControl), hControl, (atBOOL)lParam2 );
			r = 1;
			break;
	}

	return r;
}



atHCONTROL atVPROGRESSBAR_Create( int nIndex, int nX, int nY, int nWidth, int nHeight, 
								 int style, 	int nCursorX, int nCursorY, int nCursorLimit )
{
	atHCONTROL		hControl;
	atLPCONTROL		lpControl;

//	lpControl	= atCONTROL_CreateBaseControl( hParentWnd, nIndex, nX, nY, nWidth, nHeight, CTL_PARENTTYPE_WINDOW, atNULL );
	hControl = atCONTROL_Create( nIndex, nX, nY, nWidth, nHeight, CTL_PARENTTYPE_WINDOW, 0, atNULL, atNULL );
	lpControl = atCONTROL_GetPtr(hControl);
	
	if ( lpControl != atNULL )
	{
		atlpVProgressBar	lpVProgressBar;

//		lpControl->nControlTypeID	= CTLID_VPROGRESSBAR;
		atCONTROL_SetFocusable( hControl, atFALSE );

		lpVProgressBar = (atlpVProgressBar)UI_MEMALLOC(sizeof(atVPROGRESSBAR));
		if ( lpVProgressBar == atNULL )
		{
			UI_MEMFREE(lpControl);
			return atNULL;
		}
		atUIAPI_memset(lpVProgressBar, 0, sizeof(atVPROGRESSBAR));

		lpVProgressBar->style = style;
		
		lpVProgressBar->nSize = 100;
		lpVProgressBar->nPosition = 0;		

		lpVProgressBar->nCursorX = nCursorX;
		lpVProgressBar->nCursorY = nCursorY;

		if ( nWidth <= 0 ){
			lpControl->Base.rect.nEndX		= nX + 20;
		}
		lpVProgressBar->nCursorLimit = nCursorLimit;
		if ( nCursorLimit <= 0 ){
			lpVProgressBar->nCursorLimit = RECT_HEIGHT(lpControl->Base.rect) - nCursorY;
		}
		
		lpControl->Base.nObjDataSize = sizeof(atVPROGRESSBAR);
		lpControl->Base.lpObjData = lpVProgressBar;
		lpControl->Base.fnObjProc = VProgressBarProc;
	}

//	return atCONTROL_GetHandle(lpControl);
	return hControl;
}

static void atVPROGRESSBAR_OnDestroy( atHCONTROL hControl )
{
	atLPCONTROL lpControl = atCONTROL_GetPtr(hControl);
	atlpVProgressBar lpVProgressBar = (atlpVProgressBar)atCONTROL_GetDataPtr( hControl );

//	if( lpVProgressBar->lpVProgressBarJBM ) ReleaseImage(g_lpImageMG, &lpVProgressBar->lpVProgressBarJBM);
//	if( lpVProgressBar->lpProgressCursorJBM ) ReleaseImage(g_lpImageMG, &lpVProgressBar->lpProgressCursorJBM);
}


static void atVPROGRESSBAR_OnDraw( atHGDC hCanvas, atHCONTROL hControl, atBOOL bFocused )
{
	atLPCONTROL lpControl = atCONTROL_GetPtr(hControl);
	atlpVProgressBar	lpVProgressBar = (atlpVProgressBar)atCONTROL_GetDataPtr(hControl);
	int height;
	int size = lpVProgressBar->nSize;
	int pos = lpVProgressBar->nPosition;
	int posY;
	atRECT rect;
	int sx, sy;

	rect = lpControl->Base.rect;
//	atWINDOW_ClientToScreen( atCONTROL_GetParentWindow(hControl), &rect.nStartX, &rect.nStartY );
//	atWINDOW_ClientToScreen( atCONTROL_GetParentWindow(hControl), &rect.nEndX, &rect.nEndY );
	sx = lpVProgressBar->nCursorX;
	sy = lpVProgressBar->nCursorY;
//	atWINDOW_ClientToScreen( atCONTROL_GetParentWindow(hControl), &sx, &sy );

//	if ( bFocused )
//	{
		atUIAPI_DrawRect(hCanvas, 0, 0, RECT_WIDTH(rect), RECT_HEIGHT(rect), UI_COLOR_GRAY, 1);
		
		if( (size > 0) && (pos > 0) ){
			height = RECT_HEIGHT(rect) - sy*2;
			posY = height * pos / size;

			atUIAPI_FillRect(hCanvas, sx, sy + RECT_HEIGHT(rect) - sy*2 - posY, RECT_WIDTH(rect)-sx*2, posY, atUIAPI_GetRGB(0,0,255) );	
		}
		
		if( lpVProgressBar->style == 1 ){
			atTCHAR str[10];
			UI_SPRINTF( str, _AT("%d/%d"), lpVProgressBar->nPosition, lpVProgressBar->nSize );
			atUIAPI_DrawStringAlign( hCanvas, 0 + RECT_WIDTH(rect)/2, 0 + RECT_HEIGHT(rect)/2, str, ALIGN_CENTER_MIDDLE, UI_COLOR_BLACK);
		} else if( lpVProgressBar->style == 2 ){
			atTCHAR str[10];
			UI_SPRINTF( str, _AT("%d%%"), lpVProgressBar->nPosition / lpVProgressBar->nSize );
			atUIAPI_DrawStringAlign( hCanvas, 0 + RECT_WIDTH(rect)/2, 0 + RECT_HEIGHT(rect)/2, str, ALIGN_CENTER_MIDDLE, UI_COLOR_BLACK);
		}
//	}
//	else
//	{
//		atUIAPI_DrawRect(rect.nStartX, rect.nStartY, rect.nEndX-rect.nStartX+1, rect.nEndY-rect.nStartY+1, UI_COLOR_GRAY);
//	}
}

int atVPROGRESSBAR_GetSize( atHCONTROL hControl )
{
	atlpVProgressBar	lpVProgressBar = (atlpVProgressBar)atCONTROL_GetDataPtr(hControl);
	return lpVProgressBar->nSize;
}

void atVPROGRESSBAR_SetSize( atHCONTROL hControl, int size )
{
	atlpVProgressBar	lpVProgressBar = (atlpVProgressBar)atCONTROL_GetDataPtr(hControl);
	lpVProgressBar->nSize = size;
	if( lpVProgressBar->nSize < 0 ) lpVProgressBar->nSize = 0;
	if( lpVProgressBar->nPosition > lpVProgressBar->nSize ) lpVProgressBar->nPosition = lpVProgressBar->nSize;
	if( lpVProgressBar->nPosition < 0 ) lpVProgressBar->nPosition = 0;
}

int atVPROGRESSBAR_GetPosition( atHCONTROL hControl )
{
	atlpVProgressBar	lpVProgressBar = (atlpVProgressBar)atCONTROL_GetDataPtr(hControl);
	return lpVProgressBar->nPosition;
}

int atVPROGRESSBAR_SetPosition( atHCONTROL hControl, int pos )
{
	atlpVProgressBar	lpVProgressBar = (atlpVProgressBar)atCONTROL_GetDataPtr(hControl);
	lpVProgressBar->nPosition = pos;
	if( lpVProgressBar->nPosition > lpVProgressBar->nSize ) lpVProgressBar->nPosition = lpVProgressBar->nSize;
	if( lpVProgressBar->nPosition < 0 ) lpVProgressBar->nPosition = 0;

//	atCONTROL_Draw(hControl, atTRUE);

	return lpVProgressBar->nPosition;
}
