//
// Button.c
//
///////////////////////////////////////////////////////////////////////

#include "A_UIENG.h"
#include "CMP_ButtonEx.h"
#include <string.h>


typedef struct _atBUTTON {
	atHBITMAP		lpNormalImg_Left, lpNormalImg_Center, lpNormalImg_Right;
	atHBITMAP		lpFocusedImg_Left, lpFocusedImg_Center, lpFocusedImg_Right;
	atHBITMAP		lpSelectedImg_Left, lpSelectedImg_Center, lpSelectedImg_Right;
	atHBITMAP		lpDisabledImg_Left, lpDisabledImg_Center, lpDisabledImg_Right;
	atCOLOR			clSelectedTxtColor;
	atTCHAR		    szText[256];
} atBUTTON, * atLPBUTTON;

static int ButtonProc( atHCONTROL hControl, int nMsg, long lParam1, long lParam2 );


struct _CreateParam {
	atLPTSTR szText;
    atHBITMAP lpNormalImg_Left, lpNormalImg_Center, lpNormalImg_Right;
	atHBITMAP lpFocusedImg_Left, lpFocusedImg_Center, lpFocusedImg_Right;
	atHBITMAP lpSelectedImg_Left, lpSelectedImg_Center, lpSelectedImg_Right;
	atHBITMAP lpDisabledImg_Left, lpDisabledImg_Center, lpDisabledImg_Right;
};



atHCONTROL atBUTTON_EXT_Create( int nIndex, int nX, int nY, int nWidth, int nHeight, 
	                     atLPTSTR szText, 
						 atHBITMAP lpNormalImg_Left, atHBITMAP lpNormalImg_Center, atHBITMAP lpNormalImg_Right,
						 atHBITMAP lpFocusedImg_Left, atHBITMAP lpFocusedImg_Center, atHBITMAP lpFocusedImg_Right,
						 atHBITMAP lpSelectedImg_Left, atHBITMAP lpSelectedImg_Center, atHBITMAP lpSelectedImg_Right,
						 atHBITMAP lpDisabledImg_Left, atHBITMAP lpDisabledImg_Center, atHBITMAP lpDisabledImg_Right )
{
	atHCONTROL hControl;
	struct _CreateParam param;

	param.szText = szText;
	param.lpNormalImg_Left		= lpNormalImg_Left;
	param.lpNormalImg_Center	= lpNormalImg_Center;
	param.lpNormalImg_Right		= lpNormalImg_Right;
	param.lpFocusedImg_Left 	= lpFocusedImg_Left;
	param.lpFocusedImg_Center	= lpFocusedImg_Center;
	param.lpFocusedImg_Right	= lpFocusedImg_Right;
	param.lpSelectedImg_Left	= lpSelectedImg_Left;
	param.lpSelectedImg_Center	= lpSelectedImg_Center;
	param.lpSelectedImg_Right	= lpSelectedImg_Right;
	param.lpDisabledImg_Left 	= lpDisabledImg_Left;
	param.lpDisabledImg_Center	= lpDisabledImg_Center;
	param.lpDisabledImg_Right	= lpDisabledImg_Right;

	hControl = atCONTROL_Create( nIndex, nX, nY, nWidth, nHeight, CTL_PARENTTYPE_WINDOW, sizeof(atBUTTON), ButtonProc, (void *)&param );
	if( hControl == atNULL ) return atNULL;

	{
		atLPCONTROL lpControl = atCONTROL_GetPtr(hControl);
		atLPBUTTON lpButton = atCONTROL_GetDataPtr(hControl);

		if ( nWidth <= 0 )	{
			int nStrWidth = atUIAPI_GetStringWidth( atCONTROL_GetFont(hControl), szText, -1);
			lpControl->Base.rect.nEndX		= nX + nStrWidth + 8*2;
		}
		if ( nHeight <= 0 ){
			atHBITMAP lpBM = lpButton->lpNormalImg_Center;
			if( lpBM )
			  lpControl->Base.rect.nEndY		= nY + atUIAPI_GetBitmapHeight(lpBM) - 1;
			else
			  lpControl->Base.rect.nEndY		= nY + 18 - 1;
		}
	}

	return hControl;
}


static void atBUTTON_OnInit( atHCONTROL hControl, struct _CreateParam *param )
{
	atLPBUTTON	lpButton = (atLPBUTTON)atCONTROL_GetDataPtr(hControl);
	if ( lpButton == atNULL ) return;
	
	UI_STRNCPY(lpButton->szText, param->szText, 256-1);
	lpButton->szText[256-1] = _ATC('\0');

	lpButton->lpNormalImg_Left		= param->lpNormalImg_Left;
	lpButton->lpNormalImg_Center	= param->lpNormalImg_Center;
	lpButton->lpNormalImg_Right 	= param->lpNormalImg_Right;
	lpButton->lpFocusedImg_Left 	= param->lpFocusedImg_Left;
	lpButton->lpFocusedImg_Center	= param->lpFocusedImg_Center;
	lpButton->lpFocusedImg_Right	= param->lpFocusedImg_Right;
	lpButton->lpSelectedImg_Left	= param->lpSelectedImg_Left;
	lpButton->lpSelectedImg_Center	= param->lpSelectedImg_Center;
	lpButton->lpSelectedImg_Right	= param->lpSelectedImg_Right;
	lpButton->lpDisabledImg_Left 	= param->lpDisabledImg_Left;
	lpButton->lpDisabledImg_Center	= param->lpDisabledImg_Center;
	lpButton->lpDisabledImg_Right	= param->lpDisabledImg_Right;
	lpButton->clSelectedTxtColor	= atUI_COLOR_BLACK;

	//atCONTROL_SetSoftKey( hControl, atNULL, _AT("OK"), atNULL );
}

static void atBUTTON_OnDestroy( atHCONTROL hControl )
{
	atLPCONTROL 	lpControl = atCONTROL_GetPtr(hControl);
	atLPBUTTON	lpButton = (atLPBUTTON)atCONTROL_GetDataPtr(hControl);
}

static void atBUTTON_OnDraw( atHGDC hCanvas, atHCONTROL hControl, atBOOL bFocused )
{
	atLPCONTROL lpControl = atCONTROL_GetPtr(hControl);
	atLPBUTTON	lpButton;
	atHBITMAP	lpBM_L, lpBM_C, lpBM_R;
	atRECT rect;
	atCOLOR txt_color;
	atTCHAR szBuff[256];

	if ( atCONTROL_IsVisible(hControl) == atFALSE ) return;

	lpButton = (atLPBUTTON)atCONTROL_GetDataPtr( hControl );

	atCONTROL_GetControlRect( hControl, &rect );
		
	if( atCONTROL_IsEnable(hControl) ){
		if( lpControl->Base.bMouseDown )
		{
			lpBM_L = lpButton->lpSelectedImg_Left;
			lpBM_C = lpButton->lpSelectedImg_Center;
			lpBM_R = lpButton->lpSelectedImg_Right;
			txt_color = lpButton->clSelectedTxtColor;
		}
		else if( bFocused ){
			lpBM_L = lpButton->lpFocusedImg_Left;
			lpBM_C = lpButton->lpFocusedImg_Center;
			lpBM_R = lpButton->lpFocusedImg_Right; 
			txt_color = lpControl->tControlColor.clFocusedTxtColor;
		} else {
			lpBM_L = lpButton->lpNormalImg_Left;
			lpBM_C = lpButton->lpNormalImg_Center;
			lpBM_R = lpButton->lpNormalImg_Right; 
			txt_color = lpControl->tControlColor.clNormalTxtColor;
		}
	} else {
		lpBM_L = lpButton->lpDisabledImg_Left;
		lpBM_C = lpButton->lpDisabledImg_Center;
		lpBM_R = lpButton->lpDisabledImg_Right; 
		txt_color = lpControl->tControlColor.clDisableTxtColor;
	}

	if( !lpBM_L || !lpBM_C || !lpBM_R )
	{
		lpBM_L = lpButton->lpNormalImg_Left;
		lpBM_C = lpButton->lpNormalImg_Center;
		lpBM_R = lpButton->lpNormalImg_Right; 
		txt_color = lpControl->tControlColor.clNormalTxtColor;
	}

	if( lpBM_L && lpBM_C && lpBM_R )
	    atUIAPI_DrawClient1LineStretch( hCanvas, 0, 0, RECT_WIDTH(rect), RECT_HEIGHT(rect), lpBM_L, lpBM_C, lpBM_R );
	else {
		if( atCONTROL_IsEnable(hControl) )
			atUIAPI_FillRect(hCanvas, 0, 0, RECT_WIDTH(rect), RECT_HEIGHT(rect), atUIAPI_GetRGB(127,127,127) );
		else
			atUIAPI_FillRect(hCanvas, 0, 0, RECT_WIDTH(rect), RECT_HEIGHT(rect), (bFocused)? atUIAPI_GetRGB(0, 230, 0) : atUIAPI_GetRGB(255, 128, 0) );
	}

   	myStringCut( hCanvas, lpButton->szText, szBuff, RECT_WIDTH(rect), atTRUE);	
	atUIAPI_DrawStringAlign(hCanvas, RECT_WIDTH(rect)/2, RECT_HEIGHT(rect)/2, szBuff, ALIGN_CENTER_MIDDLE, txt_color);
	
//	atUIAPI_DrawStringAlign( hCanvas, 0 + RECT_WIDTH(rect)/2, 0 + RECT_HEIGHT(rect)/2, lpButton->szText, ALIGN_CENTER_MIDDLE, txt_color);
}


static int ButtonProc( atHCONTROL hControl, int nMsg, long lParam1, long lParam2 )
{
	atLPCONTROL lpControl = atCONTROL_GetPtr(hControl);
//	int x=0, y=0;
	int r = 0;

	switch( nMsg )
	{
		case CTLMSG_INIT:
			atBUTTON_OnInit( hControl, (struct _CreateParam *)lParam1 );
			break;
		case CTLMSG_RELEASE:
			atBUTTON_OnDestroy( hControl );
			r = 1;
			break;
		case CTLMSG_PAINT:
			atBUTTON_OnDraw( atCONTROL_GetGDC(hControl), hControl, (atBOOL)lParam2 );
			r = 1;
			break;
		case CTLMSG_KEYDOWN:
			switch( lParam1 )
			{
				case atVKEY_UP:
				case atVKEY_DOWN:
				case atVKEY_LEFT:
				case atVKEY_RIGHT:
				case atVKEY_FF:
				case atVKEY_REW:
					return atCONTROL_DefControlProc( hControl, nMsg, lParam1, lParam2 );
					break;
				case atVKEY_SELECT:
					atCONTROL_PostMessageToParent( hControl, WNDMSG_COMMAND, (long)lpControl->Base.nID, 0 );
					r = 1;
					break;
				default:
//					return atCONTROL_DefControlProc( hControl, nMsg, lParam1, lParam2 );
					break;
			}
			break;
/*
		case CTLMSG_MOUSEUP:
			if(lpControl->Base.bMouseDown)
			{
				lpControl->Base.bMouseDown = atFALSE;

				x = lParam2 >> 16;
				y = (short)(lParam2 & 0xffff);

				x += lpControl->Base.rect.nStartX;
				y += lpControl->Base.rect.nStartY;

				if( atUIAPI_IsPtInRect(x, y, &lpControl->Base.rect) )
					atCONTROL_ProcessControlProc( hControl, CTLMSG_KEYDOWN, atVKEY_SELECT, 0 );

				//atCONTROL_Refresh( hControl );
			}
			r = 1;
			break;
*/
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


//===========================================

atVOID atBUTTON_EXT_SetText( atHCONTROL hControl, atLPTSTR szText )
{
	atLPBUTTON lpButton = atCONTROL_GetDataPtr(hControl);
	if( lpButton == atNULL )  return;

	UI_STRNCPY(lpButton->szText, (atLPTSTR)szText, 256-1);
	lpButton->szText[256-1] = _ATC('\0');	
}

atVOID	atBUTTON_EXT_GetText( atHCONTROL hControl, atLPTSTR szText, atINT nMaxLen )
{
	atLPBUTTON lpButton = atCONTROL_GetDataPtr(hControl);
	if( lpButton == atNULL )  return;

	UI_STRNCPY(szText, lpButton->szText, nMaxLen-1);
	szText[nMaxLen-1] = _ATC('\0');
}

atVOID	atBUTTON_EXT_SetBitmap( atHCONTROL hControl, atHBITMAP lpNormalImg_Left, atHBITMAP lpNormalImg_Center, atHBITMAP lpNormalImg_Right,
							   atHBITMAP lpFocusedImg_Left, atHBITMAP lpFocusedImg_Center, atHBITMAP lpFocusedImg_Right,
							   atHBITMAP lpSelectedImg_Left, atHBITMAP lpSelectedImg_Center, atHBITMAP lpSelectedImg_Right,
							   atHBITMAP lpDisabledImg_Left, atHBITMAP lpDisabledImg_Center, atHBITMAP lpDisabledImg_Right)
{
	atLPBUTTON lpButton = atCONTROL_GetDataPtr(hControl);
	if( lpButton == atNULL )  return;

	lpButton->lpNormalImg_Left		= lpNormalImg_Left;
	lpButton->lpNormalImg_Center	= lpNormalImg_Center;
	lpButton->lpNormalImg_Right		= lpNormalImg_Right;

	lpButton->lpFocusedImg_Left		= lpFocusedImg_Left;
	lpButton->lpFocusedImg_Center	= lpFocusedImg_Center;
	lpButton->lpFocusedImg_Right	= lpFocusedImg_Right;

	lpButton->lpSelectedImg_Left	= lpSelectedImg_Left;
	lpButton->lpSelectedImg_Center	= lpSelectedImg_Center;
	lpButton->lpSelectedImg_Right	= lpSelectedImg_Right;

	lpButton->lpDisabledImg_Left	= lpDisabledImg_Left;
	lpButton->lpDisabledImg_Center	= lpDisabledImg_Center;
	lpButton->lpDisabledImg_Right	= lpDisabledImg_Right;
}							   