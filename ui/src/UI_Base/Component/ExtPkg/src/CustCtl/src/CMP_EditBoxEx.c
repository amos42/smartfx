#include "A_UIENG.h"
#include "UI_Util.h"
#include "CMP_EditBoxEx.h"


#define EDIT_CURSOR_BLINK_ELAPSE (500)


typedef struct _tagatEDITBOX {
	atU32		uAttr;
	atTCHAR		szText[256];
	// Kyle Start [Add 2009/06/03-17:41]
	atTCHAR		szTempAct[2];
	// Kyle [Add 2009/06/03-17:41] End
	atBOOL		bReadOnly;

	atEDITBOX_EXT_INPUTMODE lstInputMode[atEDITBOX_EXT_INPUTMODE_MAX];
	atINT 		nInputModeCount;
		
    atHBITMAP lpNormalImg_Left, lpNormalImg_Center, lpNormalImg_Right;
	atHBITMAP lpFocusedImg_Left, lpFocusedImg_Center, lpFocusedImg_Right;
	atHBITMAP lpDisabledImg_Left, lpDisabledImg_Center, lpDisabledImg_Right;

	int			bCursorBlink;	
	int			nBlinkCnt;
	atBOOL		bAlwaysBlink;

	atUI_HIME	hIME;
	
	int 		nDispPosX;

	atUI_OIC hOIC;	
} atEDITBOX, * atLPEDITBOX;


static int EditBoxProc( atHCONTROL hControl, int nMsg, long lParam1, long lParam2 );


// OIC용 CallBack
static int _xxx( void *lpParam, int msg, long lParam1, long lParam2 )
{
	atHCONTROL hControl = (atHCONTROL)lpParam;
	atLPCONTROL lpControl = atCONTROL_GetPtr(hControl);

	switch( msg ){
		case atUI_OICMSG_CHANGE:
			atCONTROL_PostMessageToParent( hControl, WNDMSG_CHANGE, (long)lpControl->Base.nID, 0 );
			break;
	}

	return 0;
}


struct _CreateParam {
	atLPTSTR szText;
	atEDITBOX_EXT_INPUTMODE *lstInputMode;
	atBOOL bUseOIC;
	atINT 		nInputModeCount;
	atINT		nCurInputMode;
       atHBITMAP lpNormalImg_Left, lpNormalImg_Center, lpNormalImg_Right;
	atHBITMAP lpFocusedImg_Left, lpFocusedImg_Center, lpFocusedImg_Right;
	atHBITMAP lpDisabledImg_Left, lpDisabledImg_Center, lpDisabledImg_Right;
};

atHCONTROL atEDITBOX_EXT_Create( int nIndex, int nX, int nY, int nWidth, int nHeight, atLPTSTR szText,
						atBOOL bUseOIC,
						atEDITBOX_EXT_INPUTMODE lstInputMode[], atINT nInputModeCount, atINT nCurInputMode,
						 atHBITMAP lpNormalImg_Left, atHBITMAP lpNormalImg_Center, atHBITMAP lpNormalImg_Right,
						 atHBITMAP lpFocusedImg_Left, atHBITMAP lpFocusedImg_Center, atHBITMAP lpFocusedImg_Right,
						 atHBITMAP lpDisabledImg_Left, atHBITMAP lpDisabledImg_Center, atHBITMAP lpDisabledImg_Right )
{
	atHCONTROL		hControl;
	struct _CreateParam param;

	param.szText = szText;
	param.bUseOIC = bUseOIC;
	param.lstInputMode		= lstInputMode;
	param.nInputModeCount	= nInputModeCount;
	param.nCurInputMode		= nCurInputMode;
	param.lpNormalImg_Left		= lpNormalImg_Left;
	param.lpNormalImg_Center	= lpNormalImg_Center;
	param.lpNormalImg_Right		= lpNormalImg_Right;
	param.lpFocusedImg_Left 	= lpFocusedImg_Left;
	param.lpFocusedImg_Center	= lpFocusedImg_Center;
	param.lpFocusedImg_Right	= lpFocusedImg_Right;
	param.lpDisabledImg_Left 	= lpDisabledImg_Left;
	param.lpDisabledImg_Center	= lpDisabledImg_Center;
	param.lpDisabledImg_Right	= lpDisabledImg_Right;
	
	hControl = atCONTROL_Create( nIndex, nX, nY, nWidth, nHeight, CTL_PARENTTYPE_WINDOW, sizeof(atEDITBOX), EditBoxProc, (void *)&param );
	if( hControl == atNULL ) return atNULL;
	
	{
		atLPCONTROL 	lpControl = atCONTROL_GetPtr(hControl);

		nWidth = RECT_WIDTH(lpControl->Base.rect);
		nHeight = RECT_HEIGHT(lpControl->Base.rect);
		if ( nWidth <= 0 )	{
			lpControl->Base.rect.nEndX		= nX + atUIAPI_GetStringWidth(atUIAPI_GetFont(atCONTROL_GetGDC(hControl)), szText, -1) + 2;
		}
		if ( nHeight <= 0 ){
			lpControl->Base.rect.nEndY		= nY + atUIAPI_GetFontHeight(atUIAPI_GetFont(atCONTROL_GetGDC(hControl))) + 2;
		}
	}
	
//	lpEditData = (atLPEDITBOX)atCONTROL_GetDataPtr( hControl );
	
	return hControl;
}


static void atEDITBOX_OnInit( atHCONTROL hControl, struct _CreateParam *param )
{
	int i, nCurInputMode;
	atLPCONTROL lpControl = atCONTROL_GetPtr(hControl);
	atLPEDITBOX	lpEditBox = (atLPEDITBOX)atCONTROL_GetDataPtr( hControl );
	
	lpEditBox->uAttr  = atEDITBOXATTR_EXT_OUTLINE | atEDITBOXATTR_EXT_OPAQUE | (ALIGN_LEFT_MIDDLE & atEDITBOXATTR_EXT_ALIGN_MASK);
	if( param->bUseOIC ) lpEditBox->uAttr  |= atEDITBOXATTR_EXT_USE_OIC;

	atUIAPI_memset( lpEditBox->szText, 0, sizeof(lpEditBox->szText) );
	UI_STRNCPY(lpEditBox->szText, param->szText, 256-1);

	atUIAPI_memset( lpEditBox->lstInputMode, 0, atEDITBOX_EXT_INPUTMODE_MAX * sizeof(atEDITBOX_EXT_INPUTMODE) );
	if( param->lstInputMode ){
		atUIAPI_memcpy( lpEditBox->lstInputMode, param->lstInputMode, param->nInputModeCount * sizeof(atEDITBOX_EXT_INPUTMODE) );
		lpEditBox->nInputModeCount = param->nInputModeCount;
		nCurInputMode = param->nCurInputMode;
	} else {
		lpEditBox->lstInputMode[0] = atEDITBOX_EXT_INPUTMODE_NUMERIC_ONLY;
		lpEditBox->nInputModeCount = 1;
		nCurInputMode = 0;
	}
	
//	lpEditBox->szTempText[0] = _ATC('\0');
	atUIAPI_strcpy(lpEditBox->szTempAct, _AT("I\0"));
	lpEditBox->bReadOnly = atFALSE;
	
	lpEditBox->lpNormalImg_Left		= param->lpNormalImg_Left;
	lpEditBox->lpNormalImg_Center	= param->lpNormalImg_Center;
	lpEditBox->lpNormalImg_Right 	= param->lpNormalImg_Right;
	lpEditBox->lpFocusedImg_Left 	= param->lpFocusedImg_Left;
	lpEditBox->lpFocusedImg_Center	= param->lpFocusedImg_Center;
	lpEditBox->lpFocusedImg_Right	= param->lpFocusedImg_Right;
	lpEditBox->lpDisabledImg_Left 	= param->lpDisabledImg_Left;
	lpEditBox->lpDisabledImg_Center	= param->lpDisabledImg_Center;
	lpEditBox->lpDisabledImg_Right	= param->lpDisabledImg_Right;

	lpEditBox->bCursorBlink = 1;
	lpEditBox->nBlinkCnt = 0;
	lpEditBox->bAlwaysBlink = 0;

	lpEditBox->nDispPosX = 0;
	//lpEditBox->nBufferMaxLen = 255;
	
	lpEditBox->hIME = atUIAPI_IME_CreateIME( lpEditBox->szText, sizeof(lpEditBox->szText)/sizeof(atTCHAR), atNULL );
	for( i = 0; i < lpEditBox->nInputModeCount; i++ ){
		atUI_HIMEMETHOD hImeMethod;
		atLPTSTR szMethodName;

		switch( lpEditBox->lstInputMode[i] ){
		case atEDITBOX_EXT_INPUTMODE_DUMMY:
				szMethodName = _AT("Dummy"); 
				break;
			case atEDITBOX_EXT_INPUTMODE_NUMERIC_ONLY: 
				szMethodName = _AT("NUM"); 
				break;
			case atEDITBOX_EXT_INPUTMODE_ENG_LOWERCASE_ONLY: 
				szMethodName = _AT("eng"); 
				break;
			case atEDITBOX_EXT_INPUTMODE_LOWERCASE: 
				szMethodName = _AT("abc"); 
				break;
			case atEDITBOX_EXT_INPUTMODE_ENG_UPPERCASE_ONLY: 
				szMethodName = _AT("ABC"); 
				break;
			case atEDITBOX_EXT_INPUTMODE_UPPERCASE: 
				szMethodName = _AT("ENG"); 
				break;
			case atEDITBOX_EXT_INPUTMODE_NUMERIC_AND_LOWERCASE: 
				szMethodName = _AT("123abc"); 
				break;
			default:
				szMethodName = atNULL; 
				break;
		}

		hImeMethod = atUIAPI_IME_CreateIMEMethodByName( szMethodName );
		if( hImeMethod != atNULL ){
			atUIAPI_IME_AddIMEMethod( lpEditBox->hIME, hImeMethod );
		}
	}
	atUIAPI_IME_SetCurIMEMethod( lpEditBox->hIME, nCurInputMode );
		
	lpControl->tControlColor.clNormalTxtColor = atUIAPI_GetRGB(21, 104, 218);
	lpControl->tControlColor.clFocusedTxtColor = atUIAPI_GetRGB(0x00, 0x3E, 0x8B);
	lpControl->tControlColor.clDisableTxtColor = atUIAPI_GetRGB(21, 104, 218);

	atCONTROL_AddControlTimer( hControl, 0, EDIT_CURSOR_BLINK_ELAPSE, atTRUE, atFALSE, atTRUE );

	if( param->bUseOIC ){
		lpEditBox->hOIC = atUIAPI_CreateOIC( 0, 0, 100, 20, _xxx, (void *)hControl );
		atUIAPI_SetOICText( lpEditBox->hOIC, param->szText, atUIAPI_strlen(param->szText) );
	} else {
		lpEditBox->hOIC = atNULL;
	}
}

static void atEDITBOX_OnDestroy( atHCONTROL hControl )
{
	atLPEDITBOX	lpEditBox = (atLPEDITBOX)atCONTROL_GetDataPtr( hControl );
	if( lpEditBox == atNULL ) return;
	
	if( lpEditBox->hIME ){
		atUIAPI_IME_DestroyIME(lpEditBox->hIME);
		lpEditBox->hIME = atNULL;
	}

	if( lpEditBox->hOIC ){
		atUIAPI_DestroyOIC(	lpEditBox->hOIC );
		lpEditBox->hOIC = atNULL;
	}
}

static int atEDITBOX_OnKeyChar( atHCONTROL hControl, long key )
{
	atLPEDITBOX	lpEditBox = (atLPEDITBOX)atCONTROL_GetDataPtr( hControl );
	atTCHAR str[2] = { '\0', '\0' };
	int nCursorPos;
	
	if( lpEditBox->bReadOnly ) return 0;

	str[0] = (atUCS_CHAR)key;

	nCursorPos = atUIAPI_IME_GetCursorPos( lpEditBox->hIME );
	if( nCursorPos < 0 ) nCursorPos = 0;
	nCursorPos = atUIAPI_IME_InsertString( lpEditBox->hIME, nCursorPos, str, 1, atTRUE );
	atUIAPI_IME_SetCursorPos( lpEditBox->hIME, nCursorPos );
	
	atCONTROL_Refresh(hControl);
	
	return 1;
}

static int atEDITBOX_OnKeyDown( atHCONTROL hControl, long key )
{
	atLPEDITBOX	lpEditBox = (atLPEDITBOX)atCONTROL_GetDataPtr( hControl );
	atBOOL draw_flag = atFALSE;
	int result = 0;

	if( lpEditBox->bReadOnly ) return 0;

	switch( key ){
		// 포커스 안 넘어가도록 이벤트 처리를 한 것으로 인식.
		case atVKEY_LEFT:
		case atVKEY_RIGHT:
			result = 1;
			break;
			
		case atVKEY_SELECT:
			atUIAPI_IME_Flush( lpEditBox->hIME, atFALSE );
			atCONTROL_PostMessageToParent( hControl, WNDMSG_COMMAND, (atLONG)atCONTROL_GetID(hControl), (atLONG)lpEditBox->szText );
			result = 1;
			break;

		default:
			break;
	}

	lpEditBox->nBlinkCnt = 0;
	if( draw_flag ){
		atCONTROL_Refresh(hControl);
	}
	
	return result;
}

static int atEDITBOX_OnLongKeyDown( atHCONTROL hControl, long key )
{
	atLPEDITBOX	lpEditBox = (atLPEDITBOX)atCONTROL_GetDataPtr( hControl );
	atBOOL draw_flag = atFALSE;
	int result = 0;

	switch( key ){
		case atVKEY_CLEAR:
			atUIAPI_IME_Clear( lpEditBox->hIME );
			lpEditBox->bCursorBlink = 1;
			draw_flag = atTRUE;
			result = 1;
			break;

		default:
			break;
	}

	lpEditBox->nBlinkCnt = 0;
	if( draw_flag ){
		
		atCONTROL_Refresh(hControl);
	}
	
	return result;
}

static int atEDITBOX_OnKeyPress( atHCONTROL hControl, long key )
{
	atLPEDITBOX	lpEditBox = (atLPEDITBOX)atCONTROL_GetDataPtr( hControl );
	atBOOL draw_flag = atFALSE;
	int result = 0;

	if( lpEditBox->bReadOnly ) return 0;

/*
	switch( key ){
		case atVKEY_0: case atVKEY_1: case atVKEY_2: case atVKEY_3: case atVKEY_4: 
		case atVKEY_5: case atVKEY_6: case atVKEY_7: case atVKEY_8: case atVKEY_9:
		case atVKEY_STAR: case atVKEY_POUND:
		case atVKEY_LEFT: case atVKEY_RIGHT:
		case atVKEY_CLEAR:
*/
			atUIAPI_IME_Input( lpEditBox->hIME, key );
			lpEditBox->bCursorBlink = 1;
			draw_flag = atTRUE;
			result = 1;
/*
			break;

		default:
			break;
	}
*/
			
	lpEditBox->nBlinkCnt = 0;
	if( draw_flag ){
		atCONTROL_Refresh(hControl);
	}
	
	return result;
}

static void atEDITBOX_OnPaint( atHGDC hCanvas, atHCONTROL hControl, atBOOL bFocused )
{
	atLPCONTROL lpControl = atCONTROL_GetPtr(hControl);
	atLPEDITBOX	lpEditBox = (atLPEDITBOX)atCONTROL_GetDataPtr( hControl );
	int				/*nAscent, */nHeight;
	atHBITMAP	lpBM_L, lpBM_C, lpBM_R;
	long color;
	int state;
	// Kyle Start [Modify & Add 2009/06/03-11:12]
	atRECT rect, rect2, rect3;
	atINT cursor_xx;
	// Kyle [Modify & Add 2009/06/03-11:12] End
	atTCHAR szDispText[300];
	atTCHAR szTempText[32];	
	int nCursorPos;
	int cursor_x, cursor_width, cursor_height, hl_len;
	atPOINT pt;

	if( !atCONTROL_IsVisible(hControl) ) return;

	atCONTROL_GetControlRect( hControl, &rect );

	if( atCONTROL_IsEnable(hControl) ){
		if( bFocused ){
			lpBM_L = lpEditBox->lpFocusedImg_Left;
			lpBM_C = lpEditBox->lpFocusedImg_Center;
			lpBM_R = lpEditBox->lpFocusedImg_Right; 
		} else {
			lpBM_L = lpEditBox->lpNormalImg_Left;
			lpBM_C = lpEditBox->lpNormalImg_Center;
			lpBM_R = lpEditBox->lpNormalImg_Right; 
		}
	} else {
		lpBM_L = lpEditBox->lpDisabledImg_Left;
		lpBM_C = lpEditBox->lpDisabledImg_Center;
		lpBM_R = lpEditBox->lpDisabledImg_Right; 
	}

	if( !lpBM_L || !lpBM_C || !lpBM_R )
	{
		lpBM_L = lpEditBox->lpNormalImg_Left;
		lpBM_C = lpEditBox->lpNormalImg_Center;
		lpBM_R = lpEditBox->lpNormalImg_Right; 
	}

//	atUIAPI_SetFont( hCanvas, lpControl->font );
	nHeight = atUIAPI_GetFontHeight(atUIAPI_GetFont(hCanvas));

	if( atCONTROL_IsEnable(hControl) ){
		if ( bFocused ){
			if( lpEditBox->uAttr & atEDITBOXATTR_EXT_OPAQUE ){
				if( lpBM_L && lpBM_C && lpBM_R )
	    				atUIAPI_DrawClient1LineStretch( hCanvas, 0, 0, RECT_WIDTH(rect), RECT_HEIGHT(rect), lpBM_L, lpBM_C, lpBM_R );
				else
					atUIAPI_FillRect(hCanvas, 0, 0, RECT_WIDTH(rect), RECT_HEIGHT(rect), lpControl->tControlColor.clFocusedBgColor);
			}
			if( lpEditBox->uAttr & atEDITBOXATTR_EXT_OUTLINE )
				atUIAPI_DrawRect(hCanvas, 0, 0, RECT_WIDTH(rect), RECT_HEIGHT(rect), lpControl->tControlColor.clFocusedFrColor, 1);
		} else {
			if( lpEditBox->uAttr & atEDITBOXATTR_EXT_OPAQUE ){
				if( lpBM_L && lpBM_C && lpBM_R )
	    				atUIAPI_DrawClient1LineStretch( hCanvas, 0, 0, RECT_WIDTH(rect), RECT_HEIGHT(rect), lpBM_L, lpBM_C, lpBM_R );
				else
					atUIAPI_FillRect(hCanvas, 0, 0, RECT_WIDTH(rect), RECT_HEIGHT(rect), lpControl->tControlColor.clNormalBgColor);
			}
			if( lpEditBox->uAttr & atEDITBOXATTR_EXT_OUTLINE )
				atUIAPI_DrawRect(hCanvas, 0, 0, RECT_WIDTH(rect), RECT_HEIGHT(rect), lpControl->tControlColor.clNormalFrColor, 1);
		}
		if ( bFocused ){
			color = lpControl->tControlColor.clFocusedTxtColor;
		} else {
			color = lpControl->tControlColor.clNormalTxtColor;
		}
	} else {
		bFocused= atFALSE;
		if( lpEditBox->uAttr & atEDITBOXATTR_EXT_OPAQUE ){
			if( lpBM_L && lpBM_C && lpBM_R )
    				atUIAPI_DrawClient1LineStretch( hCanvas, 0, 0, RECT_WIDTH(rect), RECT_HEIGHT(rect), lpBM_L, lpBM_C, lpBM_R );
			else
				atUIAPI_FillRect(hCanvas, 0, 0, RECT_WIDTH(rect), RECT_HEIGHT(rect), lpControl->tControlColor.clDisableBgColor);
		}
		if( lpEditBox->uAttr & atEDITBOXATTR_EXT_OUTLINE )
			atUIAPI_DrawRect(hCanvas, 0, 0, RECT_WIDTH(rect), RECT_HEIGHT(rect), lpControl->tControlColor.clDisableFrColor, 1);
		color = UI_COLOR_BLACK;
	}
	
	if( lpEditBox->uAttr & atEDITBOXATTR_EXT_OUTLINE ){
		rect2.nStartX = 1 + 4;
		rect2.nStartY = 1 + 4;
	} else {
		rect2.nStartX = 0 + 4;
		rect2.nStartY = 0 + 4;
	}
	rect2.nEndX = RECT_WIDTH(rect)-1 - rect2.nStartX*2;
	rect2.nEndY = RECT_HEIGHT(rect)-1 - rect2.nStartY*2;

	if( lpEditBox->uAttr & atEDITBOXATTR_EXT_USE_OIC ){
		if( bFocused ){
			atUIAPI_RefreshOIC(lpEditBox->hOIC);
			return;
		}
	}

	atUIAPI_memset( szDispText, 0, sizeof(szDispText) );
	atUIAPI_strcpy( szDispText, lpEditBox->szText );

//	atUIAPI_MoveRect( &rect2, &rect2, 0, lpEditBox->nDispPosX );
	nCursorPos = atUIAPI_IME_GetCursorPos(lpEditBox->hIME);
	state = atUIAPI_IME_GetState(lpEditBox->hIME);
	if( state == atUI_IMESTATE_COMPOSING ){
		atUIAPI_memset( szTempText, 0, sizeof(szTempText) );
		atUIAPI_IME_GetComposeText( lpEditBox->hIME, szTempText );
		if( atUIAPI_strlen( szTempText ) > 0 ){
			atUIAPI_strins( szDispText, szTempText, nCursorPos, -1 );
		}
	}

	if( !lpEditBox->bReadOnly && (nCursorPos >= 0) ){
		cursor_x = atUIAPI_GetStringWidth( atUIAPI_GetFont(hCanvas), szDispText, nCursorPos );

		cursor_xx = 0;
		if( cursor_x - lpEditBox->nDispPosX <= 0 ) {						// Cursor 가 앞에 위치할때
			cursor_xx = nCursorPos - atEDITBOXVALUE_PAGEING;		// 커서 좌측 이동 (페이징)
			
			if(cursor_xx <= atEDITBOXVALUE_PAGEING) {
				cursor_xx = 0;
			}

			lpEditBox->nDispPosX = atUIAPI_GetStringWidth( atUIAPI_GetFont(hCanvas), szDispText, cursor_xx);
		} else if( cursor_x - lpEditBox->nDispPosX >= (RECT_WIDTH(rect) - 16) ) {	// Cursor 가 뒤에 위치할때
			cursor_xx = nCursorPos + atEDITBOXVALUE_PAGEING;				// 커서 우측 이동 (페이징)
			
			if(cursor_xx > atUIAPI_strlen(szDispText) - atEDITBOXVALUE_PAGEING) {
				cursor_xx = atUIAPI_strlen(szDispText);
			}

			lpEditBox->nDispPosX = atUIAPI_GetStringWidth( atUIAPI_GetFont(hCanvas), szDispText, cursor_xx);
			lpEditBox->nDispPosX -= (RECT_WIDTH(rect) - 16);
		} else {																	// Cursor 가 중간에 위치할때
			if(atUIAPI_strcmp(lpEditBox->szTempAct, _AT("D\0")) == 0) {				// 삭제 처리
				if(lpEditBox->nDispPosX > 0) {
					cursor_xx = atUIAPI_GetStringWidth( atUIAPI_GetFont(hCanvas), szDispText, atUIAPI_strlen(szDispText));

					if(nCursorPos == atUIAPI_strlen(szDispText)) {
						lpEditBox->nDispPosX = cursor_x - (RECT_WIDTH(rect) - 16);
					} else if(cursor_xx - lpEditBox->nDispPosX <= (RECT_WIDTH(rect) - 16)) {
						lpEditBox->nDispPosX = atUIAPI_GetStringWidth( atUIAPI_GetFont(hCanvas), szDispText, cursor_x + 1);
						lpEditBox->nDispPosX -= (RECT_WIDTH(rect) - 16);
					}
				}				

				atUIAPI_strcpy(lpEditBox->szTempAct, _AT("I\0"));
			}
		}
		// Kyle [Modify 2009/06/03-17:39] End
	}

	// Kyle Start [Modify 2009/06/03-11:12]
	// 클리핑 영역이 작아서 기존 크기로 맞게 설정 & 깨짐현상 수정
	// Kyle Start [Add 2009/06/02-15:24]
	// 에디터 박스가 스크롤이 되면 앞부분 글자에 대해 깨짐현상이 발생한다.
	// 클리핑을 처리해주었으나 글자겹침 또는 깨짐현상이 발생해서 더 살펴봐야 할듯 하다.
	atUIAPI_GetClipRect( hCanvas, &rect3 );
	rect3.nStartX = rect2.nStartX-1;
	rect3.nEndX -= 3;
	atUIAPI_SetClipRect( hCanvas, &rect3 );
	// Kyle [Add 2009/06/02-15:24] End
	// Kyle [Modify 2009/06/03-11:12] End
	
	atUIAPI_DrawStringAlignRect( hCanvas, -lpEditBox->nDispPosX, 0, &rect2, 
		szDispText,
		lpEditBox->uAttr & atEDITBOXATTR_EXT_ALIGN_MASK, color, &pt );

	if( !lpEditBox->bReadOnly && (nCursorPos >= 0) ){
		if( bFocused || lpEditBox->bAlwaysBlink ){
			if( lpEditBox->bCursorBlink ){
				cursor_x += pt.nX;
				cursor_height = atUIAPI_GetFontHeight( atUIAPI_GetFont(hCanvas) );
//				if( state == atUI_IMESTATE_COMPOSING ){
					hl_len = atUIAPI_IME_GetHilighttLength(lpEditBox->hIME);
					if( hl_len > 0 )
						cursor_width = atUIAPI_GetStringWidth( atUIAPI_GetFont(hCanvas), &szDispText[nCursorPos], hl_len );
					else
						cursor_width = 2;
					atUIAPI_FillRectXOR( hCanvas, cursor_x, pt.nY+1, cursor_width, cursor_height );
//				} else {
//					atUIAPI_FillRectXOR( hCanvas, cursor_x, pt.nY+1, 2, cursor_height );
//				}
			}
		}
	}
}

static void atEDITBOX_OnTimer( atHCONTROL hControl )
{
	atLPEDITBOX	lpEditBox = (atLPEDITBOX)atCONTROL_GetDataPtr( hControl );
	if( lpEditBox == atNULL ) return;
	if( lpEditBox->bReadOnly ) return;

	if( lpEditBox->uAttr & atEDITBOXATTR_EXT_USE_OIC ) return;
		
	if( /*atCONTROL_IsFocus(hControl) ||*/ lpEditBox->bAlwaysBlink ){
		if( ++lpEditBox->nBlinkCnt > (2000/EDIT_CURSOR_BLINK_ELAPSE) ){
			atUIAPI_IME_TimeOut( lpEditBox->hIME );
//			atUIAPI_IME_Flush( lpEditBox->hIME, atTRUE );
			lpEditBox->nBlinkCnt = 0;
		}		
			
		lpEditBox->bCursorBlink ^= 0x1;
		atCONTROL_Refresh(hControl);
	}
}


/*
#ifdef UNDER_CE			
static  atHCONTROL hCtl;
extern void OEM_UI_StartIME(void);
extern atTCHAR OEM_UI_IME_Buffer[256];
extern atLPUI_IME_NOTIFY_FUNC fnImeMethodFunc;

static int xxx( atUI_HIME hIme, atINT nMsg, atLONG nParam1, atLONG nParam2 )
{
	atLPCONTROL lpControl = atCONTROL_GetPtr(hCtl);
	atLPEDITBOX lpEditBox = (atLPEDITBOX )atCONTROL_GetDataPtr( hCtl );

if( nMsg == 10 && hCtl != atNULL){
//BasePkg_CreateMsgBox(atNULL, 0, _AT("xxxx"), _AT("yyyyyy"), 100 );	
if( atUIAPI_strlen( OEM_UI_IME_Buffer ) > 0 ){			
atUIAPI_strcpy( lpEditBox->szText, OEM_UI_IME_Buffer );
//_FlushComposite( lpEditBox );
atUIAPI_IME_Flush( lpEditBox->hIME, atFALSE );
atCONTROL_Refresh(hCtl);
}
hCtl = atNULL;
}
return 0;
}
#endif
*/


static int EditBoxProc( atHCONTROL hControl, int nMsg, long lParam1, long lParam2 )
{
	atLPCONTROL lpControl = atCONTROL_GetPtr(hControl);
	atLPEDITBOX lpEditBox = (atLPEDITBOX )atCONTROL_GetDataPtr( hControl );
	atRECT rect;
	int i;	
	int r = 0;

	switch( nMsg ){
		case CTLMSG_INIT:
			atEDITBOX_OnInit( hControl, (struct _CreateParam *)lParam1 );
			break;
		case CTLMSG_RELEASE:
			atEDITBOX_OnDestroy( hControl );
			r = 1;
			break;
		case CTLMSG_PAINT:
			atEDITBOX_OnPaint( atCONTROL_GetGDC(hControl), hControl, (atBOOL)lParam2 );
			r = 1;
			break;
		case CTLMSG_KEYCHAR:
			r = atEDITBOX_OnKeyChar( hControl, lParam1 );
			break;
		case CTLMSG_KEYDOWN:
			r = atEDITBOX_OnKeyDown( hControl, lParam1 );
			break;
		case CTLMSG_LONGKEYDOWN:
			r = atEDITBOX_OnLongKeyDown( hControl, lParam1 );
			break;
		case CTLMSG_KEYPRESS:
			r = atEDITBOX_OnKeyPress( hControl, lParam1 );
			break;
		case CTLMSG_TIMER:
			atEDITBOX_OnTimer( hControl );
			break;
		case CTLMSG_GOTFOCUS:
			if( lpEditBox->uAttr & atEDITBOXATTR_EXT_USE_OIC ){
				atUIAPI_SetOICText(lpEditBox->hOIC, lpEditBox->szText, COUNTOF(lpEditBox->szText));
				atUIAPI_SetOICFont(lpEditBox->hOIC, atUIAPI_GetFontHeight(lpControl->hFont) );
				atUIAPI_ShowAndSetFocusOIC(lpEditBox->hOIC);
			} else if( !lpEditBox->bReadOnly && !lpEditBox->bAlwaysBlink ){
				lpEditBox->bCursorBlink = 1;
				atCONTROL_StartControlTimer( hControl, 0 );
				atUIAPI_IME_Start( lpEditBox->hIME );
			}
			break;
		case CTLMSG_LOSTFOCUS:
			if( lpEditBox->uAttr & atEDITBOXATTR_EXT_USE_OIC ){
				atUIAPI_GetOICText(lpEditBox->hOIC, lpEditBox->szText, COUNTOF(lpEditBox->szText));
				atUIAPI_HideAndLostFocusOIC(lpEditBox->hOIC);
			} else if( !lpEditBox->bReadOnly && !lpEditBox->bAlwaysBlink ){
				atCONTROL_StopControlTimer( hControl, 0 );
				atUIAPI_IME_Flush( lpEditBox->hIME, atFALSE );
				atUIAPI_IME_Stop( lpEditBox->hIME );
			}
			break;		
		case CTLMSG_CHANGELAYOUT:
			if( lpEditBox->uAttr & atEDITBOXATTR_EXT_USE_OIC ){
				atWINOBJ_GetRect( hControl, &rect );
				atUIAPI_MoveRect( &rect, &rect, -rect.nStartX, -rect.nStartY );
				if( atWINOBJ_ConvScreenValidateRect(hControl, &rect, &rect ) ){
					i = 1;
					if( lpEditBox->uAttr & atEDITBOXATTR_EXT_OUTLINE ) i++;
					atUIAPI_MoveOIC( lpEditBox->hOIC, rect.nStartX+i, rect.nStartY+i );
					atUIAPI_SetOICSize( lpEditBox->hOIC, rect.nEndX-rect.nStartX-i*2, rect.nEndY-rect.nStartY-i*2 );
				//	atUIAPI_ShowAndSetFocusOIC(lpEditBox->hOIC);
				} else {
				//	atUIAPI_HideAndLostFocusOIC(lpEditBox->hOIC);
				}
			}
			break;
	}

	return r;
}



//=============================================================================


atUI_HIME atEDITBOX_EXT_GetIME( atHCONTROL hControl )
{
	atLPEDITBOX lpEditBox = (atLPEDITBOX )atCONTROL_GetDataPtr( hControl );
	if( lpEditBox == atNULL ) return atNULL;

	return lpEditBox->hIME;
}

atLPTSTR atEDITBOX_EXT_GetText( atHCONTROL hControl, atLPTSTR szDesText )
{
	atLPEDITBOX lpEditBox = (atLPEDITBOX )atCONTROL_GetDataPtr( hControl );
	if( lpEditBox == atNULL ) return atNULL;

	if( lpEditBox->uAttr & atEDITBOXATTR_EXT_USE_OIC ){
		atUIAPI_GetOICText( lpEditBox->hOIC, szDesText, 255 );
		return szDesText;
	}

	if( szDesText ){
		UI_STRCPY( szDesText, lpEditBox->szText );
		return szDesText;
	} else {
		return lpEditBox->szText;
	}
}

atLPTSTR atEDITBOX_EXT_GetFullText( atHCONTROL hControl, atLPTSTR szDesText )
{
	atLPEDITBOX lpEditBox = (atLPEDITBOX )atCONTROL_GetDataPtr( hControl );
	if( lpEditBox == atNULL ) return atNULL;

	if( szDesText == atNULL ) return atNULL;

	if( lpEditBox->uAttr & atEDITBOXATTR_EXT_USE_OIC ){
		atUIAPI_GetOICText( lpEditBox->hOIC, szDesText, 255 );
		return szDesText;
	}

//	atUIAPI_strcpy( szDesText, lpEditBox->szText );
//	atUIAPI_strins( szDesText, lpEditBox->szTempText, lpEditBox->nCursorPos, -1 );
	atUIAPI_IME_GetFullCompositeText( lpEditBox->hIME, szDesText );
	
	return szDesText;
}

void atEDITBOX_EXT_SetText( atHCONTROL hControl, atLPTSTR szNewText )
{
	atLPEDITBOX lpEditBox = (atLPEDITBOX )atCONTROL_GetDataPtr( hControl );
	if( lpEditBox == atNULL ) return;

//	UI_STRNCPY(lpEditBox->szText, szNewText, 255);
//	lpEditBox->szText[255] = '\0';

//	if( lpEditBox->nCursorPos > atUIAPI_strlen(lpEditBox->szText) ){
//		lpEditBox->nCursorPos = atUIAPI_strlen(lpEditBox->szText);
//	}

	if( lpEditBox->uAttr & atEDITBOXATTR_EXT_USE_OIC ){
		atUIAPI_SetOICText( lpEditBox->hOIC, szNewText, 255 );
		return;
	}

	atUIAPI_IME_SetText( lpEditBox->hIME, szNewText );	
//	lpEditBox->nCursorPos = atUIAPI_IME_GetCursorPos(lpEditBox->hIME);
}

int atEDITBOX_EXT_GetTextLength( atHCONTROL hControl )
{
	atLPTSTR str = atEDITBOX_EXT_GetText(hControl, atNULL);

	return (str != atNULL)? UI_STRLEN(str) : 0;
}

int atEDITBOX_EXT_GetFullTextLength( atHCONTROL hControl )
{
	atTCHAR szFullText[256];
	atLPEDITBOX lpEditBox = (atLPEDITBOX )atCONTROL_GetDataPtr( hControl );
	atLPTSTR str = atEDITBOX_EXT_GetFullText(hControl, szFullText);

	return (str != atNULL)? UI_STRLEN(str) : 0;
}

atU32 atEDITBOX_EXT_GetAttr(atHCONTROL hControl )
{
	atLPEDITBOX lpEditBox = (atLPEDITBOX )atCONTROL_GetDataPtr( hControl );
	if( lpEditBox == atNULL ) return 0;

	return lpEditBox->uAttr;
}

// Kyle Start [Add 2009/06/17-20:50]
int atEDITBOX_EXT_GetBufferMaxLength(atHCONTROL hControl)
{
	atLPEDITBOX lpEditBox = (atLPEDITBOX )atCONTROL_GetDataPtr( hControl );
	if( lpEditBox == atNULL ) return 0;

//	return lpEditBox->nBufferMaxLen;
	return atUIAPI_IME_GetMaxLen( lpEditBox->hIME );
}

// Kyle Start [Modify 2009/06/18-09:39]
int atEDITBOX_EXT_SetBufferMaxLength(atHCONTROL hControl, int nLen)
{
	atLPEDITBOX lpEditBox = (atLPEDITBOX )atCONTROL_GetDataPtr( hControl );
	if( lpEditBox == atNULL ) return 0;
	
//	if( nLen < 0 ) return lpEditBox->nBufferMaxLen;
//	( nLen > 255 ) ? (lpEditBox->nBufferMaxLen = 255) : (lpEditBox->nBufferMaxLen = nLen);
//	return lpEditBox->nBufferMaxLen;
	
	return atUIAPI_IME_SetMaxLen( lpEditBox->hIME, nLen );
}
// Kyle [Modify 2009/06/18-09:39] End
// Kyle [Add 2009/06/17-20:50] End

void atEDITBOX_EXT_SetAttr(atHCONTROL hControl, atU32 uAttr )
{
	atLPEDITBOX lpEditBox = (atLPEDITBOX )atCONTROL_GetDataPtr( hControl );
	if( lpEditBox == atNULL ) return;

	lpEditBox->uAttr = uAttr;
}

void atEDITBOX_EXT_SetAlign(atHCONTROL hControl, int align )
{
	atLPEDITBOX lpEditBox = (atLPEDITBOX )atCONTROL_GetDataPtr( hControl );
	if( lpEditBox == atNULL ) return;

	lpEditBox->uAttr &= ~atEDITBOXATTR_EXT_ALIGN_MASK;
	lpEditBox->uAttr |= align;
}

atBOOL atEDITBOX_EXT_IsReadOnly( atHCONTROL hControl )
{
	atLPEDITBOX lpEditBox = (atLPEDITBOX )atCONTROL_GetDataPtr( hControl );
	if( lpEditBox == atNULL ) return atTRUE;

	return lpEditBox->bReadOnly;
}

void atEDITBOX_EXT_ReadOnly( atHCONTROL hControl, atBOOL bReadOnly )
{
	atLPEDITBOX lpEditBox = (atLPEDITBOX )atCONTROL_GetDataPtr( hControl );
	if( lpEditBox == atNULL ) return;

	if( lpEditBox->bReadOnly == bReadOnly ) return;

	lpEditBox->bReadOnly = bReadOnly;
	if( !bReadOnly ){
		//if( atCONTROL_IsFocus(hControl) ){
			lpEditBox->bCursorBlink = 1;
			atCONTROL_StartControlTimer( hControl, 0 );	
		//}
	}
}

void atEDITBOX_EXT_SetAwaysBlink( atHCONTROL hControl, atBOOL bAwaysBlink )
{
	atLPEDITBOX lpEditBox = (atLPEDITBOX )atCONTROL_GetDataPtr( hControl );
	if( lpEditBox == atNULL ) return;

	if( lpEditBox->bAlwaysBlink == bAwaysBlink ) return;

	lpEditBox->bAlwaysBlink = bAwaysBlink;

	if( bAwaysBlink )
		atCONTROL_StartControlTimer( hControl, 0 );
}
