/*#############################################################################
-------------------------------------------------------------------------------

                                  IMG_RT.c
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
#include "../Graphic/_AB_int.h"


static void _DrawBitmapLine( atGRPX_PIXEL *ptr, atGRPX_BITMAP *lpBitmap, int width, atREAL x, atREAL y, atREAL dx, atREAL dy, atLONG ExParam1 )
{
    int i;
    int xx, yy;
    atGRPX_PIXEL *img_ptr = (atGRPX_PIXEL *)lpBitmap->lpBits;
    atGRPX_PIXEL pixel, trans_color;
    int bmWidth = lpBitmap->nWidth;
//    int bmHeight = lpBitmap->nHeight;

    if( lpBitmap->dwAttr & atGRPX_BMATTR_TRANSPARENT ){
        trans_color = COLOR_TO_PIXEL(lpBitmap->clTransColor);
        if( ExParam1 == 255/2 ){
          for( i = 0; i < width; i++ ){
            xx = REALTOINT(x);
            yy = REALTOINT(y);
    //	    if( (x >= 0) && (x < bmWidth) && (y >= 0) && (y < bmHeight) ){ // 불안해서 넣을까 말까 고민 중.
              pixel = *(img_ptr + yy*bmWidth + xx);
              if( pixel != trans_color ){
                __ALPHA_PIXEL_50P( ptr, (atGRPX_PIXEL)pixel );
              }
    //      }
            ptr++;
            x += dx;
            y += dy;
         }
       } else {
    	  //atINT weight2 = 255 - ExParam1;
          atBYTE r0, g0, b0;
          atBYTE rr, gg, bb;
          atGRPX_COLOR cc;
    
          for( i = 0; i < width; i++ ){
            xx = REALTOINT(x);
            yy = REALTOINT(y);
    //	    if( (x >= 0) && (x < bmWidth) && (y >= 0) && (y < bmHeight) ){ // 불안해서 넣을까 말까 고민 중.
              pixel = *(img_ptr + yy*bmWidth + xx);
              if( pixel != trans_color ){
                r0 = GET_RED(pixel); g0 = GET_GREEN(pixel); b0 = GET_BLUE(pixel);
    	        __ALPHA_PIXEL_I( ptr, r0, g0, b0, cc, rr, gg, bb, ExParam1, weight2 );
    	      }
    //      }
            ptr++;
            x += dx;
            y += dy;
         }
       }
    } else {
        if( ExParam1 == 255/2 ){
          for( i = 0; i < width; i++ ){
            xx = REALTOINT(x);
            yy = REALTOINT(y);
    //	    if( (x >= 0) && (x < bmWidth) && (y >= 0) && (y < bmHeight) ){ // 불안해서 넣을까 말까 고민 중.
              pixel = *(img_ptr + yy*bmWidth + xx);
    	      __ALPHA_PIXEL_50P( ptr, (atGRPX_PIXEL)pixel );
    //      }
            ptr++;
            x += dx;
            y += dy;
         }
       } else {
    	  //atINT weight2 = 255 - ExParam1;
          atBYTE r0, g0, b0;
          atBYTE rr, gg, bb;
          atGRPX_COLOR cc;
    
          for( i = 0; i < width; i++ ){
            xx = REALTOINT(x);
            yy = REALTOINT(y);
    //	    if( (x >= 0) && (x < bmWidth) && (y >= 0) && (y < bmHeight) ){ // 불안해서 넣을까 말까 고민 중.
              pixel = *(img_ptr + yy*bmWidth + xx);
              r0 = GET_RED(pixel); g0 = GET_GREEN(pixel); b0 = GET_BLUE(pixel);
    	      __ALPHA_PIXEL_I( ptr, r0, g0, b0, cc, rr, gg, bb, ExParam1, weight2 );
    //      }
            ptr++;
            x += dx;
            y += dy;
         }
       }
    }
}

static void _DrawPixel( atGRPX_PIXEL *ptr, atGRPX_BITMAP *lpBitmap, atINT X, atINT Y, atLONG ExParam1 )
{
    atBYTE r0, g0, b0;
    atBYTE rr, gg, bb;
    atGRPX_COLOR cc;
    //atINT weight2 = 255 - ExParam1;
    atGRPX_PIXEL *img_ptr = lpBitmap->lpBits;
    atGRPX_PIXEL pixel;

    pixel = *(img_ptr + Y * lpBitmap->nWidth + X);

    if( (lpBitmap->dwAttr & atGRPX_BMATTR_TRANSPARENT) && (pixel == COLOR_TO_PIXEL(lpBitmap->clTransColor)) ) return;
        
    r0 = GET_RED(pixel); g0 = GET_GREEN(pixel); b0 = GET_BLUE(pixel);
    __ALPHA_PIXEL_I( ptr, r0, g0, b0, cc, rr, gg, bb, ExParam1, weight2 );
}


/*****************************
  이미지를 회전하여 출력한다.
******************************/
void atGRPX_DrawBitmapRotateTrans( atGRPX_CANVAS *lpCanvas, atGRPX_BITMAP *lpBitmap, atINT X, atINT Y, atREAL Angle, atINT nAlpha )
{
    __atGRPX_DrawBitmapRotate0( lpCanvas, lpBitmap, X, Y, Angle, (atLONG)nAlpha, _DrawPixel, _DrawBitmapLine );
}
