//
// MenuWnd.c
//
///////////////////////////////////////////////////////////////////////

#include "A_UIENG.h"
#include "UI_Util.h"
#include "CMP_AniButton.h"
#include <string.h>
#include <stdarg.h>


typedef struct _MYANIBUTTON {
	atBOOL          bRepeat;
	atBOOL          bIsMask;
	int             nImageCount;
	atHBITMAP *		lpImageList;
	int             nImageIdx;
//	atHTIMER        hAniTimer;
} MYANIBUTTON, * LPMYANIBUTTON;


static int ButtonProc( atHCONTROL hControl, int nMsg, long lParam1, long lParam2 );


typedef struct {
	atLONG nTimerInteval;
	atINT bRepeat;
} _CreateParam;


atHCONTROL atANIBUTTON_Create( int nIndex, int nX, int nY, int nWidth, int nHeight,
						       atINT nTimerInteval, atBOOL bRepeat, atINT nImageCount, ... )
{
	atHCONTROL hControl;
	_CreateParam param;

	param.nTimerInteval = nTimerInteval;
	param.bRepeat = bRepeat;

	hControl = atCONTROL_Create( nIndex, nX, nY, nWidth, nHeight, CTL_PARENTTYPE_WINDOW, sizeof(MYANIBUTTON), ButtonProc, &param );
	if( hControl == atNULL ) return atNULL;
	
	{
		LPMYANIBUTTON	lpButton = (LPMYANIBUTTON)atCONTROL_GetDataPtr(hControl);
		atLPCONTROL	lpControl = atCONTROL_GetPtr(hControl);
		atHBITMAP		lpBM;
		int i;
		va_list l;

		lpButton->nImageCount   = nImageCount;
		lpButton->lpImageList	= UI_MEMALLOC( nImageCount * sizeof(atHBITMAP) );
		
        va_start( l, nImageCount );
		for( i = 0; i < nImageCount; i++ ){
			lpButton->lpImageList[i] = va_arg(l, atHBITMAP);
		}
        va_end( l );

//		lpButton->nImageIdx = 0;

//		if( nTimerInteval > 0 )
//		  lpButton->hAniTimer = atCONTROL_AddControlTimer( atCONTROL_GetHandle(lpControl), 0, nTimerInteval, atTRUE, atFALSE );
//		else
//		  lpButton->hAniTimer = atNULL;
//		atCONTROL_AddControlTimer( hControl, 0, nTimerInteval, atTRUE, atFALSE );

		lpBM = lpButton->lpImageList[0];

		if ( nWidth <= 0 )
		{
			if( lpBM )
			  lpControl->Base.rect.nEndX		= nX + atUIAPI_GetBitmapWidth(lpBM);
			else
			  lpControl->Base.rect.nEndX		= nX + 20;
		}
		if ( nHeight <= 0 )
		{
			if( lpBM )
			  lpControl->Base.rect.nEndY		= nY + atUIAPI_GetBitmapHeight(lpBM);
			else
			  lpControl->Base.rect.nEndY		= nY + 20;
		}
	}

	return hControl;
}

static void atANIBUTTON_Init( atHCONTROL hControl, _CreateParam *param )
{
//	atLPCONTROL 	lpControl = atCONTROL_GetPtr(hControl);
	LPMYANIBUTTON	lpButton = (LPMYANIBUTTON)atCONTROL_GetDataPtr(hControl);

	atCONTROL_AddControlTimer( hControl, 0, param->nTimerInteval, atTRUE, atFALSE, atFALSE );	

	lpButton->bRepeat			= param->bRepeat;
	lpButton->bIsMask			= atFALSE;
	lpButton->nImageCount		= 0;
	lpButton->lpImageList		= atNULL;
	lpButton->nImageIdx			= 0;
	
	atCONTROL_SetSoftKey( hControl, atNULL, _AT("OK"), atNULL );
}

static void atANIBUTTON_Destroy( atHCONTROL hControl )
{
	LPMYANIBUTTON	lpButton = (LPMYANIBUTTON)atCONTROL_GetDataPtr(hControl);

//    if( lpButton->hAniTimer )
//		atUIAPI_RemoveTimer( lpButton->hAniTimer );

	if( lpButton->lpImageList != atNULL ) UI_MEMFREE( lpButton->lpImageList );
}

static void atANIBUTTON_Draw( atHGDC hCanvas, atHCONTROL hControl, atBOOL bFocused )
{
//	atLPCONTROL lpControl = atCONTROL_GetPtr(hControl);
	LPMYANIBUTTON	lpButton;
	atHBITMAP			lpBM;
//	atHBITMAPMASK		lpBMmsk = atNULL;
	atRECT rect;

	if ( atCONTROL_IsVisible(hControl) == atFALSE ) return;

	atCONTROL_GetControlRect(hControl, &rect);
		
	lpButton = (LPMYANIBUTTON)atCONTROL_GetDataPtr( hControl );

	lpBM = lpButton->lpImageList[ lpButton->nImageIdx ];

	if( lpBM ){
	    atUIAPI_DrawImage(hCanvas, 0, 0, lpBM);
	} else {
		atUIAPI_FillRect(hCanvas, 0, 0, RECT_WIDTH(rect), RECT_HEIGHT(rect), (bFocused)? atUIAPI_GetRGB(255,128,0) : atUIAPI_GetRGB(0,230,0) );
	}
}

static void atANIBUTTON_Timer( atHCONTROL hControl, atLONG id )
{
	LPMYANIBUTTON lpButton = (LPMYANIBUTTON)atCONTROL_GetDataPtr( hControl );

	lpButton->nImageIdx ++;
	if( lpButton->nImageIdx >= lpButton->nImageCount ){
		if( lpButton->bRepeat ){
			lpButton->nImageIdx = 0;
		} else {
			lpButton->nImageIdx = lpButton->nImageCount-1;
//			atUIAPI_EnableTimer( atCONTROL_GetControlTimer(hControl,0), atFALSE );
			atCONTROL_StopControlTimer( hControl, 0 );
			return;
		}
	}

	if( lpButton->bIsMask ){
//		atWINDOW_Draw( atCONTROL_GetParentWindow(hControl), atTRUE);
		atCONTROL_Refresh(hControl);
	} else {
//		atCONTROL_Draw( hControl, atTRUE );
		atCONTROL_Refresh(hControl);
	}
}


static int ButtonProc( atHCONTROL hControl, int nMsg, long lParam1, long lParam2 )
{
	atLPCONTROL lpControl = atCONTROL_GetPtr(hControl);
	LPMYANIBUTTON lpButton = (LPMYANIBUTTON)atCONTROL_GetDataPtr( hControl );
	int r = 0;

	switch( nMsg )
	{
		case CTLMSG_INIT:
			atANIBUTTON_Init( hControl, (_CreateParam *)lParam1 );
			r = 1;
			break;
		case CTLMSG_RELEASE:
			atANIBUTTON_Destroy( hControl );
			r = 1;
			break;
		case CTLMSG_PAINT:
			atANIBUTTON_Draw( atCONTROL_GetGDC(hControl), hControl, (atBOOL)lParam2 );
			r = 1;
			break;
		case CTLMSG_TIMER:
			atANIBUTTON_Timer( hControl, lParam1 );
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
		case CTLMSG_GOTFOCUS:
			lpButton->nImageIdx = 0;
			atCONTROL_StartControlTimer(hControl, 0);
			break;
		case CTLMSG_LOSTFOCUS:
			atCONTROL_StopControlTimer(hControl, 0);
			lpButton->nImageIdx = 0;
			break;
		case CTLMSG_MOUSEUP:
			atCONTROL_ProcessControlProc( hControl, CTLMSG_KEYDOWN, atVKEY_SELECT, 0 );
			r = 1;
			break;
//		case CTLMSG_SUSPEND:
//			break;
//		case CTLMSG_RESUME:
//			break;
		default :
//			return atCONTROL_DefControlProc( hControl, nMsg, lParam1, lParam2 );
			break;
	}

	return r;
}



