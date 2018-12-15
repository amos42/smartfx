/*#############################################################################
-------------------------------------------------------------------------------

                                  PUTIMG.c
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
#include "_IMG_func.h"
#include "../Graphic/_AB_int.h"


/********************
  이미지를 출력한다.
*********************/
static void _DrawBitmap( atGRPX_CANVAS *lpCanvas, atGRPX_BITMAP *lpBitmap, atINT X, atINT Y, atINT ImageX, atINT ImageY, atINT ImageWidth, atINT ImageHeight, atLONG ExParam1 )
{
    atGRPX_PIXEL *ScrPtr, *ImgPtr, pixel;
    int i, j;
    int sskip, iskip;
    atGRPX_PIXEL pixel2;
    int S_Red, S_Green, S_Blue;
    int T_Red, T_Green, T_Blue;
	int nAlpha = ExParam1;
	int nAlpha2;
    atGRPX_PIXEL trans_color;

    ScrPtr = (atGRPX_PIXEL *)atGRPX_GetFrameBufferPtrXY( lpCanvas, X, Y );
    ImgPtr = (atGRPX_PIXEL *)atGRPX_GetBitmapPtrXY( lpBitmap, ImageX, ImageY );

    sskip = atGRPX_GetVMemWidth(lpCanvas) - ImageWidth;
    iskip = lpBitmap->nWidth - ImageWidth;

	nAlpha2 = 0xFF - nAlpha;

    if( lpBitmap->dwAttr & atGRPX_BMATTR_TRANSPARENT ){
        trans_color = COLOR_TO_PIXEL(lpBitmap->clTransColor);
        if( nAlpha == (0xFF / 2) ){
        	for( i = 0; i < ImageHeight; i ++ ){
        	  for( j = 0; j < ImageWidth; j ++ ){
                pixel = *ImgPtr++;
                if( pixel != trans_color ){
        			__ALPHA_PIXEL_50P(ScrPtr, pixel);
        		}
        		ScrPtr ++;
        	  }
        
        	  ScrPtr += sskip;
        	  ImgPtr += iskip;
        	}
        } else {
        	for( i = 0; i < ImageHeight; i ++ ){
        	  for( j = 0; j < ImageWidth; j ++ ){
                pixel = *ImgPtr++;
        		if( pixel != trans_color ){
        			S_Red = GET_RED(pixel); S_Green = GET_GREEN(pixel); S_Blue = GET_BLUE(pixel);
        			__ALPHA_PIXEL_I(ScrPtr, S_Red,S_Green,S_Blue, pixel2,T_Red,T_Green,T_Blue, nAlpha, nAlpha2 );
        		}
        		ScrPtr ++;
        	  }
        
        	  ScrPtr += sskip;
        	  ImgPtr += iskip;
        	}
        }
    } else {
        if( nAlpha == (0xFF / 2) ){
        	for( i = 0; i < ImageHeight; i ++ ){
        	  for( j = 0; j < ImageWidth; j ++ ){
                pixel = *ImgPtr++;
       			__ALPHA_PIXEL_50P(ScrPtr, pixel);
        		ScrPtr ++;
        	  }
        
        	  ScrPtr += sskip;
        	  ImgPtr += iskip;
        	}
        } else {
        	for( i = 0; i < ImageHeight; i ++ ){
        	  for( j = 0; j < ImageWidth; j ++ ){
                pixel = *ImgPtr++;
       			S_Red = GET_RED(pixel); S_Green = GET_GREEN(pixel); S_Blue = GET_BLUE(pixel);
       			__ALPHA_PIXEL_I(ScrPtr, S_Red,S_Green,S_Blue, pixel2,T_Red,T_Green,T_Blue, nAlpha, nAlpha2 );
        		ScrPtr ++;
        	  }
        
        	  ScrPtr += sskip;
        	  ImgPtr += iskip;
        	}
        }
    }
}


/********************
  이미지를 일부만 출력한다.
*********************/
void atGRPX_DrawBitmapTransPart( atGRPX_CANVAS *lpCanvas, atGRPX_BITMAP *lpBitmap, atINT X, atINT Y, atINT ImageX, atINT ImageY, atINT ImageWidth, atINT ImageHeight, atINT nAlpha )
{
	if( nAlpha == 0 ) return;
	if( nAlpha == atGRPX_MAX_ALPHA ){
		atGRPX_DrawBitmapPart( lpCanvas, lpBitmap, X, Y, ImageX, ImageY, ImageWidth, ImageHeight );
		return;
	}

    __atGRPX_DrawBitmapPart0( lpCanvas, lpBitmap, X, Y, ImageX, ImageY, ImageWidth, ImageHeight, (atLONG)nAlpha, _DrawBitmap );
}


/********************
  이미지를 출력한다.
*********************/
void atGRPX_DrawBitmapTrans( atGRPX_CANVAS *lpCanvas, atGRPX_BITMAP *lpBitmap, atINT X, atINT Y, atINT nAlpha )
{
    atGRPX_DrawBitmapTransPart( lpCanvas, lpBitmap, X, Y, 0, 0, lpBitmap->nWidth, lpBitmap->nHeight, nAlpha );
}
