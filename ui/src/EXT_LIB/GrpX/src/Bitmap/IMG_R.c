/*#############################################################################
-------------------------------------------------------------------------------

                                  IMG_R.c
                           (* =================== *)


 Version           : 1.0.0
 Dependent Modules :
 Author            : Ju, Gyeong-min ( Amos )
 Create            : 2001/04/25    01:47:09am
 Update            : 2003/04/29    00:00:00am
 C Compiler        : ADS 1.01, SDT 2.51, Borland C/C++ 5.0, Visual C/C++ 6.0
 Assembler         : ADS 1.01, SDT 2.51, Borland TASM 3.1, MASM 6.0
 Description       : Put image function definition

-------------------------------------------------------------------------------
#############################################################################*/
#include "GrpX.h"
#include "A_Math.h"
#include "_IMG_func.h"


static void _DrawBitmapLine( atGRPX_PIXEL *ptr, atGRPX_BITMAP *lpBitmap, atINT width, atREAL x, atREAL y, atREAL dx, atREAL dy, atLONG ExParam1 )
{
    int i;
    int xx, yy;
    atGRPX_PIXEL *img_ptr = (atGRPX_PIXEL *)lpBitmap->lpBits;
    atGRPX_PIXEL pixel, trans_color;
    int bmWidth = lpBitmap->nWidth;
//    int bmHeight = lpBitmap->nHeight;

    if( lpBitmap->dwAttr & atGRPX_BMATTR_TRANSPARENT ){
        trans_color = COLOR_TO_PIXEL(lpBitmap->clTransColor);
        for( i = 0; i < width; i++ ){
          xx = REALTOINT(x);
          yy = REALTOINT(y);
    //	  if( (x >= 0) && (x < bmWidth) && (y >= 0) && (y < bmHeight) ){ // 불안해서 넣을까 말까 고민 중.
            pixel = *(img_ptr + yy*bmWidth + xx);
    	    if( pixel != trans_color ) *ptr = pixel;
    //    }
          ptr++;
          x += dx;
          y += dy;
       }
    } else {
        for( i = 0; i < width; i++ ){
          xx = REALTOINT(x);
          yy = REALTOINT(y);
    //	  if( (x >= 0) && (x < bmWidth) && (y >= 0) && (y < bmHeight) ){ // 불안해서 넣을까 말까 고민 중.
    	    *ptr = *(img_ptr + yy*bmWidth + xx);
    //    }
          ptr++;
          x += dx;
          y += dy;
       }
    }
}

static void _DrawPixel( atGRPX_PIXEL *ptr, atGRPX_BITMAP *lpBitmap, atINT X, atINT Y, atLONG ExParam1 )
{
    atGRPX_PIXEL pixel ;

    pixel = *((atGRPX_PIXEL *)lpBitmap->lpBits + Y * lpBitmap->nWidth + X);

    if( (lpBitmap->dwAttr & atGRPX_BMATTR_TRANSPARENT) && (pixel == COLOR_TO_PIXEL(lpBitmap->clTransColor)) ) return;
        
    *ptr = pixel;
}


/*****************************
  이미지를 회전하여 출력한다.
******************************/
void atGRPX_DrawBitmapRotate( atGRPX_CANVAS *lpCanvas, atGRPX_BITMAP *lpBitmap, atINT X, atINT Y, atREAL Angle )
{
    if( (Angle == INTTOREAL(0)) || (Angle == INTTOREAL(360)) ){
        atGRPX_DrawBitmap( lpCanvas, lpBitmap, X, Y );
        return;
    }
	
    __atGRPX_DrawBitmapRotate0( lpCanvas, lpBitmap, X, Y, Angle, (atLONG)0, _DrawPixel, _DrawBitmapLine );
}
