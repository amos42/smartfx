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


static void _DrawBitmapLine_1bpp( atGRPX_PIXEL *ptr, atGRPX_BITMAP *lpBitmap, int width, atREAL x, atREAL y, atREAL dx, atREAL dy, atLONG ExParam1 )
{
    int i;
    int xx, yy;
    atBYTE *img_ptr = (atBYTE *)lpBitmap->lpBits;
//    int bmWidth = lpBitmap->nWidth;
//    int bmHeight = lpBitmap->nHeight;
    int bmWidthBytes = lpBitmap->nWidthBytes;
    atGRPX_PIXEL color = (atGRPX_PIXEL)ExParam1;
    atBYTE cl;

    for( i = 0; i < width; i++ ){
      xx = REALTOINT(x);
      yy = REALTOINT(y);
//	  if( (x >= 0) && (x < bmWidth) && (y >= 0) && (y < bmHeight) ){ // 불안해서 넣을까 말까 고민 중.
        cl = *(img_ptr + yy*bmWidthBytes + (xx >> 3));
	    if( cl & ((0x80) >> (xx & 0x7)) ) *ptr = color;
//    }
      ptr++;
      x += dx;
      y += dy;
   }
}

static void _DrawPixel_1bpp( atGRPX_PIXEL *ptr, atGRPX_BITMAP *lpBitmap, atINT X, atINT Y, atLONG ExParam1 )
{
    atBYTE *img_ptr = (atBYTE *)lpBitmap->lpBits;
    atGRPX_PIXEL color = (atGRPX_PIXEL)ExParam1;
    atBYTE cl;

    cl = *(img_ptr + Y*lpBitmap->nWidthBytes + X/8);
    if( cl & ((0x80) >> (X & 0x7)) ) *ptr = color;
}


static void _DrawBitmapLine_8bpp( atGRPX_PIXEL *ptr, atGRPX_BITMAP *lpBitmap, int width, atREAL x, atREAL y, atREAL dx, atREAL dy, atLONG ExParam1 )
{
    int i;
    int xx, yy;
    atBYTE *img_ptr = (atBYTE *)lpBitmap->lpBits;
//    int bmWidth = lpBitmap->nWidth;
//    int bmHeight = lpBitmap->nHeight;
    int bmWidthBytes = lpBitmap->nWidthBytes;
    atBYTE cl;
    atGRPX_COLOR cc;
    atBYTE r0, g0, b0;
    atBYTE rr, gg, bb;
    atBYTE weight2;
	atGRPX_PIXEL color = COLOR_TO_PIXEL( (atGRPX_COLOR)ExParam1 );

    r0 = GET_RED(color); g0 = GET_GREEN(color); b0 = GET_BLUE(color);

    for( i = 0; i < width; i++ ){
      xx = REALTOINT(x);
      yy = REALTOINT(y);
	  if( (xx >= 0) && (xx < lpBitmap->nWidth) && (yy >= 0) && (yy < lpBitmap->nHeight) ){ // 불안해서 넣을까 말까 고민 중.
        cl = *(img_ptr + yy*bmWidthBytes + xx);
        if( cl == 0xff ){
          *ptr = color;
        } else if( cl != 0x00 ){
          weight2 = 255 - cl;
          __ALPHA_PIXEL_I( ptr, r0,g0,b0, cc,rr,gg,bb, cl, weight2 );
        }
    }
      ptr++;
      x += dx;
      y += dy;
   }
}

static void _DrawPixel_8bpp( atGRPX_PIXEL *ptr, atGRPX_BITMAP *lpBitmap, atINT X, atINT Y, atLONG ExParam1 )
{
    atBYTE *img_ptr = (atBYTE *)lpBitmap->lpBits;
    atBYTE cl;
	atGRPX_PIXEL cc;
    atBYTE r0, g0, b0;
    atBYTE rr, gg, bb;
    atINT weight2;
	atGRPX_COLOR color = (atGRPX_COLOR)ExParam1;

    r0 = GET_RED(color); g0 = GET_GREEN(color); b0 = GET_BLUE(color);

    cl = *(img_ptr + Y*lpBitmap->nWidthBytes + X);
    weight2 = 255 - cl;
    __ALPHA_PIXEL_I( ptr, r0,g0,b0, cc,rr,gg,bb, cl, weight2 );
}


/*****************************
  이미지를 회전하여 Glyph으로 출력한다.
******************************/
void atGRPX_DrawGlyphRotate( atGRPX_CANVAS *lpCanvas, atGRPX_BITMAP *lpBitmap, atINT X, atINT Y, atGRPX_COLOR Color, atREAL Angle )
{
	if( lpBitmap->nBPP == 1 ){
		__atGRPX_DrawBitmapRotate0( lpCanvas, lpBitmap, X, Y, Angle, (atLONG)Color, _DrawPixel_1bpp, _DrawBitmapLine_1bpp );
	} else if( lpBitmap->nBPP == 8 ){
		__atGRPX_DrawBitmapRotate0( lpCanvas, lpBitmap, X, Y, Angle, (atLONG)Color, _DrawPixel_8bpp, _DrawBitmapLine_8bpp );
	}
}

