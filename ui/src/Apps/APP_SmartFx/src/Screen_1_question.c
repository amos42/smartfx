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
#include "Screen_1_question.h"
#include "Screen_1_descript.h"
//#include "Screen_1_result.h"

#include <string.h>

static SCR_1_DESC_WNDDATA descWndData;
//static SCR_1_RESULT_WNDDATA resultWndData;

static void Scr_1_Question_WndOnCommand(atHWINDOW hWnd, long nParam1, long nParam2)
{
	LPSCR_1_QUESTION_WNDDATA lpData = (LPSCR_1_QUESTION_WNDDATA)atWINDOW_GetDataPtr(hWnd);
  
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
			if (lpData->nType == 102) {
				memset(&descWndData, 0, sizeof(SCR_1_DESC_WNDDATA));
				descWndData.nType = 102;

				atHWINDOW wnd;
				wnd = atAPP_CreateWindow(atAPPMNG_GetCurApp(), atNULL, 0, atNULL, _AT(""), 0, 0, 0, 0, sizeof(SCR_1_DESC_WNDDATA), Scr_1_Desc_WndOnEvent, &descWndData, atNULL);
				atWINDOW_SetAnchor(wnd, 0, 0, 0, 0);
				atAPP_ShowWindow(atAPPMNG_GetCurApp(), wnd, atTRUE);
			}

			atAPP_DestroyWindow(atAPPMNG_GetCurApp(), hWnd);
		}
			break;

		case 1002:
		{
			/*if (lpData->nType == 102) {
				memset(&resultWndData, 0, sizeof(SCR_1_RESULT_WNDDATA));
				resultWndData.nType = 103;

				atHWINDOW wnd;
				wnd = atAPP_CreateWindow(atAPPMNG_GetCurApp(), atNULL, 0, atNULL, _AT(""), 0, 0, 0, 0, sizeof(SCR_1_RESULT_WNDDATA), Scr_1_Result_WndOnEvent, &resultWndData, atNULL);
				atWINDOW_SetAnchor(wnd, 0, 0, 0, 0);
				atAPP_ShowWindow(atAPPMNG_GetCurApp(), wnd, atTRUE);
			}*/

			atAPP_DestroyWindow(atAPPMNG_GetCurApp(), hWnd);
		}
			break;
	}
}


static void Scr_1_Question_WndOnChange(atHWINDOW hWnd, long nParam1, long nParam2)
{
	LPSCR_1_QUESTION_WNDDATA lpData = (LPSCR_1_QUESTION_WNDDATA)atWINDOW_GetDataPtr(hWnd);

    switch(nParam1)
    {
        case 103 :

			break;
	}
}

static atINT xxx( atLISTBOX_EXT *lpListBox, atLPVOID lpParam )
{
	return 8;
}



static void Scr_1_Question_WndOnInit(atHWINDOW hWnd)
{
	LPSCR_1_QUESTION_WNDDATA	lpData = (LPSCR_1_QUESTION_WNDDATA)atWINDOW_GetDataPtr(hWnd);

	atHCONTROL ctl;
	atHCONTROL hWidget;

	//atHRESSRC hResSrc = atUIAPI_AddResSource(_AT("TESTMAIN"), atUI_RES_SRC_RESSTREAM, (const atLPVOID)pUser_ResData, sizeof(pUser_ResData));
	atHRESSRC hResSrc = atUIAPI_GetResSource(_AT("TESTMAIN"));

	lpData->hBm_bg = atUIAPI_CreateBitmapFromRes(hResSrc, _AT("Scr_1_bg.img"));
	lpData->hBm_title = atUIAPI_CreateBitmapFromRes(hResSrc, _AT("Scr_1_question_title.img"));
	lpData->hBm_content = atUIAPI_CreateBitmapFromRes(hResSrc, _AT("Scr_1_question_content.img"));

	lpData->hBm_btn_yes_n = atUIAPI_CreateBitmapFromRes(hResSrc, _AT("Scr_def_btn_ok_n.img"));
	lpData->hBm_btn_no_n = atUIAPI_CreateBitmapFromRes(hResSrc, _AT("Scr_def_btn_no_n.img"));
	lpData->hBm_btn_yes_p = atUIAPI_CreateBitmapFromRes(hResSrc, _AT("Scr_def_btn_ok_p.img"));
	lpData->hBm_btn_no_p = atUIAPI_CreateBitmapFromRes(hResSrc, _AT("Scr_def_btn_no_p.img"));

	//hWidget = BasePkg_CreateImageBox(atNULL, 3000, 0, 0, 320, 480, lpData->hBm_bg, 1);
	//hWidget = BasePkg_CreateImageBox(atNULL, 4000, 65, 50, 301, 327, lpData->hBm_title, 1);
	//hWidget = BasePkg_CreateImageBox(atNULL, 5000, 41, 162, 301, 327, lpData->hBm_content, 1);

	hWidget = ExtPkg_CreateImageButton(hWnd, 1001, 160, 397, 124, 64, lpData->hBm_btn_yes_n, lpData->hBm_btn_yes_p, atNULL);
	atCONTROL_SetFocusable(hWidget, atFALSE);
	hWidget = ExtPkg_CreateImageButton(hWnd, 1002, 35, 397, 124, 64, lpData->hBm_btn_no_n, lpData->hBm_btn_no_p, atNULL);
	atCONTROL_SetFocusable(hWidget, atFALSE);
}



static void Scr_1_Question_WndOnRelease(atHWINDOW hWnd)
{
	LPSCR_1_QUESTION_WNDDATA	lpData = (LPSCR_1_QUESTION_WNDDATA)atWINDOW_GetDataPtr(hWnd);

	atUIAPI_DestroyBitmap(lpData->hBm_bg);
	atUIAPI_DestroyBitmap(lpData->hBm_title);
	atUIAPI_DestroyBitmap(lpData->hBm_content);

	atUIAPI_DestroyBitmap(lpData->hBm_btn_yes_n);
	atUIAPI_DestroyBitmap(lpData->hBm_btn_no_n);
	atUIAPI_DestroyBitmap(lpData->hBm_btn_yes_p);
	atUIAPI_DestroyBitmap(lpData->hBm_btn_no_p);
}


static int Scr_1_Question_WndOnKeyDown(atHWINDOW hWnd, int nKeyCode)
{
	LPSCR_1_QUESTION_WNDDATA lpData = (LPSCR_1_QUESTION_WNDDATA)atWINDOW_GetDataPtr(hWnd);
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

static int Scr_1_Question_WndOnKeyPress(atHWINDOW hWnd, int nKeyCode)
{
	LPSCR_1_QUESTION_WNDDATA lpData = (LPSCR_1_QUESTION_WNDDATA)atWINDOW_GetDataPtr(hWnd);

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


static void Scr_1_Question_WndOnPaint(atHWINDOW hWnd)
{
	LPSCR_1_QUESTION_WNDDATA	lpData = (LPSCR_1_QUESTION_WNDDATA)atWINDOW_GetDataPtr(hWnd);

	atHGDC gdc = atWINDOW_GetClientGDC(hWnd);
	atREGION rgn;
	atHFONT oldfont;
	atTCHAR str[64];

	atUIAPI_DrawImage(gdc, 0, 0, lpData->hBm_bg);
	atUIAPI_DrawImage(gdc, 65, 50, lpData->hBm_title);
	atUIAPI_DrawImage(gdc, 41, 162, lpData->hBm_content);
}


int Scr_1_Question_WndOnEvent(atHWINDOW hWnd, int nMsg, long nParam1, long nParam2)
{
	int ret = 0;

	LPSCR_1_QUESTION_WNDDATA	lpData = (LPSCR_1_QUESTION_WNDDATA)atWINDOW_GetDataPtr(hWnd);
	LPSCR_1_QUESTION_WNDDATA  Data = (LPSCR_1_QUESTION_WNDDATA)nParam1;

	switch( nMsg )
	{
		case WNDMSG_INIT:
			lpData->nType = Data->nType;
			Scr_1_Question_WndOnInit( hWnd );
			break;
		case WNDMSG_RELEASE:
			Scr_1_Question_WndOnRelease( hWnd );
			break;
		case WNDMSG_PAINT:
			Scr_1_Question_WndOnPaint( hWnd );
			break;
		case WNDMSG_KEYDOWN:
			return Scr_1_Question_WndOnKeyDown( hWnd, nParam1 );
			break;
		case WNDMSG_KEYPRESS:
			return Scr_1_Question_WndOnKeyPress( hWnd, nParam1 );
			break;
		case WNDMSG_CHANGE:
			Scr_1_Question_WndOnChange( hWnd, nParam1, nParam2 );
			break;
		case WNDMSG_COMMAND:
			Scr_1_Question_WndOnCommand( hWnd, nParam1, nParam2 );
			break;
		default:
			return 0;
			break;
	}

	return ret;
}
