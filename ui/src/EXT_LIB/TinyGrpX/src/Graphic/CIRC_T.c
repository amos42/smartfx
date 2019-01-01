/*#############################################################################
-------------------------------------------------------------------------------

                                  CirCle_F.c
                           (* =================== *)


 Version           : 1.0.0
 Dependent Modules :
 Author            : Ju, Gyeong-min ( Amos )
 Create            : 2001/04/25    01:47:09am
 Update            : 2003/04/29    00:00:00am
 C Compiler        : ADS 1.01, SDT 2.51, Borland C/C++ 5.0, Visual C/C++ 6.0
 Assembler         : ADS 1.01, SDT 2.51, Borland TASM 3.1, MASM 6.0
 Description       : Circle function definition

-------------------------------------------------------------------------------
#############################################################################*/
#include <stdlib.h>
#include "GrpX.h"


/***************************
  속이 채워진 원을 그린다.
****************************/
void atGRPX_DrawCircleTrans( atGRPX_CANVAS *lpCanvas, atINT X, atINT Y, atINT Radius, atGRPX_COLOR Color, atINT nAlpha )
{
   int x, y, thres;

   if( Radius <= 0 ) return;
   
   if( Radius == 1 ){
     atGRPX_DrawPixelTrans( lpCanvas, X, Y, Color, nAlpha );
     return;
    }

   y = Radius;
   thres = 3 - (Radius << 1);

   for( x = 0; x < y; x ++ ){
     if( thres < 0 ){
       thres += 6 + (x << 2);
     } else{
       thres += 10 + ((x - y) << 2);
       y --;
     }

     atGRPX_DrawPixelTrans( lpCanvas, X+x, Y+y, Color, nAlpha ); atGRPX_DrawPixelTrans( lpCanvas, X+y, Y+x, Color, nAlpha );
     atGRPX_DrawPixelTrans( lpCanvas, X-x, Y+y, Color, nAlpha ); atGRPX_DrawPixelTrans( lpCanvas, X-y, Y+x, Color, nAlpha );
     atGRPX_DrawPixelTrans( lpCanvas, X+x, Y-y, Color, nAlpha ); atGRPX_DrawPixelTrans( lpCanvas, X+y, Y-x, Color, nAlpha );
     atGRPX_DrawPixelTrans( lpCanvas, X-x, Y-y, Color, nAlpha ); atGRPX_DrawPixelTrans( lpCanvas, X-y, Y-x, Color, nAlpha );
    }
}
