/*###########################################################################
-----------------------------------------------------------------------------

                                rTRI_F.h
                         (* =================== *)


 Version           : 1.0.0
 Dependent Modules :
 Author            : Ju, Gyeong-min ( Amos )
 Create            : 2001/04/25    01:47:09am
 Update            : 2003/04/29    00:00:00am
 C Compiler        : ADS 1.01, SDT 2.51, Borland C/C++ 5.0, Visual C/C++ 6.0
 Assembler         : ADS 1.01, SDT 2.51, Borland TASM 3.1, MASM 6.0
 Dependent Modules :
 Description       : TRI related function definition
 
-----------------------------------------------------------------------------
###########################################################################*/
#include "GrpX.h"
#include <string.h>


atLPGRPX_PEN atGRPX_CreatePen( atGRPXMNG *lpGrpX, atGRPX_PENTYPE nPenType, atBOOL bAntiAliasing, atINT nPenWidth, atINT nAlpha, atDWORD dwDrawMethod )
{
    atLPGRPX_PEN lpPen;

    if( lpGrpX == atNULL ) return atNULL;
        
    lpPen = (atGRPX_PEN *)atGRPX_AllocMem( lpGrpX, sizeof(atGRPX_PEN) );
	if( lpPen == atNULL ) return atNULL;
    memset( lpPen, 0, sizeof(atGRPX_PEN) );
	    
	lpPen->lpGrpX = lpGrpX;
	
	lpPen->nPenType         = nPenType;
	lpPen->bAntiAliasing    = bAntiAliasing;
	lpPen->nPenWidth        = nPenWidth;
	lpPen->nAlpha           = nAlpha;

    switch( lpPen->nPenType ){
        case atGRPX_PENTYPE_SOLID  : lpPen->clColor = (atGRPX_COLOR)dwDrawMethod;                break;
        case atGRPX_PENTYPE_BITMAP : lpPen->lpBitmap = (atGRPX_BITMAP *)(atINTPTR)dwDrawMethod;  break;
        default                    : lpPen->dwDrawMethod = dwDrawMethod;                         break;
    }	
	
	return lpPen;
}


atVOID atGRPX_DestroyPen( atLPGRPX_PEN lpPen )
{
    if( lpPen == atNULL ) return;
        
    atGRPX_FreeMem( lpPen->lpGrpX, lpPen );
}


atLPGRPX_BRUSH atGRPX_CreateBrush( atGRPXMNG *lpGrpX, atGRPX_BRUSHTYPE nBrushType, atBOOL bAntiAliasing, atINT nAlpha, atDWORD dwDrawMethod )
{
    atLPGRPX_BRUSH lpBrush;
    
    if( lpGrpX == atNULL ) return atNULL;
        
    lpBrush = (atGRPX_BRUSH *)atGRPX_AllocMem( lpGrpX, sizeof(atGRPX_BRUSH) );
	if( lpBrush == atNULL ) return atNULL;
    memset( lpBrush, 0, sizeof(atGRPX_BRUSH) );
	    
	lpBrush->lpGrpX = lpGrpX;
	
	lpBrush->nBrushType       = nBrushType;
	lpBrush->bAntiAliasing    = bAntiAliasing;
	lpBrush->nAlpha           = nAlpha;

    switch( lpBrush->nBrushType ){
        case atGRPX_BRUSHTYPE_SOLID  : lpBrush->clColor = (atGRPX_COLOR)dwDrawMethod;                break;
        case atGRPX_BRUSHTYPE_BITMAP : lpBrush->lpBitmap = (atGRPX_BITMAP *)(atINTPTR)dwDrawMethod;  break;
        default                      : lpBrush->dwDrawMethod = dwDrawMethod;                         break;
    }	

	return lpBrush;
}


atVOID atGRPX_DestroyBrush( atLPGRPX_BRUSH lpBrush )
{
    if( lpBrush == atNULL ) return;
        
    atGRPX_FreeMem( lpBrush->lpGrpX, lpBrush );
}

