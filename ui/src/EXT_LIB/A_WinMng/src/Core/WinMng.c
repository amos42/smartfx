/**
  @file WinMng.c
  @brief WinMng�� ���� API
  @remark WinMng�� ���� API ����
  @author Amos
  @date 2010-04-19 ���� 11:41:00
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
 @brief WinMng�� �����Ѵ�.
 
 WinMng�� �����Ѵ�.

 @param hParentWnd [in] : WinMng�� ������ �θ� Window�� �ڵ�
 @return ������ WinMng�� �ڵ�
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
 @brief WinMng�� �ı��Ѵ�.
 
 WinMng�� �ı��Ѵ�.

 @param hWinMng [in] : hWinMng �ڵ�
 @return ����
*/
void atWINMNG_DestroyWinMng( atHWINMNG hWinMng )
{
	atLPWINMNG lpWinMng = atWINMNG_GetPtr(hWinMng);
	if( lpWinMng == atNULL ) return;
	
	atUIAPI_DestroyList( lpWinMng->lpWinClassList, atTRUE );

	atWINOBJMNG_DestroyWinObjMng( _TO_WINOBJMNG(hWinMng) );
}


/** 
 @brief Default ���̾ƿ��� �����Ѵ�.
 
 Default ���̾ƿ��� �����Ѵ�.

 @param hWinMng [in] : hWinMng �ڵ�
 @return ����
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
 @brief WinClass�� ����Ѵ�.
 
 WinClass�� ����Ѵ�.

 @param hWinMng [in] : hWinMng �ڵ�
 @param szClassName [in] : WinClass�� �̸�
 @param pWinClass [in] : WinClass ����ü�� ������
 @return ������ atTRUE, ���н� atFALSE
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
 @brief WinClass ����� �����Ѵ�.
 
 WinClass ����� �����Ѵ�.

 @param hWinMng [in] : hWinMng �ڵ�
 @param szClassName [in] : WinClass�� �̸�
 @return ������ atTRUE, ���н� atFALSE
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
 @brief �־��� �̸��� �ش��ϴ� WinClass�� ��´�.
 
 �־��� �̸��� �ش��ϴ� WinClass�� ��´�.

 @param hWinMng [in] : hWinMng �ڵ�
 @param szClassName [in] : WinClass�� �̸�
 @return �ش� �̸��� WinClass ����ü ������. ���н� atNULL
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
 @brief Window�� �����Ѵ�.
 
 Window�� �����Ѵ�.

 @param hWinMng [in] : WinMng �ڵ�
 @param hOwnerWnd [in] : �θ� Window�� �ڵ�
 @param nID [in] : Window�� ID
 @param nStyle [in] : Window Style
 @param szWindowText [in] : Window Ÿ��Ʋ
 @param nWidth, nHeight [in] : ������ �������� ũ�� 
 @param rtLayoutMargin [in] : �ܰ����� ����. �� ������ �����Ͽ� Ŭ���̾�Ʈ�� ũ�Ⱑ �����ȴ�.
 @param rtAnchor [in] : Anchor�� �����Ѵ�.
 @param dwUserDataSize [in] : ����� �߰� ������ ũ��
 @param fnWndProc [in] : ����� �̺�Ʈ �ڵ鷯
 @param lpLayoutBm [in] : ���̾ƿ� ��Ʈ�� ��Ʈ 
 @param lpCreateParam [in] : ������ ������ �Ķ����
 @param lpExData [in] : ������ ������ �߰� ������. �ַ� WinClass���� ����.
 @return Window �ڵ�
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
 @brief Window�� �����Ѵ�.
 
 Window�� �����Ѵ�.

 @param hWinMng [in] : WinMng �ڵ�
 @param hOwnerWnd [in] : �θ� Window�� �ڵ�
 @param nID [in] : Window�� ID
 @param nStyle [in] : Window Style
 @param szWindowText [in] : Window Ÿ��Ʋ
 @param nStartY, nStartY [in] : ������ �������� ��ġ
 @param nWidth, nHeight [in] : ������ �������� ũ�� 
 @param dwUserDataSize [in] : ����� �߰� ������ ũ��
 @param fnWndProc [in] : ����� �̺�Ʈ �ڵ鷯
 @param lpCreateParam [in] : ������ ������ �Ķ����
 @param lpExData [in] : ������ ������ �߰� ������. �ַ� WinClass���� ����.
 @return Window �ڵ�
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
 @brief WinClass�� �̿��Ͽ� Window�� �����Ѵ�.
 
 WinClass�� �̿��Ͽ� Window�� �����Ѵ�.

 @param hWinMng [in] : WinMng �ڵ�
 @param hOwnerWnd [in] : �θ� Window�� �ڵ�
 @param nID [in] : Window�� ID
 @param nStyle [in] : Window Style
 @param szWindowText [in] : Window Ÿ��Ʋ
 @param nWidth, nHeight [in] : ������ �������� ũ�� 
 @param rtAnchor [in] : Anchor�� �����Ѵ�.
 @param szClassName [in] : WinClass �̸�
 @param lpCreateParam [in] : ������ ������ �Ķ����
 @return Window �ڵ�
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
 @brief Window�� �Ҹ��Ų��.
 
 Window�� �Ҹ��Ų��.

 @param hWinMng [in] : hWinMng �ڵ�
 @param hWnd [in] : Window �ڵ�
 @return ����
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
 @brief ���Ե� ��� Window�� �Ҹ��Ų��.
 
 ���Ե� ��� Window�� �Ҹ��Ų��.

 @param hWinMng [in] : hWinMng �ڵ�
 @return ����
*/
void atWINMNG_DestroyAllWindows( atHWINMNG hWinMng )
{
	atWINOBJMNG_DestroyAllWinObjs( _TO_WINOBJMNG(hWinMng) );

	atSOFTKEY_SetSoftKeyText(_AT(""), _AT(""), _AT(""));
	atDESKTOP_Refresh(atDESKTOP_AREA_ALL);
}


/** 
 @brief ��� Window�� �ı� (���� �Ǵ� �ڵ鷯)
 
 ���� ��ϵ� ��� Window��, ���ǿ� �´� �����츸 ��� �ı��Ѵ�.\n
 �̶�, �ı��� ���� �Ǵ� �ڵ鷯�� �Ķ���ͷ� �����Ѵ�.

 @param hWinMng [in] : hWinMng �ڵ�
 @param fnIsDel [in] : ���� �Ǵ� �ڵ鷯.
 @return ����
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
