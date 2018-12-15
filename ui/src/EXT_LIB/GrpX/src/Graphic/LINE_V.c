/*#############################################################################
-------------------------------------------------------------------------------

                                  Line_V.c
                           (* =================== *)


 Version           : 1.0.0
 Dependent Modules :
 Author            : Ju, Gyeong-min ( Amos )
 Create            : 2001/04/25    01:47:09am
 Update            : 2003/04/29    00:00:00am
 C Compiler        : ADS 1.01, SDT 2.51, Borland C/C++ 5.0, Visual C/C++ 6.0
 Assembler         : ADS 1.01, SDT 2.51, Borland TASM 3.1, MASM 6.0
 Description       : Vertical line function definition

-------------------------------------------------------------------------------
#############################################################################*/
#include "GrpX.h"


/*******************
  수직선을 그린다.
********************/
void atGRPX_DrawVertLine( atGRPX_CANVAS *lpCanvas, atINT StartX, atINT StartY, atINT Length, atGRPX_COLOR Color )
{
    int t;
    atGRPX_PIXEL *p;
    atRECT *clip;
	int inc;
	atGRPX_PIXEL cl = (atGRPX_PIXEL)Color;

    if( Length <= 0 ) return;

    clip = atGRPX_GetClipRect(lpCanvas);

    if( (StartX < clip->nStartX) || (StartX > clip->nEndX) ) return;
    if( StartY > clip->nEndY ) return;
    t = StartY + Length - 1;
    if( t < clip->nStartY ) return;

    if( t > clip->nEndY ){
      Length -= t - clip->nEndY;
    }
    if( StartY < clip->nStartY ){
      Length -= clip->nStartY - StartY;
      StartY = clip->nStartY;
    }
    
    p = atGRPX_GetFrameBufferPtrXY( lpCanvas, StartX, StartY );

	inc = atGRPX_GetVMemWidth(lpCanvas);

    while( Length-- > 0 ){
      *p = cl;
      p += inc;
    }
}

int atGRPX_DrawVertLineDash( atGRPX_CANVAS *lpCanvas, atINT StartX, atINT StartY, atINT Length, atGRPX_COLOR Color, atINT nDashLen, atINT nGap, atINT init_gapdash )
{
    int t, EndY, x1, y1, y2, span, dist, init_gap, init_dash;
    atGRPX_PIXEL *p;
    atRECT *clip;
	int inc;
	atGRPX_PIXEL cl = (atGRPX_PIXEL)Color;
	
    if( Length <= 0 ) return init_gapdash;
	
    clip = atGRPX_GetClipRect(lpCanvas);
	
    if( (StartX < clip->nStartX) || (StartX > clip->nEndX) ) return init_gapdash;
    if( StartY > clip->nEndY ) return init_gapdash;
    t = StartY + Length - 1;
    if( t < clip->nStartY ) return init_gapdash;
	
    if( t > clip->nEndY ){
		Length -= t - clip->nEndY;
    }
    if( StartY < clip->nStartY ){
		Length -= clip->nStartY - StartY;
		StartY = clip->nStartY;
    }
	
	init_gap = init_gapdash > 0 ? init_gapdash : 0;
	init_dash = init_gapdash < 0 ? init_gapdash : 0;
	
	if(nGap < 1)
		nGap = 1;
	
	dist = init_gap + init_dash;
	
	EndY = StartY + Length;
	if(dist > Length)
		return dist - Length;

	inc = atGRPX_GetVMemWidth(lpCanvas);
	
	x1 = StartX;
	y1 = StartY + dist;
	if(y1<0 )
		y1 = 0;
	
	while(1)
	{
		p = atGRPX_GetFrameBufferPtrXY( lpCanvas, x1, y1 );
		dist += nDashLen;
		if(dist>=Length)
		{
			y2 = EndY;
			span = y2 - y1;
			while(span-- > 0)
			{
				*p = cl;
				p += inc;
			}
			init_gapdash = dist - Length - nDashLen;
			break;
		}
		else
		{
			y2 = StartY + dist;
			if(y2>clip->nEndY )
				y2 = clip->nEndY;
			span = y2 - y1;
			while(span-- > 0)
			{
				*p = cl;
				p += inc;
			}
		}
		
		dist += nGap;
		if(dist>=Length)
		{
			init_gapdash = dist - Length;
			break;
		}
		x1 = StartX;
		y1 = StartY + dist;
	}
	return init_gapdash;
}
