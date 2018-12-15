#include "A_UIENG.h"
#include "UI_Util.h"
#include "CMP_SpinBoxEx.h"


#define FONT_HEIGHT_GAP			1


typedef struct _tagatSPINBOX {
	atHBITMAP			lpNormalLeftImage;
	atHBITMAP			lpNormalRightImage;
	atHBITMAP			lpNormalBGImage;
	atHBITMAP			lpFocusedLeftImage;
	atHBITMAP			lpFocusedRightImage;
	atHBITMAP			lpFocusBGImage;

	atINT		iCurrDataPos;
	atLPSPIN_EXT_ITEM	lpCurrData;
	atUILIST		lpDataList;

	atBOOL				bActionValidate;
} atSPINBOX, * atLPSPINBOX;


static void atSPINBOX_Destroy( atHCONTROL hControl );
static void atSPINBOX_Draw( atHGDC hCanvas, atHCONTROL hControl, atBOOL bFocused );


static int SpinBoxProc( atHCONTROL hControl, int nMsg, long lParam1, long lParam2 )
{
	atLPCONTROL lpControl = atCONTROL_GetPtr(hControl);
	atLPSPINBOX	lpSpinBox	= (atLPSPINBOX)atCONTROL_GetDataPtr( hControl );
	int r = 0;
	int pos;
	void *itm;
	int x, y;
	atRECT rect;

	switch( nMsg )
	{
		case CTLMSG_INIT:
			break;
		case CTLMSG_RELEASE:
			atSPINBOX_Destroy( hControl );
			r = 1;
			break;
		case CTLMSG_PAINT:
//			atSPINBOX_Draw( atWINDOW_GetClientGDC(atCONTROL_GetParentWindow(hControl)), hControl, lpControl->Base.bFocus );
			atSPINBOX_Draw( atCONTROL_GetGDC(hControl), hControl, (atBOOL)lParam2 );
			r = 1;
			break;
		case CTLMSG_KEYDOWN:
			switch( lParam1 )
			{
				case atVKEY_UP:
				case atVKEY_DOWN:
//					return atCONTROL_DefControlProc( hControl, nMsg, lParam1, lParam2 );
					break;
				case atVKEY_VOLUP:
				case atVKEY_LEFT:
					pos = lpSpinBox->iCurrDataPos;
					itm = atUIAPI_GetListPrev( lpSpinBox->lpDataList, &pos );
					if ( itm != atNULL )
					{
						lpSpinBox->iCurrDataPos	= pos;
						lpSpinBox->lpCurrData		= itm;
					}
					else
					{
//						lpSpinBox->iCurrDataPos	= atUIAPI_GetListTailPos(lpSpinBox->lpDataList);
						lpSpinBox->lpCurrData	= (atLPSPIN_EXT_ITEM)atUIAPI_GetListTail(lpSpinBox->lpDataList, &lpSpinBox->iCurrDataPos);
					}
//					atCONTROL_Draw(hControl, atTRUE);
					atCONTROL_Refresh( hControl );
					atCONTROL_PostMessageToParent( hControl, WNDMSG_CHANGE, (long)lpControl->Base.nID, (long)lpSpinBox->iCurrDataPos );
					r = 1;
					break;
				case atVKEY_VOLDOWN:
				case atVKEY_RIGHT:
					pos = lpSpinBox->iCurrDataPos;
					itm = atUIAPI_GetListNext( lpSpinBox->lpDataList, &pos );
					if ( itm != atNULL )
					{
						lpSpinBox->iCurrDataPos	= pos;
						lpSpinBox->lpCurrData		= itm;
					}
					else
					{
//						lpSpinBox->iCurrDataPos	= atUIAPI_GetListHeadPos(lpSpinBox->lpDataList);
						lpSpinBox->lpCurrData	= (atLPSPIN_EXT_ITEM)atUIAPI_GetListHead(lpSpinBox->lpDataList, &lpSpinBox->iCurrDataPos);
					}
//					atCONTROL_Draw(hControl, atTRUE);
					atCONTROL_Refresh( hControl );
					atCONTROL_PostMessageToParent( hControl, WNDMSG_CHANGE, (long)lpControl->Base.nID, (long)lpSpinBox->iCurrDataPos );
					r = 1;
					break;
//				case atVKEY_SELECT:
//					atCONTROL_PostMessageToParent( hControl, WNDMSG_COMMAND, (long)lpControl->nIndex, 0 );
//					r = 1;
//					break;
				default:
//					return atCONTROL_DefControlProc( hControl, nMsg, lParam1, lParam2 );
					break;
			}
			break;
		case CTLMSG_MOUSEDOWN:
			x = lParam2 >> 16;
			y = lParam2 & 0xffff;
			atCONTROL_GetControlRect( hControl, &rect );
			if( x >= 0 && x <= 10 ){
				lpSpinBox->bActionValidate = 1;
			} else if( x >= RECT_WIDTH(rect)-10 && x < RECT_WIDTH(rect) ){
				lpSpinBox->bActionValidate = 2;
			}
			if( lpSpinBox->bActionValidate != 0 ){
//				atCONTROL_Draw(hControl, atTRUE);
				atCONTROL_Refresh( hControl );
			}
			r = 1;
			break;

		case CTLMSG_MOUSEUP:
			if( lpSpinBox->bActionValidate == 1 ){
				atCONTROL_ProcessControlProc( hControl, CTLMSG_KEYDOWN, atVKEY_LEFT, 0 );
			} else if( lpSpinBox->bActionValidate == 2 ){
				atCONTROL_ProcessControlProc( hControl, CTLMSG_KEYDOWN, atVKEY_RIGHT, 0 );
			}
			lpSpinBox->bActionValidate = 0;
			r = 1;
			break;
		default :
//			return atCONTROL_DefControlProc( hControl, nMsg, lParam1, lParam2 );
			break;
	}

	return r;
}


atHCONTROL atSPINBOX_EXT_Create( int nIndex, int nX, int nY, int nWidth, int nHeight, 
						atHBITMAP lpNormalLeftImage, atHBITMAP lpNormalRightImage, atHBITMAP lpNormalBGImage,
						atHBITMAP lpFocusedLeftImage, atHBITMAP lpFocusedRightImage, atHBITMAP lpFocusBGImage)
{
	atHCONTROL		hControl;
	atLPCONTROL		lpControl;

//	lpControl	= atCONTROL_CreateBaseControl( hParentWnd, nIndex, nX, nY, nWidth, nHeight, CTL_PARENTTYPE_WINDOW, atNULL );
	hControl = atCONTROL_Create( nIndex, nX, nY, nWidth, nHeight, CTL_PARENTTYPE_WINDOW, 0, atNULL, atNULL );
	lpControl = atCONTROL_GetPtr(hControl);
	
	if ( lpControl != atNULL )
	{
		atLPSPINBOX		lpSpinBox;
		atHBITMAP				lpBM;

//		lpControl->nControlTypeID	= CTLID_SPINBOX;

		lpSpinBox = (atLPSPINBOX)UI_MEMALLOC(sizeof(atSPINBOX));
		if ( lpSpinBox == atNULL ){
			UI_MEMFREE(lpControl);
			return atNULL;
		}
		atUIAPI_memset(lpSpinBox, 0, sizeof(atSPINBOX));

//		lpSpinBox->lpNormalLeftImage	= CreateImageEx(g_lpImageMG, szNormalLeftImage, atNULL);
//		lpSpinBox->lpNormalRightImage	= CreateImageEx(g_lpImageMG, szNormalRightImage, atNULL);
//		lpSpinBox->lpOnStartXImage		= CreateImageEx(g_lpImageMG, szOnStartXImage, atNULL);
//		lpSpinBox->lpOnEndXImage		= CreateImageEx(g_lpImageMG, szOnEndXImage, atNULL);
		lpSpinBox->lpNormalLeftImage	= lpNormalLeftImage;
		lpSpinBox->lpNormalRightImage	= lpNormalRightImage;
		lpSpinBox->lpNormalBGImage		= lpNormalBGImage;
		lpSpinBox->lpFocusedLeftImage	= lpFocusedLeftImage;
		lpSpinBox->lpFocusedRightImage	= lpFocusedRightImage;
		lpSpinBox->lpFocusBGImage		= lpFocusBGImage;

		lpBM = lpSpinBox->lpNormalLeftImage;
		if ( nHeight <= 0 ){
			if( lpBM )
				lpControl->Base.rect.nEndY		= nY + atUIAPI_GetBitmapHeight(lpBM);
			else
				lpControl->Base.rect.nEndY		= nY + 20;
		}

		lpSpinBox->bActionValidate = atFALSE;

		lpSpinBox->lpDataList			= atUIAPI_CreateList();

		lpControl->Base.nObjDataSize = sizeof(atSPINBOX);
		lpControl->Base.lpObjData = lpSpinBox;
		lpControl->Base.fnObjProc = SpinBoxProc;
	}

//	return atCONTROL_GetHandle(lpControl);
	return hControl;
}

static void atSPINBOX_Destroy( atHCONTROL hControl )
{
	atLPCONTROL lpControl = atCONTROL_GetPtr(hControl);

//	ReleaseImage(g_lpImageMG, &((atLPSPINBOX)atCONTROL_GetDataPtr( hControl ))->lpNormalLeftImage);
//	ReleaseImage(g_lpImageMG, &((atLPSPINBOX)atCONTROL_GetDataPtr( hControl ))->lpNormalRightImage);
//	ReleaseImage(g_lpImageMG, &((atLPSPINBOX)atCONTROL_GetDataPtr( hControl ))->lpNormalRightImage);
//	ReleaseImage(g_lpImageMG, &((atLPSPINBOX)atCONTROL_GetDataPtr( hControl ))->lpOnEndXImage);

	atUIAPI_DestroyList(((atLPSPINBOX)atCONTROL_GetDataPtr( hControl ))->lpDataList, atTRUE);
}

static void atSPINBOX_Draw( atHGDC hCanvas, atHCONTROL hControl, atBOOL bFocused )
{
	atLPCONTROL lpControl = atCONTROL_GetPtr(hControl);
	atLPSPINBOX	lpSpinBox;
	int				/*nAscent, */nHeight;
//	char	szNewText[2048];
	atHBITMAP			lpBM;
	int				nT1, nT2;
	atLPSPIN_EXT_ITEM lpSpinData;
	atRECT rect;
	long color;
//	atHFONT fnt;
	int x, y;

	if ( lpControl->Base.bShow == atFALSE ){
		return;
	}

	lpSpinBox = (atLPSPINBOX)atCONTROL_GetDataPtr( hControl );

	atCONTROL_GetControlRect( hControl, &rect );

//	fnt = atUIAPI_SetFont(hCanvas, lpControl->font);
	nHeight = atUIAPI_GetFontHeight( atCONTROL_GetFont(hControl) );

	if ( bFocused ){
		lpBM =lpSpinBox->lpFocusedLeftImage;
		if( lpBM ){
			nT1 = atUIAPI_GetBitmapWidth(lpBM);
			atUIAPI_DrawImage(hCanvas, 0, 0, lpBM);
		} else {
			nT1 = 10;
			atUIAPI_FillRect( hCanvas, 0, 0, nT1, RECT_HEIGHT(rect), atUIAPI_GetRGB(255,128,0));
			x = (10-3)/2; y = (RECT_HEIGHT(rect))/2;
			atUIAPI_DrawLine( hCanvas, x+0, y-1, x+0, y+1, atUIAPI_GetRGB(255,255,255), 1, atFALSE );
			atUIAPI_DrawLine( hCanvas, x+1, y-2, x+1, y+2, atUIAPI_GetRGB(255,255,255), 1, atFALSE );
			atUIAPI_DrawLine( hCanvas, x+2, y-3, x+2, y+3, atUIAPI_GetRGB(255,255,255), 1, atFALSE );
		}
		lpBM = lpSpinBox->lpFocusedRightImage;
		if( lpBM ){
			nT2 = RECT_WIDTH(rect)-1 - atUIAPI_GetBitmapWidth(lpBM);
			atUIAPI_DrawImage(hCanvas, nT2, 0, lpBM);
		} else {
			nT2 = RECT_WIDTH(rect)-1 - 10;
			atUIAPI_FillRect( hCanvas, nT2, 0, 10, RECT_HEIGHT(rect), atUIAPI_GetRGB(255,128,0));
			x = nT2 + 10-1 - (10-3)/2; y = (RECT_HEIGHT(rect))/2;
			atUIAPI_DrawLine( hCanvas, x-0, y-1, x-0, y+1, atUIAPI_GetRGB(255,255,255), 1, atFALSE );
			atUIAPI_DrawLine( hCanvas, x-1, y-2, x-1, y+2, atUIAPI_GetRGB(255,255,255), 1, atFALSE );
			atUIAPI_DrawLine( hCanvas, x-2, y-3, x-2, y+3, atUIAPI_GetRGB(255,255,255), 1, atFALSE );
		}
		lpBM = lpSpinBox->lpFocusBGImage;
		if( lpBM ){
			atUIAPI_DrawImage(hCanvas, nT1, 0, lpBM);
		} else {
 			atUIAPI_FillRect(hCanvas, nT1, 0, ABS(nT2-nT1), RECT_HEIGHT(rect), atUIAPI_GetRGB(162, 248, 248));
 			atUIAPI_DrawLine(hCanvas, nT1, 0, nT2-1, 0, atUIAPI_GetRGB(31, 118, 219), 1, atFALSE );
 			atUIAPI_DrawLine(hCanvas, nT1, 1, nT2-1, 1, atUIAPI_GetRGB(114, 217, 238), 1, atFALSE );
 			atUIAPI_DrawLine(hCanvas, nT1, RECT_HEIGHT(rect)-1, nT2-1, RECT_HEIGHT(rect)-1, atUIAPI_GetRGB(76, 149, 248), 1, atFALSE );
		}
	} else {
		lpBM =lpSpinBox->lpNormalLeftImage;
		if( lpBM ){
			nT1 = atUIAPI_GetBitmapWidth(lpBM);
			atUIAPI_DrawImage(hCanvas, 0, 0, lpBM);
		} else {
			nT1 = 10;
			atUIAPI_FillRect( hCanvas, 0, 0, nT1, RECT_HEIGHT(rect), atUIAPI_GetRGB(0,230,0));
			x = (10-3)/2; y = (RECT_HEIGHT(rect))/2;
			atUIAPI_DrawLine( hCanvas, x+0, y-1, x+0, y+1, atUIAPI_GetRGB(0,0,0), 1, atFALSE );
			atUIAPI_DrawLine( hCanvas, x+1, y-2, x+1, y+2, atUIAPI_GetRGB(0,0,0), 1, atFALSE );
			atUIAPI_DrawLine( hCanvas, x+2, y-3, x+2, y+3, atUIAPI_GetRGB(0,0,0), 1, atFALSE );
//			atUIAPI_FillRect( hCanvas, nT1, 0, 10, RECT_HEIGHT(rect), atUIAPI_GetRGB(255,128,0));
		}
		lpBM = lpSpinBox->lpNormalRightImage;
		if( lpBM ){
			nT2 = RECT_WIDTH(rect)-1 - atUIAPI_GetBitmapWidth(lpBM);
			atUIAPI_DrawImage(hCanvas, nT2, 0, lpBM);
		} else {
			nT2 = RECT_WIDTH(rect)-1 - 10;
			atUIAPI_FillRect( hCanvas, nT2, 0, 10, RECT_HEIGHT(rect), atUIAPI_GetRGB(0,230,0));
			x = nT2 + 10-1 - (10-3)/2; y = (RECT_HEIGHT(rect))/2;
			atUIAPI_DrawLine( hCanvas, x-0, y-1, x-0, y+1, atUIAPI_GetRGB(0,0,0), 1, atFALSE );
			atUIAPI_DrawLine( hCanvas, x-1, y-2, x-1, y+2, atUIAPI_GetRGB(0,0,0), 1, atFALSE );
			atUIAPI_DrawLine( hCanvas, x-2, y-3, x-2, y+3, atUIAPI_GetRGB(0,0,0), 1, atFALSE );
		}
		lpBM = lpSpinBox->lpNormalBGImage;
		if( lpBM ){
			atUIAPI_DrawImage(hCanvas, nT1, 0, lpBM);
		} else {
			atUIAPI_FillRect(hCanvas, nT1, 0, ABS(nT2-nT1), RECT_HEIGHT(rect), atUIAPI_GetRGB(236, 255, 255));
			atUIAPI_DrawLine(hCanvas, nT1, 0, nT2-1, 0, atUIAPI_GetRGB(76, 149, 248), 1, atFALSE );
			atUIAPI_DrawLine(hCanvas, nT1, RECT_HEIGHT(rect)-1, nT2-1, RECT_HEIGHT(rect)-1, atUIAPI_GetRGB(76, 149, 248), 1, atFALSE);
		}
	}

	if ( bFocused ){
		color = lpControl->tControlColor.clFocusedTxtColor;
		//color = atUIAPI_GetRGB(0, 62, 139);
	} else {
		color = lpControl->tControlColor.clNormalTxtColor;
		//color = atUIAPI_GetRGB(21, 104, 218);
	}
	lpSpinData = (atLPSPIN_EXT_ITEM)lpSpinBox->lpCurrData;
	if( lpSpinData != atNULL ) {
		int sw = atUIAPI_GetStringWidth(atUIAPI_GetFont(hCanvas), lpSpinData->szText, -1);
		
		int igap = (RECT_HEIGHT(rect) % 2) + (nHeight % 2);  // 버튼, 폰트 높이가 홀수이면 가운데 정렬할때 오차를 보정 
		atUIAPI_DrawString( hCanvas, nT1 + ((nT2-nT1) - sw) / 2, 
					/*+ nAscent*/ + ABS(RECT_HEIGHT(rect) - nHeight)/2 - igap, 
					lpSpinData->szText, color);
	}
	
//	atUIAPI_SetFont(hCanvas, fnt);
}


int atSPINBOX_EXT_GetCurrItem( atHCONTROL hControl )
{
	atLPCONTROL lpControl = atCONTROL_GetPtr(hControl);
	atLPSPINBOX	lpSpinBox	= (atLPSPINBOX)atCONTROL_GetDataPtr( hControl );

//	if ( lpControl->nControlTypeID != CTLID_SPINBOX )
//		return -1;
	if( lpSpinBox->lpCurrData == atNULL ) return -1;

	return lpSpinBox->lpCurrData->nIndex;
}

atBOOL atSPINBOX_EXT_SetCurrItem(atHCONTROL hControl, int nSpinBoxItemIndex)
{
	atLPCONTROL lpControl = atCONTROL_GetPtr(hControl);
	atLPSPINBOX	lpSpinBox	= (atLPSPINBOX)atCONTROL_GetDataPtr( hControl );
	atLPSPIN_EXT_ITEM	lpSpinData;
	int		findPos;

//	if ( lpControl->nControlTypeID != CTLID_SPINBOX )
//		return atFALSE;

	lpSpinData = atSPINBOX_EXT_FindItem(hControl, nSpinBoxItemIndex);
	if ( lpSpinData != atNULL )
	{
		findPos = atUIAPI_FindListItemIdx(lpSpinBox->lpDataList, lpSpinData, 0);
		if ( findPos != -1 )
		{
			lpSpinBox->lpCurrData		= lpSpinData;
			lpSpinBox->iCurrDataPos	= findPos;
			
//			atCONTROL_Draw(hControl, atFALSE);
			atCONTROL_Refresh( hControl );
			return atTRUE;
		}
	}
	return atFALSE;
}

atLPSPIN_EXT_ITEM atSPINBOX_EXT_FindItem(atHCONTROL hControl, int nIndex)
{
	atLPSPINBOX	lpSpinBox	= (atLPSPINBOX)atCONTROL_GetDataPtr( hControl );
	int		pos;
	atLPSPIN_EXT_ITEM	lpSpinData;

//	pos = atUIAPI_GetListHeadPos(lpSpinBox->lpDataList);
	lpSpinData = (atLPSPIN_EXT_ITEM)atUIAPI_GetListHead(lpSpinBox->lpDataList, &pos);
	while ( lpSpinData != atNULL ){
		if ( lpSpinData->nIndex == nIndex )
		{
			return lpSpinData;
		}
		lpSpinData = (atLPSPIN_EXT_ITEM)atUIAPI_GetListNext(lpSpinBox->lpDataList, &pos);
	}
	return atNULL;
}

atLPTSTR atSPINBOX_EXT_GetCurrItemText(atHCONTROL hControl)
{
	atLPCONTROL lpControl = atCONTROL_GetPtr(hControl);
	atLPSPINBOX	lpSpinBox	= (atLPSPINBOX)atCONTROL_GetDataPtr( hControl );
	atLPSPIN_EXT_ITEM	lpSpinData;

	lpSpinData = lpSpinBox->lpCurrData;
	return lpSpinData->szText;
}

void* atSPINBOX_EXT_GetCurrItemData(atHCONTROL hControl)
{
	atLPCONTROL lpControl = atCONTROL_GetPtr(hControl);
	atLPSPINBOX	lpSpinBox	= (atLPSPINBOX)atCONTROL_GetDataPtr( hControl );
	atLPSPIN_EXT_ITEM	lpSpinData;

	lpSpinData = lpSpinBox->lpCurrData;
	return lpSpinData->data;
}

int atSPINBOX_EXT_AddItem(atHCONTROL hControl, int nIndex, atLPTSTR szText)
{
	atLPCONTROL lpControl = atCONTROL_GetPtr(hControl);
	atLPSPINBOX	lpSpinBox	= (atLPSPINBOX)atCONTROL_GetDataPtr( hControl );
	atLPSPIN_EXT_ITEM	lpSpinData;
	atBOOL			bExist;

	lpSpinData = atSPINBOX_EXT_FindItem(hControl, nIndex);
	if ( lpSpinData == atNULL ){
		lpSpinData = (atLPSPIN_EXT_ITEM)UI_MEMALLOC(sizeof(atSPIN_EXT_ITEM));
		bExist		= atFALSE;
	}
	else
	{
		bExist		= atTRUE;
	}
	atUIAPI_memset(lpSpinData, 0, sizeof(atSPIN_EXT_ITEM));
	UI_STRNCPY(lpSpinData->szText, szText, sizeof(lpSpinData->szText)/sizeof(atTCHAR)-1);
	lpSpinData->nIndex = nIndex;

	if ( !bExist ){
		atUIAPI_AddListTail(lpSpinBox->lpDataList, lpSpinData);

//		lpSpinBox->iCurrDataPos	= atUIAPI_GetListHeadPos(lpSpinBox->lpDataList);
		lpSpinBox->lpCurrData		= (atLPSPIN_EXT_ITEM)atUIAPI_GetListHead(lpSpinBox->lpDataList, &lpSpinBox->iCurrDataPos);
		return 1;
	}

	return 0;
}

int atSPINBOX_EXT_AddItemWithData(atHCONTROL hControl, int nIndex, atLPTSTR szText, void *data)
{
	atLPCONTROL lpControl = atCONTROL_GetPtr(hControl);
	atLPSPINBOX	lpSpinBox	= (atLPSPINBOX)atCONTROL_GetDataPtr( hControl );
	atLPSPIN_EXT_ITEM	lpSpinData;
	atBOOL			bExist;

	lpSpinData = atSPINBOX_EXT_FindItem(hControl, nIndex);
	if ( lpSpinData == atNULL )
	{
		lpSpinData = (atLPSPIN_EXT_ITEM)UI_MEMALLOC(sizeof(atSPIN_EXT_ITEM));
		bExist		= atFALSE;
	}
	else
	{
		bExist		= atTRUE;
	}
	atUIAPI_memset(lpSpinData, 0, sizeof(atSPIN_EXT_ITEM));
	UI_STRNCPY(lpSpinData->szText, szText, sizeof(lpSpinData->szText)/sizeof(atTCHAR)-1);
	lpSpinData->nIndex = nIndex; 
	lpSpinData->data = data;
	
	atUIAPI_AddListTail(lpSpinBox->lpDataList, lpSpinData);

	if ( !bExist )
	{
//		lpSpinBox->iCurrDataPos	= atUIAPI_GetListHeadPos(lpSpinBox->lpDataList);
		lpSpinBox->lpCurrData		= (atLPSPIN_EXT_ITEM)atUIAPI_GetListHead(lpSpinBox->lpDataList, &lpSpinBox->iCurrDataPos);
		return 1;
	}

	return 0;
}


void atSPINBOX_EXT_ClearAllItem(atHCONTROL hControl)
{
	atLPSPINBOX lpSpinBox = (atLPSPINBOX)atCONTROL_GetDataPtr(hControl);

	atUIAPI_RemoveListAll( lpSpinBox->lpDataList, atTRUE);
}



