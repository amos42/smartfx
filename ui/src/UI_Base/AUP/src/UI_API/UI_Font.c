/**
  @file UI_Font.c
  @brief Font에 관한 API
  @remark Font에 관한 API 모음
  @author Amos
  @date 2010-04-19 오전 11:41:00
  @see UI_Font.h
*/
#include "UI_Types.h"
#include "UI_API/UI_API.h"
#include "OEM_UI_Font.h"
#include "UI_Util.h"
#include <stdio.h>
#include <string.h>


typedef struct tagatUI_FONT {
	atTCHAR			szFontName[32];
	atINT			nFontSize;
	atDWORD			dwFontStyle;
	atBOOL			bIsStock;

	atINT			nRefCount;
	atUI_OEM_FONT	lpOemFont;
} atUI_FONT, *atLPUI_FONT;


#define		MAX_STOCK_FONT	10

static atHFONT	stock_font[MAX_STOCK_FONT];
static atHFONT font_list[ atUIAPI_MAX_FONT_COUNT ];

static void _reset_handle(void)
{
	int i;
	for( i = 0; i < atUIAPI_MAX_FONT_COUNT; i++ ){
		font_list[i] = atNULL;
	}
}

static int _get_free_handle_idx(void)
{
	int i;
	for( i = 0; i < atUIAPI_MAX_FONT_COUNT; i++ ){
		if( font_list[i] == atNULL ) return i;
	}
	return -1;
}

static atHFONT _get_handle( int idx )
{
	if( idx < 0 || idx >= atUIAPI_MAX_FONT_COUNT ) return atNULL;
	return font_list[idx];
}

static atBOOL _set_handle( int idx, atHFONT hFont )
{
	if( idx < 0 || idx >= atUIAPI_MAX_FONT_COUNT ) return atFALSE;
	font_list[idx] = hFont;
	return atTRUE;
}

static void _delete_handle( atHFONT hFont )
{
	int i;
	for( i = 0; i < atUIAPI_MAX_FONT_COUNT; i++ ){
		if( font_list[i] == hFont ){
			font_list[i] = atNULL;
			break;
		}
	}
}


//static atHANDLE g_SysHandle;
//static atTCHAR *g_pFontPath, *g_pFontName;


static OEM_UI_FONT_T tOemFn;

atVOID	OEM_UI_Font_SetOemFn( OEM_UI_FONT_T *fnOem )
{
	tOemFn = *fnOem;
}


atVOID	OEM_UI_Font_GetOemFn( OEM_UI_FONT_T *fnOem )
{
	*fnOem = tOemFn;
}


/** 
 @brief Font 시스템을 초기화 한다. (내부용)
 
 Font 시스템을 초기화 한다.

 @param lSysInst [in] : UI엔진의 인스턴스 (OEM 전달용. 내부용)
 @param lInitData1, lInitData2 [in] : 초기화 파라미터 (OEM 전달용. 내부용)
 @return 성공하면 atTRUE, 실패하면 atFALSE
 @remark 내부용으로 사용된다.
*/
void 		atUIAPI_InitFont( atLONG lSysInst, atLONG lInitData1, atLONG lInitData2 )
{
	atHFONT hFont;

	if( tOemFn.fnInitFontSystem != atNULL ){
		tOemFn.fnInitFontSystem( lSysInst, lInitData1, lInitData2 );	
	}

	_reset_handle();

	stock_font[0] = atUIAPI_CreateFont( atUIAPI_FACE_SYSTEM, atUIAPI_SIZE_MEDIUM, atUIAPI_STYLE_PLAIN, atTRUE );
	stock_font[1] = atUIAPI_CreateFont( atUIAPI_FACE_SYSTEM, atUIAPI_SIZE_MEDIUM, atUIAPI_STYLE_BOLD, atTRUE );
	stock_font[2] = atUIAPI_CreateFont( atUIAPI_FACE_SYSTEM, atUIAPI_SIZE_SMALL, atUIAPI_STYLE_PLAIN, atTRUE );
	
	stock_font[3] = atUIAPI_CreateFont( atUIAPI_FACE_SYSTEM, atUIAPI_SIZE_SMALL, atUIAPI_STYLE_BOLD, atTRUE );
	stock_font[4]  = atUIAPI_CreateFont( atUIAPI_FACE_SYSTEM, atUIAPI_SIZE_LARGE, atUIAPI_STYLE_PLAIN, atTRUE );
	stock_font[5] = atUIAPI_CreateFont( atUIAPI_FACE_SYSTEM, atUIAPI_SIZE_LARGE, atUIAPI_STYLE_BOLD, atTRUE );
	stock_font[6] = atUIAPI_CreateFont( atUIAPI_FACE_SYSTEM, atUIAPI_SIZE_TINY, atUIAPI_STYLE_PLAIN, atTRUE );	
	stock_font[7] = atUIAPI_CreateFont( atUIAPI_FACE_SYSTEM, atUIAPI_SIZE_TINY, atUIAPI_STYLE_BOLD, atTRUE );
	
	stock_font[8] = atUIAPI_CreateFont( atUIAPI_FACE_SYSTEM, atUIAPI_SIZE_SMALL+2, atUIAPI_STYLE_BOLD, atTRUE );	
	stock_font[9] = atUIAPI_CreateFont( atUIAPI_FACE_SYSTEM, atUIAPI_SIZE_MEDIUM+2, atUIAPI_STYLE_BOLD, atTRUE );	
}


/** 
 @brief 
 
 ......

 @param 
 @return 없음
*/
void			atUIAPI_FinalFont(void)
{
	atUINT i;
	atHFONT hFont;
	
	for( i = 1; i < MAX_STOCK_FONT; i ++ ){
		if(stock_font[i] &&  stock_font[i] != stock_font[0]) {
			atUIAPI_DestroyFont( stock_font[i] );
		}
	}
	atUIAPI_DestroyFont( stock_font[0] );

	for( i = 0; i < atUIAPI_MAX_FONT_COUNT; i++ ){
		hFont = _get_handle(i);
		if( hFont ){
			atUIAPI_DestroyFont( hFont );
		}
	}

	_reset_handle();
	
	if( tOemFn.fnCloseFontSystem != atNULL ){
		tOemFn.fnCloseFontSystem();
	}
}


/** 
 @brief 
 
 ......

 @param 
 @return 없음
*/
static atHFONT atUIAPI_GetFontHandle(atLPUI_FONT lpFont)
{
	return (atHFONT)lpFont;
}


/** 
 @brief 
 
 ......

 @param 
 @return 없음
*/
static atLPUI_FONT atUIAPI_GetFontPtr(atHFONT hFont)
{
	return (atLPUI_FONT)hFont;
}


/** 
 @brief 
 
 ......

 @param 
 @return 없음
*/
OEM_UI_FONT atUIAPI_GetOEMPtr(atHFONT hFont)
{
	atLPUI_FONT lpFont = atUIAPI_GetFontPtr(hFont);
	if(lpFont == atNULL) {
		return (OEM_UI_FONT)atNULL;
	}

	return lpFont->lpOemFont;
}


/** 
 @brief 
 
 ......

 @param 
 @return 없음
*/
atHFONT 	atUIAPI_CreateFont( atLPTSTR font_face, int font_size, int font_style, atBOOL IsStock )
{
	atLPUI_FONT	lpFont;
	atHFONT hFont;
	int idx;

	idx = _get_free_handle_idx();
	if( idx < 0 ) return atNULL;
	
	lpFont = (atLPUI_FONT)UI_MEMALLOC(sizeof(atUI_FONT));
	if(lpFont == atNULL) {
		atUIAPI_DebugPrintf(_AT("Error atUIAPI_CreateFont\n"));
		return atNULL;
	}
	
	atUIAPI_strncpy( lpFont->szFontName, font_face, 32-1 );
	lpFont->nFontSize = font_size;
	lpFont->dwFontStyle = (atDWORD)font_style;
	lpFont->bIsStock = IsStock;
	
	lpFont->nRefCount = (IsStock)? 0xEFFF : 0;
	if( tOemFn.fnCreateFont != atNULL ){
		lpFont->lpOemFont = tOemFn.fnCreateFont( font_face, font_size, font_style, atTRUE );
	} else {
		lpFont->lpOemFont = atNULL;
	}
	if(!lpFont->lpOemFont)
	{
		SYSAPI_DebugPrintf(_AT("Error OEM_CreateFont\n"));
		UI_MEMFREE(lpFont);
		return atNULL;
	}
	
	hFont = atUIAPI_GetFontHandle(lpFont);
	
	_set_handle(idx, hFont);

	return hFont;
}


/** 
 @brief 
 
 ......

 @param 
 @return 없음
*/
void  atUIAPI_DestroyFont( atHFONT hFont )
{
	atLPUI_FONT	lpFont = atUIAPI_GetFontPtr(hFont);

	if( lpFont == atNULL ) return;

	if( tOemFn.fnDestroyFont != atNULL ){
		tOemFn.fnDestroyFont( lpFont->lpOemFont );
	}

	UI_MEMFREE( lpFont );

	_delete_handle(hFont);
}


/** 
 @brief 
 
 ......

 @param 
 @return 없음
*/
atBOOL	atUIAPI_ChangeFont( atHFONT hFont, atLPTSTR font_face, int font_size, int font_style )
{
	atLPUI_FONT	lpFont = atUIAPI_GetFontPtr(hFont);

	if( lpFont == atNULL ) return atFALSE;

	if( tOemFn.fnDestroyFont != atNULL ){
		tOemFn.fnDestroyFont( lpFont->lpOemFont );
	}
	if( tOemFn.fnCreateFont != atNULL ){
		lpFont->lpOemFont = tOemFn.fnCreateFont( font_face, font_size, font_style, atTRUE );
	} else {
		lpFont->lpOemFont = atNULL;
	}
	
	return atTRUE;
}


/** 
 @brief 
 
 ......

 @param 
 @return 없음
*/
atHFONT atUIAPI_ReferenceFont( atHFONT hFont )
{
	atLPUI_FONT	lpFont = atUIAPI_GetFontPtr(hFont);
	if( lpFont == atNULL ) return 0;

	if( lpFont->nRefCount == 0xEFFF ) return hFont;
	lpFont->nRefCount++;

    return hFont;
}


/** 
 @brief 
 
 ......

 @param 
 @return 없음
*/
atHFONT atUIAPI_ReleaseFont( atHFONT hFont, atBOOL bAutoDestory )
{
	atLPUI_FONT	lpFont = atUIAPI_GetFontPtr(hFont);
	if( lpFont == atNULL ) return 0;

	if( lpFont->nRefCount == 0xEFFF ) return hFont;

	if( --lpFont->nRefCount <= 0 ){
		if( bAutoDestory ) atUIAPI_DestroyFont( hFont );
		return atNULL;
	} else {
		return hFont;
	}
}


/** 
 @brief 
 
 ......

 @param 
 @return 없음
*/
atINT	atUIAPI_GetStringWidth( atHFONT hFont, atLPTSTR szText, atINT nLength )
{	
	atLPUI_FONT	lpFont = atUIAPI_GetFontPtr(hFont);
	if( lpFont == atNULL ) return 0;

	if( nLength < 0 ) nLength = UI_STRLEN(szText);

	if( tOemFn.fnGetStringWidth != atNULL ){
		return nLength > 0 ? tOemFn.fnGetStringWidth( lpFont->lpOemFont, szText, nLength ) : 0;
	}

	return 0;
}


/** 
 @brief 
 
 ......

 @param 
 @return 없음
*/
atINT	atUIAPI_GetFontHeight( atHFONT hFont )
{
	atLPUI_FONT	lpFont = atUIAPI_GetFontPtr(hFont);
	if( lpFont == atNULL ) return 0;

	if( tOemFn.fnGetFontHeight != atNULL ){
		return tOemFn.fnGetFontHeight( lpFont->lpOemFont );
	}

	return 0;
}


/** 
 @brief 
 
 ......

 @param 
 @return 없음
*/
atINT	atUIAPI_GetStringMetric( atHFONT hFont, atLPTSTR szText, atINT nLength, atSIZE *lpMetric )
{
	int i, cnt;
	int idx, width, max_width;
	atTCHAR str[256];
	 atTCHAR ch;
	 
	if( hFont == atNULL ) return 0;

	if( nLength < 0 ) nLength = UI_STRLEN(szText);

	idx = 0;
	cnt = 0;
	max_width = 0;
	for( i = 0; i < nLength; i++ ){
		if( (ch = *szText++) == _ATC('\0') ) break;
		if( ch == _ATC('\n') ){
			str[idx++] = _ATC('\0');
			width = atUIAPI_GetStringWidth( hFont, str, idx );
			idx = 0;
			cnt++;
			if( width > max_width ) max_width = width;
		}
		str[idx++] = ch;
	}
	if( idx > 0 ){
		str[idx++] = _ATC('\0');
		width = atUIAPI_GetStringWidth(hFont, str, idx );
		cnt++;
		if( width > max_width ) max_width = width;
	}

	if( lpMetric != atNULL ){
		if( cnt > 0 ){
			lpMetric->nWidth = max_width;
			lpMetric->nHeight = cnt * atUIAPI_GetFontHeight(hFont);
		} else {
			lpMetric->nWidth = 0;
			lpMetric->nHeight = 0;
		}
	}
	
	return cnt;
}


/** 
 @brief 
 
 ......

 @param 
 @return 없음
*/
atHFONT		atUIAPI_GetStockFont( int font_idx )
{
	if( (font_idx < 0) || (font_idx >= MAX_STOCK_FONT) ) return atNULL; 
	return stock_font[font_idx];
}


/** 
 @brief 
 
 ......

 @param 
 @return 없음
*/
atBOOL	   atUIAPI_ReleaseAllOEMFont( NOARGS )
{
	int i;
	atHFONT hFont;
	atLPUI_FONT	lpFont;
	
	for( i = 0; i < atUIAPI_MAX_FONT_COUNT; i++ ){
		hFont = _get_handle(i);
		if( hFont ){
			lpFont = atUIAPI_GetFontPtr(hFont);
			if( lpFont != atNULL ){
				if( tOemFn.fnDestroyFont != atNULL ){
					tOemFn.fnDestroyFont( lpFont->lpOemFont );
				}
				lpFont->lpOemFont = atNULL;
			}
		}
	}

//	tOemFn.fnCloseFontSystem();
	if( tOemFn.fnSuspendFontSystem != atNULL ){
		tOemFn.fnSuspendFontSystem();
	}
	
	return atTRUE;
}


/** 
 @brief 
 
 ......

 @param 
 @return 없음
*/
atBOOL	   atUIAPI_ResyncAllOEMFont( NOARGS )
{
	int i;
	atHFONT hFont;
	atLPUI_FONT	lpFont;

//	tOemFn.fnInitFontSystem(	(atLONG)g_SysHandle, (atLONG)g_pFontPath, (atLONG)g_pFontName );
	if( tOemFn.fnResumeFontSystem != atNULL ){
		tOemFn.fnResumeFontSystem();
	}
	
	for( i = 0; i < atUIAPI_MAX_FONT_COUNT; i++ ){
		hFont = _get_handle(i);
		if( hFont ){
			lpFont = atUIAPI_GetFontPtr(hFont);
			if( lpFont->lpOemFont != atNULL ) {
				if( tOemFn.fnDestroyFont != atNULL ){
					tOemFn.fnDestroyFont( lpFont->lpOemFont );
				}
				lpFont->lpOemFont = atNULL;
			}
			if( tOemFn.fnCreateFont != atNULL ){
				lpFont->lpOemFont = tOemFn.fnCreateFont( lpFont->szFontName, lpFont->nFontSize, lpFont->dwFontStyle, atTRUE );
			} else {
				lpFont->lpOemFont = atNULL;
			}
		}
	}

	return atTRUE;
}


