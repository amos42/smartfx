/*###########################################################################
-----------------------------------------------------------------------------

                                PLY_F.c
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
#include "_PLY_func.h"
#include "_CVX_func.h"
#include "_CVX2_func.h"


static void _HorizLine( atGRPX_CANVAS *lpCanvas, atINT scan_y, atREAL lx, atREAL ly, atREAL rx, atREAL ry, atREAL ldxdy, atREAL rdxdy, atREAL alpha_init, atDWORD attr, atLONG ExParam )
{
	atGRPX_PIXEL color = (atGRPX_PIXEL)ExParam;
	atREAL x1, x2;
    int ix1, ix2;

	x1 = lx + REALHALF(ldxdy);
	x2 = rx + REALHALF(rdxdy);
    ix1 = REALTOINT(x1);
    ix2 = REALTOINT(x2);
	if( REALREALK(x1) > FLOATTOREAL(0.5+0.1) ) ix1++;
	if( REALREALK(x2) < FLOATTOREAL(0.5-0.1) ) ix2--;

    atGRPX_DrawHorizLine( lpCanvas, ix1, scan_y, ix2-ix1+1, color );
}


/*******************
  다각형 라인을 그린다.
********************/
void atGRPX_FillPolygon( atGRPX_CANVAS *lpCanvas, atPOINT lpPtrList[], atINT nCount, atGRPX_COLOR Color )
{
    atGRPX_FillPolyPolygon( lpCanvas, lpPtrList, &nCount, 1, Color );
}


/*******************
  다각형 라인을 그린다.
********************/
void atGRPX_FillPolygon_REAL( atGRPX_CANVAS *lpCanvas, atRPOINT lpPtrList[], atINT nCount, atGRPX_COLOR Color )
{
    atGRPX_FillPolyPolygon_REAL( lpCanvas, lpPtrList, &nCount, 1, Color );
}


/*******************
  다각형 라인을 그린다.
********************/
void atGRPX_FillConvexPolygon( atGRPX_CANVAS *lpCanvas, atPOINT lpPtrList[], atINT nCount, atGRPX_COLOR Color )
{
    
//    __atGRPX_FillConvexPolygon0( lpCanvas, lpPtrList, nCount, (atLONG)Color, _HorizLine );
}


/*******************
  다각형 라인을 그린다.
********************/
void atGRPX_FillConvexPolygon_REAL( atGRPX_CANVAS *lpCanvas, atRPOINT lpPtrList[], atINT nCount, atGRPX_COLOR Color )
{
//     __atGRPX_FillConvexPolygon0_REAL( lpCanvas, lpPtrList, nCount, atNULL, (atLONG)Color, _HorizLine );
    __atGRPX_FillConvexPolygon1_REAL( lpCanvas, lpPtrList, nCount, Color, atFALSE, atFALSE );
}
void atGRPX_FillConvexPolygonEx_REAL( atGRPX_CANVAS *lpCanvas, atRPOINT lpPtrList[], atINT nCount, atINT lpAttr[], atGRPX_COLOR Color )
{
    __atGRPX_FillConvexPolygon0_REAL( lpCanvas, lpPtrList, nCount, lpAttr, (atLONG)Color, _HorizLine );
//     __atGRPX_FillConvexPolygon1_REAL( lpCanvas, lpPtrList, nCount, Color, atFALSE, atFALSE );
}




