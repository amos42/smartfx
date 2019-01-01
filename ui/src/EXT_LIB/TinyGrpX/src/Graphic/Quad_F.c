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

#include "TinyGrpX.h"
//#include "_QUD_func.h"


/*
static void _HorizLine( atTGRPX_CANVAS *lpCanvas, atINT scan_y, atREAL lx, atREAL ly, atREAL rx, atREAL ry, atREAL ldxdy, atREAL rdxdy, atREAL alpha_init, atLONG ExParam )
{
	int i;
	atTGRPX_PIXEL *ptr;
	atTGRPX_PIXEL color = (atTGRPX_PIXEL)ExParam;
    int ix1, ix2;

    ix1 = REALTOINT(lx);
    ix2 = REALTOINT(rx);
	ptr = atTGRPX_GetFrameBufferPtrXY( lpCanvas, ix1, scan_y );

    for( i = ix1; i <= ix2; i ++ ){
      *ptr++ = color;
    }
}
*/


/*******************************
  속이 채워진 사각형을 그린다.
********************************/
void atTGRPX_FillQuadrangle( atTGRPX_CANVAS *lpCanvas, atINT X1, atINT Y1, atINT X2, atINT Y2, atINT X3, atINT Y3, atINT X4, atINT Y4, atTGRPX_COLOR Color )
{
//	atTGRPX_FillQuadrangle_REAL( lpCanvas, INTTOREAL(X1), INTTOREAL(Y1), INTTOREAL(X2), INTTOREAL(Y2), INTTOREAL(X3), INTTOREAL(Y3), INTTOREAL(X4), INTTOREAL(Y4), Color );
	atPOINT ptrs[4];
	ptrs[0].nX = X1; ptrs[0].nY = Y1;
	ptrs[1].nX = X2; ptrs[1].nY = Y2;
	ptrs[2].nX = X3; ptrs[2].nY = Y3;
	ptrs[3].nX = X4; ptrs[3].nY = Y4;
	atTGRPX_FillConvexPolygon( lpCanvas, ptrs, 4, Color );
}
