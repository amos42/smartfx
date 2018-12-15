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

#define __GRPX_IMPLEMENT_
#include "FontTDmp_E.h"
#include "FontSDmp_E.h"
#include "FontMDmp_E.h"
#include "FontEDmp_E.h"
#include <string.h>


/********************

*********************/
atBOOL atGRPX_InitFontMng( atLPGRPXMNG lpGrpX )
{
	if( lpGrpX->lstFontFuncs.lpData ){
		lpGrpX->lpOEMFontMng = lpGrpX->lstFontFuncs.fnInitFontMng( (atLPVOID)lpGrpX, lpGrpX->lstFontFuncs.lpData, lpGrpX->lstFontFuncs.nDataType );
		lpGrpX->dwFontAttr = lpGrpX->lstFontFuncs.fnGetFontMngCaps( lpGrpX->lpOEMFontMng );
	} else {
		lpGrpX->lpOEMFontMng = atNULL;
		lpGrpX->dwFontAttr = atGRPX_FONTATTR_NONE;
	}

	lpGrpX->lpDefFont = (atLPGRPX_FONT)atGRPX_CreateFont( lpGrpX, atGRPX_DEF_FONT_NAME, atGRPX_DEF_FONT_SIZE, 0, INTTOREAL(0) );
	if( lpGrpX->lpDefFont )
		((atLPGRPX_FONT)(lpGrpX->lpDefFont))->nRefCount = 0xEFFF;

	return atTRUE;
}


/********************

*********************/
atBOOL atGRPX_CloseFontMng( atLPGRPXMNG lpGrpX )
{
	if( lpGrpX->lpDefFont ){
		atGRPX_DestroyFont( (atLPGRPX_FONT)(lpGrpX->lpDefFont) );
		lpGrpX->lpDefFont = atNULL;
	}
	
	if( lpGrpX->lpOEMFontMng ){
		lpGrpX->lstFontFuncs.fnCloseFontMng( lpGrpX->lpOEMFontMng );
		lpGrpX->lpOEMFontMng = atNULL;
	} else {
		// NOP...
	}

	return atTRUE;
}


/********************

*********************/
atGRPX_FONT * atGRPX_CreateFont( atLPGRPXMNG lpGrpX, atLPTSTR szFontName, atINT nSize, atDWORD dwStyle, atREAL rAngle )
{
	atGRPX_FONT *lpFont;

	// 내장 폰트 유효성 검사
	if( !lpGrpX->lpOEMFontMng ){
		if( (nSize != TINY_FONT_HEIGHT) && (nSize != SMALL_FONT_HEIGHT) &&
		    (nSize != MIDDLE_FONT_HEIGHT) && (nSize != EXTEND_FONT_HEIGHT)
		   ) return atNULL;
	}

	lpFont = atGRPX_AllocMem( lpGrpX, sizeof(atGRPX_FONT) );
	if( lpFont == atNULL ) return atNULL;
	
	lpFont->lpGrpX  = lpGrpX;
	lpFont->nFontSize = nSize;
	lpFont->dwStyle = dwStyle;
	memcpy( lpFont->szFontName, szFontName, sizeof(lpFont->szFontName) );
    lpFont->szFontName[sizeof(lpFont->szFontName)/sizeof(atTCHAR)-1] = _ATC('\0');
	lpFont->nRefCount = 0;

	if( lpGrpX->lpOEMFontMng ){
		lpFont->OEMFont = lpGrpX->lstFontFuncs.fnCreateFont( lpGrpX->lpOEMFontMng, szFontName, nSize, dwStyle, rAngle, &lpFont->nFontBPP );
		if(!lpFont->OEMFont)
		{
			atGRPX_FreeMem(lpGrpX,lpFont);
			return atNULL;
		}
		lpFont->dwAttr = lpGrpX->dwFontAttr;
		if(  lpFont->OEMFont )
			lpGrpX->lstFontFuncs.fnGetFontMetric( lpFont->OEMFont, &lpFont->nAscent, &lpFont->nDescent );		
		lpFont->rAngle = rAngle;
	} else {
		lpFont->OEMFont = atNULL;
		lpFont->nFontBPP = 1;
		lpFont->dwAttr = lpGrpX->dwFontAttr;
		lpFont->nAscent = nSize;
		lpFont->nDescent = 0;
		lpFont->rAngle = INTTOREAL(0);
	}

    return lpFont;
}


/********************

*********************/
void  atGRPX_DestroyFont( atGRPX_FONT *lpFont )
{
	if( lpFont == atNULL ) return; 

	if( lpFont->OEMFont ){
		lpFont->lpGrpX->lstFontFuncs.fnDestroyFont( lpFont->OEMFont );
	}
	
	atGRPX_FreeMem( lpFont->lpGrpX, lpFont );
}


/********************

*********************/
atGRPX_FONT * atGRPX_ReferenceFont( atGRPX_FONT *lpFont )
{
	if( lpFont == atNULL ) return atNULL;

	if( lpFont->nRefCount == 0xEFFF ) return lpFont;
	lpFont->nRefCount++;

    return lpFont;
}


/********************

*********************/
atGRPX_FONT * atGRPX_ReleaseFont( atGRPX_FONT *lpFont, atBOOL bAutoDestory )
{
	if( lpFont == atNULL ) return atNULL;

	if( lpFont->nRefCount == 0xEFFF ) return lpFont;

	if( --lpFont->nRefCount <= 0 ){
		if( bAutoDestory ) atGRPX_DestroyFont( lpFont );
		return atNULL;
	} else {
		return lpFont;
	}
}

/********************

*********************/
void  atGRPX_SetFontStyle( atGRPX_FONT *lpFont, atINT Style )
{
	if( lpFont == atNULL ) return;

	if( lpFont->OEMFont ){
		lpFont->lpGrpX->lstFontFuncs.fnSetFontStyle( lpFont->OEMFont, Style );
	}

	lpFont->dwStyle = (atDWORD)Style;
}


/********************

*********************/
atBOOL atGRPX_SetFontAngle( atGRPX_FONT *lpFont, atREAL rAngle )
{
	if( lpFont == atNULL ) return atFALSE;

	if( !(lpFont->lpGrpX->dwFontAttr & atGRPX_FONTATTR_ROTATABLE) ) return atFALSE;

	if( lpFont->rAngle == rAngle ) return atTRUE;
	
	if( lpFont->OEMFont ){
		if( lpFont->lpGrpX->dwFontAttr & atGRPX_FONTATTR_ROTATABLE_CHANGED ){
			lpFont->lpGrpX->lstFontFuncs.fnSetFontAngle( lpFont->OEMFont, rAngle );
		} else {
			lpFont->lpGrpX->lstFontFuncs.fnDestroyFont( lpFont->OEMFont );
			lpFont->OEMFont = lpFont->lpGrpX->lstFontFuncs.fnCreateFont( lpFont->lpGrpX->lpOEMFontMng, lpFont->szFontName, lpFont->nFontSize, lpFont->dwStyle, rAngle, atNULL );
		}
	}

	lpFont->rAngle = rAngle;

	return atTRUE;
}


/********************

*********************/
atINT  atGRPX_GetStringWidth( atGRPX_FONT *lpFont, atLPTSTR szStr, atINT nLen )
{
    int i;
    atTCHAR ch;
    atSIZE size;
	int width = 0;

	if( lpFont == atNULL ) return 0; 

	if( nLen < 0 ) nLen = 256; // Maimum
	
	for( i = 0; i < nLen; i++ ){
		if( (ch = *szStr++) == _ATC('\0') ) break;
		if( atGRPX_GetGlyphMetric( lpFont, atGRPX_GetCharIndex(lpFont, ch), &size ) )
			width += size.nWidth;
	}

	return width;
}


/********************

*********************/
atVOID  atGRPX_GetFontSize( atGRPX_FONT *lpFont, atINT *lpAscent, atINT *lpDescent )
{
	if( lpFont == atNULL ) return; 

	if(  lpFont->OEMFont )
		lpFont->lpGrpX->lstFontFuncs.fnGetFontMetric( lpFont->OEMFont, &lpFont->nAscent, &lpFont->nDescent );

	if( lpAscent ) *lpAscent = lpFont->nAscent;
	if( lpDescent ) *lpDescent = lpFont->nDescent;
}


/********************

*********************/
atINT  atGRPX_GetFontHeight( atGRPX_FONT *lpFont )
{
	if( lpFont == atNULL ) return 0; 

    return lpFont->nAscent + lpFont->nDescent;
}


/********************

*********************/
atGRPX_OEM_FONT atGRPX_GetCurOemFont( atGRPX_FONT *lpFont )
{
	if( lpFont == atNULL ) return atNULL;

	return lpFont->OEMFont;
}


/********************

*********************/
atUINT atGRPX_GetCharIndex( atGRPX_FONT *lpFont, atTCHAR Chr )
{
    unsigned int idx;
    
	if( lpFont == atNULL ) return -1;

	if( lpFont->OEMFont ){
		idx = lpFont->lpGrpX->lstFontFuncs.fnGetCharIndex( lpFont->OEMFont, Chr );
	} else {
		idx = ( (Chr >= _ATC(' ')) && (Chr <= _ATC('~')) ) ? Chr - _ATC(' ') + 1 : 0;
	}

    return idx;
}


/********************

*********************/
atBOOL  atGRPX_GetGlyphMetric( atGRPX_FONT *lpFont, atUINT index, atSIZE *lpSize )
{
	if( lpSize == atNULL ) return atFALSE;
	if( lpFont == atNULL ){
		lpSize->nWidth = 	lpSize->nHeight = 0;
		return atFALSE;
	}

	if( lpFont->OEMFont ){
		atBOOL r = lpFont->lpGrpX->lstFontFuncs.fnGetGlyphMetric( lpFont->OEMFont, index, lpSize );
		if( !r ) lpSize->nWidth = lpSize->nHeight = 0;
		return r;
	} else {
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
	}

	return atTRUE;
}


/********************

*********************/
atBOOL  atGRPX_GetGlyph( atGRPX_FONT *lpFont, atUINT index, atGRPX_GLYPHINFO *lpGlyph )
{
	atSIZE size;

	if( lpGlyph == atNULL ) return atFALSE;
	if( lpFont == atNULL ) return atFALSE;

	if( lpFont->OEMFont ){
	 	lpFont->lpGrpX->lstFontFuncs.fnGetFontMetric( lpFont->OEMFont, &lpFont->nAscent, &lpFont->nDescent );		
		return lpFont->lpGrpX->lstFontFuncs.fnGetGlyphBitmap( lpFont->OEMFont, index, lpGlyph );
	} else {
		if( !atGRPX_GetGlyphMetric( lpFont, index, &size ) ) return atFALSE;
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

/*
lpGlyph->dwAttr |= atGRPX_FONT_ATTR_OWNERBUFFER;
lpGlyph->lpBitmap = lpFont->lpGrpX->lstFuncs.fnAllocMem( size.nWidth * size.nHeight );
lpGlyph->nFontWidthBytes = size.nWidth;
lpGlyph->nBPP = 8;
lpGlyph->nX = 0;
lpGlyph->nY = 0;
lpGlyph->nBitmapWidth = 8;
lpGlyph->nBitmapHeight = 16;
{
int i, k;
atBYTE *src, *des, cl;

src = &DefaultEngFont[index];
des = (atBYTE *)lpGlyph->lpBitmap;
for( i = 0; i < 16; i ++ ){
  cl = *src++;
  for( k = 0; k < 8; k++ ){
    if( cl & (0x80 >> k) ) *des = i*16; else *des = 0x00;
    des ++;
  }
}
}
*/

	}

    return atTRUE;
}


/********************

*********************/
void atGRPX_ReleaseGlyph( atGRPX_FONT *lpFont, atGRPX_GLYPHINFO *lpGlyph )
{
	if( lpFont == atNULL ) return;

	if( lpFont->OEMFont ){
		lpFont->lpGrpX->lstFontFuncs.fnFreeGlyphBitmap( lpFont->OEMFont, lpGlyph );
	} else {
        if( lpGlyph->dwAttr & atGRPX_GLYPHATTR_OWNERBITMAP ){
          if( lpGlyph->lpBitmap ) atGRPX_FreeMem( lpFont->lpGrpX, lpGlyph->lpBitmap );
        }
    }
    lpGlyph->lpBitmap = atNULL;
}



/********************

*********************/
atBOOL atGRPX_DisconnectOEMFontMng( atLPGRPXMNG lpGrpX )
{
	if( lpGrpX == atNULL ) return atFALSE;

	if( lpGrpX->lstFontFuncs.lpData == atNULL ) return atFALSE;
	if( lpGrpX->lpOEMFontMng == atNULL ) return atFALSE;
	
	lpGrpX->lstFontFuncs.fnCloseFontMng( lpGrpX->lpOEMFontMng );
	lpGrpX->lpOEMFontMng = atNULL;

	return atTRUE;
}


/********************

*********************/
atBOOL atGRPX_DisconnectOEMFont( atGRPX_FONT *lpFont )
{
	atLPGRPXMNG lpGrpX;

	if( lpFont == atNULL ) return atFALSE;
	lpGrpX = lpFont->lpGrpX;
	if( lpGrpX == atNULL ) return atFALSE;
	
	if( lpGrpX->lstFontFuncs.lpData == atNULL ) return atFALSE;
	if( lpGrpX->lpOEMFontMng == atNULL ) return atFALSE;
	if( lpFont->OEMFont == atNULL ) return atFALSE;
	
	lpGrpX->lstFontFuncs.fnDestroyFont( lpFont->OEMFont );
	lpFont->OEMFont = atNULL;
			
	return atTRUE;
}


/********************

*********************/
atBOOL atGRPX_ResyncOEMFontMng( atLPGRPXMNG lpGrpX )
{
	if( lpGrpX == atNULL ) return atFALSE;
	
	if( lpGrpX->lstFontFuncs.lpData == atNULL ) return atFALSE;
	
	if( lpGrpX->lpOEMFontMng )
		lpGrpX->lstFontFuncs.fnCloseFontMng( lpGrpX->lpOEMFontMng );
	
	lpGrpX->lpOEMFontMng = lpGrpX->lstFontFuncs.fnInitFontMng( (atLPVOID)lpGrpX, lpGrpX->lstFontFuncs.lpData, lpGrpX->lstFontFuncs.nDataType );
	if( lpGrpX->lpOEMFontMng == atNULL ) return atFALSE;
	
	lpGrpX->dwFontAttr = lpGrpX->lstFontFuncs.fnGetFontMngCaps( lpGrpX->lpOEMFontMng );

	return atTRUE;
}


/********************

*********************/
atBOOL atGRPX_ResyncOEMFont( atGRPX_FONT *lpFont )
{
	atLPGRPXMNG lpGrpX;

	if( lpFont == atNULL ) return atFALSE;
	lpGrpX = lpFont->lpGrpX;
	if( lpGrpX == atNULL ) return atFALSE;
	if( lpGrpX->lpOEMFontMng == atNULL ) return atFALSE;
	
	if( lpFont->OEMFont )
		lpGrpX->lstFontFuncs.fnDestroyFont( lpFont->OEMFont );
			
	lpFont->OEMFont = lpGrpX->lstFontFuncs.fnCreateFont( lpGrpX->lpOEMFontMng, lpFont->szFontName, lpFont->nFontSize, lpFont->dwStyle, lpFont->rAngle, &lpFont->nFontBPP );
	if( lpFont->OEMFont == atNULL ) return atFALSE;
			
	lpGrpX->lstFontFuncs.fnGetFontMetric( lpFont->OEMFont, &lpFont->nAscent, &lpFont->nDescent );

	return atTRUE;
}

