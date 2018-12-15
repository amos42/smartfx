//
// MainWnd.c 
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
#include "MainWnd.h"
#include "SettingWnd.h"
#include "PaintBoxTestWnd.h"
#include "TestWnd.h"
#include "SoundTestWnd.h"
#include <string.h>


static atLPTSTR setting[][3] = {
	{ _AT("Themexxxxxxxxxxxxxxxxxxyyyyyyyyyyyyzzzzzzzzzzz"),			_AT(""), 			_AT("")		},
	{ _AT("Infomation"), 	_AT(""), 				_AT("")			},
	{ _AT("PaintBox Test"), 	_AT(""), 			_AT("Equalizer")	},
	{ _AT("TextBox"), 	_AT(""), 			_AT("Equalizer")	},
	{ _AT("Setting"), 		_AT(""), 			_AT("")			},
	{ _AT("Sound Test"), 		_AT(""), 			_AT("")			},
	{ _AT("Etc Test"), 		_AT(""), 			_AT("")			},
	{ _AT("Memory Info"), 		_AT(""), 			_AT("")			},
	{ _AT("Window Info"), 		_AT(""), 			_AT("")			},
	{ _AT("Close"), 		_AT(""), 		_AT("")	},
	{ _AT("Exit"), 		_AT(""), 		_AT("")	},
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
	_AT("1\n")
	_AT("2\n")
	_AT("3\n")
	_AT("4\n")
	_AT("5\n")
	_AT("...\n")
	_AT("\n")
	_AT("Etc...\n")
	_AT("Etc.......\n")
	_AT("Etc............");


static atTCHAR xx[][32] = {
	_AT("xxxxxxxx"),
	_AT("yyyyyyyy"),
	_AT("zzzzzzzz")
};

static atTCHAR xxyy[100];


static void MainWndOnCommand(atHWINDOW hWnd, long nParam1, long nParam2)
{
    LPMAINWNDDATA lpData = (LPMAINWNDDATA)atWINDOW_GetDataPtr(hWnd);
//    LPCONFUSERINFO lpConfUserInfo = lpData->lpConfUserInfo;    
	atHWINDOW wnd;
	atMARGIN anchor;
	static atTCHAR test_str[100];

    switch(nParam1)
    {
        case 101 : // 메인 메뉴리스트 
            switch( nParam2 ){
				case 1 :
//					CreateListBoxWnd( hWnd, 102, setting[nParam2-1][2], setting_theme, 2, g_GlobalSetting.nTheme );
//					CreatePopupMenuWnd( hWnd, 102, 0, 0, 120, 100, xx, 3, MENU_ALIGN_LEFT | MENU_ALIGN_BOTTOM );
//atUIAPI_SetClipRect( 20, 50, 200, 200 );
		

CreateFileListWnd( hWnd, 300, _AT("xxxxxxx"), _AT("..\\"), _AT("*.*"), 100, atTRUE, xxyy );
//CreateMsgBox( hWnd, 109, setting[nParam2-1][2], atUIAPI_itoa( 0x12345ABC, xxyy, 16 ), atMBT_CONFIRM );
					break;
				case 2 :
					CreateTextViewWnd( hWnd, 108, setting[nParam2-1][2], setting_info );
					break;
				case 3 :
					wnd  = atAPP_CreateWindow( atAPPMNG_GetCurApp(), atNULL, 0, atWS_FRAMEWINDOW | atWS_MODAL, _AT("PaintBoxTest Window"), 20, 30, 200, 220, sizeof(PAINTBOXTESTWNDDATA), PaintBoxTestWndOnEvent, atNULL, atNULL );
//					wnd  = atAPP_CreateWindow( atAPPMNG_GetCurApp(), atNULL, 0, atWS_NONE, _AT("Test Window #2"), 20, 30, 200, 220, sizeof(TESTWNDDATA), TestWndOnEvent, atNULL, atNULL );
//atWINOBJ_SetAttr( wnd, atOBJATTR_TRANSPARENT );
//atWINOBJ_ChangeAttr( wnd, atOBJATTR_TRANSPARENT, atOBJATTR_NONE );
//atWINDOW_SetAnchor( wnd, atANCHOR_IGNORE, atANCHOR_IGNORE, 20, 20 );
atWINDOW_SetAnchor( wnd, 10, 10, 20, 20 );
//atAPP_AdjustWindowArea( atAPPMNG_GetCurApp(), wnd );
					atAPP_ShowWindow( atAPPMNG_GetCurApp(), wnd, atTRUE );
					break;
				case 4 :
					atUIAPI_strcpy( test_str, _AT("Test String") );
					BasePkg_CreateEditBoxWnd( hWnd, 200, _AT("Test"), _AT("Test Msg"), test_str );
//					ExtPkg_CreateEditBoxWnd( hWnd, 200, _AT("Test"), _AT("Test Msg"), test_str, atTRUE );
					break;
				case 5 :
					wnd = atAPP_CreateWindow( atAPPMNG_GetCurApp(), atNULL, 0, atWS_FRAMEWINDOW | atWS_MODAL | atWS_UNFOCUSGRAY, _AT("Test Window #2"), 20, 30, 200, 220, sizeof(SETTINGWNDDATA), SettingWndOnEvent, atNULL, atNULL );
atWINDOW_SetAnchor( wnd, atANCHOR_IGNORE, atANCHOR_IGNORE, atANCHOR_IGNORE, atANCHOR_IGNORE );
//atAPP_AdjustWindowArea( atAPPMNG_GetCurApp(), wnd );
					atAPP_ShowWindow( atAPPMNG_GetCurApp(), wnd, atTRUE );
					break;
				case 6 :
					wnd = atAPP_CreateWindow( atAPPMNG_GetCurApp(), atNULL, 0, atWS_FRAMEWINDOW | atWS_MODAL, _AT("Test Window #2"), 20, 30, 200, 220, sizeof(SOUNDTESTWNDDATA), SoundTestWndOnEvent, atNULL, atNULL );
atWINDOW_SetAnchor( wnd, atANCHOR_IGNORE, atANCHOR_IGNORE, atANCHOR_IGNORE, atANCHOR_IGNORE );
//atAPP_AdjustWindowArea( atAPPMNG_GetCurApp(), wnd );
					atAPP_ShowWindow( atAPPMNG_GetCurApp(), wnd, atTRUE );
					break;
				case 7 :
					//wnd  = atAPP_CreateWindow( atAPPMNG_GetCurApp(), atNULL, 0, atWS_FRAMEWINDOW, _AT("Test Window"), 20, 30, 200, 220, sizeof(TESTWNDDATA), TestWndOnEvent, atNULL, atNULL );
					wnd  = atAPP_CreateWindow( atAPPMNG_GetCurApp(), atNULL, 0, atWS_NONE | atWS_MODAL, _AT("Test Window"), 20, 30, 200, 220, sizeof(TESTWNDDATA), TestWndOnEvent, atNULL, atNULL );
					atWINDOW_SetAnchor( wnd, 10, 10, 20, 20 );
					atAPP_ShowWindow( atAPPMNG_GetCurApp(), wnd, atTRUE );
					break;
				case 8 :
					BasePkg_CreateMemoryInfoWnd( hWnd );
					break;
				case 9 :
					BasePkg_CreateWindowInfoWnd( hWnd );
					break;
				case 10:
					atAPP_DestroyWindow( atAPPMNG_GetCurApp(), hWnd );
					break;
				case 11 :
					ExtPkg_CreateMsgBoxEx( hWnd, 110, setting[nParam2-1][2], _AT("Exit?"), atMBT_QUESTION );
					break;
            }
            break;

		case 110: // Format
			if( nParam2 == atMB_YES )
				atAPPMNG_Exit(0);
			break;
		case 200: // Test2
			if( nParam2 == atMB_OK )
				ExtPkg_CreateMsgBoxEx( hWnd, 110, _AT("Test"), test_str, atMBT_CONFIRM );
			break;
	}
}


static atINT _xx( atHCONTROL hControl, atINT nIndex, atLPLISTBOX_EXT_ITEM lpItem, atLPVOID lpParam )
{
	int xyz = 10;
	xyz += nIndex * 10;
	return xyz;
}


static void MainWndOnInit(atHWINDOW hWnd)
{
    int i;
    LPMAINWNDDATA	lpData = (LPMAINWNDDATA)atWINDOW_GetDataPtr(hWnd);

/*
	lpData->hListBox = BasePkg_CreateListBox( hWnd, 101, 5, 5, -5, -5, 20, atTRUE, 1 );
	for( i = 0; i < 8; i++ ){
		atLISTBOX_Start_AddItem(lpData->hListBox);
		atLISTBOX_AddItem_NO(lpData->hListBox, i+1);
		atLISTBOX_AddItem_String(lpData->hListBox, setting[i][0], 0);
		atLISTBOX_Update_AddItem(lpData->hListBox);
	}
*/

//BasePkg_CreateEditBox( hWnd, 100, 10, 10, 100, 22, _AT("x") );

//atWINOBJ_SetClientOrigin( hWnd, 0, -90 );
	lpData->hListBox = ExtPkg_CreateListBox( hWnd, 101, 5, 5, 80, 80, 20, atTRUE, 0, atTRUE );
//	atLISTBOX_EXT_SetupFunc( lpData->hListBox, NULL, _xx, NULL, NULL, NULL, NULL );
	atCONTROL_SetAnchor( lpData->hListBox, 5, 5, 5, 5 );
	for( i = 0; i < 9; i++ ){
		atLISTBOX_EXT_Start_AddItem(lpData->hListBox);
		atLISTBOX_EXT_AddItem_ItemID(lpData->hListBox, i+1);
		atLISTBOX_EXT_AddItem_NO(lpData->hListBox, i+1);
		atLISTBOX_EXT_AddItem_String(lpData->hListBox, setting[i][0], 0);
		atLISTBOX_EXT_Update_AddItem(lpData->hListBox);
	}

//atLISTBOX_EXT_SetupKey(lpData->hListBox, atVKEY_SOFT1, atVKEY_SELECT );
atLISTBOX_EXT_SetAttr(lpData->hListBox, atLISTBOX_EXT_ATTR_MOVE_WRAP);

	atWINDOW_SetSoftKey( hWnd, _AT("Menu"), _AT(""), _AT("") );
}



static void MainWndOnRelease(atHWINDOW hWnd)
{
    LPMAINWNDDATA	lpData = (LPMAINWNDDATA)atWINDOW_GetDataPtr(hWnd);

//    atUIAPI_DestroyBitmap( lpData->hBm[0] );
//    atUIAPI_DestroyBitmap( lpData->hBm[1] );
}



static int MainWndOnKeyDown(atHWINDOW hWnd, int nKeyCode)
{
    LPMAINWNDDATA lpData = (LPMAINWNDDATA)atWINDOW_GetDataPtr(hWnd);
	atHWINDOW hPopup;

    switch( nKeyCode )
    {
		case atVKEY_SOFT1:
			hPopup = ExtPkg_CreatePopupMenuWnd( hWnd, 2000, 0, 0, 200, 100, xx, 3, 3, MENU_ALIGN_LEFT | MENU_ALIGN_BOTTOM );
			atPOPUPMENU_AddSubMenu( hPopup, 0, 100, setting_theme, 2, 0 );
//			atWINDOW_Draw(hPopup,atTRUE);			
//			atUIAPI_FlushScreen();
//			atAPP_RedrawAllWindow(atAPPMNG_GetCurApp());			
			atWINDOW_Refresh(hPopup);
			break;

//        case atVKEY_CLEAR:
//        case atVKEY_MENU:
//            atAPP_DestroyWindow( atAPPMNG_GetCurApp(), hWnd );
//            break;

/*            
        case atVKEY_FF:
        case atVKEY_REW:
			lpData->idx ++;
			if( lpData->idx >= 2 ) lpData->idx = 0;
			atWINDOW_Draw( hWnd, atTRUE );
            break;
*/
			
        default:
            return 0;
    }

    return 1; // WindowOnEvent 가 호출되지 않도록 0을 리턴한다.
}


static void MainWndOnPaint(atHWINDOW hWnd)
{
//    LPMAINWNDDATA	lpData = (LPMAINWNDDATA)atWINDOW_GetDataPtr(hWnd);
    
//	atHGDC gdc = atWINDOW_GetClientGDC(hWnd);

//    atUIAPI_DrawImage( , 0, 0, lpData->hBm[lpData->idx] );

//	atUIAPI_Clear(atWINDOW_GetClientGDC(hWnd), UI_COLOR_WHITE);
//	atUIAPI_DrawString( atWINDOW_GetClientGDC(hWnd), 10, 10, _AT("設"), UI_COLOR_BLACK);
//	atUIAPI_DrawString( atWINDOW_GetClientGDC(hWnd), 10, 10, _AT("定"), UI_COLOR_BLACK);
//	atUIAPI_DrawLineWidth( gdc, UI_COLOR_WHITE);
	
}



int MainWndOnEvent(atHWINDOW hWnd, int nMsg, long nParam1, long nParam2)
{
	switch( nMsg )
	{
		case WNDMSG_INIT:
			MainWndOnInit( hWnd );
			break;
		case WNDMSG_RELEASE:
			MainWndOnRelease( hWnd );
			break;
		case WNDMSG_PAINT:
			MainWndOnPaint( hWnd );
			break;
		case WNDMSG_KEYDOWN:
			return MainWndOnKeyDown( hWnd, nParam1 );
			break;
		case WNDMSG_COMMAND:
			MainWndOnCommand( hWnd, nParam1, nParam2 );
			break;
		default:
			return 0;
			break;
	}

	return 1;
}
