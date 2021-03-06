//
// Screen_3_chart_2.c 
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
#include "Screen_3_chart_2.h"
#include "Screen_3_image.h"

#include <string.h>

static SCR_3_IMAGE_WNDDATA imgWndData;

static void Scr_3_Chart_2_WndOnCommand(atHWINDOW hWnd, long nParam1, long nParam2)
{
	LPSCR_3_CHART_2_WNDDATA lpData = (LPSCR_3_CHART_2_WNDDATA)atWINDOW_GetDataPtr(hWnd);

	switch (nParam1)
	{
	case 100:
		atAPP_DestroyWindow(atAPPMNG_GetCurApp(), hWnd);
		break;

	case 1000:
		atAPP_DestroyWindow(atAPPMNG_GetCurApp(), hWnd);
		break;

		// EXIT
	case 1001:
	{
		atAPP_DestroyWindow(atAPPMNG_GetCurApp(), hWnd);
	}
	break;

	// IMAGE
	case 1011:
	case 1012:
	case 1013:
	case 1014:
	case 1015:
	{
		memset(&imgWndData, 0, sizeof(SCR_3_IMAGE_WNDDATA));
		if (nParam1 == 1011)
			imgWndData.nType = 401;
		else if (nParam1 == 1012)
			imgWndData.nType = 402;
		else if (nParam1 == 1013)
			imgWndData.nType = 403;
		else if (nParam1 == 1014)
			imgWndData.nType = 404;
		else if (nParam1 == 1015)
			imgWndData.nType = 405;

		atHWINDOW wnd;
		wnd = atAPP_CreateWindow(atAPPMNG_GetCurApp(), atNULL, 0, atNULL, _AT(""), 0, 0, 0, 0, sizeof(SCR_3_IMAGE_WNDDATA), Scr_3_Image_WndOnEvent, &imgWndData, atNULL);
		atWINDOW_SetAnchor(wnd, 0, 0, 0, 0);
		atAPP_ShowWindow(atAPPMNG_GetCurApp(), wnd, atTRUE);
	}
	break;
	}
}


static void Scr_3_Chart_2_WndOnChange(atHWINDOW hWnd, long nParam1, long nParam2)
{
	LPSCR_3_CHART_2_WNDDATA lpData = (LPSCR_3_CHART_2_WNDDATA)atWINDOW_GetDataPtr(hWnd);

	switch (nParam1)
	{
	case 103:

		break;
	}
}

static void Scr_3_Chart_2_WndOnInit(atHWINDOW hWnd, long nParam)
{
	LPSCR_3_CHART_2_WNDDATA lpData = (LPSCR_3_CHART_2_WNDDATA)atWINDOW_GetDataPtr(hWnd);

	atHCONTROL ctl;
	atHCONTROL hWidget;

	//atHRESSRC hResSrc = atUIAPI_AddResSource(_AT("TESTMAIN"), atUI_RES_SRC_RESSTREAM, (const atLPVOID)pUser_ResData, sizeof(pUser_ResData));
	atHRESSRC hResSrc = atUIAPI_GetResSource(_AT("TESTMAIN"));

	lpData->hBm_bg = atUIAPI_CreateBitmapFromRes(hResSrc, _AT("Scr_3_bg.img"));
	lpData->hBm_title = atUIAPI_CreateBitmapFromRes(hResSrc, _AT("Scr_3_select_vision_n.img"));

	lpData->hBm_btn_01_n = atUIAPI_CreateBitmapFromRes(hResSrc, _AT("Scr_3_vision_btn_1_n.img"));
	lpData->hBm_btn_02_n = atUIAPI_CreateBitmapFromRes(hResSrc, _AT("Scr_3_vision_btn_2_n.img"));
	lpData->hBm_btn_03_n = atUIAPI_CreateBitmapFromRes(hResSrc, _AT("Scr_3_vision_btn_3_n.img"));
	lpData->hBm_btn_04_n = atUIAPI_CreateBitmapFromRes(hResSrc, _AT("Scr_3_vision_btn_4_n.img"));
	lpData->hBm_btn_05_n = atUIAPI_CreateBitmapFromRes(hResSrc, _AT("Scr_3_vision_btn_5_n.img"));
	lpData->hBm_btn_exit_n = atUIAPI_CreateBitmapFromRes(hResSrc, _AT("Scr_def_btn_exit_n.img"));

	lpData->hBm_btn_01_p = atUIAPI_CreateBitmapFromRes(hResSrc, _AT("Scr_3_vision_btn_1_p.img"));
	lpData->hBm_btn_02_p = atUIAPI_CreateBitmapFromRes(hResSrc, _AT("Scr_3_vision_btn_2_p.img"));
	lpData->hBm_btn_03_p = atUIAPI_CreateBitmapFromRes(hResSrc, _AT("Scr_3_vision_btn_3_p.img"));
	lpData->hBm_btn_04_p = atUIAPI_CreateBitmapFromRes(hResSrc, _AT("Scr_3_vision_btn_4_p.img"));
	lpData->hBm_btn_05_p = atUIAPI_CreateBitmapFromRes(hResSrc, _AT("Scr_3_vision_btn_5_p.img"));
	lpData->hBm_btn_exit_p = atUIAPI_CreateBitmapFromRes(hResSrc, _AT("Scr_def_btn_exit_p.img"));

	//hWidget = BasePkg_CreateImageBox(atNULL, 3000, 0, 0, 320, 480, lpData->hBm_bg, 1);
	//hWidget = BasePkg_CreateImageBox(atNULL, 4000, 47, 60, 0, 0, lpData->hBm_title, 1);
	
	hWidget = ExtPkg_CreateImageButton(hWnd, 1011, 35, 207, 55, 55, lpData->hBm_btn_01_n, lpData->hBm_btn_01_p, atNULL);
	atCONTROL_SetFocusable(hWidget, atFALSE);
	hWidget = ExtPkg_CreateImageButton(hWnd, 1012, 100, 207, 55, 55, lpData->hBm_btn_02_n, lpData->hBm_btn_02_p, atNULL);
	atCONTROL_SetFocusable(hWidget, atFALSE);
	hWidget = ExtPkg_CreateImageButton(hWnd, 1013, 166, 207, 55, 55, lpData->hBm_btn_03_n, lpData->hBm_btn_03_p, atNULL);
	atCONTROL_SetFocusable(hWidget, atFALSE);
	hWidget = ExtPkg_CreateImageButton(hWnd, 1014, 229, 207, 55, 55, lpData->hBm_btn_04_n, lpData->hBm_btn_04_p, atNULL);
	atCONTROL_SetFocusable(hWidget, atFALSE);
	hWidget = ExtPkg_CreateImageButton(hWnd, 1015, 35, 306, 55, 55, lpData->hBm_btn_05_n, lpData->hBm_btn_05_p, atNULL);
	atCONTROL_SetFocusable(hWidget, atFALSE);
	hWidget = ExtPkg_CreateImageButton(hWnd, 1001, 100, 306, 55, 55, lpData->hBm_btn_exit_n, lpData->hBm_btn_exit_p, atNULL);
	atCONTROL_SetFocusable(hWidget, atFALSE);
}

static void Scr_3_Chart_2_WndOnRelease(atHWINDOW hWnd)
{
	LPSCR_3_CHART_2_WNDDATA lpData = (LPSCR_3_CHART_2_WNDDATA)atWINDOW_GetDataPtr(hWnd);

	atUIAPI_DestroyBitmap(lpData->hBm_bg);
	atUIAPI_DestroyBitmap(lpData->hBm_title);

	atUIAPI_DestroyBitmap(lpData->hBm_btn_01_n);
	atUIAPI_DestroyBitmap(lpData->hBm_btn_02_n);
	atUIAPI_DestroyBitmap(lpData->hBm_btn_03_n);
	atUIAPI_DestroyBitmap(lpData->hBm_btn_04_n);
	atUIAPI_DestroyBitmap(lpData->hBm_btn_05_n);
	atUIAPI_DestroyBitmap(lpData->hBm_btn_exit_n);

	atUIAPI_DestroyBitmap(lpData->hBm_btn_01_p);
	atUIAPI_DestroyBitmap(lpData->hBm_btn_02_p);
	atUIAPI_DestroyBitmap(lpData->hBm_btn_03_p);
	atUIAPI_DestroyBitmap(lpData->hBm_btn_04_p);
	atUIAPI_DestroyBitmap(lpData->hBm_btn_05_p);
	atUIAPI_DestroyBitmap(lpData->hBm_btn_exit_p);
}


static int Scr_3_Chart_2_WndOnKeyDown(atHWINDOW hWnd, int nKeyCode)
{
	LPSCR_3_CHART_2_WNDDATA lpData = (LPSCR_3_CHART_2_WNDDATA)atWINDOW_GetDataPtr(hWnd);
	atHWINDOW hPopup;

	switch (nKeyCode)
	{

	case atVKEY_SOFT1:
		break;

	case atVKEY_SOFT2:
		break;

	case atVKEY_CLEAR:
		atAPP_DestroyWindow(atAPPMNG_GetCurApp(), hWnd);
		break;

	default:
		return 0;
	}

	return 0; // WindowOnEvent 가 호출되지 않도록 0을 리턴한다.
}


#define DELTA 5

static int Scr_3_Chart_2_WndOnKeyPress(atHWINDOW hWnd, int nKeyCode)
{
	LPSCR_3_CHART_2_WNDDATA lpData = (LPSCR_3_CHART_2_WNDDATA)atWINDOW_GetDataPtr(hWnd);

	switch (nKeyCode)
	{
	case atVKEY_LEFT:
		atWINDOW_Refresh(hWnd);
		break;

	case atVKEY_RIGHT:
		atWINDOW_Refresh(hWnd);
		break;

	case atVKEY_UP:
		atWINDOW_Refresh(hWnd);
		break;

	case atVKEY_DOWN:
		atWINDOW_Refresh(hWnd);
		break;

	default:
		return 0;
	}

	return 1; // WindowOnEvent 가 호출되지 않도록 0을 리턴한다.
}


static void Scr_3_CHART_2_WndOnPaint(atHWINDOW hWnd)
{
	LPSCR_3_CHART_2_WNDDATA	lpData = (LPSCR_3_CHART_2_WNDDATA)atWINDOW_GetDataPtr(hWnd);

	atHGDC gdc = atWINDOW_GetClientGDC(hWnd);
	atREGION rgn;
	atHFONT oldfont;
	atTCHAR str[64];

	atUIAPI_DrawImage(gdc, 0, 0, lpData->hBm_bg);
	atUIAPI_DrawImage(gdc, 47, 60, lpData->hBm_title);
}


int Scr_3_Chart_2_WndOnEvent(atHWINDOW hWnd, int nMsg, long nParam1, long nParam2)
{
	int ret = 0;
	LPSCR_3_CHART_2_WNDDATA	lpData = (LPSCR_3_CHART_2_WNDDATA)atWINDOW_GetDataPtr(hWnd);
	LPSCR_3_CHART_2_WNDDATA  Data = (LPSCR_3_CHART_2_WNDDATA)nParam1;

	switch (nMsg)
	{
	case WNDMSG_INIT:
	{
		lpData->nType = Data->nType;
		Scr_3_Chart_2_WndOnInit(hWnd, nParam1);
	}
	break;
	case WNDMSG_RELEASE:
		Scr_3_Chart_2_WndOnRelease(hWnd);
		break;
	case WNDMSG_PAINT:
		Scr_3_CHART_2_WndOnPaint(hWnd);
		break;
	case WNDMSG_KEYDOWN:
		return Scr_3_Chart_2_WndOnKeyDown(hWnd, nParam1);
		break;
	case WNDMSG_KEYPRESS:
		return Scr_3_Chart_2_WndOnKeyPress(hWnd, nParam1);
		break;
	case WNDMSG_CHANGE:
		Scr_3_Chart_2_WndOnChange(hWnd, nParam1, nParam2);
		break;
	case WNDMSG_COMMAND:
		Scr_3_Chart_2_WndOnCommand(hWnd, nParam1, nParam2);
		break;
	default:
		return 0;
		break;
	}

	return ret;
}
