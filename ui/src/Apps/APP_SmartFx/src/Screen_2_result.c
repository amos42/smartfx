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
#include "Screen_2_result.h"

#include <string.h>

static void Scr_2_Result_WndOnCommand(atHWINDOW hWnd, long nParam1, long nParam2)
{
	LPSCR_2_RESULT_WNDDATA lpData = (LPSCR_2_RESULT_WNDDATA)atWINDOW_GetDataPtr(hWnd);
  
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
			/** send 
			 * case 219 // Right + Left + Read 
			 * case 229 // Right + Read
			 * case 239 // Right
			 * case 249 // Right + Left
			 * case 400 // Main Result
			*/

			atAPP_DestroyWindow(atAPPMNG_GetCurApp(), hWnd);
		}
			break;
	}
}


static void Scr_2_Result_WndOnChange(atHWINDOW hWnd, long nParam1, long nParam2)
{
	LPSCR_2_RESULT_WNDDATA lpData = (LPSCR_2_RESULT_WNDDATA)atWINDOW_GetDataPtr(hWnd);

    switch(nParam1)
    {
        case 103 :

			break;
	}
}

static void Scr_2_RESULT_WndOnInit(atHWINDOW hWnd, long nParam)
{
	LPSCR_2_RESULT_WNDDATA lpData = (LPSCR_2_RESULT_WNDDATA)atWINDOW_GetDataPtr(hWnd);

	atHCONTROL ctl;
	atHCONTROL hWidget;

	lpData->bResult = atFALSE;

	nDistChart = 1;

	//atHRESSRC hResSrc = atUIAPI_AddResSource(_AT("TESTMAIN"), atUI_RES_SRC_RESSTREAM, (const atLPVOID)pUser_ResData, sizeof(pUser_ResData));
	atHRESSRC hResSrc = atUIAPI_GetResSource(_AT("TESTMAIN"));

	lpData->hBm_bg = atUIAPI_CreateBitmapFromRes(hResSrc, _AT("Scr_1_bg.img"));
	lpData->hBm_title = atUIAPI_CreateBitmapFromRes(hResSrc, _AT("Scr_2_result_title.img"));

	lpData->hBm_btn_home_n = atUIAPI_CreateBitmapFromRes(hResSrc, _AT("Scr_def_btn_home_n.img"));
	lpData->hBm_btn_check_n = atUIAPI_CreateBitmapFromRes(hResSrc, _AT("Scr_def_btn_check_n.img"));
	lpData->hBm_btn_home_p = atUIAPI_CreateBitmapFromRes(hResSrc, _AT("Scr_def_btn_home_p.img"));
	lpData->hBm_btn_check_p = atUIAPI_CreateBitmapFromRes(hResSrc, _AT("Scr_def_btn_check_p.img"));

	hWidget = ExtPkg_CreateImageButton(hWnd, 1000, 0, 399, 161, 82, lpData->hBm_btn_home_n, lpData->hBm_btn_home_p, atNULL);
	atCONTROL_SetFocusable(hWidget, atFALSE);
	hWidget = ExtPkg_CreateImageButton(hWnd, 1001, 160, 399, 161, 82, lpData->hBm_btn_check_n, lpData->hBm_btn_check_p, atNULL);
	atCONTROL_SetFocusable(hWidget, atFALSE);

	lpData->hFnt_main = atUIAPI_CreateFont(atUIAPI_FACE_SYSTEM, 16, atUIAPI_STYLE_PLAIN, atFALSE);

	if (nDistRight == 0)
		nDistRight = 1;

	if (nDistLeft == 0)
		nDistLeft = 1;

	//nDistLeft = 27;
	//nDistRight = 19;
	//nDistRead = 32;

	lpData->fContL = 100.0 / nDistLeft;
	lpData->fContR = 100.0 / nDistRight;
	lpData->fInL = (100.0 / nDistRead) - (lpData->fContL / 2.0);
	lpData->fInR = (100.0 / nDistRead) - (lpData->fContR / 2.0);
	lpData->fBrL_A = 100.0 / (lpData->fContL + lpData->fInL);
	lpData->fBrL_B = 100.0 / lpData->fInL;
	lpData->fBrR_A = 100.0 / (lpData->fContR + lpData->fInR);
	lpData->fBrR_B = 100.0 / lpData->fInR;

	atWINDOW_RefreshClient(hWnd);
}

static void Scr_2_Result_WndOnRelease(atHWINDOW hWnd)
{
	LPSCR_2_RESULT_WNDDATA lpData = (LPSCR_2_RESULT_WNDDATA)atWINDOW_GetDataPtr(hWnd);

	atUIAPI_DestroyBitmap(lpData->hBm_bg);
	atUIAPI_DestroyBitmap(lpData->hBm_title);

	atUIAPI_DestroyFont(lpData->hFnt_main);

	atUIAPI_DestroyBitmap(lpData->hBm_btn_check_n);
	atUIAPI_DestroyBitmap(lpData->hBm_btn_home_n);
	atUIAPI_DestroyBitmap(lpData->hBm_btn_check_p);
	atUIAPI_DestroyBitmap(lpData->hBm_btn_home_p);
}


static int Scr_2_Result_WndOnKeyDown(atHWINDOW hWnd, int nKeyCode)
{
	LPSCR_2_RESULT_WNDDATA lpData = (LPSCR_2_RESULT_WNDDATA)atWINDOW_GetDataPtr(hWnd);
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

static int Scr_2_Result_WndOnKeyPress(atHWINDOW hWnd, int nKeyCode)
{
	LPSCR_2_RESULT_WNDDATA lpData = (LPSCR_2_RESULT_WNDDATA)atWINDOW_GetDataPtr(hWnd);

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


static void Scr_2_Result_WndOnPaint(atHWINDOW hWnd)
{
	LPSCR_2_RESULT_WNDDATA	lpData = (LPSCR_2_RESULT_WNDDATA)atWINDOW_GetDataPtr(hWnd);

	atHGDC gdc = atWINDOW_GetClientGDC(hWnd);
	atREGION rgn;
	atHFONT oldfont;
	atTCHAR str[64];

	atUIAPI_DrawImage(gdc, 0, 0, lpData->hBm_bg);
	atUIAPI_DrawImage(gdc, 108, 64, lpData->hBm_title);

	oldfont = atUIAPI_SetFont(gdc, lpData->hFnt_main);

	atU16 lmargin = 15;
	// X : 85 -> lmargin
	atUIAPI_sprintf(str, _AT("독서거리 : %d cm"), nDistRead);
	atUIAPI_DrawStringAlign(gdc, lmargin, 123, str, ALIGN_LEFT_MIDDLE, atUI_COLOR_BLACK);

	atUIAPI_sprintf(str, _AT("조절 근점 : R) %d cm / L) %d cm"), nDistRight, nDistLeft);
	atUIAPI_DrawStringAlign(gdc, lmargin, 151, str, ALIGN_LEFT_MIDDLE, atUI_COLOR_BLACK);

	atUIAPI_sprintf(str, _AT("조절력     : R) +%.2f D / L) +%.2f D"), lpData->fContR, lpData->fContL);
	atUIAPI_DrawStringAlign(gdc, lmargin, 178, str, ALIGN_LEFT_MIDDLE, atUI_COLOR_BLACK);


	if (lpData->fInL <= 0)
		lpData->fInL = 0.00;

	if (lpData->fInR <= 0)
		lpData->fInR = 0.00;


	atUIAPI_sprintf(str, _AT("가입도"), lpData->fInL);
	atUIAPI_DrawStringAlign(gdc, lmargin, 211, str, ALIGN_LEFT_MIDDLE, atUI_COLOR_BLACK);

	// X ; 70 -> lmargin
	atUIAPI_sprintf(str, _AT("우안 : + %.2f D"), lpData->fInR);
	atUIAPI_DrawStringAlign(gdc, lmargin, 238, str, ALIGN_LEFT_MIDDLE, atUI_COLOR_BLACK);

	atUIAPI_sprintf(str, _AT("좌안 : + %.2f D"), lpData->fInL);
	atUIAPI_DrawStringAlign(gdc, lmargin, 265, str, ALIGN_LEFT_MIDDLE, atUI_COLOR_BLACK);

	atUIAPI_sprintf(str, _AT("명시거리"), lpData->fInL);
	atUIAPI_DrawStringAlign(gdc, lmargin, 299, str, ALIGN_LEFT_MIDDLE, atUI_COLOR_BLACK);

	if (lpData->fInR > 0) {
		atUIAPI_sprintf(str, _AT("우안 : %d ~ %d cm"), (int)(lpData->fBrR_A), (int)(lpData->fBrR_B));
		atUIAPI_DrawStringAlign(gdc, lmargin, 326, str, ALIGN_LEFT_MIDDLE, atUI_COLOR_BLACK);
	}

	if (lpData->fInL > 0) {
		atUIAPI_sprintf(str, _AT("좌안 : %d ~ %d cm"), (int)(lpData->fBrL_A), (int)(lpData->fBrL_B));
		atUIAPI_DrawStringAlign(gdc, lmargin, 355, str, ALIGN_LEFT_MIDDLE, atUI_COLOR_BLACK);
	}

	atUIAPI_SetFont(gdc, oldfont);
}


int Scr_2_Result_WndOnEvent(atHWINDOW hWnd, int nMsg, long nParam1, long nParam2)
{
	LPSCR_2_RESULT_WNDDATA	lpData = (LPSCR_2_RESULT_WNDDATA)atWINDOW_GetDataPtr(hWnd);
	LPSCR_2_RESULT_WNDDATA  Data = (LPSCR_2_RESULT_WNDDATA)nParam1;
	
	switch( nMsg )
	{
		case WNDMSG_INIT:
		{
			lpData->nType = Data->nType;
			Scr_2_RESULT_WndOnInit(hWnd, nParam1);
		}
			break;

		case WNDMSG_RELEASE:
			Scr_2_Result_WndOnRelease( hWnd );
			break;

		case WNDMSG_PAINT:
			Scr_2_Result_WndOnPaint( hWnd );
			break;

		case WNDMSG_KEYDOWN:
			return Scr_2_Result_WndOnKeyDown( hWnd, nParam1 );
			break;

		case WNDMSG_KEYPRESS:
			return Scr_2_Result_WndOnKeyPress( hWnd, nParam1 );
			break;

		case WNDMSG_CHANGE:
			Scr_2_Result_WndOnChange( hWnd, nParam1, nParam2 );
			break;

		case WNDMSG_COMMAND:
			Scr_2_Result_WndOnCommand( hWnd, nParam1, nParam2 );
			break;

		default:
			return 0;
			break;
	}

	return 0;
}
