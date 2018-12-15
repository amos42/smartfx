/**
  @file WinMng.h
  @brief WinMng에 관한 API
  @remark WinMng에 관한 API 모음
  @author Amos
  @date 2010-04-19 오전 11:41:00
  @see WObjMng.h WinMng.inl Window.h WinMng.c
*/
#ifndef __WINMNG_H_
#define __WINMNG_H_


#ifndef __AT_TYPES_H_
#include "AT_Types.h"
#endif
#include "Window.h"
#include "WObjMng.h"


#ifdef __cplusplus
extern "C" {
#endif


typedef atHANDLE  atHWINMNG;			/**< Window Manager의 핸들 타입 */


/**
 @brief WinClass 개체
*/
typedef struct _tagatWINCLASS {
	atDWORD		dwAttr;				/**< Window 스타일 속성 */
	atDWORD		dwUserDataSize;		/**< Window 사용자 요구 데이터 크기 */
	atLPWNDPROC	fnWndProc;			/**< Window 핸들러 */
	atMARGIN		rtMargin;			/**< Window 레이아웃 여백 */
	atLAYOUTBITMAP  tLayoutBm;			/**< Window 레이아웃 비트맵 세트 */
	atBYTE			ExData[256];			/**< 기타 확장 데이터 */
} atWINCLASS;

/**
 @brief WinClass의 Iterator (내부용)
*/
typedef struct _tagatWINCLASS_ITOR {
	atTCHAR			szClassName[64];	/**< WinClass 이름 */
	atWINCLASS		ClassData;			/**< WinClass 데이터 */
} atWINCLASS_ITOR;

/**
 @brief Window Manager 개체
*/
typedef struct _tagatWINMNG {
	atWINOBJMNG	    Base;						/**< WinMng, CtlMng 등의 기본 속성. 반드시 맨 앞에 있어야 한다. */
	//atHWINDOW		hFocusedWindow;
	//atHWINOBJ		hFocusedObj;
	atUILIST        lpWinClassList;				/**< WinClass의 리스트. WinClass Iterator 타입을 갖는다. */
	atLAYOUTBITMAP  DefLayoutBitmap[3];		/**< Default 레아이웃 비트맵 세트 */
	atMARGIN		DefLayoutMargin[3];		/**< Default 레이아웃 여백 */
} atWINMNG, *atLPWINMNG;


atHWINMNG	atWINMNG_CreateWinMng( atHWINDOW hParentWnd );
void		atWINMNG_DestroyWinMng( atHWINMNG hWinMng );

atBOOL  atWINMNG_SetDefaultLayout( atHWINMNG hWinMng, atINT DefIndex, atLAYOUTBITMAP *lpLayoutBitmap, atMARGIN *lpLayoutMargin );

atBOOL		atWINMNG_RegistWinClass( atHWINMNG hWinMng, atLPTSTR szClassName, atWINCLASS *pWinClass );
atBOOL		atWINMNG_UnregistWinClass( atHWINMNG hWinMng, atLPTSTR szClassName );
atWINCLASS *atWINMNG_GetWinClass( atHWINMNG hWinMng, atLPTSTR szClassName  );

atHWINDOW	atWINMNG_CreateWindowEx( atHWINMNG hWinMng, atHWINDOW hOwnerWnd, atINT nID, int nStyle, atLPTSTR szWindowText, 
											int nWidth, int nHeight, atMARGIN *lpLayoutMargin, atMARGIN *rtAnchor,
											atDWORD dwUserDataSize, atLPWNDPROC fnWndProc, 
											atLAYOUTBITMAP *lpLayoutBm, 
											void *lpCreateParam, void *lpExData );
atHWINDOW	atWINMNG_CreateWindow( atHWINMNG hWinMng, atHWINDOW hOwnerWnd, atINT nID, int nStyle, atLPTSTR szWindowText, 
											int nStartX, int nStartY, int nWidth, int nHeight,
											atDWORD dwUserDataSize, atLPWNDPROC fnWndProc,
											void *lpCreateParam, void *lpExData );
atHWINDOW	atWINMNG_CreateWindowByClass( atHWINMNG hWinMng, atHWINDOW hOwnerWnd, atINT nID, atLPTSTR szWindowText, 
											int nWidth, int nHeight, atMARGIN *rtAnchor,
											atLPTSTR szClassName, void *lpCreateParam );
void		atWINMNG_DestroyWindow( atHWINMNG hWinMng, atHWINDOW hWnd );
void 		atWINMNG_DestroyAllWindows( atHWINMNG hWinMng );
void 		atWINMNG_DestroyWindowsEx( atHWINMNG hWinMng, atBOOL (*fnIsDel)(atHWINDOW) );

void		atWINMNG_SetFocusToWindow( atHWINMNG hWinMng, atHWINDOW hWindow );
void		atWINMNG_SetFocusToControl( atHWINMNG hWinMng, atHCONTROL hControl );


#ifdef __cplusplus
}
#endif


#include "WinMng.inl"


#endif // _MYWINDOW_H_INCLUDED

