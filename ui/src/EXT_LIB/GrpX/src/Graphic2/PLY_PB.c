/*###########################################################################
-----------------------------------------------------------------------------

                                PLY_F.c
                         (* =================== *)


 Version           : 1.0.0
 Dependent Modules :
 Author            : Ju, Gyeong-min ( Amos )
 Create            : 2001/04/25    01:47:09am
 Update            : 2003/04/29    00:00:00am
 C Compiler        : ADS 1.01, SDT 2.51, Borland C/C++ 5.0, Visual C/C++ 6.0
 Assembler         : ADS 1.01, SDT 2.51, Borland TASM 3.1, MASM 6.0
 Dependent Modules :
 Description       : TRI related function definition

-----------------------------------------------------------------------------
###########################################################################*/
#include "GrpX.h"


/***************************
  펜을 이용해 폴리라인을 그린다.
****************************/
void atGRPX_DrawPolyLineWithPen( atGRPX_CANVAS *lpCanvas, atPOINT lpPtrList[], atINT nCount, atBOOL bClosed, atGRPX_PEN *lpPen )
{
    switch( lpPen->nPenType ){
        case atGRPX_PENTYPE_SOLID  :
            if( lpPen->nAlpha == 0x00 ){
                // Nothing
            } else if( lpPen->nAlpha == 0xFF ){
                if( lpPen->bAntiAliasing )
                    atGRPX_DrawPolyLineWidthAA( lpCanvas, lpPtrList, nCount, bClosed, lpPen->clColor, lpPen->nPenWidth );
                else
                    atGRPX_DrawPolyLineWidth( lpCanvas, lpPtrList, nCount, bClosed, lpPen->clColor, lpPen->nPenWidth );
            } else {
                // 미구현...
            }
            break;
        case atGRPX_PENTYPE_BITMAP :
            if( lpPen->nAlpha == 0x00 ){
                // Nothing
            } else if( lpPen->nAlpha == 0xFF ){
                // 미구현
            } else {
                // 미구현
            }
            break;
    }
}


/***************************
  브러쉬를 이용해 폴리곤을 칠한다.
****************************/
void atGRPX_FillPolygonWithBrush( atGRPX_CANVAS *lpCanvas, atPOINT lpPtrList[], atINT nCount, atGRPX_BRUSH *lpBrush )
{
    switch( lpBrush->nBrushType ){
        case atGRPX_BRUSHTYPE_SOLID  :
            if( lpBrush->nAlpha == 0x00 ){
                // Nothing
            } else if( lpBrush->nAlpha == 0xFF ){
                if( lpBrush->bAntiAliasing )
                    atGRPX_FillPolygonAA( lpCanvas, lpPtrList, nCount, lpBrush->clColor );
                else
                    atGRPX_FillPolygon( lpCanvas, lpPtrList, nCount, lpBrush->clColor );
            } else {
                // 미구현
            }
            break;
        case atGRPX_BRUSHTYPE_BITMAP :
            if( lpBrush->nAlpha == 0x00 ){
                // Nothing
            } else if( lpBrush->nAlpha == 0xFF ){
                atGRPX_FillPolygonBitmap( lpCanvas, lpPtrList, nCount, lpBrush->lpBitmap );
            } else {
                // 미구현
            }
            break;
    }
}

