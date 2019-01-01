/*#############################################################################
-------------------------------------------------------------------------------

                                  Line_V.c
                           (* =================== *)


 Version           : 1.0.0
 Dependent Modules :
 Author            : Ju, Gyeong-min ( Amos )
 Create            : 2001/04/25    01:47:09am
 Update            : 2003/04/29    00:00:00am
 C Compiler        : ADS 1.01, SDT 2.51, Borland C/C++ 5.0, Visual C/C++ 6.0
 Assembler         : ADS 1.01, SDT 2.51, Borland TASM 3.1, MASM 6.0
 Description       : Vertical line function definition

-------------------------------------------------------------------------------
#############################################################################*/
#include "TinyGrpX.h"


/*******************
  수직선을 그린다.
********************/
void atTGRPX_DrawVertLine( atTGRPX_CANVAS *lpCanvas, atINT StartX, atINT StartY, atINT Length, atTGRPX_COLOR Color )
{
    int t;
    atTGRPX_PIXEL *p;
    atRECT *clip;
	int inc;
	atTGRPX_PIXEL cl = (atTGRPX_PIXEL)Color;

    if( Length <= 0 ) return;

    clip = atTGRPX_GetClipRect(lpCanvas);

    if( (StartX < clip->nStartX) || (StartX > clip->nEndX) ) return;
    if( StartY > clip->nEndY ) return;
    t = StartY + Length - 1;
    if( t < clip->nStartY ) return;

    if( t > clip->nEndY ){
      Length -= t - clip->nEndY;
    }
    if( StartY < clip->nStartY ){
      Length -= clip->nStartY - StartY;
      StartY = clip->nStartY;
    }
    
    p = atTGRPX_GetFrameBufferPtrXY( lpCanvas, StartX, StartY );

	inc = atTGRPX_GetVMemWidth(lpCanvas);

    while( Length-- > 0 ){
      *p = cl;
      p += inc;
    }
}
