//
// MenuWnd.c
//
///////////////////////////////////////////////////////////////////////

#include "A_UIENG.h"
#include "UI_Util.h"
#include "CMP_RadioButton.h"


#define CONTROL_STRING_GAP		3
#define FONT_HEIGHT_GAP			1


typedef struct _atRADIOBUTTON {
	int			nGroupID;		// Group By nGroupID
	atHBITMAP		lpNormalImage;
	atHBITMAP		lpSelectImage;
	atTCHAR		szText[64];
	atBOOL		bSelected;
} atRADIOBUTTON, * atLPRADIOBUTTON;


static int RadioButtonProc( atHCONTROL hControl, int nMsg, long lParam1, long lParam2 );


atHCONTROL atRADIOBUTTON_Create( int nIndex, int nX, int nY, int nWidth, int nHeight, 
							int nGroupID, atLPTSTR szText )
{
	atLPCONTROL		lpControl;
	atLPRADIOBUTTON	lpRadioBtn;
	int					nStrWidth;
	atHCONTROL		hControl;
	
//	lpControl	= atCONTROL_CreateBaseControl( hParentWnd, nIndex, nX, nY, nWidth, nHeight, CTL_PARENTTYPE_WINDOW, atNULL );
	hControl = atCONTROL_Create( nIndex, nX, nY, nWidth, nHeight, CTL_PARENTTYPE_WINDOW, 0, atNULL, atNULL );
	if( hControl == atNULL ) return atNULL;

	lpControl = atCONTROL_GetPtr(hControl);
	
//	lpControl->nControlTypeID	= CTLID_RADIOBUTTON;

	lpRadioBtn = (atLPRADIOBUTTON)UI_MEMALLOC(sizeof(atRADIOBUTTON));
	if ( lpRadioBtn == atNULL ){
		UI_MEMFREE(lpControl);
		return atNULL;
	}
	atUIAPI_memset(lpRadioBtn, 0, sizeof(atRADIOBUTTON));

	UI_STRNCPY(lpRadioBtn->szText, szText, sizeof(lpRadioBtn->szText)/sizeof(atTCHAR)-1);
	lpRadioBtn->nGroupID		= nGroupID;

	nStrWidth = atUIAPI_GetStringWidth(atCONTROL_GetFont(hControl), szText, -1);

	if( nWidth == 0 ){
		lpControl->Base.rect.nEndX		= nX + 2 + nHeight + CONTROL_STRING_GAP + nStrWidth + 3;
	}
	if( nHeight == 0 ){
		lpControl->Base.rect.nEndY		= nY + nHeight - 1;
	}

	lpControl->Base.nObjDataSize = sizeof(atRADIOBUTTON);
	lpControl->Base.lpObjData = lpRadioBtn;
	lpControl->Base.fnObjProc = RadioButtonProc;

//	return atCONTROL_GetHandle(lpControl);
	return hControl;
}

void atRADIOBUTTON_Destroy( atHCONTROL hControl )
{
	atLPCONTROL 	lpControl = atCONTROL_GetPtr(hControl);
	atLPRADIOBUTTON	lpRadioButton = atCONTROL_GetDataPtr( hControl );

//	ReleaseImage(g_lpImageMG, &lpRadioButton->lpNormalImage);
//	ReleaseImage(g_lpImageMG, &lpRadioButton->lpSelectImage);
}

void atRADIOBUTTON_Draw( atHGDC hCanvas, atHCONTROL hControl, atBOOL bFocused )
{
	atLPCONTROL lpControl = atCONTROL_GetPtr(hControl);
	atLPRADIOBUTTON	lpRadioButton;
	atCOLOR fr_color, bg_color, txt_color;
	atREGION rgn;
	int ratio;
	int font_height;

	if( atCONTROL_IsVisible(hControl) == atFALSE ) return;

	lpRadioButton = (atLPRADIOBUTTON)atCONTROL_GetDataPtr( hControl );

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

//	atUIAPI_FillRect( hCanvas, 0, 0, RECT_WIDTH(lpControl->Base.rect), RECT_HEIGHT(lpControl->Base.rect), bg_color );

	if( bFocused )
	  atUIAPI_FillRect( hCanvas, 0, 0, rgn.nHeight, rgn.nHeight, bg_color );

	ratio = (rgn.nHeight-2)/2;
	atUIAPI_DrawCircle(hCanvas, 2+ratio, 1+ratio, ratio, fr_color, 1);
	if( lpRadioButton->bSelected ){
	  atUIAPI_FillCircle(hCanvas, 2+ratio, 1+ratio, 5, fr_color);
    }

	font_height = atUIAPI_GetFontHeight( atCONTROL_GetFont(hControl) );

	atUIAPI_DrawStringAlign( hCanvas, rgn.nHeight + 2, rgn.nHeight/2,
							 lpRadioButton->szText, ALIGN_LEFT_MIDDLE, txt_color );
}


static int RadioButtonProc( atHCONTROL hControl, int nMsg, long lParam1, long lParam2 )
{
	atLPCONTROL lpControl = atCONTROL_GetPtr(hControl);
	atLPRADIOBUTTON	lpRadioButton = (atLPRADIOBUTTON)atCONTROL_GetDataPtr( hControl );
	atHWINDOW hWnd = lpControl->Base.hParentObj;
	atLPWINDOW lpWnd = atWINDOW_GetPtr(hWnd);
	int i, cnt, iSel;
	int r = 0;


	switch( nMsg ){
		case CTLMSG_INIT:
			break;
		case CTLMSG_RELEASE:
			atRADIOBUTTON_Destroy( hControl );
			r = 1;
			break;
		case CTLMSG_PAINT:
			atRADIOBUTTON_Draw( atCONTROL_GetGDC(hControl), hControl, (atBOOL)lParam2 );
			r = 1;
			break;
		case CTLMSG_KEYDOWN:
			switch( lParam1 ){
				case atVKEY_UP:
				case atVKEY_LEFT:
					cnt = atWINDOW_GetControlCount( hWnd );	
					iSel = -1;
					for(i = 0; i < cnt; i++){
						if(atRADIOBUTTON_IsSelected(atWINDOW_GetControl(hWnd, i))){
							iSel = i - 1;
							if(iSel < 0) iSel = cnt - 1;
							break;
						}
					}
					
					if( iSel >= 0 ){
						atRADIOBUTTON_SetSelect( atWINDOW_GetControl(hWnd, iSel), atTRUE);
						r = 1;
					}
					break;
				case atVKEY_DOWN:
				case atVKEY_RIGHT:
					cnt = atWINDOW_GetControlCount( hWnd );	
					iSel = -1;
					for(i = 0; i < cnt; i++){
						if(atRADIOBUTTON_IsSelected(atWINDOW_GetControl(hWnd, i))){
							iSel = i + 1;
							if(iSel >= cnt) iSel = 0;
							break;
						}
					}
					
					if( iSel >= 0 ){
						atRADIOBUTTON_SetSelect( atWINDOW_GetControl(hWnd, iSel), atTRUE);
						r = 1;
					}
					break;
				case atVKEY_SELECT:
					atCONTROL_PostMessageToParent( hControl, WNDMSG_COMMAND, (long)lpControl->Base.nID, 0 );
					r = 1;
					break;
			}
			break;
		case CTLMSG_MOUSEUP:
			atCONTROL_ProcessControlProc( hControl, CTLMSG_KEYDOWN, atVKEY_SELECT, 0 );
			r = 1;
			break;
	}

	return r;
}


//==================================================================

atBOOL atRADIOBUTTON_IsSelected( atHCONTROL hControl )
{
	atLPCONTROL 	lpControl = atCONTROL_GetPtr(hControl);
	atLPRADIOBUTTON	lpRadioButton = atCONTROL_GetDataPtr( hControl );

	if ( lpControl == atNULL )
		return atFALSE;

	return lpRadioButton->bSelected;	
}

void atRADIOBUTTON_SetSelect( atHCONTROL hControl, atBOOL bEnable )
{
	atLPCONTROL 	lpControl = atCONTROL_GetPtr(hControl);
	atLPRADIOBUTTON	lpRadioButton = atCONTROL_GetDataPtr( hControl );

	if ( lpControl == atNULL )
		return;

	lpRadioButton->bSelected = bEnable;	
}

