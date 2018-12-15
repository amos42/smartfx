//
// MainWnd.c 
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
#include "MainWnd.h"

#include <string.h>


#include "Screen_1_descript.h"
#include "Screen_2_descript.h"
#include "Screen_3_select.h"
#include "Screen_setting.h"
#include "Screen_1_result_2.h"
#include "Screen_2_result.h"


static SCR_1_DESC_WNDDATA		descWndData;
static SCR_2_DESC_WNDDATA		desc2WndData;
static SCR_3_SELECT_WNDDATA		chartSelWndData;
static SCR_SETTING_WNDDATA		setWndData;
static SCR_1_RESULT_2_WNDDATA	scr1Res2Data;
static SCR_2_RESULT_WNDDATA		scr2ResData;

static void MainWndOnCommand(atHWINDOW hWnd, long nParam1, long nParam2)
{
    LPMAINWNDDATA lpData = (LPMAINWNDDATA)atWINDOW_GetDataPtr(hWnd);
  
	atHWINDOW wnd;
	atMARGIN anchor;
	static atTCHAR test_str[100];

    switch(nParam1)
    {
		case 1001:	
		{
			lpData->nCount = 0;
			memset(&descWndData, 0, sizeof(SCR_1_DESC_WNDDATA));
			descWndData.nType = 101;

			nDistDivid = nDistRecover = 10;
			nDistPenDivid = nDistPenRecover = 10;
			bScr1ResDist = bScr1ResPen = atFALSE;

			wnd = atAPP_CreateWindow(atAPPMNG_GetCurApp(), hWnd, 0, atWS_NONEFRAME | atWS_MODAL, _AT(""), 0, 0, 0, 0, sizeof(SCR_1_DESC_WNDDATA), Scr_1_Desc_WndOnEvent, &descWndData, atNULL);
			atWINDOW_SetAnchor(wnd, 0, 0, 0, 0);
			atAPP_ShowWindow(atAPPMNG_GetCurApp(), wnd, atTRUE);
		}
			break;

		case 1002:
		{
			lpData->nCount = 0;
			memset(&desc2WndData, 0, sizeof(SCR_2_DESC_WNDDATA));
			desc2WndData.nType = 211;

			nDistRight = nDistLeft = 1;
			nDistRead = 40;

			wnd = atAPP_CreateWindow(atAPPMNG_GetCurApp(), hWnd, 0, atWS_NONEFRAME | atWS_MODAL, _AT(""), 0, 0, 0, 0, sizeof(SCR_2_DESC_WNDDATA), Scr_2_Desc_WndOnEvent, &desc2WndData, atNULL);
			atWINDOW_SetAnchor(wnd, 0, 0, 0, 0);
			atAPP_ShowWindow(atAPPMNG_GetCurApp(), wnd, atTRUE);
		}
			break;

		case 1003:
		{
			lpData->nCount = 0;
			memset(&chartSelWndData, 0, sizeof(SCR_3_SELECT_WNDDATA));
			chartSelWndData.nType = 301;

			wnd = atAPP_CreateWindow(atAPPMNG_GetCurApp(), atNULL, 0, atNULL, _AT(""), 0, 0, 0, 0, sizeof(SCR_3_SELECT_WNDDATA), Scr_3_Select_WndOnEvent, &chartSelWndData, atNULL);
			atWINDOW_SetAnchor(wnd, 0, 0, 0, 0);
			atAPP_ShowWindow(atAPPMNG_GetCurApp(), wnd, atTRUE);
		}
			break;

		case 1004:
		{
			lpData->nCount = 0;
			memset(&scr1Res2Data, 0, sizeof(SCR_1_RESULT_2_WNDDATA));

			wnd = atAPP_CreateWindow(atAPPMNG_GetCurApp(), atNULL, 0, atNULL, _AT(""), 0, 0, 0, 0, sizeof(SCR_1_RESULT_2_WNDDATA), Scr_1_Result_2_WndOnEvent, &scr1Res2Data, atNULL);
			atWINDOW_SetAnchor(wnd, 0, 0, 0, 0);
			atAPP_ShowWindow(atAPPMNG_GetCurApp(), wnd, atTRUE);
		}
			break;

		case 1005:
		{
			lpData->nCount = 0;
			memset(&scr2ResData, 0, sizeof(SCR_2_RESULT_WNDDATA));
			scr2ResData.nType = 400;

			wnd = atAPP_CreateWindow(atAPPMNG_GetCurApp(), atNULL, 0, atNULL, _AT(""), 0, 0, 0, 0, sizeof(SCR_2_RESULT_WNDDATA), Scr_2_Result_WndOnEvent, &scr2ResData, atNULL);
			atWINDOW_SetAnchor(wnd, 0, 0, 0, 0);
			atAPP_ShowWindow(atAPPMNG_GetCurApp(), wnd, atTRUE);
		}
		break;

		case 1100:
		{
			lpData->nCount++;
			if (lpData->nCount >= 7) {
				lpData->nCount = 0;
				memset(&setWndData, 0, sizeof(SCR_SETTING_WNDDATA));
				wnd = atAPP_CreateWindow(atAPPMNG_GetCurApp(), atNULL, 0, atNULL, _AT(""), 0, 0, 0, 0, sizeof(SCR_SETTING_WNDDATA), Scr_Setting_WndOnEvent, &setWndData, atNULL);
				atWINDOW_SetAnchor(wnd, 0, 0, 0, 0);
				atAPP_ShowWindow(atAPPMNG_GetCurApp(), wnd, atTRUE);
			}
		}
			break;
	}
}

static void MainWndOnTimer(atHWINDOW hWnd, long nParam1, long nParam2)
{
	LPMAINWNDDATA lpData = (LPMAINWNDDATA)atWINDOW_GetDataPtr(hWnd);

	switch (nParam2)
	{
		case 0x100:
		{
			atHRESSRC hResSrc = atUIAPI_GetResSource(_AT("TESTMAIN"));

			atUIAPI_DestroyBitmap(lpData->hBm_battery);
			atWINDOW_RemoveControl(hWnd, lpData->ctlBattery);

			if (defSetting.nBattery > GetBatteryValue()) {
				lpData->hBm_battery = atUIAPI_CreateBitmapFromRes(hResSrc, _AT("Scr_def_battery_r.img"));
				lpData->ctlBattery = ExtPkg_CreateImageButton(lpData->ctlPanel, 1100, 276, 8, 37, 23, lpData->hBm_battery, atNULL, atNULL);
				atPANEL_AddControl(lpData->ctlPanel, lpData->ctlBattery);
			}
			else {
				lpData->hBm_battery = atUIAPI_CreateBitmapFromRes(hResSrc, _AT("Scr_def_battery_b.img"));
				lpData->ctlBattery = ExtPkg_CreateImageButton(lpData->ctlPanel, 1100, 276, 8, 37, 23, lpData->hBm_battery, atNULL, atNULL);
				atPANEL_AddControl(lpData->ctlPanel, lpData->ctlBattery);
			}

			atWINDOW_Refresh(hWnd);
		}
			break;
	}
}

static void MainWndOnInit(atHWINDOW hWnd)
{
    int i;
    LPMAINWNDDATA	lpData = (LPMAINWNDDATA)atWINDOW_GetDataPtr(hWnd);

	atSCROLLPANEL_FUNCS funcs;
	atHCONTROL hWidget;

	SETTINGDEVICE *devVal = GetDeviceValues();
	if(devVal != atNULL)
		memcpy(&defSetting, devVal, sizeof(SETTINGDEVICE));

	nDistDivid = nDistRecover = nDistPenDivid = nDistPenRecover = nDistRight = nDistLeft = nDistChart = 1;
	nDistRead = 40;
	bPrintMsg = defSetting.bPrintMsg;

	//atHRESSRC hResSrc = atUIAPI_AddResSource(_AT("TESTMAIN"), atUI_RES_SRC_RESSTREAM, (const atLPVOID)pUser_ResData, sizeof(pUser_ResData));
	atHRESSRC hResSrc = atUIAPI_GetResSource(_AT("TESTMAIN"));

	lpData->hBm_bg = atUIAPI_CreateBitmapFromRes(hResSrc, _AT("Scr_main_bg.img"));
	lpData->hBm_battery = atUIAPI_CreateBitmapFromRes(hResSrc, _AT("Scr_def_battery_b.img"));

	lpData->hBm_btn_01_n = atUIAPI_CreateBitmapFromRes(hResSrc, _AT("Scr_main_btn_1_n.img"));
	lpData->hBm_btn_01_p = atUIAPI_CreateBitmapFromRes(hResSrc, _AT("Scr_main_btn_1_p.img"));
	lpData->hBm_btn_02_n = atUIAPI_CreateBitmapFromRes(hResSrc, _AT("Scr_main_btn_2_n.img"));
	lpData->hBm_btn_02_p = atUIAPI_CreateBitmapFromRes(hResSrc, _AT("Scr_main_btn_2_p.img"));
	lpData->hBm_btn_03_n = atUIAPI_CreateBitmapFromRes(hResSrc, _AT("Scr_main_btn_3_n.img"));
	lpData->hBm_btn_03_p = atUIAPI_CreateBitmapFromRes(hResSrc, _AT("Scr_main_btn_3_p.img"));
	lpData->hBm_btn_setting = atUIAPI_CreateBitmapFromRes(hResSrc, _AT("Scr_main_setting.img"));

	lpData->hBm_btn_01_res_n = atUIAPI_CreateBitmapFromRes(hResSrc, _AT("Scr_main_btn_res_1_n.img"));
	lpData->hBm_btn_01_res_p = atUIAPI_CreateBitmapFromRes(hResSrc, _AT("Scr_main_btn_res_p.img"));
	lpData->hBm_btn_02_res_n = atUIAPI_CreateBitmapFromRes(hResSrc, _AT("Scr_main_btn_res_2_n.img"));
	lpData->hBm_btn_02_res_p = atUIAPI_CreateBitmapFromRes(hResSrc, _AT("Scr_main_btn_res_p.img"));

	lpData->ctlPanel = ExtPkg_CreatePanel(hWnd, 200, 0, 0, APP_SCREEN_WIDTH, APP_SCREEN_HEIGHT, 0);
	atCONTROL_SetAnchor(lpData->ctlPanel, 0, 0, 0, 0);

	hWidget = BasePkg_CreateImageBox(lpData->ctlPanel, 3000, 0, 0, 320, 480, lpData->hBm_bg, 1);
	atPANEL_AddControl(lpData->ctlPanel, hWidget);

	hWidget = BasePkg_CreateImageBox(lpData->ctlPanel, 3000, 276, 8, 37, 23, lpData->hBm_battery, 1);
	atPANEL_AddControl(lpData->ctlPanel, hWidget);

	hWidget = ExtPkg_CreateImageButton(lpData->ctlPanel, 1100, 0, 30, 72, 139, lpData->hBm_btn_setting, atNULL, atNULL);
	atPANEL_AddControl(lpData->ctlPanel, hWidget);

	hWidget = ExtPkg_CreateImageButton(lpData->ctlPanel, 1001, 87, 41, 233, 139, lpData->hBm_btn_01_n, lpData->hBm_btn_01_p, atNULL);
	atPANEL_AddControl(lpData->ctlPanel, hWidget);

	hWidget = ExtPkg_CreateImageButton(lpData->ctlPanel, 1002, 87, 191, 233, 139, lpData->hBm_btn_02_n, lpData->hBm_btn_02_p, atNULL);
	atPANEL_AddControl(lpData->ctlPanel, hWidget);

	hWidget = ExtPkg_CreateImageButton(lpData->ctlPanel, 1003, 87, 341, 233, 139, lpData->hBm_btn_03_n, lpData->hBm_btn_03_p, atNULL);
	atPANEL_AddControl(lpData->ctlPanel, hWidget);



	hWidget = ExtPkg_CreateImageButton(lpData->ctlPanel, 1004, 16, 124, 39, 42, lpData->hBm_btn_01_res_n, lpData->hBm_btn_01_res_p, atNULL);
	atPANEL_AddControl(lpData->ctlPanel, hWidget);

	hWidget = ExtPkg_CreateImageButton(lpData->ctlPanel, 1005, 16, 276, 39, 42, lpData->hBm_btn_02_res_n, lpData->hBm_btn_02_res_p, atNULL);
	atPANEL_AddControl(lpData->ctlPanel, hWidget);

	atWINDOW_AddTimer(hWnd, 0x100, 60000, atTRUE, atTRUE, atTRUE);
}


static void MainWndOnRelease(atHWINDOW hWnd)
{
    LPMAINWNDDATA	lpData = (LPMAINWNDDATA)atWINDOW_GetDataPtr(hWnd);

	atUIAPI_DestroyBitmap(lpData->hBm_bg);
	atUIAPI_DestroyBitmap(lpData->hBm_battery);

	atUIAPI_DestroyBitmap(lpData->hBm_btn_01_n);
	atUIAPI_DestroyBitmap(lpData->hBm_btn_02_n);
	atUIAPI_DestroyBitmap(lpData->hBm_btn_03_n);
	atUIAPI_DestroyBitmap(lpData->hBm_btn_01_p);
	atUIAPI_DestroyBitmap(lpData->hBm_btn_02_p);
	atUIAPI_DestroyBitmap(lpData->hBm_btn_03_p);

	atUIAPI_DestroyBitmap(lpData->hBm_btn_01_res_n);
	atUIAPI_DestroyBitmap(lpData->hBm_btn_01_res_p);
	atUIAPI_DestroyBitmap(lpData->hBm_btn_02_res_n);
	atUIAPI_DestroyBitmap(lpData->hBm_btn_02_res_p);

	atUIAPI_DestroyBitmap(lpData->hBm_btn_setting);
}



static int MainWndOnKeyDown(atHWINDOW hWnd, int nKeyCode)
{
    LPMAINWNDDATA lpData = (LPMAINWNDDATA)atWINDOW_GetDataPtr(hWnd);
	atHWINDOW hPopup;

    switch( nKeyCode )
    {
		case atVKEY_SOFT1:
//			hPopup = ExtPkg_CreatePopupMenuWnd( hWnd, 2000, 0, 0, 200, 100, xx, 3, 3, MENU_ALIGN_LEFT | MENU_ALIGN_BOTTOM );
//			atPOPUPMENU_AddSubMenu( hPopup, 0, 100, setting_theme, 2, 0 );
//			atWINDOW_Draw(hPopup,atTRUE);			
//			atUIAPI_FlushScreen();
//			atAPP_RedrawAllWindow(atAPPMNG_GetCurApp());			
//			atWINDOW_Refresh(hPopup);
			break;

//        case atVKEY_CLEAR:
//        case atVKEY_MENU:
//            atAPP_DestroyWindow( atAPPMNG_GetCurApp(), hWnd );
//            break;

/*            
        case atVKEY_FF:
        case atVKEY_REW:
			lpData->idx ++;
			if( lpData->idx >= 2 ) lpData->idx = 0;
			atWINDOW_Draw( hWnd, atTRUE );
            break;
*/
			
        default:
            return 0;
    }

    return 1; // WindowOnEvent 가 호출되지 않도록 0을 리턴한다.
}


static void MainWndOnPaint(atHWINDOW hWnd)
{
}



int MainWndOnEvent(atHWINDOW hWnd, int nMsg, long nParam1, long nParam2)
{
	switch( nMsg )
	{
		case WNDMSG_INIT:
			MainWndOnInit( hWnd );
			break;

		case WNDMSG_RELEASE:
			MainWndOnRelease( hWnd );
			break;

		case WNDMSG_PAINT:
			MainWndOnPaint( hWnd );
			break;

		case WNDMSG_KEYDOWN:
			return MainWndOnKeyDown( hWnd, nParam1 );
			break;

		case WNDMSG_COMMAND:
			MainWndOnCommand( hWnd, nParam1, nParam2 );
			break;

		case WNDMSG_TIMER:
			MainWndOnTimer( hWnd, nParam1, nParam2);
			break;

		default:
			return 0;
			break;
	}

	return 1;
}
