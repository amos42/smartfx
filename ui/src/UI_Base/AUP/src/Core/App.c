/** 
  @file App.c
  @brief Application에 관한 API
  @remark Application 관련 API들 모임.
  @author Amos
  @date 2010-04-19 오전 11:41:00
  @see App.h
*/
#include "UI_Types.h"
#include "UI_MNG.h"
#include "App.h"
#include "WinMng.h"
#include "Window.h"
#include "UI_API/UI_API.h"
#include "UI_Util.h"
#include <string.h>


/** @brief Application 개체 (내부용) */
typedef struct _tagatAPP {
	atLPVOID	   lpAppHeap;
	atHWINMNG      hWinMng;	
	atHMSGPROC     hMsgProc;
} atAPP, *atLPAPP;



/** 
 @brief Application 개체의 포인터를 얻는다. (내부용)
 
 Application 개체의 포인터를 얻는다. (내부용)

 @param hApp [in] : Application 핸들
 @return Application 개체의 포인터
 @remark 내부용
*/
atLPAPP atAPP_GetPtr(atHAPP hApp)
{
	return (atLPAPP)hApp;
}


/** 
 @brief Application 개체의 핸들을 얻는다. (내부용)
 
 Application 개체의 핸들을 얻는다. (내부용)

 @param lpApp [in] : Application 개체의 포인터
 @return Application의 핸들
 @remark 내부용
*/
atHAPP atAPP_GetHandle(atLPAPP lpApp)
{
	return (atHAPP)lpApp;
}


/** 
 @brief Application을 생성한다.
 
 Application을 생성한다.

 @param lHeapSize [in] : 애플리케이션이 사용하는 Heap 사이즈
 @return 생성된 Application의 핸들
 @remark 현재 Heap 크기는 사용되지 않는다.
*/
atHAPP	atAPP_CreateApplication( atLONG lHeapSize )
{
	atLPAPP lpApp;

	lpApp = (atLPAPP)UI_MEMALLOC(sizeof(atAPP));
	if ( lpApp == atNULL ) return atNULL;

	lpApp->hWinMng = atWINMNG_CreateWinMng( atNULL );
	lpApp->hMsgProc = atMSG_CreateMsgProc();
	
	return atAPP_GetHandle(lpApp);
}


/** 
 @brief Application을 파괴한다.
 
 Application을 파괴한다.

 @param hApp [in] : Application 핸들
 @return 없음
*/
void atAPP_DestroyApplication( atHAPP hApp )
{
	atLPAPP lpApp = atAPP_GetPtr(hApp);
	if( lpApp == atNULL ) return;
	
	if( lpApp->hWinMng ){
		atWINMNG_DestroyWinMng( lpApp->hWinMng );
		lpApp->hWinMng = atNULL;
	}
	
	if( lpApp->hMsgProc ){
		atMSG_DestroyMsgProc( lpApp->hMsgProc );
		lpApp->hMsgProc = atNULL;
	}
	
	UI_MEMFREE( lpApp );
}


/** 
 @brief Window의 Default 레이아웃을 결정
 
 Window의 Default 레이아웃을 결정한다.

 @param hApp [in] : Application 핸들
 @param DefIndex [in] : 레이아웃 인덱스. 0~2까지 사용 가능. 0-FrameWindow, 1-PopupWindow, 2-UserDefWindow
 @param lpLayoutBitmap [in] : 레이아웃을 위한 8방향 비트맵
 @param lpLayoutMargin [in] : 레이아웃의 여백 크기
 @return 성공시 atTRUE, 실패시 atFALSE
*/
atBOOL  atAPP_SetDefaultLayout( atHAPP hApp, atINT DefIndex, atLAYOUTBITMAP *lpLayoutBitmap, atMARGIN *lpLayoutMargin )
{
	atLPAPP lpApp = atAPP_GetPtr(hApp);
	if( lpApp == atNULL ) return atFALSE;
	
	return atWINMNG_SetDefaultLayout( lpApp->hWinMng, DefIndex, lpLayoutBitmap, lpLayoutMargin );
}


/** 
 @brief WinClass를 등록한다.
 
 WinClass를 등록한다.

 @param hApp [in] : Application 핸들
 @param szClassName [in] : 클래스 이름
 @param pWinClass [in] : 등록할 WinClass 구조의 포인터
 @return 성공시 atTRUE, 실패시 atFALSE
*/
atBOOL		atAPP_RegistWinClass( atHAPP hApp, atLPTSTR szClassName, atWINCLASS *pWinClass )
{
	atLPAPP lpApp = atAPP_GetPtr(hApp);
	if( lpApp == atNULL ) return atFALSE;
	
	return atWINMNG_RegistWinClass( lpApp->hWinMng, szClassName, pWinClass );
}


/** 
 @brief WinClass 등록을 해지한다.
 
 WinClass 등록을 해지한다.

 @param hApp [in] : Application 핸들
 @param szClassName [in] : 클래스 이름
 @return 성공시 atTRUE, 실패시 atFALSE
*/
atBOOL		atAPP_UnregistWinClass( atHAPP hApp, atLPTSTR szClassName )
{
	atLPAPP lpApp = atAPP_GetPtr(hApp);
	if( lpApp == atNULL ) return atFALSE;
	
	return atWINMNG_UnregistWinClass( lpApp->hWinMng, szClassName );
}


/** 
 @brief WinClass를 얻는다.
 
 클래스 이름을 이용하여 등록된 WinClass를 얻는다.

 @param hApp [in] : Application 핸들
 @param szClassName [in] : 얻어낼 클래스 이름
 @return WinClass의 포인터. 실패시 atNULL
*/
atWINCLASS *atAPP_GetWinClass( atHAPP hApp, atLPTSTR szClassName  )
{
	atLPAPP lpApp = atAPP_GetPtr(hApp);
	if( lpApp == atNULL ) return atNULL;
	
	return atWINMNG_GetWinClass( lpApp->hWinMng, szClassName );
}


/** 
 @brief Window를 예비 생성한다.
 
 Window를 예비 생성한다.

 @param hApp [in] : Application 핸들
 @param hOwnerWnd [in] : 부모 Window의 핸들
 @param nID [in] : Window의 ID
 @param nStyle [in] : Window Style
 @param szWindowText [in] : Window 타이틀
 @param nWidth, nHeight [in] : 생성될 윈도우의 크기 
 @param rtLayoutMargin [in] : 외각선의 여백. 이 여백을 제외하여 클라이언트의 크기가 결정된다.
 @param rtAnchor [in] : Anchor를 지정한다.
 @param dwUserDataSize [in] : 사용자 추가 데이터 크기
 @param fnWndProc [in] : 사용자 이벤트 핸들러.
 @param lpCreateParam [in] : 생성시 전달할 파라미터
 @param lpExData [in] : 생성시 참고할 추가 데이터. 주로 WinClass에서 전달.
 @return Window 핸들
*/
atHWINDOW	atAPP_PreCreateWindowEx( atHAPP hApp, atHWINDOW hOwnerWnd, atINT nID, int nStyle, atLPTSTR szWindowText, 
								int nWidth, int nHeight, atMARGIN *rtLayoutMargin, atMARGIN *rtAnchor,
								atDWORD dwUserDataSize, atLPWNDPROC fnWndProc, 
								void *lpCreateParam, void *lpExData )
{
	atHWINDOW hWnd;
	atLPAPP lpApp = atAPP_GetPtr(hApp);
	if( lpApp == atNULL ) return atNULL;
	
	hWnd = atWINMNG_CreateWindowEx( lpApp->hWinMng, hOwnerWnd, nID, nStyle, szWindowText, 
									nWidth, nHeight, rtLayoutMargin, rtAnchor,
									dwUserDataSize, fnWndProc, 
									atNULL, 
									lpCreateParam, lpExData );

	return hWnd;
}


/** 
 @brief Window를 생성한다. (확장)
 
 Window를 생성한다.

 @param hApp [in] : Application 핸들
 @param hOwnerWnd [in] : 부모 Window의 핸들
 @param nID [in] : Window의 ID
 @param nStyle [in] : Window Style
 @param szWindowText [in] : Window 타이틀
 @param nWidth, nHeight [in] : 생성될 윈도우의 크기 
 @param rtLayoutMargin [in] : 외각선의 여백. 이 여백을 제외하여 클라이언트의 크기가 결정된다.
 @param rtAnchor [in] : Anchor를 지정한다.
 @param dwUserDataSize [in] : 사용자 추가 데이터 크기
 @param fnWndProc [in] : 사용자 이벤트 핸들러.
 @param lpCreateParam [in] : 생성시 전달할 파라미터
 @param lpExData [in] : 생성시 참고할 추가 데이터. 주로 WinClass에서 전달.
 @return Window 핸들
*/
atHWINDOW	atAPP_CreateWindowEx( atHAPP hApp, atHWINDOW hOwnerWnd, atINT nID, int nStyle, atLPTSTR szWindowText, 
								int nWidth, int nHeight, atMARGIN *rtLayoutMargin, atMARGIN *rtAnchor,
								atDWORD dwUserDataSize, atLPWNDPROC fnWndProc, 
								void *lpCreateParam, void *lpExData )
{
	atHWINDOW hWnd;
	
	atKERNEL_LockInput();

	hWnd = atAPP_PreCreateWindowEx( hApp, hOwnerWnd, nID, nStyle, szWindowText, 
									nWidth, nHeight, rtLayoutMargin, rtAnchor,
									dwUserDataSize, fnWndProc, 
									lpCreateParam, lpExData );
	atAPP_RegistWindow( hApp, hWnd );

	atKERNEL_UnlockInput();

	return hWnd;
}


/** 
 @brief Window를 생성한다.
 
 Window를 생성한다.

 @param hApp [in] : Application 핸들
 @param hOwnerWnd [in] : 부모 Window의 핸들
 @param nID [in] : Window의 ID
 @param nStyle [in] : Window Style
 @param szWindowText [in] : Window 타이틀
 @param nStartX, nStartY [in] : 생성될 윈도우의 위치
 @param nWidth, nHeight [in] : 생성될 윈도우의 크기 
 @param dwUserDataSize [in] : 사용자 추가 데이터 크기
 @param fnWndProc [in] : 사용자 이벤트 핸들러.
 @param lpCreateParam [in] : 생성시 전달할 파라미터
 @param lpExData [in] : 생성시 참고할 추가 데이터. 주로 WinClass에서 전달.
 @return Window 핸들
*/
atHWINDOW	atAPP_CreateWindow( atHAPP hApp, atHWINDOW hOwnerWnd, atINT nID, int nStyle, atLPTSTR szWindowText, 
								int nStartX, int nStartY, int nWidth, int nHeight,
								atDWORD dwUserDataSize, atLPWNDPROC fnWndProc, 
								void *lpCreateParam, void *lpExData )
{
	atHWINDOW hWnd;
	atMARGIN rtAnchor;
	atLPAPP lpApp = atAPP_GetPtr(hApp);
	
	if( lpApp == atNULL ) return atNULL;
	
	SET_MARGIN( rtAnchor, nStartY, atANCHOR_IGNORE, nStartX, atANCHOR_IGNORE );	
	hWnd = atAPP_CreateWindowEx( hApp, hOwnerWnd, nID, nStyle, szWindowText, 
								nWidth, nHeight, atNULL, &rtAnchor,
								dwUserDataSize, fnWndProc, lpCreateParam, lpExData );

	return hWnd;
}


/** 
 @brief WinClass로부터 Window를 생성한다.
 
 WinClass로부터 Window를 생성한다.

 @param hApp [in] : Application 핸들
 @param hOwnerWnd [in] : 부모 Window의 핸들
 @param nID [in] : Window의 ID
 @param nStyle [in] : Window Style
 @param szWindowText [in] : Window 타이틀
 @param nWidth, nHeight [in] : 생성될 윈도우의 크기 
 @param rtAnchor [in] : Anchor를 지정한다.
 @param szClassName [in] : WinClass 이름
 @param lpCreateParam [in] : 생성시 전달할 파라미터
 @return Window 핸들
*/
atHWINDOW	atAPP_CreateWindowByClass( atHAPP hApp, atHWINDOW hOwerWnd, atINT nID, atLPTSTR szWindowText, 
										int nWidth, int nHeight, atMARGIN *rtAnchor,
										atLPTSTR szClassName, void *lpCreateParam )
{
	atHWINDOW hWnd;
	atLPWINDOW lpWnd;
	atLPAPP lpApp = atAPP_GetPtr(hApp);
	if( lpApp == atNULL ) return atNULL;
	
	hWnd = atWINMNG_CreateWindowByClass( lpApp->hWinMng, hOwerWnd, nID, szWindowText, 
											nWidth, nHeight, rtAnchor,
											szClassName, lpCreateParam );
//	atAPP_RegistWindow( hApp, hWnd );
	lpWnd = atWINDOW_GetPtr(hWnd);
	if( lpWnd == atNULL ) return atNULL;
	lpWnd->hApp = hApp;	

	return hWnd;
}

	
/** 
 @brief Window를 파괴
 
 Window를 파괴한다.

 @param hApp [in] : Application 핸들
 @param hWnd [in] : 파괴할 Window 핸들
 @return 없음
*/
void atAPP_DestroyWindow( atHAPP hApp, atHWINDOW hWnd )
{
	atLPAPP lpApp = atAPP_GetPtr(hApp);
	if( lpApp == atNULL ) return;
	
	atWINMNG_DestroyWindow( lpApp->hWinMng, hWnd );
}


/** 
 @brief 모든 Window를 파괴
 
 현재 Application에 등록된 모든 Window를 파괴한다.

 @param hApp [in] : Application 핸들
 @return 없음
*/
void atAPP_DestroyAllWindows( atHAPP hApp )
{
	atLPAPP lpApp = atAPP_GetPtr(hApp);
	if( lpApp == atNULL ) return;
	
	atWINMNG_DestroyAllWindows( lpApp->hWinMng );
}


/** 
 @brief 모든 Window를 파괴 (조건 판단 핸들러)
 
 현재 Application에 등록된 모든 Window중, 조건에 맞는 윈도우만 골라서 파괴한다.\n
 이때, 파괴할 조건 판단 핸들러를 파라미터로 전달한다.

 @param hApp [in] : Application 핸들
 @param fnIsDel [in] : 조건 판단 핸들러.
 @return 없음
*/
void atAPP_DestroyWindowsEx( atHAPP hApp, atBOOL (*fnIsDel)(atHWINDOW) )
{
	atLPAPP lpApp = atAPP_GetPtr(hApp);
	if( lpApp == atNULL ) return;
	
	atWINMNG_DestroyWindowsEx( lpApp->hWinMng, fnIsDel );
}


/** 
 @brief Window의 갯수를 얻는다.
 
 현재 Application에 등록된 Window의 갯수를 얻는다.

 @param hApp [in] : Application 핸들
 @return Window의 갯수
*/
atINT atAPP_GetWindowCount( atHAPP hApp )
{
	atLPAPP lpApp = atAPP_GetPtr(hApp);
	if( lpApp == atNULL ) return 0;
	
	return atWINMNG_GetWindowCount( lpApp->hWinMng );
}


/** 
 @brief 특정 인덱스의 Window를 얻는다.
 
 특정 인덱스의 Window를 얻는다.

 @param hApp [in] : Application 핸들
 @param nIndex [in] : Window Index
 @return Window 핸들
 @remark Window의 Index는 수시로 변하기 때문에 Unique하지 않다. 내부용으로 주로 사용된다.
*/
atHWINDOW atAPP_GetWindowByIndex( atHAPP hApp, int nIndex )
{
	atLPAPP lpApp = atAPP_GetPtr(hApp);
	if( lpApp == atNULL ) return atNULL;
	
	return atWINMNG_GetWindowByIndex( lpApp->hWinMng, nIndex );
}


/** 
 @brief Window를 등록
 
 Application에 Window를 등록한다.

 @param hApp [in] : Application 핸들
 @param hWnd [in] : Window 핸들
 @return 성공시 atTRUE, 실패시 atFALSE
*/
atBOOL atAPP_RegistWindow( atHAPP hApp, atHWINDOW hWnd )
{
	atLPWINDOW lpWnd;
	atBOOL ret;
	atLPAPP lpApp = atAPP_GetPtr(hApp);
	if( lpApp == atNULL ) return atFALSE;
	
	lpWnd = atWINDOW_GetPtr(hWnd);
	if( lpWnd == atNULL ) return atFALSE;
	
	lpWnd->hApp = hApp;
	
	ret = atWINMNG_RegistWindow( lpApp->hWinMng, hWnd );
	if( ret == atFALSE ) return atFALSE;

//	atWINDOW_Refresh( hWnd );
		
	return ret;
}
 

/** 
 @brief 등록된 Window 해제
 
 Application에 등록된 Window를 해제한다.\n
 등록되었다가 해제된 윈도우는 자동으로 파괴된다. 

 @param hApp [in] : Application 핸들
 @param hWnd [in] : Window 핸들
 @return 성공시 atTRUE, 실패시 atFALSE
*/
atBOOL atAPP_ReleaseWindow( atHAPP hApp, atHWINDOW hWnd )
{
	atBOOL ret;
	atREGION rgn;
	atLPAPP lpApp = atAPP_GetPtr(hApp);
	if( lpApp == atNULL ) return atFALSE;

	atWINDOW_GetScreenRegion( hWnd, &rgn );
	ret = atWINMNG_ReleaseWindow( lpApp->hWinMng, hWnd );
	if( ret == atFALSE ) return atFALSE;
	
	atUIAPI_AddInvalideArea( &rgn );
		
	return ret;
}   


/** 
 @brief 현재 포커스를 가진 Window를 얻는다.
 
 현재 포커스를 가진 Window를 얻는다.

 @param hApp [in] : Application 핸들
 @return 포커스를 가진 Window의 핸들. 없을 시, atNULL
*/
atHWINDOW atAPP_GetCurWindow( atHAPP hApp )
{
	atLPAPP lpApp = atAPP_GetPtr(hApp);
	if( lpApp == atNULL ) return atNULL;
	
	return atWINMNG_GetCurWindow( lpApp->hWinMng );
}

/** 
 @brief Window에 포커스를 이동시킨다.
 
 Window에 포커스를 이동시킨다.

 @param hApp [in] : Application 핸들
 @param hWnd [in] : Window 핸들
 @return 이전에 포커스를 가진 Window의 핸들. 없을 시, atNULL
*/
atHWINDOW	atAPP_SetCurWindow( atHAPP hApp, atHWINDOW hWnd )
{
	atLPAPP lpApp = atAPP_GetPtr(hApp);
	if( lpApp == atNULL ) return atNULL;
	
	return atWINMNG_SetCurWindow( lpApp->hWinMng, hWnd );
}

void atAPP_SetFocusToWindow( atHAPP hApp, atHWINDOW hWindow )
{
	atLPAPP lpApp = atAPP_GetPtr(hApp);
	if( lpApp == atNULL ) return;
	
	atWINMNG_SetFocusToWindow( lpApp->hWinMng, hWindow );
}

//atHWINOBJ atAPP_GetFocusedObj( atHAPP hApp )
//{
//	atLPAPP lpApp = atAPP_GetPtr(hApp);
//	if( lpApp == atNULL ) return atNULL;
//	
//	return atWINMNG_GetFocusedObj( lpApp->hWinMng );
//}

/** 
 @brief Window에 Anchor를 설정한다.
 
 Window에 Anchor를 설정한다.

 @param hApp [in] : Application 핸들
 @param hWnd [in] : Window 핸들
 @param nTop [in] : 윗쪽 Anchor 값. 무시할 땐 atANCHOR_IGNORE
 @param nBottom [in] : 아랫쪽 Anchor 값. 무시할 땐 atANCHOR_IGNORE
 @param nLeft [in] : 왼쪽 Anchor 값. 무시할 땐 atANCHOR_IGNORE
 @param nRight [in] : 오른쪽 Anchor 값. 무시할 땐 atANCHOR_IGNORE
 @return 없음
 @remark Top/Bottom, Left/Right 등, 양편을 모두 atANCHOR_IGNORE로 설정하면 크기 변경 없이 위치만 중앙으로 이동하는 효과를 갖는다.\n
 				 해당 윈도우는 자동으로 레이아웃 재정렬까지 행해 준다.
*/
atVOID	atAPP_SetAnchor( atHAPP hApp, atHWINDOW hWnd, atINT nTop, atINT nBottom, atINT nLeft, atINT nRight )
{
	atLPAPP lpApp = atAPP_GetPtr(hApp);
	if( lpApp == atNULL ) return;
	
	atWINDOW_SetAnchor( hWnd, nTop, nBottom, nLeft, nRight );
	atWINMNG_AdjustWindowArea( lpApp->hWinMng, hWnd );
}


/** 
 @brief Window를 가장 앞으로 옮겨 온다.
 
 Window를 가장 앞으로 옮겨 온다.

 @param hApp [in] : Application 핸들
 @param hWnd [in] : Window 핸들
 @return 실제로 가장 전면으로 옮겨진 윈도우의 핸들
 @remark StayOnTop 속성에 의해 가장 앞쪽으로 옮기지 못할 수도 있다.
*/
atHWINDOW	atAPP_SetTopWindow( atHAPP hApp, atHWINDOW hWnd )
{
	atLPAPP lpApp = atAPP_GetPtr(hApp);
	if( lpApp == atNULL ) return atNULL;
	
	return atWINMNG_SetTopWindow( lpApp->hWinMng, hWnd );
}


/** 
 @brief Window를 가장 뒷쪽으로 옮긴다.
 
 Window를 가장 뒷쪽으로 옮긴다.

 @param hApp [in] : Application 핸들
 @param hWnd [in] : Window 핸들
 @return 실제로 가장 뒷쪽에 옮겨진 윈도우의 핸들
*/
atHWINDOW	atAPP_SetBottomWindow( atHAPP hApp, atHWINDOW hWnd )
{
	atLPAPP lpApp = atAPP_GetPtr(hApp);
	if( lpApp == atNULL ) return atNULL;
	
	return atWINMNG_SetBottomWindow( lpApp->hWinMng, hWnd );
}


/** 
 @brief 해당 ID의 윈도우를 찾는다.
 
 해당 ID의 윈도우를 찾는다.

 @param hApp [in] : Application 핸들
 @param nWindowID [in] : Window의 ID
 @return 찾은 Window의 핸들. 없을 시 atNULL
 @remark ID는 중복이 허용되기 때문에 동일 ID의 윈도우가 2개 이상일 시, 첫번째 것만 찾을 수 있다.
*/
atHWINDOW	atAPP_FindWindowByID( atHAPP hApp, atINT nWindowID )
{
	atLPAPP lpApp = atAPP_GetPtr(hApp);
	if( lpApp == atNULL ) return atNULL;
	
	return atWINMNG_FindWindowByID( lpApp->hWinMng, nWindowID );
}


/** 
 @brief 해당 이름의 윈도우를 찾는다.
 
 해당 ID의 윈도우를 찾는다.

 @param hApp [in] : Application 핸들
 @param szWindowText [in] : Window의 이름
 @return 찾은 Window의 핸들. 없을 시 atNULL
*/
atHWINDOW	atAPP_FindWindowByName( atHAPP hApp, atLPTSTR szWindowText )
{
	atLPAPP lpApp = atAPP_GetPtr(hApp);
	if( lpApp == atNULL ) return atNULL;
	
	return atWINMNG_FindWindowByName( lpApp->hWinMng, szWindowText );
}


/** 
 @brief 화면상의 해당 지점의 최상단 윈도우를 찾는다.
 
 화면상의 해당 지점의 최상단 윈도우를 찾는다.

 @param hApp [in] : Application 핸들
 @param nX, nY [in] : 기준이 되는 화면상의 위치
 @return 찾은 Window의 핸들. 없을 시 atNULL
*/
atHWINDOW	atAPP_FindWindowAt( atHAPP hApp, atINT nX, atINT nY )
{
	atLPAPP lpApp = atAPP_GetPtr(hApp);
	if( lpApp == atNULL ) return atNULL;
	
	return atWINMNG_FindWindowAt( lpApp->hWinMng, nX, nY );
}


/** 
 @brief Application의 Draw를 멈추거나 재개한다.
 
 Application의 Draw를 멈추거나 재개한다.

 @param hApp [in] : Application 핸들
 @param IsDraw [in] : Draw 동작 여부
 @return 없음
*/
void atAPP_SetDrawFlag( atHAPP hApp,  atBOOL IsDraw )
{
	atLPAPP lpApp = atAPP_GetPtr(hApp);
	if( lpApp == atNULL ) return;
	
	atWINMNG_SetDrawFlag( lpApp->hWinMng, IsDraw );
}


/** 
 @brief Window를 Show/Hide 시킨다.
 
 Window를 Show/Hide 시킨다.

 @param hApp [in] : Application 핸들
 @param hWnd [in] : Window 핸들
 @param bShow [in] : Show/Hide
 @return 없음
*/
void atAPP_ShowWindow( atHAPP hApp, atHWINDOW hWnd, atBOOL bShow )
{
	atLPAPP lpApp = atAPP_GetPtr(hApp);
	if( lpApp == atNULL ) return;
	
	atWINMNG_ShowWindow( lpApp->hWinMng, hWnd, bShow );
}


/** 
 @brief Window를 모두 다시 그린다.
 
 특정 베이스 윈도우를 기준으로 Application에 포함된 Window를 모두 다시 그린다.

 @param hApp [in] : Application 핸들
 @param hWndBase [in] : 기준이 되는 Window 핸들
 @return 그려진 윈도우의 갯수
*/
int atAPP_RedrawAllWindowEx( atHAPP hApp, atHWINDOW hWndBase )
{
	atLPAPP lpApp = atAPP_GetPtr(hApp);
	if( lpApp == atNULL ) return -1;
	
	return atWINMNG_RedrawAllWindowEx( lpApp->hWinMng, hWndBase );
}


/** 
 @brief Window를 모두 다시 그린다.
 
 Application에 포함된 Window를 모두 다시 그린다.

 @param hApp [in] : Application 핸들
 @return 그려진 윈도우의 갯수
*/
int atAPP_RedrawAllWindow( atHAPP hApp )
{
	atLPAPP lpApp = atAPP_GetPtr(hApp);
	if( lpApp == atNULL ) return -1;
	
	return atWINMNG_RedrawAllWindow( lpApp->hWinMng );
}


/** 
 @brief 특정 영역에 걸친 Window를 모두 다시 그린다.
 
 특정 영역에 걸친 Window를 모두 다시 그린다.

 @param hApp [in] : Application 핸들
 @param rtWinRect [in] : 그릴 영역
 @return 그려진 윈도우의 갯수
*/
int atAPP_RedrawAllWindowArea( atHAPP hApp, atRECT *rtWinRect )
{
	atLPAPP lpApp = atAPP_GetPtr(hApp);
	if( lpApp == atNULL ) return -1;
	
	return atWINMNG_RedrawAllWindowArea( lpApp->hWinMng, rtWinRect );
}


/** 
 @brief Application의 종료를 요청한다.
 
 Application의 종료를 요청한다.

 @param hApp [in] : Application 핸들
 @param nExitCode [in] : 종료코드
 @return 없음
*/
atVOID 	atAPP_SendExitCode( atHAPP hApp, atINT nExitCode )
{
	atLPAPP lpApp = atAPP_GetPtr(hApp);
	if( lpApp == atNULL ) return;
	
	atMSG_SendExitCode( lpApp->hMsgProc, nExitCode );
}


/** 
 @brief Application의 메시지 핸들러를 진행한다.(내부용)
 
 Application의 메시지 핸들러를 진행한다.(내부용)

 @param hApp [in] : Application 핸들
 @return 결과 코드
*/
int atAPP_ProcessEvents( atHAPP hApp )
{
	atLPAPP lpApp = atAPP_GetPtr(hApp);
	if( lpApp == atNULL ) return -1;
	
	return atMSG_ProcessEvents( lpApp->hMsgProc);
}


/** 
 @brief Window에 Message를 보낸다.
 
 Window에 Message를 보낸다.

 @param hApp [in] : Application 핸들
 @param hwin [in] : Window 핸들
 @param msg [in] : Message
 @param param1, param2 [in] : 파라미터
 @return 윈도우 핸들러 호출 결과값
*/
int atAPP_SendMessage( atHAPP hApp, atHWINDOW hwin, int msg, long param1, long param2 )
{
	atLPAPP lpApp = atAPP_GetPtr(hApp);
	if( lpApp == atNULL ) return -1;
	
	return atMSG_SendMessage( lpApp->hMsgProc, hwin, msg, param1, param2 );
}


/** 
 @brief Window에 Message를 보낸다.
 
 Window에 Message를 보낸다.

 @param hApp [in] : Application 핸들
 @param hwin [in] : Window 핸들
 @param msg [in] : Message
 @param param1, param2 [in] : 파라미터
 @return 윈도우 핸들러 호출 결과값
 @remark Message Queue를 이용하여 처리한다.
*/
int atAPP_PostMessage(  atHAPP hApp, atHWINDOW hwin, int msg, long param1, long param2 )
{
	atLPAPP lpApp = atAPP_GetPtr(hApp);
	if( lpApp == atNULL ) return -1;
	
	return atMSG_PostMessage( lpApp->hMsgProc, hwin, msg, param1, param2 );
}


/** 
 @brief Hooker를 설치한다.
 
 Hooker를 설치한다.

 @param hApp [in] : Application 핸들
 @param fnHookerCallback [in] : Hooker의 Handler
 @return Hooker의 ID
*/
atINT atAPP_AddMessageHooker( atHAPP hApp, atLPWNDPROC fnHookerCallback )
{
	atLPAPP lpApp = atAPP_GetPtr(hApp);
	if( lpApp == atNULL ) return -1;
	
	return atMSG_AddMessageHooker( lpApp->hMsgProc, (atLPWINOBJPROC)fnHookerCallback );
}


/** 
 @brief Hooker를 제거한다.
 
 Hooker를 제거한다.

 @param hApp [in] : Application 핸들
 @param nHookerID [in] : Hooker ID
 @return 없음
*/
void atAPP_RemoveMessageHooker( atHAPP hApp, atINT nHookerID )
{
	atLPAPP lpApp = atAPP_GetPtr(hApp);
	if( lpApp == atNULL ) return;
	
	atMSG_RemoveMessageHooker( lpApp->hMsgProc, nHookerID );
}


/** 
 @brief 윈도우의 레이아웃을 재조정
 
 Anchor를 적용하여 윈도우의 레이아웃을 재조정 한다.

 @param hApp [in] : Application 핸들
 @param hWnd [in] : Window 핸들
 @return 없음
*/
void	atAPP_AdjustWindowArea( atHAPP hApp, atHWINDOW hWnd )
{
	atLPAPP lpApp = atAPP_GetPtr(hApp);
	if( lpApp == atNULL ) return;
	
	atWINMNG_AdjustWindowArea( lpApp->hWinMng, hWnd );
}


/** 
 @brief 모든 윈도우의 레이아웃을 재조정
 
 모든 윈도우의 레이아웃을 Anchor를 적용하여 재조정 한다.

 @param hApp [in] : Application 핸들
 @return 없음
*/
void	atAPP_AdjustAllWindowArea( atHAPP hApp )
{
	atLPAPP lpApp = atAPP_GetPtr(hApp);
	if( lpApp == atNULL ) return;
	
	atWINMNG_AdjustAllWindowArea( lpApp->hWinMng );
}


/** 
 @brief 윈도우의 영역을 설정
 
 윈도우의 영역을 설정한다.

 @param hApp [in] : Application 핸들
 @param hWnd [in] : Window 핸들
 @param nX, nY [in] : 윈도우의 위치
 @param nWidth, nHeight [in] : 윈도우의 크기
 @return 없음
*/
atVOID	atAPP_SetWindowRegion( atHAPP hApp, atHWINDOW hWnd, atINT nX, atINT nY, atINT nWidth, atINT nHeight )
{
	atLPAPP lpApp = atAPP_GetPtr(hApp);
	if( lpApp == atNULL ) return;
	
	atWINMNG_SetWindowRegion( lpApp->hWinMng, hWnd, nX, nY, nWidth, nHeight );
}


/** 
 @brief 메시지 처리 tick
 
 메시지 처리 tick. 일정한 주기로 반복해서 호출해 줘야 한다. 보통은 내부에서 proc에 의해 호출.

 @param hApp [in] : Application 핸들
 @return 없음
*/
atVOID atAPP_TickMsgProcess( atHAPP hApp )
{
	atLPAPP lpApp = atAPP_GetPtr(hApp);
	if( lpApp == atNULL ) return;

	atMSG_ProcessEvents( lpApp->hMsgProc );
}
