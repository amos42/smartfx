 //
// Canvas_wipi.c
//
////////////////////////////////////////////////////////////////

#include "OEM_UI_GDI.h"
#include "OEM_UI_Mem.h"
#include "GrpX.h"


#define MAKE565(r,g,b) ((atWORD)(((((atWORD)((r) & 0xF8)) << (5+6-3))) | \
					   ((((atWORD)((g) & 0xFC)) << (5-2))) | \
					   (((atWORD)((b) & 0xF8)) >> 3)))

//atLPGRPXMNG g_lpGrpX; // Font¿¡¼­µµ ¾¸


static atULONG OEM_GetRGB(atBYTE r, atBYTE g, atBYTE b)
{
	return atGRPX_GetRGB(r,g,b);
}

static atBYTE OEM_GetRValue(atULONG nColor)
{
	return atGRPX_GetRValue(nColor);
}

static atBYTE OEM_GetGValue(atULONG nColor)
{
	return atGRPX_GetGValue(nColor);
}

static atBYTE OEM_GetBValue(atULONG nColor)
{
	return atGRPX_GetBValue(nColor);
}


static OEM_UI_GDC OEM_CreateCanvas( atINT nX, atINT nY, atINT nWidth, atINT nHeight, atINT bpp, atDWORD dwAttr )
{
	atGRPX_CANVAS *cnv;
	if( dwAttr & atUI_CANVAS_ATTR_OWNERBUFFER )
		cnv = atGRPX_CreateCanvas( g_lpGrpX, nX, nY, nWidth, nHeight, atGRPX_CANVAS_ATTR_OWNERBUFFER, atNULL, 0 );
	else
		cnv = atGRPX_CreateCanvas( g_lpGrpX, nX, nY, nWidth, nHeight, atGRPX_CANVAS_ATTR_NORMAL, atNULL, 0 );

	return (OEM_UI_GDC)cnv;
}


static atVOID OEM_DestroyCanvas( OEM_UI_GDC gdc )
{
//	OEM_CANVAS_T *cnv_t = (OEM_CANVAS_T *)gdc;
	if( gdc == atNULL ) return;
	
	atGRPX_DestroyCanvas( (atGRPX_CANVAS *)gdc );
	
//	atUIAPI_FreeMem( cnv_t );
}


static atVOID OEM_GetCanvasAttr( OEM_UI_GDC gdc, atUI_CANVAS_ATTR *lpCanvasAttr )
{
	atGRPX_CANVAS *cnv;
		
	if( gdc == atNULL ) return;

	cnv = (atGRPX_CANVAS *)gdc ;

	lpCanvasAttr->dwAttr = 0;
	atUIAPI_memcpy( &lpCanvasAttr->rgnArea, atGRPX_GetCanvasArea(cnv), sizeof(atREGION) );
	atUIAPI_memcpy( &lpCanvasAttr->rtClipRect, atGRPX_GetOriginalClipRect(cnv), sizeof(atRECT) );
	lpCanvasAttr->nBPP = cnv->nBPP;
	lpCanvasAttr->lpVideoPtr = cnv->lpVideoPtr;
	lpCanvasAttr->nVMemWidth = cnv->nVMemWidth;	
}


static atVOID OEM_GetCanvasArea(OEM_UI_GDC gdc, atREGION *rgnArea)
{
//	OEM_CANVAS_T *cnv_t = (OEM_CANVAS_T *)gdc;

	atUIAPI_memcpy( rgnArea, atGRPX_GetCanvasArea( (atGRPX_CANVAS *)gdc ), sizeof(atREGION) );
}

static atVOID OEM_SetCanvasArea(OEM_UI_GDC gdc, atREGION *rgnArea)
{
//	OEM_CANVAS_T *cnv_t = (OEM_CANVAS_T *)gdc;
//	atRECT *rect;

//	*atGRPX_GetCanvasArea((atGRPX_CANVAS *)gdc) = *rgnArea;
/*
	rect = atGRPX_GetClipRect((atGRPX_CANVAS *)gdc);
	rect->nStartX = 0;//rgnArea->nX;
	rect->nStartY = 0;//rgnArea->nY;
	rect->nEndX = rect->nStartX + rgnArea->nWidth - 1;
	rect->nEndY = rect->nStartY + rgnArea->nHeight - 1;
*/
	atGRPX_ChangeCanvasSize( (atGRPX_CANVAS *)gdc, rgnArea->nWidth, rgnArea->nHeight, 0 );
	atGRPX_SetClipRect( (atGRPX_CANVAS *)gdc, rgnArea->nX, rgnArea->nY, rgnArea->nX+rgnArea->nWidth-1, rgnArea->nY+rgnArea->nHeight-1 );
	atGRPX_MoveCanvas( (atGRPX_CANVAS *)gdc, rgnArea->nX, rgnArea->nY );
}

static atVOID OEM_GetCanvasClipRect(OEM_UI_GDC gdc, atRECT *rtClip)
{
	atUIAPI_memcpy( rtClip, atGRPX_GetClipRect( (atGRPX_CANVAS *)gdc ), sizeof(atRECT) );
}

static atVOID OEM_SetCanvasClipRect(OEM_UI_GDC gdc, atRECT *rtClip)
{
	atGRPX_SetClipRect( (atGRPX_CANVAS *)gdc, rtClip->nStartX, rtClip->nStartY, rtClip->nEndX, rtClip->nEndY );
}

/*
static atVOID	OEM_FlushCanvasRect( OEM_UI_GDC gdc, atINT nX, atINT nY, atINT nWidth, atINT nHeight )
{  	
	atREGION rgn;

	OEM_GetCanvasArea( gdc, &rgn );

	OEM_UI_FlushScreenRgn( rgn.nX, rgn.nY, rgn.nWidth, rgn.nHeight );
}
*/

static atVOID  OEM_CopyCanvas( OEM_UI_GDC hDesCanvas, atINT nDesX, atINT nDesY, OEM_UI_GDC hSrcCanvas, atINT nX, atINT nY, atINT nWidth, atINT nHeight )
{
	atGRPX_CopyCanvas( (atGRPX_CANVAS *)hDesCanvas, nDesX, nDesY, (atGRPX_CANVAS *)hSrcCanvas, nX, nY, nWidth, nHeight );
}

/*
static OEM_UI_FONT OEM_SetFont( OEM_UI_GDC gdc, OEM_UI_FONT hFont )
{
	return (OEM_UI_FONT)atGRPX_SetCurFont( (atGRPX_CANVAS *)gdc, (atGRPX_FONT*)hFont );
}

static OEM_UI_FONT OEM_GetFont( OEM_UI_GDC gdc  )
{
	return 	atGRPX_GetCurFont( (atGRPX_CANVAS *)gdc );
}
*/

static atVOID OEM_SetFrColor( OEM_UI_GDC gdc, atCOLOR color )
{

	atGRPX_SetFrColor( (atGRPX_CANVAS *)gdc, color );
}	 

static atCOLOR OEM_GetFrColor( OEM_UI_GDC gdc )
{
	return atGRPX_GetFrColor( (atGRPX_CANVAS *)gdc );
}	 

static atVOID OEM_SetBgColor( OEM_UI_GDC gdc, atCOLOR color )
{
	atGRPX_SetBgColor( (atGRPX_CANVAS *)gdc, color );
}	 

static atCOLOR OEM_GetBgColor( OEM_UI_GDC gdc )
{
	return atGRPX_GetBgColor( (atGRPX_CANVAS *)gdc );
}	 

static atCOLOR OEM_GetPixel( OEM_UI_GDC gdc, atINT x, atINT y )
{
	return atGRPX_GetPixel( (atGRPX_CANVAS *)gdc, x, y );
}

static atVOID OEM_DrawPixel( OEM_UI_GDC gdc, atINT x, atINT y, atCOLOR color )
{
	atGRPX_DrawPixel( (atGRPX_CANVAS *)gdc, x, y, color );
}

static atINT OEM_DrawString( OEM_UI_GDC gdc, atINT x, atINT y, atLPTSTR str, atINT len, atCOLOR color, atINT nLineGap, OEM_UI_FONT hOemFont )
{   
	atGRPX_DrawStringEx( (atGRPX_CANVAS *)gdc, (atGRPX_FONT *)hOemFont, x, y, str, len, color, nLineGap );
	return len;
}

static atINT OEM_DrawStringRotate( OEM_UI_GDC gdc, atINT x, atINT y, atLPTSTR str, atINT len, atCOLOR color, atREAL rAngle, atINT nBaseX, atINT nBase, OEM_UI_FONT hOemFont )
{
	atGRPX_DrawStringRotate( (atGRPX_CANVAS *)gdc, (atGRPX_FONT *)hOemFont, x, y, str, len, color, rAngle, nBaseX, nBase );
	return 0;
}

static atVOID	OEM_FillRect( OEM_UI_GDC gdc, atINT nX, atINT nY, atINT nW, atINT nH, atCOLOR nColor, atINT nAlpha)
{
	atGRPX_FillRectTrans( (atGRPX_CANVAS *)gdc, nX, nY, nX+nW-1, nY+nH-1, nColor, nAlpha );
}

static atVOID  OEM_FillRectXOR( OEM_UI_GDC gdc, atINT nX, atINT nY, atINT nW, atINT nH )
{
	atGRPX_FillRectXOR( (atGRPX_CANVAS *)gdc, nX, nY, nX+nW-1, nY+nH-1 );
}

static atVOID	OEM_DrawRect( OEM_UI_GDC gdc, atINT nX, atINT nY, atINT nW, atINT nH, atCOLOR nColor, atINT nLineWidth)
{	
	atGRPX_DrawRectWidth( (atGRPX_CANVAS *)gdc, nX, nY, nX+nW-1, nY+nH-1, nColor, nLineWidth );
}

static atVOID	OEM_DrawLine( OEM_UI_GDC gdc, atINT nX1, atINT nY1, atINT nX2, atINT nY2, atCOLOR nColor, atINT nLineWidth, atBOOL IsAA)
{	
    if( IsAA )
	    atGRPX_DrawLineWidthAA( (atGRPX_CANVAS *)gdc, nX1, nY1, nX2, nY2, nColor, nLineWidth );
    else
	    atGRPX_DrawLineWidth( (atGRPX_CANVAS *)gdc, nX1, nY1, nX2, nY2, nColor, nLineWidth );
}

static atVOID	OEM_DrawCircle( OEM_UI_GDC gdc, atINT nX, atINT nY, atINT nRadius, atCOLOR nColor, atINT nLineWidth)
{	
//	atGRPX_DrawCircleWidth( (atGRPX_CANVAS *)gdc, nX, nY, nRadius, nColor, nLineWidth );
	atGRPX_DrawCircle( (atGRPX_CANVAS *)gdc, nX, nY, nRadius, nColor );
}

static atVOID	OEM_FillCircle( OEM_UI_GDC gdc, atINT nX, atINT nY, atINT nRadius, atCOLOR nColor, atINT nAlpha)
{	
	atGRPX_FillCircleTrans( (atGRPX_CANVAS *)gdc, nX, nY, nRadius, nColor, nAlpha );
}

static atVOID	OEM_DrawEllipse( OEM_UI_GDC gdc, atINT nX, atINT nY, atINT nRadiusX, atINT nRadiusY, atCOLOR nColor, atINT nLineWidth)
{
	atGRPX_DrawEllipse( (atGRPX_CANVAS *)gdc, nX, nY, nRadiusX, nRadiusY, nColor );
}

static atVOID	OEM_FillEllipse( OEM_UI_GDC gdc, atINT nX, atINT nY, atINT nRadiusX, atINT nRadiusY, atCOLOR nColor, atINT nAlpha)
{
	atGRPX_FillEllipse( (atGRPX_CANVAS *)gdc, nX, nY, nRadiusX, nRadiusY, nColor );
}

static atVOID		OEM_DrawPolyLine( OEM_UI_GDC gdc, atPOINT lpPtrList[], atINT nCount, atBOOL bClosed, atCOLOR Color, atINT nWidth, atBOOL IsAA )
{
     if( IsAA )
         atGRPX_DrawPolyLineWidthAA( (atGRPX_CANVAS *)gdc, lpPtrList, nCount, bClosed, Color, nWidth );
     else
         atGRPX_DrawPolyLineWidth( (atGRPX_CANVAS *)gdc, lpPtrList, nCount, bClosed, Color, nWidth );
}

static atVOID		OEM_DrawPolyLineEx( OEM_UI_GDC gdc, atPOINT lpPtrList[], atINT nCount, atBOOL bClosed, atCOLOR Color, atINT nWidth, atBOOL IsAA, atUI_FUNC_COORD_CONV_REAL *lpFunc, atLPVOID lpParam )
{
     if( IsAA )
         atGRPX_DrawPolyLineWidthExAA( (atGRPX_CANVAS *)gdc, lpPtrList, nCount, bClosed, Color, nWidth, (atGRPX_FUNC_COORD_CONV_REAL *)lpFunc, lpParam );
     else
         atGRPX_DrawPolyLineWidthEx( (atGRPX_CANVAS *)gdc, lpPtrList, nCount, bClosed, Color, nWidth, (atGRPX_FUNC_COORD_CONV_REAL *)lpFunc, lpParam );
}

static atVOID		OEM_FillPolygon( OEM_UI_GDC gdc, atPOINT lpPtrList[], atINT nCount, atCOLOR Color, atBOOL IsAA, atINT nAlpha )
{
    if( IsAA )
        atGRPX_FillPolygonAA( (atGRPX_CANVAS *)gdc, lpPtrList, nCount, Color );
    else
        atGRPX_FillPolygon( (atGRPX_CANVAS *)gdc, lpPtrList, nCount, Color );
}

static atVOID		OEM_FillPolygonBitmap( OEM_UI_GDC gdc, atPOINT lpPtrList[], atINT nCount, OEM_UI_IMAGE hBitmap )
{
    atGRPX_FillPolygonBitmap( (atGRPX_CANVAS *)gdc, lpPtrList, nCount, (atGRPX_BITMAP *)hBitmap );
}

static atVOID		OEM_FillPolyPolygon( OEM_UI_GDC gdc, atPOINT lpPtrList[], atINT nPartCount[], atINT nPolyCount, atCOLOR Color, atBOOL IsAA, atINT nAlpha )
{
    if( IsAA )
        atGRPX_FillPolyPolygonAA( (atGRPX_CANVAS *)gdc, lpPtrList, nPartCount, nPolyCount, Color );
    else
        atGRPX_FillPolyPolygon( (atGRPX_CANVAS *)gdc, lpPtrList, nPartCount, nPolyCount, Color );
}

static atVOID		OEM_FillPolyPolygonEx( OEM_UI_GDC gdc, atPOINT lpPtrList[], atINT nPartCount[], atINT nPolyCount, atCOLOR Color, atBOOL IsAA, atINT nAlpha, atUI_FUNC_COORD_CONV *lpFunc, atLPVOID lpParam )
{
    if( IsAA )
        atGRPX_FillPolyPolygonExAA( (atGRPX_CANVAS *)gdc, lpPtrList, nPartCount, nPolyCount, Color, (atGRPX_FUNC_COORD_CONV *)lpFunc, lpParam );
    else
        atGRPX_FillPolyPolygonEx( (atGRPX_CANVAS *)gdc, lpPtrList, nPartCount, nPolyCount, Color, (atGRPX_FUNC_COORD_CONV *)lpFunc, lpParam );
}

static atVOID		OEM_FillPolyPolygonBitmap( OEM_UI_GDC gdc, atPOINT lpPtrList[], atINT nPartCount[], atINT nPolyCount, OEM_UI_IMAGE hBitmap )
{
    atGRPX_FillPolyPolygonBitmap( (atGRPX_CANVAS *)gdc, lpPtrList, nPartCount, nPolyCount, (atGRPX_BITMAP *)hBitmap );
}

static atVOID		OEM_FillPolyPolygonBitmapEx( OEM_UI_GDC gdc, atPOINT lpPtrList[], atINT nPartCount[], atINT nPolyCount, OEM_UI_IMAGE hBitmap, atUI_FUNC_COORD_CONV *lpFunc, atLPVOID lpParam )
{
    atGRPX_FillPolyPolygonBitmapEx( (atGRPX_CANVAS *)gdc, lpPtrList, nPartCount, nPolyCount, (atGRPX_BITMAP *)hBitmap, (atGRPX_FUNC_COORD_CONV *)lpFunc, lpParam );
}


static OEM_UI_STRBITMAP OEM_CreateStrBitmap( OEM_UI_FONT hOemFont, atLPTSTR szStrData, atINT nLen )
{
	return (OEM_UI_STRBITMAP)atGRPX_CreateStrBitmap( g_lpGrpX, (atGRPX_FONT*)hOemFont, szStrData, nLen );
}

static atVOID OEM_DestroyStrBitmap( OEM_UI_STRBITMAP hStrBitmap )
{
//	atGRPX_DestroyStrBitmap( (atGRPX_STRBITMAP *)hStrBitmap );
	atGRPX_DestroyBitmap( (atGRPX_BITMAP *)hStrBitmap );
}

static atVOID OEM_GetStrBitmapMetric( OEM_UI_STRBITMAP hStrBitmap, atSIZE *lpSize )
{
//	atGRPX_GetStrBitmapMetric( (atGRPX_STRBITMAP *)hStrBitmap, lpSize );
	atGRPX_GetBitmapMetric( (atGRPX_BITMAP *)hStrBitmap, lpSize );
}

static atVOID OEM_DrawStrBitmap( OEM_UI_GDC hCanvas, atINT X, atINT Y, OEM_UI_STRBITMAP hStrBitmap, atINT iStartPos, atINT iWidth, atCOLOR Color )
{
//    atGRPX_DrawStrBitmap( (atGRPX_CANVAS *)hCanvas, X, Y, (atGRPX_STRBITMAP *)hStrBitmap, iStartPos, iWidth, Color );
    atGRPX_DrawGlyphPart( (atGRPX_CANVAS *)hCanvas, (atGRPX_BITMAP *)hStrBitmap, X, Y, Color, iStartPos, iWidth );
}



static OEM_UI_IMAGE 	OEM_CreateBlankBitmap( atINT nWidth, atINT nHeight, atINT nBPP, atLPCALLBACKALLOC fnAllocator, atLPCALLBACKFREE fnDeallocator )
{
	atGRPX_BITMAP *img = atGRPX_CreateBlankBitmapEx( g_lpGrpX, nWidth, nHeight, nBPP, 0, fnAllocator, fnDeallocator );
	if( img == atNULL ) return atNULL;

	return (OEM_UI_IMAGE)img;
}

static atVOID 		OEM_DestroyBitmap( OEM_UI_IMAGE lpBM )
{
    atGRPX_DestroyBitmap( (atGRPX_BITMAP *)lpBM);
}

static atLPVOID		OEM_GetBitmapBits( OEM_UI_IMAGE lpBM )
{
    atGRPX_BITMAP *lpBitmap = (atGRPX_BITMAP *)lpBM;
	if( lpBitmap == atNULL ) return 0;
	
    return lpBitmap->lpBits;
}

static atINT		OEM_GetBitmapBPP( OEM_UI_IMAGE lpBM )
{
    atGRPX_BITMAP *lpBitmap = (atGRPX_BITMAP *)lpBM;
	if( lpBitmap == atNULL ) return 0;
	
    return lpBitmap->nBPP;
}

static atINT			OEM_GetBitmapWidth( OEM_UI_IMAGE lpBM )
{
    atGRPX_BITMAP *lpBitmap = (atGRPX_BITMAP *)lpBM;
	if( lpBitmap == atNULL ) return 0;
	
    return lpBitmap->nWidth;
}

static atINT			OEM_GetBitmapWidthBytes( OEM_UI_IMAGE lpBM )
{
    atGRPX_BITMAP *lpBitmap = (atGRPX_BITMAP *)lpBM;
	if( lpBitmap == atNULL ) return 0;
	
    return lpBitmap->nWidthBytes;
}

static atINT			OEM_GetBitmapHeight( OEM_UI_IMAGE lpBM )
{
    atGRPX_BITMAP *lpBitmap = (atGRPX_BITMAP *)lpBM;
	if( lpBitmap == atNULL ) return 0;
	
    return lpBitmap->nHeight;
}

static atVOID OEM_GetBitmapLogPos( OEM_UI_IMAGE lpBM, atINT *pLogX, atINT *pLogY )
{
	atGRPX_GetBitmapLogPos( (atGRPX_BITMAP *)lpBM, pLogX, pLogY );
}

static atVOID OEM_SetBitmapLogPos( OEM_UI_IMAGE lpBM, atINT nLogX, atINT nLogY )
{
	atGRPX_SetBitmapLogPos( (atGRPX_BITMAP *)lpBM, nLogX, nLogY );
}

static OEM_UI_IMAGE 	OEM_CreateBitmapFromStream( atBYTE* lpBMPSrc, atINT dwSize, atLPCALLBACKALLOC fnAllocator, atLPCALLBACKFREE fnDeallocator )
{
	atGRPX_BITMAP *img = atGRPX_CreateBitmapFromStreamEx( g_lpGrpX, (atCHAR *)lpBMPSrc, 16, 0, fnAllocator, fnDeallocator );

	return( (OEM_UI_IMAGE)img );
}

/*
static OEM_BITMAPMASK OEM_CreateBitmapMask( atVOID *lpSrcData, atLONG size )
{
	return (OEM_BITMAPMASK)atGRPX_CreateMaskFromStream( g_lpGrpX, (const char *)lpSrcData );
}

static atVOID OEM_DestroyBitmapMask( OEM_BITMAPMASK lpBM)
{
	atUIAPI_FreeMem( (atVOID *)lpBM );
}
*/

static atVOID 		OEM_SetBitmapTransColor( OEM_UI_IMAGE lpBM, atCOLOR clTransColor )
{
	atGRPX_SetBitmapTransColor( (atGRPX_BITMAP *)lpBM, clTransColor );
}

static atVOID 		OEM_ClearBitmapTransColor( OEM_UI_IMAGE lpBM )
{
	atGRPX_ClearBitmapTransColor( (atGRPX_BITMAP *)lpBM );
}

/*
static atVOID 		OEM_SetBitmapMask( OEM_UI_IMAGE lpBM, OEM_BITMAPMASK lpMask, atBOOL bOwnMask )
{
	atGRPX_SetBitmapAlphaChannel( (atGRPX_BITMAP *)lpBM, (atLPBYTE)lpMask, bOwnMask );
}
*/

static atVOID 		OEM_GetBitmap( OEM_UI_GDC gdc, OEM_UI_IMAGE bmpDDImage, atINT nPosX, atINT nPosY )
{
    atGRPX_GetBitmap( (atGRPX_CANVAS *)gdc, (atGRPX_BITMAP *)bmpDDImage, nPosX, nPosY );
}

static atVOID 		OEM_DrawBitmap( OEM_UI_GDC gdc, OEM_UI_IMAGE bmpDDImage, atINT nPosX, atINT nPosY, atINT nWidth, atINT nHeight, 
			  					   atINT nSrcPosX, atINT nSrcPosY )
{
//    if( ((atGRPX_BITMAP *)bmpDDImage)->lpAlphaChannel )
//    	atGRPX_DrawBitmapPartWithAlphaChannel( (atGRPX_CANVAS *)gdc, (atGRPX_BITMAP *)bmpDDImage, nPosX, nPosY, nSrcPosX, nSrcPosY, nWidth, nHeight, ((atGRPX_BITMAP *)bmpDDImage)->lpAlphaChannel );
//    else
	atGRPX_DrawBitmapPart( (atGRPX_CANVAS *)gdc, (atGRPX_BITMAP *)bmpDDImage, nPosX, nPosY, nSrcPosX, nSrcPosY, nWidth, nHeight );
}

static void 		OEM_DrawBitmapStretch( OEM_UI_GDC gdc, OEM_UI_IMAGE bmpDDImage, int nPosX, int nPosY, int nWidth, int nHeight )
{
    atGRPX_DrawBitmapStretch( (atGRPX_CANVAS *)gdc, (atGRPX_BITMAP *)bmpDDImage, nPosX, nPosY, nWidth, nHeight );
}

static void 		OEM_DrawBitmapRotate( OEM_UI_GDC gdc, OEM_UI_IMAGE bmpDDImage, int nPosX, int nPosY, atREAL rAngle )
{
    atGRPX_DrawBitmapRotate( (atGRPX_CANVAS *)gdc, (atGRPX_BITMAP *)bmpDDImage, nPosX, nPosY, rAngle );
}

/*
static atVOID 		OEM_DrawBitmapWithAlphaChannel( OEM_UI_GDC gdc, OEM_UI_IMAGE bmpDDImage, atINT nPosX, atINT nPosY, atINT nWidth, atINT nHeight, 
			  					   atINT nSrcPosX, atINT nSrcPosY, OEM_BITMAPMASK bmpMask )
{
    atGRPX_DrawBitmapPartWithAlphaChannel( (atGRPX_CANVAS *)gdc, (atGRPX_BITMAP *)bmpDDImage, nPosX, nPosY, nSrcPosX, nSrcPosY, nWidth, nHeight, (atBYTE *)bmpMask );
}

static atVOID 		OEM_DrawBitmapStretchWithAlphaChannel( OEM_UI_GDC gdc, OEM_UI_IMAGE bmpDDImage, atINT nPosX, atINT nPosY, atINT nWidth, atINT nHeight, OEM_BITMAPMASK bmpMask )
{
    atGRPX_DrawBitmapStretchWithAlphaChannel( (atGRPX_CANVAS *)gdc, (atGRPX_BITMAP *)bmpDDImage, nPosX, nPosY, nWidth, nHeight, (atBYTE *)bmpMask );
}

static void 		OEM_DrawBitmapRotateWithAlphaChannel( OEM_UI_GDC gdc, OEM_UI_IMAGE bmpDDImage, int nPosX, int nPosY, atREAL rAngle, OEM_BITMAPMASK bmpMask )
{
    atGRPX_DrawBitmapRotateWithAlphaChannel( (atGRPX_CANVAS *)gdc, (atGRPX_BITMAP *)bmpDDImage, nPosX, nPosY, rAngle, (atBYTE *)bmpMask );
}

static atVOID 		OEM_DrawBitmapTrans( OEM_UI_GDC gdc, OEM_UI_IMAGE bmpDDImage, atINT nPosX, atINT nPosY, atINT nAlpha )
{
    atGRPX_DrawBitmapTrans( (atGRPX_CANVAS *)gdc, (atGRPX_BITMAP *)bmpDDImage, nPosX, nPosY, nAlpha );
}
*/
