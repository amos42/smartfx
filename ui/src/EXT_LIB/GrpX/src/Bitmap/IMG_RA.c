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
#include "../Graphic/_AB_int.h"


static void _DrawBitmapLine( atGRPX_PIXEL *ptr, atGRPX_BITMAP *lpBitmap, atINT width, atREAL x, atREAL y, atREAL dx, atREAL dy, atLONG ExParam1 )
{
    int i;
    int xx, yy;
    atGRPX_PIXEL *img_ptr = (atGRPX_PIXEL *)lpBitmap->lpBits;
    atLPBYTE pAlphaChannel = (atLPBYTE)(atINTPTR)ExParam1;
    atGRPX_PIXEL pixel;
    int bmWidth = lpBitmap->nWidth;
    int offs;
#if atGRPX_PIXEL_BPP == 16
    atGRPX_PIXEL pixel2;
    int S_Red, S_Green, S_Blue;
    int T_Red, T_Green, T_Blue;
	int alpha, S_Alpha2;
#endif    
    
//    int bmHeight = lpBitmap->nHeight;

    for( i = 0; i < width; i++ ){
      xx = REALTOINT(x);
      yy = REALTOINT(y);
//	  if( (x >= 0) && (x < bmWidth) && (y >= 0) && (y < bmHeight) ){ // 불안해서 넣을까 말까 고민 중.
        offs = yy*bmWidth + xx;
        pixel = *(img_ptr + offs);
        alpha = *(pAlphaChannel + offs);
        
		if( alpha == 0x00 ){
		    //
		} else if( alpha == 0xFF ){
			*ptr = pixel;
		} else if( alpha == 0xFF/2 ){
			__ALPHA_PIXEL_50P( ptr, (atGRPX_PIXEL)pixel );
		} else {
#if atGRPX_PIXEL_BPP == 16
            S_Red = (pixel >> 8) & 0xF8;
            S_Green = (pixel >> 3) & 0xFC;
            S_Blue = (pixel << 3) & 0xF8;
#endif            
            S_Alpha2 = 255-alpha;
			__ALPHA_PIXEL_I( ptr, S_Red,S_Green,S_Blue, pixel2,T_Red,T_Green,T_Blue, alpha, S_Alpha2 );
        }                
//    }
      ptr++;
      x += dx;
      y += dy;
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
void atGRPX_DrawBitmapRotateWithAlphaChannel( atGRPX_CANVAS *lpCanvas, atGRPX_BITMAP *lpBitmap, atINT X, atINT Y, atREAL Angle, atBYTE *pAlphaChannel )
{
    if( (Angle == INTTOREAL(0)) || (Angle == INTTOREAL(360)) ){
        atGRPX_DrawBitmapWithAlphaChannel( lpCanvas, lpBitmap, X, Y, pAlphaChannel );
        return;
    }

    if( lpBitmap == atNULL ) return;
    if( pAlphaChannel == atNULL ) pAlphaChannel = lpBitmap->lpAlphaChannel;
    if( lpBitmap == atNULL ) return;
	
    __atGRPX_DrawBitmapRotate0( lpCanvas, lpBitmap, X, Y, Angle, (atLONG)(atINTPTR)pAlphaChannel, _DrawPixel, _DrawBitmapLine );
}
