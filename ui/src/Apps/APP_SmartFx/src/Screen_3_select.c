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
#include "Screen_3_select.h"
#include "Screen_3_chart.h"
#include "Screen_3_chart_2.h"

#include <string.h>

 
static SCR_3_CHART_WNDDATA		chartWndData;
static SCR_3_CHART_2_WNDDATA	chart2WndData;

static void Scr_3_Select_WndOnCommand(atHWINDOW hWnd, long nParam1, long nParam2)
{
	LPSCR_3_SELECT_WNDDATA lpData = (LPSCR_3_SELECT_WNDDATA)atWINDOW_GetDataPtr(hWnd);
  
    switch(nParam1)
    {
        case 1001 :
			{
				atHWINDOW wnd;
				memset(&chartWndData, 0, sizeof(SCR_3_CHART_WNDDATA));
				chartWndData.nType = 301;
				wnd = atAPP_CreateWindow(atAPPMNG_GetCurApp(), atNULL, 0, atNULL, _AT(""), 0, 0, 0, 0, sizeof(SCR_3_CHART_WNDDATA), Scr_3_Chart_WndOnEvent, &chartWndData, atNULL);
				atWINDOW_SetAnchor(wnd, 0, 0, 0, 0);
				atAPP_ShowWindow(atAPPMNG_GetCurApp(), wnd, atTRUE);
			}
			break;

		case 1002:
			{
				atHWINDOW wnd;
				memset(&chart2WndData, 0, sizeof(SCR_3_CHART_2_WNDDATA));
				chart2WndData.nType = 401;
				wnd = atAPP_CreateWindow(atAPPMNG_GetCurApp(), hWnd, 0, atWS_NONEFRAME | atWS_MODAL, _AT(""), 0, 0, 0, 0, sizeof(SCR_3_CHART_2_WNDDATA), Scr_3_Chart_2_WndOnEvent, &chartWndData, atNULL);
				atWINDOW_SetAnchor(wnd, 0, 0, 0, 0);
				atAPP_ShowWindow(atAPPMNG_GetCurApp(), wnd, atTRUE);
			}
			break;

		case 1003:
			{
				atAPP_DestroyWindow(atAPPMNG_GetCurApp(), hWnd);
			}
			break;
	}
}


static void Scr_3_Select_WndOnChange(atHWINDOW hWnd, long nParam1, long nParam2)
{
	LPSCR_3_SELECT_WNDDATA lpData = (LPSCR_3_SELECT_WNDDATA)atWINDOW_GetDataPtr(hWnd);

    switch(nParam1)
    {
        case 103 :

			break;
	}
}


static void Scr_3_Select_WndOnInit(atHWINDOW hWnd, long nParam)
{
	LPSCR_3_SELECT_WNDDATA lpData = (LPSCR_3_SELECT_WNDDATA)atWINDOW_GetDataPtr(hWnd);

	atHCONTROL ctl;
	atHCONTROL hWidget;

	//atHRESSRC hResSrc = atUIAPI_AddResSource(_AT("TESTMAIN"), atUI_RES_SRC_RESSTREAM, (const atLPVOID)pUser_ResData, sizeof(pUser_ResData));
	atHRESSRC hResSrc = atUIAPI_GetResSource(_AT("TESTMAIN"));

	lpData->hBm_bg = atUIAPI_CreateBitmapFromRes(hResSrc, _AT("Scr_3_bg_home.img"));

	lpData->hBm_btn_sp_n = atUIAPI_CreateBitmapFromRes(hResSrc, _AT("Scr_3_select_special_n.img"));
	lpData->hBm_btn_vi_n = atUIAPI_CreateBitmapFromRes(hResSrc, _AT("Scr_3_select_vision_n.img"));
	lpData->hBm_btn_home_n = atUIAPI_CreateBitmapFromRes(hResSrc, _AT("Scr_def_btn_home_3_n.img"));

	lpData->hBm_btn_sp_p = atUIAPI_CreateBitmapFromRes(hResSrc, _AT("Scr_3_select_special_p.img"));
	lpData->hBm_btn_vi_p = atUIAPI_CreateBitmapFromRes(hResSrc, _AT("Scr_3_select_vision_p.img"));
	lpData->hBm_btn_home_p = atUIAPI_CreateBitmapFromRes(hResSrc, _AT("Scr_def_btn_home_3_p.img"));

	//hWidget = BasePkg_CreateImageBox(atNULL, 3000, 0, 0, 320, 480, lpData->hBm_bg, 1);

	hWidget = ExtPkg_CreateImageButton(hWnd, 1001, 47, 157, 226, 34, lpData->hBm_btn_sp_n, lpData->hBm_btn_sp_p, atNULL);
	atCONTROL_SetFocusable(hWidget, atFALSE);
	hWidget = ExtPkg_CreateImageButton(hWnd, 1002, 47, 237, 226, 34, lpData->hBm_btn_vi_n, lpData->hBm_btn_vi_p, atNULL);
	atCONTROL_SetFocusable(hWidget, atFALSE);
	hWidget = ExtPkg_CreateImageButton(hWnd, 1003, 97, 398, 124, 64, lpData->hBm_btn_home_n, lpData->hBm_btn_home_p, atNULL);
	atCONTROL_SetFocusable(hWidget, atFALSE);
}



static void Scr_3_Select_WndOnRelease(atHWINDOW hWnd)
{
	LPSCR_3_SELECT_WNDDATA lpData = (LPSCR_3_SELECT_WNDDATA)atWINDOW_GetDataPtr(hWnd);

	atUIAPI_DestroyBitmap(lpData->hBm_bg);

	atUIAPI_DestroyBitmap(lpData->hBm_btn_sp_n);
	atUIAPI_DestroyBitmap(lpData->hBm_btn_vi_n);
	atUIAPI_DestroyBitmap(lpData->hBm_btn_home_n);

	atUIAPI_DestroyBitmap(lpData->hBm_btn_sp_p);
	atUIAPI_DestroyBitmap(lpData->hBm_btn_vi_p);
	atUIAPI_DestroyBitmap(lpData->hBm_btn_home_p);
}


static int Scr_3_Select_WndOnKeyDown(atHWINDOW hWnd, int nKeyCode)
{
	LPSCR_3_SELECT_WNDDATA lpData = (LPSCR_3_SELECT_WNDDATA)atWINDOW_GetDataPtr(hWnd);
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

static int Scr_3_Select_WndOnKeyPress(atHWINDOW hWnd, int nKeyCode)
{
	LPSCR_3_SELECT_WNDDATA lpData = (LPSCR_3_SELECT_WNDDATA)atWINDOW_GetDataPtr(hWnd);

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


static void Scr_3_Select_WndOnPaint(atHWINDOW hWnd)
{
	LPSCR_3_SELECT_WNDDATA lpData = (LPSCR_3_SELECT_WNDDATA)atWINDOW_GetDataPtr(hWnd);

	atHGDC gdc = atWINDOW_GetClientGDC(hWnd);
	atREGION rgn;
	atHFONT oldfont;
	atTCHAR str[64];

	atUIAPI_DrawImage(gdc, 0, 0, lpData->hBm_bg);
}


int Scr_3_Select_WndOnEvent(atHWINDOW hWnd, int nMsg, long nParam1, long nParam2)
{
	int ret = 0;
	LPSCR_3_SELECT_WNDDATA lpData = (LPSCR_3_SELECT_WNDDATA)atWINDOW_GetDataPtr(hWnd);
	LPSCR_3_SELECT_WNDDATA Data = (LPSCR_3_SELECT_WNDDATA)nParam1;
	

	switch( nMsg )
	{
		case WNDMSG_INIT:
		{
			lpData->nType = Data->nType;
			Scr_3_Select_WndOnInit(hWnd, nParam1);
		}
			break;
		case WNDMSG_RELEASE:
			Scr_3_Select_WndOnRelease( hWnd );
			break;
		case WNDMSG_PAINT:
			Scr_3_Select_WndOnPaint( hWnd );
			break;
		case WNDMSG_KEYDOWN:
			return Scr_3_Select_WndOnKeyDown( hWnd, nParam1 );
			break;
		case WNDMSG_KEYPRESS:
			return Scr_3_Select_WndOnKeyPress( hWnd, nParam1 );
			break;
		case WNDMSG_CHANGE:
			Scr_3_Select_WndOnChange( hWnd, nParam1, nParam2 );
			break;
		case WNDMSG_COMMAND:
			Scr_3_Select_WndOnCommand( hWnd, nParam1, nParam2 );
			break;
		default:
			return 0;
			break;
	}

	return ret;
}
