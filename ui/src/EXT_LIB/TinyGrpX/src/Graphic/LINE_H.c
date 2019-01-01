/*#############################################################################
-------------------------------------------------------------------------------

                                  LINE_H.c
                           (* =================== *)


 Version           : 1.0.0
 Dependent Modules :
 Author            : Ju, Gyeong-min ( Amos )
 Create            : 2001/04/25    01:47:09am
 Update            : 2003/04/29    00:00:00am
 C Compiler        : ADS 1.01, SDT 2.51, Borland C/C++ 5.0, Visual C/C++ 6.0
 Assembler         : ADS 1.01, SDT 2.51, Borland TASM 3.1, MASM 6.0
 Description       : H Line function definition

-------------------------------------------------------------------------------
#############################################################################*/
//#define __X86_

#include "TinyGrpX.h"


/***********************
  수평선을 그린다.
************************/
void atTGRPX_DrawHorizLine( atTGRPX_CANVAS *lpCanvas, atINT StartX, atINT StartY, atINT Length, atTGRPX_COLOR Color )
{
    int t;
    atTGRPX_PIXEL *p;
    atRECT *clip;
	atTGRPX_PIXEL cl = COLOR_TO_PIXEL(Color);

    if( Length <= 0 ) return;

    clip = atTGRPX_GetClipRect(lpCanvas);

    if( (StartY < clip->nStartY) || (StartY > clip->nEndY) ) return;

    if( StartX > clip->nEndX ) return;
    if( StartX < clip->nStartX ){
      Length -= clip->nStartX - StartX;
      StartX = clip->nStartX;
    }
    t = StartX + Length - 1;
    if( t < clip->nStartX ) return;
    if( t > clip->nEndX ){
      Length -= t - clip->nEndX;
    }

    p = atTGRPX_GetFrameBufferPtrXY( lpCanvas, StartX, StartY );

	while(Length-- > 0)
   		*p++ = cl;
}
