/*#############################################################################
-------------------------------------------------------------------------------

                                  GETIMG.c
                           (* =================== *)


 Version           : 1.0.0
 Dependent Modules :
 Author            : Ju, Gyeong-min ( Amos )
 Create            : 2001/04/25    01:47:09am
 Update            : 2003/04/29    00:00:00am
 C Compiler        : ADS 1.01, SDT 2.51, Borland C/C++ 5.0, Visual C/C++ 6.0
 Assembler         : ADS 1.01, SDT 2.51, Borland TASM 3.1, MASM 6.0
 Description       : Get image function definition

-------------------------------------------------------------------------------
#############################################################################*/
//#define __X86_

#include "GrpX.h"


/***********************

************************/
void atGRPX_GetBitmap( atGRPX_CANVAS *lpCanvas, atGRPX_BITMAP *lpBitmap, atINT X, atINT Y )
{
    int i;
    atGRPX_PIXEL *ScrPtr, *ImgPtr;
    int width, height;
    int skip;


    width = lpBitmap->nWidth;
    height = lpBitmap->nHeight;

    if( width == 0 || height == 0 ) return;

    ScrPtr = atGRPX_GetFrameBufferPtrXY( lpCanvas, X + lpBitmap->nLogX, Y + lpBitmap->nLogY );
    ImgPtr = lpBitmap->lpBits;
	skip = atGRPX_GetVMemWidth(lpCanvas) - width;

/*
    t2 = width & 0x03;
    width >>= 2;

    while( height -- ){
      for( i = 0; i < width; i ++ ){
        *((long *)ImgPtr) = *((long *)ScrPtr);
        ImgPtr += sizeof(long);
        ScrPtr += sizeof(long);
      }

      for( i = 0; i < t2; i ++ )
        *ImgPtr++ = *ScrPtr++;

      ScrPtr += skip;
    }
*/    
    while( height -- ){
      for( i = 0; i < width; i ++ ){
        *ImgPtr++ = *ScrPtr++;
      }
      ScrPtr += skip;
    }
}

