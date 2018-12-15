/*#############################################################################
-------------------------------------------------------------------------------

                                  G_Kernel.c
                           (* =================== *)


 Version           : 1.0.0
 Dependent Modules :
 Author            : Ju, Gyeong-min ( Amos )
 Create            : 2001/04/25    01:47:09am
 Update            : 2003/04/29    00:00:00am
 C Compiler        : ADS 1.01, SDT 2.51, Borland C/C++ 5.0, Visual C/C++ 6.0
 Assembler         : ADS 1.01, SDT 2.51, Borland TASM 3.1, MASM 6.0
 Description       : G_Kernel related fucntion definition

-------------------------------------------------------------------------------
#############################################################################*/

#include "GrpX.h"
#include <string.h>


atLPVOID atGRPX_AllocMem( atLPGRPXMNG lpGrpX, atLONG nLen )
{
	if( lpGrpX == atNULL ) return atNULL;
	if( lpGrpX->lstFuncs.fnAllocMem == atNULL ) return atNULL;

	return lpGrpX->lstFuncs.fnAllocMem( nLen );
}

atVOID atGRPX_FreeMem( atLPGRPXMNG lpGrpX, atLPVOID lpPtr )
{
	if( lpGrpX == atNULL ) return;
	if( lpGrpX->lstFuncs.fnFreeMem == atNULL ) return;

	lpGrpX->lstFuncs.fnFreeMem( lpPtr );
}

atBOOL  atGRPX_Reset( atLPGRPXMNG lpGrpX, atINT nWidth, atINT nHeight, atINT nBPP, atVOID *pFrameBufPtr, atINT VMemWidth, atGRPXFUNCS *lstFuncs )
{
	int i;
	
	if( lpGrpX == atNULL ) return atFALSE;

	lpGrpX->lstFuncs.fnAllocMem = lstFuncs->fnAllocMem;
	lpGrpX->lstFuncs.fnFreeMem  = lstFuncs->fnFreeMem;

	lpGrpX->ScrInfo.nFrameWidth = nWidth;
	lpGrpX->ScrInfo.nFrameHeight = nHeight;
	lpGrpX->ScrInfo.lpFrameBufferPtr = pFrameBufPtr;
	lpGrpX->ScrInfo.nBPP = nBPP;
	lpGrpX->ScrInfo.nVMemWidth = VMemWidth;

	for( i = 0; i < atGRPX_DEF_MAX_CANVAS_COUNT; i++ )
		lpGrpX->lstCanvas[i] = atNULL;

	memset( &lpGrpX->lstFontFuncs, 0, sizeof(atGRPX_FONTFUNCS) );
	lpGrpX->lpOEMFontMng = atNULL;
	atGRPX_InitFontMng( lpGrpX );

	lpGrpX->lpTempBuf = atNULL;

#ifdef atGRPX_USES_POLYGON_CACHE
    lpGrpX->lpPolygonCache = atNULL;
    lpGrpX->nPolygonCacheSize = 0;
#endif

	return atTRUE;
}


atLPGRPXMNG  atGRPX_Initialize( atINT nWidth, atINT nHeight, atINT nBPP, atVOID *pFrameBufPtr, atINT VMemWidth, atGRPXFUNCS *lstFuncs )
{
	atLPGRPXMNG lpGrpX;

	if( lstFuncs->fnAllocMem == atNULL ) return atNULL;

	lpGrpX = (atLPGRPXMNG)atCALLFUNC(lstFuncs->fnAllocMem)( sizeof(atGRPXMNG) );
	if( lpGrpX == atNULL ) return atNULL;
	memset( lpGrpX, 0, sizeof(atGRPXMNG) );
	
	atGRPX_Reset( lpGrpX, nWidth, nHeight, nBPP, pFrameBufPtr, VMemWidth, lstFuncs );

	return lpGrpX;
}


void  atGRPX_Finalize( atLPGRPXMNG lpGrpX )
{
	if( lpGrpX == atNULL ) return;

#ifdef atGRPX_USES_POLYGON_CACHE
	if( lpGrpX->lpPolygonCache ) atGRPX_FreeMem( lpGrpX, lpGrpX->lpPolygonCache );
       lpGrpX->nPolygonCacheSize = 0;
#endif

	if( lpGrpX->lpTempBuf ) atGRPX_FreeMem( lpGrpX, lpGrpX->lpTempBuf );

	atGRPX_CloseFontMng( lpGrpX );

	atGRPX_FreeMem( lpGrpX, lpGrpX );
}


atBOOL  atGRPX_SetFrameBuffer( atLPGRPXMNG lpGrpX, atLPVOID pFrameBufPtr )
{
	if( lpGrpX == atNULL ) return atFALSE;

	lpGrpX->ScrInfo.lpFrameBufferPtr = pFrameBufPtr;

	return atTRUE;
}


atBOOL  atGRPX_SetScreenResolution( atLPGRPXMNG lpGrpX, atINT nWidth, atINT nHeight, atINT VMemWidth )
{
	int i;
	atLPGRPX_CANVAS lpCanvas;
	
	if( lpGrpX == atNULL ) return atFALSE;

	lpGrpX->ScrInfo.nFrameWidth = nWidth;
	lpGrpX->ScrInfo.nFrameHeight = nHeight;
	lpGrpX->ScrInfo.nVMemWidth = VMemWidth;

	for( i = 0; i < atGRPX_DEF_MAX_CANVAS_COUNT; i++ ){
		lpCanvas = (atLPGRPX_CANVAS)lpGrpX->lstCanvas[i];
		if( lpCanvas == atNULL ) continue;

		atGRPX_RecalcCanvas(lpCanvas);
	}

	return atTRUE;
}


void  atGRPX_GetScreenInfo( atLPGRPXMNG lpGrpX, atGRPXSCRINFO *lpScrInfo )
{
	*lpScrInfo = lpGrpX->ScrInfo;
}


atGRPX_COLOR atGRPX_GetRGB( atBYTE r, atBYTE g, atBYTE b )
{
	return MAKE565( r,g,b );
}

atBYTE atGRPX_GetRValue( atGRPX_COLOR nColor )
{
	return GET_RED( nColor );
}

atBYTE atGRPX_GetGValue( atGRPX_COLOR nColor )
{
	return GET_GREEN( nColor );
}

atBYTE atGRPX_GetBValue( atGRPX_COLOR nColor )
{
	return GET_BLUE( nColor );
}


atDWORD atGRPX_GetCodePage( atLPGRPXMNG lpGrpX )
{
    return 0;
}


void  atGRPX_SetCodePage( atLPGRPXMNG lpGrpX, atDWORD CodePage )
{
}

void  atGRPX_RegistOEMFontMng( atLPGRPXMNG lpGrpX, atGRPX_FONTFUNCS *lstFontFuncs )
{
    if( lpGrpX == atNULL ) return;
    if( lstFontFuncs == atNULL ){
      memset( &lpGrpX->lstFontFuncs, 0, sizeof(lpGrpX->lstFontFuncs) );
      return;
    }

	lpGrpX->lstFontFuncs.lpData = lstFontFuncs->lpData;
	lpGrpX->lstFontFuncs.nDataType = lstFontFuncs->nDataType;
	lpGrpX->lstFontFuncs.fnInitFontMng = lstFontFuncs->fnInitFontMng;
	lpGrpX->lstFontFuncs.fnCloseFontMng = lstFontFuncs->fnCloseFontMng;
	lpGrpX->lstFontFuncs.fnGetFontMngCaps = lstFontFuncs->fnGetFontMngCaps;
	lpGrpX->lstFontFuncs.fnCreateFont = lstFontFuncs->fnCreateFont;
	lpGrpX->lstFontFuncs.fnDestroyFont = lstFontFuncs->fnDestroyFont;
	lpGrpX->lstFontFuncs.fnSetFontStyle = lstFontFuncs->fnSetFontStyle;
	lpGrpX->lstFontFuncs.fnSetFontAngle = lstFontFuncs->fnSetFontAngle;
	lpGrpX->lstFontFuncs.fnGetFontMetric = lstFontFuncs->fnGetFontMetric;
	lpGrpX->lstFontFuncs.fnGetCharIndex = lstFontFuncs->fnGetCharIndex;
	lpGrpX->lstFontFuncs.fnGetGlyphMetric = lstFontFuncs->fnGetGlyphMetric;
	lpGrpX->lstFontFuncs.fnGetGlyphBitmap = lstFontFuncs->fnGetGlyphBitmap;
	lpGrpX->lstFontFuncs.fnFreeGlyphBitmap = lstFontFuncs->fnFreeGlyphBitmap;

	lpGrpX->lpOEMFontMng = atSAFECALLFUNC(lpGrpX->lstFontFuncs.fnInitFontMng)( lpGrpX, lpGrpX->lstFontFuncs.lpData, lpGrpX->lstFontFuncs.nDataType );
	lpGrpX->dwFontAttr = atSAFECALLFUNC(lpGrpX->lstFontFuncs.fnGetFontMngCaps)( lpGrpX->lpOEMFontMng );	
}


void  atGRPX_ReleaseOEMFontMng( atLPGRPXMNG lpGrpX )
{
    if( lpGrpX == atNULL ) return;
	if( lpGrpX->lpOEMFontMng ){
 		atCALLFUNC(lpGrpX->lstFontFuncs.fnCloseFontMng)( lpGrpX->lpOEMFontMng );
		lpGrpX->lpOEMFontMng = atNULL;
	}

    memset( &lpGrpX->lstFontFuncs, 0, sizeof(lpGrpX->lstFontFuncs) );
}
