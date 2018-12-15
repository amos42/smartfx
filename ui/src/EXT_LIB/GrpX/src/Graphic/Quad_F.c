/*###########################################################################
-----------------------------------------------------------------------------

                                Quad_F.c
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
//#include "_QUD_func.h"


/*
static void _HorizLine( atGRPX_CANVAS *lpCanvas, atINT scan_y, atREAL lx, atREAL ly, atREAL rx, atREAL ry, atREAL ldxdy, atREAL rdxdy, atREAL alpha_init, atLONG ExParam )
{
	int i;
	atGRPX_PIXEL *ptr;
	atGRPX_PIXEL color = (atGRPX_PIXEL)ExParam;
    int ix1, ix2;

    ix1 = REALTOINT(lx);
    ix2 = REALTOINT(rx);
	ptr = atGRPX_GetFrameBufferPtrXY( lpCanvas, ix1, scan_y );

    for( i = ix1; i <= ix2; i ++ ){
      *ptr++ = color;
    }
}
*/


/*******************************
  속이 채워진 사각형을 그린다.
********************************/
void atGRPX_FillQuadrangle( atGRPX_CANVAS *lpCanvas, atINT X1, atINT Y1, atINT X2, atINT Y2, atINT X3, atINT Y3, atINT X4, atINT Y4, atGRPX_COLOR Color )
{
//	atGRPX_FillQuadrangle_REAL( lpCanvas, INTTOREAL(X1), INTTOREAL(Y1), INTTOREAL(X2), INTTOREAL(Y2), INTTOREAL(X3), INTTOREAL(Y3), INTTOREAL(X4), INTTOREAL(Y4), Color );
	atPOINT ptrs[4];
	ptrs[0].nX = X1; ptrs[0].nY = Y1;
	ptrs[1].nX = X2; ptrs[1].nY = Y2;
	ptrs[2].nX = X3; ptrs[2].nY = Y3;
	ptrs[3].nX = X4; ptrs[3].nY = Y4;
	atGRPX_FillConvexPolygon( lpCanvas, ptrs, 4, Color );
}


/*******************************
  속이 채워진 사각형을 그린다. (Real)
********************************/
void atGRPX_FillQuadrangle_REAL( atGRPX_CANVAS *lpCanvas, atREAL rX1, atREAL rY1, atREAL rX2, atREAL rY2, atREAL rX3, atREAL rY3, atREAL rX4, atREAL rY4, atGRPX_COLOR Color )
{
//    __atGRPX_FillQuadrangle_REAL0( lpCanvas, rX1, rY1, rX2, rY2, rX3, rY3, rX4, rY4, (atLONG)Color, _HorizLine );
	atRPOINT ptrs[4];
	ptrs[0].rX = rX1; ptrs[0].rY = rY1;
	ptrs[1].rX = rX2; ptrs[1].rY = rY2;
	ptrs[2].rX = rX3; ptrs[2].rY = rY3;
	ptrs[3].rX = rX4; ptrs[3].rY = rY4;
	atGRPX_FillConvexPolygon_REAL( lpCanvas, ptrs, 4, Color );
}

