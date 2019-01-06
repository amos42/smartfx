/**
  @file GDC.h
  @brief GDI(Graphic Draw Interface)에 관한 API
  @remark GDI(Graphic Draw Interface)에 관한 API
  @author Amos
  @date 2010-04-19 오전 11:41:00
  @see GDC.c
*/
#ifndef __GDC_H_
#define __GDC_H_


#include "UI_Types.h"


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


atINT         atUIAPI_GetScreenWidth( NOARGS );
atINT         atUIAPI_GetScreenHeight( NOARGS );

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

atHGDC      atUIAPI_GetScreenCanvas( NOARGS );

atVOID        atUIAPI_FlushRgn( atHGDC hCanvas, atINT nX, atINT nY, atINT nWidth, atINT nHeight );
atVOID        atUIAPI_Flush( atHGDC hCanvas );



#ifdef __cplusplus
}
#endif


#endif // __GDC_H_
