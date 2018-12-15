/*#############################################################################
-------------------------------------------------------------------------------

                                  HLINE_TX.c
                           (* =================== *)


 Version           : 1.0.0
 Dependent Modules :
 Author            : Ju, Gyeong-min ( Amos )
 Create            : 2001/04/25    01:47:09am
 Update            : 2003/04/29    00:00:00am
 C Compiler        : ADS 1.01, SDT 2.51, Borland C/C++ 5.0, Visual C/C++ 6.0
 Assembler         : ADS 1.01, SDT 2.51, Borland TASM 3.1, MASM 6.0
 Description       : Rectangle related function definition

-------------------------------------------------------------------------------
#############################################################################*/
#include "GrpX.h"
#include <string.h>


/*********************************

**********************************/
void atGRPX_DrawTextureHorizLine( atGRPX_CANVAS *lpCanvas, atINT nX, atINT nY, atINT nWidth, atGRPX_BITMAP *lpTextureBB )
{
    int i, cnt;
    atGRPX_PIXEL *pBits;
	atGRPX_PIXEL *imgptr;
    int ix, iy, iw, ibpp;
    int ww;
    atRECT *clip;

    if( nWidth <= 0 ) return;

    if( lpCanvas->nBPP != lpTextureBB->nBPP ) return;  // 현재로선 Canvas와 텍스쳐의 BPP가 같아야만 지원
        
    clip = atGRPX_GetClipRect( lpCanvas );
    if( (nY < clip->nStartY) || (nY > clip->nEndY) ) return;
    if( ((nX+nWidth) <= clip->nStartX) || (nX > clip->nEndX) ) return;
    
    if( nX < clip->nStartX ) nX = clip->nStartX;
                
    pBits = atGRPX_GetFrameBufferPtrXY( lpCanvas, nX, nY );
    
    iw = lpTextureBB->nWidth;
    ix = nX % iw;
    iy = nY % lpTextureBB->nHeight;
    ibpp = lpTextureBB->nBPP;
    
    ww = iw - ix;
    if( ww >= nWidth ){
	    imgptr = atGRPX_GetBitmapPtrXY( lpTextureBB, ix, iy );
	    memcpy( pBits, imgptr, nWidth * ibpp / 8 );
    } else {
        if( ix > 0 ){
            imgptr = atGRPX_GetBitmapPtrXY( lpTextureBB, ix, iy );
	        memcpy( pBits, imgptr, ww * ibpp / 8 );
            nWidth -= ww;
            pBits += ww;
        }
	    imgptr = atGRPX_GetBitmapPtrY( lpTextureBB, iy );
        cnt = nWidth / iw;
        for( i = 0; i < cnt; i++ ){
	        memcpy( pBits, imgptr, iw * ibpp / 8 );
            pBits += iw;
        }
        nWidth -= cnt * iw;
        if( nWidth > 0 ){
	        memcpy( pBits, imgptr, nWidth * ibpp / 8 );
        }
    }
}

