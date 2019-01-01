/*###########################################################################
-----------------------------------------------------------------------------

                                PPLY_FAA.c
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
#include "_PLY2_func.h"


static void _HorizLine( atGRPX_CANVAS *lpCanvas, atREAL lx, atREAL rx, atINT scan_y, atREAL ldxdy, atREAL rdxdy, atDWORD dwAttr, atLONG ExParam )
{
//    atGRPX_DrawHorizLine( lpCanvas, REALTOINT(x1), y, REALTOINT(x2) - REALTOINT(x1) + 1, (atGRPX_COLOR)ExParam1 );

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


/*******************************
  다각형 라인을 그린다.
  (Multi Polygon, Hole Polygon)
********************************/
atVOID atGRPX_FillPolyPolygon( atGRPX_CANVAS *lpCanvas, atPOINT lpPtrList[], atINT nPartCount[], atINT nPolyCount, atGRPX_COLOR Color )
{
	if(nPolyCount==1)
		__atGRPX_FillComplexPolygon( lpCanvas, lpPtrList, nPartCount[0], Color, atFALSE );
	else
		__atGRPX_FillPolyPolygon0( lpCanvas, lpPtrList, nPartCount, nPolyCount, (atLONG)Color, _HorizLine, atNULL, atNULL );
}


/*******************************
  다각형 라인을 그린다.
  (Multi Polygon, Hole Polygon)
********************************/
atVOID atGRPX_FillPolyPolygonEx( atGRPX_CANVAS *lpCanvas, atPOINT lpPtrList[], atINT nPartCount[], atINT nPolyCount, atGRPX_COLOR Color, atLPGRPX_FUNC_COORD_CONV lpCnvFunc, atLPVOID lpParam )
{
    __atGRPX_FillPolyPolygon0( lpCanvas, lpPtrList, nPartCount, nPolyCount, (atLONG)Color, _HorizLine, lpCnvFunc, lpParam );
}


/*******************************
  다각형 라인을 그린다.
  (Multi Polygon, Hole Polygon)
********************************/
atVOID atGRPX_FillPolyPolygon_REAL( atGRPX_CANVAS *lpCanvas, atRPOINT lpPtrList[], atINT nPartCount[], atINT nPolyCount, atGRPX_COLOR Color )
{
    __atGRPX_FillPolyPolygon0_REAL( lpCanvas, lpPtrList, nPartCount, nPolyCount, (atLONG)Color, _HorizLine );
}


