//
// CheckBox.c
//
///////////////////////////////////////////////////////////////////////

#include "A_UIENG.h"
#include "CMP_CheckBoxEx.h"
#include <string.h>


#define CONTROL_STRING_GAP		3
#define FONT_HEIGHT_GAP			1


typedef struct _atCHECKBOX {
	atHBITMAP		lpNormalImage;
	atHBITMAP		lpCheckImage;
	atTCHAR		szText[64];
	atBOOL		bChecked;
} atCHECKBOX, * atLPCHECKBOX;


static int CheckBoxProc( atHCONTROL hControl, int nMsg, long lParam1, long lParam2 )
{
	atLPCONTROL lpControl = atCONTROL_GetPtr(hControl);
	atLPCHECKBOX lpCheckBox = (atLPCHECKBOX)atCONTROL_GetDataPtr(hControl);
	int r = 0;
	
	switch( nMsg )
	{
		case CTLMSG_INIT:
			atCONTROL_SetSoftKey(hControl, atNULL, atNULL, _AT("Check"));
			break;
		case CTLMSG_RELEASE:
			atCHECKBOX_EXT_Destroy( hControl );
			r = 1;
			break;
		case CTLMSG_PAINT:
			atCHECKBOX_EXT_Draw( atCONTROL_GetGDC(hControl), hControl, (atBOOL)lParam2 );
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
//			RestoreSoftKey( lpControl->Base.hParentObj );
			r = 1;
			break;
		case CTLMSG_KEYDOWN:
			switch( lParam1 )
			{
				case atVKEY_UP:
				case atVKEY_DOWN:
				case atVKEY_LEFT:
				case atVKEY_RIGHT:
//					return atCONTROL_DefControlProc( hControl, nMsg, lParam1, lParam2 );
					break;
//				case atVKEY_SELECT:
//					atCONTROL_PostMessageToParent( hControl, WNDMSG_COMMAND, (long)lpControl->nIndex, 0 );
//					break;
				case atVKEY_SELECT:
				case atVKEY_SOFT2:
					lpCheckBox->bChecked = !lpCheckBox->bChecked;
					//if( atCONTROL_IsFocus(hControl) ){
						if( lpCheckBox->bChecked )
							atCONTROL_SetSoftKey(hControl, atNULL, atNULL, _AT("UnChk"));
						else
							atCONTROL_SetSoftKey(hControl, atNULL, atNULL, _AT("Check"));
						atSOFTKEY_RefreshSoftKey();
					//}
					atCONTROL_Refresh(hControl);
					atCONTROL_PostMessageToParent( hControl, WNDMSG_CHANGE, (long)lpControl->Base.nID, lpCheckBox->bChecked );
					r = 1;
					break;
				default:
//					return atCONTROL_DefControlProc( hControl, nMsg, lParam1, lParam2 );
					break;
			}
			break;
			
		case CTLMSG_MOUSEDOWN:
			atCONTROL_Refresh(hControl);
			break;
		case CTLMSG_MOUSEUP:
			atCONTROL_Refresh(hControl);
			break;
			
		case CTLMSG_MOUSECLICK:
			atCONTROL_ProcessControlProc( hControl, CTLMSG_KEYDOWN, atVKEY_SELECT, 0 );
			r = 1;
			break;
		default :
//			return atCONTROL_DefControlProc( hControl, nMsg, lParam1, lParam2 );
			break;
	}

	return r;
}

atHCONTROL atCHECKBOX_EXT_Create( int nIndex, int nX, int nY, int nWidth, int nHeight, atLPTSTR szText, atBOOL bChecked,
						 				atHBITMAP lpNormalImage, atHBITMAP lpCheckImage)
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
		atHBITMAP		lpBM;

//		lpControl->nControlTypeID	= CTLID_CHECKBOX;

		lpCheckBox = (atLPCHECKBOX)atCONTROL_GetDataPtr(hControl);

		atUIAPI_memset(lpCheckBox, 0, sizeof(atCHECKBOX));
		UI_STRNCPY(lpCheckBox->szText, szText, 64-1);

		lpCheckBox->bChecked = bChecked;
//		lpCheckBox->lpNormalImage	= CreateImageEx(g_lpImageMG, szNormalImage, atNULL);
//		lpCheckBox->lpCheckImage	= CreateImageEx(g_lpImageMG, szCheckImage, atNULL);
		lpCheckBox->lpNormalImage	= lpNormalImage;
		lpCheckBox->lpCheckImage	= lpCheckImage;

		lpBM = lpCheckBox->lpNormalImage;

		nStrWidth = atUIAPI_GetStringWidth( atCONTROL_GetFont(hControl), szText, -1 );

		if ( nWidth <= 0 ){
			lpControl->Base.rect.nEndX		= nX + atUIAPI_GetBitmapWidth(lpBM) + CONTROL_STRING_GAP + nStrWidth + 2;
		}
		if ( nHeight <= 0 ){
			lpControl->Base.rect.nEndY		= nY + atUIAPI_GetBitmapHeight(lpBM) + 2;
		}
	}

//	return atCONTROL_GetHandle(lpControl);
	return hControl;
}

void atCHECKBOX_EXT_Destroy( atHCONTROL hControl )
{
//	atLPCONTROL 	lpControl = atCONTROL_GetPtr(hControl);
//	atLPCHECKBOX	lpCheckBox = atCONTROL_GetDataPtr( hControl );

//	ReleaseImage(g_lpImageMG, &lpCheckBox->lpNormalImage);
//	ReleaseImage(g_lpImageMG, &lpCheckBox->lpCheckImage);
}

void atCHECKBOX_EXT_Draw( atHGDC hCanvas, atHCONTROL hControl, atBOOL bFocused )
{
	atLPCONTROL lpControl = atCONTROL_GetPtr(hControl);
	atLPCHECKBOX	lpCheckBox;
	int				nHeight;
	atHBITMAP			lpBM;
	atRECT	rect;
	long color;

	if ( lpControl->Base.bShow == atFALSE )	{
		return;
	}

//	atCONTROL_GetScreenRect( hControl, &rect );
rect = lpControl->Base.rect;
	
	lpCheckBox = (atLPCHECKBOX)atCONTROL_GetDataPtr( hControl );

	nHeight = atUIAPI_GetFontHeight( atCONTROL_GetFont(hControl) );

	if ( bFocused ){
		atUIAPI_FillRect( hCanvas, 0, 0, RECT_WIDTH(rect), RECT_HEIGHT(rect), atUIAPI_GetRGB(248, 111, 0));
	}else{
//		atUIAPI_FillRect( hCanvas, rect.nStartX, rect.nStartY, RECT_WIDTH(rect), RECT_HEIGHT(rect), atUIAPI_GetRGB(255, 255, 255));
	}

	lpBM = (lpCheckBox->bChecked)? lpCheckBox->lpCheckImage : lpCheckBox->lpNormalImage;
	atUIAPI_DrawImage( hCanvas, 0, 0, lpBM );
	
	if ( bFocused )
	{
		color = UI_COLOR_WHITE;
	}
	else
	{
		color = UI_COLOR_BLACK;
	}
	
	atUIAPI_DrawStringAlign( hCanvas, 2 + atUIAPI_GetBitmapWidth(lpBM) + CONTROL_STRING_GAP, 
						nHeight/2,
						lpCheckBox->szText, ALIGN_LEFT_MIDDLE, color);

}

void	atCHECKBOX_EXT_SetCheck(atHCONTROL hControl, atBOOL bCheck)
{
	atLPCONTROL lpControl = atCONTROL_GetPtr(hControl);
	atLPCHECKBOX		lpCheckBox	= (atLPCHECKBOX)atCONTROL_GetDataPtr( hControl );
		
	lpCheckBox->bChecked = bCheck;
}

atBOOL	atCHECKBOX_EXT_IsChecked(atHCONTROL hControl)
{
	atLPCONTROL lpControl = atCONTROL_GetPtr(hControl);
	atLPCHECKBOX		lpCheckBox	= (atLPCHECKBOX)atCONTROL_GetDataPtr( hControl );
		
	return lpCheckBox->bChecked;
}

