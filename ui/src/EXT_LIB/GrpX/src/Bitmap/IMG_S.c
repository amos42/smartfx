/*#############################################################################
-------------------------------------------------------------------------------

                                  IMG_S.c
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


/***********************

************************/
static void __fnDrawBitmap( atGRPX_CANVAS *lpCanvas, atGRPX_BITMAP *lpBitmap, atINT X, atINT Y, atINT tWidth, atINT tHeight, atINT ImageX, atINT ImageY, atREAL dx, atREAL dy, atLONG ExParam1 )
{
    int i, j;
    int width;//, height;
    atREAL xx, yy;
    atGRPX_PIXEL *ScrPtr, *ImgPtr, *sa;
    atGRPX_PIXEL pixel, trans_color;
    int sskip;

    ImgPtr = atGRPX_GetBitmapPtrXY( lpBitmap, ImageX, ImageY );
    ScrPtr = atGRPX_GetFrameBufferPtrXY( lpCanvas, X, Y );

	if( (ImgPtr == atNULL) || (ScrPtr == atNULL) ) return;

	width = lpBitmap->nWidth;
	//height = lpBitmap->nHeight;
    sskip = atGRPX_GetVMemWidth(lpCanvas) - tWidth;
    yy = INTTOREAL(0);

    if( lpBitmap->dwAttr & atGRPX_BMATTR_TRANSPARENT ){
        trans_color = COLOR_TO_PIXEL(lpBitmap->clTransColor);
        for( i = 0; i < tHeight; i ++ ){
          sa = ImgPtr + REALTOINT(yy) * width;
    
          xx = 0;
          for( j = 0; j < tWidth; j ++ ){
            pixel = *(sa + REALTOINT(xx));
            if( pixel != trans_color ) *ScrPtr = pixel;
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
            *ScrPtr++ = *(sa + REALTOINT(xx));
            xx += dx;
          }
    
          ScrPtr += sskip;
          yy += dy;
        }
    }
}


/***********************

************************/
static void __fnDrawBitmap2( atGRPX_CANVAS *lpCanvas, atGRPX_BITMAP *lpBitmap, atINT X, atINT Y, atINT tWidth, atINT tHeight, atINT ImageX, atINT ImageY, atREAL dx, atREAL dy, atLONG ExParam1 )
{
    int i, j;
    int width, height;
    atREAL xx, yy;
    atGRPX_PIXEL *ScrPtr, *ImgPtr, *sa;
    atGRPX_PIXEL pixel, trans_color;
    int sskip;

    ImgPtr = atGRPX_GetBitmapPtrXY( lpBitmap, ImageX, ImageY );
    ScrPtr = atGRPX_GetFrameBufferPtrXY( lpCanvas, X, Y );

	if( (ImgPtr == atNULL) || (ScrPtr == atNULL) ) return;

	width = lpBitmap->nWidth;
	height = lpBitmap->nHeight;
    sskip = atGRPX_GetVMemWidth(lpCanvas) - tWidth;
    yy = INTTOREAL(0);

    if( lpBitmap->dwAttr & atGRPX_BMATTR_TRANSPARENT ){
        trans_color = COLOR_TO_PIXEL(lpBitmap->clTransColor);
        for( i = 0; i < tHeight; i ++ ){
          sa = ImgPtr + REALTOINT(yy) * width;
    
          xx = 0;
          for( j = 0; j < tWidth; j ++ ){
            pixel = *(sa + REALTOINT(xx));
    
{
atGRPX_PIXEL cl11, cl12, cl21, cl22;
atBYTE rr, gg, bb;
int r1, g1, b1, r2, g2, b2, r3, g3, b3, r4, g4, b4;
atREAL alp;

            if( pixel != trans_color ) 
if( REALREALK(xx) > 0 && REALREALK(yy) > 0 ){
/*
cl11 = *(sa + REALTOINT(xx));
cl12 = *(sa + REALTOINT(xx) + 1);
cl21 = *(sa + REALTOINT(xx) + width);
cl22 = *(sa + REALTOINT(xx) + width + 1);
__CNV_ALPHA_PIXEL_R( cl11, rr,gg,bb, REALMUL(REAL_ONE-REALREALK(xx), REAL_ONE-REALREALK(yy)) );
__CNV_ALPHA_PIXEL_R( cl12, rr,gg,bb, REALMUL(REALREALK(xx), REAL_ONE-REALREALK(yy)) );
__CNV_ALPHA_PIXEL_R( cl21, rr,gg,bb, REALMUL(REAL_ONE-REALREALK(xx), REALREALK(yy)) );
__CNV_ALPHA_PIXEL_R( cl22, rr,gg,bb, REALMUL(REALREALK(xx), REALREALK(yy)) );
cl = cl11 + cl12 + cl21 + cl22;
*/

cl11 = *(sa + REALTOINT(xx));
cl12 = *(sa + REALTOINT(xx) + 1);
cl21 = *(sa + REALTOINT(xx) + width);
cl22 = *(sa + REALTOINT(xx) + width + 1);

alp = REALMUL(REAL_ONE-REALREALK(xx), REAL_ONE-REALREALK(yy));
rr = GET_RED(cl11); gg = GET_GREEN(cl11); bb = GET_BLUE(cl11);
r1 = rr * alp; g1 = gg * alp; b1 = bb * alp;

alp = REALMUL(REALREALK(xx), REAL_ONE-REALREALK(yy));
rr = GET_RED(cl12); gg = GET_GREEN(cl12); bb = GET_BLUE(cl12);
r2 = rr * alp; g2 = gg * alp; b2 = bb * alp;

alp = REALMUL(REAL_ONE-REALREALK(xx), REALREALK(yy));
rr = GET_RED(cl21); gg = GET_GREEN(cl21); bb = GET_BLUE(cl21); 
r3 = rr * alp; g3 = gg * alp; b3 = bb * alp;

alp = REALMUL(REALREALK(xx), REALREALK(yy));
rr = GET_RED(cl22); gg = GET_GREEN(cl22); bb = GET_BLUE(cl22); 
r4 = rr * alp; g4 = gg * alp; b4 = bb * alp;

rr = REALTOINT(r1 + r2 + r3 + r4);// if( rr > 255 ) rr = 255;
gg = REALTOINT(g1 + g2 + g3 + g4);// if( gg > 255 ) gg = 255;
bb = REALTOINT(b1 + b2 + b3 + b4);// if( bb > 255 ) bb = 255;

pixel = MAKERGB(rr, gg,bb);
} else if( REALREALK(xx) > 0 ){
cl11 = *(sa + REALTOINT(xx));
cl12 = *(sa + REALTOINT(xx) + 1);

alp = REAL_ONE-REALREALK(xx);
rr = GET_RED(cl11); gg = GET_GREEN(cl11); bb = GET_BLUE(cl11); 
r1 = rr * alp; g1 = gg * alp; b1 = bb * alp;

alp = REALREALK(xx);
rr = GET_RED(cl12); gg = GET_GREEN(cl12); bb = GET_BLUE(cl12); 
r2 = rr * alp; g2 = gg * alp; b2 = bb * alp;

rr = REALTOINT(r1 + r2);// if( rr > 255 ) rr = 255;
gg = REALTOINT(g1 + g2);// if( gg > 255 ) gg = 255;
bb = REALTOINT(b1 + b2);// if( bb > 255 ) bb = 255;

pixel = MAKERGB(rr, gg,bb);
} else if( REALREALK(yy) > 0 ){
cl21 = *(sa + REALTOINT(xx) + width);
cl22 = *(sa + REALTOINT(xx) + width + 1);

alp = REAL_ONE-REALREALK(yy);
rr = GET_RED(cl21); gg = GET_GREEN(cl21); bb = GET_BLUE(cl21); 
r3 = rr * alp; g3 = gg * alp; b3 = bb * alp;

alp = REALREALK(yy);
rr = GET_RED(cl22); gg = GET_GREEN(cl22); bb = GET_BLUE(cl22); 
r4 = rr * alp; g4 = gg * alp; b4 = bb * alp;

rr = REALTOINT(r3 + r4);// if( rr > 255 ) rr = 255;
gg = REALTOINT(g3 + g4);// if( gg > 255 ) gg = 255;
bb = REALTOINT(b3 + b4);// if( bb > 255 ) bb = 255;

pixel = MAKERGB(rr, gg,bb);
} else {
//pixel = 0x0;
}
*ScrPtr = pixel;
}
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
//            *ScrPtr++ = *(sa + REALTOINT(xx));
{
atGRPX_PIXEL cl11, cl12, cl21, cl22;
atBYTE rr, gg, bb;
int r1, g1, b1, r2, g2, b2, r3, g3, b3, r4, g4, b4;
atREAL alp;

if( REALREALK(xx) > 0 && REALREALK(yy) > 0 ){
/*
cl11 = *(sa + REALTOINT(xx));
cl12 = *(sa + REALTOINT(xx) + 1);
cl21 = *(sa + REALTOINT(xx) + width);
cl22 = *(sa + REALTOINT(xx) + width + 1);
__CNV_ALPHA_PIXEL_R( cl11, rr,gg,bb, REALMUL(REAL_ONE-REALREALK(xx), REAL_ONE-REALREALK(yy)) );
__CNV_ALPHA_PIXEL_R( cl12, rr,gg,bb, REALMUL(REALREALK(xx), REAL_ONE-REALREALK(yy)) );
__CNV_ALPHA_PIXEL_R( cl21, rr,gg,bb, REALMUL(REAL_ONE-REALREALK(xx), REALREALK(yy)) );
__CNV_ALPHA_PIXEL_R( cl22, rr,gg,bb, REALMUL(REALREALK(xx), REALREALK(yy)) );
cl = cl11 + cl12 + cl21 + cl22;
*/

cl11 = *(sa + REALTOINT(xx));
cl12 = *(sa + REALTOINT(xx) + 1);
cl21 = *(sa + REALTOINT(xx) + width);
cl22 = *(sa + REALTOINT(xx) + width + 1);

alp = REALMUL(REAL_ONE-REALREALK(xx), REAL_ONE-REALREALK(yy));
rr = GET_RED(cl11); gg = GET_GREEN(cl11); bb = GET_BLUE(cl11);
r1 = rr * alp; g1 = gg * alp; b1 = bb * alp;

alp = REALMUL(REALREALK(xx), REAL_ONE-REALREALK(yy));
rr = GET_RED(cl12); gg = GET_GREEN(cl12); bb = GET_BLUE(cl12);
r2 = rr * alp; g2 = gg * alp; b2 = bb * alp;

alp = REALMUL(REAL_ONE-REALREALK(xx), REALREALK(yy));
rr = GET_RED(cl21); gg = GET_GREEN(cl21); bb = GET_BLUE(cl21); 
r3 = rr * alp; g3 = gg * alp; b3 = bb * alp;

alp = REALMUL(REALREALK(xx), REALREALK(yy));
rr = GET_RED(cl22); gg = GET_GREEN(cl22); bb = GET_BLUE(cl22); 
r4 = rr * alp; g4 = gg * alp; b4 = bb * alp;

rr = REALTOINT(r1 + r2 + r3 + r4);// if( rr > 255 ) rr = 255;
gg = REALTOINT(g1 + g2 + g3 + g4);// if( gg > 255 ) gg = 255;
bb = REALTOINT(b1 + b2 + b3 + b4);// if( bb > 255 ) bb = 255;

pixel = MAKERGB(rr, gg,bb);
} else if( REALREALK(xx) > 0 ){
cl11 = *(sa + REALTOINT(xx));
cl12 = *(sa + REALTOINT(xx) + 1);

alp = REAL_ONE-REALREALK(xx);
rr = GET_RED(cl11); gg = GET_GREEN(cl11); bb = GET_BLUE(cl11); 
r1 = rr * alp; g1 = gg * alp; b1 = bb * alp;

alp = REALREALK(xx);
rr = GET_RED(cl12); gg = GET_GREEN(cl12); bb = GET_BLUE(cl12); 
r2 = rr * alp; g2 = gg * alp; b2 = bb * alp;

rr = REALTOINT(r1 + r2);// if( rr > 255 ) rr = 255;
gg = REALTOINT(g1 + g2);// if( gg > 255 ) gg = 255;
bb = REALTOINT(b1 + b2);// if( bb > 255 ) bb = 255;

pixel = MAKERGB(rr, gg,bb);
} else if( REALREALK(yy) > 0 ){
cl21 = *(sa + REALTOINT(xx) + width);
cl22 = *(sa + REALTOINT(xx) + width + 1);

alp = REAL_ONE-REALREALK(yy);
rr = GET_RED(cl21); gg = GET_GREEN(cl21); bb = GET_BLUE(cl21); 
r3 = rr * alp; g3 = gg * alp; b3 = bb * alp;

alp = REALREALK(yy);
rr = GET_RED(cl22); gg = GET_GREEN(cl22); bb = GET_BLUE(cl22); 
r4 = rr * alp; g4 = gg * alp; b4 = bb * alp;

rr = REALTOINT(r3 + r4);// if( rr > 255 ) rr = 255;
gg = REALTOINT(g3 + g4);// if( gg > 255 ) gg = 255;
bb = REALTOINT(b3 + b4);// if( bb > 255 ) bb = 255;

pixel = MAKERGB(rr, gg,bb);
} else {
//pixel = 0x0;
}
*ScrPtr = pixel;
}

            xx += dx;
          }
    
          ScrPtr += sskip;
          yy += dy;
        }
    }
}


/***********************

************************/
void atGRPX_DrawBitmapRawStretch( atGRPX_CANVAS *lpCanvas, atGRPX_BITMAP *lpBitmap, atINT X, atINT Y, atINT tWidth, atINT tHeight )
{
    if( lpBitmap == atNULL ) return;
	
    if( (lpBitmap->nWidth == tWidth) && (lpBitmap->nHeight == tHeight) ){
	atGRPX_DrawBitmapRaw( lpCanvas, lpBitmap, X, Y );
	return;
    }
	
    __atGRPX_DrawBitmapStretch0( lpCanvas, lpBitmap, X, Y, tWidth, tHeight, (atLONG)0, __fnDrawBitmap );
}


/***********************

************************/
void atGRPX_DrawBitmapRawScale( atGRPX_CANVAS *lpCanvas, atGRPX_BITMAP *lpBitmap, atINT X, atINT Y, atINT Scale )
{
    int width, height;
    atREAL rScale;

    if( Scale <= 0 ) return;
    if( Scale == 100 ){
	atGRPX_DrawBitmapRaw( lpCanvas, lpBitmap, X, Y );
	return;
    }
	
    width  = lpBitmap->nWidth;
    height = lpBitmap->nHeight;

    rScale = INTTOREAL(Scale) / 100;
    width  = REALTOINT( width * rScale );
    height = REALTOINT( height * rScale );

    if( width == 0 || height == 0 ) return;

    atGRPX_DrawBitmapRawStretch( lpCanvas, lpBitmap, X, Y, width, height );
}

/***********************

************************/
void atGRPX_DrawBitmapStretch( atGRPX_CANVAS *lpCanvas, atGRPX_BITMAP *lpBitmap, atINT X, atINT Y, atINT tWidth, atINT tHeight )
{
	if( lpBitmap == atNULL ) return;
	if( lpBitmap->lpAlphaChannel == atNULL ){
		atGRPX_DrawBitmapRawStretch( lpCanvas, lpBitmap, X, Y, tWidth, tHeight );
	} else {
		atGRPX_DrawBitmapStretchWithAlphaChannel( lpCanvas, lpBitmap, X, Y, tWidth, tHeight, atNULL );
	}
}


/***********************

************************/
void atGRPX_DrawBitmapScale( atGRPX_CANVAS *lpCanvas, atGRPX_BITMAP *lpBitmap, atINT X, atINT Y, atINT Scale )
{
	if( lpBitmap == atNULL ) return;
	if( lpBitmap->lpAlphaChannel == atNULL ){
		atGRPX_DrawBitmapRawScale( lpCanvas, lpBitmap, X, Y, Scale );
	} else {
		atGRPX_DrawBitmapScaleWithAlphaChannel( lpCanvas, lpBitmap, X, Y, Scale, atNULL );
	}
}

/***********************

************************/
void atGRPX_DrawBitmapStretchEx( atGRPX_CANVAS *lpCanvas, atGRPX_BITMAP *lpBitmap, atINT X, atINT Y, atINT tWidth, atINT tHeight )
{
    __atGRPX_DrawBitmapStretch0( lpCanvas, lpBitmap, X, Y, tWidth, tHeight, (atLONG)0, __fnDrawBitmap2 );
}


/***********************

************************/
void atGRPX_DrawBitmapScaleEx( atGRPX_CANVAS *lpCanvas, atGRPX_BITMAP *lpBitmap, atINT X, atINT Y, atINT Scale )
{
    int width, height;
    atREAL rScale;

    width  = lpBitmap->nWidth;
    height = lpBitmap->nHeight;

    rScale = INTTOREAL(Scale) / 100;
    width  = REALTOINT( width * rScale );
    height = REALTOINT( height * rScale );

    if( width == 0 || height == 0 ) return;

    atGRPX_DrawBitmapStretchEx( lpCanvas, lpBitmap, X, Y, width, height );
}
