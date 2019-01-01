/**
  @file UI_Util.h
  @brief UI Utilite에 관한 API
  @remark UI Utilite에 관한 API 모음
  @author Amos
  @date 2010-04-19 오전 11:41:00
  @remark 유틸리티 함수 모음. 향후 용도에 따라 세분화시키는 것이 좋을 듯 함.
  @see UI_Util.c
*/
#ifndef __UI_UTIL_H_
#define __UI_UTIL_H_

#include "UI_Types.h"
#include "UI_API/UI_API.h"


#ifdef __cplusplus
extern "C" {
#endif


#define __USES_MYDEBUG_


#define     UI_COLOR_WHITE             (atUIAPI_GetRGB(255, 255, 255))
#define     UI_COLOR_BLACK             (atUIAPI_GetRGB(0, 0, 0))
#define     UI_COLOR_RED               (atUIAPI_GetRGB(255, 0, 0))
#define     UI_COLOR_GREEN             (atUIAPI_GetRGB(0, 255, 0))
#define     UI_COLOR_BLUE              (atUIAPI_GetRGB(0, 0, 255))
#define     UI_COLOR_DARKGRAY          (atUIAPI_GetRGB(128, 128, 128))
#define     UI_COLOR_GRAY              (atUIAPI_GetRGB(192, 192, 192))


#ifdef __USES_MYDEBUG_
#define UI_MEMALLOC(a)     	myAlloc((atLONG)(a), __FILE__, __LINE__)
#define UI_MEMFREE(a)      	myFree((void *)(a), __FILE__, __LINE__)
#define UI_MEMCHECK()      	myMemCheck(__FILE__, __LINE__)
#define UI_MEMCHECK2(a)      	myMemCheck2((void *)(a), __FILE__, __LINE__)
#else
#define UI_MEMALLOC(a)     	atUIAPI_AllocMem((atLONG)(a))
#define UI_MEMFREE(a)      	atUIAPI_FreeMem((void *)(a))
#define UI_MEMCHECK()      
#define UI_MEMCHECK2(a)      
#endif
#define UI_SAFEMEMFREE(a)		if(a){ UI_MEMFREE(a); (a)=atNULL;}

#define __NEW(t)    (t *)UI_MEMALLOC( sizeof(t) )
#define __DELETE(a) UI_SAFEMEMFREE( a )


#ifdef __USES_MYDEBUG_
#define UI_MSG_F 				myDebugOut
#else
#define UI_MSG_F
#endif
//#define UI_SPRINTF 	    	mySprintf

#define UI_STRLEN(a)      		atUIAPI_strlen(a)
#define UI_STRCPY(a,b)    		atUIAPI_strcpy(a,b)
#define UI_STRNCPY(a,b,c)		atUIAPI_strncpy(a,b,c)
#define UI_STRCAT(a,b)    		atUIAPI_strcat(a,b)
#define UI_STRCMP(a,b)    		atUIAPI_strcmp(a,b)
#define UI_SPRINTF 				atUIAPI_sprintf


atLPTSTR myStringCut( atHGDC hDC, atLPTSTR szIn, atLPTSTR szOut, int nScreenWidth, atBOOL bFlag );
int     myGetLineNum( atLPTSTR szMsg );
int     myGetStrLine( atLPTSTR szDestBuffer, atLPTSTR szSrcBuffer, int nLineNo, atTCHAR byDivideCode );
int     myAdjustStr( int swidth, int sheight, atLPTSTR str, int stidx, void *data, int (*strwidth)(void*,atLPTSTR,int), void (*drawfunc)(void*,int,int,int,atLPTSTR) );
atDWORD AutoLineDown( atLPTSTR szText, atHFONT hFont, atINT nMaxLen, atLONG lMaxWidth);
void	GetStringCut( atLPTSTR szOut, int out_size, atLPTSTR szIn, atHFONT hFt, int scr_cx );

#ifdef __USES_MYDEBUG_
void 	myAssert( atLPTSTR fmt, ... );
void *	myAlloc( long size, char *filename, int line );
atBOOL	myFree( void *ptr, char *filename, int line );
atBOOL 	myMemCheck( char *filename, int line );
atBOOL 	myMemCheck2( void *ptr, char *filename, int line );
int 	myDebugOut( const atLPTSTR fmt, ... );
#endif


atBOOL atUIAPI_IsPtInRect( atINT nX, atINT nY, atRECT *lpSrcRect );
atBOOL atUIAPI_IsPtInRect2( atPOINT *lpPt, atRECT *lpSrcRect );
atBOOL atUIAPI_IsEmbededRect( atRECT *lpRect, atRECT *lpParentRect );
atBOOL atUIAPI_IsIntersectRect( atRECT *lpSrcRect1, atRECT *lpSrcRect2 );
atBOOL atUIAPI_IntersectRect( atRECT *lpDesRect, atRECT *lpSrcRect1, atRECT *lpSrcRect2 );
atVOID atUIAPI_UnionRect( atRECT *lpDesRect, atRECT *lpSrcRect1, atRECT *lpSrcRect2 );
atVOID atUIAPI_ToEmbededRect( atRECT *lpDesRect, atRECT *lpParentRect );
atVOID atUIAPI_MoveRect(atRECT *lpDesRect, atRECT *lpSrcRect, atINT nX, atINT nY );
atVOID atUIAPI_OffsetRect(atRECT *lpDesRect, atINT nX, atINT nY );


typedef struct _tagatLAYOUTBITMAP {
	atHBITMAP bmTopLeft,     bmTopCenter,     bmTopRight;
	atHBITMAP bmMiddleLeft,  bmMiddleCenter,  bmMiddleRight;
	atHBITMAP bmBottomLeft,  bmBottomCenter,  bmBottomRight;
} atLAYOUTBITMAP;

atVOID        	atUIAPI_DrawClient1Line( atHGDC hCanvas, atINT nX, atINT nY, atINT nWidth,
                                     atHBITMAP lpTopLeftBM, atHBITMAP lpTopCenterBM, atHBITMAP lpTopRightBM );
atVOID        	atUIAPI_DrawClient1LineStretch( atHGDC hCanvas, atINT nX, atINT nY, atINT nWidth, atINT nHeight,
                                     atHBITMAP lpTopLeftBM, atHBITMAP lpTopCenterBM, atHBITMAP lpTopRightBM );
atVOID 		atUIAPI_DrawClient1LineVert( atHGDC hCanvas, atINT nX, atINT nY, atINT nHeight, 
									 atHBITMAP lpTopBM, atHBITMAP lpMiddleBM, atHBITMAP lpBottomBM );
atVOID 		atUIAPI_DrawClient1LineVertStretch( atHGDC hCanvas, atINT nX, atINT nY, atINT nWidth, atINT nHeight, 
					atHBITMAP lpTopBM, atHBITMAP lpMiddleBM, atHBITMAP lpBottomBM );
atVOID       	atUIAPI_DrawClient( atHGDC hCanvas, atINT nX, atINT nY, atINT nWidth, atINT nHeight,
                                atHBITMAP lpTopLeftBM, atHBITMAP lpTopCenterBM, atHBITMAP lpTopRightBM,
                                atHBITMAP lpMiddleLeftBM, atHBITMAP lpMiddleCenterBM, atHBITMAP lpMiddleRightBM,
                                atHBITMAP lpBottomLeftBM, atHBITMAP lpBottomCenterBM, atHBITMAP lpBottomRightBM,
								atBOOL IsClientDraw	);
atVOID       	atUIAPI_DrawClient2( atHGDC hCanvas, atINT nX, atINT nY, atINT nWidth, atINT nHeight, atLAYOUTBITMAP *lpLayoutBitmap, atBOOL IsClientDraw );


#ifdef __cplusplus
}
#endif


#endif
