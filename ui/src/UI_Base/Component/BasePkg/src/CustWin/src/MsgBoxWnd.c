//
// MsgBox.h
//
//  Date: 2005-10-26
// Coder: YongChul, Jin
//
///////////////////////////////////////////////////////////////////////


#include "A_UIENG.h"
#include "UI_Util.h"
#include "MsgBoxWnd.h"
#include "AEECOMP_BasePkg.h"



typedef struct _MSGBOXWNDDATA {
	int		nMsgBoxType;
	int		nStrLineNum;
	int		nMaxStrLineNum;

	atTCHAR	szMsg[1024];

	long	lWaitTime;
	atHTIMER	hWaitTimer;

	atHCONTROL	hControl1, hControl2, hControl3;
} MSGBOXWNDDATA, * LPMSGBOXWNDDATA;


static int MsgBoxWndOnEvent(atHWINDOW hWnd, int nMsg, long nParam1, long nParam2);


atBOOL      RegistMsgBoxWndClass(void)
{
	atWINCLASS wc;
	
	atUIAPI_memset( &wc, 0, sizeof(atWINCLASS) );
	wc.dwAttr = atWS_POPUPWINDOW | atWS_MODAL;
 	wc.dwUserDataSize = sizeof(MSGBOXWNDDATA);
	wc.fnWndProc = MsgBoxWndOnEvent;
	
	return atAPP_RegistWinClass( atAPPMNG_GetCurApp(), BASEPKG_CUSTWIN_MSGBOX, &wc );
}

void	UnregistMsgBoxWndClass(void)
{
	atAPP_UnregistWinClass( atAPPMNG_GetCurApp(), BASEPKG_CUSTWIN_MSGBOX );
}


struct _CreateParam {
	int nLineNum;
	int nMaxLineNum;
    int nMsgBoxType;
	int lWaitTime;
	int unCenterHeight;
	atLPTSTR szMsg;
};

static atHWINDOW	CreateMsgBox_0( atHWINDOW hParentWnd, int id, atLPTSTR szWindowText, atLPTSTR szMsg, int nMsgBoxType, long lWaitTime )
{
	atHWINDOW			hWnd;
	struct _CreateParam param;
	int					nLineNum, nMaxLineNum;
	atWORD				unCenterHeight, unHeight, unWidth;
	atMARGIN anchor;

	nLineNum = myGetLineNum(szMsg);
	nMaxLineNum = nLineNum;

	switch ( nMsgBoxType ){
		case atMBT_ALERT:
			break;
		case atMBT_CONFIRM:
			break; 
		case atMBT_QUESTION:
			break;
		case atMBT_QUESTION_CANCEL:
			break;
		case atMBT_QUESTION_CHECK:
			nLineNum--;
			break;
		case atMBT_ERROR:
			break;
		case atMBT_CUSTOM_1BTN:
			nLineNum--;
			nMaxLineNum--;
			break;
		case atMBT_CUSTOM_2BTN:
			nLineNum-=2;
			nMaxLineNum-=2;
			break;
		case atMBT_CUSTOM_3BTN:
			nLineNum-=3;
			nMaxLineNum-=3;
			break;
	}

	unWidth = /*atUIAPI_GetScreenWidth()*/ 240 - 6*2;
	if ( nMaxLineNum <= 3 ){
		nMaxLineNum = 3;
	} else if ( nMaxLineNum <= 4 ) {
		nMaxLineNum = 4;
	} else {
		nMaxLineNum = 5;
	}

	unCenterHeight	= 18*nMaxLineNum + 53;
	unHeight = unCenterHeight + 34 + 4;

	param.nLineNum		= nLineNum;
	param.nMaxLineNum	= nMaxLineNum;
    param.nMsgBoxType	= nMsgBoxType;
	param.lWaitTime		= lWaitTime;
	param.unCenterHeight = unCenterHeight;
	param.szMsg			= szMsg;

	SET_MARGIN( anchor, atANCHOR_IGNORE, atANCHOR_IGNORE, atANCHOR_IGNORE, atANCHOR_IGNORE );
    hWnd = atAPP_CreateWindowByClass( atAPPMNG_GetCurApp(), hParentWnd, id, szWindowText, 
								100, unHeight,
								&anchor,
								BASEPKG_CUSTWIN_MSGBOX, (void *)&param );
	atAPP_SetAnchor( atAPPMNG_GetCurApp(), hWnd, atANCHOR_IGNORE, atANCHOR_IGNORE, 20, 20 );
	atAPP_ShowWindow( atAPPMNG_GetCurApp(), hWnd, atTRUE );

	return hWnd;
}


atHWINDOW	CreateMsgBox( atHWINDOW hParentWnd, int id, atLPTSTR szWindowText, atLPTSTR szMsg, int nMsgBoxType )
{
	return CreateMsgBox_0( hParentWnd, id, szWindowText, szMsg, nMsgBoxType, 0 );
}

atHWINDOW	CreateMsgBox2( atHWINDOW hParentWnd, int id, atLPTSTR szWindowText, atLPTSTR szMsg, int nMsgBoxType, long lWaitTime )
{
	return CreateMsgBox_0( hParentWnd, id, szWindowText, szMsg, nMsgBoxType, lWaitTime );
}


static void MsgBoxWndOnInit( atHWINDOW hWnd, struct _CreateParam *lpParam )
{
	LPMSGBOXWNDDATA lpData = (LPMSGBOXWNDDATA)atWINDOW_GetDataPtr(hWnd);
//	int					nLineNum, nMaxLineNum, nBtnYPos;
//	atWORD				unCenterHeight, unHeight, unWidth;
//	atHCONTROL		hControl;
	int nBtnYPos;
	atRECT rect;
	int ClientWidth;
	int BtnWidth;
	atTCHAR buf[256];
	atHGDC hCanvas;

	nBtnYPos = lpParam->unCenterHeight - 36;//199-24;

//	atWINDOW_SetLayoutMargin( hMsgBoxWnd, 34, 4, 4, 4 );
//	atWINDOW_SetTitleText( hWnd, _AT("") );

	lpData->nStrLineNum		= lpParam->nLineNum;
	lpData->nMaxStrLineNum	= lpParam->nMaxLineNum;
    lpData->nMsgBoxType		= lpParam->nMsgBoxType;
	lpData->lWaitTime		= lpParam->lWaitTime;

	hCanvas = atWINDOW_GetClientGDC(hWnd);
//	atUIAPI_SetFont( hCanvas, atUIAPI_GetStockFont(0) );

	UI_STRNCPY( lpData->szMsg, lpParam->szMsg, sizeof(lpData->szMsg)/sizeof(atTCHAR)-1 );

	atWINDOW_GetClientRect( hWnd, &rect );
	ClientWidth = RECT_WIDTH(rect);
	BtnWidth = 60;

	switch ( lpParam->nMsgBoxType ){
	case atMBT_ALERT:
//		atVD_SOUND_Play( IMSOUND.hSound2, atFALSE );
		lpData->hControl1 = BasePkg_CreateButton(hWnd, atMB_CANCEL, (ClientWidth-BtnWidth)>>1, nBtnYPos, BtnWidth, 18, _AT("Close"));
		atCONTROL_SetAnchor( lpData->hControl1, atANCHOR_IGNORE, 20, atANCHOR_IGNORE, atANCHOR_IGNORE );
		break;
	case atMBT_CONFIRM:
//		atVD_SOUND_Play( IMSOUND.hSound4, atFALSE );
		lpData->hControl1 = BasePkg_CreateButton(hWnd, atMB_OK, (ClientWidth-BtnWidth)>>1, nBtnYPos, BtnWidth, 18, _AT("OK"));
		atCONTROL_SetAnchor( lpData->hControl1, atANCHOR_IGNORE, 20, atANCHOR_IGNORE, atANCHOR_IGNORE );
		break;
	case atMBT_QUESTION:
//		atVD_SOUND_Play( IMSOUND.hSound3, atFALSE );
		lpData->hControl1 = BasePkg_CreateButton(hWnd, atMB_YES,  (ClientWidth-(BtnWidth*2 + 20))>>1, nBtnYPos, BtnWidth, 18, _AT("Yes"));
		atCONTROL_SetAnchor( lpData->hControl1, atANCHOR_IGNORE, 20, 20, atANCHOR_IGNORE );
		lpData->hControl2 = BasePkg_CreateButton(hWnd, atMB_NO,  ((ClientWidth-(BtnWidth*2 + 20))>>1) + BtnWidth+20, nBtnYPos, BtnWidth, 18, _AT("No"));
		atCONTROL_SetAnchor( lpData->hControl2, atANCHOR_IGNORE, 20, atANCHOR_IGNORE, 20 );
		break;
	case atMBT_QUESTION_CANCEL:
//		atVD_SOUND_Play( IMSOUND.hSound3, atFALSE );
		lpData->hControl1 = BasePkg_CreateButton(hWnd, atMB_YES,     (ClientWidth-(BtnWidth*3 + 10*2))>>1, nBtnYPos, BtnWidth, 18, _AT("Yes"));
		atCONTROL_SetAnchor( lpData->hControl1, atANCHOR_IGNORE, 20, 20, atANCHOR_IGNORE );
		lpData->hControl2 = BasePkg_CreateButton(hWnd, atMB_NO,     ((ClientWidth-(BtnWidth*3 + 10*2))>>1) + BtnWidth+10, nBtnYPos, BtnWidth, 18, _AT("No"));
		atCONTROL_SetAnchor( lpData->hControl2, atANCHOR_IGNORE, 20, atANCHOR_IGNORE, atANCHOR_IGNORE );
		lpData->hControl3 = BasePkg_CreateButton(hWnd, atMB_CANCEL, ((ClientWidth-(BtnWidth*3 + 10*2))>>1) + BtnWidth*2+10*2, nBtnYPos, BtnWidth, 18, _AT("Cancel"));
		atCONTROL_SetAnchor( lpData->hControl3, atANCHOR_IGNORE, 20, atANCHOR_IGNORE, 20 );
		atWINDOW_SetCurControl( hWnd, lpData->hControl3 );
		break;
	case atMBT_QUESTION_CHECK:
//		atVD_SOUND_Play( IMSOUND.hSound3, atFALSE );
		myGetStrLine(buf, lpData->szMsg, lpData->nStrLineNum + 1, _AT('\n')); // 가장 마지막 줄을 CheckBox 메시지로 사용.
//		lpData->nStrLineNum --; // 마지막 한줄을 없앤다.
		lpData->hControl1 = BasePkg_CreateCheckBox(hWnd, atMB_YES_CHECK, (ClientWidth-atUIAPI_GetStringWidth(atUIAPI_GetFont(hCanvas),buf,-1))>>1, nBtnYPos-28, 0, 18, buf);
		atCONTROL_SetAnchor( lpData->hControl1, atANCHOR_IGNORE, 20, atANCHOR_IGNORE, atANCHOR_IGNORE );
		lpData->hControl2 = BasePkg_CreateButton(hWnd, atMB_YES, (ClientWidth-(BtnWidth*2 + 20))>>1, nBtnYPos, BtnWidth, 18, _AT("Yes"));
		atCONTROL_SetAnchor( lpData->hControl2, atANCHOR_IGNORE, 20, 20, atANCHOR_IGNORE );
		lpData->hControl3 = BasePkg_CreateButton(hWnd, atMB_NO, ((ClientWidth-(BtnWidth*2 + 20))>>1) + BtnWidth+20, nBtnYPos, BtnWidth, 18, _AT("No"));
		atCONTROL_SetAnchor( lpData->hControl3, atANCHOR_IGNORE, 20, atANCHOR_IGNORE, 20 );
		atWINDOW_SetCurControl( hWnd, lpData->hControl2 );
		break;
	case atMBT_ERROR:
//		atVD_SOUND_Play( IMSOUND.hSound2, atFALSE );
		lpData->hControl1 = BasePkg_CreateButton(hWnd, atMB_OK, (ClientWidth-BtnWidth)>>1, nBtnYPos, 60, 18, _AT("Close"));
		atCONTROL_SetAnchor( lpData->hControl1, atANCHOR_IGNORE, 20, atANCHOR_IGNORE, atANCHOR_IGNORE );
		break;
	case atMBT_CUSTOM_1BTN:
		myGetStrLine(buf, lpData->szMsg, lpData->nStrLineNum + 1, _AT('\n')); // 가장 마지막 줄을 Button 메시지로 사용.
		lpData->hControl1 = BasePkg_CreateButton(hWnd, atMB_CUSTOM_1, (ClientWidth-BtnWidth)>>1, nBtnYPos, BtnWidth, 18, buf);
		atCONTROL_SetAnchor( lpData->hControl1, atANCHOR_IGNORE, 20, atANCHOR_IGNORE, atANCHOR_IGNORE );
		break;
	case atMBT_CUSTOM_2BTN:
		myGetStrLine(buf, lpData->szMsg, lpData->nStrLineNum + 1, _AT('\n')); // 가장 마지막 줄을 Button 메시지로 사용.
		lpData->hControl1 = BasePkg_CreateButton(hWnd, atMB_CUSTOM_1, 62, nBtnYPos, BtnWidth, 18, buf);
		atCONTROL_SetAnchor( lpData->hControl1, atANCHOR_IGNORE, 20, 20, atANCHOR_IGNORE );
		myGetStrLine(buf, lpData->szMsg, lpData->nStrLineNum + 2, _AT('\n')); // 가장 마지막 줄을 Button 메시지로 사용.
		lpData->hControl2 = BasePkg_CreateButton(hWnd, atMB_CUSTOM_2, 130, nBtnYPos, BtnWidth, 18, buf);
		atCONTROL_SetAnchor( lpData->hControl2, atANCHOR_IGNORE, 20, atANCHOR_IGNORE, 20 );
		break;
	case atMBT_CUSTOM_3BTN:
		myGetStrLine(buf, lpData->szMsg, lpData->nStrLineNum + 1, _AT('\n')); // 가장 마지막 줄을 Button 메시지로 사용.
		lpData->hControl1 = BasePkg_CreateButton(hWnd, atMB_CUSTOM_1, 32, nBtnYPos, 0, 18, buf);
		atCONTROL_SetAnchor( lpData->hControl1, atANCHOR_IGNORE, 20, 20, atANCHOR_IGNORE );
		myGetStrLine(buf, lpData->szMsg, lpData->nStrLineNum + 2, _AT('\n')); // 가장 마지막 줄을 Button 메시지로 사용.
		lpData->hControl2 = BasePkg_CreateButton(hWnd, atMB_CUSTOM_2, 90, nBtnYPos, 0, 18, buf);
		atCONTROL_SetAnchor( lpData->hControl2, atANCHOR_IGNORE, 20, atANCHOR_IGNORE, atANCHOR_IGNORE );
		myGetStrLine(buf, lpData->szMsg, lpData->nStrLineNum + 3, _AT('\n')); // 가장 마지막 줄을 Button 메시지로 사용.
		lpData->hControl3 = BasePkg_CreateButton(hWnd, atMB_CUSTOM_3, 148, nBtnYPos, 0, 18, buf);
		atCONTROL_SetAnchor( lpData->hControl3, atANCHOR_IGNORE, 20, atANCHOR_IGNORE, 20 );
		atWINDOW_SetCurControl( hWnd, lpData->hControl3 );
		break;
	}
}

static void MsgBoxWndOnRegister(atHWINDOW hWnd)
{
	LPMSGBOXWNDDATA lpData = (LPMSGBOXWNDDATA)atWINDOW_GetDataPtr(hWnd);

	if( lpData->lWaitTime > 0 ){
		lpData->hWaitTimer = atWINDOW_AddTimer( hWnd, 1000, lpData->lWaitTime, atFALSE, atTRUE, atFALSE );
	}
}

static void MsgBoxWndOnRelease(atHWINDOW hWnd)
{
//	LPMSGBOXWNDDATA lpData = (LPMSGBOXWNDDATA)atWINDOW_GetDataPtr(hWnd);

}

static int MsgBoxWndOnKeyDown(atHWINDOW hWnd, int nKeyCode)
{
	switch( nKeyCode )
	{
		case atVKEY_CLEAR:
			atAPP_DestroyWindow( atAPPMNG_GetCurApp(), hWnd );
			break;

		default:
			return 0;
	}

	return 1;
}

static void MsgBoxWndOnPaint(atHWINDOW hWnd)
{
	atLPWINDOW lpWnd = atWINDOW_GetPtr(hWnd);
	LPMSGBOXWNDDATA lpData = (LPMSGBOXWNDDATA)atWINDOW_GetDataPtr(hWnd);
	atHGDC hCanvas;
	atRECT rect;
	int				nPosY, ii, nFontHeight, nStrWidth;
	atTCHAR			szLineBuff[2048];
	
    hCanvas = atWINDOW_GetClientGDC(hWnd);
	atWINDOW_GetClientRect( hWnd, &rect );
	
//	nFontAscent = MC_grpGetFontAscent(lpData->font);
	nFontHeight = atUIAPI_GetFontHeight( atUIAPI_GetFont(hCanvas) );

	if ( lpData->nStrLineNum <= 1 ){
		nPosY = 8 + lpData->nMaxStrLineNum*nFontHeight / 2 - lpData->nStrLineNum*nFontHeight/2;
	} else if ( lpData->nStrLineNum <= 2 ){
		nPosY =  8 + lpData->nMaxStrLineNum*nFontHeight / 2 - lpData->nStrLineNum*nFontHeight/2;
	} else if ( lpData->nStrLineNum <= 3 ) {
		nPosY =  8 + lpData->nMaxStrLineNum*nFontHeight / 2 - lpData->nStrLineNum*nFontHeight/2;
	} else if ( lpData->nStrLineNum <= 4 ) {
		nPosY =  8 + lpData->nMaxStrLineNum*nFontHeight / 2 - lpData->nStrLineNum*nFontHeight/2;
	} else {
		nPosY =  8 + lpData->nMaxStrLineNum*nFontHeight / 2 - lpData->nStrLineNum*nFontHeight/2;
	}

    for ( ii = 0; ii < lpData->nStrLineNum; ii++ ){
		atUIAPI_memset(szLineBuff, 0, sizeof(szLineBuff));
		if ( myGetStrLine(szLineBuff, lpData->szMsg, ii + 1, _ATC('\n')) > 0 ){
			nStrWidth = atUIAPI_GetStringWidth(atUIAPI_GetFont(hCanvas), szLineBuff, -1);
			atUIAPI_DrawString( hCanvas, 
				(RECT_WIDTH(rect) - nStrWidth) / 2, 
				nPosY /*+ nFontAscent*/ + 18 * ii, 
				szLineBuff, UI_COLOR_BLACK);
		} else {
			break;
		}
    }
}

static int MsgBoxWndOnEvent(atHWINDOW hWnd, int nMsg, long nParam1, long nParam2)
{
	LPMSGBOXWNDDATA lpData = (LPMSGBOXWNDDATA)atWINDOW_GetDataPtr(hWnd);

	switch( nMsg ){
		case WNDMSG_INIT:
			MsgBoxWndOnInit( hWnd, (void *)nParam1 );
			break;
		case WNDMSG_REGIST:
			MsgBoxWndOnRegister( hWnd );
			break;
		case WNDMSG_RELEASE:
			MsgBoxWndOnRelease( hWnd );
			break;
		case WNDMSG_PAINT:
			MsgBoxWndOnPaint( hWnd );
			break;
		case WNDMSG_KEYDOWN:
			return MsgBoxWndOnKeyDown( hWnd, nParam1 );
			break;
		case WNDMSG_TIMER:
			atWINDOW_PostMessage( hWnd, WNDMSG_KEYDOWN, atVKEY_SELECT, 0 );
			break;
		case WNDMSG_COMMAND:
			if( atWINDOW_GetID(hWnd) != 0 ){
				if( lpData->nMsgBoxType == atMBT_QUESTION_CHECK ){
					if( nParam1 == atMB_YES && atCHECKBOX_IsChecked(lpData->hControl1) )
						nParam1 = atMB_YES_CHECK;
				}
				atWINDOW_PostMessage( atWINDOW_GetParentWindow(hWnd), nMsg, atWINDOW_GetID(hWnd), nParam1 );
			}
			atAPP_DestroyWindow( atAPPMNG_GetCurApp(), hWnd );
			break;
		default:
			return 0;
			break;
	}

	return 1;
}


