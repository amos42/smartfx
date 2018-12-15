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
#include "Screen_3_image.h"
#include "Screen_information.h"
#include "Screen_calculate.h"

#include <string.h>

static SCR_INFORMATION_WNDDATA inforData;
static SCR_CALCULATE_WNDDATA calcuData;

static void Scr_3_image_WndOnCommand(atHWINDOW hWnd, long nParam1, long nParam2)
{
	LPSCR_3_IMAGE_WNDDATA lpData = (LPSCR_3_IMAGE_WNDDATA)atWINDOW_GetDataPtr(hWnd);
	atHRESSRC hResSrc = atUIAPI_GetResSource(_AT("TESTMAIN"));
	atTCHAR str[200];
  
    switch(nParam1)
    {
		// Calculate
		case 1000:
			memset(&calcuData, 0, sizeof(SCR_CALCULATE_WNDDATA));
			calcuData.nType = lpData->nType;

			atHWINDOW wnd;
			wnd = atAPP_CreateWindow(atAPPMNG_GetCurApp(), atNULL, 0, atNULL, _AT(""), 0, 0, 0, 0, sizeof(SCR_CALCULATE_WNDDATA), Scr_Calculate_WndOnEvent, &calcuData, atNULL);
			atWINDOW_SetAnchor(wnd, 0, 0, 0, 0);
			atAPP_ShowWindow(atAPPMNG_GetCurApp(), wnd, atTRUE);
			break;

		case 1001:
			atAPP_DestroyWindow(atAPPMNG_GetCurApp(), hWnd);
			break;

		/*case 1003:
		{
			if ((lpData->nType > 301) && (lpData->nType <= 307)) {
				lpData->nType--;
				atUIAPI_DestroyBitmap(lpData->hBm_btn_center_n);
				atWINDOW_RemoveControl(hWnd, lpData->hBtn_center);
				atUIAPI_sprintf(str, _AT("Scr_3_special_img_%d.img"), lpData->nType - 300);
				lpData->hBm_btn_center_n = atUIAPI_CreateBitmapFromRes(hResSrc, str);
				lpData->hBtn_center = ExtPkg_CreateImageButton(hWnd, 1000, 111, 231, 100, 100, lpData->hBm_btn_center_n, atNULL, atNULL);
			}	 
			else if ((lpData->nType > 401) && (lpData->nType <= 405)) {
				lpData->nType--;
				atUIAPI_DestroyBitmap(lpData->hBm_btn_center_n);
				atWINDOW_RemoveControl(hWnd, lpData->hBtn_center);
				atUIAPI_sprintf(str, _AT("Scr_3_vision_img_%d.img"), lpData->nType - 400);
				lpData->hBm_btn_center_n = atUIAPI_CreateBitmapFromRes(hResSrc, str);
				lpData->hBtn_center = ExtPkg_CreateImageButton(hWnd, 1000, 111, 231, 100, 100, lpData->hBm_btn_center_n, atNULL, atNULL);
			}
		}
			break;

		case 1004:
		{
			if ((lpData->nType >= 301) && (lpData->nType < 307)) {
				lpData->nType++;
				atUIAPI_DestroyBitmap(lpData->hBm_btn_center_n);
				atWINDOW_RemoveControl(hWnd, lpData->hBtn_center);
				atUIAPI_sprintf(str, _AT("Scr_3_special_img_%d.img"), lpData->nType - 300);
				lpData->hBm_btn_center_n = atUIAPI_CreateBitmapFromRes(hResSrc, str);
				lpData->hBtn_center = ExtPkg_CreateImageButton(hWnd, 1000, 111, 231, 100, 100, lpData->hBm_btn_center_n, atNULL, atNULL);
			}
			else if ((lpData->nType >= 401) && (lpData->nType < 405)) {
				lpData->nType++;
				atUIAPI_DestroyBitmap(lpData->hBm_btn_center_n);
				atWINDOW_RemoveControl(hWnd, lpData->hBtn_center);
				atUIAPI_sprintf(str, _AT("Scr_3_vision_img_%d.img"), lpData->nType - 400);
				lpData->hBm_btn_center_n = atUIAPI_CreateBitmapFromRes(hResSrc, str);
				lpData->hBtn_center = ExtPkg_CreateImageButton(hWnd, 1000, 111, 231, 100, 100, lpData->hBm_btn_center_n, atNULL, atNULL);
			}
		}
			break;*/

		case 1005:
		{
			memset(&inforData, 0, sizeof(SCR_INFORMATION_WNDDATA));

			if (lpData->nType == 101)
				inforData.nType = 10011;
			else if (lpData->nType == 102)
				inforData.nType = 10031;

			switch (lpData->nType)
			{
				case 301:
					inforData.nType = 30011;
					break;

				case 302:
					inforData.nType = 30020;
					break;

				case 303:
					inforData.nType = 30030;
					break;

				case 304:
					inforData.nType = 30041;
					break;

				case 305:
					inforData.nType = 30050;
					break;

				case 306:
					inforData.nType = 30060;
					break;

				case 307:
					inforData.nType = 30070;
					break;

				case 401:
					inforData.nType = 30110;
					break;

				case 402:
					inforData.nType = 30120;
					break;

				case 403:
					inforData.nType = 30130;
					break;

				case 404:
					inforData.nType = 30140;
					break;

				case 405:
					inforData.nType = 30150;
					break;
			}

			atHWINDOW wnd;
			wnd = atAPP_CreateWindow(atAPPMNG_GetCurApp(), atNULL, 0, atNULL, _AT(""), 0, 0, 0, 0, sizeof(SCR_INFORMATION_WNDDATA), Scr_inforamtion_WndOnEvent, &inforData, atNULL);
			atWINDOW_SetAnchor(wnd, 0, 0, 0, 0);
			atAPP_ShowWindow(atAPPMNG_GetCurApp(), wnd, atTRUE);
		}
			break;
	}
}

static void Scr_3_Image_WndOnChange(atHWINDOW hWnd, long nParam1, long nParam2)
{
	LPSCR_3_IMAGE_WNDDATA lpData = (LPSCR_3_IMAGE_WNDDATA)atWINDOW_GetDataPtr(hWnd);

    switch(nParam1)
    {
        case 103 :

			break;
	}
}

static void Scr_3_Image_WndOnInit(atHWINDOW hWnd, long nParam)
{
	LPSCR_3_IMAGE_WNDDATA lpData = (LPSCR_3_IMAGE_WNDDATA)atWINDOW_GetDataPtr(hWnd);

	atHCONTROL ctl;
	atHCONTROL hWidget;

	//atHRESSRC hResSrc = atUIAPI_AddResSource(_AT("TESTMAIN"), atUI_RES_SRC_RESSTREAM, (const atLPVOID)pUser_ResData, sizeof(pUser_ResData));
	atHRESSRC hResSrc = atUIAPI_GetResSource(_AT("TESTMAIN"));

	lpData->hBm_bg = atUIAPI_CreateBitmapFromRes(hResSrc, _AT("Scr_3_bg.img"));
	
	if ((lpData->nType >= 401) && (lpData->nType <= 405))
		lpData->hBm_title = atUIAPI_CreateBitmapFromRes(hResSrc, _AT("Scr_3_select_vision_n.img"));
	else if ((lpData->nType >= 301) && (lpData->nType <= 307))
		lpData->hBm_title = atUIAPI_CreateBitmapFromRes(hResSrc, _AT("Scr_3_select_special_n.img"));

	if (lpData->nType == 301) {
		lpData->hBm_btn_center_n = atUIAPI_CreateBitmapFromRes(hResSrc, _AT("Scr_3_special_img_1.img"));
	}
	else if (lpData->nType == 302) {
		lpData->hBm_btn_center_n = atUIAPI_CreateBitmapFromRes(hResSrc, _AT("Scr_3_special_img_2.img"));
	}
	else if (lpData->nType == 303) {
		lpData->hBm_btn_center_n = atUIAPI_CreateBitmapFromRes(hResSrc, _AT("Scr_3_special_img_3.img"));
	}
	else if (lpData->nType == 304) {
		lpData->hBm_btn_center_n = atUIAPI_CreateBitmapFromRes(hResSrc, _AT("Scr_3_special_img_4.img"));
	}
	else if (lpData->nType == 305) {
		lpData->hBm_btn_center_n = atUIAPI_CreateBitmapFromRes(hResSrc, _AT("Scr_3_special_img_5.img"));
	}
	else if (lpData->nType == 306) {
		lpData->hBm_btn_center_n = atUIAPI_CreateBitmapFromRes(hResSrc, _AT("Scr_3_special_img_6.img"));
	}
	else if (lpData->nType == 307) {
		lpData->hBm_btn_center_n = atUIAPI_CreateBitmapFromRes(hResSrc, _AT("Scr_3_special_img_7.img"));
	}
	else if (lpData->nType == 401) {
		lpData->hBm_btn_center_n = atUIAPI_CreateBitmapFromRes(hResSrc, _AT("Scr_3_vision_img_1.img"));
	}
	else if (lpData->nType == 402) {
		lpData->hBm_btn_center_n = atUIAPI_CreateBitmapFromRes(hResSrc, _AT("Scr_3_vision_img_2.img"));
	}
	else if (lpData->nType == 403) {
		lpData->hBm_btn_center_n = atUIAPI_CreateBitmapFromRes(hResSrc, _AT("Scr_3_vision_img_3.img"));
	}
	else if (lpData->nType == 404) {
		lpData->hBm_btn_center_n = atUIAPI_CreateBitmapFromRes(hResSrc, _AT("Scr_3_vision_img_4.img"));
	}
	else if (lpData->nType == 405) {
		lpData->hBm_btn_center_n = atUIAPI_CreateBitmapFromRes(hResSrc, _AT("Scr_3_vision_img_5.img"));
	}
	
	lpData->hBm_btn_info_n = atUIAPI_CreateBitmapFromRes(hResSrc, _AT("Scr_def_btn_info_n.img"));
	lpData->hBm_btn_info_p = atUIAPI_CreateBitmapFromRes(hResSrc, _AT("Scr_def_btn_info_p.img"));

	lpData->hBm_btn_back_n = atUIAPI_CreateBitmapFromRes(hResSrc, _AT("Scr_def_btn_ok_n.img"));
	lpData->hBm_btn_back_p = atUIAPI_CreateBitmapFromRes(hResSrc, _AT("Scr_def_btn_ok_p.img"));
	

	lpData->hBtn_center = ExtPkg_CreateImageButton(hWnd, 1000, 111, 200, 100, 100, lpData->hBm_btn_center_n, atNULL, atNULL);
	atCONTROL_SetFocusable(lpData->hBtn_center, atFALSE);
	hWidget = ExtPkg_CreateImageButton(hWnd, 1005, 257, 60, 40, 40, lpData->hBm_btn_info_n, lpData->hBm_btn_info_p, atNULL);
	atCONTROL_SetFocusable(hWidget, atFALSE);

	hWidget = ExtPkg_CreateImageButton(hWnd, 1001, 98, 398, 126, 64, lpData->hBm_btn_back_n, lpData->hBm_btn_back_p, atNULL);
	atCONTROL_SetFocusable(hWidget, atFALSE);

	lpData->hFnt_main = atUIAPI_CreateFont(atUIAPI_FACE_SYSTEM, 18, atUIAPI_STYLE_PLAIN, atFALSE);
}

static void Scr_3_Image_WndOnRelease(atHWINDOW hWnd)
{
	LPSCR_3_IMAGE_WNDDATA lpData = (LPSCR_3_IMAGE_WNDDATA)atWINDOW_GetDataPtr(hWnd);

	atUIAPI_DestroyBitmap(lpData->hBm_bg);
	atUIAPI_DestroyBitmap(lpData->hBm_title);

	atUIAPI_DestroyBitmap(lpData->hBm_btn_center_n);
	atUIAPI_DestroyBitmap(lpData->hBm_btn_info_n);
	atUIAPI_DestroyBitmap(lpData->hBm_btn_info_p);

	atUIAPI_DestroyBitmap(lpData->hBm_btn_back_n);
	atUIAPI_DestroyBitmap(lpData->hBm_btn_back_p);

	atUIAPI_DestroyFont(lpData->hFnt_main);
}


static int Scr_3_Image_WndOnKeyDown(atHWINDOW hWnd, int nKeyCode)
{
	LPSCR_3_IMAGE_WNDDATA lpData = (LPSCR_3_IMAGE_WNDDATA)atWINDOW_GetDataPtr(hWnd);
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

static int Scr_3_Image_WndOnKeyPress(atHWINDOW hWnd, int nKeyCode)
{
	LPSCR_3_IMAGE_WNDDATA lpData = (LPSCR_3_IMAGE_WNDDATA)atWINDOW_GetDataPtr(hWnd);

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


static void Scr_3_Image_WndOnPaint(atHWINDOW hWnd)
{
	LPSCR_3_IMAGE_WNDDATA lpData = (LPSCR_3_IMAGE_WNDDATA)atWINDOW_GetDataPtr(hWnd);

	atHGDC gdc = atWINDOW_GetClientGDC(hWnd);
	atREGION rgn;
	atHFONT oldfont;
	atTCHAR str[64];

	atUIAPI_DrawImage(gdc, 0, 0, lpData->hBm_bg);
	atUIAPI_DrawImage(gdc, 24, 60, lpData->hBm_title);

	atUIAPI_sprintf(str, _AT("%d cm"), nDistChart);

	atWINDOW_GetClientRegion(hWnd, &rgn);
	oldfont = atUIAPI_SetFont(gdc, lpData->hFnt_main);
	atUIAPI_DrawString(gdc, 30, 370, str, atUI_COLOR_BLACK);
	atUIAPI_SetFont(gdc, oldfont);
}


int Scr_3_Image_WndOnEvent(atHWINDOW hWnd, int nMsg, long nParam1, long nParam2)
{
	int ret = 0;
	LPSCR_3_IMAGE_WNDDATA lpData = (LPSCR_3_IMAGE_WNDDATA)atWINDOW_GetDataPtr(hWnd);
	LPSCR_3_IMAGE_WNDDATA Data = (LPSCR_3_IMAGE_WNDDATA)nParam1;
	

	switch( nMsg )
	{
		case WNDMSG_INIT:
		{
			lpData->nType = Data->nType;
			Scr_3_Image_WndOnInit(hWnd, nParam1);
		}
			break;
		case WNDMSG_RELEASE:
			Scr_3_Image_WndOnRelease( hWnd );
			break;
		case WNDMSG_PAINT:
			Scr_3_Image_WndOnPaint( hWnd );
			break;
		case WNDMSG_KEYDOWN:
			return Scr_3_Image_WndOnKeyDown( hWnd, nParam1 );
			break;
		case WNDMSG_KEYPRESS:
			return Scr_3_Image_WndOnKeyPress( hWnd, nParam1 );
			break;
		case WNDMSG_CHANGE:
			Scr_3_Image_WndOnChange( hWnd, nParam1, nParam2 );
			break;
		case WNDMSG_COMMAND:
			Scr_3_image_WndOnCommand( hWnd, nParam1, nParam2 );
			break;
		default:
			return 0;
			break;
	}

	return ret;
}
