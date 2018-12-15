/*###########################################################################
-----------------------------------------------------------------------------

                                rTRI_F.h
                         (* =================== *)


 Version           : 1.0.0
 Dependent Modules :
 Author            : Ju, Gyeong-min ( Amos )
 Create            : 2001/04/25    01:47:09am
 Update            : 2003/04/29    00:00:00am
 C Compiler        : ADS 1.01, SDT 2.51, Borland C/C++ 5.0, Visual C/C++ 6.0
 Assembler         : ADS 1.01, SDT 2.51, Borland TASM 3.1, MASM 6.0
 Dependent Modules :
 Description       : TRI related function definition

-----------------------------------------------------------------------------
###########################################################################*/

#include "GrpX.h"
#include "GrpX_samsung.h"


#include "_TRI_Int.h"
#include "_TRI_func.h"
#include "_TRI_func_samsung.h"
#include "_AB_Int.h"


static void _HorizLine( atGRPX_CANVAS *lpCanvas, atREAL x1, atREAL x2, atINT y, atREAL dx1, atREAL dx2, atLONG ExParam1 )
{
	int i, width;
	atGRPX_PIXEL *ptr;
	atGRPX_PIXEL color = (atGRPX_PIXEL)ExParam1;
	atREAL k1, k2;
    atBYTE r0, g0, b0;
    atBYTE rr, gg, bb;
	atGRPX_PIXEL cc;
    atREAL weight2;

	ptr = atGRPX_GetFrameBufferPtrXY( lpCanvas, REALTOINT(x1), y );
	width = REALTOINT(x2) - REALTOINT(x1) + 1;

	k1 = REALREALK(x1); // 실수부만 떼낸다.
	k2 = REALREALK(x2);

    r0 = GET_RED(color); g0 = GET_GREEN(color); b0 = GET_BLUE(color);

    if( k1 > 0 ){
      weight2 = INTTOREAL(1) - k1;
      if( ABS(dx1) > INTTOREAL(1) ) weight2 = REALDIV( weight2, ABS(dx1) );
      k1 = INTTOREAL(1) - weight2;
      __ALPHA_PIXEL_R( ptr-1, r0,g0,b0, cc,rr,gg,bb, weight2, k1 );
    }

    if( ABS(dx1) >= INTTOREAL(1) ){
      for( i = 1; i < REALTOINT(ABS(dx1)); i++ ){
        if( width <= 0 ) return;
        weight2 = INTTOREAL(1) * i / REALTOINT(ABS(dx1));
        k1 = INTTOREAL(1) - weight2;
        __ALPHA_PIXEL_R( ptr, r0,g0,b0, cc,rr,gg,bb, weight2, k1 );
        ptr++;
	    width--;
      }
    }

    width -= REALTOINT(ABS(dx2));
    for( i = 0; i < width; i ++ ){
      *ptr++ = color;
    }

    if( ABS(dx2) >= INTTOREAL(1) ){
        for( i = REALTOINT((ABS(dx2))); i >= 1; i-- ){
          if( width <= 0 ) return;
          weight2 = INTTOREAL(1) * i / REALTOINT((ABS(dx2)));
          k1 = INTTOREAL(1) - weight2;
          __ALPHA_PIXEL_R( ptr, r0,g0,b0, cc,rr,gg,bb, weight2, k1 );
	      ptr++;
        }
    }

	if( k2 > 0 ){
      if( ABS(dx2) > INTTOREAL(1) ) k2 = REALDIV( k2, ABS(dx2) );
      weight2 = INTTOREAL(1) - k2;
      __ALPHA_PIXEL_R( ptr, r0,g0,b0, cc,rr,gg,bb, k2, weight2 );
    }
}

void atGRPX_FillRoundCapTriangleAA( atGRPX_CANVAS *lpCanvas, atLONG X1, atLONG Y1, atLONG X2, atLONG Y2, atLONG X3, atLONG Y3, atGRPX_COLOR Color )
{
    __atGRPX_FillRoundCapTriangle0( lpCanvas, X1, Y1, X2, Y2, X3, Y3, (atLONG)Color, _HorizLine );
}
