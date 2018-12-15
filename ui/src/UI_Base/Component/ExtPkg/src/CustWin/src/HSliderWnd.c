//
// HSliderWnd.h
//
// Date: 2005-12-08
// Coder: You Keun Sook
//
///////////////////////////////////////////////////////////////////////


#include "A_UIENG.h"
#include "UI_Util.h"
#include "HSliderWnd.h"
#include "AEECOMP_ExtPkg.h"
#include "AEECOMP_BasePkg.h"


typedef struct _HSLIDERWNDDATA {
	atHCONTROL	    hHSlider;			// 에디트박스 컴포넌트 핸들
	atHCONTROL      hStatic;
	atHCONTROL		hButton;

	void			*lpUserData;
} HSLIDERWNDDATA,  * LPHSLIDERWNDDATA;


static int	HSliderWndOnEvent(atHWINDOW hWnd, int nMsg, long nParam1, long nParam2);


atBOOL      RegistHSliderWndClass(void)
{
	atWINCLASS wc;
	
	atUIAPI_memset( &wc, 0, sizeof(atWINCLASS) );
	wc.dwAttr = atWS_FRAMEWINDOW | atWS_MODAL;
 	wc.dwUserDataSize = sizeof(HSLIDERWNDDATA);
	wc.fnWndProc = HSliderWndOnEvent;
	
	return atAPP_RegistWinClass( atAPPMNG_GetCurApp(), EXTPKG_CUSTWIN_HSLIDER, &wc );
}


struct _CreateParam {
	void *data;
	int size;
	int pos;
};

atHWINDOW	CreateHSliderWnd(atHWINDOW hParentWnd, int id, atLPTSTR szTitle, void *data, int size, int pos)
{
	atHWINDOW		hWnd;
	struct _CreateParam param;
	atMARGIN anchor;

	param.size = size;
	param.pos = pos;
	param.data = data;

	SET_MARGIN( anchor, atANCHOR_IGNORE, atANCHOR_IGNORE, atANCHOR_IGNORE, atANCHOR_IGNORE );
    hWnd = atAPP_CreateWindowByClass( atAPPMNG_GetCurApp(), hParentWnd, id, szTitle, 
								200, 130,
								&anchor,
								EXTPKG_CUSTWIN_HSLIDER, &param );
	atWINDOW_SetAnchor( hWnd, atANCHOR_IGNORE, atANCHOR_IGNORE, 20, 20 );
	atAPP_ShowWindow( atAPPMNG_GetCurApp(), hWnd, atTRUE );

//    atWINDOW_SetLayoutBitmap2( hWnd, &g_Images.lpPopupLayout[type] );
//    atWINDOW_SetLayoutMargin2( hWnd, &margin );

/*
	hMsgWnd	= PreCreateMyPopupWindow(hParentWnd, szTitle,
								atUIAPI_GetScreenWidth()/2 - 200/2, atUIAPI_GetScreenHeight()/2 - 130/2, 
								200, 130,
								sizeof(HSLIDERWNDDATA),
								HSliderWndOnEvent, 0, atNULL);
*/


//	atAPP_AddWindow( atAPPMNG_GetCurApp(), hMsgWnd );
	
	return hWnd;
}


static void	HSliderWndOnInit( atHWINDOW hWnd, struct _CreateParam *lpParam )
{
	LPHSLIDERWNDDATA		lpData	= (LPHSLIDERWNDDATA)atWINDOW_GetDataPtr(hWnd);
	atTCHAR str[50];

	atUIAPI_memset(lpData, 0, sizeof(HSLIDERWNDDATA));

	lpData->hHSlider = ExtPkg_CreateHSlider( hWnd, 101, 10, 10, 170, 20, lpParam->size, lpParam->pos );
	atCONTROL_SetAnchor( lpData->hHSlider, 10, atANCHOR_IGNORE, 10, 10 );

	UI_SPRINTF( str, _AT("%d/%d"), lpParam->pos, lpParam->size );
	lpData->hStatic = BasePkg_CreateStaticBox( hWnd, 102, 70, 35, 50, 20, str );
	atSTATICBOX_SetAttr( lpData->hStatic, atSTATICBOXATTR_OPAQUE | ALIGN_CENTER_BOTTOM );
	atCONTROL_SetAnchor( lpData->hStatic, 35, atANCHOR_IGNORE, atANCHOR_IGNORE, atANCHOR_IGNORE );
	
	lpData->hButton = ExtPkg_CreateButton( hWnd, 103, 70, 70, 50, 18, _AT("OK") );
	atCONTROL_SetAnchor( lpData->hButton, atANCHOR_IGNORE, 10, atANCHOR_IGNORE, atANCHOR_IGNORE );
}

static void	HSliderWndOnRelease(atHWINDOW hWnd)
{
//	LPHSLIDERWNDDATA		lpData	= (LPHSLIDERWNDDATA)atWINDOW_GetDataPtr(hWnd);
}

static int		HSliderWndOnKeyDown(atHWINDOW hWnd, int nKeyCode)
{
//	LPHSLIDERWNDDATA		lpData	= (LPHSLIDERWNDDATA)atWINDOW_GetDataPtr(hWnd);

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

static void	HSliderWndOnPaint(atHWINDOW hWnd)
{
//	LPHSLIDERWNDDATA		lpData	= (LPHSLIDERWNDDATA)atWINDOW_GetDataPtr(hWnd);

}

static void	HSliderWndOnChange(atHWINDOW hWnd, long nParam1, long nParam2)
{
	LPHSLIDERWNDDATA		lpData	= (LPHSLIDERWNDDATA)atWINDOW_GetDataPtr(hWnd);
	atTCHAR str[50];

	switch( nParam1 ){
		case 101: 
			UI_SPRINTF( str, _AT("%d/%d"), atHSLIDER_EXT_GetPosition(lpData->hHSlider), atHSLIDER_EXT_GetMaxPos(lpData->hHSlider) );
			atSTATICBOX_SetText( lpData->hStatic, str );
//			atCONTROL_Draw( lpData->hStatic, atTRUE );
			atCONTROL_Refresh(lpData->hStatic);
			break;
	}
}

static int	HSliderWndOnEvent(atHWINDOW hWnd, int nMsg, long nParam1, long nParam2)
{
	LPHSLIDERWNDDATA		lpData	= (LPHSLIDERWNDDATA)atWINDOW_GetDataPtr(hWnd);

	switch( nMsg )
	{
		case WNDMSG_INIT:
			HSliderWndOnInit( hWnd, (void *)nParam1 );
			break;
		case WNDMSG_RELEASE:
			HSliderWndOnRelease( hWnd );
			break;
		case WNDMSG_PAINT:
			HSliderWndOnPaint( hWnd );
			break;
		case WNDMSG_KEYDOWN:
			return HSliderWndOnKeyDown( hWnd, nParam1 );
			break;
		case WNDMSG_CHANGE:
			HSliderWndOnChange( hWnd, nParam1, nParam2 );
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

