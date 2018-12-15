/*###########################################################################
-----------------------------------------------------------------------------

                                PPLY_FAA.c
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
void atGRPX_DrawPolyPolyLineWithPen( atGRPX_CANVAS *lpCanvas, atPOINT lpPtrList[], atINT nPartCount[], atINT nPolyCount, atBOOL bClosed, atGRPX_PEN *lpPen )
{
    switch( lpPen->nPenType ){
        case atGRPX_PENTYPE_SOLID  :
            if( lpPen->nAlpha == 0x00 ){
                // Nothing
            } else if( lpPen->nAlpha == 0xFF ){
                if( lpPen->bAntiAliasing )
                    atGRPX_DrawPolyPolyLineWidthAA( lpCanvas, lpPtrList, nPartCount, nPolyCount, bClosed, lpPen->clColor, lpPen->nPenWidth );
                else
                    atGRPX_DrawPolyPolyLineWidth( lpCanvas, lpPtrList, nPartCount, nPolyCount, bClosed, lpPen->clColor, lpPen->nPenWidth );
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
void atGRPX_FillPolyPolygonWithBrush( atGRPX_CANVAS *lpCanvas, atPOINT lpPtrList[], atINT nPartCount[], atINT nPolyCount, atGRPX_BRUSH *lpBrush )
{
    switch( lpBrush->nBrushType ){
        case atGRPX_BRUSHTYPE_SOLID  :
            if( lpBrush->nAlpha == 0x00 ){
                // Nothing
            } else if( lpBrush->nAlpha == 0xFF ){
                if( lpBrush->bAntiAliasing )
                    atGRPX_FillPolyPolygonAA( lpCanvas, lpPtrList, nPartCount, nPolyCount, lpBrush->clColor );
                else
                    atGRPX_FillPolyPolygon( lpCanvas, lpPtrList, nPartCount, nPolyCount, lpBrush->clColor );
            } else {
                // 미구현
            }
            break;
        case atGRPX_BRUSHTYPE_BITMAP :
            if( lpBrush->nAlpha == 0x00 ){
                // Nothing
            } else if( lpBrush->nAlpha == 0xFF ){
                atGRPX_FillPolyPolygonBitmap( lpCanvas, lpPtrList, nPartCount, nPolyCount, lpBrush->lpBitmap );
            } else {
                // 미구현
            }
            break;
    }
}

