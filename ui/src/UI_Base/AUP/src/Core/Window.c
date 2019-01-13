/**
  @file Window.c
  @brief Window�� ���� API
  @remark Window�� ���� API ����
  @author Amos
  @date 2010-04-19 ���� 11:41:00
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
 @brief �����츦 �����Ѵ�.
 
 �����츦 �����Ѵ�.

 @param hOwnerWnd [in] : �Ҹ��� �Բ� �� ���� ������ �ڵ�
 @param nID [in] : �������� ID
 @param nStyle [in] : ������ ��Ÿ��
 @param szWindowText [in] : �������� Ÿ��Ʋ �ؽ�Ʈ
 @param nWidth, nHeight [in] : �������� ���� ũ��
 @param lpLayoutMargin [in] : ������� Ŭ���̾�Ʈ ������ ���� ũ��
 @param rtAnchor [in] : Anchor
 @param dwUserDataSize [in] : ����� ������ ũ��
 @param fnWndProc [in] : ������ �ڵ鷯
 @param lpLayoutBm [in] : ���̾ƿ� ��Ʈ�� ��Ʈ
 @param lpCreateParam [in] : ����ڰ� ������ ���� �Ķ����
 @param lpExData [in] : Extra ������ (�ַ� WinClass���� ����)
 @return ������ ������ �ڵ�
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
 @brief �����츦 �����Ѵ�.
 
 �����츦 �����Ѵ�.

 @param hOwnerWnd [in] : �Ҹ��� �Բ� �� ���� ������ �ڵ�
 @param nID [in] : �������� ID
 @param nStyle [in] : ������ ��Ÿ��
 @param szWindowText [in] : �������� Ÿ��Ʋ �ؽ�Ʈ
 @param nStartX, nStartY [in] : �������� ���� ���� ��ġ
 @param nWidth, nHeight [in] : �������� ���� ũ��
 @param dwUserDataSize [in] : ����� ������ ũ��
 @param fnWndProc [in] : ������ �ڵ鷯
 @param lpCreateParam [in] : ����ڰ� ������ ���� �Ķ����
 @param lpExData [in] : Extra ������ (�ַ� WinClass���� ����)
 @return ������ ������ �ڵ�
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
 @brief �����츦 �Ҹ��Ų��.
 
 �����츦 �Ҹ��Ų��.

 @param hWnd [in] : �Ҹ��ų ������ �ڵ�
 @return ����
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
 @brief ������ Ŭ���̾�Ʈ�� GDC�� ��´�.
 
 ������ Ŭ���̾�Ʈ�� GDC�� ��´�.

 @param hWnd [in] : Window �ڵ�
 @return ������ Ŭ���̾�Ʈ�� GDC �ڵ�
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
 @brief ������ Ÿ��Ʋ ���ڿ��� �����Ѵ�.
 
 ������ Ÿ��Ʋ ���ڿ��� �����Ѵ�.\n
 Ÿ��Ʋ ������ ������ ǥ���Ѵ�.

 @param hWnd [in] : Window �ڵ�
 @param szWindowText [in] : Ÿ��Ʋ ���ڿ�
 @return ����
*/
void atWINDOW_SetTitleText(atHWINDOW hWnd, atLPTSTR szWindowText)
{
	atLPWINDOW		lpWnd = atWINDOW_GetPtr(hWnd);

	atUIAPI_memset( lpWnd->szWindowTitle, 0, WINDOWTITLE_MAX_LENGTH );
	UI_STRNCPY(lpWnd->szWindowTitle, szWindowText, WINDOWTITLE_MAX_LENGTH-1);
}


/** 
 @brief ������ Ÿ��Ʋ ���ڿ��� IDS�� �����Ѵ�.
 
 ������ Ÿ��Ʋ ���ڿ��� IDS�� �����Ѵ�.

 @param hWnd [in] : Window �ڵ�
 @param ids [in] : Ÿ��Ʋ ���ڿ��� IDS
 @return ����
*/
void atWINDOW_SetTitleIDS(atHWINDOW hWnd, atDWORD ids)
{
	atLPWINDOW		lpWnd = atWINDOW_GetPtr(hWnd);

	atUIAPI_memset( lpWnd->szWindowTitle, 0, WINDOWTITLE_MAX_LENGTH );
	lpWnd->nTitleIDS = ids;
}


/** 
 @brief �������� Ÿ��Ʋ ���ڿ��� ��´�.
 
 �������� Ÿ��Ʋ ���ڿ��� ��´�.

 @param hWnd [in] : Window �ڵ�
 @return Ÿ��Ʋ ���ڿ�
*/
atLPTSTR atWINDOW_GetTitleText(atHWINDOW hWnd)
{
	atLPWINDOW		lpWnd = atWINDOW_GetPtr(hWnd);
	if( lpWnd == atNULL ) return _AT("");
	return lpWnd->szWindowTitle;
}


/** 
 @brief �����쿡 ��Ʈ���� �߰��Ѵ�.
 
 �����쿡 ��Ʈ���� �߰��Ѵ�.

 @param hWnd [in] : Window �ڵ�
 @param hControl [in] : Control �ڵ�
 @return ������ atTRUE, ���н� atFALSE
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
 @brief �����쿡 ���Ե� ��Ʈ���� �����Ѵ�.
 
 �����쿡 ���Ե� ��Ʈ���� �����Ѵ�.\n
 ��Ʈ���� ������ �� ��Ʈ���� �������� �Ҹ� �Լ��� �ƴ϶� �̰��� ��� �Ѵ�.

 @param hWnd [in] : Window �ڵ�
 @param hControl [in] : Control �ڵ�
 @return ����
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
 @brief ������ Ŭ���̾�Ʈ ������ �׸���.
 
 ������ Ŭ���̾�Ʈ ������ �׸���.

 @param hWnd [in] : Window �ڵ�
 @return ����
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
 @brief ������ ���̾ƿ��� �׸���.
 
 ������ ���̾ƿ��� �׸���.

 @param hWnd [in] : Window �ڵ�
 @return ����
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
				// Ÿ��Ʋ���̶� ��Ʈ���� �����ϸ�... (ȿ�뼺�� �ǹ�...)
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
 @brief ��ũ�� ��ǥ�� Ŭ���̾�Ʈ ��ǥ�� ��ȯ�Ѵ�.
 
 ��ũ�� ��ǥ�� Ŭ���̾�Ʈ ��ǥ�� ��ȯ�Ѵ�.

 @param hWnd [in] : Window �ڵ�
 @param x, y [in, out] : ��ȯ�� ��ũ�� ��ǥ
 @return ����
*/
void atWINDOW_ScreenToClient( atHWINDOW hWnd, int *x, int *y )
{
	atRECT rect;
	
	atWINDOW_GetClientRect(hWnd, &rect);
	
	*x -= rect.nStartX;
	*y -= rect.nStartY;
}


/** 
 @brief ��ũ�� ��ǥ�� Ŭ���̾�Ʈ ��ǥ�� ��ȯ�Ѵ�.
 
 ��ũ�� ��ǥ�� Ŭ���̾�Ʈ ��ǥ�� ��ȯ�Ѵ�.

 @param hWnd [in] : Window �ڵ�
 @param x1, y1, x2, y2 [in, out] : ��ȯ�� ��ũ�� ��ǥ
 @return ����
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
 @brief Ŭ���̾�Ʈ ��ǥ�� ��ũ�� ��ǥ�� ��ȯ�Ѵ�.
 
 Ŭ���̾�Ʈ ��ǥ�� ��ũ�� ��ǥ�� ��ȯ�Ѵ�.

 @param hWnd [in] : Window �ڵ�
 @param x, y [in, out] : ��ȯ�� ��ũ�� ��ǥ
 @return ����
*/
void atWINDOW_ClientToScreen( atHWINDOW hWnd, int *x, int *y ) 
{
	atRECT rect;
	
	atWINDOW_GetClientRect(hWnd, &rect);
	
	*x += rect.nStartX;
	*y += rect.nStartY;
}


/** 
 @brief Ŭ���̾�Ʈ ��ǥ�� ��ũ�� ��ǥ�� ��ȯ�Ѵ�.
 
 Ŭ���̾�Ʈ ��ǥ�� ��ũ�� ��ǥ�� ��ȯ�Ѵ�.

 @param hWnd [in] : Window �ڵ�
 @param x1, y1, x2, y2 [in, out] : ��ȯ�� ��ũ�� ��ǥ
 @return ����
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
 @brief ���̾ƿ� ��Ʈ���� �����Ѵ�.
 
 ......

 @param hWnd [in] : Window �ڵ�
 @param hBmLayoutTopLeft [in] : �� ���� ���� ��Ʈ�� �ڵ�
 @param hBmLayoutTopCenter [in] : �� ���� ��� ��Ʈ�� �ڵ�
 @param hBmLayoutTopRight [in] : �� ���� ������ ��Ʈ�� �ڵ�
 @param hBmLayoutMiddleLeft [in] : �߰��� ���� ��Ʈ�� �ڵ�
 @param hBmLayoutMiddleCenter [in] : �߰��� ��� ��Ʈ�� �ڵ�
 @param hBmLayoutMiddleRight [in] : �߰��� ������ ��Ʈ�� �ڵ�
 @param hBmLayoutBottomLeft [in] : �� �Ʒ��� ���� ��Ʈ�� �ڵ�
 @param hBmLayoutBottomCenter [in] : �� �Ʒ��� ��� ��Ʈ�� �ڵ�
 @param hBmLayoutBottomRight [in] : �� �Ʒ��� ������ ��Ʈ�� �ڵ�
 @return ������ atTRUE, ���н� atFALSE
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
 @brief ���̾ƿ� ��Ʈ���� ��´�.
 
 ���̾ƿ� ��Ʈ���� ��´�.

 @param hWnd [in] : Window �ڵ�
 @param lpLayoutBitmap [out] : �����̿� ��Ʈ�� ��Ʈ
 @return ����
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
 @brief ���̾ƿ� ��Ʈ���� �����Ѵ�.
 
 ���̾ƿ� ��Ʈ���� �����Ѵ�.

 @param hWnd [in] : Window �ڵ�
 @param lpLayoutBitmap [out] : �����̿� ��Ʈ�� ��Ʈ
 @return ����
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
 @brief Window�� �޽����� ������.
 
 Window�� �޽����� ������.

 @param hWnd [in] : Window�� �ڵ�
 @param nMsg [in] : ������ �޽���
 @param lParam1, lParam2 [in] : �޽����� �Ķ����
 @return �޽��� �ڵ鷯�� �����
 @remark �޽��� ó���� �ڵ鷯�� �������� Call�� ���� �ﰢ������ �Ͼ��.
*/
int	atWINDOW_SendMessage( atHWINDOW hWnd, int nMsg, long lParam1, long lParam2 )
{
	atLPWINDOW lpWnd = atWINDOW_GetPtr(hWnd);

	if( lpWnd == atNULL ) return -1;

	return atAPP_SendMessage( lpWnd->hApp, hWnd, nMsg, lParam1, lParam2 );
}


/** 
 @brief Window�� �޽����� ������.
 
 Window�� �޽����� ������.

 @param hWnd [in] : Window�� �ڵ�
 @param nMsg [in] : ������ �޽���
 @param lParam1, lParam2 [in] : �޽����� �Ķ����
 @return �޽��� ó�� ��û ���.
 @remark �޽��� ó���� �޽��� Queue�� ���� �Ͼ�� ������, �񵿱������� �Ͼ��.
*/
int	atWINDOW_PostMessage( atHWINDOW hWnd, int nMsg, long lParam1, long lParam2 )
{
	atLPWINDOW lpWnd = atWINDOW_GetPtr(hWnd);
	
	if( lpWnd == atNULL ) return -1;

	return atAPP_PostMessage( lpWnd->hApp, hWnd, nMsg, lParam1, lParam2 );
}


/** 
 @brief Window�� Default �޽��� �ڵ鷯
 
 Window�� Default �޽��� �ڵ鷯

 @param hWnd [in] : Window�� �ڵ�
 @param nMsg [in] : ������ �޽���
 @param lParam1, lParam2 [in] : �޽����� �Ķ����
 @return �޽��� �ڵ鷯�� �����
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
		// Window�� ���� ������, WinObj�� �ڵ鷯 ó���� �ѱ��.
		ret = atWINOBJ_DefObjProc( _TO_WINOBJ(hWnd), nMsg, lParam1, lParam2 );
	}
	
	return ret;
}

