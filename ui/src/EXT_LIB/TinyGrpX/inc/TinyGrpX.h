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
#ifndef __TINYGRPX_H_
#define __TINYGRPX_H_


#if defined(__GNUC__)
#pragma GCC diagnostic ignored "-Wunknown-pragmas"
#endif
#if defined(__GNUC__) || defined(__arm)
#pragma anon_unions
#endif


/* 설정 파일 */
#ifdef __TINYGRPX_USER_CFG_
#ifdef __USER_GRPX_CFG_H_
#include __USER_GRPX_CFG_H_
#else
#include "TinyGrpX_CFG.h"
#endif
#else  /* Default Config */
#define atTGRPX_PIXEL_BPP           (16)
//#define __FIXEDPTR_
//#define FIXED_RADIX_SIZE    (16)
#endif

/*
#define __FIXEDPTR_
#define atTGRPX_PIXEL_BPP         (16)
#define atTGRPX_ALPHA_BPP         (8)
*/

#ifndef __AT_TYPES_H_
#include "AT_Types.h"       /* 기본 제공하는 타입 선언 */
#endif
#ifndef __AT_REAL_H_
//#include "AT_Real.h"		/* 실수 사용을 위해 필요! */
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


#if atTGRPX_PIXEL_BPP == 16
#define MAKERGB(r,g,b) MAKE565((r),(g),(b))
#define GET_RED(cl)    GET565R((cl))
#define GET_GREEN(cl)  GET565G((cl))
#define GET_BLUE(cl)   GET565B((cl))
#elif atTGRPX_PIXEL_BPP == 24
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
#define atTGRPX_COLOR_BLACK		MAKERGB(0,0,0)
#define atTGRPX_COLOR_WHITE		MAKERGB(0xFF,0xFF,0xFF)
#define atTGRPX_COLOR_RED		MAKERGB(0xFF,0,0)
#define atTGRPX_COLOR_GREEN		MAKERGB(0,0xFF,0)
#define atTGRPX_COLOR_BLUE		MAKERGB(0,0,0xFF)


#if atTGRPX_PIXEL_BPP == 8
#define atTGRPX_DEF_TRANS_COLOR ((atTGRPX_COLOR)0x00)
#elif atTGRPX_PIXEL_BPP == 16
#define atTGRPX_DEF_TRANS_COLOR ((atTGRPX_COLOR)0xf81f)
#elif atTGRPX_PIXEL_BPP == 24
#define atTGRPX_DEF_TRANS_COLOR ((atTGRPX_COLOR)0xf81fe4)
#endif

#if atTGRPX_PIXEL_BPP == 8
typedef atBYTE           atTGRPX_PIXEL;
#elif atTGRPX_PIXEL_BPP == 16
typedef atWORD           atTGRPX_PIXEL;
#elif atTGRPX_PIXEL_BPP == 24
typedef atBYTE           atTGRPX_PIXEL[3];
#elif atTGRPX_PIXEL_BPP == 32
typedef atBYTE           atTGRPX_PIXEL[4];
#endif

typedef atDWORD          atTGRPX_COLOR;

typedef struct _tagatRGBCOLOR {
	atBYTE		nRed;
	atBYTE		nGreen;
	atBYTE		nBlue;
} atRGBCOLOR;

/* atTGRPX_COLOR 타입과 atTGRPX_PIXEL 타입을 전환을 위한 매크로. 현재 그냥 1:1로 변환하고 있음. */
#define COLOR_TO_PIXEL(a) ((atTGRPX_PIXEL)(a))
#define PIXEL_TO_COLOR(a) ((atTGRPX_COLOR)(a))

typedef atBOOL (atTGRPX_FUNC_COORD_CONV)( atLPVOID lpParam, atLONG *lpDesX, atLONG *lpDesY, atLONG nSrcX, atLONG nSrcY );
typedef atTGRPX_FUNC_COORD_CONV* atLPGRPX_FUNC_COORD_CONV;


/* Kernel */

typedef struct _tagatGRPXSCRINFO {
    atLPVOID	 lpFrameBufferPtr;
    atINT        nFrameWidth, nFrameHeight;
    atINT		 nBPP;    
	atINT		 nVMemWidth;
} atGRPXSCRINFO;

typedef atHANDLE atHGRPX_CANVAS;

typedef struct _tagatGRPXMNG {
	atGRPXSCRINFO		ScrInfo;

	atLPVOID			lpTempBuf;
} atGRPXMNG, *atLPGRPXMNG;


atINT			atTGRPX_GetInstanceSize(NOARGS);
atBOOL  		atTGRPX_Initialize( atLPGRPXMNG lpGrpX, atINT nWidth, atINT nHeight, atINT nBPP, atVOID *pFrameBufPtr, atINT VMemWidth );
atBOOL  		atTGRPX_SetScreenResolution( atLPGRPXMNG lpGrpX, atINT nWidth, atINT nHeight, atINT VMemWidth );
atBOOL  		atTGRPX_SetFrameBuffer( atLPGRPXMNG lpGrpX, atLPVOID pFrameBufPtr );
atVOID          atTGRPX_GetScreenInfo( atLPGRPXMNG lpGrpX, atGRPXSCRINFO *lpScrInfo );
atTGRPX_COLOR	atTGRPX_GetRGB( atBYTE r, atBYTE g, atBYTE b );
atBYTE			atTGRPX_GetRValue( atTGRPX_COLOR clColor );
atBYTE			atTGRPX_GetGValue( atTGRPX_COLOR clColor );
atBYTE			atTGRPX_GetBValue( atTGRPX_COLOR clColor );


/* Canvas */

typedef struct _tagatTGRPX_CANVAS {
	atLPGRPXMNG     lpGrpX;
	atBOOL               bActivate;

//    atREGION       rgnOrgArea;
    atREGION       rgnArea;
    atRECT         rtOrgClipRect, rtScreenClipRect;
    atLPVOID        lpVideoPtr;
	atINT           nBPP;
	atINT           nVMemWidth;
} atTGRPX_CANVAS, *atLPGRPX_CANVAS;

atINT			atTGRPX_GetCanvasInstanceSize(NOARGS);
atBOOL 			atTGRPX_InitCanvas( atTGRPX_CANVAS *lpCanvas, atLPGRPXMNG lpGrpX, atINT X, atINT Y, atINT Width, atINT Height );
atVOID    		atTGRPX_ActivateCanvas( atTGRPX_CANVAS *lpCanvas );
atVOID    		atTGRPX_DeactivateCanvas( atTGRPX_CANVAS *lpCanvas );
atBOOL			atTGRPX_IsActiveCanvas( atTGRPX_CANVAS *lpCanvas );
atVOID 			atTGRPX_RecalcCanvas( atTGRPX_CANVAS *lpCanvas );
atREGION*		atTGRPX_GetCanvasArea( atTGRPX_CANVAS *lpCanvas );
atINT           atTGRPX_GetBPP( atTGRPX_CANVAS *lpCanvas );
atTGRPX_PIXEL*	atTGRPX_GetFrameBufferPtr( atTGRPX_CANVAS *lpCanvas );
atTGRPX_PIXEL*	atTGRPX_GetFrameBufferPtrXY( atTGRPX_CANVAS *lpCanvas, atINT X, atINT Y );
atINT           atTGRPX_GetVMemWidth( atTGRPX_CANVAS *lpCanvas );
atRECT*			atTGRPX_GetClipRect( atTGRPX_CANVAS *lpCanvas );
atRECT*			atTGRPX_GetOriginalClipRect( atTGRPX_CANVAS *lpCanvas );
atBOOL          atTGRPX_SetClipRect( atTGRPX_CANVAS *lpCanvas, atINT nStartX, atINT nStartY, atINT nEndX, atINT nEndY );
atBOOL 			atTGRPX_SetClipRect2( atTGRPX_CANVAS *lpCanvas, atRECT *lpClip );
atBOOL			atTGRPX_MoveCanvas( atTGRPX_CANVAS *lpCanvas, atINT X, atINT Y );
atBOOL			atTGRPX_ChangeCanvasSize( atTGRPX_CANVAS *lpCanvas, atINT nWidth, atINT nHeight, atINT nFrameWidth );
atVOID	        atTGRPX_CopyCanvas( atTGRPX_CANVAS *lpCanvas_Des, atINT nDesX, atINT nDesY, atTGRPX_CANVAS *lpCanvas_Src, atINT nX, atINT nY, atINT nWidth, atINT nHeight );


/* GDI Method */

#define  atTGRPX_LINEEXT_NONE 				(0)
#define  atTGRPX_LINEEXT_START_EXTEND		(1)
#define  atTGRPX_LINEEXT_END_EXTEND		(2)
#define  atTGRPX_LINEEXT_START_ROUNDCAP	(4)
#define  atTGRPX_LINEEXT_END_ROUNDCAP		(8)
#define  atTGRPX_LINEEXT_EXTEND				(atTGRPX_LINEEXT_START_EXTEND | atTGRPX_LINEEXT_END_EXTEND)
#define  atTGRPX_LINEEXT_ROUNDCAP			(atTGRPX_LINEEXT_START_ROUNDCAP | atTGRPX_LINEEXT_END_ROUNDCAP)

atVOID				atTGRPX_ClearScreen( atTGRPX_CANVAS *lpCanvas, atTGRPX_COLOR Color );
atVOID				atTGRPX_ClearScreenAll( atTGRPX_CANVAS *lpCanvas, atTGRPX_COLOR Color );
atTGRPX_COLOR       atTGRPX_GetPixel( atTGRPX_CANVAS *lpCanvas, atINT X, atINT Y );
atTGRPX_PIXEL *     atTGRPX_DrawPixel( atTGRPX_CANVAS *lpCanvas, atINT X, atINT Y, atTGRPX_COLOR Color );
atVOID				atTGRPX_DrawHorizLine( atTGRPX_CANVAS *lpCanvas, atINT StartX, atINT StartY, atINT Length, atTGRPX_COLOR Color );
atVOID				atTGRPX_DrawVertLine( atTGRPX_CANVAS *lpCanvas, atINT StartX, atINT StartY, atINT Length, atTGRPX_COLOR Color );
atVOID				atTGRPX_DrawRect( atTGRPX_CANVAS *lpCanvas, atINT StartX, atINT StartY, atINT EndX, atINT EndY, atTGRPX_COLOR Color );
atVOID				atTGRPX_FillRect( atTGRPX_CANVAS *lpCanvas, atINT StartX, atINT StartY, atINT EndX, atINT EndY, atTGRPX_COLOR Color );


#ifdef __cplusplus
}
#endif


#endif
