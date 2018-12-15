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
#include "Screen_information.h"


/*
Scr_info_1_divid_1.img			; 10011
Scr_info_1_divid_2.img			; 10012
Scr_info_1_divid_3.img			; 10013
Scr_info_1_recover_1.img		; 10021
Scr_info_1_recover_2.img		; 10022
Scr_info_1_pen_divid_1.img		; 10031
Scr_info_1_pen_divid_2.img		; 10032
Scr_info_1_pen_recover_1.img	; 10041
Scr_info_1_pen_recover_2.img	; 10042
//Scr_info_1_penlight_1.img
//Scr_info_1_penlight_2.img

Scr_info_2_eyes_1.img			; 20011
Scr_info_2_eyes_2.img			; 20012
Scr_info_2_read_1.img			; 20031
Scr_info_2_read_2.img			; 20032

Scr_info_3_special_1_1.img		; 30011
Scr_info_3_special_1_2.img		; 30012
Scr_info_3_special_2.img		; 30020
Scr_info_3_special_3.img		; 30030
Scr_info_3_special_4_1.img		; 30041
Scr_info_3_special_5.img		; 30050
Scr_info_3_special_6.img		; 30060
Scr_info_3_special_7.img		; 30070
Scr_info_3_vision_1.img			; 30110
Scr_info_3_vision_2.img			; 30120
Scr_info_3_vision_3.img			; 30130
Scr_info_3_vision_4.img			; 30140
Scr_info_3_vision_5.img			; 30150
*/


//#define GDI_TEST

static void Scr_info_WndOnCommand(atHWINDOW hWnd, long nParam1, long nParam2)
{
	LPSCR_INFORMATION_WNDDATA lpData = (LPSCR_INFORMATION_WNDDATA)atWINDOW_GetDataPtr(hWnd);
	//LPSCR_1_DESC_1_WNDDATA  Data = (LPSCR_1_DESC_1_WNDDATA)nParam2;
	atHRESSRC hResSrc = atUIAPI_GetResSource(_AT("TESTMAIN"));
  
    switch(nParam1)
    {
        case 100 :
			atAPP_DestroyWindow( atAPPMNG_GetCurApp(), hWnd );
			break;

		case 1000:
			atAPP_DestroyWindow(atAPPMNG_GetCurApp(), hWnd);
			break;

		case 1001:
			atAPP_DestroyWindow(atAPPMNG_GetCurApp(), hWnd);
			break;

		case 1008:
		{
			switch (lpData->nType)
			{
				case 10012:
				{
					lpData->nType = 10011;
					atUIAPI_DestroyBitmap(lpData->hBm_title);
					lpData->hBm_title = atUIAPI_CreateBitmapFromRes(hResSrc, _AT("Scr_info_1_divid_1.img"));
					atWINDOW_RefreshClient(hWnd);
				}
				break;

				case 10013:
				{
					lpData->nType = 10012;
					atUIAPI_DestroyBitmap(lpData->hBm_title);
					lpData->hBm_title = atUIAPI_CreateBitmapFromRes(hResSrc, _AT("Scr_info_1_divid_2.img"));
					atWINDOW_RefreshClient(hWnd);
				}
				break;

				case 10022:
				{
					lpData->nType = 10021;
					atUIAPI_DestroyBitmap(lpData->hBm_title);
					lpData->hBm_title = atUIAPI_CreateBitmapFromRes(hResSrc, _AT("Scr_info_1_recover_1.img"));
					atWINDOW_RefreshClient(hWnd);
				}
				break;

				case 10032:
				{
					lpData->nType = 10031;
					atUIAPI_DestroyBitmap(lpData->hBm_title);
					lpData->hBm_title = atUIAPI_CreateBitmapFromRes(hResSrc, _AT("Scr_info_1_pen_divid_1.img"));
					atWINDOW_RefreshClient(hWnd);
				}
				break;

				case 10042:
				{
					lpData->nType = 10041;
					atUIAPI_DestroyBitmap(lpData->hBm_title);
					lpData->hBm_title = atUIAPI_CreateBitmapFromRes(hResSrc, _AT("Scr_info_1_pen_recover_1.img"));
					atWINDOW_RefreshClient(hWnd);
				}
				break;

				case 20012:
				{
					lpData->nType = 20011;
					atUIAPI_DestroyBitmap(lpData->hBm_title);
					lpData->hBm_title = atUIAPI_CreateBitmapFromRes(hResSrc, _AT("Scr_info_2_eyes_1.img"));
					atWINDOW_RefreshClient(hWnd);
				}
				break;

				case 20032:
				{
					lpData->nType = 20031;
					atUIAPI_DestroyBitmap(lpData->hBm_title);
					lpData->hBm_title = atUIAPI_CreateBitmapFromRes(hResSrc, _AT("Scr_info_2_read_1.img"));
					atWINDOW_RefreshClient(hWnd);
				}
				break;

				case 30012:
				{
					lpData->nType = 30011;
					atUIAPI_DestroyBitmap(lpData->hBm_title);
					lpData->hBm_title = atUIAPI_CreateBitmapFromRes(hResSrc, _AT("Scr_info_3_special_1_1.img"));
					atWINDOW_RefreshClient(hWnd);
				}
				break;

				case 30042:
				{
					lpData->nType = 30041;
					atUIAPI_DestroyBitmap(lpData->hBm_title);
					lpData->hBm_title = atUIAPI_CreateBitmapFromRes(hResSrc, _AT("Scr_info_3_special_4_1.img"));
					atWINDOW_RefreshClient(hWnd);
				}
				break;
			}
		}
		break;

		case 1009:
		{
			switch (lpData->nType)
			{
				case 10011:
				{
					lpData->nType = 10012;
					atUIAPI_DestroyBitmap(lpData->hBm_title);
					lpData->hBm_title = atUIAPI_CreateBitmapFromRes(hResSrc, _AT("Scr_info_1_divid_2.img"));
					atWINDOW_RefreshClient(hWnd);
				}
					break;

				case 10012:
				{
					lpData->nType = 10013;
					atUIAPI_DestroyBitmap(lpData->hBm_title);
					lpData->hBm_title = atUIAPI_CreateBitmapFromRes(hResSrc, _AT("Scr_info_1_divid_3.img"));
					atWINDOW_RefreshClient(hWnd);
				}
					break;

				case 10021:
				{
					lpData->nType = 10022;
					atUIAPI_DestroyBitmap(lpData->hBm_title);
					lpData->hBm_title = atUIAPI_CreateBitmapFromRes(hResSrc, _AT("Scr_info_1_recover_2.img"));
					atWINDOW_RefreshClient(hWnd);
				}
					break;

				case 10031:
				{
					lpData->nType = 10032;
					atUIAPI_DestroyBitmap(lpData->hBm_title);
					lpData->hBm_title = atUIAPI_CreateBitmapFromRes(hResSrc, _AT("Scr_info_1_pen_divid_2.img"));
					atWINDOW_RefreshClient(hWnd);
				}
					break;

				case 10041:
				{
					lpData->nType = 10042;
					atUIAPI_DestroyBitmap(lpData->hBm_title);
					lpData->hBm_title = atUIAPI_CreateBitmapFromRes(hResSrc, _AT("Scr_info_1_pen_recover_2.img"));
					atWINDOW_RefreshClient(hWnd);
				}
					break;
					
				case 20011:
				{
					lpData->nType = 20012;
					atUIAPI_DestroyBitmap(lpData->hBm_title);
					lpData->hBm_title = atUIAPI_CreateBitmapFromRes(hResSrc, _AT("Scr_info_2_eyes_2.img"));
					atWINDOW_RefreshClient(hWnd);
				}
					break;

				case 20031:
				{
					lpData->nType = 20032;
					atUIAPI_DestroyBitmap(lpData->hBm_title);
					lpData->hBm_title = atUIAPI_CreateBitmapFromRes(hResSrc, _AT("Scr_info_2_read_2.img"));
					atWINDOW_RefreshClient(hWnd);
				}
					break;

				case 30011:
				{
					lpData->nType = 30012;
					atUIAPI_DestroyBitmap(lpData->hBm_title);
					lpData->hBm_title = atUIAPI_CreateBitmapFromRes(hResSrc, _AT("Scr_info_3_special_1_2.img"));
					atWINDOW_RefreshClient(hWnd);
				}
					break;

				case 30041:
				{
					lpData->nType = 30042;
					atUIAPI_DestroyBitmap(lpData->hBm_title);
					lpData->hBm_title = atUIAPI_CreateBitmapFromRes(hResSrc, _AT("Scr_info_3_special_4_2.img"));
					atWINDOW_RefreshClient(hWnd);
				}
				break;
			}
		}
		break;

#ifdef GDI_TEST
		case 1009:
			lpData->test_value++;
			atWINDOW_RefreshClient(hWnd);
			break;
#endif

	}
}


static void Scr_info_WndOnChange(atHWINDOW hWnd, long nParam1, long nParam2)
{
	LPSCR_INFORMATION_WNDDATA lpData = (LPSCR_INFORMATION_WNDDATA)atWINDOW_GetDataPtr(hWnd);

    switch(nParam1)
    {
        case 103 :

			break;
	}
}


static void Scr_info_WndOnInit(atHWINDOW hWnd, long nParam)
{
	LPSCR_INFORMATION_WNDDATA lpData = (LPSCR_INFORMATION_WNDDATA)atWINDOW_GetDataPtr(hWnd);

	atHCONTROL ctl;
	atHCONTROL hWidget;

	//atHRESSRC hResSrc = atUIAPI_AddResSource(_AT("TESTMAIN"), atUI_RES_SRC_RESSTREAM, (const atLPVOID)pUser_ResData, sizeof(pUser_ResData));
	atHRESSRC hResSrc = atUIAPI_GetResSource(_AT("TESTMAIN"));
	lpData->hBm_bg = atUIAPI_CreateBitmapFromRes(hResSrc, _AT("Scr_3_bg.img"));

	switch (lpData->nType) {
	case 10011: 
		lpData->hBm_title = atUIAPI_CreateBitmapFromRes(hResSrc, _AT("Scr_info_1_divid_1.img"));
		break;

	case 10031:
		lpData->hBm_title = atUIAPI_CreateBitmapFromRes(hResSrc, _AT("Scr_info_1_pen_divid_1.img"));
		break;

	case 10021:
		lpData->hBm_title = atUIAPI_CreateBitmapFromRes(hResSrc, _AT("Scr_info_1_recover_1.img"));
		break;

	case 10041:
		lpData->hBm_title = atUIAPI_CreateBitmapFromRes(hResSrc, _AT("Scr_info_1_pen_recover_1.img"));
		break;
		
	case 20011:
		lpData->hBm_title = atUIAPI_CreateBitmapFromRes(hResSrc, _AT("Scr_info_2_eyes_1.img"));
		break;

	case 20031:
		lpData->hBm_title = atUIAPI_CreateBitmapFromRes(hResSrc, _AT("Scr_info_2_read_1.img"));
		break;

	case 30011:
		lpData->hBm_title = atUIAPI_CreateBitmapFromRes(hResSrc, _AT("Scr_info_3_special_1_1.img"));
		break;

	case 30020:
		lpData->hBm_title = atUIAPI_CreateBitmapFromRes(hResSrc, _AT("Scr_info_3_special_2.img"));
		break;

	case 30030:
		lpData->hBm_title = atUIAPI_CreateBitmapFromRes(hResSrc, _AT("Scr_info_3_special_3.img"));
		break;

	case 30041:
		lpData->hBm_title = atUIAPI_CreateBitmapFromRes(hResSrc, _AT("Scr_info_3_special_4_1.img"));
		break;

	case 30050:
		lpData->hBm_title = atUIAPI_CreateBitmapFromRes(hResSrc, _AT("Scr_info_3_special_5.img"));
		break;

	case 30060:
		lpData->hBm_title = atUIAPI_CreateBitmapFromRes(hResSrc, _AT("Scr_info_3_special_7.img"));
		break;

	case 30070:
		lpData->hBm_title = atUIAPI_CreateBitmapFromRes(hResSrc, _AT("Scr_info_3_special_7.img"));
		break;

	case 30110:
		lpData->hBm_title = atUIAPI_CreateBitmapFromRes(hResSrc, _AT("Scr_info_3_vision_1.img"));
		break;

	case 30120:
		lpData->hBm_title = atUIAPI_CreateBitmapFromRes(hResSrc, _AT("Scr_info_3_vision_2.img"));
		break;

	case 30130:
		lpData->hBm_title = atUIAPI_CreateBitmapFromRes(hResSrc, _AT("Scr_info_3_vision_3.img"));
		break;

	case 30140:
		lpData->hBm_title = atUIAPI_CreateBitmapFromRes(hResSrc, _AT("Scr_info_3_vision_4.img"));
		break;

	case 30150:
		lpData->hBm_title = atUIAPI_CreateBitmapFromRes(hResSrc, _AT("Scr_info_3_vision_5.img"));
		break;
	}

	lpData->hBm_btn_back_n = atUIAPI_CreateBitmapFromRes(hResSrc, _AT("Scr_def_btn_ext_n.img"));
	lpData->hBm_btn_left_n = atUIAPI_CreateBitmapFromRes(hResSrc, _AT("Scr_def_btn_left.img"));
	lpData->hBm_btn_right_n = atUIAPI_CreateBitmapFromRes(hResSrc, _AT("Scr_def_btn_right.img"));

	lpData->hBm_btn_back_p = atUIAPI_CreateBitmapFromRes(hResSrc, _AT("Scr_def_btn_ext_p.img"));


	/*hWidget = BasePkg_CreateImageBox(atNULL, 3000, 0, 0, 320, 480, lpData->hBm_bg, 1);

	//case
	switch (lpData->nType) {
	case 101:
		hWidget = BasePkg_CreateImageBox(atNULL, 4000, 0, 0, 0, 0, lpData->hBm_title, 1);
		break;
	}*/

	hWidget = ExtPkg_CreateImageButton(hWnd, 1001, 10, 50, 27, 27, lpData->hBm_btn_back_n, lpData->hBm_btn_back_p, atNULL);
	atCONTROL_SetFocusable(hWidget, atFALSE);
	hWidget = ExtPkg_CreateImageButton(hWnd, 1008, 80, 420, 59, 29, lpData->hBm_btn_left_n, atNULL, atNULL);
	atCONTROL_SetFocusable(hWidget, atFALSE);
	hWidget = ExtPkg_CreateImageButton(hWnd, 1009, 180, 420, 59, 29, lpData->hBm_btn_right_n, atNULL, atNULL);
	atCONTROL_SetFocusable(hWidget, atFALSE);
}

static void Scr_info_WndOnRelease(atHWINDOW hWnd)
{
	LPSCR_INFORMATION_WNDDATA	lpData = (LPSCR_INFORMATION_WNDDATA)atWINDOW_GetDataPtr(hWnd);

	atUIAPI_DestroyBitmap(lpData->hBm_bg);
	atUIAPI_DestroyBitmap(lpData->hBm_title);

	atUIAPI_DestroyBitmap(lpData->hBm_btn_back_n);
	atUIAPI_DestroyBitmap(lpData->hBm_btn_left_n);
	atUIAPI_DestroyBitmap(lpData->hBm_btn_right_n);

	atUIAPI_DestroyBitmap(lpData->hBm_btn_back_p);
}


static int Scr_info_WndOnKeyDown(atHWINDOW hWnd, int nKeyCode)
{
	LPSCR_INFORMATION_WNDDATA lpData = (LPSCR_INFORMATION_WNDDATA)atWINDOW_GetDataPtr(hWnd);
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

static int Scr_info_WndOnKeyPress(atHWINDOW hWnd, int nKeyCode)
{
	LPSCR_INFORMATION_WNDDATA lpData = (LPSCR_INFORMATION_WNDDATA)atWINDOW_GetDataPtr(hWnd);

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


static void Scr_info_WndOnPaint(atHWINDOW hWnd)
{
	LPSCR_INFORMATION_WNDDATA lpData = (LPSCR_INFORMATION_WNDDATA)atWINDOW_GetDataPtr(hWnd);
	atHGDC gdc = atWINDOW_GetClientGDC(hWnd);
	atREGION rgn;
	atHFONT oldfont;
	atTCHAR str[64];

	atUIAPI_DrawImage(gdc, 0, 0, lpData->hBm_bg);
	atUIAPI_DrawImage(gdc, 33, 90, lpData->hBm_title);

#ifdef GDI_TEST
	/*
	atWINDOW_GetClientRegion(hWnd, &rgn);

	oldfont = atUIAPI_SetFont(gdc, lpData->hFnt_main);
	atUIAPI_DrawStringAlign(gdc, rgn.nX + rgn.nWidth / 2, rgn.nY + rgn.nHeight / 2 + 100, _AT("Test (테스트) #123"), ALIGN_CENTER_MIDDLE, atUI_COLOR_BLACK);
	atUIAPI_SetFont(gdc, oldfont);


	oldfont = atUIAPI_SetFont(gdc, lpData->hFnt_sub);
	atUIAPI_sprintf(str, _AT("Test value = %d"), lpData->test_value);
	atUIAPI_DrawStringAlign(gdc, rgn.nX + rgn.nWidth / 2, rgn.nY + rgn.nHeight / 2 + 150, str, ALIGN_CENTER_MIDDLE, atUI_COLOR_RED);
	atUIAPI_SetFont(gdc, oldfont);
	*/
#endif
}


int Scr_inforamtion_WndOnEvent(atHWINDOW hWnd, int nMsg, long nParam1, long nParam2)
{
	int ret = 0;
	LPSCR_INFORMATION_WNDDATA	lpData = (LPSCR_INFORMATION_WNDDATA)atWINDOW_GetDataPtr(hWnd);
	LPSCR_INFORMATION_WNDDATA  Data = (LPSCR_INFORMATION_WNDDATA)nParam1;
	

	switch( nMsg )
	{
		case WNDMSG_INIT:
		{
			lpData->nType = Data->nType;
			Scr_info_WndOnInit(hWnd, nParam1);
		}
			break;
		case WNDMSG_RELEASE:
			Scr_info_WndOnRelease( hWnd );
			break;
		case WNDMSG_PAINT:
			Scr_info_WndOnPaint( hWnd );
			break;
		case WNDMSG_KEYDOWN:
			return Scr_info_WndOnKeyDown( hWnd, nParam1 );
			break;
		case WNDMSG_KEYPRESS:
			return Scr_info_WndOnKeyPress( hWnd, nParam1 );
			break;
		case WNDMSG_CHANGE:
			Scr_info_WndOnChange( hWnd, nParam1, nParam2 );
			break;
		case WNDMSG_COMMAND:
			Scr_info_WndOnCommand( hWnd, nParam1, nParam2 );
			break;
		default:
			break;
	}

	return ret;
}
