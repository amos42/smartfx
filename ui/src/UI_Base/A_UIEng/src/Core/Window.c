/**
  @file Window.c
  @brief Window에 관한 API
  @remark Window에 관한 API 모음
  @author Amos
  @date 2010-04-19 오전 11:41:00
  @see Window.h WinObj.h
*/
#include "UI_Types.h"
#include "UI_OpDef.h"
#include "Window.h"
//#include "WinMng.h"
#include "Control.h"
#include "CtlMng.h"
#include "App.h"
#include "UI_API/UI_API.h"
#include "UI_Util.h"


#define __DEBUG_MSG_MYWINDOW

//#define DEFAULT_FRAME_TITLE_HEIGHT 28
//#define DEFAULT_POPUP_TITLE_HEIGHT 24
//#define DEFAULT_FRAME_TITLE_WIDTH  240

/*
typedef struct _tagatWINTIMER {
	atHTIMER  hTimer;
	atHWINDOW hWnd;
	atBOOL    bFocus;
	atDWORD   id;	
} atWINTIMER;
*/


static atHWINOBJ g_hCaptureKeyObj = atNULL;


/** 
 @brief 윈도우를 생성한다.
 
 윈도우를 생성한다.

 @param hOwnerWnd [in] : 소멸을 함께 할 주인 윈도우 핸들
 @param nID [in] : 윈도우의 ID
 @param nStyle [in] : 윈도우 스타일
 @param szWindowText [in] : 윈도우의 타이틀 텍스트
 @param nWidth, nHeight [in] : 윈도우의 영역 크기
 @param lpLayoutMargin [in] : 윈도우와 클라이언트 사이의 여백 크기
 @param rtAnchor [in] : Anchor
 @param dwUserDataSize [in] : 사용자 데이터 크기
 @param fnWndProc [in] : 윈도우 핸들러
 @param lpLayoutBm [in] : 레이아웃 비트맵 세트
 @param lpCreateParam [in] : 사용자가 지정한 시작 파라미터
 @param lpExData [in] : Extra 데이터 (주로 WinClass에서 전달)
 @return 생성된 윈도우 핸들
 @see atWINOBJ_Create
*/
atHWINDOW	atWINDOW_CreateEx( atHWINDOW hOwnerWnd, int nID, int nStyle, atLPTSTR szWindowText, 
								int nWidth, int nHeight, atMARGIN *lpLayoutMargin, atMARGIN *rtAnchor,
								atDWORD dwUserDataSize, atLPWNDPROC fnWndProc,
								atLAYOUTBITMAP *lpLayoutBm,
								void *lpCreateParam, void *lpExData )
{
	atLPWINDOW	lpWnd;
	atHWINDOW	hWnd;
	atDWORD dwAttr;


	dwAttr = atOBJATTR_NORMAL | atOBJATTR_FOCUSABLE | atOBJATTR_EVENTTARGET;
	if( nStyle & atWS_MODAL ) dwAttr |= atOBJATTR_MODAL;
	if( nStyle & atWS_STAYONTOP ) dwAttr |= atOBJATTR_STAYONTOP;
	if( nStyle & atWS_TRANSPARENT ) dwAttr |= atOBJATTR_TRANSPARENT;	
	if( (nStyle & 0xFFF) == atWS_NONE ) dwAttr |= atOBJATTR_TRANSPARENT;	
	if( nStyle & atWS_UNFOCUSGRAY ) dwAttr |= atWS_UNFOCUSGRAY;	
	if( nStyle & atWS_NOFOCUSABLE ) dwAttr &= ~atOBJATTR_FOCUSABLE;	
	
	hWnd = (atHWINDOW)atWINOBJ_PreCreate( _TO_WINOBJ(hOwnerWnd), 0x0001, nID, szWindowText, dwAttr, 
										lpLayoutMargin, 
										dwUserDataSize,
										atWINDOW_DefWindowProc, fnWndProc, 
										lpCreateParam, lpExData,
									    sizeof(atWINDOW) - sizeof(atWINOBJ) );
	if( hWnd == atNULL ) return atNULL;

	lpWnd = atWINDOW_GetPtr(hWnd);

	lpWnd->nWindowStyle = nStyle;
	if( szWindowText ){
		UI_STRCPY(lpWnd->Base.szObjName, szWindowText);
		UI_STRCPY(lpWnd->szWindowTitle, szWindowText);
	} else {
		UI_STRCPY(lpWnd->Base.szObjName, _AT("<Unknown>"));
		UI_STRCPY(lpWnd->szWindowTitle, _AT("<Unknown>"));
	}

	if( lpLayoutBm ){
		atWINDOW_SetLayoutBitmap2( hWnd, lpLayoutBm );
	}
	
	lpWnd->hClientGDC = atUIAPI_CreateGDC(0, 0, nWidth, nHeight, atUIAPI_GetScreenBPP(), 0L );

	atWINOBJ_PostCreate( _TO_WINOBJ(hWnd), nWidth, nHeight, rtAnchor );
	
	return hWnd;
}


/** 
 @brief 윈도우를 생성한다.
 
 윈도우를 생성한다.

 @param hOwnerWnd [in] : 소멸을 함께 할 주인 윈도우 핸들
 @param nID [in] : 윈도우의 ID
 @param nStyle [in] : 윈도우 스타일
 @param szWindowText [in] : 윈도우의 타이틀 텍스트
 @param nStartX, nStartY [in] : 윈도우의 영역 시작 위치
 @param nWidth, nHeight [in] : 윈도우의 영역 크기
 @param dwUserDataSize [in] : 사용자 데이터 크기
 @param fnWndProc [in] : 윈도우 핸들러
 @param lpCreateParam [in] : 사용자가 지정한 시작 파라미터
 @param lpExData [in] : Extra 데이터 (주로 WinClass에서 전달)
 @return 생성된 윈도우 핸들
 @see atWINOBJ_Create
*/
atHWINDOW	atWINDOW_Create( atHWINDOW hOwnerWnd, int nID, int nStyle, atLPTSTR szWindowText, 
								int nStartX, int nStartY, int nWidth, int nHeight,
								atDWORD dwUserDataSize, atLPWNDPROC fnWndProc,
								void *lpCreateParam, void *lpExData )
{
	atMARGIN rtAnchor;
	SET_MARGIN( rtAnchor, nStartY, atANCHOR_IGNORE, nStartX, atANCHOR_IGNORE );
	return atWINDOW_CreateEx( hOwnerWnd, nID, nStyle, szWindowText, nWidth, nHeight, atNULL, &rtAnchor, dwUserDataSize, fnWndProc, atNULL, lpCreateParam, lpExData );
}


/** 
 @brief 윈도우를 소멸시킨다.
 
 윈도우를 소멸시킨다.

 @param hWnd [in] : 소멸시킬 윈도우 핸들
 @return 없음
*/
void atWINDOW_Destroy(atHWINDOW hWnd)
{
	atWINDOW_ProcessWindowProc( hWnd, WNDMSG_REQ_DESTROY, 0, 0 );
}

static void _atWINDOW_Destroy(atHWINDOW hWnd)
{
	atLPWINDOW		lpWnd = atWINDOW_GetPtr(hWnd);

	if ( lpWnd == atNULL ) return;
	if( !atWINDOW_IsValidate(hWnd) ) return;

	atUIAPI_DestroyGDC( lpWnd->hClientGDC );
	lpWnd->hClientGDC = atNULL;
}


/** 
 @brief 윈도우 클라이언트의 GDC를 얻는다.
 
 윈도우 클라이언트의 GDC를 얻는다.

 @param hWnd [in] : Window 핸들
 @return 윈도우 클라이언트의 GDC 핸들
*/
atHGDC atWINDOW_GetClientGDC(atHWINDOW hWnd)
{
	atLPWINDOW		lpWnd = atWINDOW_GetPtr(hWnd);
	atHGDC hGDC;
	atREGION rgn;
	atBOOL ret;

	if( lpWnd == atNULL ) return atNULL;
	
	hGDC = lpWnd->hClientGDC;
	if( hGDC == atNULL ) return atNULL;
	
	atWINDOW_GetClientRegion(hWnd, &rgn);
	rgn.nX = rgn.nY = 0;
	ret = atWINOBJ_ConvClientScreenRegion( _TO_WINOBJ(hWnd), &rgn, &rgn );
	if( !ret ){
		if( hGDC ) atUIAPI_ActivateGDC( hGDC, atFALSE );
		return atNULL;
	}

	ret = atUIAPI_SetGDCArea( hGDC, &rgn );
	if( !ret ){
		atUIAPI_ActivateGDC( hGDC, atFALSE );
		return atNULL;
	}

	return hGDC;
}


/** 
 @brief 윈도우 타이틀 문자열을 지정한다.
 
 윈도우 타이틀 문자열을 지정한다.\n
 타이틀 영역에 실제로 표시한다.

 @param hWnd [in] : Window 핸들
 @param szWindowText [in] : 타이틀 문자열
 @return 없음
*/
void atWINDOW_SetTitleText(atHWINDOW hWnd, atLPTSTR szWindowText)
{
	atLPWINDOW		lpWnd = atWINDOW_GetPtr(hWnd);

	atUIAPI_memset( lpWnd->szWindowTitle, 0, WINDOWTITLE_MAX_LENGTH );
	UI_STRNCPY(lpWnd->szWindowTitle, szWindowText, WINDOWTITLE_MAX_LENGTH-1);
}


/** 
 @brief 윈도우 타이틀 문자열의 IDS를 지정한다.
 
 윈도우 타이틀 문자열의 IDS를 지정한다.

 @param hWnd [in] : Window 핸들
 @param ids [in] : 타이틀 문자열의 IDS
 @return 없음
*/
void atWINDOW_SetTitleIDS(atHWINDOW hWnd, atDWORD ids)
{
	atLPWINDOW		lpWnd = atWINDOW_GetPtr(hWnd);

	atUIAPI_memset( lpWnd->szWindowTitle, 0, WINDOWTITLE_MAX_LENGTH );
	lpWnd->nTitleIDS = ids;
}


/** 
 @brief 윈도우의 타이틀 문자열을 얻는다.
 
 윈도우의 타이틀 문자열을 얻는다.

 @param hWnd [in] : Window 핸들
 @return 타이틀 문자열
*/
atLPTSTR atWINDOW_GetTitleText(atHWINDOW hWnd)
{
	atLPWINDOW		lpWnd = atWINDOW_GetPtr(hWnd);
	if( lpWnd == atNULL ) return _AT("");
	return lpWnd->szWindowTitle;
}


/** 
 @brief 윈도우에 컨트롤을 추가한다.
 
 윈도우에 컨트롤을 추가한다.

 @param hWnd [in] : Window 핸들
 @param hControl [in] : Control 핸들
 @return 성공시 atTRUE, 실패시 atFALSE
*/
atBOOL atWINDOW_AddControl( atHWINDOW hWnd, atHCONTROL hControl )
{
	atLPWINDOW lpWnd = atWINDOW_GetPtr(hWnd);
	atHCONTROL hOldCtl;
	
	if( (lpWnd == atNULL) || (hControl == atNULL) ) return atFALSE;

	if( lpWnd->Base.hChildObjMng == atNULL ){
		lpWnd->Base.hChildObjMng = atCTLMNG_CreateControlMng( _TO_WINOBJ(hWnd) );
		if( lpWnd->Base.hChildObjMng == atNULL ) return atFALSE;
	}

	hOldCtl = atCTLMNG_GetCurControl(lpWnd->Base.hChildObjMng);

	atCTLMNG_AddControl( lpWnd->Base.hChildObjMng, hControl );

	if( hOldCtl == atNULL ){
		if( atCONTROL_IsFocusable(hControl) ){
			atCTLMNG_SetTopControl( lpWnd->Base.hChildObjMng, hControl );
		}
	}
	
    return atTRUE;
}


/** 
 @brief 윈도우에 포함된 컨트롤을 삭제한다.
 
 윈도우에 포함된 컨트롤을 삭제한다.\n
 컨트롤을 삭제할 땐 컨트롤의 직접적인 소멸 함수가 아니라 이것을 써야 한다.

 @param hWnd [in] : Window 핸들
 @param hControl [in] : Control 핸들
 @return 없음
*/
void atWINDOW_RemoveControl( atHWINDOW hWnd, atHCONTROL hControl )
{
	atLPWINDOW lpWnd = atWINDOW_GetPtr(hWnd);
	if( (lpWnd == atNULL) || (hControl == atNULL) ) return;

	if( atCTLMNG_GetCurControl(lpWnd->Base.hChildObjMng) == hControl ){
		atCTLMNG_SetCurControl( lpWnd->Base.hChildObjMng, atNULL );
	}

	atCTLMNG_DestroyControl( lpWnd->Base.hChildObjMng, hControl );
}


/** 
 @brief 윈도우 클라이언트 영역을 그린다.
 
 윈도우 클라이언트 영역을 그린다.

 @param hWnd [in] : Window 핸들
 @return 없음
*/
void atWINDOW_DrawClient(atHWINDOW hWnd, atBOOL bFocused)
{
	if ( atWINDOW_IsVisible(hWnd) ){
//		atWINDOW_ProcessWindowProc( hWnd, WNDMSG_PAINT, atFALSE, 0 );
		atWINDOW_DrawAllControls( hWnd, bFocused );
//		if( bFlush ) atWINDOW_FlushClient(hWnd);

	}
}


/** 
 @brief 윈도우 레이아웃을 그린다.
 
 윈도우 레이아웃을 그린다.

 @param hWnd [in] : Window 핸들
 @return 없음
*/
void atWINDOW_DrawLayout( atHWINDOW hWnd )
{
	atLPWINDOW lpWnd = atWINDOW_GetPtr(hWnd);
	atHGDC hGDC = atWINDOW_GetWindowGDC(hWnd);

	switch( lpWnd->nWindowStyle & 0xFFF ){
		case atWS_NONE:
			break;
		case atWS_NONEFRAME:
			if( !(lpWnd->nWindowStyle & atWS_NONECLIENT) )
				atUIAPI_FillRect( hGDC, 0, 0, RECT_WIDTH(lpWnd->Base.rect), RECT_HEIGHT(lpWnd->Base.rect), UI_COLOR_WHITE );
			break;
		case atWS_FRAMEWINDOW:
		case atWS_POPUPWINDOW:
		case atWS_USERDEFINE:
			if( lpWnd->bActivateLayout ){
				atUIAPI_DrawClient( hGDC, 0, 0, RECT_WIDTH(lpWnd->Base.rect), RECT_HEIGHT(lpWnd->Base.rect),
							lpWnd->lpLayoutBm.bmTopLeft,    lpWnd->lpLayoutBm.bmTopCenter,    lpWnd->lpLayoutBm.bmTopRight,
							lpWnd->lpLayoutBm.bmMiddleLeft, lpWnd->lpLayoutBm.bmMiddleCenter, lpWnd->lpLayoutBm.bmMiddleRight,
							lpWnd->lpLayoutBm.bmBottomLeft, lpWnd->lpLayoutBm.bmBottomCenter, lpWnd->lpLayoutBm.bmBottomRight,
							!(lpWnd->nWindowStyle & atWS_NONECLIENT) );
			} else {
				// 타이틀만이라도 비트맵이 존재하면... (효용성엔 의문...)
				if( lpWnd->lpLayoutBm.bmTopLeft && lpWnd->lpLayoutBm.bmTopCenter && lpWnd->lpLayoutBm.bmTopRight  ){
					atUIAPI_FillRect( hGDC, 0, 0, RECT_WIDTH(lpWnd->Base.rect), RECT_HEIGHT(lpWnd->Base.rect), UI_COLOR_BLUE );
					atUIAPI_DrawClient1Line( hGDC, 0, 0, RECT_WIDTH(lpWnd->Base.rect), 
							lpWnd->lpLayoutBm.bmTopLeft,    lpWnd->lpLayoutBm.bmTopCenter,    lpWnd->lpLayoutBm.bmTopRight );
					if( !(lpWnd->nWindowStyle & atWS_NONECLIENT) )
					    atUIAPI_FillRect( hGDC, lpWnd->Base.rtLayoutMargin.nLeft, lpWnd->Base.rtLayoutMargin.nTop,
							  RECT_WIDTH(lpWnd->Base.rect) - lpWnd->Base.rtLayoutMargin.nLeft - lpWnd->Base.rtLayoutMargin.nRight,
							  RECT_HEIGHT(lpWnd->Base.rect) - lpWnd->Base.rtLayoutMargin.nTop - lpWnd->Base.rtLayoutMargin.nBottom,
							  UI_COLOR_BLACK);
				} else {
					atUIAPI_FillRect( hGDC, 0, 0, RECT_WIDTH(lpWnd->Base.rect), RECT_HEIGHT(lpWnd->Base.rect), UI_COLOR_BLUE );
					if( !(lpWnd->nWindowStyle & atWS_NONECLIENT) )
					    atUIAPI_FillRect( hGDC, lpWnd->Base.rtLayoutMargin.nLeft, lpWnd->Base.rtLayoutMargin.nTop,
							  RECT_WIDTH(lpWnd->Base.rect) - lpWnd->Base.rtLayoutMargin.nLeft - lpWnd->Base.rtLayoutMargin.nRight,
							  RECT_HEIGHT(lpWnd->Base.rect) - lpWnd->Base.rtLayoutMargin.nTop - lpWnd->Base.rtLayoutMargin.nBottom,
							  UI_COLOR_BLACK);
				}
			}
			if( !(lpWnd->nWindowStyle & atWS_NOTITLE) ){
				if( lpWnd->Base.rtLayoutMargin.nTop >= 12 ){
					atTCHAR str[256] = _AT("");
					atHFONT fnt = atUIAPI_SetFont( hGDC, atUIAPI_GetStockFont(1) );
					if(lpWnd->nTitleIDS){
						atUIAPI_LoadString(lpWnd->nTitleIDS, lpWnd->szWindowTitle, WINDOWTITLE_MAX_LENGTH);
					}
					myStringCut( hGDC,  lpWnd->szWindowTitle, str, RECT_WIDTH(lpWnd->Base.rect)-lpWnd->Base.rtLayoutMargin.nLeft - lpWnd->Base.rtLayoutMargin.nRight, atTRUE );
					atUIAPI_DrawStringAlign(hGDC, RECT_WIDTH(lpWnd->Base.rect)/2, lpWnd->Base.rtLayoutMargin.nTop/2, str, ALIGN_CENTER_MIDDLE, UI_COLOR_WHITE);
					atUIAPI_SetFont( hGDC, fnt );
				}
			}
			break;
		case atWS_SINGLEFRAME:
/*			
			DrawRect( hGDC, lpWnd->Base.rect.nStartX, lpWnd->Base.rect.nStartY, RECT_WIDTH(lpWnd->Base.rect), RECT_HEIGHT(lpWnd->Base.rect), UI_COLOR_BLUE );
			FillRect( hGDC, lpWnd->Base.rect.nStartX+1, lpWnd->Base.rect.nStartY+1, RECT_WIDTH(lpWnd->Base.rect)-2, RECT_HEIGHT(lpWnd->Base.rect)-2, UI_COLOR_WHITE);
*/			
			atUIAPI_DrawRect( hGDC, 0, 0, RECT_WIDTH(lpWnd->Base.rect), RECT_HEIGHT(lpWnd->Base.rect), UI_COLOR_BLUE, 1 );
			if( !(lpWnd->nWindowStyle & atWS_NONECLIENT) )
			atUIAPI_FillRect( hGDC, 1, 1, RECT_WIDTH(lpWnd->Base.rect)-2, RECT_HEIGHT(lpWnd->Base.rect)-2, UI_COLOR_WHITE);
			break;
	}
}


/** 
 @brief 스크린 좌표를 클라이언트 좌표로 변환한다.
 
 스크린 좌표를 클라이언트 좌표로 변환한다.

 @param hWnd [in] : Window 핸들
 @param x, y [in, out] : 변환할 스크린 좌표
 @return 없음
*/
void atWINDOW_ScreenToClient( atHWINDOW hWnd, int *x, int *y )
{
	atRECT rect;
	
	atWINDOW_GetClientRect(hWnd, &rect);
	
	*x -= rect.nStartX;
	*y -= rect.nStartY;
}


/** 
 @brief 스크린 좌표를 클라이언트 좌표로 변환한다.
 
 스크린 좌표를 클라이언트 좌표로 변환한다.

 @param hWnd [in] : Window 핸들
 @param x1, y1, x2, y2 [in, out] : 변환할 스크린 좌표
 @return 없음
*/
void atWINDOW_ScreenToClient2( atHWINDOW hWnd, int *x1, int *y1, int *x2, int *y2 )
{
	atRECT rect;
	
	atWINDOW_GetClientRect(hWnd, &rect);
	
	*x1 -= rect.nStartX;
	*x2 -= rect.nStartX;
	*y1 -= rect.nStartY;
	*y2 -= rect.nStartY;
}


/** 
 @brief 클라이언트 좌표를 스크린 좌표로 변환한다.
 
 클라이언트 좌표를 스크린 좌표로 변환한다.

 @param hWnd [in] : Window 핸들
 @param x, y [in, out] : 변환할 스크린 좌표
 @return 없음
*/
void atWINDOW_ClientToScreen( atHWINDOW hWnd, int *x, int *y ) 
{
	atRECT rect;
	
	atWINDOW_GetClientRect(hWnd, &rect);
	
	*x += rect.nStartX;
	*y += rect.nStartY;
}


/** 
 @brief 클라이언트 좌표를 스크린 좌표로 변환한다.
 
 클라이언트 좌표를 스크린 좌표로 변환한다.

 @param hWnd [in] : Window 핸들
 @param x1, y1, x2, y2 [in, out] : 변환할 스크린 좌표
 @return 없음
*/
void atWINDOW_ClientToScreen2( atHWINDOW hWnd, int *x1, int *y1, int *x2, int *y2 ) 
{
	atRECT rect;
	
	atWINDOW_GetClientRect(hWnd, &rect);
	
	(*x1) += rect.nStartX;
	(*x2) += rect.nStartX;
	(*y1) += rect.nStartY;
	(*y2) += rect.nStartY;
}


/** 
 @brief 레이아웃 비트맵을 설정한다.
 
 ......

 @param hWnd [in] : Window 핸들
 @param hBmLayoutTopLeft [in] : 맨 윗줄 왼쪽 비트맵 핸들
 @param hBmLayoutTopCenter [in] : 맨 윗줄 가운데 비트맵 핸들
 @param hBmLayoutTopRight [in] : 맨 윗줄 오른쪽 비트맵 핸들
 @param hBmLayoutMiddleLeft [in] : 중간층 왼쪽 비트맵 핸들
 @param hBmLayoutMiddleCenter [in] : 중간층 가운데 비트맵 핸들
 @param hBmLayoutMiddleRight [in] : 중간측 오른쪽 비트맵 핸들
 @param hBmLayoutBottomLeft [in] : 맨 아랫줄 왼쪽 비트맵 핸들
 @param hBmLayoutBottomCenter [in] : 맨 아랫줄 가운데 비트맵 핸들
 @param hBmLayoutBottomRight [in] : 맨 아랫줄 오른쪽 비트맵 핸들
 @return 성공시 atTRUE, 실패시 atFALSE
*/
atBOOL atWINDOW_SetLayoutBitmap( atHWINDOW hWnd,
								atHBITMAP hBmLayoutTopLeft, atHBITMAP hBmLayoutTopCenter, atHBITMAP hBmLayoutTopRight,
								atHBITMAP hBmLayoutMiddleLeft, atHBITMAP hBmLayoutMiddleCenter, atHBITMAP hBmLayoutMiddleRight,
								atHBITMAP hBmLayoutBottomLeft, atHBITMAP hBmLayoutBottomCenter, atHBITMAP hBmLayoutBottomRight )
{
	atLPWINDOW lpWnd = atWINDOW_GetPtr(hWnd);

	lpWnd->lpLayoutBm.bmTopLeft 	= hBmLayoutTopLeft;
	lpWnd->lpLayoutBm.bmTopCenter 	= hBmLayoutTopCenter;
	lpWnd->lpLayoutBm.bmTopRight 	= hBmLayoutTopRight;
	lpWnd->lpLayoutBm.bmMiddleLeft 	= hBmLayoutMiddleLeft;
	lpWnd->lpLayoutBm.bmMiddleCenter= hBmLayoutMiddleCenter;
	lpWnd->lpLayoutBm.bmMiddleRight = hBmLayoutMiddleRight;
	lpWnd->lpLayoutBm.bmBottomLeft 	= hBmLayoutBottomLeft;
	lpWnd->lpLayoutBm.bmBottomCenter= hBmLayoutBottomCenter;
	lpWnd->lpLayoutBm.bmBottomRight = hBmLayoutBottomRight;

	if( hBmLayoutTopLeft && hBmLayoutTopCenter && hBmLayoutTopRight &&
		hBmLayoutMiddleLeft && hBmLayoutMiddleCenter && hBmLayoutMiddleRight &&
		hBmLayoutBottomLeft && hBmLayoutBottomCenter && hBmLayoutBottomRight )
	{
//		atWINDOW_SetLayoutMargin( hWnd, atUIAPI_GetBitmapHeight(hBmLayoutTopLeft), atUIAPI_GetBitmapHeight(hBmLayoutBottomLeft),
//										atUIAPI_GetBitmapWidth(hBmLayoutMiddleLeft), atUIAPI_GetBitmapWidth(hBmLayoutMiddleRight) );
		lpWnd->bActivateLayout = atTRUE;
	}
	else
		lpWnd->bActivateLayout = atFALSE;

	return lpWnd->bActivateLayout;
}


/** 
 @brief 레이아웃 비트맵을 얻는다.
 
 레이아웃 비트맵을 얻는다.

 @param hWnd [in] : Window 핸들
 @param lpLayoutBitmap [out] : 레아이웃 비트맵 세트
 @return 없음
*/
atBOOL atWINDOW_GetLayoutBitmap( atHWINDOW hWnd,atLAYOUTBITMAP *lpLayoutBitmap )
{
	atLPWINDOW lpWnd = atWINDOW_GetPtr(hWnd);
	
	if( lpWnd == atNULL ) return atFALSE;
	if( lpLayoutBitmap == atNULL ) return atFALSE;

	atUIAPI_memcpy( lpLayoutBitmap, &lpWnd->lpLayoutBm, sizeof(atLAYOUTBITMAP) );

	return atTRUE;
}


/** 
 @brief 레이아웃 비트맵을 설정한다.
 
 레이아웃 비트맵을 설정한다.

 @param hWnd [in] : Window 핸들
 @param lpLayoutBitmap [out] : 레아이웃 비트맵 세트
 @return 없음
*/
atBOOL atWINDOW_SetLayoutBitmap2( atHWINDOW hWnd, atLAYOUTBITMAP *lpLayoutBitmap )
{
	if( lpLayoutBitmap == atNULL ){
		atLPWINDOW lpWnd = atWINDOW_GetPtr(hWnd);
		if( lpWnd == atNULL ) return atFALSE;
		RESET_LAYOUTBITMAP(lpWnd->lpLayoutBm);
	}  else {
		return atWINDOW_SetLayoutBitmap( hWnd,
					lpLayoutBitmap->bmTopLeft, 		lpLayoutBitmap->bmTopCenter, 	lpLayoutBitmap->bmTopRight, 
					lpLayoutBitmap->bmMiddleLeft, 	lpLayoutBitmap->bmMiddleCenter, 	lpLayoutBitmap->bmMiddleRight, 
					lpLayoutBitmap->bmBottomLeft, 	lpLayoutBitmap->bmBottomCenter, 	lpLayoutBitmap->bmBottomRight );
	}

	return atTRUE;
}


/** 
 @brief Window에 메시지를 보낸다.
 
 Window에 메시지를 보낸다.

 @param hWnd [in] : Window의 핸들
 @param nMsg [in] : 전송할 메시지
 @param lParam1, lParam2 [in] : 메시지의 파라미터
 @return 메시지 핸들러의 결과값
 @remark 메시지 처리가 핸들러의 직접적인 Call을 통해 즉각적으로 일어난다.
*/
int	atWINDOW_SendMessage( atHWINDOW hWnd, int nMsg, long lParam1, long lParam2 )
{
	atLPWINDOW lpWnd = atWINDOW_GetPtr(hWnd);

	if( lpWnd == atNULL ) return -1;

	return atAPP_SendMessage( lpWnd->hApp, hWnd, nMsg, lParam1, lParam2 );
}


/** 
 @brief Window에 메시지를 보낸다.
 
 Window에 메시지를 보낸다.

 @param hWnd [in] : Window의 핸들
 @param nMsg [in] : 전송할 메시지
 @param lParam1, lParam2 [in] : 메시지의 파라미터
 @return 메시지 처리 요청 결과.
 @remark 메시지 처리가 메시지 Queue를 통해 일어나기 때문에, 비동기적으로 일어난다.
*/
int	atWINDOW_PostMessage( atHWINDOW hWnd, int nMsg, long lParam1, long lParam2 )
{
	atLPWINDOW lpWnd = atWINDOW_GetPtr(hWnd);
	
	if( lpWnd == atNULL ) return -1;

	return atAPP_PostMessage( lpWnd->hApp, hWnd, nMsg, lParam1, lParam2 );
}


/** 
 @brief Window의 Default 메시지 핸들러
 
 Window의 Default 메시지 핸들러

 @param hWnd [in] : Window의 핸들
 @param nMsg [in] : 전송할 메시지
 @param lParam1, lParam2 [in] : 메시지의 파라미터
 @return 메시지 핸들러의 결과값
*/
int atWINDOW_DefWindowProc( atHWINDOW hWnd, int nMsg, long lParam1, long lParam2 )
{
	atHCONTROL hCtl;
	atLPWINDOW lpWnd = atWINDOW_GetPtr(hWnd);
	int ret = 0;
	
	if( lpWnd == atNULL ) return 0;
	
	switch( nMsg ){
		case WNDMSG_INIT:
			break;

		case WNDMSG_RELEASE:
			break;

		case WNDMSG_PREDESTROY:
			break;
			
		case WNDMSG_DESTROY:
			_atWINDOW_Destroy( hWnd );
			break;
			
		case WNDMSG_CHANGEPOS:
			break;
			
		case WNDMSG_PRERESIZE:
			break;
		case WNDMSG_RESIZE:
			break;

		case WNDMSG_KEYDOWN:
			hCtl = atCTLMNG_GetCurControl(lpWnd->Base.hChildObjMng);
			if( hCtl ){
				g_hCaptureKeyObj = hCtl;
				ret = atCONTROL_ProcessControlProc( hCtl, CTLMSG_KEYDOWN, lParam1, 0 );
				break;
			} 
			
			switch( lParam1 ){
				case atVKEY_LEFT:
				case atVKEY_UP:	
//					atWINDOW_ProcessWindowProc( hWnd, WNDMSG_REQ_CHANGECONTROLFOCUS, 0, 0 );
//					break;
				case atVKEY_RIGHT:
				case atVKEY_DOWN:
					atWINDOW_ProcessWindowProc( hWnd, WNDMSG_REQ_CHANGECONTROLFOCUS, 0, 1 );
					break;
				default:
					g_hCaptureKeyObj = hWnd;
					break;
			}
			break;
		case WNDMSG_LONGKEYDOWN:
		case WNDMSG_KEYPRESS:
			if( g_hCaptureKeyObj ){
				if( atWINOBJ_GetClassID(g_hCaptureKeyObj) == 0x0011 )
					ret = atCONTROL_ProcessControlProc( g_hCaptureKeyObj, nMsg, lParam1, 0 );
			}
			break;
		case WNDMSG_KEYUP:
			if( g_hCaptureKeyObj ){
				int r = 0;
				if( atWINOBJ_GetClassID(g_hCaptureKeyObj) == 0x0011 ){
					r = atCONTROL_ProcessControlProc( g_hCaptureKeyObj, nMsg, lParam1, 0 );
				}
				g_hCaptureKeyObj = atNULL;
				ret = r;
			}
			break;
		case WNDMSG_KEYCHAR:
			if( atCTLMNG_GetCurControl(lpWnd->Base.hChildObjMng) ){
				ret = atCONTROL_ProcessControlProc( atCTLMNG_GetCurControl(lpWnd->Base.hChildObjMng), CTLMSG_KEYCHAR, lParam1, 0 );
			}
			break;

		case WNDMSG_TIMER:
			break;

		case WNDMSG_PREPAINT:
			break;

		case WNDMSG_PAINTLAYOUT:
			atWINDOW_DrawLayout( hWnd );
			ret = 1;
			break;

		case WNDMSG_PAINT:
			break;

		case WNDMSG_PAINTOBJS:
			break;

		case WNDMSG_POSTPAINT:
			if ( atWINDOW_IsVisible(hWnd) ){
				if( !(atBOOL)lParam2 && (atWINDOW_GetAttr(hWnd) & atWS_UNFOCUSGRAY)){
					atREGION rgn;
					atWINDOW_GetWindowRegion( hWnd, &rgn );
					atUIAPI_FillRectTrans( atWINDOW_GetWindowGDC(hWnd), 0, 0, rgn.nWidth, rgn.nHeight, UI_COLOR_BLACK, 0xff/2);
				}
			}
			ret = 1;
			break;

		//case WNDMSG_GOTFOCUS:
		//	atWINDOW_Refresh(hWnd);
		//	break;

		//case WNDMSG_LOSTFOCUS:
		//	atWINDOW_Refresh(hWnd);
		//	break;

		case WNDMSG_REQ_CHANGECONTROLFOCUS:
			switch( lParam2 ){
				case 0:
					hCtl = atCTLMNG_GetPrevEnabledControl( lpWnd->Base.hChildObjMng, (atHCONTROL)lParam1 );
					break;
				case 1:
					hCtl = atCTLMNG_GetNextEnabledControl( lpWnd->Base.hChildObjMng, (atHCONTROL)lParam1 );
					break;
				default :
					hCtl = atNULL;
			}
			if( (hCtl != atNULL) && (hCtl != (atHCONTROL)lParam1) ){
				atWINDOW_SetCurControl( hWnd, hCtl );
				atCONTROL_Refresh((atHCONTROL)lParam1);
				atCONTROL_Refresh(hCtl);
				g_hCaptureKeyObj = hCtl;
				ret = 1;
			}
			break;
			
		case WNDMSG_PAUSE:
//			if( lpControl->bIsTimerStart ) atUIAPI_EnableTimer( lpControl->hCtlTimer, atFALSE );
//			r = 1;
			break;
		case WNDMSG_RESUME:
//			if( lpControl->bIsTimerStart ) atUIAPI_EnableTimer( lpControl->hCtlTimer, atTRUE );
//			r = 1;
			break;

		default:
//			if( atWINDOW_GetCurControl(lpControl->Base.hChildObjMng) ){
//				ret = atCONTROL_ProcessControlProc( atCTLMNG_GetCurControl(lpControl->Base.hChildObjMng), nMsg, lParam1, lParam2 );
//			}	
			break;
	}

	if( ret == 0 ){
		// Window의 상위 구조인, WinObj의 핸들러 처리로 넘긴다.
		ret = atWINOBJ_DefObjProc( _TO_WINOBJ(hWnd), nMsg, lParam1, lParam2 );
	}
	
	return ret;
}

