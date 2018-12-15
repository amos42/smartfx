//
// EditBoxWnd.h
//
// Date: 2005-12-08
// Coder: You Keun Sook
//
///////////////////////////////////////////////////////////////////////


#include "A_UIENG.h"
#include "UI_Util.h"
#include "EditBoxWnd.h"
#include "AEECOMP_BasePkg.h"


typedef struct _EDITBOXWNDDATA {
	atHCONTROL	    hEditBox;			// 에디트박스 컴포넌트 핸들
	atHCONTROL      hStatic;
	atHCONTROL      hBtnOK, hBtnCancel;

    atLPTSTR        lpEditText;
	void			*lpUserData;
} EDITBOXWNDDATA,  * LPEDITBOXWNDDATA;


static int	EditBoxWndOnEvent(atHWINDOW hWnd, int nMsg, long nParam1, long nParam2);


atBOOL RegistEditBoxWndClass(void)
{
	atWINCLASS wc;
	
	atUIAPI_memset( &wc, 0, sizeof(atWINCLASS) );
	wc.dwAttr = atWS_FRAMEWINDOW | atWS_MODAL;
 	wc.dwUserDataSize = sizeof(EDITBOXWNDDATA);
	wc.fnWndProc = EditBoxWndOnEvent;
	
	return atAPP_RegistWinClass( atAPPMNG_GetCurApp(), BASEPKG_CUSTWIN_EDITBOX, &wc );
}


atVOID UnregistEditBoxWndClass(void)
{
	atAPP_UnregistWinClass( atAPPMNG_GetCurApp(), BASEPKG_CUSTWIN_EDITBOX );
}


struct _CreateParam {
	atLPTSTR text;
	atLPTSTR msg;
};

atHWINDOW	CreateEditBoxWnd(atHWINDOW hParentWnd, int id, atLPTSTR szTitle, atLPTSTR szMsg, atLPTSTR szText )
{
	atHWINDOW		hWnd;
	struct _CreateParam param;
	atMARGIN anchor;

	param.text = szText;
	param.msg =  szMsg;

	SET_MARGIN( anchor, atANCHOR_IGNORE, atANCHOR_IGNORE, atANCHOR_IGNORE, atANCHOR_IGNORE );
	hWnd	= atAPP_CreateWindowByClass( atAPPMNG_GetCurApp(), hParentWnd, id, szTitle,
								200, 160,
								&anchor,
								BASEPKG_CUSTWIN_EDITBOX, &param );
	atAPP_ShowWindow( atAPPMNG_GetCurApp(), hWnd, atTRUE );

	return hWnd;
}

static void	EditBoxWndOnInit(atHWINDOW hWnd, struct _CreateParam *param)
{
	LPEDITBOXWNDDATA		lpData	= (LPEDITBOXWNDDATA)atWINDOW_GetDataPtr(hWnd);
	static atEDITBOX_INPUTMODE lstInput[3] = {
		atEDITBOX_INPUTMODE_ENG_LOWERCASE_ONLY,
		atEDITBOX_INPUTMODE_NUMERIC_ONLY,
		atEDITBOX_INPUTMODE_ENG_UPPERCASE_ONLY
	};

	lpData->hStatic = BasePkg_CreateStaticBox( hWnd, 102, 10, 10, 50, 50, param->msg );
	atCONTROL_SetAnchor( lpData->hStatic, 10, atANCHOR_IGNORE, 10, 10 );

	lpData->hEditBox = BasePkg_CreateEditBox( hWnd, 103, 10, 60, 50, 22, param->text, lstInput, 3, 0 );
	atCONTROL_SetAnchor( lpData->hEditBox, atANCHOR_IGNORE, atANCHOR_IGNORE, 10, 10 );

	lpData->hBtnOK = BasePkg_CreateButton( hWnd, 104, 20, 100, 60, 18, _AT("OK") );
	lpData->hBtnCancel = BasePkg_CreateButton( hWnd, 105, 110, 100, 60, 18, _AT("Cancel") );

	atWINDOW_SetCurControl( hWnd, lpData->hEditBox );
	
	lpData->lpEditText = param->text;
}

static void	EditBoxWndOnRegister(atHWINDOW hWnd)
{
//	LPEDITBOXWNDDATA		lpData	= (LPEDITBOXWNDDATA)atWINDOW_GetDataPtr(hWnd);
}

static void	EditBoxWndOnRelease(atHWINDOW hWnd)
{
//	LPEDITBOXWNDDATA		lpData	= (LPEDITBOXWNDDATA)atWINDOW_GetDataPtr(hWnd);

}

static int		EditBoxWndOnKeyDown(atHWINDOW hWnd, int nKeyCode)
{
//	LPEDITBOXWNDDATA		lpData	= (LPEDITBOXWNDDATA)atWINDOW_GetDataPtr(hWnd);

	switch (nKeyCode)
	{
	case atVKEY_END:
		atAPP_DestroyWindow( atAPPMNG_GetCurApp(), hWnd );
		break;
	default:
		return 0;
	}

	return 1;
}

static void	EditBoxWndOnPaint(atHWINDOW hWnd)
{
//	LPEDITBOXWNDDATA		lpData	= (LPEDITBOXWNDDATA)atWINDOW_GetDataPtr(hWnd);

}

static void	EditBoxWndOnChange(atHWINDOW hWnd, long nParam1, long nParam2)
{
//	LPEDITBOXWNDDATA		lpData	= (LPEDITBOXWNDDATA)atWINDOW_GetDataPtr(hWnd);

}

static void	EditBoxWndOnCommand(atHWINDOW hWnd, long nParam1, long nParam2)
{
	LPEDITBOXWNDDATA		lpData	= (LPEDITBOXWNDDATA)atWINDOW_GetDataPtr(hWnd);

	if( nParam1 == 103 || nParam1 == 104 ){
		if( lpData->lpEditText ){
			atEDITBOX_GetFullText( lpData->hEditBox, lpData->lpEditText );
		}
		atWINDOW_PostMessage( atWINDOW_GetParentWindow(hWnd), WNDMSG_COMMAND, atWINDOW_GetID(hWnd), (atLONG)lpData->lpEditText );
	} else if( nParam1 == 105 ){
		atWINDOW_PostMessage( atWINDOW_GetParentWindow(hWnd), WNDMSG_COMMAND, atWINDOW_GetID(hWnd), atNULL );
	}
	
	atAPP_DestroyWindow( atAPPMNG_GetCurApp(), hWnd );    
}

static int	EditBoxWndOnEvent(atHWINDOW hWnd, int nMsg, long nParam1, long nParam2)
{
	LPEDITBOXWNDDATA		lpData	= (LPEDITBOXWNDDATA)atWINDOW_GetDataPtr(hWnd);

	switch( nMsg )
	{
		case WNDMSG_INIT:
			EditBoxWndOnInit( hWnd, (void *)nParam1 );
			break;
		case WNDMSG_REGIST:
			EditBoxWndOnRegister( hWnd );
			break;
		case WNDMSG_RELEASE:
			EditBoxWndOnRelease( hWnd );
			break;
		case WNDMSG_PAINT:
			EditBoxWndOnPaint( hWnd );
			break;
		case WNDMSG_KEYDOWN:
			return EditBoxWndOnKeyDown( hWnd, nParam1 );
			break;
		case WNDMSG_CHANGE:
			EditBoxWndOnChange( hWnd, nParam1, nParam2 );
			break;
		case WNDMSG_COMMAND:
			EditBoxWndOnCommand( hWnd, nParam1, nParam2 );
			break;
		default:
			return 0;
			break;
	}

	return 1;
}

