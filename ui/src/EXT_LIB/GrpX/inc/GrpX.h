/*#############################################################################
-------------------------------------------------------------------------------

                                  GrpX.h
                           (* =================== *)


 Version           : 1.0.0
 Dependent Modules :
 Author            : Ju, Gyeong-min ( Amos )
 Create            : 2001/04/25    01:47:09am
 Update            : 2003/04/29    00:00:00am
 C Compiler        : ADS 1.01, SDT 2.51, Borland C/C++ 5.0, Visual C/C++ 6.0
 Assembler         : ADS 1.01, SDT 2.51, Borland TASM 3.1, MASM 6.0
 Description       : GrpX - 2D Graphics function pool

-------------------------------------------------------------------------------
#############################################################################*/
#ifndef __GRPX_H_
#define __GRPX_H_


#if defined(__GNUC__)
#pragma GCC diagnostic ignored "-Wunknown-pragmas"
#endif
#if defined(__GNUC__) || defined(__arm)
#pragma anon_unions
#endif


/* 설정 파일 */
#ifdef __GRPX_USER_CFG_
#ifdef __USER_GRPX_CFG_H_
#include __USER_GRPX_CFG_H_
#else
#include "GrpX_CFG.h"
#endif
#else  /* Default Config */
#define atGRPX_PIXEL_BPP           (16)
#define atGRPX_ALPHA_BPP           (8)
//#define __FIXEDPTR_
//#define FIXED_RADIX_SIZE    (16)
#define atGRPX_DEF_MAX_CANVAS_COUNT	(256)
#define atGRPX_DEF_FONT_NAME       _AT("System")
#define atGRPX_DEF_FONT_SIZE		(16)
#define atGRPX_USES_ALPHABLEND
#define atGRPX_USES_POLYGON_CACHE
#define atGRPX_BASE_POLYGON_ENTRY   (3000)
#define atGRPX_MAX_POLYGON_ENTRY   (10000)
#ifdef atGRPX_USES_POLYGON_CACHE
#define atGRPX_AUTOINC_POLYGON_ENTRY
#endif
#endif

/*
#define __FIXEDPTR_
#define atGRPX_PIXEL_BPP         (16)
#define atGRPX_ALPHA_BPP         (8)
*/

#ifndef __AT_TYPES_H_
#include "AT_Types.h"       /* 기본 제공하는 타입 선언 */
#endif
#ifndef __AT_REAL_H_
#include "AT_Real.h"		/* 실수 사용을 위해 필요! */
#endif
/*#include "AT_Math.h"*/	/* 삼각함수 사용을 위해 필요! */


#ifdef __cplusplus
  extern "C" {
#endif

 
#define MAKE565(r,g,b) ((atWORD)(((((atWORD)((r) & 0xF8)) << (5+6-3))) | \
					   ((((atWORD)((g) & 0xFC)) << (5-2))) | \
					   (((atWORD)((b) & 0xF8)) >> 3)))
#define GET565R(cl)    ((atBYTE)(((cl) >> 8) & 0xF8))
#define GET565G(cl)    ((atBYTE)(((cl) >> 3) & 0xFC))
#define GET565B(cl)    ((atBYTE)(((cl) << 3) & 0xF8))


#if atGRPX_PIXEL_BPP == 16
#define MAKERGB(r,g,b) MAKE565((r),(g),(b))
#define GET_RED(cl)    GET565R((cl))
#define GET_GREEN(cl)  GET565G((cl))
#define GET_BLUE(cl)   GET565B((cl))
#elif atGRPX_PIXEL_BPP == 24
#define MAKERGB(r,g,b) (((atDWORD)(r))<<16) | ((atDWORD)(g))<<8) | ((atDWORD)(b)))
#define GET_RED(cl)    ((atBYTE)((cl)>>16))
#define GET_GREEN(cl)  ((atBYTE)((cl)>>8))
#define GET_BLUE(cl)   ((atBYTE)((cl)))
#else
#define MAKERGB(r,g,b) 0xFF  /* 사실상 지원하지 않는다. */
#define GET_RED(cl)    0xFF
#define GET_GREEN(cl)  0xFF
#define GET_BLUE(cl)   0xFF
#endif

/* Stock Color */
#define atGRPX_COLOR_BLACK		MAKERGB(0,0,0)
#define atGRPX_COLOR_WHITE		MAKERGB(0xFF,0xFF,0xFF)
#define atGRPX_COLOR_RED		MAKERGB(0xFF,0,0)
#define atGRPX_COLOR_GREEN		MAKERGB(0,0xFF,0)
#define atGRPX_COLOR_BLUE		MAKERGB(0,0,0xFF)


#if atGRPX_PIXEL_BPP == 8
#define atGRPX_DEF_TRANS_COLOR ((atGRPX_COLOR)0x00)
#elif atGRPX_PIXEL_BPP == 16
#define atGRPX_DEF_TRANS_COLOR ((atGRPX_COLOR)0xf81f)
#elif atGRPX_PIXEL_BPP == 24
#define atGRPX_DEF_TRANS_COLOR ((atGRPX_COLOR)0xf81fe4)
#endif

#if atGRPX_PIXEL_BPP == 8
typedef atBYTE           atGRPX_PIXEL;
#elif atGRPX_PIXEL_BPP == 16
typedef atWORD           atGRPX_PIXEL;
#elif atGRPX_PIXEL_BPP == 24
typedef atBYTE           atGRPX_PIXEL[3];
#elif atGRPX_PIXEL_BPP == 32
typedef atBYTE           atGRPX_PIXEL[4];
#endif

typedef atDWORD          atGRPX_COLOR;

typedef struct _tagatRGBCOLOR {
	atBYTE		nRed;
	atBYTE		nGreen;
	atBYTE		nBlue;
} atRGBCOLOR;

typedef struct _tagatARGBCOLOR {
	atBYTE		nRed;
	atBYTE		nGreen;
	atBYTE		nBlue;
	atBYTE		nAlpha;
} atARGBCOLOR;

#ifndef __DEF_RPOINT_TYPE_
#define  __DEF_RPOINT_TYPE_
typedef struct tagatRPOINT {
    atREAL rX, rY;
} atRPOINT;

typedef struct tagatRRECT {
    atREAL rStartX, rStartY;
    atREAL rEndX, rEndY;
} atRRECT;
#endif


/* atGRPX_COLOR 타입과 atGRPX_PIXEL 타입을 전환을 위한 매크로. 현재 그냥 1:1로 변환하고 있음. */
#define COLOR_TO_PIXEL(a) ((atGRPX_PIXEL)(a))
#define PIXEL_TO_COLOR(a) ((atGRPX_COLOR)(a))

typedef atCALLBACKALLOC atGRPX_FUNC_ALLOC;
typedef atGRPX_FUNC_ALLOC* atLPGRPX_FUNC_ALLOC;
typedef atCALLBACKFREE atGRPX_FUNC_FREE;
typedef atGRPX_FUNC_FREE* atLPGRPX_FUNC_FREE;
typedef atBOOL (atGRPX_FUNC_COORD_CONV)( atLPVOID lpParam, atLONG *lpDesX, atLONG *lpDesY, atLONG nSrcX, atLONG nSrcY );
typedef atGRPX_FUNC_COORD_CONV* atLPGRPX_FUNC_COORD_CONV;
typedef atBOOL (atGRPX_FUNC_COORD_CONV_REAL)( atLPVOID lpParam, atREAL *lpDesX, atREAL *lpDesY, atREAL rSrcX, atREAL rSrcY );
typedef atGRPX_FUNC_COORD_CONV_REAL* atLPGRPX_FUNC_COORD_CONV_REAL;


/* Kernel */

typedef struct _tagatGRPXSCRINFO {
    atLPVOID	 lpFrameBufferPtr;
    atINT        nFrameWidth, nFrameHeight;
    atINT		 nBPP;    
	atINT		 nVMemWidth;
} atGRPXSCRINFO;

typedef struct _tagatGRPXFUNCS {
	atLPVOID				   lpData;
	atLPGRPX_FUNC_ALLOC   fnAllocMem;
	atLPGRPX_FUNC_FREE     fnFreeMem;
} atGRPXFUNCS;

typedef atHANDLE atHGRPX_CANVAS;

typedef atVOID *atGRPX_OEM_FONT;
typedef atVOID *atGRPX_OEM_GLYPH;

#define atGRPX_GLYPHATTR_NONE            	(0x00)
#define atGRPX_GLYPHATTR_NORMAL            (0x00)
#define atGRPX_GLYPHATTR_OWNERBITMAP (0x01)

typedef struct _tagatGLYPHINFO {
    atDWORD    dwAttr;
    atINT    nHorizAdvance;
    atSHORT    nBearingX, nBearingY;
    atSHORT    nWidth, nHeight;
	atSHORT    nBPP;
	atSHORT    nFontWidthBytes;
    atLPVOID   lpBitmap;
	atREAL rAdvX, rAdvY;
    atGRPX_OEM_GLYPH 	OEMGlyph;
} atGRPX_GLYPHINFO;

typedef struct _tagatGRPX_FONTFUNCS {
	atLPVOID		lpData;
	atINT			nDataType;
	atLPVOID		(*fnInitFontMng)( /*atLPGRPXMNG*/ atLPVOID lpGrpX, atLPVOID lpData, atINT nDataType );
	atDWORD			(*fnGetFontMngCaps)( atLPVOID lpInstData );
	atVOID			(*fnCloseFontMng)( atLPVOID lpInstData );
	atGRPX_OEM_FONT	(*fnCreateFont)( atLPVOID lpInstData, atLPTSTR FontName, atINT nSize, atDWORD dwStyle, atREAL rAngle, atINT *nBPP );
	atVOID			(*fnDestroyFont)( atGRPX_OEM_FONT OemFont );
	atVOID			(*fnSetFontStyle)( atGRPX_OEM_FONT OemFont, atINT Style );
	atVOID			(*fnSetFontAngle)( atGRPX_OEM_FONT OemFont, atREAL rAngle );
	atVOID			(*fnGetFontMetric)( atGRPX_OEM_FONT OemFont, atINT *lpAscent, atINT *lpDescent );
	atINT			(*fnGetCharIndex)( atGRPX_OEM_FONT OemFont, atTCHAR Chr );
	atBOOL			(*fnGetGlyphMetric)( atGRPX_OEM_FONT OemFont, atUINT index, atSIZE *lpSize );
	atBOOL			(*fnGetGlyphBitmap)( atGRPX_OEM_FONT OemFont, atUINT index, atGRPX_GLYPHINFO *lpGlyph );
	atVOID			(*fnFreeGlyphBitmap)( atGRPX_OEM_FONT OemFont, atGRPX_GLYPHINFO *lpGlyph );
} atGRPX_FONTFUNCS;


typedef struct _tagatGRPXMNG {
	atGRPXFUNCS			lstFuncs;
	atGRPXSCRINFO		ScrInfo;

	atHGRPX_CANVAS 	lstCanvas[atGRPX_DEF_MAX_CANVAS_COUNT];

   	atDWORD         	dwFontAttr;
	atGRPX_FONTFUNCS 	lstFontFuncs;
	atLPVOID			lpOEMFontMng;
	
	atLPVOID			lpDefFont;

	atLPVOID			lpTempBuf;

#ifdef atGRPX_USES_POLYGON_CACHE
	atLPVOID				lpPolygonCache;  /* 폴리곤 그리기 위한 캐쉬 */
	atINT				nPolygonCacheSize;  /* 폴리곤 그리기 위한 캐쉬 */
#endif
} atGRPXMNG, *atLPGRPXMNG;


atLPVOID		atGRPX_AllocMem( atLPGRPXMNG lpGrpX, atLONG nLen );
atVOID		atGRPX_FreeMem( atLPGRPXMNG lpGrpX, atLPVOID lpPtr );

atBOOL  		atGRPX_Reset( atLPGRPXMNG lpGrpX, atINT nWidth, atINT nHeight, atINT nBPP, atVOID *pFrameBufPtr, atINT VMemWidth, atGRPXFUNCS *lstFuncs );
atLPGRPXMNG     atGRPX_Initialize( atINT nWidth, atINT nHeight, atINT nBPP, atVOID *pFrameBufPtr, atINT VMemWidth, atGRPXFUNCS *lstFuncs );
atVOID            atGRPX_Finalize( atLPGRPXMNG lpGrpX );
atBOOL  		atGRPX_SetScreenResolution( atLPGRPXMNG lpGrpX, atINT nWidth, atINT nHeight, atINT VMemWidth );
atBOOL  		atGRPX_SetFrameBuffer( atLPGRPXMNG lpGrpX, atLPVOID pFrameBufPtr );
atVOID            atGRPX_GetScreenInfo( atLPGRPXMNG lpGrpX, atGRPXSCRINFO *lpScrInfo );
atGRPX_COLOR			atGRPX_GetRGB( atBYTE r, atBYTE g, atBYTE b );
atBYTE			atGRPX_GetRValue( atGRPX_COLOR clColor );
atBYTE			atGRPX_GetGValue( atGRPX_COLOR clColor );
atBYTE			atGRPX_GetBValue( atGRPX_COLOR clColor );
atDWORD         atGRPX_GetCodePage( atLPGRPXMNG lpGrpX );
atVOID            atGRPX_SetCodePage( atLPGRPXMNG lpGrpX, atDWORD CodePage );
atVOID            atGRPX_RegistOEMFontMng( atLPGRPXMNG lpGrpX, atGRPX_FONTFUNCS *lstFontFuncs );
atVOID            atGRPX_ReleaseOEMFontMng( atLPGRPXMNG lpGrpX );


/* Font */
#define atGRPX_FONT_SIZE_TINY	8   /// 6x8
#define atGRPX_FONT_SIZE_SMALL	10  /// 7x10
#define atGRPX_FONT_SIZE_MIDDLE	12  /// 8x12
#define atGRPX_FONT_SIZE_EXTEND 16	/// 8x16
#define atGRPX_FONT_SIZE_LARGE	20  /// 12x20

#define atGRPX_FONTATTR_NONE                   			(0x0000)
#define atGRPX_FONTATTR_OWNERBUFFER    			(0x0001)
#define atGRPX_FONTATTR_ROTATABLE         			(0x0002)
#define atGRPX_FONTATTR_ROTATABLE_CHANGED         	(0x0004)
#define atGRPX_FONTATTR_SIZABLE              			(0x0008)
#define atGRPX_FONTATTR_CACHEABLE         			(0x0010)

#define atGRPX_FS_MASK             (0x03)
#define atGRPX_FS_NORMAL         (0x00)
#define atGRPX_FS_BOLD              (0x01)
#define atGRPX_FS_ITALIC           (0x02)

typedef struct _tagatGRPX_FONT {
	atLPGRPXMNG     	lpGrpX;

   	atTCHAR         	szFontName[32];
   	atDWORD         	dwAttr;
   	atDWORD         	dwStyle;
   	atINT           		nFontBPP;

	atINT			nFontSize;
	atINT			nAscent, nDescent;
	atREAL			rAngle;

	atINT           		nRefCount;
    atGRPX_OEM_FONT 	OEMFont;
} atGRPX_FONT, *atLPGRPX_FONT;

atBOOL 		atGRPX_InitFontMng( atLPGRPXMNG lpGrpX );
atBOOL 		atGRPX_CloseFontMng( atLPGRPXMNG lpGrpX );
atGRPX_FONT   * atGRPX_CreateFont( atLPGRPXMNG lpGrpX, atLPTSTR szFontName, atINT nSize, atDWORD dwStyle, atREAL rAngle );
atVOID            atGRPX_DestroyFont( atGRPX_FONT *lpFont );
atGRPX_FONT	  * atGRPX_ReferenceFont( atGRPX_FONT *lpFont );
atGRPX_FONT   * atGRPX_ReleaseFont( atGRPX_FONT *lpFont, atBOOL bAutoDestory );
atVOID            atGRPX_SetFontStyle( atGRPX_FONT *lpFont, atINT Style );
atBOOL		  atGRPX_SetFontAngle( atGRPX_FONT *lpFont, atREAL rAngle );
atINT           atGRPX_GetStringWidth( atGRPX_FONT *lpFont, atLPTSTR szStr, atINT iLen );
atVOID  	    atGRPX_GetFontSize( atGRPX_FONT *lpFont, atINT *lpAscent, atINT *lpDescent );
atINT           atGRPX_GetFontHeight( atGRPX_FONT *lpFont );
atUINT          atGRPX_GetCharIndex( atGRPX_FONT *lpFont, atTCHAR Chr );
atBOOL			atGRPX_GetGlyphMetric( atGRPX_FONT *lpFont, atUINT nIndex, atSIZE *lpSize );
atBOOL			atGRPX_GetGlyph( atGRPX_FONT *lpFont, atUINT nIndex, atGRPX_GLYPHINFO *lpGlyph );
atVOID			atGRPX_ReleaseGlyph( atGRPX_FONT *lpFont, atGRPX_GLYPHINFO *lpGlyph );
atGRPX_OEM_FONT atGRPX_GetOEMFont( atGRPX_FONT *lpFont );

atBOOL 			atGRPX_DisconnectOEMFontMng( atLPGRPXMNG lpGrpX );
atBOOL 			atGRPX_DisconnectOEMFont( atGRPX_FONT *lpFont );
atBOOL 			atGRPX_ResyncOEMFontMng( atLPGRPXMNG lpGrpX );
atBOOL 			atGRPX_ResyncOEMFont( atGRPX_FONT *lpFont );


/* Canvas */

#define atGRPX_CANVAS_ATTR_NONE           (0x00)
#define atGRPX_CANVAS_ATTR_PARENTBUFFER    (0x00)
#define atGRPX_CANVAS_ATTR_OWNERBUFFER    (0x01)
#define atGRPX_CANVAS_ATTR_EXTBUFFER    (0x02)
#define atGRPX_CANVAS_ATTR_NORMAL    	(atGRPX_CANVAS_ATTR_PARENTBUFFER)

typedef struct _tagatGRPX_CANVAS {
	atLPGRPXMNG     lpGrpX;
	atBOOL               bActivate;
	atINT		     nCanvasID;

    atULONG         dwAttr;
//    atREGION       rgnOrgArea;
    atREGION       rgnArea;
    atRECT         rtOrgClipRect, rtScreenClipRect;
	atINT           nBPP;
    atLPVOID        lpVideoPtr;
    atINT           nVMemWidth;
	atLPVOID		lpPaletteTable;
    
//    atGRPX_FONT   * lpCurFont;
    atGRPX_COLOR         clFrColor, clBgColor;
} atGRPX_CANVAS, *atLPGRPX_CANVAS;

atGRPX_CANVAS * atGRPX_CreateCanvas( atLPGRPXMNG lpGrpX, atINT X, atINT Y, atINT Width, atINT Height, atDWORD dwAttr, atLPVOID lpFrameBuf, atINT nFrameWidth );
atVOID            atGRPX_DestroyCanvas( atGRPX_CANVAS *lpCanvas );
atBOOL 		atGRPX_InitCanvas( atGRPX_CANVAS *lpCanvas, atLPGRPXMNG lpGrpX, atINT X, atINT Y, atINT Width, atINT Height, atDWORD dwAttr, atLPVOID lpFrameBuf, atINT nFrameWidth );
atVOID 		atGRPX_ReleaseCanvas( atGRPX_CANVAS *lpCanvas );
atVOID    	atGRPX_ActivateCanvas( atGRPX_CANVAS *lpCanvas );
atVOID    	atGRPX_DeactivateCanvas( atGRPX_CANVAS *lpCanvas );
atBOOL		atGRPX_IsActiveCanvas( atGRPX_CANVAS *lpCanvas );
atVOID 		atGRPX_RecalcCanvas( atGRPX_CANVAS *lpCanvas );
atREGION     * atGRPX_GetCanvasArea( atGRPX_CANVAS *lpCanvas );
atINT             atGRPX_GetBPP( atGRPX_CANVAS *lpCanvas );
atGRPX_PIXEL * atGRPX_GetFrameBufferPtr( atGRPX_CANVAS *lpCanvas );
atGRPX_PIXEL * atGRPX_GetFrameBufferPtrXY( atGRPX_CANVAS *lpCanvas, atINT X, atINT Y );
atINT             atGRPX_GetVMemWidth( atGRPX_CANVAS *lpCanvas );
atRECT       * atGRPX_GetClipRect( atGRPX_CANVAS *lpCanvas );
atRECT 	   * atGRPX_GetOriginalClipRect( atGRPX_CANVAS *lpCanvas );
atBOOL          atGRPX_SetClipRect( atGRPX_CANVAS *lpCanvas, atINT nStartX, atINT nStartY, atINT nEndX, atINT nEndY );
atBOOL 		atGRPX_SetClipRect2( atGRPX_CANVAS *lpCanvas, atRECT *lpClip );
atVOID            atGRPX_Cvs2Scr( atGRPX_CANVAS *lpCanvas, atINT *nX, atINT *nY );
atVOID            atGRPX_Cvs2Scr2( atGRPX_CANVAS *lpCanvas, atINT *nX1, atINT *nY1, atINT *nX2, atINT *nY2 );
//atGRPX_FONT   * atGRPX_SetCurFont( atGRPX_CANVAS *lpCanvas, atGRPX_FONT *lpFont );
//atGRPX_FONT   * atGRPX_GetCurFont( atGRPX_CANVAS *lpCanvas );
atGRPX_COLOR         atGRPX_SetFrColor( atGRPX_CANVAS *lpCanvas, atGRPX_COLOR color );
atGRPX_COLOR         atGRPX_GetFrColor( atGRPX_CANVAS *lpCanvas );
atGRPX_COLOR         atGRPX_SetBgColor( atGRPX_CANVAS *lpCanvas, atGRPX_COLOR color );
atGRPX_COLOR         atGRPX_GetBgColor( atGRPX_CANVAS *lpCanvas );
atBOOL			atGRPX_MoveCanvas( atGRPX_CANVAS *lpCanvas, atINT X, atINT Y );
atBOOL			atGRPX_ChangeCanvasSize( atGRPX_CANVAS *lpCanvas, atINT nWidth, atINT nHeight, atINT nFrameWidth );
atBOOL			atGRPX_RealizeCanvas( atGRPX_CANVAS *lpCanvas );
atVOID	        atGRPX_CopyCanvas( atGRPX_CANVAS *lpCanvas_Des, atINT nDesX, atINT nDesY, atGRPX_CANVAS *lpCanvas_Src, atINT nX, atINT nY, atINT nWidth, atINT nHeight );


/* GDI Method */

#if atGRPX_ALPHA_BPP == 1
#define atGRPX_MAX_ALPHA           (1)
#elif atGRPX_ALPHA_BPP == 4
#define atGRPX_MAX_ALPHA           (0xF)
#elif atGRPX_ALPHA_BPP == 8
#define atGRPX_MAX_ALPHA           (0xFF)
#elif atGRPX_ALPHA_BPP == 16
#define atGRPX_MAX_ALPHA           (0xFFFF)
#else
#error "Alpha BPP is Invalidate"
#endif

#define  atGRPX_LINEEXT_NONE 				(0)
#define  atGRPX_LINEEXT_START_EXTEND		(1)
#define  atGRPX_LINEEXT_END_EXTEND		(2)
#define  atGRPX_LINEEXT_START_ROUNDCAP	(4)
#define  atGRPX_LINEEXT_END_ROUNDCAP		(8)
#define  atGRPX_LINEEXT_EXTEND				(atGRPX_LINEEXT_START_EXTEND | atGRPX_LINEEXT_END_EXTEND)
#define  atGRPX_LINEEXT_ROUNDCAP			(atGRPX_LINEEXT_START_ROUNDCAP | atGRPX_LINEEXT_END_ROUNDCAP)

atVOID            atGRPX_ClearScreen( atGRPX_CANVAS *lpCanvas, atGRPX_COLOR Color );
atVOID			atGRPX_ClearScreenAll( atGRPX_CANVAS *lpCanvas, atGRPX_COLOR Color );
atGRPX_COLOR        atGRPX_GetPixel( atGRPX_CANVAS *lpCanvas, atINT X, atINT Y );
atGRPX_PIXEL *      atGRPX_DrawPixel( atGRPX_CANVAS *lpCanvas, atINT X, atINT Y, atGRPX_COLOR Color );
atGRPX_PIXEL *      atGRPX_DrawPixelTrans( atGRPX_CANVAS *lpCanvas, atINT X, atINT Y, atGRPX_COLOR Color, atINT nAlpha );
atGRPX_PIXEL *		atGRPX_DrawPixelTransR( atGRPX_CANVAS *lpCanvas, atINT X, atINT Y, atGRPX_COLOR Color, atREAL rAlpha );
atGRPX_PIXEL * 		atGRPX_DrawPixelReal( atGRPX_CANVAS *lpCanvas, atREAL rX, atREAL rY, atGRPX_COLOR Color );
atVOID 			atGRPX_DrawPixelAA( atGRPX_CANVAS *lpCanvas, atINT nX, atINT nY, atREAL rDX, atREAL rDY, atGRPX_COLOR Color );
atVOID 			atGRPX_DrawPixelRealAA( atGRPX_CANVAS *lpCanvas, atREAL rX, atREAL rY, atGRPX_COLOR Color );
atVOID			atGRPX_DrawPixelAA_One( atGRPX_CANVAS *lpCanvas, atINT nX, atINT nY, atREAL rDX, atREAL rDY, atGRPX_COLOR Color );
atVOID            atGRPX_DrawHorizLine( atGRPX_CANVAS *lpCanvas, atINT StartX, atINT StartY, atINT Length, atGRPX_COLOR Color );
atINT            atGRPX_DrawHorizLineDash( atGRPX_CANVAS *lpCanvas, atINT StartX, atINT StartY, atINT Length, atGRPX_COLOR Color, atINT nDashLen, atINT nGap, atINT init_gapdash );
atVOID            atGRPX_DrawHorizLineTrans( atGRPX_CANVAS *lpCanvas, atINT StartX, atINT StartY, atINT Length, atGRPX_COLOR Color, atINT nAlpha );
atVOID            atGRPX_DrawVertLine( atGRPX_CANVAS *lpCanvas, atINT StartX, atINT StartY, atINT Length, atGRPX_COLOR Color );
atINT            atGRPX_DrawVertLineDash( atGRPX_CANVAS *lpCanvas, atINT StartX, atINT StartY, atINT Length, atGRPX_COLOR Color, atINT nDashLen, atINT nGap, atINT init_gapdash );
atVOID            atGRPX_DrawVertLineTrans( atGRPX_CANVAS *lpCanvas, atINT StartX, atINT StartY, atINT Length, atGRPX_COLOR Color, atINT nAlpha );
atVOID            atGRPX_DrawLine( atGRPX_CANVAS *lpCanvas, atINT StartX, atINT StartY, atINT EndX, atINT EndY, atGRPX_COLOR Color );
atINT            atGRPX_DrawLineDash( atGRPX_CANVAS *lpCanvas, atINT StartX, atINT StartY, atINT EndX, atINT EndY, atGRPX_COLOR Color, atINT nDashLen, atINT nGap, atINT init_gapdash );
atINT            atGRPX_DrawLineDot( atGRPX_CANVAS *lpCanvas, atINT StartX, atINT StartY, atINT EndX, atINT EndY, atGRPX_COLOR Color, atINT nGap, atINT init_gapdash );
atVOID            atGRPX_DrawLineAA( atGRPX_CANVAS *lpCanvas, atINT StartX, atINT StartY, atINT EndX, atINT EndY, atGRPX_COLOR Color );
atINT            atGRPX_DrawLineDashAA( atGRPX_CANVAS *lpCanvas, atINT StartX, atINT StartY, atINT EndX, atINT EndY, atGRPX_COLOR Color, atINT nDashLen, atINT nGap, atINT init_gapdash );
atINT            atGRPX_DrawLineDotAA( atGRPX_CANVAS *lpCanvas, atINT StartX, atINT StartY, atINT EndX, atINT EndY, atGRPX_COLOR Color, atINT nGap, atINT init_gapdash );
atVOID            atGRPX_DrawLine_REAL( atGRPX_CANVAS *lpCanvas, atREAL rStartX, atREAL rStartY, atREAL rEndX, atREAL rEndY, atGRPX_COLOR Color );
atVOID            atGRPX_DrawLineAA_REAL( atGRPX_CANVAS *lpCanvas, atREAL rStartX, atREAL rStartY, atREAL rEndX, atREAL rEndY, atGRPX_COLOR Color );
atVOID            atGRPX_DrawLineTrans( atGRPX_CANVAS *lpCanvas, atINT StartX, atINT StartY, atINT EndX, atINT EndY, atGRPX_COLOR Color, atINT nAlpha  );
atVOID		 atGRPX_DrawLineWidth( atGRPX_CANVAS *lpCanvas, atINT StartX, atINT StartY, atINT EndX, atINT EndY, atGRPX_COLOR Color, atINT nWidth );
atVOID		 atGRPX_DrawLineWidthDash( atGRPX_CANVAS *lpCanvas, atINT StartX, atINT StartY, atINT EndX, atINT EndY, atGRPX_COLOR Color, atINT nWidth, atINT nDashLen, atINT nGap );
atVOID		 atGRPX_DrawLineWidthDot( atGRPX_CANVAS *lpCanvas, atINT StartX, atINT StartY, atINT EndX, atINT EndY, atGRPX_COLOR Color, atINT nWidth, atINT nGap );
atVOID	 	 atGRPX_DrawLineWidthAA( atGRPX_CANVAS *lpCanvas, atINT StartX, atINT StartY, atINT EndX, atINT EndY, atGRPX_COLOR Color, atINT nWidth );
atVOID	 	 atGRPX_DrawLineWidthDashAA( atGRPX_CANVAS *lpCanvas, atINT StartX, atINT StartY, atINT EndX, atINT EndY, atGRPX_COLOR Color, atINT nWidth, atINT nDashLen, atINT nGap );
atVOID	 	 atGRPX_DrawLineWidthDotAA( atGRPX_CANVAS *lpCanvas, atINT StartX, atINT StartY, atINT EndX, atINT EndY, atGRPX_COLOR Color, atINT nWidth, atINT nGap );
atVOID		 atGRPX_DrawLineWidthExt( atGRPX_CANVAS *lpCanvas, atINT StartX, atINT StartY, atINT EndX, atINT EndY, atGRPX_COLOR Color, atINT nWidth, atINT ExtType );
atVOID	 	 atGRPX_DrawLineWidthExtAA( atGRPX_CANVAS *lpCanvas, atINT StartX, atINT StartY, atINT EndX, atINT EndY, atGRPX_COLOR Color, atINT nWidth, atINT ExtType );
atVOID            atGRPX_DrawCrossLine( atGRPX_CANVAS *lpCanvas, atINT X, atINT Y, atINT Length, atGRPX_COLOR Color );
atVOID            atGRPX_DrawRect( atGRPX_CANVAS *lpCanvas, atINT StartX, atINT StartY, atINT EndX, atINT EndY, atGRPX_COLOR Color );
atVOID            atGRPX_DrawRectWidth( atGRPX_CANVAS *lpCanvas, atINT StartX, atINT StartY, atINT EndX, atINT EndY, atGRPX_COLOR Color, atINT nWidth );
atVOID            atGRPX_DrawRectTrans( atGRPX_CANVAS *lpCanvas, atINT StartX, atINT StartY, atINT EndX, atINT EndY, atGRPX_COLOR Color, atINT nAlpha );
atVOID            atGRPX_FillRect( atGRPX_CANVAS *lpCanvas, atINT StartX, atINT StartY, atINT EndX, atINT EndY, atGRPX_COLOR Color );
atVOID            atGRPX_FillRectTrans( atGRPX_CANVAS *lpCanvas, atINT StartX, atINT StartY, atINT EndX, atINT EndY, atGRPX_COLOR Color, atINT nAlpha );
atVOID            atGRPX_FillRectXOR( atGRPX_CANVAS *lpCanvas, atINT StartX, atINT StartY, atINT EndX, atINT EndY );
atVOID            atGRPX_DrawCircle( atGRPX_CANVAS *lpCanvas, atINT X, atINT Y, atINT Radius, atGRPX_COLOR Color );
atVOID            atGRPX_DrawCircleTrans( atGRPX_CANVAS *lpCanvas, atINT X, atINT Y, atINT Radius, atGRPX_COLOR Color, atINT nAlpha );
atVOID            atGRPX_FillCircle( atGRPX_CANVAS *lpCanvas, atINT X, atINT Y, atINT Radius, atGRPX_COLOR Color );
atVOID            atGRPX_FillCircleAA( atGRPX_CANVAS *lpCanvas, atINT X, atINT Y, atINT Radius, atGRPX_COLOR Color );
atVOID            atGRPX_FillCircleTrans( atGRPX_CANVAS *lpCanvas, atINT X, atINT Y, atINT Radius, atGRPX_COLOR Color, atINT nAlpha );
atVOID            atGRPX_DrawEllipse( atGRPX_CANVAS *lpCanvas, atINT nX, atINT nY, atINT RadiusX, atINT RediusY, atGRPX_COLOR Color );
atVOID            atGRPX_FillEllipse( atGRPX_CANVAS *lpCanvas, atINT nX, atINT nY, atINT RadiusX, atINT RediusY, atGRPX_COLOR Color );
atVOID            atGRPX_DrawTriangle( atGRPX_CANVAS *lpCanvas, atINT X1, atINT Y1, atINT X2, atINT Y2, atINT X3, atINT Y3, atGRPX_COLOR Color );
atVOID            atGRPX_DrawTriangleAA( atGRPX_CANVAS *lpCanvas, atINT X1, atINT Y1, atINT X2, atINT Y2, atINT X3, atINT Y3, atGRPX_COLOR Color );
atVOID            atGRPX_FillTriangle( atGRPX_CANVAS *lpCanvas, atINT X1, atINT Y1, atINT X2, atINT Y2, atINT X3, atINT Y3, atGRPX_COLOR Color );
atVOID		 atGRPX_FillTriangle_REAL( atGRPX_CANVAS *lpCanvas, atREAL rX1, atREAL rY1, atREAL rX2, atREAL rY2, atREAL rX3, atREAL rY3, atGRPX_COLOR Color );
atVOID            atGRPX_FillTriangleAA( atGRPX_CANVAS *lpCanvas, atINT X1, atINT Y1, atINT X2, atINT Y2, atINT X3, atINT Y3, atGRPX_COLOR Color );
atVOID		 atGRPX_FillTriangleAA_REAL( atGRPX_CANVAS *lpCanvas, atREAL rX1, atREAL rY1, atREAL rX2, atREAL rY2, atREAL rX3, atREAL rY3, atGRPX_COLOR Color );
atVOID            atGRPX_DrawQuadrangle( atGRPX_CANVAS *lpCanvas, atINT X1, atINT Y1, atINT X2, atINT Y2, atINT X3, atINT Y3, atINT X4, atINT Y4, atGRPX_COLOR Color );
atVOID            atGRPX_DrawQuadrangleAA( atGRPX_CANVAS *lpCanvas, atINT X1, atINT Y1, atINT X2, atINT Y2, atINT X3, atINT Y3, atINT X4, atINT Y4, atGRPX_COLOR Color );
atVOID            atGRPX_FillQuadrangle( atGRPX_CANVAS *lpCanvas, atINT X1, atINT Y1, atINT X2, atINT Y2, atINT X3, atINT Y3, atINT X4, atINT Y4, atGRPX_COLOR Color );
atVOID		  atGRPX_FillQuadrangle_REAL( atGRPX_CANVAS *lpCanvas, atREAL rX1, atREAL rY1, atREAL rX2, atREAL rY2, atREAL rX3, atREAL rY3, atREAL rX4, atREAL rY4, atGRPX_COLOR Color );
atVOID            atGRPX_FillQuadrangleAA( atGRPX_CANVAS *lpCanvas, atINT X1, atINT Y1, atINT X2, atINT Y2, atINT X3, atINT Y3, atINT X4, atINT Y4, atGRPX_COLOR Color );
atVOID		  atGRPX_FillQuadrangleAA_REAL( atGRPX_CANVAS *lpCanvas, atREAL rX1, atREAL rY1, atREAL rX2, atREAL rY2, atREAL rX3, atREAL rY3, atREAL rX4, atREAL rY4, atGRPX_COLOR Color );
atVOID			  atGRPX_DrawPolyLine( atGRPX_CANVAS *lpCanvas, atPOINT lpPtrList[], atINT nCount, atBOOL bClosed, atGRPX_COLOR Color );
atVOID			  atGRPX_DrawPolyLineDash( atGRPX_CANVAS *lpCanvas, atPOINT lpPtrList[], atINT nCount, atBOOL bClosed, atGRPX_COLOR Color, atINT nDashLen, atINT nGap );
atVOID			  atGRPX_DrawPolyLineDot( atGRPX_CANVAS *lpCanvas, atPOINT lpPtrList[], atINT nCount, atBOOL bClosed, atGRPX_COLOR Color, atINT nGap );
atVOID			atGRPX_DrawPolyLine_REAL( atGRPX_CANVAS *lpCanvas, atRPOINT *lpPtrList, atINT nCount, atBOOL bClosed, atGRPX_COLOR Color );
atVOID			  atGRPX_DrawPolyLineAA( atGRPX_CANVAS *lpCanvas, atPOINT lpPtrList[], atINT nCount, atBOOL bClosed, atGRPX_COLOR Color );
atVOID			  atGRPX_DrawPolyLineDashAA( atGRPX_CANVAS *lpCanvas, atPOINT lpPtrList[], atINT nCount, atBOOL bClosed, atGRPX_COLOR Color, atINT nDashLen, atINT nGap );
atVOID			  atGRPX_DrawPolyLineDotAA( atGRPX_CANVAS *lpCanvas, atPOINT lpPtrList[], atINT nCount, atBOOL bClosed, atGRPX_COLOR Color, atINT nGap );
atVOID			atGRPX_DrawPolyLineAA_REAL( atGRPX_CANVAS *lpCanvas, atRPOINT *lpPtrList, atINT nCount, atBOOL bClosed, atGRPX_COLOR Color );
atVOID            atGRPX_DrawPolyLineWidth( atGRPX_CANVAS *lpCanvas, atPOINT lpPtrList[], atINT nCount, atBOOL bClosed, atGRPX_COLOR Color, atINT nWidth );
atVOID            atGRPX_DrawPolyLineWidthAA( atGRPX_CANVAS *lpCanvas, atPOINT lpPtrList[], atINT nCount, atBOOL bClosed, atGRPX_COLOR Color, atINT nWidth );
atVOID            atGRPX_DrawPolyLineWidthDash( atGRPX_CANVAS *lpCanvas, atPOINT lpPtrList[], atINT nCount, atBOOL bClosed, atGRPX_COLOR Color, atINT nWidth, atINT nDashLen, atINT nGap );
atVOID            atGRPX_DrawPolyLineWidthDashAA( atGRPX_CANVAS *lpCanvas, atPOINT lpPtrList[], atINT nCount, atBOOL bClosed, atGRPX_COLOR Color, atINT nWidth, atINT nDashLen, atINT nGap );
atVOID            atGRPX_DrawPolyLineWidthDot( atGRPX_CANVAS *lpCanvas, atPOINT lpPtrList[], atINT nCount, atBOOL bClosed, atGRPX_COLOR Color, atINT nWidth, atINT nGap );
atVOID            atGRPX_DrawPolyLineWidthDotAA( atGRPX_CANVAS *lpCanvas, atPOINT lpPtrList[], atINT nCount, atBOOL bClosed, atGRPX_COLOR Color, atINT nWidth, atINT nGap );
atVOID		atGRPX_DrawPolyLineWidthEx( atGRPX_CANVAS *lpCanvas, atPOINT *lpPtrList, atINT nCount, atBOOL bClosed, atGRPX_COLOR Color, atINT nWidth, atLPGRPX_FUNC_COORD_CONV_REAL lpCnvFunc, atLPVOID lpParam );
atVOID 		atGRPX_DrawPolyLineWidthExAA( atGRPX_CANVAS *lpCanvas, atPOINT *lpPtrList, atINT nCount, atBOOL bClosed, atGRPX_COLOR Color, atINT nWidth, atLPGRPX_FUNC_COORD_CONV_REAL lpCnvFunc, atLPVOID lpParam );
atVOID 		atGRPX_DrawPolyLineWidthEdge( atGRPX_CANVAS *lpCanvas, atPOINT *lpPtrList, atINT nCount, atBOOL bClosed, atGRPX_COLOR Color, atINT nWidth );
atVOID 		atGRPX_DrawPolyLineWidthEdgeAA( atGRPX_CANVAS *lpCanvas, atPOINT *lpPtrList, atINT nCount, atBOOL bClosed, atGRPX_COLOR Color, atINT nWidth );
atVOID            atGRPX_FillConvexPolygon( atGRPX_CANVAS *lpCanvas, atPOINT lpPtrList[], atINT nCount, atGRPX_COLOR Color );
atVOID            atGRPX_FillConvexPolygon_REAL( atGRPX_CANVAS *lpCanvas, atRPOINT lpPtrList[], atINT nCount, atGRPX_COLOR Color );
atVOID		 atGRPX_FillConvexPolygonEx_REAL( atGRPX_CANVAS *lpCanvas, atRPOINT lpPtrList[], atINT nCount, atINT lpAttr[], atGRPX_COLOR Color );
atVOID            atGRPX_FillConvexPolygonAA( atGRPX_CANVAS *lpCanvas, atPOINT lpPtrList[], atINT nCount, atGRPX_COLOR Color );
atVOID            atGRPX_FillConvexPolygonAA_REAL( atGRPX_CANVAS *lpCanvas, atRPOINT lpPtrList[], atINT nCount, atGRPX_COLOR Color );
atVOID		 atGRPX_FillConvexPolygonExAA_REAL( atGRPX_CANVAS *lpCanvas, atRPOINT lpPtrList[], atINT nCount, atINT lpAttr[], atGRPX_COLOR Color );
atVOID			atGRPX_FillPolygon( atGRPX_CANVAS *lpCanvas, atPOINT lpPtrList[], atINT nCount, atGRPX_COLOR Color );
atVOID			atGRPX_FillPolygon_REAL( atGRPX_CANVAS *lpCanvas, atRPOINT lpPtrList[], atINT nCount, atGRPX_COLOR Color );
atVOID			atGRPX_FillPolygonAA( atGRPX_CANVAS *lpCanvas, atPOINT lpPtrList[], atINT nCount, atGRPX_COLOR Color );
atVOID			atGRPX_FillPolygonAA_REAL( atGRPX_CANVAS *lpCanvas, atRPOINT lpPtrList[], atINT nCount, atGRPX_COLOR Color );
atVOID            atGRPX_DrawPolyPolyLine( atGRPX_CANVAS *lpCanvas, atPOINT lpPtrList[], atINT nPartCount[], atINT nPolyCount, atBOOL bClosed, atGRPX_COLOR Color );
atVOID            atGRPX_DrawPolyPolyLineAA( atGRPX_CANVAS *lpCanvas, atPOINT lpPtrList[], atINT nPartCount[], atINT nPolyCount, atBOOL bClosed, atGRPX_COLOR Color );
atVOID			atGRPX_FillPolyPolygon( atGRPX_CANVAS *lpCanvas, atPOINT lpPtrList[], atINT nPartCount[], atINT nPolyCount, atGRPX_COLOR Color );
atVOID			atGRPX_FillPolyPolygonEx( atGRPX_CANVAS *lpCanvas, atPOINT lpPtrList[], atINT nPartCount[], atINT nPolyCount, atGRPX_COLOR Color, atLPGRPX_FUNC_COORD_CONV lpCnvFunc, atLPVOID lpParam );
atVOID			atGRPX_FillPolyPolygon_REAL( atGRPX_CANVAS *lpCanvas, atRPOINT lpPtrList[], atINT nPartCount[], atINT nPolyCount, atGRPX_COLOR Color );
atVOID			atGRPX_FillPolyPolygonAA( atGRPX_CANVAS *lpCanvas, atPOINT lpPtrList[], atINT nPartCount[], atINT nPolyCount, atGRPX_COLOR Color );
atVOID			atGRPX_FillPolyPolygonExAA( atGRPX_CANVAS *lpCanvas, atPOINT lpPtrList[], atINT nPartCount[], atINT nPolyCount, atGRPX_COLOR Color, atLPGRPX_FUNC_COORD_CONV lpCnvFunc, atLPVOID lpParam );
atVOID			atGRPX_FillPolyPolygonAA_REAL( atGRPX_CANVAS *lpCanvas, atRPOINT lpPtrList[], atINT nPartCount[], atINT nPolyCount, atGRPX_COLOR Color );
atVOID            atGRPX_DrawPolyPolyLineWidth( atGRPX_CANVAS *lpCanvas, atPOINT lpPtrList[], atINT nPartCount[], atINT nPolyCount, atBOOL bClosed, atGRPX_COLOR Color, atINT nWidth );
atVOID            atGRPX_DrawPolyPolyLineWidthAA( atGRPX_CANVAS *lpCanvas, atPOINT lpPtrList[], atINT nPartCount[], atINT nPolyCount, atBOOL bClosed, atGRPX_COLOR Color, atINT nWidth );


/* Image */

#define         atGRPX_BMATTR_NONE              			0x0000
#define         atGRPX_BMATTR_TRANSPARENT      	 	0x8000
#define         atGRPX_BMATTR_PALETTE           			0x4000
#define         atGRPX_BMATTR_OWN_ALPHACHANNEL 	0x2000

typedef struct tagatGRPX_BITMAP {
	atLPGRPXMNG     lpGrpX;

    atDWORD			dwAttr;
    atSHORT			nWidth, nHeight;
    atSHORT			nLogX, nLogY;
    atBYTE			nBPP, nPalBPP;
	atSHORT			nWidthBytes;
	
    atGRPX_COLOR			clTransColor;

    atLPVOID		lpBits;
	atLPVOID		lpPaletteTable;
	atLPVOID		lpAlphaChannel;

 	atLPCALLBACKALLOC fnAllocator;
	atLPCALLBACKFREE fnDeallocator;
} atGRPX_BITMAP, *atLPGRPX_BITMAP;

atGRPX_BITMAP * atGRPX_CreateBlankBitmap( atLPGRPXMNG lpGrpX, atINT Width, atINT Height, atINT nBPP, atINT nPalBPP );
atGRPX_BITMAP * atGRPX_CreateBitmapFromStream( atLPGRPXMNG lpGrpX, const atCHAR *lpStream, atINT nBPP, atINT nPalBPP );
atGRPX_BITMAP * atGRPX_CreateBlankBitmapEx( atLPGRPXMNG lpGrpX, atINT Width, atINT Height, atINT nBPP, atINT nPalBPP, atLPCALLBACKALLOC fnAllocator, atLPCALLBACKFREE fnDeallocator );
atGRPX_BITMAP * atGRPX_CreateBitmapFromStreamEx( atLPGRPXMNG lpGrpX, const atCHAR *lpStream, atINT nBPP, atINT nPalBPP, atLPCALLBACKALLOC fnAllocator, atLPCALLBACKFREE fnDeallocator );
atVOID            atGRPX_DestroyBitmap( atGRPX_BITMAP *lpBitmap );
atVOID            atGRPX_GetBitmapMetric( atGRPX_BITMAP *lpBitmap, atSIZE *lpSize );
atVOID            atGRPX_GetBitmapLogPos( atGRPX_BITMAP *lpBitmap, atINT *pLogX, atINT *pLogY );
atVOID            atGRPX_SetBitmapLogPos( atGRPX_BITMAP *lpBitmap, atINT LogX, atINT LogY );
atVOID            atGRPX_SetBitmapLogPosCenter( atGRPX_BITMAP *lpBitmap );
atVOID            atGRPX_SetBitmapTransColor( atGRPX_BITMAP *lpBitmap, atGRPX_COLOR clTransColor );
atVOID            atGRPX_ClearBitmapTransColor( atGRPX_BITMAP *lpBitmap );
atLPVOID        atGRPX_GetBitmapPtr( atGRPX_BITMAP *lpBitmap );
atLPVOID        atGRPX_GetBitmapPtrY( atGRPX_BITMAP *lpBitmap, atINT nY );
atLPVOID        atGRPX_GetBitmapPtrXY( atGRPX_BITMAP *lpBitmap, atINT nX, atINT nY );
atLPVOID        atGRPX_GetBitmapPtrLogXY( atGRPX_BITMAP *lpBitmap, atINT nX, atINT nY );
atVOID  		atGRPX_SetBitmapAlphaChannel( atGRPX_BITMAP *lpBitmap, atBYTE *lpAlphaChannel, atBOOL bOwnMask );

atVOID            atGRPX_DrawBitmapRaw( atGRPX_CANVAS *lpCanvas, atGRPX_BITMAP *lpBitmap, atINT X, atINT Y );
atVOID            atGRPX_DrawBitmapRawPart( atGRPX_CANVAS *lpCanvas, atGRPX_BITMAP *lpBitmap, atINT X, atINT Y, atINT ImageX, atINT ImageY, atINT ImageWidth, atINT ImageHeight );
atVOID            atGRPX_DrawBitmap( atGRPX_CANVAS *lpCanvas, atGRPX_BITMAP *lpBitmap, atINT X, atINT Y );
atVOID            atGRPX_DrawBitmapPart( atGRPX_CANVAS *lpCanvas, atGRPX_BITMAP *lpBitmap, atINT X, atINT Y, atINT ImageX, atINT ImageY, atINT ImageWidth, atINT ImageHeight );
atVOID            atGRPX_DrawBitmapRawStretch( atGRPX_CANVAS *lpCanvas, atGRPX_BITMAP *lpBitmap, atINT X, atINT Y, atINT tWidth, atINT tHeight );
atVOID            atGRPX_DrawBitmapRawScale( atGRPX_CANVAS *lpCanvas, atGRPX_BITMAP *lpBitmap, atINT X, atINT Y, atINT Scale );
atVOID            atGRPX_DrawBitmapStretch( atGRPX_CANVAS *lpCanvas, atGRPX_BITMAP *lpBitmap, atINT X, atINT Y, atINT tWidth, atINT tHeight );
atVOID            atGRPX_DrawBitmapScale( atGRPX_CANVAS *lpCanvas, atGRPX_BITMAP *lpBitmap, atINT X, atINT Y, atINT Scale );
atVOID            atGRPX_DrawBitmapStretchEx( atGRPX_CANVAS *lpCanvas, atGRPX_BITMAP *lpBitmap, atINT X, atINT Y, atINT tWidth, atINT tHeight );
atVOID            atGRPX_DrawBitmapScaleEx( atGRPX_CANVAS *lpCanvas, atGRPX_BITMAP *lpBitmap, atINT X, atINT Y, atINT Scale );
atVOID            atGRPX_DrawBitmapTrans( atGRPX_CANVAS *lpCanvas, atGRPX_BITMAP *lpBitmap, atINT X, atINT Y, atINT nAlpha );
atVOID            atGRPX_DrawBitmapTransPart( atGRPX_CANVAS *lpCanvas, atGRPX_BITMAP *lpBitmap, atINT X, atINT Y, atINT ImageX, atINT ImageY, atINT ImageWidth, atINT ImageHeight, atINT nAlpha );
atVOID            atGRPX_DrawBitmapStretchTrans( atGRPX_CANVAS *lpCanvas, atGRPX_BITMAP *lpBitmap, atINT X, atINT Y, atINT tWidth, atINT tHeight, atINT nAlpha );
atVOID            atGRPX_DrawBitmapScaleTrans( atGRPX_CANVAS *lpCanvas, atGRPX_BITMAP *lpBitmap, atINT X, atINT Y, atINT Scale, atINT nAlpha );
atVOID            atGRPX_DrawBitmapWithAlphaChannel( atGRPX_CANVAS *lpCanvas, atGRPX_BITMAP *lpBitmap, atINT X, atINT Y, atBYTE *pAlphaChannel );
atVOID            atGRPX_DrawBitmapPartWithAlphaChannel( atGRPX_CANVAS *lpCanvas, atGRPX_BITMAP *lpBitmap, atINT X, atINT Y, atINT ImageX, atINT ImageY, atINT ImageWidth, atINT ImageHeight, atBYTE *pAlphaChannel );
atVOID            atGRPX_DrawBitmapStretchWithAlphaChannel( atGRPX_CANVAS *lpCanvas, atGRPX_BITMAP *lpBitmap, atINT X, atINT Y, atINT tWidth, atINT tHeight, atBYTE *pAlphaChannel );
atVOID            atGRPX_DrawBitmapScaleWithAlphaChannel( atGRPX_CANVAS *lpCanvas, atGRPX_BITMAP *lpBitmap, atINT X, atINT Y, atINT Scale, atBYTE *pAlphaChannel );
atVOID            atGRPX_DrawBitmapRawRotate( atGRPX_CANVAS *lpCanvas, atGRPX_BITMAP *lpBitmap, atINT X, atINT Y, atREAL Angle );
atVOID            atGRPX_DrawBitmapRotateTrans( atGRPX_CANVAS *lpCanvas, atGRPX_BITMAP *lpBitmap, atINT X, atINT Y, atREAL Angle, atINT nAlpha );
atVOID            atGRPX_DrawBitmapRotateWithAlphaChannel( atGRPX_CANVAS *lpCanvas, atGRPX_BITMAP *lpBitmap, atINT X, atINT Y, atREAL Angle, atBYTE *pAlphaChannel );
atVOID            atGRPX_DrawBitmapRotate( atGRPX_CANVAS *lpCanvas, atGRPX_BITMAP *lpBitmap, atINT X, atINT Y, atREAL Angle );

atVOID            atGRPX_GetBitmap( atGRPX_CANVAS *lpCanvas, atGRPX_BITMAP *lpBitmap, atINT X, atINT Y );

atLPBYTE  	atGRPX_CreateMaskFromStream( atLPGRPXMNG lpGrpX, const atCHAR *lpStream );

atVOID            atGRPX_DrawGlyphRotate( atGRPX_CANVAS *lpCanvas, atGRPX_BITMAP *lpBitmap, atINT X, atINT Y, atGRPX_COLOR Color, atREAL Angle );
atVOID            atGRPX_DrawGlyphPart( atGRPX_CANVAS *lpCanvas, atGRPX_BITMAP *lpBitmap, atINT X, atINT Y, atGRPX_COLOR Color, atINT iStartPos, atINT nWidth );


/* Graphics + Bitmap */

atVOID            atGRPX_DrawTextureHorizLine( atGRPX_CANVAS *lpCanvas, atINT nX, atINT nY, atINT nWidth, atGRPX_BITMAP *lpTextureBB );
atVOID            atGRPX_DrawTextureRect( atGRPX_CANVAS *lpCanvas, atINT StartX, atINT StartY, atINT EndX, atINT EndY, atGRPX_BITMAP *lpTextureBB );
atVOID            atGRPX_FillPolygonBitmap( atGRPX_CANVAS *lpCanvas, atPOINT lpPtrList[], atINT nCount, atGRPX_BITMAP *lpBB );
atVOID            atGRPX_FillPolygonBitmapEx( atGRPX_CANVAS *lpCanvas, atPOINT lpPtrList[], atINT nCount, atGRPX_BITMAP *lpBB, atLPGRPX_FUNC_COORD_CONV lpCnvFunc, atLPVOID lpParam );
atVOID            atGRPX_FillPolyPolygonBitmap( atGRPX_CANVAS *lpCanvas, atPOINT lpPtrList[], atINT nPartCount[], atINT nPolyCount, atGRPX_BITMAP *lpBB );
atVOID		 atGRPX_FillPolyPolygonBitmapEx( atGRPX_CANVAS *lpCanvas, atPOINT lpPtrList[], atINT nPartCount[], atINT nPolyCount, atGRPX_BITMAP *lpBB, atLPGRPX_FUNC_COORD_CONV lpCnvFunc, atLPVOID lpParam );


/* String */

atVOID            atGRPX_DrawString( atGRPX_CANVAS *lpCanvas, atGRPX_FONT *lpFont, atINT X, atINT Y, atLPTSTR szStrData, atINT nLen, atGRPX_COLOR Color );
atVOID            atGRPX_DrawStringEx( atGRPX_CANVAS *lpCanvas, atGRPX_FONT *lpFont, atINT X, atINT Y, atLPTSTR szStrData, atINT nLen, atGRPX_COLOR Color, atINT nLineGap );
atVOID 		 atGRPX_DrawStringRotate( atGRPX_CANVAS *lpCanvas, atGRPX_FONT *lpFont, atINT nX, atINT nY, atLPTSTR szStrData, atINT nLen, atGRPX_COLOR Color, atREAL rAngle, atINT nBaseX, atINT nBaseY );
atGRPX_BITMAP  * atGRPX_CreateStrBitmap( atLPGRPXMNG lpGrpX, atGRPX_FONT *lpFont, atLPTSTR szStrData, atINT nLen );


/* Graphics with Pen & Brush */

typedef enum {
	atGRPX_PENTYPE_SOLID,
	atGRPX_PENTYPE_BITMAP,
} atGRPX_PENTYPE;

typedef struct tagatGRPX_PEN {
	atLPGRPXMNG     lpGrpX;

	atGRPX_PENTYPE		nPenType;
	atBOOL				bAntiAliasing;
	atINT				nPenWidth;
	atINT				nAlpha;
	union {
		atDWORD         dwDrawMethod;
		atGRPX_COLOR			clColor;
		atLPGRPX_BITMAP lpBitmap;
	};
} atGRPX_PEN, *atLPGRPX_PEN;

typedef enum {
	atGRPX_BRUSHTYPE_SOLID,
	atGRPX_BRUSHTYPE_BITMAP,
} atGRPX_BRUSHTYPE;

typedef struct tagatGRPX_BRUSH {
	atLPGRPXMNG     lpGrpX;

	atGRPX_BRUSHTYPE	nBrushType;
	atBOOL				bAntiAliasing;
	atINT				nAlpha;
	union {
		atDWORD         dwDrawMethod;
		atGRPX_COLOR			clColor;
		atLPGRPX_BITMAP lpBitmap;
	};
} atGRPX_BRUSH, *atLPGRPX_BRUSH;

atLPGRPX_PEN	atGRPX_CreatePen( atGRPXMNG *lpGrpX, atGRPX_PENTYPE nPenType, atBOOL bAntiAliasing, atINT nPenWidth, atINT nAlpha, atDWORD dwDrawMethod );
atVOID			atGRPX_DestroyPen( atLPGRPX_PEN lpPen );
atLPGRPX_BRUSH	atGRPX_CreateBrush( atGRPXMNG *lpGrpX, atGRPX_BRUSHTYPE nBrushType, atBOOL bAntiAliasing, atINT nAlpha, atDWORD dwDrawMethod );
atVOID			atGRPX_DestroyBrush( atLPGRPX_BRUSH lpBrush );

atVOID            atGRPX_DrawPixelWithPen( atGRPX_CANVAS *lpCanvas, atINT X, atINT Y, atGRPX_PEN *lpPen );
atVOID            atGRPX_DrawPixelWithBrush( atGRPX_CANVAS *lpCanvas, atINT X, atINT Y, atGRPX_BRUSH *lpBrush );
atVOID            atGRPX_DrawLineWithPen( atGRPX_CANVAS *lpCanvas, atINT StartX, atINT StartY, atINT EndX, atINT EndY, atGRPX_PEN *lpPen );
atVOID            atGRPX_DrawRectWithPen( atGRPX_CANVAS *lpCanvas, atINT StartX, atINT StartY, atINT EndX, atINT EndY, atGRPX_PEN *lpPen );
atVOID            atGRPX_FillRectWithBrush( atGRPX_CANVAS *lpCanvas, atINT StartX, atINT StartY, atINT EndX, atINT EndY, atGRPX_BRUSH *lpBrush );
atVOID            atGRPX_DrawCircleWithPen( atGRPX_CANVAS *lpCanvas, atINT X, atINT Y, atINT Radius, atGRPX_PEN *lpPen );
atVOID            atGRPX_FillCircleWithBrush( atGRPX_CANVAS *lpCanvas, atINT X, atINT Y, atINT Radius, atGRPX_BRUSH *lpBrush );
atVOID            atGRPX_DrawTriangleWithPen( atGRPX_CANVAS *lpCanvas, atINT X1, atINT Y1, atINT X2, atINT Y2, atINT X3, atINT Y3, atGRPX_PEN *lpPen );
atVOID            atGRPX_FillTriangleWithBrush( atGRPX_CANVAS *lpCanvas, atINT X1, atINT Y1, atINT X2, atINT Y2, atINT X3, atINT Y3, atGRPX_BRUSH *lpBrush );
atVOID            atGRPX_DrawQuadrangleWithPen( atGRPX_CANVAS *lpCanvas, atINT X1, atINT Y1, atINT X2, atINT Y2, atINT X3, atINT Y3, atINT X4, atINT Y4, atGRPX_PEN *lpPen );
atVOID            atGRPX_FillQuadrangleWithBrush( atGRPX_CANVAS *lpCanvas, atINT X1, atINT Y1, atINT X2, atINT Y2, atINT X3, atINT Y3, atINT X4, atINT Y4, atGRPX_BRUSH *lpBrush );
atVOID			atGRPX_DrawPolyLineWithPen( atGRPX_CANVAS *lpCanvas, atPOINT lpPtrList[], atINT nCount, atBOOL bClosed, atGRPX_PEN *lpPen );
atVOID			atGRPX_FillPolygonWithBrush( atGRPX_CANVAS *lpCanvas, atPOINT lpPtrList[], atINT nCount, atGRPX_BRUSH *lpBrush );
atVOID            atGRPX_DrawPolyPolyLineWithPen( atGRPX_CANVAS *lpCanvas, atPOINT lpPtrList[], atINT nPartCount[], atINT nPolyCount, atBOOL bClosed, atGRPX_PEN *lpPen );
atVOID			atGRPX_FillPolyPolygonWithBrush( atGRPX_CANVAS *lpCanvas, atPOINT lpPtrList[], atINT nPartCount[], atINT nPolyCount, atGRPX_BRUSH *lpBrush );


#ifdef __cplusplus
}
#endif


#endif
