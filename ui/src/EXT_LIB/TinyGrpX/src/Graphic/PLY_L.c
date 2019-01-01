/*###########################################################################
-----------------------------------------------------------------------------

                                rTRI.c
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


typedef atGRPX_PIXEL *_FUNC_DRAW_PIXEL( atGRPX_CANVAS *, atINT, atINT, atGRPX_COLOR );
typedef void _FUNC_DRAW_LINE( atGRPX_CANVAS *, atINT, atINT, atINT, atINT, atGRPX_COLOR );
typedef int _FUNC_DRAW_LINE_DASH( atGRPX_CANVAS *, atINT, atINT, atINT, atINT, atGRPX_COLOR, atINT, atINT, atINT );

// pc release 에서 함수 파라미터 값을 한단계씩 밀려 넘기는 경우 때문에..
#if !defined(__GNUC__)
#pragma optimize( "g", off )
#endif

/*******************
  다각형 라인을 그린다.
********************/
atVOID _atGRPX_DrawPolyLine0( atGRPX_CANVAS *lpCanvas, atPOINT *lpPtrList, atINT nCount, atBOOL bClosed, atGRPX_COLOR Color, _FUNC_DRAW_PIXEL *fnDrawPixel, _FUNC_DRAW_LINE *fnDrawLine )
{
	int i;
	atPOINT *lpPtr, *lpPtr2;

	if( lpPtrList == atNULL ) return;
	if( nCount <= 0 ) return;	

	lpPtr = lpPtr2 = lpPtrList;

	if( nCount == 1 ){
		fnDrawPixel( lpCanvas, lpPtr->nX, lpPtr->nY, Color );
		return;
	}

	for( i = 0; i < nCount-1; i++ ){
		lpPtr2 ++;
		// 점의 위치가 같으면 무시
		if( (lpPtr->nX == lpPtr2->nX) && (lpPtr->nY == lpPtr2->nY) ) continue;
		
		fnDrawLine( lpCanvas, lpPtr->nX, lpPtr->nY, lpPtr2->nX, lpPtr2->nY, Color );
		lpPtr = lpPtr2;
	}

	if( bClosed && (nCount >= 3) && ((lpPtr->nX != lpPtrList->nX) || (lpPtr->nY != lpPtrList->nY)) )
		fnDrawLine( lpCanvas, lpPtr->nX, lpPtr->nY, lpPtrList->nX, lpPtrList->nY, Color);
}

atVOID _atGRPX_DrawPolyLineDash0( atGRPX_CANVAS *lpCanvas, atPOINT *lpPtrList, atINT nCount, atBOOL bClosed, atGRPX_COLOR Color, atINT nDashLen, atINT nGap, _FUNC_DRAW_PIXEL *fnDrawPixel, _FUNC_DRAW_LINE_DASH *fnDrawLine )
{
	int i, init_gapdash;
	atPOINT *lpPtr, *lpPtr2;
	
	if( lpPtrList == atNULL ) return;
	if( nCount <= 0 ) return;	
	
	lpPtr = lpPtr2 = lpPtrList;
	
	if( nCount == 1 ){
		fnDrawPixel( lpCanvas, lpPtr->nX, lpPtr->nY, Color );
		return;
	}
	
	init_gapdash = nGap / 2;
	for( i = 0; i < nCount-1; i++ ){
		lpPtr2 ++;
		// 점의 위치가 같으면 무시
		if( (lpPtr->nX == lpPtr2->nX) && (lpPtr->nY == lpPtr2->nY) ) continue;
		
		init_gapdash = fnDrawLine( lpCanvas, lpPtr->nX, lpPtr->nY, lpPtr2->nX, lpPtr2->nY, Color, nDashLen, nGap, init_gapdash );
		lpPtr = lpPtr2;
	}
	
	if( bClosed && (nCount >= 3) && ((lpPtr->nX != lpPtrList->nX) || (lpPtr->nY != lpPtrList->nY)) )
		fnDrawLine( lpCanvas, lpPtr->nX, lpPtr->nY, lpPtrList->nX, lpPtrList->nY, Color, nDashLen, nGap, init_gapdash );
}

#pragma optimize( "g", on )

static void _atGRPX_DrawPolyLine_REAL0( atGRPX_CANVAS *lpCanvas, atRPOINT *lpPtrList, atINT nCount, atBOOL bClosed, atGRPX_COLOR Color, 
						   _FUNC_DRAW_PIXEL *fnDrawPixel, _FUNC_DRAW_LINE *fnDrawLine )
{
	int i;
	atRPOINT *lpPtr, *lpPtr2;

	if( lpPtrList == atNULL ) return;
	if( nCount <= 0 ) return;	

	lpPtr = lpPtr2 = lpPtrList;

	if( nCount == 1 ){
		fnDrawPixel( lpCanvas, REALTOINT(lpPtr->rX), REALTOINT(lpPtr->rY), Color );
		return;
	}

	lpPtr2 = lpPtr + 1;
	for( i = 0; i < nCount-1; i++ ){ 
		lpPtr2 ++;
		// 점의 위치가 같으면 무시
		if( (lpPtr->rX == lpPtr2->rX) && (lpPtr->rY == lpPtr2->rY) ) continue;
		
		fnDrawLine( lpCanvas, REALTOINT(lpPtr->rX), REALTOINT(lpPtr->rY), REALTOINT(lpPtr2->rX), REALTOINT(lpPtr2->rY), Color );
		lpPtr = lpPtr2;
	}

	if( bClosed && (nCount >= 3) && ((lpPtr->rX != lpPtrList->rX) || (lpPtr->rY != lpPtrList->rY)) )
		fnDrawLine( lpCanvas, REALTOINT(lpPtr->rX), REALTOINT(lpPtr->rY), REALTOINT(lpPtrList->rX), REALTOINT(lpPtrList->rY), Color );
}


/*******************
  다각형 라인을 그린다.
********************/
void atGRPX_DrawPolyLine( atGRPX_CANVAS *lpCanvas, atPOINT *lpPtrList, atINT nCount, atBOOL bClosed, atGRPX_COLOR Color )
{
	_atGRPX_DrawPolyLine0( lpCanvas, lpPtrList, nCount, bClosed, Color, atGRPX_DrawPixel, atGRPX_DrawLine );
}


/*******************
  ------------------- 그린다.
********************/
void atGRPX_DrawPolyLineDash( atGRPX_CANVAS *lpCanvas, atPOINT *lpPtrList, atINT nCount, atBOOL bClosed, atGRPX_COLOR Color, atINT nDashLen, atINT nGap )
{
	_atGRPX_DrawPolyLineDash0( lpCanvas, lpPtrList, nCount, bClosed, Color, nDashLen, nGap, atGRPX_DrawPixel, atGRPX_DrawLineDash );
}


/*******************
  ................... 그린다.
********************/
void atGRPX_DrawPolyLineDot( atGRPX_CANVAS *lpCanvas, atPOINT *lpPtrList, atINT nCount, atBOOL bClosed, atGRPX_COLOR Color, atINT nGap )
{
	_atGRPX_DrawPolyLineDash0( lpCanvas, lpPtrList, nCount, bClosed, Color, 2, nGap, atGRPX_DrawPixel, atGRPX_DrawLineDash );
}

/*******************
  다각형 라인을 그린다.
********************/
void atGRPX_DrawPolyLine_REAL( atGRPX_CANVAS *lpCanvas, atRPOINT *lpPtrList, atINT nCount, atBOOL bClosed, atGRPX_COLOR Color )
{
	_atGRPX_DrawPolyLine_REAL0( lpCanvas, lpPtrList, nCount, bClosed, Color, atGRPX_DrawPixel, atGRPX_DrawLine );
}


/*******************
  다각형 라인을 그린다.
  (Anti-Aliasing)
********************/
void atGRPX_DrawPolyLineAA( atGRPX_CANVAS *lpCanvas, atPOINT *lpPtrList, atINT nCount, atBOOL bClosed, atGRPX_COLOR Color )
{
	_atGRPX_DrawPolyLine0( lpCanvas, lpPtrList, nCount, bClosed, Color, atGRPX_DrawPixel, atGRPX_DrawLineAA );
}


/*******************
  -------------------- 그린다.
  (Anti-Aliasing)
********************/
void atGRPX_DrawPolyLineDashAA( atGRPX_CANVAS *lpCanvas, atPOINT *lpPtrList, atINT nCount, atBOOL bClosed, atGRPX_COLOR Color, atINT nDashLen, atINT nGap )
{
	_atGRPX_DrawPolyLineDash0( lpCanvas, lpPtrList, nCount, bClosed, Color, nDashLen, nGap, atGRPX_DrawPixel, atGRPX_DrawLineDashAA );
}


/*******************
  .................... 그린다.
  (Anti-Aliasing)
********************/
void atGRPX_DrawPolyLineDotAA( atGRPX_CANVAS *lpCanvas, atPOINT *lpPtrList, atINT nCount, atBOOL bClosed, atGRPX_COLOR Color, atINT nGap )
{
	_atGRPX_DrawPolyLineDash0( lpCanvas, lpPtrList, nCount, bClosed, Color, 2, nGap, atGRPX_DrawPixel, atGRPX_DrawLineDashAA );
}

/*******************
  다각형 라인을 그린다.
  (Anti-Aliasing)
********************/
void atGRPX_DrawPolyLineAA_REAL( atGRPX_CANVAS *lpCanvas, atRPOINT *lpPtrList, atINT nCount, atBOOL bClosed, atGRPX_COLOR Color )
{
	_atGRPX_DrawPolyLine_REAL0( lpCanvas, lpPtrList, nCount, bClosed, Color, atGRPX_DrawPixel, atGRPX_DrawLineAA );
}

