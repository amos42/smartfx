/**
  @file UI_gdi.c
  @brief GDI (Graphic Draw Interface)에 관한 API
  @remark GDI (Graphic Draw Interface)에 관한 API
  @author Amos
  @date 2010-04-19 오전 11:41:00
  @see UI_gdi.h
*/
#include "UI_Types.h"
#include "GDC.h"


typedef struct _tagatGDC {
    atBOOL			bActivate;
    atREGION        Rgn;
//  atVOID *        lpScrBuf;

    atHFONT         hFont;
    atCOLOR         FrColor, BgColor;

    OEM_UI_GDC     lpOemCanvas;
} atGDC, *atLPGDC;

/*
typedef struct _tagatBITMAP {
	atINT		  	nWidth, nHeight;
//	atBOOL 			bOwnMask;
//	atHBITMAPMASK	hBitmapMask;
	OEM_IMAGE 		lpOemBitmap;
} atBITMAP, *atLPBITMAP;
*/

static atHGDC g_ScreenGDC;
static atRECT g_ClipRect;
static atBOOL       g_bFlushScreenEnabled;




/** 
 @brief 
 
 ......

 @param 
 @return 없음
*/
atVOID atUIAPI_FinalGraphic( NOARGS )
{
	g_bFlushScreenEnabled = atFALSE;
		
	atUIAPI_DestroyGDC( g_ScreenGDC );
	
	if( tOemFn.fnCloseGraphicSystem != atNULL ){
		tOemFn.fnCloseGraphicSystem();
	}
}


/** 
 @brief 
 
 ......

 @param 
 @return 없음
*/
atBOOL atUIAPI_SetScreenResolution( atINT nWidth, atINT nHeight )
{
	atBOOL r = atFALSE;
	atREGION rgn;
	
	if( tOemFn.fnSetScreenResolution != atNULL ){
		r = tOemFn.fnSetScreenResolution( nWidth, nHeight );
	}
	if( !r ) return atFALSE;

	atUIAPI_SetScreenClipRect( 0, 0, atUIAPI_GetScreenWidth()-1, atUIAPI_GetScreenHeight()-1 );
	
	SET_REGION( rgn, 0, 0, atUIAPI_GetScreenWidth(), atUIAPI_GetScreenHeight() );
	atUIAPI_SetGDCArea( g_ScreenGDC, &rgn );
		
	return atTRUE;
}


/** 
 @brief 
 
 ......

 @param 
 @return 없음
*/
atBOOL	  atUIAPI_GetScreenDPI( atINT *lpnX_DPI, atINT *lpnY_DPI )
{
	if( tOemFn.fnGetScreenDPI != atNULL ){
		return tOemFn.fnGetScreenDPI( lpnX_DPI, lpnY_DPI );
	}

	return 0;
}


/** 
 @brief 
 
 ......

 @param 
 @return 없음
*/
atVOID atUIAPI_FlushScreenEnable(atBOOL bEnable)
{
	g_bFlushScreenEnabled = bEnable;
}


/** 
 @brief 
 
 ......

 @param 
 @return 없음
*/
atINT atUIAPI_GetScreenWidth( NOARGS )
{
	if( tOemFn.fnGetScreenWidth != atNULL ){
		return tOemFn.fnGetScreenWidth();
	}

	return 0;
}


/** 
 @brief 
 
 ......

 @param 
 @return 없음
*/
atINT atUIAPI_GetScreenHeight( NOARGS )
{
	if( tOemFn.fnGetScreenHeight != atNULL ){
		return tOemFn.fnGetScreenHeight();
	}

	return 0;
}


/** 
 @brief 
 
 ......

 @param 
 @return 없음
*/
atBOOL		atUIAPI_BeginDraw( NOARGS )
{
	if( tOemFn.fnBeginDraw != atNULL ){
		return tOemFn.fnBeginDraw();
	}

	return atFALSE;
}


/** 
 @brief 
 
 ......

 @param 
 @return 없음
*/
atVOID		atUIAPI_EndDraw( NOARGS )
{
	if( tOemFn.fnEndDraw != atNULL ){
		tOemFn.fnEndDraw();
	}
}


/** 
 @brief 
 
 ......

 @param 
 @return 없음
*/
atVOID	atUIAPI_FlushScreenRgn( atINT nX, atINT nY, atINT nWidth, atINT nHeight )
{  	
	if( !g_bFlushScreenEnabled ) return;
		
	if( tOemFn.fnFlushScreenRgn != atNULL ){
		tOemFn.fnFlushScreenRgn(nX, nY, nWidth, nHeight); 
	}
}


/** 
 @brief 
 
 ......

 @param 
 @return 없음
*/
atVOID	atUIAPI_FlushScreen( NOARGS )
{
	atUIAPI_FlushScreenRgn( 0, 0, atUIAPI_GetScreenWidth(), atUIAPI_GetScreenHeight() );
}


/** 
 @brief 
 
 ......

 @param 
 @return 없음
*/
atVOID    atUIAPI_ClearScreen( atCOLOR nColor )
{
    atUIAPI_Clear( g_ScreenGDC, nColor );
}


/** 
 @brief 
 
 ......

 @param 
 @return 없음
*/
atVOID		atUIAPI_SetScreenClipRect( atINT nStartX, atINT nStartY, atINT nEndX, atINT nEndY )
{
	g_ClipRect.nStartX = nStartX;
	g_ClipRect.nStartY = nStartY;
	g_ClipRect.nEndX = nEndX;
	g_ClipRect.nEndY = nEndY;
}


/** 
 @brief 
 
 ......

 @param 
 @return 없음
*/
atVOID		atUIAPI_SetScreenClipRect2( atRECT *lpRect )
{
	atUIAPI_memcpy( (atLPVOID)&g_ClipRect, (const atLPVOID)lpRect, sizeof(atRECT) );
}


/** 
 @brief 
 
 ......

 @param 
 @return 없음
*/
atRECT	*	atUIAPI_GetScreenClipRect( NOARGS )
{
	return &g_ClipRect;
}


/** 
 @brief 
 
 ......

 @param 
 @return 없음
*/
atHGDC atUIAPI_CreateGDC( atINT nX, atINT nY, atINT nWidth, atINT nHeight, atINT bpp, atDWORD dwAttr )
{
	atHGDC hCanvas;
	atLPGDC lpCanvas = (atLPGDC)UI_MEMALLOC(sizeof(atGDC));
	if( !lpCanvas ) return atNULL;

	atUIAPI_memset( lpCanvas, 0, sizeof(atGDC) );

	if( tOemFn.fnCreateCanvas != atNULL ){
		lpCanvas->lpOemCanvas = tOemFn.fnCreateCanvas( nX, nY, nWidth, nHeight, bpp, dwAttr );
	} else {
		lpCanvas->lpOemCanvas = atNULL;
	}

	if( lpCanvas->lpOemCanvas == atNULL ){
		UI_MEMFREE( lpCanvas );
		return atNULL;
	}

	lpCanvas->BgColor = atUIAPI_GetRGB(255,255,255);
	lpCanvas->FrColor = atUIAPI_GetRGB(0,0,0);
	lpCanvas->hFont = atUIAPI_GetStockFont(0);
//	lpCanvas->Rgn.nX = nX;
//	lpCanvas->Rgn.nY = nY;
//	lpCanvas->Rgn.nWidth = nWidth;
//	lpCanvas->Rgn.nHeight = nHeight;

	lpCanvas->bActivate = atTRUE;

	hCanvas = (atHGDC)lpCanvas;
	
	atUIAPI_SetFont( hCanvas, atUIAPI_GetStockFont(0) );

	return hCanvas;
}


/** 
 @brief 
 
 ......

 @param 
 @return 없음
*/
atVOID	atUIAPI_DestroyGDC( atHGDC hCanvas )
{
	atLPGDC lpCanvas = (atLPGDC)hCanvas;

	if( lpCanvas == atNULL ) return;

	lpCanvas->bActivate = atFALSE;

	if( tOemFn.fnDestroyCanvas != atNULL ){
		tOemFn.fnDestroyCanvas(lpCanvas->lpOemCanvas);
	}

	UI_MEMFREE( lpCanvas );
}


/** 
 @brief 
 
 ......

 @param 
 @return 없음
*/
atVOID  atUIAPI_ActivateGDC( atHGDC hCanvas, atBOOL bActivate )
{
	atLPGDC lpCanvas = (atLPGDC)hCanvas;

	if( lpCanvas == atNULL ) return;

	lpCanvas->bActivate = bActivate;
}


/** 
 @brief 
 
 ......

 @param 
 @return 없음
*/
atLPVOID atUIAPI_GetOEMGDC(atHGDC hCanvas)
{
	atLPGDC lpCanvas = (atLPGDC)hCanvas;

	if( lpCanvas == atNULL ) return atNULL;

	return (atVOID *)lpCanvas->lpOemCanvas;
}


/** 
 @brief 
 
 ......

 @param 
 @return 없음
*/
atBOOL atUIAPI_IsCanvasActivate(atHGDC hCanvas)
{
	atLPGDC lpCanvas = (atLPGDC)hCanvas;

	if( lpCanvas == atNULL ) return atFALSE;

	return lpCanvas->bActivate;
}


/** 
 @brief 
 
 ......

 @param 
 @return 없음
*/
atVOID atUIAPI_GetCanvasAttr( atHGDC hCanvas, atUI_CANVAS_ATTR *lpCanvasAttr )
{
	atLPGDC lpCanvas = (atLPGDC)hCanvas;

	if( lpCanvasAttr == atNULL ) return;
	if( lpCanvas == atNULL ){
		atUIAPI_memset( lpCanvasAttr, 0, sizeof(atUI_CANVAS_ATTR) );
		return;
	}

	if( tOemFn.fnGetCanvasAttr != atNULL ){
		tOemFn.fnGetCanvasAttr( lpCanvas->lpOemCanvas, lpCanvasAttr );
	}
}


/** 
 @brief 
 
 ......

 @param 
 @return 없음
*/
atVOID atUIAPI_GetClipRect( atHGDC hCanvas, atRECT *lpRect )
{
	atLPGDC lpCanvas = (atLPGDC)hCanvas;

	if( lpCanvas == atNULL ) return;

	if( tOemFn.fnGetCanvasClipRect != atNULL ){
		tOemFn.fnGetCanvasClipRect( lpCanvas->lpOemCanvas, lpRect);
	}
}


/** 
 @brief 
 
 ......

 @param 
 @return 없음
*/
atVOID atUIAPI_SetClipRect( atHGDC hCanvas, atRECT *lpRect )
{
	atLPGDC lpCanvas = (atLPGDC)hCanvas;
	atRECT clip;
	atREGION rgn;

	if( lpCanvas == atNULL ) return;

	atUIAPI_GetGDCArea( hCanvas, &rgn );
	atUIAPI_MoveRect( &clip, lpRect, rgn.nX, rgn.nY );

	if( atUIAPI_IntersectRect( &clip, &clip, atUIAPI_GetScreenClipRect() ) ){
		atUIAPI_MoveRect( &clip, &clip, -rgn.nX, -rgn.nY );
		if( tOemFn.fnSetCanvasClipRect != atNULL ){
			tOemFn.fnSetCanvasClipRect( lpCanvas->lpOemCanvas, &clip);
		}
		lpCanvas->bActivate = atTRUE;
	} else {
		lpCanvas->bActivate = atFALSE;
	}

//	tOemFn.fnSetCanvasClipRect( lpCanvas->lpOemCanvas, lpRect);
}


/** 
 @brief 
 
 ......

 @param 
 @return 없음
*/
atVOID atUIAPI_GetGDCArea(atHGDC hCanvas, atREGION *rgnArea)
{
	atLPGDC lpCanvas = (atLPGDC)hCanvas;

	if( lpCanvas == atNULL ) return;

	tOemFn.fnGetCanvasArea( lpCanvas->lpOemCanvas, rgnArea );
}


/** 
 @brief 
 
 ......

 @param 
 @return 없음
*/
atBOOL atUIAPI_SetGDCArea(atHGDC hCanvas, atREGION *rgnArea)
{
	atLPGDC lpCanvas = (atLPGDC)hCanvas;
	atRECT clip;

	if( lpCanvas == atNULL ) return atFALSE;

	if( tOemFn.fnSetCanvasArea != atNULL ){
		tOemFn.fnSetCanvasArea( lpCanvas->lpOemCanvas, rgnArea );
	}

	REGION2RECT( clip, *rgnArea );

	if( atUIAPI_IntersectRect( &clip, &clip, atUIAPI_GetScreenClipRect() ) ){
		atUIAPI_MoveRect( &clip, &clip, -rgnArea->nX, -rgnArea->nY );
		if( tOemFn.fnSetCanvasClipRect != atNULL ){
			tOemFn.fnSetCanvasClipRect( lpCanvas->lpOemCanvas, &clip);
		}
		lpCanvas->bActivate = atTRUE;
	} else {
		lpCanvas->bActivate = atFALSE;
	}

	lpCanvas->Rgn.nX = rgnArea->nX;
	lpCanvas->Rgn.nY = rgnArea->nY;

	return atTRUE;
}


/** 
 @brief 
 
 ......

 @param 
 @return 없음
*/
atVOID atUIAPI_SetGDCPos(atHGDC hCanvas, atINT nX, atINT nY)
{
	atREGION rgn;

	atUIAPI_GetGDCArea( hCanvas, &rgn );
	rgn.nX = nX;
	rgn.nY = nY;
	atUIAPI_SetGDCArea( hCanvas, &rgn );
}


/** 
 @brief 
 
 ......

 @param 
 @return 없음
*/
atVOID atUIAPI_CopyGDC( atHGDC hDesCanvas, atINT nDesX, atINT nDesY, atHGDC hSrcCanvas, atINT nX, atINT nY, atINT nWidth, atINT nHeight )
{
	atLPGDC lpDesCanvas = (atLPGDC)hDesCanvas;
	atLPGDC lpSrcCanvas = (atLPGDC)hSrcCanvas;

	if( !atUIAPI_IsCanvasActivate(hDesCanvas) || !atUIAPI_IsCanvasActivate(hSrcCanvas) ) return;

	if( tOemFn.fnCopyCanvas != atNULL ){
		tOemFn.fnCopyCanvas( lpDesCanvas->lpOemCanvas, nDesX, nDesY, lpSrcCanvas->lpOemCanvas, nX, nY, nWidth, nHeight );
	}
}


/** 
 @brief 
 
 ......

 @param 
 @return 없음
*/
atVOID atUIAPI_Cvs2Scr(atHGDC hCanvas, atINT *nX, atINT *nY)
{
	atLPGDC lpCanvas = (atLPGDC)hCanvas;

	if( lpCanvas == atNULL ) return;
	
	*nX += lpCanvas->Rgn.nX;
	*nY += lpCanvas->Rgn.nY;
}


/** 
 @brief 
 
 ......

 @param 
 @return 없음
*/
atVOID atUIAPI_Cvs2Scr2(atHGDC hCanvas, atINT *nX1, atINT *nY1, atINT *nX2, atINT *nY2)
{
	atLPGDC lpCanvas = (atLPGDC)hCanvas;

	if( lpCanvas == atNULL ) return;
	
//	*nY1 += GetAnnounceHeight();
//	*nY2 += GetAnnounceHeight();

	*nX1 += lpCanvas->Rgn.nX;
	*nY1 += lpCanvas->Rgn.nY;
	*nX2 += lpCanvas->Rgn.nX;
	*nY2 += lpCanvas->Rgn.nY;
}


/** 
 @brief 
 
 ......

 @param 
 @return 없음
*/
atHGDC atUIAPI_GetScreenCanvas( NOARGS )
{
	return g_ScreenGDC;
}


/** 
 @brief 
 
 ......

 @param 
 @return 없음
*/
atVOID	atUIAPI_FlushRgn( atHGDC hCanvas, atINT nX, atINT nY, atINT nWidth, atINT nHeight )
{  	
    atREGION rgn;

	if( hCanvas == atNULL ) return;
	if( !atUIAPI_IsCanvasActivate(hCanvas) ) return;

	atUIAPI_GetGDCArea(hCanvas, &rgn);
	atUIAPI_FlushScreenRgn( rgn.nX + nX, rgn.nY + nY, nWidth, nHeight ); 
}


/** 
 @brief 
 
 ......

 @param 
 @return 없음
*/
atVOID	atUIAPI_Flush( atHGDC hCanvas )
{  
    atREGION rgn;
	
	if( hCanvas == atNULL ) return;
	if( !atUIAPI_IsCanvasActivate(hCanvas) ) return;

	atUIAPI_GetGDCArea(hCanvas, &rgn);
	atUIAPI_FlushRgn( hCanvas, 0, 0, rgn.nWidth, rgn.nHeight ); 
}


