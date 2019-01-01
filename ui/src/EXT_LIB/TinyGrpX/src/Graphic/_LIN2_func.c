#include "TinyGrpX.h"
#include "A_Math.h"
#include "_AB_int.h"
#include "_AB_table.h"
#include "_ClipPoly_func.h"


atVOID __atTGRPX_DrawLine1( atTGRPX_CANVAS *lpCanvas, atINT StartX, atINT StartY, atINT EndX, atINT EndY,	atTGRPX_COLOR clrColor )
{
	atREAL x1, y1, x2, y2;
	atREAL dx, dy;
	atTGRPX_PIXEL *pBuf;
	atRECT *clip;
	
	int xpxl1, ypxl1, xpxl2, ypxl2, x, y;
	atREAL inter, gradient;
	atREAL tmp;
	atREAL rA, rInvA;
// 	atINT nA, nInvA;
// 	atTGRPX_COLOR sum;
	
	int r, g, b, tr, tg, tb;
	atTGRPX_PIXEL cc;
	r = GET_RED(clrColor); g = GET_GREEN(clrColor); b = GET_BLUE(clrColor);
	
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
		xpxl1 = REALTOINT_R(x1);
		xpxl2 = REALTOINT(x2);
		y = REALTOINT(StartY);
		for(x=xpxl1; x<=xpxl2; x++)
		{
			pBuf = atTGRPX_GetFrameBufferPtrXY( lpCanvas, x, y );
			*pBuf = (atTGRPX_PIXEL)clrColor;
		}
		return;
	}

	clip = atTGRPX_GetClipRect(lpCanvas);
    if( ABS(dy) >= ABS(dx) )
	{
		if(y2 < y1)
		{
			SWAP(x1, x2, tmp);
			SWAP(y1, y2, tmp);
		}
		gradient = REALDIV(dx, dy);
		ypxl1 = REALTOINT_R(y1);	// handle first endpoint
		ypxl2 = REALTOINT(y2);	// handle second endpoint
		
		// main loop
		inter = x1;				// first y-intersection for the main loop
		for(y=ypxl1; y<=ypxl2; y++)
		{
			x = REALTOINT(inter);
			rA = REALREALK(inter);
			rInvA = REAL_ONE - rA;
			pBuf = atTGRPX_GetFrameBufferPtrXY( lpCanvas, x, y );
			if(x>=clip->nStartX)
//				__ALPHA_BLEND565(pBuf, clrColor, rInvA, rA, nA, nInvA, sum);
				__ALPHA_PIXEL_R(pBuf, r, g, b, cc, tr, tg, tb, rInvA, rA);
			if(x+1<=clip->nEndX)
//				__ALPHA_BLEND565(pBuf+1, clrColor, rA, rInvA, nA, nInvA, sum);
				__ALPHA_PIXEL_R(pBuf+1, r, g, b, cc, tr, tg, tb, rA, rInvA);
			inter += gradient;
		}
    }
	else
	{
		if(x2 < x1)
		{
			SWAP(x1, x2, tmp);
			SWAP(y1, y2, tmp);
		}
		gradient = REALDIV(dy, dx);
		xpxl1 = REALTOINT_R(x1);	// handle first endpoint
		xpxl2 = REALTOINT(x2);	// handle second endpoint
		
		// main loop
		inter = y1;				// first y-intersection for the main loop
		for(x=xpxl1; x<=xpxl2; x++)
		{
			y = REALTOINT(inter);
			rA = REALREALK(inter);
			rInvA = REAL_ONE - rA;
			pBuf = atTGRPX_GetFrameBufferPtrXY( lpCanvas, x, y );
			if(y>=clip->nStartY)
//				__ALPHA_BLEND565(pBuf, clrColor, rInvA, rA, nA, nInvA, sum);
				__ALPHA_PIXEL_R(pBuf, r, g, b, cc, tr, tg, tb, rInvA, rA);
			if(y+1<=clip->nEndY)
//				__ALPHA_BLEND565(pBuf+lpCanvas->nVMemWidth, clrColor, rA, rInvA, nA, nInvA, sum);
				__ALPHA_PIXEL_R(pBuf+lpCanvas->nVMemWidth, r, g, b, cc, tr, tg, tb, rA, rInvA);
			inter += gradient;
		}
    }
}

atVOID __atTGRPX_DrawLine1_REAL( atTGRPX_CANVAS *lpCanvas, atREAL StartX, atREAL StartY, atREAL EndX, atREAL EndY, atTGRPX_COLOR clrColor )
{
	atREAL x1, y1, x2, y2;
	atREAL dx, dy;
	atTGRPX_PIXEL *pBuf;
	atRECT *clip;
	
	int xpxl1, ypxl1, xpxl2, ypxl2, x, y;
	atREAL inter, gradient;
	atREAL tmp;
	atREAL rA, rInvA;
// 	atINT nA, nInvA;
// 	atTGRPX_COLOR sum;
	
	int r, g, b, tr, tg, tb;
	atTGRPX_PIXEL cc;
	r = GET_RED(clrColor); g = GET_GREEN(clrColor); b = GET_BLUE(clrColor);
	
	if(StartX==EndX && StartY==EndY) return;
	
	x1 = StartX;
	y1 = StartY;
	x2 = EndX;
	y2 = EndY;
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
		y = REALTOINT(StartY);
		for(x=xpxl1; x<=xpxl2; ++x)
		{
			pBuf = atTGRPX_GetFrameBufferPtrXY( lpCanvas, x, y );
			*pBuf = (atTGRPX_PIXEL)clrColor;
		}
		return;
	}
	
	clip = atTGRPX_GetClipRect(lpCanvas);
    if( ABS(dy) >= ABS(dx) )
	{
		if(y2 < y1)
		{
			SWAP(x1, x2, tmp);
			SWAP(y1, y2, tmp);
			dx = x2 - x1;
			dy = y2 - y1;
		}
		gradient = REALDIV(dx, dy);
		
		ypxl1 = REALTOINT(y1);	// handle first endpoint
		ypxl2 = REALTOINT(y2);	// handle second endpoint
		
		// main loop
		inter = x1;				// first y-intersection for the main loop
		for(y=ypxl1; y<=ypxl2; ++y)
		{
			x = REALTOINT(inter);
			rA = REALREALK(inter);
			rInvA = REAL_ONE - rA;
			pBuf = atTGRPX_GetFrameBufferPtrXY( lpCanvas, x, y );
			if(x>=clip->nStartX)
//				__ALPHA_BLEND565(pBuf, clrColor, rInvA, rA, nA, nInvA, sum);
				__ALPHA_PIXEL_R(pBuf, r, g, b, cc, tr, tg, tb, rInvA, rA);
			if(x+1<=clip->nEndX)
//				__ALPHA_BLEND565(pBuf+1, clrColor, rA, rInvA, nA, nInvA, sum);
				__ALPHA_PIXEL_R(pBuf+1, r, g, b, cc, tr, tg, tb, rA, rInvA);
			inter += gradient;
		}
    }
	else
	{
		if(x2 < x1)
		{
			SWAP(x1, x2, tmp);
			SWAP(y1, y2, tmp);
			dx = x2 - x1;
			dy = y2 - y1;
		}
		gradient = REALDIV(dy, dx);
		
		xpxl1 = REALTOINT(x1);	// handle first endpoint
		xpxl2 = REALTOINT(x2);	// handle second endpoint
		
		// main loop
		inter = y1;				// first y-intersection for the main loop
		for(x=xpxl1; x<=xpxl2; ++x)
		{
			y = REALTOINT(inter);
			rA = REALREALK(inter);
			rInvA = REAL_ONE - rA;
			pBuf = atTGRPX_GetFrameBufferPtrXY( lpCanvas, x, y );
			if(y>=clip->nStartY)
//				__ALPHA_BLEND565(pBuf, clrColor, rInvA, rA, nA, nInvA, sum);
				__ALPHA_PIXEL_R(pBuf, r, g, b, cc, tr, tg, tb, rInvA, rA);
			if(y+1<=clip->nEndY)
//				__ALPHA_BLEND565(pBuf+lpCanvas->nVMemWidth, clrColor, rA, rInvA, nA, nInvA, sum);
				__ALPHA_PIXEL_R(pBuf+lpCanvas->nVMemWidth, r, g, b, cc, tr, tg, tb, rA, rInvA);
			inter += gradient;
		}
    }
}
