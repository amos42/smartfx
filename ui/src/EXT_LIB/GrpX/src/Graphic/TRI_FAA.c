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


#include "_TRI_int.h"
#include "_TRI_func.h"
//#include "_AB_int.h"
#include "_AA_func.h"


/*
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
      weight2 = REAL_ONE - k1;
      __ALPHA_PIXEL_R( ptr, r0,g0,b0, cc,rr,gg,bb, weight2, k1 );
	  ptr++;
	  width--;
	}

    for( i = 0; i < width; i ++ ){
      *ptr++ = color;
    }

	if( k2 > 0 ){
      weight2 = REAL_ONE - k2;
      __ALPHA_PIXEL_R( ptr, r0,g0,b0, cc,rr,gg,bb, k2, weight2 );
	}
}
*/
/*
static void _HorizLine( atGRPX_CANVAS *lpCanvas, atREAL x1, atREAL x2, atINT y, atREAL dx1, atREAL dx2, atLONG ExParam1 )
{
	__AA_DrawHorizeLine( lpCanvas, y, x1, INTTOREAL(y), x2, INTTOREAL(y), dx1, dx2, (atGRPX_PIXEL)ExParam1, REAL_ONE, __AA_ADJ_BOTH );
}
*/

/*******************************
  속이 채워진 삼각형을 그린다.
********************************/
void atGRPX_FillTriangleAA( atGRPX_CANVAS *lpCanvas, atINT X1, atINT Y1, atINT X2, atINT Y2, atINT X3, atINT Y3, atGRPX_COLOR Color )
{
//    __atGRPX_FillTriangle0( lpCanvas, X1, Y1, X2, Y2, X3, Y3, (atLONG)Color, _HorizLine );
	atPOINT ptrs[3];
	ptrs[0].nX = X1; ptrs[0].nY = Y1;
	ptrs[1].nX = X2; ptrs[1].nY = Y2;
	ptrs[2].nX = X3; ptrs[2].nY = Y3;
	atGRPX_FillConvexPolygonAA( lpCanvas, ptrs, 3, Color );
}


/*******************************
  속이 채워진 삼각형을 그린다.
********************************/
void atGRPX_FillTriangleAA_REAL( atGRPX_CANVAS *lpCanvas, atREAL rX1, atREAL rY1, atREAL rX2, atREAL rY2, atREAL rX3, atREAL rY3, atGRPX_COLOR Color )
{
//    __atGRPX_FillTriangle0_REAL( lpCanvas, rX1, rY1, rX2, rY2, rX3, rY3, (atLONG)Color, _HorizLine );
	atRPOINT ptrs[3];
	ptrs[0].rX = rX1; ptrs[0].rY = rY1;
	ptrs[1].rX = rX2; ptrs[1].rY = rY2;
	ptrs[2].rX = rX3; ptrs[2].rY = rY3;
	atGRPX_FillConvexPolygonAA_REAL( lpCanvas, ptrs, 3, Color );
}



