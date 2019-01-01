/*#############################################################################
-------------------------------------------------------------------------------

                                  putPixel.c
                           (* =================== *)


 Version           : 1.0.0
 Dependent Modules :
 Author            : Ju, Gyeong-min ( Amos )
 Create            : 2001/04/25    01:47:09am
 Update            : 2003/04/29    00:00:00am
 C Compiler        : ADS 1.01, SDT 2.51, Borland C/C++ 5.0, Visual C/C++ 6.0
 Assembler         : ADS 1.01, SDT 2.51, Borland TASM 3.1, MASM 6.0
 Description       : PutPixel related function definition

-------------------------------------------------------------------------------
#############################################################################*/
#include "GrpX.h"
#include "_AA_func.h"


/*********************************
  ÁöÁ¤µÈ ÁÂÇ¥ÀÇ Á¡À» Âï´Â´Ù.
**********************************/
void atGRPX_DrawPixelAA_One( atGRPX_CANVAS *lpCanvas, atINT nX, atINT nY, atREAL rDX, atREAL rDY, atGRPX_COLOR Color )
{
    atRECT *clip;
    atGRPX_PIXEL *pBuf;
    int r, g, b;

    if( rDX == INTTOREAL(0) && rDY == INTTOREAL(0) ){
	 atGRPX_DrawPixel( lpCanvas, nX, nY, Color );
	 return;
    }
	
    clip = atGRPX_GetClipRect(lpCanvas);

    if( nX < clip->nStartX || nX > clip->nEndX ||
        nY < clip->nStartY || nY > clip->nEndY ) return;

    pBuf = atGRPX_GetFrameBufferPtrXY( lpCanvas, nX, nY );
	r = GET_RED(Color); g = GET_GREEN(Color); b = GET_BLUE(Color);

    __AA_DrawPixelAA_One( pBuf, rDX, rDY, r, g, b );
}


/*********************************
  ÁöÁ¤µÈ ÁÂÇ¥ÀÇ Á¡À» Âï´Â´Ù.
**********************************/
void atGRPX_DrawPixelAA( atGRPX_CANVAS *lpCanvas, atINT nX, atINT nY, atREAL rDX, atREAL rDY, atGRPX_COLOR Color )
{
    atRECT *clip;
    atGRPX_PIXEL *pBuf;
    int r, g, b;

    if( rDX == INTTOREAL(0) && rDY == INTTOREAL(0) ){
	 atGRPX_DrawPixel( lpCanvas, nX, nY, Color );
	 return;
    }
	
    clip = atGRPX_GetClipRect(lpCanvas);

    if( nX < clip->nStartX || nX > clip->nEndX ||
        nY < clip->nStartY || nY > clip->nEndY ) return;

    pBuf = atGRPX_GetFrameBufferPtrXY( lpCanvas, nX, nY );
	r = GET_RED(Color); g = GET_GREEN(Color); b = GET_BLUE(Color);

    if( rDX != INTTOREAL(0) && rDY != INTTOREAL(0) )
    	__AA_DrawPixelAA( pBuf, atGRPX_GetVMemWidth(lpCanvas), nX, nY, rDX, rDY, r, g, b );
    else if( rDX != INTTOREAL(0) )
    	__AA_DrawPixelAA_H( pBuf, rDX, r, g, b, nX, clip );
    else
    	__AA_DrawPixelAA_V( pBuf, atGRPX_GetVMemWidth(lpCanvas), rDY, r, g, b, nY, clip );
}


/*********************************
  ÁöÁ¤µÈ ÁÂÇ¥ÀÇ Á¡À» Âï´Â´Ù.
**********************************/
void atGRPX_DrawPixelRealAA( atGRPX_CANVAS *lpCanvas, atREAL rX, atREAL rY, atGRPX_COLOR Color )
{
/*
    atRECT *clip;
    int X, Y;
    atGRPX_PIXEL *pBuf;
    int r, g, b;

    X = REALTOINT(rX); Y = REALTOINT(rY);
    clip = atGRPX_GetClipRect(lpCanvas);

    if( X < clip->nStartX || X > clip->nEndX ||
        Y < clip->nStartY || Y > clip->nEndY ) return;

    pBuf = atGRPX_GetFrameBufferPtrXY( lpCanvas, X, Y );
	r = GET_RED(Color); g = GET_GREEN(Color); b = GET_BLUE(Color);
    
//    __AA_DrawPixelRealAA( pBuf, atGRPX_GetVMemWidth(lpCanvas), rX, rY, r, g, b );
    __AA_DrawPixelRealAA_One( pBuf, rX, rY, r, g, b );
*/
	atGRPX_DrawPixelAA( lpCanvas, REALTOINT(rX), REALTOINT(rY), REALREALK(rX), REALREALK(rY), Color );
}


