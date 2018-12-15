/** 
  @file App.h
  @brief Application에 관한 API
  @remark App.c의 인터페이스 헤더
  @author Amos
  @date 2010-04-19 오전 11:41:00
  @see App.c
*/
#ifndef __APP_H_
#define __APP_H_


#include "UI_Types.h"
#include "Window.h"
#include "WinMng.h"
#include "Msg.h"


#ifdef __cplusplus
extern "C" {
#endif


typedef atHANDLE atHAPP;		///< Application의 핸들


atHAPP		atAPP_CreateApplication( atLONG lHeapSize );
void		atAPP_DestroyApplication( atHAPP hApp );

atBOOL  atAPP_SetDefaultLayout( atHAPP hApp, atINT DefIndex, atLAYOUTBITMAP *lpLayoutBitmap, atMARGIN *lpLayoutMargin );

atBOOL		atAPP_RegistWinClass( atHAPP hApp, atLPTSTR szClassName, atWINCLASS *pWinClass );
atBOOL		atAPP_UnregistWinClass( atHAPP hApp, atLPTSTR szClassName );
atWINCLASS *atAPP_GetWinClass( atHAPP hApp, atLPTSTR szClassName  );

atHWINDOW	atAPP_PreCreateWindowEx( atHAPP hApp, atHWINDOW hOwnerWnd, atINT nID, int nStyle, atLPTSTR szWindowText, 
								int nWidth, int nHeight, atMARGIN *rtLayoutMargin, atMARGIN *rtAnchor,
								atDWORD dwUserDataSize, atLPWNDPROC fnWndProc, 
								void *lpCreateParam, void *lpExData );
atHWINDOW	atAPP_CreateWindowEx( atHAPP hApp, atHWINDOW hOwerWnd, atINT nID, int nStyle, atLPTSTR szWindowText, 
								int nWidth, int nHeight, atMARGIN *rtLayoutMargin, atMARGIN *rtAnchor,
								atDWORD dwUserDataSize, atLPWNDPROC fnWndProc, 
								void *lpCreateParam, void *lpExData );
atHWINDOW	atAPP_CreateWindow( atHAPP hApp, atHWINDOW hOwerWnd, atINT nID, int nStyle, atLPTSTR szWindowText, 
								int nStartX, int nStartY, int nWidth, int nHeight,
								atDWORD dwUserDataSize, atLPWNDPROC fnWndProc, 
								void *lpCreateParam, void *lpExData );
atHWINDOW	atAPP_CreateWindowByClass( atHAPP hApp, atHWINDOW hOwerWnd, atINT nID, atLPTSTR szWindowText, 
										int nWidth, int nHeight, atMARGIN *rtAnchor,
										atLPTSTR szClassName, void *CreateParam );
void 		atAPP_DestroyWindow( atHAPP hApp, atHWINDOW hWnd );

void 		atAPP_DestroyAllWindows( atHAPP hApp );
void 		atAPP_DestroyWindowsEx( atHAPP hApp, atBOOL (*fnIsDel)(atHWINDOW) );

int 		atAPP_GetWindowCount( atHAPP hApp );
atHWINDOW 	atAPP_GetWindowByIndex( atHAPP hApp, atINT nIndex );

atBOOL		atAPP_RegistWindow( atHAPP hApp, atHWINDOW hWnd );
atBOOL 		atAPP_ReleaseWindow( atHAPP hApp, atHWINDOW hWnd );

atHWINDOW	atAPP_FindWindowByID( atHAPP hApp, atINT nWindowID );
atHWINDOW	atAPP_FindWindowByName( atHAPP hApp, atLPTSTR szWindowText );
atHWINDOW	atAPP_FindWindowAt( atHAPP hApp, atINT nX, atINT nY );

atHWINDOW	atAPP_GetCurWindow( atHAPP hApp );
atHWINDOW	atAPP_SetCurWindow( atHAPP hApp, atHWINDOW hWnd );

atVOID		atAPP_SetAnchor( atHAPP hApp, atHWINDOW hWnd, atINT nTop, atINT nBottom, atINT nLeft, atINT nRight );
atHWINDOW	atAPP_SetTopWindow( atHAPP hApp, atHWINDOW hWnd );

void		atAPP_SetDrawFlag( atHAPP hApp, atBOOL IsDraw );
void 		atAPP_ShowWindow( atHAPP hApp, atHWINDOW hWnd, atBOOL bShow );

int		atAPP_RedrawAllWindowEx( atHAPP hApp, atHWINDOW hWndBase );
int		atAPP_RedrawAllWindow( atHAPP hApp );
int		atAPP_RedrawAllWindowArea( atHAPP hApp, atRECT *rtWinRect );

atVOID 	atAPP_SendExitCode( atHAPP hApp, atINT nExitCode );
int 		atAPP_ProcessEvents( atHAPP hApp );
int 		atAPP_SendMessage( atHAPP hApp, atHWINDOW hwin, int msg, long param1, long param2 );
int 		atAPP_PostMessage(  atHAPP hApp, atHWINDOW hwin, int msg, long param1, long param2 );
atINT 		atAPP_AddMessageHooker( atHAPP hApp, atLPWNDPROC fnHookerCallback );
void 		atAPP_RemoveMessageHooker( atHAPP hApp, atINT nHookerID );

void		atAPP_AdjustWindowArea( atHAPP hApp, atHWINDOW hWnd );
void 	atAPP_AdjustAllWindowArea( atHAPP hApp );
atVOID	atAPP_SetWindowRegion( atHAPP hApp, atHWINDOW hWnd, atINT nX, atINT nY, atINT nWidth, atINT nHeight );
atVOID  atAPP_TickMsgProcess( atHAPP hApp );


#ifdef __cplusplus
}
#endif


#endif // __APP_H_

