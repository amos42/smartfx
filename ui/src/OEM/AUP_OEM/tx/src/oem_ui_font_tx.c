#include "OEM_UI_Font.h"
#include "GrpX.h"
#include "UI_API/UI_API.h"

#include "SA_mem.h"

#define USE_FREETYPE   //:  bitmap font 
//#define USE_WIN32GDI


extern atLPGRPXMNG g_lpGrpX; // GDI에서 GrpX 인스턴스를 먼저 생성하며, GrpX 생성시, 내장 폰트 시스템 초기화 됨.


//==============================================================
//==============================================================


//#define TTF_FONT_FILE _AT("tahoma.ttf")
#define TTF_FONT_FILE _AT("ARIALUNI_ColumbusLB.ttf")

#ifdef _SDIC_VIEWMAP_
  #define TTF_FONT_PATH _AT("I:\\MapView\\res")
#else
  #define TTF_FONT_PATH _AT("I:\\Columbus\\res")
#endif

#define USE_FREETYPE_CACHE
#define USE_FREETYPE_CACHE_DALLOC
#define USE_FREETYPE_STREAM



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
static char *g_font_mem = atNULL;
static long g_font_size = 0;
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

static _FONTCACHE g_font_cache[GLYPH_CACHE_SIZE];
static int g_vv = 0;
#ifdef USE_FREETYPE_CACHE_DALLOC
static char *g_cache_tmp = atNULL;
#endif

#ifdef USE_FREETYPE_CACHE_DALLOC
#include "A_Dalloc.h"
#endif


#define atUIAPI_FontAllocMem(size)     SYSAPI_AllocMem(size) 
#define atUIAPI_FontFreeMem(ptr) 	   SYSAPI_FreeMem(ptr)


#define BNS_FT_Load_Glyph   		FT_Load_Glyph
#define BNS_FT_Bitmap_Embolden		FT_Bitmap_Embolden
#define BNS_FT_Render_Glyph			FT_Render_Glyph
#define BNS_FT_Init_FreeType		FT_Init_FreeType
#define BNS_FT_Done_FreeType		FT_Done_FreeType
#define BNS_FT_New_Memory_Face		FT_New_Memory_Face
#define BNS_FT_Set_Pixel_Sizes		FT_Set_Pixel_Sizes
#define BNS_FT_Get_Char_Index		FT_Get_Char_Index
#define BNS_FT_Load_Glyph			FT_Load_Glyph	
#define BNS_FT_Set_Transform		FT_Set_Transform
#define BNS_FT_Done_Face			FT_Done_Face
#define BNS_FT_Set_Transform		FT_Set_Transform
#define BNS_FT_Get_Char_Index       FT_Get_Char_Index
#define BNS_FT_Library_Version		FT_Library_Version


static void init_cache(void)
{
	int i;
	_FONTCACHE *pcache;

#ifdef USE_FREETYPE_CACHE_DALLOC
	g_cache_tmp = atUIAPI_AllocMem( 1*1024*1024L );
	atDALLOC_InitHeap( g_cache_tmp, 1*1024*1024L );
#endif

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
#ifdef USE_FREETYPE_CACHE_DALLOC
				if( pcache->lpGlyph->lpBitmap ) atDALLOC_FreeMem( g_cache_tmp, pcache->lpGlyph->lpBitmap );
				atDALLOC_FreeMem( g_cache_tmp, pcache->lpGlyph );
#else
				if( pcache->lpGlyph->lpBitmap ) atUIAPI_FreeMem( pcache->lpGlyph->lpBitmap );
				atUIAPI_FreeMem( pcache->lpGlyph );
#endif
				pcache->lpGlyph = atNULL;
			}
		}
		pcache++;
	} 
	
#ifdef USE_FREETYPE_CACHE_DALLOC
		atUIAPI_FreeMem( g_cache_tmp );
		g_cache_tmp = atNULL;
#endif
	
}

static atGRPX_GLYPHINFO *add_cache( FT_Library hLibrary, FT_Face hFTFace, int index, int size, int attr, atREAL angle )
{
	int i;
	_FONTCACHE *pcache;
	int old_time, old_idx;
	atGRPX_GLYPHINFO *lpGlyph;
	FT_GlyphSlot  glyph = hFTFace->glyph;

//	BGL_FT_Load_Glyph(hFTFace, index, FT_LOAD_RENDER);
//	BGL_FT_Load_Glyph(hFTFace, index, FT_LOAD_MONOCHROME);
	BNS_FT_Load_Glyph(hFTFace, index, FT_LOAD_DEFAULT);
	// 만약 모노크롬도 아니고 256그레이도 아니면, 256그레이로 통일.
	if( (glyph->bitmap.pixel_mode != FT_PIXEL_MODE_MONO) && (glyph->bitmap.pixel_mode != FT_PIXEL_MODE_GRAY) )
		BNS_FT_Render_Glyph(glyph, FT_RENDER_MODE_NORMAL);
//	BGL_FT_Render_Glyph(glyph, FT_RENDER_MODE_MONO);

	if( attr & atGRPX_FS_BOLD ){
		BNS_FT_Bitmap_Embolden( hLibrary, &glyph->bitmap, (hFTFace->size->metrics.x_ppem*10/100)*64, 0 );
	}

#ifdef USE_FREETYPE_CACHE_DALLOC
	lpGlyph = (atGRPX_GLYPHINFO *)atDALLOC_AllocMem(g_cache_tmp, sizeof(atGRPX_GLYPHINFO));
#else
	lpGlyph = (atGRPX_GLYPHINFO *)atUIAPI_AllocMem(sizeof(atGRPX_GLYPHINFO));
#endif

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
#ifdef USE_FREETYPE_CACHE_DALLOC
	lpGlyph->lpBitmap = atDALLOC_AllocMem( g_cache_tmp, lpGlyph->nFontWidthBytes * lpGlyph->nHeight );	
#else
	lpGlyph->lpBitmap = atUIAPI_AllocMem( lpGlyph->nFontWidthBytes * lpGlyph->nHeight );	
#endif
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
#ifdef USE_FREETYPE_CACHE_DALLOC
	  if( pcache->lpGlyph->lpBitmap ) atDALLOC_FreeMem( g_cache_tmp, pcache->lpGlyph->lpBitmap );
	  atDALLOC_FreeMem( g_cache_tmp, pcache->lpGlyph );
#else
	  if( pcache->lpGlyph->lpBitmap ) atUIAPI_FreeMem( pcache->lpGlyph->lpBitmap );
	  atUIAPI_FreeMem( pcache->lpGlyph );
#endif
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

#endif //USE_FREETYPE_CACHE


// 여기서부터 OEM Font 관련 함수들...

static atLPVOID	_InitFontMng( atLPVOID lpGrpX, atLPVOID lpData )
{
    FT_Library ftLibrary;
	FT_Int ver_major, ver_min, ver_rel;
/*	
#ifdef USE_FREETYPE_STREAM
	atTCHAR pathFont[MAX_PATH];

	atUIAPI_sprintf(pathFont, _AT("%s\\%s"), TTF_FONT_PATH, TTF_FONT_FILE);

	g_font_size = atUIAPI_GetFileSize(pathFont);
	if(g_font_size)
	{
		g_font_mem = (unsigned char*)atUIAPI_FontAllocMem(g_font_size);
		if(!g_font_mem)
		{
			SYSAPI_DebugPrintf(_AT("[navi] error _InitFontMng::atUIAPI_FontAllocMem(%d)\n"), g_font_size);	
			return atNULL;
		}
			
		atUIAPI_LoadFile( pathFont, g_font_mem, g_font_size, 0 );
	}
	else
	{
		SYSAPI_DebugPrintf(_AT("[navi] Error : load system font (%s)\n"), pathFont);
	}
#endif

#ifdef USE_FREETYPE_CACHE
	init_cache();
#endif
*/

	BNS_FT_Init_FreeType( &ftLibrary );
	if( ftLibrary == atNULL ) return atNULL;

	BNS_FT_Library_Version(ftLibrary, &ver_major, &ver_min, &ver_rel);
	fprintf(stderr, 
"FT Library version %d.%d.%d\n", ver_major, ver_min, ver_rel);

	return (void *)ftLibrary;
}

static void	_CloseFontMng( atLPVOID lpInstData )
{
    FT_Library ftLibrary = (FT_Library)lpInstData;

//#ifdef USE_FREETYPE_CACHE
//	close_cache();
//#endif

/*
#ifdef USE_FREETYPE_STREAM
	if( g_font_mem ){
		atUIAPI_FontFreeMem(g_font_mem);
		g_font_mem = atNULL;
	}
#endif
*/

	BNS_FT_Done_FreeType(ftLibrary);
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
static atGRPX_OEM_FONT	_CreateFont( atLPVOID lpInstData, atLPTSTR FontName, atINT nSize, atDWORD dwStyle, atREAL rAngle, atINT *lpBPP )
{
    FT_Error  r;
	FT_Face	hFTFace = atNULL;
	LPOEMFont fnt;
    FT_Library ftLibrary = (FT_Library)lpInstData;
	int bpp;

#ifndef USE_FREETYPE_STREAM
	atTCHAR pathFont[MAX_PATH];
	atCHAR pathFontA[MAX_PATH];
#ifdef FONT_FILE
	atUIAPI_strcpy(pathFont, FONT_FILE);	
#else
	atTCHAR pathWindows[MAX_PATH];
#ifdef __USES_UNICODE_
	GetWindowsDirectoryW( pathWindows, MAX_PATH );
#else
	GetWindowsDirectoryA( pathWindows, MAX_PATH );
#endif
	atUIAPI_sprintf(pathFont, _AT("%s\\Fonts\\%s.ttf"), pathWindows, _AT("tahoma"));
#endif
#endif

/*
	if( atUIAPI_strcmp( FontName, _AT("System") ){
		sprintf(pathFont, "%s\\Fonts\\%s.ttf", pathWindows, "tahoma");
	} else {
		sprintf(pathFont, "%s\\Fonts\\%s.ttf", pathWindows, FontName);
	}
*/

	//SYSAPI_DebugPrintf(_AT("[navi] _CreateFont  LINE %d\n"), __LINE__);

	
#ifndef USE_FREETYPE_STREAM
	atUIAPI_ToANSI( pathFontA, pathFont, -1 );
	r = BGL_FT_New_Face( ftLibrary, pathFontA, 0, &hFTFace );
#else	
	r = BNS_FT_New_Memory_Face( ftLibrary, (FT_Bytes)g_font_mem, g_font_size, 0, &hFTFace );
#endif
	if(r != FT_Err_Ok) return (atGRPX_OEM_FONT)atNULL;

	BNS_FT_Set_Pixel_Sizes(hFTFace, 0, nSize);

	BNS_FT_Load_Glyph( hFTFace, BNS_FT_Get_Char_Index(hFTFace, _ATC('.')), FT_LOAD_DEFAULT );
	if( hFTFace->glyph->bitmap.pixel_mode == FT_PIXEL_MODE_MONO )
		bpp = 1;
	else if( hFTFace->glyph->bitmap.pixel_mode == FT_PIXEL_MODE_GRAY )
		bpp = 8;
	else
		bpp = 0;

	//SYSAPI_DebugPrintf(_AT("[navi] _CreateFont  LINE %d\n"), __LINE__);

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
		matrix.xx = (long)COS(-rAngle);
		matrix.xy = (long)-SIN(-rAngle);
		matrix.yx = (long)SIN(-rAngle);
		matrix.yy = (long)COS(-rAngle);
		pen.x = 0*64;
		pen.y = -0*64;
		BNS_FT_Set_Transform( hFTFace, &matrix, &pen );
	}

	//SYSAPI_DebugPrintf(_AT("[navi] _CreateFont  LINE %d\n"), __LINE__);

	fnt = (LPOEMFont)atUIAPI_FontAllocMem( sizeof(OEMFont) );
	if(!fnt)
	{
		SYSAPI_DebugPrintf(_AT("[navi] error _CreateFont  LINE %d\n"), __LINE__);
		return atNULL;
	}
	atUIAPI_strcpy( fnt->font_name, FontName );
	fnt->font_size = nSize;
	fnt->font_attr = dwStyle;
	fnt->font_angle = rAngle;
	fnt->oem_library = ftLibrary;
	fnt->oem_font = hFTFace;

	//SYSAPI_DebugPrintf(_AT("[navi] _CreateFont  LINE %d\n"), __LINE__);
	return (atGRPX_OEM_FONT)fnt;
}

static void			_DestroyFont( atGRPX_OEM_FONT OemFont )
{
	LPOEMFont fnt = (LPOEMFont)OemFont;
	FT_Face	hFTFace = fnt->oem_font;

	BNS_FT_Done_Face(hFTFace);

	atUIAPI_FontFreeMem( fnt );
}

static void			_SetFontStyle( atGRPX_OEM_FONT OemFont, atINT Style )
{
	LPOEMFont fnt = (LPOEMFont)OemFont;
	FT_Face	hFTFace = fnt->oem_font;

	fnt->font_attr = Style;
}

static void			_SetFontAngle( atGRPX_OEM_FONT OemFont, atREAL rAngle )
{
	LPOEMFont fnt = (LPOEMFont)OemFont;
	FT_Face	hFTFace = fnt->oem_font;
	FT_Matrix matrix;
	FT_Vector pen;
	FT_SizeRec *size = hFTFace->size;

	matrix.xx = (long)COS(-rAngle);
	matrix.xy = (long)-SIN(-rAngle);
	matrix.yx = (long)SIN(-rAngle);
	matrix.yy = (long)COS(-rAngle);
	pen.x = 0*64;
	pen.y = -0*64;
	BNS_FT_Set_Transform( hFTFace, &matrix, &pen );

	fnt->font_angle = rAngle;
}

static void			_GetFontMetric( atGRPX_OEM_FONT OemFont, atINT *lpAscent, atINT *lpDescent )
{
	LPOEMFont fnt = (LPOEMFont)OemFont;
	FT_Face	hFTFace = fnt->oem_font;
	FT_SizeRec *size = hFTFace->size;

	if( lpAscent ) *lpAscent = size->metrics.ascender >> 6;
	if( lpDescent ) *lpDescent = -(size->metrics.descender >> 6);
}

static atINT			_GetCharIndex( atGRPX_OEM_FONT OemFont, atTCHAR Chr )
{
	LPOEMFont fnt = (LPOEMFont)OemFont;
	FT_Face	hFTFace = fnt->oem_font;

	return BNS_FT_Get_Char_Index( hFTFace, Chr );
}

static atBOOL			_GetGlyphMetric( atGRPX_OEM_FONT OemFont, atUINT index, atSIZE *lpSize )
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
	BNS_FT_Load_Glyph(hFTFace, index, FT_LOAD_DEFAULT);

	if( fnt->font_attr & atGRPX_FS_BOLD ){
		BNS_FT_Bitmap_Embolden( fnt->oem_library, &hFTFace->glyph->bitmap, (hFTFace->size->metrics.x_ppem*10/100)*64, 0 );
	}

	lpSize->nWidth = (glyph->metrics.horiAdvance >> 6);
	lpSize->nHeight = ((size->metrics.ascender >> 6) - (size->metrics.descender >> 6));
#endif		

	return atTRUE;
}

static atBOOL			_GetGlyphBitmap( atGRPX_OEM_FONT OemFont, atUINT index, atGRPX_GLYPHINFO *lpGlyph )
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
//	BGL_FT_Load_Glyph(hFTFace, index, FT_LOAD_RENDER);
//	BGL_FT_Load_Glyph(hFTFace, index, FT_LOAD_MONOCHROME);
	BNS_FT_Load_Glyph(hFTFace, index, FT_LOAD_DEFAULT);

	// 만약 모노크롬도 아니고 256그레이도 아니면, 256그레이로 통일.
	if( (glyph->bitmap.pixel_mode != FT_PIXEL_MODE_MONO) && (glyph->bitmap.pixel_mode != FT_PIXEL_MODE_GRAY) )
		BNS_FT_Render_Glyph(glyph, FT_RENDER_MODE_NORMAL);
//	BGL_FT_Render_Glyph(glyph, FT_RENDER_MODE_MONO);

	if( fnt->font_attr & atGRPX_FS_BOLD ){
		BNS_FT_Bitmap_Embolden( fnt->oem_library, &glyph->bitmap, (hFTFace->size->metrics.x_ppem*10/100)*64, 0 );
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

static void			_FreeGlyphBitmap( atGRPX_OEM_FONT OemFont, atGRPX_GLYPHINFO *lpGlyph )
{
	LPOEMFont fnt = (LPOEMFont)OemFont;
	FT_Face	hFTFace = fnt->oem_font;

//	BGL_FT_Done_GlyphSlot( hFTFace->glyph );
}


atVOID		OEM_InitFontSystem( atLONG lSysInst, atLONG lInitData1, atLONG lInitData2 )
{
#ifdef USE_FREETYPE_STREAM
	atTCHAR pathFont[MAX_PATH];

	atUIAPI_sprintf(pathFont, _AT("%s\\%s"), TTF_FONT_PATH, TTF_FONT_FILE);

	g_font_size = atUIAPI_GetFileSize(pathFont);
	if(g_font_size)
	{
		g_font_mem = (unsigned char*)atUIAPI_FontAllocMem(g_font_size);
		if(!g_font_mem)
		{
			SYSAPI_DebugPrintf(_AT("[navi] error _InitFontMng::atUIAPI_FontAllocMem(%d)\n"), g_font_size);	
			return atNULL;
		}
			
		atUIAPI_LoadFile( pathFont, g_font_mem, g_font_size, 0 );
	}
	else
	{
		SYSAPI_DebugPrintf(_AT("[navi] Error : load system font (%s)\n"), pathFont);
	}
#endif

#ifdef USE_FREETYPE_CACHE
	init_cache();
#endif

	OEM_ResumeFontSystem();
}

atVOID		OEM_CloseFontSystem( NOARGS )
{
	OEM_SuspendFontSystem();
	
#ifdef USE_FREETYPE_CACHE
	close_cache();
#endif

#ifdef USE_FREETYPE_STREAM
	if( g_font_mem ){
		atUIAPI_FontFreeMem(g_font_mem);
		g_font_mem = atNULL;
	}
#endif
}

atVOID 		OEM_SuspendFontSystem( NOARGS )
{
	atGRPX_ReleaseOEMFontMng( g_lpGrpX );
}

atVOID 		OEM_ResumeFontSystem( NOARGS )
{
#if defined(USE_FREETYPE)
	atGRPX_FONTFUNCS lstFontFuncs;
	lstFontFuncs.lpData = (void *)_AT("FreeType");
	lstFontFuncs.fnInitFontMng    = _InitFontMng;
	lstFontFuncs.fnCloseFontMng   = _CloseFontMng;
	lstFontFuncs.fnGetFontMngCaps = _GetFontMngCaps;
	lstFontFuncs.fnCreateFont     = _CreateFont;
	lstFontFuncs.fnDestroyFont    = _DestroyFont;
	lstFontFuncs.fnSetFontStyle   = _SetFontStyle;
	lstFontFuncs.fnSetFontAngle   = _SetFontAngle;
	lstFontFuncs.fnGetFontMetric  = _GetFontMetric;
	lstFontFuncs.fnGetCharIndex   = _GetCharIndex;
	lstFontFuncs.fnGetGlyphMetric = _GetGlyphMetric;
	lstFontFuncs.fnGetGlyphBitmap = _GetGlyphBitmap;
	lstFontFuncs.fnFreeGlyphBitmap = _FreeGlyphBitmap;
	atGRPX_RegistOEMFontMng( g_lpGrpX, &lstFontFuncs );
#endif	
}

OEM_FONT 	OEM_CreateFont( atLPTSTR font_face, atINT font_size, atINT font_style, atBOOL IsStock )
{
	atGRPX_FONT *lpFont = atGRPX_CreateFont( g_lpGrpX, font_face, font_size, font_style, INTTOREAL(0) );
	if( IsStock && lpFont ) lpFont->nRefCount = 0xEFFF;

	return (OEM_FONT)lpFont;
}

atVOID 		OEM_DestroyFont( OEM_FONT hFont )
{
	atGRPX_DestroyFont( (atGRPX_FONT*)hFont );
}

atINT		OEM_GetStringWidth( OEM_FONT hOemFont, atLPTSTR str, atINT len )
{	
	return atGRPX_GetStringWidth( (atGRPX_FONT*)hOemFont, str, len );
}

atINT		OEM_GetFontHeight( OEM_FONT hOemFont )
{
	return atGRPX_GetFontHeight( (atGRPX_FONT*)hOemFont );
}

