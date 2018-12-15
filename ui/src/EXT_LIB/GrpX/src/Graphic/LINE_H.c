/*#############################################################################
-------------------------------------------------------------------------------

                                  LINE_H.c
                           (* =================== *)


 Version           : 1.0.0
 Dependent Modules :
 Author            : Ju, Gyeong-min ( Amos )
 Create            : 2001/04/25    01:47:09am
 Update            : 2003/04/29    00:00:00am
 C Compiler        : ADS 1.01, SDT 2.51, Borland C/C++ 5.0, Visual C/C++ 6.0
 Assembler         : ADS 1.01, SDT 2.51, Borland TASM 3.1, MASM 6.0
 Description       : H Line function definition

-------------------------------------------------------------------------------
#############################################################################*/
//#define __X86_

#include "GrpX.h"


/***********************
  수평선을 그린다.
************************/
void atGRPX_DrawHorizLine( atGRPX_CANVAS *lpCanvas, atINT StartX, atINT StartY, atINT Length, atGRPX_COLOR Color )
{
    int t;
    atGRPX_PIXEL *p;
    atRECT *clip;
	atGRPX_PIXEL cl = COLOR_TO_PIXEL(Color);

    if( Length <= 0 ) return;

    clip = atGRPX_GetClipRect(lpCanvas);

    if( (StartY < clip->nStartY) || (StartY > clip->nEndY) ) return;

    if( StartX > clip->nEndX ) return;
    if( StartX < clip->nStartX ){
      Length -= clip->nStartX - StartX;
      StartX = clip->nStartX;
    }
    t = StartX + Length - 1;
    if( t < clip->nStartX ) return;
    if( t > clip->nEndX ){
      Length -= t - clip->nEndX;
    }

    p = atGRPX_GetFrameBufferPtrXY( lpCanvas, StartX, StartY );

	while(Length-- > 0)
   		*p++ = cl;
}

int atGRPX_DrawHorizLineDash( atGRPX_CANVAS *lpCanvas, atINT StartX, atINT StartY, atINT Length, atGRPX_COLOR Color, atINT nDashLen, atINT nGap, atINT init_gapdash )
{
    int t, EndX, x1, y1, x2, span, dist, init_gap, init_dash;
    atGRPX_PIXEL *p;
    atRECT *clip;
	atGRPX_PIXEL cl = COLOR_TO_PIXEL(Color);
	
    if( Length <= 0 ) return init_gapdash;
	
    clip = atGRPX_GetClipRect(lpCanvas);
	
    if( (StartY < clip->nStartY) || (StartY > clip->nEndY) ) return init_gapdash;
	
    if( StartX > clip->nEndX ) return init_gapdash;
    if( StartX < clip->nStartX ){
		Length -= clip->nStartX - StartX;
		StartX = clip->nStartX;
    }
    t = StartX + Length - 1;
    if( t < clip->nStartX ) return init_gapdash;
    if( t > clip->nEndX ){
		Length -= t - clip->nEndX;
    }
	
	init_gap = init_gapdash > 0 ? init_gapdash : 0;
	init_dash = init_gapdash < 0 ? init_gapdash : 0;
	
	if(nGap < 1)
		nGap = 1;
	
	dist = init_gap + init_dash;
	
	if(dist > Length)
		return dist - Length;
	
	EndX = StartX + Length;
	x1 = StartX + dist;
	y1 = StartY;
	
	while(1)
	{
		p = atGRPX_GetFrameBufferPtrXY( lpCanvas, x1, y1 );
		dist += nDashLen;
		if(dist>=Length)
		{
			x2 = EndX;
			span = x2 - x1;
			while(span-- > 0)
				*p++ = cl;
			init_gapdash = dist - Length - nDashLen;
			break;
		}
		else
		{
			x2 = StartX + dist;
			if(x2>clip->nEndX )
				x2 = clip->nEndX;
			span = x2 - x1;
			while(span-- > 0)
				*p++ = cl;
		}
		
		dist += nGap;
		if(dist>=Length)
		{
			init_gapdash = dist - Length;
			break;
		}
		x1 = StartX + dist;
		y1 = StartY;
	}
	return init_gapdash;
}
