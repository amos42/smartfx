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


atULONG OEM_GetRGB(atBYTE r, atBYTE g, atBYTE b)
{
	return atGRPX_GetRGB(r,g,b);
}

atBYTE OEM_GetRValue(atULONG nColor)
{
	return atGRPX_GetRValue(nColor);
}

atBYTE OEM_GetGValue(atULONG nColor)
{
	return atGRPX_GetGValue(nColor);
}

atBYTE OEM_GetBValue(atULONG nColor)
{
	return atGRPX_GetBValue(nColor);
}


OEM_GDC OEM_CreateCanvas( atINT nX, atINT nY, atINT nWidth, atINT nHeight, atINT bpp, atDWORD dwAttr )
{
	atGRPX_CANVAS *cnv;
	if( dwAttr & atUI_CANVAS_ATTR_OWNERBUFFER )
		cnv = atGRPX_CreateCanvas( g_lpGrpX, nX, nY, nWidth, nHeight, atGRPX_CANVAS_ATTR_OWNERBUFFER, atNULL, 0 );
	else
		cnv = atGRPX_CreateCanvas( g_lpGrpX, nX, nY, nWidth, nHeight, atGRPX_CANVAS_ATTR_NORMAL, atNULL, 0 );

	return (OEM_GDC)cnv;
}


atVOID OEM_DestroyCanvas( OEM_GDC gdc )
{
//	OEM_CANVAS_T *cnv_t = (OEM_CANVAS_T *)gdc;
	if( gdc == atNULL ) return;
	
	atGRPX_DestroyCanvas( (atGRPX_CANVAS *)gdc );
	
//	atUIAPI_FreeMem( cnv_t );
}


atVOID OEM_GetCanvasAttr( OEM_GDC gdc, atUI_CANVAS_ATTR *lpCanvasAttr )
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


atVOID OEM_GetCanvasArea(OEM_GDC gdc, atREGION *rgnArea)
{
//	OEM_CANVAS_T *cnv_t = (OEM_CANVAS_T *)gdc;

	atUIAPI_memcpy( rgnArea, atGRPX_GetCanvasArea( (atGRPX_CANVAS *)gdc ), sizeof(atREGION) );
}

atVOID OEM_SetCanvasArea(OEM_GDC gdc, atREGION *rgnArea)
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

atVOID OEM_GetCanvasClipRect(OEM_GDC gdc, atRECT *rtClip)
{
	atUIAPI_memcpy( rtClip, atGRPX_GetClipRect( (atGRPX_CANVAS *)gdc ), sizeof(atRECT) );
}

atVOID OEM_SetCanvasClipRect(OEM_GDC gdc, atRECT *rtClip)
{
	atGRPX_SetClipRect( (atGRPX_CANVAS *)gdc, rtClip->nStartX, rtClip->nStartY, rtClip->nEndX, rtClip->nEndY );
}

/*
atVOID	OEM_FlushCanvasRect( OEM_GDC gdc, atINT nX, atINT nY, atINT nWidth, atINT nHeight )
{  	
	atREGION rgn;

	OEM_GetCanvasArea( gdc, &rgn );

	OEM_UI_FlushScreenRgn( rgn.nX, rgn.nY, rgn.nWidth, rgn.nHeight );
}
*/

atVOID  OEM_CopyCanvas( OEM_GDC hDesCanvas, atINT nDesX, atINT nDesY, OEM_GDC hSrcCanvas, atINT nX, atINT nY, atINT nWidth, atINT nHeight )
{
	atGRPX_CopyCanvas( (atGRPX_CANVAS *)hDesCanvas, nDesX, nDesY, (atGRPX_CANVAS *)hSrcCanvas, nX, nY, nWidth, nHeight );
}

/*
OEM_FONT OEM_SetFont( OEM_GDC gdc, OEM_FONT hFont )
{
	return (OEM_FONT)atGRPX_SetCurFont( (atGRPX_CANVAS *)gdc, (atGRPX_FONT*)hFont );
}

OEM_FONT OEM_GetFont( OEM_GDC gdc  )
{
	return 	atGRPX_GetCurFont( (atGRPX_CANVAS *)gdc );
}
*/

atVOID OEM_SetFrColor( OEM_GDC gdc, atCOLOR color )
{

	atGRPX_SetFrColor( (atGRPX_CANVAS *)gdc, color );
}	 

atCOLOR OEM_GetFrColor( OEM_GDC gdc )
{
	return atGRPX_GetFrColor( (atGRPX_CANVAS *)gdc );
}	 

atVOID OEM_SetBgColor( OEM_GDC gdc, atCOLOR color )
{
	atGRPX_SetBgColor( (atGRPX_CANVAS *)gdc, color );
}	 

atCOLOR OEM_GetBgColor( OEM_GDC gdc )
{
	return atGRPX_GetBgColor( (atGRPX_CANVAS *)gdc );
}	 

atCOLOR OEM_GetPixel( OEM_GDC gdc, atINT x, atINT y )
{
	return atGRPX_GetPixel( (atGRPX_CANVAS *)gdc, x, y );
}

atVOID OEM_DrawPixel( OEM_GDC gdc, atINT x, atINT y, atCOLOR color )
{
	atGRPX_DrawPixel( (atGRPX_CANVAS *)gdc, x, y, color );
}

atINT OEM_DrawString( OEM_GDC gdc, atINT x, atINT y, atLPTSTR str, atINT len, atCOLOR color, atINT nLineGap, OEM_FONT hOemFont )
{   
	atGRPX_DrawStringEx( (atGRPX_CANVAS *)gdc, (atGRPX_FONT *)hOemFont, x, y, str, len, color, nLineGap );
	return len;
}

atINT OEM_DrawStringRotate( OEM_GDC gdc, atINT x, atINT y, atLPTSTR str, atINT len, atCOLOR color, atREAL rAngle, atINT nBaseX, atINT nBase, OEM_FONT hOemFont )
{
	atGRPX_DrawStringRotate( (atGRPX_CANVAS *)gdc, (atGRPX_FONT *)hOemFont, x, y, str, len, color, rAngle, nBaseX, nBase );
	return 0;
}

atVOID	OEM_FillRect( OEM_GDC gdc, atINT nX, atINT nY, atINT nW, atINT nH, atCOLOR nColor, atINT nAlpha)
{
	atGRPX_FillRectTrans( (atGRPX_CANVAS *)gdc, nX, nY, nX+nW-1, nY+nH-1, nColor, nAlpha );
}

atVOID  OEM_FillRectXOR( OEM_GDC gdc, atINT nX, atINT nY, atINT nW, atINT nH )
{
	atGRPX_FillRectXOR( (atGRPX_CANVAS *)gdc, nX, nY, nX+nW-1, nY+nH-1 );
}

atVOID	OEM_DrawRect( OEM_GDC gdc, atINT nX, atINT nY, atINT nW, atINT nH, atCOLOR nColor, atINT nLineWidth)
{	
	atGRPX_DrawRectWidth( (atGRPX_CANVAS *)gdc, nX, nY, nX+nW-1, nY+nH-1, nColor, nLineWidth );
}

atVOID	OEM_DrawLine( OEM_GDC gdc, atINT nX1, atINT nY1, atINT nX2, atINT nY2, atCOLOR nColor, atINT nLineWidth, atBOOL IsAA)
{	
    if( IsAA )
	    atGRPX_DrawLineWidthAA( (atGRPX_CANVAS *)gdc, nX1, nY1, nX2, nY2, nColor, nLineWidth );
    else
	    atGRPX_DrawLineWidth( (atGRPX_CANVAS *)gdc, nX1, nY1, nX2, nY2, nColor, nLineWidth );
}

atVOID	OEM_DrawCircle( OEM_GDC gdc, atINT nX, atINT nY, atINT nRadius, atCOLOR nColor, atINT nLineWidth)
{	
//	atGRPX_DrawCircleWidth( (atGRPX_CANVAS *)gdc, nX, nY, nRadius, nColor, nLineWidth );
	atGRPX_DrawCircle( (atGRPX_CANVAS *)gdc, nX, nY, nRadius, nColor );
}

atVOID	OEM_FillCircle( OEM_GDC gdc, atINT nX, atINT nY, atINT nRadius, atCOLOR nColor, atINT nAlpha)
{	
	atGRPX_FillCircleTrans( (atGRPX_CANVAS *)gdc, nX, nY, nRadius, nColor, nAlpha );
}

atVOID	OEM_DrawEllipse( OEM_GDC gdc, atINT nX, atINT nY, atINT nRadiusX, atINT nRadiusY, atCOLOR nColor, atINT nLineWidth)
{
	atGRPX_DrawEllipse( (atGRPX_CANVAS *)gdc, nX, nY, nRadiusX, nRadiusY, nColor );
}

atVOID	OEM_FillEllipse( OEM_GDC gdc, atINT nX, atINT nY, atINT nRadiusX, atINT nRadiusY, atCOLOR nColor, atINT nAlpha)
{
	atGRPX_FillEllipse( (atGRPX_CANVAS *)gdc, nX, nY, nRadiusX, nRadiusY, nColor );
}

atVOID		OEM_DrawPolyLine( OEM_GDC gdc, atPOINT lpPtrList[], atINT nCount, atBOOL bClosed, atCOLOR Color, atINT nWidth, atBOOL IsAA )
{
     if( IsAA )
         atGRPX_DrawPolyLineWidthAA( (atGRPX_CANVAS *)gdc, lpPtrList, nCount, bClosed, Color, nWidth );
     else
         atGRPX_DrawPolyLineWidth( (atGRPX_CANVAS *)gdc, lpPtrList, nCount, bClosed, Color, nWidth );
}

atVOID		OEM_DrawPolyLineEx( OEM_GDC gdc, atPOINT lpPtrList[], atINT nCount, atBOOL bClosed, atCOLOR Color, atINT nWidth, atBOOL IsAA, atUI_FUNC_COORD_CONV_REAL *lpFunc, atLPVOID lpParam )
{
     if( IsAA )
         atGRPX_DrawPolyLineWidthExAA( (atGRPX_CANVAS *)gdc, lpPtrList, nCount, bClosed, Color, nWidth, (atGRPX_FUNC_COORD_CONV_REAL *)lpFunc, lpParam );
     else
         atGRPX_DrawPolyLineWidthEx( (atGRPX_CANVAS *)gdc, lpPtrList, nCount, bClosed, Color, nWidth, (atGRPX_FUNC_COORD_CONV_REAL *)lpFunc, lpParam );
}

atVOID		OEM_FillPolygon( OEM_GDC gdc, atPOINT lpPtrList[], atINT nCount, atCOLOR Color, atBOOL IsAA, atINT nAlpha )
{
    if( IsAA )
        atGRPX_FillPolygonAA( (atGRPX_CANVAS *)gdc, lpPtrList, nCount, Color );
    else
        atGRPX_FillPolygon( (atGRPX_CANVAS *)gdc, lpPtrList, nCount, Color );
}

atVOID		OEM_FillPolygonBitmap( OEM_GDC gdc, atPOINT lpPtrList[], atINT nCount, OEM_IMAGE hBitmap )
{
    atGRPX_FillPolygonBitmap( (atGRPX_CANVAS *)gdc, lpPtrList, nCount, (atGRPX_BITMAP *)hBitmap );
}

atVOID		OEM_FillPolyPolygon( OEM_GDC gdc, atPOINT lpPtrList[], atINT nPartCount[], atINT nPolyCount, atCOLOR Color, atBOOL IsAA, atINT nAlpha )
{
    if( IsAA )
        atGRPX_FillPolyPolygonAA( (atGRPX_CANVAS *)gdc, lpPtrList, nPartCount, nPolyCount, Color );
    else
        atGRPX_FillPolyPolygon( (atGRPX_CANVAS *)gdc, lpPtrList, nPartCount, nPolyCount, Color );
}

atVOID		OEM_FillPolyPolygonEx( OEM_GDC gdc, atPOINT lpPtrList[], atINT nPartCount[], atINT nPolyCount, atCOLOR Color, atBOOL IsAA, atINT nAlpha, atUI_FUNC_COORD_CONV *lpFunc, atLPVOID lpParam )
{
    if( IsAA )
        atGRPX_FillPolyPolygonExAA( (atGRPX_CANVAS *)gdc, lpPtrList, nPartCount, nPolyCount, Color, (atGRPX_FUNC_COORD_CONV *)lpFunc, lpParam );
    else
        atGRPX_FillPolyPolygonEx( (atGRPX_CANVAS *)gdc, lpPtrList, nPartCount, nPolyCount, Color, (atGRPX_FUNC_COORD_CONV *)lpFunc, lpParam );
}

atVOID		OEM_FillPolyPolygonBitmap( OEM_GDC gdc, atPOINT lpPtrList[], atINT nPartCount[], atINT nPolyCount, OEM_IMAGE hBitmap )
{
    atGRPX_FillPolyPolygonBitmap( (atGRPX_CANVAS *)gdc, lpPtrList, nPartCount, nPolyCount, (atGRPX_BITMAP *)hBitmap );
}

atVOID		OEM_FillPolyPolygonBitmapEx( OEM_GDC gdc, atPOINT lpPtrList[], atINT nPartCount[], atINT nPolyCount, OEM_IMAGE hBitmap, atUI_FUNC_COORD_CONV *lpFunc, atLPVOID lpParam )
{
    atGRPX_FillPolyPolygonBitmapEx( (atGRPX_CANVAS *)gdc, lpPtrList, nPartCount, nPolyCount, (atGRPX_BITMAP *)hBitmap, (atGRPX_FUNC_COORD_CONV *)lpFunc, lpParam );
}


OEM_STRBITMAP OEM_CreateStrBitmap( OEM_FONT hOemFont, atLPTSTR szStrData, atINT nLen )
{
	return (OEM_STRBITMAP)atGRPX_CreateStrBitmap( g_lpGrpX, (atGRPX_FONT*)hOemFont, szStrData, nLen );
}

atVOID OEM_DestroyStrBitmap( OEM_STRBITMAP hStrBitmap )
{
//	atGRPX_DestroyStrBitmap( (atGRPX_STRBITMAP *)hStrBitmap );
	atGRPX_DestroyBitmap( (atGRPX_BITMAP *)hStrBitmap );
}

atVOID OEM_GetStrBitmapMetric( OEM_STRBITMAP hStrBitmap, atSIZE *lpSize )
{
//	atGRPX_GetStrBitmapMetric( (atGRPX_STRBITMAP *)hStrBitmap, lpSize );
	atGRPX_GetBitmapMetric( (atGRPX_BITMAP *)hStrBitmap, lpSize );
}

atVOID OEM_DrawStrBitmap( OEM_GDC hCanvas, atINT X, atINT Y, OEM_STRBITMAP hStrBitmap, atINT iStartPos, atINT iWidth, atCOLOR Color )
{
//    atGRPX_DrawStrBitmap( (atGRPX_CANVAS *)hCanvas, X, Y, (atGRPX_STRBITMAP *)hStrBitmap, iStartPos, iWidth, Color );
    atGRPX_DrawGlyphPart( (atGRPX_CANVAS *)hCanvas, (atGRPX_BITMAP *)hStrBitmap, X, Y, Color, iStartPos, iWidth );
}



OEM_IMAGE 	OEM_CreateBlankBitmap( atINT nWidth, atINT nHeight, atINT nBPP, atLPCALLBACKALLOC fnAllocator, atLPCALLBACKFREE fnDeallocator )
{
	atGRPX_BITMAP *img = atGRPX_CreateBlankBitmapEx( g_lpGrpX, nWidth, nHeight, nBPP, 0, fnAllocator, fnDeallocator );
	if( img == atNULL ) return atNULL;

	return (OEM_IMAGE)img;
}

atVOID 		OEM_DestroyBitmap( OEM_IMAGE lpBM )
{
    atGRPX_DestroyBitmap( (atGRPX_BITMAP *)lpBM);
}

atLPVOID		OEM_GetBitmapBits( OEM_IMAGE lpBM )
{
    atGRPX_BITMAP *lpBitmap = (atGRPX_BITMAP *)lpBM;
	if( lpBitmap == atNULL ) return 0;
	
    return lpBitmap->lpBits;
}

atINT		OEM_GetBitmapBPP( OEM_IMAGE lpBM )
{
    atGRPX_BITMAP *lpBitmap = (atGRPX_BITMAP *)lpBM;
	if( lpBitmap == atNULL ) return 0;
	
    return lpBitmap->nBPP;
}

atINT			OEM_GetBitmapWidth( OEM_IMAGE lpBM )
{
    atGRPX_BITMAP *lpBitmap = (atGRPX_BITMAP *)lpBM;
	if( lpBitmap == atNULL ) return 0;
	
    return lpBitmap->nWidth;
}

atINT			OEM_GetBitmapWidthBytes( OEM_IMAGE lpBM )
{
    atGRPX_BITMAP *lpBitmap = (atGRPX_BITMAP *)lpBM;
	if( lpBitmap == atNULL ) return 0;
	
    return lpBitmap->nWidthBytes;
}

atINT			OEM_GetBitmapHeight( OEM_IMAGE lpBM )
{
    atGRPX_BITMAP *lpBitmap = (atGRPX_BITMAP *)lpBM;
	if( lpBitmap == atNULL ) return 0;
	
    return lpBitmap->nHeight;
}

atVOID OEM_GetBitmapLogPos( OEM_IMAGE lpBM, atINT *pLogX, atINT *pLogY )
{
	atGRPX_GetBitmapLogPos( (atGRPX_BITMAP *)lpBM, pLogX, pLogY );
}

atVOID OEM_SetBitmapLogPos( OEM_IMAGE lpBM, atINT nLogX, atINT nLogY )
{
	atGRPX_SetBitmapLogPos( (atGRPX_BITMAP *)lpBM, nLogX, nLogY );
}

OEM_IMAGE 	OEM_CreateBitmapFromStream( atBYTE* lpBMPSrc, atINT dwSize, atLPCALLBACKALLOC fnAllocator, atLPCALLBACKFREE fnDeallocator )
{
	atGRPX_BITMAP *img = atGRPX_CreateBitmapFromStreamEx( g_lpGrpX, (atCHAR *)lpBMPSrc, 16, 0, fnAllocator, fnDeallocator );

	return( (OEM_IMAGE)img );
}

/*
OEM_BITMAPMASK OEM_CreateBitmapMask( atVOID *lpSrcData, atLONG size )
{
	return (OEM_BITMAPMASK)atGRPX_CreateMaskFromStream( g_lpGrpX, (const char *)lpSrcData );
}

atVOID OEM_DestroyBitmapMask( OEM_BITMAPMASK lpBM)
{
	atUIAPI_FreeMem( (atVOID *)lpBM );
}
*/

atVOID 		OEM_SetBitmapTransColor( OEM_IMAGE lpBM, atCOLOR clTransColor )
{
	atGRPX_SetBitmapTransColor( (atGRPX_BITMAP *)lpBM, clTransColor );
}

atVOID 		OEM_ClearBitmapTransColor( OEM_IMAGE lpBM )
{
	atGRPX_ClearBitmapTransColor( (atGRPX_BITMAP *)lpBM );
}

/*
atVOID 		OEM_SetBitmapMask( OEM_IMAGE lpBM, OEM_BITMAPMASK lpMask, atBOOL bOwnMask )
{
	atGRPX_SetBitmapAlphaChannel( (atGRPX_BITMAP *)lpBM, (atLPBYTE)lpMask, bOwnMask );
}
*/

atVOID 		OEM_GetBitmap( OEM_GDC gdc, OEM_IMAGE bmpDDImage, atINT nPosX, atINT nPosY )
{
    atGRPX_GetBitmap( (atGRPX_CANVAS *)gdc, (atGRPX_BITMAP *)bmpDDImage, nPosX, nPosY );
}

atVOID 		OEM_DrawBitmap( OEM_GDC gdc, OEM_IMAGE bmpDDImage, atINT nPosX, atINT nPosY, atINT nWidth, atINT nHeight, 
			  					   atINT nSrcPosX, atINT nSrcPosY )
{
//    if( ((atGRPX_BITMAP *)bmpDDImage)->lpAlphaChannel )
//    	atGRPX_DrawBitmapPartWithAlphaChannel( (atGRPX_CANVAS *)gdc, (atGRPX_BITMAP *)bmpDDImage, nPosX, nPosY, nSrcPosX, nSrcPosY, nWidth, nHeight, ((atGRPX_BITMAP *)bmpDDImage)->lpAlphaChannel );
//    else
	atGRPX_DrawBitmapPart( (atGRPX_CANVAS *)gdc, (atGRPX_BITMAP *)bmpDDImage, nPosX, nPosY, nSrcPosX, nSrcPosY, nWidth, nHeight );
}

void 		OEM_DrawBitmapStretch( OEM_GDC gdc, OEM_IMAGE bmpDDImage, int nPosX, int nPosY, int nWidth, int nHeight )
{
    atGRPX_DrawBitmapStretch( (atGRPX_CANVAS *)gdc, (atGRPX_BITMAP *)bmpDDImage, nPosX, nPosY, nWidth, nHeight );
}

void 		OEM_DrawBitmapRotate( OEM_GDC gdc, OEM_IMAGE bmpDDImage, int nPosX, int nPosY, atREAL rAngle )
{
    atGRPX_DrawBitmapRotate( (atGRPX_CANVAS *)gdc, (atGRPX_BITMAP *)bmpDDImage, nPosX, nPosY, rAngle );
}

/*
atVOID 		OEM_DrawBitmapWithAlphaChannel( OEM_GDC gdc, OEM_IMAGE bmpDDImage, atINT nPosX, atINT nPosY, atINT nWidth, atINT nHeight, 
			  					   atINT nSrcPosX, atINT nSrcPosY, OEM_BITMAPMASK bmpMask )
{
    atGRPX_DrawBitmapPartWithAlphaChannel( (atGRPX_CANVAS *)gdc, (atGRPX_BITMAP *)bmpDDImage, nPosX, nPosY, nSrcPosX, nSrcPosY, nWidth, nHeight, (atBYTE *)bmpMask );
}

atVOID 		OEM_DrawBitmapStretchWithAlphaChannel( OEM_GDC gdc, OEM_IMAGE bmpDDImage, atINT nPosX, atINT nPosY, atINT nWidth, atINT nHeight, OEM_BITMAPMASK bmpMask )
{
    atGRPX_DrawBitmapStretchWithAlphaChannel( (atGRPX_CANVAS *)gdc, (atGRPX_BITMAP *)bmpDDImage, nPosX, nPosY, nWidth, nHeight, (atBYTE *)bmpMask );
}

void 		OEM_DrawBitmapRotateWithAlphaChannel( OEM_GDC gdc, OEM_IMAGE bmpDDImage, int nPosX, int nPosY, atREAL rAngle, OEM_BITMAPMASK bmpMask )
{
    atGRPX_DrawBitmapRotateWithAlphaChannel( (atGRPX_CANVAS *)gdc, (atGRPX_BITMAP *)bmpDDImage, nPosX, nPosY, rAngle, (atBYTE *)bmpMask );
}

atVOID 		OEM_DrawBitmapTrans( OEM_GDC gdc, OEM_IMAGE bmpDDImage, atINT nPosX, atINT nPosY, atINT nAlpha )
{
    atGRPX_DrawBitmapTrans( (atGRPX_CANVAS *)gdc, (atGRPX_BITMAP *)bmpDDImage, nPosX, nPosY, nAlpha );
}
*/
