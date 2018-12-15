#include "OEM_UI_Font.h"
#include "GrpX.h"
#include "UI_API/UI_API.h"


#define USE_FREETYPE


extern atLPGRPXMNG g_lpGrpX; // GDI에서 GrpX 인스턴스를 먼저 생성하며, GrpX 생성시, 내장 폰트 시스템 초기화 됨.


//==============================================================
#ifdef USE_FREETYPE
//==============================================================

#define DEF_FONT_FILENAME _AT("tahoma.ttf")

//#define FONT_FULL_FILENAME	_AT("c:\\windows\\fonts\\tahoma.ttf")
#define FONT_FILENAME _AT("tahoma.ttf")
//#define FONT_FILENAME _AT("MALGUN.TTF")
//#define FONT_FILENAME _AT("HMKMRHD.ttf")

// 0 - No Use Cache
// 1 - Freetype Cache (Not Yet... -_-;;)
// 2 - Amos Cache
//#define USE_FREETYPE_CACHE 	0
#define USE_FREETYPE_CACHE 	2
#define USE_FREETYPE_STREAM


#include "ft2build.h"
#include FT_FREETYPE_H
#include FT_BITMAP_H

#if USE_FREETYPE_CACHE == 1
#include FT_CACHE_CHARMAP_H
#include FT_CACHE_IMAGE_H
#endif

#include "A_Math.h"

#ifndef MAX_PATH
#define MAX_PATH 256
#endif


#if USE_FREETYPE_CACHE == 1
#define FTCACHE_MAX_FACES  0 /// Default
#define FTCACHE_MAX_SIZES  0 /// Default
#define FTCACHE_MAX_BYTES  0 /// Default
//#define FTCACHE_MAX_FACES  65535
//#define FTCACHE_MAX_SIZES  64
//#define FTCACHE_MAX_BYTES  2048
#endif

typedef struct _tagOEMFontMng {
	FT_Library ftLibrary;
#if USE_FREETYPE_CACHE == 1
	FTC_Manager    ftcManager;
	FTC_CMapCache  ftcCMapCache;
	FTC_ImageCache ftcImageCache;
	FTC_SBitCache  ftcSBitCache;
#endif
} OEMFontMng, *LPOEMFontMng;

typedef struct _tagOEMFont {
	atTCHAR font_name[32];
	long font_size;
	atDWORD font_attr;
	atREAL font_angle;
//	FT_Library oem_library;
	LPOEMFontMng oem_fontmng;
	FT_Face oem_font;
#if USE_FREETYPE_CACHE == 1
	FTC_ImageTypeRec      mFTDesc;
	FT_Matrix matrix;
	FT_Vector pen;
	atBOOL		bTrans;
#endif
} OEMFont, *LPOEMFont;

#ifdef USE_FREETYPE_STREAM
static char *g_font_mem;
static long g_font_size;
static atHRES fontres = atNULL;
#endif

#if USE_FREETYPE_CACHE == 2

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

// 여기서부터 OEM Font 관련 함수들...

#if USE_FREETYPE_CACHE == 1
static FT_Face g_font;
FT_Error _Face_Requester( FTC_FaceID  face_id,
                         FT_Library  library,
                         FT_Pointer  request_data,
                         FT_Face*    aface )
{
//	if( g_font ){
//		*aface = g_font;
//	} else {
		FT_New_Memory_Face( library, (FT_Bytes)g_font_mem, g_font_size, 0, aface );
//	}

	return 0;
}
#endif

static atLPVOID	_InitFontMng( atLPVOID lpGrpX, atLPVOID lpData, atINT nDataType )
{
	LPOEMFontMng fontmng;
	FT_Error ret;
//    FT_Library ftLibrary;
#ifdef USE_FREETYPE_STREAM
	atTCHAR pathFont[MAX_PATH];
#endif

#ifdef USE_FREETYPE_STREAM

	if (nDataType == 0) {
		if (lpData != atNULL)
		{
			atUIAPI_strcpy(pathFont, (atLPTSTR)lpData);
		}
		else
		{
  		atUIAPI_strcpy(pathFont, FONT_FILENAME);
		}
		g_font_size = atUIAPI_GetFileSize(pathFont);
		g_font_mem = (char*)atUIAPI_AllocMem(g_font_size);
		atUIAPI_LoadFile(pathFont, g_font_mem, g_font_size, 0);
	} else {
		atHRESSRC ressrc = atUIAPI_GetResSource((atLPTSTR)nDataType);
		if (ressrc != atNULL) {
			atUIAPI_RESSRC_INFO ressrcinfo;
			atUIAPI_RES_INFO resinfo;
			atUIAPI_GetResSourceInfo(ressrc, &ressrcinfo);
			atUIAPI_GetResInfo(ressrc, lpData, &resinfo);
			//if (ressrcinfo.type == 3 && resinfo.nCompressMethod != 1) {
			//	fontres = atNULL;
			//	g_font_mem = (char *)ressrcinfo.lpPtr + resinfo.nStreamOffset;
			//	g_font_size = resinfo.nFileSize;
			//} else {
				fontres = atUIAPI_LoadRes(ressrc, lpData);
				if (fontres != atNULL) atUIAPI_GetResData(fontres, (atLPVOID *)&g_font_mem, &g_font_size);
			//}
		}
	}

#endif

#if USE_FREETYPE_CACHE == 2
	init_cache();
#endif

	fontmng = (LPOEMFontMng)atUIAPI_AllocMem( sizeof(OEMFontMng) );
	if( fontmng == atNULL ) return atNULL;
	atUIAPI_memset( fontmng, 0, sizeof(OEMFontMng) );

	ret = FT_Init_FreeType( &fontmng->ftLibrary );
	if( ret != 0 ) return atNULL;

	if( fontmng->ftLibrary == atNULL ) return atNULL;

#if USE_FREETYPE_CACHE == 1
  	ret = FTC_Manager_New( fontmng->ftLibrary, FTCACHE_MAX_FACES, FTCACHE_MAX_SIZES, FTCACHE_MAX_BYTES, 
						   _Face_Requester, atNULL, &fontmng->ftcManager );
	if( ret != 0 ) return atNULL;

  	ret = FTC_CMapCache_New( fontmng->ftcManager, &fontmng->ftcCMapCache );
	if( ret != 0 ) return atNULL;

	ret = FTC_SBitCache_New( fontmng->ftcManager, &fontmng->ftcSBitCache );
	if( ret != 0 ) return atNULL;

	ret = FTC_ImageCache_New( fontmng->ftcManager, &fontmng->ftcImageCache );
	if( ret != 0 ) return atNULL;
#endif

//	return (void *)ftLibrary;
	return (void *)fontmng;
}

static void	_CloseFontMng( atLPVOID lpInstData )
{
	LPOEMFontMng fontmng = (LPOEMFontMng)lpInstData;
//    FT_Library ftLibrary = (FT_Library)lpInstData;
	FT_Library ftLibrary = fontmng->ftLibrary;

#if USE_FREETYPE_CACHE == 2
	close_cache();
#endif

#if USE_FREETYPE_CACHE == 1
//	FTC_Node_Unref(ftcNode, ftcManager);
	if( fontmng->ftcManager )
		FTC_Manager_Done( fontmng->ftcManager );
#endif

#ifdef USE_FREETYPE_STREAM
	if (fontres != atNULL) {
		atUIAPI_FreeRes(fontres);
		fontres = atNULL;
		g_font_mem = atNULL;
	} else {
		if (g_font_mem) {
			atUIAPI_FreeMem(g_font_mem);
			g_font_mem = atNULL;
		}
	}
#endif

	FT_Done_FreeType( ftLibrary );
	atUIAPI_FreeMem( fontmng );
}

static atDWORD _GetFontMngCaps( atLPVOID lpInstData )
{
#if USE_FREETYPE_CACHE == 2
	return atGRPX_FONTATTR_ROTATABLE | atGRPX_FONTATTR_ROTATABLE_CHANGED | atGRPX_FONTATTR_SIZABLE | atGRPX_FONTATTR_CACHEABLE;
#else
	return atGRPX_FONTATTR_ROTATABLE | atGRPX_FONTATTR_ROTATABLE_CHANGED | atGRPX_FONTATTR_SIZABLE;
#endif
}

#include <math.h>
static atGRPX_OEM_FONT	_CreateFont( atLPVOID lpInstData, atLPTSTR FontName, atINT nSize, atDWORD dwStyle, atREAL rAngle, atINT *lpBPP )
{
    FT_Error  r;
	FT_Face	hFTFace;
	LPOEMFont fnt;
	int bpp;
	LPOEMFontMng fontmng = (LPOEMFontMng)lpInstData;
//    FT_Library ftLibrary = (FT_Library)lpInstData;
	FT_Library ftLibrary = fontmng->ftLibrary;

#ifndef USE_FREETYPE_STREAM
	atTCHAR pathFont[MAX_PATH];
	atCHAR pathFontA[MAX_PATH];

	atUIAPI_strcpy(pathFont, DEF_FONT_FILENAME);	
#endif

#if USE_FREETYPE_CACHE != 1
#ifndef USE_FREETYPE_STREAM
	atUIAPI_ToANSI( pathFontA, pathFont, -1 );
	r = FT_New_Face( ftLibrary, pathFontA, 0, &hFTFace );
#else	
	r = FT_New_Memory_Face( ftLibrary, (FT_Bytes)g_font_mem, g_font_size, 0, &hFTFace );
#endif
	if(r != FT_Err_Ok) return (atGRPX_OEM_FONT)atNULL;

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
		matrix.xx = (long)COS(-rAngle);
		matrix.xy = (long)-SIN(-rAngle);
		matrix.yx = (long)SIN(-rAngle);
		matrix.yy = (long)COS(-rAngle);
		pen.x = 0*64;
		pen.y = -0*64;
		FT_Set_Transform( hFTFace, &matrix, &pen );
	}
#endif

	fnt = (LPOEMFont)atUIAPI_AllocMem( sizeof(OEMFont) );
	atUIAPI_memset( fnt, 0, sizeof(OEMFont) );
	atUIAPI_strcpy( fnt->font_name, FontName );
	fnt->font_size = nSize;
	fnt->font_attr = dwStyle;
	fnt->font_angle = rAngle;
	fnt->oem_fontmng = fontmng;
#if USE_FREETYPE_CACHE == 1
	fnt->oem_font = atNULL;
#else
	fnt->oem_font = hFTFace;
#endif

#if USE_FREETYPE_CACHE == 1
	atUIAPI_memset( &fnt->mFTDesc, 0, sizeof(FTC_ImageTypeRec) );
#endif

	return (atGRPX_OEM_FONT)fnt;
}

static void			_DestroyFont( atGRPX_OEM_FONT OemFont )
{
	LPOEMFont fnt = (LPOEMFont)OemFont;
	FT_Face	hFTFace = fnt->oem_font;

	FT_Done_Face(hFTFace);

	atUIAPI_FreeMem( fnt );
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
#if USE_FREETYPE_CACHE != 1
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
	FT_Set_Transform( hFTFace, &matrix, &pen );
#else
	if( rAngle != 0 ){
		fnt->matrix.xx = (long)COS(-rAngle);
		fnt->matrix.xy = (long)-SIN(-rAngle);
		fnt->matrix.yx = (long)SIN(-rAngle);
		fnt->matrix.yy = (long)COS(-rAngle);
		fnt->pen.x = 0*64;
		fnt->pen.y = -0*64;
		fnt->bTrans = atTRUE;
	} else {
		fnt->bTrans = atFALSE;
	}
#endif

	fnt->font_angle = rAngle;
}

static void			_GetFontMetric( atGRPX_OEM_FONT OemFont, atINT *lpAscent, atINT *lpDescent )
{
	LPOEMFont fnt = (LPOEMFont)OemFont;
#if USE_FREETYPE_CACHE != 1
	FT_Face	hFTFace = fnt->oem_font;
	FT_SizeRec *size = hFTFace->size;
#endif

#if USE_FREETYPE_CACHE == 1
	if( lpAscent ) *lpAscent = 0;
	if( lpDescent ) *lpDescent = -0;
#else
	if( lpAscent ) *lpAscent = size->metrics.ascender >> 6;
	if( lpDescent ) *lpDescent = -(size->metrics.descender >> 6);
#endif
}

static atINT			_GetCharIndex( atGRPX_OEM_FONT OemFont, atTCHAR Chr )
{
	LPOEMFont fnt = (LPOEMFont)OemFont;
	FT_Face	hFTFace = fnt->oem_font;

#if USE_FREETYPE_CACHE == 1
	int idx;
	idx = FTC_CMapCache_Lookup( fnt->oem_fontmng->ftcCMapCache, fnt->mFTDesc.face_id, -1, Chr );
	return idx;
#else
	return FT_Get_Char_Index( hFTFace, Chr );
#endif
}

static atBOOL			_GetGlyphMetric( atGRPX_OEM_FONT OemFont, atUINT index, atSIZE *lpSize )
{
#if USE_FREETYPE_CACHE == 2
	atGRPX_GLYPHINFO *lpGlyph;
	int font_size;
	atREAL font_angle;
#endif
	LPOEMFont fnt = (LPOEMFont)OemFont;
#if USE_FREETYPE_CACHE != 1
	FT_Face	hFTFace = fnt->oem_font;
	FT_GlyphSlot  glyph = hFTFace->glyph;
	FT_SizeRec *size = hFTFace->size;
#endif

#if USE_FREETYPE_CACHE == 2
	font_size = fnt->font_size;
	font_angle = fnt->font_angle;
	if( (lpGlyph = font_find( fnt->oem_font, index, font_size, 0, font_angle )) == atNULL ){
		lpGlyph = add_cache( fnt->oem_fontmng->ftLibrary, fnt->oem_font, index, font_size, 0, font_angle );
	}
	if( lpGlyph == atNULL ) return atFALSE;
	
	lpSize->nWidth = lpGlyph->nHorizAdvance;
	lpSize->nHeight = (size->metrics.ascender >> 6) - (size->metrics.descender >> 6);
#elif USE_FREETYPE_CACHE == 1
/*
	FT_Load_Glyph(hFTFace, index, FT_LOAD_DEFAULT);

	if( fnt->font_attr & atGRPX_FS_BOLD ){
		FT_Bitmap_Embolden( fnt->oem_fontmng->ftLibrary, &hFTFace->glyph->bitmap, (hFTFace->size->metrics.x_ppem*10/100)*64, 0 );
	}

	lpSize->nWidth = (glyph->metrics.horiAdvance >> 6);
	lpSize->nHeight = ((size->metrics.ascender >> 6) - (size->metrics.descender >> 6));
*/
	lpSize->nWidth = 24;
	lpSize->nHeight = 24;
#else
	FT_Load_Glyph(hFTFace, index, FT_LOAD_DEFAULT);

	if( fnt->font_attr & atGRPX_FS_BOLD ){
		FT_Bitmap_Embolden( fnt->oem_fontmng->ftLibrary, &hFTFace->glyph->bitmap, (hFTFace->size->metrics.x_ppem*10/100)*64, 0 );
	}

	lpSize->nWidth = (glyph->metrics.horiAdvance >> 6);
	lpSize->nHeight = ((size->metrics.ascender >> 6) - (size->metrics.descender >> 6));
#endif		

	return atTRUE;
}

static atBOOL			_GetGlyphBitmap( atGRPX_OEM_FONT OemFont, atUINT index, atGRPX_GLYPHINFO *lpGlyph )
{
#if USE_FREETYPE_CACHE == 2
	atGRPX_GLYPHINFO *lpCacheGlyph;
	int font_size;
	int font_attr;
	atREAL font_angle;
#endif
	LPOEMFont fnt = (LPOEMFont)OemFont;
#if USE_FREETYPE_CACHE != 1
	FT_Face	hFTFace = fnt->oem_font;
	FT_GlyphSlot  glyph = hFTFace->glyph;
#endif

#if USE_FREETYPE_CACHE == 2
	FT_SizeRec *size = hFTFace->size;
	font_size = fnt->font_size;
	font_angle = fnt->font_angle;
	font_attr = fnt->font_attr;
	if( (lpCacheGlyph = font_find( fnt->oem_font, index, font_size, font_attr, font_angle )) == atNULL ){
		lpCacheGlyph = add_cache( fnt->oem_fontmng->ftLibrary, fnt->oem_font, index, font_size, font_attr, font_angle );
	}
	if( lpCacheGlyph == atNULL ) return atFALSE;

	atUIAPI_memcpy( lpGlyph, lpCacheGlyph, sizeof(atGRPX_GLYPHINFO) );

	return atTRUE;
#elif USE_FREETYPE_CACHE == 1
	{
	FT_Error ret;
	FT_Glyph ft_glyph;
	FT_BitmapGlyph glyph;
	FTC_ImageTypeRec desc = fnt->mFTDesc; // Our query, with .flags = FT_LOAD_DEFAULT

    desc.face_id = fnt->mFTDesc.face_id; 
//    desc.width   = (short) typeface->scale.width; 
//    desc.height  = (short) typeface->scale.height; 
desc.width   = (short)24; 
desc.height  = (short)24; 
    desc.flags   = FT_LOAD_DEFAULT; 

    ret = FTC_ImageCache_Lookup( fnt->oem_fontmng->ftcImageCache, &desc, index, &ft_glyph, NULL ); 
	if( ret != 0 ) return atFALSE;

    if( fnt->bTrans ){ 
//        FT_Glyph        tmp_glyph; 
//        ret = FT_Glyph_Copy( ft_glyph, &tmp_glyph ); 
//        FT_Glyph_Transform( tmp_glyph, &fnt->matrix, NULL); 
//        ft_glyph = tmp_glyph; 
        FT_Glyph_Transform( ft_glyph, &fnt->matrix, NULL); 
    } 

    ret = FT_Glyph_To_Bitmap(&ft_glyph, FT_RENDER_MODE_NORMAL, NULL, 0); 
	if( ret != 0 ) return atFALSE;

    glyph = (FT_BitmapGlyph)ft_glyph;

	lpGlyph->nHorizAdvance = (short)(glyph->root.advance.x >> 6);
	lpGlyph->dwAttr = atGRPX_GLYPHATTR_NONE;

	lpGlyph->nBearingX = glyph->left;
	lpGlyph->nBearingY = glyph->top;
	lpGlyph->nWidth = glyph->bitmap.width;
	lpGlyph->nHeight = glyph->bitmap.rows;
	lpGlyph->rAdvX = FIXEDTOREAL(glyph->root.advance.x << (16-6));
	lpGlyph->rAdvY = FIXEDTOREAL(-(glyph->root.advance.y << (16-6)));
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

	lpGlyph->OEMGlyph = ft_glyph;
//    FT_Done_Glyph(ft_glyph); 
	}
#else
//	FT_Load_Glyph(hFTFace, index, FT_LOAD_RENDER);
//	FT_Load_Glyph(hFTFace, index, FT_LOAD_MONOCHROME);
	FT_Load_Glyph(hFTFace, index, FT_LOAD_DEFAULT);

	// 만약 모노크롬도 아니고 256그레이도 아니면, 256그레이로 통일.
	if( (glyph->bitmap.pixel_mode != FT_PIXEL_MODE_MONO) && (glyph->bitmap.pixel_mode != FT_PIXEL_MODE_GRAY) )
		FT_Render_Glyph(glyph, FT_RENDER_MODE_NORMAL);
//	FT_Render_Glyph(glyph, FT_RENDER_MODE_MONO);

	if( fnt->font_attr & atGRPX_FS_BOLD ){
		FT_Bitmap_Embolden( fnt->oem_fontmng->ftLibrary, &glyph->bitmap, (hFTFace->size->metrics.x_ppem*10/100)*64, 0 );
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
#if USE_FREETYPE_CACHE != 1
	LPOEMFont fnt = (LPOEMFont)OemFont;
	FT_Face	hFTFace = fnt->oem_font;

//	FT_Done_GlyphSlot( hFTFace->glyph );
#else
	if( lpGlyph == atNULL ) return;
	FT_Done_Glyph(lpGlyph->OEMGlyph); 
	lpGlyph->OEMGlyph = atNULL;
#endif
}


//==============================================================
#endif
//==============================================================



atVOID		OEM_InitFontSystem( atLONG lSysInst, atLONG lInitData1, atLONG lInitData2 )
{
#if defined(USE_FREETYPE)
	atGRPX_FONTFUNCS lstFontFuncs;
	lstFontFuncs.lpData = (void *)lInitData1;//_AT("FreeType");
	lstFontFuncs.nDataType = lInitData2;//_AT("FreeType");
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


atVOID		OEM_CloseFontSystem( NOARGS )
{
	atGRPX_ReleaseOEMFontMng( g_lpGrpX );
}

atVOID 		OEM_SuspendFontSystem( NOARGS )
{
}

atVOID 		OEM_ResumeFontSystem( NOARGS )
{
}

OEM_UI_FONT 	OEM_CreateFont( atLPTSTR font_face, atINT font_size, atINT font_style, atBOOL IsStock )
{
//	atINT face, size, style;

	atGRPX_FONT *lpFont = atGRPX_CreateFont( g_lpGrpX, font_face, font_size, font_style, INTTOREAL(0) );
	if( IsStock && lpFont ) lpFont->nRefCount = 0xEFFF;

	return (OEM_UI_FONT)lpFont;
}

atVOID 		OEM_DestroyFont( OEM_UI_FONT hFont )
{
	atGRPX_DestroyFont( (atGRPX_FONT*)hFont );
}

atINT		OEM_GetStringWidth( OEM_UI_FONT hFont, atLPTSTR str, atINT len )
{	
	return atGRPX_GetStringWidth( (atGRPX_FONT*)hFont, str, len );
}

atINT		OEM_GetFontHeight( OEM_UI_FONT hFont )
{
	return atGRPX_GetFontHeight( (atGRPX_FONT*)hFont );
}

atINT		OEM_GetGlyph( OEM_UI_FONT hFont )
{
	return atGRPX_GetFontHeight( (atGRPX_FONT*)hFont );
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
