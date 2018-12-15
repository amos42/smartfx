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
#include "_LIN_int.h"
#include "_LIN_func.h"


#define _USE_BRESENHAM		/// 브리슨헴 알고리즘을 쓸 것인가


/*******************************
  양 끝 점을 잇는 선을 그린다.
********************************/
static void _Line( atGRPX_CANVAS *lpCanvas, atINT StartX, atINT StartY, atINT EndX, atINT EndY, atLONG ExParam1 )
{
#ifdef _USE_BRESENHAM
    int i;
    atGRPX_PIXEL *p;
    int dx, dy;
    int inc, inc2, half, err;
    atGRPX_PIXEL color = (atGRPX_PIXEL)ExParam1;

	
    dx = EndX - StartX;
    dy = EndY - StartY;
//    if( dx < 0 ) dx = -dx;
//    if( dy < 0 ) dy = -dy;

    if( ABS(dx) >= ABS(dy) ){
      if( StartX > EndX ){
//        temp = StartX; StartX = EndX; EndX = temp;
//        temp = StartY; StartY = EndY; EndY = temp;
inc = -1;
      } else inc = 1;

	  inc2 = atGRPX_GetVMemWidth(lpCanvas);
      if(EndY < StartY)  inc2 = -inc2;

      half = ABS(dx) >> 1;
	  err = 0;

      p = (atGRPX_PIXEL *)atGRPX_GetFrameBufferPtrXY( lpCanvas, StartX, StartY );
	  if(!p) return;

//      for( i = StartX; i <= EndX; i ++ ){
      for( i = 0; i <= ABS(dx); i ++ ){
        *p = color;
        p += inc;
        err += ABS(dy);
        if( err > half ){
          p += inc2;
          err -= ABS(dx);
        }
      }
    } else {
      if( StartY > EndY ){
//        temp = StartX; StartX = EndX; EndX = temp;
//        temp = StartY; StartY = EndY; EndY = temp;
      }

      inc = (StartX <= EndX) ? 1 : -1;
	  inc2 = atGRPX_GetVMemWidth(lpCanvas);
      if(EndY < StartY)  inc2 = -inc2;

      half = ABS(dy) >> 1;
	  err = 0;

      p = atGRPX_GetFrameBufferPtrXY( lpCanvas, StartX, StartY );
	  if(!p) return;

//      for( i = StartY; i <= EndY; i ++ ){
      for( i = 0; i <= ABS(dy); i ++ ){
        *p = color;
        p += inc2;
        err += ABS(dx);
        if( err > half ){
          p += inc;
          err -= ABS(dy);
        }
      }
    }
#else
    int i, cnt;
	int xx, yy;
    atGRPX_PIXEL *p;
	atREAL x, y;
    int dx, dy;
	atREAL dd;
    int inc;
    atGRPX_PIXEL color = COLOR_TO_PIXEL((atGRPX_COLOR)ExParam1);


    dx = EndX - StartX;
	if( dx >= 0 ) dx++; else dx--;
    dy = EndY - StartY;
	if( dy >= 0 ) dy++; else dy--;

    if( ABS(dx) >= ABS(dy) ){
	  if( dx >= 0 ){
	    xx = StartX;
	    inc = 1;
		cnt = dx;
		y = INTTOREAL(StartY);
	    dd = INTTOREAL(dy) / dx;
	  } else {
	    xx = EndX;
	    inc = -1;
		cnt = -dx;
		y = INTTOREAL(EndY);
	    dd = INTTOREAL(dy) / dx;
	  }

      for( i = 0; i < cnt; i ++ ){
		p = (atGRPX_PIXEL *)atGRPX_GetFrameBufferPtrXY( lpCanvas, xx, REALTOINT(y) );
        *p = color;
//		atGRPX_DrawPixelTrans( lpCanvas, xx, REALTOINT(y), Color, 127 );
		xx ++;
		y += dd;
      }
    } else {
	  if( dy >= 0 ){
	    yy = StartY;
	    inc = 1;
		cnt = dy;
		x = INTTOREAL(StartX);
	    dd = INTTOREAL(dx) / dy;
	  } else {
	    yy = EndY;
	    inc = -1;
		cnt = -dy;
		x = INTTOREAL(EndX);
	    dd = INTTOREAL(dx) / dy;
	  }

      for( i = 0; i < cnt; i ++ ){
		p = (atGRPX_PIXEL *)atGRPX_GetFrameBufferPtrXY( lpCanvas, REALTOINT(x), yy );
        *p = color;
//		atGRPX_DrawPixelTrans( lpCanvas, REALTOINT(x), yy, Color, 127 );
		yy ++;
		x += dd;
      }
    }
#endif
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
			_Line(lpCanvas, x1, y1, x2, y2, ExParam1);
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
			_Line(lpCanvas, x1, y1, x2, y2, ExParam1);
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
		if(x1<clip->nStartX) x1 = clip->nStartX;
		if(x1>clip->nEndX) x1 = clip->nEndX;
		if(y1<clip->nStartY) y1 = clip->nStartY;
		if(y1>clip->nEndY) y1 = clip->nEndY;
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


void atGRPX_DrawLine_REAL( atGRPX_CANVAS *lpCanvas, atREAL rStartX, atREAL rStartY, atREAL rEndX, atREAL rEndY, atGRPX_COLOR Color )
{
    int i;
	int xx, yy;
    atGRPX_PIXEL *p;
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
	r =  __atGRPX_GetClippedEdge_REAL( &rStartX, &rStartY, &rEndX, &rEndY, rStartX, rStartY, rEndX, rEndY, &rtClipR );
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
		p = (atGRPX_PIXEL *)atGRPX_GetFrameBufferPtrXY( lpCanvas, xx, REALTOINT(y) );
             *p = color;
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
	  	yy++;
	  	len--;
	  } else {
	  }
	  if( REALREALK( rEndY ) > 0 ){
	  	len--;
	  }
	  
	  inc = 1;
	  x = rStartX;
	  dd = REALDIV(dx, dy);

      for( i = 0; i < len; i ++ ){
		p = (atGRPX_PIXEL *)atGRPX_GetFrameBufferPtrXY( lpCanvas, REALTOINT(x), yy );
        *p = color;
		yy ++;
		x += dd;
      }
    }
}


/*******************************
  양 끝 점을 잇는 선을 그린다.
********************************/
void atGRPX_DrawLine( atGRPX_CANVAS *lpCanvas, atINT StartX, atINT StartY, atINT EndX, atINT EndY, atGRPX_COLOR Color )
{
	__atGRPX_DrawLine0( lpCanvas, StartX, StartY, EndX, EndY, (atLONG)Color, _Pixel, _Line, _HorizLine, _VertLine ); 
//	atGRPX_DrawLine_REAL( lpCanvas, INTTOREAL(StartX), INTTOREAL(StartY), INTTOREAL(EndX), INTTOREAL(EndY), (atLONG)Color ); // 클리핑 영역에 문제가 있어서 일단 보류..
}


/*******************************
  양 끝 점을 잇는 선을 그린다.
********************************/
int atGRPX_DrawLineDash( atGRPX_CANVAS *lpCanvas, atINT StartX, atINT StartY, atINT EndX, atINT EndY, atGRPX_COLOR Color, atINT nDashLen, atINT nGap, atINT init_gapdash )
{
	init_gapdash = __atGRPX_DrawLineDash0( lpCanvas, StartX, StartY, EndX, EndY, (atLONG)Color, nDashLen, nGap, init_gapdash, _Pixel, _LineDash, _HorizLineDash, _VertLineDash ); 
	return init_gapdash;
}


/*******************************
  양 끝 점을 잇는 선을 그린다.
********************************/
int atGRPX_DrawLineDot( atGRPX_CANVAS *lpCanvas, atINT StartX, atINT StartY, atINT EndX, atINT EndY, atGRPX_COLOR Color, atINT nGap, atINT init_gapdash )
{
	init_gapdash = __atGRPX_DrawLineDash0( lpCanvas, StartX, StartY, EndX, EndY, (atLONG)Color, 2, nGap, init_gapdash, _Pixel, _LineDash, _HorizLineDash, _VertLineDash ); 
	return init_gapdash;
}
