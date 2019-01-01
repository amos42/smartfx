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
void atTGRPX_DrawRectWidth( atTGRPX_CANVAS *lpCanvas, atINT StartX, atINT StartY, atINT EndX, atINT EndY, atTGRPX_COLOR Color, atINT nWidth )
{
    int t, hw, hw2;

    if( nWidth <= 0 ) return;
    if( nWidth == 1 ){
        atTGRPX_DrawRect( lpCanvas, StartX, StartY, EndX, EndY, Color );
        return;
    }
    
    if( StartX > EndX ) SWAP( StartX, EndX, t );
    if( StartY > EndY ) SWAP( StartY, EndY, t );

    hw = nWidth >> 1;
    hw2 = nWidth - hw - 1;
    atTGRPX_FillRect( lpCanvas, StartX-hw,  StartY-hw,  EndX+hw,    StartY+hw2,	Color );
    atTGRPX_FillRect( lpCanvas, StartX-hw,  StartY+hw2, StartX+hw2, EndY+hw,     Color );
    atTGRPX_FillRect( lpCanvas, EndX-hw2,   StartY+hw2, EndX+hw,    EndY+hw,     Color );
    atTGRPX_FillRect( lpCanvas, StartX+hw2, EndY-hw2,   EndX-hw2,   EndY+hw,     Color );
}

