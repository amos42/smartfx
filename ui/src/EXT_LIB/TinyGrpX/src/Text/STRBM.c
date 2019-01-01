/*#############################################################################
-------------------------------------------------------------------------------

                                  STRBM.c
                           (* =================== *)


 Version           : 1.0.0
 Dependent Modules :
 Author            : Ju, Gyeong-min ( Amos )
 Create            : 2001/04/25    01:47:09am
 Update            : 2003/04/29    00:00:00am
 C Compiler        : ADS 1.01, SDT 2.51, Borland C/C++ 5.0, Visual C/C++ 6.0
 Assembler         : ADS 1.01, SDT 2.51, Borland TASM 3.1, MASM 6.0
 Description       : String Bitmap API

-------------------------------------------------------------------------------
#############################################################################*/

#include "GrpX.h"
#include "_STR_func.h"
#include <string.h>



static void PutW8FontNormal( atBYTE *di, atU8 *si, int sx, int w, int h, int wskip, int vmemwidth )
{
    int i, j;
    int d;

	vmemwidth -= w;
    for( i = 0; i < h; i ++ ){
      d = *si++;
	  if( sx > 0 ) d <<= sx;
      for( j = 0; j < w; j ++ ){
        if( d & 0x80 ) *di = 0xFF;
        d <<= 1;
        di ++;
      }
      di += vmemwidth;
    }
}

static void PutW16FontNormal( atBYTE *di, atU16 *si, int sx, int w, int h, int wskip, int vmemwidth )
{
    int i, j;
    int d;

	vmemwidth -= w;
    for( i = 0; i < h; i ++ ){
      d = *si++;
	  if( sx > 0 ) d <<= sx;
      for( j = 0; j < w; j ++ ){
        if( d & 0x8000 ) *di = 0xFF;
        d <<= 1;
        di ++;
      }
      di += vmemwidth;
    }
}

static void PutW32FontNormal( atBYTE *di, atU8 *si, int sx, int w, int h, int wskip, int vmemwidth )
{
    int i, j, k;
    unsigned int d;
	atU8 *ssi, *ssi0;
	int cnt, ww;

	ssi = ssi0 = (atU8 *)si;
	vmemwidth -= w;
    for( i = 0; i < h; i ++ ){
	  ssi = ssi0;
	  if( sx > 0 ) d <<= sx;
      ww = w;
	  for( j = 0; j < 4; j ++ ){
        cnt = (ww > 8)? 8 : ww;
	    ww -= cnt;

	    d = *ssi++;
        for( k = 0; k < cnt; k ++ ){
          if( d & 0x80 ) *di = 0xFF;
          d <<= 1;
          di ++;
        }

        if( ww <= 0 ) break;
      }
	  ssi0 += 4;
      di += vmemwidth;
    }
}

static void PutBPP8FontNormal( atBYTE *di, atU8 *si, int sx, int w, int h, int wskip, int vmemwidth )
{
    int i, j;
atLPBYTE ss;	

	vmemwidth -= w;
    for( i = 0; i < h; i ++ ){
ss = si + sx;
	  for( j = 0; j < w; j ++ ){
          *di++ = *ss++;
	  }
	  si += wskip;
      di += vmemwidth;
    }
}


static void _draw_glyph( atBYTE *di, atGRPX_GLYPHINFO *glp, atU8 *si, int sx, int w, int h, int wskip, int vmemwidth, atLONG ExParam1 )
{
	if( glp->nBPP == 1 ){
		if( glp->nFontWidthBytes == 1 )
			PutW8FontNormal( di, (atU8*)si, sx, w, h, wskip, vmemwidth );
		else if( glp->nFontWidthBytes == 2 )
			PutW16FontNormal( di, (atU16*)si, sx, w, h, wskip, vmemwidth );
		else if( glp->nFontWidthBytes == 4 )
			PutW32FontNormal( di, (atU8*)si, sx, w, h, wskip, vmemwidth );
	} else if( glp->nBPP == 8 ){
		PutBPP8FontNormal( di, (atU8*)si, sx, w, h, wskip, vmemwidth );
	}
	
}



/******************************
   문자열을 비트맵을 생성한다.
*******************************/
atGRPX_BITMAP * atGRPX_CreateStrBitmap( atLPGRPXMNG lpGrpX, atGRPX_FONT *lpFont, atLPTSTR szStrData, atINT nLen )
{
    int fontwidth, fontheight;
    atGRPX_BITMAP *lpStrBitmap;
	int ascent;
	atRECT clip;

	if( lpFont == atNULL ) return atNULL;

//	if( nLen < 0 ) nLen = atVD_SYSAPI_tstrlen( szStrData );
	if( nLen < 0 ) nLen = 255;

	ascent = lpFont->nAscent;

	fontwidth = atGRPX_GetStringWidth( lpFont, szStrData, -1 );
    fontheight = atGRPX_GetFontHeight( lpFont );

	if( (fontwidth <= 0) || (fontheight <= 0) ){
		return atNULL;
	}

//	lpStrBitmap = atGRPX_CreateBlankBitmap( lpGrpX, fontwidth, fontheight, lpFont->nFontBPP, 0 );
	lpStrBitmap = atGRPX_CreateBlankBitmap( lpGrpX, fontwidth, fontheight, 8, 0 ); // 무조건 8 BPP
	if( lpStrBitmap == atNULL ) return atNULL;
	memset( lpStrBitmap->lpBits, 0, lpStrBitmap->nWidthBytes * lpStrBitmap->nHeight );

	clip.nStartX = 0;
	clip.nStartY = 0;
	clip.nEndX = fontwidth - 1;
	clip.nEndY = fontheight - 1;
	__atGRPX_DrawString0( lpFont, (atLPBYTE)lpStrBitmap->lpBits, 8, lpStrBitmap->nWidthBytes, &clip, 0, 0, szStrData, -1, 0, (atLONG)atNULL, _draw_glyph );

	return lpStrBitmap;
}

