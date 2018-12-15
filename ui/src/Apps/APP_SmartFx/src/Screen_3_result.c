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
#include "Screen_3_result.h"

static void Scr_3_Result_WndOnCommand(atHWINDOW hWnd, long nParam1, long nParam2)
{
	LPSCR_3_RESULT_WNDDATA lpData = (LPSCR_3_RESULT_WNDDATA)atWINDOW_GetDataPtr(hWnd);
  
    switch(nParam1)
    {
		case 1001:
			atAPP_DestroyWindow(atAPPMNG_GetCurApp(), hWnd);
			break;
	}
}


static void Scr_3_Result_WndOnChange(atHWINDOW hWnd, long nParam1, long nParam2)
{
	LPSCR_3_RESULT_WNDDATA lpData = (LPSCR_3_RESULT_WNDDATA)atWINDOW_GetDataPtr(hWnd);

    switch(nParam1)
    {
        case 103 :

			break;
	}
}


static void Scr_3_Result_WndOnInit(atHWINDOW hWnd, long nParam)
{
	LPSCR_3_RESULT_WNDDATA lpData = (LPSCR_3_RESULT_WNDDATA)atWINDOW_GetDataPtr(hWnd);

	atHCONTROL ctl;
	atHCONTROL hWidget;

	//atHRESSRC hResSrc = atUIAPI_AddResSource(_AT("TESTMAIN"), atUI_RES_SRC_RESSTREAM, (const atLPVOID)pUser_ResData, sizeof(pUser_ResData));
	atHRESSRC hResSrc = atUIAPI_GetResSource(_AT("TESTMAIN"));
	lpData->hBm_bg = atUIAPI_CreateBitmapFromRes(hResSrc, _AT("Scr_1_bg.img"));
	lpData->hBm_title = atUIAPI_CreateBitmapFromRes(hResSrc, _AT("Scr_3_special_result.img"));

	lpData->hBm_btn_back_n = atUIAPI_CreateBitmapFromRes(hResSrc, _AT("Scr_def_btn_ok_n.img"));
	lpData->hBm_btn_back_p = atUIAPI_CreateBitmapFromRes(hResSrc, _AT("Scr_def_btn_ok_p.img"));

	hWidget = ExtPkg_CreateImageButton(hWnd, 1001, 98, 398, 126, 64, lpData->hBm_btn_back_n, lpData->hBm_btn_back_p, atNULL);
	atCONTROL_SetFocusable(hWidget, atFALSE);
}

static void Scr_3_Result_WndOnRelease(atHWINDOW hWnd)
{
	LPSCR_3_RESULT_WNDDATA	lpData = (LPSCR_3_RESULT_WNDDATA)atWINDOW_GetDataPtr(hWnd);

	atUIAPI_DestroyBitmap(lpData->hBm_bg);
	atUIAPI_DestroyBitmap(lpData->hBm_title);
	atUIAPI_DestroyBitmap(lpData->hBm_btn_back_n);
	atUIAPI_DestroyBitmap(lpData->hBm_btn_back_p);
}


static int Scr_3_Result_WndOnKeyDown(atHWINDOW hWnd, int nKeyCode)
{
	LPSCR_3_RESULT_WNDDATA lpData = (LPSCR_3_RESULT_WNDDATA)atWINDOW_GetDataPtr(hWnd);

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

static int Scr_3_Result_WndOnKeyPress(atHWINDOW hWnd, int nKeyCode)
{
	LPSCR_3_RESULT_WNDDATA lpData = (LPSCR_3_RESULT_WNDDATA)atWINDOW_GetDataPtr(hWnd);

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


static void Scr_3_Result_WndOnPaint(atHWINDOW hWnd)
{
	LPSCR_3_RESULT_WNDDATA lpData = (LPSCR_3_RESULT_WNDDATA)atWINDOW_GetDataPtr(hWnd);

	atHGDC gdc = atWINDOW_GetClientGDC(hWnd);

	atUIAPI_DrawImage(gdc, 0, 0, lpData->hBm_bg);
	atUIAPI_DrawImage(gdc, 33, 90, lpData->hBm_title);
}


int Scr_3_Result_WndOnEvent(atHWINDOW hWnd, int nMsg, long nParam1, long nParam2)
{
	LPSCR_3_RESULT_WNDDATA	lpData = (LPSCR_3_RESULT_WNDDATA)atWINDOW_GetDataPtr(hWnd);
	LPSCR_3_RESULT_WNDDATA  Data = (LPSCR_3_RESULT_WNDDATA)nParam1;

	switch( nMsg )
	{
		case WNDMSG_INIT:
		{
			lpData->nType = Data->nType;
			Scr_3_Result_WndOnInit(hWnd, nParam1);
		}
			break;

		case WNDMSG_RELEASE:
			Scr_3_Result_WndOnRelease( hWnd );
			break;

		case WNDMSG_PAINT:
			Scr_3_Result_WndOnPaint( hWnd );
			break;

		case WNDMSG_KEYDOWN:
			return Scr_3_Result_WndOnKeyDown( hWnd, nParam1 );
			break;

		case WNDMSG_KEYPRESS:
			return Scr_3_Result_WndOnKeyPress( hWnd, nParam1 );
			break;

		case WNDMSG_CHANGE:
			Scr_3_Result_WndOnChange( hWnd, nParam1, nParam2 );
			break;

		case WNDMSG_COMMAND:
			Scr_3_Result_WndOnCommand( hWnd, nParam1, nParam2 );
			break;

		default:
			return 0;
			break;
	}

	return 0;
}
