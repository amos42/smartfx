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
#include "Screen_1_recover_exam.h"
#include "Screen_1_descript.h"
#include "Screen_1_question.h"
#include "Screen_1_result.h"
#include "Screen_information.h"
#include "Screen_calculate.h"

#include <string.h>

static SCR_1_RESULT_WNDDATA			resWndData;
static SCR_1_DESC_WNDDATA			descripWndData;
static SCR_INFORMATION_WNDDATA		inforData;
static SCR_CALCULATE_WNDDATA		calcuData;

static void Scr_1_Recover_Exam_WndOnCommand(atHWINDOW hWnd, long nParam1, long nParam2)
{
	LPSCR_1_RECOVER_EXAM_WNDDATA lpData = (LPSCR_1_RECOVER_EXAM_WNDDATA)atWINDOW_GetDataPtr(hWnd);
  
	atHWINDOW wnd;

    switch(nParam1)
    {
		case 1000:
		{
			memset(&descripWndData, 0, sizeof(SCR_1_DESC_WNDDATA));
			if (lpData->nType == 101)
				descripWndData.nType = 101;
			else if (lpData->nType == 102)
				descripWndData.nType = 102;

			wnd = atAPP_CreateWindow(atAPPMNG_GetCurApp(), hWnd, 0, atWS_NONEFRAME | atWS_MODAL, _AT(""), 0, 0, 0, 0, sizeof(SCR_1_DESC_WNDDATA), Scr_1_Desc_WndOnEvent, &descripWndData, atNULL);
			atWINDOW_SetAnchor(wnd, 0, 0, 0, 0);
			atAPP_ShowWindow(atAPPMNG_GetCurApp(), wnd, atTRUE);

			atAPP_DestroyWindow(atAPPMNG_GetCurApp(), hWnd);
		}
			break;

		case 1001:
		{
			memset(&resWndData, 0, sizeof(SCR_1_RESULT_WNDDATA));

			if (lpData->nType == 101)
				resWndData.nType = 103;
			else if (lpData->nType == 102)
				resWndData.nType = 104;

			wnd = atAPP_CreateWindow(atAPPMNG_GetCurApp(), atNULL, 0, atNULL, _AT(""), 0, 0, 0, 0, sizeof(SCR_1_RESULT_WNDDATA), Scr_1_Result_WndOnEvent, &resWndData, atNULL);

			atWINDOW_SetAnchor(wnd, 0, 0, 0, 0);
			atAPP_ShowWindow(atAPPMNG_GetCurApp(), wnd, atTRUE);

			atAPP_DestroyWindow(atAPPMNG_GetCurApp(), hWnd);
		}
			break;

		case 1003:
		{
			memset(&inforData, 0, sizeof(SCR_INFORMATION_WNDDATA));

			if (lpData->nType == 101)
				inforData.nType = 10021;
			else if (lpData->nType == 102)
				inforData.nType = 10041;

			atHWINDOW wnd;
			wnd = atAPP_CreateWindow(atAPPMNG_GetCurApp(), atNULL, 0, atNULL, _AT(""), 0, 0, 0, 0, sizeof(SCR_INFORMATION_WNDDATA), Scr_inforamtion_WndOnEvent, &inforData, atNULL);
			atWINDOW_SetAnchor(wnd, 0, 0, 0, 0);
			atAPP_ShowWindow(atAPPMNG_GetCurApp(), wnd, atTRUE);
		}
			break;

		case 1005:
		{
			memset(&calcuData, 0, sizeof(SCR_CALCULATE_WNDDATA));
			if (lpData->nType == 101)
				calcuData.nType = 103;
			else if (lpData->nType == 102)
				calcuData.nType = 104;

			atHWINDOW wnd;
			wnd = atAPP_CreateWindow(atAPPMNG_GetCurApp(), atNULL, 0, atNULL, _AT(""), 0, 0, 0, 0, sizeof(SCR_CALCULATE_WNDDATA), Scr_Calculate_WndOnEvent, &calcuData, atNULL);
			atWINDOW_SetAnchor(wnd, 0, 0, 0, 0);

			atAPP_ShowWindow(atAPPMNG_GetCurApp(), wnd, atTRUE);
		}
		break;
	}
}


static void Scr_1_Recover_Exam_WndOnChange(atHWINDOW hWnd, long nParam1, long nParam2)
{
	LPSCR_1_RECOVER_EXAM_WNDDATA lpData = (LPSCR_1_RECOVER_EXAM_WNDDATA)atWINDOW_GetDataPtr(hWnd);

    switch(nParam1)
    {
        case 103 :

			break;
	}
}

static void Scr_1_Recover_Exam_WndOnTimer(atHWINDOW hWnd, long nParam1, long nParam2)
{
	LPSCR_1_RECOVER_EXAM_WNDDATA lpData = (LPSCR_1_RECOVER_EXAM_WNDDATA)atWINDOW_GetDataPtr(hWnd);

	switch (nParam2)
	{
	case 0x100:
		atWINDOW_Refresh(hWnd);
		break;
	}
}

static void Scr_1_Recover_Exam_WndOnInit(atHWINDOW hWnd)
{
	LPSCR_1_RECOVER_EXAM_WNDDATA	lpData = (LPSCR_1_RECOVER_EXAM_WNDDATA)atWINDOW_GetDataPtr(hWnd);

	atHCONTROL ctl;
	atHCONTROL hWidget;

	//atHRESSRC hResSrc = atUIAPI_AddResSource(_AT("TESTMAIN"), atUI_RES_SRC_RESSTREAM, (const atLPVOID)pUser_ResData, sizeof(pUser_ResData));
	atHRESSRC hResSrc = atUIAPI_GetResSource(_AT("TESTMAIN"));

	//lpData->hBm_title = atUIAPI_CreateBitmapFromRes(hResSrc, _AT("Scr_1_recover_title.img"));
	lpData->hBm_bg = atUIAPI_CreateBitmapFromRes(hResSrc, _AT("Scr_1_bg.img"));

	lpData->hBm_btn_info_n = atUIAPI_CreateBitmapFromRes(hResSrc, _AT("Scr_def_btn_info_n.img"));
	lpData->hBm_btn_home_n = atUIAPI_CreateBitmapFromRes(hResSrc, _AT("Scr_def_btn_back_n.img"));
	lpData->hBm_btn_check_n = atUIAPI_CreateBitmapFromRes(hResSrc, _AT("Scr_def_btn_check_n.img"));

	lpData->hBm_btn_info_p = atUIAPI_CreateBitmapFromRes(hResSrc, _AT("Scr_def_btn_info_p.img"));
	lpData->hBm_btn_home_p = atUIAPI_CreateBitmapFromRes(hResSrc, _AT("Scr_def_btn_back_p.img"));
	lpData->hBm_btn_check_p = atUIAPI_CreateBitmapFromRes(hResSrc, _AT("Scr_def_btn_check_p.img"));

	lpData->hBm_btn_calcu_n = atUIAPI_CreateBitmapFromRes(hResSrc, _AT("Scr_1_recover_btn_center_n.img"));
	lpData->hBm_btn_calcu_p = atUIAPI_CreateBitmapFromRes(hResSrc, _AT("Scr_1_recover_btn_center_n.img"));

	hWidget = ExtPkg_CreateImageButton(hWnd, 1000, 0, 399, 161, 82, lpData->hBm_btn_home_n, lpData->hBm_btn_home_p, atNULL);
	atCONTROL_SetFocusable(hWidget, atFALSE);
	hWidget = ExtPkg_CreateImageButton(hWnd, 1001, 160, 399, 161, 82, lpData->hBm_btn_check_n, lpData->hBm_btn_check_p, atNULL);
	atCONTROL_SetFocusable(hWidget, atFALSE);
	hWidget = ExtPkg_CreateImageButton(hWnd, 1003, 257, 60, 40, 40, lpData->hBm_btn_info_n, lpData->hBm_btn_info_p, atNULL);
	atCONTROL_SetFocusable(hWidget, atFALSE);

	hWidget = ExtPkg_CreateImageButton(hWnd, 1005, 76, 201, 168, 34, lpData->hBm_btn_calcu_n, lpData->hBm_btn_calcu_p, atNULL);
	atCONTROL_SetFocusable(hWidget, atFALSE);

	lpData->hFnt_sub = atUIAPI_CreateFont(atUIAPI_FACE_SYSTEM, 24, atUIAPI_STYLE_PLAIN, atFALSE);

	atWINDOW_AddTimer(hWnd, 0x100, 1000, atTRUE, atTRUE, atTRUE);
}

static void Scr_1_Recover_Exam_WndOnRelease(atHWINDOW hWnd)
{
	LPSCR_1_RECOVER_EXAM_WNDDATA	lpData = (LPSCR_1_RECOVER_EXAM_WNDDATA)atWINDOW_GetDataPtr(hWnd);

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

static int Scr_1_Recover_Exam_WndOnKeyDown(atHWINDOW hWnd, int nKeyCode)
{
	LPSCR_1_RECOVER_EXAM_WNDDATA lpData = (LPSCR_1_RECOVER_EXAM_WNDDATA)atWINDOW_GetDataPtr(hWnd);
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

static int Scr_1_Recover_Exam_WndOnKeyPress(atHWINDOW hWnd, int nKeyCode)
{
	LPSCR_1_RECOVER_EXAM_WNDDATA lpData = (LPSCR_1_RECOVER_EXAM_WNDDATA)atWINDOW_GetDataPtr(hWnd);

    switch( nKeyCode )
    {
        case atVKEY_LEFT:	
			atWINDOW_Refresh( hWnd );
            break;

        case atVKEY_RIGHT:	
			atWINDOW_Refresh( hWnd );
            break;

        case atVKEY_UP:	
			atWINDOW_Refresh(hWnd);
            break;

        case atVKEY_DOWN:	
			atWINDOW_Refresh( hWnd );
            break;
			
		case USERKEY_1:
		{
			/*if (lpData->nType == 101) {
				//nDistRecover = GetDistanceValue();
			}
			else if (lpData->nType == 102) {
				//nDistPenRecover = GetDistanceValue();
				LedControl(atFALSE);
			}*/

			atWINDOW_Refresh(hWnd);
		}
		break;

        default:
            return 0;
    }

    return 1; // WindowOnEvent 가 호출되지 않도록 0을 리턴한다.
}


static void Scr_1_Recover_Exam_WndOnPaint(atHWINDOW hWnd)
{
	LPSCR_1_RECOVER_EXAM_WNDDATA	lpData = (LPSCR_1_RECOVER_EXAM_WNDDATA)atWINDOW_GetDataPtr(hWnd);

	atHGDC gdc = atWINDOW_GetClientGDC(hWnd);
	atREGION rgn;
	atHFONT oldfont;
	atTCHAR str[64];

	atUIAPI_DrawImage(gdc, 0, 0, lpData->hBm_bg);
	//atUIAPI_DrawImage(gdc, 94, 64, lpData->hBm_title);

	if (lpData->nType == 101)
		atUIAPI_sprintf(str, _AT("%d cm"), nDistRecover);
	else if (lpData->nType == 102)
		atUIAPI_sprintf(str, _AT("%d cm"), nDistPenRecover);

	atWINDOW_GetClientRegion(hWnd, &rgn);
	oldfont = atUIAPI_SetFont(gdc, lpData->hFnt_sub);
	atUIAPI_DrawString(gdc, 30, 340, str, atUI_COLOR_BLACK);
	atUIAPI_SetFont(gdc, oldfont);
}


int Scr_1_Recover_Exam_WndOnEvent(atHWINDOW hWnd, int nMsg, long nParam1, long nParam2)
{
	int ret = 0;

	LPSCR_1_RECOVER_EXAM_WNDDATA	lpData = (LPSCR_1_RECOVER_EXAM_WNDDATA)atWINDOW_GetDataPtr(hWnd);
	LPSCR_1_RECOVER_EXAM_WNDDATA  Data = (LPSCR_1_RECOVER_EXAM_WNDDATA)nParam1;

	switch( nMsg )
	{
		case WNDMSG_INIT:
		{
			lpData->nType = Data->nType;
			Scr_1_Recover_Exam_WndOnInit(hWnd);
		}
			break;

		case WNDMSG_RELEASE:
			Scr_1_Recover_Exam_WndOnRelease( hWnd );
			break;

		case WNDMSG_PAINT:
			Scr_1_Recover_Exam_WndOnPaint( hWnd );
			break;

		case WNDMSG_KEYDOWN:
			return Scr_1_Recover_Exam_WndOnKeyDown( hWnd, nParam1 );
			break;

		case WNDMSG_KEYPRESS:
			return Scr_1_Recover_Exam_WndOnKeyPress( hWnd, nParam1 );
			break;

		case WNDMSG_CHANGE:
			Scr_1_Recover_Exam_WndOnChange( hWnd, nParam1, nParam2 );
			break;

		case WNDMSG_COMMAND:
			Scr_1_Recover_Exam_WndOnCommand( hWnd, nParam1, nParam2 );
			break;

		case WNDMSG_TIMER:
			Scr_1_Recover_Exam_WndOnTimer(hWnd, nParam1, nParam2);
			break;

		default:
			return 0;
			break;
	}

	return ret;
}
