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
//#define __X86_

#include "GrpX.h"
#include "_IMG_func.h"


#define MAKE565(r,g,b) ((atWORD)(((((atWORD)((r) & 0xF8)) << (5+6-3))) | \
                       ((((atWORD)((g) & 0xFC)) << (5-2))) | \
                       (((atWORD)((b) & 0xF8)) >> 3)))



static void _DrawBitmap( atGRPX_CANVAS *lpCanvas, atGRPX_BITMAP *lpBitmap, atINT X, atINT Y, atINT ImageX, atINT ImageY, atINT ImageWidth, atINT ImageHeight, atLONG ExParam1 )
{
    atGRPX_PIXEL *ScrPtr, *ImgPtr, pixel;
    int i, j;
    int sskip, iskip;
//    atGRPX_PIXEL trans_color;
#if atGRPX_PIXEL_BPP == 16
    atGRPX_PIXEL pixel2;
    int S_Red, S_Green, S_Blue;
    int T_Red, T_Green, T_Blue;
    int R_Red, R_Green, R_Blue;
	int S_Alpha, S_Alpha2;
#endif    
    atBYTE *pAlphaChannel = (atBYTE *)(atINTPTR)ExParam1;
	if( pAlphaChannel == atNULL ) return;

    ScrPtr = (atGRPX_PIXEL *)atGRPX_GetFrameBufferPtrXY( lpCanvas, X, Y );
    ImgPtr = (atGRPX_PIXEL *)atGRPX_GetBitmapPtrXY( lpBitmap, ImageX, ImageY );
    if( (ScrPtr == atNULL) || (ImgPtr == atNULL) ) return;
	
    pAlphaChannel = pAlphaChannel + ImageY * lpBitmap->nWidth + ImageX;

    sskip = atGRPX_GetVMemWidth(lpCanvas) - ImageWidth;
    iskip = lpBitmap->nWidth - ImageWidth;

    for( i = 0; i < ImageHeight; i ++ ){
      for( j = 0; j < ImageWidth; j ++ ){
#if atGRPX_PIXEL_BPP == 8
		pixel = *ImgPtr++;
		ScrPtr ++;
#elif atGRPX_PIXEL_BPP == 16
        S_Alpha = *pAlphaChannel++;
        pixel = *ImgPtr++;
		if( S_Alpha == 0x00 ){
			ScrPtr++; continue;
		}
		if( S_Alpha == 0xFF ){
			*ScrPtr++ = pixel; continue;
		}
        S_Red = (pixel >> 8) & 0xF8;
        S_Green = (pixel >> 3) & 0xFC;
        S_Blue = (pixel << 3) & 0xF8;

        pixel2 = *ScrPtr;
        T_Red = (pixel2 >> 8) & 0xF8;
        T_Green = (pixel2 >> 3) & 0xFC;
        T_Blue = (pixel2 << 3) & 0xF8;
        
        S_Alpha2 = 255-S_Alpha;

        R_Red   = (S_Red * S_Alpha + T_Red * S_Alpha2) >> 8;
        R_Green = (S_Green * S_Alpha + T_Green * S_Alpha2) >> 8;
        R_Blue  = (S_Blue * S_Alpha + T_Blue * S_Alpha2) >> 8;
        
		*ScrPtr++ = MAKE565(R_Red, R_Green, R_Blue);
#endif
	  }
    
   	  ScrPtr += sskip;
      ImgPtr += iskip;
      pAlphaChannel += iskip;
    }
}



/********************
  Alpha Channel을 적용하여
  이미지를 출력한다.
*********************/
atVOID  atGRPX_DrawBitmapPartWithAlphaChannel( atGRPX_CANVAS *lpCanvas, atGRPX_BITMAP *lpBitmap, atINT X, atINT Y, atINT ImageX, atINT ImageY, atINT ImageWidth, atINT ImageHeight, atBYTE *pAlphaChannel )
{
    if( lpBitmap == atNULL ) return;
	if( pAlphaChannel == atNULL ) pAlphaChannel = lpBitmap->lpAlphaChannel;
    __atGRPX_DrawBitmapPart0( lpCanvas, lpBitmap, X, Y, ImageX, ImageY, ImageWidth, ImageHeight, (atINTPTR)pAlphaChannel, _DrawBitmap );
}


/***********************

************************/
atVOID atGRPX_DrawBitmapWithAlphaChannel( atGRPX_CANVAS *lpCanvas, atGRPX_BITMAP *lpBitmap, atINT X, atINT Y, atBYTE *pAlphaChannel )
{
    if( lpBitmap == atNULL ) return;
	if( pAlphaChannel == atNULL ) pAlphaChannel = lpBitmap->lpAlphaChannel;
    atGRPX_DrawBitmapPartWithAlphaChannel( lpCanvas, lpBitmap, X, Y, 0, 0, lpBitmap->nWidth, lpBitmap->nHeight, pAlphaChannel );
}


/***********************

************************/
atVOID atGRPX_DrawBitmapStretchWithAlphaChannel( atGRPX_CANVAS *lpCanvas, atGRPX_BITMAP *lpBitmap, atINT X, atINT Y, atINT tWidth, atINT tHeight, atBYTE *pAlphaChannel )
{
    int i, j, t;
    atGRPX_PIXEL *ScrPtr, *ImgPtr, *sa, pixel;
    atBYTE *ap;
#if atGRPX_PIXEL_BPP == 16
    atGRPX_PIXEL pixel2;
    int S_Red, S_Green, S_Blue;
    int T_Red, T_Green, T_Blue;
    int R_Red, R_Green, R_Blue;
	int S_Alpha, S_Alpha2;
#endif    
    atRECT *clip;
    int width, height;
    atREAL xx, yy;
    atREAL dx, dy;
    int sskip;

    if( (lpBitmap == atNULL) ) return;
    if( pAlphaChannel == atNULL ) pAlphaChannel = lpBitmap->lpAlphaChannel;
    if( (pAlphaChannel == atNULL) ) return;
	
    width = lpBitmap->nWidth;
    height = lpBitmap->nHeight;

    if( width == 0 || height == 0 || tWidth == 0 || tHeight == 0 ) return;

    dx = INTTOREAL(width)  / tWidth;
    dy = INTTOREAL(height) / tHeight;

    X += lpBitmap->nLogX * tWidth  / width;
    Y += lpBitmap->nLogY * tHeight / height;

    ImgPtr = lpBitmap->lpBits;
//    pAlphaChannel = pAlphaChannel;

    clip = atGRPX_GetClipRect(lpCanvas);

    if( Y > clip->nEndY ) return;
    if( Y < clip->nStartY ){
      tHeight -= clip->nStartY - Y;
      ImgPtr += REALTOINT( (clip->nStartY - Y) * dy ) * width;
      pAlphaChannel  += REALTOINT( (clip->nStartY - Y) * dy ) * width;
      Y = clip->nStartY;
    }
    t = Y + tHeight - 1;
    if( t < clip->nStartY ) return;
    if( t > clip->nEndY ){
      tHeight -= t - clip->nEndY;
    }

    if( X > clip->nEndX ) return;
    if( X < clip->nStartX ){
      tWidth -= clip->nStartX - X;
      ImgPtr += REALTOINT( (clip->nStartX - X) * dx );
      pAlphaChannel  += REALTOINT( (clip->nStartX - X) * dx );
      X = clip->nStartX;
    }
    t = X + tWidth - 1;
    if( t < clip->nStartX ) return;
    if( t > clip->nEndX ){
      tWidth -= t - clip->nEndX;
    }

    ScrPtr = atGRPX_GetFrameBufferPtrXY( lpCanvas, X, Y );

    sskip = atGRPX_GetVMemWidth(lpCanvas) - tWidth;
    yy = 0;

    for( i = 0; i < tHeight; i ++ ){
      sa = ImgPtr + REALTOINT(yy) * width;
      ap = pAlphaChannel + REALTOINT(yy) * width;

      xx = 0;
      for( j = 0; j < tWidth; j ++ ){
#if atGRPX_PIXEL_BPP == 8
        	S_Alpha = *(ap + REALTOINT(xx));
		if( S_Alpha == 0x00 ){
			ScrPtr++; continue;
		}
		pixel = *(sa + REALTOINT(xx));
		if( S_Alpha == 0xFF ){
			*ScrPtr++ = pixel; continue;
		}
		
		// 
		
		ScrPtr ++;
#elif atGRPX_PIXEL_BPP == 16
        S_Alpha = *(ap + REALTOINT(xx));
		if( S_Alpha > 0x00 ){
			pixel = *(sa + REALTOINT(xx));
			if( S_Alpha == 0xFF ){
				*ScrPtr = pixel;
			} else if( S_Alpha == 0xFF/2 ){
				S_Red = (pixel >> 8) & 0xF8;
				S_Green = (pixel >> 3) & 0xFC;
				S_Blue = (pixel << 3) & 0xF8;

				pixel2 = *ScrPtr;
				T_Red = (pixel2 >> 8) & 0xF8;
				T_Green = (pixel2 >> 3) & 0xFC;
				T_Blue = (pixel2 << 3) & 0xF8;
        
				S_Alpha2 = 255-S_Alpha;

				R_Red   = (S_Red * S_Alpha + T_Red * S_Alpha2) >> 8;
				R_Green = (S_Green * S_Alpha + T_Green * S_Alpha2) >> 8;
				R_Blue  = (S_Blue * S_Alpha + T_Blue * S_Alpha2) >> 8;
        
				*ScrPtr = MAKE565(R_Red, R_Green, R_Blue);
			} else {
				S_Red = (pixel >> 8) & 0xF8;
				S_Green = (pixel >> 3) & 0xFC;
				S_Blue = (pixel << 3) & 0xF8;

				pixel2 = *ScrPtr;
				T_Red = (pixel2 >> 8) & 0xF8;
				T_Green = (pixel2 >> 3) & 0xFC;
				T_Blue = (pixel2 << 3) & 0xF8;
        
				S_Alpha2 = 255-S_Alpha;

				R_Red   = (S_Red * S_Alpha + T_Red * S_Alpha2) >> 8;
				R_Green = (S_Green * S_Alpha + T_Green * S_Alpha2) >> 8;
				R_Blue  = (S_Blue * S_Alpha + T_Blue * S_Alpha2) >> 8;
        
				*ScrPtr = MAKE565(R_Red, R_Green, R_Blue);
			}
		}
#endif

		ScrPtr++;
        xx += dx;
      }

      ScrPtr += sskip;
      yy += dy;
    }
}


/***********************

************************/
void atGRPX_DrawBitmapScaleWithAlphaChannel( atGRPX_CANVAS *lpCanvas, atGRPX_BITMAP *lpBitmap, atINT X, atINT Y, atINT Scale, atBYTE *pAlphaChannel )
{
    int width, height;
    atREAL rScale;

    if( (lpBitmap == atNULL) ) return;
	
    width  = lpBitmap->nWidth;
    height = lpBitmap->nHeight;

    rScale = INTTOREAL(Scale) / 100;
    width  = REALTOINT( width * rScale );
    height = REALTOINT( height * rScale );

    if( width == 0 || height == 0 ) return;

    atGRPX_DrawBitmapStretchWithAlphaChannel( lpCanvas, lpBitmap, X, Y, width, height, pAlphaChannel );
}

