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
#include "Screen_2_control.h"
#include "Screen_2_question.h"
#include "Screen_2_descript.h"
#include "Screen_information.h"
#include "Screen_calculate.h"

#include <string.h>

static SCR_2_DESC_WNDDATA		descWndData;
static SCR_2_QUESTION_WNDDATA	questionWndData;
static SCR_INFORMATION_WNDDATA	inforData;
static SCR_CALCULATE_WNDDATA	calcuData;

static void Scr_2_Control_WndOnCommand(atHWINDOW hWnd, long nParam1, long nParam2)
{
	LPSCR_2_CONTROL_WNDDATA lpData = (LPSCR_2_CONTROL_WNDDATA)atWINDOW_GetDataPtr(hWnd);
  
	atHWINDOW wnd;

    switch(nParam1)
    {
        case 100 :
			atAPP_DestroyWindow( atAPPMNG_GetCurApp(), hWnd );
			break;

		case 1000:
		{
			if (lpData->nType == 212) {
				memset(&descWndData, 0, sizeof(SCR_2_DESC_WNDDATA));
				descWndData.nType = 211;
				wnd = atAPP_CreateWindow(atAPPMNG_GetCurApp(), atNULL, 0, atNULL, _AT(""), 0, 0, 0, 0, sizeof(SCR_2_DESC_WNDDATA), Scr_2_Desc_WndOnEvent, &descWndData, atNULL);
				atWINDOW_SetAnchor(wnd, 0, 0, 0, 0);
				atAPP_ShowWindow(atAPPMNG_GetCurApp(), wnd, atTRUE);
			}

			atAPP_DestroyWindow(atAPPMNG_GetCurApp(), hWnd);
		}
			break;

		case 1001:
		{
			/*if (lpData->nType == 211) {
				memset(&descWndData, 0, sizeof(SCR_2_DESC_WNDDATA));
				descWndData.nType = 212;
				wnd = atAPP_CreateWindow(atAPPMNG_GetCurApp(), atNULL, 0, atNULL, _AT(""), 0, 0, 0, 0, sizeof(SCR_2_DESC_WNDDATA), Scr_2_Desc_WndOnEvent, &descWndData, atNULL);
			}
			else if (lpData->nType == 212) {
				memset(&questionWndData, 0, sizeof(SCR_2_QUESTION_WNDDATA));
				questionWndData.nType = 213;

				wnd = atAPP_CreateWindow(atAPPMNG_GetCurApp(), atNULL, 0, atNULL, _AT(""), 0, 0, 0, 0, sizeof(SCR_2_QUESTION_WNDDATA), Scr_2_Question_WndOnEvent, &questionWndData, atNULL);
			}*/

			memset(&questionWndData, 0, sizeof(SCR_2_QUESTION_WNDDATA));
			if (lpData->nType == 211)
				questionWndData.nType = 212;
			else if (lpData->nType == 212)
				questionWndData.nType = 213;

			wnd = atAPP_CreateWindow(atAPPMNG_GetCurApp(), atNULL, 0, atNULL, _AT(""), 0, 0, 0, 0, sizeof(SCR_2_QUESTION_WNDDATA), Scr_2_Question_WndOnEvent, &questionWndData, atNULL);
			atWINDOW_SetAnchor(wnd, 0, 0, 0, 0);
			atAPP_ShowWindow(atAPPMNG_GetCurApp(), wnd, atTRUE);

			atAPP_DestroyWindow(atAPPMNG_GetCurApp(), hWnd);
		}
			break;

		case 1003:
		{
			memset(&inforData, 0, sizeof(SCR_INFORMATION_WNDDATA));
			inforData.nType = 20011;

			wnd = atAPP_CreateWindow(atAPPMNG_GetCurApp(), atNULL, 0, atNULL, _AT(""), 0, 0, 0, 0, sizeof(SCR_INFORMATION_WNDDATA), Scr_inforamtion_WndOnEvent, &inforData, atNULL);
			atWINDOW_SetAnchor(wnd, 0, 0, 0, 0);
			atAPP_ShowWindow(atAPPMNG_GetCurApp(), wnd, atTRUE);
		}
			break;

		case 1005:
		{
			memset(&calcuData, 0, sizeof(SCR_CALCULATE_WNDDATA));
			if (lpData->nType == 211)
				calcuData.nType = 105;
			else if (lpData->nType == 212)
				calcuData.nType = 106;

			wnd = atAPP_CreateWindow(atAPPMNG_GetCurApp(), atNULL, 0, atNULL, _AT(""), 0, 0, 0, 0, sizeof(SCR_CALCULATE_WNDDATA), Scr_Calculate_WndOnEvent, &calcuData, atNULL);
			atWINDOW_SetAnchor(wnd, 0, 0, 0, 0);
			atAPP_ShowWindow(atAPPMNG_GetCurApp(), wnd, atTRUE);
		}
			break;
	}
}


static void Scr_2_Control_WndOnChange(atHWINDOW hWnd, long nParam1, long nParam2)
{
	LPSCR_2_CONTROL_WNDDATA lpData = (LPSCR_2_CONTROL_WNDDATA)atWINDOW_GetDataPtr(hWnd);

    switch(nParam1)
    {
        case 103 :

			break;
	}
}

static void Scr_2_Control_WndOnTimer(atHWINDOW hWnd, long nParam1, long nParam2)
{
	LPSCR_2_CONTROL_WNDDATA lpData = (LPSCR_2_CONTROL_WNDDATA)atWINDOW_GetDataPtr(hWnd);

	switch (nParam2)
	{
	case 0x100:
		atWINDOW_Refresh(hWnd);
		break;
	}
}

static void Scr_2_Control_WndOnInit(atHWINDOW hWnd, long nParam)
{
	LPSCR_2_CONTROL_WNDDATA lpData = (LPSCR_2_CONTROL_WNDDATA)atWINDOW_GetDataPtr(hWnd);

	atTCHAR str[100];
	atHCONTROL ctl;
	atHCONTROL hWidget;

	//atHRESSRC hResSrc = atUIAPI_AddResSource(_AT("TESTMAIN"), atUI_RES_SRC_RESSTREAM, (const atLPVOID)pUser_ResData, sizeof(pUser_ResData));
	atHRESSRC hResSrc = atUIAPI_GetResSource(_AT("TESTMAIN"));

	//if (lpData->nType == 211)
	//	lpData->hBm_title = atUIAPI_CreateBitmapFromRes(hResSrc, _AT("Scr_2_right_title.img"));
	//else if (lpData->nType == 212)
	//	lpData->hBm_title = atUIAPI_CreateBitmapFromRes(hResSrc, _AT("Scr_2_left_title.img"));

	lpData->hBm_bg = atUIAPI_CreateBitmapFromRes(hResSrc, _AT("Scr_1_bg.img"));

	lpData->hBm_btn_info_n = atUIAPI_CreateBitmapFromRes(hResSrc, _AT("Scr_def_btn_info_n.img"));
	lpData->hBm_btn_home_n = atUIAPI_CreateBitmapFromRes(hResSrc, _AT("Scr_def_btn_home_n.img"));
	lpData->hBm_btn_check_n = atUIAPI_CreateBitmapFromRes(hResSrc, _AT("Scr_def_btn_check_n.img"));

	lpData->hBm_btn_info_p = atUIAPI_CreateBitmapFromRes(hResSrc, _AT("Scr_def_btn_info_p.img"));
	lpData->hBm_btn_home_p = atUIAPI_CreateBitmapFromRes(hResSrc, _AT("Scr_def_btn_home_p.img"));
	lpData->hBm_btn_check_p = atUIAPI_CreateBitmapFromRes(hResSrc, _AT("Scr_def_btn_check_p.img"));

	if (lpData->nType == 211) {
		lpData->hBm_btn_calcu_n = atUIAPI_CreateBitmapFromRes(hResSrc, _AT("Scr_2_right_btn_center_n.img"));
		lpData->hBm_btn_calcu_p = atUIAPI_CreateBitmapFromRes(hResSrc, _AT("Scr_2_right_btn_center_p.img"));
	}
	else if (lpData->nType == 212) {
		lpData->hBm_btn_calcu_n = atUIAPI_CreateBitmapFromRes(hResSrc, _AT("Scr_2_left_btn_center_n.img"));
		lpData->hBm_btn_calcu_p = atUIAPI_CreateBitmapFromRes(hResSrc, _AT("Scr_2_left_btn_center_p.img"));
	}

	hWidget = ExtPkg_CreateImageButton(hWnd, 1000, 0, 399, 161, 82, lpData->hBm_btn_home_n, lpData->hBm_btn_home_p, atNULL);
	atCONTROL_SetFocusable(hWidget, atFALSE);
	hWidget = ExtPkg_CreateImageButton(hWnd, 1001, 160, 399, 161, 82, lpData->hBm_btn_check_n, lpData->hBm_btn_check_p, atNULL);
	atCONTROL_SetFocusable(hWidget, atFALSE);
	hWidget = ExtPkg_CreateImageButton(hWnd, 1003, 257, 60, 40, 40, lpData->hBm_btn_info_n, lpData->hBm_btn_info_p, atNULL);
	atCONTROL_SetFocusable(hWidget, atFALSE);

	hWidget = ExtPkg_CreateImageButton(hWnd, 1005, 44, 201, 232, 34, lpData->hBm_btn_calcu_n, lpData->hBm_btn_calcu_p, atNULL);
	atCONTROL_SetFocusable(hWidget, atFALSE);

	lpData->hFnt_sub = atUIAPI_CreateFont(atUIAPI_FACE_SYSTEM, 24, atUIAPI_STYLE_PLAIN, atFALSE);

	atWINDOW_AddTimer(hWnd, 0x100, 1000, atTRUE, atTRUE, atTRUE);
}

static void Scr_2_Control_WndOnRelease(atHWINDOW hWnd)
{
	LPSCR_2_CONTROL_WNDDATA lpData = (LPSCR_2_CONTROL_WNDDATA)atWINDOW_GetDataPtr(hWnd);

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


static int Scr_2_Control_WndOnKeyDown(atHWINDOW hWnd, int nKeyCode)
{
	LPSCR_2_CONTROL_WNDDATA lpData = (LPSCR_2_CONTROL_WNDDATA)atWINDOW_GetDataPtr(hWnd);
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
			/*if (lpData->nType == 211) {
				nDistRight = GetDistanceValue();
			}
			else if (lpData->nType == 212) {
				nDistLeft = GetDistanceValue();
			}*/

			atWINDOW_Refresh(hWnd);
		}
		break;
			
        default:
            return 0;
    }

    return 0; // WindowOnEvent 가 호출되지 않도록 0을 리턴한다.
}


#define DELTA 5

static int Scr_2_Control_WndOnKeyPress(atHWINDOW hWnd, int nKeyCode)
{
	LPSCR_2_CONTROL_WNDDATA lpData = (LPSCR_2_CONTROL_WNDDATA)atWINDOW_GetDataPtr(hWnd);

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


static void Scr_2_Control_WndOnPaint(atHWINDOW hWnd)
{
	LPSCR_2_CONTROL_WNDDATA	lpData = (LPSCR_2_CONTROL_WNDDATA)atWINDOW_GetDataPtr(hWnd);

	atHGDC gdc = atWINDOW_GetClientGDC(hWnd);
	atREGION rgn;
	atHFONT oldfont;
	atTCHAR str[64];

	atUIAPI_DrawImage(gdc, 0, 0, lpData->hBm_bg);
	//atUIAPI_DrawImage(gdc, 38, 64, lpData->hBm_title);

	if (lpData->nType == 211)
		atUIAPI_sprintf(str, _AT("%d cm"), nDistRight);
	else if (lpData->nType == 212)
		atUIAPI_sprintf(str, _AT("%d cm"), nDistLeft);

	atWINDOW_GetClientRegion(hWnd, &rgn);
	oldfont = atUIAPI_SetFont(gdc, lpData->hFnt_sub);
	atUIAPI_DrawString(gdc, 30, 340, str, atUI_COLOR_BLACK);
	atUIAPI_SetFont(gdc, oldfont);
}


int Scr_2_Control_WndOnEvent(atHWINDOW hWnd, int nMsg, long nParam1, long nParam2)
{
	LPSCR_2_CONTROL_WNDDATA	lpData = (LPSCR_2_CONTROL_WNDDATA)atWINDOW_GetDataPtr(hWnd);
	LPSCR_2_CONTROL_WNDDATA  Data = (LPSCR_2_CONTROL_WNDDATA)nParam1;
	
	switch( nMsg )
	{
		case WNDMSG_INIT:
		{
			lpData->nType = Data->nType;
			Scr_2_Control_WndOnInit(hWnd, nParam1);
		}
			break;

		case WNDMSG_RELEASE:
			Scr_2_Control_WndOnRelease( hWnd );
			break;

		case WNDMSG_PAINT:
			Scr_2_Control_WndOnPaint( hWnd );
			break;

		case WNDMSG_KEYDOWN:
			return Scr_2_Control_WndOnKeyDown( hWnd, nParam1 );
			break;

		case WNDMSG_KEYPRESS:
			return Scr_2_Control_WndOnKeyPress( hWnd, nParam1 );
			break;

		case WNDMSG_CHANGE:
			Scr_2_Control_WndOnChange( hWnd, nParam1, nParam2 );
			break;

		case WNDMSG_COMMAND:
			Scr_2_Control_WndOnCommand( hWnd, nParam1, nParam2 );
			break;

		case WNDMSG_TIMER:
			Scr_2_Control_WndOnTimer(hWnd, nParam1, nParam2);
			break;

		default:
			return 0;
			break;
	}

	return 0;
}
