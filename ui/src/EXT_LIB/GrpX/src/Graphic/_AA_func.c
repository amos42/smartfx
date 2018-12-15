/*#############################################################################
-------------------------------------------------------------------------------

                                  Line.c
                           (* =================== *)


 Version           : 1.0.0
 Dependent Modules :
 Author            : Ju, Gyeong-min ( Amos )
 Create            : 2001/04/25    01:47:09am
 Update            : 2003/04/29    00:00:00am
 C Compiler        : ADS 1.01, SDT 2.51, Borland C/C++ 5.0, Visual C/C++ 6.0
 Assembler         : ADS 1.01, SDT 2.51, Borland TASM 3.1, MASM 6.0
 Description       : Line function definition

-------------------------------------------------------------------------------
#############################################################################*/

#include "GrpX.h"
#include "_AA_func.h"
#include "_AB_int.h"


void __AA_DrawPixelAA_One( atGRPX_PIXEL *pBuf, atREAL rDX, atREAL rDY, int ColorR, int ColorG, int ColorB )
{
	atREAL rX2 = rDX;
	atREAL rY2 = rDY;
    atREAL rr, rr1, rr2, rr3, rr4;
	atGRPX_PIXEL cc;
	atINT tr,tg,tb;

    if( (rX2 == INTTOREAL(0)) && (rY2 == INTTOREAL(0)) ){
	rr1 = REAL_ONE;
	rr2 = rr3 = rr4 = INTTOREAL(0);
    } else if( rY2 == INTTOREAL(0) ){
      rr1 = REAL_ONE - rX2;
      rr2 = rX2;    
	rr3 = rr4 = INTTOREAL(0);
    } else if( rX2 == INTTOREAL(0) ){
      rr1 = REAL_ONE - rY2;
      rr3 = rY2;    
	rr2 = rr4 = INTTOREAL(0);
    } else {
      rr = REAL_ONE - rX2;
      rr3 = REALMUL( rr, rY2 );
      rr4 = REALMUL( rX2, rY2 );
      rr1 = rr - rr3;    
      rr2 = rX2 - rr4;    
    }

	if( rr1 > INTTOREAL(0) ) __ALPHA_PIXEL_R(  pBuf, ColorR, ColorG, ColorB, cc, tr, tg, tb, rr1, INTTOREAL(1)-rr1 );
}

void __AA_DrawPixelAA( atGRPX_PIXEL *pBuf, atINT nVMemWidth, atINT nX, atINT nY, atREAL rDX, atREAL rDY, int ColorR, int ColorG, int ColorB )
{
	atREAL rX2 = rDX;
	atREAL rY2 = rDY;
    atREAL rr, rr1, rr2, rr3, rr4;
	atGRPX_PIXEL *pBuf2, cc;
	atINT tr,tg,tb;

    if( (rX2 == INTTOREAL(0)) && (rY2 == INTTOREAL(0)) ){
	rr1 = REAL_ONE;
	rr2 = rr3 = rr4 = INTTOREAL(0);
    } else if( rY2 == INTTOREAL(0) ){
      rr1 = REAL_ONE - rX2;
      rr2 = rX2;    
	rr3 = rr4 = INTTOREAL(0);
    } else if( rX2 == INTTOREAL(0) ){
      rr1 = REAL_ONE - rY2;
      rr3 = rY2;    
	rr2 = rr4 = INTTOREAL(0);
    } else {
      rr = REAL_ONE - rX2;
      rr3 = REALMUL( rr, rY2 );
      rr4 = REALMUL( rX2, rY2 );
      rr1 = rr - rr3;    
      rr2 = rX2 - rr4;    
    }
	
	if( rr1 > INTTOREAL(0) ) __ALPHA_PIXEL_R(  pBuf, ColorR, ColorG, ColorB, cc, tr, tg, tb, rr1, INTTOREAL(1)-rr1 );
	pBuf2 = pBuf + 1;
	if( rr2 > INTTOREAL(0) ) __ALPHA_PIXEL_R(  pBuf2, ColorR, ColorG, ColorB, cc, tr, tg, tb, rr2, INTTOREAL(1)-rr2 );
	pBuf2 = pBuf + nVMemWidth;
	if( rr3 > INTTOREAL(0) ) __ALPHA_PIXEL_R(  pBuf2, ColorR, ColorG, ColorB, cc, tr, tg, tb, rr3, INTTOREAL(1)-rr3 );
	pBuf2++;
	if( rr4 > INTTOREAL(0) ) __ALPHA_PIXEL_R(  pBuf2, ColorR, ColorG, ColorB, cc, tr, tg, tb, rr4, INTTOREAL(1)-rr4 );
}

void __AA_DrawPixelRealAA_One( atGRPX_PIXEL *pBuf, atREAL rX, atREAL rY, int ColorR, int ColorG, int ColorB )
{
	atREAL rX2 = REALREALK(rX);
	atREAL rY2 = REALREALK(rY);
    atREAL rr, rr1, rr2, rr3, rr4;
	atGRPX_PIXEL cc;
	atINT tr,tg,tb;

    if( (rX2 == INTTOREAL(0)) && (rY2 == INTTOREAL(0)) ){
	rr1 = REAL_ONE;
	rr2 = rr3 = rr4 = INTTOREAL(0);
    } else if( rY2 == INTTOREAL(0) ){
      rr1 = REAL_ONE - rX2;
      rr2 = rX2;    
	rr3 = rr4 = INTTOREAL(0);
    } else if( rX2 == INTTOREAL(0) ){
      rr1 = REAL_ONE - rY2;
      rr3 = rY2;    
	rr2 = rr4 = INTTOREAL(0);
    } else {
      rr = REAL_ONE - rX2;
      rr3 = REALMUL( rr, rY2 );
      rr4 = REALMUL( rX2, rY2 );
      rr1 = rr - rr3;    
      rr2 = rX2 - rr4;    
    }

//      rr1 = REAL_ONE - (rX2 + rY2)/2;

	if( rr1 > INTTOREAL(0) ) __ALPHA_PIXEL_R(  pBuf, ColorR, ColorG, ColorB, cc, tr, tg, tb, rr1, INTTOREAL(1)-rr1 );
}

void __AA_DrawPixelRealAA( atGRPX_PIXEL *pBuf, atINT nVMemWidth, atREAL rX, atREAL rY, int ColorR, int ColorG, int ColorB )
{
	atREAL rX2 = REALREALK(rX);
	atREAL rY2 = REALREALK(rY);
    atREAL rr, rr1, rr2, rr3, rr4;
	atGRPX_PIXEL *pBuf2, cc;
	atINT tr,tg,tb;

    if( (rX2 == INTTOREAL(0)) && (rY2 == INTTOREAL(0)) ){
	rr1 = REAL_ONE;
	rr2 = rr3 = rr4 = INTTOREAL(0);
    } else if( rY2 == INTTOREAL(0) ){
      rr1 = REAL_ONE - rX2;
      rr2 = rX2;    
	rr3 = rr4 = INTTOREAL(0);
    } else if( rX2 == INTTOREAL(0) ){
      rr1 = REAL_ONE - rY2;
      rr3 = rY2;    
	rr2 = rr4 = INTTOREAL(0);
    } else {
      rr = REAL_ONE - rX2;
      rr3 = REALMUL( rr, rY2 );
      rr4 = REALMUL( rX2, rY2 );
      rr1 = rr - rr3;    
      rr2 = rX2 - rr4;    
    }
	
	if( rr1 > INTTOREAL(0) ) __ALPHA_PIXEL_R(  pBuf, ColorR, ColorG, ColorB, cc, tr, tg, tb, rr1, INTTOREAL(1)-rr1 );
	pBuf2 = pBuf + 1;
	if( rr2 > INTTOREAL(0) ) __ALPHA_PIXEL_R(  pBuf2, ColorR, ColorG, ColorB, cc, tr, tg, tb, rr2, INTTOREAL(1)-rr2 );
	pBuf2 = pBuf + nVMemWidth;
	if( rr3 > INTTOREAL(0) ) __ALPHA_PIXEL_R(  pBuf2, ColorR, ColorG, ColorB, cc, tr, tg, tb, rr3, INTTOREAL(1)-rr3 );
	pBuf2++;
	if( rr4 > INTTOREAL(0) ) __ALPHA_PIXEL_R(  pBuf2, ColorR, ColorG, ColorB, cc, tr, tg, tb, rr4, INTTOREAL(1)-rr4 );
}

void __AA_DrawPixelAA_H( atGRPX_PIXEL *pBuf, atREAL rDX, int ColorR, int ColorG, int ColorB, atINT x, atRECT *clip )
{
	atREAL rr, rr2;
	atGRPX_PIXEL cc;
	atINT tr,tg,tb;

	if( rDX >= 0 ){
		rr2 = REALREALK(rDX);
		rr = REAL_ONE - rr2;
		if( rr > INTTOREAL(0) ) __ALPHA_PIXEL_R(  pBuf, ColorR, ColorG, ColorB, cc, tr, tg, tb, rr, rr2 );
		if( (rr2 > INTTOREAL(0)) && ((x + 1) <= clip->nEndX) ){
			pBuf++;
			__ALPHA_PIXEL_R(  pBuf, ColorR, ColorG, ColorB, cc, tr, tg, tb, rr2, rr );
		}
	} else {
		rr2 = REALREALK(-rDX);
		rr = REAL_ONE - rr2;
		if( rr > INTTOREAL(0) ) __ALPHA_PIXEL_R(  pBuf, ColorR, ColorG, ColorB, cc, tr, tg, tb, rr, rr2 );
		if( (rr2 > INTTOREAL(0)) && ((x - 1) >= clip->nStartX) ){
			pBuf--;
		 	__ALPHA_PIXEL_R(  pBuf, ColorR, ColorG, ColorB, cc, tr, tg, tb, rr2, rr );
		}
	}
}

void __AA_DrawPixelAA_H_One( atGRPX_PIXEL *pBuf, atREAL rDX, int ColorR, int ColorG, int ColorB )
{
	atREAL rr, rr2;
	atGRPX_PIXEL cc;
	atINT tr,tg,tb;

	if( rDX >= 0 ){
		rr2 = REALREALK(rDX);
		rr = REAL_ONE - rr2;
		if( rr > INTTOREAL(0) ) __ALPHA_PIXEL_R(  pBuf, ColorR, ColorG, ColorB, cc, tr, tg, tb, rr, rr2 );
	} else {
		rr2 = REALREALK(-rDX);
		rr = REAL_ONE - rr2;
		if( rr > INTTOREAL(0) ) __ALPHA_PIXEL_R(  pBuf, ColorR, ColorG, ColorB, cc, tr, tg, tb, rr, rr2 );
	}
}

void __AA_DrawPixelAA_V( atGRPX_PIXEL *pBuf, atINT nVMemWidth, atREAL rDY, int ColorR, int ColorG, int ColorB, atINT y, atRECT *clip )
{
	atREAL rr, rr2;
	atGRPX_PIXEL cc;
	atINT tr,tg,tb;

	if( rDY >= 0 ){
		rr2 = REALREALK(rDY);
		rr = REAL_ONE - rr2;
		if( rr > INTTOREAL(0) ) __ALPHA_PIXEL_R(  pBuf, ColorR, ColorG, ColorB, cc, tr, tg, tb, rr, rr2 );
		if( (rr2 > INTTOREAL(0)) && ((y + 1) <= clip->nEndX) ){
			pBuf += nVMemWidth;
			__ALPHA_PIXEL_R(  pBuf, ColorR, ColorG, ColorB, cc, tr, tg, tb, rr2, rr );
		}
	} else {
		rr2 = REALREALK(-rDY);
		rr = REAL_ONE - rr2;
		if( rr > INTTOREAL(0) ) __ALPHA_PIXEL_R(  pBuf, ColorR, ColorG, ColorB, cc, tr, tg, tb, rr, rr2 );
		if( (rr2 > INTTOREAL(0)) && ((y - 1) >= clip->nStartY) ){
			pBuf -= nVMemWidth;
			__ALPHA_PIXEL_R(  pBuf, ColorR, ColorG, ColorB, cc, tr, tg, tb, rr2, rr );
		}
	}
}

void __AA_DrawPixelAA_V_One( atGRPX_PIXEL *pBuf, atREAL rDY, int ColorR, int ColorG, int ColorB )
{
	atREAL rr, rr2;
	atGRPX_PIXEL cc;
	atINT tr,tg,tb;

	if( rDY >= 0 ){
		rr2 = REALREALK(rDY);
		rr = REAL_ONE - rr2;
		if( rr > INTTOREAL(0) ) __ALPHA_PIXEL_R(  pBuf, ColorR, ColorG, ColorB, cc, tr, tg, tb, rr, rr2 );
	} else {
		rr2 = REALREALK(-rDY);
		rr = REAL_ONE - rr2;
		if( rr > INTTOREAL(0) ) __ALPHA_PIXEL_R(  pBuf, ColorR, ColorG, ColorB, cc, tr, tg, tb, rr, rr2 );
	}
}

void __AA_DrawPixelRealAA_H( atGRPX_PIXEL *pBuf, atREAL rX, int ColorR, int ColorG, int ColorB )
{
	atREAL rr2 = REALREALK(rX);
	atREAL rr = REAL_ONE - rr2;
	atGRPX_PIXEL cc;
	atINT tr,tg,tb;

	if( rr > INTTOREAL(0) ) __ALPHA_PIXEL_R(  pBuf, ColorR, ColorG, ColorB, cc, tr, tg, tb, rr, rr2 );
	pBuf++;
	if( rr2 > INTTOREAL(0) ) __ALPHA_PIXEL_R(  pBuf, ColorR, ColorG, ColorB, cc, tr, tg, tb, rr2, rr );
}

void __AA_DrawPixelRealAA_V( atGRPX_PIXEL *pBuf, atINT nVMemWidth, atREAL rY, int ColorR, int ColorG, int ColorB )
{
	atREAL rr2 = REALREALK(rY);
	atREAL rr = REAL_ONE - rr2;
	atGRPX_PIXEL cc;
	atINT tr,tg,tb;

	if( rr > INTTOREAL(0) ) __ALPHA_PIXEL_R(  pBuf, ColorR, ColorG, ColorB, cc, tr, tg, tb, rr, rr2 );
	pBuf += nVMemWidth;
	if( rr2 > INTTOREAL(0) ) __ALPHA_PIXEL_R(  pBuf, ColorR, ColorG, ColorB, cc, tr, tg, tb, rr2, rr );
}




#define REALINT_KK				(65536)
#define REALINT_K				(16)


void AACopyPixel_i_2( atGRPX_CANVAS *lpCanvas, int x, int y, atUINT dx, atUINT dy, int ColorR, int ColorG, int ColorB )
{

	int i;
	atGRPX_PIXEL* pBuf[4];//lt, lb, rt, rb
	int iStride;
	atUINT iWeight[4];
	atUINT iWeight2[4];
	atREGION *rgn;
	atGRPX_PIXEL cl;
	atBYTE r, g, b;

	rgn = atGRPX_GetCanvasArea( lpCanvas );

	if( x<0 || x>=rgn->nWidth || y<0 || y>=rgn->nHeight ) return;

	iStride = ((rgn->nWidth+1)&0xfffffffc);

	if( !dx && !dy ){
		iWeight[0] = REALINT_KK;
		iWeight2[0] = 0;
	} else {
		iWeight[0] = ((REALINT_KK-dx)*(REALINT_KK-dy))>>REALINT_K;
		iWeight2[0] = REALINT_KK-iWeight[0];
	}

	pBuf[0] = atGRPX_GetFrameBufferPtrXY( lpCanvas, x, y );

//	cl = atGRPX_GetPixel(lpCanvas, x, y);
//	r = GET565R(cl); g = GET565G(cl); b = GET565B(cl);
//	r = (atUCHAR)( REALTOINT( r*iWeight2[0] + ColorR*iWeight[0] ) );
//	g = (atUCHAR)( REALTOINT( g*iWeight2[0] + ColorG*iWeight[0] ) );
//	b = (atUCHAR)( REALTOINT( b*iWeight2[0] + ColorB*iWeight[0] ) );
//	atGRPX_DrawPixel(lpCanvas, x, y, MAKE565(r,g,b) );
//	atGRPX_DrawPixelTransReal(lpCanvas, x, y, MAKE565(r,g,b), iWeight[0] );
	__ALPHA_PIXEL_R(pBuf[0], ColorR,ColorG,ColorB, cl,r,g,b, iWeight[0],iWeight2[0]);

	//if( x==0 || x==pbm->bmWidth-1 || y==0 || y==pbm->bmHeight-1 ) return;
	if( (!dx && !dy ) || (dx&&x==rgn->nWidth-1) || (dy&&y==rgn->nHeight-1) ) return;

	iWeight[1] = ((REALINT_KK-dx)*dy)>>REALINT_K;		//lb
	iWeight[2] = (dx*(REALINT_KK-dy))>>REALINT_K;		//rt
	iWeight[3] = (dx*dy)>>REALINT_K;					//rb

	iWeight2[1] = REALINT_KK-iWeight[1];
	iWeight2[2] = REALINT_KK-iWeight[2];
	iWeight2[3] = REALINT_KK-iWeight[3];

	pBuf[0] -= 1;
	pBuf[1] = (dy) ? pBuf[0]-iStride : 0;				//lb
	pBuf[2] = (dx) ? pBuf[0]+1 : 0;						//rt
	pBuf[3] = (dx && dy) ? pBuf[0]-iStride+1 : 0;		//rb

	for( i=1; i<4; i++ )
	{
		if( !pBuf[i] ) continue;
//		*pBuf[i]++ = (atUCHAR)( (*pBuf[i]*iWeight2[i] + ColorB*iWeight[i] )>>REALINT_K );
//		*pBuf[i]++ = (atUCHAR)( (*pBuf[i]*iWeight2[i] + ColorG*iWeight[i] )>>REALINT_K );
//		*pBuf[i]++ = (atUCHAR)( (*pBuf[i]*iWeight2[i] + ColorR*iWeight[i] )>>REALINT_K );
		__ALPHA_PIXEL_R(pBuf[i], ColorR,ColorG,ColorB, cl,r,g,b, iWeight[i],iWeight2[i]);
	}

}


void AACopyPixelV_i_2( atGRPX_CANVAS *lpCanvas, int x, int y, atUINT dx, atUINT dy, int ColorR, int ColorG, int ColorB )
{
	atUINT weight = REALINT_KK-dy;
	atUINT weight2 = dy;
	atRECT *rct;
	int cl, r, g, b;
	atGRPX_PIXEL *ptr;

	rct = atGRPX_GetClipRect( lpCanvas );

	if( x<rct->nStartX || x>rct->nEndX || y<rct->nStartY || y>rct->nEndY ) return;

	ptr = atGRPX_GetFrameBufferPtrXY( lpCanvas, x, y );
//	cl = MAKERGB(ColorR, ColorG, ColorB);

/*
	cl = atGRPX_GetPixel(lpCanvas, x, y);
	r = GET565R(cl); g = GET565G(cl); b = GET565B(cl);
	r = (atUCHAR)( REALTOINT( r*iWeight2 + ColorR*iWeight ) );
	g = (atUCHAR)( REALTOINT( g*iWeight2 + ColorG*iWeight ) );
	b = (atUCHAR)( REALTOINT( b*iWeight2 + ColorB*iWeight ) );
	atGRPX_DrawPixel(lpCanvas, x, y, MAKE565(r,g,b) );
*/
//	atGRPX_DrawPixelTransReal( lpCanvas, x, y, cl, weight );
	__ALPHA_PIXEL_R(ptr, ColorR,ColorG,ColorB, cl,r,g,b, weight,weight2);

//	if( dy == 0 ) return;
	if( !dy || (dy && y>=rct->nEndY) ) return;
	ptr += atGRPX_GetVMemWidth(lpCanvas);

/*
	cl = atGRPX_GetPixel(lpCanvas, x, y+1);
	r = GET565R(cl); g = GET565G(cl); b = GET565B(cl);
	r = (atUCHAR)( REALTOINT( r*iWeight + ColorR*iWeight2 ) );
	g = (atUCHAR)( REALTOINT( g*iWeight + ColorG*iWeight2 ) );
	b = (atUCHAR)( REALTOINT( b*iWeight + ColorB*iWeight2 ) );
	atGRPX_DrawPixel(lpCanvas, x, y+1, MAKE565(r,g,b) );
*/
//	atGRPX_DrawPixelTransReal( lpCanvas, x, y+1, cl, weight2 );
	__ALPHA_PIXEL_R(ptr, ColorR,ColorG,ColorB, cl,r,g,b, weight2,weight);
}


void AACopyPixelH_i_2( atGRPX_CANVAS *lpCanvas, int x, int y, atUINT dx, atUINT dy, int ColorR, int ColorG, int ColorB )
{
	atUINT weight = REALINT_KK-dx;
	atUINT weight2 = dx;
	atRECT *rct;
	int cl, r, g, b;
	atGRPX_PIXEL *ptr;

	rct = atGRPX_GetClipRect( lpCanvas );

	if( x<rct->nStartX || x>rct->nEndX || y<rct->nStartY || y>rct->nEndY ) return;

	ptr = atGRPX_GetFrameBufferPtrXY( lpCanvas, x, y );

//	cl = MAKERGB(ColorR, ColorG, ColorB);
/*
	cl = atGRPX_GetPixel(lpCanvas, x, y);
	r = GET565R(cl); g = GET565G(cl); b = GET565B(cl);
	r = (atUCHAR)( REALTOINT( r*iWeight2 + ColorR*iWeight ) );
	g = (atUCHAR)( REALTOINT( g*iWeight2 + ColorG*iWeight ) );
	b = (atUCHAR)( REALTOINT( b*iWeight2 + ColorB*iWeight ) );
	atGRPX_DrawPixel(lpCanvas, x, y, MAKE565(r,g,b) );
*/
//	atGRPX_DrawPixelTransReal( lpCanvas, x, y, cl, weight );
	__ALPHA_PIXEL_R(ptr, ColorR,ColorG,ColorB, cl,r,g,b, weight,weight2);

	if( !dx || (dx && x>=rct->nEndX) ) return;
	ptr++;

/*
	cl = atGRPX_GetPixel(lpCanvas, x+1, y);
	r = GET565R(cl); g = GET565G(cl); b = GET565B(cl);
	r = (atUCHAR)( REALTOINT( r*iWeight + ColorR*iWeight2 ) );
	g = (atUCHAR)( REALTOINT( g*iWeight + ColorG*iWeight2 ) );
	b = (atUCHAR)( REALTOINT( b*iWeight + ColorB*iWeight2 ) );
	atGRPX_DrawPixel(lpCanvas, x+1, y, MAKE565(r,g,b) );
*/
//	atGRPX_DrawPixelTransReal( lpCanvas, x+1, y, cl, weight2 );
	__ALPHA_PIXEL_R(ptr, ColorR,ColorG,ColorB, cl,r,g,b, weight2,weight);
}


/*
void AAACopyPixel_i_2( atGRPX_CANVAS *lpCanvas, int x, int y, UINT dx, UINT dy, int ColorR, int ColorG, int ColorB, int alpha )
{
	if( x<0 || x>=Width || y<0 || y>=Height ) return;
	UCHAR* pBuf[4];//lt, lb, rt, rb

	int iStride;

	UINT iWeight[4];
	UINT iWeight2[4];
	int i;

	pBuf[0] = GetPixelPtr_2( pBits, WidthBytes, Height, x, y );
	//if( pBuf[0][0]==ColorB && pBuf[0][1]==ColorG && pBuf[0][2]==ColorR ) return;

	iStride = ((Width*3+3)&0xfffffffc);

	if( dx || dy )
	{
		iWeight[0] = SHR08( SHR10( (REALINT_KK-dx)*(REALINT_KK-dy) ) * alpha );
		iWeight2[0] = REALINT_KK-iWeight[0];
	}else{
		iWeight[0] = SHR08( REALINT_KK * alpha );
		iWeight2[0] = REALINT_KK-iWeight[0];
	}

	//for( i=0; i<4; i++ ) iWeight[i] = MIN( iWeight[i]<<1, REALINT_KK );

	*pBuf[0]++ = (UCHAR)SHR( *pBuf[0]*iWeight2[0] + ColorB*iWeight[0], REALINT_K );
	*pBuf[0]++ = (UCHAR)SHR( *pBuf[0]*iWeight2[0] + ColorG*iWeight[0], REALINT_K );
	*pBuf[0]++ = (UCHAR)SHR( *pBuf[0]*iWeight2[0] + ColorR*iWeight[0], REALINT_K );

	//if( x==0 || x==pbm->bmWidth-1 || y==0 || y==pbm->bmHeight-1 ) return;
	if( (!dx && !dy ) || (dx&&x==Width-1) || (dy&&y==Height-1) ) return;

	iWeight[1] = SHR08( SHR10((REALINT_KK-dx)*dy) * alpha );		//lb
	iWeight[2] = SHR08( SHR10(dx*(REALINT_KK-dy)) * alpha );		//rt
	iWeight[3] = SHR08( SHR10(dx*dy) * alpha );						//rb

	iWeight2[1] = REALINT_KK-iWeight[1];
	iWeight2[2] = REALINT_KK-iWeight[2];
	iWeight2[3] = REALINT_KK-iWeight[3];

	pBuf[0] -= 3;
	pBuf[1] = (dy) ? pBuf[0]-iStride : 0;				//lb
	pBuf[2] = (dx) ? pBuf[0]+3 : 0;						//rt
	pBuf[3] = (dx && dy) ? pBuf[0]-iStride+3 : 0;		//rb

	for( i=1; i<4; i++ )
	{
		if( !pBuf[i] ) continue;
		*pBuf[i]++ = (UCHAR)SHR( *pBuf[i]*iWeight2[i] + ColorB*iWeight[i], REALINT_K );
		*pBuf[i]++ = (UCHAR)SHR( *pBuf[i]*iWeight2[i] + ColorG*iWeight[i], REALINT_K );
		*pBuf[i]++ = (UCHAR)SHR( *pBuf[i]*iWeight2[i] + ColorR*iWeight[i], REALINT_K );
	}
}
*/




//static void _xxx( atGRPX_CANVAS *lpCanvas, atREAL s_x, atREAL e_x, int scan_y, atGRPX_PIXEL color, atREAL alp  )
static void _xxx( atGRPX_CANVAS *lpCanvas, atREAL s_x, atREAL e_x, int scan_y, atBYTE r0, atBYTE g0, atBYTE b0, atREAL alp  )
{
    int i;
    int sss, eee;
    atREAL d;
    atREAL ll, aa, hh, h, vvv;
    atREAL alp2, alp3;
atGRPX_PIXEL *ptr, cl;
atBYTE r2, g2, b2;
atRECT *clip;
	

clip = atGRPX_GetClipRect( lpCanvas );
if( scan_y < clip->nStartY || scan_y > clip->nEndY ) return;

            d = s_x - e_x;

sss = REALTOINT(s_x);
if( REALREALK(s_x) == 0 ) sss--;
eee = REALTOINT(e_x);

if( sss < clip->nStartX || eee > clip->nEndX ) return;

ptr = atGRPX_GetFrameBufferPtrXY(lpCanvas, sss, scan_y);

              ll = REAL_ONE - REALREALK(s_x);
              aa = REAL_ONE - ll;
              h = REALDIV(aa, d);
vvv = REAL_ONE;
              
            if( REALREALK(s_x) > 0 ){
              aa = REAL_ONE - REALMUL(h, aa)/2;

//              atGRPX_DrawPixelTransR( lpCanvas, sss, scan_y, color, REALMUL(alp,aa)  );
if( sss >= clip->nStartX && sss <= clip->nEndX ){
alp2 = REALMUL(alp,aa);
alp3 = REAL_ONE - alp2;
	if( alp2 > 0 ) __ALPHA_PIXEL_R(ptr, r0,g0,b0, cl,r2,g2,b2, alp2,alp3);
}
ptr--;
              sss--;
            vvv = REAL_ONE-h;
            }

            if( REALREALK(e_x) > 0 ){
              eee++;
            }

			
            h = REALDIV(REAL_ONE, d);
//            h = REALDIV(vvv, d);
			for( i = sss; i >= eee; i-- ){
                aa = vvv - h/2;
//				atGRPX_DrawPixelTransR( lpCanvas, i, scan_y, color, REALMUL(alp,aa) );
if( i >= clip->nStartX && i <= clip->nEndX ){
alp2 = REALMUL(alp,aa);
alp3 = REAL_ONE - alp2;
	if( alp2 > 0 ) __ALPHA_PIXEL_R(ptr, r0,g0,b0, cl,r2,g2,b2, alp2,alp3);
}
ptr--;
                vvv-=h;
		if( vvv < 0 ) vvv = 0;	
			}

            if( REALREALK(e_x) > 0 ){
              ll = REAL_ONE - REALREALK(e_x);
              h = REALDIV(ll, d);
              aa = REALMUL(h, ll)/2;
              if( ll > 0 ){
//	  	 atGRPX_DrawPixelTransR( lpCanvas, REALTOINT(e_x), scan_y, color, REALMUL(alp,aa)  );
if( REALTOINT(e_x) >= clip->nStartX && REALTOINT(e_x) <= clip->nEndX ){
alp2 = REALMUL(alp,aa);
alp3 = REAL_ONE - alp2;
	if( alp2 > 0 ) __ALPHA_PIXEL_R(ptr, r0,g0,b0, cl,r2,g2,b2, alp2,alp3);
}	
ptr--;

              }
            }
}


//static void _yyy( atGRPX_CANVAS *lpCanvas, atREAL s_x, atREAL e_x, int scan_y, atGRPX_PIXEL color, atREAL alp  )
static void _yyy( atGRPX_CANVAS *lpCanvas, atREAL s_x, atREAL e_x, int scan_y, atBYTE r0, atBYTE g0, atBYTE b0, atREAL alp  )
{
    int i;
    int sss, eee;
    atREAL d;
    atREAL ll, aa, hh, h, vvv;
    atREAL alp2, alp3;
atGRPX_PIXEL *ptr, cl;
atBYTE r2, g2, b2;
atRECT *clip;
	

clip = atGRPX_GetClipRect( lpCanvas );
if( scan_y < clip->nStartY || scan_y > clip->nEndY ) return;

            d = e_x - s_x;

sss = REALTOINT(s_x);
eee = REALTOINT(e_x)-1;
if( REALREALK(e_x) > 0 ) eee++;

if( eee < clip->nStartX || sss > clip->nEndX ) return;

ptr = atGRPX_GetFrameBufferPtrXY(lpCanvas, sss, scan_y);

              ll = REALREALK(s_x);
              aa = REAL_ONE - ll;
              h = REALDIV(aa, d);
              
            if( REALREALK(s_x) > 0 ){
              aa = REAL_ONE - REALMUL(h, aa)/2;

//              atGRPX_DrawPixelTransR( lpCanvas, REALTOINT(s_x), scan_y, color, REALMUL(alp,aa)  );
if( sss >= clip->nStartX && sss <= clip->nEndX ){
alp2 = REALMUL(alp,aa);
alp3 = REAL_ONE - alp2;
	if( alp2 > 0 ) __ALPHA_PIXEL_R(ptr, r0,g0,b0, cl,r2,g2,b2, alp2,alp3);
}	
ptr++;
              sss++;
            }

            if( REALREALK(e_x) > 0 ){
              eee--;
            }

//            vvv = REAL_ONE-h;
vvv = REAL_ONE;
            h = REALDIV(REAL_ONE, d);
			for( i = sss; i <= eee; i++ ){
                aa = vvv - h/2;
//				atGRPX_DrawPixelTransR( lpCanvas, i, scan_y, color, REALMUL(alp,aa) );
if( i >= clip->nStartX && i <= clip->nEndX ){
alp2 = REALMUL(alp,aa);
alp3 = REAL_ONE - alp2;
	if( alp2 > 0 ) __ALPHA_PIXEL_R(ptr, r0,g0,b0, cl,r2,g2,b2, alp2,alp3);
}	
ptr++;

                vvv-=h;
		if( vvv < 0 ) vvv = 0;	
			}

            if( REALREALK(e_x) > 0 ){
              ll = REALREALK(e_x);
              h = REALDIV(ll, d);
              aa = REALMUL(h, ll)/2;
              if( ll > 0 ){
//	  	 atGRPX_DrawPixelTransR( lpCanvas, REALTOINT(e_x), scan_y, color, REALMUL(alp,aa)  );
if( REALTOINT(e_x) >= clip->nStartX && REALTOINT(e_x) <= clip->nEndX ){
alp2 = REALMUL(alp,aa);
alp3 = REAL_ONE - alp2;
	if( alp2 > 0 ) __ALPHA_PIXEL_R(ptr, r0,g0,b0, cl,r2,g2,b2, alp2,alp3);
}	
ptr++;
              }
            }
}


void __AA_DrawHorizeLine( atGRPX_CANVAS *lpCanvas, atINT scan_y, atREAL lx, atREAL ly, atREAL rx, atREAL ry, atREAL ldxdy, atREAL rdxdy, atGRPX_PIXEL color, atREAL init_alpha, atDWORD dwAdjEdge )
{
	int i , cnt;
	atREAL lx2, rx2;
	atREAL dv;
    	atREAL alp, alp2, alp3, aa, aa2;
	atREAL hh, hh1;
    int _ll, _rr;
    int vv;
    atREAL d;
    atREAL ll, rr, h;
    atREAL vvv;
	atGRPX_PIXEL *ptr, cl;
	atBYTE r0, g0, b0, r2, g2, b2;
	atRECT *clip;
    int sss, eee;
    atREAL s_x, e_x;

    alp = init_alpha;

	lx2 = lx + ldxdy;
	rx2 = rx + rdxdy;

    _ll = REALTOINT(lx);
    _rr = REALTOINT(rx);

	r0 = GET_RED(color); g0 = GET_GREEN(color); b0 = GET_BLUE(color);
clip = atGRPX_GetClipRect(lpCanvas);

	if( dwAdjEdge & 0x1 )	{
		if( ABS(ldxdy) > REAL_ONE ){
			if( (lx2 < lx) && (REALTOINT(lx) >= clip->nStartX) ){
//				_xxx( lpCanvas, lx, lx2, scan_y, color, alp );
				_xxx( lpCanvas, lx, lx2, scan_y, r0, g0, b0, alp );
	            		_ll = REALTOINT(lx);
			} else if( REALTOINT(lx2) >= clip->nStartX ){
//				_xxx( lpCanvas, lx2, lx, scan_y, color, alp );
				_xxx( lpCanvas, lx2, lx, scan_y, r0, g0, b0, alp );
	            		_ll = REALTOINT(lx2);
			}
		} else {
	        if( REALTOINT(lx) == REALTOINT(lx2) ){
	            if( REALREALK(lx) == INTTOREAL(0) && REALREALK(lx2) == INTTOREAL(0) ){
	//                _ll--;
	            } else if( lx2 == lx ){
	               aa = REAL_ONE-REALREALK(lx);
			       atGRPX_DrawPixelTransR( lpCanvas, REALTOINT(lx), scan_y, color, REALMUL(alp,aa) );
	               _ll++;
	            } else if( lx2 < lx ){
	               aa = (REAL_ONE-REALREALK(lx)) + (lx-lx2)/2;
			       atGRPX_DrawPixelTransR( lpCanvas, REALTOINT(lx), scan_y, color, REALMUL(alp,aa) );
	               _ll++;
	            } else if( lx2 > lx ){
	               aa = (REAL_ONE-REALREALK(lx2)) + (lx2-lx)/2;
			       atGRPX_DrawPixelTransR( lpCanvas, REALTOINT(lx), scan_y, color, REALMUL(alp,aa) );
	               _ll++;
	            }
	        } else if( REALTOINT(lx2) < REALTOINT(lx) ){
	                aa = REAL_ONE - REALREALK(lx);
	                aa2 = REAL_ONE - REALREALK(lx2);
			  hh1 = REALDIV((REAL_ONE-aa), (lx-lx2));
			  hh = REAL_ONE - hh1;
			  aa = REAL_ONE - REALMUL(hh1, (REAL_ONE-aa))/2;
			  aa2 = REALMUL(hh, aa2) / 2;

		        if(aa2>0)atGRPX_DrawPixelTransR( lpCanvas, REALTOINT(lx2), scan_y, color, REALMUL(alp,aa2) );
	            if( REALREALK(lx) > 0 ){
		            atGRPX_DrawPixelTransR( lpCanvas, REALTOINT(lx), scan_y, color, REALMUL(alp,aa) );
	                _ll++;
	            }
	        } else {
	                aa = REAL_ONE - REALREALK(lx2);
	                aa2 = REAL_ONE - REALREALK(lx);
			  hh1 = REALDIV((REAL_ONE-aa), (lx2-lx));
			  hh = REAL_ONE - hh1;
			  aa = REAL_ONE - REALMUL(hh1, (REAL_ONE-aa))/2;
			  aa2 = REALMUL(hh, aa2) / 2;

			   atGRPX_DrawPixelTransR( lpCanvas, REALTOINT(lx2), scan_y, color, REALMUL(alp,aa) );
			  if(aa2>0)atGRPX_DrawPixelTransR( lpCanvas, REALTOINT(lx), scan_y, color, REALMUL(alp,aa2) );
	                _ll++;
	        }
		}
	} else {
		lx2 = lx + REALHALF(ldxdy);
//lx2 = lx + ldxdy;
		_ll = REALTOINT(lx2);
		if( REALREALK(lx2) > FLOATTOREAL(0.5+0.1) ) _ll++;
	}
    lx = lx2;

	if( dwAdjEdge & 0x2 )	{
		if( ABS(rdxdy) > REAL_ONE ){
			if( (rx2 > rx) && (REALTOINT(rx) <= clip->nEndX) ){
//				_yyy( lpCanvas, rx, rx2, scan_y, color, alp );
				_yyy( lpCanvas, rx, rx2, scan_y, r0, g0, b0, alp );
	            _rr = REALTOINT(rx);
			} else if( REALTOINT(rx2) <= clip->nEndX ){
//				_yyy( lpCanvas, rx2, rx, scan_y, color, alp );
				_yyy( lpCanvas, rx2, rx, scan_y, r0, g0, b0, alp );
	            _rr = REALTOINT(rx2);
			}
		} else {
	        if( REALTOINT(rx) == REALTOINT(rx2) ){
	            if( REALREALK(rx) == INTTOREAL(0) && REALREALK(rx2) == INTTOREAL(0) ){
	//                _rr++;
	            } else if( rx2 == rx ){
	               aa = REALREALK(rx)/2;
			       atGRPX_DrawPixelTransR( lpCanvas, REALTOINT(rx), scan_y, color, REALMUL(alp,aa) );
	//               _rr--;
	            } else if( rx2 > rx ){
	               aa = REALREALK(rx) + (rx2-rx)/2;
			       atGRPX_DrawPixelTransR( lpCanvas, REALTOINT(rx), scan_y, color, REALMUL(alp,aa) );
			       atGRPX_DrawPixelTransR( lpCanvas, REALTOINT(rx2), scan_y, color, REALMUL(alp,REALREALK(rx2)/2) );
	//               _rr--;
	            } else if( rx2 < rx ){
	               aa = REALREALK(rx2) + (rx-rx2)/2;
			       atGRPX_DrawPixelTransR( lpCanvas, REALTOINT(rx2), scan_y, color, REALMUL(alp,aa) );
			       atGRPX_DrawPixelTransR( lpCanvas, REALTOINT(rx), scan_y, color, REALMUL(alp,REALREALK(rx)/2) );
	//               _rr--;
	            }
	        } else if( REALTOINT(rx2) > REALTOINT(rx) ){
	                aa = REALREALK(rx);
	                aa2 = REALREALK(rx2);
			  hh1 = REALDIV((REAL_ONE-aa), (rx2-rx));
			  hh = REAL_ONE - hh1;
			  aa = REAL_ONE - REALMUL(hh1, (REAL_ONE-aa))/2;
			  aa2 = REALMUL(hh, aa2) / 2;

		        atGRPX_DrawPixelTransR( lpCanvas, REALTOINT(rx), scan_y, color, REALMUL(alp,aa) );
		        if(aa2>0)atGRPX_DrawPixelTransR( lpCanvas, REALTOINT(rx2), scan_y, color, REALMUL(alp,aa2) );
	//                _rr--;
	        } else {
	                aa = REALREALK(rx2);
	                aa2 = REALREALK(rx);
			  hh1 = REALDIV((REAL_ONE-aa), (rx-rx2));
			  hh = REAL_ONE - hh1;
			  aa = REAL_ONE - REALMUL(hh1, (REAL_ONE-aa))/2;
			  aa2 = REALMUL(hh, aa2) / 2;

			   atGRPX_DrawPixelTransR( lpCanvas, REALTOINT(rx2), scan_y, color, REALMUL(alp,aa) );
			  if(aa2>0)atGRPX_DrawPixelTransR( lpCanvas, REALTOINT(rx), scan_y, color, REALMUL(alp,aa2) );
	//                _rr-=2;
	                _rr--;
	        }
		}
	} else {
		rx2 = rx + REALHALF(rdxdy);
//rx2 = rx + rdxdy;
		_rr = REALTOINT(rx2) + 1;
		if( REALREALK(rx2) < FLOATTOREAL(0.5-0.1) ) _rr--;
//		if( REALREALK(rx2) <= 0 ) _rr--;
	}
   	rx = rx2;

    if( _rr > _ll ){
      if( alp == REAL_ONE || (dwAdjEdge != 0x3) )
        atGRPX_DrawHorizLine( lpCanvas, _ll, scan_y, _rr-_ll, color );
      else if( alp > INTTOREAL(0) )
        atGRPX_DrawHorizLineTrans( lpCanvas, _ll, scan_y, _rr-_ll, color, REALTOINT(255 * alp) );
    }
}




