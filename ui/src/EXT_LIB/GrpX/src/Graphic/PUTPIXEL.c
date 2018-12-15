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
#include "_AB_int.h"


/*********************************
  지정된 좌표의 점을 찍는다.
**********************************/
atGRPX_PIXEL *atGRPX_DrawPixel( atGRPX_CANVAS *lpCanvas, atINT X, atINT Y, atGRPX_COLOR Color )
{
    atRECT *clip;
	atGRPX_PIXEL *ptr;

    clip = atGRPX_GetClipRect(lpCanvas);

    if( X < clip->nStartX || X > clip->nEndX ||
        Y < clip->nStartY || Y > clip->nEndY ) return atNULL;

	ptr = atGRPX_GetFrameBufferPtrXY( lpCanvas, X, Y );
	
    *ptr = COLOR_TO_PIXEL(Color);

	return ptr;
}


/*********************************
  지정된 좌표의 점을 찍는다.
**********************************/
atGRPX_PIXEL *atGRPX_DrawPixelReal( atGRPX_CANVAS *lpCanvas, atREAL rX, atREAL rY, atGRPX_COLOR Color )
{
     return atGRPX_DrawPixel( lpCanvas, REALTOINT(rX), REALTOINT(rY), Color );
}


/*********************************
  지정된 좌표에 알파값을 적용하여 점을 찍는다.
**********************************/
atGRPX_PIXEL *atGRPX_DrawPixelTrans( atGRPX_CANVAS *lpCanvas, atINT X, atINT Y, atGRPX_COLOR Color, atINT nAlpha )
{
	atRECT *clip;
	atGRPX_PIXEL *ptr;
	int cl, r1, g1, b1, r2, g2, b2;
	atINT nAlpha2;
	
	if( nAlpha == 0 ) return atNULL;
	if( nAlpha == 0xFF ){
		atGRPX_DrawPixel(lpCanvas, X, Y, Color );
		return atNULL;
	}

	clip = atGRPX_GetClipRect( lpCanvas );
	if( X < clip->nStartX || X > clip->nEndX || Y < clip->nStartY || Y > clip->nEndY ) return atNULL;

//	atGRPX_DrawPixelTransR( lpCanvas, X, Y, Color, INTTOREAL(nAlpha) / atGRPX_MAX_ALPHA );

	ptr = atGRPX_GetFrameBufferPtrXY( lpCanvas, X, Y );
	if( nAlpha == (0xFF / 2) ){
		__ALPHA_PIXEL_50P(ptr, (atGRPX_PIXEL)Color);
	} else {
		nAlpha2 = 0xFF - nAlpha;
		r1 = GET_RED(Color); g1 = GET_GREEN(Color); b1 = GET_BLUE(Color);
		__ALPHA_PIXEL_I(ptr, r1,g1,b1, cl,r2,g2,b2, nAlpha,nAlpha2);
	}

	return ptr;
}


/*********************************
  지정된 좌표에 알파값을 적용하여 점을 찍는다.
**********************************/
atGRPX_PIXEL *atGRPX_DrawPixelTransR( atGRPX_CANVAS *lpCanvas, atINT X, atINT Y, atGRPX_COLOR Color, atREAL rAlpha )
{
	atRECT *clip;
	atGRPX_PIXEL *ptr;
	int cl, r1, g1, b1, r2, g2, b2;
	atREAL weight2;
	
	if( rAlpha == INTTOREAL(0) ) return atNULL;
	if( rAlpha == REAL_ONE ){
		atGRPX_DrawPixel(lpCanvas, X, Y, Color );
		return atNULL;
	}

//	atGRPX_DrawPixelTrans( lpCanvas, X, Y, Color, REALTOINT(255 * rAlpha) );
//	return;

	clip = atGRPX_GetClipRect( lpCanvas );
	if( X < clip->nStartX || X > clip->nEndX || Y < clip->nStartY || Y > clip->nEndY ) return atNULL;

	weight2 = REAL_ONE - rAlpha;

	r1 = GET_RED(Color); g1 = GET_GREEN(Color); b1 = GET_BLUE(Color);

	ptr = atGRPX_GetFrameBufferPtrXY( lpCanvas, X, Y );

	__ALPHA_PIXEL_R(ptr, r1,g1,b1, cl,r2,g2,b2, rAlpha,weight2);

//	cl = atGRPX_GetPixel(lpCanvas, X, Y);
//	r2 = GET_RED(cl); g2 = GET_GREEN(cl); b2 = GET_BLUE(cl);
//	r2 = (atUCHAR)( REALTOINT( r2*weight2 + r1*rAlpha ) );
//	g2 = (atUCHAR)( REALTOINT( g2*weight2 + g1*rAlpha ) );
//	b2 = (atUCHAR)( REALTOINT( b2*weight2 + b1*rAlpha ) );
//	atGRPX_DrawPixel(lpCanvas, X, Y, MAKERGB(r2,g2,b2) );

	return ptr;
}

