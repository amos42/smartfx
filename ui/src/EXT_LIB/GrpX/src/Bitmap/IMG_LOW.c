/*#############################################################################
-------------------------------------------------------------------------------

                                  IMG_LOW.c
                           (* =================== *)


 Version           : 1.0.0
 Dependent Modules :
 Author            : Ju, Gyeong-min ( Amos )
 Create            : 2001/04/25    01:47:09am
 Update            : 2003/04/29    00:00:00am
 C Compiler        : ADS 1.01, SDT 2.51, Borland C/C++ 5.0, Visual C/C++ 6.0
 Assembler         : ADS 1.01, SDT 2.51, Borland TASM 3.1, MASM 6.0
 Description       : LOW image function definition

-------------------------------------------------------------------------------
#############################################################################*/
#include <stdlib.h>
#include <string.h>
#include "GrpX.h"


static void *_alloc_mem( atGRPX_BITMAP *lpBitmap, long size )
{
	return ( lpBitmap->fnAllocator != atNULL )? 
	    lpBitmap->fnAllocator( size )
	:
	    atGRPX_AllocMem( lpBitmap->lpGrpX, size );
}

static void _free_mem( atGRPX_BITMAP *lpBitmap, void *ptr )
{
	if( lpBitmap->fnDeallocator != atNULL )
	   lpBitmap->fnDeallocator( ptr );
	else
	   atGRPX_FreeMem( lpBitmap->lpGrpX, ptr );
}


/***********************
  빈 이미지를 생성한다.
************************/
atGRPX_BITMAP * atGRPX_CreateBlankBitmapEx( atLPGRPXMNG lpGrpX, atINT Width, atINT Height, atINT nBPP, atINT nPalBPP, atLPCALLBACKALLOC fnAllocator, atLPCALLBACKFREE fnDeallocator )
{
    int ImgSize;
    atGRPX_BITMAP *lpBitmap;
    int twidth;

	if( lpGrpX == atNULL ) return atNULL;

    if( fnAllocator != atNULL )
       lpBitmap = (atGRPX_BITMAP *)fnAllocator( sizeof(atGRPX_BITMAP) );
    else
       lpBitmap = (atGRPX_BITMAP *)atGRPX_AllocMem( lpGrpX, sizeof(atGRPX_BITMAP) );
	if( lpBitmap == atNULL ) return atNULL;
    memset( lpBitmap, 0, sizeof(atGRPX_BITMAP) );
	lpBitmap->lpGrpX = lpGrpX;
    lpBitmap->fnAllocator = fnAllocator;
    lpBitmap->fnDeallocator = fnDeallocator;

    lpBitmap->dwAttr = atGRPX_BMATTR_NONE;
    lpBitmap->nWidth = Width;
    lpBitmap->nHeight = Height;
    lpBitmap->nBPP = nBPP;
    lpBitmap->nPalBPP = nPalBPP;
    if( nBPP >= 8 ){
      twidth = Width * nBPP/8;
    } else {
      twidth = Width * nBPP/8;
      if( (Width * nBPP) % 8 ) twidth++;
    }
    lpBitmap->nWidthBytes = twidth;
    ImgSize = lpBitmap->nWidthBytes * Height;

    /* Word Alignment */
    if( ImgSize > 0 )
      lpBitmap->lpBits = (atVOID *)atGRPX_AllocMem( lpGrpX, ImgSize );
	else
      lpBitmap->lpBits = (atVOID *)atNULL;

    if( lpBitmap->lpBits == atNULL ){
	  _free_mem( lpBitmap, lpBitmap );
	  return atNULL;
    }

    lpBitmap->nLogX = lpBitmap->nLogY = 0;
    
    if( (nBPP <= 8) && (nPalBPP > 0) ){
        int pal_size = (1 << nBPP);
        int ent_size = nPalBPP/8;
        if( nPalBPP % 8 > 0 ) ent_size++;
        lpBitmap->lpPaletteTable = atGRPX_AllocMem( lpGrpX, pal_size * ent_size );
        if( lpBitmap->lpPaletteTable == atNULL ){
          _free_mem( lpBitmap, lpBitmap->lpBits );
          _free_mem( lpBitmap, lpBitmap );
		  return atNULL;
        }
    } else {
        lpBitmap->lpPaletteTable = atNULL;
    }

    lpBitmap->lpAlphaChannel = atNULL;

    return( lpBitmap );
}


/***********************
  빈 이미지를 생성한다.
************************/
atGRPX_BITMAP * atGRPX_CreateBlankBitmap( atLPGRPXMNG lpGrpX, atINT Width, atINT Height, atINT nBPP, atINT nPalBPP )
{
	return atGRPX_CreateBlankBitmapEx( lpGrpX, Width, Height, nBPP, nPalBPP, atNULL, atNULL );
}


/*********************
  이미지를 삭제한다.
**********************/
void atGRPX_DestroyBitmap( atGRPX_BITMAP *lpBitmap )
{
    if( lpBitmap == atNULL ) return;

    if( lpBitmap->lpPaletteTable ){
    	_free_mem( lpBitmap, lpBitmap->lpPaletteTable );
       lpBitmap->lpPaletteTable = atNULL;
    }
    
    if( lpBitmap->lpBits ){
        _free_mem( lpBitmap, lpBitmap->lpBits );
        lpBitmap->lpBits = atNULL;
    }
    
    if( lpBitmap->lpAlphaChannel ){
	 if( lpBitmap->dwAttr & atGRPX_BMATTR_OWN_ALPHACHANNEL ){
      		_free_mem( lpBitmap, lpBitmap->lpAlphaChannel );		
	 }
        lpBitmap->lpAlphaChannel = atNULL;
    }
		
    _free_mem( lpBitmap, lpBitmap );
}


/*************************
  이미지 사이즈를 얻는다.
**************************/
void atGRPX_GetBitmapMetric( atGRPX_BITMAP *lpBitmap, atSIZE *lpSize )
{
	if( lpSize == atNULL ) return;

    if( lpBitmap == atNULL ){
      lpSize->nWidth = lpSize->nHeight = 0; 
	  return;
    }

	lpSize->nWidth = lpBitmap->nWidth;
	lpSize->nHeight = lpBitmap->nHeight;
}


/*************************
  논리적 위치를 얻는다.
**************************/
void atGRPX_GetBitmapLogPos( atGRPX_BITMAP *lpBitmap, atINT *pLogX, atINT *pLogY )
{
	if( pLogX == atNULL ) return;

	if( lpBitmap == atNULL ){
	    *pLogX = *pLogY = 0;
	    return;
	}

    *pLogX = lpBitmap->nLogX;
    *pLogY = lpBitmap->nLogY;
}


/*************************
  논리적 위치를 지정한다.
**************************/
void atGRPX_SetBitmapLogPos( atGRPX_BITMAP *lpBitmap, atINT LogX, atINT LogY )
{
	if( lpBitmap == atNULL ) return;

    lpBitmap->nLogX = LogX;
    lpBitmap->nLogY = LogY;
}


/*************************
  논리적 위치를 가운데로 지정한다.
**************************/
void atGRPX_SetBitmapLogPosCenter( atGRPX_BITMAP *lpBitmap )
{
	if( lpBitmap == atNULL ) return;

	atGRPX_SetBitmapLogPos( lpBitmap, -lpBitmap->nWidth >> 1, -lpBitmap->nHeight >> 1 );
}


/*************************
  투명색을 세트한다.
**************************/
void atGRPX_SetBitmapTransColor( atGRPX_BITMAP *lpBitmap, atGRPX_COLOR clTransColor )
{
	if( lpBitmap == atNULL ) return;

    lpBitmap->dwAttr |= atGRPX_BMATTR_TRANSPARENT;
    lpBitmap->clTransColor = clTransColor;
}


/*************************
  투명색을 제거한다.
**************************/
void atGRPX_ClearBitmapTransColor( atGRPX_BITMAP *lpBitmap )
{
	if( lpBitmap == atNULL ) return;

    lpBitmap->dwAttr &= ~(atDWORD)atGRPX_BMATTR_TRANSPARENT;
}


/*************************
  이미지 포인터를 얻는다.
**************************/
atVOID * atGRPX_GetBitmapPtr( atGRPX_BITMAP *lpBitmap )
{
    if( lpBitmap == atNULL ) return atNULL;

    return (atVOID *)lpBitmap->lpBits;
}


/*************************
  이미지 포인터를 얻는다.
**************************/
atVOID * atGRPX_GetBitmapPtrY( atGRPX_BITMAP *lpBitmap, atINT nY )
{
    if( lpBitmap == atNULL ) return atNULL;
    if( lpBitmap->lpBits == atNULL ) return atNULL;

    return (atVOID *)((atBYTE *)lpBitmap->lpBits + nY * lpBitmap->nWidthBytes);
}


/*************************
  이미지 포인터를 얻는다.
**************************/
atVOID * atGRPX_GetBitmapPtrXY( atGRPX_BITMAP *lpBitmap, atINT nX, atINT nY )
{
    if( lpBitmap == atNULL ) return atNULL;
    if( lpBitmap->lpBits == atNULL ) return atNULL;

    return (atVOID *)((atBYTE *)lpBitmap->lpBits + nY * lpBitmap->nWidthBytes + (nX * lpBitmap->nBPP/8));
}


/*************************
  이미지 포인터를 얻는다.
**************************/
atVOID * atGRPX_GetBitmapPtrLogXY( atGRPX_BITMAP *lpBitmap, atINT nX, atINT nY )
{
    if( lpBitmap == atNULL ) return atNULL;
    if( lpBitmap->lpBits == atNULL ) return atNULL;

    nX += lpBitmap->nLogX; if( nX < 0 ) nX += lpBitmap->nWidth; else if( nX > lpBitmap->nWidth ) nX -= lpBitmap->nWidth;
    nY += lpBitmap->nLogY; if( nY < 0 ) nY += lpBitmap->nHeight; else if( nY > lpBitmap->nHeight ) nY -= lpBitmap->nHeight;
    return (atVOID *)((atBYTE *)lpBitmap->lpBits + nY * lpBitmap->nWidthBytes + (nX * lpBitmap->nBPP/8));
}


/*************************
  이미지에 알파 채널 추가
**************************/
atVOID  atGRPX_SetBitmapAlphaChannel( atGRPX_BITMAP *lpBitmap, atBYTE *lpAlphaChannel, atBOOL bOwnMask )
{
    if( lpBitmap == atNULL ) return;

    if( lpBitmap->lpAlphaChannel != atNULL ){
    	if( lpBitmap->dwAttr & atGRPX_BMATTR_OWN_ALPHACHANNEL ){
	   _free_mem( lpBitmap, lpBitmap->lpAlphaChannel );
    	}
    }

    lpBitmap->lpAlphaChannel = lpAlphaChannel;

    if( (lpAlphaChannel != atNULL) && bOwnMask ){
	lpBitmap->dwAttr |= atGRPX_BMATTR_OWN_ALPHACHANNEL;
    } else {
	lpBitmap->dwAttr &= ~atGRPX_BMATTR_OWN_ALPHACHANNEL;
    }
}

