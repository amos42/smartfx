/*#############################################################################
-------------------------------------------------------------------------------

                                  Pixel_PB.c
                           (* =================== *)


 Version           : 1.0.0
 Dependent Modules :
 Author            : Ju, Gyeong-min ( Amos )
 Create            : 2001/04/25    01:47:09am
 Update            : 2003/04/29    00:00:00am
 C Compiler        : ADS 1.01, SDT 2.51, Borland C/C++ 5.0, Visual C/C++ 6.0
 Assembler         : ADS 1.01, SDT 2.51, Borland TASM 3.1, MASM 6.0
 Description       : PutPixel related function definition

-------------------------------------------------------------------------------
#############################################################################*/
#include "GrpX.h"


/***************************
  펜을 이용해 점을 찍는다.
****************************/
void atGRPX_DrawPixelWithPen( atGRPX_CANVAS *lpCanvas, atINT X, atINT Y, atGRPX_PEN *lpPen )
{
    atGRPX_PIXEL *ptr, cl;
    
    switch( lpPen->nPenType ){
        case atGRPX_PENTYPE_SOLID  :
            if( lpPen->nAlpha == 0x00 ){
                // Nothing
            } else if( lpPen->nAlpha == 0xFF ){
                atGRPX_DrawPixel( lpCanvas, X, Y, lpPen->clColor );
            } else {
                atGRPX_DrawPixelTrans( lpCanvas, X, Y, lpPen->clColor, lpPen->nAlpha );
            }
            break;
        case atGRPX_PENTYPE_BITMAP :
            ptr = (atGRPX_PIXEL *)atGRPX_GetBitmapPtrXY( lpPen->lpBitmap, 0, 0 );
            cl = *ptr;
            if( lpPen->nAlpha == 0x00 ){
                // Nothing
            } else if( lpPen->nAlpha == 0xFF ){
                atGRPX_DrawPixel( lpCanvas, X, Y, cl );
            } else {
                atGRPX_DrawPixelTrans( lpCanvas, X, Y, cl, lpPen->nAlpha );
            }
            break;
    }
}


/***************************
  브러쉬를 이용해 점을 찍는다.
****************************/
void atGRPX_DrawPixelWithBrush( atGRPX_CANVAS *lpCanvas, atINT X, atINT Y, atGRPX_BRUSH *lpBrush )
{
    atGRPX_PIXEL *ptr, cl;
    
    switch( lpBrush->nBrushType ){
        case atGRPX_BRUSHTYPE_SOLID  :
            if( lpBrush->nAlpha == 0x00 ){
                // Nothing
            } else if( lpBrush->nAlpha == 0xFF ){
                atGRPX_DrawPixel( lpCanvas, X, Y, lpBrush->clColor );
            } else {
                atGRPX_DrawPixelTrans( lpCanvas, X, Y, lpBrush->clColor, lpBrush->nAlpha );
            }
            break;
        case atGRPX_BRUSHTYPE_BITMAP :
            ptr = atGRPX_GetBitmapPtrXY( lpBrush->lpBitmap, 0, 0 );
            cl = *ptr;
            if( lpBrush->nAlpha == 0x00 ){
                // Nothing
            } else if( lpBrush->nAlpha == 0xFF ){
                atGRPX_DrawPixel( lpCanvas, X, Y, cl );
            } else {
                atGRPX_DrawPixelTrans( lpCanvas, X, Y, cl, lpBrush->nAlpha );
            }
            break;
    }
}

