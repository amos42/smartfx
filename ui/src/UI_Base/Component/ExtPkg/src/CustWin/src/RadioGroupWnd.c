//
// RadioGroupWnd.h
//
// Date: 2005-12-08
// Coder: You Keun Sook
//
///////////////////////////////////////////////////////////////////////

#include "A_UIENG.h"
#include "UI_Util.h"
#include "RadioGroupWnd.h"
#include "AEECOMP_ExtPkg.h"


#define KTHIM_LINE_INTERVAL 20

typedef struct _RADIOGROUPWNDDATA {
	atHCONTROL	    hRadioGroup;			// 에디트박스 컴포넌트 핸들
} RADIOGROUPWNDDATA,  * LPRADIOGROUPWNDDATA;

static int	RadioGroupWndOnEvent(atHWINDOW hWnd, int nMsg, long nParam1, long nParam2);


atBOOL      RegistRadioGroupWndClass(void)
{
	atWINCLASS wc;
	
	atUIAPI_memset( &wc, 0, sizeof(atWINCLASS) );
	wc.dwAttr = atWS_FRAMEWINDOW | atWS_MODAL;
 	wc.dwUserDataSize = sizeof(RADIOGROUPWNDDATA);
	wc.fnWndProc = RadioGroupWndOnEvent;
	
	return atAPP_RegistWinClass( atAPPMNG_GetCurApp(), EXTPKG_CUSTWIN_RADIOGROUP, &wc );
}


struct _CreateParam {
	void (*add_func)(atHCONTROL,void*,int);
	void *data;
	int max_cnt;
	int init_value;
};

atHWINDOW	CreateRadioGroupWnd_0(atHWINDOW hParentWnd, int id, atLPTSTR szTitle,
										void (*add_func)(atHCONTROL,void *,int), void *data, int max_cnt, int init_value)
{
	atHWINDOW		hWnd;
	struct _CreateParam param;
	int				nLineNum;
	atMARGIN anchor;

	nLineNum = MIN( max_cnt, 10 );
	if( nLineNum <= 0 ) nLineNum = 5;
	
//	hMsgWnd	= PreCreateMyPopupWindow(hParentWnd, szTitle,
//								atUIAPI_GetScreenWidth()/2 - (atUIAPI_GetScreenWidth()-26)/2, atUIAPI_GetScreenHeight()/2 - (nLineNum*KTHIM_LINE_INTERVAL+(24+6+2*2))/2,
//								atUIAPI_GetScreenWidth()-26, nLineNum*KTHIM_LINE_INTERVAL+(24+6+2*2),
//								sizeof(RADIOGROUPWNDDATA),
//								RadioGroupWndOnEvent, 0, atNULL);
	param.add_func = add_func;
	param.data = data;
	param.max_cnt = max_cnt;
	param.init_value = init_value;

	SET_MARGIN( anchor, atANCHOR_IGNORE, atANCHOR_IGNORE, atANCHOR_IGNORE, atANCHOR_IGNORE );
    hWnd = atAPP_CreateWindowByClass( atAPPMNG_GetCurApp(), hParentWnd, id, szTitle, 
								atUIAPI_GetScreenWidth()-26, nLineNum*KTHIM_LINE_INTERVAL+(24+6+2*2),
								&anchor,
								EXTPKG_CUSTWIN_RADIOGROUP, &param );
	atAPP_ShowWindow( atAPPMNG_GetCurApp(), hWnd, atTRUE );

	return hWnd;
}


static void add_func( atHCONTROL hRadioGroup, void *data, int max_cnt )
{
	int i;
	atTCHAR (*szInputMsg)[32] = (atTCHAR (*)[32])data;

    for( i = 0; i < max_cnt; i ++ ){
    	atRADIOGROUP_EXT_AddItem( hRadioGroup, szInputMsg[i] );
	}	
}

atHWINDOW	CreateRadioGroupWnd(atHWINDOW hParentWnd, int id, atLPTSTR szTitle,
										atTCHAR (*szInputMsg)[32], int max_cnt, int init_value)
{
	return CreateRadioGroupWnd_0( hParentWnd, id, szTitle, add_func, szInputMsg, max_cnt, init_value );
}


static void	RadioGroupWndOnInit( atHWINDOW hWnd, struct _CreateParam *lpParam )
{
	LPRADIOGROUPWNDDATA		lpData	= (LPRADIOGROUPWNDDATA)atWINDOW_GetDataPtr(hWnd);

	atUIAPI_memset(lpData, 0, sizeof(RADIOGROUPWNDDATA));

	lpData->hRadioGroup = ExtPkg_CreateRadioGroup( hWnd, 1, 2, 2, 20, 20, 
											atNULL, 0, 
											(int)0, KTHIM_LINE_INTERVAL);
	atCONTROL_SetAnchor( lpData->hRadioGroup, 2, 2, 2, 2 );
	
	if( lpParam->add_func ) lpParam->add_func( lpData->hRadioGroup, lpParam->data, lpParam->max_cnt );
	atRADIOGROUP_EXT_SetSelectIndex( lpData->hRadioGroup, lpParam->init_value );
}

static void	RadioGroupWndOnRelease(atHWINDOW hWnd)
{
//	LPRADIOGROUPWNDDATA		lpData	= (LPRADIOGROUPWNDDATA)atWINDOW_GetDataPtr(hWnd);

}

static int		RadioGroupWndOnKeyDown(atHWINDOW hWnd, int nKeyCode)
{
	int r = 0;

	switch (nKeyCode)
	{
	case atVKEY_CLEAR:
		atAPP_DestroyWindow( atAPPMNG_GetCurApp(), hWnd );
		r = 1;
		break;
	}

	return r;
}

static void	RadioGroupWndOnPaint(atHWINDOW hWnd)
{
}

static int	RadioGroupWndOnEvent(atHWINDOW hWnd, int nMsg, long nParam1, long nParam2)
{
	LPRADIOGROUPWNDDATA		lpData	= (LPRADIOGROUPWNDDATA)atWINDOW_GetDataPtr(hWnd);
	int r = 0;

	switch( nMsg )
	{
		case WNDMSG_INIT:
			RadioGroupWndOnInit( hWnd, (void *)nParam1 );
			r = 1;
			break;
		case WNDMSG_RELEASE:
			RadioGroupWndOnRelease( hWnd );
			r = 1;
			break;
		case WNDMSG_PAINT:
			RadioGroupWndOnPaint( hWnd );
			r = 1;
			break;
		case WNDMSG_KEYDOWN:
			r =  RadioGroupWndOnKeyDown( hWnd, nParam1 );
			break;
		case WNDMSG_COMMAND:
			if( nParam1 == 1 )
			{
				if( atWINDOW_GetID(hWnd) != 0 ) atWINDOW_PostMessage( atWINDOW_GetParentWindow(hWnd), WNDMSG_COMMAND, atWINDOW_GetID(hWnd), nParam2 );
				atAPP_DestroyWindow( atAPPMNG_GetCurApp(), hWnd );
				r = 1;
			}
			break;
		default:
			break;
	}

	return r;
}

