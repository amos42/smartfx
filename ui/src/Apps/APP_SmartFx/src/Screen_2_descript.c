//
// TestWnd.c 
//
//
//   
//
///////////////////////////////////////////////////////////////////////
 
#include "A_UIENG.h"
#include "UI_Util.h"
#include "Global.h"
#include "AEECOMP_BasePkg.h"
#include "AEECOMP_ExtPkg.h"
#include "Screen_2_descript.h"
#include "Screen_2_control.h"

#include <string.h>

static SCR_2_CONTROL_WNDDATA descWndData;

static void Scr_2_Desc_WndOnCommand(atHWINDOW hWnd, long nParam1, long nParam2)
{
	LPSCR_2_DESC_WNDDATA lpData = (LPSCR_2_DESC_WNDDATA)atWINDOW_GetDataPtr(hWnd);
  
    switch(nParam1)
    {
        case 100 :
			atAPP_DestroyWindow( atAPPMNG_GetCurApp(), hWnd );
			break;

		case 1000:
			atAPP_DestroyWindow(atAPPMNG_GetCurApp(), hWnd);
			break;

		case 1001:
		{
			memset(&descWndData, 0, sizeof(SCR_2_CONTROL_WNDDATA));
			if (lpData->nType == 211) 
				descWndData.nType = 211;
			else if (lpData->nType == 212) 
				descWndData.nType = 212;

			atHWINDOW wnd;
			wnd = atAPP_CreateWindow(atAPPMNG_GetCurApp(), atNULL, 0, atWS_MODAL, _AT(""), 0, 0, 0, 0, sizeof(SCR_2_CONTROL_WNDDATA), Scr_2_Control_WndOnEvent, &descWndData, atNULL);
			atWINDOW_SetAnchor(wnd, 0, 0, 0, 0);
			atAPP_ShowWindow(atAPPMNG_GetCurApp(), wnd, atTRUE);

			atAPP_DestroyWindow(atAPPMNG_GetCurApp(), hWnd);
		}
			break;
	}
}

static void Scr_2_Desc_WndOnChange(atHWINDOW hWnd, long nParam1, long nParam2)
{
	LPSCR_2_DESC_WNDDATA lpData = (LPSCR_2_DESC_WNDDATA)atWINDOW_GetDataPtr(hWnd);

    switch(nParam1)
    {
        case 103 :

			break;
	}
}

static void Scr_2_Desc_WndOnInit(atHWINDOW hWnd, long nParam)
{
	LPSCR_2_DESC_WNDDATA lpData = (LPSCR_2_DESC_WNDDATA)atWINDOW_GetDataPtr(hWnd);

	atHCONTROL ctl;
	atHCONTROL hWidget;

	//atHRESSRC hResSrc = atUIAPI_AddResSource(_AT("TESTMAIN"), atUI_RES_SRC_RESSTREAM, (const atLPVOID)pUser_ResData, sizeof(pUser_ResData));
	atHRESSRC hResSrc = atUIAPI_GetResSource(_AT("TESTMAIN"));

	lpData->hBm_bg = atUIAPI_CreateBitmapFromRes(hResSrc, _AT("Scr_1_bg.img"));

	if (lpData->nType == 211)
		lpData->hBm_title = atUIAPI_CreateBitmapFromRes(hResSrc, _AT("Scr_2_desc_right_content.img"));
	else if (lpData->nType == 212)
		lpData->hBm_title = atUIAPI_CreateBitmapFromRes(hResSrc, _AT("Scr_2_desc_left_content.img"));

	lpData->hBm_btn_ok_n = atUIAPI_CreateBitmapFromRes(hResSrc, _AT("Scr_def_btn_ok_n.img"));
	lpData->hBm_btn_ok_p = atUIAPI_CreateBitmapFromRes(hResSrc, _AT("Scr_def_btn_ok_p.img"));

	//hWidget = BasePkg_CreateImageBox(atNULL, 3000, 0, 0, 320, 480, lpData->hBm_bg, 1);
	//hWidget = BasePkg_CreateImageBox(atNULL, 4000, 79, 132, 162, 162, lpData->hBm_title, 1);
	
	hWidget = ExtPkg_CreateImageButton(hWnd, 1001, 98, 398, 126, 64, lpData->hBm_btn_ok_n, lpData->hBm_btn_ok_p, atNULL);
	atCONTROL_SetFocusable(hWidget, atFALSE);
}

static void Scr_2_Desc_WndOnRelease(atHWINDOW hWnd)
{
	LPSCR_2_DESC_WNDDATA	lpData = (LPSCR_2_DESC_WNDDATA)atWINDOW_GetDataPtr(hWnd);

	atUIAPI_DestroyBitmap(lpData->hBm_bg);
	atUIAPI_DestroyBitmap(lpData->hBm_title);
	atUIAPI_DestroyBitmap(lpData->hBm_btn_ok_n);
	atUIAPI_DestroyBitmap(lpData->hBm_btn_ok_p);
}


static int Scr_2_Desc_WndOnKeyDown(atHWINDOW hWnd, int nKeyCode)
{
	LPSCR_2_DESC_WNDDATA lpData = (LPSCR_2_DESC_WNDDATA)atWINDOW_GetDataPtr(hWnd);
	atHWINDOW hPopup;

    switch( nKeyCode )
    {
        case atVKEY_SOFT1:
            break;
			
        case atVKEY_SOFT2:
            break;

        case atVKEY_CLEAR:
		atAPP_DestroyWindow( atAPPMNG_GetCurApp(), hWnd );
            break;
			
        default:
            return 0;
    }

    return 0; // WindowOnEvent 가 호출되지 않도록 0을 리턴한다.
}


#define DELTA 5

static int Scr_2_Desc_WndOnKeyPress(atHWINDOW hWnd, int nKeyCode)
{
	LPSCR_2_DESC_WNDDATA lpData = (LPSCR_2_DESC_WNDDATA)atWINDOW_GetDataPtr(hWnd);

    switch( nKeyCode )
    {
        case atVKEY_LEFT:	
			atWINDOW_Refresh( hWnd );
            break;

        case atVKEY_RIGHT:	
			atWINDOW_Refresh( hWnd );
            break;

        case atVKEY_UP:	
			atWINDOW_Refresh( hWnd );
            break;

        case atVKEY_DOWN:	
			atWINDOW_Refresh( hWnd );
            break;
			
        default:
            return 0;
    }

    return 1; // WindowOnEvent 가 호출되지 않도록 0을 리턴한다.
}


static void Scr_2_Desc_WndOnPaint(atHWINDOW hWnd)
{
	LPSCR_2_DESC_WNDDATA	lpData = (LPSCR_2_DESC_WNDDATA)atWINDOW_GetDataPtr(hWnd);

	atHGDC gdc = atWINDOW_GetClientGDC(hWnd);
	atREGION rgn;
	atHFONT oldfont;
	atTCHAR str[64];

	atUIAPI_DrawImage(gdc, 0, 0, lpData->hBm_bg);
	atUIAPI_DrawImage(gdc, 79, 132, lpData->hBm_title);
}


int Scr_2_Desc_WndOnEvent(atHWINDOW hWnd, int nMsg, long nParam1, long nParam2)
{
	int ret = 0;
	LPSCR_2_DESC_WNDDATA	lpData = (LPSCR_2_DESC_WNDDATA)atWINDOW_GetDataPtr(hWnd);
	LPSCR_2_DESC_WNDDATA  Data = (LPSCR_2_DESC_WNDDATA)nParam1;
	

	switch( nMsg )
	{
		case WNDMSG_INIT:
		{
			lpData->nType = Data->nType;
			Scr_2_Desc_WndOnInit(hWnd, nParam1);
		}
			break;
		case WNDMSG_RELEASE:
			Scr_2_Desc_WndOnRelease( hWnd );
			break;
		case WNDMSG_PAINT:
			Scr_2_Desc_WndOnPaint( hWnd );
			break;
		case WNDMSG_KEYDOWN:
			return Scr_2_Desc_WndOnKeyDown( hWnd, nParam1 );
			break;
		case WNDMSG_KEYPRESS:
			return Scr_2_Desc_WndOnKeyPress( hWnd, nParam1 );
			break;
		case WNDMSG_CHANGE:
			Scr_2_Desc_WndOnChange( hWnd, nParam1, nParam2 );
			break;
		case WNDMSG_COMMAND:
			Scr_2_Desc_WndOnCommand( hWnd, nParam1, nParam2 );
			break;
		default:
			return 0;
			break;
	}

	return ret;
}
