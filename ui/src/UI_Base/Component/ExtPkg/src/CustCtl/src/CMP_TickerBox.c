#include "A_UIENG.h"
#include "UI_Util.h"
#include "CMP_TickerBox.h"
#include <string.h>


typedef struct _tagatTICKERBOX {
	atU32		uAttr;
	atTCHAR		szText[256];
	atBYTE		bWordHL[256];
//	int         nTickerInterval;
//	atHTIMER     hTickerTimer;
	atHSTRBITMAP hStrBitmap;
	int         nTickerInitTick;
	int         nTickerInc;
	int         nFullWidth;
	int         nTickerTick;
	int			nFontSize;
	atBOOL      bIsTicking;
	atCOLOR		clHLColor;

	atINT		nLeftMargin, nRightMargin;
	atHBITMAP	hBgImageLeft, hBgImageCenter, hBgImageRight;
	
	atBOOL		bIsChanged;
	atHFONT		hDefaultFont;
	atHFONT		hCurFont;
} atTICKERBOX, * atLPTICKERBOX;

static int TickerBoxProc( atHCONTROL hControl, int nMsg, long lParam1, long lParam2 );


typedef struct {
	atLONG nTickerInterval;
	atINT nTickerInc;
	atINT nTickerInitTick;
	atINT		nLeftMargin, nRightMargin;
	atHBITMAP	hBgImageLeft, hBgImageCenter, hBgImageRight;
} _CreateParam;


atHCONTROL atTICKERBOX_Create( int nIndex, int nX, int nY, int nWidth, int nHeight, 
	                                 atLPTSTR szText, atINT nLen, 
	                                 atLONG nTickerInterval, atINT nTickerInc, atINT nTickerInitTick,
	                                 atINT nLeftMargin, atINT nRightMargin,
	                                 atHBITMAP	hBgImageLeft, atHBITMAP	hBgImageCenter, atHBITMAP hBgImageRight )
{
	atHCONTROL hControl;
	atLPCONTROL	lpControl;
	_CreateParam param;

	param.nTickerInterval = nTickerInterval;
	param.nTickerInitTick = nTickerInitTick;
	param.nTickerInc = nTickerInc;
	param.nLeftMargin = nLeftMargin;
	param.nRightMargin = nRightMargin;
	param.hBgImageLeft = hBgImageLeft;
	param.hBgImageCenter = hBgImageCenter;
	param.hBgImageRight = hBgImageRight;

	hControl = atCONTROL_Create( nIndex, nX, nY, nWidth, nHeight, CTL_PARENTTYPE_WINDOW, sizeof(atTICKERBOX), TickerBoxProc, &param );
	if( hControl == atNULL ) return atNULL;

	lpControl = atCONTROL_GetPtr(hControl);
	atCONTROL_SetFocusable( hControl, atFALSE ); // TickerBox는 기본적으로 포커스를 가지지 않는다.

	{        
		nWidth = RECT_WIDTH(lpControl->Base.rect);
		nHeight = RECT_HEIGHT(lpControl->Base.rect);
		if ( nWidth == 0 ){
			lpControl->Base.rect.nEndX		= nX + atUIAPI_GetStringWidth(atCONTROL_GetFont(hControl), szText, -1) + 2;
		}
		if ( nHeight == 0 ){
			lpControl->Base.rect.nEndY		= nY + atUIAPI_GetFontHeight(atCONTROL_GetFont(hControl)) + 2;
		}
	}
	
	atTICKERBOX_SetAttr( hControl, atTICKERBOXATTR_OPAQUE );
	atTICKERBOX_SetText( hControl, szText, nLen );
	
	return hControl;
}


static void atTICKERBOX_OnInit( atHCONTROL hControl, _CreateParam *param )
{
	atLPTICKERBOX	lpTickerBox = atCONTROL_GetDataPtr( hControl );

	lpTickerBox->uAttr = 0;
    lpTickerBox->hStrBitmap = atNULL;
    lpTickerBox->nFullWidth = 0;
    lpTickerBox->nTickerTick = lpTickerBox->nTickerInitTick = param->nTickerInitTick;
	lpTickerBox->nTickerInc = param->nTickerInc;
	lpTickerBox->nFontSize = 0;
	lpTickerBox->bIsTicking = atFALSE;	
	atUIAPI_memset(lpTickerBox->bWordHL, 0x00, sizeof(lpTickerBox->bWordHL));
	lpTickerBox->clHLColor = 0xFFFFFFFF;
	
	lpTickerBox->nLeftMargin = param->nLeftMargin;
	lpTickerBox->nRightMargin = param->nRightMargin;
	
	lpTickerBox->hBgImageLeft = param->hBgImageLeft;
	lpTickerBox->hBgImageCenter = param->hBgImageCenter;
	lpTickerBox->hBgImageRight = param->hBgImageRight;

	lpTickerBox->bIsChanged = atFALSE;
	lpTickerBox->hDefaultFont = lpTickerBox->hCurFont = atCONTROL_GetFont(hControl);
	
	atCONTROL_AddControlTimer( hControl, 0, param->nTickerInterval, atTRUE, atFALSE, atFALSE );
}

static void atTICKERBOX_OnDestroy( atHCONTROL hControl )
{
	atLPCONTROL lpControl = atCONTROL_GetPtr(hControl);
	atLPTICKERBOX	lpTickerBox;

	if ( lpControl == atNULL ) return;

	lpTickerBox = (atLPTICKERBOX)atCONTROL_GetDataPtr( hControl );

 	if( lpTickerBox->hStrBitmap ){
	  atUIAPI_DestroyStrBitmap( lpTickerBox->hStrBitmap );
	  lpTickerBox->hStrBitmap  = atNULL;
	}

	if( lpTickerBox->hCurFont != lpTickerBox->hDefaultFont ) {
		atUIAPI_DestroyFont(lpTickerBox->hCurFont);
		lpTickerBox->hCurFont = lpTickerBox->hDefaultFont;
		lpTickerBox->nFontSize = 0;
	}
}

static void atTICKERBOX_OnDraw( atHGDC hCanvas, atHCONTROL hControl, atBOOL bFocused )
{
	atLPCONTROL lpControl = atCONTROL_GetPtr(hControl);
	atLPTICKERBOX lpTickerBox;
	atRECT rect;
	atCOLOR colorLine, colorBG, colorTxt;
	int valign;

	if ( atCONTROL_IsVisible(hControl) == atFALSE ) return;

	atCONTROL_PrepareGDC( hControl, hCanvas, bFocused );

	lpTickerBox = (atLPTICKERBOX)atCONTROL_GetDataPtr( hControl );

	atCONTROL_GetControlRect( hControl, &rect );

//	colorFG = atUIAPI_GetFrColor(hCanvas);
//	colorBG = atUIAPI_GetBgColor(hCanvas);

	if( bFocused ){
		if( lpControl->Base.bEnable ){
			colorLine = lpControl->tControlColor.clFocusedFrColor;
			colorBG = lpControl->tControlColor.clFocusedBgColor;
			colorTxt = lpControl->tControlColor.clFocusedTxtColor;
		} else {
			colorLine = atUIAPI_GetRGB(76, 149, 248);
			colorBG = lpControl->tControlColor.clFocusedBgColor;
			colorTxt = lpControl->tControlColor.clDisableTxtColor;
		}
	} else {
		colorLine = lpControl->tControlColor.clNormalFrColor;
		colorBG = lpControl->tControlColor.clNormalBgColor;
		colorTxt = lpControl->tControlColor.clNormalTxtColor;
	}

	if( lpTickerBox->uAttr & atTICKERBOXATTR_OPAQUE ){
		if( lpTickerBox->hBgImageLeft && lpTickerBox->hBgImageCenter  && lpTickerBox->hBgImageRight ) {
			atUIAPI_DrawClient1Line(hCanvas, 0, 0, RECT_WIDTH(rect), lpTickerBox->hBgImageLeft, lpTickerBox->hBgImageCenter, lpTickerBox->hBgImageRight );
		} else {
			atUIAPI_FillRect(hCanvas, 0, 0, RECT_WIDTH(rect), RECT_HEIGHT(rect), colorBG);
		}
	}
	if( lpTickerBox->uAttr & atTICKERBOXATTR_OUTLINE )
		atUIAPI_DrawRect(hCanvas, 0, 0, RECT_WIDTH(rect), RECT_HEIGHT(rect), colorLine, 1);

	valign = lpTickerBox->uAttr & atTICKERBOXATTR_ALIGN_MASK;
	
	/*
	// Kyle Start [Delete 2009/07/21-18:09]
	atUIAPI_DrawStrBitmapAlign( hCanvas, lpTickerBox->nLeftMargin, RECT_HEIGHT(rect)/2, lpTickerBox->hStrBitmap, 
								(lpTickerBox->nTickerTick<0)?0:lpTickerBox->nTickerTick, 
								RECT_WIDTH(rect) - (lpTickerBox->nLeftMargin + lpTickerBox->nRightMargin), 
								ALIGN_LEFT_MIDDLE, colorTxt);
	// Kyle [Delete 2009/07/21-18:09] End
	*/

	// 이미지의 iStartPos 변경 방식으로만 하이라이트를 처리하기 힘들어서 X축 위치와 이미지 Width 로 구현하였음.
	// Kyle Start [Add 2009/07/21-18:09]
	{
		atINT nDrawLen = atUIAPI_strlen(lpTickerBox->szText);
		if(nDrawLen > 0)
		{
			atINT nWordWidth = 0, nDrawOffset = 0;
			atINT nTickOffset, ii;
			atINT nDrawPos, nDispWidth;
			atTCHAR szWord[2] = {0, };

			nTickOffset = (lpTickerBox->nTickerTick < 0) ? 0 : lpTickerBox->nTickerTick;
			nDispWidth = RECT_WIDTH(rect) - (lpTickerBox->nLeftMargin + lpTickerBox->nRightMargin);
			for(ii = 0 ; ii < nDrawLen ; ii++)
			{
				szWord[0] = lpTickerBox->szText[ii];
				nWordWidth = atUIAPI_GetStringWidth(atCONTROL_GetFont(hControl), szWord, -1);
				nDrawPos = lpTickerBox->nLeftMargin-nTickOffset+nDrawOffset;
				
				if(nDrawPos <= nDispWidth)
				{
					if( valign == atTICKERBOXATTR_ALIGN_TOP )
						atUIAPI_DrawStrBitmap( hCanvas, nDrawPos, 0, 
												lpTickerBox->hStrBitmap, 
												nDrawOffset,
												(nDrawPos+nWordWidth) > nDispWidth ? nWordWidth - (nDrawPos+nWordWidth-nDispWidth) : nWordWidth,
												lpTickerBox->bWordHL[ii] ? lpTickerBox->clHLColor : colorTxt);
					else if( valign == atTICKERBOXATTR_ALIGN_MIDDLE )
						atUIAPI_DrawStrBitmapAlign( hCanvas, nDrawPos, RECT_HEIGHT(rect)/2, 
												lpTickerBox->hStrBitmap, 
												nDrawOffset,
												(nDrawPos+nWordWidth) > nDispWidth ? nWordWidth - (nDrawPos+nWordWidth-nDispWidth) : nWordWidth,
												ALIGN_LEFT_MIDDLE, 
												lpTickerBox->bWordHL[ii] ? lpTickerBox->clHLColor : colorTxt);
					else if( valign == atTICKERBOXATTR_ALIGN_BOTTOM )
						atUIAPI_DrawStrBitmapAlign( hCanvas, nDrawPos, RECT_HEIGHT(rect), 
												lpTickerBox->hStrBitmap, 
												nDrawOffset,
												(nDrawPos+nWordWidth) > nDispWidth ? nWordWidth - (nDrawPos+nWordWidth-nDispWidth) : nWordWidth,
												ALIGN_LEFT_BOTTOM, 
												lpTickerBox->bWordHL[ii] ? lpTickerBox->clHLColor : colorTxt);
				}

				nDrawOffset += nWordWidth;
			}
		}
	} 
		
	//atUIAPI_DrawRect(hCanvas, 0, 0, RECT_WIDTH(rect), RECT_HEIGHT(rect), atUI_COLOR_RED, 1);	// Kyle Test code
	// Kyle [Add 2009/07/21-18:09] End
}					  


static void atTICKERBOX_OnTimer( atHCONTROL hControl, atLONG id )
{
	atLPTICKERBOX lpTickerBox	= (atLPTICKERBOX)atCONTROL_GetDataPtr( hControl );

	if( lpTickerBox->bIsTicking == atFALSE ) return;

	lpTickerBox->nTickerTick += lpTickerBox->nTickerInc;
	if( lpTickerBox->nTickerTick < 0 ) return;
	
	if( lpTickerBox->nTickerTick >= lpTickerBox->nFullWidth ) 
		atTICKERBOX_ResetTick( hControl );
	
//atUIAPI_EnableTimer( atCONTROL_GetControlTimer(hControl,0), atFALSE );	
//	atCONTROL_Draw( hControl, atTRUE );
	atCONTROL_Refresh( hControl );
//atUIAPI_EnableTimer( atCONTROL_GetControlTimer(hControl,0), atTRUE );	
}


static int TickerBoxProc( atHCONTROL hControl, int nMsg, long lParam1, long lParam2 )
{
	atLPCONTROL lpControl = atCONTROL_GetPtr(hControl);
	atLPTICKERBOX	lpTickerBox = (atLPTICKERBOX)atCONTROL_GetDataPtr( hControl );
	int r = 0;

	switch( nMsg )
	{
		case CTLMSG_INIT:
			atTICKERBOX_OnInit( hControl, (_CreateParam *)lParam1 );
			break;
		case CTLMSG_RELEASE:
			atTICKERBOX_OnDestroy( hControl );
			r = 1;
			break;
		case CTLMSG_PAINT:
			atTICKERBOX_OnDraw( atCONTROL_GetGDC(hControl), hControl, (atBOOL)lParam2 );
			r = 1;
			break;
		case CTLMSG_TIMER:
			atTICKERBOX_OnTimer( hControl, lParam1 );
			r = 1;
			break;
		case CTLMSG_HIDE:
		case CTLMSG_SUSPEND:
			if( lpTickerBox->bIsTicking )
				atCONTROL_StopControlTimer( hControl, 0 );
//				atUIAPI_EnableTimer( atCONTROL_GetControlTimer(hControl,0), atFALSE );
			r = 1;
			break;
		case CTLMSG_SHOW:
		case CTLMSG_RESUME:
			if( lpTickerBox->bIsTicking )
				atCONTROL_StartControlTimer( hControl, 0 );
//				atUIAPI_EnableTimer( atCONTROL_GetControlTimer(hControl,0), atTRUE );
			r = 1;
			break;
	}

	return r;
}



// ==================================================================================


atLPTSTR atTICKERBOX_GetText(atHCONTROL hControl)
{
	atLPTICKERBOX	lpTickerBox = atCONTROL_GetDataPtr( hControl );
	if( lpTickerBox == atNULL ) return atNULL;

	return lpTickerBox->szText;
}

void atTICKERBOX_SetText(atHCONTROL hControl, atLPTSTR szNewText, atINT nLen)
{
	atLPTICKERBOX	lpTickerBox = atCONTROL_GetDataPtr( hControl );
	atSIZE size;
	if( lpTickerBox == atNULL ) return;

	if( nLen < 0 ) nLen = atUIAPI_strlen(szNewText);
	//if( atUIAPI_strcmp(lpTickerBox->szText, szNewText) == 0 ) return;
	
	atUIAPI_strncpy( lpTickerBox->szText, szNewText, nLen );
	
	if( lpTickerBox->hStrBitmap ){
		atUIAPI_DestroyStrBitmap( lpTickerBox->hStrBitmap );
		lpTickerBox->hStrBitmap = atNULL;
	}
	if( nLen == 0 ) return;
	
	lpTickerBox->hStrBitmap = atUIAPI_CreateStrBitmap( atCONTROL_GetFont(hControl), szNewText, nLen );
	atUIAPI_GetStrBitmapMetric( lpTickerBox->hStrBitmap, &size );
	lpTickerBox->nFullWidth = size.nWidth;
	//lpTickerBox->hCurFont = atCONTROL_GetFont(hControl);

//	atTICKERBOX_ResetTick(hControl);
	
//	if ( atCONTROL_IsVisible(hControl) ){
//		atCONTROL_Draw( hControl, atFALSE );
//	}
}

atU32 atTICKERBOX_GetAttr(atHCONTROL hControl )
{
	atLPTICKERBOX	lpTickerBox = atCONTROL_GetDataPtr( hControl );
	if( lpTickerBox == atNULL ) return 0;

	return lpTickerBox->uAttr;
}

void atTICKERBOX_SetAttr(atHCONTROL hControl, atU32 uAttr )
{
	atLPTICKERBOX	lpTickerBox = atCONTROL_GetDataPtr( hControl );
	if( lpTickerBox == atNULL ) return;

	lpTickerBox->uAttr = uAttr;
}

void atTICKERBOX_SetTickerInterval(atHCONTROL hControl, atLONG interval )
{
//	atUIAPI_SetTimerInterval( atCONTROL_GetControlTimer(hControl,0), interval );
	atCONTROL_SetControlTimerInterval( hControl, 0, interval );
}

void atTICKERBOX_StartTicker(atHCONTROL hControl )
{
	atLPTICKERBOX	lpTickerBox = (atLPTICKERBOX)atCONTROL_GetDataPtr( hControl );
	if( lpTickerBox == atNULL ) return;

	if( lpTickerBox->hCurFont != atCONTROL_GetFont(hControl) ){
		atTICKERBOX_SetText( hControl, lpTickerBox->szText, -1 );
	}

	atTICKERBOX_ResetTick( hControl );

	if( atCONTROL_IsVisible(hControl) ){
//		atUIAPI_EnableTimer( lpTickerBox->hTickerTimer, atTRUE );
		atCONTROL_StartControlTimer( hControl, 0 );
	}
	
	lpTickerBox->bIsTicking = atTRUE;	
}

void atTICKERBOX_StopTicker(atHCONTROL hControl )
{
	atLPTICKERBOX	lpTickerBox = (atLPTICKERBOX)atCONTROL_GetDataPtr( hControl );
	if( lpTickerBox == atNULL ) return;

//	atUIAPI_EnableTimer( lpTickerBox->hTickerTimer, atFALSE );
	atCONTROL_StopControlTimer( hControl, 0 );
	
	lpTickerBox->bIsTicking = atFALSE;	
}

void atTICKERBOX_ResetTick(atHCONTROL hControl )
{
	atLPTICKERBOX	lpTickerBox = (atLPTICKERBOX)atCONTROL_GetDataPtr( hControl );
	if( lpTickerBox == atNULL ) return;

	lpTickerBox->nTickerTick = lpTickerBox->nTickerInitTick;
}

void atTICKERBOX_SetBGBitmap(atHCONTROL hControl, atHBITMAP hBgImageLeft, atHBITMAP hBgImageCenter, atHBITMAP hBgImageRight )
{
	atLPTICKERBOX	lpTickerBox = (atLPTICKERBOX)atCONTROL_GetDataPtr( hControl );
	if( lpTickerBox == atNULL ) return;

	lpTickerBox->hBgImageLeft = hBgImageLeft;
	lpTickerBox->hBgImageCenter = hBgImageCenter;
	lpTickerBox->hBgImageRight = hBgImageRight;
}

atBOOL atTICKERBOX_SetFontSize(atHCONTROL hControl, int nFontSize)
{
	atLPTICKERBOX	lpTickerBox = (atLPTICKERBOX)atCONTROL_GetDataPtr( hControl );
	if( lpTickerBox == atNULL ) return atFALSE;
	if( lpTickerBox->nFontSize == nFontSize) return atFALSE;

	if( nFontSize < 0 ) nFontSize = 0;
	if( lpTickerBox->hCurFont != lpTickerBox->hDefaultFont ) {
		atUIAPI_DestroyFont(lpTickerBox->hCurFont);
		lpTickerBox->hCurFont = lpTickerBox->hDefaultFont;
	}

	if( nFontSize ) {
		lpTickerBox->hCurFont = atUIAPI_CreateFont(atUIAPI_FACE_SYSTEM, nFontSize, atUIAPI_STYLE_PLAIN, atTRUE);
		if(!lpTickerBox->hCurFont) lpTickerBox->hCurFont = lpTickerBox->hDefaultFont;
		atCONTROL_SetFont( hControl, lpTickerBox->hCurFont );
	}

	lpTickerBox->nFontSize = nFontSize;
	return atTRUE;
}

atINT atTICKERBOX_GetFontSize(atHCONTROL hControl)
{
	atLPTICKERBOX	lpTickerBox = (atLPTICKERBOX)atCONTROL_GetDataPtr( hControl );
	return lpTickerBox ? lpTickerBox->nFontSize : -1;
}

atBOOL atTICKERBOX_SetHLWordAtrribute(atHCONTROL hControl, atBYTE * bNewWordHL, atINT nLen)
{
	atLPTICKERBOX	lpTickerBox = (atLPTICKERBOX)atCONTROL_GetDataPtr( hControl );
	if( lpTickerBox == atNULL ) return atFALSE;
	
	atUIAPI_memset(lpTickerBox->bWordHL, 0x00, sizeof(lpTickerBox->bWordHL));
	atUIAPI_memcpy(lpTickerBox->bWordHL, bNewWordHL, nLen);
	return atTRUE;
}

atBOOL atTICKERBOX_SetColorHighLight(atHCONTROL hControl, atCOLOR colorHL)
{
	atLPTICKERBOX	lpTickerBox = atCONTROL_GetDataPtr( hControl );
	if( lpTickerBox == atNULL ) return atFALSE;

	lpTickerBox->clHLColor = colorHL;
	return atTRUE;
}
