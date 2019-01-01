/*###########################################################################
-----------------------------------------------------------------------------

                                TRI.c
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


/*******************
  삼각형을 그린다.
********************/
void atGRPX_DrawTriangle( atGRPX_CANVAS *lpCanvas, atINT X1, atINT Y1, atINT X2, atINT Y2, atINT X3, atINT Y3, atGRPX_COLOR Color )
{
    atGRPX_DrawLine( lpCanvas, X1, Y1, X2, Y2, Color );
    atGRPX_DrawLine( lpCanvas, X1, Y1, X3, Y3, Color );
    atGRPX_DrawLine( lpCanvas, X2, Y2, X3, Y3, Color );

/*
	atPOINT ptr[4];
	ptr[0].nX = X1;
	ptr[0].nY = Y1;
	ptr[1].nX = X2;
	ptr[1].nY = Y2;
	ptr[2].nX = X3;
	ptr[2].nY = Y3;
	ptr[3].nX = X1;
	ptr[3].nY = Y1;
	atGRPX_DrawPolyLine( lpCanvas, ptr, 3, atTRUE, Color );
*/
}


/*******************
  삼각형을 그린다.
********************/
void atGRPX_DrawTriangleAA( atGRPX_CANVAS *lpCanvas, atINT X1, atINT Y1, atINT X2, atINT Y2, atINT X3, atINT Y3, atGRPX_COLOR Color )
{
    atGRPX_DrawLineAA( lpCanvas, X1, Y1, X2, Y2, Color );
    atGRPX_DrawLineAA( lpCanvas, X1, Y1, X3, Y3, Color );
    atGRPX_DrawLineAA( lpCanvas, X2, Y2, X3, Y3, Color );
}

