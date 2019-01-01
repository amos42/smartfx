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

#include "TinyGrpX.h"
#include "_STR_func.h"


static void PutW8FontNormal( atTGRPX_PIXEL *di, atU8 *si, int sx, int w, int h, int wskip, int vmemwidth, atTGRPX_PIXEL color )
{
    int i, j;
    int d;
	
	vmemwidth -= w*sizeof(atTGRPX_PIXEL);
	vmemwidth /= sizeof(atTGRPX_PIXEL);
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

static void PutW16FontNormal( atTGRPX_PIXEL *di, atU16 *si, int sx, int w, int h, int wskip, int vmemwidth, atTGRPX_PIXEL color )
{
    int i, j;
    int d;

	vmemwidth -= w*sizeof(atTGRPX_PIXEL);
	vmemwidth /= sizeof(atTGRPX_PIXEL);
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

static void PutW32FontNormal( atTGRPX_PIXEL *di, atU32 *si, int sx, int w, int h, int wskip, int vmemwidth, atTGRPX_PIXEL color )
{
    int i, j, k;
    unsigned int d;
	atU8 *ssi, *ssi0;
	int cnt, ww;

	ssi = ssi0 = (atU8 *)si;
	vmemwidth -= w*sizeof(atTGRPX_PIXEL);
	vmemwidth /= sizeof(atTGRPX_PIXEL);
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

static void _draw_glyph( atU8 *di, atTGRPX_GLYPHINFO *glp, atU8 *si, int sx, int w, int h, int wskip, int vmemwidth, atLONG ExParam1 )
{
	if( glp->nBPP == 1 ){
		if( glp->nFontWidthBytes == 1 )
			PutW8FontNormal( (atTGRPX_PIXEL*)di, (atU8*)si, sx, w, h, wskip, vmemwidth, (atTGRPX_PIXEL)ExParam1 );
		else if( glp->nFontWidthBytes == 2 )
			PutW16FontNormal( (atTGRPX_PIXEL*)di, (atU16*)si, sx, w, h, wskip, vmemwidth, (atTGRPX_PIXEL)ExParam1 );
		else if( glp->nFontWidthBytes == 4 )
			PutW32FontNormal( (atTGRPX_PIXEL*)di, (atU32*)si, sx, w, h, wskip, vmemwidth, (atTGRPX_PIXEL)ExParam1 );
	}
	
}


/********************
   문자열을 찍는다.
*********************/
void atTGRPX_DrawString( atTGRPX_CANVAS *lpCanvas, atTGRPX_FONT *lpFont, atINT X, atINT Y, atLPTSTR StrData, atINT nLen, atTGRPX_COLOR Color )
{
	atTGRPX_PIXEL *lpDes = atTGRPX_GetFrameBufferPtrXY( lpCanvas, X, Y );
	int vmemwidth = atTGRPX_GetVMemWidth( lpCanvas ) * atTGRPX_GetBPP(lpCanvas) / 8;
	atRECT *clip = atTGRPX_GetClipRect(lpCanvas);

	__atTGRPX_DrawString0( lpFont, (atLPBYTE)lpDes, atTGRPX_GetBPP(lpCanvas), vmemwidth, clip, X, Y, StrData, nLen, 0, (atLONG)Color, _draw_glyph );
}


/********************
   문자열을 찍는다.
*********************/
void atTGRPX_DrawStringEx( atTGRPX_CANVAS *lpCanvas, atTGRPX_FONT *lpFont, atINT X, atINT Y, atLPTSTR StrData, atINT nLen, atTGRPX_COLOR Color, atINT nLineGap )
{
	atTGRPX_PIXEL *lpDes = atTGRPX_GetFrameBufferPtrXY( lpCanvas, X, Y );
	int vmemwidth = atTGRPX_GetVMemWidth( lpCanvas ) * atTGRPX_GetBPP(lpCanvas) / 8;
	atRECT *clip = atTGRPX_GetClipRect(lpCanvas);

	__atTGRPX_DrawString0( lpFont, (atLPBYTE)lpDes, atTGRPX_GetBPP(lpCanvas), vmemwidth, clip, X, Y, StrData, nLen, nLineGap, (atLONG)Color, _draw_glyph );
}

