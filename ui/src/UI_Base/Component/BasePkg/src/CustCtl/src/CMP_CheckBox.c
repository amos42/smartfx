//
// CheckBox.c
//
///////////////////////////////////////////////////////////////////////

#include "A_UIENG.h"
#include "CMP_CheckBox.h"
#include <string.h>


#define CONTROL_STRING_GAP		3
#define FONT_HEIGHT_GAP			1


typedef struct _MYCHECKBOX {
	atTCHAR		szText[64];
	atBOOL		bChecked;
} atCHECKBOX, * atLPCHECKBOX;

static int CheckBoxProc( atHCONTROL hControl, int nMsg, long lParam1, long lParam2 );


atHCONTROL atCHECKBOX_Create( int nIndex, int nX, int nY, int nWidth, int nHeight, atLPTSTR szText, atBOOL bChecked )
{
//	atHGDC hCanvas = atWINDOW_GetClientGDC(hParentWnd);
	atHCONTROL		hControl;
	atLPCONTROL		lpControl;

	hControl = atCONTROL_Create( nIndex, nX, nY, nWidth, nHeight, CTL_PARENTTYPE_WINDOW, sizeof(atCHECKBOX), CheckBoxProc, atNULL );
	if( hControl == atNULL ) return atNULL;

	lpControl = atCONTROL_GetPtr(hControl);
	
	if ( lpControl != atNULL ){
		atLPCHECKBOX	lpCheckBox;
		int				nStrWidth;

//		lpControl->nControlTypeID	= CTLID_CHECKBOX;

		lpCheckBox = (atLPCHECKBOX)atCONTROL_GetDataPtr(hControl);

		atUIAPI_memset(lpCheckBox, 0, sizeof(atCHECKBOX));
		lpCheckBox->bChecked = bChecked;
		UI_STRNCPY(lpCheckBox->szText, szText, 64-1);

		nStrWidth = atUIAPI_GetStringWidth( atCONTROL_GetFont(hControl), szText, -1 );

		if ( nWidth <= 0 ){
			lpControl->Base.rect.nEndX		= nX + 18 + CONTROL_STRING_GAP + nStrWidth + 2;
		}
		if ( nHeight <= 0 ){
			lpControl->Base.rect.nEndY		= nY + 18 + 2;
		}
	}

	return hControl;
}

static void atCHECKBOX_OnRelease( atHCONTROL hControl )
{
//	atLPCONTROL 	lpControl = atCONTROL_GetPtr(hControl);
//	atLPCHECKBOX	lpCheckBox = atCONTROL_GetDataPtr( hControl );

}

static void atCHECKBOX_OnDraw( atHGDC hCanvas, atHCONTROL hControl, atBOOL bFocused )
{
	atLPCONTROL lpControl = atCONTROL_GetPtr(hControl);
	atLPCHECKBOX	lpCheckBox;
	atCOLOR fr_color, bg_color, txt_color;
	atREGION rgn;
	int font_height;

	if( atCONTROL_IsVisible(hControl) == atFALSE ) return;

	lpCheckBox = (atLPCHECKBOX)atCONTROL_GetDataPtr( hControl );

	atCONTROL_GetControlRegion( hControl, &rgn );
		
	if( atCONTROL_IsEnable(hControl) ){
		if( bFocused ){
		    fr_color = lpControl->tControlColor.clFocusedFrColor;
		    bg_color = lpControl->tControlColor.clFocusedBgColor;
			txt_color = lpControl->tControlColor.clNormalFrColor;
		} else {
		    fr_color = lpControl->tControlColor.clNormalFrColor;
		    bg_color = lpControl->tControlColor.clNormalBgColor;
			txt_color = lpControl->tControlColor.clNormalFrColor;
		}
	} else {
	    fr_color = lpControl->tControlColor.clDisableFrColor;
	    bg_color = lpControl->tControlColor.clDisableBgColor;
		txt_color = lpControl->tControlColor.clDisableFrColor;
	}

	if( bFocused )
	  atUIAPI_FillRect( hCanvas, 0, 0, rgn.nHeight, rgn.nHeight, bg_color );

	atUIAPI_DrawRect( hCanvas, 1, 1, rgn.nHeight-2, rgn.nHeight-2, fr_color, 1 );
	if( lpCheckBox->bChecked ){
	  atUIAPI_DrawLine( hCanvas, 1, 1, 1+rgn.nHeight-1-2, 1+rgn.nHeight-1-2, fr_color, 1, atFALSE );
	  atUIAPI_DrawLine( hCanvas, 1, 1+rgn.nHeight-1-2, 1+rgn.nHeight-1-2, 1, fr_color, 1, atFALSE );
    }

	font_height = atUIAPI_GetFontHeight( atCONTROL_GetFont(hControl) );

	atUIAPI_DrawStringAlign( hCanvas, rgn.nHeight + 2, rgn.nHeight/2,
							 lpCheckBox->szText, ALIGN_LEFT_MIDDLE, txt_color );
}

static int CheckBoxProc( atHCONTROL hControl, int nMsg, long lParam1, long lParam2 )
{
	atLPCONTROL lpControl = atCONTROL_GetPtr(hControl);
	atLPCHECKBOX lpCheckBox = (atLPCHECKBOX)atCONTROL_GetDataPtr(hControl);
	int r = 0;
	
	switch( nMsg ){
		case CTLMSG_INIT:
			atCONTROL_SetSoftKey(hControl, atNULL, atNULL, _AT("Check"));
			break;
		case CTLMSG_RELEASE:
			atCHECKBOX_OnRelease( hControl );
			r = 1;
			break;
		case CTLMSG_PAINT:
			atCHECKBOX_OnDraw( atCONTROL_GetGDC(hControl), hControl, (atBOOL)lParam2 );
//			if( lParam1 ) atCONTROL_Flush(hControl);
			r = 1;
			break;
		case CTLMSG_GOTFOCUS:
/*
			if( lpCheckBox->bChecked )
				atCONTROL_SetSoftKey(hControl, atNULL, atNULL, _AT("UnChk"));
			else
				atCONTROL_SetSoftKey(hControl, atNULL, atNULL, _AT("Check"));
			r = 1;
*/
			break;
		case CTLMSG_LOSTFOCUS:
			break;
		case CTLMSG_KEYDOWN:
			switch( lParam1 ){
				case atVKEY_UP:
				case atVKEY_DOWN:
				case atVKEY_LEFT:
				case atVKEY_RIGHT:
//					return atCONTROL_DefControlProc( hControl, nMsg, lParam1, lParam2 );
					break;
//				case atVKEY_SELECT:
//					atCONTROL_PostMessageToParent( hControl, WNDMSG_COMMAND, (long)lpControl->nIndex, 0 );
//					break;
				case atVKEY_SOFT2:
					lpCheckBox->bChecked = !lpCheckBox->bChecked;
					if( lpCheckBox->bChecked )
						atCONTROL_SetSoftKey(hControl, atNULL, atNULL, _AT("UnChk"));
					else
						atCONTROL_SetSoftKey(hControl, atNULL, atNULL, _AT("Check"));
//					atCONTROL_Draw( hControl, atTRUE );
					atSOFTKEY_RefreshSoftKey();
					atCONTROL_Refresh(hControl);
					r = 1;
					break;
				default:
//					return atCONTROL_DefControlProc( hControl, nMsg, lParam1, lParam2 );
					break;
			}
			break;
		case CTLMSG_MOUSEUP:
			atCONTROL_ProcessControlProc( hControl, CTLMSG_KEYDOWN, atVKEY_SOFT2, 0 );
			r = 1;
			break;
		default :
//			return atCONTROL_DefControlProc( hControl, nMsg, lParam1, lParam2 );
			break;
	}

	return r;
}

void	atCHECKBOX_SetCheck(atHCONTROL hControl, atBOOL bCheck)
{
	atLPCONTROL lpControl = atCONTROL_GetPtr(hControl);
	atLPCHECKBOX		lpCheckBox	= (atLPCHECKBOX)atCONTROL_GetDataPtr( hControl );
		
	lpCheckBox->bChecked = bCheck;
}

atBOOL	atCHECKBOX_IsChecked(atHCONTROL hControl)
{
	atLPCONTROL lpControl = atCONTROL_GetPtr(hControl);
	atLPCHECKBOX		lpCheckBox	= (atLPCHECKBOX)atCONTROL_GetDataPtr( hControl );
		
	return lpCheckBox->bChecked;
}

