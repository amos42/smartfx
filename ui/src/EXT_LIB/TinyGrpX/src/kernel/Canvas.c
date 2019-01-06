/*###########################################################################
-----------------------------------------------------------------------------

                                Canvas.c
                         (* =================== *)


 Version           : 1.0.0
 Dependent Modules : A_VM, A_Utils
 Author            : Ju, Gyeong-min ( Amos )
 Create            : 2001/04/25    01:47:09am
 Update            : 2003/04/29    00:00:00am
 C Compiler        : ADS 1.01, SDT 2.51, Borland C/C++ 5.0, Visual C/C++ 6.0
 Assembler         : ADS 1.01, SDT 2.51, Borland TASM 3.1, MASM 6.0
 Dependent Modules :
 Description       : View port header definition
 
-----------------------------------------------------------------------------
###########################################################################*/

#include "AT_Types.h"
#include "TinyGrpX.h"


atINT atTGRPX_GetCanvasInstanceSize(NOARGS)
{
	return sizeof(atTGRPX_CANVAS);
}

atBOOL atTGRPX_InitCanvas( atTGRPX_CANVAS *lpCanvas, atLPGRPXMNG lpGrpX, atINT X, atINT Y, atINT Width, atINT Height )
{
	atRECT *clip;

	if( lpCanvas == atNULL ) return atFALSE;
	if( lpGrpX == atNULL ) return atFALSE;

//	if( (X < 0) || (Y < 0) || (X >= lpGrpX->ScrInfo.nFrameWidth) || (Y >= lpGrpX->ScrInfo.nFrameHeight) ) return atNULL;

	lpCanvas->lpGrpX = lpGrpX;
	lpCanvas->bActivate = atTRUE;

/*
	lpCanvas->rgnOrgArea.nX = X;
	lpCanvas->rgnOrgArea.nY = Y;
	lpCanvas->rgnOrgArea.nWidth = Width;
	lpCanvas->rgnOrgArea.nHeight = Height;
	if( (X + Width)  > lpGrpX->ScrInfo.nFrameWidth )  Width  = lpGrpX->ScrInfo.nFrameWidth  - X;
	if( (Y + Height) > lpGrpX->ScrInfo.nFrameHeight ) Height = lpGrpX->ScrInfo.nFrameHeight - Y;
*/
    SET_REGION( lpCanvas->rgnArea, X, Y, Width, Height );

	lpCanvas->nBPP = lpGrpX->ScrInfo.nBPP;
    if( lpGrpX->ScrInfo.lpFrameBufferPtr != atNULL ){
		lpCanvas->lpVideoPtr = (void *)((char *)lpGrpX->ScrInfo.lpFrameBufferPtr + 
											( (Y * lpGrpX->ScrInfo.nVMemWidth + X ) 
											<< ((lpGrpX->ScrInfo.nBPP >> 3)-1) ) );
    } else {
		lpCanvas->lpVideoPtr = (void *)atNULL; 
		lpCanvas->bActivate = atFALSE;
    }
	lpCanvas->nVMemWidth = lpGrpX->ScrInfo.nVMemWidth;

    SET_RECT( lpCanvas->rtOrgClipRect, 0, 0, Width-1, Height-1 );

    clip = &lpCanvas->rtOrgClipRect;
    atTGRPX_SetClipRect2( lpCanvas, clip );	

//	lpCanvas->lpCurFont = atNULL;  //atTGRPX_ReferenceFont( atTGRPX_CreateFont( lpGrpX, atTGRPX_DEF_FONT_NAME, atTGRPX_DEF_FONT_SIZE, 1, 0 ) );
//	lpCanvas->lpCurFont = atTGRPX_ReferenceFont( (atLPGRPX_FONT)lpGrpX->lpDefFont );

	return atTRUE;
}

atVOID atTGRPX_ActivateCanvas( atTGRPX_CANVAS *lpCanvas )
{
	atLPGRPX_CANVAS This = (atLPGRPX_CANVAS)lpCanvas;
		
	if( This == atNULL ) return;
	
	This->bActivate = atTRUE;
}


atVOID atTGRPX_DeactivateCanvas( atTGRPX_CANVAS *lpCanvas )
{
	atLPGRPX_CANVAS This = (atLPGRPX_CANVAS)lpCanvas;
		
	if( This == atNULL ) return;

	This->bActivate = atFALSE;
}


atBOOL atTGRPX_IsActiveCanvas( atTGRPX_CANVAS *lpCanvas )
{
	atLPGRPX_CANVAS This = (atLPGRPX_CANVAS)lpCanvas;
		
	if( This == atNULL ) return atFALSE;

	return This->bActivate;
}

atVOID atTGRPX_RecalcCanvas( atTGRPX_CANVAS *lpCanvas )
{
    atRECT *clip;

    if( lpCanvas == atNULL ) return;
	
    // 클리핑 영역을 재조정한다.
    clip = &lpCanvas->rtOrgClipRect;
    atTGRPX_SetClipRect2( lpCanvas, clip );	

    lpCanvas->nVMemWidth = lpCanvas->lpGrpX->ScrInfo.nVMemWidth;
    if( lpCanvas->lpGrpX->ScrInfo.lpFrameBufferPtr != atNULL ){
		lpCanvas->lpVideoPtr = (void *)((char *)lpCanvas->lpGrpX->ScrInfo.lpFrameBufferPtr + 
											( (lpCanvas->rgnArea.nY * lpCanvas->nVMemWidth + lpCanvas->rgnArea.nX ) 
											<< ((lpCanvas->lpGrpX->ScrInfo.nBPP >> 3)-1) ) );
    } else {
		lpCanvas->lpVideoPtr = atNULL;
		lpCanvas->bActivate = atFALSE;
    }
}


atREGION * atTGRPX_GetCanvasArea( atTGRPX_CANVAS *lpCanvas )
{
	atLPGRPX_CANVAS This = (atLPGRPX_CANVAS)lpCanvas;

	if( This == atNULL ) return atNULL;

	return &This->rgnArea;
}

int	atTGRPX_GetBPP(atTGRPX_CANVAS *lpCanvas)
{
	if (lpCanvas == atNULL) return 0;
	return lpCanvas->nBPP;
}

atTGRPX_PIXEL * atTGRPX_GetFrameBufferPtr( atTGRPX_CANVAS *lpCanvas )
{
	if( lpCanvas == atNULL ) return (atTGRPX_PIXEL *)atNULL;
	return (atTGRPX_PIXEL *)lpCanvas->lpVideoPtr; 	
}

atTGRPX_PIXEL * atTGRPX_GetFrameBufferPtrXY( atTGRPX_CANVAS *lpCanvas, atINT X, atINT Y )
{
	if( lpCanvas == atNULL ) return (atTGRPX_PIXEL *)atNULL;
	if( lpCanvas->lpVideoPtr == atNULL ) return (atTGRPX_PIXEL *)atNULL;
	return (atTGRPX_PIXEL *)( (char *)lpCanvas->lpVideoPtr + ( (Y * lpCanvas->nVMemWidth + X) << (((lpCanvas->nBPP >> 3)-1))) );
}

int atTGRPX_GetVMemWidth( atTGRPX_CANVAS *lpCanvas )
{
	if( lpCanvas == atNULL ) return 0;
	return lpCanvas->nVMemWidth; 	
}

atRECT * atTGRPX_GetClipRect( atTGRPX_CANVAS *lpCanvas )
{
	if( lpCanvas == atNULL ) return atNULL;
	return &lpCanvas->rtScreenClipRect; 	
}

atRECT * atTGRPX_GetOriginalClipRect( atTGRPX_CANVAS *lpCanvas )
{
	if( lpCanvas == atNULL ) return atNULL;
	return &lpCanvas->rtOrgClipRect; 	
}

atBOOL atTGRPX_SetClipRect( atTGRPX_CANVAS *lpCanvas, atINT nStartX, atINT nStartY, atINT nEndX, atINT nEndY )
{
	atREGION *rgn;
    atRECT *clip;
    int t;
	int ex, ey;

	if( lpCanvas == atNULL ) return atFALSE;

    if( nStartX > nEndX ) SWAP( nStartX, nEndX, t );
    if( nStartY > nEndY ) SWAP( nStartY, nEndY, t );

	rgn = &lpCanvas->rgnArea;
    clip = &lpCanvas->rtScreenClipRect;

	if( (nStartX >= rgn->nWidth) || (nStartY >= rgn->nHeight) || (nEndX < 0) || (nEndY < 0) ){
      SET_RECT( *clip, 0, 0, 0, 0 );
	  return atFALSE;
    }

	SET_RECT( lpCanvas->rtOrgClipRect, nStartX, nStartY, nEndX, nEndY );

	if( nStartX < 0 ) nStartX = 0;
	if( nStartY < 0 ) nStartY = 0;
	if( nEndX >= rgn->nWidth ) nEndX = rgn->nWidth - 1;
	if( nEndY >= rgn->nHeight ) nEndY = rgn->nHeight - 1;

	clip->nStartX = ( (rgn->nX + nStartX) >= 0 )? nStartX : -rgn->nX;
	clip->nStartY = ( (rgn->nY + nStartY) >= 0 )? nStartY : -rgn->nY;
	ex = lpCanvas->lpGrpX->ScrInfo.nFrameWidth-1;
	ey = lpCanvas->lpGrpX->ScrInfo.nFrameHeight-1;
	clip->nEndX = ( (rgn->nX + nEndX) <= ex )? nEndX : ex - rgn->nX;
	clip->nEndY = ( (rgn->nY + nEndY) <= ey )? nEndY : ey - rgn->nY;

	return atTRUE;
}


atBOOL atTGRPX_SetClipRect2( atTGRPX_CANVAS *lpCanvas, atRECT *lpClip )
{
	return atTGRPX_SetClipRect( lpCanvas, lpClip->nStartX, lpClip->nStartY, lpClip->nEndX, lpClip->nEndY );
}


atBOOL atTGRPX_ChangeCanvasSize( atTGRPX_CANVAS *lpCanvas, atINT nWidth, atINT nHeight, atINT nFrameWidth )
{
	atLPGRPX_CANVAS This = (atLPGRPX_CANVAS)lpCanvas;
	atRECT *clip;
	
	if( This == atNULL ) return atFALSE;

    if( (This->rgnArea.nWidth != nWidth) || (This->rgnArea.nHeight != nHeight) ){
	  if( nFrameWidth <= 0 ) nFrameWidth = This->nVMemWidth;
	  
	  This->rgnArea.nWidth = nWidth;
	  This->rgnArea.nHeight = nHeight;

//	  if( (This->rgnArea.nX + This->rgnArea.nWidth)  > lpCanvas->lpGrpX->ScrInfo.nFrameWidth ){
//		This->rgnArea.nWidth = lpCanvas->lpGrpX->ScrInfo.nFrameWidth - This->rgnArea.nX;
//	  }
//	  if( (This->rgnArea.nY + This->rgnArea.nHeight) > lpCanvas->lpGrpX->ScrInfo.nFrameHeight ){
//		This->rgnArea.nHeight = lpCanvas->lpGrpX->ScrInfo.nFrameHeight - This->rgnArea.nY;
//	  }

	  // 클리핑 영역을 재조정한다.
	  //clip = atTGRPX_GetClipRect( This );
	  clip = &This->rtOrgClipRect;
      atTGRPX_SetClipRect( This, clip->nStartX, clip->nStartY, clip->nEndX, clip->nEndY );	
	}

	return atTRUE;
}


atBOOL atTGRPX_MoveCanvas( atTGRPX_CANVAS *lpCanvas, atINT X, atINT Y )
{
	atLPGRPX_CANVAS This = (atLPGRPX_CANVAS)lpCanvas;
//	atRECT *clip;

	if( This == atNULL ) return atFALSE;
	if( (This->rgnArea.nX == X) && (This->rgnArea.nY == Y) ) return atTRUE;

//	if( (X + This->rgnArea.nWidth)  > lpCanvas->lpGrpX->ScrInfo.nFrameWidth )  return atFALSE;
//	if( (Y + This->rgnArea.nHeight) > lpCanvas->lpGrpX->ScrInfo.nFrameHeight ) return atFALSE;

/*
	if( X < 0 ){
		This->rgnArea.nWidth -= X;
		X = 0;
	}
	if( Y < 0 ){
		This->rgnArea.nHeight -= Y;
		Y = 0;
	}
	if( (X + This->rgnArea.nWidth)  > lpCanvas->lpGrpX->ScrInfo.nFrameWidth ){
		This->rgnArea.nWidth = lpCanvas->lpGrpX->ScrInfo.nFrameWidth - X;
	}
	if( (Y + This->rgnArea.nHeight) > lpCanvas->lpGrpX->ScrInfo.nFrameHeight ){
		This->rgnArea.nHeight = lpCanvas->lpGrpX->ScrInfo.nFrameHeight - Y;
	}
*/
	This->rgnArea.nX = X;
	This->rgnArea.nY = Y;

	// 클리핑 영역을 재조정한다.
	atTGRPX_RecalcCanvas(lpCanvas);

	return atTRUE;
}


atVOID	atTGRPX_CopyCanvas( atTGRPX_CANVAS *lpCanvas_Des, atINT nDesX, atINT nDesY, atTGRPX_CANVAS *lpCanvas_Src, atINT nX, atINT nY, atINT nWidth, atINT nHeight )
{
	int i, j;
	atTGRPX_PIXEL *pDes, *pSrc;
	atREGION *src_rgn;
	atRECT *des_rct, *src_rct, src_rct0;
	int nDesX2, nDesY2, nX2, nY2;
	int des_w, src_w;

	if( (lpCanvas_Src == atNULL) || (lpCanvas_Des == atNULL) ) return;

	src_rgn = atTGRPX_GetCanvasArea( lpCanvas_Src );
	REGION2RECT( src_rct0, *src_rgn );
	src_rct = &src_rct0;
	des_rct = atTGRPX_GetClipRect( lpCanvas_Des );

	// 찍을 위치가 완전히 클리핑 영역 밖으로 벗어난 경우
	if( nDesX > des_rct->nEndX ) return;
	if( nDesY > des_rct->nEndY ) return;
	if( nX > src_rct->nEndX ) return;
	if( nY > src_rct->nEndY ) return;
	
	// == 시작 좌표 클리핑 ==
	
	// 찍을 X 좌표가 클리핑 영역을 벗어날 경우...
	if( nDesX < des_rct->nStartX ){
		nWidth -= des_rct->nStartX - nDesX;
		nX += des_rct->nStartX - nDesX;
		nDesX = des_rct->nStartX;
	}
	// 찍을 위치가 완전히 클리핑 영역 밖으로 벗어난 경우
	if( nX > src_rct->nEndX ) return;
	if( nX < src_rct->nStartX ){
		nWidth -= src_rct->nStartX - nX;
		nDesX += src_rct->nStartX - nX;
		nX = des_rct->nStartX;
	}
	// 찍을 위치가 완전히 클리핑 영역 밖으로 벗어난 경우
	if( nDesX > des_rct->nEndX ) return;

	// 찍을 Y 좌표가 클리핑 영역을 벗어날 경우...
	if( nDesY < des_rct->nStartY ){
		nHeight -= des_rct->nStartY - nDesY;
		nY += des_rct->nStartY - nDesY;
		nDesY = des_rct->nStartY;
	}
	// 찍을 위치가 완전히 클리핑 영역 밖으로 벗어난 경우
	if( nY > src_rct->nEndY ) return;
	if( nY < src_rct->nStartY ){
		nHeight -= src_rct->nStartY - nY;
		nDesY += src_rct->nStartY - nY;
		nY = des_rct->nStartY;
	}
	if( nDesY > des_rct->nEndY ) return;
	// 찍을 위치가 완전히 클리핑 영역 밖으로 벗어난 경우
	if( nDesY > des_rct->nEndY ) return;

	// == 끝 좌표 클리핑 ==

	nDesX2 = nDesX + nWidth - 1;
	// 찍을 위치가 완전히 클리핑 영역 밖으로 벗어난 경우
	if( nDesX2 < des_rct->nStartX ) return;
	// 찍을 수평 영역이 클리핑 영역을 벗어난 경우
	if( nDesX2 > des_rct->nEndX ){
		nWidth -= nDesX2 - des_rct->nEndX;
	}
	if( nWidth <= 0 ) return;
	
	nX2 = nX + nWidth - 1;
	if( nX2 < src_rct->nStartX ) return;
	if( nX2 > src_rct->nEndX ){
		nWidth -= nX2 - src_rct->nEndX;
	}
	if( nWidth <= 0 ) return;
	
	// 찍을 위치가 완전히 클리핑 영역 밖으로 벗어난 경우
	nDesX2 = nDesX + nWidth - 1;
	if( nDesX2 < des_rct->nStartX ) return;

	nDesY2 = nDesY + nHeight - 1;
	// 찍을 위치가 완전히 클리핑 영역 밖으로 벗어난 경우
	if( nDesY2 < des_rct->nStartY ) return;
	// 찍을 수평 영역이 클리핑 영역을 벗어난 경우
	if( nDesY2 > des_rct->nEndY ){
		nHeight -= nDesY2 - des_rct->nEndY;
	}
	if( nHeight <= 0 ) return;
	
	nY2 = nY + nHeight - 1;
	if( nY2 < src_rct->nStartY ) return;
	if( nY2 > src_rct->nEndY ){
		nHeight -= nY2 - src_rct->nEndY;
	}
	if( nHeight <= 0 ) return;
	
	// 찍을 위치가 완전히 클리핑 영역 밖으로 벗어난 경우
	nDesY2 = nDesY + nHeight - 1;
	if( nDesY2 < des_rct->nStartY ) return;


	pDes = atTGRPX_GetFrameBufferPtrXY( lpCanvas_Des, nDesX, nDesY );
	pSrc = atTGRPX_GetFrameBufferPtrXY( lpCanvas_Src, nX, nY );
	if( pDes == pSrc || !pDes || !pSrc) return;

	des_w = atTGRPX_GetVMemWidth( lpCanvas_Des ) - nWidth;
	src_w = atTGRPX_GetVMemWidth( lpCanvas_Src ) - nWidth;

	for( i = 0; i < nHeight; i++ ){
	  for( j = 0; j < nWidth; j++ ){
		  *pDes++ = *pSrc++;
	  }
	  pSrc += src_w;
	  pDes += des_w;
	}
}


