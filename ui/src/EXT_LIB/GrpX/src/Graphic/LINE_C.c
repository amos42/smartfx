/*#############################################################################
-------------------------------------------------------------------------------

                                  LINE_C.c
                           (* =================== *)


 Version           : 1.0.0
 Dependent Modules :
 Author            : Ju, Gyeong-min ( Amos )
 Create            : 2001/04/25    01:47:09am
 Update            : 2003/04/29    00:00:00am
 C Compiler        : ADS 1.01, SDT 2.51, Borland C/C++ 5.0, Visual C/C++ 6.0
 Assembler         : ADS 1.01, SDT 2.51, Borland TASM 3.1, MASM 6.0
 Description       : Cross line function definition

-------------------------------------------------------------------------------
#############################################################################*/
#include "GrpX.h"


/***********************
  대각선을 그린다.
************************/
void atGRPX_DrawCrossLine( atGRPX_CANVAS *lpCanvas, atINT StartX, atINT StartY, atINT Length, atGRPX_COLOR Color )
{
    int t;
    atGRPX_PIXEL *p, pixel;
    atRECT *clip;
	int inc;

//    if( Color == TRANS_COLOR ) return;

    clip = atGRPX_GetClipRect(lpCanvas);

    if( StartX > clip->nEndX || StartY > clip->nEndY ) return;

    if( StartX < clip->nStartX ){
      Length -= clip->nStartX - StartX;
      StartY += clip->nStartX - StartX;
      StartX = clip->nStartX;
    }
    if( StartY < clip->nStartY ){
      Length -= clip->nStartY - StartY;
      StartX += clip->nStartY - StartY;
      StartY = clip->nStartY;
    }

    t = StartX + Length - 1;
    if( t < clip->nStartX ) return;
    if( t > clip->nEndX ){
      Length -= t - clip->nEndX;
    }
    t = StartY + Length - 1;
    if( t < clip->nStartY ) return;
    if( t > clip->nEndY ){
      Length -= t - clip->nEndY;
    }

    p = atGRPX_GetFrameBufferPtrXY( lpCanvas, StartX, StartY );
	inc = atGRPX_GetVMemWidth(lpCanvas) + 1;

    pixel = COLOR_TO_PIXEL(Color);
    while( Length -- ){
      *p = pixel;
      p += inc;
    }
}

