/*###########################################################################
-----------------------------------------------------------------------------

                                Quad.c
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
  사각형을 그린다.
********************/
void atGRPX_DrawQuadrangle( atGRPX_CANVAS *lpCanvas, atINT X1, atINT Y1, atINT X2, atINT Y2, atINT X3, atINT Y3, atINT X4, atINT Y4, atGRPX_COLOR Color )
{
    atGRPX_DrawLine( lpCanvas, X1, Y1, X2, Y2, Color );
    atGRPX_DrawLine( lpCanvas, X2, Y2, X3, Y3, Color );
    atGRPX_DrawLine( lpCanvas, X3, Y3, X4, Y4, Color );
    atGRPX_DrawLine( lpCanvas, X4, Y4, X1, Y1, Color );
}


/*******************
  사각형을 그린다.
********************/
void atGRPX_DrawQuadrangleAA( atGRPX_CANVAS *lpCanvas, atINT X1, atINT Y1, atINT X2, atINT Y2, atINT X3, atINT Y3, atINT X4, atINT Y4, atGRPX_COLOR Color )
{
    atGRPX_DrawLineAA( lpCanvas, X1, Y1, X2, Y2, Color );
    atGRPX_DrawLineAA( lpCanvas, X2, Y2, X3, Y3, Color );
    atGRPX_DrawLineAA( lpCanvas, X3, Y3, X4, Y4, Color );
    atGRPX_DrawLineAA( lpCanvas, X4, Y4, X1, Y1, Color );
}

