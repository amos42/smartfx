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
#include "../Graphic/_AB_int.h"
#include "A_Math.h"


static void PutBPP1FontNormal_R( atGRPX_PIXEL *di, atU8 *si, int sx, int w, int h, atREAL ddx, atREAL ddy, int vmemwidth, atGRPX_COLOR Color )
{
	int i, j;
//	atGRPX_PIXEL cc;
	atBYTE cl;
    atBYTE r0, g0, b0;
//    atBYTE rr, gg, bb;
//    atINT weight2;
	atGRPX_PIXEL color = (atGRPX_PIXEL)Color;
	atGRPX_PIXEL *di0 = di;
	atREAL xx, yy, xx0, yy0;
	int vmemwidth0 = vmemwidth;

    r0 = GET_RED(color); g0 = GET_GREEN(color); b0 = GET_BLUE(color);

	xx0 = yy0 = 0;

	//	vmemwidth -= w;
    for( i = 0; i < h; i ++ ){
	  yy = yy0;
	  xx = xx0;
	  //si += sx;
	  for( j = sx; j < w; j ++ ){
        //cl = *si++;
		  cl = ((*si) >> (w - j + 1)) & 0x1;
		di = di0 + REALTOINT(xx) + REALTOINT(yy) * vmemwidth0;
        //if( cl == 0xff ){
		if( cl == 0x1 ){
          *di = color;
        } 
		/*
		else if( cl != 0x00 ){
          weight2 = 255 - cl;
          __ALPHA_PIXEL_I( di, r0,g0,b0, cc,rr,gg,bb, cl, weight2 );
        }
		*/
		xx += ddx;
		yy += ddy;

//	    di ++;
	  }
//      di += vmemwidth;

	  si ++;

	  yy0 += ddx;
	  xx0 += -ddy;
    }
}

static void PutBPP8FontNormal_R( atGRPX_PIXEL *di, atU8 *si, int sx, int w, int h, atREAL ddx, atREAL ddy, int vmemwidth, atGRPX_COLOR Color )
{
    int i, j;
	atGRPX_PIXEL cc;
	atBYTE cl;
    atBYTE r0, g0, b0;
    atBYTE rr, gg, bb;
    atINT weight2;
	atGRPX_PIXEL color = (atGRPX_PIXEL)Color;
	atGRPX_PIXEL *di0 = di;
	atREAL xx, yy, xx0, yy0;
	int vmemwidth0 = vmemwidth;

    r0 = GET_RED(color); g0 = GET_GREEN(color); b0 = GET_BLUE(color);

	xx0 = yy0 = 0;

//	vmemwidth -= w;
    for( i = 0; i < h; i ++ ){
	  yy = yy0;
	  xx = xx0;
	  si += sx;
	  for( j = 0; j < w; j ++ ){
        cl = *si++;
		di = di0 + REALTOINT(xx) + REALTOINT(yy) * vmemwidth0;
        if( cl == 0xff ){
          *di = color;
        } else if( cl != 0x00 ){
          weight2 = 255 - cl;
          __ALPHA_PIXEL_I( di, r0,g0,b0, cc,rr,gg,bb, cl, weight2 );
        }
		xx += ddx;
		yy += ddy;

//	    di ++;
	  }
//      di += vmemwidth;
	  yy0 += ddx;
	  xx0 += -ddy;
    }
}


static void PutBPP8FontNormal_2( atGRPX_PIXEL *di, atU8 *si, int sx, int w, int h, int wskip, int vmemwidth, atGRPX_PIXEL color )
{
    int width;
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
	while(h-- > 0){
ss = si + sx;		
	  width = w;
	  while(width-- > 0){
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
/*
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
*/	
	PutBPP8FontNormal_2( (atGRPX_PIXEL*)di, (atU8*)si, sx, w, h, wskip, vmemwidth, (atGRPX_PIXEL)ExParam1 );
}


/********************
   문자열을 찍는다.
*********************/
atVOID atGRPX_DrawStringRotate_1( atGRPX_CANVAS *lpCanvas, atGRPX_FONT *lpFont, atINT nX, atINT nY, atLPTSTR szStrData, atINT nLen, atGRPX_COLOR Color, atREAL rAngle )
{
    int i;
	int xx, yy;
	atTCHAR ch;
	atU8 *si;
	atGRPX_PIXEL *di, *tdi;
	int vmemwidth;
    int fontheight;
	atGRPX_GLYPHINFO glp;
	atBOOL r;
	atRECT *clip;
	int w, h, sx;
	atREAL cos_v, sin_v;
	atREAL xxx, yyy;
	int ascent;

if( lpCanvas == atNULL ) return;

//	lpFont = atGRPX_GetCurFont(lpCanvas);
	if( lpFont == atNULL ) return;
	
	atGRPX_GetFontSize( lpFont, &ascent, atNULL );
//ascent  = lpFont->nAscent;

	cos_v = COS( rAngle );
	sin_v = SIN( rAngle );
	xxx = INTTOREAL(nX);
	yyy = INTTOREAL(nY);

	xx = nX;
	yy = nY - ascent;
	di = atGRPX_GetFrameBufferPtrXY( lpCanvas, xx, yy );
	vmemwidth = atGRPX_GetVMemWidth( lpCanvas );
    fontheight = atGRPX_GetFontHeight( lpFont );

//	if( nLen < 0 ) nLen = atVD_SYSAPI_tstrlen( StrData );
	if( nLen < 0 ) nLen = 256;

	clip = atGRPX_GetClipRect(lpCanvas);
	if( (yy + fontheight) <= clip->nStartY ) return;
	
    for( i = 0; i < nLen; i++ ){
//	  if( (xx > clip->nEndX) ) break; /// 줄바꿈이 일어날 수도 있으니...
	  if( (yy > clip->nEndY) ) break;
	  if( (ch = *szStrData++) == _ATC('\0') ) break;

	  if( ch == _ATC('\n') ){
		di += vmemwidth * fontheight - (xx - nX);
		xx = nX;
		yy += fontheight;
	  } else {
		int xx2, yy2;
		r = atGRPX_GetGlyph( lpFont, atGRPX_GetCharIndex(lpFont, ch), &glp );
		if( !r ) continue;
		
		xx2 = xx + glp.nBearingX;
		yy2 = yy + ascent - glp.nBearingY;
//		tdi = di;
//		tdi += glp.nX;
//		tdi += -glp.nY * vmemwidth;

		if( r && (xx2 <= clip->nEndX) && ((xx2 + glp.nWidth) > clip->nStartX) ){
//			w = glp.rtMetric.nWidth;  
//			h = glp.rtMetric.nHeight;  
			w = glp.nWidth;  
			h = glp.nHeight;  

			si = glp.lpBitmap;
//			tdi = di;
            sx = 0; 
			if( xx2 < clip->nStartX ){
              sx = (clip->nStartX - xx2);
			  w -= sx;
//              tdi += sx;
            }
			if( (xx2+w) >= clip->nEndX ){
			  w = clip->nEndX - xx2 + 1;
            }
			if( yy2 < clip->nStartY ){
              si += (clip->nStartY - yy2) * glp.nFontWidthBytes;
			  h -= (clip->nStartY - yy2);
//		      tdi += vmemwidth * (clip->nStartY - yy2);
            }
			if( (yy2+h) >= clip->nEndY ){
			  h = clip->nEndY - yy2 + 1;
            }

			if( glp.nBPP == 1 ){
				tdi = atGRPX_GetFrameBufferPtrXY( lpCanvas, REALTOINT(xxx), REALTOINT(yyy) );
				//PutBPP1FontNormal( tdi, (atU8*)si, sx, w, h, cos_v, sin_v, lpCanvas->nVMemWidth, Color );
				PutBPP1FontNormal_R( tdi, (atU8*)si, sx, w, h, cos_v, sin_v, lpCanvas->nVMemWidth, Color );
			} else if( glp.nBPP == 8 ){
				tdi = atGRPX_GetFrameBufferPtrXY( lpCanvas, REALTOINT(xxx+cos_v*glp.nBearingX), REALTOINT(yyy+sin_v*glp.nBearingY) );
				PutBPP8FontNormal_R( tdi, (atU8*)si, sx, w, h, cos_v, sin_v, lpCanvas->nVMemWidth, Color );
			}

			atGRPX_ReleaseGlyph( lpFont, &glp );
		}

//		di += glp.rtMetric.nWidth;  
//		xx += glp.rtMetric.nWidth;
		xxx += glp.nHorizAdvance * cos_v;
		yyy += glp.nHorizAdvance * sin_v;
	  }  
	}
}


/********************
   문자열을 찍는다.
*********************/
atVOID atGRPX_DrawStringRotate_2( atGRPX_CANVAS *lpCanvas, atGRPX_FONT *lpFont, atINT nX, atINT nY, atLPTSTR szStrData, atINT nLen, atGRPX_COLOR Color, atREAL rAngle )
{
    int i;
	int xx, yy;
	atTCHAR ch;
	atU8 *si;
	atLPBYTE di, tdi;
	int widthbytes;
    int fontheight;
	atGRPX_GLYPHINFO glp;
	atBOOL r;
//	atRECT *clip;
	int w, h, sx, wskip;
	int ascent;
//	int descent;
	atRECT *clip;
	atREAL cos_v, sin_v;
atREAL rxx, ryy;	
atGRPX_FONT *lpOldFont;

if( lpCanvas == atNULL ) return;

	clip = atGRPX_GetClipRect(lpCanvas);
//	lpOldFont = atGRPX_GetCurFont(lpCanvas);
lpOldFont = lpFont;
	if( lpOldFont == atNULL ) return;
	ascent  = lpOldFont->nAscent;

	if( lpOldFont->dwAttr & atGRPX_FONTATTR_ROTATABLE_CHANGED ){
		lpFont = lpOldFont;
		atGRPX_SetFontAngle( lpFont, rAngle );
	} else {
		lpFont = atGRPX_CreateFont( lpCanvas->lpGrpX, lpOldFont->szFontName, lpOldFont->nFontSize, lpOldFont->dwStyle, rAngle );
		if( lpFont == atNULL ) return;
//		atGRPX_SetCurFont(lpCanvas, lpFont);
	}
	

	cos_v = COS( rAngle + INTTOREAL(90) );
	sin_v = SIN( rAngle + INTTOREAL(90) );
	rxx = INTTOREAL(nX) + ascent * cos_v;
	ryy = INTTOREAL(nY) + ascent * sin_v;
	cos_v = COS( rAngle );
	sin_v = SIN( rAngle );

	xx = nX;
	yy = nY;// + ascent;
    fontheight = atGRPX_GetFontHeight( lpFont );

	widthbytes = atGRPX_GetVMemWidth( lpCanvas ) * atGRPX_GetBPP(lpCanvas) / 8;
	di = (atLPBYTE)atGRPX_GetFrameBufferPtrXY( lpCanvas, xx, yy );

//	if( nLen < 0 ) nLen = atVD_SYSAPI_tstrlen( StrData );
	if( nLen < 0 ) nLen = 256;

//	if( (yy + descent) <= clip->nStartY ) return; /// 줄바꿈이 일어날 수도 있으니...
	
    for( i = 0; i < nLen; i++ ){
//	  if( (xx > clip->nEndX) ) break; /// 줄바꿈이 일어날 수도 있으니... 
//	  if( (yy > clip->nEndY) ) break;
	  if( (ch = *szStrData++) == _ATC('\0') ) break;

	  if( ch == _ATC('\n') ){
		di += widthbytes * fontheight - (xx - nX) * atGRPX_GetBPP(lpCanvas) / 8;
		xx = nX;
		yy += REALTOINT(fontheight * sin_v);
		if( (yy > clip->nEndY) ) break;
	  } else {
		int xx2, yy2;
		r = atGRPX_GetGlyph( lpFont, atGRPX_GetCharIndex(lpFont, ch), &glp );
		if( !r ) continue;

		if( glp.lpBitmap != atNULL ){
			xx2 = xx + glp.nBearingX;
			yy2 = yy + ascent - glp.nBearingY;
//xx2 = REALTOINT(rxx + glp.nBearingX * cos_v);			
//yy2 = REALTOINT(ryy + (ascent - glp.nBearingY) * sin_v);			
xx2 = REALTOINT(rxx) + glp.nBearingX;			
yy2 = REALTOINT(ryy) - glp.nBearingY;			
			/*
			tdi = di;
			tdi += glp.nBearingX * atGRPX_GetBPP(lpCanvas) / 8;
			tdi += (ascent - glp.nBearingY) * widthbytes;
			*/
			tdi = (atLPBYTE)atGRPX_GetFrameBufferPtrXY(lpCanvas, xx2, yy2);

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
				  tdi += sx * atGRPX_GetBPP(lpCanvas) / 8;
				}
				if( (xx2+w-1) > clip->nEndX ){
				  wskip += (xx2+w-1) - clip->nEndX;
				  w -= (xx2+w-1) - clip->nEndX;
				}
				if( yy2 < clip->nStartY ){
				  si += (clip->nStartY - yy2) * glp.nFontWidthBytes;
				  h -= (clip->nStartY - yy2);
				  tdi += widthbytes * (clip->nStartY - yy2);
				}
				if( (yy2+h-1) > clip->nEndY ){
				  h -= (yy2+h-1) - clip->nEndY;
				}

				_draw_glyph( (atU8 *)tdi, &glp, (atU8*)si, sx, w, h, /*wskip*/glp.nFontWidthBytes, widthbytes, Color );
			}
		  }  

		  di += glp.nHorizAdvance * atGRPX_GetBPP(lpCanvas) / 8;  
		  xx += glp.nHorizAdvance;
//rxx += glp.nHorizAdvance * cos_v;
//ryy += glp.nHorizAdvance * sin_v;
rxx += glp.rAdvX;
ryy += glp.rAdvY;

		  atGRPX_ReleaseGlyph( lpFont, &glp );
	      }
	}
	
	if( lpOldFont->dwAttr & atGRPX_FONTATTR_ROTATABLE_CHANGED ){
		atGRPX_SetFontAngle( lpFont, INTTOREAL(0) );
	} else {
//		atGRPX_SetCurFont(lpCanvas, lpOldFont);
		atGRPX_DestroyFont(lpFont); // 삭제할 필요 없음.
	}
}

atVOID atGRPX_DrawStringRotate( atGRPX_CANVAS *lpCanvas, atGRPX_FONT *lpFont, atINT nX, atINT nY, atLPTSTR szStrData, atINT nLen, atGRPX_COLOR Color, atREAL rAngle, atINT nBaseX, atINT nBaseY )
{
	if( lpCanvas == atNULL ) return;
	if(lpFont == atNULL) return;

	if( (rAngle == INTTOREAL(0)) || (rAngle == INTTOREAL(360)) ){
		atGRPX_DrawString( lpCanvas, lpFont, nX - nBaseX, nY- nBaseY, szStrData, -1, Color );
		return;
	}
	
	if( (nBaseX != 0) || (nBaseY != 0) ){
		nX -= REALTOINT( COS(rAngle) * (nBaseX) - SIN(rAngle) * (nBaseY) );
		nY -= REALTOINT( SIN(rAngle) * (nBaseX) + COS(rAngle) * (nBaseY) );
	}

	if( lpFont->dwAttr & atGRPX_FONTATTR_ROTATABLE ){
		atGRPX_DrawStringRotate_2( lpCanvas, lpFont, nX, nY, szStrData, nLen, Color, rAngle );
	} else {
		atGRPX_DrawStringRotate_1( lpCanvas, lpFont, nX, nY, szStrData, nLen, Color, rAngle );
	}
}


