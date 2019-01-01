/**
  @file WObjMng.c
  @brief WinObjMng 관한 API
  @remark WinObjMng에 관한 API 모음
  @author Amos
  @date 2010-04-19 오전 11:41:00
  @see WObjMng.h WinObj.h
*/
#include "UI_MNG.h"
#include "WinObj.h"
#include "WObjMng.h"



/** 
 @brief WinObjMng 개체의 포인터를 얻는다. (내부용)
 
 WinObjMng 개체의 포인터를 얻는다. (내부용)

 @param hWinObjMng [in] : WinObjMng 핸들
 @return WinObjMng 개체의 포인터
 @remark 내부용
*/
atLPWINOBJMNG atWINOBJMNG_GetPtr(atHWINOBJMNG hWinObjMng)
{
	return (atLPWINOBJMNG)hWinObjMng;
}


/**
 @brief WinObjMng 개체의 핸들을 얻는다. (내부용)
 
 WinObjMng 개체의 핸들을 얻는다. (내부용)

 @param lpApp [in] : WinObjMng 개체의 포인터
 @return WinObjMng의 핸들
 @remark 내부용
*/
atHWINOBJMNG atWINOBJMNG_GetHandle(atLPWINOBJMNG lpWinObjMng)
{
	return (atHWINOBJMNG)lpWinObjMng;
}


/** 
 @brief WinObjMng을 생성한다.
 
 WinObjMng을 생성한다.

 @param lHeapSize [in] : 애플리케이션이 사용하는 Heap 사이즈
 @return 생성된 WinObjMng의 핸들
 @remark 현재 Heap 크기는 사용되지 않는다.
*/
atHWINOBJMNG	atWINOBJMNG_CreateWinObjMng0( atLPWINOBJMNG lpWinObjMng, atHWINOBJ hParentWinObj )
{
	lpWinObjMng->bActivate = atTRUE;

	lpWinObjMng->hParentWinObj = hParentWinObj;

	lpWinObjMng->lpWinObjList = atUIAPI_CreateList();

	lpWinObjMng->hCurWinObj = atNULL;
	lpWinObjMng->nCurWinObjOrder = -1;

	lpWinObjMng->bIsDraw = atTRUE;
	
	return atWINOBJMNG_GetHandle(lpWinObjMng);
}


/** 
 @brief WinObjMng을 생성한다.
 
 WinObjMng을 생성한다.

 @param hParentWinObj [in] : WinObjMng를 포함할 부모 WinObj의 핸들
 @param ExtObjMngSize [in] : WinObjMng가 내부적으로 사용할 추가 데이터의 크기
 @return 생성된 WinObjMng의 핸들
*/
atHWINOBJMNG	atWINOBJMNG_CreateWinObjMng( atHWINOBJ hParentWinObj, atINT ExtObjMngSize )
{
	atLPWINOBJMNG lpWinObjMng;

	lpWinObjMng = (atLPWINOBJMNG)UI_MEMALLOC( sizeof(atWINOBJMNG) + ExtObjMngSize );
	if ( lpWinObjMng == atNULL ) return atNULL;

//	atUIAPI_memset( (atBYTE *)lpWinObjMng + sizeof(atWINOBJMNG), 0, ExtObjMngSize );
	atUIAPI_memset( (atBYTE *)lpWinObjMng, 0, sizeof(atWINOBJMNG) + ExtObjMngSize );

	return atWINOBJMNG_CreateWinObjMng0( lpWinObjMng, hParentWinObj );
}


/** 
 @brief WinObjMng을 파괴한다.
 
 WinObjMng을 파괴한다.

 @param hWinObjMng [in] : WinObjMng 핸들
 @return 없음
*/
void atWINOBJMNG_DestroyWinObjMng( atHWINOBJMNG hWinObjMng )
{
	atLPWINOBJMNG lpWinObjMng = atWINOBJMNG_GetPtr(hWinObjMng);
	if( lpWinObjMng == atNULL ) return;
	
	lpWinObjMng->bActivate = atFALSE;

	atWINOBJMNG_DestroyAllWinObjs(hWinObjMng);

//	atUIAPI_DestroyList( lpWinObjMng->lpWinObjList, atTRUE );
	atUIAPI_DestroyList( lpWinObjMng->lpWinObjList, atFALSE );
	lpWinObjMng->lpWinObjList = atNULL;

	UI_MEMFREE( lpWinObjMng );
}


/** 
 @brief WinObj를 파괴
 
 WinObj를 파괴한다.

 @param hWinObjMng [in] : WinObjMng 핸들
 @param hWinObj [in] : 파괴할 WinObj 핸들
 @return 없음
*/
void atWINOBJMNG_DestroyWinObj( atHWINOBJMNG hWinObjMng, atHWINOBJ hWinObj )
{
	atLPWINOBJMNG lpWinObjMng = atWINOBJMNG_GetPtr(hWinObjMng);
	atHWINOBJ hWinObj2;
	int pos;
	
	atWINOBJMNG_ReleaseWinObj( hWinObjMng, hWinObj );
	atWINOBJ_ProcessObjProc( hWinObj, OBJMSG_REQ_DESTROY, 0, 0 );

	hWinObj2 = (atHWINOBJ)atUIAPI_GetListTail(lpWinObjMng->lpWinObjList, &pos);
	while ( hWinObj2 != atNULL){
		if( atWINOBJ_GetParentObj(hWinObj2) == hWinObj ){
			atWINOBJMNG_DestroyWinObj( hWinObjMng, hWinObj2 );
		}
		hWinObj2 = (atHWINOBJ)atUIAPI_GetListPrev(lpWinObjMng->lpWinObjList, &pos);
	}
}


/** 
 @brief 모든 WinObj를 파괴
 
 현재 WinObjMng에 등록된 모든 WinObj를 파괴한다.

 @param hWinObjMng [in] : WinObjMng 핸들
 @return 없음
*/
void atWINOBJMNG_DestroyAllWinObjs( atHWINOBJMNG hWinObjMng )
{
	atHWINOBJ	hWinObj;
	atLPWINOBJMNG lpWinObjMng = atWINOBJMNG_GetPtr(hWinObjMng);
	if( lpWinObjMng == atNULL ) return;

	atWINOBJMNG_SetDrawFlag( hWinObjMng, atFALSE );
	
	hWinObj = (atHWINOBJ)atUIAPI_GetListTail(lpWinObjMng->lpWinObjList, atNULL);
	while ( hWinObj != atNULL){
		if( hWinObj == lpWinObjMng->hCurWinObj ){
//			atWINOBJ_ProcessObjProc( hWinObj, OBJMSG_LOSTFOCUS, 0, 0);
			//atWINOBJ_SetFocus( hWinObj, atFALSE );
			lpWinObjMng->hCurWinObj = atNULL;
			lpWinObjMng->nCurWinObjOrder = -1;
		}

		atWINOBJMNG_DestroyWinObj( hWinObjMng, hWinObj );

		hWinObj = (atHWINOBJ)atUIAPI_GetListTail(lpWinObjMng->lpWinObjList, atNULL);
	}

	lpWinObjMng->hCurWinObj = atNULL;
	lpWinObjMng->nCurWinObjOrder = -1;
	
	if( lpWinObjMng->hParentWinObj == atNULL ){
		atSOFTKEY_SetSoftKeyText(_AT(""), _AT(""), _AT(""));
	}
	
	atWINOBJMNG_SetDrawFlag( hWinObjMng, atTRUE );	
}


/** 
 @brief 모든 WinObj를 파괴 (조건 판단 핸들러)
 
 현재 WinObjMng에 등록된 모든 WinObj중, 조건에 맞는 윈도우만 골라서 파괴한다.\n
 이때, 파괴할 조건 판단 핸들러를 파라미터로 전달한다.

 @param hWinObjMng [in] : WinObjMng 핸들
 @param fnIsDel [in] : 조건 판단 핸들러.
 @return 없음
*/
void atWINOBJMNG_DestroyWinObjsEx( atHWINOBJMNG hWinObjMng, atBOOL (*fnIsDel)(atHWINOBJ) )
{
	atHWINOBJ	hWinObj, hWinObj2;
	int pos;
	atLPWINOBJMNG lpWinObjMng = atWINOBJMNG_GetPtr(hWinObjMng);
	if( lpWinObjMng == atNULL ) return;
	

	if( fnIsDel == atNULL ){
		atWINOBJMNG_DestroyAllWinObjs(hWinObjMng);
		return;
	}
		
	atWINOBJMNG_SetDrawFlag( hWinObjMng, atFALSE );
	
	hWinObj = (atHWINOBJ)atUIAPI_GetListTail(lpWinObjMng->lpWinObjList, &pos);
	while( hWinObj != atNULL){
		if( fnIsDel(hWinObj) ){
			hWinObj2 = (atHWINOBJ)atUIAPI_GetListPrev(lpWinObjMng->lpWinObjList, &pos);
			atWINOBJMNG_ReleaseWinObj( hWinObjMng, hWinObj );
			atWINOBJ_Destroy(hWinObj);
			UI_MEMFREE( atWINOBJ_GetPtr(hWinObj) );
			hWinObj = hWinObj2;
		} else {
			hWinObj = (atHWINOBJ)atUIAPI_GetListPrev(lpWinObjMng->lpWinObjList, &pos);
		}
	}

	atWINOBJMNG_SetDrawFlag( hWinObjMng, atTRUE );	

//	g_hCurWinObj = atUIAPI_GetListHead(g_lpWinObjList);
//	g_hCurrEventWinObj = g_hCurWinObj;

	if( lpWinObjMng->bActivate )
		if( atUIAPI_GetListCount(lpWinObjMng->lpWinObjList) > 0 ) {
//			atWINOBJMNG_RedrawAllWinObj( hWinObjMng, atTRUE );
			atUIAPI_AddInvalideAreaAll();
		}
}


/** 
 @brief WinObj의 갯수를 얻는다.
 
 현재 WinObjMng에 등록된 WinObj의 갯수를 얻는다.

 @param hWinObjMng [in] : WinObjMng 핸들
 @return WinObj의 갯수
*/
atINT atWINOBJMNG_GetWinObjCount( atHWINOBJMNG hWinObjMng )
{
	atLPWINOBJMNG lpWinObjMng = atWINOBJMNG_GetPtr(hWinObjMng);
	if( lpWinObjMng == atNULL ) return 0;
	
	return atUIAPI_GetListCount(lpWinObjMng->lpWinObjList);
}


/** 
 @brief WinObjMng에 등록된 WinObj의 리스트를 얻는다. (내부용)
 
 WinObjMng에 등록된 WinObj의 리스트를 얻는다.

 @param hWinObjMng [in] : WinObjMng 핸들
 @return WinObjMng에 등록된 WinObj의 리스트
 @remark 내부용
*/
atUILIST atWINOBJMNG_GetWinObjList( atHWINOBJMNG hWinObjMng )
{
	atLPWINOBJMNG lpWinObjMng = atWINOBJMNG_GetPtr(hWinObjMng);
	if( lpWinObjMng == atNULL ) return atNULL;
	
	return lpWinObjMng->lpWinObjList;
}


/** 
 @brief 특정 인덱스의 WinObj를 얻는다.(내부용)
 
 특정 인덱스의 WinObj를 얻는다.

 @param hWinObjMng [in] : WinObjMng 핸들
 @param nIndex [in] : WinObj Index
 @return WinObj 핸들
 @remark WinObj의 Index는 수시로 변하기 때문에 Unique하지 않다. 내부용으로 주로 사용된다.
*/
atHWINOBJ atWINOBJMNG_GetWinObjByIndex( atHWINOBJMNG hWinObjMng, atINT nIndex )
{
	atLPWINOBJMNG lpWinObjMng = atWINOBJMNG_GetPtr(hWinObjMng);
	if( lpWinObjMng == atNULL ) return atNULL;
	
	return (atHWINOBJ)atUIAPI_GetListAt(lpWinObjMng->lpWinObjList, nIndex);
}


/** 
 @brief WinObj의 리스트상 인덱스를 얻는다. (내부용)
 
 WinObj의 리스트상 인덱스를 얻는다.

 @param hWinObjMng [in] : WinObjMng 핸들
 @param hWinObj [in] : WinObj 핸들
 @return 없음
 @remark WinObj의 Index는 수시로 변하기 때문에 Unique하지 않다. 내부용으로 주로 사용된다.
*/
atINT atWINOBJMNG_GetWinObjIndex( atHWINOBJMNG hWinObjMng, atHWINOBJ hWinObj )
{
	atLPWINOBJMNG lpWinObjMng = atWINOBJMNG_GetPtr(hWinObjMng);
	int pos;
	atHWINOBJ wnd;

	if( lpWinObjMng == atNULL ) return -1;

	wnd = atUIAPI_GetListHead(lpWinObjMng->lpWinObjList, &pos);

	while( wnd != atNULL ){
		if( (atHWINOBJ)atUIAPI_GetListNext(lpWinObjMng->lpWinObjList, &pos) == hWinObj ){
			return pos;
		}
	}

	return -1;
}


/** 
 @brief WinObj를 등록
 
 WinObjMng에 WinObj를 등록한다.

 @param hWinObjMng [in] : WinObjMng 핸들
 @param hWinObj [in] : WinObj 핸들
 @return 성공시 atTRUE, 실패시 atFALSE
*/
atBOOL atWINOBJMNG_RegistWinObj( atHWINOBJMNG hWinObjMng, atHWINOBJ hWinObj )
{
	int isactive = atTRUE;
	atHWINOBJ hWinObj2;
	atLPWINOBJ lpWinObj, lpWinObj2;
	int idx, cnt;
	atLPWINOBJMNG lpWinObjMng = atWINOBJMNG_GetPtr(hWinObjMng);
	if( lpWinObjMng == atNULL ) return atFALSE;
	
	if( hWinObj == atNULL ) return atFALSE;
	lpWinObj = atWINOBJ_GetPtr(hWinObj);

	if( lpWinObj->dwAttr & atOBJATTR_STAYONTOP ){
    		atKERNEL_LockScreen();
		atUIAPI_AddListTail(lpWinObjMng->lpWinObjList, hWinObj);
		atKERNEL_UnlockScreen();
	} else {
		cnt = atUIAPI_GetListCount(lpWinObjMng->lpWinObjList);
		if( cnt > 0 ){
			hWinObj2 = atUIAPI_GetListTail(lpWinObjMng->lpWinObjList, &idx);
			while( hWinObj2 ){
				lpWinObj2 = atWINOBJ_GetPtr(hWinObj2);
				if( lpWinObj2 && !(lpWinObj2->dwAttr & atOBJATTR_STAYONTOP) ){
					break;
				}
				hWinObj2 = atUIAPI_GetListPrev(lpWinObjMng->lpWinObjList, &idx);
			}
		} else {
			idx = cnt;
		}
    		atKERNEL_LockScreen();
		if( idx >= cnt-1 )
			atUIAPI_AddListTail(lpWinObjMng->lpWinObjList, hWinObj);
		else
			atUIAPI_InsertListAfter(lpWinObjMng->lpWinObjList, idx, hWinObj);
		atKERNEL_UnlockScreen();
	}

	lpWinObj->hParentObj = lpWinObjMng->hParentWinObj;
	lpWinObj->bActivate = atTRUE;

	atWINOBJ_ProcessObjProc( hWinObj, OBJMSG_REGIST, (long)&isactive, 0 );
//	atWINOBJ_ProcessObjProc( hWinObj, OBJMSG_RESIZE, (long)&isactive, 0 );
	if( !isactive ){
		atWINOBJMNG_ReleaseWinObj( hWinObjMng, hWinObj );
		return atFALSE;
	}

	atWINOBJMNG_AdjustWinObjArea( hWinObjMng, hWinObj );

	if( lpWinObj->dwAttr & atOBJATTR_FOCUSABLE ){
		atWINOBJMNG_SetCurWinObj( hWinObjMng, hWinObj );
	}

	atWINOBJ_Refresh( hWinObj );

	return atTRUE;
}


/** 
 @brief 등록된 WinObj 해제
 
 WinObjMng에 등록된 WinObj를 해제한다.\n
 등록되었다가 해제된 윈도우는 자동으로 파괴된다. 

 @param hWinObjMng [in] : WinObjMng 핸들
 @param hWinObj [in] : WinObj 핸들
 @return 성공시 atTRUE, 실패시 atFALSE
*/
atBOOL atWINOBJMNG_ReleaseWinObj( atHWINOBJMNG hWinObjMng, atHWINOBJ hWinObj )
{
	int	pos;
	atBOOL iscur = atFALSE;
	atHWINOBJ hActObj, hParentObj;
	atLPWINOBJMNG lpWinObjMng = atWINOBJMNG_GetPtr(hWinObjMng);
	if( lpWinObjMng == atNULL ) return atFALSE;

	hParentObj = atWINOBJ_GetParentObj(hWinObj);
	hActObj = ( hParentObj != atNULL )? hParentObj : atWINOBJMNG_FindTopActiveWinObjEx( hWinObjMng, hWinObj );

	atWINOBJMNG_SetCurWinObj( hWinObjMng, hActObj );

	pos	= atUIAPI_FindListItemIdx(lpWinObjMng->lpWinObjList, hWinObj, 0);
	if( pos != -1 ){
    		atKERNEL_LockScreen();		
		atUIAPI_RemoveListAt(lpWinObjMng->lpWinObjList, pos, atFALSE);
		atKERNEL_UnlockScreen();
	}

	return atTRUE;
}   


/** 
 @brief WinObj에 포커스를 이동시킨다.
 
 WinObj에 포커스를 이동시킨다.

 @param hWinObjMng [in] : WinObjMng 핸들
 @param hWinObj [in] : WinObj 핸들
 @return 이전에 포커스를 가진 WinObj의 핸들. 없을 시, atNULL
*/
atHWINOBJ atWINOBJMNG_SetCurWinObj( atHWINOBJMNG hWinObjMng, atHWINOBJ hWinObj )
{
	int	pos, nNewOrder;
	atLPWINOBJ lpWinObj;
	atBOOL bChange;
	atLPWINOBJMNG lpWinObjMng = atWINOBJMNG_GetPtr(hWinObjMng);
	if( lpWinObjMng == atNULL ) return atNULL;

	if( lpWinObjMng->hCurWinObj == hWinObj ) return hWinObj;

	nNewOrder = -1;
	bChange = atTRUE;

	if( hWinObj != atNULL ){
		pos	= atUIAPI_FindListItemIdx(lpWinObjMng->lpWinObjList, hWinObj, 0);
		if( pos != -1 ){
			lpWinObj = atWINOBJ_GetPtr(hWinObj);
			if( lpWinObj == atNULL ) return lpWinObjMng->hCurWinObj;

			if( lpWinObj->bShow && lpWinObj->bEnable && (lpWinObj->dwAttr & atOBJATTR_FOCUSABLE) ){
				nNewOrder = pos;
			} else {
				bChange = atFALSE;
			}
		} else {
			bChange = atFALSE;
		}
	}

	if( bChange ){
		if( lpWinObjMng->hCurWinObj ){ 
			atWINOBJ_ProcessObjProc( lpWinObjMng->hCurWinObj, OBJMSG_LOSTFOCUS, 0, 0 );
			//atWINOBJ_SetFocus( lpWinObjMng->hCurWinObj, atFALSE );
			atWINOBJ_Refresh( lpWinObjMng->hCurWinObj );
		}

		atBOOL bFocus = atFALSE;
		if( hWinObj != atNULL){
			atWINOBJ_ProcessObjProc( hWinObj, OBJMSG_GOTFOCUS, 0, 0);
			//bFocus = atWINOBJ_SetFocus( hWinObj, atTRUE );
			bFocus = atWINOBJ_IsFocusable( hWinObj );
			if( bFocus ){
				atWINOBJ_Refresh( hWinObj );
				lpWinObjMng->hCurWinObj = hWinObj;
				lpWinObjMng->nCurWinObjOrder = nNewOrder;
			}
		}
		
		if(!bFocus){
			lpWinObjMng->hCurWinObj = atNULL;
			lpWinObjMng->nCurWinObjOrder = -1;
		}
	}

//  Recusive가 일어날 가능성이 커서 일단 제거...
//	atWINOBJMNG_SendMessageToParentWinObj( hWinObjMng, OBJMSG_CHANGECHILDOBJFOCUS, (long)hWinObj, (long)hOldWinObj );

	return hWinObj;
}


/** 
 @brief 현재 포커스를 가진 WinObj를 얻는다.
 
 현재 포커스를 가진 WinObj를 얻는다.

 @param hWinObjMng [in] : WinObjMng 핸들
 @return 포커스를 가진 WinObj의 핸들. 없을 시, atNULL
*/
atHWINOBJ atWINOBJMNG_GetCurWinObj( atHWINOBJMNG hWinObjMng )
{
	atLPWINOBJMNG lpWinObjMng = atWINOBJMNG_GetPtr(hWinObjMng);
	if( lpWinObjMng == atNULL ) return atNULL;

	return lpWinObjMng->hCurWinObj;
}


/** 
 @brief 현재 포커스를 가진 WinObj의 인덱스를 얻는다. (내부용)
 
 현재 포커스를 가진 WinObj의 인덱스를 얻는다.

 @param hWinObjMng [in] : WinObjMng 핸들
 @return 현재 포커스를 가진 WinObj의 인덱스
 @remark WinObj의 Index는 수시로 변하기 때문에 Unique하지 않다. 내부용으로 주로 사용된다.
*/
atINT atWINOBJMNG_GetCurWinObjOrder( atHWINOBJMNG hWinObjMng )
{
	atLPWINOBJMNG lpWinObjMng = atWINOBJMNG_GetPtr(hWinObjMng);
	if( lpWinObjMng == atNULL ) return -1;

	return lpWinObjMng->nCurWinObjOrder;
}


/** 
 @brief WinObj를 가장 앞으로 옮겨 온다.
 
 WinObj를 가장 앞으로 옮겨 온다.

 @param hWinObjMng [in] : WinObjMng 핸들
 @param hWinObj [in] : WinObj 핸들
 @return 실제로 가장 전면으로 옮겨진 윈도우의 핸들
 @remark StayOnTop 속성에 의해 가장 앞쪽으로 옮기지 못할 수도 있다.
*/
atHWINOBJ	atWINOBJMNG_SetTopWinObj( atHWINOBJMNG hWinObjMng, atHWINOBJ hWinObj )
{
	int	pos, cnt, idx;
	atHWINOBJ hWinObj2;
	atLPWINOBJ lpWinObj, lpWinObj2;
	atLPWINOBJMNG lpWinObjMng = atWINOBJMNG_GetPtr(hWinObjMng);
	if( lpWinObjMng == atNULL ) return atNULL;

	if( hWinObj == atNULL ) return atNULL;
	if( lpWinObjMng->hCurWinObj == hWinObj ) return hWinObj;

	cnt = atUIAPI_GetListCount(lpWinObjMng->lpWinObjList);
	if( cnt <= 1 ) return hWinObj;
	
	pos	= atUIAPI_FindListItemIdx(lpWinObjMng->lpWinObjList, hWinObj, 0);
	if( pos != -1 ){
		lpWinObj = atWINOBJ_GetPtr(hWinObj);

		if( lpWinObj->dwAttr & atOBJATTR_STAYONTOP ){
			// 이미 탑 윈도우므로... 무시...
		} else {
			cnt = atUIAPI_GetListCount(lpWinObjMng->lpWinObjList);
			if( cnt > 0 ){
				hWinObj2 = atUIAPI_GetListTail(lpWinObjMng->lpWinObjList, &idx);
				while( hWinObj2 ){
					lpWinObj2 = atWINOBJ_GetPtr(hWinObj2);
					if( lpWinObj2 && !(lpWinObj2->dwAttr & atOBJATTR_STAYONTOP) ){
						break;
					}
					hWinObj2 = atUIAPI_GetListPrev(lpWinObjMng->lpWinObjList, &idx);
				}
			} else {
				idx = cnt;
			}

			if( idx != pos ){
				atUIAPI_RemoveListAt(lpWinObjMng->lpWinObjList, pos, atFALSE);		
				if( idx >= cnt-1 )
					atUIAPI_AddListTail(lpWinObjMng->lpWinObjList, hWinObj);
				else
					atUIAPI_InsertListAfter(lpWinObjMng->lpWinObjList, idx, hWinObj);
			}
		}

		if( lpWinObj->bShow ){
			atWINOBJ_Refresh( hWinObj );
		}
	} else {
		hWinObj = lpWinObjMng->hCurWinObj;
	}

	return hWinObj;
}


/** 
 @brief WinObj를 가장 뒷쪽으로 옮긴다.
 
 WinObj를 가장 뒷쪽으로 옮긴다.

 @param hWinObjMng [in] : WinObjMng 핸들
 @param hWinObj [in] : WinObj 핸들
 @return 실제로 가장 뒷쪽에 옮겨진 윈도우의 핸들
*/
atHWINOBJ	atWINOBJMNG_SetBottomWinObj( atHWINOBJMNG hWinObjMng, atHWINOBJ hWinObj )
{
	int	pos, cnt;
	atLPWINOBJ lpWinObj;
	atLPWINOBJMNG lpWinObjMng = atWINOBJMNG_GetPtr(hWinObjMng);
	if( lpWinObjMng == atNULL ) return atNULL;

	if( hWinObj == atNULL ) return atNULL;
	if( lpWinObjMng->hCurWinObj == hWinObj ) return hWinObj;

	cnt = atUIAPI_GetListCount(lpWinObjMng->lpWinObjList);
	if( cnt <= 1 ) return hWinObj;
	
	pos	= atUIAPI_FindListItemIdx(lpWinObjMng->lpWinObjList, hWinObj, 0);
	if( pos != -1 ){
		lpWinObj = atWINOBJ_GetPtr(hWinObj);

		if( lpWinObj->bShow && lpWinObj->bEnable ){
			if( hWinObj == lpWinObjMng->hCurWinObj ){ 
				atWINOBJMNG_SetCurWinObj( hWinObjMng, atNULL );
			}
		}

		atUIAPI_RemoveListAt(lpWinObjMng->lpWinObjList, pos, atFALSE);
		atUIAPI_AddListHead(lpWinObjMng->lpWinObjList, hWinObj);

		hWinObj = atWINOBJMNG_FindTopActiveWinObj(hWinObjMng);
		if( hWinObj == atNULL ) return atNULL;
		
		lpWinObj = atWINOBJ_GetPtr(hWinObj);

		if( lpWinObj->bShow && lpWinObj->bEnable ){
			lpWinObjMng->hCurWinObj = hWinObj;
			lpWinObjMng->nCurWinObjOrder = 0;
			atWINOBJ_ProcessObjProc( hWinObj, OBJMSG_GOTFOCUS, 0, 0);
		}

		if( lpWinObj->bShow ){
//			atWINOBJMNG_RedrawAllWinObj( hWinObjMng, atTRUE );
			atWINOBJ_Refresh( hWinObj );
		}
	} else {
		hWinObj = lpWinObjMng->hCurWinObj;
	}

	return hWinObj;
}


/** 
 @brief 가장 앞쪽의 활성화 가능한 WinObj를 얻는다. (내부용)
 
 가장 앞쪽의 활성화 가능한 WinObj를 얻는다.\n
 주로 개체들이 생성 소멸 과정에서 다음번 포커스를 가질 후보를 찾을 때 쓴다.

 @param hWinObjMng [in] : WinObjMng 핸들
 @return 활성화 가능한 WinObj의 핸들
*/
atHWINOBJ	atWINOBJMNG_FindTopActiveWinObj( atHWINOBJMNG hWinObjMng )
{
	return atWINOBJMNG_FindTopActiveWinObjEx( hWinObjMng, atNULL );
}


/** 
 @brief 가장 앞쪽의 활성화 가능한 WinObj를 얻는다. (내부용)
 
 가장 앞쪽의 활성화 가능한 WinObj를 얻는다.\n
 주로 개체들이 생성 소멸 과정에서 다음번 포커스를 가질 후보를 찾을 때 쓴다.

 @param hWinObjMng [in] : WinObjMng 핸들
 @param hExcludeObj [in] : 제외할 WinObj 핸들 
 @return 활성화 가능한 WinObj의 핸들
*/
atHWINOBJ	atWINOBJMNG_FindTopActiveWinObjEx( atHWINOBJMNG hWinObjMng, atHWINOBJ hExcludeObj )
{
	atHWINOBJ hWinObj;
	atLPWINOBJ	lpWinObj;
	atHWINOBJ saveWnd;
	int	pos;
	atLPWINOBJMNG lpWinObjMng = atWINOBJMNG_GetPtr(hWinObjMng);
	if( lpWinObjMng == atNULL ) return atNULL;

	saveWnd = atNULL;

	hWinObj = (atHWINOBJ)atUIAPI_GetListTail(lpWinObjMng->lpWinObjList, &pos);
	while ( pos != -1 ){
		if ( (hWinObj != atNULL) && (hWinObj != hExcludeObj) ){
			lpWinObj = atWINOBJ_GetPtr(hWinObj);
			if ( lpWinObj && lpWinObj->bShow && lpWinObj->bEnable && (lpWinObj->dwAttr & atOBJATTR_FOCUSABLE) ){
				saveWnd = hWinObj;
				break; 
			}
		}
		hWinObj = (atHWINOBJ)atUIAPI_GetListPrev(lpWinObjMng->lpWinObjList, &pos);
	}

	return saveWnd;
}


/** 
 @brief 해당 ID의 윈도우를 찾는다.
 
 해당 ID의 윈도우를 찾는다.

 @param hWinObjMng [in] : WinObjMng 핸들
 @param nWinObjID [in] : WinObj의 ID
 @return 찾은 WinObj의 핸들. 없을 시 atNULL
 @remark ID는 중복이 허용되기 때문에 동일 ID의 윈도우가 2개 이상일 시, 첫번째 것만 찾을 수 있다.
*/
atHWINOBJ	atWINOBJMNG_FindWinObjByID( atHWINOBJMNG hWinObjMng, atINT nWinObjID )
{
	atHWINOBJ hWinObj;
	atLPWINOBJ	lpWinObj;
	atHWINOBJ saveWnd;
	int	pos;
	atLPWINOBJMNG lpWinObjMng = atWINOBJMNG_GetPtr(hWinObjMng);
	if( lpWinObjMng == atNULL ) return atNULL;

	saveWnd = atNULL;

	hWinObj = (atHWINOBJ)atUIAPI_GetListHead(lpWinObjMng->lpWinObjList, &pos);
	while ( pos != -1 ){
		if ( hWinObj != atNULL ){
			lpWinObj = atWINOBJ_GetPtr(hWinObj);
			if ( lpWinObj->nID == nWinObjID ){
				saveWnd = hWinObj;
				break; 
			}
		}
		hWinObj = (atHWINOBJ)atUIAPI_GetListNext(lpWinObjMng->lpWinObjList, &pos);
	}

	return saveWnd;
}


/** 
 @brief 해당 이름의 윈도우를 찾는다.
 
 해당 ID의 윈도우를 찾는다.

 @param hWinObjMng [in] : WinObjMng 핸들
 @param szWinObjText [in] : WinObj의 이름
 @return 찾은 WinObj의 핸들. 없을 시 atNULL
*/
atHWINOBJ	atWINOBJMNG_FindWinObjByName( atHWINOBJMNG hWinObjMng, atLPTSTR szWinObjText )
{
	atHWINOBJ hWinObj;
	atLPWINOBJ	lpWinObj;
	atHWINOBJ saveWnd;
	int	pos;
	atLPWINOBJMNG lpWinObjMng = atWINOBJMNG_GetPtr(hWinObjMng);
	if( lpWinObjMng == atNULL ) return atNULL;

	saveWnd = atNULL;

	hWinObj = (atHWINOBJ)atUIAPI_GetListHead(lpWinObjMng->lpWinObjList, &pos);
	while ( pos != -1 ){
		if ( hWinObj != atNULL ){
			lpWinObj = atWINOBJ_GetPtr(hWinObj);
			if ( UI_STRCMP(lpWinObj->szObjName, szWinObjText) == 0 ){
				saveWnd = hWinObj;
				break; 
			}
		}
		hWinObj = (atHWINOBJ)atUIAPI_GetListNext(lpWinObjMng->lpWinObjList, &pos);
	}

	return saveWnd;
}


/** 
 @brief 화면상의 해당 지점의 최상단 윈도우를 찾는다.
 
 화면상의 해당 지점의 최상단 윈도우를 찾는다.

 @param hWinObjMng [in] : WinObjMng 핸들
 @param nX, nY [in] : 기준이 되는 화면상의 위치
 @return 찾은 WinObj의 핸들. 없을 시 atNULL
*/
atHWINOBJ atWINOBJMNG_FindWinObjAt( atHWINOBJMNG hWinObjMng, int x, int y )
{
	atLPWINOBJMNG lpWinObjMng = atWINOBJMNG_GetPtr(hWinObjMng);
	atUILIST lst;
	int		pos;
	atHWINOBJ hWinObj;
	atLPWINOBJ lpWinObj;

	if( lpWinObjMng == atNULL ) return atNULL;
	
	lst = lpWinObjMng->lpWinObjList;
	if( lst == atNULL ) return atNULL;

	hWinObj = (atHWINOBJ)atUIAPI_GetListTail(lst, &pos);
	while ( hWinObj != atNULL ){
		lpWinObj = atWINOBJ_GetPtr(hWinObj);
		if ( lpWinObj && lpWinObj->bShow ){
			if( x >= lpWinObj->rect.nStartX && x <= lpWinObj->rect.nEndX &&
				y >= lpWinObj->rect.nStartY && y <= lpWinObj->rect.nEndY ){
				return hWinObj;
			}
		}

		hWinObj	= (atHWINOBJ)atUIAPI_GetListPrev(lst, &pos);
	}
	
	return atNULL;
}


/** 
 @brief WinObjMng의 Draw를 멈추거나 재개한다.
 
 WinObjMng의 Draw를 멈추거나 재개한다.

 @param hWinObjMng [in] : WinObjMng 핸들
 @param IsDraw [in] : Draw 동작 여부
 @return 없음
*/
void atWINOBJMNG_SetDrawFlag( atHWINOBJMNG hWinObjMng,  atBOOL IsDraw )
{
	atLPWINOBJMNG lpWinObjMng = atWINOBJMNG_GetPtr(hWinObjMng);
	if( lpWinObjMng == atNULL ) return;
	
	lpWinObjMng->bIsDraw = IsDraw;
}


/** 
 @brief WinObj를 Show/Hide 시킨다.
 
 WinObj를 Show/Hide 시킨다.

 @param hWinObjMng [in] : WinObjMng 핸들
 @param hWinObj [in] : WinObj 핸들
 @param bShow [in] : Show/Hide
 @return 없음
*/
void atWINOBJMNG_ShowWinObj( atHWINOBJMNG hWinObjMng, atHWINOBJ hWinObj, atBOOL bShow )
{
	atLPWINOBJ lpWinObj;
	atHWINOBJ hActObj, hParentObj;	
	atBOOL iscur = atFALSE;
	atLPWINOBJMNG lpWinObjMng = atWINOBJMNG_GetPtr(hWinObjMng);
	if( lpWinObjMng == atNULL ) return;

//	if( atWINOBJ_IsVisible(hWinObj) == bShow ) return;
	lpWinObj = atWINOBJ_GetPtr(hWinObj);
	if( lpWinObj->bShow == bShow )  return;
	
	atWINOBJMNG_AdjustAllWinObjArea( hWinObjMng );
	atWINOBJ_SetVisible( hWinObj, bShow );	

	if( !lpWinObjMng->bActivate ) return;

	hActObj = atWINOBJMNG_FindTopActiveWinObj( hWinObjMng ); 

	if( bShow ){
		if( hActObj == hWinObj ){
			atWINOBJMNG_SetCurWinObj( hWinObjMng, hWinObj );			
		} else {
			atWINOBJ_Refresh( hWinObj );
		}
	} else {
		if( hWinObj == lpWinObjMng->hCurWinObj ){
			hParentObj = atWINOBJ_GetParentObj(hWinObj);
			atWINOBJMNG_SetCurWinObj( hWinObjMng, ( hParentObj )? hParentObj : hActObj );			
		} else {
			atWINOBJ_Refresh( hWinObj );
		}
	}
}


static int _RedrawWinObjList( /*atLPWINOBJMNG lpWinObjMng, */ atHWINOBJ lstWinObj[], int nWinCnt, atBOOL IsAnnDraw, atBOOL IsSoftKeyDraw, atHWINOBJ hFocusWinObj )
{
	int i;
	atHWINOBJ hWinObj;
//	int pos;
	int draw_cnt = 0;
	//atRECT parent_rect, rect;
	
//	if( !lpWinObjMng->bIsDraw ) return 0;

	// 가장 바닥에 있는 윈도우부터 차례로 그려 올라온다.
	for( i = nWinCnt-1; i >=0; i-- ){
		hWinObj = lstWinObj[i];
		if(hWinObj == atNULL) {
			atDESKTOP_DrawDesktopBackground();
		} else {
			if ( atWINOBJ_IsVisible(hWinObj) ){
				atWINOBJ_Draw(hWinObj, hWinObj == hFocusWinObj);
			}
		}
	}

	if( IsAnnDraw )	
		atANNBAR_DrawAnnBar();
	
	if( IsSoftKeyDraw )	
		atSOFTKEY_DrawSoftKey();

	return draw_cnt;
}


static int _GetNeedDrawWinObj( atLPWINOBJMNG lpWinObjMng, atHWINOBJ *lstWinObj, int MaxWinCount, atRECT *parent_rect, int base_pos, atBOOL *pbIsFull )
{
	atHWINOBJ hWinObj;
	int pos, draw_cnt = 0;
	atRECT invalide_rect, rect;
	atRECT clip;
//	int cnt;
	
	if( !lpWinObjMng->bIsDraw ) return 0;
	if( base_pos < 0 ) return 0;

/*
	// 유효한 화면 범위를 구한다.
	if( lpWinObjMng->hParentWinObj != atNULL ){
		atWINOBJ_GetClientRect( lpWinObjMng->hParentWinObj, &parent_rect );
	} else {
		parent_rect.nStartX = 0;
		parent_rect.nEndX = atUIAPI_GetScreenWidth() - 1;
		parent_rect.nStartY = 0;
		parent_rect.nEndY = atUIAPI_GetScreenHeight() - 1;
	}
*/

	clip = *atUIAPI_GetScreenClipRect();
//	atUIAPI_SetScreenClipRect2( &invalide_rect );

	// 0, 0 베이스로 이동
	atUIAPI_MoveRect( &invalide_rect, parent_rect, -parent_rect->nStartX, -parent_rect->nStartY );

	// 전체 화면인 윈도우를 찾는다.
	hWinObj = (atHWINOBJ)atUIAPI_GetListTail(lpWinObjMng->lpWinObjList, &pos);

      if( pbIsFull ) *pbIsFull = atFALSE;
	  
	// 가장 바닥에 해당하는 윈도우를 찾는다.
	while( hWinObj != atNULL ){
		if( atWINOBJ_IsVisible(hWinObj) ){
			lstWinObj[draw_cnt] = hWinObj;
			draw_cnt ++;

			// 화면 전체를 차지한 윈도우가 있으면 이후엔 그리지 않는다.
			atWINOBJ_GetRect( hWinObj, &rect );
			if( atUIAPI_IsEmbededRect(&invalide_rect, &rect) ){
			  if( !(atWINOBJ_GetAttr(hWinObj) & atOBJATTR_TRANSPARENT) )
			    if( pbIsFull ) *pbIsFull = atTRUE;
			    break;
			}
		}

		if( pos <= base_pos ) break;

		hWinObj = (atHWINOBJ)atUIAPI_GetListPrev(lpWinObjMng->lpWinObjList, &pos);
	}

	return draw_cnt;
}


static int _RedrawWinObj( atLPWINOBJMNG lpWinObjMng, atRECT *parent_rect, int base_pos, atBOOL *pbIsFull, atHWINOBJ hFocusedObjs )
{
//	atHWINOBJ hWinObj;
	int draw_cnt = 0;
//	atRECT rect;
//	atRECT clip;
	atHWINOBJ lstWinObj[atWINOBJ_MAX_WINOBJ_CNT];
//	int cnt;

	draw_cnt = _GetNeedDrawWinObj( lpWinObjMng, lstWinObj, atWINOBJ_MAX_WINOBJ_CNT, parent_rect, base_pos, pbIsFull );

	if( draw_cnt <= 0 ) return 0;

/*
	if( (pos <= 0) && (lpWinObjMng->hParentWinObj == atNULL) ){
		if( hWinObj != atNULL ){
			atWINOBJ_GetRect( hWinObj, &rect );
			// 가장 밑 바닥 윈도우가 전체 화면이 아니라면 배경을 그린다.
			if( !atUIAPI_IsEmbededRect(&invalide_rect, &rect) ){
				atDESKTOP_DrawDesktop(); // 일단 임시로 제거.
			}
			hWinObj = (atHWINOBJ)atUIAPI_GetListHead(lpWinObjMng->lpWinObjList, &pos);
		} else {
			atDESKTOP_DrawDesktop();
		}
	}
*/

	// 가장 바닥에 있는 윈도우부터 차례로 그려 올라온다.
	_RedrawWinObjList( /*lpWinObjMng, */ lstWinObj, draw_cnt, atFALSE, atFALSE, hFocusedObjs );
	
//	atUIAPI_SetScreenClipRect2( &clip );

	return draw_cnt;
}


/** 
 @brief WinObj를 모두 다시 그린다.
 
 특정 베이스 윈도우를 기준으로 WinObjMng에 포함된 WinObj를 모두 다시 그린다.

 @param hWinObjMng [in] : WinObjMng 핸들
 @param hWinObjBase [in] : 기준이 되는 WinObj 핸들
 @return 그려진 윈도우의 갯수
*/
int atWINOBJMNG_RedrawAllWinObjEx( atHWINOBJMNG hWinObjMng, atHWINOBJ hWinObjBase, atBOOL bFocused )
{
	int base_pos, draw_cnt;
	atRECT parent_rect;
	atLPWINOBJMNG lpWinObjMng = atWINOBJMNG_GetPtr(hWinObjMng);
	if( lpWinObjMng == atNULL ) return -1;

	if( lpWinObjMng->hParentWinObj && !atWINOBJ_IsValidate(lpWinObjMng->hParentWinObj) ) return -1;
	if( !lpWinObjMng->bIsDraw ) return -1;
		
	// 유효한 화면 범위를 구한다.
	if( lpWinObjMng->hParentWinObj != atNULL ){
		atWINOBJ_GetClientRect( lpWinObjMng->hParentWinObj, &parent_rect );
	} else {
		parent_rect.nStartX = 0;
		parent_rect.nEndX = atUIAPI_GetScreenWidth() - 1;
		parent_rect.nStartY = 0;
		parent_rect.nEndY = atUIAPI_GetScreenHeight() - 1;
	}

	base_pos = atWINOBJMNG_GetWinObjIndex( hWinObjMng, hWinObjBase );

	draw_cnt = 0;
	if( base_pos >= 0 ){		
		atHWINOBJ hFocusedObj = (bFocused)? atWINOBJMNG_GetCurWinObj( hWinObjMng ) : atNULL;
		draw_cnt = _RedrawWinObj( lpWinObjMng, &parent_rect, base_pos, atNULL, hFocusedObj );
	}
	
	if( (lpWinObjMng->hParentWinObj == atNULL) && (draw_cnt <= 0) ){
		// 윈도우가 전혀 없는 상태.
		atDESKTOP_DrawDesktop();
	} else {
		atANNBAR_DrawAnnBar();
		atSOFTKEY_DrawSoftKey();
	}

//	if( bFlush ) atUIAPI_FlushScreen();

	return draw_cnt;
}


/** 
 @brief WinObj를 모두 다시 그린다.
 
 WinObjMng에 포함된 WinObj를 모두 다시 그린다.

 @param hWinObjMng [in] : WinObjMng 핸들
 @return 그려진 윈도우의 갯수
*/
int atWINOBJMNG_RedrawAllWinObj( atHWINOBJMNG hWinObjMng, atBOOL bFocused )
{
	int draw_cnt;
	atRECT parent_rect;
	atLPWINOBJMNG lpWinObjMng = atWINOBJMNG_GetPtr(hWinObjMng);
	if( lpWinObjMng == atNULL ) return -1;

	if( (lpWinObjMng->hParentWinObj == atNULL) && (atWINOBJMNG_GetWinObjCount(hWinObjMng)<= 0) ){
		// 윈도우가 전혀 없는 상태.
		atDESKTOP_DrawDesktop();
		return 0;
	} 
	
	if( lpWinObjMng->hParentWinObj && !atWINOBJ_IsValidate(lpWinObjMng->hParentWinObj) ) return -1;
	if( !lpWinObjMng->bIsDraw ) return -1;
		
	// 유효한 화면 범위를 구한다.
	if( lpWinObjMng->hParentWinObj != atNULL ){
		atWINOBJ_GetClientRect( lpWinObjMng->hParentWinObj, &parent_rect );
	} else {
		parent_rect.nStartX = 0;
		parent_rect.nEndX = atUIAPI_GetScreenWidth() - 1;
		parent_rect.nStartY = 0;
		parent_rect.nEndY = atUIAPI_GetScreenHeight() - 1;
	}

	{
	atHWINOBJ hFocusedObj = (bFocused)? atWINOBJMNG_GetCurWinObj( hWinObjMng ) : atNULL;
	draw_cnt = _RedrawWinObj( lpWinObjMng, &parent_rect, 0, atNULL, hFocusedObj );
	}

	if( lpWinObjMng->hParentWinObj == atNULL ){
		atANNBAR_DrawAnnBar();
		atSOFTKEY_DrawSoftKey();
	}

//	if( bFlush ) atUIAPI_FlushScreen();

	return draw_cnt;
}


/** 
 @brief 특정 영역에 걸친 WinObj를 모두 다시 그린다.
 
 특정 영역에 걸친 WinObj를 모두 다시 그린다.

 @param hWinObjMng [in] : WinObjMng 핸들
 @param rtWinRect [in] : 그릴 영역
 @return 그려진 윈도우의 갯수
 @todo 좀 더 효율적으로 처리하기 위해 넣은 함수가 상황에 따라 문제를 일으켜서, 일단 클리핑 영역은 전체를 기준으로 하였음.
*/
int atWINOBJMNG_RedrawAllWinObjArea( atHWINOBJMNG hWinObjMng, atRECT *rtWinRect, atBOOL bFocused )
{
	atHWINOBJ hWinObj;
	int pos, draw_cnt;
//	atRECT screen_rect;
	atBOOL is_first;
	atRECT invalid_rect, parent_rect, rect, max_rect;
//	atRECT clip;
	atREGION rgn;
	atHWINOBJ lstWinObj[atWINOBJ_MAX_WINOBJ_CNT];
	atLPWINOBJMNG lpWinObjMng = atWINOBJMNG_GetPtr(hWinObjMng);
	if( lpWinObjMng == atNULL ) return -1;

	if( (lpWinObjMng->hParentWinObj == atNULL) && (atWINOBJMNG_GetWinObjCount(hWinObjMng)<= 0) ){
		// 윈도우가 전혀 없는 상태.
		atDESKTOP_DrawDesktop();
		return 0;
	} 
	
	if( lpWinObjMng->hParentWinObj && !atWINOBJ_IsValidate(lpWinObjMng->hParentWinObj) ) return -1;
	if( !lpWinObjMng->bIsDraw ) return -1;

	// 갱신할 화면 범위와 부모 화면 범위 중, 유효한 범위만을 고른다.
//	if( !atUIAPI_IntersectRect( &invalid_rect, rtWinRect, &parent_rect ) ) return 0;
// 좀 더 효율적으로 처리하기 위해 넣은 함수가 상황에 따라 문제를 일으켜서, 일단 무조건 전체를 갱신하도록 하였음.
parent_rect = invalid_rect = *rtWinRect;
is_first = atTRUE;
SET_RECT( max_rect, 0, 0, 0, 0 );

	// 전체 화면인 윈도우를 찾는다.
	hWinObj = (atHWINOBJ)atUIAPI_GetListTail(lpWinObjMng->lpWinObjList, &pos);

	// 가장 바닥에 해당하는 윈도우를 찾는다.
	draw_cnt = 0;
	atDWORD lastattr = atOBJATTR_NONE;
	while( hWinObj != atNULL ){
		if( atWINOBJ_IsVisible(hWinObj) ){
			atWINOBJ_GetRegion(hWinObj, &rgn );
			rgn.nX = rgn.nY = 0;
			atWINOBJ_ConvScreenRegion( hWinObj, &rgn, &rgn );
			REGION2RECT( rect, rgn );
			if( atUIAPI_IsIntersectRect(&invalid_rect, &rect) ){
				if( is_first || !atUIAPI_IsEmbededRect(&rect, &max_rect) ){
					lstWinObj[draw_cnt++] = hWinObj;

					lastattr = atWINOBJ_GetAttr(hWinObj);
					if( !(lastattr & atOBJATTR_TRANSPARENT) ) {
						// 화면 전체를 차지한 윈도우가 있으면 이후엔 그리지 않는다.
						if( atUIAPI_IsEmbededRect(&invalid_rect, &rect) ){
							break;
						}
					}

					if( is_first && !(atWINOBJ_GetAttr(hWinObj) & atOBJATTR_TRANSPARENT) ){
						max_rect = rect; 
						is_first = atFALSE; 
					} else {
						atUIAPI_UnionRect( &max_rect, &max_rect, &rect );
					}
//					atUIAPI_ToEmbededRect( &max_rect, &invalid_rect );
				}
			}
		}

		hWinObj = (atHWINOBJ)atUIAPI_GetListPrev(lpWinObjMng->lpWinObjList, &pos);
	}

	// 가장 바닥의 윈도우가 투명했거나, 화면을 가득 채우지 않았으면 desktop 영역도 추가 (사실상 소프트키와 안테나 영역 때문에 항상 포함)
	if( (lastattr & atOBJATTR_TRANSPARENT) || !atUIAPI_IsEmbededRect(&invalid_rect, &max_rect) ){
		lstWinObj[draw_cnt++] = atNULL;
	}


	// 화면 클리핑 ScreenClipRect범위를 한정한다.
//	clip = *atUIAPI_GetScreenClipRect();
//	atUIAPI_MoveRect( &invalid_rect, &invalid_rect, parent_rect.nStartX, parent_rect.nStartY );
//	atUIAPI_SetScreenClipRect2( &invalid_rect ); // 화면 일부 갱신에 문제가 생겨 일단 막는다.

	// 부모 윈도우가 없는 최상위 윈도우 매니저일 경우엔
/*
	if( lpWinObjMng->hParentWinObj == atNULL ){
		if( hWinObj != atNULL ){
			atWINOBJ_GetRect( hWinObj, &rect );
			// 가장 밑 바닥 윈도우가 전체 화면이 아니라면 배경을 그린다.
			if( (rect.nStartX > parent_rect.nStartX) || (rect.nStartY > parent_rect.nStartY) && 
				(rect.nEndX < parent_rect.nEndX) || (rect.nEndY < parent_rect.nEndY) ){
				atDESKTOP_DrawDesktop(atFALSE);
			}
			hWinObj = (atHWINOBJ)atUIAPI_GetListHead(lpWinObjMng->lpWinObjList, &pos);
		} else {
			atDESKTOP_DrawDesktop(atFALSE);
		}
	}
*/

	// 가장 바닥에 있는 윈도우부터 차례로 그려 올라온다.
	if( draw_cnt > 0 ) {
		atHWINOBJ hFocusedObj = (bFocused)? atWINOBJMNG_GetCurWinObj( hWinObjMng ) : atNULL;
		_RedrawWinObjList( /*lpWinObjMng, */ lstWinObj, draw_cnt, atFALSE, atFALSE, hFocusedObj );
	}
	
	if( lpWinObjMng->hParentWinObj == atNULL ){
		atANNBAR_DrawAnnBar();
		atSOFTKEY_DrawSoftKey();
	}

//	atUIAPI_SetScreenClipRect2( &clip );
	
	return draw_cnt;
}


/** 
 @brief 지정한 WinObj의 다음 순서의 활성화 가능한 개체를 찾는다. (내부용)
 
 현재 포커스 가진 WinObj의 다음 순서의 활성화 가능한 개체를 찾는다.\n
 맨 끝까지 찾아도 없으면 처음으로 돌아가서 계속 찾기 시작한다.

 @param hWinObjMng [in] : WinObjMng 핸들
 @param hWinObj [in] : 시작 WinObj 핸들
 @return 찾은 WinObj 핸들. 못 찾았을 경우 atNULL
 @remark 키보드로 컨트롤의 포커스를 이동시킬 때 내부용으로 주로 씀.
*/
atHWINOBJ atWINOBJMNG_GetNextEnabledWinObj( atHWINOBJMNG hWinObjMng, atHWINOBJ hWinObj )
{
	atLPWINOBJMNG lpWinObjMng = atWINOBJMNG_GetPtr(hWinObjMng);
	atUILIST lst;
	int		pos, orgpos;
	atHWINOBJ hTempWinObj;
	atLPWINOBJ lpTempWinObj;

	if( lpWinObjMng == atNULL ) return atNULL;
	
	lst = lpWinObjMng->lpWinObjList;
	if( lst == atNULL ) return atNULL;

	orgpos = (hWinObj != atNULL)? atUIAPI_FindListItemIdx(lst, hWinObj, 0) : -1;

	pos = orgpos;
	hTempWinObj	= (atHWINOBJ)atUIAPI_GetListNext(lst, &pos);
	while ( hTempWinObj != atNULL ){
		lpTempWinObj = atWINOBJ_GetPtr(hTempWinObj);
		if ( lpTempWinObj && lpTempWinObj->bEnable && (lpTempWinObj->dwAttr & atOBJATTR_FOCUSABLE) && lpTempWinObj->bShow ){
			return hTempWinObj;
		}

		hTempWinObj	= (atHWINOBJ)atUIAPI_GetListNext(lst, &pos);
	}
	hTempWinObj	= (atHWINOBJ)atUIAPI_GetListHead(lst, &pos);
	while ( hTempWinObj != atNULL ){
		if ( hTempWinObj == hWinObj )
			return hWinObj;

		lpTempWinObj = atWINOBJ_GetPtr(hTempWinObj);
		if ( lpTempWinObj->bEnable && (lpTempWinObj->dwAttr & atOBJATTR_FOCUSABLE) && lpTempWinObj->bShow ){
			return hTempWinObj;
		}

		hTempWinObj	= (atHWINOBJ)atUIAPI_GetListNext(lst, &pos);
	}

	return atNULL;
}


/** 
 @brief 지정한 WinObj의 이전 순서의 활성화 가능한 개체를 찾는다. (내부용)
 
 현재 포커스 가진 WinObj의 이전 순서의 활성화 가능한 개체를 찾는다.\n
 처음까지 찾아도 없으면 맨 끝으로 돌아가서 계속 찾기 시작한다.

 @param hWinObjMng [in] : WinObjMng 핸들
 @param hWinObj [in] : 시작 WinObj 핸들
 @return 찾은 WinObj 핸들. 못 찾았을 경우 atNULL
 @remark 키보드로 컨트롤의 포커스를 이동시킬 때 내부용으로 주로 씀.
*/
atHWINOBJ atWINOBJMNG_GetPrevEnabledWinObj( atHWINOBJMNG hWinObjMng, atHWINOBJ hWinObj )
{
	atLPWINOBJMNG lpWinObjMng = atWINOBJMNG_GetPtr(hWinObjMng);
	atUILIST lst;
	int		pos, orgpos;
	atHWINOBJ hTempWinObj;
	atLPWINOBJ lpTempWinObj;

	if( lpWinObjMng == atNULL ) return atNULL;
	
	lst = lpWinObjMng->lpWinObjList;
	if( lst == atNULL ) return atNULL;

	orgpos = (hWinObj != atNULL)? atUIAPI_FindListItemIdx(lst, hWinObj, 0) : -1;

	pos = orgpos;
	hTempWinObj	= (atHWINOBJ)atUIAPI_GetListPrev(lst, &pos);
	while ( hTempWinObj != atNULL ){
		lpTempWinObj = atWINOBJ_GetPtr(hTempWinObj);
		if ( lpTempWinObj->bEnable && (lpTempWinObj->dwAttr & atOBJATTR_FOCUSABLE) && lpTempWinObj->bShow ){
			return hTempWinObj;
		}

		hTempWinObj	= (atHWINOBJ)atUIAPI_GetListPrev(lst, &pos);
	}
	hTempWinObj	= (atHWINOBJ)atUIAPI_GetListTail(lst, &pos);
	while ( hTempWinObj != atNULL ){
		if ( hTempWinObj == hWinObj )
			return hWinObj;

		lpTempWinObj = atWINOBJ_GetPtr(hTempWinObj);
		if ( lpTempWinObj->bEnable && (lpTempWinObj->dwAttr & atOBJATTR_FOCUSABLE) && lpTempWinObj->bShow ){
			return hTempWinObj;
		}

		hTempWinObj	= (atHWINOBJ)atUIAPI_GetListPrev(lst, &pos);
	}
	
	return atNULL;
}


/** 
 @brief WinObj를 Anchor를 적용하여 부모 WinObj에 맞게 조정된 크기를 얻는다.
 
 WinObj를 Anchor를 적용하여 부모 WinObj에 맞게 조정된 크기를 얻는다.

 @param hWinObjMng [in] : WinObjMng 핸들
 @param hWinObj [in] : WinObj 핸들
 @param pRect [out] : 재조정된 영역 정보
 @return 성공시 atTRUE, 실패시 atFALSE
*/
atBOOL atWINOBJMNG_GetAnchorAdjRect( atHWINOBJMNG hWinObjMng, atHWINOBJ hWinObj, atRECT *pRect )
{
	atLPWINOBJ		lpWinObj = atWINOBJ_GetPtr(hWinObj);
	atHWINOBJ		hParentWnd;
	atRECT			rect, parentrect;
	atMARGIN		*anchor;

	if( lpWinObj == atNULL ){
		SET_RECT( *pRect, 0, 0, 0, 0 );
		return atFALSE;
	}

	if( !(lpWinObj->dwAttr & atOBJATTR_USEANCHOR) ) return atFALSE;
	
//	if( (lpWinObj->rtAnchor.nLeft < 0) && (lpWinObj->rtAnchor.nTop < 0) && 
//	    (lpWinObj->rtAnchor.nRight < 0) && (lpWinObj->rtAnchor.nBottom < 0) ) return atFALSE;

	hParentWnd = atWINOBJ_GetParentObj(hWinObj);
	if( (lpWinObj->dwAttr & atOBJATTR_CHILD) && (hParentWnd != atNULL) ){
		atWINOBJ_GetClientRect( hParentWnd, &parentrect );
		atUIAPI_MoveRect( &parentrect, &parentrect, -parentrect.nStartX, -parentrect.nStartY );
	} else {
//		atLPWINOBJMNG lpWinObjMng = atWINOBJMNG_GetPtr(hWinObjMng);
//		if( lpWinObjMng->hParentWinObj != atNULL ){
//			atWINOBJ_GetClientRect( lpWinObjMng->hParentWinObj, &parentrect );
//		} else {
			atREGION rgn;
			atDESKTOP_GetDesktopArea( atDESKTOP_AREA_CLIENT, &rgn );
			REGION2RECT( parentrect, rgn );
//		}
	}
	
	atWINOBJ_GetRect( hWinObj, &rect );
	anchor = &lpWinObj->rtAnchor;

	if( (anchor->nLeft == atANCHOR_IGNORE) && (anchor->nRight == atANCHOR_IGNORE) ){
		pRect->nStartX = parentrect.nStartX + ((RECT_WIDTH(parentrect) - RECT_WIDTH(rect)) >> 1);
		pRect->nEndX = pRect->nStartX + RECT_WIDTH(rect) - 1;
	} else {
		pRect->nStartX = ( anchor->nLeft != atANCHOR_IGNORE )? parentrect.nStartX + anchor->nLeft : rect.nStartX;
		if( anchor->nRight != atANCHOR_IGNORE ){
			pRect->nEndX = parentrect.nEndX - anchor->nRight;
			if( anchor->nLeft  == atANCHOR_IGNORE ) pRect->nStartX = pRect->nEndX - RECT_WIDTH(rect) + 1;
		} else
			pRect->nEndX = pRect->nStartX + RECT_WIDTH(rect) - 1;
	}
	
	if( (anchor->nTop == atANCHOR_IGNORE) && (anchor->nBottom == atANCHOR_IGNORE) ){
		pRect->nStartY = parentrect.nStartY + ((RECT_HEIGHT(parentrect) - RECT_HEIGHT(rect)) >> 1);
		pRect->nEndY = pRect->nStartY + RECT_HEIGHT(rect) - 1;
	} else {
		pRect->nStartY = ( anchor->nTop != atANCHOR_IGNORE )? parentrect.nStartY + anchor->nTop : rect.nStartY;
		if( anchor->nBottom != atANCHOR_IGNORE ){
			pRect->nEndY = parentrect.nEndY - anchor->nBottom;
			if( anchor->nTop == atANCHOR_IGNORE ) pRect->nStartY = pRect->nEndY - RECT_HEIGHT(rect) + 1;
		} else
			pRect->nEndY = pRect->nStartY + RECT_HEIGHT(rect) - 1;
	}

	return (pRect->nStartX != rect.nStartX) || (pRect->nStartY != rect.nStartY) || 
		    (pRect->nEndX != rect.nEndX) || (pRect->nEndY != rect.nEndY);
}


/** 
 @brief WinObj에 Anchor가 적용된 크기를 구한다.
 
 WinObj에 Anchor가 적용된 크기를 구한다.

 @param hWinObjMng [in] : WinObjMng 핸들
 @param 
 @return 없음
*/
atBOOL atWINOBJMNG_GetAnchorAdjRegion( atHWINOBJMNG hWinObjMng, atHWINOBJ hWinObj, atREGION *pRegion )
{
	atRECT rect;

	REGION2RECT( rect, *pRegion );	
	
	if( atWINOBJMNG_GetAnchorAdjRect( hWinObjMng, hWinObj, &rect ) ){
		RECT2REGION( *pRegion, rect );
		return atTRUE;
	} else 
		return atFALSE;
}


/** 
 @brief 윈도우의 레이아웃을 재조정
 
 Anchor를 적용하여 윈도우의 레이아웃을 재조정 한다.

 @param hWinObjMng [in] : WinObjMng 핸들
 @param hWinObj [in] : WinObj 핸들
 @return 없음
*/
void atWINOBJMNG_AdjustWinObjArea( atHWINOBJMNG hWinObjMng, atHWINOBJ hWinObj )
{
	int r;
	atREGION rgn;
	
	atWINOBJ_GetRegion( hWinObj, &rgn );
	r = atWINOBJMNG_GetAnchorAdjRegion( hWinObjMng, hWinObj, &rgn );
	if( r ){
		atWINOBJ_SetRegion( hWinObj, rgn.nX, rgn.nY, rgn.nWidth, rgn.nHeight, atFALSE );
	}
}


/** 
 @brief 모든 윈도우의 레이아웃을 재조정
 
 모든 윈도우의 레이아웃을 Anchor를 적용하여 재조정 한다.

 @param hWinObjMng [in] : WinObjMng 핸들
 @return 없음
*/
void atWINOBJMNG_AdjustAllWinObjArea( atHWINOBJMNG hWinObjMng )
{
	atLPWINOBJMNG lpWinObjMng = atWINOBJMNG_GetPtr(hWinObjMng);
	atUILIST lst;
	int 	pos;
	atHWINOBJ hWinObj;

	if( lpWinObjMng == atNULL ) return;

	lst = lpWinObjMng->lpWinObjList;
	if( lst == atNULL ) return;
	
	hWinObj = atUIAPI_GetListHead(lst, &pos);

	while ( hWinObj != atNULL ){
		atWINOBJMNG_AdjustWinObjArea( hWinObjMng, hWinObj );

		hWinObj = (atHWINOBJ)atUIAPI_GetListNext(lst, &pos);
	}
}


/** 
 @brief 윈도우의 영역을 설정
 
 윈도우의 영역을 설정한다.

 @param hWinObjMng [in] : WinObjMng 핸들
 @param hWinObj [in] : WinObj 핸들
 @param nX, nY [in] : 윈도우의 위치
 @param nWidth, nHeight [in] : 윈도우의 크기
 @return 없음
*/
atVOID atWINOBJMNG_SetWinObjRegion( atHWINOBJMNG hWinObjMng, atHWINOBJ hWinObj, atINT nX, atINT nY, atINT nWidth, atINT nHeight )
{
	atLPWINOBJ		lpWinObj = atWINOBJ_GetPtr(hWinObj);
	
	lpWinObj->rect.nStartX = nX;
	lpWinObj->rect.nStartY = nY;
	lpWinObj->rect.nEndX = nX + nWidth - 1;
	lpWinObj->rect.nEndY = nY + nHeight - 1;
	
	atWINOBJMNG_AdjustWinObjArea( hWinObjMng, hWinObj );
}


/** 
 @brief WinObj에 Message를 보낸다.
 
 WinObj에 Message를 보낸다.

 @param hWinObjMng [in] : WinObjMng 핸들
 @param nMsg [in] : Message
 @param lParam1, lParam2 [in] : 파라미터
 @return 윈도우 핸들러 호출 결과값
*/atVOID atWINOBJMNG_SendMessageAllWinObjs( atHWINOBJMNG hWinObjMng, int nMsg, long lParam1, long lParam2 )
{
	int 	pos;
	atUILIST lst;
	atHWINOBJ hWinObj;

	lst = atWINOBJMNG_GetWinObjList(hWinObjMng);
	if( lst == atNULL ) return;

	hWinObj = atUIAPI_GetListHead(lst, &pos);

	while ( hWinObj != atNULL ){
		atWINOBJ_ProcessObjProc( hWinObj, nMsg, lParam1, lParam2);
		hWinObj = (atHWINOBJ)atUIAPI_GetListNext(lst, &pos);
	}
}


/** 
 @brief WinObj에 Message를 보낸다.
 
 WinObj에 Message를 보낸다.

 @param hWinObjMng [in] : WinObjMng 핸들
 @param nMsg [in] : Message
 @param lParam1, lParam2 [in] : 파라미터
 @return 윈도우 핸들러 호출 결과값
*/
atINT atWINOBJMNG_SendMessageToParentWinObj( atHWINOBJMNG hWinObjMng, int nMsg, long lParam1, long lParam2 )
{
	atLPWINOBJMNG lpWinObjMng = atWINOBJMNG_GetPtr(hWinObjMng);
	if( lpWinObjMng == atNULL ) return -1;
	if( lpWinObjMng->hParentWinObj == atNULL ) return 0;

	return atWINOBJ_ProcessObjProc( lpWinObjMng->hParentWinObj, nMsg, lParam1, lParam2 );		
}

