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
#include "GrpX_samsung.h"
#include "_AA_func.h"
#include "_LIN_Int.h"
#include "_LIN_func.h"
#include "math.h" //for Bresenham
#include "_BRE_func_samsung.h"

unsigned long Sqrt0( unsigned long v )
{
    signed long eax, ebx, ecx, edx;
	
    eax = v;
    ecx = 0;
	
    ebx = 0x40000000;
	
LP1:
    edx = eax;
    edx -= ebx;
    if( edx < 0 ) goto LP2;
	
    edx -= ecx;
    if( edx < 0 ) goto LP2;
	
    eax = edx;
    ecx >>= 1;  // 논리 쉬프트로 해야 함.
    ecx |= ebx;
	
    ebx >>= 2;
    if( ebx > 0 ) goto LP1;
    goto LP3;
	
LP2:
    ecx >>= 1;
    ebx >>= 2;
    if( ebx > 0 ) goto LP1;
	
LP3:
    eax = ecx;
	
    return( (unsigned long)eax );
}

/*******************************
  양 끝 점을 잇는 선을 그린다.
********************************/
static void _Line( atGRPX_CANVAS *lpCanvas, atINT StartX, atINT StartY, atINT EndX, atINT EndY, atLONG ExParam1 )
{
#ifdef _USE_BRESENHAM

	atINT dx = EndX-StartX;
    atINT dy = EndY-StartY;
	atSHORT drawu_s = 0;
	atSHORT drawv_s = 0;
	atSHORT drawu_e = 0;
	atSHORT drawv_e = 0;
	atGRPX_PIXEL Color = (atGRPX_PIXEL)ExParam1;
	atINT du = ABS(dx);
	atINT dv = ABS(dy);
	atINT u = EndX;
	atINT v = EndY;
	
	atINT uincr_mem;		// 메모리 주소
	atINT vincr_mem;
	atINT uincr;			//Screen 좌표
	atINT vincr;
	
	atINT uend;
    atINT d;				/* Initial value as in Bresenham's */
    atINT incrS;			/* Δd for straight increments */
    atINT incrD;			/* Δd for diagonal increments */
    atINT twovdu;			/* Numerator of distance; starts at 0 */

    atREAL invD;			/* Precomputed inverse denominator */
    atREAL invD2du;		/* Precomputed constant */

	atBOOL inverse;
	atINT alpha_1 = 0;
	atINT alpha_2 = 0;

	atREAL diff_distance = 0;
	
	atGRPX_PIXEL* addr_s; // 픽셀을 출력할 버퍼의 위치
	atGRPX_PIXEL* addr_e; // 픽셀을 출력할 버퍼의 위치
	
	atBOOL isOdd = atTRUE;
	atRECT* clip;

	atINT alpha_var_1 = 0; // 알파값 매개 변수
	atINT alpha_var_2 = 0;

    /* By switching to (u,v), we combine all eight octants */
    if (ABS(dx) > ABS(dy))
    {
	/* Note: If this were actual C, these integers would be lost
	* at the closing brace.  That's not what I mean to do.  Do what
		* I mean. */
		du = ABS(dx);
		dv = ABS(dy);
		u = EndX;
		v = EndY;
		drawu_s = StartX;
		drawv_s = StartY;
		drawu_e = EndX;
		drawv_e = EndY;
		uincr_mem = 1;
		vincr_mem = lpCanvas->nVMemWidth;
		uincr = 1;
		vincr = 1;
		if (dx < 0)
		{
			uincr_mem = -uincr_mem;
			uincr = -uincr;
		}
		if (dy < 0)
		{
			vincr_mem = -vincr_mem;
			vincr = -vincr;
		}
		inverse = atFALSE;
    }
    else
    {
		du = ABS(dy);
		dv = ABS(dx);
		u = EndY;
		v = EndX;
		drawu_s = StartY;
		drawv_s = StartX;
		drawu_e = EndY;
		drawv_e = EndX;
		uincr_mem = lpCanvas->nVMemWidth;
		vincr_mem = 1;
		uincr = 1;
		vincr = 1;
		if (dy < 0)
		{
			uincr_mem = -uincr_mem;
			uincr = -uincr;
		}
		if (dx < 0)
		{
			vincr_mem = -vincr_mem;
			vincr = -vincr ;
		}
		inverse = atTRUE;
    }
    
	if(du % 2)
	{
		isOdd = atTRUE;
	}
	else
	{
		isOdd = atFALSE;
	}

	uend = u + (du>>1);
    d = (2 * dv) - du;	    /* Initial value as in Bresenham's */
    incrS = 2 * dv;	/* Δd for straight increments */
    incrD = 2 * (dv - du);	/* Δd for diagonal increments */
    twovdu = 0;	/* Numerator of distance; starts at 0 */
  
	//for AA
	invD = REALDIV(INTTOREAL(1),INTTOREAL(2 * IntSqrt((unsigned long)(du*du) + (unsigned long)(dv*dv))));
	invD2du = REALMUL(INTTOREAL(2),REALMUL(INTTOREAL(du),invD));   /* Precomputed constant */ //코사인 
    
	addr_s = atGRPX_GetFrameBufferPtrXY( lpCanvas, StartX, StartY );
	addr_e = atGRPX_GetFrameBufferPtrXY( lpCanvas, EndX, EndY );

	clip = atGRPX_GetClipRect( lpCanvas );

	do
    {
		diff_distance = REALMUL(invD, INTTOREAL(twovdu));
		alpha_var_1 = REALTOINT(REALMUL(invD2du, INTTOREAL(100)));
		alpha_var_2 = REALTOINT(REALMUL(diff_distance,INTTOREAL(100)));

		
		if(inverse)
		{
			if(drawv_s == clip->nStartX || drawv_s == clip->nEndX || drawv_e == clip->nStartX || drawv_e == clip->nEndX
				|| drawu_s == clip->nStartY || drawu_s == clip->nEndY || drawu_s == clip->nStartY || drawu_s == clip->nEndY)
			{
				atGRPX_DrawPixelBrePixel(lpCanvas, drawv_s, drawu_s, Color, alpha_var_2, clip);
				atGRPX_DrawPixelBrePixel(lpCanvas, drawv_s + vincr, drawu_s, Color, alpha_var_1 - alpha_var_2, clip);
				atGRPX_DrawPixelBrePixel(lpCanvas, drawv_s - vincr, drawu_s, Color, alpha_var_1 + alpha_var_2, clip);
				
				atGRPX_DrawPixelBrePixel(lpCanvas, drawv_e, drawu_e, Color, alpha_var_2, clip);
				atGRPX_DrawPixelBrePixel(lpCanvas, drawv_e - vincr, drawu_e, Color, alpha_var_1 - alpha_var_2, clip);
				atGRPX_DrawPixelBrePixel(lpCanvas, drawv_e + vincr, drawu_e, Color, alpha_var_1 + alpha_var_2, clip);

			}
			else
			{	
				atGRPX_DrawPixelBreMir( addr_s, addr_e, vincr_mem, Color, alpha_var_1, alpha_var_2);
			}

		}
		else
		{
			if(drawu_s == clip->nStartX || drawu_s == clip->nEndX || drawu_s == clip->nStartX || drawu_s == clip->nEndX
				||drawu_s == clip->nStartY || drawu_s == clip->nEndY || drawu_s == clip->nStartY || drawu_s == clip->nEndY)
			{
				atGRPX_DrawPixelBrePixel(lpCanvas, drawu_s, drawv_s, Color, alpha_var_2, clip);
				atGRPX_DrawPixelBrePixel(lpCanvas, drawu_s, drawv_s + vincr, Color, alpha_var_1 - alpha_var_2, clip);
				atGRPX_DrawPixelBrePixel(lpCanvas, drawu_s, drawv_s - vincr, Color, alpha_var_1 + alpha_var_2, clip);
				
				atGRPX_DrawPixelBrePixel(lpCanvas, drawu_e, drawv_e, Color, alpha_var_2, clip);
				atGRPX_DrawPixelBrePixel(lpCanvas, drawu_e, drawv_e - vincr, Color, alpha_var_1 - alpha_var_2, clip);
				atGRPX_DrawPixelBrePixel(lpCanvas, drawu_e, drawv_e + vincr, Color, alpha_var_1 + alpha_var_2, clip);
			}
			else
			{
				atGRPX_DrawPixelBreMir( addr_s, addr_e, vincr_mem, Color,  alpha_var_1, alpha_var_2);
			}

		}

		if (d < 0)
		{
			/* choose straight (u direction) */
			twovdu = d + du;
			d = d + incrS;
		}
		else
		{
			/* choose diagonal (u+v direction) */
			twovdu = d - du;
			d = d + incrD;
			v = v+1;
			addr_s = addr_s + vincr_mem;
			addr_e = addr_e - vincr_mem;
			drawv_s = drawv_s + vincr;
			drawv_e = drawv_e - vincr;
		}
		u = u+1;
		addr_s = addr_s + uincr_mem;
		addr_e = addr_e - uincr_mem;
		drawu_s = drawu_s + uincr;
		drawu_e = drawu_e - uincr;
    } while (u < uend);

	if(!isOdd)
	{
		diff_distance = REALMUL(invD, INTTOREAL(twovdu));
		atGRPX_DrawPixelBre(addr_s, vincr_mem, Color, REALTOINT(REALMUL(invD2du, INTTOREAL(100))), REALTOINT(REALMUL(diff_distance,INTTOREAL(100))));
	}
	else
	{
		diff_distance = REALMUL(invD, INTTOREAL(twovdu));
		atGRPX_DrawPixelBreMir( addr_s, addr_e, vincr_mem, Color, REALTOINT(REALMUL(invD2du, INTTOREAL(100))), REALTOINT(REALMUL(diff_distance,INTTOREAL(100))));	
	}


#else
    int i, i2;
	atREAL x1, y1, x2, y2;
	atREAL dx, dy;
	atGRPX_PIXEL Color = (atGRPX_PIXEL)ExParam1;
	atREAL ratio, kdv;
	int r, g, b;
	atGRPX_PIXEL *pBuf = atGRPX_GetFrameBufferPtrXY( lpCanvas, StartX, StartY );
	int vmem_width = atGRPX_GetVMemWidth(lpCanvas);
	
	r = GET_RED(Color); g = GET_GREEN(Color); b = GET_BLUE(Color);

	x1 = INTTOREAL(StartX);
	y1 = INTTOREAL(StartY);
	x2 = INTTOREAL(EndX);
	y2 = INTTOREAL(EndY);
	dx = x2 - x1;
	dy = y2 - y1;

//	__AA_DrawPixelRealAA( pBuf, INTOREAL(StartX), INTOREAL(StartY), r, g, b ); // 어차피 실수부는 0이니까 쓸모 없음.
//	__AA_DrawPixelRealAA( pBuf, INTOREAL(EndX), INTOREAL(EndY), r, g, b ); // 어차피 실수부는 0이니까 쓸모 없음.

    kdv = INTTOREAL(0);
    if( ABS(dy) >= ABS(dx) ){
	  ratio = REALDIV(dx, dy);
	  if( dy > 0 ){
	        for( i = StartY; i <= EndY; i++, kdv += ratio ){
//			__AA_DrawPixelRealAA_H( pBuf + REALTOINT(kdv), kdv, r, g, b );
//			__AA_DrawPixelRealAA( pBuf + REALTOINT(kdv), vmem_width, kdv, INTTOREAL(i), r, g, b );
__AA_DrawPixelAA_H( pBuf + REALTOINT(kdv), kdv, r, g, b );
			pBuf += vmem_width;
		}
	  } else {
	        for( i = StartY; i >= EndY; i--, kdv -= ratio ){
//			__AA_DrawPixelRealAA_H( pBuf + REALTOINT(kdv), kdv, r, g, b );
//			__AA_DrawPixelRealAA( pBuf + REALTOINT(kdv), vmem_width, kdv, INTTOREAL(i), r, g, b );
__AA_DrawPixelAA_H( pBuf + REALTOINT(kdv), kdv, r, g, b );
			pBuf -= vmem_width;
	        }
	  }
    } else {
	  ratio = REALDIV(dy, dx);
	  if( dx > 0 ){
	        for( i = StartX; i <= EndX; i++, kdv += ratio ){
//			__AA_DrawPixelRealAA_V( pBuf + REALTOINT(kdv) * vmem_width, vmem_width, kdv, r, g, b );
//			__AA_DrawPixelRealAA( pBuf + REALTOINT(kdv) * vmem_width, vmem_width, INTTOREAL(i), kdv, r, g, b );
__AA_DrawPixelAA_V( pBuf + REALTOINT(kdv) * vmem_width, vmem_width, kdv, r, g, b );
			pBuf++;
	        }
	  } else {
	        for( i = StartX; i >= EndX; i--, kdv -= ratio ){
//			__AA_DrawPixelRealAA_V( pBuf + REALTOINT(kdv) * vmem_width, vmem_width, kdv, r, g, b );
//			__AA_DrawPixelRealAA( pBuf + REALTOINT(kdv) * vmem_width, vmem_width, INTTOREAL(i), kdv, r, g, b );
__AA_DrawPixelAA_V( pBuf + REALTOINT(kdv) * vmem_width, vmem_width, kdv, r, g, b );
			pBuf--;
	        }
	  }
    }
#endif
}

static void _Pixel( atGRPX_CANVAS *lpCanvas, atINT X, atINT Y, atLONG ExParam1 )
{
	atGRPX_DrawPixel( lpCanvas, X, Y, (atGRPX_COLOR)ExParam1 );
}

static void _HorizLine( atGRPX_CANVAS *lpCanvas, atINT StartX, atINT StartY, atINT Length, atLONG ExParam1 )
{
	atGRPX_DrawHorizLine( lpCanvas, StartX, StartY, Length, (atGRPX_COLOR)ExParam1 );
}

static void _VertLine( atGRPX_CANVAS *lpCanvas, atINT StartX, atINT StartY, atINT Length, atLONG ExParam1 )
{
	atGRPX_DrawVertLine( lpCanvas, StartX, StartY, Length, (atGRPX_COLOR)ExParam1 );
}


/*******************************
  양 끝 점을 잇는 선을 그린다.
  (Anti Aliasing 버전)
********************************/
void atGRPX_DrawLineAA_Samsung( atGRPX_CANVAS *lpCanvas, atINT StartX, atINT StartY, atINT EndX, atINT EndY, atGRPX_COLOR Color )
{
	__atGRPX_DrawLine0( lpCanvas, StartX, StartY, EndX, EndY, (atLONG)Color,
		 			    _Pixel, _Line, _HorizLine, _VertLine ); 
}

