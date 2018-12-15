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


/********************
   문자열을 찍는다.
*********************/
void __atGRPX_DrawString0( atGRPX_FONT *lpFont, atLPBYTE lpDes, int nBPP, int widthbytes, atRECT *clip, atINT nX, atINT nY, atLPTSTR szStrData, atINT nLen, atINT nLineGap,
						   		atLONG ExParam1,
						   		__STRCALLBACK__DRAW_GLYPH *fnDrawGlyph )
{
    int i;
	atREAL xx, yy;
	atTCHAR ch;
	atU8 *si;
	atLPBYTE di, tdi;
//	int vmemwidth;
    int fontheight, lineheight;
	atGRPX_GLYPHINFO glp;
	atBOOL r;
//	atRECT *clip;
	int w, h, sx, wskip;
	int ascent;
//	int descent;

	if( lpFont == atNULL ) return;

	atGRPX_GetFontSize( lpFont, &ascent, atNULL );
//	ascent = lpFont->nAscent;
//	descent = lpFont->nDescent;

    if( (lpFont == atNULL) || (lpDes == atNULL) ) return;
        
	xx = INTTOREAL(nX);
	yy = INTTOREAL(nY + ascent);
    fontheight = atGRPX_GetFontHeight( lpFont );
	lineheight = fontheight + nLineGap;
	
	di = lpDes;

//	if( nLen < 0 ) nLen = atVD_SYSAPI_tstrlen( StrData );
	if( nLen < 0 ) nLen = 256;

//	if( (yy + descent) <= clip->nStartY ) return; /// 줄바꿈이 일어날 수도 있으니...
	
    for( i = 0; i < nLen; i++ ){
//	  if( (xx > clip->nEndX) ) break; /// 줄바꿈이 일어날 수도 있으니...
//	  if( (yy > clip->nEndY) ) break;
	  if( (ch = *szStrData++) == _ATC('\0') ) break;

	  if( ch == _ATC('\n') ){
		di += widthbytes * lineheight - (REALTOINT(xx) - nX) * nBPP/8;
		xx = INTTOREAL(nX);
		yy += INTTOREAL(lineheight);
		if( REALTOINT(yy) > clip->nEndY ) break;
	  } else {
		int xx2, yy2;
		r = atGRPX_GetGlyph( lpFont, atGRPX_GetCharIndex(lpFont, ch), &glp );
		if( !r ) continue;

		if( glp.lpBitmap != atNULL ){
//			xx2 = xx + glp.nBearingX;
//			yy2 = yy + ascent - glp.nBearingY;
xx2 = REALTOINT(xx) + glp.nBearingX;			
yy2 = REALTOINT(yy) - glp.nBearingY;			
			tdi = di;
			tdi += glp.nBearingX * nBPP/8;
			tdi += (ascent - glp.nBearingY) * widthbytes;

			if( (yy2 <= clip->nEndY) && (xx2 <= clip->nEndX) && ((xx2 + glp.nHorizAdvance) > clip->nStartX) ){
				w = glp.nWidth;  
				h = glp.nHeight;  

				si = glp.lpBitmap;
	//			tdi = di;
				sx = 0;
				wskip = 0; 
				if( xx2 < clip->nStartX ){
				  sx = (clip->nStartX - xx2);
				  wskip += sx;
				  w -= sx;
				  tdi += sx * nBPP/8;
				}
				if( (xx2+w-1) > clip->nEndX ){
				  wskip += (xx2+w-1) - clip->nEndX;
				  w -= (xx2+w-1) - clip->nEndX;
				}
				if( yy2 < clip->nStartY ){
				  si += (clip->nStartY - yy2) * glp.nFontWidthBytes;
				  h -= (clip->nStartY - yy2);
				  tdi += widthbytes * (clip->nStartY - yy2);
                  yy2 = clip->nStartY;
				}
				if( (yy2+h-1) > clip->nEndY ){
				  h -= (yy2+h-1) - clip->nEndY;
				}

				fnDrawGlyph( (atU8 *)tdi, &glp, (atU8*)si, sx, w, h, /*wskip*/glp.nFontWidthBytes, widthbytes, ExParam1 );
			}
		  }  

		  di += glp.nHorizAdvance * nBPP/8;  
//		  xx += glp.nHorizAdvance;
xx += (glp.rAdvX);
yy += (glp.rAdvY);

		  atGRPX_ReleaseGlyph( lpFont, &glp );
	      }
	}
}

