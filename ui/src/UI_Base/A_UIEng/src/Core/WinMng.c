/**
  @file WinMng.c
  @brief WinMng에 관한 API
  @remark WinMng에 관한 API 모음
  @author Amos
  @date 2010-04-19 오전 11:41:00
  @see WinMng.h
*/
#include "UI_Types.h"
#include "UI_OpDef.h"
#include "Window.h"
#include "Control.h"
#include "WObjMng.h"
#include "WinMng.h"
#include "UI_API/UI_API.h"
#include "UI_Util.h"
#include "UI_Desk.h"
#include "UI_Kern.h"
#include <string.h>


atHWINOBJ atWINMNG_GetFocusedObj( atHWINMNG hWinMng );

#ifdef __USES_NOTIFIER
static void _notifier( atHWINOBJ hWinObj, atINT nMsg, atLONG lParam1, atLONG lParam2, atLPVOID lpNotifyData, atINT nRetCode )
{
	atHWINMNG hWinMng = (atHWINMNG)lpNotifyData;
	atLPWINMNG lpWinMng = atWINMNG_GetPtr( hWinMng );\
	if( nMsg == OBJMSG_CHANGECHILDOBJFOCUS || nMsg == OBJMSG_SHOW ) {
		lpWinMng->hFocusedWindow = atWINMNG_GetCurWindow( hWinMng );
		lpWinMng->hFocusedObj = atWINMNG_GetCurWindow( hWinMng ); //atWINMNG_GetFocusedObj(hWinMng);
	}
}
#endif

/*
atLPWINMNG atWINMNG_GetPtr(atHWINMNG hWinMng)
{
	return (atLPWINMNG)atWINOBJMNG_GetPtr( _TO_WINOBJMNG(hWinMng) );
}
*/


/** 
 @brief WinMng을 생성한다.
 
 WinMng을 생성한다.

 @param hParentWnd [in] : WinMng를 포함할 부모 Window의 핸들
 @return 생성된 WinMng의 핸들
*/
atHWINMNG	atWINMNG_CreateWinMng( atHWINDOW hParentWnd )
{
	int i;
	atHWINOBJMNG hWinObjMng;
	atLPWINMNG lpWinMng;

	hWinObjMng = atWINOBJMNG_CreateWinObjMng( _TO_WINOBJ(hParentWnd), sizeof(atWINMNG) - sizeof(atWINOBJMNG) );
	if( hWinObjMng == atNULL ) return atNULL;

	lpWinMng = atWINMNG_GetPtr( (atHWINMNG)hWinObjMng );

	lpWinMng->lpWinClassList = atUIAPI_CreateList();

	for( i = 0; i < 3; i ++ ){
		RESET_LAYOUTBITMAP( lpWinMng->DefLayoutBitmap[i] );
		SET_MARGIN( lpWinMng->DefLayoutMargin[i], 0, 0, 0, 0 );
	}

	SET_MARGIN( lpWinMng->DefLayoutMargin[0], 26, 3, 3, 3 );
	SET_MARGIN( lpWinMng->DefLayoutMargin[1], 1, 1, 1, 1 );
	SET_MARGIN( lpWinMng->DefLayoutMargin[2], 34, 4, 4, 4 );

	return (atHWINMNG)hWinObjMng;
}


/** 
 @brief WinMng을 파괴한다.
 
 WinMng을 파괴한다.

 @param hWinMng [in] : hWinMng 핸들
 @return 없음
*/
void atWINMNG_DestroyWinMng( atHWINMNG hWinMng )
{
	atLPWINMNG lpWinMng = atWINMNG_GetPtr(hWinMng);
	if( lpWinMng == atNULL ) return;
	
	atUIAPI_DestroyList( lpWinMng->lpWinClassList, atTRUE );

	atWINOBJMNG_DestroyWinObjMng( _TO_WINOBJMNG(hWinMng) );
}


/** 
 @brief Default 레이아웃을 설정한다.
 
 Default 레이아웃을 설정한다.

 @param hWinMng [in] : hWinMng 핸들
 @return 없음
*/
atBOOL  atWINMNG_SetDefaultLayout( atHWINMNG hWinMng, atINT DefIndex, atLAYOUTBITMAP *lpLayoutBitmap, atMARGIN *lpLayoutMargin )
{
	atLPWINMNG lpWinMng = atWINMNG_GetPtr(hWinMng);
	if( lpWinMng == atNULL ) return atFALSE;

	if( DefIndex > 2 ) return atFALSE;

	if( lpLayoutBitmap == atNULL )	{
		RESET_LAYOUTBITMAP( lpWinMng->DefLayoutBitmap[DefIndex] );
	} else {
		lpWinMng->DefLayoutBitmap[DefIndex] = *lpLayoutBitmap;
	}

	if( lpLayoutMargin == atNULL ){
		SET_MARGIN( lpWinMng->DefLayoutMargin[DefIndex], 0, 0, 0, 0 );
	} else {
	 	lpWinMng->DefLayoutMargin[DefIndex] = *lpLayoutMargin;
	}

	return atTRUE;
}


/** 
 @brief WinClass를 등록한다.
 
 WinClass를 등록한다.

 @param hWinMng [in] : hWinMng 핸들
 @param szClassName [in] : WinClass의 이름
 @param pWinClass [in] : WinClass 구조체의 포인터
 @return 성공시 atTRUE, 실패시 atFALSE
*/
atBOOL atWINMNG_RegistWinClass( atHWINMNG hWinMng, atLPTSTR szClassName, atWINCLASS *pWinClass )
{
	atWINCLASS_ITOR *lpWinClassItor;
	atLPWINMNG lpWinMng = atWINMNG_GetPtr(hWinMng);
	if( lpWinMng == atNULL ) return atFALSE;
	
	lpWinClassItor = (atWINCLASS_ITOR *)UI_MEMALLOC( sizeof(atWINCLASS_ITOR) );
	if( lpWinClassItor == atNULL ) return atFALSE;

	UI_STRCPY( lpWinClassItor->szClassName, szClassName );
	atUIAPI_memcpy( &lpWinClassItor->ClassData, pWinClass, sizeof(atWINCLASS) );

	atUIAPI_AddListTail( lpWinMng->lpWinClassList, lpWinClassItor );

	return atTRUE;
}


/** 
 @brief WinClass 등록을 해지한다.
 
 WinClass 등록을 해지한다.

 @param hWinMng [in] : hWinMng 핸들
 @param szClassName [in] : WinClass의 이름
 @return 성공시 atTRUE, 실패시 atFALSE
*/
atBOOL atWINMNG_UnregistWinClass( atHWINMNG hWinMng, atLPTSTR szClassName )
{
	int pos;
	atWINCLASS_ITOR *lpWinClassItor;
	atLPWINMNG lpWinMng = atWINMNG_GetPtr(hWinMng);
	if( lpWinMng == atNULL ) return atFALSE;
	if( lpWinMng->lpWinClassList == atNULL ) return atFALSE;
	
	lpWinClassItor = (atWINCLASS_ITOR *)atUIAPI_GetListHead( lpWinMng->lpWinClassList, &pos );
	
	while( lpWinClassItor != atNULL ){
		if( UI_STRCMP(lpWinClassItor->szClassName, szClassName) == 0 ){
			atUIAPI_RemoveListItem( lpWinMng->lpWinClassList, lpWinClassItor, atTRUE );
			return atTRUE;
		}
		
		lpWinClassItor = (atWINCLASS_ITOR *)atUIAPI_GetListNext( lpWinMng->lpWinClassList, &pos );
	}

	return atFALSE;
}


/** 
 @brief 주어진 이름에 해당하는 WinClass를 얻는다.
 
 주어진 이름에 해당하는 WinClass를 얻는다.

 @param hWinMng [in] : hWinMng 핸들
 @param szClassName [in] : WinClass의 이름
 @return 해당 이름의 WinClass 구조체 포인터. 실패시 atNULL
*/
atWINCLASS *atWINMNG_GetWinClass( atHWINMNG hWinMng, atLPTSTR szClassName )
{
	int pos;
	atWINCLASS_ITOR *lpWinClassItor;
	atLPWINMNG lpWinMng = atWINMNG_GetPtr(hWinMng);
	if( lpWinMng == atNULL ) return atNULL;
	if( lpWinMng->lpWinClassList == atNULL ) return atNULL;
	
	lpWinClassItor = (atWINCLASS_ITOR *)atUIAPI_GetListHead( lpWinMng->lpWinClassList, &pos );
	
	while( lpWinClassItor != atNULL ){
		if( UI_STRCMP(lpWinClassItor->szClassName, szClassName) == 0 ){
			return &lpWinClassItor->ClassData;
		}
		
		lpWinClassItor = (atWINCLASS_ITOR *)atUIAPI_GetListNext( lpWinMng->lpWinClassList, &pos );
	}
	
	return atNULL;
}


/** 
 @brief Window를 생성한다.
 
 Window를 생성한다.

 @param hWinMng [in] : WinMng 핸들
 @param hOwnerWnd [in] : 부모 Window의 핸들
 @param nID [in] : Window의 ID
 @param nStyle [in] : Window Style
 @param szWindowText [in] : Window 타이틀
 @param nWidth, nHeight [in] : 생성될 윈도우의 크기 
 @param rtLayoutMargin [in] : 외각선의 여백. 이 여백을 제외하여 클라이언트의 크기가 결정된다.
 @param rtAnchor [in] : Anchor를 지정한다.
 @param dwUserDataSize [in] : 사용자 추가 데이터 크기
 @param fnWndProc [in] : 사용자 이벤트 핸들러
 @param lpLayoutBm [in] : 레이아웃 비트맵 세트 
 @param lpCreateParam [in] : 생성시 전달할 파라미터
 @param lpExData [in] : 생성시 참고할 추가 데이터. 주로 WinClass에서 전달.
 @return Window 핸들
*/
atHWINDOW	atWINMNG_CreateWindowEx( atHWINMNG hWinMng, atHWINDOW hOwnerWnd, atINT nID, int nStyle, atLPTSTR szWindowText, 
										int nWidth, int nHeight, atMARGIN *lpLayoutMargin, atMARGIN *rtAnchor,
										atDWORD dwUserDataSize, atLPWNDPROC fnWndProc, 
										atLAYOUTBITMAP *lpLayoutBm, 
										void *lpCreateParam, void *lpExData )
{
	atHWINDOW hWnd;
	atREGION rgn;
	atLPWINMNG lpWinMng = atWINMNG_GetPtr(hWinMng);

	switch( nStyle & 0xFF ){
		case atWS_NONE:
				lpLayoutBm = atNULL;
				lpLayoutMargin = atNULL;
				break;
		case atWS_NONEFRAME:
				lpLayoutBm = atNULL;
				lpLayoutMargin = atNULL;
				break;
		case atWS_FRAMEWINDOW:
				lpLayoutBm = &lpWinMng->DefLayoutBitmap[0];
				lpLayoutMargin = &lpWinMng->DefLayoutMargin[0];
				break;
		case atWS_SINGLEFRAME:
				lpLayoutBm = &lpWinMng->DefLayoutBitmap[1];
				lpLayoutMargin = &lpWinMng->DefLayoutMargin[1];
				break;
		case atWS_POPUPWINDOW:
				lpLayoutBm = &lpWinMng->DefLayoutBitmap[2];
				lpLayoutMargin = &lpWinMng->DefLayoutMargin[2];
				break;
		case atWS_USERDEFINE:
//				lpLayoutBm = atNULL;
//				lpLayoutMargin = atNULL;
				break;
		default:
				lpLayoutBm = atNULL;
				lpLayoutMargin = atNULL;
				break;
	}

	if( (nWidth <= 0) || (nHeight <= 0) ){
		if( lpWinMng->Base.hParentWinObj ){
			atWINOBJ_GetClientRegion( lpWinMng->Base.hParentWinObj, &rgn );
		} else {
			atDESKTOP_GetDesktopArea( atDESKTOP_AREA_CLIENT, &rgn );
		}

		if( nWidth <= 0 ) nWidth = rgn.nWidth - nWidth;
		if( nHeight <= 0 ) nHeight = rgn.nHeight - nHeight;
	}		
	
	hWnd = atWINDOW_CreateEx( hOwnerWnd, nID, nStyle, szWindowText, 
							nWidth, nHeight, lpLayoutMargin, rtAnchor,
							dwUserDataSize, fnWndProc,
							lpLayoutBm, 
							lpCreateParam, lpExData );
	if( hWnd == atNULL ) return atNULL;

#ifdef __USES_NOTIFIER
	atWINOBJ_SetObjNotifier( _TO_WINOBJ(hWnd), _notifier, (atLPVOID)hWinMng );
#endif

	return hWnd;
}

	
/** 
 @brief Window를 생성한다.
 
 Window를 생성한다.

 @param hWinMng [in] : WinMng 핸들
 @param hOwnerWnd [in] : 부모 Window의 핸들
 @param nID [in] : Window의 ID
 @param nStyle [in] : Window Style
 @param szWindowText [in] : Window 타이틀
 @param nStartY, nStartY [in] : 생성될 윈도우의 위치
 @param nWidth, nHeight [in] : 생성될 윈도우의 크기 
 @param dwUserDataSize [in] : 사용자 추가 데이터 크기
 @param fnWndProc [in] : 사용자 이벤트 핸들러
 @param lpCreateParam [in] : 생성시 전달할 파라미터
 @param lpExData [in] : 생성시 참고할 추가 데이터. 주로 WinClass에서 전달.
 @return Window 핸들
*/
atHWINDOW	atWINMNG_CreateWindow( atHWINMNG hWinMng, atHWINDOW hOwnerWnd, atINT nID, int nStyle, atLPTSTR szWindowText, 
										int nStartX, int nStartY, int nWidth, int nHeight,
										atDWORD dwUserDataSize, atLPWNDPROC fnWndProc,
										void *lpCreateParam, void *lpExData )
{
	atBOOL r;
//	atLPWINMNG lpWinMng = atWINMNG_GetPtr(hWinMng);
	atHWINDOW hWnd;
	atMARGIN rtAnchor;
	
	SET_MARGIN( rtAnchor, nStartY, atANCHOR_IGNORE, nStartX, atANCHOR_IGNORE);	
	hWnd = atWINMNG_CreateWindowEx( hWinMng, hOwnerWnd, nID, nStyle, szWindowText,
									nWidth, nHeight, atNULL, &rtAnchor,
									dwUserDataSize, fnWndProc,
									atNULL, 
									lpCreateParam, lpExData );
	if( hWnd == atNULL ) return atNULL;
	
	r = atWINMNG_RegistWindow( hWinMng, hWnd );
	if( r == atNULL ){
		atWINMNG_DestroyWindow( hWinMng, hWnd );
	}

	return (r)? hWnd : atNULL;
}


/** 
 @brief WinClass를 이용하여 Window를 생성한다.
 
 WinClass를 이용하여 Window를 생성한다.

 @param hWinMng [in] : WinMng 핸들
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
atHWINDOW	atWINMNG_CreateWindowByClass( atHWINMNG hWinMng, atHWINDOW hOwnerWnd, atINT nID, atLPTSTR szWindowText, 
										int nWidth, int nHeight, atMARGIN *rtAnchor,
										atLPTSTR szClassName, void *lpCreateParam )
{
	atBOOL r;
	atLPWINMNG lpWinMng = atWINMNG_GetPtr(hWinMng);
	atHWINDOW hWnd;
	atWINCLASS_ITOR *lpWinClassItor;
	int pos;
	atWINCLASS *lpWinClass = atNULL;
	atLAYOUTBITMAP *layoutbm;
	atMARGIN *margin;

	lpWinClassItor = (atWINCLASS_ITOR *)atUIAPI_GetListHead(lpWinMng->lpWinClassList, &pos);
	if( lpWinClassItor == atNULL ) return atNULL;

	atKERNEL_LockInput();

	while( lpWinClassItor != atNULL ){
		if( UI_STRCMP( lpWinClassItor->szClassName, szClassName ) == 0 ){
			lpWinClass = &lpWinClassItor->ClassData;
			break;
		}

		lpWinClassItor = (atWINCLASS_ITOR *)atUIAPI_GetListNext(lpWinMng->lpWinClassList, &pos);
	}
	if( lpWinClass == atNULL ){
		atKERNEL_UnlockInput();
		return atNULL;
	}

	if( (lpWinClass->dwAttr & 0xFF) == atWS_USERDEFINE ){
		layoutbm = &lpWinClass->tLayoutBm;
		margin = &lpWinClass->rtMargin;
	} else {
		layoutbm = atNULL;
		margin = atNULL;
	}

	hWnd = atWINMNG_CreateWindowEx( hWinMng, hOwnerWnd, nID, lpWinClass->dwAttr, szWindowText, 
							nWidth, nHeight, margin, rtAnchor, 
							lpWinClass->dwUserDataSize, lpWinClass->fnWndProc,
							layoutbm, 
							lpCreateParam, (void *)lpWinClass->ExData );
	if( hWnd == atNULL ){
		atKERNEL_UnlockInput();
		return atNULL;
	}
	
	r = atWINMNG_RegistWindow( hWinMng, hWnd );

	atKERNEL_UnlockInput();

	return (r)? hWnd : atNULL;
}

	
/** 
 @brief Window를 소멸시킨다.
 
 Window를 소멸시킨다.

 @param hWinMng [in] : hWinMng 핸들
 @param hWnd [in] : Window 핸들
 @return 없음
*/
void atWINMNG_DestroyWindow( atHWINMNG hWinMng, atHWINDOW hWnd )
{
	atWINOBJMNG_DestroyWinObj( _TO_WINOBJMNG(hWinMng), _TO_WINOBJ(hWnd) );
//	atWINMNG_DestroyWindowEx( hWinMng, hWnd, atTRUE );

	if( atWINOBJMNG_GetWinObjCount(_TO_WINOBJMNG(hWinMng)) <= 0 ){
		atSOFTKEY_SetSoftKeyText(_AT(""), _AT(""), _AT(""));
//		atDESKTOP_DrawDesktop(atFALSE);
		atDESKTOP_Refresh(atDESKTOP_AREA_ALL);
	}
}


/** 
 @brief 포함된 모든 Window를 소멸시킨다.
 
 포함된 모든 Window를 소멸시킨다.

 @param hWinMng [in] : hWinMng 핸들
 @return 없음
*/
void atWINMNG_DestroyAllWindows( atHWINMNG hWinMng )
{
	atWINOBJMNG_DestroyAllWinObjs( _TO_WINOBJMNG(hWinMng) );

	atSOFTKEY_SetSoftKeyText(_AT(""), _AT(""), _AT(""));
	atDESKTOP_Refresh(atDESKTOP_AREA_ALL);
}


/** 
 @brief 모든 Window를 파괴 (조건 판단 핸들러)
 
 현재 등록된 모든 Window중, 조건에 맞는 윈도우만 골라서 파괴한다.\n
 이때, 파괴할 조건 판단 핸들러를 파라미터로 전달한다.

 @param hWinMng [in] : hWinMng 핸들
 @param fnIsDel [in] : 조건 판단 핸들러.
 @return 없음
*/
void atWINMNG_DestroyWindowsEx( atHWINMNG hWinMng, atBOOL (*fnIsDel)(atHWINDOW) )
{
	atWINOBJMNG_DestroyWinObjsEx( _TO_WINOBJMNG(hWinMng), (atBOOL (*)(atHWINOBJ))fnIsDel );

	if( atWINMNG_GetWindowCount( hWinMng ) <= 0 ){
		atSOFTKEY_SetSoftKeyText(_AT(""), _AT(""), _AT(""));
		atDESKTOP_Refresh(atDESKTOP_AREA_ALL);
	}
}


//int	atWINMNG_RedrawAllWindowEx( atHWINMNG hWinMng, atHWINDOW hWindowBase )
//{
//	atLPWINMNG lpWinMng = atWINMNG_GetPtr(hWinMng);
//	if(lpWinMng == atNULL) return -1;
//
//	return atWINOBJMNG_RedrawAllWinObjEx( _TO_WINOBJMNG(hWinMng), _TO_WINOBJ(hWindowBase), lpWinMng->hFocusedWindow );
//}
//
//int	atWINMNG_RedrawAllWindow( atHWINMNG hWinMng )
//{
//	atLPWINMNG lpWinMng = atWINMNG_GetPtr(hWinMng);
//	if(lpWinMng == atNULL) return -1;
//
//	return atWINOBJMNG_RedrawAllWinObj( _TO_WINOBJMNG(hWinMng), lpWinMng->hFocusedWindow );
//}
//
//int	atWINMNG_RedrawAllWindowArea( atHWINMNG hWinMng, atRECT *rtWinRect, atHWINOBJ hFocusedObj )
//{
//	atLPWINMNG lpWinMng = atWINMNG_GetPtr(hWinMng);
//	if(lpWinMng == atNULL) return -1;
//
//	return atWINOBJMNG_RedrawAllWinObjArea( _TO_WINOBJMNG(hWinMng), rtWinRect, lpWinMng->hFocusedWindow );
//}


void atWINMNG_SetFocusToWindow( atHWINMNG hWinMng, atHWINDOW hWindow )
{
	atLPWINMNG lpWinMng = atWINMNG_GetPtr(hWinMng);
	if(lpWinMng == atNULL) return;
	
	atWINMNG_SetCurWindow( hWinMng, hWindow );
}


void atWINMNG_SetFocusToControl( atHWINMNG hWinMng, atHCONTROL hControl )
{
	atLPWINMNG lpWinMng = atWINMNG_GetPtr(hWinMng);
	if(lpWinMng == atNULL) return;

	atHWINDOW hWindow = atCONTROL_GetParentWindow( hControl );

	atWINMNG_SetCurWindow( hWinMng, hWindow );
	atWINDOW_SetCurControl( hWindow, hControl );
}


//atHWINOBJ atWINMNG_GetFocusedObj( atHWINMNG hWinMng )
//{
//	atHWINDOW hWindow;
//	atHCONTROL hControl;
//	atLPWINMNG lpWinMng = atWINMNG_GetPtr(hWinMng);
//	if(lpWinMng == atNULL) return -1;
//
//	hWindow = atWINMNG_GetCurWindow( hWinMng );
//	if(hWindow == atNULL) return atNULL;
//
//	hControl = atWINDOW_GetCurControl( hWindow );
//
//	if(hControl != atNULL) return _TO_WINOBJ(hControl);
//	else return _TO_WINOBJ(hWindow);
//}
