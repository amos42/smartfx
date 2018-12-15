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
#include "Screen_Calculate.h"
#include "Screen_3_result.h"

SCR_3_RESULT_WNDDATA resData;

static void Scr_Calculate_WndOnCommand(atHWINDOW hWnd, long nParam1, long nParam2)
{
	LPSCR_CALCULATE_WNDDATA lpData = (LPSCR_CALCULATE_WNDDATA)atWINDOW_GetDataPtr(hWnd);
  
    switch(nParam1)
    {
		case 1001:
		{
			if( (lpData->nType == 102) || (lpData->nType == 104) || (lpData->nType == 301) )
				LedControl(atFALSE);

			if (lpData->nType == 304) {
				memset(&resData, 0, sizeof(SCR_3_RESULT_WNDDATA));

				atHWINDOW wnd;
				wnd = atAPP_CreateWindow(atAPPMNG_GetCurApp(), atNULL, 0, atNULL, _AT(""), 0, 0, 0, 0, sizeof(SCR_3_RESULT_WNDDATA), Scr_3_Result_WndOnEvent, &resData, atNULL);
				atWINDOW_SetAnchor(wnd, 0, 0, 0, 0);
				atAPP_ShowWindow(atAPPMNG_GetCurApp(), wnd, atTRUE);
			}

			atAPP_DestroyWindow(atAPPMNG_GetCurApp(), hWnd);
		}
			break;
	}
}

static void Scr_Calculate_WndOnChange(atHWINDOW hWnd, long nParam1, long nParam2)
{
	LPSCR_CALCULATE_WNDDATA lpData = (LPSCR_CALCULATE_WNDDATA)atWINDOW_GetDataPtr(hWnd);

    switch(nParam1)
    {
        case 103 :

			break;
	}
}

static void Scr_Calculate_WndOnTimer(atHWINDOW hWnd, long nParam1, long nParam2)
{
	LPSCR_CALCULATE_WNDDATA lpData = (LPSCR_CALCULATE_WNDDATA)atWINDOW_GetDataPtr(hWnd);

	int value = GetDistanceValue();

	switch (nParam2)
	{
		case 0x100: 
		{
			switch (lpData->nType) {
				case 301:
				{
					if ((value > 26) && (value < 34))
						LedControl(atFALSE);
					else
						LedControl(atTRUE);
				}
					break;

				case 302:
				case 303:
				case 304:
				case 401:
				case 402:
				case 403:
				case 404:
				case 405:
				{
					if ((value > 36) && (value < 44))
						LedControl(atFALSE);
					else
						LedControl(atTRUE);
				}
				break;
			}



			atWINDOW_Refresh(hWnd);
		}
			break;
	}

	
}


static void Scr_Calculate_WndOnInit(atHWINDOW hWnd, long nParam)
{
	LPSCR_CALCULATE_WNDDATA lpData = (LPSCR_CALCULATE_WNDDATA)atWINDOW_GetDataPtr(hWnd);

	atHCONTROL ctl;
	atHCONTROL hWidget;

	atHRESSRC hResSrc = atUIAPI_GetResSource(_AT("TESTMAIN"));

	bPrintMsg = defSetting.bPrintMsg;
	lpData->nVoltage = 0;

	switch (lpData->nType)
	{
		case 101:
		case 103:
			lpData->hBm_btn_img = atUIAPI_CreateBitmapFromRes(hResSrc, _AT("Scr_1_divid_center.img"));
			break;

		case 105:
		case 106:
			lpData->hBm_btn_img = atUIAPI_CreateBitmapFromRes(hResSrc, _AT("Scr_2_calculate_content_w.img"));
			break;

		case 107:
			lpData->hBm_btn_img = atUIAPI_CreateBitmapFromRes(hResSrc, _AT("Scr_2_read_content_big.img"));
			break;

		case 301:
			lpData->hBm_btn_img = atUIAPI_CreateBitmapFromRes(hResSrc, _AT("Scr_3_special_img_test_1_big.img"));
			break;

		case 302:
			lpData->hBm_btn_img = atUIAPI_CreateBitmapFromRes(hResSrc, _AT("Scr_3_special_img_test_2.img"));
			break;

		case 303:
			lpData->hBm_btn_img = atUIAPI_CreateBitmapFromRes(hResSrc, _AT("Scr_3_special_img_test_3.img"));
			break;

		case 304:
			lpData->hBm_btn_img = atUIAPI_CreateBitmapFromRes(hResSrc, _AT("Scr_3_special_img_test_4_big.img"));
			break;

		case 305:
			lpData->hBm_btn_img = atUIAPI_CreateBitmapFromRes(hResSrc, _AT("Scr_3_special_img_test_5.img"));
			break;

		case 306:
			lpData->hBm_btn_img = atUIAPI_CreateBitmapFromRes(hResSrc, _AT("Scr_3_special_img_test_6.img"));
			break;

		case 307:
			lpData->hBm_btn_img = atUIAPI_CreateBitmapFromRes(hResSrc, _AT("Scr_3_special_img_test_7.img"));
			break;

		case 401:
			lpData->hBm_btn_img = atUIAPI_CreateBitmapFromRes(hResSrc, _AT("Scr_3_vision_img_test_1.img"));
			break;

		case 402:
			lpData->hBm_btn_img = atUIAPI_CreateBitmapFromRes(hResSrc, _AT("Scr_3_vision_img_test_2.img"));
			break;

		case 403:
			lpData->hBm_btn_img = atUIAPI_CreateBitmapFromRes(hResSrc, _AT("Scr_3_vision_img_test_3.img"));
			break;

		case 404:
			lpData->hBm_btn_img = atUIAPI_CreateBitmapFromRes(hResSrc, _AT("Scr_3_vision_img_test_4.img"));
			break;

		case 405:
			lpData->hBm_btn_img = atUIAPI_CreateBitmapFromRes(hResSrc, _AT("Scr_3_vision_img_test_5.img"));
			break;
	}

	switch (lpData->nType)
	{
		case 105:
		case 106:
		case 401:
		case 402:
		case 403:
			lpData->hBm_btn_ok = atUIAPI_CreateBitmapFromRes(hResSrc, _AT("Scr_def_btn_bottom_white.img"));
			break;

		default:
			lpData->hBm_btn_ok = atUIAPI_CreateBitmapFromRes(hResSrc, _AT("Scr_def_btn_bottom_black.img"));
			break;
	}	

	hWidget = ExtPkg_CreateImageButton(hWnd, 1001, 0, 420, 320, 60, lpData->hBm_btn_ok, lpData->hBm_btn_ok, atNULL);
	atCONTROL_SetFocusable(hWidget, atFALSE);

	lpData->hFnt_main = atUIAPI_CreateFont(atUIAPI_FACE_SYSTEM, 14, atUIAPI_STYLE_PLAIN, atFALSE);

	atWINDOW_AddTimer(hWnd, 0x100, 500, atTRUE, atTRUE, atTRUE);

	switch (lpData->nType)
	{
		case 102:
		case 104:
		case 301:
		case 302:
		case 303:
		case 304:
		case 401:
		case 402:
		case 403:
		case 404:
		case 405:
		{
			LedControl(atTRUE);
		}
		break;
	}
}



static void Scr_Calculate_WndOnRelease(atHWINDOW hWnd)
{
	LPSCR_CALCULATE_WNDDATA	lpData = (LPSCR_CALCULATE_WNDDATA)atWINDOW_GetDataPtr(hWnd);

	atUIAPI_DestroyBitmap(lpData->hBm_btn_img);

	atUIAPI_DestroyFont(lpData->hFnt_main);	
}


static int Scr_Calculate_WndOnKeyDown(atHWINDOW hWnd, int nKeyCode)
{
	LPSCR_CALCULATE_WNDDATA lpData = (LPSCR_CALCULATE_WNDDATA)atWINDOW_GetDataPtr(hWnd);

	switch( nKeyCode )
	{
		case atVKEY_SOFT1:
			break;
			
		case atVKEY_SOFT2:
			break;

		case atVKEY_CLEAR:
			atAPP_DestroyWindow( atAPPMNG_GetCurApp(), hWnd );
			break;

		// Hardware key
		case atVKEY_SELECT :
			break;

		case USERKEY_1:
		{
			lpData->nVoltage = GetVolategeValue();

			switch (lpData->nType)
			{
			case 101:
				nDistDivid = GetDistanceValue();
				break;

			case 102:
				nDistPenDivid = GetDistanceValue();
				break;

			case 103:
				nDistRecover = GetDistanceValue();
				break;

			case 104:
				nDistPenRecover = GetDistanceValue();
				break;

			case 105:
				nDistRight = GetDistanceValue();
				break;

			case 106:
				nDistLeft = GetDistanceValue();
				break;

			case 107:
				nDistRead = GetDistanceValue();
				break;

			case 301:
				nDistChart = GetDistanceValue();
				break;

			case 302:
				nDistChart = GetDistanceValue();
				break;

			case 303:
				nDistChart = GetDistanceValue();
				break;

			case 304:
				nDistChart = GetDistanceValue();
				break;

			case 305:
				nDistChart = GetDistanceValue();
				break;

			case 306:
				nDistChart = GetDistanceValue();
				break;

			case 307:
				nDistChart = GetDistanceValue();
				break;

			case 401:
				nDistChart = GetDistanceValue();
				break;

			case 402:
				nDistChart = GetDistanceValue();
				break;

			case 403:
				nDistChart = GetDistanceValue();
				break;

			case 404:
				nDistChart = GetDistanceValue();
				break;

			case 405:
				nDistChart = GetDistanceValue();
				break;
			}

			atWINDOW_Refresh(hWnd);
		}
			break;
			
		default:
			return 0;
	}

    return 0; // WindowOnEvent 가 호출되지 않도록 0을 리턴한다.
}

static int Scr_Calculate_WndOnKeyPress(atHWINDOW hWnd, int nKeyCode)
{
	LPSCR_CALCULATE_WNDDATA lpData = (LPSCR_CALCULATE_WNDDATA)atWINDOW_GetDataPtr(hWnd);

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


static void Scr_Calculate_WndOnPaint(atHWINDOW hWnd)
{
	LPSCR_CALCULATE_WNDDATA lpData = (LPSCR_CALCULATE_WNDDATA)atWINDOW_GetDataPtr(hWnd);
	atHGDC gdc = atWINDOW_GetClientGDC(hWnd);
	atREGION rgn;
	atHFONT oldfont;
	atTCHAR str[64];

	switch (lpData->nType) 
	{
		case 105:
		case 106:
		case 401:
		case 402:
		case 403:
			atUIAPI_FillRect(gdc, 0, 0, 320, 480, UI_COLOR_WHITE);
			break;

		default:
			atUIAPI_FillRect(gdc, 0, 0, 320, 480, UI_COLOR_BLACK);
			break;
	}

	switch (lpData->nType)
	{
		case 101:
			atUIAPI_DrawImage(gdc, 157, 173, lpData->hBm_btn_img);
			atUIAPI_sprintf(str, _AT("%d cm"), nDistDivid);
			break;

		case 102:
			atUIAPI_sprintf(str, _AT("%d cm"), nDistPenDivid);
			break;

		case 103:
			atUIAPI_DrawImage(gdc, 157, 173, lpData->hBm_btn_img);
			atUIAPI_sprintf(str, _AT("%d cm"), nDistRecover);
			break;

		case 104:
			atUIAPI_sprintf(str, _AT("%d cm"), nDistPenRecover);
			break;

		case 105:
			atUIAPI_DrawImage(gdc, 140, 216, lpData->hBm_btn_img);
			atUIAPI_sprintf(str, _AT("%d cm"), nDistRight);
			break;

		case 106:
			atUIAPI_DrawImage(gdc, 140, 216, lpData->hBm_btn_img);
			atUIAPI_sprintf(str, _AT("%d cm"), nDistLeft);
			break;

		case 107:
			atUIAPI_DrawImage(gdc, 0, 100, lpData->hBm_btn_img);
			atUIAPI_sprintf(str, _AT("%d cm"), nDistRead);
			break;

		case 301:
			atUIAPI_DrawImage(gdc, 70, 100, lpData->hBm_btn_img);
			atUIAPI_sprintf(str, _AT("%d cm"), nDistChart);
			break;

		case 302:
			atUIAPI_DrawImage(gdc, 153, 179, lpData->hBm_btn_img);
			atUIAPI_sprintf(str, _AT("%d cm"), nDistChart);
			break;

		case 303:
			atUIAPI_DrawImage(gdc, 130, 199, lpData->hBm_btn_img);
			atUIAPI_sprintf(str, _AT("%d cm"), nDistChart);
			break;

		case 304:
			atUIAPI_DrawImage(gdc, 70, 120, lpData->hBm_btn_img);
			atUIAPI_sprintf(str, _AT("%d cm"), nDistChart);
			break;

		case 305:
			atUIAPI_DrawImage(gdc, 125, 179, lpData->hBm_btn_img);
			atUIAPI_sprintf(str, _AT("%d cm"), nDistChart);
			break;

		case 306:
			atUIAPI_DrawImage(gdc, 125, 179, lpData->hBm_btn_img);
			atUIAPI_sprintf(str, _AT("%d cm"), nDistChart);
			break;

		case 307:
			atUIAPI_DrawImage(gdc, 125, 179, lpData->hBm_btn_img);
			atUIAPI_sprintf(str, _AT("%d cm"), nDistChart);
			break;

		case 401:
			atUIAPI_DrawImage(gdc, 125, 179, lpData->hBm_btn_img);
			atUIAPI_sprintf(str, _AT("%d cm"), nDistChart);
			break;

		case 402:
			atUIAPI_DrawImage(gdc, 125, 179, lpData->hBm_btn_img);
			atUIAPI_sprintf(str, _AT("%d cm"), nDistChart);
			break;

		case 403:
			atUIAPI_DrawImage(gdc, 125, 179, lpData->hBm_btn_img);
			atUIAPI_sprintf(str, _AT("%d cm"), nDistChart);
			break;

		case 404:
			atUIAPI_DrawImage(gdc, 125, 179, lpData->hBm_btn_img);
			atUIAPI_sprintf(str, _AT("%d cm"), nDistChart);
			break;

		case 405:
			atUIAPI_DrawImage(gdc, 125, 179, lpData->hBm_btn_img);
			atUIAPI_sprintf(str, _AT("%d cm"), nDistChart);
			break;
	}


	switch (lpData->nType) 
	{
		case 105:
		case 106:
		case 401:
		case 402:
		case 403:
		{
			if (bPrintMsg) {
				atWINDOW_GetClientRegion(hWnd, &rgn);
				oldfont = atUIAPI_SetFont(gdc, lpData->hFnt_main);
				atUIAPI_DrawString(gdc, 50, 380, str, atUI_COLOR_BLACK);

				atUIAPI_sprintf(str, _AT("%d V"), lpData->nVoltage);
				atUIAPI_DrawString(gdc, 50, 400, str, atUI_COLOR_BLACK);
				atUIAPI_SetFont(gdc, oldfont);

				atUIAPI_sprintf(str, _AT("%d cm"), GetDistanceValue());

				atWINDOW_GetClientRegion(hWnd, &rgn);
				oldfont = atUIAPI_SetFont(gdc, lpData->hFnt_main);
				atUIAPI_DrawString(gdc, 230, 380, str, atUI_COLOR_BLACK);
				atUIAPI_SetFont(gdc, oldfont);
			}
		}
			break;

		default:
		{
			if (bPrintMsg) {
				atWINDOW_GetClientRegion(hWnd, &rgn);
				oldfont = atUIAPI_SetFont(gdc, lpData->hFnt_main);
				atUIAPI_DrawString(gdc, 50, 380, str, atUI_COLOR_WHITE);

				atUIAPI_sprintf(str, _AT("%d V"), lpData->nVoltage);
				atUIAPI_DrawString(gdc, 50, 400, str, atUI_COLOR_WHITE);
				atUIAPI_SetFont(gdc, oldfont);

				atUIAPI_sprintf(str, _AT("%d cm"), GetDistanceValue());

				atWINDOW_GetClientRegion(hWnd, &rgn);
				oldfont = atUIAPI_SetFont(gdc, lpData->hFnt_main);
				atUIAPI_DrawString(gdc, 230, 380, str, atUI_COLOR_WHITE);
				atUIAPI_SetFont(gdc, oldfont);
			}
		}
			break;
	}
	
}


int Scr_Calculate_WndOnEvent(atHWINDOW hWnd, int nMsg, long nParam1, long nParam2)
{
	LPSCR_CALCULATE_WNDDATA	lpData = (LPSCR_CALCULATE_WNDDATA)atWINDOW_GetDataPtr(hWnd);
	LPSCR_CALCULATE_WNDDATA	Data = (LPSCR_CALCULATE_WNDDATA)nParam1;
	
	switch( nMsg )
	{
		case WNDMSG_INIT:
		{
			lpData->nType = Data->nType;
			Scr_Calculate_WndOnInit(hWnd, nParam1);
		}
			break;

		case WNDMSG_RELEASE:
			Scr_Calculate_WndOnRelease( hWnd );
			break;

		case WNDMSG_PAINT:
			Scr_Calculate_WndOnPaint( hWnd );
			break;

		case WNDMSG_KEYDOWN:
			return Scr_Calculate_WndOnKeyDown( hWnd, nParam1 );
			break;

		case WNDMSG_KEYPRESS:
			return Scr_Calculate_WndOnKeyPress( hWnd, nParam1 );
			break;

		case WNDMSG_CHANGE:
			Scr_Calculate_WndOnChange( hWnd, nParam1, nParam2 );
			break;

		case WNDMSG_COMMAND:
			Scr_Calculate_WndOnCommand( hWnd, nParam1, nParam2 );
			break;

		case WNDMSG_TIMER:
			Scr_Calculate_WndOnTimer(hWnd, nParam1, nParam2);
			break;

		default:
			return 0;
			break;
	}

	return 0;
}
