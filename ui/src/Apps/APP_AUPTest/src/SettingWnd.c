//
// SettingWnd.c 
//
//
//   
//
///////////////////////////////////////////////////////////////////////
 
#include "A_UIENG.h"
#include "UI_Util.h"
#include "Global.h"
//#include "MyControl.h"
#include "AEECOMP_BasePkg.h"
#include "AEECOMP_ExtPkg.h"
#include "SettingWnd.h"
#include "EqSetWnd.h"
#include <string.h>


static atLPTSTR setting[][3] = {
//	{ _AT("Setting"), 		_AT("환경설정"), 			_AT("設定")			},
	{ _AT("Themexxxxxxxxxxxxxxxxxxyyyyyyyyyyyyzzzzzzzzzzz"),			_AT(""), 			_AT("")		},
	{ _AT("Bright"), 		_AT(""), 				_AT("")		},
	{ _AT("Contrast"), 		_AT(""), 				_AT("")	},
	{ _AT("Caption"), 		_AT(""), 				_AT("")			},
	{ _AT("Multi audio"),	_AT(""), 			_AT("")		},
	{ _AT("Auto-off"), 		_AT(""), 		_AT("")	},
	{ _AT("Infomation"), 	_AT(""), 				_AT("")			},
	{ _AT("Equalizer"), 	_AT(""), 			_AT("Equalizer")	},
	{ _AT("Close"), 		_AT(""), 		_AT("")	},
	{ _AT("Test #1"), 		_AT(""), 			_AT("Test #1")		},
	{ _AT("Test #2"), 		_AT(""), 			_AT("Test #2")		},
	{ _AT("Test0 Test1 Test2 Test3 Test4 Test5 Test6"), 		_AT(""), 			_AT("Test #3")		}
};

static atTCHAR setting_theme[2][32] = {
	_AT("Theme #1"),
	_AT("Theme #2")
};

static atTCHAR setting_caption[][32] = {
	_AT("On"),
	_AT("Off")
};

static atTCHAR setting_info[] = 
	_AT("==================\n")
	_AT(" >>Program Info<<\n")
	_AT("==================\n")
	_AT("\n")
	_AT("Title : 1Seg Portable-TV\n")
	_AT("\n")
	_AT("Version : v0.0.1\n")
	_AT("Date : 2007.8.19\n")
	_AT("Author : gmju\n")
	_AT("\n")
	_AT("Etc...\n")
	_AT("Etc.......\n")
	_AT("Etc............");


static atTCHAR xx[][32] = {
	_AT("xxxxxxxx"),
	_AT("yyyyyyyy"),
	_AT("Close")
};

static atTCHAR xxyy[100];


static void SettingWndOnCommand(atHWINDOW hWnd, long nParam1, long nParam2)
{
    LPSETTINGWNDDATA lpData = (LPSETTINGWNDDATA)atWINDOW_GetDataPtr(hWnd);
//    LPCONFUSERINFO lpConfUserInfo = lpData->lpConfUserInfo;    
	atHWINDOW wnd;

    switch(nParam1)
    {
        case 1000 : // 메인 메뉴리스트 
            switch( nParam2 ){
				case 1 :
//					CreateListBoxWnd( hWnd, 102, setting[nParam2-1][2], setting_theme, 2, g_GlobalSetting.nTheme );
//					CreatePopupMenuWnd( hWnd, 102, 0, 0, 120, 100, xx, 3, MENU_ALIGN_LEFT | MENU_ALIGN_BOTTOM );
CreateFileListWnd( hWnd, 300, _AT("xxxxxxx"), _AT("..\\"), _AT("*.*"), 100, atTRUE, xxyy );
//CreateMsgBox( hWnd, 109, setting[nParam2-1][2], atUIAPI_itoa( 0x12345ABC, xxyy, 16 ), atMBT_CONFIRM );
					break;
				case 2 :
					CreateHSliderWnd( hWnd, 103, setting[nParam2-1][2], atNULL, 10, g_GlobalSetting.nBright );
//					CreatePopupMenuWnd( hWnd, 102, 0, 0, 120, 100, xx, 3, MENU_ALIGN_RIGHT | MENU_ALIGN_BOTTOM );
					break;
				case 3 :
					CreateHSliderWnd( hWnd, 104, setting[nParam2-1][2], atNULL, 10, g_GlobalSetting.nContrast );
					break;
				case 4 :
					CreateRadioGroupWnd( hWnd, 105, setting[nParam2-1][2], setting_caption, 2, (g_GlobalSetting.bCaption)? 0 : 1 );
					break;
				case 5 :
					CreateRadioGroupWnd( hWnd, 106, setting[nParam2-1][2], setting_caption, 2, 0 );
					break;
				case 6 :
					CreateRadioGroupWnd( hWnd, 107, setting[nParam2-1][2], setting_caption, 2, 0 );
					break;
				case 7 :
					CreateTextViewWnd( hWnd, 108, setting[nParam2-1][2], setting_info );
					break;
				case 8 :
					wnd = atAPP_CreateWindow( atAPPMNG_GetCurApp(), atNULL, 0, atWS_FRAMEWINDOW | atWS_MODAL, _AT("Test Window #2"), atANCHOR_IGNORE, atANCHOR_IGNORE, 200, 220, sizeof(EQSETWNDDATA), EqSetWndOnEvent, atNULL, atNULL );
					atAPP_ShowWindow( atAPPMNG_GetCurApp(), wnd, atTRUE );
					break;
				case 9 :
					atAPP_DestroyWindow( atAPPMNG_GetCurApp(), hWnd );
					break;
            }
            break;

		case 102: // Theme
			g_GlobalSetting.nTheme = nParam2;
			break;
			
		case 103: // Bright
			g_GlobalSetting.nBright = nParam2;
			break;
			
		case 104: // Contrast
			g_GlobalSetting.nContrast = nParam2;
			break;
			
		case 105: // Caption
			g_GlobalSetting.bCaption = (nParam2 == 0)? atTRUE : atFALSE;
#ifndef __AEE_EMULATOR_
			// caption....
#endif
			break;

        case 2000 : // 메인 메뉴리스트 
            switch( nParam2 ){
				case 2 :
					atAPP_DestroyWindow( atAPPMNG_GetCurApp(), hWnd );
					break;
			}
			break;
	}
}


static void SettingWndOnInit(atHWINDOW hWnd)
{
    int i;
    LPSETTINGWNDDATA	lpData = (LPSETTINGWNDDATA)atWINDOW_GetDataPtr(hWnd);

/*
	lpData->hListBox = CreateListBox( hWnd, 101, 5, 5, -5, -5, 20, atTRUE, atFALSE );
	for( i = 0; i < 9+3; i++ ){
		atLISTBOX_Start_AddItem(lpData->hListBox);
		atLISTBOX_AddItem_NO(lpData->hListBox, i+1);
		atLISTBOX_AddItem_String(lpData->hListBox, setting[i][0], 0);
		atLISTBOX_Update_AddItem(lpData->hListBox);
	}
*/

	lpData->hListBox = ExtPkg_CreateListBox( hWnd, 1000, 5, 5, 50, 50, 20, atTRUE, atFALSE, atTRUE );
	atCONTROL_SetAnchor( lpData->hListBox, 5, 5, 5, 5 );
	for( i = 0; i < 9+3; i++ ){
		atLISTBOX_EXT_Start_AddItem(lpData->hListBox);
		atLISTBOX_EXT_AddItem_ItemID(lpData->hListBox, i+1);
		atLISTBOX_EXT_AddItem_NO(lpData->hListBox, i+1);
		atLISTBOX_EXT_AddItem_String(lpData->hListBox, setting[i][0], 0);
		atLISTBOX_EXT_Update_AddItem(lpData->hListBox);
	}


//	CreateEditBox( hWnd, 102, 5, 100, 150, 50, _AT("Label"), atNULL, atNULL );
	atWINDOW_SetSoftKey( hWnd, _AT("Menu"), _AT(""), _AT("") );
}



static void SettingWndOnRelease(atHWINDOW hWnd)
{
    LPSETTINGWNDDATA	lpData = (LPSETTINGWNDDATA)atWINDOW_GetDataPtr(hWnd);

//    atUIAPI_DestroyBitmap( lpData->hBm[0] );
//    atUIAPI_DestroyBitmap( lpData->hBm[1] );
}



static int SettingWndOnKeyDown(atHWINDOW hWnd, int nKeyCode)
{
    LPSETTINGWNDDATA lpData = (LPSETTINGWNDDATA)atWINDOW_GetDataPtr(hWnd);
//	atHWINDOW hPopup;

    switch( nKeyCode )
    {
        case atVKEY_CLEAR:
//        case atVKEY_MENU:
            atAPP_DestroyWindow( atAPPMNG_GetCurApp(), hWnd );
            break;

/*            
        case atVKEY_FF:
        case atVKEY_REW:
			lpData->idx ++;
			if( lpData->idx >= 2 ) lpData->idx = 0;
			atWINDOW_Draw( hWnd, atTRUE );
            break;
*/
        case atVKEY_SOFT1:
			ExtPkg_CreatePopupMenuWnd( hWnd, 2000, 0, 0, 120, 100, xx, 3, 3, MENU_ALIGN_LEFT | MENU_ALIGN_BOTTOM );
            break;
			
        default:
            return 0;
    }

    return 1; // WindowOnEvent 가 호출되지 않도록 0을 리턴한다.
}



static void SettingWndOnPaint(atHWINDOW hWnd)
{
//    LPSETTINGWNDDATA	lpData = (LPSETTINGWNDDATA)atWINDOW_GetDataPtr(hWnd);
    
//	atHGDC gdc = atWINDOW_GetClientGDC(hWnd);

//    atUIAPI_DrawImage( , 0, 0, lpData->hBm[lpData->idx] );

//	atUIAPI_Clear(atWINDOW_GetClientGDC(hWnd), UI_COLOR_WHITE);
//	atUIAPI_DrawString( atWINDOW_GetClientGDC(hWnd), 10, 10, _AT("設"), UI_COLOR_BLACK);
//	atUIAPI_DrawString( atWINDOW_GetClientGDC(hWnd), 10, 10, _AT("定"), UI_COLOR_BLACK);
//	atUIAPI_DrawLineWidth( gdc, UI_COLOR_WHITE);
	
}



int SettingWndOnEvent(atHWINDOW hWnd, int nMsg, long nParam1, long nParam2)
{
	switch( nMsg )
	{
		case WNDMSG_INIT:
			SettingWndOnInit( hWnd );
			break;
		case WNDMSG_RELEASE:
			SettingWndOnRelease( hWnd );
			break;
		case WNDMSG_PAINT:
			SettingWndOnPaint( hWnd );
			break;
		case WNDMSG_KEYDOWN:
			return SettingWndOnKeyDown( hWnd, nParam1 );
			break;
		case WNDMSG_COMMAND:
			SettingWndOnCommand( hWnd, nParam1, nParam2 );
			break;
		default:
			return 0;
			break;
	}

	return 1;
}
