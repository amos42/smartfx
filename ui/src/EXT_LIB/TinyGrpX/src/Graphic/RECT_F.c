/*#############################################################################
-------------------------------------------------------------------------------

                                  Rect_f.c
                           (* =================== *)


 Version           : 1.0.0
 Dependent Modules :
 Author            : Ju, Gyeong-min ( Amos )
 Create            : 2001/04/25    01:47:09am
 Update            : 2003/04/29    00:00:00am
 C Compiler        : ADS 1.01, SDT 2.51, Borland C/C++ 5.0, Visual C/C++ 6.0
 Assembler         : ADS 1.01, SDT 2.51, Borland TASM 3.1, MASM 6.0
 Description       : Rec_f related function definition

-------------------------------------------------------------------------------
#############################################################################*/
//#define __X86_

#include "AT_Types.h"
#include "TinyGrpX.h"


/*********************************

**********************************/
void atTGRPX_FillRect( atTGRPX_CANVAS *lpCanvas, atINT StartX, atINT StartY, atINT EndX, atINT EndY, atTGRPX_COLOR Color )
{
    int i, j;
    int width, height, skip;
    atTGRPX_PIXEL *p, pixel;
#if atTGRPX_PIXEL_BPP == 8
    long c;
#endif
#if 0
	int w1, w2;
#endif
    atRECT *clip;


//    if( Color == TRANS_COLOR ) return;

    if( StartX > EndX ){
      SWAP( StartX, EndX, i );
    }
    if( StartY > EndY ){
      SWAP( StartY, EndY, i );
    }

    clip = atTGRPX_GetClipRect(lpCanvas);

    if( (StartX > clip->nEndX) || (EndX < clip->nStartX) ||
        (StartY > clip->nEndY) || (EndY < clip->nStartY) )
      return;

    if( StartX < clip->nStartX ) StartX = clip->nStartX;
    if( EndX   > clip->nEndX   ) EndX   = clip->nEndX;
    if( StartY < clip->nStartY ) StartY = clip->nStartY;
    if( EndY   > clip->nEndY   ) EndY   = clip->nEndY;

    width  = EndX - StartX + 1;
    height = EndY - StartY + 1;

#if atTGRPX_PIXEL_BPP == 8
    c = (long)Color;
    c |= c << 8;
    c |= c << 16;
#endif

    p = atTGRPX_GetFrameBufferPtrXY( lpCanvas, StartX, StartY );
	if(!p) return;

    skip = atTGRPX_GetVMemWidth(lpCanvas) - width;

#if 0

#ifdef __X86_
    w2 = width & 0x03;
    width >>= 2;

    for( i = 0; i < height; i ++ ){
      for( j = 0; j < width; j ++ ){
        *((long *)p) = c;
        p += sizeof(long);
      }

      for( j = 0; j < w2; j ++ )
        *p++ = (atTGRPX_PIXEL)c;

      p += skip;
    }
#else
    w1 = (long)p & 0x3;
    if( w1 ){
      w1 = sizeof(long) - w1;
      w1 = ( w1 < width ) ? w1 : width;
      width -= w1;
    }

    w2 = width & 0x3;
    width >>= 2;

    for( i = 0; i < height; i ++ ){
      for( j = 0; j < w1; j ++ )
        *p++ = (atTGRPX_PIXEL)c;

      for( j = 0; j < width; j ++ ){
        *(long *)p = c;
        p += sizeof(long);
      }

      for( j = 0; j < w2; j ++ )
        *p++ = (atTGRPX_PIXEL)c;

      p += skip;
    }
#endif    

#endif

    pixel = COLOR_TO_PIXEL(Color);
    
    for( i = 0; i < height; i ++ ){
      for( j = 0; j < width; j ++ ){
        *p++ = pixel;
//atTGRPX_DrawPixelTrans( lpCanvas, StartX+j, StartY+i, Color, 128);
      }

      p += skip;
    }
}

