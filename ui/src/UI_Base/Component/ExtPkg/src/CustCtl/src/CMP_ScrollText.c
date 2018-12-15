#include "A_UIENG.h"
#include "UI_Util.h"
#include "CMP_ScrollBarEx.h"
#include "CMP_ScrollText.h"




typedef struct _tagatSCROLLTEXT {
	int 			attr;

	atMARGIN	tMargin;
	atINT		nLineGap;
	
	int			nScrollPos;
	int			nItemHeight;
	int			nOneScreenCount;

	atUILIST		lpScrollTextList;
	atLPTSTR			lpText;

	atBOOL		bNeedScroll;
	atHCONTROL	hScrollBar;

	atBOOL		bMouseDown, bMouseScroll;
	atBOOL		bActionValidate; 
	int			nFirstViewTopY;
	int			nFirstMouseX, nFirstMouseY;
	int			nPrevMouseX, nPrevMouseY;
	atBOOL		bIsChange;
	
	atBOOL		bReadySmoothScroll;
	atBOOL		bUseSmoothScroll;		// 스무스 스크롤 사용여부
} atSCROLLTEXT,  *atLPSCROLLTEXT;

struct  _CreateParam {
	atDWORD	  dwAttr;
	//atHBITMAP lpScrollbarImgTop, lpScrollbarImage, lpScrollbarImgBottom, lpScrollcursorImage;
	atHBITMAP lpScrollbarImgTop, lpScrollbarImage, lpScrollbarImgBottom;
	atHBITMAP lpScrollCursorImgTop, lpScrollCursorImgMid, lpScrollCursorImgBot;
};


//static void         atSCROLLTEXT_OnInit( atHCONTROL hControl, struct _CreateParam *CreateParam );
//static void         atSCROLLTEXT_OnDestroy( atHCONTROL hControl );
//static void         atSCROLLTEXT_OnDraw( atHGDC hCanvas, atHCONTROL hControl, atBOOL bFocused );
static int ScrollTextProc( atHCONTROL hControl, int nMsg, long lParam1, long lParam2 );


static void drawfunc( void *data, int stidx, int len, int y, atLPTSTR str );

static atCOLOR fontcolor;

static unsigned char get2v( atLPTSTR str )
{
	int i;
	atTCHAR cl;
	int v = 0;

	for( i = 0; i < 2; i ++ )
	{
		cl = *str++;
		if( cl >= _ATC('0') && cl <= _ATC('9') ) cl -= _ATC('0');
		else if( cl >= _ATC('a') && cl <= _ATC('f') ) cl = cl - _ATC('a') + 10;
		else if( cl >= _ATC('A') && cl <= _ATC('F') ) cl = cl - _ATC('A') + 10;
		v = (v << 4) | cl;
	}

	return v;
}

/*
static void HTML_DrawString( atHGDC hCanvas, int x, int y, atLPTSTR str, int len )
{
	int i, j;
	atTCHAR strbuff[256], *buff, *last;
	atTCHAR tt[128];
	int idx;
	atTCHAR *pt, ch;
	unsigned char *pp;
	unsigned char r, g, b;
//	atCOLOR color;

	idx = 0;
    j = 0;
    i = 0;

	buff = strbuff;
	last = str + len - 1;

    while(*str != '\0' && str <= last){
		ch = *str++;

		if( ch == '<' ){
			pt = atUIAPI_strstr( str, ">" );
			if( pt != atNULL ){
				*buff = '\0';
				atUIAPI_DrawStringEx( hCanvas, x, y, strbuff, UI_STRLEN(strbuff), fontcolor );
				x += atUIAPI_GetStringWidth( atUIAPI_GetFont(hCanvas), strbuff, -1 );
				buff = strbuff;

				atUIAPI_strncpy( tt, str, pt-str );
				tt[pt-str] = '\0';

				if( atUIAPI_strncmp(tt, "font", sizeof("font")-1) == 0 ){
					pp = (unsigned char *)atUIAPI_strstr( tt, "color=#" );
					if( pp != atNULL )
					{
						pp += sizeof("color=#")-1;
						r = get2v(pp);
						g = get2v(pp+2);
						b = get2v(pp+4);
						fontcolor = atUIAPI_GetRGB(r,g,b);
					}
				} else if( atUIAPI_strncmp(tt, "/font", sizeof("/font")-1) == 0 ){
					fontcolor = UI_COLOR_BLACK;
				}

				str = pt + 1;
				ch = 0;
			}
		}

		if( ch != 0 )
		{
			*buff++ = ch;
		}

		if( *str == '\0' || str > last)
		{
			*buff = '\0';
		    atUIAPI_DrawStringEx( hCanvas, x, y, strbuff, UI_STRLEN(strbuff), fontcolor );
			x += atUIAPI_GetStringWidth( atUIAPI_GetFont(hCanvas), strbuff, -1 );
			buff = strbuff;
		}
    } 
}
*/

/*
static int HTML_AdjustStr( atHGDC hCanvas, atUILIST lpList, int swidth, int sheight, char *src_str )
{
	int size, total_len;
	char strbuff[256], *buff;
	int idx, stidx;
	char *pt, ch;
	char *str = src_str, *str0;
	int len;

    total_len = strlen(str);

	idx = 0;
	stidx = 0;
	while( *str != '\0' )
	{
		buff = strbuff;
		str0 = str;
		len = 0;

		while( *str != '\0' )
		{
			ch = *str++;

			if( ch == '<' )
			{
				pt = strstr( str, ">" );
				if( pt != atNULL )
				{
					len += pt - str + 2;
					str = pt + 1;
					ch = 0;
					if( (len >= 3) && (strncmp(str-3, "<P>", 3) == 0) )
					{
						len -= 3;
						break;
					}
				}
			}
			else if( ch == '\n' )
			{
				if( (*str != '\0') && (*str == '\r') ) str++;
				break;
			}
			else if( ch == '\r' )
			{
				if( (*str != '\0') && (*str == '\n') ) str++;
				break;
			}
			
			if( ch != 0 )
			{
				int chlen = 0;

				*buff++ = ch; chlen++;
				// 한글일 경우 한문자를 더 넣는다.
				if( (unsigned char)ch & 0x80 )
				{
					ch = *str++;
					if( ch != '\0' )
					{
						*buff++ = ch; chlen++;
					}
				}

				*buff = '\0';
				size = atUIAPI_GetStringWidth( atUIAPI_GetFont(hCanvas), strbuff, -1);
				if( size > swidth ){ str -= chlen; break; }
				len += chlen;
				if( size == swidth ) break;
			}
		} 

		drawfunc( atNULL, str0-src_str, len, idx, strbuff );
		idx ++;
    } 

	return idx;
}
*/


atHCONTROL atSCROLLTEXT_Create( int nIndex, int nX, int nY, int nWidth, int nHeight, int attr, 
							     //atHBITMAP lpScrollbarImgTop, atHBITMAP lpScrollbarImage, atHBITMAP lpScrollbarImgBottom, atHBITMAP lpScrollcursorImage )
								 atHBITMAP lpScrollbarImgTop, atHBITMAP lpScrollbarImage, atHBITMAP lpScrollbarImgBottom,
								 atHBITMAP lpScrollCursorImgTop, atHBITMAP lpScrollCursorImgMid, atHBITMAP lpScrollCursorImgBot )
{
	atHCONTROL		hControl;
	struct _CreateParam param;

	param.dwAttr = (atDWORD)attr;
	param.lpScrollbarImgTop	= lpScrollbarImgTop;
	param.lpScrollbarImage	= lpScrollbarImage;
	param.lpScrollbarImgBottom	= lpScrollbarImgBottom;
	//param.lpScrollcursorImage	= lpScrollcursorImage;
	param.lpScrollCursorImgTop	= lpScrollCursorImgTop;
	param.lpScrollCursorImgMid	= lpScrollCursorImgMid;
	param.lpScrollCursorImgBot	= lpScrollCursorImgBot;

	hControl = atCONTROL_Create( nIndex, nX, nY, nWidth, nHeight, CTL_PARENTTYPE_WINDOW, sizeof(atSCROLLTEXT), ScrollTextProc, &param );

	return hControl;
}


static void atSCROLLTEXT_OnInit( atHCONTROL hControl, struct _CreateParam *param )
{
	atLPCONTROL 	lpControl = atCONTROL_GetPtr(hControl);
	atLPSCROLLTEXT lpScrollText = (atLPSCROLLTEXT)atCONTROL_GetDataPtr( hControl );
	int	nTextHeight;
	int nWidth, nHeight;
	int sw;

	lpScrollText->attr	= param->dwAttr;

	SET_MARGIN( lpScrollText->tMargin, 0, 0, 0, 0 );
	lpScrollText->nLineGap = 0;
	
	lpScrollText->lpScrollTextList = atUIAPI_CreateList();
	lpScrollText->lpText = atNULL;

	lpScrollText->nScrollPos			= 0;

	nWidth = RECT_WIDTH(lpControl->Base.rect);
	nHeight = RECT_HEIGHT(lpControl->Base.rect);

	nTextHeight = atUIAPI_GetFontHeight( lpControl->hFont );
	lpScrollText->nOneScreenCount	=	nHeight / nTextHeight;		

	lpScrollText->bNeedScroll = atFALSE;
	if( param->lpScrollbarImgTop )
		sw = atUIAPI_GetBitmapWidth(param->lpScrollbarImgTop);
	else if( param->lpScrollCursorImgTop )
		sw = atUIAPI_GetBitmapWidth(param->lpScrollCursorImgTop);
	else
		sw = 8;
	lpScrollText->hScrollBar = atSCROLLBAR_EXT_Create( 100, nWidth-sw, 0, sw, nHeight, 
                              						atSCROLLBAR_EXT_TYPE_WINDOW_FRICTION_STICK, 0, 0, 0, 0,
						  						   param->lpScrollbarImgTop, param->lpScrollbarImage, param->lpScrollbarImgBottom, 
						  						   //param->lpScrollcursorImage );
												   param->lpScrollCursorImgTop, param->lpScrollCursorImgMid, param->lpScrollCursorImgBot );
	atCONTROL_SetAnchor( lpScrollText->hScrollBar, 0, 0, atANCHOR_IGNORE, 0 );
	atCONTROL_SetAttr( lpScrollText->hScrollBar, atCONTROL_GetAttr(lpScrollText->hScrollBar) | CTL_PARENTTYPE_CONTROL );
	atCONTROL_Enable( lpScrollText->hScrollBar, atFALSE );
	atCONTROL_AddChildControl( hControl, lpScrollText->hScrollBar );
}

static void atSCROLLTEXT_OnDestroy( atHCONTROL hControl )
{	
	atLPCONTROL 	lpControl = atCONTROL_GetPtr(hControl);
	atLPSCROLLTEXT lpScrollText = (atLPSCROLLTEXT)atCONTROL_GetDataPtr( hControl );

	atUIAPI_DestroyList(lpScrollText->lpScrollTextList, atTRUE);
	if( lpScrollText->lpText ) UI_MEMFREE( lpScrollText->lpText );

	atCONTROL_Destroy( lpScrollText->hScrollBar );
}


static void atSCROLLTEXT_OnDraw( atHGDC hCanvas, atHCONTROL hControl, atBOOL bFocused )
{
	atLPCONTROL lpControl = atCONTROL_GetPtr(hControl);
	atLPSCROLLTEXT lpScrollText = (atLPSCROLLTEXT)atCONTROL_GetDataPtr( hControl );
	int pos;
	int *itm;
	int x, y, w, h, fh;
//	atCOLOR txt_color;
	atTCHAR *str;
	int stidx, len;
	atRECT rect;
	atREGION rgn2;
	int fnt_height, scroll_y;

//	atUIAPI_SetFont( hCanvas, lpControl->font );
	
//	MC_grpSetContext(&g_cxt, MC_GRP_CONTEXT_FONT_IDX, (void*)lpControl->font);
//	nAscent = MC_grpGetFontAscent(lpControl->font);
	fnt_height = atUIAPI_GetFontHeight( atCONTROL_GetFont(hControl) );

	x = 0;
	y = 0; 
	w = RECT_WIDTH(lpControl->Base.rect);
	h = RECT_HEIGHT(lpControl->Base.rect);

	if ( atCONTROL_IsVisible(lpScrollText->hScrollBar) ){
		atCONTROL_GetControlRegion( lpScrollText->hScrollBar, &rgn2 );
		w -= rgn2.nWidth;
	}

	atUIAPI_FillRect(hCanvas, x, y, w, h, lpControl->tControlColor.clNormalBgColor);  // 배경을 지운다.
	if( lpControl->Base.dwAttr & atCS_DRAW_BORDER ){
		atUIAPI_DrawRect(hCanvas, x, y, w, h, lpControl->tControlColor.clNormalFrColor, 1);  // 배경을 지운다.
		x ++; y ++; w -= 2; h -= 2;
	}

	if( lpScrollText->lpText == atNULL )  return;

	x += lpScrollText->tMargin.nLeft;
	y += lpScrollText->tMargin.nTop;
	w -= lpScrollText->tMargin.nLeft + lpScrollText->tMargin.nRight;
	h -= lpScrollText->tMargin.nTop + lpScrollText->tMargin.nBottom;

	scroll_y = atSCROLLBAR_EXT_GetPosition( lpScrollText->hScrollBar );

	SET_RECT( rect, x, y, x+w, y+h );
	atUIAPI_SetClipRect( hCanvas, &rect );

    fh = fnt_height + lpScrollText->nLineGap;

	pos = scroll_y / fh;
	y -= scroll_y % fh;
	if( pos < 0 ){
		y += (-pos) * fh;
		pos = 0;
	}

	itm = (int *)atUIAPI_GetListAt( lpScrollText->lpScrollTextList, pos );

	fontcolor = lpControl->tControlColor.clNormalTxtColor;
	str = lpScrollText->lpText;

	while( itm != atNULL ){
		stidx = itm[0];
		len = itm[1];

		if( lpScrollText->attr == 0 )
			atUIAPI_DrawStringEx( hCanvas, x, y, &str[stidx], len, UI_COLOR_BLACK, 0 );
//		else if( lpScrollText->attr == 1 )
//			HTML_DrawString( hCanvas, x, y, &str[stidx], len );

		y += fh;
		if( y >= h ) break;

		itm = (int *)atUIAPI_GetListNext( lpScrollText->lpScrollTextList, &pos );
	}
}


static int atSCROLLTEXT_OnMouseDown( atHCONTROL hControl, atINT nButton, atINT nX, atINT nY )
{
	int r = 0;
	atLPSCROLLTEXT lpScrollText = (atLPSCROLLTEXT)atCONTROL_GetDataPtr( hControl );
	
	if( atSCROLLBAR_EXT_IsProcessGestureScroll(lpScrollText->hScrollBar) ){
		atSCROLLBAR_EXT_StopAndResetGestureScroll(lpScrollText->hScrollBar);
		lpScrollText->bActionValidate = atFALSE;
	}

	if( !atCONTROL_IsVisible(lpScrollText->hScrollBar) ) return 0;

	lpScrollText->nFirstViewTopY = atSCROLLBAR_EXT_GetPosition(lpScrollText->hScrollBar);
	lpScrollText->nFirstMouseY = nY;
	lpScrollText->bActionValidate = atTRUE;
	
	atSCROLLBAR_EXT_StartGestureScroll( lpScrollText->hScrollBar, lpScrollText->nFirstMouseY );
	
	return 0;
}

static int atSCROLLTEXT_OnMouseMove( atHCONTROL hControl, atINT nButton, atINT nX, atINT nY )
{
	int r = 0;
	atLPSCROLLTEXT lpScrollText = (atLPSCROLLTEXT)atCONTROL_GetDataPtr( hControl );
	
	if( lpScrollText->bActionValidate ){
		atSCROLLBAR_EXT_ProcessGestureScroll( lpScrollText->hScrollBar, nY );
	}
	
	return 0;
}

static int atSCROLLTEXT_OnMouseUp( atHCONTROL hControl, atINT nButton, atINT nX, atINT nY )
{
	int r = 0;
	atLPSCROLLTEXT lpScrollText = (atLPSCROLLTEXT)atCONTROL_GetDataPtr( hControl );
	
	if( lpScrollText->bActionValidate ){
		atSCROLLBAR_EXT_EndGestureScroll( lpScrollText->hScrollBar, nY );
		lpScrollText->bActionValidate = atFALSE;
	}
	
	return 0;
}




//static atHGDC dc;
//static atUILIST lst;
static atHFONT font;
static int strwidth( void *data, atLPTSTR buff, int len )
{
	return atUIAPI_GetStringWidth( font, buff, len );
}
static void drawfunc( void *data, int stidx, int len, int y, atLPTSTR str )
{
/*
	char *itm;
	int len;

	len = strlen(str);
	itm = (char *)UI_MEMALLOC( len+1 );
	strcpy( itm, str );
	atUIAPI_AddListTail( lst, itm );
*/	
	atUILIST lst = (atUILIST)data;
	int *itm = (int *)UI_MEMALLOC( sizeof(int)*2 );
	itm[0] = stidx;
	itm[1] = len;
	atUIAPI_AddListTail( lst, itm );
}
int atSCROLLTEXT_Recalc(atHCONTROL hControl, int sidx, atBOOL IsAll)
{
	atLPCONTROL lpControl = atCONTROL_GetPtr(hControl);
	atLPSCROLLTEXT lpScrollText = (atLPSCROLLTEXT)atCONTROL_GetDataPtr( hControl );
	//atHGDC 	hCanvas = atWINDOW_GetClientGDC(atCONTROL_GetParentWindow(hControl));
	atHFONT hFont;
	int scr_width, scr_height, fnt_height, txt_height;
	atLPCONTROL		lpControl2;
	int len;
	

//	atUIAPI_SetFont( hCanvas, lpControl->font );
	hFont = atCONTROL_GetFont( hControl );

	scr_width = RECT_WIDTH(lpControl->Base.rect);
	scr_height = RECT_HEIGHT(lpControl->Base.rect);

//	if ( atCONTROL_IsVisible(lpScrollText->hScrollBar) )
	if( lpScrollText->bNeedScroll )
	{
		lpControl2 = atCONTROL_GetPtr(lpScrollText->hScrollBar);
		scr_width -= RECT_WIDTH(lpControl2->Base.rect);
	}

	scr_width -= lpScrollText->tMargin.nLeft + lpScrollText->tMargin.nRight;

	if( scr_width <= 0 || scr_height <= 0 ){
		lpScrollText->nOneScreenCount = 0;
		return 0;
	}

	if( IsAll ){
		atUIAPI_RemoveListAll( lpScrollText->lpScrollTextList, atTRUE );
	}

	if( lpScrollText->lpText == atNULL ) return 0;

	font = hFont;
	len = UI_STRLEN(lpScrollText->lpText);
	if( lpScrollText->attr == 0 )
		myAdjustStr( scr_width, 0, lpScrollText->lpText, sidx, (void *)lpScrollText->lpScrollTextList, strwidth, drawfunc );
//	else if( lpScrollText->attr == 1 )
//		HTML_AdjustStr( hCanvas, lpScrollText->lpScrollTextList, scr_width, 0, text );

	fnt_height = atUIAPI_GetFontHeight(hFont);
	if( fnt_height > 0 ){
		lpScrollText->nOneScreenCount	=	scr_height / fnt_height;		
	} else {
		lpScrollText->nOneScreenCount	=	0;		
	}

	txt_height = (fnt_height + lpScrollText->nLineGap) * atUIAPI_GetListCount(lpScrollText->lpScrollTextList) - lpScrollText->nLineGap;
	txt_height += lpScrollText->tMargin.nTop + lpScrollText->tMargin.nBottom;

	if( txt_height > scr_height ){
		atSCROLLBAR_EXT_SetSize( lpScrollText->hScrollBar, txt_height );
		atCONTROL_SetVisible( lpScrollText->hScrollBar, atTRUE );
		atCONTROL_Enable( lpScrollText->hScrollBar, atTRUE );
		lpScrollText->bNeedScroll = atTRUE;
	} else {
		atCONTROL_SetVisible( lpScrollText->hScrollBar, atFALSE );
		atCONTROL_Enable( lpScrollText->hScrollBar, atFALSE );
		lpScrollText->bNeedScroll = atFALSE;
	}
/*
	if( atUIAPI_GetListCount(lpScrollText->lpScrollTextList) > lpScrollText->nOneScreenCount){
		atSCROLLBAR_EXT_SetSize( lpScrollText->hScrollBar, atUIAPI_GetListCount(lpScrollText->lpScrollTextList) - lpScrollText->nOneScreenCount + 1);
		atCONTROL_SetVisible( lpScrollText->hScrollBar, atTRUE );
		atCONTROL_Enable( lpScrollText->hScrollBar, atTRUE );
	} else {
		atCONTROL_SetVisible( lpScrollText->hScrollBar, atFALSE );
		atCONTROL_Enable( lpScrollText->hScrollBar, atFALSE );
	}
*/

	lpScrollText->nScrollPos = 0;
	atSCROLLBAR_EXT_SetPosition( lpScrollText->hScrollBar, 0 );

	return atUIAPI_GetListCount( lpScrollText->lpScrollTextList );
}

int atSCROLLTEXT_AddNewlineText(atHCONTROL hControl, atLPTSTR text)
{
	atLPCONTROL lpControl = atCONTROL_GetPtr(hControl);
	atLPSCROLLTEXT lpScrollText = (atLPSCROLLTEXT)atCONTROL_GetDataPtr( hControl );
	int len0, len;

	len = UI_STRLEN(text);
	if( lpScrollText->lpText ){
		atTCHAR *pNewText;
		len0 = UI_STRLEN( lpScrollText->lpText );
		pNewText = (atTCHAR *)UI_MEMALLOC( (len0 + len + 2 + 1) * sizeof(atTCHAR) );
		if( pNewText == atNULL ) return 0;
		atUIAPI_memcpy( pNewText, lpScrollText->lpText, len0 * sizeof(atTCHAR) );
		pNewText[len0+0] = _ATC('\r');
		pNewText[len0+1] = _ATC('\n');
		pNewText[len0+2] = _ATC('\0');
		UI_MEMFREE(lpScrollText->lpText);
		lpScrollText->lpText = pNewText;
		len0 += 2;
	} else {
		len0 = 0;
		lpScrollText->lpText = (atTCHAR *)UI_MEMALLOC( (len + 1) * sizeof(atTCHAR) );
		if( lpScrollText->lpText == atNULL )  return 0;
		lpScrollText->lpText[0] = _ATC('\0');
	}
	UI_STRCAT( lpScrollText->lpText, text );

	return atSCROLLTEXT_Recalc(hControl, len0, atFALSE);
}

int atSCROLLTEXT_SetText(atHCONTROL hControl, atLPTSTR text)
{
	atLPCONTROL lpControl = atCONTROL_GetPtr(hControl);
	atLPSCROLLTEXT lpScrollText = (atLPSCROLLTEXT)atCONTROL_GetDataPtr( hControl );
//	atLPCONTROL		lpControl2;
//	int scr_width, scr_height, txt_height;
	atHGDC 	hCanvas = atWINDOW_GetClientGDC(atCONTROL_GetParentWindow(hControl));
//	int len;

	atUIAPI_RemoveListAll( lpScrollText->lpScrollTextList, atTRUE );
	if( lpScrollText->lpText ){
		UI_MEMFREE( lpScrollText->lpText );
		lpScrollText->lpText = atNULL;
	}

	return atSCROLLTEXT_AddNewlineText( hControl, text );
}

atLPTSTR atSCROLLTEXT_GetText(atHCONTROL hControl)
{
	atLPSCROLLTEXT lpScrollText = (atLPSCROLLTEXT)atCONTROL_GetDataPtr( hControl );

	return lpScrollText->lpText;
}

void		atSCROLLTEXT_SetMargin( atHCONTROL hControl, atINT nMarginTop, atINT nMarginBottom, atINT nMarginLeft, atINT nMarginRight )
{
	atLPSCROLLTEXT lpScrollText = (atLPSCROLLTEXT)atCONTROL_GetDataPtr( hControl );

	SET_MARGIN( lpScrollText->tMargin, nMarginTop, nMarginBottom, nMarginLeft, nMarginRight );
}

void		atSCROLLTEXT_SetLineGap( atHCONTROL hControl, atINT nLineGap )
{
	atLPSCROLLTEXT lpScrollText = (atLPSCROLLTEXT)atCONTROL_GetDataPtr( hControl );

	lpScrollText->nLineGap = nLineGap;
}

void atSCROLLTEXT_Scroll(atHCONTROL hControl, int nDirect)
{
	atLPCONTROL	lpControl = atCONTROL_GetPtr(hControl);
	atLPSCROLLTEXT lpScrollText = (atLPSCROLLTEXT)atCONTROL_GetDataPtr(hControl);
	int cnt;

//	cnt = atUIAPI_GetListCount(lpScrollText->lpScrollTextList) - lpScrollText->nOneScreenCount + 1;
	if( atCONTROL_IsEnable(lpScrollText->hScrollBar) )
		cnt = atSCROLLBAR_EXT_GetSize(lpScrollText->hScrollBar);
	else
		cnt = 0;
	
	if( cnt <= 0 ) return;

	lpScrollText->nScrollPos += nDirect;
	if(nDirect > 0)
	{
		if(lpScrollText->nScrollPos > cnt-1)
		{
			lpScrollText->nScrollPos = cnt-1;
		}
	}
	else if(nDirect < 0)
	{
		if(lpScrollText->nScrollPos < 0)
		{	
			lpScrollText->nScrollPos = 0;		
		}
	}

	atSCROLLBAR_EXT_SetPosition( lpScrollText->hScrollBar, lpScrollText->nScrollPos );
}


static int ScrollTextProc( atHCONTROL hControl, int nMsg, long lParam1, long lParam2 )
{
	atLPCONTROL lpControl = atCONTROL_GetPtr(hControl);
	atLPSCROLLTEXT lpScrollText	= (atLPSCROLLTEXT)atCONTROL_GetDataPtr( hControl );
//	atREGION region;
	atCONTROL_COLOR cl;
	int x, y;
	int r = 0;

	switch( nMsg ){
		case CTLMSG_INIT:
			atSCROLLTEXT_OnInit( hControl, (struct _CreateParam *)lParam1 );
			break;
		case CTLMSG_RELEASE:
			atSCROLLTEXT_OnDestroy( hControl );
			r = 1;
			break;
		case CTLMSG_RESIZE:
			atSCROLLTEXT_Recalc( hControl, 0, atTRUE );
			r = 1;
			break;
		case CTLMSG_CHANGE:
			if( lParam1 == 100 ) {	// CMP_ScrollBarEx
				//_SetScrollPos( hControl, lParam2, atFALSE );
				atCONTROL_Refresh( hControl );
				r = 1;
			}
			break;
		case CTLMSG_PAINT:
			atSCROLLTEXT_OnDraw( atCONTROL_GetGDC(hControl), hControl, (atBOOL)lParam2 );
			r = 1;
			break;
		case CTLMSG_KEYDOWN:
			switch( lParam1 ){
				case atVKEY_SIDE1:
					atSCROLLTEXT_Scroll(hControl, -lpScrollText->nOneScreenCount);
					atCONTROL_Refresh(hControl);
					r = 1;
					break;
				case atVKEY_SIDE2:
					atSCROLLTEXT_Scroll(hControl, lpScrollText->nOneScreenCount);
					atCONTROL_Refresh(hControl);
					r = 1;
					break;
				case atVKEY_REW:
				case atVKEY_UP:
					atSCROLLTEXT_Scroll(hControl, -1);
					atCONTROL_Refresh(hControl);
					r = 1;
					break;
				case atVKEY_FF:
				case atVKEY_DOWN:
					atSCROLLTEXT_Scroll(hControl, 1);
					atCONTROL_Refresh(hControl);
					r = 1;
					break;
			}
		case CTLMSG_MOUSEDOWN:
			x = (short)(lParam2 >> 16);
			y = (short)(lParam2 & 0xffff);
			atSCROLLTEXT_OnMouseDown( hControl, lParam1, x, y );
			break;
		case CTLMSG_MOUSEMOVE:
			x = (short)(lParam2 >> 16);
			y = (short)(lParam2 & 0xffff);
			atSCROLLTEXT_OnMouseMove( hControl, lParam1, x, y );
			break;
		case CTLMSG_MOUSEUP:
			x = (short)(lParam2 >> 16);
			y = (short)(lParam2 & 0xffff);
			atSCROLLTEXT_OnMouseUp( hControl, lParam1, x, y );
			break;
		case CTLMSG_CHANGELAYOUT:
			atCONTROL_GetControlColor( hControl, &cl );
			atCONTROL_SetControlColor( lpScrollText->hScrollBar, &cl );
			break;
			break;
	}

	return r;
}
