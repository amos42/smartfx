//
// ProgressBar.c
//
///////////////////////////////////////////////////////////////////////

#include "A_UIENG.h"
#include "UI_Util.h"
#include "CMP_VProgressBarEx.h"


typedef struct _atVPROGRESSBAR {
	atHBITMAP	lpProgressBarTopImage;
	atHBITMAP	lpProgressBarMiddleImage;
	atHBITMAP	lpProgressBarBottomImage;
	atHBITMAP	lpProgressMeasureTopImage;
	atHBITMAP	lpProgressMeasureMiddleImage;
	atHBITMAP	lpProgressMeasureBottomImage;

	int style;
	
	int	nSize;
	int nPosition;

	int nMesureTopMargin, nMesureBottomMargin, nMesureX;
} atVPROGRESSBAR,  *atLPVPROGRESSBAR;


static void atVPROGRESSBAR_EXT_OnDestroy( atHCONTROL hControl );
static void atVPROGRESSBAR_EXT_OnDraw( atHGDC hCanvas, atHCONTROL hControl, atBOOL bFocused );

static int VProgressBarProc( atHCONTROL hControl, int nMsg, long lParam1, long lParam2 )
{
	atLPCONTROL lpControl = atCONTROL_GetPtr(hControl);
//	atLPVPROGRESSBAR	lpVProgressBar = (atLPVPROGRESSBAR)atCONTROL_GetDataPtr(hControl);
	int r = 0;

	switch( nMsg )
	{
		case CTLMSG_INIT:
			break;
		case CTLMSG_RELEASE:
			atVPROGRESSBAR_EXT_OnDestroy( hControl );
			r = 1;
			break;
		case CTLMSG_PAINT:
			atVPROGRESSBAR_EXT_OnDraw( atCONTROL_GetGDC(hControl), hControl, (atBOOL)lParam2 );
			r = 1;
			break;
	}

	return r;
}



atHCONTROL atVPROGRESSBAR_EXT_Create( int nIndex, int nX, int nY, int nWidth, int nHeight, int style,
					  			 atHBITMAP lpProgressBarTopImage, atHBITMAP lpProgressBarMiddleImage, atHBITMAP lpProgressBarBottomImage, 
					  			 atHBITMAP lpProgressMeasureTopImage, atHBITMAP lpProgressMeasureMiddleImage, atHBITMAP lpProgressMeasureBottomImage,
								 int nMesureTopMargin, int nMesureBottomMargin, int nMesureX )
{
	atHCONTROL		hControl;
	atLPCONTROL		lpControl;

//	lpControl	= atCONTROL_CreateBaseControl( hParentWnd, nIndex, nX, nY, nWidth, nHeight, CTL_PARENTTYPE_WINDOW, atNULL );
	hControl = atCONTROL_Create( nIndex, nX, nY, nWidth, nHeight, CTL_PARENTTYPE_WINDOW, 0, atNULL, atNULL );
	lpControl = atCONTROL_GetPtr(hControl);
	
	if ( lpControl != atNULL )
	{
		atLPVPROGRESSBAR	lpVProgressBar;
		atHBITMAP				lpBM;

//		lpControl->nControlTypeID	= CTLID_PROGRESSBAR;
		atCONTROL_SetFocusable( hControl, atFALSE ); // ProgressBar는 기본적으로 포커스를 가지지 않는다.

		lpVProgressBar = (atLPVPROGRESSBAR)UI_MEMALLOC(sizeof(atVPROGRESSBAR));
		if ( lpVProgressBar == atNULL )
		{
			UI_MEMFREE(lpControl);
			return atNULL;
		}
		atUIAPI_memset(lpVProgressBar, 0, sizeof(atVPROGRESSBAR));

		lpVProgressBar->lpProgressBarTopImage		= lpProgressBarTopImage;
		lpVProgressBar->lpProgressBarMiddleImage		= lpProgressBarMiddleImage;
		lpVProgressBar->lpProgressBarBottomImage		= lpProgressBarBottomImage;
		lpVProgressBar->lpProgressMeasureTopImage	= lpProgressMeasureTopImage;
		lpVProgressBar->lpProgressMeasureMiddleImage	= lpProgressMeasureMiddleImage;
		lpVProgressBar->lpProgressMeasureBottomImage	= lpProgressMeasureBottomImage;

		lpVProgressBar->style = style;
		
		lpVProgressBar->nSize = 100;
		lpVProgressBar->nPosition = 0;		

		lpVProgressBar->nMesureTopMargin = nMesureTopMargin;
		lpVProgressBar->nMesureBottomMargin = nMesureBottomMargin;
		lpVProgressBar->nMesureX = nMesureX;

		lpBM = lpVProgressBar->lpProgressBarTopImage;
		if ( nWidth <= 0 )
		{
			if( lpBM )
				lpControl->Base.rect.nEndX		= nX + atUIAPI_GetBitmapWidth(lpBM);
			else
				lpControl->Base.rect.nEndX		= nX + 20;
		}
		if ( lpBM && nHeight <= 0 )
		{
//			lpControl->Base.rect.nEndY		= nY + atUIAPI_GetBitmapHeight(lpBM);
		}
		
		lpControl->Base.nObjDataSize = sizeof(atVPROGRESSBAR);
		lpControl->Base.lpObjData = lpVProgressBar;
		lpControl->Base.fnObjProc = VProgressBarProc;
	}

//	return atCONTROL_GetHandle(lpControl);
	return hControl;
}

static void atVPROGRESSBAR_EXT_OnDestroy( atHCONTROL hControl )
{
//	atLPCONTROL lpControl = atCONTROL_GetPtr(hControl);
//	atLPVPROGRESSBAR lpVProgressBar = (atLPVPROGRESSBAR)atCONTROL_GetDataPtr( hControl );

//	if( lpVProgressBar->lpProgressBarJBM ) ReleaseImage(g_lpImageMG, &lpVProgressBar->lpProgressBarJBM);
//	if( lpVProgressBar->lpProgressCursorJBM ) ReleaseImage(g_lpImageMG, &lpVProgressBar->lpProgressCursorJBM);
}


static void atVPROGRESSBAR_EXT_OnDraw( atHGDC hCanvas, atHCONTROL hControl, atBOOL bFocused )
{
	atLPCONTROL lpControl = atCONTROL_GetPtr(hControl);
	atLPVPROGRESSBAR	lpVProgressBar = (atLPVPROGRESSBAR)atCONTROL_GetDataPtr(hControl);
	int height;
	int size = lpVProgressBar->nSize;
	int pos = lpVProgressBar->nPosition;
	int posY;
	atRECT rect;
	int sx, sy;

	atCONTROL_PrepareGDC( hControl, hCanvas, bFocused );

	rect = lpControl->Base.rect;
//	atWINDOW_ClientToScreen( atCONTROL_GetParentWindow(hControl), &rect.nStartX, &rect.nStartY );
//	atWINDOW_ClientToScreen( atCONTROL_GetParentWindow(hControl), &rect.nEndX, &rect.nEndY );
	sx = lpVProgressBar->nMesureX;
	sy = lpVProgressBar->nMesureTopMargin;
//	atWINDOW_ClientToScreen( atCONTROL_GetParentWindow(hControl), &sx, &sy );

//	if ( bFocused )
//	{
		if( lpVProgressBar->lpProgressBarTopImage && lpVProgressBar->lpProgressBarMiddleImage && lpVProgressBar->lpProgressBarBottomImage ){
			atUIAPI_DrawClient1LineVertStretch( hCanvas,  0, 0, RECT_WIDTH(rect), RECT_HEIGHT(rect), 
										lpVProgressBar->lpProgressBarTopImage, 
										lpVProgressBar->lpProgressBarMiddleImage,
										lpVProgressBar->lpProgressBarBottomImage );
		} else {
			atUIAPI_DrawRect(hCanvas, 0, 0, RECT_WIDTH(rect), RECT_HEIGHT(rect), UI_COLOR_GRAY, 1);
		}
		
		if( (size > 0) && (pos > 0) ){
			if( lpVProgressBar->lpProgressMeasureTopImage && lpVProgressBar->lpProgressMeasureMiddleImage && lpVProgressBar->lpProgressMeasureBottomImage){
				height = RECT_HEIGHT(rect) - lpVProgressBar->nMesureTopMargin - lpVProgressBar->nMesureBottomMargin;
				posY = height * pos / size;
				atUIAPI_DrawClient1LineVertStretch( hCanvas,  sx, sy + RECT_HEIGHT(rect) - sy*2 - posY, atUIAPI_GetBitmapWidth(lpVProgressBar->lpProgressMeasureTopImage), posY, 
										lpVProgressBar->lpProgressMeasureTopImage, 
										lpVProgressBar->lpProgressMeasureMiddleImage,
										lpVProgressBar->lpProgressMeasureBottomImage );
			} else {
				height = RECT_HEIGHT(rect) - sy*2;
				posY = height * pos / size;

				atUIAPI_FillRect(hCanvas, sx, sy + RECT_HEIGHT(rect) - sy*2 - posY, RECT_WIDTH(rect)-sx*2, posY, atUIAPI_GetRGB(0,0,255) );	
			}
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

int atVPROGRESSBAR_EXT_GetSize( atHCONTROL hControl )
{
	atLPVPROGRESSBAR	lpVProgressBar = (atLPVPROGRESSBAR)atCONTROL_GetDataPtr(hControl);
	return lpVProgressBar->nSize;
}

void atVPROGRESSBAR_EXT_SetSize( atHCONTROL hControl, int size )
{
	atLPVPROGRESSBAR	lpVProgressBar = (atLPVPROGRESSBAR)atCONTROL_GetDataPtr(hControl);
	lpVProgressBar->nSize = size;
	if( lpVProgressBar->nSize < 0 ) lpVProgressBar->nSize = 0;
	if( lpVProgressBar->nPosition > lpVProgressBar->nSize ) lpVProgressBar->nPosition = lpVProgressBar->nSize;
	if( lpVProgressBar->nPosition < 0 ) lpVProgressBar->nPosition = 0;
}

int atVPROGRESSBAR_EXT_GetPosition( atHCONTROL hControl )
{
	atLPVPROGRESSBAR	lpVProgressBar = (atLPVPROGRESSBAR)atCONTROL_GetDataPtr(hControl);
	return lpVProgressBar->nPosition;
}

int atVPROGRESSBAR_EXT_SetPosition( atHCONTROL hControl, int pos )
{
	atLPVPROGRESSBAR	lpVProgressBar = (atLPVPROGRESSBAR)atCONTROL_GetDataPtr(hControl);
	lpVProgressBar->nPosition = pos;
	if( lpVProgressBar->nPosition > lpVProgressBar->nSize ) lpVProgressBar->nPosition = lpVProgressBar->nSize;
	if( lpVProgressBar->nPosition < 0 ) lpVProgressBar->nPosition = 0;

//	atCONTROL_Draw(hControl, atTRUE);

	return lpVProgressBar->nPosition;
}

void	atVPROGRESSBAR_EXT_SetBitmap( atHCONTROL hControl,
					  	atHBITMAP hImgProgressTop, atHBITMAP hImgProgressMiddle, atHBITMAP hImgProgressBottom,
						  atHBITMAP hImgMeasureTop, atHBITMAP hImgMeasureMiddle, atHBITMAP hImgMeasureBottom,
						  int nMesureTopMargin, int nMesureBottomMargin, int nMesureX )
{
	atLPVPROGRESSBAR	lpVProgressBar = (atLPVPROGRESSBAR)atCONTROL_GetDataPtr(hControl);
	if (!lpVProgressBar) return;

	lpVProgressBar->lpProgressBarTopImage = hImgProgressTop; 
	lpVProgressBar->lpProgressBarMiddleImage = hImgProgressMiddle; 
	lpVProgressBar->lpProgressBarBottomImage = hImgProgressBottom; 
	lpVProgressBar->lpProgressMeasureTopImage = hImgMeasureTop; 
	lpVProgressBar->lpProgressMeasureMiddleImage = hImgMeasureMiddle; 
	lpVProgressBar->lpProgressMeasureBottomImage = hImgMeasureBottom; 
	
	lpVProgressBar->nMesureTopMargin = nMesureTopMargin;
	lpVProgressBar->nMesureBottomMargin = nMesureBottomMargin;
	lpVProgressBar->nMesureX = nMesureX;
}

