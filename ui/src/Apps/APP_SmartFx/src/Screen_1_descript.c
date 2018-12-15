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
#include "Screen_1_descript.h"
#include "Screen_1_divid_exam.h"

static SCR_1_DIVID_EXAM_WNDDATA descWndData;

static void Scr_1_Desc_WndOnCommand(atHWINDOW hWnd, long nParam1, long nParam2)
{
	LPSCR_1_DESC_WNDDATA lpData = (LPSCR_1_DESC_WNDDATA)atWINDOW_GetDataPtr(hWnd);
	//LPSCR_1_DESC_1_WNDDATA  Data = (LPSCR_1_DESC_1_WNDDATA)nParam2;
  
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
			memset(&descWndData, 0, sizeof(SCR_1_DIVID_EXAM_WNDDATA));
			if (lpData->nType == 101) 
				descWndData.nType = 101;
			else if (lpData->nType == 102) 
				descWndData.nType = 102;

			atHWINDOW wnd;
			wnd = atAPP_CreateWindow(atAPPMNG_GetCurApp(), atNULL, 0, atNULL, _AT(""), 0, 0, 0, 0, sizeof(SCR_1_DIVID_EXAM_WNDDATA), Scr_1_Divid_Exam_WndOnEvent, &descWndData, atNULL);
			atWINDOW_SetAnchor(wnd, 0, 0, 0, 0);
			atAPP_ShowWindow(atAPPMNG_GetCurApp(), wnd, atTRUE);

			atAPP_DestroyWindow(atAPPMNG_GetCurApp(), hWnd);
		}
			break;
	}
}


static void Scr_1_Desc_WndOnChange(atHWINDOW hWnd, long nParam1, long nParam2)
{
	LPSCR_1_DESC_WNDDATA lpData = (LPSCR_1_DESC_WNDDATA)atWINDOW_GetDataPtr(hWnd);

    switch(nParam1)
    {
        case 103 :

			break;
	}
}


static void Scr_1_Desc_WndOnInit(atHWINDOW hWnd, long nParam)
{
	LPSCR_1_DESC_WNDDATA lpData = (LPSCR_1_DESC_WNDDATA)atWINDOW_GetDataPtr(hWnd);

	atHCONTROL ctl;
	atHCONTROL hWidget;

	//atHRESSRC hResSrc = atUIAPI_AddResSource(_AT("TESTMAIN"), atUI_RES_SRC_RESSTREAM, (const atLPVOID)pUser_ResData, sizeof(pUser_ResData));
	atHRESSRC hResSrc = atUIAPI_GetResSource(_AT("TESTMAIN"));
	lpData->hBm_bg = atUIAPI_CreateBitmapFromRes(hResSrc, _AT("Scr_1_bg.img"));

	if(lpData->nType == 101)
		lpData->hBm_title = atUIAPI_CreateBitmapFromRes(hResSrc, _AT("Scr_1_desc_1_content.img"));
	else if(lpData->nType == 102)
		lpData->hBm_title = atUIAPI_CreateBitmapFromRes(hResSrc, _AT("Scr_1_desc_2_content.img"));

	lpData->hBm_btn_ok_n = atUIAPI_CreateBitmapFromRes(hResSrc, _AT("Scr_def_btn_ok_n.img"));
	lpData->hBm_btn_ok_p = atUIAPI_CreateBitmapFromRes(hResSrc, _AT("Scr_def_btn_ok_p.img"));

	hWidget = ExtPkg_CreateImageButton(hWnd, 1001, 98, 398, 126, 64, lpData->hBm_btn_ok_n, lpData->hBm_btn_ok_p, atNULL);
	atCONTROL_SetFocusable(hWidget, atFALSE);
}

static void Scr_1_Desc_WndOnRelease(atHWINDOW hWnd)
{
	LPSCR_1_DESC_WNDDATA	lpData = (LPSCR_1_DESC_WNDDATA)atWINDOW_GetDataPtr(hWnd);

	atUIAPI_DestroyBitmap(lpData->hBm_bg);
	atUIAPI_DestroyBitmap(lpData->hBm_title);
	atUIAPI_DestroyBitmap(lpData->hBm_btn_ok_n);
	atUIAPI_DestroyBitmap(lpData->hBm_btn_ok_p);

	atUIAPI_DestroyFont(lpData->hFnt_main);	;
}


static int Scr_1_Desc_WndOnKeyDown(atHWINDOW hWnd, int nKeyCode)
{
	LPSCR_1_DESC_WNDDATA lpData = (LPSCR_1_DESC_WNDDATA)atWINDOW_GetDataPtr(hWnd);
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

			// Hardware key
	   case atVKEY_SELECT :
		    break;
			
        default:
            return 0;
    }

    return 0; // WindowOnEvent 가 호출되지 않도록 0을 리턴한다.
}


#define DELTA 5

static int Scr_1_Desc_WndOnKeyPress(atHWINDOW hWnd, int nKeyCode)
{
	LPSCR_1_DESC_WNDDATA lpData = (LPSCR_1_DESC_WNDDATA)atWINDOW_GetDataPtr(hWnd);

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


static void Scr_1_Desc_WndOnPaint(atHWINDOW hWnd)
{
	LPSCR_1_DESC_WNDDATA lpData = (LPSCR_1_DESC_WNDDATA)atWINDOW_GetDataPtr(hWnd);
	atHGDC gdc = atWINDOW_GetClientGDC(hWnd);
	atREGION rgn;
	atHFONT oldfont;
	atTCHAR str[64];

	atUIAPI_DrawImage(gdc, 0, 0, lpData->hBm_bg);
	if (lpData->nType == 101)
		atUIAPI_DrawImage(gdc, 65, 154, lpData->hBm_title);
	else if (lpData->nType == 102)
		atUIAPI_DrawImage(gdc, 35, 138, lpData->hBm_title);
}


int Scr_1_Desc_WndOnEvent(atHWINDOW hWnd, int nMsg, long nParam1, long nParam2)
{
	LPSCR_1_DESC_WNDDATA	lpData = (LPSCR_1_DESC_WNDDATA)atWINDOW_GetDataPtr(hWnd);
	LPSCR_1_DESC_WNDDATA  Data = (LPSCR_1_DESC_WNDDATA)nParam1;

	switch( nMsg )
	{
		case WNDMSG_INIT:
		{
			lpData->nType = Data->nType;
			Scr_1_Desc_WndOnInit(hWnd, nParam1);
		}
			break;
		case WNDMSG_RELEASE:
			Scr_1_Desc_WndOnRelease( hWnd );
			break;
		case WNDMSG_PAINT:
			Scr_1_Desc_WndOnPaint( hWnd );
			break;
		case WNDMSG_KEYDOWN:
			return Scr_1_Desc_WndOnKeyDown( hWnd, nParam1 );
			break;
		case WNDMSG_KEYPRESS:
			return Scr_1_Desc_WndOnKeyPress( hWnd, nParam1 );
			break;
		case WNDMSG_CHANGE:
			Scr_1_Desc_WndOnChange( hWnd, nParam1, nParam2 );
			break;
		case WNDMSG_COMMAND:
			Scr_1_Desc_WndOnCommand( hWnd, nParam1, nParam2 );
			break;
		default:
			break;
	}

	return 0;
}
