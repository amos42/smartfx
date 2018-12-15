//
// TextViewWnd.h
//
// Date: 2005-12-08
// Coder: You Keun Sook
//
///////////////////////////////////////////////////////////////////////


#include "A_UIENG.h"
#include "UI_Util.h"
#include "TextViewWnd.h"
#include "AEECOMP_ExtPkg.h"
#include <string.h>


typedef struct _TEXTVIEWWNDDATA {
	atHCONTROL	    hTextView;			// 에디트박스 컴포넌트 핸들
	atHCONTROL      hButton;
} TEXTVIEWWNDDATA,  * LPTEXTVIEWWNDDATA;

static int	TextViewWndOnEvent(atHWINDOW hWnd, int nMsg, long nParam1, long nParam2);


atBOOL      RegistTextViewWndClass(void)
{
	atWINCLASS wc;
	
	atUIAPI_memset( &wc, 0, sizeof(atWINCLASS) );
	wc.dwAttr = atWS_FRAMEWINDOW | atWS_MODAL;
 	wc.dwUserDataSize = sizeof(TEXTVIEWWNDDATA);
	wc.fnWndProc = TextViewWndOnEvent;
	
	return atAPP_RegistWinClass( atAPPMNG_GetCurApp(), EXTPKG_CUSTWIN_TEXTVIEW, &wc );
}


struct _CreateParam {
	atLPTSTR szText;
};

atHWINDOW	CreateTextViewWnd(atHWINDOW hParentWnd, int id, atLPTSTR szTitle, atLPTSTR szText )
{
	atHWINDOW		hWnd;
	struct _CreateParam param;
	int				nWidth, nHeight;
	atMARGIN anchor;

	nWidth = atUIAPI_GetScreenWidth()-10;
	nHeight = 200;
	
	param.szText		= szText;

//	hMsgWnd	= PreCreateMyPopupWindow(hParentWnd, szTitle,
//								(atUIAPI_GetScreenWidth() - 300)/2, (atUIAPI_GetScreenHeight()-200)/2, 
//								300, 200,
//								sizeof(TEXTVIEWWNDDATA),
//								TextViewWndOnEvent, 0, atNULL);

    SET_MARGIN( anchor, 10, 10, 10, 10 );
    hWnd = atAPP_CreateWindowByClass( atAPPMNG_GetCurApp(), hParentWnd, id, szTitle, 
								nWidth, nHeight,
								&anchor,
								EXTPKG_CUSTWIN_TEXTVIEW, &param );
	atAPP_ShowWindow( atAPPMNG_GetCurApp(), hWnd, atTRUE );

	return hWnd;
}


static void	TextViewWndOnInit( atHWINDOW hWnd, struct _CreateParam *lpParam )
{
	LPTEXTVIEWWNDDATA		lpData	= (LPTEXTVIEWWNDDATA)atWINDOW_GetDataPtr(hWnd);
	atRECT rect;

	atUIAPI_memset(lpData, 0, sizeof(TEXTVIEWWNDDATA));

	lpData->hTextView = ExtPkg_CreateScrollText( hWnd, 1, 4, 4, 80, 80, 0 );
	atCONTROL_SetAnchor( lpData->hTextView, 4, 24, 4, 4 );
	atSCROLLTEXT_SetText( lpData->hTextView, lpParam->szText );

	atWINDOW_GetClientRect(hWnd, &rect);
	lpData->hButton = ExtPkg_CreateButton( hWnd, 103, (RECT_WIDTH(rect)-50)/2, 149, 50, 18, _AT("OK") );
	atCONTROL_SetAnchor( lpData->hButton, atANCHOR_IGNORE, 4, atANCHOR_IGNORE, atANCHOR_IGNORE );
}

static void	TextViewWndOnRelease(atHWINDOW hWnd)
{
	LPTEXTVIEWWNDDATA		lpData	= (LPTEXTVIEWWNDDATA)atWINDOW_GetDataPtr(hWnd);

	if( atWINDOW_GetID(hWnd) ){
		atWINDOW_PostMessage( atWINDOW_GetParentWindow(hWnd), WNDMSG_COMMAND, atWINDOW_GetID(hWnd), 0 );
	}
}

static int		TextViewWndOnCommand(atHWINDOW hWnd, int nParam1, int nParam2 )
{
	LPTEXTVIEWWNDDATA		lpData	= (LPTEXTVIEWWNDDATA)atWINDOW_GetDataPtr(hWnd);
	int r = 0;

	switch( nParam1 ){
		case 1:
			if( atWINDOW_GetID(hWnd) != 0 ) atWINDOW_PostMessage( atWINDOW_GetParentWindow(hWnd), WNDMSG_COMMAND, atWINDOW_GetID(hWnd), nParam2 );
			atAPP_DestroyWindow( atAPPMNG_GetCurApp(), hWnd );
			r = 1;
			break;
		case 103:
			atAPP_DestroyWindow( atAPPMNG_GetCurApp(), hWnd );
			r = 1;
			break;
	}

	return r;
}

static int		TextViewWndOnKeyDown(atHWINDOW hWnd, int nKeyCode)
{
	int r = 0;

	switch (nKeyCode)
	{
    	case atVKEY_SELECT:
    	case atVKEY_CLEAR:
		case atVKEY_MENU:
    		atAPP_DestroyWindow( atAPPMNG_GetCurApp(), hWnd );
    		r = 1;
    		break;
	}

	return r;
}

static void	TextViewWndOnPaint(atHWINDOW hWnd)
{
}

static int	TextViewWndOnEvent(atHWINDOW hWnd, int nMsg, long nParam1, long nParam2)
{
	LPTEXTVIEWWNDDATA		lpData	= (LPTEXTVIEWWNDDATA)atWINDOW_GetDataPtr(hWnd);
	int r = 0;

	switch( nMsg )
	{
		case WNDMSG_INIT:
			TextViewWndOnInit( hWnd, (void *)nParam1 );
			r = 1;
			break;
		case WNDMSG_RELEASE:
			TextViewWndOnRelease( hWnd );
			r = 1;
			break;
		case WNDMSG_COMMAND:
			TextViewWndOnCommand( hWnd, nParam1, nParam2 );
			r = 1;
			break;
		case WNDMSG_PAINT:
			TextViewWndOnPaint( hWnd );
			r = 1;
			break;
		case WNDMSG_KEYDOWN:
			r =  TextViewWndOnKeyDown( hWnd, nParam1 );
			break;
		default:
			break;
	}

	return r;
}

