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
#include "Screen_setting.h"

#include <stdio.h>

static void refreshWindows(atHWINDOW hWnd)
{
	LPSCR_SETTING_WNDDATA lpData = (LPSCR_SETTING_WNDDATA)atWINDOW_GetDataPtr(hWnd);

	SETTINGDEVICE *devVal = GetDeviceValues();
	if (devVal != atNULL)
		memcpy(&defSetting, devVal, sizeof(SETTINGDEVICE));

	lpData->nIndexDist = 0;
	lpData->nTempBattery = GetBatteryValue();

	atWINDOW_RefreshClient(hWnd);
}

static void Scr_Setting_WndOnCommand(atHWINDOW hWnd, long nParam1, long nParam2)
{
	LPSCR_SETTING_WNDDATA lpData = (LPSCR_SETTING_WNDDATA)atWINDOW_GetDataPtr(hWnd);

	switch (nParam1)
	{
	case 100:
		atAPP_DestroyWindow(atAPPMNG_GetCurApp(), hWnd);
		break;

	case 1000:
		atAPP_DestroyWindow(atAPPMNG_GetCurApp(), hWnd);
		break;

	case 1001:
	{
		defSetting.sig = D_SIGNITURE;
 		SetDeviceValues(defSetting);

		lpData->bInit = atFALSE;
		refreshWindows(hWnd);
	}
		break;

	case 1020:
	{
		// refresh
		InitDefaultDeviceValues();

		lpData->bInit = atTRUE;
		refreshWindows(hWnd);
	}
		break;

	case 1021:
	case 1022:
	case 1023:
	case 1024:
	case 1025:
	{
		defSetting.nBright = nParam1 - 1020;
		SetBackLight(defSetting.nBright);
		atWINDOW_RefreshClient(hWnd);
	}
	break;

	// Distance
	case 1030:
	{
		if (lpData->nIndexDist > 0) {
			lpData->nIndexDist--;

			switch (lpData->nIndexDist)
			{
			case 39:
				lpData->nIndexDist = 35;
				break;

			case 49:
				lpData->nIndexDist = 40;
				break;

			case 59:
				lpData->nIndexDist = 50;
				break;

			case 69:
				lpData->nIndexDist = 60;
				break;
			}
		}
		
		atWINDOW_RefreshClient(hWnd);
	}
		break;

	case 1031:
	{
		if (lpData->nIndexDist < 69) {
			lpData->nIndexDist++;

			switch (lpData->nIndexDist)
			{
			case 36:
				lpData->nIndexDist = 40;
				break;

			case 41:
				lpData->nIndexDist = 50;
				break;

			case 51:
				lpData->nIndexDist = 60;
				break;

			case 61:
				lpData->nIndexDist = 70;
				break;
			}
		}
			
		atWINDOW_RefreshClient(hWnd);
	}
		break;

	// Battery
	case 1040:
	{
		defSetting.nBattery -= 10;
		atWINDOW_RefreshClient(hWnd);
	}
		break;

	case 1041:
	{
		defSetting.nBattery += 10;
		atWINDOW_RefreshClient(hWnd);
	}
		break;

	// Message Check
	case 1050:
	{
		defSetting.bPrintMsg = atTRUE;
		atWINDOW_RefreshClient(hWnd);
	}
		break;

	case 1051:
	{
		defSetting.bPrintMsg = atFALSE;
		atWINDOW_RefreshClient(hWnd);
	}
		break;

	/*case 1031:
	case 1032:
	case 1033:
	{
		//lpData->nIdxDist = nParam1 - 1030;
		atWINDOW_RefreshClient(hWnd);
	}
	break;*/
	}
}


static void Scr_Setting_WndOnChange(atHWINDOW hWnd, long nParam1, long nParam2)
{
	LPSCR_SETTING_WNDDATA lpData = (LPSCR_SETTING_WNDDATA)atWINDOW_GetDataPtr(hWnd);

    switch(nParam1)
    {
        case 103 :

			break;
	}
}


static void Scr_Setting_WndOnInit(atHWINDOW hWnd, long nParam)
{
	LPSCR_SETTING_WNDDATA lpData = (LPSCR_SETTING_WNDDATA)atWINDOW_GetDataPtr(hWnd);

	atHCONTROL ctl;
	atHCONTROL hWidget;

	atHRESSRC hResSrc = atUIAPI_GetResSource(_AT("TESTMAIN"));
	lpData->hBm_bg = atUIAPI_CreateBitmapFromRes(hResSrc, _AT("Scr_1_bg.img"));
	lpData->hBm_title = atUIAPI_CreateBitmapFromRes(hResSrc, _AT("Scr_set_title.img"));

	lpData->hBm_btn_home_n = atUIAPI_CreateBitmapFromRes(hResSrc, _AT("Scr_def_btn_home_n.img"));
	lpData->hBm_btn_check_n = atUIAPI_CreateBitmapFromRes(hResSrc, _AT("Scr_def_btn_check_n.img"));
	lpData->hBm_btn_home_p = atUIAPI_CreateBitmapFromRes(hResSrc, _AT("Scr_def_btn_home_p.img"));
	lpData->hBm_btn_check_p = atUIAPI_CreateBitmapFromRes(hResSrc, _AT("Scr_def_btn_check_p.img"));

	lpData->hBm_bright_1 = atUIAPI_CreateBitmapFromRes(hResSrc, _AT("Scr_set_bright_1.img"));
	lpData->hBm_bright_2 = atUIAPI_CreateBitmapFromRes(hResSrc, _AT("Scr_set_bright_2.img"));
	lpData->hBm_bright_3 = atUIAPI_CreateBitmapFromRes(hResSrc, _AT("Scr_set_bright_3.img"));
	lpData->hBm_bright_4 = atUIAPI_CreateBitmapFromRes(hResSrc, _AT("Scr_set_bright_4.img"));
	lpData->hBm_bright_5 = atUIAPI_CreateBitmapFromRes(hResSrc, _AT("Scr_set_bright_5.img"));

	lpData->hBm_btn_dist_l = atUIAPI_CreateBitmapFromRes(hResSrc, _AT("Scr_def_btn_left_s.img"));
	lpData->hBm_btn_dist_r = atUIAPI_CreateBitmapFromRes(hResSrc, _AT("Scr_def_btn_right_s.img"));
	lpData->hBm_btn_battery_low = atUIAPI_CreateBitmapFromRes(hResSrc, _AT("Scr_def_btn_left_s.img"));
	lpData->hBm_btn_battery_high = atUIAPI_CreateBitmapFromRes(hResSrc, _AT("Scr_def_btn_right_s.img"));

	lpData->hBm_btn_msg_on = atUIAPI_CreateBitmapFromRes(hResSrc, _AT("Scr_set_btn_check_on.img"));
	lpData->hBm_btn_msg_off = atUIAPI_CreateBitmapFromRes(hResSrc, _AT("Scr_set_btn_check_off.img"));

	hWidget = ExtPkg_CreateImageButton(hWnd, 1020, 96, 64, 126, 34, lpData->hBm_title, atNULL, atNULL);
	atCONTROL_SetFocusable(hWidget, atFALSE);
	hWidget = ExtPkg_CreateImageButton(hWnd, 1021, 38, 150, 36, 36, lpData->hBm_bright_1, atNULL, atNULL);
	atCONTROL_SetFocusable(hWidget, atFALSE);
	hWidget = ExtPkg_CreateImageButton(hWnd, 1022, 89, 150, 36, 36, lpData->hBm_bright_2, atNULL, atNULL);
	atCONTROL_SetFocusable(hWidget, atFALSE);
	hWidget = ExtPkg_CreateImageButton(hWnd, 1023, 143, 150, 36, 36, lpData->hBm_bright_3, atNULL, atNULL);
	atCONTROL_SetFocusable(hWidget, atFALSE);
	hWidget = ExtPkg_CreateImageButton(hWnd, 1024, 195, 150, 36, 36, lpData->hBm_bright_4, atNULL, atNULL);
	atCONTROL_SetFocusable(hWidget, atFALSE);
	hWidget = ExtPkg_CreateImageButton(hWnd, 1025, 247, 150, 36, 36, lpData->hBm_bright_5, atNULL, atNULL);
	atCONTROL_SetFocusable(hWidget, atFALSE);

	hWidget = ExtPkg_CreateImageButton(hWnd, 1030, 225, 228, 30, 25, lpData->hBm_btn_dist_l, atNULL, atNULL);
	atCONTROL_SetFocusable(hWidget, atFALSE);
	hWidget = ExtPkg_CreateImageButton(hWnd, 1031, 270, 228, 30, 25, lpData->hBm_btn_dist_r, atNULL, atNULL);
	atCONTROL_SetFocusable(hWidget, atFALSE);

	hWidget = ExtPkg_CreateImageButton(hWnd, 1040, 195, 326, 30, 25, lpData->hBm_btn_battery_low, atNULL, atNULL);
	atCONTROL_SetFocusable(hWidget, atFALSE);
	hWidget = ExtPkg_CreateImageButton(hWnd, 1041, 240, 326, 30, 25, lpData->hBm_btn_battery_high, atNULL, atNULL);
	atCONTROL_SetFocusable(hWidget, atFALSE);

	hWidget = ExtPkg_CreateImageButton(hWnd, 1050, 180, 267, 51, 25, lpData->hBm_btn_msg_on, atNULL, atNULL);
	atCONTROL_SetFocusable(hWidget, atFALSE);
	hWidget = ExtPkg_CreateImageButton(hWnd, 1051, 240, 267, 51, 25, lpData->hBm_btn_msg_off, atNULL, atNULL);
	atCONTROL_SetFocusable(hWidget, atFALSE);

	hWidget = ExtPkg_CreateImageButton(hWnd, 1000, 0, 399, 161, 82, lpData->hBm_btn_home_n, lpData->hBm_btn_home_p, atNULL);
	atCONTROL_SetFocusable(hWidget, atFALSE);
	hWidget = ExtPkg_CreateImageButton(hWnd, 1001, 160, 399, 161, 82, lpData->hBm_btn_check_n, lpData->hBm_btn_check_p, atNULL);
	atCONTROL_SetFocusable(hWidget, atFALSE);

	lpData->hFnt_main = atUIAPI_CreateFont(atUIAPI_FACE_SYSTEM, 18, atUIAPI_STYLE_PLAIN, atFALSE);
	lpData->hFnt_sub = atUIAPI_CreateFont(atUIAPI_FACE_SYSTEM, 14, atUIAPI_STYLE_PLAIN, atFALSE);

	refreshWindows(hWnd);
}



static void Scr_Setting_WndOnRelease(atHWINDOW hWnd)
{
	LPSCR_SETTING_WNDDATA	lpData = (LPSCR_SETTING_WNDDATA)atWINDOW_GetDataPtr(hWnd);

	atUIAPI_DestroyBitmap(lpData->hBm_bg);
	atUIAPI_DestroyBitmap(lpData->hBm_title);

	atUIAPI_DestroyBitmap(lpData->hBm_bright_1);
	atUIAPI_DestroyBitmap(lpData->hBm_bright_2);
	atUIAPI_DestroyBitmap(lpData->hBm_bright_3);
	atUIAPI_DestroyBitmap(lpData->hBm_bright_4);
	atUIAPI_DestroyBitmap(lpData->hBm_bright_5);

	atUIAPI_DestroyBitmap(lpData->hBm_btn_dist_l);
	atUIAPI_DestroyBitmap(lpData->hBm_btn_dist_r);

	atUIAPI_DestroyBitmap(lpData->hBm_btn_battery_low);
	atUIAPI_DestroyBitmap(lpData->hBm_btn_battery_high);

	atUIAPI_DestroyBitmap(lpData->hBm_btn_msg_on);
	atUIAPI_DestroyBitmap(lpData->hBm_btn_msg_off);

	atUIAPI_DestroyBitmap(lpData->hBm_btn_home_n);
	atUIAPI_DestroyBitmap(lpData->hBm_btn_home_p);
	atUIAPI_DestroyBitmap(lpData->hBm_btn_check_n);
	atUIAPI_DestroyBitmap(lpData->hBm_btn_check_p);

	atUIAPI_DestroyFont(lpData->hFnt_main);	
	atUIAPI_DestroyFont(lpData->hFnt_sub);
}


static int Scr_Setting_WndOnKeyDown(atHWINDOW hWnd, int nKeyCode)
{
	LPSCR_SETTING_WNDDATA lpData = (LPSCR_SETTING_WNDDATA)atWINDOW_GetDataPtr(hWnd);
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

		case USERKEY_1:
		{
			int i = 0;

			defSetting.nDist[lpData->nIndexDist] = GetVolategeValue();

			if ( lpData->nIndexDist == 35 ) {
				for (i = lpData->nIndexDist; i < (lpData->nIndexDist + 5); i++)
					defSetting.nDist[i] = defSetting.nDist[lpData->nIndexDist];
			}

			if ((lpData->nIndexDist == 35) ||
				(lpData->nIndexDist == 40) ||
				(lpData->nIndexDist == 50) ||
				(lpData->nIndexDist == 60) ||
				(lpData->nIndexDist == 70)) {
				for (i = lpData->nIndexDist; i < (lpData->nIndexDist + 10); i++)
					defSetting.nDist[i] = defSetting.nDist[lpData->nIndexDist];
			}

			atWINDOW_Refresh(hWnd);
		}
			break;
			
        default:
            return 0;
    }

    return 0; // WindowOnEvent 가 호출되지 않도록 0을 리턴한다.
}


static int Scr_Setting_WndOnKeyPress(atHWINDOW hWnd, int nKeyCode)
{
	LPSCR_SETTING_WNDDATA lpData = (LPSCR_SETTING_WNDDATA)atWINDOW_GetDataPtr(hWnd);

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

		case atVKEY_SELECT:
		{
		
		}
		break;
			
        default:
            return 0;
    }

    return 1; // WindowOnEvent 가 호출되지 않도록 0을 리턴한다.
}


static void Scr_Setting_WndOnPaint(atHWINDOW hWnd)
{
	LPSCR_SETTING_WNDDATA lpData = (LPSCR_SETTING_WNDDATA)atWINDOW_GetDataPtr(hWnd);
	atHGDC gdc = atWINDOW_GetClientGDC(hWnd);
	atREGION rgn;
	atHFONT oldfont;
	atTCHAR str[64];

	atUIAPI_DrawImage(gdc, 0, 0, lpData->hBm_bg);
	//atUIAPI_DrawImage(gdc, 96, 64, lpData->hBm_title);

	atWINDOW_GetClientRegion(hWnd, &rgn);

	oldfont = atUIAPI_SetFont(gdc, lpData->hFnt_main);

	/* BACK Light */
	atUIAPI_sprintf(str, _AT("백라이트"));
	atUIAPI_DrawStringAlign(gdc, 30, 130, str, ALIGN_LEFT_MIDDLE, atUI_COLOR_BLACK);

	atUIAPI_sprintf(str, _AT("거리보정"));
	atUIAPI_DrawStringAlign(gdc, 30, 220, str, ALIGN_LEFT_MIDDLE, atUI_COLOR_BLACK);

	atUIAPI_sprintf(str, _AT("측정거리 표시"));
	atUIAPI_DrawStringAlign(gdc, 30, 280, str, ALIGN_LEFT_MIDDLE, atUI_COLOR_BLACK);

	atUIAPI_sprintf(str, _AT("배터리"));
	atUIAPI_DrawStringAlign(gdc, 30, 320, str, ALIGN_LEFT_MIDDLE, atUI_COLOR_BLACK);

	atUIAPI_SetFont(gdc, oldfont);


	oldfont = atUIAPI_SetFont(gdc, lpData->hFnt_sub);

	/* BACK Light */
	atUIAPI_sprintf(str, _AT("%d"), defSetting.nBright);
	atUIAPI_DrawStringAlign(gdc, 110, 130, str, ALIGN_LEFT_MIDDLE, atUI_COLOR_DARKGRAY);

	/* 거리보정 */
	atUIAPI_sprintf(str, _AT("%d cm"), lpData->nIndexDist + 10);
	atUIAPI_DrawStringAlign(gdc, 30, 240, str, ALIGN_LEFT_MIDDLE, atUI_COLOR_DARKGRAY);

	if(lpData->nIndexDist == 0)
		atUIAPI_sprintf(str, _AT("%d"), defSetting.nDist[lpData->nIndexDist]);
	else 
		atUIAPI_sprintf(str, _AT("%d"), defSetting.nDist[lpData->nIndexDist - 1]);
	atUIAPI_DrawStringAlign(gdc, 95, 240, str, ALIGN_LEFT_MIDDLE, atUI_COLOR_DARKGRAY);

	atUIAPI_sprintf(str, _AT("설정"));
	atUIAPI_DrawStringAlign(gdc, 150, 240, str, ALIGN_LEFT_MIDDLE, atUI_COLOR_BLACK);

	atUIAPI_sprintf(str, _AT("%d"), defSetting.nDist[lpData->nIndexDist]);
	atUIAPI_DrawStringAlign(gdc, 185, 240, str, ALIGN_LEFT_MIDDLE, atUI_COLOR_DARKGRAY);

	if (lpData->nIndexDist >= 79) {
		atUIAPI_sprintf(str, _AT("FINISH"));
		atUIAPI_DrawStringAlign(gdc, 250, 240, str, ALIGN_LEFT_MIDDLE, atUI_COLOR_RED);
	}

	/* 측정거리 표시 */
	if(defSetting.bPrintMsg)
		atUIAPI_sprintf(str, _AT("ON"));
	else
		atUIAPI_sprintf(str, _AT("OFF"));
	atUIAPI_DrawStringAlign(gdc, 150, 280, str, ALIGN_LEFT_MIDDLE, atUI_COLOR_DARKGRAY);

	/* Battery */

	atUIAPI_sprintf(str, _AT("%d"), lpData->nTempBattery);
	atUIAPI_DrawStringAlign(gdc, 30, 338, str, ALIGN_LEFT_MIDDLE, atUI_COLOR_DARKGRAY);

	atUIAPI_sprintf(str, _AT("설정"));
	atUIAPI_DrawStringAlign(gdc, 110, 338, str, ALIGN_LEFT_MIDDLE, atUI_COLOR_BLACK);

	atUIAPI_sprintf(str, _AT("%d"), defSetting.nBattery);
	atUIAPI_DrawStringAlign(gdc, 145, 338, str, ALIGN_LEFT_MIDDLE, atUI_COLOR_DARKGRAY);

	atUIAPI_SetFont(gdc, oldfont);

	/**
	 * SUB FONT
	 */
	oldfont = atUIAPI_SetFont(gdc, lpData->hFnt_sub);

	// INIT
	if (lpData->bInit) {
		atUIAPI_sprintf(str, _AT("INITIALIZED"));
		atUIAPI_DrawStringAlign(gdc, 225, 83, str, ALIGN_LEFT_MIDDLE, atUI_COLOR_RED);
	}

	// LOG
	atUIAPI_sprintf(str, _AT("[Version 1.5]"));
	atUIAPI_DrawStringAlign(gdc, 30, 365, str, ALIGN_LEFT_MIDDLE, atUI_COLOR_GRAY);

	atUIAPI_SetFont(gdc, oldfont);
}


int Scr_Setting_WndOnEvent(atHWINDOW hWnd, int nMsg, long nParam1, long nParam2)
{
	int ret = 0;
	LPSCR_SETTING_WNDDATA	lpData = (LPSCR_SETTING_WNDDATA)atWINDOW_GetDataPtr(hWnd);
	LPSCR_SETTING_WNDDATA  Data = (LPSCR_SETTING_WNDDATA)nParam1;

	switch( nMsg )
	{
		case WNDMSG_INIT:
		{
			lpData->nType = Data->nType;
			Scr_Setting_WndOnInit(hWnd, nParam1);
		}
			break;
		case WNDMSG_RELEASE:
			Scr_Setting_WndOnRelease( hWnd );
			break;
		case WNDMSG_PAINT:
			Scr_Setting_WndOnPaint( hWnd );
			break;
		case WNDMSG_KEYDOWN:
			return Scr_Setting_WndOnKeyDown( hWnd, nParam1 );
			break;
		case WNDMSG_KEYPRESS:
			return Scr_Setting_WndOnKeyPress( hWnd, nParam1 );
			break;
		case WNDMSG_CHANGE:
			Scr_Setting_WndOnChange( hWnd, nParam1, nParam2 );
			break;
		case WNDMSG_COMMAND:
			Scr_Setting_WndOnCommand( hWnd, nParam1, nParam2 );
			break;
		default:
			return 0;
			break;
	}

	return ret;
}
