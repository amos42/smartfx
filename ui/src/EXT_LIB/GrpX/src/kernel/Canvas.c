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
#include "GrpX.h"


atBOOL atGRPX_InitCanvas( atGRPX_CANVAS *lpCanvas, atLPGRPXMNG lpGrpX, atINT X, atINT Y, atINT Width, atINT Height, atDWORD dwAttr, atLPVOID lpFrameBuf, atINT nFrameWidth )
{
	atRECT *clip;
	atDWORD dwAttr2;

	if( lpCanvas == atNULL ) return atFALSE;
	if( lpGrpX == atNULL ) return atFALSE;

//	if( (X < 0) || (Y < 0) || (X >= lpGrpX->ScrInfo.nFrameWidth) || (Y >= lpGrpX->ScrInfo.nFrameHeight) ) return atNULL;

	lpCanvas->lpGrpX = lpGrpX;
	lpCanvas->bActivate = atTRUE;
	lpCanvas->nCanvasID = 0x01;

/*
	lpCanvas->rgnOrgArea.nX = X;
	lpCanvas->rgnOrgArea.nY = Y;
	lpCanvas->rgnOrgArea.nWidth = Width;
	lpCanvas->rgnOrgArea.nHeight = Height;
	if( (X + Width)  > lpGrpX->ScrInfo.nFrameWidth )  Width  = lpGrpX->ScrInfo.nFrameWidth  - X;
	if( (Y + Height) > lpGrpX->ScrInfo.nFrameHeight ) Height = lpGrpX->ScrInfo.nFrameHeight - Y;
*/
    SET_REGION( lpCanvas->rgnArea, X, Y, Width, Height );
    if( nFrameWidth <= 0 ) nFrameWidth = Width;	  

	dwAttr2 = 0x0000;
    if( (dwAttr & atGRPX_CANVAS_ATTR_OWNERBUFFER) ){
	  if( lpFrameBuf )
	    lpCanvas->lpVideoPtr = lpFrameBuf;
	  else
		lpCanvas->lpVideoPtr = atGRPX_AllocMem( lpGrpX, nFrameWidth * Height * (lpGrpX->ScrInfo.nBPP / 8) );
      lpCanvas->nBPP       = lpGrpX->ScrInfo.nBPP;
	  lpCanvas->nVMemWidth = nFrameWidth;
	  dwAttr2 |= atGRPX_CANVAS_ATTR_OWNERBUFFER;
    } else if( (dwAttr & atGRPX_CANVAS_ATTR_EXTBUFFER) && (lpFrameBuf != atNULL) ){
      lpCanvas->lpVideoPtr = lpFrameBuf;
      lpCanvas->nBPP       = lpGrpX->ScrInfo.nBPP;
	  lpCanvas->nVMemWidth = nFrameWidth;
	  dwAttr2 |= atGRPX_CANVAS_ATTR_EXTBUFFER;
    } else {
      if( lpGrpX->ScrInfo.lpFrameBufferPtr != atNULL ){
        lpCanvas->lpVideoPtr = (void *)((char *)lpGrpX->ScrInfo.lpFrameBufferPtr + 
											( (Y * lpGrpX->ScrInfo.nVMemWidth + X ) 
											<< ((lpGrpX->ScrInfo.nBPP >> 3)-1) ) );
      } else {
        lpCanvas->lpVideoPtr = (void *)atNULL; 
	    lpCanvas->bActivate = atFALSE;
      }
      lpCanvas->nBPP       = lpGrpX->ScrInfo.nBPP;    
	  lpCanvas->nVMemWidth = lpGrpX->ScrInfo.nVMemWidth;
      dwAttr &= ~atGRPX_CANVAS_ATTR_OWNERBUFFER;
      dwAttr &= ~atGRPX_CANVAS_ATTR_EXTBUFFER;
    }
    
    lpCanvas->dwAttr = dwAttr2;

    SET_RECT( lpCanvas->rtOrgClipRect, 0, 0, Width-1, Height-1 );

    clip = &lpCanvas->rtOrgClipRect;
    atGRPX_SetClipRect2( lpCanvas, clip );	

//	lpCanvas->lpCurFont = atNULL;  //atGRPX_ReferenceFont( atGRPX_CreateFont( lpGrpX, atGRPX_DEF_FONT_NAME, atGRPX_DEF_FONT_SIZE, 1, 0 ) );
//	lpCanvas->lpCurFont = atGRPX_ReferenceFont( (atLPGRPX_FONT)lpGrpX->lpDefFont );

	return atTRUE;
}

atGRPX_CANVAS * atGRPX_CreateCanvas( atLPGRPXMNG lpGrpX, atINT X, atINT Y, atINT Width, atINT Height, atDWORD dwAttr, atLPVOID lpFrameBuf, atINT nFrameWidth )
{
	int i, id;
	atLPGRPX_CANVAS This;
	atHGRPX_CANVAS *lppHandle;

	if( lpGrpX == atNULL ) return atNULL;

//	if( (X < 0) || (Y < 0) || (X >= lpGrpX->ScrInfo.nFrameWidth) || (Y >= lpGrpX->ScrInfo.nFrameHeight) ) return atNULL;

	lppHandle = atNULL;
	for( i = 0; i < atGRPX_DEF_MAX_CANVAS_COUNT; i++ ){
		if( lpGrpX->lstCanvas[i] == atNULL ){
			id = i;
			lppHandle = &lpGrpX->lstCanvas[i];
			break;
		}
	}
	if( lppHandle == atNULL ) return atNULL;
		
	This = atGRPX_AllocMem( lpGrpX, sizeof(atGRPX_CANVAS) );
	if( This == atNULL ) return atNULL;

	if( !atGRPX_InitCanvas( This, lpGrpX, X, Y, Width, Height, dwAttr, lpFrameBuf, nFrameWidth ) ){
		atGRPX_FreeMem( lpGrpX, This );		
		return atNULL;
	}

	*lppHandle = This;
	This->nCanvasID = id;

	return This;
}


void atGRPX_ReleaseCanvas( atGRPX_CANVAS *lpCanvas )
{
	atLPGRPX_CANVAS This = (atLPGRPX_CANVAS)lpCanvas;
		
	if( This == atNULL ) return;

//	if( This->lpCurFont ){
//		atGRPX_ReleaseFont( (atLPGRPX_FONT)This->lpCurFont, atTRUE );
//		This->lpCurFont = atNULL;
//	}

    if( This->dwAttr & atGRPX_CANVAS_ATTR_OWNERBUFFER ){
	  if( This->lpVideoPtr ){
	  	atGRPX_FreeMem( This->lpGrpX, This->lpVideoPtr );
		This->lpVideoPtr = atNULL;
	  }
    }
}


void atGRPX_DestroyCanvas( atGRPX_CANVAS *lpCanvas )
{
	atLPGRPX_CANVAS This = (atLPGRPX_CANVAS)lpCanvas;
		
	if( This == atNULL ) return;
	
	atGRPX_ReleaseCanvas( lpCanvas );

	if( This->nCanvasID >= 0 ){
		This->lpGrpX->lstCanvas[This->nCanvasID] = atNULL;
	}
		
	atGRPX_FreeMem( This->lpGrpX, This );
}      


atVOID atGRPX_ActivateCanvas( atGRPX_CANVAS *lpCanvas )
{
	atLPGRPX_CANVAS This = (atLPGRPX_CANVAS)lpCanvas;
		
	if( This == atNULL ) return;
	
	This->bActivate = atTRUE;
}


atVOID atGRPX_DeactivateCanvas( atGRPX_CANVAS *lpCanvas )
{
	atLPGRPX_CANVAS This = (atLPGRPX_CANVAS)lpCanvas;
		
	if( This == atNULL ) return;

	This->bActivate = atFALSE;
}


atBOOL atGRPX_IsActiveCanvas( atGRPX_CANVAS *lpCanvas )
{
	atLPGRPX_CANVAS This = (atLPGRPX_CANVAS)lpCanvas;
		
	if( This == atNULL ) return atFALSE;

	return This->bActivate;
}

atVOID atGRPX_RecalcCanvas( atGRPX_CANVAS *lpCanvas )
{
    atRECT *clip;

    if( lpCanvas == atNULL ) return;
	
    // 클리핑 영역을 재조정한다.
    clip = &lpCanvas->rtOrgClipRect;
    atGRPX_SetClipRect2( lpCanvas, clip );	

    if( lpCanvas->dwAttr  == atGRPX_CANVAS_ATTR_PARENTBUFFER ){
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
}


atREGION * atGRPX_GetCanvasArea( atGRPX_CANVAS *lpCanvas )
{
	atLPGRPX_CANVAS This = (atLPGRPX_CANVAS)lpCanvas;

	if( This == atNULL ) return atNULL;

	return &This->rgnArea;
}

int	atGRPX_GetBPP( atGRPX_CANVAS *lpCanvas )
{
	if( lpCanvas == atNULL ) return 0;
	return lpCanvas->nBPP; 	
}

atGRPX_PIXEL * atGRPX_GetFrameBufferPtr( atGRPX_CANVAS *lpCanvas )
{
	if( lpCanvas == atNULL ) return (atGRPX_PIXEL *)atNULL;
	return (atGRPX_PIXEL *)lpCanvas->lpVideoPtr; 	
}

atGRPX_PIXEL * atGRPX_GetFrameBufferPtrXY( atGRPX_CANVAS *lpCanvas, atINT X, atINT Y )
{
	if( lpCanvas == atNULL ) return (atGRPX_PIXEL *)atNULL;
	if( lpCanvas->lpVideoPtr == atNULL ) return (atGRPX_PIXEL *)atNULL;
	return (atGRPX_PIXEL *)( (char *)lpCanvas->lpVideoPtr + ( (Y * lpCanvas->nVMemWidth + X) << (((lpCanvas->nBPP >> 3)-1))) );
}

int atGRPX_GetVMemWidth( atGRPX_CANVAS *lpCanvas )
{
	if( lpCanvas == atNULL ) return 0;
	return lpCanvas->nVMemWidth; 	
}

atRECT * atGRPX_GetClipRect( atGRPX_CANVAS *lpCanvas )
{
	if( lpCanvas == atNULL ) return atNULL;
	return &lpCanvas->rtScreenClipRect; 	
}

atRECT * atGRPX_GetOriginalClipRect( atGRPX_CANVAS *lpCanvas )
{
	if( lpCanvas == atNULL ) return atNULL;
	return &lpCanvas->rtOrgClipRect; 	
}

atBOOL atGRPX_SetClipRect( atGRPX_CANVAS *lpCanvas, atINT nStartX, atINT nStartY, atINT nEndX, atINT nEndY )
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

atBOOL atGRPX_SetClipRect2( atGRPX_CANVAS *lpCanvas, atRECT *lpClip )
{
	return atGRPX_SetClipRect( lpCanvas, lpClip->nStartX, lpClip->nStartY, lpClip->nEndX, lpClip->nEndY );
}

/*
atGRPX_FONT	* atGRPX_SetCurFont( atGRPX_CANVAS *lpCanvas, atGRPX_FONT *lpFont )
{
	atGRPX_FONT *old_fnt;
	
	if( lpFont == atNULL ) return atNULL;

// 폰트 Gabage Collection 기능 막는다. 차후 아예 없앨지, 다시 살릴지 미정
//	old_fnt = atGRPX_ReleaseFont( lpCanvas->lpCurFont, atTRUE );
//	lpCanvas->lpCurFont = atGRPX_ReferenceFont( lpFont );
	old_fnt = lpCanvas->lpCurFont;
	lpCanvas->lpCurFont = lpFont;

	return old_fnt;
}


atGRPX_FONT	* atGRPX_GetCurFont( atGRPX_CANVAS *lpCanvas )
{
	return lpCanvas->lpCurFont;
}
*/

atGRPX_COLOR	atGRPX_SetFrColor( atGRPX_CANVAS *lpCanvas, atGRPX_COLOR color )
{
	atGRPX_COLOR old_color = lpCanvas->clFrColor;
	lpCanvas->clFrColor = color;
	return old_color;
}


atGRPX_COLOR	atGRPX_GetFrColor( atGRPX_CANVAS *lpCanvas )
{
	return lpCanvas->clFrColor;
}


atGRPX_COLOR	atGRPX_SetBgColor( atGRPX_CANVAS *lpCanvas, atGRPX_COLOR color )
{
	atGRPX_COLOR old_color = lpCanvas->clBgColor;
	lpCanvas->clBgColor = color;
	return old_color;
}


atGRPX_COLOR	atGRPX_GetBgColor( atGRPX_CANVAS *lpCanvas )
{
	return lpCanvas->clBgColor;
}


atBOOL atGRPX_ChangeCanvasSize( atGRPX_CANVAS *lpCanvas, atINT nWidth, atINT nHeight, atINT nFrameWidth )
{
	atLPGRPX_CANVAS This = (atLPGRPX_CANVAS)lpCanvas;
	atRECT *clip;
	void *ptr;
	
	if( This == atNULL ) return atFALSE;

    if( (This->rgnArea.nWidth != nWidth) || (This->rgnArea.nHeight != nHeight) ){
	  if( nFrameWidth <= 0 ) nFrameWidth = This->nVMemWidth;
	  
	  // 자체 버퍼가 있다면, 해제하고 재할당한다.
	  if( This->dwAttr & atGRPX_CANVAS_ATTR_OWNERBUFFER ){
	  	if( nFrameWidth < nWidth ) nFrameWidth = nWidth;

		if( (nFrameWidth != This->nVMemWidth) || (nHeight != This->rgnArea.nHeight ) ){
			ptr = atGRPX_AllocMem( This->lpGrpX, nFrameWidth * nHeight * (This->nBPP / 8) );
			if( ptr == atNULL ) return atFALSE;
			if( This->lpVideoPtr ) atGRPX_FreeMem( This->lpGrpX, This->lpVideoPtr );
			This->lpVideoPtr = ptr;
		}
	  }
	  
	  This->rgnArea.nWidth = nWidth;
	  This->rgnArea.nHeight = nHeight;

	  if( !(This->dwAttr & atGRPX_CANVAS_ATTR_PARENTBUFFER) )
	  	This->nVMemWidth = nFrameWidth;

//	  if( (This->rgnArea.nX + This->rgnArea.nWidth)  > lpCanvas->lpGrpX->ScrInfo.nFrameWidth ){
//		This->rgnArea.nWidth = lpCanvas->lpGrpX->ScrInfo.nFrameWidth - This->rgnArea.nX;
//	  }
//	  if( (This->rgnArea.nY + This->rgnArea.nHeight) > lpCanvas->lpGrpX->ScrInfo.nFrameHeight ){
//		This->rgnArea.nHeight = lpCanvas->lpGrpX->ScrInfo.nFrameHeight - This->rgnArea.nY;
//	  }

	  // 클리핑 영역을 재조정한다.
	  //clip = atGRPX_GetClipRect( This );
	  clip = &This->rtOrgClipRect;
      atGRPX_SetClipRect( This, clip->nStartX, clip->nStartY, clip->nEndX, clip->nEndY );	
	}

	return atTRUE;
}


atBOOL atGRPX_MoveCanvas( atGRPX_CANVAS *lpCanvas, atINT X, atINT Y )
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
	atGRPX_RecalcCanvas(lpCanvas);

	return atTRUE;
}


atBOOL	atGRPX_RealizeCanvas( atGRPX_CANVAS *lpCanvas )
{
	if( lpCanvas->lpGrpX->ScrInfo.lpFrameBufferPtr == atNULL ) return atTRUE;

    if( lpCanvas->dwAttr & atGRPX_CANVAS_ATTR_OWNERBUFFER ){
		int i, j;
		atGRPX_PIXEL *ptr, *t_ptr;

		ptr = (atGRPX_PIXEL *)atGRPX_GetFrameBufferPtr( lpCanvas );
		t_ptr = (atGRPX_PIXEL *)lpCanvas->lpGrpX->ScrInfo.lpFrameBufferPtr + lpCanvas->rgnArea.nY * lpCanvas->lpGrpX->ScrInfo.nVMemWidth + lpCanvas->rgnArea.nX; 

		for( i = 0; i < lpCanvas->rgnArea.nHeight; i++ ){
			for( j = 0; j < lpCanvas->rgnArea.nWidth; j++ ){
			  *t_ptr++ = *ptr++;
			}
			ptr += (lpCanvas->nVMemWidth - lpCanvas->rgnArea.nWidth);
			t_ptr += (lpCanvas->lpGrpX->ScrInfo.nVMemWidth - lpCanvas->rgnArea.nWidth);
		}
	}

	return atTRUE;
}

atVOID	atGRPX_CopyCanvas( atGRPX_CANVAS *lpCanvas_Des, atINT nDesX, atINT nDesY, atGRPX_CANVAS *lpCanvas_Src, atINT nX, atINT nY, atINT nWidth, atINT nHeight )
{
	int i, j;
	atGRPX_PIXEL *pDes, *pSrc;
	atREGION *src_rgn;
	atRECT *des_rct, *src_rct, src_rct0;
	int nDesX2, nDesY2, nX2, nY2;
	int des_w, src_w;

	if( (lpCanvas_Src == atNULL) || (lpCanvas_Des == atNULL) ) return;

	src_rgn = atGRPX_GetCanvasArea( lpCanvas_Src );
	REGION2RECT( src_rct0, *src_rgn );
	src_rct = &src_rct0;
	des_rct = atGRPX_GetClipRect( lpCanvas_Des );

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


	pDes = atGRPX_GetFrameBufferPtrXY( lpCanvas_Des, nDesX, nDesY );
	pSrc = atGRPX_GetFrameBufferPtrXY( lpCanvas_Src, nX, nY );
	if( pDes == pSrc || !pDes || !pSrc) return;

	des_w = atGRPX_GetVMemWidth( lpCanvas_Des ) - nWidth;
	src_w = atGRPX_GetVMemWidth( lpCanvas_Src ) - nWidth;

	for( i = 0; i < nHeight; i++ ){
	  for( j = 0; j < nWidth; j++ ){
		  *pDes++ = *pSrc++;
	  }
	  pSrc += src_w;
	  pDes += des_w;
	}
}


