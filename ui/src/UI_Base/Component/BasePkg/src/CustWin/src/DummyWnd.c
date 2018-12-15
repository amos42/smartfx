// DummyWnd.c
// Custom Window 개발시 참고용


#include "A_UIENG.h"
#include "DummyWnd.h"


typedef struct _tagDUMMYWNDDATA {
	int		dummy;
} DUMMYWNDDATA, * LPDUMMYWNDDATA;

static int DummyWndOnEvent(atHWINDOW hWnd, int nMsg, long nParam1, long nParam2);


atBOOL      RegistDummyWndClass(void)
{
	atWINCLASS wc;
	
	atUIAPI_memset( &wc, 0, sizeof(atWINCLASS) );
	wc.dwAttr = atWS_FRAMEWINDOW;
 	wc.dwUserDataSize = sizeof(DUMMYWNDDATA);
	wc.fnWndProc = DummyWndOnEvent;
	
	return atAPP_RegistWinClass( atAPPMNG_GetCurApp(), BASEPKG_CUSTWIN_DUMMY, &wc );
}

void	UnregistDummyWndClass(void)
{
	atAPP_UnregistWinClass( atAPPMNG_GetCurApp(), BASEPKG_CUSTWIN_DUMMY );
}


struct _CreateParam {
	int dummy;
};


atHWINDOW	CreateDummyWnd( atHWINDOW hParentWnd )
{
	struct _CreateParam param;
	atHWINDOW		hWnd;
	atMARGIN anchor;

	param.dummy = 0;
	
	SET_MARGIN( anchor, 0, 0, 0, 0 );
	hWnd	= atAPP_CreateWindowByClass( atAPPMNG_GetCurApp(), hParentWnd, 0, _AT("Dummy"),
								0, 0, &anchor,
								BASEPKG_CUSTWIN_DUMMY, (void *)&param );
	atAPP_ShowWindow( atAPPMNG_GetCurApp(), hWnd, atTRUE );

	return hWnd;
}


static void DummyWndOnInit(atHWINDOW hWnd, struct _CreateParam *param)
{
	LPDUMMYWNDDATA lpData = (LPDUMMYWNDDATA)atWINDOW_GetDataPtr(hWnd);
	//
}

static void DummyWndOnRelease(atHWINDOW hWnd)
{
	LPDUMMYWNDDATA lpData = (LPDUMMYWNDDATA)atWINDOW_GetDataPtr(hWnd);
	//
}

static int DummyWndOnKeyDown(atHWINDOW hWnd, int nKeyCode)
{
	switch(nKeyCode)
	{
		case atVKEY_SOFT1 :
			break;
		case atVKEY_SOFT2 :
			break;
		case atVKEY_SELECT:			
			break;
		case atVKEY_CLEAR:			
		case atVKEY_EXIT:
			atAPP_DestroyWindow( atAPPMNG_GetCurApp(), hWnd );
			break;
		default:
			break;
	}

	return 0;
}

static int DummyWndOnKeyUp(atHWINDOW hWnd, int nKeyCode)
{
	return 0;
}

static void DummyWndOnPaint(atHWINDOW hWnd)
{
	//atHGDC gc = atWINDOW_GetClientGDC(hWnd);
	//
}

static void DummyWndOnPause(atHWINDOW hWnd)
{
}

static void DummyWndOnResume(atHWINDOW hWnd)
{
}

static int DummyWndOnEvent(atHWINDOW hWnd, int nMsg, long nParam1, long nParam2)
{
	switch( nMsg )
	{
		case WNDMSG_INIT:
			DummyWndOnInit( hWnd, (void *)nParam1 );
			break;
		case WNDMSG_RELEASE:
			DummyWndOnRelease( hWnd );
			break;
		case WNDMSG_PAINT:
			DummyWndOnPaint( hWnd );
			break;
		case WNDMSG_PAUSE:
			DummyWndOnPause( hWnd );
			break;
		case WNDMSG_RESUME:
			DummyWndOnResume( hWnd );
			break;
		case WNDMSG_KEYDOWN:
			DummyWndOnKeyDown( hWnd, nParam1 );
			break;
		case WNDMSG_KEYUP:
			DummyWndOnKeyUp( hWnd, nParam1 );
			break;
		default:
			break;
	}

	return 0;
}


