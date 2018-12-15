/*#############################################################################
-------------------------------------------------------------------------------

                                  PUTSTR.c
                           (* =================== *)


 Version           : 1.0.0
 Dependent Modules :
 Author            : Ju, Gyeong-min ( Amos )
 Create            : 2001/04/25    01:47:09am
 Update            : 2003/04/29    00:00:00am
 C Compiler        : ADS 1.01, SDT 2.51, Borland C/C++ 5.0, Visual C/C++ 6.0
 Assembler         : ADS 1.01, SDT 2.51, Borland TASM 3.1, MASM 6.0
 Description       : Put string related fucntion definition

-------------------------------------------------------------------------------
#############################################################################*/

#include "GrpX.h"
#include "_STR_func.h"
#include "../Graphic/_AB_int.h"


static void PutW8FontNormal( atGRPX_PIXEL *di, atU8 *si, int sx, int w, int h, int wskip, int vmemwidth, atGRPX_PIXEL color )
{
    int i, j;
    int d;
	
	vmemwidth -= w*sizeof(atGRPX_PIXEL);
	vmemwidth /= sizeof(atGRPX_PIXEL);
    for( i = 0; i < h; i ++ ){
      d = *si++;
	  if( sx > 0 ) d <<= sx;
      for( j = 0; j < w; j ++ ){
        if( d & 0x80 ) *di = color;
        d <<= 1;
        di ++;
      }
      di += vmemwidth;
    }
}

static void PutW16FontNormal( atGRPX_PIXEL *di, atU16 *si, int sx, int w, int h, int wskip, int vmemwidth, atGRPX_PIXEL color )
{
    int i, j;
    int d;

	vmemwidth -= w*sizeof(atGRPX_PIXEL);
	vmemwidth /= sizeof(atGRPX_PIXEL);
    for( i = 0; i < h; i ++ ){
      d = *si++;
	  if( sx > 0 ) d <<= sx;
      for( j = 0; j < w; j ++ ){
        if( d & 0x8000 ) *di = color;
        d <<= 1;
        di ++;
      }
      di += vmemwidth;
    }
}

static void PutW32FontNormal( atGRPX_PIXEL *di, atU32 *si, int sx, int w, int h, int wskip, int vmemwidth, atGRPX_PIXEL color )
{
    int i, j, k;
    unsigned int d;
	atU8 *ssi, *ssi0;
	int cnt, ww;

	ssi = ssi0 = (atU8 *)si;
	vmemwidth -= w*sizeof(atGRPX_PIXEL);
	vmemwidth /= sizeof(atGRPX_PIXEL);
    for( i = 0; i < h; i ++ ){
	  ssi = ssi0;
	  if( sx > 0 ) d <<= sx;
      ww = w;
	  for( j = 0; j < 4; j ++ ){
        cnt = (ww > 8)? 8 : ww;
	    ww -= cnt;

	    d = *ssi++;
        for( k = 0; k < cnt; k ++ ){
          if( d & 0x80 ) *di = color;
          d <<= 1;
          di ++;
        }

        if( ww <= 0 ) break;
      }
	  ssi0 += 4;
      di += vmemwidth;
    }
}

static void PutBPP8FontNormal( atGRPX_PIXEL *di, atU8 *si, int sx, int w, int h, int wskip, int vmemwidth, atGRPX_PIXEL color )
{
    int i, j;
	atGRPX_PIXEL cc;
	atBYTE cl;
    atBYTE r0, g0, b0;
    atBYTE rr, gg, bb;
    atINT weight2;
atLPBYTE ss;	

    r0 = GET_RED(color); g0 = GET_GREEN(color); b0 = GET_BLUE(color);

	vmemwidth -= w*sizeof(atGRPX_PIXEL);
	vmemwidth /= sizeof(atGRPX_PIXEL);
si += sx;
    for( i = 0; i < h; i ++ ){
//ss = si + sx;		// Amos [Delete 2009/06/03-13:00]
ss = si;			// Amos [Add 2009/06/03-13:00]
	  for( j = 0; j < w; j ++ ){
        cl = *ss++;
        if( cl == 0xff ){
          *di = color;
        } else if( cl != 0x00 ){
          weight2 = 255 - cl;
          __ALPHA_PIXEL_I( di, r0,g0,b0, cc,rr,gg,bb, cl, weight2 );
        }
	    di ++;
	  }
	  si += wskip;
      di += vmemwidth;
    }
}


static void _draw_glyph( atU8 *di, atGRPX_GLYPHINFO *glp, atU8 *si, int sx, int w, int h, int wskip, int vmemwidth, atLONG ExParam1 )
{
	if( glp->nBPP == 1 ){
		if( glp->nFontWidthBytes == 1 )
			PutW8FontNormal( (atGRPX_PIXEL*)di, (atU8*)si, sx, w, h, wskip, vmemwidth, (atGRPX_PIXEL)ExParam1 );
		else if( glp->nFontWidthBytes == 2 )
			PutW16FontNormal( (atGRPX_PIXEL*)di, (atU16*)si, sx, w, h, wskip, vmemwidth, (atGRPX_PIXEL)ExParam1 );
		else if( glp->nFontWidthBytes == 4 )
			PutW32FontNormal( (atGRPX_PIXEL*)di, (atU32*)si, sx, w, h, wskip, vmemwidth, (atGRPX_PIXEL)ExParam1 );
	} else if( glp->nBPP == 8 ){
		PutBPP8FontNormal( (atGRPX_PIXEL*)di, (atU8*)si, sx, w, h, wskip, vmemwidth, (atGRPX_PIXEL)ExParam1 );
	}
	
}


/********************
   문자열을 찍는다.
*********************/
void atGRPX_DrawString( atGRPX_CANVAS *lpCanvas, atGRPX_FONT *lpFont, atINT X, atINT Y, atLPTSTR StrData, atINT nLen, atGRPX_COLOR Color )
{
	atGRPX_PIXEL *lpDes = atGRPX_GetFrameBufferPtrXY( lpCanvas, X, Y );
	int vmemwidth = atGRPX_GetVMemWidth( lpCanvas ) * atGRPX_GetBPP(lpCanvas) / 8;
	atRECT *clip = atGRPX_GetClipRect(lpCanvas);

	__atGRPX_DrawString0( lpFont, (atLPBYTE)lpDes, atGRPX_GetBPP(lpCanvas), vmemwidth, clip, X, Y, StrData, nLen, 0, (atLONG)Color, _draw_glyph );
}


/********************
   문자열을 찍는다.
*********************/
void atGRPX_DrawStringEx( atGRPX_CANVAS *lpCanvas, atGRPX_FONT *lpFont, atINT X, atINT Y, atLPTSTR StrData, atINT nLen, atGRPX_COLOR Color, atINT nLineGap )
{
	atGRPX_PIXEL *lpDes = atGRPX_GetFrameBufferPtrXY( lpCanvas, X, Y );
	int vmemwidth = atGRPX_GetVMemWidth( lpCanvas ) * atGRPX_GetBPP(lpCanvas) / 8;
	atRECT *clip = atGRPX_GetClipRect(lpCanvas);

	__atGRPX_DrawString0( lpFont, (atLPBYTE)lpDes, atGRPX_GetBPP(lpCanvas), vmemwidth, clip, X, Y, StrData, nLen, nLineGap, (atLONG)Color, _draw_glyph );
}

