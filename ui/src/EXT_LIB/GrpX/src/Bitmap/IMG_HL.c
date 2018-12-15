/*#############################################################################
-------------------------------------------------------------------------------

                                  IMG.c
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
//#define __X86_

#include "GrpX.h"
#include "_IMG_func.h"


#if 0

static void _DrawBitmap( atGRPX_CANVAS *lpCanvas, atGRPX_BITMAP *lpBitmap, atINT X, atINT Y, atINT ImageX, atINT ImageY, atINT ImageWidth, atINT ImageHeight, atLONG ExParam1 )
{
    atGRPX_PIXEL *ScrPtr, *ImgPtr, pixel;
    int i, j;
    int sskip, iskip;

    ScrPtr = (atGRPX_PIXEL *)atGRPX_GetFrameBufferPtrXY( lpCanvas, X, Y );
    ImgPtr = (atGRPX_PIXEL *)atGRPX_GetBitmapPtrXY( lpBitmap, ImageX, ImageY );

    sskip = atGRPX_GetVMemWidth(lpCanvas) - ImageWidth;
    iskip = lpBitmap->nWidth - ImageWidth;

	for( i = 0; i < ImageHeight; i ++ ){
	  for( j = 0; j < ImageWidth; j ++ ){
		pixel = *ImgPtr++;
		if( pixel != TRANS_COLOR ) *ScrPtr = pixel;
		ScrPtr ++;
	  }

	  ScrPtr += sskip;
	  ImgPtr += iskip;
	}
}



/********************
  이미지를 일부만 출력한다.
*********************/
void atGRPX_DrawBitmapPart( atGRPX_CANVAS *lpCanvas, atGRPX_BITMAP *lpBitmap, atINT X, atINT Y, atINT ImageX, atINT ImageY, atINT ImageWidth, atINT ImageHeight )
{
    __atGRPX_DrawBitmapPart0( lpCanvas, lpBitmap, X, Y, ImageX, ImageY, ImageWidth, ImageHeight, (atLONG)0, _DrawBitmap );
}


/********************
  이미지를 출력한다.
*********************/
void atGRPX_DrawBitmap( atGRPX_CANVAS *lpCanvas, atGRPX_BITMAP *lpBitmap, atINT X, atINT Y )
{
    __atGRPX_DrawBitmapPart0( lpCanvas, lpBitmap, X, Y, 0, 0, lpBitmap->nWidth, lpBitmap->nHeight, (atLONG)0, _DrawBitmap );
}


#endif