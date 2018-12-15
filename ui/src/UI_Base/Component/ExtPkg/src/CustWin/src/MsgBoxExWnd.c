//
// MsgBox.h
//
//  Date: 2005-10-26
// Coder: YongChul, Jin
//
///////////////////////////////////////////////////////////////////////


#include "A_UIENG.h"
#include "UI_Util.h"
#include "MsgBoxExWnd.h"
#include "AEECOMP_ExtPkg.h"
#include "AEECOMP_BasePkg.h"



typedef struct _MSGBOXWNDDATA {
	atHWINDOW		hTargetWnd;
	atHBITMAP	lpTitleJBM1, lpTitleJBM2;		// use one image only for draw speed

	int		nMsgBoxType;
	int		nStrLineNum;
	int		nMaxStrLineNum;

	atTCHAR	szMsg[1024];

	long	lWaitTime;
	atHTIMER	hWaitTimer;

	atHCONTROL	hControl1, hControl2, hControl3;
} MSGBOXWNDDATA, * LPMSGBOXWNDDATA;

int MsgBoxWndOnEvent(atHWINDOW hWnd, int nMsg, long nParam1, long nParam2);


struct _ClassExData {
	atHBITMAP	   lpTitleBG;
	atHBITMAP	   lpTitleAlert;
	atHBITMAP	   lpTitleConfirm;
	atHBITMAP	   lpTitleQuest;
	atHBITMAP	   lpTitleError;
};

atBOOL      RegistMsgBoxExWndClass( atHRESSRC hResSrc )
{
	atWINCLASS wc;
	struct _ClassExData *ExData;
	
	atUIAPI_memset( &wc, 0, sizeof(atWINCLASS) );
	wc.dwAttr = atWS_USERDEFINE | atWS_STAYONTOP | atWS_MODAL;
 	wc.dwUserDataSize = sizeof(MSGBOXWNDDATA);
	wc.fnWndProc = MsgBoxWndOnEvent;

    wc.tLayoutBm.bmTopLeft      = atUIAPI_CreateBitmapFromRes( hResSrc, _AT("pop_msg_tl.img") );
	atUIAPI_SetBitmapTransColor( wc.tLayoutBm.bmTopLeft, atUI_COLOR_TRANS );
    wc.tLayoutBm.bmTopCenter    = atUIAPI_CreateBitmapFromRes( hResSrc, _AT("pop_msg_tc.img") );
    wc.tLayoutBm.bmTopRight     = atUIAPI_CreateBitmapFromRes( hResSrc, _AT("pop_msg_tr.img") );
	atUIAPI_SetBitmapTransColor( wc.tLayoutBm.bmTopRight, atUI_COLOR_TRANS );
    wc.tLayoutBm.bmMiddleLeft   = atUIAPI_CreateBitmapFromRes( hResSrc, _AT("pop_msg_ml.img") );
    wc.tLayoutBm.bmMiddleCenter = atUIAPI_CreateBitmapFromRes( hResSrc, _AT("pop_msg_mc.img") );
    wc.tLayoutBm.bmMiddleRight  = atUIAPI_CreateBitmapFromRes( hResSrc, _AT("pop_msg_mr.img") );
    wc.tLayoutBm.bmBottomLeft   = atUIAPI_CreateBitmapFromRes( hResSrc, _AT("pop_msg_bl.img") );
	atUIAPI_SetBitmapTransColor( wc.tLayoutBm.bmBottomLeft, atUI_COLOR_TRANS );
    wc.tLayoutBm.bmBottomCenter = atUIAPI_CreateBitmapFromRes( hResSrc, _AT("pop_msg_bc.img") );
    wc.tLayoutBm.bmBottomRight  = atUIAPI_CreateBitmapFromRes( hResSrc, _AT("pop_msg_br.img") );
	atUIAPI_SetBitmapTransColor( wc.tLayoutBm.bmBottomRight, atUI_COLOR_TRANS );
	
	SET_MARGIN( wc.rtMargin, 34, 4, 4, 4 );

    ExData = (struct _ClassExData *)wc.ExData;
	
	ExData->lpTitleBG				= atUIAPI_CreateBitmapFromRes( hResSrc, _AT("pop_msg_title.img") );
	ExData->lpTitleAlert				= atUIAPI_CreateBitmapFromRes( hResSrc, _AT("title_alert.img") );
	ExData->lpTitleConfirm			= atUIAPI_CreateBitmapFromRes( hResSrc, _AT("title_confirm.img") );
	ExData->lpTitleQuest				= atUIAPI_CreateBitmapFromRes( hResSrc, _AT("title_quest.img") );
	ExData->lpTitleError				= atUIAPI_CreateBitmapFromRes( hResSrc, _AT("title_error.img") );
	
//	atUIAPI_memcpy( wc.ExData, &ExData, sizeof(struct _ClassExData) );
	
	return atAPP_RegistWinClass( atAPPMNG_GetCurApp(), EXTPKG_CUSTWIN_MSGBOX, &wc );
}

void	UnregistMsgBoxExWndClass(void)
{
	atWINCLASS *wc;
	struct _ClassExData *ExData;

    wc = atAPP_GetWinClass( atAPPMNG_GetCurApp(), EXTPKG_CUSTWIN_MSGBOX );
    if( wc == atNULL ) return;
	
    ExData = (struct _ClassExData *)wc->ExData;
	
	atUIAPI_DestroyBitmap( ExData->lpTitleError );
	atUIAPI_DestroyBitmap( ExData->lpTitleQuest );
	atUIAPI_DestroyBitmap( ExData->lpTitleConfirm );
	atUIAPI_DestroyBitmap( ExData->lpTitleAlert );
	atUIAPI_DestroyBitmap( ExData->lpTitleBG );

	// 윈도우 레이아웃용 비트맵을 해제한다. 임시로 이렇게 했고, 나중에 이미지 관리하는 적절한 방법을 구상하자.
    atUIAPI_DestroyBitmap( wc->tLayoutBm.bmTopLeft );
    atUIAPI_DestroyBitmap( wc->tLayoutBm.bmTopCenter ); 
    atUIAPI_DestroyBitmap( wc->tLayoutBm.bmTopRight ); 
    atUIAPI_DestroyBitmap( wc->tLayoutBm.bmMiddleLeft ); 
    atUIAPI_DestroyBitmap( wc->tLayoutBm.bmMiddleCenter ); 
    atUIAPI_DestroyBitmap( wc->tLayoutBm.bmMiddleRight );
    atUIAPI_DestroyBitmap( wc->tLayoutBm.bmBottomLeft ); 
    atUIAPI_DestroyBitmap( wc->tLayoutBm.bmBottomCenter );
    atUIAPI_DestroyBitmap( wc->tLayoutBm.bmBottomRight ); 
}

struct _CreateParam {
	atHWINDOW		hTargetWnd;
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
	atWORD				unCenterHeight, unHeight;//, unWidth;
	atMARGIN anchor;

	nLineNum = myGetLineNum(szMsg);
	nMaxLineNum = nLineNum;

	switch ( nMsgBoxType ){
		case atMBT_EX_ALERT:
			break;
		case atMBT_EX_CONFIRM:
			break; 
		case atMBT_EX_QUESTION:
			break;
		case atMBT_EX_QUESTION_CANCEL:
			break;
		case atMBT_EX_QUESTION_CHECK:
			nLineNum--;
			break;
		case atMBT_EX_ERROR:
			break;
		case atMBT_EX_CUSTOM_1BTN:
			nLineNum--;
			nMaxLineNum--;
			break;
		case atMBT_EX_CUSTOM_2BTN:
			nLineNum-=2;
			nMaxLineNum-=2;
			break;
		case atMBT_EX_CUSTOM_3BTN:
			nLineNum-=3;
			nMaxLineNum-=3;
			break;
	}

//	unWidth = atUIAPI_GetScreenWidth() - 6*2;
	if ( nMaxLineNum <= 3 ){
		nMaxLineNum = 3;
	} else if ( nMaxLineNum <= 4 ) {
		nMaxLineNum = 4;
	} else {
		nMaxLineNum = 5;
	}

	unCenterHeight	= 18*nMaxLineNum + 53;
	unHeight = unCenterHeight + 34 + 4;

	param.hTargetWnd = hParentWnd;
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
								EXTPKG_CUSTWIN_MSGBOX, &param );
	atAPP_SetAnchor( atAPPMNG_GetCurApp(), hWnd, atANCHOR_IGNORE, atANCHOR_IGNORE, 20, 20 );
	atAPP_ShowWindow( atAPPMNG_GetCurApp(), hWnd, atTRUE );

	return hWnd;
}


atHWINDOW	CreateMsgBoxEx( atHWINDOW hParentWnd, int id, atLPTSTR szWindowText, atLPTSTR szMsg, int nMsgBoxType )
{
	return CreateMsgBox_0( hParentWnd, id, szWindowText, szMsg, nMsgBoxType, 0 );
}

atHWINDOW	CreateMsgBoxEx2( atHWINDOW hParentWnd, int id, atLPTSTR szWindowText, atLPTSTR szMsg, int nMsgBoxType, long lWaitTime )
{
	return CreateMsgBox_0( hParentWnd, id, szWindowText, szMsg, nMsgBoxType, lWaitTime );
}


/*
static void tmr( atHTIMER hTimer, void *data )
{
	atMSG_PostMessage( (atHWINDOW)data, WNDMSG_KEYDOWN, atVKEY_SELECT, 0 );
}
*/

static void MsgBoxWndOnInit( atHWINDOW hWnd, struct _CreateParam *lpParam, struct _ClassExData *ExData )
{
	LPMSGBOXWNDDATA lpData = (LPMSGBOXWNDDATA)atWINDOW_GetDataPtr(hWnd);
//	int					nLineNum, nMaxLineNum, nBtnYPos;
//	atWORD				unCenterHeight, unHeight, unWidth;
//	atHCONTROL		hControl;
	int nBtnYPos;
	atHBITMAP				lpBM1, lpBM2;
	atRECT rect;
	int ClientWidth;
	int BtnWidth;
	atTCHAR buf[256];
	atHGDC hCanvas;

	// 윈도우 레이아웃을 설정한다. 임시로 이렇게 했고, 나중에 이미지 관리하는 적절한 방법을 구상하자.
//    atWINDOW_SetLayoutBitmap2( hWnd, &ExData->WinLayout );
//    atWINDOW_SetLayoutMargin2( hWnd, &ExData->margin );

	nBtnYPos = lpParam->unCenterHeight - 36;//199-24;

//	atWINDOW_SetLayoutMargin( hMsgBoxWnd, 34, 4, 4, 4 );
	atWINDOW_SetTitleText( hWnd, _AT("") );

	lpData->hTargetWnd = lpParam->hTargetWnd;	
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

	lpBM1 = ExData->lpTitleBG;
	lpBM2 = atNULL;
	switch ( lpParam->nMsgBoxType ){
	case atMBT_EX_ALERT:
//		atVD_SOUND_Play( IMSOUND.hSound2, atFALSE );
		lpBM2 = ExData->lpTitleAlert;
		lpData->hControl1 = ExtPkg_CreateButton(hWnd, atMB_EX_CANCEL, (ClientWidth-BtnWidth)>>1, nBtnYPos, BtnWidth, 18, _AT("Close"));
		atCONTROL_SetAnchor( lpData->hControl1, atANCHOR_IGNORE, 20, atANCHOR_IGNORE, atANCHOR_IGNORE );
		break;
	case atMBT_EX_CONFIRM:
//		atVD_SOUND_Play( IMSOUND.hSound4, atFALSE );
		lpBM2 = ExData->lpTitleConfirm;
		lpData->hControl1 = ExtPkg_CreateButton(hWnd, atMB_EX_OK, (ClientWidth-BtnWidth)>>1, nBtnYPos, BtnWidth, 18, _AT("OK"));
		atCONTROL_SetAnchor( lpData->hControl1, atANCHOR_IGNORE, 20, atANCHOR_IGNORE, atANCHOR_IGNORE );
		break;
	case atMBT_EX_QUESTION:
//		atVD_SOUND_Play( IMSOUND.hSound3, atFALSE );
		lpBM2 = ExData->lpTitleQuest;
		lpData->hControl1 = ExtPkg_CreateButton(hWnd, atMB_EX_YES,  (ClientWidth-(BtnWidth*2 + 20))>>1, nBtnYPos, BtnWidth, 18, _AT("Yes"));
		atCONTROL_SetAnchor( lpData->hControl1, atANCHOR_IGNORE, 20, 20, atANCHOR_IGNORE );
		lpData->hControl2 = ExtPkg_CreateButton(hWnd, atMB_EX_NO,  ((ClientWidth-(BtnWidth*2 + 20))>>1) + BtnWidth+20, nBtnYPos, BtnWidth, 18, _AT("No"));
		atCONTROL_SetAnchor( lpData->hControl2, atANCHOR_IGNORE, 20, atANCHOR_IGNORE, 20 );
		break;
	case atMBT_EX_QUESTION_CANCEL:
//		atVD_SOUND_Play( IMSOUND.hSound3, atFALSE );
		lpBM2 = ExData->lpTitleQuest;
		lpData->hControl1 = ExtPkg_CreateButton(hWnd, atMB_EX_YES,     (ClientWidth-(BtnWidth*3 + 10*2))>>1, nBtnYPos, BtnWidth, 18, _AT("Yes"));
		atCONTROL_SetAnchor( lpData->hControl1, atANCHOR_IGNORE, 20, 20, atANCHOR_IGNORE );
		lpData->hControl2 = ExtPkg_CreateButton(hWnd, atMB_EX_NO,     ((ClientWidth-(BtnWidth*3 + 10*2))>>1) + BtnWidth+10, nBtnYPos, BtnWidth, 18, _AT("No"));
		atCONTROL_SetAnchor( lpData->hControl2, atANCHOR_IGNORE, 20, atANCHOR_IGNORE, atANCHOR_IGNORE );
		lpData->hControl3 = ExtPkg_CreateButton(hWnd, atMB_EX_CANCEL, ((ClientWidth-(BtnWidth*3 + 10*2))>>1) + BtnWidth*2+10*2, nBtnYPos, BtnWidth, 18, _AT("Cancel"));
		atCONTROL_SetAnchor( lpData->hControl3, atANCHOR_IGNORE, 20, atANCHOR_IGNORE, 20 );
		atWINDOW_SetCurControl( hWnd, lpData->hControl3 );
		break;
	case atMBT_EX_QUESTION_CHECK:
//		atVD_SOUND_Play( IMSOUND.hSound3, atFALSE );
		lpBM2 = ExData->lpTitleQuest;
		myGetStrLine(buf, lpData->szMsg, lpData->nStrLineNum + 1, _AT('\n')); // 가장 마지막 줄을 CheckBox 메시지로 사용.
//		lpData->nStrLineNum --; // 마지막 한줄을 없앤다.
		lpData->hControl1 = ExtPkg_CreateCheckBox(hWnd, atMB_EX_YES_CHECK, (ClientWidth-atUIAPI_GetStringWidth(atUIAPI_GetFont(hCanvas),buf,-1))>>1, nBtnYPos-28, 0, 18, buf);
		atCONTROL_SetAnchor( lpData->hControl1, atANCHOR_IGNORE, 20, atANCHOR_IGNORE, atANCHOR_IGNORE );
		lpData->hControl2 = ExtPkg_CreateButton(hWnd, atMB_EX_YES, (ClientWidth-(BtnWidth*2 + 20))>>1, nBtnYPos, BtnWidth, 18, _AT("Yes"));
		atCONTROL_SetAnchor( lpData->hControl2, atANCHOR_IGNORE, 20, 20, atANCHOR_IGNORE );
		lpData->hControl3 = ExtPkg_CreateButton(hWnd, atMB_EX_NO, ((ClientWidth-(BtnWidth*2 + 20))>>1) + BtnWidth+20, nBtnYPos, BtnWidth, 18, _AT("No"));
		atCONTROL_SetAnchor( lpData->hControl3, atANCHOR_IGNORE, 20, atANCHOR_IGNORE, 20 );
		atWINDOW_SetCurControl( hWnd, lpData->hControl2 );
		break;
	case atMBT_EX_ERROR:
//		atVD_SOUND_Play( IMSOUND.hSound2, atFALSE );
		lpBM2 = ExData->lpTitleError;
		lpData->hControl1 = ExtPkg_CreateButton(hWnd, atMB_EX_OK, (ClientWidth-BtnWidth)>>1, nBtnYPos, 60, 18, _AT("Close"));
		atCONTROL_SetAnchor( lpData->hControl1, atANCHOR_IGNORE, 20, atANCHOR_IGNORE, atANCHOR_IGNORE );
		break;
	case atMBT_EX_CUSTOM_1BTN:
		lpBM2 = ExData->lpTitleConfirm;
		myGetStrLine(buf, lpData->szMsg, lpData->nStrLineNum + 1, _AT('\n')); // 가장 마지막 줄을 Button 메시지로 사용.
		lpData->hControl1 = ExtPkg_CreateButton(hWnd, atMB_EX_CUSTOM_1, (ClientWidth-BtnWidth)>>1, nBtnYPos, BtnWidth, 18, buf);
		atCONTROL_SetAnchor( lpData->hControl1, atANCHOR_IGNORE, 20, atANCHOR_IGNORE, atANCHOR_IGNORE );
		break;
	case atMBT_EX_CUSTOM_2BTN:
		lpBM2 = ExData->lpTitleQuest;
		myGetStrLine(buf, lpData->szMsg, lpData->nStrLineNum + 1, _AT('\n')); // 가장 마지막 줄을 Button 메시지로 사용.
		lpData->hControl1 = ExtPkg_CreateButton(hWnd, atMB_EX_CUSTOM_1, 62, nBtnYPos, BtnWidth, 18, buf);
		atCONTROL_SetAnchor( lpData->hControl1, atANCHOR_IGNORE, 20, 20, atANCHOR_IGNORE );
		myGetStrLine(buf, lpData->szMsg, lpData->nStrLineNum + 2, _AT('\n')); // 가장 마지막 줄을 Button 메시지로 사용.
		lpData->hControl2 = ExtPkg_CreateButton(hWnd, atMB_EX_CUSTOM_2, 130, nBtnYPos, BtnWidth, 18, buf);
		atCONTROL_SetAnchor( lpData->hControl2, atANCHOR_IGNORE, 20, atANCHOR_IGNORE, 20 );
		break;
	case atMBT_EX_CUSTOM_3BTN:
		lpBM2 = ExData->lpTitleQuest;
		myGetStrLine(buf, lpData->szMsg, lpData->nStrLineNum + 1, _AT('\n')); // 가장 마지막 줄을 Button 메시지로 사용.
		lpData->hControl1 = ExtPkg_CreateButton(hWnd, atMB_EX_CUSTOM_1, 32, nBtnYPos, 0, 18, buf);
		atCONTROL_SetAnchor( lpData->hControl1, atANCHOR_IGNORE, 20, 20, atANCHOR_IGNORE );
		myGetStrLine(buf, lpData->szMsg, lpData->nStrLineNum + 2, _AT('\n')); // 가장 마지막 줄을 Button 메시지로 사용.
		lpData->hControl2 = ExtPkg_CreateButton(hWnd, atMB_EX_CUSTOM_2, 90, nBtnYPos, 0, 18, buf);
		atCONTROL_SetAnchor( lpData->hControl2, atANCHOR_IGNORE, 20, atANCHOR_IGNORE, atANCHOR_IGNORE );
		myGetStrLine(buf, lpData->szMsg, lpData->nStrLineNum + 3, _AT('\n')); // 가장 마지막 줄을 Button 메시지로 사용.
		lpData->hControl3 = ExtPkg_CreateButton(hWnd, atMB_EX_CUSTOM_3, 148, nBtnYPos, 0, 18, buf);
		atCONTROL_SetAnchor( lpData->hControl3, atANCHOR_IGNORE, 20, atANCHOR_IGNORE, 20 );
		atWINDOW_SetCurControl( hWnd, lpData->hControl3 );
		break;
	}

	lpData->lpTitleJBM1 = lpBM1;		
	lpData->lpTitleJBM2 = lpBM2;		
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
	LPMSGBOXWNDDATA lpData = (LPMSGBOXWNDDATA)atWINDOW_GetDataPtr(hWnd);

//	if( lpData->hWaitTimer ) atUIAPI_RemoveTimer( lpData->hWaitTimer );

//    if( lpData->lpTitleJBM1 ) atUIAPI_DestroyBitmap( lpData->lpTitleJBM1 );
//    if( lpData->lpTitleJBM2 ) atUIAPI_DestroyBitmap( lpData->lpTitleJBM2 );
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
	
	hCanvas = atWINDOW_GetWindowGDC(hWnd);
	atWINDOW_GetWindowRect( hWnd, &rect );

	if( lpData->lpTitleJBM1 )
		atUIAPI_DrawImageAlign( hCanvas, RECT_WIDTH(rect)/2, 0, lpData->lpTitleJBM1, ALIGN_CENTER_TOP);
	if( lpData->lpTitleJBM2 )
		atUIAPI_DrawImageAlign( hCanvas, RECT_WIDTH(rect)/2, 7, lpData->lpTitleJBM2, ALIGN_CENTER_TOP );

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

int MsgBoxWndOnEvent(atHWINDOW hWnd, int nMsg, long nParam1, long nParam2)
{
	LPMSGBOXWNDDATA lpData = (LPMSGBOXWNDDATA)atWINDOW_GetDataPtr(hWnd);

	switch( nMsg ){
		case WNDMSG_INIT:
			MsgBoxWndOnInit( hWnd, (void *)nParam1, (void *)nParam2 );
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
				if( lpData->nMsgBoxType == atMBT_EX_QUESTION_CHECK ){
					if( nParam1 == atMB_EX_YES && atCHECKBOX_EXT_IsChecked(lpData->hControl1) )
						nParam1 = atMB_EX_YES_CHECK;
				}
				atWINDOW_PostMessage( lpData->hTargetWnd, nMsg, atWINDOW_GetID(hWnd), nParam1 );
			}
			atAPP_DestroyWindow( atAPPMNG_GetCurApp(), hWnd );
			break;
		default:
			return 0;
			break;
	}

	return 1;
}


