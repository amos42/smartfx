/**
  @file WinMng.h
  @brief WinMng�� ���� API
  @remark WinMng�� ���� API ����
  @author Amos
  @date 2010-04-19 ���� 11:41:00
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


typedef atHANDLE  atHWINMNG;			/**< Window Manager�� �ڵ� Ÿ�� */


/**
 @brief WinClass ��ü
*/
typedef struct _tagatWINCLASS {
	atDWORD		dwAttr;				/**< Window ��Ÿ�� �Ӽ� */
	atDWORD		dwUserDataSize;		/**< Window ����� �䱸 ������ ũ�� */
	atLPWNDPROC	fnWndProc;			/**< Window �ڵ鷯 */
	atMARGIN		rtMargin;			/**< Window ���̾ƿ� ���� */
	atLAYOUTBITMAP  tLayoutBm;			/**< Window ���̾ƿ� ��Ʈ�� ��Ʈ */
	atBYTE			ExData[256];			/**< ��Ÿ Ȯ�� ������ */
} atWINCLASS;

/**
 @brief WinClass�� Iterator (���ο�)
*/
typedef struct _tagatWINCLASS_ITOR {
	atTCHAR			szClassName[64];	/**< WinClass �̸� */
	atWINCLASS		ClassData;			/**< WinClass ������ */
} atWINCLASS_ITOR;

/**
 @brief Window Manager ��ü
*/
typedef struct _tagatWINMNG {
	atWINOBJMNG	    Base;						/**< WinMng, CtlMng ���� �⺻ �Ӽ�. �ݵ�� �� �տ� �־�� �Ѵ�. */
	//atHWINDOW		hFocusedWindow;
	//atHWINOBJ		hFocusedObj;
	atUILIST        lpWinClassList;				/**< WinClass�� ����Ʈ. WinClass Iterator Ÿ���� ���´�. */
	atLAYOUTBITMAP  DefLayoutBitmap[3];		/**< Default �����̿� ��Ʈ�� ��Ʈ */
	atMARGIN		DefLayoutMargin[3];		/**< Default ���̾ƿ� ���� */
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

