//
// ProgressBar.c
//
///////////////////////////////////////////////////////////////////////

#include "A_UIENG.h"
#include "UI_Util.h"
#include "CMP_ProgressBarEx.h"


typedef struct _atPROGRESSBAR {
	atHBITMAP	lpProgressBarLeftImage;
	atHBITMAP	lpProgressBarCenterImage;
	atHBITMAP	lpProgressBarRightImage;
	atHBITMAP	lpProgressMeasureLeftImage;
	atHBITMAP	lpProgressMeasureCenterImage;
	atHBITMAP	lpProgressMeasureRightImage;

	int style;
	
	int	nSize;
	int nPosition;

	int nMesureLeftMargin, nMesureRightMargin, nMesureY;
} atPROGRESSBAR,  *atLPPROGRESSBAR;


static void atPROGRESSBAR_EXT_OnDestroy( atHCONTROL hControl );
static void atPROGRESSBAR_EXT_OnDraw( atHGDC hCanvas, atHCONTROL hControl, atBOOL bFocused );

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
			atPROGRESSBAR_EXT_OnDestroy( hControl );
			r = 1;
			break;
		case CTLMSG_PAINT:
			atPROGRESSBAR_EXT_OnDraw( atCONTROL_GetGDC(hControl), hControl, (atBOOL)lParam2 );
			r = 1;
			break;
	}

	return r;
}



atHCONTROL atPROGRESSBAR_EXT_Create( int nIndex, int nX, int nY, int nWidth, int nHeight, int style, 
					  			 atHBITMAP lpProgressBarLeftImage, atHBITMAP lpProgressBarCenterImage, atHBITMAP lpProgressBarRightImage, 
					  			 atHBITMAP lpProgressMeasureLeftImage, atHBITMAP lpProgressMeasureCenterImage, atHBITMAP lpProgressMeasureRightImage,
								 int nMesureLeftMargin, int nMesureRightMargin, int nMesureY )
{
	atHCONTROL		hControl;
	atLPCONTROL		lpControl;

//	lpControl	= atCONTROL_CreateBaseControl( hParentWnd, nIndex, nX, nY, nWidth, nHeight, CTL_PARENTTYPE_WINDOW, atNULL );
	hControl = atCONTROL_Create( nIndex, nX, nY, nWidth, nHeight, CTL_PARENTTYPE_WINDOW, 0, atNULL, atNULL );
	lpControl = atCONTROL_GetPtr(hControl);
	
	if ( lpControl != atNULL )
	{
		atLPPROGRESSBAR	lpProgressBar;
		atHBITMAP				lpBM;

//		lpControl->nControlTypeID	= CTLID_PROGRESSBAR;
		atCONTROL_SetFocusable( hControl, atFALSE ); // ProgressBar는 기본적으로 포커스를 가지지 않는다.

		lpProgressBar = (atLPPROGRESSBAR)UI_MEMALLOC(sizeof(atPROGRESSBAR));
		if ( lpProgressBar == atNULL )
		{
			UI_MEMFREE(lpControl);
			return atNULL;
		}
		atUIAPI_memset(lpProgressBar, 0, sizeof(atPROGRESSBAR));

//		lpProgressBar->lpProgressBarJBM			= CreateImageEx(g_lpImageMG, szProgressBarImage, atNULL);
//		lpProgressBar->lpProgressCursorJBM		= CreateImageEx(g_lpImageMG, szProgressCursorImage, atNULL);
		lpProgressBar->lpProgressBarLeftImage		= lpProgressBarLeftImage;
		lpProgressBar->lpProgressBarCenterImage		= lpProgressBarCenterImage;
		lpProgressBar->lpProgressBarRightImage		= lpProgressBarRightImage;
		lpProgressBar->lpProgressMeasureLeftImage	= lpProgressMeasureLeftImage;
		lpProgressBar->lpProgressMeasureCenterImage	= lpProgressMeasureCenterImage;
		lpProgressBar->lpProgressMeasureRightImage	= lpProgressMeasureRightImage;

		lpProgressBar->style = style;
		
		lpProgressBar->nSize = 100;
		lpProgressBar->nPosition = 0;		

		lpProgressBar->nMesureLeftMargin = nMesureLeftMargin;
		lpProgressBar->nMesureRightMargin = nMesureRightMargin;
		lpProgressBar->nMesureY = nMesureY;

		lpBM = lpProgressBar->lpProgressBarLeftImage;
		if ( lpBM && nWidth <= 0 ){
//			lpControl->Base.rect.nEndX		= nX + atUIAPI_GetBitmapWidth(lpBM);
		}
		if ( nHeight <= 0 ){
			if( lpBM )
				lpControl->Base.rect.nEndY		= nY + atUIAPI_GetBitmapHeight(lpBM);
			else
				lpControl->Base.rect.nEndY		= nY + 20;
		}
		
		lpControl->Base.nObjDataSize = sizeof(atPROGRESSBAR);
		lpControl->Base.lpObjData = lpProgressBar;
		lpControl->Base.fnObjProc = ProgressBarProc;
	}

//	return atCONTROL_GetHandle(lpControl);
	return hControl;
}

static void atPROGRESSBAR_EXT_OnDestroy( atHCONTROL hControl )
{
	atLPCONTROL lpControl = atCONTROL_GetPtr(hControl);
	atLPPROGRESSBAR lpProgressBar = (atLPPROGRESSBAR)atCONTROL_GetDataPtr( hControl );

//	if( lpProgressBar->lpProgressBarJBM ) ReleaseImage(g_lpImageMG, &lpProgressBar->lpProgressBarJBM);
//	if( lpProgressBar->lpProgressCursorJBM ) ReleaseImage(g_lpImageMG, &lpProgressBar->lpProgressCursorJBM);
}


static void atPROGRESSBAR_EXT_OnDraw( atHGDC hCanvas, atHCONTROL hControl, atBOOL bFocused )
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
	sx = lpProgressBar->nMesureLeftMargin;
	sy = lpProgressBar->nMesureY;
//	atWINDOW_ClientToScreen( atCONTROL_GetParentWindow(hControl), &sx, &sy );

//	if ( bFocused )
//	{
		if( lpProgressBar->lpProgressBarLeftImage && lpProgressBar->lpProgressBarCenterImage && lpProgressBar->lpProgressBarRightImage ){
			if(atUIAPI_GetBitmapHeight(lpProgressBar->lpProgressBarLeftImage) != RECT_HEIGHT(rect))
			{   // 이미지 높이의 크기와 만들고자 하는 프로그래스바 높이가 같다면 Stretch 로 찍을 필요가 없을거 같다 Adolph
				atUIAPI_DrawClient1LineStretch(hCanvas,  0, 0, RECT_WIDTH(rect), RECT_HEIGHT(rect),
											lpProgressBar->lpProgressBarLeftImage, 
											lpProgressBar->lpProgressBarCenterImage,
											lpProgressBar->lpProgressBarRightImage );
			}
			else
			{
				atUIAPI_DrawClient1Line(hCanvas, 0, 0, RECT_WIDTH(rect), 
											lpProgressBar->lpProgressBarLeftImage, 
											lpProgressBar->lpProgressBarCenterImage,
											lpProgressBar->lpProgressBarRightImage );
			}
		} else {
			atUIAPI_DrawRect(hCanvas, 0, 0, RECT_WIDTH(rect), RECT_HEIGHT(rect), UI_COLOR_GRAY, 1);
		}
		
		if( (size > 0) && (pos > 0) ){
			if( lpProgressBar->lpProgressMeasureLeftImage && lpProgressBar->lpProgressMeasureCenterImage && lpProgressBar->lpProgressMeasureRightImage){
				width = RECT_WIDTH(rect) - lpProgressBar->nMesureLeftMargin - lpProgressBar->nMesureRightMargin;
				posY = width * pos / size;
//				atUIAPI_DrawClient1LineStretch(hCanvas,  sx, sy, posY, atUIAPI_GetBitmapHeight(lpProgressBar->lpProgressMeasureLeftImage),
				atUIAPI_DrawClient1Line(hCanvas,  sx, sy, posY, 
										lpProgressBar->lpProgressMeasureLeftImage, 
										lpProgressBar->lpProgressMeasureCenterImage,
										lpProgressBar->lpProgressMeasureRightImage );
			} else {
				width = RECT_WIDTH(rect) - 2;
				posY = width * pos / size;
				// sopp: i is used without being initialized
				//atUIAPI_FillRect(sx + 1, sy + 1, sx + i, RECT_HEIGHT(rect)-1, atUIAPI_GetRGB(0,0,255) );	
				atUIAPI_FillRect(hCanvas, sx + 1, sy + 1, posY, RECT_HEIGHT(rect)-2, atUIAPI_GetRGB(0,0,255) );	
			}
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

int atPROGRESSBAR_EXT_GetSize( atHCONTROL hControl )
{
	atLPPROGRESSBAR	lpProgressBar = (atLPPROGRESSBAR)atCONTROL_GetDataPtr(hControl);
	return lpProgressBar ? lpProgressBar->nSize : 0;
}

void atPROGRESSBAR_EXT_SetSize( atHCONTROL hControl, int size )
{
	atLPPROGRESSBAR	lpProgressBar = (atLPPROGRESSBAR)atCONTROL_GetDataPtr(hControl);
	if (!lpProgressBar) return;

	lpProgressBar->nSize = size;
	if( lpProgressBar->nSize < 0 ) lpProgressBar->nSize = 0;
	if( lpProgressBar->nPosition > lpProgressBar->nSize ) lpProgressBar->nPosition = lpProgressBar->nSize;
	if( lpProgressBar->nPosition < 0 ) lpProgressBar->nPosition = 0;
}

int atPROGRESSBAR_EXT_GetPosition( atHCONTROL hControl )
{
	atLPPROGRESSBAR	lpProgressBar = (atLPPROGRESSBAR)atCONTROL_GetDataPtr(hControl);
	return lpProgressBar ? lpProgressBar->nPosition : 0;
}

int atPROGRESSBAR_EXT_SetPosition( atHCONTROL hControl, int pos )
{
	atLPPROGRESSBAR	lpProgressBar = (atLPPROGRESSBAR)atCONTROL_GetDataPtr(hControl);
	if (!lpProgressBar) return 0;
	
	lpProgressBar->nPosition = pos;
	if( lpProgressBar->nPosition > lpProgressBar->nSize ) lpProgressBar->nPosition = lpProgressBar->nSize;
	if( lpProgressBar->nPosition < 0 ) lpProgressBar->nPosition = 0;

	return lpProgressBar->nPosition;
}

void	atPROGRESSBAR_EXT_SetBitmap( atHCONTROL hControl,
					  	atHBITMAP hImgProgressLeft, atHBITMAP hImgProgressCenter, atHBITMAP hImgProgressRight,
						  atHBITMAP hImgMeasureLeft, atHBITMAP hImgMeasureCenter, atHBITMAP hImgMeasureRight,
						  int nMesureLeftMargin, int nMesureRightMargin, int nMesureY )
{
	atLPPROGRESSBAR	lpProgressBar = (atLPPROGRESSBAR)atCONTROL_GetDataPtr(hControl);
	if (!lpProgressBar) return;

	lpProgressBar->lpProgressBarLeftImage = hImgProgressLeft; 
	lpProgressBar->lpProgressBarCenterImage = hImgProgressCenter; 
	lpProgressBar->lpProgressBarRightImage = hImgProgressRight; 
	lpProgressBar->lpProgressMeasureLeftImage = hImgMeasureLeft; 
	lpProgressBar->lpProgressMeasureCenterImage = hImgMeasureCenter; 
	lpProgressBar->lpProgressMeasureRightImage = hImgMeasureRight; 
	
	lpProgressBar->nMesureLeftMargin = nMesureLeftMargin;
	lpProgressBar->nMesureRightMargin = nMesureRightMargin;
	lpProgressBar->nMesureY = nMesureY;
}

