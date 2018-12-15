#include "OEM_UI_Font.h"
#include "GrpX.h"
#include "UI_API/UI_API.h"


#define USE_FREETYPE
//#define USE_WIN32GDI


extern atLPGRPXMNG g_lpGrpX; // GDI에서 GrpX 인스턴스를 먼저 생성하며, GrpX 생성시, 내장 폰트 시스템 초기화 됨.


//==============================================================
#ifdef USE_FREETYPE
//==============================================================

#define USE_SYSTEM_FONT		1
#define SYSTEM_FONT_NAME		_AT("Courier New")

#define DEF_FONT_FILENAME 		_AT("tahoma.ttf")

//#define FONT_FULL_FILENAME		_AT("\\windows\\fonts\\tahoma.ttf")
//#define FONT_FULL_FILENAME 	_AT("\\My Storage\\Tornado\\res\\ARIALUNI_ColumbusLB.ttf")
//#define FONT_FULL_FILENAME 	_AT("\\My Storage\\Tornado\\res\\tahoma.ttf")
//#define FONT_FULL_FILENAME 	_AT("\\My Storage\\Tornado\\res\\HMKMRHD.ttf")
#define FONT_FILENAME 		_AT("tahoma.ttf")
//#define FONT_FILENAME 			_AT("ARIALUNI_ColumbusLB.ttf")


#define USE_FREETYPE_CACHE
#define USE_FREETYPE_STREAM

#ifdef USE_FREETYPE_STREAM
#define USE_FREETYPE_STREAM_MMF		1
#endif

#include <windows.h> 
#include "ft2build.h"
#include FT_FREETYPE_H
#include FT_BITMAP_H

#include "A_Math.h"

#ifndef MAX_PATH
#define MAX_PATH 256
#endif

typedef struct _tagOEMFont {
	atTCHAR font_name[32];
	long font_size;
	atDWORD font_attr;
	atREAL font_angle;
	FT_Library oem_library;
	FT_Face oem_font;
} OEMFont, *LPOEMFont;

#ifdef USE_FREETYPE_STREAM
static char *g_font_mem;
static long g_font_size;
static atHRES fontres = atNULL;
#endif

#ifdef USE_FREETYPE_CACHE

#define GLYPH_CACHE_SIZE	512


typedef struct tagFONTCACHE {
	int font_idx;
	int font_size;
	int font_attr;
	atREAL font_angle;
	atGRPX_GLYPHINFO *lpGlyph;
	int last_time;
	int next_idx;
} _FONTCACHE;

static _FONTCACHE g_font_cache[GLYPH_CACHE_SIZE] = {{0,}, };
static int g_vv = 0;


static void init_cache(void)
{
	int i;
	_FONTCACHE *pcache;

	pcache = g_font_cache;
	for( i = 0; i < GLYPH_CACHE_SIZE; i++ ){
		pcache->font_idx = -1;
		pcache->lpGlyph = atNULL;
		pcache++;
	} 
	g_vv = 0;
}

static void close_cache(void)
{
	int i;
	_FONTCACHE *pcache;

	pcache = g_font_cache;
	for( i = 0; i < GLYPH_CACHE_SIZE; i++ ){
		if( pcache->font_idx != -1 ){
			if( pcache->lpGlyph ){
				if( pcache->lpGlyph->lpBitmap ) atUIAPI_FreeMem( pcache->lpGlyph->lpBitmap );
				atUIAPI_FreeMem( pcache->lpGlyph );
				pcache->lpGlyph = atNULL;
			}
		}
		pcache++;
	} 
}

static atGRPX_GLYPHINFO *add_cache( FT_Library hLibrary, FT_Face hFTFace, int index, int size, int attr, atREAL angle )
{
	int i;
	_FONTCACHE *pcache;
	int old_time, old_idx;
	atGRPX_GLYPHINFO *lpGlyph;
	FT_GlyphSlot  glyph = hFTFace->glyph;

//	FT_Load_Glyph(hFTFace, index, FT_LOAD_RENDER);
//	FT_Load_Glyph(hFTFace, index, FT_LOAD_MONOCHROME);
	FT_Load_Glyph(hFTFace, index, FT_LOAD_DEFAULT);
	// 만약 모노크롬도 아니고 256그레이도 아니면, 256그레이로 통일.
	if( (glyph->bitmap.pixel_mode != FT_PIXEL_MODE_MONO) && (glyph->bitmap.pixel_mode != FT_PIXEL_MODE_GRAY) )
		FT_Render_Glyph(glyph, FT_RENDER_MODE_NORMAL);
//	FT_Render_Glyph(glyph, FT_RENDER_MODE_MONO);

	if( attr & atGRPX_FS_BOLD ){
		FT_Bitmap_Embolden( hLibrary, &glyph->bitmap, (hFTFace->size->metrics.x_ppem*10/100)*64, 0 );
	}

	lpGlyph = (atGRPX_GLYPHINFO *)atUIAPI_AllocMem(sizeof(atGRPX_GLYPHINFO));

	lpGlyph->nHorizAdvance = (short)(glyph->metrics.horiAdvance >> 6);
	lpGlyph->dwAttr = atGRPX_FONTATTR_NONE;

	lpGlyph->nBearingX = glyph->bitmap_left;
	lpGlyph->nBearingY = glyph->bitmap_top;
	lpGlyph->nWidth = glyph->bitmap.width;
	lpGlyph->nHeight = glyph->bitmap.rows;
	lpGlyph->rAdvX = FIXEDTOREAL(glyph->advance.x << (16-6));
	lpGlyph->rAdvY = FIXEDTOREAL(-(glyph->advance.y << (16-6)));
	if( glyph->bitmap.pixel_mode == FT_PIXEL_MODE_MONO )
		lpGlyph->nBPP = 1;
	else if( glyph->bitmap.pixel_mode == FT_PIXEL_MODE_GRAY )
		lpGlyph->nBPP = 8;
	else if( glyph->bitmap.pixel_mode == FT_PIXEL_MODE_GRAY2 )
		lpGlyph->nBPP = 2;
	else if( glyph->bitmap.pixel_mode == FT_PIXEL_MODE_GRAY4 )
		lpGlyph->nBPP = 4;
	else
		lpGlyph->nBPP = 0;
	lpGlyph->nFontWidthBytes = glyph->bitmap.width * lpGlyph->nBPP / 8;
	lpGlyph->lpBitmap = atUIAPI_AllocMem( lpGlyph->nFontWidthBytes * lpGlyph->nHeight );	
	if( lpGlyph->lpBitmap )
		atUIAPI_memcpy( lpGlyph->lpBitmap, glyph->bitmap.buffer, lpGlyph->nFontWidthBytes * lpGlyph->nHeight );

	pcache = g_font_cache;
	for( i = 0; i < GLYPH_CACHE_SIZE; i++ ){
		if( pcache->font_idx == -1 ){
			pcache->font_idx = index;
			pcache->font_size = size;
			pcache->font_attr = attr;
			pcache->font_angle = angle;
			pcache->lpGlyph = lpGlyph;
			pcache->last_time = ++g_vv;
			return pcache->lpGlyph;
		}
		pcache++;
	} 

	pcache = g_font_cache;
	old_time = pcache->last_time;
	old_idx = 0;
	pcache++;
	for( i = 1; i < GLYPH_CACHE_SIZE; i++ ){
		if( pcache->last_time < old_time ){
			old_time = pcache->last_time;
			old_idx = i;
		}
		pcache++;
	} 	

	pcache = &g_font_cache[old_idx];
	pcache->font_idx = index;
	pcache->font_size = size;
	pcache->font_attr = attr;
	pcache->font_angle = angle;
	if( pcache->lpGlyph ){
	  if( pcache->lpGlyph->lpBitmap ) atUIAPI_FreeMem( pcache->lpGlyph->lpBitmap );
	  atUIAPI_FreeMem( pcache->lpGlyph );
	}
	pcache->lpGlyph = lpGlyph;
	pcache->last_time = ++g_vv;

	return lpGlyph;
}

static atGRPX_GLYPHINFO *font_find( FT_Face	hFTFace, int font_idx, int size, int attr, atREAL angle )
{
	int i;
	_FONTCACHE *pcache;

	pcache = g_font_cache;
	for( i = 0; i < GLYPH_CACHE_SIZE; i++ ){
		if( (pcache->font_idx == font_idx) && (pcache->font_size == size) && 
			(pcache->font_attr == attr) && (pcache->font_angle == angle)){
			pcache->last_time = ++g_vv;
			return pcache->lpGlyph;
		}
		pcache++;
	} 

	return atNULL;
}

#endif

static atBOOL   _get_font_filename( atLPTSTR szFontFileName )
{
#if USE_SYSTEM_FONT
    HKEY    hKey;  
    TCHAR   szData[256];   
	DWORD   dwSize = COUNTOF(szData);  
	TCHAR	*ptr;
  
    if( RegOpenKeyEx(HKEY_LOCAL_MACHINE, _AT("Software\\Microsoft\\FontLink\\SystemLink"), 0, KEY_READ, &hKey) != ERROR_SUCCESS){  
        return atFALSE;  
    }  
  
    if( RegQueryValueEx(hKey, SYSTEM_FONT_NAME, 0, NULL, (LPBYTE)szData, &dwSize) != ERROR_SUCCESS ){  
         return atFALSE;  
    }  

	ptr = atUIAPI_strchr( szData, _AT(',') );
	if( ptr != NULL ){
		*ptr = _ATC('\0');
	}

    RegCloseKey( hKey );  


	if( atUIAPI_IsExistFile(szData) ){
		atUIAPI_strcpy( szFontFileName, szData );  
		return atTRUE;
	} else {
		return atFALSE;
	}

#else
#ifdef FONT_FULL_FILENAME
	atUIAPI_strcpy(szFontFileName, FONT_FULL_FILENAME);	
#else
	atTCHAR pathWindows[MAX_PATH];

#ifdef __USES_UNICODE_
	GetWindowsDirectoryW( pathWindows, MAX_PATH );
#else
	GetWindowsDirectoryA( pathWindows, MAX_PATH );
#endif

	atUIAPI_sprintf( szFontFileName, _AT("%s\\Fonts\\%s"), pathWindows, FONT_FILENAME );
#endif
#endif
}

// 여기서부터 OEM Font 관련 함수들...

#ifdef USE_FREETYPE_STREAM_MMF
static HANDLE g_hSharedMemForFont;
static atVOID* g_pBaseForFont;
#endif

static atLPVOID	_InitFontMng( atLPVOID lpGrpX, atLPVOID lpData, atINT nDataType )
{
	void **param;
    FT_Library ftLibrary;
#ifdef USE_FREETYPE_STREAM
	atTCHAR pathFont[MAX_PATH];
#endif

#ifdef USE_FREETYPE_STREAM
	if (nDataType == 0) {
		param = (void **)lpData;
		if (param[0] != atNULL) {
			atUIAPI_strcpy(pathFont, (atLPTSTR)param[0]); //FONT_FILE);	
		}
		else {
			_get_font_filename(pathFont);
			if (atUIAPI_stricmp(&pathFont[atUIAPI_strlen(pathFont) - 3], _AT("ttf")) != 0) {
				if (param[1] == 0) return atNULL;
				atUIAPI_strcpy(pathFont, (atLPTSTR)param[1]); //FONT_FILE);	
			}
		}
		g_font_size = atUIAPI_GetFileSize(pathFont);

#ifdef USE_FREETYPE_STREAM_MMF
		g_hSharedMemForFont = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, g_font_size, 0);
		g_pBaseForFont = MapViewOfFile(g_hSharedMemForFont, FILE_MAP_ALL_ACCESS, 0, 0, 0);
		g_font_mem = (char *)(g_pBaseForFont);
#else
		g_font_mem = (char *)atUIAPI_AllocMem(g_font_size);
#endif

		atUIAPI_LoadFile(pathFont, g_font_mem, g_font_size, 0);
	} else {
		atHRESSRC ressrc = atUIAPI_GetResSource((atLPTSTR)nDataType);
		if (ressrc != atNULL) {
			atUIAPI_RESSRC_INFO ressrcinfo;
			atUIAPI_RES_INFO resinfo;
			atUIAPI_GetResSourceInfo(ressrc, &ressrcinfo);
			atUIAPI_GetResInfo(ressrc, (atLPTSTR)lpData, &resinfo);
			if (ressrcinfo.type == 3 && resinfo.nCompressMethod != 1) {
				fontres = atNULL;
				g_font_mem = (char *)ressrcinfo.lpPtr + resinfo.nStreamOffset;
				g_font_size = resinfo.nFileSize;
			}
			else {
				fontres = atUIAPI_LoadRes(ressrc, (atLPTSTR)lpData);
				if (fontres != atNULL) atUIAPI_GetResData(fontres, (atLPVOID *)&g_font_mem, &g_font_size);
			}
		}

	}
#endif

#ifdef USE_FREETYPE_CACHE
	init_cache();
#endif

	FT_Init_FreeType( &ftLibrary );
	if( ftLibrary == atNULL ) return atNULL;

	return (void *)ftLibrary;
}

static void	_CloseFontMng( atLPVOID lpInstData )
{
    FT_Library ftLibrary = (FT_Library)lpInstData;

#ifdef USE_FREETYPE_CACHE
	close_cache();
#endif

#ifdef USE_FREETYPE_STREAM

#ifdef USE_FREETYPE_STREAM_MMF
	if( g_font_mem )
	{
		UnmapViewOfFile(g_pBaseForFont);
		g_pBaseForFont = atNULL;
		CloseHandle( g_hSharedMemForFont );
		g_hSharedMemForFont = atNULL;	
	}
#else
	if( g_font_mem ){
		atUIAPI_FreeMem(g_font_mem);
		g_font_mem = atNULL;
	}
#endif
#endif

	FT_Done_FreeType(ftLibrary);
}

static atDWORD _GetFontMngCaps( atLPVOID lpInstData )
{
#ifdef USE_FREETYPE_CACHE
	return atGRPX_FONTATTR_ROTATABLE | atGRPX_FONTATTR_ROTATABLE_CHANGED | atGRPX_FONTATTR_SIZABLE | atGRPX_FONTATTR_CACHEABLE;
#else
	return atGRPX_FONTATTR_ROTATABLE | atGRPX_FONTATTR_ROTATABLE_CHANGED | atGRPX_FONTATTR_SIZABLE;
#endif
}

#include <math.h>
static atGRPX_OEM_UI_FONT	_CreateFont( atLPVOID lpInstData, atLPTSTR FontName, atINT nSize, atDWORD dwStyle, atREAL rAngle, atINT *lpBPP )
{
    FT_Error  r;
	FT_Face	hFTFace;
	LPOEMFont fnt;
    FT_Library ftLibrary = (FT_Library)lpInstData;
	int bpp;

#ifndef USE_FREETYPE_STREAM
	atTCHAR pathFont[MAX_PATH];
	atCHAR pathFontA[MAX_PATH];
	_get_font_filename( pathFont );

	atUIAPI_ToANSI( pathFontA, pathFont, -1 );
	r = FT_New_Face( ftLibrary, pathFontA, 0, &hFTFace );
#else	
	r = FT_New_Memory_Face( ftLibrary, (FT_Bytes)g_font_mem, g_font_size, 0, &hFTFace );
#endif

	if(r != FT_Err_Ok) return (atGRPX_OEM_UI_FONT)atNULL;

	FT_Set_Pixel_Sizes(hFTFace, 0, nSize);

	FT_Load_Glyph( hFTFace, FT_Get_Char_Index(hFTFace, _ATC('.')), FT_LOAD_DEFAULT );
	if( hFTFace->glyph->bitmap.pixel_mode == FT_PIXEL_MODE_MONO )
		bpp = 1;
	else if( hFTFace->glyph->bitmap.pixel_mode == FT_PIXEL_MODE_GRAY )
		bpp = 8;
	else
		bpp = 0;

	if( lpBPP ) *lpBPP = bpp;

	if( rAngle != INTTOREAL(0) ){
		FT_Matrix matrix;
		FT_Vector pen;
		FT_SizeRec *size = hFTFace->size;
/*
		float angle = ((float)REALTOFLOAT(-rAngle))/180*3.1415926536;
		matrix.xx = (FT_Fixed)(cos(angle) * 0x10000L);
		matrix.xy = (FT_Fixed)(-sin(angle) * 0x10000L);
		matrix.yx = (FT_Fixed)(sin(angle) * 0x10000L);
		matrix.yy = (FT_Fixed)(cos(angle) * 0x10000L);
*/
		matrix.xx = COS(-rAngle);
		matrix.xy = -SIN(-rAngle);
		matrix.yx = SIN(-rAngle);
		matrix.yy = COS(-rAngle);
		pen.x = 0*64;
		pen.y = -0*64;
		FT_Set_Transform( hFTFace, &matrix, &pen );
	}

	fnt = (LPOEMFont)atUIAPI_AllocMem( sizeof(OEMFont) );
	atUIAPI_strcpy( fnt->font_name, FontName );
	fnt->font_size = nSize;
	fnt->font_attr = dwStyle;
	fnt->font_angle = rAngle;
	fnt->oem_library = ftLibrary;
	fnt->oem_font = hFTFace;

	return (atGRPX_OEM_UI_FONT)fnt;
}

static void			_DestroyFont( atGRPX_OEM_UI_FONT OemFont )
{
	LPOEMFont fnt = (LPOEMFont)OemFont;
	FT_Face	hFTFace = fnt->oem_font;

	FT_Done_Face(hFTFace);

	atUIAPI_FreeMem( fnt );
}

static void			_SetFontStyle( atGRPX_OEM_UI_FONT OemFont, atINT Style )
{
	LPOEMFont fnt = (LPOEMFont)OemFont;
	FT_Face	hFTFace = fnt->oem_font;

	fnt->font_attr = Style;
}

static void			_SetFontAngle( atGRPX_OEM_UI_FONT OemFont, atREAL rAngle )
{
	LPOEMFont fnt = (LPOEMFont)OemFont;
	FT_Face	hFTFace = fnt->oem_font;
	FT_Matrix matrix;
	FT_Vector pen;
	FT_SizeRec *size = hFTFace->size;

	matrix.xx = COS(-rAngle);
	matrix.xy = -SIN(-rAngle);
	matrix.yx = SIN(-rAngle);
	matrix.yy = COS(-rAngle);
	pen.x = 0*64;
	pen.y = -0*64;
	FT_Set_Transform( hFTFace, &matrix, &pen );

	fnt->font_angle = rAngle;
}

static void			_GetFontMetric( atGRPX_OEM_UI_FONT OemFont, atINT *lpAscent, atINT *lpDescent )
{
	LPOEMFont fnt = (LPOEMFont)OemFont;
	FT_Face	hFTFace = fnt->oem_font;
	FT_SizeRec *size = hFTFace->size;

	if( lpAscent ) *lpAscent = size->metrics.ascender >> 6;
	if( lpDescent ) *lpDescent = -(size->metrics.descender >> 6);
}

static atINT			_GetCharIndex( atGRPX_OEM_UI_FONT OemFont, atTCHAR Chr )
{
	LPOEMFont fnt = (LPOEMFont)OemFont;
	FT_Face	hFTFace = fnt->oem_font;

	return FT_Get_Char_Index( hFTFace, Chr );
}

static atBOOL			_GetGlyphMetric( atGRPX_OEM_UI_FONT OemFont, atUINT index, atSIZE *lpSize )
{
#ifdef USE_FREETYPE_CACHE
	atGRPX_GLYPHINFO *lpGlyph;
	int font_size;
	atREAL font_angle;
#endif
	LPOEMFont fnt = (LPOEMFont)OemFont;
	FT_Face	hFTFace = fnt->oem_font;
	FT_GlyphSlot  glyph = hFTFace->glyph;
	FT_SizeRec *size = hFTFace->size;

#ifdef USE_FREETYPE_CACHE
	font_size = fnt->font_size;
	font_angle = fnt->font_angle;
	if( (lpGlyph = font_find( fnt->oem_font, index, font_size, 0, font_angle )) == atNULL ){
		lpGlyph = add_cache( fnt->oem_library, fnt->oem_font, index, font_size, 0, font_angle );
	}
	if( lpGlyph == atNULL ) return atFALSE;
	
	lpSize->nWidth = lpGlyph->nHorizAdvance;
	lpSize->nHeight = (size->metrics.ascender >> 6) - (size->metrics.descender >> 6);
#else
	FT_Load_Glyph(hFTFace, index, FT_LOAD_DEFAULT);

	if( fnt->font_attr & atGRPX_FS_BOLD ){
		FT_Bitmap_Embolden( fnt->oem_library, &hFTFace->glyph->bitmap, (hFTFace->size->metrics.x_ppem*10/100)*64, 0 );
	}

	lpSize->nWidth = (glyph->metrics.horiAdvance >> 6);
	lpSize->nHeight = ((size->metrics.ascender >> 6) - (size->metrics.descender >> 6));
#endif		

	return atTRUE;
}

static atBOOL			_GetGlyphBitmap( atGRPX_OEM_UI_FONT OemFont, atUINT index, atGRPX_GLYPHINFO *lpGlyph )
{
#ifdef USE_FREETYPE_CACHE
	atGRPX_GLYPHINFO *lpCacheGlyph;
	int font_size;
	int font_attr;
	atREAL font_angle;
#endif
	LPOEMFont fnt = (LPOEMFont)OemFont;
	FT_Face	hFTFace = fnt->oem_font;
	FT_GlyphSlot  glyph = hFTFace->glyph;

#ifdef USE_FREETYPE_CACHE
	FT_SizeRec *size = hFTFace->size;
	font_size = fnt->font_size;
	font_angle = fnt->font_angle;
	font_attr = fnt->font_attr;
	if( (lpCacheGlyph = font_find( fnt->oem_font, index, font_size, font_attr, font_angle )) == atNULL ){
		lpCacheGlyph = add_cache( fnt->oem_library, fnt->oem_font, index, font_size, font_attr, font_angle );
	}
	if( lpCacheGlyph == atNULL ) return atFALSE;

	atUIAPI_memcpy( lpGlyph, lpCacheGlyph, sizeof(atGRPX_GLYPHINFO) );

	return atTRUE;
#else
//	FT_Load_Glyph(hFTFace, index, FT_LOAD_RENDER);
//	FT_Load_Glyph(hFTFace, index, FT_LOAD_MONOCHROME);
	FT_Load_Glyph(hFTFace, index, FT_LOAD_DEFAULT);

	// 만약 모노크롬도 아니고 256그레이도 아니면, 256그레이로 통일.
	if( (glyph->bitmap.pixel_mode != FT_PIXEL_MODE_MONO) && (glyph->bitmap.pixel_mode != FT_PIXEL_MODE_GRAY) )
		FT_Render_Glyph(glyph, FT_RENDER_MODE_NORMAL);
//	FT_Render_Glyph(glyph, FT_RENDER_MODE_MONO);

	if( fnt->font_attr & atGRPX_FS_BOLD ){
		FT_Bitmap_Embolden( fnt->oem_library, &glyph->bitmap, (hFTFace->size->metrics.x_ppem*10/100)*64, 0 );
	}

	lpGlyph->nHorizAdvance = (short)(glyph->metrics.horiAdvance >> 6);
	lpGlyph->dwAttr = atGRPX_GLYPHATTR_NONE;

	lpGlyph->nBearingX = glyph->bitmap_left;
	lpGlyph->nBearingY = glyph->bitmap_top;
	lpGlyph->nWidth = glyph->bitmap.width;
	lpGlyph->nHeight = glyph->bitmap.rows;
	lpGlyph->rAdvX = FIXEDTOREAL(glyph->advance.x << (16-6));
	lpGlyph->rAdvY = FIXEDTOREAL(-(glyph->advance.y << (16-6)));
	if( glyph->bitmap.pixel_mode == FT_PIXEL_MODE_MONO )
		lpGlyph->nBPP = 1;
	else if( glyph->bitmap.pixel_mode == FT_PIXEL_MODE_GRAY )
		lpGlyph->nBPP = 8;
	else if( glyph->bitmap.pixel_mode == FT_PIXEL_MODE_GRAY2 )
		lpGlyph->nBPP = 2;
	else if( glyph->bitmap.pixel_mode == FT_PIXEL_MODE_GRAY4 )
		lpGlyph->nBPP = 4;
	else
		lpGlyph->nBPP = 0;
	lpGlyph->nFontWidthBytes = glyph->bitmap.width * lpGlyph->nBPP / 8;
	lpGlyph->lpBitmap = glyph->bitmap.buffer;
#endif

	return atTRUE;
}

static void			_FreeGlyphBitmap( atGRPX_OEM_UI_FONT OemFont, atGRPX_GLYPHINFO *lpGlyph )
{
	LPOEMFont fnt = (LPOEMFont)OemFont;
	FT_Face	hFTFace = fnt->oem_font;

//	FT_Done_GlyphSlot( hFTFace->glyph );
}


//==============================================================
#elif defined(USE_WIN32GDI)
//==============================================================


#include <windows.h>

//#define FONT_BPP 1
#define FONT_BPP 8

#ifndef MAX_PATH
#define MAX_PATH 256
#endif

//#define FONT_FILE _AT("xxxxxxxxxxxxxxxxx\\tahoma.ttf")
//#define FONT_FILE _AT("e:\\xxxxxxxxxxxxxxxxx\\tahoma.ttf")

extern HWND g_hWindow;


// 여기서부터 OEM Font 관련 함수들...

atLPVOID	_InitFontMng( atLPVOID lpGrpX, atLPVOID lpData )
{
	return (atLPVOID)g_hWindow;
}

void	_CloseFontMng( atLPVOID lpInstData )
{
}

atDWORD _GetFontMngCaps( atLPVOID lpInstData )
{
	return atGRPX_FONTATTR_ROTATABLE | atGRPX_FONTATTR_SIZABLE | atGRPX_FONTATTR_CACHEABLE;
}

#include <math.h>
static atGRPX_OEM_UI_FONT	_CreateFont( atLPVOID lpInstData, atLPTSTR FontName, atINT nSize, atDWORD dwStyle, atREAL rAngle, atINT *lpBPP )
{
	HDC hDC, hFontDC;
    LOGFONT lf;
	HFONT hMyFont;
	
	hDC = GetDC( (HWND)lpInstData );
	hFontDC = CreateCompatibleDC( hDC );
	ReleaseDC( (HWND)lpInstData, hDC );

	SetMapMode( hFontDC, MM_TEXT );

	// build a TrueType font.
//	GetObject( GetStockObject(SYSTEM_FONT), sizeof(lf), &lf );
//	lf.lfHeight = nSize;
	lf.lfHeight = nSize * GetDeviceCaps(hFontDC, LOGPIXELSY) / 72;
	lf.lfWidth			= 0;						// 너비 
	lf.lfEscapement		= REALTOINT(rAngle * -10);						// 문자열기울기 
	lf.lfOrientation	= 0;						// 문자개별각도 
	lf.lfWeight			= (dwStyle==0)? FW_NORMAL : FW_BOLD;				// 굵기 
	lf.lfItalic			= FALSE;					// 이탤릭 
	lf.lfUnderline		= FALSE;					// 밑줄 
	lf.lfStrikeOut		= FALSE;					// 취소선 
	lf.lfCharSet		= DEFAULT_CHARSET;			// 필수 
	lf.lfOutPrecision	= OUT_DEFAULT_PRECIS;                
	lf.lfClipPrecision	= CLIP_DEFAULT_PRECIS;      // 가변폭폰트 고정폭폰트 
	lf.lfPitchAndFamily	= DEFAULT_PITCH | FF_DONTCARE; // 글꼴이름 
	if( atUIAPI_strcmp( FontName, _AT("System")) == 0 )
		atUIAPI_strcpy( (atLPTSTR)lf.lfFaceName, _AT("Tahoma"));
	else
		atUIAPI_strcpy(lf.lfFaceName, FontName);
	hMyFont = CreateFontIndirect(&lf);

	SelectObject( hFontDC, hMyFont );

	if( lpBPP ) *lpBPP = FONT_BPP;

	return (atGRPX_OEM_UI_FONT)hFontDC;
}

static void			_DestroyFont( atGRPX_OEM_UI_FONT OemFont )
{
	HFONT hMyFont;
	
	hMyFont = (HFONT)SelectObject( (HDC)OemFont, GetStockObject(SYSTEM_FONT) );
	DeleteDC( (HDC)OemFont );
	DeleteObject( hMyFont );
}

static void			_SetFontStyle( atGRPX_OEM_UI_FONT OemFont, atINT Style )
{
}

static void			_SetFontAngle( atGRPX_OEM_UI_FONT OemFont, atREAL rAngle )
{
}

static void			_GetFontMetric( atGRPX_OEM_UI_FONT OemFont, atINT *lpAscent, atINT *lpDescent )
{
    TEXTMETRIC tm;
    GetTextMetrics((HDC)OemFont, &tm);

	if( lpAscent ) *lpAscent = tm.tmAscent;
	if( lpDescent ) *lpDescent = tm.tmDescent;
}

static atINT			_GetCharIndex( atGRPX_OEM_UI_FONT OemFont, atTCHAR Chr )
{
	return (int)Chr;
}

static FIXED PASCAL NEAR FixedFromDouble(double d)
{
    long l;

    l = (long) (d * 65536L);
    return *(FIXED *)&l;
}
static IdentityMat(LPMAT2 lpMat)
{
    lpMat->eM11 = FixedFromDouble(1);
    lpMat->eM12 = FixedFromDouble(0);
    lpMat->eM21 = FixedFromDouble(0);
    lpMat->eM22 = FixedFromDouble(1);
}

static atBOOL			_GetGlyphMetric( atGRPX_OEM_UI_FONT OemFont, atUINT index, atSIZE *lpSize )
{
	GLYPHMETRICS gm;
	MAT2 tm;
	int dwSize;

	IdentityMat(&tm);
	dwSize = GetGlyphOutline( (HDC)OemFont, index, GGO_METRICS, &gm, 0, NULL, &tm );
	if( dwSize == GDI_ERROR ) return atFALSE;

	lpSize->nWidth = gm.gmCellIncX;//gm.gmptGlyphOrigin.x + gm.gmBlackBoxX;
	lpSize->nHeight = gm.gmptGlyphOrigin.y + gm.gmBlackBoxY;

	return atTRUE;
}

static atBOOL			_GetGlyphBitmap( atGRPX_OEM_UI_FONT OemFont, atUINT index, atGRPX_GLYPHINFO *lpGlyph )
{
	GLYPHMETRICS gm;
	atLPBYTE bitmap;
	int font_type;
	MAT2 tm;
	int dwSize;
	int i;

	IdentityMat(&tm);
#if FONT_BPP == 1
	font_type = GGO_BITMAP;
#elif FONT_BPP == 8
	font_type = GGO_GRAY8_BITMAP;
#else
	return atFALSE;
#endif
	dwSize = GetGlyphOutline( (HDC)OemFont, index, font_type, &gm, 0, NULL, &tm );
	if( dwSize == GDI_ERROR ) return atFALSE;

	if( dwSize > 0 ){
		bitmap = (atLPBYTE)atUIAPI_AllocMem( dwSize );
		if( bitmap == atNULL ) return atFALSE;
		dwSize = GetGlyphOutline( (HDC)OemFont, index, font_type, &gm, dwSize, bitmap, &tm );
		if( dwSize == GDI_ERROR ) return atFALSE;
		lpGlyph->nWidth = gm.gmBlackBoxX;
		lpGlyph->nHeight = gm.gmBlackBoxY;
	} else {
		bitmap = atNULL;
		lpGlyph->nWidth = 0;
		lpGlyph->nHeight = 0;
	}

	lpGlyph->nHorizAdvance = gm.gmCellIncX;//gm.gmptGlyphOrigin.x + gm.gmBlackBoxX;
	lpGlyph->dwAttr = atGRPX_FONTATTR_NONE;

	lpGlyph->nBearingX = (short)gm.gmptGlyphOrigin.x; //gm.gmCellIncX; //gm.gmptGlyphOrigin.x;
	lpGlyph->nBearingY = (short)gm.gmptGlyphOrigin.y; //-gm.gmCellIncY; //gm.gmptGlyphOrigin.y;
	lpGlyph->rAdvX = INTTOREAL(gm.gmCellIncX);
	lpGlyph->rAdvY = INTTOREAL(gm.gmCellIncY);
	lpGlyph->nBPP = FONT_BPP;
	lpGlyph->lpBitmap = bitmap;

	if( bitmap != atNULL ){
		if( lpGlyph->nBPP == 1 ){
			lpGlyph->nFontWidthBytes =  (((lpGlyph->nWidth + 31) >> 5) << 2);
		} else if( lpGlyph->nBPP == 8 ){
			lpGlyph->nFontWidthBytes =  (lpGlyph->nWidth + 0x3) & ~0x3;
			for( i = 0; i < dwSize;i++ )
			  bitmap[i] = (bitmap[i] >= 64)? 0xff : bitmap[i] << 2;
		} else {
			lpGlyph->nFontWidthBytes =  0;
		}
	}

	return atTRUE;
}

static void			_FreeGlyphBitmap( atGRPX_OEM_UI_FONT OemFont, atGRPX_GLYPHINFO *lpGlyph )
{
	atUIAPI_FreeMem( lpGlyph->lpBitmap );
}


//==============================================================
#endif
//==============================================================



static atVOID		OEM_InitFontSystem( atLONG lSysInst, atLONG lInitData1, atLONG lInitData2 )
{
#if defined(USE_FREETYPE) || defined(USE_WIN32GDI)
	atGRPX_FONTFUNCS lstFontFuncs;
	void *xx[2];
	xx[0] = (void *)lInitData1;//(void *)_AT("FreeType");
	xx[1] = (void *)lInitData2;//(void *)_AT("FreeType");
	lstFontFuncs.lpData = (void *)xx;
	lstFontFuncs.nDataType = 0;
	lstFontFuncs.fnInitFontMng = _InitFontMng;
	lstFontFuncs.fnCloseFontMng = _CloseFontMng;
	lstFontFuncs.fnGetFontMngCaps = _GetFontMngCaps;
	lstFontFuncs.fnCreateFont = _CreateFont;
	lstFontFuncs.fnDestroyFont = _DestroyFont;
	lstFontFuncs.fnSetFontStyle = _SetFontStyle;
	lstFontFuncs.fnSetFontAngle = _SetFontAngle;
	lstFontFuncs.fnGetFontMetric = _GetFontMetric;
	lstFontFuncs.fnGetCharIndex = _GetCharIndex;
	lstFontFuncs.fnGetGlyphMetric = _GetGlyphMetric;
	lstFontFuncs.fnGetGlyphBitmap = _GetGlyphBitmap;
	lstFontFuncs.fnFreeGlyphBitmap = _FreeGlyphBitmap;
	atGRPX_RegistOEMFontMng( g_lpGrpX, &lstFontFuncs );
#endif	
}

static atVOID		OEM_CloseFontSystem( NOARGS )
{
	atGRPX_ReleaseOEMFontMng( g_lpGrpX );
}

static atVOID 		OEM_SuspendFontSystem( NOARGS )
{
}

static atVOID 		OEM_ResumeFontSystem( NOARGS )
{
}

static OEM_UI_FONT 	OEM_CreateFont( atLPTSTR font_face, atINT font_size, atINT font_style, atBOOL IsStock )
{
//	atINT face, size, style;

	atGRPX_FONT *lpFont = atGRPX_CreateFont( g_lpGrpX, font_face, font_size, font_style, INTTOREAL(0) );
	if( IsStock && lpFont ) lpFont->nRefCount = 0xEFFF;

	return (OEM_UI_FONT)lpFont;
}

static atVOID 		OEM_DestroyFont( OEM_UI_FONT hFont )
{
	atGRPX_DestroyFont( (atGRPX_FONT*)hFont );
}

static atINT		OEM_GetStringWidth( OEM_UI_FONT hOemFont, atLPTSTR str, atINT len )
{	
	return atGRPX_GetStringWidth( (atGRPX_FONT*)hOemFont, str, len );
}

static atINT		OEM_GetFontHeight( OEM_UI_FONT hOemFont )
{
	return atGRPX_GetFontHeight( (atGRPX_FONT*)hOemFont );
}


void init_uiapi_font_oem() 
{
	OEM_UI_FONT_T oem_funcs = {
		OEM_InitFontSystem,
		OEM_CloseFontSystem,
		OEM_SuspendFontSystem,
		OEM_ResumeFontSystem,
		OEM_CreateFont,
		OEM_DestroyFont,
		OEM_GetStringWidth,
		OEM_GetFontHeight
	};

	OEM_UI_Font_SetOemFn( &oem_funcs );
}
