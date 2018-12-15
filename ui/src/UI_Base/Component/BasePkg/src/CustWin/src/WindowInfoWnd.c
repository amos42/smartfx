//
// WindowInfoWnd.c
//
///////////////////////////////////////////////////////////////////////

#include "A_UIENG.h"
#include "UI_API/UI_API.h"
#include "AEECOMP_BasePkg.h"
#include "WindowInfoWnd.h"
#include <stdio.h>


/*
typedef struct _WINDOWINFOWNDDATA {

} WINDOWINFOWNDDATA, * LPWINDOWINFOWNDDATA;
*/

static int WindowInfoWndOnEvent(atHWINDOW hWnd, int nMsg, long nParam1, long nParam2);


atBOOL      RegistWindowInfoWndClass(void)
{
	atWINCLASS wc;
	
	atUIAPI_memset( &wc, 0, sizeof(atWINCLASS) );
	wc.dwAttr = atWS_FRAMEWINDOW | atWS_STAYONTOP | atWS_MODAL;
 	wc.dwUserDataSize = 0/*sizeof(WINDOWINFOWNDDATA)*/;
	wc.fnWndProc = WindowInfoWndOnEvent;
	
	return atAPP_RegistWinClass( atAPPMNG_GetCurApp(), BASEPKG_CUSTWIN_WINDOWINFO, &wc );
}

atVOID      UnregistWindowInfoWndClass(void)
{
	atAPP_UnregistWinClass( atAPPMNG_GetCurApp(), BASEPKG_CUSTWIN_WINDOWINFO );
}


atHWINDOW	CreateWindowInfoWnd(atHWINDOW hParentWnd)
{
	atHWINDOW		hWnd;
	atMARGIN rtAnchor;

	SET_MARGIN( rtAnchor, 0, 0, 0, 0 );
	hWnd	= atAPP_CreateWindowByClass( atAPPMNG_GetCurApp(), hParentWnd, 0x01, _AT("Window Info"),
								0, 0, &rtAnchor,
								BASEPKG_CUSTWIN_WINDOWINFO, atNULL );
	atWINDOW_SetAnchor( hWnd, 0, 0, 0, 0 );
	atAPP_ShowWindow( atAPPMNG_GetCurApp(), hWnd, atTRUE );
	
	return hWnd;
}

static void WindowInfoWndOnInit(atHWINDOW hWnd)
{
//	LPWINDOWINFOWNDDATA lpData = (LPWINDOWINFOWNDDATA)atWINDOW_GetDataPtr(LPWINDOWINFOWNDDATA, hWnd);
	atHCONTROL hListBox, hButton;
	int i, cnt;
	atHWINDOW hw;
	atLPWINDOW lpw;
	atTCHAR buf[128];
	atHFONT hFont;
	int font_height;

	hFont = atUIAPI_GetStockFont(0);
	font_height = atUIAPI_GetFontHeight(hFont);

	hListBox = atLISTBOX_Create( 1, 1, 1, 80, 80, font_height+2, atTRUE, atFALSE );
	atCONTROL_SetAnchor( hListBox, 1, font_height+8, 1, 1 );
   	atWINDOW_AddControl( hWnd, hListBox );

    cnt = atAPP_GetWindowCount( atAPPMNG_GetCurApp() );
    for( i = 0; i < cnt; i ++ ){
        hw = atAPP_GetWindowByIndex( atAPPMNG_GetCurApp(), i );
	    lpw = atWINDOW_GetPtr(hw);
		if( lpw == atNULL ) continue;
        
        atLISTBOX_Start_AddItem(hListBox);
        atLISTBOX_AddItem_NO(hListBox, i+1);
		atUIAPI_sprintf( buf, _AT("%s (Data:%d)"), atWINDOW_GetName(hw), lpw->Base.nObjDataSize );
        atLISTBOX_AddItem_String(hListBox, buf, 0);
        atLISTBOX_Update_AddItem(hListBox);
    }

	hButton = atBUTTON_Create( 0x1000, 50, 100, 120, font_height+2, _AT("OK") );
	atCONTROL_SetAnchor( hButton, atANCHOR_IGNORE, 4, atANCHOR_IGNORE, atANCHOR_IGNORE );
   	atWINDOW_AddControl( hWnd, hButton );
    	
//	SetSoftKey(hWnd, SOFTKEY_NULL, SOFTKEY_CLOSE, SOFTKEY_NULL);
}

static void WindowInfoWndOnRelease(atHWINDOW hWnd)
{
//	LPWINDOWINFOWNDDATA lpData = (LPWINDOWINFOWNDDATA)atWINDOW_GetDataPtr(LPWINDOWINFOWNDDATA, hWnd);
}

static int WindowInfoWndOnKeyDown(atHWINDOW hWnd, int nKeyCode)
{
	switch(nKeyCode)
	{
		case atVKEY_SELECT:			
		case atVKEY_MENU:			
		case atVKEY_CLEAR:			
			atAPP_DestroyWindow( atAPPMNG_GetCurApp(), hWnd );		
			break;
		default:
			return 0;
	}

	return 1;
}

static int WindowInfoWndOnCommand(atHWINDOW hWnd, int nParam1, int nParam2)
{
	switch(nParam1)
	{
	case 0x1000:			
		atAPP_DestroyWindow( atAPPMNG_GetCurApp(), hWnd );		
		return 1;
		break;
	}
	
	return 0;
}

static void WindowInfoWndOnPaint(atHWINDOW hWnd)
{
//	LPWINDOWINFOWNDDATA lpData = (LPWINDOWINFOWNDDATA)atWINDOW_GetDataPtr(hWnd);
//	atHGDC hCanvas = atWINDOW_GetClientGDC(hWnd);
}

static int WindowInfoWndOnEvent(atHWINDOW hWnd, int nMsg, long nParam1, long nParam2)
{
	switch( nMsg )
	{
		case WNDMSG_INIT:
			WindowInfoWndOnInit( hWnd );
			break;
		case WNDMSG_RELEASE:
			WindowInfoWndOnRelease( hWnd );
			break;
		case WNDMSG_PAINT:
			WindowInfoWndOnPaint( hWnd );
			break;
		case WNDMSG_KEYDOWN:
			return WindowInfoWndOnKeyDown( hWnd, nParam1 );
			break;
		case WNDMSG_COMMAND:
			return WindowInfoWndOnCommand( hWnd, nParam1, nParam2 );
			break;
		default:
			return 0;
			break;
	}

	return 1;
}

