 //
// Canvas_wipi.c
//
////////////////////////////////////////////////////////////////

#include "OEM_UI_GDI.h"
#include "UI_API/UI_API.h"



atBOOL DEF_OEM_UI_InitGraphicSystem( atLONG lSysInst, atLONG lInitData1, atLONG lInitData2 )
{
	return atTRUE;
}

void DEF_OEM_UI_CloseGraphicSystem(void)
{
}

atBOOL DEF_OEM_UI_SetScreenResolution( atINT nWidth, atINT nHeight )
{
	return atTRUE;
}

atBOOL DEF_OEM_UI_GetScreenDPI( atINT *lpnX_DPI, atINT *lpnY_DPI )
{
	return atTRUE;
}

atINT DEF_OEM_UI_GetScreenWidth( NOARGS )
{
	return 0;
}

atINT DEF_OEM_UI_GetScreenHeight( NOARGS )
{
	return 0;
}

atINT DEF_OEM_UI_GetScreenBPP( NOARGS )
{
	return SYSAPI_GetScreenBPP();
}

atVOID * DEF_OEM_UI_GetScreenBufferPtr( NOARGS )
{
	return atNULL;
}

 //
// Canvas_wipi.c
//
////////////////////////////////////////////////////////////////

#include "OEM_UI_GDI.h"
#include "OEM_UI_Mem.h"


#define MAKE565(r,g,b) ((atWORD)(((((atWORD)((r) & 0xF8)) << (5+6-3))) | \
					   ((((atWORD)((g) & 0xFC)) << (5-2))) | \
					   (((atWORD)((b) & 0xF8)) >> 3)))

atULONG DEF_OEM_GetRGB(atBYTE r, atBYTE g, atBYTE b)
{
	return 0;
}

atBYTE DEF_OEM_GetRValue(atULONG nColor)
{
	return 0;
}

atBYTE DEF_OEM_GetGValue(atULONG nColor)
{
	return 0;
}

atBYTE DEF_OEM_GetBValue(atULONG nColor)
{
	return 0;
}


OEM_GDC DEF_OEM_CreateCanvas( atINT nX, atINT nY, atINT nWidth, atINT nHeight, atINT bpp, atDWORD dwAttr )
{
	return (OEM_GDC)atNULL;
}


atVOID DEF_OEM_DestroyCanvas( OEM_GDC gdc )
{
}


atVOID DEF_OEM_GetCanvasAttr( OEM_GDC gdc, atUI_CANVAS_ATTR *lpCanvasAttr )
{
}


atVOID DEF_OEM_GetCanvasArea(OEM_GDC gdc, atREGION *rgnArea)
{
}

atVOID DEF_OEM_SetCanvasArea(OEM_GDC gdc, atREGION *rgnArea)
{
}

atVOID DEF_OEM_GetCanvasClipRect(OEM_GDC gdc, atRECT *rtClip)
{
}

atVOID DEF_OEM_SetCanvasClipRect(OEM_GDC gdc, atRECT *rtClip)
{
}

atVOID  DEF_OEM_CopyCanvas( OEM_GDC hDesCanvas, atINT nDesX, atINT nDesY, OEM_GDC hSrcCanvas, atINT nX, atINT nY, atINT nWidth, atINT nHeight )
{
}

atVOID DEF_OEM_SetFrColor( OEM_GDC gdc, atCOLOR color )
{
}	 

atCOLOR DEF_OEM_GetFrColor( OEM_GDC gdc )
{
	return (atCOLOR)0x0000;
}	 

atVOID DEF_OEM_SetBgColor( OEM_GDC gdc, atCOLOR color )
{
}	 

atCOLOR DEF_OEM_GetBgColor( OEM_GDC gdc )
{
	return (atCOLOR)0x0000;
}	 

atCOLOR DEF_OEM_GetPixel( OEM_GDC gdc, atINT x, atINT y )
{
	return (atCOLOR)0x0000;
}

atVOID DEF_OEM_DrawPixel( OEM_GDC gdc, atINT x, atINT y, atCOLOR color )
{
}

atINT DEF_OEM_DrawString( OEM_GDC gdc, atINT x, atINT y, atLPTSTR str, atINT len, atCOLOR color, atINT nLineGap, OEM_FONT hOemFont )
{   
	return 0;
}

atINT DEF_OEM_DrawStringRotate( OEM_GDC gdc, atINT x, atINT y, atLPTSTR str, atINT len, atCOLOR color, atREAL rAngle, atINT nBaseX, atINT nBase, OEM_FONT hOemFont )
{
	return 0;
}

atVOID	DEF_OEM_FillRect( OEM_GDC gdc, atINT nX, atINT nY, atINT nW, atINT nH, atCOLOR nColor, atINT nAlpha)
{
}

atVOID  DEF_OEM_FillRectXOR( OEM_GDC gdc, atINT nX, atINT nY, atINT nW, atINT nH )
{
}

atVOID	DEF_OEM_DrawRect( OEM_GDC gdc, atINT nX, atINT nY, atINT nW, atINT nH, atCOLOR nColor, atINT nLineWidth)
{	
}

atVOID	DEF_OEM_DrawLine( OEM_GDC gdc, atINT nX1, atINT nY1, atINT nX2, atINT nY2, atCOLOR nColor, atINT nLineWidth, atBOOL IsAA)
{	
}

atVOID	DEF_OEM_DrawCircle( OEM_GDC gdc, atINT nX, atINT nY, atINT nRadius, atCOLOR nColor, atINT nLineWidth)
{	
}

atVOID	DEF_OEM_FillCircle( OEM_GDC gdc, atINT nX, atINT nY, atINT nRadius, atCOLOR nColor, atINT nAlpha)
{	
}

atVOID	DEF_OEM_DrawEllipse( OEM_GDC gdc, atINT nX, atINT nY, atINT nRadiusX, atINT nRadiusY, atCOLOR nColor, atINT nLineWidth)
{
}

atVOID	DEF_OEM_FillEllipse( OEM_GDC gdc, atINT nX, atINT nY, atINT nRadiusX, atINT nRadiusY, atCOLOR nColor, atINT nAlpha)
{
}

atVOID		DEF_OEM_DrawPolyLine( OEM_GDC gdc, atPOINT lpPtrList[], atINT nCount, atBOOL bClosed, atCOLOR Color, atINT nWidth, atBOOL IsAA )
{
}

atVOID		DEF_OEM_DrawPolyLineEx( OEM_GDC gdc, atPOINT lpPtrList[], atINT nCount, atBOOL bClosed, atCOLOR Color, atINT nWidth, atBOOL IsAA, atUI_FUNC_COORD_CONV_REAL *lpFunc, atLPVOID lpParam )
{
}

atVOID		DEF_OEM_FillPolygon( OEM_GDC gdc, atPOINT lpPtrList[], atINT nCount, atCOLOR Color, atBOOL IsAA, atINT nAlpha )
{
}

atVOID		DEF_OEM_FillPolygonBitmap( OEM_GDC gdc, atPOINT lpPtrList[], atINT nCount, OEM_IMAGE hBitmap )
{
}

atVOID		DEF_OEM_FillPolyPolygon( OEM_GDC gdc, atPOINT lpPtrList[], atINT nPartCount[], atINT nPolyCount, atCOLOR Color, atBOOL IsAA, atINT nAlpha )
{
}

atVOID		DEF_OEM_FillPolyPolygonEx( OEM_GDC gdc, atPOINT lpPtrList[], atINT nPartCount[], atINT nPolyCount, atCOLOR Color, atBOOL IsAA, atINT nAlpha, atUI_FUNC_COORD_CONV *lpFunc, atLPVOID lpParam )
{
}

atVOID		DEF_OEM_FillPolyPolygonBitmap( OEM_GDC gdc, atPOINT lpPtrList[], atINT nPartCount[], atINT nPolyCount, OEM_IMAGE hBitmap )
{
}

atVOID		DEF_OEM_FillPolyPolygonBitmapEx( OEM_GDC gdc, atPOINT lpPtrList[], atINT nPartCount[], atINT nPolyCount, OEM_IMAGE hBitmap, atUI_FUNC_COORD_CONV *lpFunc, atLPVOID lpParam )
{
}


OEM_STRBITMAP DEF_OEM_CreateStrBitmap( OEM_FONT hOemFont, atLPTSTR szStrData, atINT nLen )
{
	return atNULL;
}

atVOID DEF_OEM_DestroyStrBitmap( OEM_STRBITMAP hStrBitmap )
{
}

atVOID DEF_OEM_GetStrBitmapMetric( OEM_STRBITMAP hStrBitmap, atSIZE *lpSize )
{
}

atVOID DEF_OEM_DrawStrBitmap( OEM_GDC hCanvas, atINT X, atINT Y, OEM_STRBITMAP hStrBitmap, atINT iStartPos, atINT iWidth, atCOLOR Color )
{
}

OEM_IMAGE 	DEF_OEM_CreateBlankBitmap( atINT nWidth, atINT nHeight, atINT nBPP, atLPCALLBACKALLOC fnAllocator, atLPCALLBACKFREE fnDeallocator )
{
	return atNULL;
}

atVOID 		DEF_OEM_DestroyBitmap( OEM_IMAGE lpBM )
{
}

atLPVOID		DEF_OEM_GetBitmapBits( OEM_IMAGE lpBM )
{
    return atNULL;
}

atINT		DEF_OEM_GetBitmapBPP( OEM_IMAGE lpBM )
{
    return 0;
}

atINT			DEF_OEM_GetBitmapWidth( OEM_IMAGE lpBM )
{
    return 0;
}

atINT			DEF_OEM_GetBitmapWidthBytes( OEM_IMAGE lpBM )
{
    return 0;
}

atINT			DEF_OEM_GetBitmapHeight( OEM_IMAGE lpBM )
{
    return 0;
}

atVOID DEF_OEM_GetBitmapLogPos( OEM_IMAGE lpBM, atINT *pLogX, atINT *pLogY )
{
}

atVOID DEF_OEM_SetBitmapLogPos( OEM_IMAGE lpBM, atINT nLogX, atINT nLogY )
{
}

OEM_IMAGE 	DEF_OEM_CreateBitmapFromStream( atBYTE* lpBMPSrc, atINT dwSize, atLPCALLBACKALLOC fnAllocator, atLPCALLBACKFREE fnDeallocator )
{
	return atNULL;
}

atVOID 		DEF_OEM_SetBitmapTransColor( OEM_IMAGE lpBM, atCOLOR clTransColor )
{
}

atVOID 		DEF_OEM_ClearBitmapTransColor( OEM_IMAGE lpBM )
{
}

atVOID 		DEF_OEM_GetBitmap( OEM_GDC gdc, OEM_IMAGE bmpDDImage, atINT nPosX, atINT nPosY )
{
}

atVOID 		DEF_OEM_DrawBitmap( OEM_GDC gdc, OEM_IMAGE bmpDDImage, atINT nPosX, atINT nPosY, atINT nWidth, atINT nHeight, 
			  					   atINT nSrcPosX, atINT nSrcPosY )
{
}

void 		DEF_OEM_DrawBitmapStretch( OEM_GDC gdc, OEM_IMAGE bmpDDImage, int nPosX, int nPosY, int nWidth, int nHeight )
{
}

void 		DEF_OEM_DrawBitmapRotate( OEM_GDC gdc, OEM_IMAGE bmpDDImage, int nPosX, int nPosY, atREAL rAngle )
{
}
