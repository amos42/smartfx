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


static void __fnDrawBitmap( atGRPX_CANVAS *lpCanvas, atGRPX_BITMAP *lpBitmap, atINT X, atINT Y, atINT tWidth, atINT tHeight, atINT ImageX, atINT ImageY, atREAL dx, atREAL dy, atLONG ExParam1 )
{
    int i, j;
    int width, height;
    atREAL xx, yy;
    atGRPX_PIXEL *ScrPtr, *ImgPtr, *sa;
    atGRPX_PIXEL pixel, trans_color;
    int sskip;
    atINT nAlpha = (atINT)ExParam1;

    ImgPtr = atGRPX_GetBitmapPtrXY( lpBitmap, ImageX, ImageY );
    ScrPtr = atGRPX_GetFrameBufferPtrXY( lpCanvas, X, Y );

	width = lpBitmap->nWidth;
	height = lpBitmap->nHeight;
    sskip = atGRPX_GetVMemWidth(lpCanvas) - tWidth;
    yy = INTTOREAL(0);

    if( lpBitmap->dwAttr & atGRPX_BMATTR_TRANSPARENT ){
        trans_color = COLOR_TO_PIXEL(lpBitmap->clTransColor);
        for( i = 0; i < tHeight; i ++ ){
          sa = ImgPtr + REALTOINT(yy) * width;
    
          xx = INTTOREAL(0);
          for( j = 0; j < tWidth; j ++ ){
            pixel = *(sa + REALTOINT(xx));

            if( pixel != trans_color )    
    		    atGRPX_DrawPixelTrans( lpCanvas, X+j, Y+i, pixel, nAlpha ); // 일단 임시로...
    		ScrPtr ++;        
    
            xx += dx;
          }
    
          ScrPtr += sskip;
          yy += dy;
        }
    } else {
        for( i = 0; i < tHeight; i ++ ){
          sa = ImgPtr + REALTOINT(yy) * width;
    
          xx = 0;
          for( j = 0; j < tWidth; j ++ ){
            pixel = *(sa + REALTOINT(xx));
    
    		atGRPX_DrawPixelTrans( lpCanvas, X+j, Y+i, pixel, nAlpha ); // 일단 임시로...
    		ScrPtr ++;        
    
            xx += dx;
          }
    
          ScrPtr += sskip;
          yy += dy;
        }
    }
}



/***********************

************************/
void atGRPX_DrawBitmapStretchTrans( atGRPX_CANVAS *lpCanvas, atGRPX_BITMAP *lpBitmap, atINT X, atINT Y, atINT tWidth, atINT tHeight, atINT nAlpha )
{
	if( nAlpha == 0 ) return;
	if( nAlpha == atGRPX_MAX_ALPHA ){
		atGRPX_DrawBitmapStretch( lpCanvas, lpBitmap, X, Y, tWidth, tHeight );
		return;
	}

    __atGRPX_DrawBitmapStretch0( lpCanvas, lpBitmap, X, Y, tWidth, tHeight, (atLONG)nAlpha, __fnDrawBitmap );
}


/***********************

************************/
void atGRPX_DrawBitmapScaleTrans( atGRPX_CANVAS *lpCanvas, atGRPX_BITMAP *lpBitmap, atINT X, atINT Y, atINT Scale, atINT nAlpha )
{
    int width, height;
    atREAL rScale;

    width  = lpBitmap->nWidth;
    height = lpBitmap->nHeight;

    rScale = INTTOREAL(Scale) / 100;
    width  = REALTOINT( width * rScale );
    height = REALTOINT( height * rScale );

    if( width == 0 || height == 0 ) return;

    atGRPX_DrawBitmapStretchTrans( lpCanvas, lpBitmap, X, Y, width, height, nAlpha );
}

