//
// Button.c
//
///////////////////////////////////////////////////////////////////////

#include "A_UIENG.h"
#include "CMP_Button.h"
#include <string.h>


typedef struct _atBUTTON {
	atTCHAR		    szText[32];
} atBUTTON, * atLPBUTTON;


static int ButtonProc( atHCONTROL hControl, int nMsg, long lParam1, long lParam2 );


atHCONTROL atBUTTON_Create( int nID, int nX, int nY, int nWidth, int nHeight, atLPTSTR szText ) 
{
	atHCONTROL hControl;

	hControl = atCONTROL_Create( nID, nX, nY, nWidth, nHeight, CTL_PARENTTYPE_WINDOW, sizeof(atBUTTON), ButtonProc, (void *)szText );
	if( hControl == atNULL ) return atNULL;

	{
		atLPCONTROL lpControl = atCONTROL_GetPtr(hControl);
		atLPBUTTON lpButton = atCONTROL_GetDataPtr(hControl);

		if ( nWidth <= 0 )	{
		  int nStrWidth = atUIAPI_GetStringWidth( atCONTROL_GetFont(hControl), szText, -1 );
		  lpControl->Base.rect.nEndX		= nX + nStrWidth + 8*2;
		}
		if ( nHeight <= 0 ){
		  lpControl->Base.rect.nEndY		= nY + 18 - 1;
		}
	}

	return hControl;
}


static void atBUTTON_OnInit( atHCONTROL hControl, void *param )
{
	atLPBUTTON	lpButton = (atLPBUTTON)atCONTROL_GetDataPtr(hControl);
	if ( lpButton == atNULL ) return;
	
	if(param){
		UI_STRNCPY(lpButton->szText, (atLPTSTR)param, 32-1);
		lpButton->szText[32-1] = _ATC('\0');
	} else
		lpButton->szText[0] = _ATC('\0');

	atCONTROL_SetSoftKey( hControl, atNULL, _AT("OK"), atNULL );
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
	atCOLOR fr_color, bg_color, txt_color;
	atREGION rgn;

	if ( atCONTROL_IsVisible(hControl) == atFALSE ) return;

	lpButton = (atLPBUTTON)atCONTROL_GetDataPtr( hControl );

	atCONTROL_GetControlRegion( hControl, &rgn );
		
	if( atCONTROL_IsEnable(hControl) ){
		if( bFocused ){
		    fr_color = lpControl->tControlColor.clFocusedFrColor;
		    txt_color = lpControl->tControlColor.clFocusedTxtColor;
		    bg_color = lpControl->tControlColor.clFocusedBgColor;
		} else {
		    fr_color = lpControl->tControlColor.clNormalFrColor;
		    txt_color = lpControl->tControlColor.clNormalTxtColor;
		    bg_color = lpControl->tControlColor.clNormalBgColor;
		}
	} else {
	    fr_color = lpControl->tControlColor.clDisableFrColor;
	    txt_color = lpControl->tControlColor.clDisableTxtColor;
	    bg_color = lpControl->tControlColor.clDisableBgColor;
	}

	atUIAPI_FillRect( hCanvas, 0, 0, rgn.nWidth, rgn.nHeight, bg_color );
	atUIAPI_DrawStringAlign( hCanvas, 0 + rgn.nWidth/2, 0 + rgn.nHeight/2, lpButton->szText, ALIGN_CENTER_MIDDLE, txt_color );
}


static int ButtonProc( atHCONTROL hControl, int nMsg, long lParam1, long lParam2 )
{
	atLPCONTROL lpControl = atCONTROL_GetPtr(hControl);
	int r = 0;
//	int x, y;

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

atVOID atBUTTON_SetText( atHCONTROL hControl, atLPTSTR szText )
{
	atLPBUTTON lpButton = atCONTROL_GetDataPtr(hControl);
	if( lpButton == atNULL )  return;

	UI_STRNCPY(lpButton->szText, (atLPTSTR)szText, 32-1);
	lpButton->szText[32-1] = _ATC('\0');	
}


atLPTSTR atBUTTON_GetText( atHCONTROL hControl )
{
	atLPBUTTON lpButton = atCONTROL_GetDataPtr(hControl);
	if( lpButton == atNULL )  return atNULL;

	return lpButton->szText;	
}


atVOID atBUTTON_RegistControl( atNOARGS )
{
	//atCONTROL_RegistControl( _AT("BUTTON"),
	//	atBUTTON_Create,
	//	atCONTROL_GenMethodList(
	//		{"SetText", atTYPE_VOID, 1, {atTYPE_STR}}
	//	),
	//	atCONTROLGenPropertyList(
	//		{"Text", atTYPE_STR, atBUTTON_GetText, atBUTTON_atBUTTON_SetText}
	//	)
	//);
}

