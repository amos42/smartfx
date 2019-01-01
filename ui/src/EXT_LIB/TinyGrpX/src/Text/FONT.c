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

#define __GRPX_IMPLEMENT_
#include "FontTDmp_E.h"
#include "FontSDmp_E.h"
#include "FontMDmp_E.h"
#include "FontEDmp_E.h"
#include <string.h>


/********************

*********************/
atBOOL atTGRPX_InitFontMng( atLPGRPXMNG lpGrpX )
{
	lpGrpX->lpDefFont = (atLPGRPX_FONT)atTGRPX_CreateFont( lpGrpX, atTGRPX_DEF_FONT_NAME, atTGRPX_DEF_FONT_SIZE, 0, INTTOREAL(0) );
	if( lpGrpX->lpDefFont )
		((atLPGRPX_FONT)(lpGrpX->lpDefFont))->nRefCount = 0xEFFF;

	return atTRUE;
}


/********************

*********************/
atBOOL atTGRPX_CloseFontMng( atLPGRPXMNG lpGrpX )
{
	if( lpGrpX->lpDefFont ){
		atTGRPX_DestroyFont( (atLPGRPX_FONT)(lpGrpX->lpDefFont) );
		lpGrpX->lpDefFont = atNULL;
	}
	
	return atTRUE;
}


/********************

*********************/
atTGRPX_FONT * atTGRPX_CreateFont( atLPGRPXMNG lpGrpX, atLPTSTR szFontName, atINT nSize, atDWORD dwStyle, atREAL rAngle )
{
	atTGRPX_FONT *lpFont;

	// 내장 폰트 유효성 검사
	lpFont = atTGRPX_AllocMem( lpGrpX, sizeof(atTGRPX_FONT) );
	if( lpFont == atNULL ) return atNULL;
	
	lpFont->lpGrpX  = lpGrpX;
	lpFont->nFontSize = nSize;
	lpFont->dwStyle = dwStyle;
	memcpy( lpFont->szFontName, szFontName, sizeof(lpFont->szFontName) );
    lpFont->szFontName[sizeof(lpFont->szFontName)/sizeof(atTCHAR)-1] = _ATC('\0');
	lpFont->nRefCount = 0;

	lpFont->nFontBPP = 1;
	lpFont->dwAttr = lpGrpX->dwFontAttr;
	lpFont->nAscent = nSize;
	lpFont->nDescent = 0;
	lpFont->rAngle = INTTOREAL(0);

    return lpFont;
}


/********************

*********************/
void  atTGRPX_DestroyFont( atTGRPX_FONT *lpFont )
{
	if( lpFont == atNULL ) return; 

	atTGRPX_FreeMem( lpFont->lpGrpX, lpFont );
}


/********************

*********************/
atTGRPX_FONT * atTGRPX_ReferenceFont( atTGRPX_FONT *lpFont )
{
	if( lpFont == atNULL ) return atNULL;

	if( lpFont->nRefCount == 0xEFFF ) return lpFont;
	lpFont->nRefCount++;

    return lpFont;
}


/********************

*********************/
atTGRPX_FONT * atTGRPX_ReleaseFont( atTGRPX_FONT *lpFont, atBOOL bAutoDestory )
{
	if( lpFont == atNULL ) return atNULL;

	if( lpFont->nRefCount == 0xEFFF ) return lpFont;

	if( --lpFont->nRefCount <= 0 ){
		if( bAutoDestory ) atTGRPX_DestroyFont( lpFont );
		return atNULL;
	} else {
		return lpFont;
	}
}

/********************

*********************/
void  atTGRPX_SetFontStyle( atTGRPX_FONT *lpFont, atINT Style )
{
	if( lpFont == atNULL ) return;

	lpFont->dwStyle = (atDWORD)Style;
}


/********************

*********************/
atBOOL atTGRPX_SetFontAngle( atTGRPX_FONT *lpFont, atREAL rAngle )
{
	if( lpFont == atNULL ) return atFALSE;

	if( !(lpFont->lpGrpX->dwFontAttr & atTGRPX_FONTATTR_ROTATABLE) ) return atFALSE;

	if( lpFont->rAngle == rAngle ) return atTRUE;
	
	lpFont->rAngle = rAngle;

	return atTRUE;
}


/********************

*********************/
atINT  atTGRPX_GetStringWidth( atTGRPX_FONT *lpFont, atLPTSTR szStr, atINT nLen )
{
    int i;
    atTCHAR ch;
    atSIZE size;
	int width = 0;

	if( lpFont == atNULL ) return 0; 

	if( nLen < 0 ) nLen = 256; // Maimum
	
	for( i = 0; i < nLen; i++ ){
		if( (ch = *szStr++) == _ATC('\0') ) break;
		if( atTGRPX_GetGlyphMetric( lpFont, atTGRPX_GetCharIndex(lpFont, ch), &size ) )
			width += size.nWidth;
	}

	return width;
}


/********************

*********************/
atVOID  atTGRPX_GetFontSize( atTGRPX_FONT *lpFont, atINT *lpAscent, atINT *lpDescent )
{
	if( lpFont == atNULL ) return; 

	if( lpAscent ) *lpAscent = lpFont->nAscent;
	if( lpDescent ) *lpDescent = lpFont->nDescent;
}


/********************

*********************/
atINT  atTGRPX_GetFontHeight( atTGRPX_FONT *lpFont )
{
	if( lpFont == atNULL ) return 0; 

    return lpFont->nAscent + lpFont->nDescent;
}


/********************

*********************/
atUINT atTGRPX_GetCharIndex( atTGRPX_FONT *lpFont, atTCHAR Chr )
{
    unsigned int idx;
    
	if( lpFont == atNULL ) return -1;

	idx = ( (Chr >= _ATC(' ')) && (Chr <= _ATC('~')) ) ? Chr - _ATC(' ') + 1 : 0;

    return idx;
}


/********************

*********************/
atBOOL  atTGRPX_GetGlyphMetric( atTGRPX_FONT *lpFont, atUINT index, atSIZE *lpSize )
{
	if( lpSize == atNULL ) return atFALSE;
	if( lpFont == atNULL ){
		lpSize->nWidth = 	lpSize->nHeight = 0;
		return atFALSE;
	}

	if( lpFont->nFontSize == TINY_FONT_HEIGHT ){
		lpSize->nWidth = TINY_FONT_WIDTH;
		lpSize->nHeight = TINY_FONT_HEIGHT;
	} else if( lpFont->nFontSize == SMALL_FONT_HEIGHT ){
		lpSize->nWidth = SMALL_FONT_WIDTH;
		lpSize->nHeight = SMALL_FONT_HEIGHT;
	} else if( lpFont->nFontSize == MIDDLE_FONT_HEIGHT ){
		lpSize->nWidth = MIDDLE_FONT_WIDTH;
		lpSize->nHeight = MIDDLE_FONT_HEIGHT;
	} else if( lpFont->nFontSize == EXTEND_FONT_HEIGHT ){
		lpSize->nWidth = EXTEND_FONT_WIDTH;
		lpSize->nHeight = EXTEND_FONT_HEIGHT;
	} else {
		lpSize->nWidth = 	lpSize->nHeight = 0;
		return atFALSE;
	}

	return atTRUE;
}


/********************

*********************/
atBOOL  atTGRPX_GetGlyph( atTGRPX_FONT *lpFont, atUINT index, atTGRPX_GLYPHINFO *lpGlyph )
{
	atSIZE size;

	if( lpGlyph == atNULL ) return atFALSE;
	if( lpFont == atNULL ) return atFALSE;

	if( !atTGRPX_GetGlyphMetric( lpFont, index, &size ) ) return atFALSE;

	lpGlyph->nHorizAdvance = size.nWidth;
	lpGlyph->dwAttr = lpFont->dwAttr;
	lpGlyph->nBPP = lpFont->nFontBPP;
	lpGlyph->nFontWidthBytes = size.nWidth * lpGlyph->nBPP / 8 + (size.nWidth * lpGlyph->nBPP % 8)?1:0;
	lpGlyph->nBearingX = 0;
	lpGlyph->nBearingY = lpFont->nFontSize;
	lpGlyph->nWidth = (atSHORT)size.nWidth;
	lpGlyph->nHeight = (atSHORT)size.nHeight;
	lpGlyph->rAdvX = INTTOREAL(size.nWidth);
	lpGlyph->rAdvY = INTTOREAL(0);
	if( lpFont->nFontSize == TINY_FONT_HEIGHT )
	    	lpGlyph->lpBitmap = (void *)&DefaultTinyEngFont[index];
	else if( lpFont->nFontSize == SMALL_FONT_HEIGHT )
	    	lpGlyph->lpBitmap = (void *)&DefaultSmallEngFont[index];
	else if( lpFont->nFontSize == MIDDLE_FONT_HEIGHT )
	    	lpGlyph->lpBitmap = (void *)&DefaultMiddleEngFont[index];
	else if( lpFont->nFontSize == EXTEND_FONT_HEIGHT )
	    	lpGlyph->lpBitmap = (void *)&DefaultExtendEngFont[index];
	else
	    	lpGlyph->lpBitmap = (void *)atNULL;

    return atTRUE;
}


/********************

*********************/
void atTGRPX_ReleaseGlyph( atTGRPX_FONT *lpFont, atTGRPX_GLYPHINFO *lpGlyph )
{
	if( lpFont == atNULL ) return;

    lpGlyph->lpBitmap = atNULL;
}
