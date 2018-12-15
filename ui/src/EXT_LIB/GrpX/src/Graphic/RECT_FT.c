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
#include "_AB_int.h"


/*********************************

**********************************/
void atGRPX_FillRectTrans( atGRPX_CANVAS *lpCanvas, atINT StartX, atINT StartY, atINT EndX, atINT EndY, atGRPX_COLOR Color, atINT nAlpha )
{
    int i, j;
    int width, height, skip;
    atGRPX_PIXEL *p;
    atRECT *clip;
#if (atGRPX_PIXEL_BPP == 16) || (atGRPX_PIXEL_BPP == 24)
	atINT nAlpha2;
	atGRPX_COLOR Color2;
	atBYTE S_Red, S_Green, S_Blue;
	atBYTE T_Red, T_Green, T_Blue;
	atGRPX_PIXEL pixel2;
#endif

	if( nAlpha == 0 ) return;
	if( nAlpha == atGRPX_MAX_ALPHA ){
		atGRPX_FillRect( lpCanvas, StartX, StartY, EndX, EndY, Color );
		return;
	}

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

#if atGRPX_PIXEL_BPP == 16
	Color2 = __ALPHA_PIXEL_50P_CNV(Color);
#elif atGRPX_PIXEL_BPP == 24
	Color2 = __ALPHA_PIXEL_50P_CNV_2(Color);
#endif

	nAlpha2 = 0xFF - nAlpha;
	S_Red = GET_RED(Color); S_Green = GET_GREEN(Color); S_Blue = GET_BLUE(Color);
    for( i = 0; i < height; i ++ ){
      for( j = 0; j < width; j ++ ){
//        atGRPX_DrawPixelTrans( lpCanvas, StartX+j, StartY+i, Color, nAlpha );
		if( nAlpha == (0xFF / 2) ){
			__ALPHA_PIXEL_50P_2(p, (atGRPX_PIXEL)Color2);
		} else {
			__ALPHA_PIXEL_I(p, S_Red,S_Green,S_Blue, pixel2,T_Red,T_Green,T_Blue, nAlpha, nAlpha2 );
		}
		p++;
      }

      p += skip;
    }
}

