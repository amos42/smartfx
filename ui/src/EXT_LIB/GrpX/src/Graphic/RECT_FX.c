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
#include "GrpX.h"


/*********************************

**********************************/
atVOID atGRPX_FillRectXOR( atGRPX_CANVAS *lpCanvas, atINT StartX, atINT StartY, atINT EndX, atINT EndY )
{
    int i, j;
    int width, height, skip;
    atGRPX_PIXEL *p;
	atGRPX_PIXEL cl;
    atRECT *clip;


//    if( Color == TRANS_COLOR ) return;

    if( StartX > EndX ){
      SWAP( StartX, EndX, i );
    }
    if( StartY > EndY ){
      SWAP( StartY, EndY, i );
    }

    clip = atGRPX_GetClipRect(lpCanvas);

    if( (StartX > clip->nEndX) || (EndX < clip->nStartX) ||
        (StartY > clip->nEndY) || (EndY < clip->nStartY) )
      return;

    if( StartX < clip->nStartX ) StartX = clip->nStartX;
    if( EndX   > clip->nEndX   ) EndX   = clip->nEndX;
    if( StartY < clip->nStartY ) StartY = clip->nStartY;
    if( EndY   > clip->nEndY   ) EndY   = clip->nEndY;

    width  = EndX - StartX + 1;
    height = EndY - StartY + 1;

    p = atGRPX_GetFrameBufferPtrXY( lpCanvas, StartX, StartY );

    skip = atGRPX_GetVMemWidth(lpCanvas) - width;

    for( i = 0; i < height; i ++ ){
      for( j = 0; j < width; j ++ ){
		cl = *p;
        *p++ = ~cl;
      }

      p += skip;
    }
}

