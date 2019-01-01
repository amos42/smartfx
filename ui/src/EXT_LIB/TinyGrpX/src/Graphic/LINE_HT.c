/*#############################################################################
-------------------------------------------------------------------------------

                                  LINE_HT.c
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

#include "GrpX.h"
#include "_AB_int.h"


/***********************
  수평선을 그린다.
************************/
void atGRPX_DrawHorizLineTrans( atGRPX_CANVAS *lpCanvas, atINT StartX, atINT StartY, atINT Length, atGRPX_COLOR Color, atINT nAlpha )
{
    int i, t;
    atGRPX_PIXEL *p;
    atRECT *clip;
	atGRPX_PIXEL cl = COLOR_TO_PIXEL(Color);
#if (atGRPX_PIXEL_BPP == 16) || (atGRPX_PIXEL_BPP == 24)
	atINT nAlpha2;
	atGRPX_COLOR Color2;
	atBYTE S_Red, S_Green, S_Blue;
	atBYTE T_Red, T_Green, T_Blue;
	atGRPX_PIXEL pixel2;
#endif

    if( Length <= 0 ) return;

    clip = atGRPX_GetClipRect(lpCanvas);

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

    p = atGRPX_GetFrameBufferPtrXY( lpCanvas, StartX, StartY );

#if atGRPX_PIXEL_BPP == 16
	Color2 = __ALPHA_PIXEL_50P_CNV(Color);
#elif atGRPX_PIXEL_BPP == 24
	Color2 = __ALPHA_PIXEL_50P_CNV_2(Color);
#endif

	if( nAlpha == (0xFF / 2) ){
	    for( i = 0; i < Length; i ++ ){
			__ALPHA_PIXEL_50P_2(p, (atGRPX_PIXEL)Color2);
		    p++;
		}
	} else {
	    nAlpha2 = 0xFF - nAlpha;
	    S_Red = GET_RED(Color); S_Green = GET_GREEN(Color); S_Blue = GET_BLUE(Color);
	    for( i = 0; i < Length; i ++ ){
			__ALPHA_PIXEL_I(p, S_Red,S_Green,S_Blue, pixel2,T_Red,T_Green,T_Blue, nAlpha, nAlpha2 );
		    p++;
		}
   	}
}

