/**
  @file UI_GDI.h
  @brief GDI(Graphic Draw Interface)에 관한 API
  @remark GDI(Graphic Draw Interface)에 관한 API
  @author Amos
  @date 2010-04-19 오전 11:41:00
  @see UI_GDI.c
*/
#ifndef __VD_GDI_H_
#define __VD_GDI_H_


#include "UI_Types.h"
#include "UI_API/UI_Font.h"


#ifdef __cplusplus
extern "C" {
#endif


typedef atHANDLE    atHGDC;
typedef atHANDLE    atHBITMAP;
typedef atHANDLE    atHSTRBITMAP;
//typedef atHANDLE	atHBITMAPMASK;

typedef atU32		atCOLOR;


typedef enum {
        ALIGN_LEFT_TOP,
        ALIGN_CENTER_TOP,
        ALIGN_RIGHT_TOP,
        ALIGN_LEFT_MIDDLE,
        ALIGN_CENTER_MIDDLE,
        ALIGN_RIGHT_MIDDLE,
        ALIGN_LEFT_BOTTOM,
        ALIGN_CENTER_BOTTOM,
        ALIGN_RIGHT_BOTTOM
} DRAW_ALIGN;

typedef struct tagatUI_CANVAS_ATTR {
    atULONG         dwAttr;
    atREGION       rgnArea;
    atRECT           rtClipRect;
    atINT              nBPP;
    atLPVOID        lpVideoPtr;
    atINT              nVMemWidth;
} atUI_CANVAS_ATTR;


#define     atUI_COLOR_TRANS             (atUIAPI_GetRGB(255, 0, 255))

#define     atUI_COLOR_WHITE             (atUIAPI_GetRGB(255, 255, 255))
#define     atUI_COLOR_BLACK             (atUIAPI_GetRGB(0, 0, 0))
#define     atUI_COLOR_RED               (atUIAPI_GetRGB(255, 0, 0))
#define     atUI_COLOR_GREEN             (atUIAPI_GetRGB(0, 255, 0))
#define     atUI_COLOR_BLUE              (atUIAPI_GetRGB(0, 0, 255))
#define     atUI_COLOR_YELLOW          (atUIAPI_GetRGB(255, 255, 0))
#define     atUI_COLOR_DARKGRAY      (atUIAPI_GetRGB(128, 128, 128))
#define     atUI_COLOR_GRAY              (atUIAPI_GetRGB(192, 192, 192))


#define atUI_CANVAS_ATTR_NONE           				(0x00)
#define atUI_CANVAS_ATTR_DYNAMIC_RESOLUTION    	(0x02)
#define atUI_CANVAS_ATTR_OWNERBUFFER   			(0x01)


typedef atBOOL (atUI_FUNC_COORD_CONV)( atLPVOID lpParam, atINT *lpDesX, atINT *lpDesY, atINT nSrcX, atINT nSrcY );
typedef atBOOL (atUI_FUNC_COORD_CONV_REAL)( atLPVOID lpParam, atREAL *lpDesX, atREAL *lpDesY, atREAL rSrcX, atREAL rSrcY );


atBOOL      atUIAPI_InitGraphic( atLONG lSysInst, atLONG lInitData1, atLONG lInitData2 );
atVOID      atUIAPI_FinalGraphic( NOARGS );


atCOLOR     atUIAPI_GetRGB(atUCHAR r, atUCHAR g, atUCHAR b);
atBYTE      atUIAPI_GetRValue(atCOLOR nColor);
atBYTE      atUIAPI_GetGValue(atCOLOR nColor);
atBYTE      atUIAPI_GetBValue(atCOLOR nColor);

atBOOL	  atUIAPI_SetScreenResolution( atINT nWidth, atINT nHeight );
atBOOL	  atUIAPI_GetScreenDPI( atINT *lpnX_DPI, atINT *lpnY_DPI );

atINT         atUIAPI_GetScreenWidth( NOARGS );
atINT         atUIAPI_GetScreenHeight( NOARGS );
atINT 		atUIAPI_GetScreenBPP( NOARGS );

atCOLOR atUIAPI_ConvColorFrom24BPP(atCOLOR nColor);
atCOLOR atUIAPI_ConvColorFrom24iBPP(atCOLOR nColor);
atCOLOR atUIAPI_ConvColorFrom16BPP(atCOLOR nColor);

atVOID		atUIAPI_SetScreenClipRect( atINT nStartX, atINT nStartY, atINT nEndX, atINT nEndY );
atVOID		atUIAPI_SetScreenClipRect2( atRECT *lpRect );
atRECT	*	atUIAPI_GetScreenClipRect( NOARGS );

atBOOL		atUIAPI_BeginDraw( NOARGS );
atVOID		atUIAPI_EndDraw( NOARGS );

atVOID		atUIAPI_FlushScreenRgn( atINT nX, atINT nY, atINT nWidth, atINT nHeight );
atVOID		atUIAPI_FlushScreen( NOARGS );
atVOID		atUIAPI_ClearScreen( atCOLOR nColor );
atVOID 		atUIAPI_FlushScreenEnable(atBOOL bEnable);

atHGDC		atUIAPI_CreateGDC( atINT nX, atINT nY, atINT nWidth, atINT nHeight, atINT bpp, atDWORD dwAttr );
atVOID        atUIAPI_DestroyGDC( atHGDC hCanvas );
atVOID  		atUIAPI_ActivateGDC( atHGDC hCanvas, atBOOL bActivate );
atLPVOID    atUIAPI_GetOEMGDC(atHGDC hCanvas);
atBOOL	atUIAPI_IsCanvasActivate(atHGDC hCanvas);

atVOID 		atUIAPI_GetCanvasAttr( atHGDC hCanvas, atUI_CANVAS_ATTR *lpCanvasAttr );

atVOID      atUIAPI_GetClipRect( atHGDC hCanvas, atRECT *lpRect );
atVOID	  atUIAPI_SetClipRect( atHGDC hCanvas, atRECT *lpRect );
atVOID        atUIAPI_GetGDCArea(atHGDC hCanvas, atREGION *rgnArea);
atBOOL   atUIAPI_SetGDCArea(atHGDC hCanvas, atREGION *rgnArea);
atVOID 	atUIAPI_CopyGDC( atHGDC hDesCanvas, atINT nDesX, atINT nDesY, atHGDC hSrcCanvas, atINT nX, atINT nY, atINT nWidth, atINT nHeight );

atVOID        atUIAPI_Cvs2Scr( atHGDC hCanvas, atINT *nX, atINT *nY );
atVOID        atUIAPI_Cvs2Scr2( atHGDC hCanvas, atINT *nX1, atINT *nY1, atINT *nX2, atINT *nY2 );
atHFONT     atUIAPI_SetFont( atHGDC hCanvas, atHFONT nFont );
atHFONT     atUIAPI_GetFont( atHGDC hCanvas );
atCOLOR     atUIAPI_SetFrColor( atHGDC hCanvas, atCOLOR color );
atCOLOR     atUIAPI_GetFrColor( atHGDC hCanvas );
atCOLOR     atUIAPI_SetBgColor( atHGDC hCanvas, atCOLOR color );
atCOLOR     atUIAPI_GetBgColor( atHGDC hCanvas );

atHGDC      atUIAPI_GetScreenCanvas( NOARGS );

atINT atUIAPI_DrawStringLine(atHGDC hCanvas, atHFONT hFont, atLPTSTR szText, atINT nLength, atRECT *lpRect, atCOLOR textColor );
atINT	atUIAPI_DrawString( atHGDC hCanvas, atINT nX, atINT nY, atLPTSTR str, atCOLOR color );
atINT	atUIAPI_DrawStringEx( atHGDC hCanvas, atINT nX, atINT nY, atLPTSTR str, atINT len, atCOLOR color, atINT nLineGap );
atINT	atUIAPI_DrawStringAlign( atHGDC hCanvas, atINT nX, atINT nY, atLPTSTR str, atINT align, atCOLOR color );
atINT	atUIAPI_DrawStringAlignRect( atHGDC hCanvas, atINT nX, atINT nY, atRECT *lpRect, atLPTSTR str, atINT align, atCOLOR color,  atPOINT *lpOutCoord );
atINT atUIAPI_DrawStringAlignRegion( atHGDC hCanvas, atINT nX, atINT nY, atREGION *lpRect, atLPTSTR str, atINT align, atCOLOR color,  atPOINT *lpOutCoord );
atINT	atUIAPI_DrawStringRotate( atHGDC hCanvas, atINT nX, atINT nY, atLPTSTR str, atINT len, atCOLOR color, atREAL rAngle );
atINT	atUIAPI_DrawStringRotateAlign( atHGDC hCanvas, atINT nX, atINT nY, atLPTSTR str, atINT len, atCOLOR color, atREAL rAngle, atINT nAlign);
atINT	atUIAPI_DrawStringRotateEx( atHGDC hCanvas, atINT nX, atINT nY, atLPTSTR str, atINT len, atCOLOR color, atCOLOR colorOut, atREAL rAngle, atINT nAlign, atINT nOL);

atHSTRBITMAP atUIAPI_CreateStrBitmap( atHFONT hFont, atLPTSTR szStrData, atINT nLen );
atHSTRBITMAP atUIAPI_CreateStrBitmapEx( atHFONT hFont, atLPTSTR szStrData, atINT nLen, atLPCALLBACKALLOC fnAllocator, atLPCALLBACKFREE fnDeallocator );
atVOID         atUIAPI_DestroyStrBitmap( atHSTRBITMAP hStrBitmap );
atVOID         atUIAPI_GetStrBitmapMetric( atHSTRBITMAP hStrBitmap, atSIZE *lpSize );
atVOID         atUIAPI_DrawStrBitmap( atHGDC hCanvas, atINT X, atINT Y, atHSTRBITMAP hStrBitmap, atINT iStartPos, atINT iWidth, atCOLOR Color );
atVOID         atUIAPI_DrawStrBitmapAlign( atHGDC hCanvas, atINT nX, atINT nY, atHSTRBITMAP hStrBitmap, atINT iStartPos, atINT iWidth, atINT align, atCOLOR Color );

atVOID        atUIAPI_FlushRgn( atHGDC hCanvas, atINT nX, atINT nY, atINT nWidth, atINT nHeight );
atVOID        atUIAPI_Flush( atHGDC hCanvas );
atCOLOR      atUIAPI_GetPixel( atHGDC hCanvas, atINT nX, atINT nY );
atVOID        atUIAPI_DrawPixel( atHGDC hCanvas, atINT nX, atINT nY, atCOLOR nColor );
atVOID        atUIAPI_FillRect( atHGDC hCanvas, atINT nX, atINT nY, atINT nWidth, atINT nHeight, atCOLOR nColor );
atVOID        atUIAPI_FillRectTrans( atHGDC hCanvas, atINT nX, atINT nY, atINT nWidth, atINT nHeight, atCOLOR nColor, atINT nAlpha );
atVOID        atUIAPI_FillRectXOR( atHGDC hCanvas, atINT nX, atINT nY, atINT nWidth, atINT nHeight );
atVOID        atUIAPI_DrawRect( atHGDC hCanvas, atINT nX, atINT nY, atINT nWidth, atINT nHeight, atCOLOR nColor, atINT nLineWidth );
atVOID        atUIAPI_Clear( atHGDC hCanvas, atCOLOR nColor );
atVOID        atUIAPI_DrawLine( atHGDC hCanvas, atINT nX1, atINT nY1, atINT nX2, atINT nY2, atCOLOR nColor, atINT nLineWidth, atBOOL IsAA );
atVOID		atUIAPI_DrawCircle( atHGDC hCanvas, atINT nX, atINT nY, atINT nRadius, atCOLOR nColor, atINT nLineWidth );
atVOID		atUIAPI_FillCircle( atHGDC hCanvas, atINT nX, atINT nY, atINT nRadius, atCOLOR nColor );
atVOID		atUIAPI_FillCircleTrans( atHGDC hCanvas, atINT nX, atINT nY, atINT nRadius, atCOLOR nColor, atINT nAlpha );
atVOID		atUIAPI_DrawEllipse( atHGDC hCanvas, atINT nX, atINT nY, atINT nRadiusX, atINT nRadiusY, atCOLOR nColor, atINT nLineWidth );
atVOID		atUIAPI_FillEllipse( atHGDC hCanvas, atINT nX, atINT nY, atINT nRadiusX, atINT nRadiusY, atCOLOR nColor );
atVOID		atUIAPI_FillEllipseTrans( atHGDC hCanvas, atINT nX, atINT nY, atINT nRadiusX, atINT nRadiusY, atCOLOR nColor, atINT nAlpha );
atVOID		atUIAPI_DrawPolyLine( atHGDC hCanvas, atPOINT lpPtrList[], atINT nCount, atBOOL bClosed, atCOLOR Color, atINT nWidth, atBOOL IsAA );
atVOID		atUIAPI_DrawPolyLineEx( atHGDC hCanvas, atPOINT lpPtrList[], atINT nCount, atBOOL bClosed, atCOLOR Color, atINT nWidth, atBOOL IsAA, atUI_FUNC_COORD_CONV_REAL *lpFunc, atLPVOID lpParam );
atVOID		atUIAPI_FillPolygon( atHGDC hCanvas, atPOINT lpPtrList[], atINT nCount, atCOLOR Color, atBOOL IsAA );
atVOID		atUIAPI_FillPolygonTrans( atHGDC hCanvas, atPOINT lpPtrList[], atINT nCount, atCOLOR Color, atBOOL IsAA, atINT nAlpha );
atVOID		atUIAPI_FillPolygonBitmap( atHGDC hCanvas, atPOINT lpPtrList[], atINT nCount, atHBITMAP hBitmap );
atVOID		atUIAPI_DrawPolyPolyLine( atHGDC hCanvas, atPOINT lpPtrList[], atINT nPartCount[], atINT nPolyCount, atBOOL bClosed, atCOLOR Color, atINT nWidth, atBOOL IsAA );
atVOID		atUIAPI_DrawPolyPolyLineEx( atHGDC hCanvas, atPOINT lpPtrList[], atINT nPartCount[], atINT nPolyCount, atBOOL bClosed, atCOLOR Color, atINT nWidth, atBOOL IsAA, atUI_FUNC_COORD_CONV_REAL *lpFunc, atLPVOID lpParam );
atVOID		atUIAPI_FillPolyPolygon( atHGDC hCanvas, atPOINT lpPtrList[], atINT nPartCount[], atINT nPolyCount, atCOLOR Color, atBOOL IsAA );
atVOID    	atUIAPI_FillPolyPolygonEx( atHGDC hCanvas, atPOINT lpPtrList[], atINT nPartCount[], atINT nPolyCount, atCOLOR Color, atBOOL IsAA, atUI_FUNC_COORD_CONV *lpFunc, atLPVOID lpParam );
atVOID		atUIAPI_FillPolyPolygonTrans( atHGDC hCanvas, atPOINT lpPtrList[], atINT nPartCount[], atINT nPolyCount, atCOLOR Color, atBOOL IsAA, atINT nAlpha );
atVOID		atUIAPI_FillPolyPolygonBitmap( atHGDC hCanvas, atPOINT lpPtrList[], atINT nPartCount[], atINT nPolyCount, atHBITMAP hBitmap );
atVOID		atUIAPI_FillPolyPolygonBitmapEx( atHGDC hCanvas, atPOINT lpPtrList[], atINT nPartCount[], atINT nPolyCount, atHBITMAP hBitmap, atUI_FUNC_COORD_CONV *lpFunc, atLPVOID lpParam );


//typedef atVOID * OEM_IMAGE;

atHBITMAP   atUIAPI_CreateBlankBitmap( atINT nWidth, atINT nHeight, atINT nBPP );
atHBITMAP   atUIAPI_CreateBlankBitmapEx( atINT nWidth, atINT nHeight, atINT nBPP, atLPCALLBACKALLOC fnAllocator, atLPCALLBACKFREE fnDeallocator );
atHBITMAP atUIAPI_CreateBitmapFromStream( atLPBYTE lpSrcData, atINT size );
atHBITMAP atUIAPI_CreateBitmapFromStreamEx( atLPBYTE lpSrcData, atINT size, atLPCALLBACKALLOC fnAllocator, atLPCALLBACKFREE fnDeallocator );
atHBITMAP   atUIAPI_CreateBitmapFromRes( atHRESSRC hResSrc, atLPTSTR FileName );
atHBITMAP   atUIAPI_CreateBitmapFromFile( atLPTSTR FileName );
atHBITMAP   atUIAPI_CreateBitmapFromFileEx( atLPTSTR FileName, atLPCALLBACKALLOC fnAllocator, atLPCALLBACKFREE fnDeallocator );
atHBITMAP	atUIAPI_CreateBitmapFromYUV422Stream( atBYTE* yuv, atINT width, atINT height, atINT mem_width);
atHBITMAP	atUIAPI_CreateBitmapFromYUV422StreamEx( atBYTE* yuv, atINT width, atINT height, atINT mem_width, atLPCALLBACKALLOC fnAllocator, atLPCALLBACKFREE fnDeallocator );

atVOID        atUIAPI_DestroyBitmap( atHBITMAP lpBM );

atLPVOID	   atUIAPI_GetBitmapBits( atHBITMAP lpBM );
atINT         atUIAPI_GetBitmapBPP( atHBITMAP lpBM );
atINT         atUIAPI_GetBitmapWidth( atHBITMAP lpBM );
atINT         atUIAPI_GetBitmapHeight( atHBITMAP lpBM );
atINT         atUIAPI_GetBitmapWidthBytes( atHBITMAP lpBM );
atLPVOID   atUIAPI_GetOEMBitmap( atHBITMAP lpBM );

atVOID      atUIAPI_GetBitmapLogPos( atHBITMAP lpBM, atINT *pLogX, atINT *pLogY );
atVOID      atUIAPI_SetBitmapLogPos( atHBITMAP lpBM, atINT nLogX, atINT nLogY );
atVOID      atUIAPI_SetBitmapLogPosCenter( atHBITMAP lpBM );

/*
atHBITMAPMASK atUIAPI_CreateBitmapMask( atLPBYTE lpSrcData, atINT size );
atHBITMAPMASK atUIAPI_CreateBitmapMaskEx( atLPBYTE lpSrcData, atINT size );
atHBITMAPMASK atUIAPI_CreateBitmapMaskFromRes( atLPTSTR FileName );
atHBITMAPMASK atUIAPI_CreateBitmapMaskFromFile( atLPTSTR FileName );
atHBITMAPMASK atUIAPI_CreateBitmapMaskFromFileEx( atLPTSTR FileName );
atVOID 		atUIAPI_DestroyBitmapMask(atHBITMAPMASK lpBMmsk);
*/

atVOID 		atUIAPI_SetBitmapTransColor( atHBITMAP lpBM, atCOLOR clTransColor );
atVOID 		atUIAPI_ClearBitmapTransColor( atHBITMAP lpBM );

//atVOID   	atUIAPI_SetBitmapMask( atHBITMAP lpBM, atHBITMAPMASK lpBMmsk, atBOOL bOwnMask );

atVOID        atUIAPI_GetImage( atHGDC hCanvas, atINT nX, atINT nY, atHBITMAP lpBM );
atVOID        atUIAPI_DrawImage( atHGDC hCanvas, atINT nX, atINT nY, atHBITMAP lpBM );
atVOID        atUIAPI_DrawImagePart( atHGDC hCanvas, atINT nX, atINT nY, atHBITMAP lpBM, atINT nImgX, atINT nImgY, atINT nWidth, atINT nHeight );
atVOID        atUIAPI_DrawImageStretch( atHGDC hCanvas, atINT nX, atINT nY, atHBITMAP lpBM, atINT nWidth, atINT nHeight );
atVOID        atUIAPI_DrawImageAlign(atHGDC hCanvas, atINT nX, atINT nY, atHBITMAP lpBM, atINT nAlign);
atVOID        atUIAPI_DrawImageAlignPart(atHGDC hCanvas, atINT nX, atINT nY, atHBITMAP lpBM, atINT nImgX, atINT nImgY, atINT nImgWidth, atINT nImgHeight, atINT nAlign);
atVOID        atUIAPI_DrawImageRotate(atHGDC hCanvas, atINT nX, atINT nY, atHBITMAP lpBM, atREAL rAngle );
	
/*
atVOID		atUIAPI_DrawImageWithAlphaChannel( atHGDC hCanvas, atINT nX, atINT nY, atHBITMAP lpBM, atHBITMAPMASK lpBMmsk );
atVOID		atUIAPI_DrawImageStretchWithAlphaChannel( atHGDC hCanvas, atINT nX, atINT nY, atHBITMAP lpBM, atINT nWidth, atINT nHeight, atHBITMAPMASK lpBMmsk );
atVOID		atUIAPI_DrawImagePartWithAlphaChannel( atHGDC hCanvas, atINT nX, atINT nY, atHBITMAP lpBM, atINT nImgX, atINT nImgY, atINT nWidth, atINT nHeight, atHBITMAPMASK lpBMmsk );
*/

atLONG	    atUIAPI_ExtGraphicMethod( atHGDC hCanvas, atINT func_no, atLONG param1, atLONG param2, atLONG param3, atLONG param4, atLONG param5, atLONG param6 );


#ifdef __cplusplus
}
#endif


#endif // __VD_GDI_H_
