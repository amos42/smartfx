/*###########################################################################
-----------------------------------------------------------------------------

                                PPLY_FAA.c
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
#include "_PLY_func.h"
#include <string.h>


static void _HorizLine( atGRPX_CANVAS *lpCanvas, atREAL x1, atREAL x2, atINT y, atREAL dx1, atREAL dx2, atDWORD dwAttr, atLONG ExParam1 )
{
//    atGRPX_DrawTextureHorizLine( lpCanvas, REALTOINT_R(x1), y, REALTOINT_R(x2)-REALTOINT_R(x1)+1, (atGRPX_BITMAP *)ExData1 );

    int i, cnt;
    atGRPX_BITMAP *lpBB = (atGRPX_BITMAP *)(atINTPTR)ExParam1;
    int xx1 = REALTOINT_R(x1);
    int xx2 = REALTOINT(x2);
    int width = xx2 - xx1 + 1;
    atGRPX_PIXEL *pBits = atGRPX_GetFrameBufferPtrXY( lpCanvas, xx1, y );
    int iw = lpBB->nWidth;
    int ix = xx1 % iw;
    int iy = y % lpBB->nHeight;
	atGRPX_PIXEL *pclBits;
    int ww;

    ww = iw - ix;
    if( ww >= width ){
	    pclBits = atGRPX_GetBitmapPtrXY( lpBB, ix, iy );
	    memcpy( pBits, pclBits, width * sizeof(atGRPX_PIXEL) );
    } else {
        if( ix > 0 ){
            pclBits = atGRPX_GetBitmapPtrXY( lpBB, ix, iy );
	        memcpy( pBits, pclBits, ww * sizeof(atGRPX_PIXEL) );
            width -= ww;
            pBits += ww;
        }
	    pclBits = atGRPX_GetBitmapPtrY( lpBB, iy );
        cnt = width / iw;
        for( i = 0; i < cnt; i++ ){
	        memcpy( pBits, pclBits, iw * sizeof(atGRPX_PIXEL) );
            pBits += iw;
        }
        width -= cnt * iw;
        if( width > 0 ){
	        memcpy( pBits, pclBits, width * sizeof(atGRPX_PIXEL) );
        }
    }
}


/*******************************
  다각형 라인을 그린다.
  (Multi Polygon, Hole Polygon)
********************************/
void atGRPX_FillPolyPolygonBitmap( atGRPX_CANVAS *lpCanvas, atPOINT lpPtrList[], atINT nPartCount[], atINT nPolyCount, atGRPX_BITMAP *lpBB )
{
	if( (lpCanvas == atNULL) || (lpBB == atNULL) ) return;
    if( lpCanvas->nBPP != lpBB->nBPP ) return;  // 현재로선 Canvas와 텍스쳐의 BPP가 같아야만 지원
    __atGRPX_FillPolyPolygon0( lpCanvas, lpPtrList, nPartCount, nPolyCount, (atLONG)(atINTPTR)lpBB, _HorizLine, atNULL, atNULL );
}


/*******************************
  다각형 라인을 그린다.
  (Multi Polygon, Hole Polygon)
********************************/
void atGRPX_FillPolyPolygonBitmapEx( atGRPX_CANVAS *lpCanvas, atPOINT lpPtrList[], atINT nPartCount[], atINT nPolyCount, atGRPX_BITMAP *lpBB, atLPGRPX_FUNC_COORD_CONV lpCnvFunc, atLPVOID lpParam )
{
	if( (lpCanvas == atNULL) || (lpBB == atNULL) ) return;
    if( lpCanvas->nBPP != lpBB->nBPP ) return;  // 현재로선 Canvas와 텍스쳐의 BPP가 같아야만 지원
    __atGRPX_FillPolyPolygon0( lpCanvas, lpPtrList, nPartCount, nPolyCount, (atLONG)(atINTPTR)lpBB, _HorizLine, lpCnvFunc, lpParam );
}


