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
#include "A_Math.h"
#include "_AA_func.h"
#include "_AB_int.h"
#include "_AB_table.h"
#include "_LIN_int.h"
#include "_LIN_func.h"


/*******************************
  양 끝 점을 잇는 선을 그린다.
********************************/
static void _Line( atGRPX_CANVAS *lpCanvas, atINT StartX, atINT StartY, atINT EndX, atINT EndY, atLONG ExParam1 )
{
    int i;
	atREAL x1, y1, x2, y2;
	atREAL dx, dy;
	atGRPX_PIXEL Color = (atGRPX_PIXEL)ExParam1;
	atREAL ratio, kdv;
	int r, g, b;
	atGRPX_PIXEL *pBuf = atGRPX_GetFrameBufferPtrXY( lpCanvas, StartX, StartY );
	int vmem_width = atGRPX_GetVMemWidth(lpCanvas);
	atRECT *clip;
	
	r = GET_RED(Color); g = GET_GREEN(Color); b = GET_BLUE(Color);

	x1 = INTTOREAL(StartX);
	y1 = INTTOREAL(StartY);
	x2 = INTTOREAL(EndX);
	y2 = INTTOREAL(EndY);
	dx = x2 - x1;
	dy = y2 - y1;

	clip = atGRPX_GetClipRect(lpCanvas);
	
//	__AA_DrawPixelRealAA( pBuf, INTOREAL(StartX), INTOREAL(StartY), r, g, b ); // 어차피 실수부는 0이니까 쓸모 없음.
//	__AA_DrawPixelRealAA( pBuf, INTOREAL(EndX), INTOREAL(EndY), r, g, b ); // 어차피 실수부는 0이니까 쓸모 없음.

    kdv = INTTOREAL(0);
    if( ABS(dy) >= ABS(dx) ){
	  ratio = REALDIV(dx, dy);
	  if( dy > 0 ){
        for( i = StartY; i <= EndY; i++, kdv += ratio ){
			__AA_DrawPixelAA_H( pBuf + REALTOINT(kdv), kdv, r, g, b, StartX + REALTOINT(kdv), clip );
			pBuf += vmem_width;
		}
	  } else {
        for( i = StartY; i >= EndY; i--, kdv -= ratio ){
			__AA_DrawPixelAA_H( pBuf + REALTOINT(kdv), kdv, r, g, b, StartX + REALTOINT(kdv), clip );
			pBuf -= vmem_width;
        }
	  }
    } else {
	  ratio = REALDIV(dy, dx);
	  if( dx > 0 ){
        for( i = StartX; i <= EndX; i++, kdv += ratio ){
			__AA_DrawPixelAA_V( pBuf + REALTOINT(kdv) * vmem_width, vmem_width, kdv, r, g, b, StartY + REALTOINT(kdv), clip );
			pBuf++;
        }
	  } else {
        for( i = StartX; i >= EndX; i--, kdv -= ratio ){
			__AA_DrawPixelAA_V( pBuf + REALTOINT(kdv) * vmem_width, vmem_width, kdv, r, g, b, StartY + REALTOINT(kdv), clip );
			pBuf--;
        }
	  }
    }
}

/*******************************
  양 끝 점을 잇는 선을 그린다. 개선 버전
********************************/
static void _Line2( atGRPX_CANVAS *lpCanvas, atINT StartX, atINT StartY, atINT EndX, atINT EndY, atLONG ExParam1 )
{
	atREAL x1, y1, x2, y2;
	atREAL dx, dy;
	atGRPX_PIXEL Color = (atGRPX_PIXEL)ExParam1;
	atGRPX_PIXEL *pBuf;
	int vmem_width = atGRPX_GetVMemWidth(lpCanvas);
	atRECT *clip;

	int xpxl1, ypxl1, xpxl2, ypxl2, x, y;
	atREAL inter, gradient;
	atREAL tmp;
	atREAL rA, rInvA;
//	atINT nA, nInvA;
//	atGRPX_COLOR sum;

	int r, g, b, tr, tg, tb;
	atGRPX_PIXEL cc;
	r = GET_RED(Color); g = GET_GREEN(Color); b = GET_BLUE(Color);

	if(StartX==EndX && StartY==EndY) return;
	
	x1 = INTTOREAL(StartX);
	y1 = INTTOREAL(StartY);
	x2 = INTTOREAL(EndX);
	y2 = INTTOREAL(EndY);
	dx = x2 - x1;
	dy = y2 - y1;
	
	if(StartY==EndY)
	{
		if(x2 < x1)
		{
			SWAP(x1, x2, tmp);
			SWAP(y1, y2, tmp);
			dx = x2 - x1;
			dy = y2 - y1;
		}
		xpxl1 = REALTOINT(x1);
		xpxl2 = REALTOINT(x2);
		for(x=xpxl1; x<=xpxl2; ++x)
		{
			pBuf = atGRPX_GetFrameBufferPtrXY( lpCanvas, x, StartY );
			if(pBuf!=atNULL)
				*pBuf = (atGRPX_PIXEL)Color;
		}
		return;
	}

	clip = atGRPX_GetClipRect(lpCanvas);
	
    if( ABS(dy) >= ABS(dx) ){
		if(y2 < y1)
		{
			SWAP(x1, x2, tmp);
			SWAP(y1, y2, tmp);
			dx = x2 - x1;
			dy = y2 - y1;
		}
		gradient = REALDIV(dx, dy);
		
		// handle first endpoint
		ypxl1 = REALTOINT(y1);	// this will be used in the main loop
		inter = x1;				// first y-intersection for the main loop
		
		// handle second endpoint
		ypxl2 = REALTOINT(y2);	// this will be used in the main loop
		
		// main loop
		for(y=ypxl1; y<=ypxl2; ++y)
		{
			if(y<clip->nStartY || y>clip->nEndY)
				continue;
			x = REALTOINT(inter);
			rA = REALREALK(inter);
			rInvA = REAL_ONE - rA;
			pBuf = atGRPX_GetFrameBufferPtrXY( lpCanvas, x, y );
			if(x>=clip->nStartX && x<=clip->nEndX)
//				__ALPHA_BLEND565(pBuf, Color, rInvA, rA, nA, nInvA, sum);
				__ALPHA_PIXEL_R(pBuf, r, g, b, cc, tr, tg, tb, rInvA, rA);
			if(x+1>=clip->nStartX && x+1<=clip->nEndX)
//				__ALPHA_BLEND565(pBuf, Color, rA, rInvA, nA, nInvA, sum);
				__ALPHA_PIXEL_R(pBuf+1, r, g, b, cc, tr, tg, tb, rA, rInvA);
			inter += gradient;
		}
    } else {
		if(x2 < x1)
		{
			SWAP(x1, x2, tmp);
			SWAP(y1, y2, tmp);
			dx = x2 - x1;
			dy = y2 - y1;
		}
		gradient = REALDIV(dy, dx);
		
		// handle first endpoint
		xpxl1 = REALTOINT(x1);	// this will be used in the main loop
		inter = y1;				// first y-intersection for the main loop
		
		// handle second endpoint
		xpxl2 = REALTOINT(x2);	// this will be used in the main loop
		
		// main loop
		for(x=xpxl1; x<=xpxl2; ++x)
		{
			y = REALTOINT(inter);
			rA = REALREALK(inter);
			rInvA = REAL_ONE - rA;
			pBuf = atGRPX_GetFrameBufferPtrXY( lpCanvas, x, y );
			if(y>=clip->nStartY)
//				__ALPHA_BLEND565(pBuf, Color, rInvA, rA, nA, nInvA, sum);
				__ALPHA_PIXEL_R(pBuf, r, g, b, cc, tr, tg, tb, rInvA, rA);
			if(y+1<=clip->nEndY)
//				__ALPHA_BLEND565(pBuf+vmem_width, Color, rA, rInvA, nA, nInvA, sum);
				__ALPHA_PIXEL_R(pBuf+vmem_width, r, g, b, cc, tr, tg, tb, rA, rInvA);
			inter += gradient;
		}
    }
}

static int _LineDash( atGRPX_CANVAS *lpCanvas, atINT StartX, atINT StartY, atINT EndX, atINT EndY, atLONG ExParam1, atINT nDashLen, atINT nGap, atINT init_gapdash )
{
	atINT x1, y1, x2, y2, dist, init_gap, init_dash;
	atREAL dx, dy, theta, cosval, sinval, rdist, len;
	atRECT* clip;
	
	init_gap = init_gapdash > 0 ? init_gapdash : 0;
	init_dash = init_gapdash < 0 ? init_gapdash : 0;
	
	if(nGap < 1)
		nGap = 1;
	
	dist = init_gap + init_dash;
	
	dx = INTTOREAL(EndX - StartX);
	if(dx==0)
	{
		if(EndY > StartY)
			init_gapdash = atGRPX_DrawVertLineDash( lpCanvas, StartX, StartY, EndY - StartY + 1, (atGRPX_COLOR)ExParam1, nDashLen, nGap, init_gapdash );
		else
			init_gapdash = atGRPX_DrawVertLineDash( lpCanvas, EndX, EndY, StartY - EndY + 1, (atGRPX_COLOR)ExParam1, nDashLen, nGap, init_gapdash );
		return init_gapdash;
	}
	dy = INTTOREAL(EndY - StartY);
	if(dy==0)
	{
		if(EndX > StartX)
			init_gapdash = atGRPX_DrawHorizLineDash( lpCanvas, StartX, StartY, EndX - StartX + 1, (atGRPX_COLOR)ExParam1, nDashLen, nGap, init_gapdash );
		else
			init_gapdash = atGRPX_DrawHorizLineDash( lpCanvas, EndX, EndY, StartX - EndX + 1, (atGRPX_COLOR)ExParam1, nDashLen, nGap, init_gapdash );
		return init_gapdash;
	}
	theta = ATAN2(dx, dy);
	cosval = COS(theta);
	sinval = SIN(theta);
	rdist = INTTOREAL(dist);
	len = REALDIV(dx, cosval);
	if(rdist>len)
		return REALTOINT(rdist - len);
	
	if(dist > 0)
	{
		x1 = StartX + REALTOINT(dist * cosval);
		y1 = StartY + REALTOINT(dist * sinval);
	}
	else
	{
		x1 = StartX;
		y1 = StartY;
	}
	
	clip = atGRPX_GetClipRect(lpCanvas);

	while(1)
	{
		dist += nDashLen;
		rdist = INTTOREAL(dist);
		if(rdist>=len)
		{
			x2 = EndX;
			y2 = EndY;
			if(x1<clip->nStartX) x1 = clip->nStartX;
			if(y1<clip->nStartY) y1 = clip->nStartY;
			_Line2(lpCanvas, x1, y1, x2, y2, ExParam1);
			init_gapdash = REALTOINT(rdist - len) - nDashLen;
			break;
		}
		else
		{
			x2 = StartX + REALTOINT(dist * cosval);
			y2 = StartY + REALTOINT(dist * sinval);
			if(x2<clip->nStartX) x2 = clip->nStartX;
			if(x2>clip->nEndX) x2 = clip->nEndX;
			if(y2<clip->nStartY) y2 = clip->nStartY;
			if(y2>clip->nEndY) y2 = clip->nEndY;
			_Line2(lpCanvas, x1, y1, x2, y2, ExParam1);
		}
		
		dist += nGap;
		rdist = INTTOREAL(dist);
		if(rdist>=len)
		{
			init_gapdash = REALTOINT(rdist - len);
			break;
		}
		x1 = StartX + REALTOINT(dist * cosval);
		y1 = StartY + REALTOINT(dist * sinval);
	}

	return init_gapdash;
}

static void _Pixel( atGRPX_CANVAS *lpCanvas, atINT X, atINT Y, atLONG ExParam1 )
{
	atGRPX_DrawPixel( lpCanvas, X, Y, (atGRPX_COLOR)ExParam1 );
}

static void _HorizLine( atGRPX_CANVAS *lpCanvas, atINT StartX, atINT StartY, atINT Length, atLONG ExParam1 )
{
	atGRPX_DrawHorizLine( lpCanvas, StartX, StartY, Length, (atGRPX_COLOR)ExParam1 );
}

static int _HorizLineDash( atGRPX_CANVAS *lpCanvas, atINT StartX, atINT StartY, atINT Length, atLONG ExParam1, atINT nDashLen, atINT nGap, atINT init_gapdash )
{
	init_gapdash = atGRPX_DrawHorizLineDash( lpCanvas, StartX, StartY, Length, (atGRPX_COLOR)ExParam1, nDashLen, nGap, init_gapdash );
	return init_gapdash;
}

static void _VertLine( atGRPX_CANVAS *lpCanvas, atINT StartX, atINT StartY, atINT Length, atLONG ExParam1 )
{
	atGRPX_DrawVertLine( lpCanvas, StartX, StartY, Length, (atGRPX_COLOR)ExParam1 );
}

static int _VertLineDash( atGRPX_CANVAS *lpCanvas, atINT StartX, atINT StartY, atINT Length, atLONG ExParam1, atINT nDashLen, atINT nGap, atINT init_gapdash )
{
	init_gapdash = atGRPX_DrawVertLineDash( lpCanvas, StartX, StartY, Length, (atGRPX_COLOR)ExParam1, nDashLen, nGap, init_gapdash );
	return init_gapdash;
}


/*******************************
  양 끝 점을 잇는 선을 그린다.
  (Anti Aliasing 버전)
********************************/
void atGRPX_DrawLineAA_REAL( atGRPX_CANVAS *lpCanvas, atREAL rStartX, atREAL rStartY, atREAL rEndX, atREAL rEndY, atGRPX_COLOR Color )
{
    int i;
	int xx, yy;
//    atGRPX_PIXEL *p;
	atREAL x, y;
    atREAL dx, dy;
	atREAL dd;
    int inc;
	int r;
	  atREAL t;
	  int len;
	  atGRPX_PIXEL color = COLOR_TO_PIXEL( Color );
	  atRRECT rtClipR;
	  atRECT *lpClip;

	lpClip = atGRPX_GetClipRect(lpCanvas);
	rtClipR.rStartX = INTTOREAL(lpClip->nStartX);
	rtClipR.rStartY = INTTOREAL(lpClip->nStartY);
	rtClipR.rEndX = INTTOREAL(lpClip->nEndX);
	rtClipR.rEndY = INTTOREAL(lpClip->nEndY);
	 r = __atGRPX_GetClippedEdge_REAL( &rStartX, &rStartY, &rEndX, &rEndY, rStartX, rStartY, rEndX, rEndY, &rtClipR );
	 if( r == -1 ) return;

    dx = rEndX - rStartX;
	if( dx >= 0 ) dx++; else dx--;
    dy = rEndY - rStartY;
	if( dy >= 0 ) dy++; else dy--;

    if( ABS(dx) >= ABS(dy) ){
	  if( dx < 0 ){
	  	SWAP( rStartX, rEndX, t );
	  	SWAP( rStartY, rEndY, t );
	  }
	  
	 len = REALTOINT(rEndX) - REALTOINT(rStartX);
	
  	  xx = REALTOINT(rStartX);
	  if( REALREALK( rStartX ) > 0 ){
	  	xx++;
	  	len--;
	  } else {
	  }
	  if( REALREALK( rEndX ) > 0 ){
	  	len--;
	  }
	  
	  inc = 1;
	  y = rStartY;
	  dd = REALDIV(dy, dx);

      for( i = 0; i < len; i ++ ){
//		p = (atGRPX_PIXEL *)atGRPX_GetFrameBufferPtrXY( lpCanvas, xx, REALTOINT(y) );
//             *p = color;
atGRPX_DrawPixelAA( lpCanvas, xx, REALTOINT(y), INTTOREAL(0), REALREALK(y), Color );
		xx ++;
		y += dd;
      }
    } else {
	  if( dy < 0 ){
	  	SWAP( rStartY, rEndY, t );
	  	SWAP( rStartX, rEndX, t );
	  }
	  
	 len = REALTOINT(rEndY) - REALTOINT(rStartY);
	
  	  yy = REALTOINT(rStartY);
	  if( REALREALK( rStartY ) > 0 ){
atGRPX_DrawPixelAA( lpCanvas, REALTOINT(rStartX), yy,  REALREALK(rStartX), INTTOREAL(0), Color );
	  	yy++;
	  	len--;
	  } else {
	  }
	  if( REALREALK( rEndY ) > 0 ){
atGRPX_DrawPixelAA( lpCanvas, REALTOINT(rEndX), INTTOREAL(rEndY), REALREALK(rEndX), INTTOREAL(0), Color );
	  	len--;
	  }
	  
	  inc = 1;
	  x = rStartX;
	  dd = REALDIV(dx, dy);

      for( i = 0; i < len; i ++ ){
//		p = (atGRPX_PIXEL *)atGRPX_GetFrameBufferPtrXY( lpCanvas, REALTOINT(x), yy );
//             *p = color;
atGRPX_DrawPixelAA( lpCanvas, REALTOINT(x), yy,  REALREALK(x), INTTOREAL(0), Color );
		yy ++;
		x += dd;
      }
    }
}


/*******************************
  양 끝 점을 잇는 선을 그린다.
********************************/
void atGRPX_DrawLineAA( atGRPX_CANVAS *lpCanvas, atINT StartX, atINT StartY, atINT EndX, atINT EndY, atGRPX_COLOR Color )
{
	__atGRPX_DrawLine0( lpCanvas, StartX, StartY, EndX, EndY, (atLONG)Color, _Pixel, _Line2, _HorizLine, _VertLine ); 
//	atGRPX_DrawLineAA_REAL( lpCanvas, INTTOREAL(StartX), INTTOREAL(StartY), INTTOREAL(EndX), INTTOREAL(EndY), (atLONG)Color );
}


/*******************************
  양 끝 점을 잇는 선을 그린다.
********************************/
int atGRPX_DrawLineDashAA( atGRPX_CANVAS *lpCanvas, atINT StartX, atINT StartY, atINT EndX, atINT EndY, atGRPX_COLOR Color, atINT nDashLen, atINT nGap, atINT init_gapdash )
{
	init_gapdash = __atGRPX_DrawLineDash0( lpCanvas, StartX, StartY, EndX, EndY, (atLONG)Color, nDashLen, nGap, init_gapdash, _Pixel, _LineDash, _HorizLineDash, _VertLineDash ); 
	return init_gapdash;
}


/*******************************
  양 끝 점을 잇는 선을 그린다.
********************************/
int atGRPX_DrawLineDotAA( atGRPX_CANVAS *lpCanvas, atINT StartX, atINT StartY, atINT EndX, atINT EndY, atGRPX_COLOR Color, atINT nGap, atINT init_gapdash )
{
	init_gapdash = __atGRPX_DrawLineDash0( lpCanvas, StartX, StartY, EndX, EndY, (atLONG)Color, 2, nGap, init_gapdash, _Pixel, _LineDash, _HorizLineDash, _VertLineDash ); 
	return init_gapdash;
}
