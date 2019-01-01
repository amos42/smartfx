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
#include "TinyGrpX.h"


/*********************************
  
**********************************/
void atTGRPX_DrawRectTrans( atTGRPX_CANVAS *lpCanvas, atINT StartX, atINT StartY, atINT EndX, atINT EndY, atTGRPX_COLOR Color, atINT nAlpha )
{
    int t;

    if( StartX > EndX ) SWAP( StartX, EndX, t );
    if( StartY > EndY ) SWAP( StartY, EndY, t );

    atTGRPX_DrawHorizLineTrans( lpCanvas, StartX, StartY, EndX - StartX + 1, Color, nAlpha );
    atTGRPX_DrawVertLineTrans(  lpCanvas, StartX, StartY, EndY - StartY,     Color, nAlpha );
    atTGRPX_DrawVertLineTrans(  lpCanvas, EndX,   StartY, EndY - StartY,     Color, nAlpha );
    atTGRPX_DrawHorizLineTrans( lpCanvas, StartX, EndY,   EndX - StartX + 1, Color, nAlpha );
}
