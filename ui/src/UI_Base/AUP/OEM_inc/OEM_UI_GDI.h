/**
  @file OEM_UI_GDI.h
  @brief GDI에 관한 포팅 OEM API 선언
  @remark GDI에 관한 포팅 OEM API 선언
  @author Amos
  @date 2010-04-19 오전 11:41:00
  @see UI_GDI.h
*/
#ifndef __OEM_UI_GDI_H_
#define __OEM_UI_GDI_H_

#include "UI_Types.h"
#include "A_UIENG.h"
#include "AT_Real.h"
#include "OEM_UI_Font.h"


#ifdef __cplusplus
extern "C" {
#endif

typedef atLPVOID  OEM_UI_GDC;
typedef atLPVOID  OEM_UI_IMAGE;
//typedef atLPVOID  OEM_UI_BITMAPMASK;
typedef atLPVOID  OEM_UI_STRBITMAP;


typedef atBOOL				FN_OEM_UI_InitGraphicSystem( atLONG lSysInst, atLONG lInitData1, atLONG lInitData2 );
typedef atVOID				FN_OEM_UI_CloseGraphicSystem( NOARGS );
typedef atVOID 				FN_OEM_UI_FlushScreenEnable(atBOOL bEnable);

typedef atBOOL 				FN_OEM_UI_SetScreenResolution( atINT nWidth, atINT nHeight );
typedef atBOOL	  			FN_OEM_UI_GetScreenDPI( atINT *lpnX_DPI, atINT *lpnY_DPI );
typedef atINT 				FN_OEM_UI_GetScreenWidth( NOARGS );
typedef atINT 				FN_OEM_UI_GetScreenHeight( NOARGS );
typedef atINT 				FN_OEM_UI_GetScreenBPP( NOARGS );
typedef atLPVOID			FN_OEM_UI_GetScreenBufferPtr( NOARGS );

typedef atBOOL				FN_OEM_UI_BeginDraw( NOARGS );
typedef atVOID				FN_OEM_UI_EndDraw( NOARGS );

//typedef atINT 			FN_OEM_UI_GetAnnounceHeight( NOARGS );
typedef atVOID 	 			FN_OEM_UI_FlushScreenRgn( atINT nX, atINT nY, atINT nWidth, atINT nHeight );
//typedef atVOID			FN_OEM_UI_FlushToDC(atHGDC dc, atINT sx, atINT dy, atINT width, atINT height);

typedef atULONG 			FN_OEM_UI_GetRGB(atBYTE r, atBYTE g, atBYTE b);
typedef atBYTE				FN_OEM_UI_GetRValue(atULONG nColor);
typedef atBYTE				FN_OEM_UI_GetGValue(atULONG nColor);
typedef atBYTE				FN_OEM_UI_GetBValue(atULONG nColor);

//typedef atINT				FN_OEM_UI_GetStringWidth( OEM_UI_FONT hOemFont, atLPTSTR str, atINT len );
//typedef atINT				FN_OEM_UI_GetFontHeight( OEM_UI_FONT hOemFont );

typedef OEM_UI_GDC			FN_OEM_UI_CreateCanvas( atINT nX, atINT nY, atINT nWidth, atINT nHeight, atINT bpp, atDWORD dwAttr );
typedef atVOID 				FN_OEM_UI_DestroyCanvas( OEM_UI_GDC gdc );
typedef atVOID 				FN_OEM_UI_GetCanvasAttr( OEM_UI_GDC gdc, atUI_CANVAS_ATTR *lpCanvasAttr );

typedef atVOID				FN_OEM_UI_GetCanvasArea(OEM_UI_GDC gdc, atREGION *rgnArea);
typedef atVOID				FN_OEM_UI_SetCanvasArea(OEM_UI_GDC gdc, atREGION *rgnArea);
typedef atVOID				FN_OEM_UI_GetCanvasClipRect(OEM_UI_GDC gdc, atRECT *rtClip);
typedef atVOID				FN_OEM_UI_SetCanvasClipRect(OEM_UI_GDC gdc, atRECT *rtClip);
//typedef atLPVOID			FN_OEM_UI_GetScreenBufferPtr( OEM_UI_GDC cnv );
typedef atVOID 				FN_OEM_UI_CopyCanvas( OEM_UI_GDC hDesCanvas, atINT nDesX, atINT nDesY, OEM_UI_GDC hSrcCanvas, atINT nX, atINT nY, atINT nWidth, atINT nHeight );

//typedef atVOID				FN_OEM_UI_FlushCanvasRgn( OEM_UI_GDC cnv, atINT nX, atINT nY, atINT nWidth, atINT nHeight );

//typedef OEM_UI_FONT		FN_OEM_UI_SetFont( OEM_UI_GDC cnv, OEM_UI_FONT hFont );
//typedef OEM_UI_FONT 		FN_OEM_UI_GetFont( OEM_UI_GDC cnv );
typedef atVOID 				FN_OEM_UI_SetFrColor( OEM_UI_GDC cnv, atCOLOR color );
typedef atCOLOR 			FN_OEM_UI_GetFrColor( OEM_UI_GDC cnv );
typedef atVOID 				FN_OEM_UI_SetBgColor( OEM_UI_GDC cnv, atCOLOR color );
typedef atCOLOR 			FN_OEM_UI_GetBgColor( OEM_UI_GDC cnv );

typedef atCOLOR				FN_OEM_UI_GetPixel( OEM_UI_GDC gdc, atINT x, atINT y );
typedef atVOID				FN_OEM_UI_DrawPixel( OEM_UI_GDC gdc, atINT x, atINT y, atCOLOR color );
typedef atVOID				FN_OEM_UI_FillRect( OEM_UI_GDC cnv, atINT nX, atINT nY, atINT nW, atINT nH, atCOLOR nColor, atINT nAlpha);
typedef atVOID  			FN_OEM_UI_FillRectXOR( OEM_UI_GDC gdc, atINT nX, atINT nY, atINT nW, atINT nH );
typedef atVOID				FN_OEM_UI_DrawRect( OEM_UI_GDC cnv, atINT nX, atINT nY, atINT nW, atINT nH, atCOLOR nColor, atINT nLineWidth);
typedef atVOID				FN_OEM_UI_DrawLine( OEM_UI_GDC cnv, atINT nX1, atINT nY1, atINT nX2, atINT nY2, atCOLOR nColor, atINT nLineWidth, atBOOL IsAA);
typedef atVOID				FN_OEM_UI_DrawCircle( OEM_UI_GDC gdc, atINT nX, atINT nY, atINT nRadius, atCOLOR nColor, atINT nLineWidth);
typedef atVOID				FN_OEM_UI_FillCircle( OEM_UI_GDC gdc, atINT nX, atINT nY, atINT nRadius, atCOLOR nColor, atINT nAlpha);
typedef atVOID				FN_OEM_UI_DrawEllipse( OEM_UI_GDC gdc, atINT nX, atINT nY, atINT nRadiusX, atINT nRadiusY, atCOLOR nColor, atINT nLineWidth);
typedef atVOID				FN_OEM_UI_FillEllipse( OEM_UI_GDC gdc, atINT nX, atINT nY, atINT nRadiusX, atINT nRadiusY, atCOLOR nColor, atINT nAlpha);
typedef atVOID				FN_OEM_UI_DrawPolyLine( OEM_UI_GDC gdc, atPOINT lpPtrList[], atINT nCount, atBOOL bClosed, atCOLOR Color, atINT nWidth, atBOOL IsAA );
typedef atVOID				FN_OEM_UI_DrawPolyLineEx( OEM_UI_GDC gdc, atPOINT lpPtrList[], atINT nCount, atBOOL bClosed, atCOLOR Color, atINT nWidth, atBOOL IsAA, atUI_FUNC_COORD_CONV_REAL *lpFunc, atLPVOID lpParam );
typedef atVOID				FN_OEM_UI_FillPolygon( OEM_UI_GDC gdc, atPOINT lpPtrList[], atINT nCount, atCOLOR Color, atBOOL IsAA, atINT nAlpha );
typedef atVOID				FN_OEM_UI_FillPolygonBitmap( OEM_UI_GDC gdc, atPOINT lpPtrList[], atINT nCount, OEM_UI_IMAGE hBitmap );
typedef atVOID				FN_OEM_UI_FillPolyPolygon( OEM_UI_GDC gdc, atPOINT lpPtrList[], atINT nPartCount[], atINT nPolyCount, atCOLOR Color, atBOOL IsAA, atINT nAlpha );
typedef atVOID				FN_OEM_UI_FillPolyPolygonEx( OEM_UI_GDC gdc, atPOINT lpPtrList[], atINT nPartCount[], atINT nPolyCount, atCOLOR Color, atBOOL IsAA, atINT nAlpha, atUI_FUNC_COORD_CONV *lpFunc, atLPVOID lpParam );
typedef atVOID				FN_OEM_UI_FillPolyPolygonBitmap( OEM_UI_GDC gdc, atPOINT lpPtrList[], atINT nPartCount[], atINT nPolyCount, OEM_UI_IMAGE hBitmap );
typedef atVOID				FN_OEM_UI_FillPolyPolygonBitmapEx( OEM_UI_GDC gdc, atPOINT lpPtrList[], atINT nPartCount[], atINT nPolyCount, OEM_UI_IMAGE hBitmap, atUI_FUNC_COORD_CONV *lpFunc, atLPVOID lpParam );

typedef atINT 				FN_OEM_UI_DrawString( OEM_UI_GDC cnv, atINT x, atINT y, atLPTSTR str, atINT len, atCOLOR color, atINT nLineGap, OEM_UI_FONT hOemFont );
typedef atINT 				FN_OEM_UI_DrawStringRotate( OEM_UI_GDC gdc, atINT x, atINT y, atLPTSTR str, atINT len, atCOLOR color, atREAL rAngle, atINT nBaseX, atINT nBase, OEM_UI_FONT hOemFont );

typedef OEM_UI_STRBITMAP	FN_OEM_UI_CreateStrBitmap( OEM_UI_FONT hOemFont, atLPTSTR szStrData, atINT nLen );
typedef atVOID				FN_OEM_UI_DestroyStrBitmap( OEM_UI_STRBITMAP hStrBitmap );
typedef atVOID				FN_OEM_UI_GetStrBitmapMetric( OEM_UI_STRBITMAP hStrBitmap, atSIZE *lpSize );
typedef atVOID				FN_OEM_UI_DrawStrBitmap( OEM_UI_GDC hCanvas, atINT X, atINT Y, OEM_UI_STRBITMAP hStrBitmap, atINT iStartPos, atINT iWidth, atCOLOR Color );

typedef OEM_UI_IMAGE 		FN_OEM_UI_CreateBlankBitmap( atINT nWidth, atINT nHeight, atINT nBPP, atLPCALLBACKALLOC fnAllocator, atLPCALLBACKFREE fnDeallocator );
typedef atVOID 				FN_OEM_UI_DestroyBitmap( OEM_UI_IMAGE lpBM );
typedef atLPVOID			FN_OEM_UI_GetBitmapBits( OEM_UI_IMAGE lpBM );
typedef atINT				FN_OEM_UI_GetBitmapBPP( OEM_UI_IMAGE lpBM );
typedef atINT				FN_OEM_UI_GetBitmapWidth( OEM_UI_IMAGE lpBM );
typedef atINT				FN_OEM_UI_GetBitmapHeight( OEM_UI_IMAGE lpBM );
typedef atINT				FN_OEM_UI_GetBitmapWidthBytes( OEM_UI_IMAGE lpBM );
typedef atVOID 				FN_OEM_UI_GetBitmapLogPos( OEM_UI_IMAGE lpBM, atINT *pLogX, atINT *pLogY );
typedef atVOID 				FN_OEM_UI_SetBitmapLogPos( OEM_UI_IMAGE lpBM, atINT nLogX, atINT nLogY );
typedef atVOID 				FN_OEM_UI_SetBitmapTransColor( OEM_UI_IMAGE lpBM, atCOLOR clTransColor );
typedef atVOID 				FN_OEM_UI_ClearBitmapTransColor( OEM_UI_IMAGE lpBM );
//typedef atVOID 			FN_OEM_UI_SetBitmapMask( OEM_UI_IMAGE lpBM, OEM_UI_BITMAPMASK lpMask, atBOOL bOwnMask );
typedef OEM_UI_IMAGE 		FN_OEM_UI_CreateBitmapFromStream( atBYTE* lpBMPSrc, atINT dwSize, atLPCALLBACKALLOC fnAllocator, atLPCALLBACKFREE fnDeallocator );

//typedef OEM_UI_BITMAPMASK FN_OEM_UI_CreateBitmapMask( atVOID *lpSrcData, atLONG size );
//typedef atVOID 			FN_OEM_UI_DestroyBitmapMask( OEM_UI_BITMAPMASK lpBM);

typedef atVOID 				FN_OEM_UI_GetBitmap( OEM_UI_GDC gdc, OEM_UI_IMAGE bmpDDImage, atINT nPosX, atINT nPosY );
typedef atVOID 				FN_OEM_UI_DrawBitmap( OEM_UI_GDC gdc, OEM_UI_IMAGE bmpDDImage, atINT nPosX, atINT nPosY, atINT nWidth, atINT nHeight, atINT nSrcPosX, atINT nSrcPosY );
//typedef atVOID 			FN_OEM_UI_DrawBitmapWithAlphaChannel( OEM_UI_GDC gdc, OEM_UI_IMAGE bmpDDImage, atINT nPosX, atINT nPosY, atINT nWidth, atINT nHeight, atINT nSrcPosX, atINT nSrcPosY, OEM_UI_BITMAPMASK bmpMask );
typedef atVOID 				FN_OEM_UI_DrawBitmapStretch( OEM_UI_GDC gdc, OEM_UI_IMAGE bmpDDImage, atINT nPosX, atINT nPosY, atINT nWidth, atINT nHeight ); 
//typedef atVOID 			FN_OEM_UI_DrawBitmapStretchWithAlphaChannel( OEM_UI_GDC gdc, OEM_UI_IMAGE bmpDDImage, atINT nPosX, atINT nPosY, atINT nWidth, atINT nHeight, OEM_UI_BITMAPMASK bmpMask ); 
typedef atVOID 				FN_OEM_UI_DrawBitmapRotate( OEM_UI_GDC gdc, OEM_UI_IMAGE bmpDDImage, atINT nPosX, atINT nPosY, atREAL rAngle ); 
//typedef atVOID 			FN_OEM_UI_DrawBitmapRotateWithAlphaChannel( OEM_UI_GDC gdc, OEM_UI_IMAGE bmpDDImage, atINT nPosX, atINT nPosY, atREAL rAngle, OEM_UI_BITMAPMASK bmpMask ); 

typedef atLONG				FN_OEM_UI_ExtGraphicMethod( atBOOL CanvasActivate, OEM_UI_GDC gdc, atINT func_no, atLONG param1, atLONG param2, atLONG param3, atLONG param4, atLONG param5, atLONG param6 );



typedef struct tagOEM_UI_GDI {
	FN_OEM_UI_InitGraphicSystem						*fnInitGraphicSystem;
	FN_OEM_UI_CloseGraphicSystem					*fnCloseGraphicSystem;
	FN_OEM_UI_FlushScreenEnable						*fnFlushScreenEnable;

	FN_OEM_UI_SetScreenResolution					*fnSetScreenResolution;
	FN_OEM_UI_GetScreenDPI							*fnGetScreenDPI;
	FN_OEM_UI_GetScreenWidth						*fnGetScreenWidth;
	FN_OEM_UI_GetScreenHeight						*fnGetScreenHeight;
	FN_OEM_UI_GetScreenBPP							*fnGetScreenBPP;
	FN_OEM_UI_GetScreenBufferPtr					*fnGetScreenBufferPtr;

	FN_OEM_UI_BeginDraw								*fnBeginDraw;
	FN_OEM_UI_EndDraw								*fnEndDraw;

	//FN_OEM_UI_GetAnnounceHeight					*fnGetAnnounceHeight;
	FN_OEM_UI_FlushScreenRgn						*fnFlushScreenRgn;
	//FN_OEM_UI_FlushToDC							*fnFlushToDC;

	FN_OEM_UI_GetRGB								*fnGetRGB;
	FN_OEM_UI_GetRValue								*fnGetRValue;
	FN_OEM_UI_GetGValue								*fnGetGValue;
	FN_OEM_UI_GetBValue								*fnGetBValue;

	//FN_OEM_UI_GetStringWidth						*fnGetStringWidth;
	//FN_OEM_UI_GetFontHeight						*fnGetFontHeight;

	FN_OEM_UI_CreateCanvas							*fnCreateCanvas;
	FN_OEM_UI_DestroyCanvas							*fnDestroyCanvas;
	FN_OEM_UI_GetCanvasAttr							*fnGetCanvasAttr;

	FN_OEM_UI_GetCanvasArea							*fnGetCanvasArea;
	FN_OEM_UI_SetCanvasArea							*fnSetCanvasArea;
	FN_OEM_UI_GetCanvasClipRect						*fnGetCanvasClipRect;
	FN_OEM_UI_SetCanvasClipRect						*fnSetCanvasClipRect;
	//FN_OEM_UI_GetScreenBufferPtr					*fnGetScreenBufferPtr;
	FN_OEM_UI_CopyCanvas							*fnCopyCanvas;

	//FN_OEM_UI_FlushCanvasRgn						*fnFlushCanvasRgn;

	//FN_OEM_UI_SetFont								*fnSetFont;
	//FN_OEM_UI_GetFont								*fnGetFont;
	FN_OEM_UI_SetFrColor							*fnSetFrColor;
	FN_OEM_UI_GetFrColor							*fnGetFrColor;
	FN_OEM_UI_SetBgColor							*fnSetBgColor;
	FN_OEM_UI_GetBgColor							*fnGetBgColor;

	FN_OEM_UI_GetPixel								*fnGetPixel;
	FN_OEM_UI_DrawPixel								*fnDrawPixel;
	FN_OEM_UI_FillRect								*fnFillRect;
	FN_OEM_UI_FillRectXOR							*fnFillRectXOR;
	FN_OEM_UI_DrawRect								*fnDrawRect;
	FN_OEM_UI_DrawLine								*fnDrawLine;
	FN_OEM_UI_DrawCircle							*fnDrawCircle;
	FN_OEM_UI_FillCircle							*fnFillCircle;
	FN_OEM_UI_DrawEllipse							*fnDrawEllipse;
	FN_OEM_UI_FillEllipse							*fnFillEllipse;
	FN_OEM_UI_DrawPolyLine							*fnDrawPolyLine;
	FN_OEM_UI_DrawPolyLineEx						*fnDrawPolyLineEx;
	FN_OEM_UI_FillPolygon							*fnFillPolygon;
	FN_OEM_UI_FillPolygonBitmap						*fnFillPolygonBitmap;
	FN_OEM_UI_FillPolyPolygon						*fnFillPolyPolygon;
	FN_OEM_UI_FillPolyPolygonEx						*fnFillPolyPolygonEx;
	FN_OEM_UI_FillPolyPolygonBitmap					*fnFillPolyPolygonBitmap;
	FN_OEM_UI_FillPolyPolygonBitmapEx				*fnFillPolyPolygonBitmapEx;

	FN_OEM_UI_DrawString							*fnDrawString;
	FN_OEM_UI_DrawStringRotate						*fnDrawStringRotate;

	FN_OEM_UI_CreateStrBitmap						*fnCreateStrBitmap;
	FN_OEM_UI_DestroyStrBitmap						*fnDestroyStrBitmap;
	FN_OEM_UI_GetStrBitmapMetric					*fnGetStrBitmapMetric;
	FN_OEM_UI_DrawStrBitmap							*fnDrawStrBitmap;

	FN_OEM_UI_CreateBlankBitmap						*fnCreateBlankBitmap;
	FN_OEM_UI_DestroyBitmap							*fnDestroyBitmap;
	FN_OEM_UI_GetBitmapBits							*fnGetBitmapBits;
	FN_OEM_UI_GetBitmapBPP							*fnGetBitmapBPP;
	FN_OEM_UI_GetBitmapWidth						*fnGetBitmapWidth;
	FN_OEM_UI_GetBitmapHeight						*fnGetBitmapHeight;
	FN_OEM_UI_GetBitmapWidthBytes					*fnGetBitmapWidthBytes;
	FN_OEM_UI_GetBitmapLogPos						*fnGetBitmapLogPos;
	FN_OEM_UI_SetBitmapLogPos						*fnSetBitmapLogPos;
	FN_OEM_UI_SetBitmapTransColor					*fnSetBitmapTransColor;
	FN_OEM_UI_ClearBitmapTransColor					*fnClearBitmapTransColor;
	//FN_OEM_UI_SetBitmapMask						*fnSetBitmapMask;
	FN_OEM_UI_CreateBitmapFromStream				*fnCreateBitmapFromStream;

	//FN_OEM_UI_CreateBitmapMask					*fnCreateBitmapMask;
	//FN_OEM_UI_DestroyBitmapMask					*fnDestroyBitmapMask;

	FN_OEM_UI_GetBitmap								*fnGetBitmap;
	FN_OEM_UI_DrawBitmap							*fnDrawBitmap;
	//FN_OEM_UI_DrawBitmapWithAlphaChannel			*fnDrawBitmapWithAlphaChannel;
	FN_OEM_UI_DrawBitmapStretch						*fnDrawBitmapStretch;
	//FN_OEM_UI_DrawBitmapStretchWithAlphaChannel	*fnDrawBitmapStretchWithAlphaChannel;
	FN_OEM_UI_DrawBitmapRotate						*fnDrawBitmapRotate;
	//FN_OEM_UI_DrawBitmapRotateWithAlphaChannel	*fnDrawBitmapRotateWithAlphaChannel;

	FN_OEM_UI_ExtGraphicMethod						*fnExtGraphicMethod;
} OEM_UI_GDI_T;


atVOID	OEM_UI_GDI_SetOemFn( OEM_UI_GDI_T *fnOem );
atVOID	OEM_UI_GDI_GetOemFn( OEM_UI_GDI_T *fnOem );


#ifdef __cplusplus
}
#endif


#endif 

