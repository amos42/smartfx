/*###########################################################################
-----------------------------------------------------------------------------

                                rTRI_F.h
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
  펜을 이용해 삼각형을 그린다.
****************************/
void atGRPX_DrawTriangleWithPen( atGRPX_CANVAS *lpCanvas, atINT X1, atINT Y1, atINT X2, atINT Y2, atINT X3, atINT Y3, atGRPX_PEN *lpPen )
{
    switch( lpPen->nPenType ){
        case atGRPX_PENTYPE_SOLID  :
            if( lpPen->nAlpha == 0x00 ){
                // Nothing
            } else if( lpPen->nAlpha == 0xFF ){
                if( lpPen->bAntiAliasing )
                    atGRPX_DrawTriangleAA( lpCanvas, X1, Y1, X2, Y2, X3, Y3, lpPen->clColor );
                else
                    atGRPX_DrawTriangle( lpCanvas, X1, Y1, X2, Y2, X3, Y3, lpPen->clColor );
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
  브러쉬를 이용해 삼각형을 칠한다.
****************************/
void atGRPX_FillTriangleWithBrush( atGRPX_CANVAS *lpCanvas, atINT X1, atINT Y1, atINT X2, atINT Y2, atINT X3, atINT Y3, atGRPX_BRUSH *lpBrush )
{
    switch( lpBrush->nBrushType ){
        case atGRPX_BRUSHTYPE_SOLID  :
            if( lpBrush->nAlpha == 0x00 ){
                // Nothing
            } else if( lpBrush->nAlpha == 0xFF ){
                if( lpBrush->bAntiAliasing )
                    atGRPX_FillTriangleAA( lpCanvas, X1, Y1, X2, Y2, X3, Y3, lpBrush->clColor );
                else
                    atGRPX_FillTriangle( lpCanvas, X1, Y1, X2, Y2, X3, Y3, lpBrush->clColor );
            } else {
                // 미구현
            }
            break;
        case atGRPX_BRUSHTYPE_BITMAP :
            if( lpBrush->nAlpha == 0x00 ){
                // Nothing
            } else if( lpBrush->nAlpha == 0xFF ){
                // 미구현
            } else {
                // 미구현
            }
            break;
    }
}
