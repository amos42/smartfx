//
// MenuWnd.c
//
///////////////////////////////////////////////////////////////////////

#include "A_UIENG.h"
#include "UI_Util.h"
#include "CMP_RadioButtonEx.h"


#define CONTROL_STRING_GAP		3
#define FONT_HEIGHT_GAP			1


// Grouping by nGroupID
typedef struct _MYRADIOBUTTON {
	int			nGroupID;		// Group By nGroupID
	atHBITMAP		lpNormalImage;
	atHBITMAP		lpSelectImage;
	atTCHAR		szText[64];
	atBOOL		bSelected;

} MYRADIOBUTTON, * LPMYRADIOBUTTON;


static int RadioButtonProc( atHCONTROL hControl, int nMsg, long lParam1, long lParam2 );


atHCONTROL atRADIOBUTTON_EXT_Create( int nIndex, int nX, int nY, int nWidth, int nHeight, 
							int nGroupID, atLPTSTR szText, 
							atHBITMAP lpNormalImage, atHBITMAP lpSelectImage)
{
	atLPCONTROL		lpControl;
	LPMYRADIOBUTTON	lpRadioBtn;
	atHBITMAP				lpBM;
	int					nStrWidth;
	atHCONTROL		hControl;
	
//	lpControl	= atCONTROL_CreateBaseControl( hParentWnd, nIndex, nX, nY, nWidth, nHeight, CTL_PARENTTYPE_WINDOW, atNULL );
	hControl = atCONTROL_Create( nIndex, nX, nY, nWidth, nHeight, CTL_PARENTTYPE_WINDOW, 0, atNULL, atNULL );
	lpControl = atCONTROL_GetPtr(hControl);
	
//	lpControl->nControlTypeID	= CTLID_RADIOBUTTON;

	lpRadioBtn = (LPMYRADIOBUTTON)UI_MEMALLOC(sizeof(MYRADIOBUTTON));
	if ( lpRadioBtn == atNULL )
	{
		UI_MEMFREE(lpControl);
		return atNULL;
	}
	atUIAPI_memset(lpRadioBtn, 0, sizeof(MYRADIOBUTTON));

	UI_STRNCPY(lpRadioBtn->szText, szText, sizeof(lpRadioBtn->szText)/sizeof(atTCHAR)-1);
	lpRadioBtn->nGroupID		= nGroupID;

//	lpRadioBtn->lpNormalImage	= CreateImageEx(g_lpImageMG, szNormalImage, atNULL);
//	lpRadioBtn->lpSelectImage	= CreateImageEx(g_lpImageMG, szSelectImage, atNULL);
	lpRadioBtn->lpNormalImage	= lpNormalImage;
	lpRadioBtn->lpSelectImage	= lpSelectImage;

	lpBM = lpRadioBtn->lpNormalImage;

	nStrWidth = atUIAPI_GetStringWidth(atCONTROL_GetFont(hControl), szText, -1);

	if ( nWidth == 0 )
	{
		lpControl->Base.rect.nEndX		= nX + 2 + atUIAPI_GetBitmapWidth(lpBM) + CONTROL_STRING_GAP + nStrWidth + 3;
	}
	if ( nHeight == 0 )
	{
		lpControl->Base.rect.nEndY		= nY + atUIAPI_GetBitmapHeight(lpBM) + 2;
	}

	lpControl->Base.nObjDataSize = sizeof(MYRADIOBUTTON);
	lpControl->Base.lpObjData = lpRadioBtn;
	lpControl->Base.fnObjProc = RadioButtonProc;

//	return atCONTROL_GetHandle(lpControl);
	return hControl;
}

static void atRADIOBUTTON_Destroy( atHCONTROL hControl )
{
	atLPCONTROL 	lpControl = atCONTROL_GetPtr(hControl);
	LPMYRADIOBUTTON	lpRadioButton = atCONTROL_GetDataPtr( hControl );

//	ReleaseImage(g_lpImageMG, &lpRadioButton->lpNormalImage);
//	ReleaseImage(g_lpImageMG, &lpRadioButton->lpSelectImage);
}

static void atRADIOBUTTON_Draw( atHGDC hCanvas, atHCONTROL hControl, atBOOL bFocused )
{
	atLPCONTROL lpControl = atCONTROL_GetPtr(hControl);
	LPMYRADIOBUTTON	lpRadioButton;
	int				/*nAscent, */nHeight;
	atHBITMAP			lpBM;
	atCOLOR bg_color, color;
//	atHFONT fnt;

	if ( lpControl->Base.bShow == atFALSE ) return;

	lpRadioButton = (LPMYRADIOBUTTON)atCONTROL_GetDataPtr( hControl );

	nHeight = atUIAPI_GetFontHeight( atCONTROL_GetFont(hControl) );

	if ( lpRadioButton->bSelected  ) {
		lpBM = lpRadioButton->lpSelectImage;
		bg_color = atUIAPI_GetRGB(248, 111, 0);
		color = UI_COLOR_WHITE;
	} else {
		bg_color = UI_COLOR_WHITE;
		lpBM = lpRadioButton->lpNormalImage;
		color = UI_COLOR_BLACK;
	}
	atUIAPI_FillRect(hCanvas, 0, 0, RECT_WIDTH(lpControl->Base.rect), RECT_HEIGHT(lpControl->Base.rect), bg_color );
	if( lpBM ){
		atUIAPI_DrawImage(hCanvas, 2, 1, lpBM);
	} else {
		int ratio = (RECT_HEIGHT(lpControl->Base.rect)-2)/2;
		atUIAPI_DrawCircle(hCanvas, 2+ratio, 1+ratio, ratio, UI_COLOR_BLACK, 1);
		if ( lpRadioButton->bSelected  )
		  atUIAPI_FillCircle(hCanvas, 2+ratio, 1+ratio, 5, UI_COLOR_BLACK);
	}

	atUIAPI_DrawString(hCanvas, 2 + atUIAPI_GetBitmapWidth(lpBM) + CONTROL_STRING_GAP, 
		/*nAscent +*/ ABS(atUIAPI_GetBitmapHeight(lpBM) - nHeight)/2 + FONT_HEIGHT_GAP, 
		lpRadioButton->szText, color);
	
//	atUIAPI_SetFont(hCanvas, fnt);
}


static int RadioButtonProc( atHCONTROL hControl, int nMsg, long lParam1, long lParam2 )
{
	atLPCONTROL lpControl = atCONTROL_GetPtr(hControl);
	LPMYRADIOBUTTON	lpRadioButton = (LPMYRADIOBUTTON)atCONTROL_GetDataPtr( hControl );
	atHWINDOW hWnd = lpControl->Base.hParentObj;
	atLPWINDOW lpWnd = atWINDOW_GetPtr(hWnd);
//	int i, cnt, iSel;
	int r = 0;


	switch( nMsg )
	{
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
			switch( lParam1 )
			{
/*			
				case atVKEY_UP:
				case atVKEY_LEFT:
					cnt = atWINDOW_GetControlCount( hWnd );	
					iSel = -1;
					for(i = 0; i < cnt; i++){
						if(atRADIOBUTTON_EXT_IsSelected(atWINDOW_GetControl(hWnd, i)))
						{
							iSel = i - 1;
							if(iSel < 0) iSel = cnt - 1;
							break;
						}
					}
					
					if( iSel >= 0 )
					{
						atRADIOBUTTON_EXT_SetSelect( atWINDOW_GetControl(hWnd, iSel), atTRUE);
						r = 1;
					}
					break;
				case atVKEY_DOWN:
				case atVKEY_RIGHT:
					cnt = atWINDOW_GetControlCount( hWnd );	
					iSel = -1;
					for(i = 0; i < cnt; i++)
					{
						if(atRADIOBUTTON_EXT_IsSelected(atWINDOW_GetControl(hWnd, i)))
						{
							iSel = i + 1;
							if(iSel >= cnt) iSel = 0;
							break;
						}
					}
					
					if( iSel >= 0 )
					{
						atRADIOBUTTON_EXT_SetSelect( atWINDOW_GetControl(hWnd, iSel), atTRUE);
						r = 1;
					}
					break;
*/					
				case atVKEY_SELECT:
					atRADIOBUTTON_EXT_SetSelect( hControl, atTRUE );
					atCONTROL_Refresh( hControl );
					atCONTROL_PostMessageToParent( hControl, WNDMSG_COMMAND, (long)lpControl->Base.nID, 0 );
					r = 1;
					break;
			}
			break;
		case CTLMSG_MOUSECLICK:
			atCONTROL_ProcessControlProc( hControl, CTLMSG_KEYDOWN, atVKEY_SELECT, 0 );
			r = 1;
			break;
	}

	return r;
}


//==================================================================

atBOOL atRADIOBUTTON_EXT_IsSelected( atHCONTROL hControl )
{
	atLPCONTROL 	lpControl = atCONTROL_GetPtr(hControl);
	LPMYRADIOBUTTON	lpRadioButton = atCONTROL_GetDataPtr( hControl );

	if ( lpControl == atNULL )
		return atFALSE;

	return lpRadioButton->bSelected;	
}

void atRADIOBUTTON_EXT_SetSelect( atHCONTROL hControl, atBOOL bEnable )
{
	atLPCONTROL 	lpControl = atCONTROL_GetPtr(hControl);
	LPMYRADIOBUTTON	lpRadioButton = atCONTROL_GetDataPtr( hControl );

	if ( lpControl == atNULL )
		return;

	lpRadioButton->bSelected = bEnable;	
}

