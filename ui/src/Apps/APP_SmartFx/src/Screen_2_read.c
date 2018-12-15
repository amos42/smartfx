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
#include "Screen_2_read.h"
#include "Screen_2_result.h"
#include "Screen_information.h"
#include "Screen_calculate.h"

#include <string.h>

static SCR_2_RESULT_WNDDATA		descWndData;
static SCR_INFORMATION_WNDDATA	inforData;
static SCR_CALCULATE_WNDDATA	calcuData;

static void Scr_2_Read_WndOnCommand(atHWINDOW hWnd, long nParam1, long nParam2)
{
	LPSCR_2_READ_WNDDATA lpData = (LPSCR_2_READ_WNDDATA)atWINDOW_GetDataPtr(hWnd);
  
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
			memset(&descWndData, 0, sizeof(SCR_2_RESULT_WNDDATA));
			if (lpData->nType == 213) 
				descWndData.nType = 219;
			else if (lpData->nType == 223) 
				descWndData.nType = 229;

			atHWINDOW wnd;
			wnd = atAPP_CreateWindow(atAPPMNG_GetCurApp(), atNULL, 0, atNULL, _AT(""), 20, 30, 200, 220, sizeof(SCR_2_RESULT_WNDDATA), Scr_2_Result_WndOnEvent, &descWndData, atNULL);
			atWINDOW_SetAnchor(wnd, 0, 0, 0, 0);
			atAPP_ShowWindow(atAPPMNG_GetCurApp(), wnd, atTRUE);

			atAPP_DestroyWindow(atAPPMNG_GetCurApp(), hWnd);
		}
			break;

		case 1003:
		{
			memset(&inforData, 0, sizeof(SCR_INFORMATION_WNDDATA));
			inforData.nType = 20031;

			atHWINDOW wnd;
			wnd = atAPP_CreateWindow(atAPPMNG_GetCurApp(), atNULL, 0, atNULL, _AT(""), 0, 0, 0, 0, sizeof(SCR_INFORMATION_WNDDATA), Scr_inforamtion_WndOnEvent, &inforData, atNULL);
			atWINDOW_SetAnchor(wnd, 0, 0, 0, 0);
			atAPP_ShowWindow(atAPPMNG_GetCurApp(), wnd, atTRUE);
		}
			break;

		case 1005:
		{
			memset(&calcuData, 0, sizeof(SCR_CALCULATE_WNDDATA));
			calcuData.nType = 107;

			atHWINDOW wnd;
			wnd = atAPP_CreateWindow(atAPPMNG_GetCurApp(), atNULL, 0, atNULL, _AT(""), 0, 0, 0, 0, sizeof(SCR_CALCULATE_WNDDATA), Scr_Calculate_WndOnEvent, &calcuData, atNULL);
			atWINDOW_SetAnchor(wnd, 0, 0, 0, 0);
			atAPP_ShowWindow(atAPPMNG_GetCurApp(), wnd, atTRUE);
		}
			break;
	}
}


static void Scr_2_Read_WndOnChange(atHWINDOW hWnd, long nParam1, long nParam2)
{
	LPSCR_2_READ_WNDDATA lpData = (LPSCR_2_READ_WNDDATA)atWINDOW_GetDataPtr(hWnd);

    switch(nParam1)
    {
        case 103 :

			break;
	}
}

static void Scr_2_Read_WndOnTimer(atHWINDOW hWnd, long nParam1, long nParam2)
{
	LPSCR_2_READ_WNDDATA lpData = (LPSCR_2_READ_WNDDATA)atWINDOW_GetDataPtr(hWnd);

	switch (nParam2)
	{
	case 0x100:
		atWINDOW_Refresh(hWnd);
		break;
	}
}

static void Scr_2_Read_WndOnInit(atHWINDOW hWnd, long nParam)
{
	LPSCR_2_READ_WNDDATA lpData = (LPSCR_2_READ_WNDDATA)atWINDOW_GetDataPtr(hWnd);

	atHCONTROL ctl;
	atHCONTROL hWidget;

	//atHRESSRC hResSrc = atUIAPI_AddResSource(_AT("TESTMAIN"), atUI_RES_SRC_RESSTREAM, (const atLPVOID)pUser_ResData, sizeof(pUser_ResData));
	atHRESSRC hResSrc = atUIAPI_GetResSource(_AT("TESTMAIN"));

	lpData->hBm_bg = atUIAPI_CreateBitmapFromRes(hResSrc, _AT("Scr_1_bg.img"));
	//lpData->hBm_title = atUIAPI_CreateBitmapFromRes(hResSrc, _AT("Scr_2_read_title.img"));

	lpData->hBm_btn_info_n = atUIAPI_CreateBitmapFromRes(hResSrc, _AT("Scr_def_btn_info_n.img"));
	lpData->hBm_btn_home_n = atUIAPI_CreateBitmapFromRes(hResSrc, _AT("Scr_def_btn_home_n.img"));
	lpData->hBm_btn_check_n = atUIAPI_CreateBitmapFromRes(hResSrc, _AT("Scr_def_btn_check_n.img"));

	lpData->hBm_btn_info_p = atUIAPI_CreateBitmapFromRes(hResSrc, _AT("Scr_def_btn_info_p.img"));
	lpData->hBm_btn_home_p = atUIAPI_CreateBitmapFromRes(hResSrc, _AT("Scr_def_btn_home_p.img"));
	lpData->hBm_btn_check_p = atUIAPI_CreateBitmapFromRes(hResSrc, _AT("Scr_def_btn_check_p.img"));

	lpData->hBm_btn_calcu_n = atUIAPI_CreateBitmapFromRes(hResSrc, _AT("Scr_2_read_btn_center_n.img"));
	lpData->hBm_btn_calcu_p = atUIAPI_CreateBitmapFromRes(hResSrc, _AT("Scr_2_read_btn_center_p.img"));
	
	hWidget = ExtPkg_CreateImageButton(hWnd, 1000, 0, 399, 161, 82, lpData->hBm_btn_home_n, lpData->hBm_btn_home_p, atNULL);
	atCONTROL_SetFocusable(hWidget, atFALSE);
	hWidget = ExtPkg_CreateImageButton(hWnd, 1001, 160, 399, 161, 82, lpData->hBm_btn_check_n, lpData->hBm_btn_check_p, atNULL);
	atCONTROL_SetFocusable(hWidget, atFALSE);
	hWidget = ExtPkg_CreateImageButton(hWnd, 1003, 257, 60, 40, 40, lpData->hBm_btn_info_n, lpData->hBm_btn_info_p, atNULL);
	atCONTROL_SetFocusable(hWidget, atFALSE);

	hWidget = ExtPkg_CreateImageButton(hWnd, 1005, 65, 201, 190, 34, lpData->hBm_btn_calcu_n, lpData->hBm_btn_calcu_p, atNULL);
	atCONTROL_SetFocusable(hWidget, atFALSE);

	lpData->hFnt_sub = atUIAPI_CreateFont(atUIAPI_FACE_SYSTEM, 24, atUIAPI_STYLE_PLAIN, atFALSE);

	atWINDOW_AddTimer(hWnd, 0x100, 1000, atTRUE, atTRUE, atTRUE);
}

static void Scr_2_Read_WndOnRelease(atHWINDOW hWnd)
{
	LPSCR_2_READ_WNDDATA lpData = (LPSCR_2_READ_WNDDATA)atWINDOW_GetDataPtr(hWnd);

	atUIAPI_DestroyBitmap(lpData->hBm_bg);
	//atUIAPI_DestroyBitmap(lpData->hBm_title);

	atUIAPI_DestroyBitmap(lpData->hBm_btn_check_n);
	atUIAPI_DestroyBitmap(lpData->hBm_btn_home_n);
	atUIAPI_DestroyBitmap(lpData->hBm_btn_info_n);

	atUIAPI_DestroyBitmap(lpData->hBm_btn_check_p);
	atUIAPI_DestroyBitmap(lpData->hBm_btn_home_p);
	atUIAPI_DestroyBitmap(lpData->hBm_btn_info_p);

	atUIAPI_DestroyBitmap(lpData->hBm_btn_calcu_n);
	atUIAPI_DestroyBitmap(lpData->hBm_btn_calcu_p);

	atUIAPI_DestroyFont(lpData->hFnt_sub);
}

static int Scr_2_Read_WndOnKeyDown(atHWINDOW hWnd, int nKeyCode)
{
	LPSCR_2_READ_WNDDATA lpData = (LPSCR_2_READ_WNDDATA)atWINDOW_GetDataPtr(hWnd);
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
			//nDistRead = GetDistanceValue();
			atWINDOW_Refresh(hWnd);
		}
		break;
			
        default:
            return 0;
    }

    return 0; // WindowOnEvent 가 호출되지 않도록 0을 리턴한다.
}


#define DELTA 5

static int Scr_2_Read_WndOnKeyPress(atHWINDOW hWnd, int nKeyCode)
{
	LPSCR_2_READ_WNDDATA lpData = (LPSCR_2_READ_WNDDATA)atWINDOW_GetDataPtr(hWnd);

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


static void Scr_2_Read_WndOnPaint(atHWINDOW hWnd)
{
	LPSCR_2_READ_WNDDATA	lpData = (LPSCR_2_READ_WNDDATA)atWINDOW_GetDataPtr(hWnd);

	atHGDC gdc = atWINDOW_GetClientGDC(hWnd);
	atREGION rgn;
	atHFONT oldfont;
	atTCHAR str[64];

	atUIAPI_DrawImage(gdc, 0, 0, lpData->hBm_bg);
	//atUIAPI_DrawImage(gdc, 61, 64, lpData->hBm_title);

	atUIAPI_sprintf(str, _AT("%d cm"), nDistRead);

	atWINDOW_GetClientRegion(hWnd, &rgn);
	oldfont = atUIAPI_SetFont(gdc, lpData->hFnt_sub);
	atUIAPI_DrawString(gdc, 30, 340, str, atUI_COLOR_BLACK);
	atUIAPI_SetFont(gdc, oldfont);
}


int Scr_2_Read_WndOnEvent(atHWINDOW hWnd, int nMsg, long nParam1, long nParam2)
{
	int ret = 0;
	LPSCR_2_READ_WNDDATA	lpData = (LPSCR_2_READ_WNDDATA)atWINDOW_GetDataPtr(hWnd);
	LPSCR_2_READ_WNDDATA  Data = (LPSCR_2_READ_WNDDATA)nParam1;
	
	switch( nMsg )
	{
		case WNDMSG_INIT:
		{
			lpData->nType = Data->nType;
			Scr_2_Read_WndOnInit(hWnd, nParam1);
		}
			break;

		case WNDMSG_RELEASE:
			Scr_2_Read_WndOnRelease( hWnd );
			break;

		case WNDMSG_PAINT:
			Scr_2_Read_WndOnPaint( hWnd );
			break;

		case WNDMSG_KEYDOWN:
			return Scr_2_Read_WndOnKeyDown( hWnd, nParam1 );
			break;

		case WNDMSG_KEYPRESS:
			return Scr_2_Read_WndOnKeyPress( hWnd, nParam1 );
			break;

		case WNDMSG_CHANGE:
			Scr_2_Read_WndOnChange( hWnd, nParam1, nParam2 );
			break;

		case WNDMSG_COMMAND:
			Scr_2_Read_WndOnCommand( hWnd, nParam1, nParam2 );
			break;

		case WNDMSG_TIMER:
			Scr_2_Read_WndOnTimer(hWnd, nParam1, nParam2);
			break;

		default:
			return 0;
			break;
	}

	return ret;
}
