/*#############################################################################
-------------------------------------------------------------------------------

                                  Line_VT.c
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
#include "GrpX.h"
#include "_AB_int.h"


/*******************
  수직선을 그린다.
********************/
void atGRPX_DrawVertLineTrans( atGRPX_CANVAS *lpCanvas, atINT StartX, atINT StartY, atINT Length, atGRPX_COLOR Color, atINT nAlpha )
{
    int i, t;
    atGRPX_PIXEL *p;
    atRECT *clip;
	int inc;
	atGRPX_PIXEL cl = (atGRPX_PIXEL)Color;
#if (atGRPX_PIXEL_BPP == 16) || (atGRPX_PIXEL_BPP == 24)
	atINT nAlpha2;
	atGRPX_COLOR Color2;
	atBYTE S_Red, S_Green, S_Blue;
	atBYTE T_Red, T_Green, T_Blue;
	atGRPX_PIXEL pixel2;
#endif

    if( Length <= 0 ) return;

    clip = atGRPX_GetClipRect(lpCanvas);

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
    
    p = atGRPX_GetFrameBufferPtrXY( lpCanvas, StartX, StartY );

	inc = atGRPX_GetVMemWidth(lpCanvas);

#if atGRPX_PIXEL_BPP == 16
	Color2 = __ALPHA_PIXEL_50P_CNV(Color);
#elif atGRPX_PIXEL_BPP == 24
	Color2 = __ALPHA_PIXEL_50P_CNV_2(Color);
#endif

	if( nAlpha == (0xFF / 2) ){
	    for( i = 0; i < Length; i ++ ){
			__ALPHA_PIXEL_50P_2(p, (atGRPX_PIXEL)Color2);
            p += inc;
		}
	} else {
	    nAlpha2 = 0xFF - nAlpha;
	    S_Red = GET_RED(Color); S_Green = GET_GREEN(Color); S_Blue = GET_BLUE(Color);
	    for( i = 0; i < Length; i ++ ){
			__ALPHA_PIXEL_I(p, S_Red,S_Green,S_Blue, pixel2,T_Red,T_Green,T_Blue, nAlpha, nAlpha2 );
            p += inc;
		}
   	}
}
