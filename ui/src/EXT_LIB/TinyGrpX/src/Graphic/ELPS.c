/*#############################################################################
-------------------------------------------------------------------------------

                                    ELPS.c
                           (* =================== *)


 Version           : 1.0.0
 Dependent Modules :
 Author            : Ju, Gyeong-min ( Amos )
 Create            : 2001/04/25    01:47:09am
 Update            : 2003/04/29    00:00:00am
 C Compiler        : ADS 1.01, SDT 2.51, Borland C/C++ 5.0, Visual C/C++ 6.0
 Assembler         : ADS 1.01, SDT 2.51, Borland TASM 3.1, MASM 6.0
 Description       : Ellipse function definition

-------------------------------------------------------------------------------
#############################################################################*/
#include <stdlib.h>
#include "GrpX.h"


#define INT_ROUND(a) (int)((float)a + 0.5)

/***************
  타원을 그린다.
****************/
void atGRPX_DrawEllipse( atGRPX_CANVAS *lpCanvas, atINT nX, atINT nY, atINT RadiusX, atINT RediusY, atGRPX_COLOR Color )
{  
    int Rx2 = RadiusX * RadiusX;  
    int Ry2 = RediusY * RediusY;  
    int twoRx2 = 2 * Rx2;  
    int twoRy2 = 2 * Ry2;  
    int p;  
    int x = 0;  
    int y = RediusY;  
    int px = 0;  
      
    int py = twoRx2 * y;  
    
    /* Plot the initial point in each quadrant. */  
    atGRPX_DrawPixel( lpCanvas, nX + x, nY + y, Color );  
    atGRPX_DrawPixel( lpCanvas, nX - x, nY + y, Color );  
    atGRPX_DrawPixel( lpCanvas, nX + x, nY - y, Color );  
    atGRPX_DrawPixel( lpCanvas, nX - x, nY - y, Color );  
    
    /* Region 1 */  
    p = /*round*/( Ry2 - (Rx2 * RediusY) + INT_ROUND(Rx2 * 0.25) );
    while( px < py ){  
        x++;  
        px += twoRy2;  
        if( p < 0 ){  
            p += Ry2 + px;  
        } else {  
            y--;  
            py -= twoRx2;  
            p += Ry2 + px - py;  
        }  
		atGRPX_DrawPixel( lpCanvas, nX + x, nY + y, Color );  
		atGRPX_DrawPixel( lpCanvas, nX - x, nY + y, Color );  
		atGRPX_DrawPixel( lpCanvas, nX + x, nY - y, Color );  
		atGRPX_DrawPixel( lpCanvas, nX - x, nY - y, Color );  
    }  
    
    /* Region 2 */  
    p = /*round*/(Ry2 * ((x * x) + x) + INT_ROUND(Ry2 * 0.25) + Rx2 * (y-1) * (y-1) - Rx2 * Ry2);  
    while( y > 0 ){  
        y--;  
        py -= twoRx2;  
        if( p > 0 ){
            p += Rx2 - py;          
        } else {  
            x++;  
            px += twoRy2;  
            p += Rx2 - py + px;  
        }  
		atGRPX_DrawPixel( lpCanvas, nX + x, nY + y, Color );  
		atGRPX_DrawPixel( lpCanvas, nX - x, nY + y, Color );  
		atGRPX_DrawPixel( lpCanvas, nX + x, nY - y, Color );  
		atGRPX_DrawPixel( lpCanvas, nX - x, nY - y, Color );  
    } // end of while  
}  
 