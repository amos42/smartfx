/*#############################################################################
-------------------------------------------------------------------------------

                                  Rect.c
                           (* =================== *)


 Version           : 1.0.0
 Dependent Modules :
 Author            : Ju, Gyeong-min ( Amos )
 Create            : 2001/04/25    01:47:09am
 Update            : 2003/04/29    00:00:00am
 C Compiler        : ADS 1.01, SDT 2.51, Borland C/C++ 5.0, Visual C/C++ 6.0
 Assembler         : ADS 1.01, SDT 2.51, Borland TASM 3.1, MASM 6.0
 Description       : Rect function definition

-------------------------------------------------------------------------------
#############################################################################*/
#include "GrpX.h"



/***************************
  펜을 이용해 사각형을 그린다.
****************************/
void atGRPX_DrawRectWithPen( atGRPX_CANVAS *lpCanvas, atINT StartX, atINT StartY, atINT EndX, atINT EndY, atGRPX_PEN *lpPen )
{
    atGRPX_PIXEL *ptr, cl;
    
    switch( lpPen->nPenType ){
        case atGRPX_PENTYPE_SOLID  :
            if( lpPen->nAlpha == 0x00 ){
                // Nothing
            } else if( lpPen->nAlpha == 0xFF ){
                atGRPX_DrawRectWidth( lpCanvas, StartX, StartY, EndX, EndY, lpPen->clColor, lpPen->nPenWidth );
            } else {
                // 미구현...
            }
            break;
        case atGRPX_PENTYPE_BITMAP :
            ptr = (atGRPX_PIXEL *)atGRPX_GetBitmapPtrXY( lpPen->lpBitmap, 0, 0 );
            cl = *ptr;
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
  브러쉬를 이용해 사각형을 칠한다.
****************************/
void atGRPX_FillRectWithBrush( atGRPX_CANVAS *lpCanvas, atINT StartX, atINT StartY, atINT EndX, atINT EndY, atGRPX_BRUSH *lpBrush )
{
    switch( lpBrush->nBrushType ){
        case atGRPX_BRUSHTYPE_SOLID  :
            if( lpBrush->nAlpha == 0x00 ){
                // Nothing
            } else if( lpBrush->nAlpha == 0xFF ){
                atGRPX_FillRect( lpCanvas, StartX, StartY, EndX, EndY, lpBrush->clColor );
            } else {
                atGRPX_FillRectTrans( lpCanvas, StartX, StartY, EndX, EndY, lpBrush->clColor, lpBrush->nAlpha );
            }
            break;
        case atGRPX_BRUSHTYPE_BITMAP :
            if( lpBrush->nAlpha == 0x00 ){
                // Nothing
            } else if( lpBrush->nAlpha == 0xFF ){
                atGRPX_DrawTextureRect( lpCanvas, StartX, StartY, EndX, EndY, lpBrush->lpBitmap );
            } else {
                // 미구현
            }
            break;
    }
}
