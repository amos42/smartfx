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
void atGRPX_FillCircle( atGRPX_CANVAS *lpCanvas, atINT X, atINT Y, atINT Radius, atGRPX_COLOR Color )
{
/*   int x, y, thres;

   if( Radius <= 0 ) return;
   
   if( Radius == 1 ){
     atGRPX_DrawPixel( lpCanvas, X, Y, Color );
     return;
   }

   y = Radius;
   thres = 3 - (Radius << 1);

   for( x = 0; x < y; x ++ ){
     if( thres < 0 ){
       thres += 6 + (x << 2);
     } else {
       thres += 10 + ((x - y) << 2);
       y --;
       atGRPX_DrawHorizLine( lpCanvas, X - x, Y + y, (x << 1) + 1, Color );
       atGRPX_DrawHorizLine( lpCanvas, X - x, Y - y, (x << 1) + 1, Color );
     }

     if( Y + x != Y + y )
       atGRPX_DrawHorizLine( lpCanvas, X - y, Y + x, (y << 1) + 1, Color );
     if( Y - x != Y - y && Y - x != Y + x )
       atGRPX_DrawHorizLine( lpCanvas, X - y, Y - x, (y << 1) + 1, Color );
   }*/
	int h, x, y;
	int deltaE, deltaNE;
	
	x = 0;
	y = Radius;
	h = 1 - Radius;
	deltaE = 3;
	deltaNE = 5 - 2 * Radius;
	while(y>=x)
	{
		atGRPX_DrawHorizLine( lpCanvas, -x + X, y + Y, 2 * x, Color );
		atGRPX_DrawHorizLine( lpCanvas, -y + X, x + Y, 2 * y, Color );
		atGRPX_DrawHorizLine( lpCanvas, -y + X, -x + Y, 2 * y, Color );
		atGRPX_DrawHorizLine( lpCanvas, -x + X, -y + Y, 2 * x, Color );
		if(h<0)
		{
			h += deltaE;
			deltaE += 2;
			deltaNE += 2;
		}
		else
		{
			h += deltaNE;
			deltaE += 2;
			deltaNE += 4;
			y--;
		}
		x++;
	}
}

