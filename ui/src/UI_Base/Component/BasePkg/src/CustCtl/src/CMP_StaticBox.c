#include "A_UIENG.h"
#include "UI_Util.h"
#include "CMP_StaticBox.h"


typedef struct _tagatSTATICBOX {
//	int			uAlign;
	atU32		uAttr;
	atDWORD			nTitleIds;
	atTCHAR		szText[256];
} atSTATICBOX, * atLPSTATICBOX;


/*
typedef struct {
	atLPTSTR szText;
} _CreateParam;
*/

#define FONT_HEIGHT_GAP			1

static int StaticBoxProc( atHCONTROL hControl, int nMsg, long lParam1, long lParam2 );


atHCONTROL atSTATICBOX_Create( int nIndex, int nX, int nY, int nWidth, int nHeight, atLPTSTR szText )
{
	atHCONTROL		hControl;

	hControl = atCONTROL_Create( nIndex, nX, nY, nWidth, nHeight, CTL_PARENTTYPE_WINDOW, sizeof(atSTATICBOX), StaticBoxProc, (void *)szText );
	if( hControl == atNULL ) return atNULL;
	
	{
		atLPCONTROL 	lpControl = atCONTROL_GetPtr(hControl);
//		atLPSTATICBOX	lpStatic = atCONTROL_GetDataPtr( hControl );

		atCONTROL_SetAttr( hControl, atCONTROL_GetAttr(hControl) & ~atOBJATTR_EVENTTARGET );
		atCONTROL_SetFocusable( hControl, atFALSE );	// StaticBox는 기본적으로 포커스를 가지지 않는다.

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


atHCONTROL atSTATICBOX_CreateIDS( int nIndex, int nX, int nY, int nWidth, int nHeight, atLONG nTextIDS)
{
	atHCONTROL		hControl;
	atTCHAR szText[256];

	hControl = atCONTROL_Create( nIndex, nX, nY, nWidth, nHeight, CTL_PARENTTYPE_WINDOW , sizeof(atSTATICBOX), StaticBoxProc, atNULL);
	if( hControl == atNULL ) return atNULL;
	
	{
		atLPCONTROL 	lpControl = atCONTROL_GetPtr(hControl);
		atLPSTATICBOX	lpStaticBox = atCONTROL_GetDataPtr( hControl );

		atCONTROL_SetFocusable( hControl, atFALSE );	// StaticBox는 기본적으로 포커스를 가지지 않는다.

		lpStaticBox->nTitleIds = nTextIDS;

		atUIAPI_LoadString(nTextIDS, szText, 256);

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


static void atSTATICBOX_Init( atHCONTROL hControl, atLPTSTR param )
{
	atLPSTATICBOX	lpStaticBox = (atLPSTATICBOX)atCONTROL_GetDataPtr( hControl );
	atLPCONTROL lpControl = atCONTROL_GetPtr(hControl);

	lpStaticBox->uAttr  = atSTATICBOXATTR_OPAQUE | (ALIGN_LEFT_TOP & atSTATICBOXATTR_ALIGN_MASK);

	if(lpControl)
	{
		if(lpStaticBox->nTitleIds)
			atUIAPI_LoadString(lpStaticBox->nTitleIds, lpStaticBox->szText, 256);
		else
		{
			if(param)
			{
				UI_STRNCPY(lpStaticBox->szText, param, 256-1);
				lpStaticBox->szText[256-1] = _ATC('\0');
			}
		}
	}
	else
		lpStaticBox->szText[0] = 0;


	//UI_STRNCPY(lpStaticBox->szText, param, 256-1);
	//lpStaticBox->szText[256-1] = _ATC('\0');
}

static void atSTATICBOX_Destroy( atHCONTROL hControl )
{
//	atLPCONTROL lpControl = atCONTROL_GetPtr(hControl);
	
}

static void atSTATICBOX_Draw( atHGDC hCanvas, atHCONTROL hControl, atBOOL bFocused )
{
	atLPCONTROL lpControl = atCONTROL_GetPtr(hControl);
	atLPSTATICBOX	lpStaticBox;
	int				/*nAscent, */nHeight;
//	atTCHAR	szNewText[256+10];
	long color;
	atRECT rect;

	if ( lpControl->Base.bShow == atFALSE )	return;

	lpStaticBox = (atLPSTATICBOX)atCONTROL_GetDataPtr( hControl );

	atCONTROL_GetControlRect( hControl, &rect );

//	atUIAPI_SetFont( hCanvas, lpControl->font );
	nHeight = atUIAPI_GetFontHeight(atUIAPI_GetFont(hCanvas));

	if( atCONTROL_IsFocusable(hControl) ){
		if ( bFocused ){
			if( lpStaticBox->uAttr & atSTATICBOXATTR_OUTLINE )
				atUIAPI_DrawRect(hCanvas, 0, 0, RECT_WIDTH(rect), RECT_HEIGHT(rect), atUIAPI_GetRGB(0x00, 0x3E, 0x8B), 1);
			if( lpStaticBox->uAttr & atSTATICBOXATTR_OPAQUE )
				atUIAPI_FillRect(hCanvas, 1, 1, RECT_WIDTH(rect) - 2, RECT_HEIGHT(rect)- 2, atUIAPI_GetRGB(0xA2, 0xF8, 0xF8));
		} else {
			if( lpStaticBox->uAttr & atSTATICBOXATTR_OUTLINE )
				atUIAPI_DrawRect(hCanvas, 0, 0, RECT_WIDTH(rect), RECT_HEIGHT(rect), atUIAPI_GetRGB(76, 149, 248), 1);
			if( lpStaticBox->uAttr & atSTATICBOXATTR_OPAQUE )
				atUIAPI_FillRect(hCanvas, 1, 1, RECT_WIDTH(rect) - 2, RECT_HEIGHT(rect)- 2, atUIAPI_GetRGB(236, 255, 255));
		}
		if ( bFocused ){
			color = lpControl->tControlColor.clFocusedTxtColor;
		} else {
			color = lpControl->tControlColor.clNormalTxtColor;
		}
	} else {
		if( lpStaticBox->uAttr & atSTATICBOXATTR_OPAQUE )
			atUIAPI_FillRect(hCanvas, 0, 0, RECT_WIDTH(rect), RECT_HEIGHT(rect), atUIAPI_GetRGB(255, 255, 255));
		if( lpStaticBox->uAttr & atSTATICBOXATTR_OUTLINE )
			atUIAPI_DrawRect(hCanvas, 0, 0, RECT_WIDTH(rect), RECT_HEIGHT(rect), atUIAPI_GetRGB(76, 149, 248), 1);
		color = lpControl->tControlColor.clNormalTxtColor;
	}

	// to-do : resource가 변경되었을 경우에만, 문자열을 읽어 들이도록 수정 필요.
	if(lpStaticBox->nTitleIds)
	{
		atUIAPI_LoadString(lpStaticBox->nTitleIds, lpStaticBox->szText, 256);
	}
	
	rect.nEndX = RECT_WIDTH(rect)-1;
	rect.nEndY = RECT_HEIGHT(rect)-1;
	rect.nStartX = 2;  // 제일 왼쪽 글씨(ex 'V', 'A')가 짤려 보이기 때문에 간격을 2 정도 둠
	//rect.nStartX = 0;
	rect.nStartY = 0;
	atUIAPI_DrawStringAlignRect( hCanvas, 0, 0, &rect, 
		lpStaticBox->szText,
//		myStringCut(hCanvas, lpStaticBox->szText, szNewText, RECT_WIDTH(rect)-2, atTRUE), 
		lpStaticBox->uAttr & atSTATICBOXATTR_ALIGN_MASK, color, atNULL );
}


static int StaticBoxProc( atHCONTROL hControl, int nMsg, long lParam1, long lParam2 )
{
	atLPCONTROL lpControl = atCONTROL_GetPtr(hControl);
	int r = 0;

	switch( nMsg )
	{
		case CTLMSG_INIT:
			atSTATICBOX_Init( hControl, (atLPTSTR)lParam1);
			break;
		case CTLMSG_RELEASE:
			atSTATICBOX_Destroy( hControl );
			r = 1;
			break;
		case CTLMSG_PAINT:
			atSTATICBOX_Draw( atCONTROL_GetGDC(hControl), hControl, (atBOOL)lParam2 );
			r = 1;
			break;
	}

	return r;
}


//=============================================================================


atLPTSTR atSTATICBOX_GetText( atHCONTROL hControl )
{
	atLPSTATICBOX lpStaticBox = atCONTROL_GetDataPtr( hControl );
	if( lpStaticBox == atNULL ) return atNULL;

	return lpStaticBox->szText;
}

void atSTATICBOX_SetText( atHCONTROL hControl, atLPTSTR szNewText )
{
	atLPSTATICBOX lpStaticBox = atCONTROL_GetDataPtr( hControl );
	if( lpStaticBox == atNULL ) return;

atSTATICBOX_SetTextIDS(hControl, 0);
	
	UI_STRNCPY(lpStaticBox->szText, szNewText, 255);
	lpStaticBox->szText[255] = _ATC('\0');
}

void atSTATICBOX_SetTextIDS(atHCONTROL hControl, atDWORD nTextIds)
{
	atLPCONTROL   lpControl   = atCONTROL_GetDataPtr(hControl);
	atLPSTATICBOX lpStaticBox = atCONTROL_GetDataPtr(hControl);
	
	lpStaticBox->nTitleIds = nTextIds;

	atUIAPI_LoadString(nTextIds, lpStaticBox->szText, 255);
}

int atSTATICBOX_GetTextLength( atHCONTROL hControl )
{
	atLPTSTR str = atSTATICBOX_GetText(hControl);

	return (str != atNULL)? UI_STRLEN(str) : 0;
}

atU32 atSTATICBOX_GetAttr(atHCONTROL hControl )
{
	atLPSTATICBOX lpStaticBox = atCONTROL_GetDataPtr( hControl );
	if( lpStaticBox == atNULL ) return 0;

	return lpStaticBox->uAttr;
}

void atSTATICBOX_SetAttr(atHCONTROL hControl, atU32 uAttr )
{
	atLPSTATICBOX lpStaticBox = atCONTROL_GetDataPtr( hControl );
	if( lpStaticBox == atNULL ) return;

	lpStaticBox->uAttr = uAttr;
}

void atSTATICBOX_SetAlign(atHCONTROL hControl, int align )
{
	atLPSTATICBOX lpStaticBox = atCONTROL_GetDataPtr( hControl );
	if( lpStaticBox == atNULL ) return;

	lpStaticBox->uAttr &= ~atSTATICBOXATTR_ALIGN_MASK;
	lpStaticBox->uAttr |= align;
}

