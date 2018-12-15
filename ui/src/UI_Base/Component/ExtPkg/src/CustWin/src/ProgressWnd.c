//
// ProgressWnd.h
//
// Date: 2005-12-08
// Coder: You Keun Sook
//
///////////////////////////////////////////////////////////////////////


#include "A_UIENG.h"
#include "UI_Util.h"
#include "ProgressWnd.h"
#include "AEECOMP_ExtPkg.h"
#include "AEECOMP_BasePkg.h"


typedef struct _PROGRESSRWNDDATA {
	atHCONTROL	    hProgress;			// 에디트박스 컴포넌트 핸들
	atHCONTROL      hStatic;
	atHCONTROL      hButton;

	void			*lpUserData;
} PROGRESSRWNDDATA,  * LPPROGRESSRWNDDATA;


static int	ProgressWndOnEvent(atHWINDOW hWnd, int nMsg, long nParam1, long nParam2);


atBOOL      RegistProgressWndClass(void)
{
	atWINCLASS wc;
	
	atUIAPI_memset( &wc, 0, sizeof(atWINCLASS) );
	wc.dwAttr = atWS_FRAMEWINDOW | atWS_MODAL;
 	wc.dwUserDataSize = sizeof(PROGRESSRWNDDATA);
	wc.fnWndProc = ProgressWndOnEvent;
	
	return atAPP_RegistWinClass( atAPPMNG_GetCurApp(), EXTPKG_CUSTWIN_PROGRESS, &wc );
}


struct _CreateParam {
	int size;
	int pos;
	atLPTSTR text;
	atLPTSTR button_caption;
};

atHWINDOW	CreateProgressWnd(atHWINDOW hParentWnd, int id, atLPTSTR szTitle, int size, int pos, atLPTSTR text, atLPTSTR button_caption)
{
	atHWINDOW		hWnd;
	struct _CreateParam param;
	atMARGIN anchor;

	param.size = size;
	param.pos = pos;
	param.text = text;
	param.button_caption = button_caption;
	
	SET_MARGIN( anchor, atANCHOR_IGNORE, atANCHOR_IGNORE, atANCHOR_IGNORE, atANCHOR_IGNORE );
	hWnd	= atAPP_CreateWindowByClass( atAPPMNG_GetCurApp(), hParentWnd, id, szTitle,
								200, 130,
								&anchor,
								EXTPKG_CUSTWIN_PROGRESS, &param );
	atAPP_ShowWindow( atAPPMNG_GetCurApp(), hWnd, atTRUE );
	
	return hWnd;
}

void ChangeProgressWnd( atHWINDOW hWnd, int pos, atLPTSTR text )
{
	LPPROGRESSRWNDDATA		lpData	= (LPPROGRESSRWNDDATA)atWINDOW_GetDataPtr(hWnd);

	atPROGRESSBAR_EXT_SetPosition( lpData->hProgress, pos );
	atSTATICBOX_SetText( lpData->hStatic, text );
}


static void	ProgressWndOnInit(atHWINDOW hWnd, struct _CreateParam *param)
{
	LPPROGRESSRWNDDATA		lpData	= (LPPROGRESSRWNDDATA)atWINDOW_GetDataPtr(hWnd);

	lpData->hProgress = ExtPkg_CreateProgressBar( hWnd, 102, 10, 10, -10, -0, param->size, atTRUE );
	atPROGRESSBAR_EXT_SetPosition( lpData->hProgress, param->pos );

	lpData->hStatic = BasePkg_CreateStaticBox( hWnd, 102, 10, 40, -10, 20, param->text );
	atSTATICBOX_SetAttr( lpData->hStatic, atSTATICBOXATTR_OPAQUE | (ALIGN_CENTER_MIDDLE & atSTATICBOXATTR_ALIGN_MASK) );

	lpData->hButton = ExtPkg_CreateButton( hWnd, 101, 60, 70, 60, 20, param->button_caption );
}

static void	ProgressWndOnRegister(atHWINDOW hWnd)
{
//	LPPROGRESSRWNDDATA		lpData	= (LPPROGRESSRWNDDATA)atWINDOW_GetDataPtr(hWnd);
}

static void	ProgressWndOnRelease(atHWINDOW hWnd)
{
//	LPPROGRESSRWNDDATA		lpData	= (LPPROGRESSRWNDDATA)atWINDOW_GetDataPtr(hWnd);

}

static int		ProgressWndOnKeyDown(atHWINDOW hWnd, int nKeyCode)
{
//	LPPROGRESSRWNDDATA		lpData	= (LPPROGRESSRWNDDATA)atWINDOW_GetDataPtr(hWnd);

	switch (nKeyCode)
	{
	case atVKEY_CLEAR:
	case atVKEY_MENU:
		atAPP_DestroyWindow( atAPPMNG_GetCurApp(), hWnd );
		break;
	default:
		return 0;
	}

	return 1;
}

static void	ProgressWndOnPaint(atHWINDOW hWnd)
{
//	LPPROGRESSRWNDDATA		lpData	= (LPPROGRESSRWNDDATA)atWINDOW_GetDataPtr(hWnd);

}

static void	ProgressWndOnChange(atHWINDOW hWnd, long nParam1, long nParam2)
{
//	LPPROGRESSRWNDDATA		lpData	= (LPPROGRESSRWNDDATA)atWINDOW_GetDataPtr(hWnd);

}

static int	ProgressWndOnEvent(atHWINDOW hWnd, int nMsg, long nParam1, long nParam2)
{
	LPPROGRESSRWNDDATA		lpData	= (LPPROGRESSRWNDDATA)atWINDOW_GetDataPtr(hWnd);

	switch( nMsg )
	{
		case WNDMSG_INIT:
			ProgressWndOnInit( hWnd, (void *)nParam1 );
			break;
		case WNDMSG_REGIST:
			ProgressWndOnRegister( hWnd );
			break;
		case WNDMSG_RELEASE:
			ProgressWndOnRelease( hWnd );
			break;
		case WNDMSG_PAINT:
			ProgressWndOnPaint( hWnd );
			break;
		case WNDMSG_KEYDOWN:
			return ProgressWndOnKeyDown( hWnd, nParam1 );
			break;
		case WNDMSG_CHANGE:
			ProgressWndOnChange( hWnd, nParam1, nParam2 );
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

