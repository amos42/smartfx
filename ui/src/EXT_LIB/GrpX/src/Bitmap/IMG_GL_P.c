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
#include "../Graphic/_AB_int.h"
#include "_IMG_func.h"


void _DrawGlyph_1bpp( atGRPX_CANVAS *lpCanvas, atGRPX_BITMAP *lpBitmap, atINT X, atINT Y, atINT ImageX, atINT ImageY, atINT ImageWidth, atINT ImageHeight, atLONG ExParam1 )
{
    atGRPX_PIXEL *ScrPtr;
    atBYTE *ImgPtr, cl;
    int i, j, k;
    int sskip, iskip;
    int width1, width2_b, width3;
	atGRPX_PIXEL color = (atGRPX_PIXEL)ExParam1;

    ScrPtr = (atGRPX_PIXEL *)atGRPX_GetFrameBufferPtrXY( lpCanvas, X, Y );
    ImgPtr = (atBYTE *)atGRPX_GetBitmapPtrXY( lpBitmap, ImageX, ImageY );

    sskip = atGRPX_GetVMemWidth(lpCanvas) - ImageWidth;

	width1 = ( (ImageX & 0x7) > 0 )? 8-(ImageX & 0x7) : 0;
	ImageWidth -= width1;
	width2_b = ImageWidth >> 3;
	width3 = ImageWidth & 0x7;

	iskip = lpBitmap->nWidthBytes - width2_b;
	if( width1 > 0 ) iskip --;
	if( width3 > 0 ) iskip --;

	for( i = 0; i < ImageHeight; i ++ ){
	  if( width1 > 0 ){
        cl = *ImgPtr++;
        for( k = 8-width1; k < 8; k++ ){
	        if( cl & (0x80 >> k) ) *ScrPtr = color;
		    ScrPtr ++;
        }
	  }

	  for( j = 0; j < width2_b; j ++ ){
        cl = *ImgPtr++;
        for( k = 0; k < 8; k++ ){
	        if( cl & (0x80 >> k) ) *ScrPtr = color;
		    ScrPtr ++;
        }
	  }

	  if( width3 > 0 ){
        cl = *ImgPtr++;
        for( k = 0; k < width3; k++ ){
	        if( cl & (0x80 >> k) ) *ScrPtr = color;
		    ScrPtr ++;
        }
	  }

	  ScrPtr += sskip;
	  ImgPtr += iskip;
	}
}


void _DrawGlyph_8bpp( atGRPX_CANVAS *lpCanvas, atGRPX_BITMAP *lpBitmap, atINT X, atINT Y, atINT ImageX, atINT ImageY, atINT ImageWidth, atINT ImageHeight, atLONG ExParam1 )
{
    atGRPX_PIXEL *ScrPtr;
    atBYTE *ImgPtr, cl;
    int i, j;
	atGRPX_PIXEL cc;
    int sskip, iskip;
    atBYTE r0, g0, b0;
    atBYTE rr, gg, bb;
    atINT weight2;
	atGRPX_PIXEL color = (atGRPX_PIXEL)ExParam1;

    ScrPtr = (atGRPX_PIXEL *)atGRPX_GetFrameBufferPtrXY( lpCanvas, X, Y );
    ImgPtr = (atBYTE *)atGRPX_GetBitmapPtrXY( lpBitmap, ImageX, ImageY );

    sskip = atGRPX_GetVMemWidth(lpCanvas) - ImageWidth;
	iskip = lpBitmap->nWidthBytes - ImageWidth;

    r0 = GET_RED(color); g0 = GET_GREEN(color); b0 = GET_BLUE(color);

	for( i = 0; i < ImageHeight; i ++ ){
	  for( j = 0; j < ImageWidth; j ++ ){
        cl = *ImgPtr++;
        if( cl == 0xff ){
          *ScrPtr = color;
        } else if( cl != 0x00 ){
          weight2 = 255 - cl;
          __ALPHA_PIXEL_I( ScrPtr, r0,g0,b0, cc,rr,gg,bb, cl, weight2 );
        }
	    ScrPtr ++;
	  }

	  ScrPtr += sskip;
	  ImgPtr += iskip;
	}
}


/*****************************
  이미지의 일부를 Glyph으로 출력한다.
******************************/
void  atGRPX_DrawGlyphPart( atGRPX_CANVAS *lpCanvas, atGRPX_BITMAP *lpBitmap, atINT X, atINT Y, atGRPX_COLOR Color, atINT iStartPos, atINT nWidth )
{
	if( lpBitmap == atNULL ) return;

	if( lpBitmap->nBPP == 1 )
		__atGRPX_DrawBitmapPart0( lpCanvas, lpBitmap, X, Y, iStartPos, 0, nWidth, lpBitmap->nHeight, (atLONG)Color, _DrawGlyph_1bpp );
	else if( lpBitmap->nBPP == 8 )
		__atGRPX_DrawBitmapPart0( lpCanvas, lpBitmap, X, Y, iStartPos, 0, nWidth, lpBitmap->nHeight, (atLONG)Color, _DrawGlyph_8bpp );
}

