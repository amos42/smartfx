//
// ListBoxWnd.h
//
// Date: 2005-12-08
// Coder: You Keun Sook
//
///////////////////////////////////////////////////////////////////////


#include "A_UIENG.h"
#include "UI_Util.h"
#include "ListBoxWnd.h"
#include "AEECOMP_ExtPkg.h"


#define KTHIM_LINE_INTERVAL 20

typedef struct _LISTBOXWNDDATA {
	atHCONTROL	    hListBox;			// 에디트박스 컴포넌트 핸들

	void			*lpUserData;
} LISTBOXWNDDATA,  * LPLISTBOXWNDDATA;

static int	ListBoxWndOnEvent(atHWINDOW hWnd, int nMsg, long nParam1, long nParam2);


atBOOL      RegistListBoxWndClass(void)
{
	atWINCLASS wc;
	
	atUIAPI_memset( &wc, 0, sizeof(atWINCLASS) );
	wc.dwAttr = atWS_FRAMEWINDOW | atWS_MODAL;
 	wc.dwUserDataSize = sizeof(LISTBOXWNDDATA);
	wc.fnWndProc = ListBoxWndOnEvent;
	
	return atAPP_RegistWinClass( atAPPMNG_GetCurApp(), EXTPKG_CUSTWIN_LISTBOX, &wc );
}


struct _CreateParam {
	void (*add_func)(atHCONTROL,void*,int);
	void *data;
	int max_cnt;
	int sel;
};

atHWINDOW	CreateListBoxWnd_0(atHWINDOW hParentWnd, int id, atLPTSTR szTitle, void (*add_func)(atHCONTROL,void*,int), void *data, int max_cnt, int sel)
{
	atHWINDOW		hWnd;
	struct _CreateParam param;
	int				nLineNum;
	atMARGIN anchor;

	param.add_func = add_func;
	param.data = data;
	param.max_cnt = max_cnt;
	param.sel = sel;

	nLineNum = MIN( max_cnt, 10 );
	if( nLineNum <= 0 ) nLineNum = 5;

	SET_MARGIN( anchor, atANCHOR_IGNORE, atANCHOR_IGNORE, atANCHOR_IGNORE, atANCHOR_IGNORE );
    hWnd = atAPP_CreateWindowByClass( atAPPMNG_GetCurApp(), hParentWnd, id, szTitle, 
								atUIAPI_GetScreenWidth()-20, nLineNum*KTHIM_LINE_INTERVAL+(24+6+2*2),
								&anchor,
								EXTPKG_CUSTWIN_LISTBOX, &param );
	atAPP_ShowWindow( atAPPMNG_GetCurApp(), hWnd, atTRUE );

	return hWnd;
}

static void add_func( atHCONTROL hListBox, void *data, int max_cnt )
{
	int i;
	atTCHAR (*szInputMsg)[32] = (atTCHAR(*)[32])data;

    for( i = 0; i < max_cnt; i ++ ) {
		atLISTBOX_EXT_Start_AddItem(hListBox);
		atLISTBOX_EXT_AddItem_NO(hListBox, i+1);
		atLISTBOX_EXT_AddItem_String(hListBox, szInputMsg[i], 0 );
		atLISTBOX_EXT_Update_AddItem(hListBox);
    }	
}

atHWINDOW	CreateListBoxWnd(atHWINDOW hParentWnd, int id, atLPTSTR szTitle, atTCHAR (*szInputMsg)[32], int max_cnt, int sel)
{
	return CreateListBoxWnd_0( hParentWnd, id, szTitle, add_func, (void *)szInputMsg, max_cnt, sel );
}


static void	ListBoxWndOnInit( atHWINDOW hWnd, struct _CreateParam *lpParam )
{
	LPLISTBOXWNDDATA		lpData	= (LPLISTBOXWNDDATA)atWINDOW_GetDataPtr(hWnd);

	atUIAPI_memset(lpData, 0, sizeof(LISTBOXWNDDATA));

	lpData->hListBox = ExtPkg_CreateListBox( hWnd, 1, 2, 2, -2, -2, KTHIM_LINE_INTERVAL, atFALSE, atFALSE, atFALSE );
	
	if( lpParam->add_func ) lpParam->add_func( lpData->hListBox, lpParam->data, lpParam->max_cnt );

	if( (lpParam->sel > 0) && (lpParam->sel < lpParam->max_cnt) ){
		atLISTBOX_EXT_SetSelectIndex( lpData->hListBox, lpParam->sel );
	}	
}

static void	ListBoxWndOnRelease(atHWINDOW hWnd)
{
//	LPLISTBOXWNDDATA		lpData	= (LPLISTBOXWNDDATA)atWINDOW_GetDataPtr(hWnd);
}

static int		ListBoxWndOnKeyDown(atHWINDOW hWnd, int nKeyCode)
{
//	LPLISTBOXWNDDATA		lpData	= (LPLISTBOXWNDDATA)atWINDOW_GetDataPtr(hWnd);

	switch (nKeyCode)
	{
	case atVKEY_CLEAR:
	case atVKEY_MENU:
	case atVKEY_EXIT:
		atAPP_DestroyWindow( atAPPMNG_GetCurApp(), hWnd );
		break;
	default:
		return 0;
	}

	return 1;
}

static void	ListBoxWndOnPaint(atHWINDOW hWnd)
{
//	LPLISTBOXWNDDATA		lpData	= (LPLISTBOXWNDDATA)atWINDOW_GetDataPtr(hWnd);

}

static int	ListBoxWndOnEvent(atHWINDOW hWnd, int nMsg, long nParam1, long nParam2)
{
	LPLISTBOXWNDDATA		lpData	= (LPLISTBOXWNDDATA)atWINDOW_GetDataPtr(hWnd);

	switch( nMsg )
	{
		case WNDMSG_INIT:
			ListBoxWndOnInit( hWnd, (void*)nParam1 );
			break;
		case WNDMSG_RELEASE:
			ListBoxWndOnRelease( hWnd );
			break;
		case WNDMSG_PAINT:
			ListBoxWndOnPaint( hWnd );
			break;
		case WNDMSG_KEYDOWN:
			return ListBoxWndOnKeyDown( hWnd, nParam1 );
			break;
		case WNDMSG_COMMAND:
			atWINDOW_PostMessage( atWINDOW_GetParentWindow(hWnd), nMsg, atWINDOW_GetID(hWnd), nParam2 );
			atAPP_DestroyWindow( atAPPMNG_GetCurApp(), hWnd );
			break;
		default:
			return 0;
			break;
	}

	return 1;
}

