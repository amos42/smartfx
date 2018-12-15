//
// ImageBox.c
//
///////////////////////////////////////////////////////////////////////

#include "A_UIENG.h"
#include "UI_Util.h"
#include "CMP_ImageBox.h"
#include <string.h>


#define FONT_HEIGHT_GAP			1


typedef struct _tagatIMAGEBOX {
	int			uAlign;
	atU32		uAttr;
	atHBITMAP       lpBitmap;
	atBOOL      bOwnImage;
} atIMAGEBOX, * atLPIMAGEBOX;

static int ImageBoxProc( atHCONTROL hControl, int nMsg, long lParam1, long lParam2 );


atHCONTROL atIMAGEBOX_Create( int nIndex, int nX, int nY, int nWidth, int nHeight, atHBITMAP lpBitmap, atBOOL bOwnImage )
{
	atHCONTROL		hControl;
	atLPCONTROL		lpControl;

//	lpControl	= atCONTROL_CreateBaseControl( hParentWnd, nIndex, nX, nY, nWidth, nHeight, CTL_PARENTTYPE_WINDOW, atNULL );
	hControl = atCONTROL_Create( nIndex, nX, nY, nWidth, nHeight, CTL_PARENTTYPE_WINDOW, 0, atNULL, atNULL );
	lpControl = atCONTROL_GetPtr(hControl);
	
	if ( lpControl != atNULL )
	{
		atLPIMAGEBOX	lpImageBox;

//		lpControl->nControlTypeID = CTLID_IMAGEBOX;
//		lpControl->Base.bFocusable = atFALSE;		// ImageBox는 기본적으로 포커스를 가지지 않는다.
		atCONTROL_SetAttr( hControl, atCONTROL_GetAttr(hControl) & ~atOBJATTR_EVENTTARGET );
		atCONTROL_SetFocusable( hControl, atFALSE );

		lpImageBox = (atLPIMAGEBOX)UI_MEMALLOC(sizeof(atIMAGEBOX));
		if ( lpImageBox == atNULL )
		{
			UI_MEMFREE(lpControl);
			return atNULL;
		}
		atUIAPI_memset(lpImageBox, 0, sizeof(atIMAGEBOX));

		if( bOwnImage )
		{
			lpImageBox->lpBitmap = lpBitmap;
		}
		else
		{
			lpImageBox->lpBitmap = lpBitmap;
		}

		if ( nWidth == 0 )
		{
			lpControl->Base.rect.nEndX		= nX + atUIAPI_GetBitmapWidth(lpBitmap);
		}
		if ( nHeight == 0 )
		{
			lpControl->Base.rect.nEndY		= nY + atUIAPI_GetBitmapHeight(lpBitmap);
		}

		lpControl->Base.nObjDataSize = sizeof(atIMAGEBOX);
		lpControl->Base.lpObjData = lpImageBox;
		lpControl->Base.fnObjProc = ImageBoxProc;
	}
	
//	return atCONTROL_GetHandle(lpControl);
	return hControl;
}


static void atIMAGEBOX_OnDestroy( atHCONTROL hControl )
{
	atLPCONTROL lpControl = atCONTROL_GetPtr(hControl);
	atLPIMAGEBOX	lpImageBox = (atLPIMAGEBOX)atCONTROL_GetDataPtr( hControl );

	if( lpImageBox->bOwnImage ){
		if (lpImageBox->lpBitmap){ // check memory 
			atUIAPI_DestroyBitmap( lpImageBox->lpBitmap );
			lpImageBox->lpBitmap = atNULL;
		}
	}
}

static void atIMAGEBOX_OnDraw( atHGDC hCanvas, atHCONTROL hControl, atBOOL bFocused )
{
	atLPCONTROL lpControl = atCONTROL_GetPtr(hControl);
	atLPIMAGEBOX	lpImageBox = (atLPIMAGEBOX)atCONTROL_GetDataPtr( hControl );
	atRECT rect;

	if ( lpControl->Base.bShow == atFALSE )	return;

	lpImageBox = (atLPIMAGEBOX)atCONTROL_GetDataPtr( hControl );
   
//	atCONTROL_GetScreenRect( hControl, &rect );
rect = lpControl->Base.rect;

//	atUIAPI_SetFont( hCanvas, lpControl->font );

	if( atCONTROL_IsFocusable(hControl) ){
		if ( bFocused ){
			if( lpImageBox->uAttr & atIMAGEBOXATTR_OUTLINE )
				atUIAPI_DrawRect(hCanvas, 0, 0, RECT_WIDTH(rect), RECT_HEIGHT(rect), atUIAPI_GetRGB(0x00, 0x3E, 0x8B), 1);
			if( lpImageBox->uAttr & atIMAGEBOXATTR_OPAQUE )
				atUIAPI_FillRect(hCanvas, 1, 1, RECT_WIDTH(rect) - 2, RECT_HEIGHT(rect)- 2, atUIAPI_GetRGB(0xA2, 0xF8, 0xF8));
		} else {
			if( lpImageBox->uAttr & atIMAGEBOXATTR_OUTLINE )
				atUIAPI_DrawRect(hCanvas, 0, 0, RECT_WIDTH(rect), RECT_HEIGHT(rect), atUIAPI_GetRGB(76, 149, 248), 1);
			if( lpImageBox->uAttr & atIMAGEBOXATTR_OPAQUE )
				atUIAPI_FillRect(hCanvas, 1, 1, RECT_WIDTH(rect) - 2, RECT_HEIGHT(rect)- 2, atUIAPI_GetRGB(236, 255, 255));
		}
	} else {
		if( lpImageBox->uAttr & atIMAGEBOXATTR_OUTLINE )
			atUIAPI_DrawRect(hCanvas, 0, 0, RECT_WIDTH(rect), RECT_HEIGHT(rect), atUIAPI_GetRGB(76, 149, 248), 1);
		if( lpImageBox->uAttr & atIMAGEBOXATTR_OPAQUE )
			atUIAPI_FillRect(hCanvas, 1, 1, RECT_WIDTH(rect) - 2, RECT_HEIGHT(rect)- 2, atUIAPI_GetRGB(236, 255, 255));
	}
	
	atUIAPI_DrawImage(hCanvas, 0, 0, lpImageBox->lpBitmap );
}

static int ImageBoxProc( atHCONTROL hControl, int nMsg, long lParam1, long lParam2 )
{
	atLPCONTROL lpControl = atCONTROL_GetPtr(hControl);
	int r = 0;

	switch( nMsg )
	{
		case CTLMSG_INIT:
			break;
		case CTLMSG_RELEASE:
			atIMAGEBOX_OnDestroy( hControl );
			r = 1;
			break;
		case CTLMSG_PAINT:
			atIMAGEBOX_OnDraw( atCONTROL_GetGDC(hControl), hControl, (atBOOL)lParam2 );
			r = 1;
			break;
	}

	return r;
}



atU32 atIMAGEBOX_GetAttr(atHCONTROL hControl )
{
	atLPIMAGEBOX	lpImageBox = atCONTROL_GetDataPtr( hControl );
	return lpImageBox->uAttr;
}

void atIMAGEBOX_SetAttr(atHCONTROL hControl, atU32 uAttr )
{
	atLPIMAGEBOX	lpImageBox = atCONTROL_GetDataPtr( hControl );
	lpImageBox->uAttr = uAttr;
}

void atIMAGEBOX_SetAlign(atHCONTROL hControl, int align )
{
	atLPIMAGEBOX	lpImageBox = atCONTROL_GetDataPtr( hControl );
	lpImageBox->uAlign = align;
}

