#include "A_UIENG.h"
#include "UI_Util.h"
#include "CMP_EditBox.h"


#define EDIT_CURSOR_BLINK_ELAPSE (500)


typedef struct _tagatEDITBOX {
	atU32		uAttr;
	atTCHAR		szText[256];
	atBOOL		bReadOnly;
	
	atEDITBOX_INPUTMODE lstInputMode[atEDITBOX_INPUTMODE_MAX];
	atINT 		nInputModeCount;
		
	int			bCursorBlink;
	int			nBlinkCnt;
	int			nStartPt;
	
	atUI_HIME	hIME;	
} atEDITBOX, * atLPEDITBOX;

static int EditBoxProc( atHCONTROL hControl, int nMsg, long lParam1, long lParam2 );


struct _CreateParam {
	atLPTSTR szText;
	atEDITBOX_INPUTMODE *lstInputMode;
	atINT 		nInputModeCount;
	atINT		nCurInputMode;
};

atHCONTROL atEDITBOX_Create( int nIndex, int nX, int nY, int nWidth, int nHeight, atLPTSTR szText,
								atEDITBOX_INPUTMODE lstInputMode[], atINT nInputModeCount, atINT nCurInputMode )
{
	atHCONTROL		hControl;
	struct _CreateParam param;

	param.szText = szText;
	param.lstInputMode		= lstInputMode;
	param.nInputModeCount	= nInputModeCount;
	param.nCurInputMode		= nCurInputMode;

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
	
	return hControl;
}


static void atEDITBOX_OnInit( atHCONTROL hControl, struct _CreateParam *param )
{
	int i, nCurInputMode;
	atLPEDITBOX	lpEditBox = (atLPEDITBOX)atCONTROL_GetDataPtr( hControl );
	
	lpEditBox->uAttr  = atEDITBOXATTR_OUTLINE | atEDITBOXATTR_OPAQUE | (ALIGN_LEFT_TOP & atEDITBOXATTR_ALIGN_MASK);
	
	UI_STRNCPY(lpEditBox->szText, param->szText, 256-1);
	lpEditBox->szText[256-1] = _ATC('\0');
	
	if( param->lstInputMode != atNULL ){
		atUIAPI_memcpy( lpEditBox->lstInputMode, param->lstInputMode, param->nInputModeCount * sizeof(atEDITBOX_INPUTMODE) );
		lpEditBox->nInputModeCount = param->nInputModeCount;
		nCurInputMode = param->nCurInputMode;
	} else {
		atUIAPI_memset( lpEditBox->lstInputMode, 0, atEDITBOX_INPUTMODE_MAX * sizeof(atEDITBOX_INPUTMODE) );
		lpEditBox->lstInputMode[0] = atEDITBOX_INPUTMODE_NUMERIC_ONLY;
		lpEditBox->nInputModeCount = 1;
		nCurInputMode = 0;
	}
	
	lpEditBox->bReadOnly = atFALSE;
	
	lpEditBox->nStartPt = 0;

	lpEditBox->bCursorBlink = 1;
	lpEditBox->nBlinkCnt = 0;
	
//	lpEditBox->hIME = atUIAPI_IME_CreateIMEByName( lpEditBox->szText, sizeof(lpEditBox->szText)/sizeof(atTCHAR), atNULL, _AT("eng"), atNULL );
//	lpEditBox->hIME = atUIAPI_IME_CreateIMEByName( lpEditBox->szText, sizeof(lpEditBox->szText)/sizeof(atTCHAR), atNULL, _AT("NUM"), _AT("eng"), _AT("ENG"), atNULL );
	lpEditBox->hIME = atUIAPI_IME_CreateIME( lpEditBox->szText, sizeof(lpEditBox->szText)/sizeof(atTCHAR), atNULL );
	for( i = 0; i < lpEditBox->nInputModeCount; i++ ){
		atUI_HIMEMETHOD hImeMethod;
		atLPTSTR szMethodName;

		switch( lpEditBox->lstInputMode[i] ){
			case atEDITBOX_INPUTMODE_DUMMY: 
				szMethodName = _AT("Dummy"); 
				break;
			case atEDITBOX_INPUTMODE_NUMERIC_ONLY: 
				szMethodName = _AT("NUM"); 
				break;
			case atEDITBOX_INPUTMODE_ENG_LOWERCASE_ONLY: 
			case atEDITBOX_INPUTMODE_LOWERCASE: 
				szMethodName = _AT("eng"); 
				break;
			case atEDITBOX_INPUTMODE_ENG_UPPERCASE_ONLY: 
			case atEDITBOX_INPUTMODE_UPPERCASE: 
				szMethodName = _AT("ENG"); 
				break;
			case atEDITBOX_INPUTMODE_NUMERIC_AND_LOWERCASE: 
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
	
	atCONTROL_AddControlTimer( hControl, 0, EDIT_CURSOR_BLINK_ELAPSE, atTRUE, atFALSE, atTRUE );
}

static void atEDITBOX_OnDestroy( atHCONTROL hControl )
{
	atLPEDITBOX	lpEditBox = (atLPEDITBOX)atCONTROL_GetDataPtr( hControl );
	if( lpEditBox == atNULL ) return;
	
	if( lpEditBox->hIME ){
		atUIAPI_IME_DestroyIME(lpEditBox->hIME);
		lpEditBox->hIME = atNULL;
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

	if( lpEditBox->bReadOnly ) return 0;

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

	switch( key ){
		case atVKEY_0: case atVKEY_1: case atVKEY_2: case atVKEY_3: case atVKEY_4: 
		case atVKEY_5: case atVKEY_6: case atVKEY_7: case atVKEY_8: case atVKEY_9:
		case atVKEY_STAR: case atVKEY_POUND:
		case atVKEY_LEFT: case atVKEY_RIGHT:
		case atVKEY_CLEAR:
			atUIAPI_IME_Input( lpEditBox->hIME, key );
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

static void atEDITBOX_OnPaint( atHGDC hCanvas, atHCONTROL hControl, atBOOL bFocused )
{
	atLPCONTROL lpControl = atCONTROL_GetPtr(hControl);
	atLPEDITBOX	lpEditBox = (atLPEDITBOX)atCONTROL_GetDataPtr( hControl );
	int				/*nAscent, */nHeight;
	long color;
	int state;
	atRECT rect, rect2;
	atTCHAR szDispText[300];
	atTCHAR szTempText[32];	
	int nCursorPos;
	int cursor_x, cursor_width, cursor_height, hl_len;
	atPOINT pt;

	if ( lpControl->Base.bShow == atFALSE )	return;

	atCONTROL_GetControlRect( hControl, &rect );

//	atUIAPI_SetFont( hCanvas, lpControl->font );
	nHeight = atUIAPI_GetFontHeight(atUIAPI_GetFont(hCanvas));

	if( atCONTROL_IsFocusable(hControl) ){
		if ( bFocused ){
			if( lpEditBox->uAttr & atEDITBOXATTR_OUTLINE )
				atUIAPI_DrawRect(hCanvas, 0, 0, RECT_WIDTH(rect), RECT_HEIGHT(rect), atUIAPI_GetRGB(0x00, 0x3E, 0x8B), 1);
			if( lpEditBox->uAttr & atEDITBOXATTR_OPAQUE )
				atUIAPI_FillRect(hCanvas, 1, 1, RECT_WIDTH(rect) - 2, RECT_HEIGHT(rect)- 2, atUIAPI_GetRGB(0xA2, 0xF8, 0xF8));
		} else {
			if( lpEditBox->uAttr & atEDITBOXATTR_OUTLINE )
				atUIAPI_DrawRect(hCanvas, 0, 0, RECT_WIDTH(rect), RECT_HEIGHT(rect), atUIAPI_GetRGB(76, 149, 248), 1);
			if( lpEditBox->uAttr & atEDITBOXATTR_OPAQUE )
				atUIAPI_FillRect(hCanvas, 1, 1, RECT_WIDTH(rect) - 2, RECT_HEIGHT(rect)- 2, atUIAPI_GetRGB(236, 255, 255));
		}
		if ( bFocused ){
			color = atUIAPI_GetRGB(0x00, 0x3E, 0x8B);
		} else {
			color = atUIAPI_GetRGB(21, 104, 218);
		}
	} else {
		if( lpEditBox->uAttr & atEDITBOXATTR_OUTLINE )
			atUIAPI_DrawRect(hCanvas, 0, 0, RECT_WIDTH(rect), RECT_HEIGHT(rect), atUIAPI_GetRGB(76, 149, 248), 1);
		if( lpEditBox->uAttr & atEDITBOXATTR_OPAQUE )
			atUIAPI_FillRect(hCanvas, 1, 1, RECT_WIDTH(rect) - 2, RECT_HEIGHT(rect)- 2, atUIAPI_GetRGB(236, 255, 255));
		color = UI_COLOR_BLACK;
	}
	
	if( lpEditBox->uAttr & atEDITBOXATTR_OUTLINE ){
		rect2.nStartX = 1;
		rect2.nStartY = 1;
	} else {
		rect2.nStartX = 0;
		rect2.nStartY = 0;
	}
	rect2.nEndX = RECT_WIDTH(rect)-1 - rect2.nStartX*2;
	rect2.nEndY = RECT_HEIGHT(rect)-1 - rect2.nStartY*2;

	atUIAPI_strcpy( szDispText, lpEditBox->szText );
	
	nCursorPos = atUIAPI_IME_GetCursorPos(lpEditBox->hIME);
	state = atUIAPI_IME_GetState(lpEditBox->hIME);
	if( state == atUI_IMESTATE_COMPOSING ){
		atUIAPI_strcpy( szTempText, _AT("") );
		atUIAPI_IME_GetComposeText( lpEditBox->hIME, szTempText );
		if( atUIAPI_strlen( szTempText ) > 0 ){
			atUIAPI_strins( szDispText, szTempText, nCursorPos, -1 );
		}
	}

	atUIAPI_DrawStringAlignRect( hCanvas, 0, 0, &rect2, 
		szDispText,
		lpEditBox->uAttr & atEDITBOXATTR_ALIGN_MASK, color, &pt );

	if( !lpEditBox->bReadOnly && (nCursorPos >= 0) ){
		if( bFocused && lpEditBox->bCursorBlink ){
			cursor_x = pt.nX + atUIAPI_GetStringWidth( atUIAPI_GetFont(hCanvas), szDispText, nCursorPos );
			cursor_height = atUIAPI_GetFontHeight( atUIAPI_GetFont(hCanvas) );
			if( state == atUI_IMESTATE_COMPOSING ){
				hl_len = atUIAPI_IME_GetHilighttLength(lpEditBox->hIME);
				if( hl_len > 0 )
					cursor_width = atUIAPI_GetStringWidth( atUIAPI_GetFont(hCanvas), &szDispText[nCursorPos], hl_len );
				else
					cursor_width = 2;
				atUIAPI_FillRectXOR( hCanvas, cursor_x, pt.nY+1, cursor_width, cursor_height );
			} else {
				atUIAPI_FillRectXOR( hCanvas, cursor_x, pt.nY+1, 2, cursor_height );
			}
		}
	}
}

static void atEDITBOX_OnTimer( atHCONTROL hControl )
{
	//if( atCONTROL_IsFocus(hControl) ){
		atLPEDITBOX	lpEditBox = (atLPEDITBOX)atCONTROL_GetDataPtr( hControl );
		if( lpEditBox == atNULL ) return;
		if( lpEditBox->bReadOnly ) return;

		if( ++lpEditBox->nBlinkCnt > (2000/EDIT_CURSOR_BLINK_ELAPSE) ){
			atUIAPI_IME_TimeOut( lpEditBox->hIME );
//			atUIAPI_IME_Flush( lpEditBox->hIME, atTRUE );
			lpEditBox->nBlinkCnt = 0;
		}		
			
		lpEditBox->bCursorBlink ^= 0x1;
		atCONTROL_Refresh(hControl);
	//}
}

static int EditBoxProc( atHCONTROL hControl, int nMsg, long lParam1, long lParam2 )
{
	atLPCONTROL lpControl = atCONTROL_GetPtr(hControl);
	atLPEDITBOX lpEditBox = (atLPEDITBOX )atCONTROL_GetDataPtr( hControl );
	
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
			if( !lpEditBox->bReadOnly ){
				lpEditBox->bCursorBlink = 1;
				atCONTROL_StartControlTimer( hControl, 0 );
				atUIAPI_IME_Start( lpEditBox->hIME );
			}
			break;
		case CTLMSG_LOSTFOCUS:
			if( !lpEditBox->bReadOnly ){
				atCONTROL_StopControlTimer( hControl, 0 );
				atUIAPI_IME_Flush( lpEditBox->hIME, atFALSE );
				atUIAPI_IME_Stop( lpEditBox->hIME );
			}
			break;			
	}

	return r;
}



//=============================================================================


atUI_HIME atEDITBOX_GetIME( atHCONTROL hControl )
{
	atLPEDITBOX lpEditBox = (atLPEDITBOX )atCONTROL_GetDataPtr( hControl );
	if( lpEditBox == atNULL ) return atNULL;

	return lpEditBox->hIME;
}

atLPTSTR atEDITBOX_GetText( atHCONTROL hControl, atLPTSTR szDesText )
{
	atLPEDITBOX lpEditBox = (atLPEDITBOX )atCONTROL_GetDataPtr( hControl );
	if( lpEditBox == atNULL ) return atNULL;

	if( szDesText ){
		UI_STRCPY( szDesText, lpEditBox->szText );
		return szDesText;
	} else {
		return lpEditBox->szText;
	}
}

atLPTSTR atEDITBOX_GetFullText( atHCONTROL hControl, atLPTSTR szDesText )
{
	atLPEDITBOX lpEditBox = (atLPEDITBOX )atCONTROL_GetDataPtr( hControl );
	if( lpEditBox == atNULL ) return atNULL;

	if( szDesText == atNULL ) return atNULL;

/*
	atUIAPI_strcpy( szDesText, lpEditBox->szText );
	atUIAPI_strins( szDesText, lpEditBox->szTempText, lpEditBox->nCursorPos, -1 );
*/
	atUIAPI_IME_GetFullCompositeText( lpEditBox->hIME, szDesText );

	return szDesText;
}

void atEDITBOX_SetText( atHCONTROL hControl, atLPTSTR szNewText )
{
	atLPEDITBOX lpEditBox = (atLPEDITBOX )atCONTROL_GetDataPtr( hControl );
	if( lpEditBox == atNULL ) return;

	atUIAPI_IME_SetText( lpEditBox->hIME, szNewText );	
}

int atEDITBOX_GetTextLength( atHCONTROL hControl )
{
	atLPTSTR str = atEDITBOX_GetText(hControl, atNULL);

	return (str != atNULL)? UI_STRLEN(str) : 0;
}

int atEDITBOX_GetFullTextLength( atHCONTROL hControl )
{
	atTCHAR szFullText[256];
	atLPEDITBOX lpEditBox = (atLPEDITBOX )atCONTROL_GetDataPtr( hControl );
	atLPTSTR str = atEDITBOX_GetFullText(hControl, szFullText);

	return (str != atNULL)? UI_STRLEN(str) : 0;
}

atU32 atEDITBOX_GetAttr(atHCONTROL hControl )
{
	atLPEDITBOX lpEditBox = (atLPEDITBOX )atCONTROL_GetDataPtr( hControl );
	if( lpEditBox == atNULL ) return 0;

	return lpEditBox->uAttr;
}

void atEDITBOX_SetAttr(atHCONTROL hControl, atU32 uAttr )
{
	atLPEDITBOX lpEditBox = (atLPEDITBOX )atCONTROL_GetDataPtr( hControl );
	if( lpEditBox == atNULL ) return;

	lpEditBox->uAttr = uAttr;
}

void atEDITBOX_SetAlign(atHCONTROL hControl, int align )
{
	atLPEDITBOX lpEditBox = (atLPEDITBOX )atCONTROL_GetDataPtr( hControl );
	if( lpEditBox == atNULL ) return;

	lpEditBox->uAttr &= ~atEDITBOXATTR_ALIGN_MASK;
	lpEditBox->uAttr |= align;
}

atBOOL atEDITBOX_IsReadOnly( atHCONTROL hControl )
{
	atLPEDITBOX lpEditBox = (atLPEDITBOX )atCONTROL_GetDataPtr( hControl );
	if( lpEditBox == atNULL ) return atTRUE;

	return lpEditBox->bReadOnly;
}

void atEDITBOX_ReadOnly( atHCONTROL hControl, atBOOL bReadOnly )
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
