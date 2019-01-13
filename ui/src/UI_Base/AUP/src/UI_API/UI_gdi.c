/**
  @file UI_gdi.c
  @brief GDI (Graphic Draw Interface)에 관한 API
  @remark GDI (Graphic Draw Interface)에 관한 API
  @author Amos
  @date 2010-04-19 오전 11:41:00
  @see UI_gdi.h
*/
#include "UI_Types.h"
#include "UI_API/UI_API.h"
#include "UI_Util.h"
#include "OEM_UI_GDI.h"
#include <stdio.h>


#define RGB24TORGB16(_X_)   (unsigned short)((((unsigned long)(_X_) & 0x00F80000L) >> 8) | (((unsigned long)(_X_) & 0x0000FC00L) >> 5) | (((unsigned long)(_X_) & 0x000000F8L) >> 3))
#define RGB16TORGB24(_X_)   (unsigned short)((((unsigned long)(_X_) & 0x0000F800L) << 8) | (((unsigned long)(_X_) & 0x000007E0L) << 5) | (((unsigned long)(_X_) & 0x0000001FL) << 3))
#define RGB24ITORGB16(_X_)   (unsigned short)((((unsigned long)(_X_) & 0x00F80000L) >> 19) | (((unsigned long)(_X_) & 0x0000FC00L) >> 5) | (((unsigned long)(_X_) & 0x000000F8L) << 8))


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


#define USES_YUV422_TO_RGB565_TABLE  0


#if USES_YUV422_TO_RGB565_TABLE == 1
static long _rgb_y_tbl[256];
static long _r_v_tbl[256];
static long _g_u_tbl[256], _g_v_tbl[256];
static long _b_u_tbl[256];
#endif


static OEM_UI_GDI_T tOemFn;

atVOID	OEM_UI_GDI_SetOemFn( OEM_UI_GDI_T *fnOem )
{
	tOemFn = *fnOem;
}

atVOID OEM_UI_GDI_GetOemFn( OEM_UI_GDI_T *fnOem )
{
	*fnOem = tOemFn;
}


/** 
 @brief Graphic 시스템을 초기화 한다. (내부용)
 
 Graphic 시스템을 초기화 한다.

 @param lSysInst [in] : UI엔진의 인스턴스 (OEM 전달용. 내부용)
 @param lInitData1, lInitData2 [in] : 초기화 파라미터 (OEM 전달용. 내부용)
 @return 성공하면 atTRUE, 실패하면 atFALSE
 @remark 내부용으로 사용된다.
*/
atBOOL atUIAPI_InitGraphic( atLONG lSysInst, atLONG lInitData1, atLONG lInitData2 )
{
	if( tOemFn.fnInitGraphicSystem != atNULL ){
		tOemFn.fnInitGraphicSystem( lSysInst, lInitData1, lInitData2 );
	}

	g_ScreenGDC	= atUIAPI_CreateGDC( 0, 0, atUIAPI_GetScreenWidth(), atUIAPI_GetScreenHeight(), atUIAPI_GetScreenBPP(), 0L );

	atUIAPI_SetScreenClipRect( 0, 0, atUIAPI_GetScreenWidth()-1, atUIAPI_GetScreenHeight()-1 );
	
	atUIAPI_ClearScreen( atUI_COLOR_DARKGRAY );
#if USES_YUV422_TO_RGB565_TABLE == 1
	{
		int i;

		// r = (y - 16) * 1.164 + (v - 128) * 1.596;
		// g = (y - 16) * 1.164 - (u - 128) * 0.391 - (v - 128) * 0.813;
		// b = (y - 16) * 1.164 + (u - 128) * 2.018;
		for( i = 0; i < 256; i++ ){
			_rgb_y_tbl[i] = (long)((i - 16) * 1.164 * 65536);
			_r_v_tbl[i] = (long)((i - 128) * 1.596 * 65536);
			_g_u_tbl[i] = (long)((i - 128) * 0.391 * 65536);
			_g_v_tbl[i] = (long)((i - 128) * 0.813 * 65536);
			_b_u_tbl[i] = (long)((i - 128) * 2.018 * 65536);
		}
	}
#endif

	g_bFlushScreenEnabled = atTRUE;
		
	return atTRUE;
}


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
atCOLOR atUIAPI_GetRGB(atBYTE r, atBYTE g, atBYTE b)
{
	if( tOemFn.fnGetRGB != atNULL ){
		return tOemFn.fnGetRGB( r, g, b );
	}

	return 0x0000;
}


/** 
 @brief 
 
 ......

 @param 
 @return 없음
*/
atBYTE atUIAPI_GetRValue(atCOLOR nColor)
{
	if( tOemFn.fnGetRValue != atNULL ){
		return tOemFn.fnGetRValue( nColor );
	}

	return 0x00;
}


/** 
 @brief 
 
 ......

 @param 
 @return 없음
*/
atBYTE atUIAPI_GetGValue(atCOLOR nColor)
{
	if( tOemFn.fnGetGValue != atNULL ){
		return tOemFn.fnGetGValue( nColor );
	}

	return 0x00;
}


/** 
 @brief 
 
 ......

 @param 
 @return 없음
*/
atBYTE atUIAPI_GetBValue(atCOLOR nColor)
{
	if( tOemFn.fnGetBValue != atNULL ){
		return tOemFn.fnGetBValue( nColor );
	}

	return 0x00;
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
atINT atUIAPI_GetScreenBPP( NOARGS )
{
	if( tOemFn.fnGetScreenBPP != atNULL ){
		return tOemFn.fnGetScreenBPP();
	}

	return 0;
}


/** 
 @brief 
 
 ......

 @param 
 @return 없음
*/
atCOLOR atUIAPI_ConvColorFrom24BPP(atCOLOR nColor)
{
	if( atUIAPI_GetScreenBPP() == 24 )
		return nColor;
	else if( atUIAPI_GetScreenBPP() == 16 ){
		return RGB24TORGB16(nColor);
	} else {
		// 일단 무시...
		atBYTE r, g, b;
		r = 0;
		g = 0;
		b = 0;
		return atUIAPI_GetRGB(r,g,b);
	}
}


/** 
 @brief 
 
 ......

 @param 
 @return 없음
*/
atCOLOR atUIAPI_ConvColorFrom24iBPP(atCOLOR nColor)
{
	if( atUIAPI_GetScreenBPP() == 24 )
		return nColor;
	else if( atUIAPI_GetScreenBPP() == 16 ){
		return RGB24ITORGB16(nColor);
	} else {
		// 일단 무시...
		atBYTE r, g, b;
		r = 0;
		g = 0;
		b = 0;
		return atUIAPI_GetRGB(r,g,b);
	}
}


/** 
 @brief 
 
 ......

 @param 
 @return 없음
*/
atCOLOR atUIAPI_ConvColorFrom16BPP(atCOLOR nColor)
{
	if( atUIAPI_GetScreenBPP() == 16 )
		return nColor;
	else if( atUIAPI_GetScreenBPP() == 24 ){
		return RGB16TORGB24(nColor);
	} else {
		// 일단 무시...
		atBYTE r, g, b;
		r = 0;
		g = 0;
		b = 0;
		return atUIAPI_GetRGB(r,g,b);
	}
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
atHFONT atUIAPI_SetFont( atHGDC hCanvas, atHFONT nFont )
{
	atLPGDC lpCanvas = (atLPGDC)hCanvas;
	atHFONT fnt;

	if( lpCanvas == atNULL ) return atNULL;
	if( nFont == atNULL ) return atNULL;

	fnt = lpCanvas->hFont;

//	if( tOemFn.fnSetFont( lpCanvas->lpOemCanvas, atUIAPI_GetOEMPtr(nFont)) == atNULL ) return atNULL;
		
	lpCanvas->hFont = nFont;

	return fnt;
}


/** 
 @brief 
 
 ......

 @param 
 @return 없음
*/
atHFONT atUIAPI_GetFont( atHGDC hCanvas )
{
	atLPGDC lpCanvas = (atLPGDC)hCanvas;

	if( lpCanvas == atNULL ) return atNULL;

	return lpCanvas->hFont;
}


/** 
 @brief 
 
 ......

 @param 
 @return 없음
*/
atCOLOR atUIAPI_SetFrColor( atHGDC hCanvas, atCOLOR color )
{
	atLPGDC lpCanvas = (atLPGDC)hCanvas;
	atCOLOR color0;

	if( lpCanvas == atNULL ) return atUI_COLOR_BLACK;

	color0 = atUIAPI_GetFrColor(hCanvas);

	if( tOemFn.fnSetFrColor != atNULL ){
		tOemFn.fnSetFrColor( lpCanvas->lpOemCanvas, color );
	}

	lpCanvas->FrColor = color;

	return color0;
}	 


/** 
 @brief 
 
 ......

 @param 
 @return 없음
*/
atCOLOR atUIAPI_GetFrColor( atHGDC hCanvas )
{
	atLPGDC lpCanvas = (atLPGDC)hCanvas;

	if( lpCanvas == atNULL ) return atUI_COLOR_BLACK;

	return lpCanvas->FrColor;
}	 


/** 
 @brief 
 
 ......

 @param 
 @return 없음
*/
atCOLOR atUIAPI_SetBgColor( atHGDC hCanvas, atCOLOR color )
{
	atLPGDC lpCanvas = (atLPGDC)hCanvas;
	atCOLOR color0;

	if( lpCanvas == atNULL ) return atUI_COLOR_BLACK;

	color0 = atUIAPI_GetBgColor(hCanvas);

	if( tOemFn.fnSetBgColor != atNULL ){
		tOemFn.fnSetBgColor( lpCanvas->lpOemCanvas, color );
	}

	lpCanvas->BgColor = color;

	return color0;
}	 


/** 
 @brief 
 
 ......

 @param 
 @return 없음
*/
atCOLOR atUIAPI_GetBgColor( atHGDC hCanvas )
{
	atLPGDC lpCanvas = (atLPGDC)hCanvas;

	if( lpCanvas == atNULL ) return atUI_COLOR_BLACK;

	return lpCanvas->BgColor;
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
atCOLOR      atUIAPI_GetPixel( atHGDC hCanvas, atINT nX, atINT nY )
{
	atLPGDC lpCanvas = (atLPGDC)hCanvas;

	if( !atUIAPI_IsCanvasActivate(hCanvas) ) return atUIAPI_GetRGB(0,0,0);

	if( tOemFn.fnGetPixel != atNULL ){
		return tOemFn.fnGetPixel( lpCanvas->lpOemCanvas, nX, nY );
	}

	return 0x0000;
}


/** 
 @brief 
 
 ......

 @param 
 @return 없음
*/
atVOID        atUIAPI_DrawPixel( atHGDC hCanvas, atINT nX, atINT nY, atCOLOR nColor )
{
	atLPGDC lpCanvas = (atLPGDC)hCanvas;

	if( !atUIAPI_IsCanvasActivate(hCanvas) ) return;

	if( tOemFn.fnDrawPixel != atNULL ){
		tOemFn.fnDrawPixel( lpCanvas->lpOemCanvas, nX, nY, nColor );
	}
}


/** 
 @brief 
 
 '\n' 을 이용하여 사용한경우 위치 계산하여 화면에 표시

 @param 
 @return 없음
*/
atINT atUIAPI_DrawStringLine(atHGDC hCanvas, atHFONT hFont, atLPTSTR szText, atINT nLength, atRECT *lpRect, atCOLOR textColor )
{
	atSIZE aSize;
	atINT ret;

	if( hFont == atNULL ) return 0;

	if( nLength < 0 ) nLength = UI_STRLEN(szText);

	atUIAPI_GetStringMetric(hFont, szText, nLength , &aSize);

	ret = atUIAPI_DrawString( hCanvas, lpRect->nStartX + ((RECT_WIDTH(*lpRect) - aSize.nWidth) / 2), lpRect->nStartY + ((RECT_HEIGHT(*lpRect) - aSize.nHeight) / 2), szText, textColor);

	return ret;
}


/** 
 @brief 
 
 ......

 @param 
 @return 없음
*/
atINT atUIAPI_DrawString( atHGDC hCanvas, atINT nX, atINT nY, atLPTSTR str, atCOLOR color )
{ 
	return atUIAPI_DrawStringEx( hCanvas, nX, nY, str, -1, color, 0 );
}


/** 
 @brief 
 
 ......

 @param 
 @return 없음
*/
atINT atUIAPI_DrawStringEx( atHGDC hCanvas, atINT nX, atINT nY, atLPTSTR str, atINT len, atCOLOR color, atINT nLineGap )
{ 
	atLPGDC lpCanvas = (atLPGDC)hCanvas;
	OEM_UI_FONT	lpFont;

	if( hCanvas == atNULL ) return 0;
	if( !atUIAPI_IsCanvasActivate(hCanvas) ) return 0;

	if(!lpCanvas->hFont) return 0;

	lpFont = atUIAPI_GetOEMPtr(lpCanvas->hFont);
	if( lpFont == atNULL ) return 0;
	
	if( len == -1 ) len = atUIAPI_strlen( str );
	
	if( tOemFn.fnDrawString != atNULL ){
		return tOemFn.fnDrawString( lpCanvas->lpOemCanvas, nX, nY, str, len, color, nLineGap, lpFont );
	}

	return 0;
}


/** 
 @brief 
 
 ......

 @param 
 @return 없음
*/
atINT atUIAPI_DrawStringAlign( atHGDC hCanvas, atINT nX, atINT nY, atLPTSTR str, atINT align, atCOLOR color )
{ 
	atINT     len;
	atINT 	width, height;
	atHFONT	hFont;

	if( hCanvas == atNULL ) return 0;
	if( !atUIAPI_IsCanvasActivate(hCanvas) ) return 0;

	hFont = atUIAPI_GetFont(hCanvas);
	if( hFont == atNULL ) return 0;
	
	len = UI_STRLEN(str);
	width 	= atUIAPI_GetStringWidth(hFont, str, -1);
	height = atUIAPI_GetFontHeight(hFont);	

	switch( align ){
		case ALIGN_LEFT_TOP:
			break;
		case ALIGN_LEFT_MIDDLE:
			nY -= height>>1;
			break;
		case ALIGN_LEFT_BOTTOM:
			nY -= height;
			break;
		case ALIGN_CENTER_TOP:
			nX -= width>>1;
			break;
		case ALIGN_CENTER_MIDDLE:
			nX -= width>>1;
			nY -= height>>1;
			break;
		case ALIGN_CENTER_BOTTOM:
			nX -= width>>1;
			nY -= height;
			break;
		case ALIGN_RIGHT_TOP:
			nX -= width;
			break;
		case ALIGN_RIGHT_MIDDLE:
			nX -= width;
			nY -= height>>1;
			break;
		case ALIGN_RIGHT_BOTTOM:
			nX -= width;
			nY -= height;
			break;
	}

	atUIAPI_DrawString( hCanvas, nX, nY, str, color );
	
	return len;			
}


/** 
 @brief 
 
 ......

 @param 
 @return 없음
*/
atINT atUIAPI_DrawStringAlignRegion( atHGDC hCanvas, atINT nX, atINT nY, atREGION *lpRect, atLPTSTR str, atINT align, atCOLOR color,  atPOINT *lpOutCoord )
{ 
	atINT     len;
	atHFONT	hFont;
	atINT 	width, height;
	atINT     str_width, str_height;
	int xx, yy;

	if( hCanvas == atNULL ) return 0;
	if( !atUIAPI_IsCanvasActivate(hCanvas) ) return 0;

	hFont = atUIAPI_GetFont(hCanvas);
	if( hFont == atNULL ) return 0;
	
	len = UI_STRLEN(str);
	xx = lpRect->nX;
	yy = lpRect->nY;
	width = lpRect->nWidth;
	height = lpRect->nHeight;	
	str_width 	= atUIAPI_GetStringWidth(hFont, str, -1);
	str_height = atUIAPI_GetFontHeight(hFont);	

	switch( align ){
		case ALIGN_LEFT_TOP:
			break;
		case ALIGN_LEFT_MIDDLE:
			yy += (height-str_height)/2;
			break;
		case ALIGN_LEFT_BOTTOM:
			yy += (height-str_height);
			break;
		case ALIGN_CENTER_TOP:
			xx += (width-str_width)/2;
			break;
		case ALIGN_CENTER_MIDDLE:
			xx += (width-str_width)/2;
			yy += (height-str_height)/2;
			break;
		case ALIGN_CENTER_BOTTOM:
			xx += (width-str_width)/2;
			yy += (height-str_height);
			break;
		case ALIGN_RIGHT_TOP:
			xx += (width-str_width);
			break;
		case ALIGN_RIGHT_MIDDLE:
			xx += (width-str_width);
			yy += (height-str_height)/2;
			break;
		case ALIGN_RIGHT_BOTTOM:
			xx += (width-str_width);
			yy += (height-str_height);
			break;
	}

	xx += nX; 
	yy += nY;
	atUIAPI_DrawString(hCanvas, xx, yy, str, color);
	
	if( lpOutCoord ){ lpOutCoord->nX = xx;  lpOutCoord->nY = yy; }
	
	return len;			
}


/** 
 @brief 
 
 ......

 @param 
 @return 없음
*/
atINT atUIAPI_DrawStringAlignRect( atHGDC hCanvas, atINT nX, atINT nY, atRECT *lpRect, atLPTSTR str, atINT align, atCOLOR color,  atPOINT *lpOutCoord )
{ 
	atINT     len;
	atHFONT	hFont;
	atINT 	width, height;
	atINT     str_width, str_height;
	int xx, yy;

	if( hCanvas == atNULL ) return 0;
	if( !atUIAPI_IsCanvasActivate(hCanvas) ) return 0;

	hFont = atUIAPI_GetFont(hCanvas);
	if( hFont == atNULL ) return 0;
	
	len = UI_STRLEN(str);
	xx = lpRect->nStartX;
	yy = lpRect->nStartY;
	width = RECT_WIDTH(*lpRect);
	height = RECT_HEIGHT(*lpRect);	
	str_width 	= atUIAPI_GetStringWidth(hFont, str, -1);
	str_height = atUIAPI_GetFontHeight(hFont);	

	switch( align ){
		case ALIGN_LEFT_TOP:
			break;
		case ALIGN_LEFT_MIDDLE:
			yy += (height-str_height)/2;
			break;
		case ALIGN_LEFT_BOTTOM:
			yy += (height-str_height);
			break;
		case ALIGN_CENTER_TOP:
			xx += (width-str_width)/2;
			break;
		case ALIGN_CENTER_MIDDLE:
			xx += (width-str_width)/2;
			yy += (height-str_height)/2;
			break;
		case ALIGN_CENTER_BOTTOM:
			xx += (width-str_width)/2;
			yy += (height-str_height);
			break;
		case ALIGN_RIGHT_TOP:
			xx += (width-str_width);
			break;
		case ALIGN_RIGHT_MIDDLE:
			xx += (width-str_width);
			yy += (height-str_height)/2;
			break;
		case ALIGN_RIGHT_BOTTOM:
			xx += (width-str_width);
			yy += (height-str_height);
			break;
	}

	xx += nX; 
	yy += nY;
	atUIAPI_DrawString(hCanvas, xx, yy, str, color);
	
	if( lpOutCoord ){ lpOutCoord->nX = xx;  lpOutCoord->nY = yy; }
	
	return len;			
}


/** 
 @brief 
 
 ......

 @param 
 @return 없음
*/
atINT atUIAPI_DrawStringRotate( atHGDC hCanvas, atINT nX, atINT nY, atLPTSTR str, atINT len, atCOLOR color, atREAL rAngle )
{
	atLPGDC lpCanvas = (atLPGDC)hCanvas;
	atHFONT	hFont;
	OEM_UI_FONT	lpFont;

	if( hCanvas == atNULL ) return 0;
	if( !atUIAPI_IsCanvasActivate(hCanvas) ) return 0;
	
	hFont = atUIAPI_GetFont(hCanvas);
	lpFont = atUIAPI_GetOEMPtr(hFont);
	if( lpFont == atNULL ) return 0;
	
	if( tOemFn.fnDrawStringRotate != atNULL ){
		return tOemFn.fnDrawStringRotate( lpCanvas->lpOemCanvas, nX, nY, str, len, color, rAngle, 0, 0, lpFont );
	}

	return 0;
}


/** 
 @brief 
 
 ......

 @param 
 @return 없음
*/
atINT  atUIAPI_DrawStringRotateAlign( atHGDC hCanvas, atINT nX, atINT nY, atLPTSTR str, atINT len, atCOLOR color, atREAL rAngle, atINT nAlign )
{
	atLPGDC lpCanvas = (atLPGDC)hCanvas;
	atHFONT	hFont;
	OEM_UI_FONT	lpFont;
	int width, height;
	int nBaseX, nBaseY;

	if( hCanvas == atNULL ) return 0;
	if( !atUIAPI_IsCanvasActivate(hCanvas) ) return 0;

	hFont = atUIAPI_GetFont(hCanvas);
	lpFont = atUIAPI_GetOEMPtr(hFont);
	if( lpFont == atNULL ) return 0;
	
	width 	= atUIAPI_GetStringWidth(hFont, str, -1);
	height = atUIAPI_GetFontHeight(hFont);	

	nBaseX = nBaseY = 0;		
	
	if( nAlign == ALIGN_LEFT_TOP){
		
	} else if (nAlign == ALIGN_LEFT_MIDDLE){
		nBaseY = (height>>1);
	} else if (nAlign == ALIGN_LEFT_BOTTOM){
		nBaseY = height;
	} else if	(nAlign == ALIGN_CENTER_TOP){
		nBaseX = (width>>1);
	} else if (nAlign == ALIGN_CENTER_MIDDLE){
		nBaseX = (width>>1);
		nBaseY = (height>>1);
    } else if (nAlign == ALIGN_CENTER_BOTTOM){
		nBaseX = (width>>1);
		nBaseY = height;
	} else if	(nAlign == ALIGN_RIGHT_TOP){
		nBaseX = width;
	} else if (nAlign == ALIGN_RIGHT_MIDDLE){
		nBaseX = width;
		nBaseY = (height>>1);
	} else if (nAlign == ALIGN_RIGHT_BOTTOM){
		nBaseX = width;
		nBaseY = height;
	}
	
	if( tOemFn.fnDrawStringRotate != atNULL ){
		return tOemFn.fnDrawStringRotate( lpCanvas->lpOemCanvas, nX, nY, str, len, color, rAngle, nBaseX, nBaseY, lpFont );
	}

	return 0;
}


/** 
 @brief 
 
 ......

 @param 
 @return 없음
*/
atINT atUIAPI_DrawStringRotateEx( atHGDC hCanvas, atINT nX, atINT nY, atLPTSTR str, atINT len, atCOLOR color, atCOLOR colorOut, atREAL rAngle, atINT nAlign, atINT nOL)
{
	atLPGDC lpCanvas = (atLPGDC)hCanvas;
	atHFONT	hFont;
	OEM_UI_FONT	lpFont;
	int width, height;
	int nBaseX, nBaseY;

	if( hCanvas == atNULL ) return 0;
	if( !atUIAPI_IsCanvasActivate(hCanvas) ) return 0;

	hFont = atUIAPI_GetFont(hCanvas);
	if(!hFont) return 0;
	lpFont = atUIAPI_GetOEMPtr(hFont);
	if( lpFont == atNULL ) return 0;
	
	width 	= atUIAPI_GetStringWidth(hFont, str, -1);
	height = atUIAPI_GetFontHeight(hFont);	

	nBaseX = nBaseY = 0;		
	
	if( nAlign == ALIGN_LEFT_TOP){
		
	} else if (nAlign == ALIGN_LEFT_MIDDLE){
		nBaseY = (height>>1);
	} else if (nAlign == ALIGN_LEFT_BOTTOM){
		nBaseY = height;
	} else if	(nAlign == ALIGN_CENTER_TOP){
		nBaseX = (width>>1);
	} else if (nAlign == ALIGN_CENTER_MIDDLE){
		nBaseX = (width>>1);
		nBaseY = (height>>1);
    } else if (nAlign == ALIGN_CENTER_BOTTOM){
		nBaseX = (width>>1);
		nBaseY = height;
	} else if	(nAlign == ALIGN_RIGHT_TOP){
		nBaseX = width;
	} else if (nAlign == ALIGN_RIGHT_MIDDLE){
		nBaseX = width;
		nBaseY = (height>>1);
	} else if (nAlign == ALIGN_RIGHT_BOTTOM){
		nBaseX = width;
		nBaseY = height;
	}

	if( tOemFn.fnDrawStringRotate != atNULL ){
		if (nOL > 0){
			tOemFn.fnDrawStringRotate( lpCanvas->lpOemCanvas, nX-nOL, nY, str, len, colorOut, rAngle, nBaseX, nBaseY, lpFont );
			tOemFn.fnDrawStringRotate( lpCanvas->lpOemCanvas, nX, nY-nOL, str, len, colorOut, rAngle, nBaseX, nBaseY, lpFont );
			tOemFn.fnDrawStringRotate( lpCanvas->lpOemCanvas, nX, nY+nOL, str, len, colorOut, rAngle, nBaseX, nBaseY, lpFont );
			tOemFn.fnDrawStringRotate( lpCanvas->lpOemCanvas, nX+nOL, nY, str, len, colorOut, rAngle, nBaseX, nBaseY, lpFont );
		}
	
		return tOemFn.fnDrawStringRotate( lpCanvas->lpOemCanvas, nX, nY, str, len, color, rAngle, nBaseX, nBaseY, lpFont );
	}

	return 0;
}


/** 
 @brief 
 
 ......

 @param 
 @return 없음
*/
atHSTRBITMAP atUIAPI_CreateStrBitmap( atHFONT hFont, atLPTSTR szStrData, atINT nLen )
{
	OEM_UI_FONT	lpFont;
	
	lpFont = atUIAPI_GetOEMPtr(hFont);
	if( lpFont == atNULL ) return 0;
	
	if( tOemFn.fnCreateStrBitmap != atNULL ){
		return tOemFn.fnCreateStrBitmap( lpFont, szStrData, nLen );
	}

	return (atHSTRBITMAP)atNULL;
}


/** 
 @brief 
 
 ......

 @param 
 @return 없음
*/
atVOID atUIAPI_GetStrBitmapMetric( atHSTRBITMAP hStrBitmap, atSIZE *lpSize )
{
	if( tOemFn.fnGetStrBitmapMetric != atNULL ){
		tOemFn.fnGetStrBitmapMetric( hStrBitmap, lpSize );
	}
}


/** 
 @brief 
 
 ......

 @param 
 @return 없음
*/
atVOID atUIAPI_DestroyStrBitmap( atHSTRBITMAP hStrBitmap )
{
	if( tOemFn.fnDestroyStrBitmap != atNULL ){
		tOemFn.fnDestroyStrBitmap( hStrBitmap );
	}
}


/** 
 @brief 
 
 ......

 @param 
 @return 없음
*/
atVOID atUIAPI_DrawStrBitmap( atHGDC hCanvas, atINT X, atINT Y, atHSTRBITMAP hStrBitmap, atINT iStartPos, atINT iWidth, atCOLOR Color )
{
	atLPGDC lpCanvas = (atLPGDC)hCanvas;

	if( lpCanvas == atNULL ) return;
	if( !atUIAPI_IsCanvasActivate(hCanvas) ) return;

	if( tOemFn.fnDrawStrBitmap != atNULL ){
		tOemFn.fnDrawStrBitmap( lpCanvas->lpOemCanvas, X, Y, hStrBitmap, iStartPos, iWidth, Color );
	}
}


/** 
 @brief 
 
 ......

 @param 
 @return 없음
*/
atVOID atUIAPI_DrawStrBitmapAlign( atHGDC hCanvas, atINT nX, atINT nY, atHSTRBITMAP hStrBitmap, atINT iStartPos, atINT iWidth, atINT align, atCOLOR Color )
{ 
	atINT 	width, height;
	atSIZE size;

	if( hCanvas == atNULL ) return;
	if( !atUIAPI_IsCanvasActivate(hCanvas) ) return;

	atUIAPI_GetStrBitmapMetric( hStrBitmap, &size );

	width  = MIN( iWidth, size.nWidth );
	height = size.nHeight;	

	if( align == ALIGN_LEFT_TOP){
		
	} else if (align == ALIGN_LEFT_MIDDLE){
		nY -= (height>>1);
	} else if (align == ALIGN_LEFT_BOTTOM){
		nY -= height;
	} else if	(align == ALIGN_CENTER_TOP){
		nX -= (width>>1);
	} else if (align == ALIGN_CENTER_MIDDLE){
		nX -= (width>>1);
		nY -= (height>>1);
    } else if (align == ALIGN_CENTER_BOTTOM){
		nX -= (width>>1);
		nY -= height;
	} else if	(align == ALIGN_RIGHT_TOP){
		nX -= width;
	} else if (align == ALIGN_RIGHT_MIDDLE){
		nX -= width;
		nY -= (height>>1);
	} else if (align == ALIGN_RIGHT_BOTTOM){
		nX -= width;
		nY -= height;
	}

	atUIAPI_DrawStrBitmap( hCanvas, nX, nY, hStrBitmap, iStartPos, iWidth, Color );
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


/** 
 @brief 
 
 ......

 @param 
 @return 없음
*/
atVOID	atUIAPI_FillRect( atHGDC hCanvas, atINT nX, atINT nY, atINT nWidth, atINT nHeight, atCOLOR nColor )
{
	atLPGDC lpCanvas = (atLPGDC)hCanvas;
	if( hCanvas == atNULL ) return;
	if( !atUIAPI_IsCanvasActivate(hCanvas) ) return;
	
	if( tOemFn.fnFillRect != atNULL ){
		tOemFn.fnFillRect( lpCanvas->lpOemCanvas, nX, nY, nWidth, nHeight, nColor, 0xFF); 
	}
}


/** 
 @brief 
 
 ......

 @param 
 @return 없음
*/
atVOID	atUIAPI_FillRectTrans( atHGDC hCanvas, atINT nX, atINT nY, atINT nWidth, atINT nHeight, atCOLOR nColor, atINT nAlpha )
{
	atLPGDC lpCanvas = (atLPGDC)hCanvas;
	if( hCanvas == atNULL ) return;
	if( !atUIAPI_IsCanvasActivate(hCanvas) ) return;
	
	if( tOemFn.fnFillRect != atNULL ){
		tOemFn.fnFillRect( lpCanvas->lpOemCanvas, nX, nY, nWidth, nHeight, nColor, nAlpha); 
	}
}


/** 
 @brief 
 
 ......

 @param 
 @return 없음
*/
atVOID   atUIAPI_FillRectXOR( atHGDC hCanvas, atINT nX, atINT nY, atINT nWidth, atINT nHeight )
{
	atLPGDC lpCanvas = (atLPGDC)hCanvas;
	if( hCanvas == atNULL ) return;
	if( !atUIAPI_IsCanvasActivate(hCanvas) ) return;
	
	if( tOemFn.fnFillRectXOR != atNULL ){
		tOemFn.fnFillRectXOR( lpCanvas->lpOemCanvas, nX, nY, nWidth, nHeight); 
	}
}


/** 
 @brief 
 
 ......

 @param 
 @return 없음
*/
atVOID	atUIAPI_DrawRect( atHGDC hCanvas, atINT nX, atINT nY, atINT nWidth, atINT nHeight, atCOLOR nColor, atINT nLineWidth )
{	
	atLPGDC lpCanvas = (atLPGDC)hCanvas;
	if( lpCanvas == atNULL ) return;
	if( !atUIAPI_IsCanvasActivate(hCanvas) ) return;
	
	if( tOemFn.fnDrawRect != atNULL ){
		tOemFn.fnDrawRect(lpCanvas->lpOemCanvas, nX, nY, nWidth, nHeight, nColor, nLineWidth); 
	}
}


/** 
 @brief 
 
 ......

 @param 
 @return 없음
*/
atVOID	atUIAPI_Clear( atHGDC hCanvas, atCOLOR nColor )
{
    atREGION rgn;

	if( !atUIAPI_IsCanvasActivate(hCanvas) ) return;
	
	atUIAPI_GetGDCArea(hCanvas, &rgn);
	atUIAPI_FillRect( hCanvas, 0, 0, rgn.nWidth, rgn.nHeight, nColor ); 
}


/** 
 @brief 
 
 ......

 @param 
 @return 없음
*/
atVOID	atUIAPI_DrawLine( atHGDC hCanvas, atINT nX1, atINT nY1, atINT nX2, atINT nY2, atCOLOR nColor, atINT nLineWidth, atBOOL bIsAA )
{	
	atLPGDC lpCanvas = (atLPGDC)hCanvas;
	if( lpCanvas == atNULL ) return;
	if( !atUIAPI_IsCanvasActivate(hCanvas) ) return;
	
	if( tOemFn.fnDrawLine != atNULL ){
		tOemFn.fnDrawLine(lpCanvas->lpOemCanvas, nX1, nY1, nX2, nY2, nColor, nLineWidth, bIsAA);
	}
}


/** 
 @brief 
 
 ......

 @param 
 @return 없음
*/
atVOID	atUIAPI_DrawCircle( atHGDC hCanvas, atINT nX, atINT nY, atINT nRadius, atCOLOR nColor, atINT nLineWidth )
{	
	atLPGDC lpCanvas = (atLPGDC)hCanvas;
	
	if( hCanvas == atNULL ) return;
	if( !atUIAPI_IsCanvasActivate(hCanvas) ) return;

	if( tOemFn.fnDrawCircle != atNULL ){
		tOemFn.fnDrawCircle(lpCanvas->lpOemCanvas, nX, nY, nRadius, nColor, nLineWidth );
	}
}


/** 
 @brief 
 
 ......

 @param 
 @return 없음
*/
atVOID	atUIAPI_FillCircle( atHGDC hCanvas, atINT nX, atINT nY, atINT nRadius, atCOLOR nColor )
{	
	atLPGDC lpCanvas = (atLPGDC)hCanvas;
	
	if( hCanvas == atNULL ) return;
	if( !atUIAPI_IsCanvasActivate(hCanvas) ) return;

	if( tOemFn.fnFillCircle != atNULL ){
		tOemFn.fnFillCircle(lpCanvas->lpOemCanvas, nX, nY, nRadius, nColor, 0xFF);
	}
}


/** 
 @brief 
 
 ......

 @param 
 @return 없음
*/
atVOID	atUIAPI_FillCircleTrans( atHGDC hCanvas, atINT nX, atINT nY, atINT nRadius, atCOLOR nColor, atINT nAlpha )
{	
	atLPGDC lpCanvas = (atLPGDC)hCanvas;
	
	if( hCanvas == atNULL ) return;
	if( !atUIAPI_IsCanvasActivate(hCanvas) ) return;

	if( tOemFn.fnFillCircle != atNULL ){
		tOemFn.fnFillCircle(lpCanvas->lpOemCanvas, nX, nY, nRadius, nColor, nAlpha);
	}
}


/** 
 @brief 
 
 ......

 @param 
 @return 없음
*/
atVOID	atUIAPI_DrawEllipse( atHGDC hCanvas, atINT nX, atINT nY, atINT nRadiusX, atINT nRadiusY, atCOLOR nColor, atINT nLineWidth )
{
	atLPGDC lpCanvas = (atLPGDC)hCanvas;
	
	if( hCanvas == atNULL ) return;
	if( !atUIAPI_IsCanvasActivate(hCanvas) ) return;

	if( tOemFn.fnDrawEllipse != atNULL ){
		tOemFn.fnDrawEllipse(lpCanvas->lpOemCanvas, nX, nY, nRadiusX, nRadiusY, nColor, nLineWidth);
	}
}


/** 
 @brief 
 
 ......

 @param 
 @return 없음
*/
atVOID	atUIAPI_FillEllipse( atHGDC hCanvas, atINT nX, atINT nY, atINT nRadiusX, atINT nRadiusY, atCOLOR nColor )
{
	atLPGDC lpCanvas = (atLPGDC)hCanvas;
	
	if( hCanvas == atNULL ) return;
	if( !atUIAPI_IsCanvasActivate(hCanvas) ) return;

	if( tOemFn.fnFillEllipse != atNULL ){
		tOemFn.fnFillEllipse(lpCanvas->lpOemCanvas, nX, nY, nRadiusX, nRadiusY, nColor, 0xFF);
	}
}


/** 
 @brief 
 
 ......

 @param 
 @return 없음
*/
atVOID	atUIAPI_FillEllipseTrans( atHGDC hCanvas, atINT nX, atINT nY, atINT nRadiusX, atINT nRadiusY, atCOLOR nColor, atINT nAlpha )
{
	atLPGDC lpCanvas = (atLPGDC)hCanvas;
	
	if( hCanvas == atNULL ) return;
	if( !atUIAPI_IsCanvasActivate(hCanvas) ) return;

	if( tOemFn.fnFillEllipse != atNULL ){
		tOemFn.fnFillEllipse(lpCanvas->lpOemCanvas, nX, nY, nRadiusX, nRadiusY, nColor, nAlpha);
	}
}


/** 
 @brief 
 
 ......

 @param 
 @return 없음
*/
atVOID    atUIAPI_DrawPolyLine( atHGDC hCanvas, atPOINT lpPtrList[], atINT nCount, atBOOL bClosed, atCOLOR Color, atINT nWidth, atBOOL IsAA )
{
	atLPGDC lpCanvas = (atLPGDC)hCanvas;
	
	if( hCanvas == atNULL ) return;
	if( !atUIAPI_IsCanvasActivate(hCanvas) ) return;

	if( tOemFn.fnDrawPolyLine != atNULL ){
		tOemFn.fnDrawPolyLine( lpCanvas->lpOemCanvas, lpPtrList, nCount, bClosed, Color, nWidth, IsAA );
	}
}


/** 
 @brief 
 
 ......

 @param 
 @return 없음
*/
atVOID    atUIAPI_DrawPolyLineEx( atHGDC hCanvas, atPOINT lpPtrList[], atINT nCount, atBOOL bClosed, atCOLOR Color, atINT nWidth, atBOOL IsAA, atUI_FUNC_COORD_CONV_REAL *lpFunc, atLPVOID lpParam )
{
	atLPGDC lpCanvas = (atLPGDC)hCanvas;
	
	if( hCanvas == atNULL ) return;
	if( !atUIAPI_IsCanvasActivate(hCanvas) ) return;

	if( tOemFn.fnDrawPolyLineEx != atNULL ){
		tOemFn.fnDrawPolyLineEx( lpCanvas->lpOemCanvas, lpPtrList, nCount, bClosed, Color, nWidth, IsAA, lpFunc, lpParam );
	}
}


/** 
 @brief 
 
 ......

 @param 
 @return 없음
*/
atVOID    atUIAPI_FillPolygon( atHGDC hCanvas, atPOINT lpPtrList[], atINT nCount, atCOLOR Color, atBOOL IsAA )
{
	atLPGDC lpCanvas = (atLPGDC)hCanvas;
	
	if( hCanvas == atNULL ) return;
	if( !atUIAPI_IsCanvasActivate(hCanvas) ) return;

	if( tOemFn.fnFillPolygon != atNULL ){
		tOemFn.fnFillPolygon( lpCanvas->lpOemCanvas, lpPtrList, nCount, Color, IsAA, 0xFF );
	}
}


/** 
 @brief 
 
 ......

 @param 
 @return 없음
*/
atVOID    atUIAPI_FillPolygonTrans( atHGDC hCanvas, atPOINT lpPtrList[], atINT nCount, atCOLOR Color, atBOOL IsAA, atINT nAlpha )
{
	atLPGDC lpCanvas = (atLPGDC)hCanvas;
	
	if( hCanvas == atNULL ) return;
	if( !atUIAPI_IsCanvasActivate(hCanvas) ) return;

	if( tOemFn.fnFillPolygon != atNULL ){
		tOemFn.fnFillPolygon( lpCanvas->lpOemCanvas, lpPtrList, nCount, Color, IsAA, nAlpha );
	}
}


/** 
 @brief 
 
 ......

 @param 
 @return 없음
*/
atVOID	atUIAPI_FillPolygonBitmap( atHGDC hCanvas, atPOINT lpPtrList[], atINT nCount, atHBITMAP hBitmap )
{
	atLPGDC lpCanvas = (atLPGDC)hCanvas;
//	atLPBITMAP lpBitmap = (atLPBITMAP)hBitmap;
	
	if( hBitmap == atNULL ) return;	
	if( hCanvas == atNULL ) return;
	if( !atUIAPI_IsCanvasActivate(hCanvas) ) return;

	if( tOemFn.fnFillPolygonBitmap != atNULL ){
		tOemFn.fnFillPolygonBitmap( lpCanvas->lpOemCanvas, lpPtrList, nCount, (OEM_UI_IMAGE)hBitmap );
	}
}


/** 
 @brief 
 
 ......

 @param 
 @return 없음
*/
atVOID    atUIAPI_DrawPolyPolyLine( atHGDC hCanvas, atPOINT lpPtrList[], atINT nPartCount[], atINT nPolyCount, atBOOL bClosed, atCOLOR Color, atINT nWidth, atBOOL IsAA )
{
    	int i;
	atLPGDC lpCanvas = (atLPGDC)hCanvas;
	
	if( hCanvas == atNULL ) return;
	if( !atUIAPI_IsCanvasActivate(hCanvas) ) return;

	if( tOemFn.fnDrawPolyLine != atNULL ){
		for( i = 0; i < nPolyCount; i++ ){
      		tOemFn.fnDrawPolyLine( lpCanvas->lpOemCanvas, lpPtrList, nPartCount[i], bClosed, Color, nWidth, IsAA );
      		lpPtrList += nPartCount[i];
		}
	}
}


/** 
 @brief 
 
 ......

 @param 
 @return 없음
*/
atVOID    atUIAPI_DrawPolyPolyLineEx( atHGDC hCanvas, atPOINT lpPtrList[], atINT nPartCount[], atINT nPolyCount, atBOOL bClosed, atCOLOR Color, atINT nWidth, atBOOL IsAA, atUI_FUNC_COORD_CONV_REAL *lpFunc, atLPVOID lpParam )
{
    	int i;
	atLPGDC lpCanvas = (atLPGDC)hCanvas;
	
	if( hCanvas == atNULL ) return;
	if( !atUIAPI_IsCanvasActivate(hCanvas) ) return;

	if( tOemFn.fnDrawPolyLineEx != atNULL ){
		for( i = 0; i < nPolyCount; i++ ){
      		tOemFn.fnDrawPolyLineEx( lpCanvas->lpOemCanvas, lpPtrList, nPartCount[i], bClosed, Color, nWidth, IsAA, lpFunc, lpParam );
      		lpPtrList += nPartCount[i];
		}
	}
}


/** 
 @brief 
 
 ......

 @param 
 @return 없음
*/
atVOID    atUIAPI_FillPolyPolygon( atHGDC hCanvas, atPOINT lpPtrList[], atINT nPartCount[], atINT nPolyCount, atCOLOR Color, atBOOL IsAA )
{
	atLPGDC lpCanvas = (atLPGDC)hCanvas;
	
	if( hCanvas == atNULL ) return;
	if( !atUIAPI_IsCanvasActivate(hCanvas) ) return;

	if( tOemFn.fnFillPolyPolygon != atNULL ){
		tOemFn.fnFillPolyPolygon( lpCanvas->lpOemCanvas, lpPtrList, nPartCount, nPolyCount, Color, IsAA, 0xFF );
	}
}


/** 
 @brief 
 
 ......

 @param 
 @return 없음
*/
atVOID    atUIAPI_FillPolyPolygonEx( atHGDC hCanvas, atPOINT lpPtrList[], atINT nPartCount[], atINT nPolyCount, atCOLOR Color, atBOOL IsAA, atUI_FUNC_COORD_CONV *lpFunc, atLPVOID lpParam )
{
	atLPGDC lpCanvas = (atLPGDC)hCanvas;
	
	if( hCanvas == atNULL ) return;
	if( !atUIAPI_IsCanvasActivate(hCanvas) ) return;

	if( tOemFn.fnFillPolyPolygonEx != atNULL ){
		tOemFn.fnFillPolyPolygonEx( lpCanvas->lpOemCanvas, lpPtrList, nPartCount, nPolyCount, Color, IsAA, 0xFF, lpFunc, lpParam );
	}
}


/** 
 @brief 
 
 ......

 @param 
 @return 없음
*/
atVOID    atUIAPI_FillPolyPolygonTrans( atHGDC hCanvas, atPOINT lpPtrList[], atINT nPartCount[], atINT nPolyCount, atCOLOR Color, atBOOL IsAA, atINT nAlpha )
{
	atLPGDC lpCanvas = (atLPGDC)hCanvas;
	
	if( hCanvas == atNULL ) return;
	if( !atUIAPI_IsCanvasActivate(hCanvas) ) return;

	if( tOemFn.fnFillPolyPolygon != atNULL ){
		tOemFn.fnFillPolyPolygon( lpCanvas->lpOemCanvas, lpPtrList, nPartCount, nPolyCount, Color, IsAA, nAlpha );
	}
}


/** 
 @brief 
 
 ......

 @param 
 @return 없음
*/
atVOID	atUIAPI_FillPolyPolygonBitmap( atHGDC hCanvas, atPOINT lpPtrList[], atINT nPartCount[], atINT nPolyCount, atHBITMAP hBitmap )
{
	atLPGDC lpCanvas = (atLPGDC)hCanvas;
//	atLPBITMAP lpBitmap = (atLPBITMAP)hBitmap;
	
	if( hBitmap == atNULL ) return;	
	if( hCanvas == atNULL ) return;
	if( !atUIAPI_IsCanvasActivate(hCanvas) ) return;

	if( tOemFn.fnFillPolyPolygonBitmap != atNULL ){
		tOemFn.fnFillPolyPolygonBitmap( lpCanvas->lpOemCanvas, lpPtrList, nPartCount, nPolyCount, (OEM_UI_IMAGE)hBitmap );
	}
}


/** 
 @brief 
 
 ......

 @param 
 @return 없음
*/
atVOID	atUIAPI_FillPolyPolygonBitmapEx( atHGDC hCanvas, atPOINT lpPtrList[], atINT nPartCount[], atINT nPolyCount, atHBITMAP hBitmap, atUI_FUNC_COORD_CONV *lpFunc, atLPVOID lpParam )
{
	atLPGDC lpCanvas = (atLPGDC)hCanvas;
//	atLPBITMAP lpBitmap = (atLPBITMAP)hBitmap;
	
	if( hBitmap == atNULL ) return;	
	if( hCanvas == atNULL ) return;
	if( !atUIAPI_IsCanvasActivate(hCanvas) ) return;

	if( tOemFn.fnFillPolyPolygonBitmapEx != atNULL ){
		tOemFn.fnFillPolyPolygonBitmapEx( lpCanvas->lpOemCanvas, lpPtrList, nPartCount, nPolyCount, (OEM_UI_IMAGE)hBitmap, lpFunc, lpParam );
	}
}


/** 
 @brief 
 
 ......

 @param 
 @return 없음
*/
atVOID	atUIAPI_DrawImageAlign(atHGDC hCanvas, atINT nX, atINT nY, atHBITMAP hBitmap, atINT nAlign)
{	
	atINT 	width, height;
	
	if (hBitmap == atNULL) return;
	if( hCanvas == atNULL ) return;
	if( !atUIAPI_IsCanvasActivate(hCanvas) ) return;

	width = atUIAPI_GetBitmapWidth(hBitmap);
	height = atUIAPI_GetBitmapHeight(hBitmap);

	if		(nAlign == ALIGN_LEFT_TOP)
		atUIAPI_DrawImage(hCanvas, nX, nY, hBitmap);
	else if	(nAlign == ALIGN_LEFT_MIDDLE)
		atUIAPI_DrawImage(hCanvas, nX, nY-(height>>1), hBitmap);
	else if	(nAlign == ALIGN_LEFT_BOTTOM)
		atUIAPI_DrawImage(hCanvas, nX, nY-height, hBitmap);
	else if (nAlign == ALIGN_CENTER_TOP)
		atUIAPI_DrawImage(hCanvas, nX-(width>>1), nY, hBitmap);
	else if (nAlign == ALIGN_CENTER_MIDDLE)
		atUIAPI_DrawImage(hCanvas, nX-(width>>1), nY-(height>>1), hBitmap);
	else if (nAlign == ALIGN_CENTER_BOTTOM)
		atUIAPI_DrawImage(hCanvas, nX-(width>>1), nY-height, hBitmap);
	else if (nAlign == ALIGN_RIGHT_TOP)
		atUIAPI_DrawImage(hCanvas, nX-width, nY, hBitmap);
	else if (nAlign == ALIGN_RIGHT_MIDDLE)
		atUIAPI_DrawImage(hCanvas, nX-width, nY-(height>>1), hBitmap);
	else if (nAlign == ALIGN_RIGHT_BOTTOM)
		atUIAPI_DrawImage(hCanvas, nX-width, nY-height, hBitmap);
}


/** 
 @brief 
 
 ......

 @param 
 @return 없음
*/
atVOID	atUIAPI_DrawImageAlignPart(atHGDC hCanvas, atINT nX, atINT nY, atHBITMAP hBitmap, atINT nImgX, atINT nImgY, atINT nImgWidth, atINT nImgHeight, atINT nAlign)
{	
	atINT 	width, height;
	
	if (hBitmap == atNULL) return;
	if( hCanvas == atNULL ) return;
	if( !atUIAPI_IsCanvasActivate(hCanvas) ) return;

	width = atUIAPI_GetBitmapWidth(hBitmap);
	width = MIN( nImgWidth, width );
	height = atUIAPI_GetBitmapHeight(hBitmap);
	height = MIN( nImgHeight, height );

	if		(nAlign == ALIGN_LEFT_TOP)
		atUIAPI_DrawImagePart(hCanvas, nX, nY, hBitmap, nImgX, nImgY, width, height);
	else if	(nAlign == ALIGN_LEFT_MIDDLE)
		atUIAPI_DrawImagePart(hCanvas, nX, nY-(height>>1), hBitmap, nImgX, nImgY, width, height);
	else if	(nAlign == ALIGN_LEFT_BOTTOM)
		atUIAPI_DrawImagePart(hCanvas, nX, nY-height, hBitmap, nImgX, nImgY, width, height);
	else if (nAlign == ALIGN_CENTER_TOP)
		atUIAPI_DrawImagePart(hCanvas, nX-(width>>1), nY, hBitmap, nImgX, nImgY, width, height);
	else if (nAlign == ALIGN_CENTER_MIDDLE)
		atUIAPI_DrawImagePart(hCanvas, nX-(width>>1), nY-(height>>1), hBitmap, nImgX, nImgY, width, height);
	else if (nAlign == ALIGN_CENTER_BOTTOM)
		atUIAPI_DrawImagePart(hCanvas, nX-(width>>1), nY-height, hBitmap, nImgX, nImgY, width, height);
	else if (nAlign == ALIGN_RIGHT_TOP)
		atUIAPI_DrawImagePart(hCanvas, nX-width, nY, hBitmap, nImgX, nImgY, width, height);
	else if (nAlign == ALIGN_RIGHT_MIDDLE)
		atUIAPI_DrawImagePart(hCanvas, nX-width, nY-(height>>1), hBitmap, nImgX, nImgY, width, height);
	else if (nAlign == ALIGN_RIGHT_BOTTOM)
		atUIAPI_DrawImagePart(hCanvas, nX-width, nY-height, hBitmap, nImgX, nImgY, width, height);
}


/** 
 @brief 
 
 ......

 @param 
 @return 없음
*/
atLPVOID	   atUIAPI_GetBitmapBits( atHBITMAP hBitmap )
{
	if( tOemFn.fnGetBitmapBits != atNULL ){
		return tOemFn.fnGetBitmapBits( (OEM_UI_IMAGE)hBitmap );
	}

	return atNULL;
}


/** 
 @brief 
 
 ......

 @param 
 @return 없음
*/
atINT         atUIAPI_GetBitmapBPP( atHBITMAP hBitmap )
{
	if( tOemFn.fnGetBitmapBPP != atNULL ){
		return tOemFn.fnGetBitmapBPP( (OEM_UI_IMAGE)hBitmap );
	}

	return 0;
}


/** 
 @brief 
 
 ......

 @param 
 @return 없음
*/
atINT atUIAPI_GetBitmapWidth(atHBITMAP hBitmap)
{
	if( tOemFn.fnGetBitmapWidth != atNULL ){
		return tOemFn.fnGetBitmapWidth( (OEM_UI_IMAGE)hBitmap );
	}

	return 0;
}


/** 
 @brief 
 
 ......

 @param 
 @return 없음
*/
atINT atUIAPI_GetBitmapHeight(atHBITMAP hBitmap)
{
	if( tOemFn.fnGetBitmapHeight != atNULL ){
		return tOemFn.fnGetBitmapHeight( (OEM_UI_IMAGE)hBitmap );
	}

	return 0;
}


/** 
 @brief 
 
 ......

 @param 
 @return 없음
*/
atINT atUIAPI_GetBitmapWidthBytes(atHBITMAP hBitmap)
{
	if( tOemFn.fnGetBitmapWidthBytes != atNULL ){
		return tOemFn.fnGetBitmapWidthBytes( (OEM_UI_IMAGE)hBitmap );
	}

	return 0;
}


/** 
 @brief 
 
 ......

 @param 
 @return 없음
*/
atLPVOID   atUIAPI_GetOEMBitmap( atHBITMAP hBitmap )
{
	return (OEM_UI_IMAGE)hBitmap;
}


/** 
 @brief 
 
 ......

 @param 
 @return 없음
*/
atVOID atUIAPI_GetBitmapLogPos( atHBITMAP hBitmap, atINT *pLogX, atINT *pLogY )
{
	if( tOemFn.fnGetBitmapLogPos != atNULL ){
		tOemFn.fnGetBitmapLogPos( (OEM_UI_IMAGE)hBitmap, pLogX, pLogY );
	}
}


/** 
 @brief 
 
 ......

 @param 
 @return 없음
*/
atVOID atUIAPI_SetBitmapLogPos( atHBITMAP hBitmap, atINT nLogX, atINT nLogY )
{
	if( tOemFn.fnSetBitmapLogPos != atNULL ){
		tOemFn.fnSetBitmapLogPos( (OEM_UI_IMAGE)hBitmap, nLogX, nLogY );
	}
}


/** 
 @brief 
 
 ......

 @param 
 @return 없음
*/
atVOID atUIAPI_SetBitmapLogPosCenter( atHBITMAP lpBM )
{
	atUIAPI_SetBitmapLogPos( lpBM, -atUIAPI_GetBitmapWidth(lpBM) >> 1, -atUIAPI_GetBitmapHeight(lpBM) >> 1 );
}


/** 
 @brief 
 
 ......

 @param 
 @return 없음
*/
atHBITMAP atUIAPI_CreateBitmapFromStreamEx( atLPBYTE lpSrcData, atINT size, atLPCALLBACKALLOC fnAllocator, atLPCALLBACKFREE fnDeallocator )
{
	OEM_UI_IMAGE oem_bm;
	
	if( tOemFn.fnCreateBitmapFromStream != atNULL ){
		oem_bm = tOemFn.fnCreateBitmapFromStream( lpSrcData, size, fnAllocator, fnDeallocator );
	} else {
		oem_bm = atNULL;
	}

	return (atHBITMAP)oem_bm;
}


/** 
 @brief 
 
 ......

 @param 
 @return 없음
*/
atHBITMAP atUIAPI_CreateBitmapFromStream( atLPBYTE lpSrcData, atINT size )
{
	return atUIAPI_CreateBitmapFromStreamEx( lpSrcData, size, atNULL, atNULL );
}


/** 
 @brief 
 
 ......

 @param 
 @return 없음
*/
atHBITMAP atUIAPI_CreateBitmapFromRes( atHRESSRC hResSrc, atLPTSTR FileName )
{
	atHRES hRes;
	atLPVOID res;
	atLONG len;
	atHBITMAP hBitmap;
	atLPCALLBACKALLOC fnAllocator;
	atLPCALLBACKFREE fnDeallocator;
	
	hRes = atUIAPI_LoadRes( hResSrc, FileName );
	if( hRes == atNULL ) return( atNULL );

	atUIAPI_GetResData( hRes, &res, &len );
	atUIAPI_GetResAllocator( hResSrc, &fnAllocator, &fnDeallocator );
	hBitmap = atUIAPI_CreateBitmapFromStreamEx( (atBYTE*)res, len, fnAllocator, fnDeallocator );

	atUIAPI_FreeRes(hRes);

	return hBitmap;
}


/** 
 @brief 
 
 ......

 @param 
 @return 없음
*/
atHBITMAP atUIAPI_CreateBitmapFromFileEx( atLPTSTR szFileName, atLPCALLBACKALLOC fnAllocator, atLPCALLBACKFREE fnDeallocator )
{
	atLPVOID res;
	atLONG filesize, len;
	atHBITMAP hBitmap;
	
	filesize = atUIAPI_GetFileSize( szFileName );
	if( filesize <= 0 ) return atNULL;
	res = fnAllocator(filesize);
	len = atUIAPI_LoadFile( szFileName, res, filesize, 0 );
	if( len != filesize ) return( atNULL );

	hBitmap = atUIAPI_CreateBitmapFromStreamEx((atBYTE*)res, len, fnAllocator, fnDeallocator );

	fnDeallocator(res);

	return hBitmap;
}


/** 
 @brief 
 
 ......

 @param 
 @return 없음
*/
atHBITMAP atUIAPI_CreateBitmapFromFile( atLPTSTR szFileName )
{
	return atUIAPI_CreateBitmapFromFileEx( szFileName, atNULL, atNULL );
}


/** 
 @brief 
 
 ......

 @param 
 @return 없음
*/
atHBITMAP atUIAPI_CreateBlankBitmapEx( atINT nWidth, atINT nHeight, atINT nBPP, atLPCALLBACKALLOC fnAllocator, atLPCALLBACKFREE fnDeallocator )
{
	OEM_UI_IMAGE oem_bm;
	
	if( tOemFn.fnCreateBlankBitmap != atNULL ){
		oem_bm = tOemFn.fnCreateBlankBitmap( nWidth, nHeight, nBPP, fnAllocator, fnDeallocator );
	} else {
		oem_bm = atNULL;
	}

	return (atHBITMAP)oem_bm;
}


/** 
 @brief 
 
 ......

 @param 
 @return 없음
*/
atHBITMAP atUIAPI_CreateBlankBitmap( atINT nWidth, atINT nHeight, atINT nBPP )
{	
	return atUIAPI_CreateBlankBitmapEx( nWidth, nHeight, nBPP, atNULL, atNULL );
}


#define MAKE565(r,g,b) ((atWORD)(((((atWORD)((r) & 0xF8)) << (5+6-3))) | \
					   ((((atWORD)((g) & 0xFC)) << (5-2))) | \
					   (((atWORD)((b) & 0xF8)) >> 3)))


#if USES_YUV422_TO_RGB565_TABLE != 1
static atUINT yuv2rgb16(atINT y, atINT u, atINT v)
{
   atINT r, g, b;

   /*
     R = Y + 1.370705 (V-128)
     G = Y - 0.698001 (V-128) - 0.337633 (U-128)
     B = Y + 1.732446 (U-128)
   */

   u -= 128;
   v -= 128;

   r = (atINT)( y + (1.370705 * (v)));
   g = (atINT)(y - (0.698001 * (v)) - (0.337633 * (u)));
   b = (atINT)(y + (1.732446 * (u)));

   // Even with proper conversion, some values still need clipping.
   if (r > 255) r = 255;
   if (g > 255) g = 255;
   if (b > 255) b = 255;
   if (r < 0) r = 0;
   if (g < 0) g = 0;
   if (b < 0) b = 0;

   return (atUINT)(MAKE565(r,g,b));
}
#endif


/** 
 @brief 
 
 ......

 @param 
 @return 없음
*/
atHBITMAP	atUIAPI_CreateBitmapFromYUV422Stream(atBYTE* yuv, atINT width, atINT height, atINT mem_width)
{

	atHBITMAP hBitmap;
	atLPVOID  lpBits;

	atUINT pixel_16;
	
	atINT i, j, skip_t;
	atINT y, u, v, y2;
#if USES_YUV422_TO_RGB565_TABLE == 1
	int yy;
	int r, g, b;
#endif

	atUINT * ptr = atNULL;
	atUSHORT* bits_ptr = atNULL;
	
	
	hBitmap = atUIAPI_CreateBlankBitmap(width, height, 16);
	if(!hBitmap) return atNULL;

	lpBits	= atUIAPI_GetBitmapBits(hBitmap);
	
	ptr =  (unsigned int*)yuv;

	bits_ptr= (unsigned short*)lpBits;

	skip_t = (mem_width - width) / 2;
	
	for(i=0; i < height; i++)
	{
		for(j=0; j < width/2; j++)
		{
			pixel_16 = *ptr++;

			y  = ((pixel_16 & 0x000000ff));
			u  = ((pixel_16 & 0x0000ff00)>>8);
			y2  = ((pixel_16 & 0x00ff0000)>>16);
			v = ((pixel_16 & 0xff000000)>>24);

#if USES_YUV422_TO_RGB565_TABLE == 1
			yy = _rgb_y_tbl[y];
	        	r = (yy + _r_v_tbl[v]) >> 16;
		    	if( r < 0 ) r = 0; if( r > 255 ) r = 255;
	        	g = (yy - _g_u_tbl[u] - _g_v_tbl[v]) >> 16;
	      		if( g < 0 ) g = 0; if( g > 255 ) g = 255;
	       	b = (yy + _b_u_tbl[u]) >> 16;
	      		if( b < 0 ) b = 0; if( b > 255 ) b = 255;
			*bits_ptr++ = MAKE565(r, g, b);			
			yy = _rgb_y_tbl[y2];
	        	r = (yy + _r_v_tbl[v]) >> 16;
		    	if( r < 0 ) r = 0; if( r > 255 ) r = 255;
	        	g = (yy - _g_u_tbl[u] - _g_v_tbl[v]) >> 16;
	      		if( g < 0 ) g = 0; if( g > 255 ) g = 255;
	       	b = (yy + _b_u_tbl[u]) >> 16;
	      		if( b < 0 ) b = 0; if( b > 255 ) b = 255;
			*bits_ptr++ = MAKE565(r, g, b);			
#else
			*bits_ptr = yuv2rgb16(y, u, v);
			bits_ptr++;

			*bits_ptr = yuv2rgb16(y2, u, v);
			bits_ptr++;
#endif
		}

		ptr += skip_t;
	}
	return hBitmap;
}


/** 
 @brief 
 
 ......

 @param 
 @return 없음
*/
atVOID atUIAPI_DestroyBitmap(atHBITMAP lpBM)
{
	if( tOemFn.fnDestroyBitmap != atNULL ){
		tOemFn.fnDestroyBitmap( (OEM_UI_IMAGE)lpBM );
	}
}


/** 
 @brief 
 
 ......

 @param 
 @return 없음
*/
atVOID	atUIAPI_SetBitmapTransColor( atHBITMAP lpBM, atCOLOR clTransColor )
{
	if( tOemFn.fnSetBitmapTransColor != atNULL ){
		tOemFn.fnSetBitmapTransColor( (OEM_UI_IMAGE)lpBM, clTransColor );
	}
}


/** 
 @brief 
 
 ......

 @param 
 @return 없음
*/
atVOID atUIAPI_ClearBitmapTransColor( atHBITMAP lpBM )
{
	if( tOemFn.fnClearBitmapTransColor != atNULL ){
		tOemFn.fnClearBitmapTransColor( (OEM_UI_IMAGE)lpBM );
	}
}


/** 
 @brief 
 
 ......

 @param 
 @return 없음
*/
atVOID	atUIAPI_GetImage( atHGDC hCanvas, atINT nX, atINT nY, atHBITMAP hBitmap )
{	
	atLPGDC lpCanvas = (atLPGDC)hCanvas;
//	atLPBITMAP lpBitmap = (atLPBITMAP)lpBM;
	
	if( hBitmap == atNULL ) return;	
	if( lpCanvas == atNULL ) return;
	if( !atUIAPI_IsCanvasActivate(hCanvas) ) return;
	
	if( tOemFn.fnGetBitmap != atNULL ){
		tOemFn.fnGetBitmap( lpCanvas->lpOemCanvas, (OEM_UI_IMAGE)hBitmap, nX, nY );
	}
}


/** 
 @brief 
 
 ......

 @param 
 @return 없음
*/
atVOID	atUIAPI_DrawImage( atHGDC hCanvas, atINT nX, atINT nY, atHBITMAP hBitmap )
{	
	atUIAPI_DrawImagePart( hCanvas, nX, nY, hBitmap, 0, 0, atUIAPI_GetBitmapWidth(hBitmap), atUIAPI_GetBitmapHeight(hBitmap) );
}


/** 
 @brief 
 
 ......

 @param 
 @return 없음
*/
atVOID	atUIAPI_DrawImagePart( atHGDC hCanvas, atINT nX, atINT nY, atHBITMAP hBitmap, atINT nImgX, atINT nImgY, atINT nWidth, atINT nHeight )
{	
	atLPGDC lpCanvas = (atLPGDC)hCanvas;
	
	if( hBitmap == atNULL ) return;	
	if( lpCanvas == atNULL ) return;
	if( !atUIAPI_IsCanvasActivate(hCanvas) ) return;

	if( tOemFn.fnDrawBitmap != atNULL ){
		tOemFn.fnDrawBitmap( lpCanvas->lpOemCanvas, (OEM_UI_IMAGE)hBitmap, nX, nY, nWidth, nHeight, nImgX, nImgY );
	}
}


/** 
 @brief 
 
 ......

 @param 
 @return 없음
*/
atVOID	atUIAPI_DrawImageStretch( atHGDC hCanvas, atINT nX, atINT nY, atHBITMAP hBitmap, atINT nWidth, atINT nHeight )
{	
	atLPGDC lpCanvas = (atLPGDC)hCanvas;
	
	if( hBitmap == atNULL ) return;	
	if( lpCanvas == atNULL ) return;
	if( !atUIAPI_IsCanvasActivate(hCanvas) ) return;
	
	if( tOemFn.fnDrawBitmapStretch != atNULL ){
		tOemFn.fnDrawBitmapStretch( lpCanvas->lpOemCanvas, (OEM_UI_IMAGE)hBitmap, nX, nY, nWidth, nHeight );
	}
}


/** 
 @brief 
 
 ......

 @param 
 @return 없음
*/
atVOID        atUIAPI_DrawImageRotate(atHGDC hCanvas, atINT nX, atINT nY, atHBITMAP hBitmap, atREAL rAngle )
{
	atLPGDC lpCanvas = (atLPGDC)hCanvas;
	
	if( hBitmap == atNULL ) return;	
	if( lpCanvas == atNULL ) return;
	if( !atUIAPI_IsCanvasActivate(hCanvas) ) return;
	
	if( tOemFn.fnDrawBitmapRotate != atNULL ){
		tOemFn.fnDrawBitmapRotate( lpCanvas->lpOemCanvas, (OEM_UI_IMAGE)hBitmap, nX, nY, rAngle );
	}
}


/** 
 @brief 
 
 ......

 @param 
 @return 없음
*/
atLONG    atUIAPI_ExtGraphicMethod( atHGDC hCanvas, atINT func_no, atLONG param1, atLONG param2, atLONG param3, atLONG param4, atLONG param5, atLONG param6 )
{
	atLPGDC lpCanvas = (atLPGDC)hCanvas;

	if( tOemFn.fnExtGraphicMethod != atNULL ){
		return tOemFn.fnExtGraphicMethod( atUIAPI_IsCanvasActivate(hCanvas), (lpCanvas)?lpCanvas->lpOemCanvas:atNULL, func_no, param1, param2, param3, param4, param5, param6 );
	}

	return 0;
}

