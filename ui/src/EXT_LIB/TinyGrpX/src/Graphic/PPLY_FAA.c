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
#include "_AA_func.h"


static void _HorizLine( atGRPX_CANVAS *lpCanvas, atREAL lx, atREAL rx, atINT y, atREAL ldxdy, atREAL rdxdy, atDWORD dwAttr, atLONG ExParam1 )
{
	__AA_DrawHorizeLine( lpCanvas, y, lx, INTTOREAL(y), rx, INTTOREAL(y), ldxdy, rdxdy, (atGRPX_PIXEL)ExParam1, REAL_ONE, __AA_ADJ_BOTH );
}


/*******************************
  다각형 라인을 그린다.
  (Multi Polygon, Hole Polygon)
  (Anti-Aliacing 버전)
********************************/
void atGRPX_FillPolyPolygonAA( atGRPX_CANVAS *lpCanvas, atPOINT lpPtrList[], atINT nPartCount[], atINT nPolyCount, atGRPX_COLOR Color )
{
	if(nPolyCount==1)
		__atGRPX_FillComplexPolygon( lpCanvas, lpPtrList, nPartCount[0], Color, atTRUE );
	else
		__atGRPX_FillPolyPolygon0( lpCanvas, lpPtrList, nPartCount, nPolyCount, (atLONG)Color, _HorizLine, atNULL, atNULL );
}


/*******************************
  다각형 라인을 그린다.
  (Multi Polygon, Hole Polygon)
  (Anti-Aliacing 버전)
********************************/
atVOID atGRPX_FillPolyPolygonExAA( atGRPX_CANVAS *lpCanvas, atPOINT lpPtrList[], atINT nPartCount[], atINT nPolyCount, atGRPX_COLOR Color, atLPGRPX_FUNC_COORD_CONV lpCnvFunc, atLPVOID lpParam )
{
    __atGRPX_FillPolyPolygon0( lpCanvas, lpPtrList, nPartCount, nPolyCount, (atLONG)Color, _HorizLine, lpCnvFunc, lpParam );
}


/*******************************
  다각형 라인을 그린다.
  (Multi Polygon, Hole Polygon)
  (Anti-Aliacing 버전)
********************************/
void atGRPX_FillPolyPolygonAA_REAL( atGRPX_CANVAS *lpCanvas, atRPOINT lpPtrList[], atINT nPartCount[], atINT nPolyCount, atGRPX_COLOR Color )
{
    __atGRPX_FillPolyPolygon0_REAL( lpCanvas, lpPtrList, nPartCount, nPolyCount, (atLONG)Color, _HorizLine );
}


