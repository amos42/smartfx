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


static void _HorizLine( atGRPX_CANVAS *lpCanvas, atREAL x1, atREAL x2, atINT y, atREAL dx1, atREAL dx2, atLONG ExParam1 )
{
	int i, width;
	atGRPX_PIXEL *ptr;
	atGRPX_PIXEL color = (atGRPX_PIXEL)ExParam1;

	ptr = atGRPX_GetFrameBufferPtrXY( lpCanvas, REALTOINT(x1), y );
	width = REALTOINT(x2) - REALTOINT(x1) + 1;

    for( i = 0; i < width; i ++ ){
      *ptr++ = color;
    }
}


/*******************************
  속이 채워진 삼각형을 그린다.
********************************/
void atGRPX_FillTriangle( atGRPX_CANVAS *lpCanvas, atINT X1, atINT Y1, atINT X2, atINT Y2, atINT X3, atINT Y3, atGRPX_COLOR Color )
{
    __atGRPX_FillTriangle0( lpCanvas, X1, Y1, X2, Y2, X3, Y3, (atLONG)Color, _HorizLine );
}


/*******************************
  속이 채워진 삼각형을 그린다.
********************************/
void atGRPX_FillTriangle_REAL( atGRPX_CANVAS *lpCanvas, atREAL rX1, atREAL rY1, atREAL rX2, atREAL rY2, atREAL rX3, atREAL rY3, atGRPX_COLOR Color )
{
 	__atGRPX_FillTriangle0_REAL( lpCanvas, rX1, rY1, rX2, rY2, rX3, rY3, (atLONG)Color, _HorizLine );
}


