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
#include "Screen_1_result.h"
#include "Screen_1_question.h"

#include <string.h>

static SCR_1_QUESTION_WNDDATA descWndData;

static void Scr_1_Result_WndOnCommand(atHWINDOW hWnd, long nParam1, long nParam2)
{
	LPSCR_1_RESULT_WNDDATA lpData = (LPSCR_1_RESULT_WNDDATA)atWINDOW_GetDataPtr(hWnd);
  
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
			atHWINDOW wnd;

			if (lpData->nType == 103) {
				if (!bScr1ResDist) {
					memset(&descWndData, 0, sizeof(SCR_1_QUESTION_WNDDATA));
					descWndData.nType = 102;

					wnd = atAPP_CreateWindow(atAPPMNG_GetCurApp(), atNULL, 0, atNULL, _AT(""), 0, 0, 0, 0, sizeof(SCR_1_QUESTION_WNDDATA), Scr_1_Question_WndOnEvent, &descWndData, atNULL);
					atWINDOW_SetAnchor(wnd, 0, 0, 0, 0);
					atAPP_ShowWindow(atAPPMNG_GetCurApp(), wnd, atTRUE);
				}

				atAPP_DestroyWindow(atAPPMNG_GetCurApp(), hWnd);
			}
			else if (lpData->nType == 104) {
				atAPP_DestroyWindow(atAPPMNG_GetCurApp(), hWnd);
			}
		}
			break;
	}
}


static void Scr_1_Result_WndOnChange(atHWINDOW hWnd, long nParam1, long nParam2)
{
	LPSCR_1_RESULT_WNDDATA lpData = (LPSCR_1_RESULT_WNDDATA)atWINDOW_GetDataPtr(hWnd);

    switch(nParam1)
    {
        case 103 :

			break;
	}
}

static void Scr_1_Result_WndOnInit(atHWINDOW hWnd, long nParam)
{
	LPSCR_1_RESULT_WNDDATA lpData = (LPSCR_1_RESULT_WNDDATA)atWINDOW_GetDataPtr(hWnd);

	atHCONTROL ctl;
	atHCONTROL hWidget;

	//atHRESSRC hResSrc = atUIAPI_AddResSource(_AT("TESTMAIN"), atUI_RES_SRC_RESSTREAM, (const atLPVOID)pUser_ResData, sizeof(pUser_ResData));
	atHRESSRC hResSrc = atUIAPI_GetResSource(_AT("TESTMAIN"));

	lpData->hBm_bg = atUIAPI_CreateBitmapFromRes(hResSrc, _AT("Scr_1_bg.img"));

	switch(lpData->nType) 
	{
		case 103:
		{
			if ((nDistDivid < 11) && (nDistRecover < 11)) {
				bScr1ResDist = atTRUE;
				lpData->hBm_title = atUIAPI_CreateBitmapFromRes(hResSrc, _AT("Scr_1_result_ok_contents.img"));
			}
			else {
				bScr1ResDist = atFALSE;
				lpData->hBm_title = atUIAPI_CreateBitmapFromRes(hResSrc, _AT("Scr_1_result_no_contents.img"));
			}
		}
		break;

		case 104:
		{
			if ((nDistPenDivid < DEF_PEN_DIVID_LIMIT) && (nDistPenRecover < DEF_PEN_RECOVER_LIMIT)) {
				bScr1ResPen = atTRUE;
				lpData->hBm_title = atUIAPI_CreateBitmapFromRes(hResSrc, _AT("Scr_1_result_ok_contents.img"));
			}
			else {
				bScr1ResPen = atFALSE;
				lpData->hBm_title = atUIAPI_CreateBitmapFromRes(hResSrc, _AT("Scr_1_result_no_contents.img"));
			}
		}
		break;
	}


	lpData->hBm_btn_home_n = atUIAPI_CreateBitmapFromRes(hResSrc, _AT("Scr_def_btn_home_n.img"));
	lpData->hBm_btn_check_n = atUIAPI_CreateBitmapFromRes(hResSrc, _AT("Scr_def_btn_check_n.img"));
	lpData->hBm_btn_home_p = atUIAPI_CreateBitmapFromRes(hResSrc, _AT("Scr_def_btn_home_p.img"));
	lpData->hBm_btn_check_p = atUIAPI_CreateBitmapFromRes(hResSrc, _AT("Scr_def_btn_check_p.img"));

	hWidget = ExtPkg_CreateImageButton(hWnd, 1000, 0, 399, 161, 82, lpData->hBm_btn_home_n, lpData->hBm_btn_home_p, atNULL);
	atCONTROL_SetFocusable(hWidget, atFALSE);
	hWidget = ExtPkg_CreateImageButton(hWnd, 1001, 160, 399, 161, 82, lpData->hBm_btn_check_n, lpData->hBm_btn_check_p, atNULL);
	atCONTROL_SetFocusable(hWidget, atFALSE);

	lpData->hFnt_main = atUIAPI_CreateFont(atUIAPI_FACE_SYSTEM, 35, atUIAPI_STYLE_PLAIN, atFALSE);

	
	/*
	// Calculate result
	switch (lpData->nType) 
	{
		case 103:
		{
			if ((nDistDivid > 10) || (nDistRecover > 10))
				lpData->bResult = atFALSE;
			else
				lpData->bResult = atTRUE;
		}
		break;

		case 104:
		{
			if ((nDistPenDivid > DEF_PEN_DIVID_LIMIT) || (nDistPenDivid > DEF_PEN_RECOVER_LIMIT))
				lpData->bResult = atTRUE;
			else
				lpData->bResult = atFALSE;
		}
		break;
	}*/
}

static void Scr_1_Result_WndOnRelease(atHWINDOW hWnd)
{
	LPSCR_1_RESULT_WNDDATA lpData = (LPSCR_1_RESULT_WNDDATA)atWINDOW_GetDataPtr(hWnd);

	atUIAPI_DestroyBitmap(lpData->hBm_bg);
	atUIAPI_DestroyBitmap(lpData->hBm_title);

	atUIAPI_DestroyFont(lpData->hFnt_main);

	atUIAPI_DestroyBitmap(lpData->hBm_btn_check_n);
	atUIAPI_DestroyBitmap(lpData->hBm_btn_home_n);
	atUIAPI_DestroyBitmap(lpData->hBm_btn_check_p);
	atUIAPI_DestroyBitmap(lpData->hBm_btn_home_p);
}

static int Scr_1_Result_WndOnKeyDown(atHWINDOW hWnd, int nKeyCode)
{
	LPSCR_1_RESULT_WNDDATA lpData = (LPSCR_1_RESULT_WNDDATA)atWINDOW_GetDataPtr(hWnd);
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

static int Scr_1_Result_WndOnKeyPress(atHWINDOW hWnd, int nKeyCode)
{
	LPSCR_1_RESULT_WNDDATA lpData = (LPSCR_1_RESULT_WNDDATA)atWINDOW_GetDataPtr(hWnd);

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


static void Scr_1_Result_WndOnPaint(atHWINDOW hWnd)
{
	LPSCR_1_RESULT_WNDDATA	lpData = (LPSCR_1_RESULT_WNDDATA)atWINDOW_GetDataPtr(hWnd);

	atHGDC gdc = atWINDOW_GetClientGDC(hWnd);
	atREGION rgn;
	atHFONT oldfont;
	atTCHAR str[64];

	atUIAPI_DrawImage(gdc, 0, 0, lpData->hBm_bg);

	atWINDOW_GetClientRegion(hWnd, &rgn);
	oldfont = atUIAPI_SetFont(gdc, lpData->hFnt_main);

	switch (lpData->nType)
	{
		case 103:
		{
			if (bScr1ResDist) {
				atUIAPI_DrawImage(gdc, 49, 180, lpData->hBm_title);
			}
			else {
				atUIAPI_DrawImage(gdc, 49, 110, lpData->hBm_title);

				atUIAPI_sprintf(str, _AT("%d"), nDistDivid);
				atUIAPI_DrawStringAlign(gdc, 172, 260, str, ALIGN_CENTER_MIDDLE, atUI_COLOR_RED);

				atUIAPI_sprintf(str, _AT("%d"), nDistRecover);
				atUIAPI_DrawStringAlign(gdc, 172, 320, str, ALIGN_CENTER_MIDDLE, atUI_COLOR_RED);
			}
		}
			break;

		case 104:
		{
			if (bScr1ResPen) {
				atUIAPI_DrawImage(gdc, 49, 180, lpData->hBm_title);
			}
			else {
				atUIAPI_DrawImage(gdc, 49, 110, lpData->hBm_title);
				atUIAPI_sprintf(str, _AT("%d"), nDistPenDivid);
				atUIAPI_DrawStringAlign(gdc, 172, 260, str, ALIGN_CENTER_MIDDLE, atUI_COLOR_RED);

				atUIAPI_sprintf(str, _AT("%d"), nDistPenRecover);
				atUIAPI_DrawStringAlign(gdc, 172, 320, str, ALIGN_CENTER_MIDDLE, atUI_COLOR_RED);
			}
		}
			break;
	}

	atUIAPI_SetFont(gdc, oldfont);
}


int Scr_1_Result_WndOnEvent(atHWINDOW hWnd, int nMsg, long nParam1, long nParam2)
{
	int ret = 0;
	LPSCR_1_RESULT_WNDDATA	lpData = (LPSCR_1_RESULT_WNDDATA)atWINDOW_GetDataPtr(hWnd);
	LPSCR_1_RESULT_WNDDATA  Data = (LPSCR_1_RESULT_WNDDATA)nParam1;
	
	switch( nMsg )
	{
		case WNDMSG_INIT:
		{
			lpData->nType = Data->nType;
			Scr_1_Result_WndOnInit(hWnd, nParam1);
		}
			break;
		case WNDMSG_RELEASE:
			Scr_1_Result_WndOnRelease( hWnd );
			break;
		case WNDMSG_PAINT:
			Scr_1_Result_WndOnPaint( hWnd );
			break;
		case WNDMSG_KEYDOWN:
			return Scr_1_Result_WndOnKeyDown( hWnd, nParam1 );
			break;
		case WNDMSG_KEYPRESS:
			return Scr_1_Result_WndOnKeyPress( hWnd, nParam1 );
			break;
		case WNDMSG_CHANGE:
			Scr_1_Result_WndOnChange( hWnd, nParam1, nParam2 );
			break;
		case WNDMSG_COMMAND:
			Scr_1_Result_WndOnCommand( hWnd, nParam1, nParam2 );
			break;
		default:
			return 0;
			break;
	}

	return ret;
}
