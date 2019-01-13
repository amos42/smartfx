/**
  @file WinObj2.c
  @brief ChildObj 관한 API
  @remark ChildObj에 관한 API 모음
  @author Amos
  @date 2010-04-19 오전 11:41:00
  @see WObjMng.h ChildObj.h
*/
#include "AT_Types.h"
#include "UI_MNG.h"
#include "WinObj.h"


/** 
 @brief ChildObj를 파괴
 
 ChildObj를 파괴한다.

 @param hWinObj [in] : ChildObjMng 핸들
 @param hChildObj [in] : 파괴할 ChildObj 핸들
 @return 없음
*/
void atWINOBJ_DestroyChildObj( atHWINOBJ hWinObj, atHWINOBJ hChildObj )
{
	atLPWINOBJ lpWinObj = atWINOBJ_GetPtr(hWinObj);
	atHWINOBJ hChildObj2;
	int pos;
	
	atWINOBJ_ReleaseChildObj( hWinObj, hChildObj );
	atWINOBJ_ProcessObjProc( hChildObj, OBJMSG_REQ_DESTROY, 0, 0 );

	hChildObj2 = (atHWINOBJ)atUIAPI_GetListTail(lpWinObj->lpChildObjList, &pos);
	while ( hChildObj2 != atNULL){
		if( atWINOBJ_GetParentObj(hChildObj2) == hChildObj ){
			atWINOBJ_DestroyChildObj( hWinObj, hChildObj2 );
		}
		hChildObj2 = (atHWINOBJ)atUIAPI_GetListPrev(lpWinObj->lpChildObjList, &pos);
	}
}


/** 
 @brief 모든 ChildObj를 파괴
 
 현재 ChildObjMng에 등록된 모든 ChildObj를 파괴한다.

 @param hWinObj [in] : ChildObjMng 핸들
 @return 없음
*/
void atWINOBJ_DestroyAllChildObjs( atHWINOBJ hWinObj )
{
	atHWINOBJ	hChildObj;
	atLPWINOBJ lpWinObj = atWINOBJ_GetPtr(hWinObj);
	if( lpWinObj == atNULL ) return;

	atWINOBJ_SetDrawFlag( hWinObj, atFALSE );
	
	hChildObj = (atHWINOBJ)atUIAPI_GetListTail(lpWinObj->lpChildObjList, atNULL);
	while ( hChildObj != atNULL){
		if( hChildObj == lpWinObj->hCurChildObj ){
//			atWINOBJ_ProcessObjProc( hChildObj, OBJMSG_LOSTFOCUS, 0, 0);
			//atWINOBJ_SetFocus( hChildObj, atFALSE );
			lpWinObj->hCurChildObj = atNULL;
			lpWinObj->nCurChildObjOrder = -1;
		}

		atWINOBJ_DestroyChildObj( hWinObj, hChildObj );

		hChildObj = (atHWINOBJ)atUIAPI_GetListTail(lpWinObj->lpChildObjList, atNULL);
	}

	lpWinObj->hCurChildObj = atNULL;
	lpWinObj->nCurChildObjOrder = -1;
	
	if( lpWinObj->hParentObj == atNULL ){
		atSOFTKEY_SetSoftKeyText(_AT(""), _AT(""), _AT(""));
	}
	
	atWINOBJ_SetDrawFlag( hWinObj, atTRUE );	
}


/** 
 @brief 모든 ChildObj를 파괴 (조건 판단 핸들러)
 
 현재 ChildObjMng에 등록된 모든 ChildObj중, 조건에 맞는 윈도우만 골라서 파괴한다.\n
 이때, 파괴할 조건 판단 핸들러를 파라미터로 전달한다.

 @param hWinObj [in] : ChildObjMng 핸들
 @param fnIsDel [in] : 조건 판단 핸들러.
 @return 없음
*/
void atWINOBJ_DestroyChildObjsEx( atHWINOBJ hWinObj, atBOOL (*fnIsDel)(atHWINOBJ) )
{
	atHWINOBJ	hChildObj, hChildObj2;
	int pos;
	atLPWINOBJ lpWinObj = atWINOBJ_GetPtr(hWinObj);
	if( lpWinObj == atNULL ) return;
	

	if( fnIsDel == atNULL ){
		atWINOBJ_DestroyAllChildObjs(hWinObj);
		return;
	}
		
	atWINOBJ_SetDrawFlag( hWinObj, atFALSE );
	
	hChildObj = (atHWINOBJ)atUIAPI_GetListTail(lpWinObj->lpChildObjList, &pos);
	while( hChildObj != atNULL){
		if( fnIsDel(hChildObj) ){
			hChildObj2 = (atHWINOBJ)atUIAPI_GetListPrev(lpWinObj->lpChildObjList, &pos);
			atWINOBJ_ReleaseChildObj( hWinObj, hChildObj );
			atWINOBJ_Destroy(hChildObj);
			UI_MEMFREE( atWINOBJ_GetPtr(hChildObj) );
			hChildObj = hChildObj2;
		} else {
			hChildObj = (atHWINOBJ)atUIAPI_GetListPrev(lpWinObj->lpChildObjList, &pos);
		}
	}

	atWINOBJ_SetDrawFlag( hWinObj, atTRUE );	

//	g_hCurChildObj = atUIAPI_GetListHead(g_lpChildObjList);
//	g_hCurrEventChildObj = g_hCurChildObj;

	if( lpWinObj->bActivate )
		if( atUIAPI_GetListCount(lpWinObj->lpChildObjList) > 0 ) {
//			atWINOBJ_RedrawAllChildObj( hWinObj, atTRUE );
			atUIAPI_AddInvalideAreaAll();
		}
}


/** 
 @brief ChildObj의 갯수를 얻는다.
 
 현재 ChildObjMng에 등록된 ChildObj의 갯수를 얻는다.

 @param hWinObj [in] : ChildObjMng 핸들
 @return ChildObj의 갯수
*/
atINT atWINOBJ_GetChildObjCount( atHWINOBJ hWinObj )
{
	atLPWINOBJ lpWinObj = atWINOBJ_GetPtr(hWinObj);
	if( lpWinObj == atNULL ) return 0;
	
	return atUIAPI_GetListCount(lpWinObj->lpChildObjList);
}


/** 
 @brief ChildObjMng에 등록된 ChildObj의 리스트를 얻는다. (내부용)
 
 ChildObjMng에 등록된 ChildObj의 리스트를 얻는다.

 @param hWinObj [in] : ChildObjMng 핸들
 @return ChildObjMng에 등록된 ChildObj의 리스트
 @remark 내부용
*/
atUILIST atWINOBJ_GetChildObjList( atHWINOBJ hWinObj )
{
	atLPWINOBJ lpWinObj = atWINOBJ_GetPtr(hWinObj);
	if( lpWinObj == atNULL ) return atNULL;
	
	return lpWinObj->lpChildObjList;
}


/** 
 @brief 특정 인덱스의 ChildObj를 얻는다.(내부용)
 
 특정 인덱스의 ChildObj를 얻는다.

 @param hWinObj [in] : ChildObjMng 핸들
 @param nIndex [in] : ChildObj Index
 @return ChildObj 핸들
 @remark ChildObj의 Index는 수시로 변하기 때문에 Unique하지 않다. 내부용으로 주로 사용된다.
*/
atHWINOBJ atWINOBJ_GetChildObjByIndex( atHWINOBJ hWinObj, atINT nIndex )
{
	atLPWINOBJ lpWinObj = atWINOBJ_GetPtr(hWinObj);
	if( lpWinObj == atNULL ) return atNULL;
	
	return (atHWINOBJ)atUIAPI_GetListAt(lpWinObj->lpChildObjList, nIndex);
}


/** 
 @brief ChildObj의 리스트상 인덱스를 얻는다. (내부용)
 
 ChildObj의 리스트상 인덱스를 얻는다.

 @param hWinObj [in] : ChildObjMng 핸들
 @param hChildObj [in] : ChildObj 핸들
 @return 없음
 @remark ChildObj의 Index는 수시로 변하기 때문에 Unique하지 않다. 내부용으로 주로 사용된다.
*/
atINT atWINOBJ_GetChildObjIndex( atHWINOBJ hWinObj, atHWINOBJ hChildObj )
{
	atLPWINOBJ lpWinObj = atWINOBJ_GetPtr(hWinObj);
	int pos;
	atHWINOBJ wnd;

	if( lpWinObj == atNULL ) return -1;

	wnd = atUIAPI_GetListHead(lpWinObj->lpChildObjList, &pos);

	while( wnd != atNULL ){
		if( (atHWINOBJ)atUIAPI_GetListNext(lpWinObj->lpChildObjList, &pos) == hChildObj ){
			return pos;
		}
	}

	return -1;
}


/** 
 @brief ChildObj를 등록
 
 ChildObjMng에 ChildObj를 등록한다.

 @param hWinObj [in] : ChildObjMng 핸들
 @param hChildObj [in] : ChildObj 핸들
 @return 성공시 atTRUE, 실패시 atFALSE
*/
atBOOL atWINOBJ_RegistChildObj( atHWINOBJ hWinObj, atHWINOBJ hChildObj )
{
	int isactive = atTRUE;
	atHWINOBJ hChildObj2;
	atLPWINOBJ lpChildObj, lpChildObj2;
	int idx, cnt;
	atLPWINOBJ lpWinObj = atWINOBJ_GetPtr(hWinObj);
	if( lpWinObj == atNULL ) return atFALSE;
	
	if( hChildObj == atNULL ) return atFALSE;
	lpChildObj = atWINOBJ_GetPtr(hChildObj);

	if( lpChildObj->dwAttr & atOBJATTR_STAYONTOP ){
    		atKERNEL_LockScreen();
		atUIAPI_AddListTail(lpWinObj->lpChildObjList, hChildObj);
		atKERNEL_UnlockScreen();
	} else {
		cnt = atUIAPI_GetListCount(lpWinObj->lpChildObjList);
		if( cnt > 0 ){
			hChildObj2 = atUIAPI_GetListTail(lpWinObj->lpChildObjList, &idx);
			while( hChildObj2 ){
				lpChildObj2 = atWINOBJ_GetPtr(hChildObj2);
				if( lpChildObj2 && !(lpChildObj2->dwAttr & atOBJATTR_STAYONTOP) ){
					break;
				}
				hChildObj2 = atUIAPI_GetListPrev(lpWinObj->lpChildObjList, &idx);
			}
		} else {
			idx = cnt;
		}
    		atKERNEL_LockScreen();
		if( idx >= cnt-1 )
			atUIAPI_AddListTail(lpWinObj->lpChildObjList, hChildObj);
		else
			atUIAPI_InsertListAfter(lpWinObj->lpChildObjList, idx, hChildObj);
		atKERNEL_UnlockScreen();
	}

	lpChildObj->hParentObj = lpWinObj->hParentObj;
	lpChildObj->bActivate = atTRUE;

	atWINOBJ_ProcessObjProc( hChildObj, OBJMSG_REGIST, (long)&isactive, 0 );
//	atWINOBJ_ProcessObjProc( hChildObj, OBJMSG_RESIZE, (long)&isactive, 0 );
	if( !isactive ){
		atWINOBJ_ReleaseChildObj( hWinObj, hChildObj );
		return atFALSE;
	}

	atWINOBJ_AdjustChildObjArea( hWinObj, hChildObj );

	if( lpChildObj->dwAttr & atOBJATTR_FOCUSABLE ){
		atWINOBJ_SetCurChildObj( hWinObj, hChildObj );
	}

	atWINOBJ_Refresh( hChildObj );

	return atTRUE;
}


/** 
 @brief 등록된 ChildObj 해제
 
 ChildObjMng에 등록된 ChildObj를 해제한다.\n
 등록되었다가 해제된 윈도우는 자동으로 파괴된다. 

 @param hWinObj [in] : ChildObjMng 핸들
 @param hChildObj [in] : ChildObj 핸들
 @return 성공시 atTRUE, 실패시 atFALSE
*/
atBOOL atWINOBJ_ReleaseChildObj( atHWINOBJ hWinObj, atHWINOBJ hChildObj )
{
	int	pos;
	atBOOL iscur = atFALSE;
	atHWINOBJ hActObj, hParentObj;
	atLPWINOBJ lpWinObj = atWINOBJ_GetPtr(hWinObj);
	if( lpWinObj == atNULL ) return atFALSE;

	hParentObj = atWINOBJ_GetParentObj(hChildObj);
	hActObj = ( hParentObj != atNULL )? hParentObj : atWINOBJ_FindTopActiveChildObjEx( hWinObj, hChildObj );

	atWINOBJ_SetCurChildObj( hWinObj, hActObj );

	pos	= atUIAPI_FindListItemIdx(lpWinObj->lpChildObjList, hChildObj, 0);
	if( pos != -1 ){
    		atKERNEL_LockScreen();		
		atUIAPI_RemoveListAt(lpWinObj->lpChildObjList, pos, atFALSE);
		atKERNEL_UnlockScreen();
	}

	return atTRUE;
}   


/** 
 @brief ChildObj에 포커스를 이동시킨다.
 
 ChildObj에 포커스를 이동시킨다.

 @param hWinObj [in] : ChildObjMng 핸들
 @param hChildObj [in] : ChildObj 핸들
 @return 이전에 포커스를 가진 ChildObj의 핸들. 없을 시, atNULL
*/
atHWINOBJ atWINOBJ_SetCurChildObj( atHWINOBJ hWinObj, atHWINOBJ hChildObj )
{
	int	pos, nNewOrder;
	atLPWINOBJ lpChildObj;
	atBOOL bChange;
	atLPWINOBJ lpWinObj = atWINOBJ_GetPtr(hWinObj);
	if( lpWinObj == atNULL ) return atNULL;

	if( lpWinObj->hCurChildObj == hChildObj ) return hChildObj;

	nNewOrder = -1;
	bChange = atTRUE;

	if( hChildObj != atNULL ){
		pos	= atUIAPI_FindListItemIdx(lpWinObj->lpChildObjList, hChildObj, 0);
		if( pos != -1 ){
			lpChildObj = atWINOBJ_GetPtr(hChildObj);
			if( lpChildObj == atNULL ) return lpWinObj->hCurChildObj;

			if( lpChildObj->bShow && lpChildObj->bEnable && (lpChildObj->dwAttr & atOBJATTR_FOCUSABLE) ){
				nNewOrder = pos;
			} else {
				bChange = atFALSE;
			}
		} else {
			bChange = atFALSE;
		}
	}

	if( bChange ){
		if( lpWinObj->hCurChildObj ){ 
			atWINOBJ_ProcessObjProc( lpWinObj->hCurChildObj, OBJMSG_LOSTFOCUS, 0, 0 );
			//atWINOBJ_SetFocus( lpWinObj->hCurChildObj, atFALSE );
			atWINOBJ_Refresh( lpWinObj->hCurChildObj );
		}

		atBOOL bFocus = atFALSE;
		if( hChildObj != atNULL){
			atWINOBJ_ProcessObjProc( hChildObj, OBJMSG_GOTFOCUS, 0, 0);
			//bFocus = atWINOBJ_SetFocus( hChildObj, atTRUE );
			bFocus = atWINOBJ_IsFocusable( hChildObj );
			if( bFocus ){
				atWINOBJ_Refresh( hChildObj );
				lpWinObj->hCurChildObj = hChildObj;
				lpWinObj->nCurChildObjOrder = nNewOrder;
			}
		}
		
		if(!bFocus){
			lpWinObj->hCurChildObj = atNULL;
			lpWinObj->nCurChildObjOrder = -1;
		}
	}

//  Recusive가 일어날 가능성이 커서 일단 제거...
//	atWINOBJ_SendMessageToParentObj( hWinObj, OBJMSG_CHANGECHILDOBJFOCUS, (long)hChildObj, (long)hOldChildObj );

	return hChildObj;
}


/** 
 @brief 현재 포커스를 가진 ChildObj를 얻는다.
 
 현재 포커스를 가진 ChildObj를 얻는다.

 @param hWinObj [in] : ChildObjMng 핸들
 @return 포커스를 가진 ChildObj의 핸들. 없을 시, atNULL
*/
atHWINOBJ atWINOBJ_GetCurChildObj( atHWINOBJ hWinObj )
{
	atLPWINOBJ lpWinObj = atWINOBJ_GetPtr(hWinObj);
	if( lpWinObj == atNULL ) return atNULL;

	return lpWinObj->hCurChildObj;
}


/** 
 @brief 현재 포커스를 가진 ChildObj의 인덱스를 얻는다. (내부용)
 
 현재 포커스를 가진 ChildObj의 인덱스를 얻는다.

 @param hWinObj [in] : ChildObjMng 핸들
 @return 현재 포커스를 가진 ChildObj의 인덱스
 @remark ChildObj의 Index는 수시로 변하기 때문에 Unique하지 않다. 내부용으로 주로 사용된다.
*/
atINT atWINOBJ_GetCurChildObjOrder( atHWINOBJ hWinObj )
{
	atLPWINOBJ lpWinObj = atWINOBJ_GetPtr(hWinObj);
	if( lpWinObj == atNULL ) return -1;

	return lpWinObj->nCurChildObjOrder;
}


/** 
 @brief ChildObj를 가장 앞으로 옮겨 온다.
 
 ChildObj를 가장 앞으로 옮겨 온다.

 @param hWinObj [in] : ChildObjMng 핸들
 @param hChildObj [in] : ChildObj 핸들
 @return 실제로 가장 전면으로 옮겨진 윈도우의 핸들
 @remark StayOnTop 속성에 의해 가장 앞쪽으로 옮기지 못할 수도 있다.
*/
atHWINOBJ	atWINOBJ_SetTopChildObj( atHWINOBJ hWinObj, atHWINOBJ hChildObj )
{
	int	pos, cnt, idx;
	atHWINOBJ hChildObj2;
	atLPWINOBJ lpChildObj, lpChildObj2;
	atLPWINOBJ lpWinObj = atWINOBJ_GetPtr(hWinObj);
	if( lpWinObj == atNULL ) return atNULL;

	if( hChildObj == atNULL ) return atNULL;
	if( lpWinObj->hCurChildObj == hChildObj ) return hChildObj;

	cnt = atUIAPI_GetListCount(lpWinObj->lpChildObjList);
	if( cnt <= 1 ) return hChildObj;
	
	pos	= atUIAPI_FindListItemIdx(lpWinObj->lpChildObjList, hChildObj, 0);
	if( pos != -1 ){
		lpChildObj = atWINOBJ_GetPtr(hChildObj);

		if( lpChildObj->dwAttr & atOBJATTR_STAYONTOP ){
			// 이미 탑 윈도우므로... 무시...
		} else {
			cnt = atUIAPI_GetListCount(lpWinObj->lpChildObjList);
			if( cnt > 0 ){
				hChildObj2 = atUIAPI_GetListTail(lpWinObj->lpChildObjList, &idx);
				while( hChildObj2 ){
					lpChildObj2 = atWINOBJ_GetPtr(hChildObj2);
					if( lpChildObj2 && !(lpChildObj2->dwAttr & atOBJATTR_STAYONTOP) ){
						break;
					}
					hChildObj2 = atUIAPI_GetListPrev(lpWinObj->lpChildObjList, &idx);
				}
			} else {
				idx = cnt;
			}

			if( idx != pos ){
				atUIAPI_RemoveListAt(lpWinObj->lpChildObjList, pos, atFALSE);		
				if( idx >= cnt-1 )
					atUIAPI_AddListTail(lpWinObj->lpChildObjList, hChildObj);
				else
					atUIAPI_InsertListAfter(lpWinObj->lpChildObjList, idx, hChildObj);
			}
		}

		if( lpChildObj->bShow ){
			atWINOBJ_Refresh( hChildObj );
		}
	} else {
		hChildObj = lpWinObj->hCurChildObj;
	}

	return hChildObj;
}


/** 
 @brief ChildObj를 가장 뒷쪽으로 옮긴다.
 
 ChildObj를 가장 뒷쪽으로 옮긴다.

 @param hWinObj [in] : ChildObjMng 핸들
 @param hChildObj [in] : ChildObj 핸들
 @return 실제로 가장 뒷쪽에 옮겨진 윈도우의 핸들
*/
atHWINOBJ	atWINOBJ_SetBottomChildObj( atHWINOBJ hWinObj, atHWINOBJ hChildObj )
{
	int	pos, cnt;
	atLPWINOBJ lpChildObj;
	atLPWINOBJ lpWinObj = atWINOBJ_GetPtr(hWinObj);
	if( lpWinObj == atNULL ) return atNULL;

	if( hChildObj == atNULL ) return atNULL;
	if( lpWinObj->hCurChildObj == hChildObj ) return hChildObj;

	cnt = atUIAPI_GetListCount(lpWinObj->lpChildObjList);
	if( cnt <= 1 ) return hChildObj;
	
	pos	= atUIAPI_FindListItemIdx(lpWinObj->lpChildObjList, hChildObj, 0);
	if( pos != -1 ){
		lpChildObj = atWINOBJ_GetPtr(hChildObj);

		if( lpChildObj->bShow && lpChildObj->bEnable ){
			if( hChildObj == lpWinObj->hCurChildObj ){ 
				atWINOBJ_SetCurChildObj( hWinObj, atNULL );
			}
		}

		atUIAPI_RemoveListAt(lpWinObj->lpChildObjList, pos, atFALSE);
		atUIAPI_AddListHead(lpWinObj->lpChildObjList, hChildObj);

		hChildObj = atWINOBJ_FindTopActiveChildObj(hWinObj);
		if( hChildObj == atNULL ) return atNULL;
		
		lpChildObj = atWINOBJ_GetPtr(hChildObj);

		if( lpChildObj->bShow && lpChildObj->bEnable ){
			lpWinObj->hCurChildObj = hChildObj;
			lpWinObj->nCurChildObjOrder = 0;
			atWINOBJ_ProcessObjProc( hChildObj, OBJMSG_GOTFOCUS, 0, 0);
		}

		if( lpChildObj->bShow ){
//			atWINOBJ_RedrawAllChildObj( hWinObj, atTRUE );
			atWINOBJ_Refresh( hChildObj );
		}
	} else {
		hChildObj = lpWinObj->hCurChildObj;
	}

	return hChildObj;
}


/** 
 @brief 가장 앞쪽의 활성화 가능한 ChildObj를 얻는다. (내부용)
 
 가장 앞쪽의 활성화 가능한 ChildObj를 얻는다.\n
 주로 개체들이 생성 소멸 과정에서 다음번 포커스를 가질 후보를 찾을 때 쓴다.

 @param hWinObj [in] : ChildObjMng 핸들
 @return 활성화 가능한 ChildObj의 핸들
*/
atHWINOBJ	atWINOBJ_FindTopActiveChildObj( atHWINOBJ hWinObj )
{
	return atWINOBJ_FindTopActiveChildObjEx( hWinObj, atNULL );
}


/** 
 @brief 가장 앞쪽의 활성화 가능한 ChildObj를 얻는다. (내부용)
 
 가장 앞쪽의 활성화 가능한 ChildObj를 얻는다.\n
 주로 개체들이 생성 소멸 과정에서 다음번 포커스를 가질 후보를 찾을 때 쓴다.

 @param hWinObj [in] : ChildObjMng 핸들
 @param hExcludeObj [in] : 제외할 ChildObj 핸들 
 @return 활성화 가능한 ChildObj의 핸들
*/
atHWINOBJ	atWINOBJ_FindTopActiveChildObjEx( atHWINOBJ hWinObj, atHWINOBJ hExcludeObj )
{
	atHWINOBJ hChildObj;
	atLPWINOBJ	lpChildObj;
	atHWINOBJ saveWnd;
	int	pos;
	atLPWINOBJ lpWinObj = atWINOBJ_GetPtr(hWinObj);
	if( lpWinObj == atNULL ) return atNULL;

	saveWnd = atNULL;

	hChildObj = (atHWINOBJ)atUIAPI_GetListTail(lpWinObj->lpChildObjList, &pos);
	while ( pos != -1 ){
		if ( (hChildObj != atNULL) && (hChildObj != hExcludeObj) ){
			lpChildObj = atWINOBJ_GetPtr(hChildObj);
			if ( lpChildObj && lpChildObj->bShow && lpChildObj->bEnable && (lpChildObj->dwAttr & atOBJATTR_FOCUSABLE) ){
				saveWnd = hChildObj;
				break; 
			}
		}
		hChildObj = (atHWINOBJ)atUIAPI_GetListPrev(lpWinObj->lpChildObjList, &pos);
	}

	return saveWnd;
}


/** 
 @brief 해당 ID의 윈도우를 찾는다.
 
 해당 ID의 윈도우를 찾는다.

 @param hWinObj [in] : ChildObjMng 핸들
 @param nChildObjID [in] : ChildObj의 ID
 @return 찾은 ChildObj의 핸들. 없을 시 atNULL
 @remark ID는 중복이 허용되기 때문에 동일 ID의 윈도우가 2개 이상일 시, 첫번째 것만 찾을 수 있다.
*/
atHWINOBJ	atWINOBJ_FindChildObjByID( atHWINOBJ hWinObj, atINT nChildObjID )
{
	atHWINOBJ hChildObj;
	atLPWINOBJ	lpChildObj;
	atHWINOBJ saveWnd;
	int	pos;
	atLPWINOBJ lpWinObj = atWINOBJ_GetPtr(hWinObj);
	if( lpWinObj == atNULL ) return atNULL;

	saveWnd = atNULL;

	hChildObj = (atHWINOBJ)atUIAPI_GetListHead(lpWinObj->lpChildObjList, &pos);
	while ( pos != -1 ){
		if ( hChildObj != atNULL ){
			lpChildObj = atWINOBJ_GetPtr(hChildObj);
			if ( lpChildObj->nID == nChildObjID ){
				saveWnd = hChildObj;
				break; 
			}
		}
		hChildObj = (atHWINOBJ)atUIAPI_GetListNext(lpWinObj->lpChildObjList, &pos);
	}

	return saveWnd;
}


/** 
 @brief 해당 이름의 윈도우를 찾는다.
 
 해당 ID의 윈도우를 찾는다.

 @param hWinObj [in] : ChildObjMng 핸들
 @param szChildObjText [in] : ChildObj의 이름
 @return 찾은 ChildObj의 핸들. 없을 시 atNULL
*/
atHWINOBJ	atWINOBJ_FindChildObjByName( atHWINOBJ hWinObj, atLPTSTR szChildObjText )
{
	atHWINOBJ hChildObj;
	atLPWINOBJ	lpChildObj;
	atHWINOBJ saveWnd;
	int	pos;
	atLPWINOBJ lpWinObj = atWINOBJ_GetPtr(hWinObj);
	if( lpWinObj == atNULL ) return atNULL;

	saveWnd = atNULL;

	hChildObj = (atHWINOBJ)atUIAPI_GetListHead(lpWinObj->lpChildObjList, &pos);
	while ( pos != -1 ){
		if ( hChildObj != atNULL ){
			lpChildObj = atWINOBJ_GetPtr(hChildObj);
			if ( UI_STRCMP(lpChildObj->szObjName, szChildObjText) == 0 ){
				saveWnd = hChildObj;
				break; 
			}
		}
		hChildObj = (atHWINOBJ)atUIAPI_GetListNext(lpWinObj->lpChildObjList, &pos);
	}

	return saveWnd;
}


/** 
 @brief 화면상의 해당 지점의 최상단 윈도우를 찾는다.
 
 화면상의 해당 지점의 최상단 윈도우를 찾는다.

 @param hWinObj [in] : ChildObjMng 핸들
 @param nX, nY [in] : 기준이 되는 화면상의 위치
 @return 찾은 ChildObj의 핸들. 없을 시 atNULL
*/
atHWINOBJ atWINOBJ_FindChildObjAt( atHWINOBJ hWinObj, int x, int y )
{
	atLPWINOBJ lpWinObj = atWINOBJ_GetPtr(hWinObj);
	atUILIST lst;
	int		pos;
	atHWINOBJ hChildObj;
	atLPWINOBJ lpChildObj;

	if( lpWinObj == atNULL ) return atNULL;
	
	lst = lpWinObj->lpChildObjList;
	if( lst == atNULL ) return atNULL;

	hChildObj = (atHWINOBJ)atUIAPI_GetListTail(lst, &pos);
	while ( hChildObj != atNULL ){
		lpChildObj = atWINOBJ_GetPtr(hChildObj);
		if ( lpChildObj && lpChildObj->bShow ){
			if( x >= lpChildObj->rect.nStartX && x <= lpChildObj->rect.nEndX &&
				y >= lpChildObj->rect.nStartY && y <= lpChildObj->rect.nEndY ){
				return hChildObj;
			}
		}

		hChildObj	= (atHWINOBJ)atUIAPI_GetListPrev(lst, &pos);
	}
	
	return atNULL;
}


/** 
 @brief ChildObjMng의 Draw를 멈추거나 재개한다.
 
 ChildObjMng의 Draw를 멈추거나 재개한다.

 @param hWinObj [in] : ChildObjMng 핸들
 @param IsDraw [in] : Draw 동작 여부
 @return 없음
*/
void atWINOBJ_SetDrawFlag( atHWINOBJ hWinObj,  atBOOL IsDraw )
{
	atLPWINOBJ lpWinObj = atWINOBJ_GetPtr(hWinObj);
	if( lpWinObj == atNULL ) return;
	
	lpWinObj->bIsDraw = IsDraw;
}


/** 
 @brief ChildObj를 Show/Hide 시킨다.
 
 ChildObj를 Show/Hide 시킨다.

 @param hWinObj [in] : ChildObjMng 핸들
 @param hChildObj [in] : ChildObj 핸들
 @param bShow [in] : Show/Hide
 @return 없음
*/
void atWINOBJ_ShowChildObj( atHWINOBJ hWinObj, atHWINOBJ hChildObj, atBOOL bShow )
{
	atLPWINOBJ lpChildObj;
	atHWINOBJ hActObj, hParentObj;	
	atBOOL iscur = atFALSE;
	atLPWINOBJ lpWinObj = atWINOBJ_GetPtr(hWinObj);
	if( lpWinObj == atNULL ) return;

//	if( atWINOBJ_IsVisible(hChildObj) == bShow ) return;
	lpChildObj = atWINOBJ_GetPtr(hChildObj);
	if( lpChildObj->bShow == bShow )  return;
	
	atWINOBJ_AdjustAllChildObjArea( hWinObj );
	atWINOBJ_SetVisible( hChildObj, bShow );	

	if( !lpWinObj->bActivate ) return;

	hActObj = atWINOBJ_FindTopActiveChildObj( hWinObj ); 

	if( bShow ){
		if( hActObj == hChildObj ){
			atWINOBJ_SetCurChildObj( hWinObj, hChildObj );			
		} else {
			atWINOBJ_Refresh( hChildObj );
		}
	} else {
		if( hChildObj == lpWinObj->hCurChildObj ){
			hParentObj = atWINOBJ_GetParentObj(hChildObj);
			atWINOBJ_SetCurChildObj( hWinObj, ( hParentObj )? hParentObj : hActObj );			
		} else {
			atWINOBJ_Refresh( hChildObj );
		}
	}
}


static int _RedrawChildObjList( /*atLPWINOBJMNG lpChildObjMng, */ atHWINOBJ lstChildObj[], int nWinCnt, atBOOL IsAnnDraw, atBOOL IsSoftKeyDraw, atHWINOBJ hFocusChildObj )
{
	int i;
	atHWINOBJ hChildObj;
//	int pos;
	int draw_cnt = 0;
	//atRECT parent_rect, rect;
	
//	if( !lpWinObj->bIsDraw ) return 0;

	// 가장 바닥에 있는 윈도우부터 차례로 그려 올라온다.
	for( i = nWinCnt-1; i >=0; i-- ){
		hChildObj = lstChildObj[i];
		if(hChildObj == atNULL) {
			atDESKTOP_DrawDesktopBackground();
		} else {
			if ( atWINOBJ_IsVisible(hChildObj) ){
				atWINOBJ_Draw(hChildObj, hChildObj == hFocusChildObj);
			}
		}
	}

	if( IsAnnDraw )	
		atANNBAR_DrawAnnBar();
	
	if( IsSoftKeyDraw )	
		atSOFTKEY_DrawSoftKey();

	return draw_cnt;
}


static int _GetNeedDrawChildObj( atLPWINOBJ lpWinObj, atHWINOBJ *lstChildObj, int MaxWinCount, atRECT *parent_rect, int base_pos, atBOOL *pbIsFull )
{
	atHWINOBJ hChildObj;
	int pos, draw_cnt = 0;
	atRECT invalide_rect, rect;
	atRECT clip;
//	int cnt;
	
	if( !lpWinObj->bIsDraw ) return 0;
	if( base_pos < 0 ) return 0;

/*
	// 유효한 화면 범위를 구한다.
	if( lpWinObj->hParentObj != atNULL ){
		atWINOBJ_GetClientRect( lpWinObj->hParentObj, &parent_rect );
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
	hChildObj = (atHWINOBJ)atUIAPI_GetListTail(lpWinObj->lpChildObjList, &pos);

      if( pbIsFull ) *pbIsFull = atFALSE;
	  
	// 가장 바닥에 해당하는 윈도우를 찾는다.
	while( hChildObj != atNULL ){
		if( atWINOBJ_IsVisible(hChildObj) ){
			lstChildObj[draw_cnt] = hChildObj;
			draw_cnt ++;

			// 화면 전체를 차지한 윈도우가 있으면 이후엔 그리지 않는다.
			atWINOBJ_GetRect( hChildObj, &rect );
			if( atUIAPI_IsEmbededRect(&invalide_rect, &rect) ){
			  if( !(atWINOBJ_GetAttr(hChildObj) & atOBJATTR_TRANSPARENT) )
			    if( pbIsFull ) *pbIsFull = atTRUE;
			    break;
			}
		}

		if( pos <= base_pos ) break;

		hChildObj = (atHWINOBJ)atUIAPI_GetListPrev(lpWinObj->lpChildObjList, &pos);
	}

	return draw_cnt;
}


static int _RedrawChildObj( atLPWINOBJMNG lpChildObjMng, atRECT *parent_rect, int base_pos, atBOOL *pbIsFull, atHWINOBJ hFocusedObjs )
{
//	atHWINOBJ hChildObj;
	int draw_cnt = 0;
//	atRECT rect;
//	atRECT clip;
	atHWINOBJ lstChildObj[atWINOBJ_MAX_WINOBJ_CNT];
//	int cnt;

	draw_cnt = _GetNeedDrawChildObj( lpChildObjMng, lstChildObj, atWINOBJ_MAX_WINOBJ_CNT, parent_rect, base_pos, pbIsFull );

	if( draw_cnt <= 0 ) return 0;

/*
	if( (pos <= 0) && (lpWinObj->hParentObj == atNULL) ){
		if( hChildObj != atNULL ){
			atWINOBJ_GetRect( hChildObj, &rect );
			// 가장 밑 바닥 윈도우가 전체 화면이 아니라면 배경을 그린다.
			if( !atUIAPI_IsEmbededRect(&invalide_rect, &rect) ){
				atDESKTOP_DrawDesktop(); // 일단 임시로 제거.
			}
			hChildObj = (atHWINOBJ)atUIAPI_GetListHead(lpWinObj->lpChildObjList, &pos);
		} else {
			atDESKTOP_DrawDesktop();
		}
	}
*/

	// 가장 바닥에 있는 윈도우부터 차례로 그려 올라온다.
	_RedrawChildObjList( /*lpChildObjMng, */ lstChildObj, draw_cnt, atFALSE, atFALSE, hFocusedObjs );
	
//	atUIAPI_SetScreenClipRect2( &clip );

	return draw_cnt;
}


/** 
 @brief ChildObj를 모두 다시 그린다.
 
 특정 베이스 윈도우를 기준으로 ChildObjMng에 포함된 ChildObj를 모두 다시 그린다.

 @param hWinObj [in] : ChildObjMng 핸들
 @param hChildObjBase [in] : 기준이 되는 ChildObj 핸들
 @return 그려진 윈도우의 갯수
*/
int atWINOBJ_RedrawAllChildObjEx( atHWINOBJ hWinObj, atHWINOBJ hChildObjBase, atBOOL bFocused )
{
	int base_pos, draw_cnt;
	atRECT parent_rect;
	atLPWINOBJ lpWinObj = atWINOBJ_GetPtr(hWinObj);
	if( lpWinObj == atNULL ) return -1;

	if( lpWinObj->hParentObj && !atWINOBJ_IsValidate(lpWinObj->hParentObj) ) return -1;
	if( !lpWinObj->bIsDraw ) return -1;
		
	// 유효한 화면 범위를 구한다.
	if( lpWinObj->hParentObj != atNULL ){
		atWINOBJ_GetClientRect( lpWinObj->hParentObj, &parent_rect );
	} else {
		parent_rect.nStartX = 0;
		parent_rect.nEndX = atUIAPI_GetScreenWidth() - 1;
		parent_rect.nStartY = 0;
		parent_rect.nEndY = atUIAPI_GetScreenHeight() - 1;
	}

	base_pos = atWINOBJ_GetChildObjIndex( hWinObj, hChildObjBase );

	draw_cnt = 0;
	if( base_pos >= 0 ){		
		atHWINOBJ hFocusedObj = (bFocused)? atWINOBJ_GetCurChildObj( hWinObj ) : atNULL;
		draw_cnt = _RedrawChildObj(lpWinObj, &parent_rect, base_pos, atNULL, hFocusedObj );
	}
	
	if( (lpWinObj->hParentObj == atNULL) && (draw_cnt <= 0) ){
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
 @brief ChildObj를 모두 다시 그린다.
 
 ChildObjMng에 포함된 ChildObj를 모두 다시 그린다.

 @param hWinObj [in] : ChildObjMng 핸들
 @return 그려진 윈도우의 갯수
*/
int atWINOBJ_RedrawAllChildObj( atHWINOBJ hWinObj, atBOOL bFocused )
{
	int draw_cnt;
	atRECT parent_rect;
	atLPWINOBJ lpWinObj = atWINOBJ_GetPtr(hWinObj);
	if( lpWinObj == atNULL ) return -1;

	if( (lpWinObj->hParentObj == atNULL) && (atWINOBJ_GetChildObjCount(hWinObj)<= 0) ){
		// 윈도우가 전혀 없는 상태.
		atDESKTOP_DrawDesktop();
		return 0;
	} 
	
	if( lpWinObj->hParentObj && !atWINOBJ_IsValidate(lpWinObj->hParentObj) ) return -1;
	if( !lpWinObj->bIsDraw ) return -1;
		
	// 유효한 화면 범위를 구한다.
	if( lpWinObj->hParentObj != atNULL ){
		atWINOBJ_GetClientRect( lpWinObj->hParentObj, &parent_rect );
	} else {
		parent_rect.nStartX = 0;
		parent_rect.nEndX = atUIAPI_GetScreenWidth() - 1;
		parent_rect.nStartY = 0;
		parent_rect.nEndY = atUIAPI_GetScreenHeight() - 1;
	}

	{
	atHWINOBJ hFocusedObj = (bFocused)? atWINOBJ_GetCurChildObj( hWinObj ) : atNULL;
	draw_cnt = _RedrawChildObj(lpWinObj, &parent_rect, 0, atNULL, hFocusedObj );
	}

	if( lpWinObj->hParentObj == atNULL ){
		atANNBAR_DrawAnnBar();
		atSOFTKEY_DrawSoftKey();
	}

//	if( bFlush ) atUIAPI_FlushScreen();

	return draw_cnt;
}


/** 
 @brief 특정 영역에 걸친 ChildObj를 모두 다시 그린다.
 
 특정 영역에 걸친 ChildObj를 모두 다시 그린다.

 @param hWinObj [in] : ChildObjMng 핸들
 @param rtWinRect [in] : 그릴 영역
 @return 그려진 윈도우의 갯수
 @todo 좀 더 효율적으로 처리하기 위해 넣은 함수가 상황에 따라 문제를 일으켜서, 일단 클리핑 영역은 전체를 기준으로 하였음.
*/
int atWINOBJ_RedrawAllChildObjArea( atHWINOBJ hWinObj, atRECT *rtWinRect, atBOOL bFocused )
{
	atHWINOBJ hChildObj;
	int pos, draw_cnt;
//	atRECT screen_rect;
	atBOOL is_first;
	atRECT invalid_rect, parent_rect, rect, max_rect;
//	atRECT clip;
	atREGION rgn;
	atHWINOBJ lstChildObj[atWINOBJ_MAX_WINOBJ_CNT];
	atLPWINOBJ lpWinObj = atWINOBJ_GetPtr(hWinObj);
	if( lpWinObj == atNULL ) return -1;

	if( (lpWinObj->hParentObj == atNULL) && (atWINOBJ_GetChildObjCount(hWinObj)<= 0) ){
		// 윈도우가 전혀 없는 상태.
		atDESKTOP_DrawDesktop();
		return 0;
	} 
	
	if( lpWinObj->hParentObj && !atWINOBJ_IsValidate(lpWinObj->hParentObj) ) return -1;
	if( !lpWinObj->bIsDraw ) return -1;

	// 갱신할 화면 범위와 부모 화면 범위 중, 유효한 범위만을 고른다.
//	if( !atUIAPI_IntersectRect( &invalid_rect, rtWinRect, &parent_rect ) ) return 0;
// 좀 더 효율적으로 처리하기 위해 넣은 함수가 상황에 따라 문제를 일으켜서, 일단 무조건 전체를 갱신하도록 하였음.
parent_rect = invalid_rect = *rtWinRect;
is_first = atTRUE;
SET_RECT( max_rect, 0, 0, 0, 0 );

	// 전체 화면인 윈도우를 찾는다.
	hChildObj = (atHWINOBJ)atUIAPI_GetListTail(lpWinObj->lpChildObjList, &pos);

	// 가장 바닥에 해당하는 윈도우를 찾는다.
	draw_cnt = 0;
	atDWORD lastattr = atOBJATTR_NONE;
	while( hChildObj != atNULL ){
		if( atWINOBJ_IsVisible(hChildObj) ){
			atWINOBJ_GetRegion(hChildObj, &rgn );
			rgn.nX = rgn.nY = 0;
			atWINOBJ_ConvScreenRegion( hChildObj, &rgn, &rgn );
			REGION2RECT( rect, rgn );
			if( atUIAPI_IsIntersectRect(&invalid_rect, &rect) ){
				if( is_first || !atUIAPI_IsEmbededRect(&rect, &max_rect) ){
					lstChildObj[draw_cnt++] = hChildObj;

					lastattr = atWINOBJ_GetAttr(hChildObj);
					if( !(lastattr & atOBJATTR_TRANSPARENT) ) {
						// 화면 전체를 차지한 윈도우가 있으면 이후엔 그리지 않는다.
						if( atUIAPI_IsEmbededRect(&invalid_rect, &rect) ){
							break;
						}
					}

					if( is_first && !(atWINOBJ_GetAttr(hChildObj) & atOBJATTR_TRANSPARENT) ){
						max_rect = rect; 
						is_first = atFALSE; 
					} else {
						atUIAPI_UnionRect( &max_rect, &max_rect, &rect );
					}
//					atUIAPI_ToEmbededRect( &max_rect, &invalid_rect );
				}
			}
		}

		hChildObj = (atHWINOBJ)atUIAPI_GetListPrev(lpWinObj->lpChildObjList, &pos);
	}

	// 가장 바닥의 윈도우가 투명했거나, 화면을 가득 채우지 않았으면 desktop 영역도 추가 (사실상 소프트키와 안테나 영역 때문에 항상 포함)
	if( (lastattr & atOBJATTR_TRANSPARENT) || !atUIAPI_IsEmbededRect(&invalid_rect, &max_rect) ){
		lstChildObj[draw_cnt++] = atNULL;
	}


	// 화면 클리핑 ScreenClipRect범위를 한정한다.
//	clip = *atUIAPI_GetScreenClipRect();
//	atUIAPI_MoveRect( &invalid_rect, &invalid_rect, parent_rect.nStartX, parent_rect.nStartY );
//	atUIAPI_SetScreenClipRect2( &invalid_rect ); // 화면 일부 갱신에 문제가 생겨 일단 막는다.

	// 부모 윈도우가 없는 최상위 윈도우 매니저일 경우엔
/*
	if( lpWinObj->hParentObj == atNULL ){
		if( hChildObj != atNULL ){
			atWINOBJ_GetRect( hChildObj, &rect );
			// 가장 밑 바닥 윈도우가 전체 화면이 아니라면 배경을 그린다.
			if( (rect.nStartX > parent_rect.nStartX) || (rect.nStartY > parent_rect.nStartY) && 
				(rect.nEndX < parent_rect.nEndX) || (rect.nEndY < parent_rect.nEndY) ){
				atDESKTOP_DrawDesktop(atFALSE);
			}
			hChildObj = (atHWINOBJ)atUIAPI_GetListHead(lpWinObj->lpChildObjList, &pos);
		} else {
			atDESKTOP_DrawDesktop(atFALSE);
		}
	}
*/

	// 가장 바닥에 있는 윈도우부터 차례로 그려 올라온다.
	if( draw_cnt > 0 ) {
		atHWINOBJ hFocusedObj = (bFocused)? atWINOBJ_GetCurChildObj( hWinObj ) : atNULL;
		_RedrawChildObjList( /*lpChildObjMng, */ lstChildObj, draw_cnt, atFALSE, atFALSE, hFocusedObj );
	}
	
	if( lpWinObj->hParentObj == atNULL ){
		atANNBAR_DrawAnnBar();
		atSOFTKEY_DrawSoftKey();
	}

//	atUIAPI_SetScreenClipRect2( &clip );
	
	return draw_cnt;
}


/** 
 @brief 지정한 ChildObj의 다음 순서의 활성화 가능한 개체를 찾는다. (내부용)
 
 현재 포커스 가진 ChildObj의 다음 순서의 활성화 가능한 개체를 찾는다.\n
 맨 끝까지 찾아도 없으면 처음으로 돌아가서 계속 찾기 시작한다.

 @param hWinObj [in] : ChildObjMng 핸들
 @param hChildObj [in] : 시작 ChildObj 핸들
 @return 찾은 ChildObj 핸들. 못 찾았을 경우 atNULL
 @remark 키보드로 컨트롤의 포커스를 이동시킬 때 내부용으로 주로 씀.
*/
atHWINOBJ atWINOBJ_GetNextEnabledChildObj( atHWINOBJ hWinObj, atHWINOBJ hChildObj )
{
	atLPWINOBJ lpWinObj = atWINOBJ_GetPtr(hWinObj);
	atUILIST lst;
	int		pos, orgpos;
	atHWINOBJ hTempChildObj;
	atLPWINOBJ lpTempChildObj;

	if( lpWinObj == atNULL ) return atNULL;
	
	lst = lpWinObj->lpChildObjList;
	if( lst == atNULL ) return atNULL;

	orgpos = (hChildObj != atNULL)? atUIAPI_FindListItemIdx(lst, hChildObj, 0) : -1;

	pos = orgpos;
	hTempChildObj	= (atHWINOBJ)atUIAPI_GetListNext(lst, &pos);
	while ( hTempChildObj != atNULL ){
		lpTempChildObj = atWINOBJ_GetPtr(hTempChildObj);
		if ( lpTempChildObj && lpTempChildObj->bEnable && (lpTempChildObj->dwAttr & atOBJATTR_FOCUSABLE) && lpTempChildObj->bShow ){
			return hTempChildObj;
		}

		hTempChildObj	= (atHWINOBJ)atUIAPI_GetListNext(lst, &pos);
	}
	hTempChildObj	= (atHWINOBJ)atUIAPI_GetListHead(lst, &pos);
	while ( hTempChildObj != atNULL ){
		if ( hTempChildObj == hChildObj )
			return hChildObj;

		lpTempChildObj = atWINOBJ_GetPtr(hTempChildObj);
		if ( lpTempChildObj->bEnable && (lpTempChildObj->dwAttr & atOBJATTR_FOCUSABLE) && lpTempChildObj->bShow ){
			return hTempChildObj;
		}

		hTempChildObj	= (atHWINOBJ)atUIAPI_GetListNext(lst, &pos);
	}

	return atNULL;
}


/** 
 @brief 지정한 ChildObj의 이전 순서의 활성화 가능한 개체를 찾는다. (내부용)
 
 현재 포커스 가진 ChildObj의 이전 순서의 활성화 가능한 개체를 찾는다.\n
 처음까지 찾아도 없으면 맨 끝으로 돌아가서 계속 찾기 시작한다.

 @param hWinObj [in] : ChildObjMng 핸들
 @param hChildObj [in] : 시작 ChildObj 핸들
 @return 찾은 ChildObj 핸들. 못 찾았을 경우 atNULL
 @remark 키보드로 컨트롤의 포커스를 이동시킬 때 내부용으로 주로 씀.
*/
atHWINOBJ atWINOBJ_GetPrevEnabledChildObj( atHWINOBJ hWinObj, atHWINOBJ hChildObj )
{
	atLPWINOBJ lpWinObj = atWINOBJ_GetPtr(hWinObj);
	atUILIST lst;
	int		pos, orgpos;
	atHWINOBJ hTempChildObj;
	atLPWINOBJ lpTempChildObj;

	if( lpWinObj == atNULL ) return atNULL;
	
	lst = lpWinObj->lpChildObjList;
	if( lst == atNULL ) return atNULL;

	orgpos = (hChildObj != atNULL)? atUIAPI_FindListItemIdx(lst, hChildObj, 0) : -1;

	pos = orgpos;
	hTempChildObj	= (atHWINOBJ)atUIAPI_GetListPrev(lst, &pos);
	while ( hTempChildObj != atNULL ){
		lpTempChildObj = atWINOBJ_GetPtr(hTempChildObj);
		if ( lpTempChildObj->bEnable && (lpTempChildObj->dwAttr & atOBJATTR_FOCUSABLE) && lpTempChildObj->bShow ){
			return hTempChildObj;
		}

		hTempChildObj	= (atHWINOBJ)atUIAPI_GetListPrev(lst, &pos);
	}
	hTempChildObj	= (atHWINOBJ)atUIAPI_GetListTail(lst, &pos);
	while ( hTempChildObj != atNULL ){
		if ( hTempChildObj == hChildObj )
			return hChildObj;

		lpTempChildObj = atWINOBJ_GetPtr(hTempChildObj);
		if ( lpTempChildObj->bEnable && (lpTempChildObj->dwAttr & atOBJATTR_FOCUSABLE) && lpTempChildObj->bShow ){
			return hTempChildObj;
		}

		hTempChildObj	= (atHWINOBJ)atUIAPI_GetListPrev(lst, &pos);
	}
	
	return atNULL;
}


/** 
 @brief ChildObj를 Anchor를 적용하여 부모 ChildObj에 맞게 조정된 크기를 얻는다.
 
 ChildObj를 Anchor를 적용하여 부모 ChildObj에 맞게 조정된 크기를 얻는다.

 @param hWinObj [in] : ChildObjMng 핸들
 @param hChildObj [in] : ChildObj 핸들
 @param pRect [out] : 재조정된 영역 정보
 @return 성공시 atTRUE, 실패시 atFALSE
*/
atBOOL atWINOBJ_GetAnchorAdjRect( atHWINOBJ hWinObj, atHWINOBJ hChildObj, atRECT *pRect )
{
	atLPWINOBJ		lpChildObj = atWINOBJ_GetPtr(hChildObj);
	atHWINOBJ		hParentWnd;
	atRECT			rect, parentrect;
	atMARGIN		*anchor;

	if( lpChildObj == atNULL ){
		SET_RECT( *pRect, 0, 0, 0, 0 );
		return atFALSE;
	}

	if( !(lpChildObj->dwAttr & atOBJATTR_USEANCHOR) ) return atFALSE;
	
//	if( (lpChildObj->rtAnchor.nLeft < 0) && (lpChildObj->rtAnchor.nTop < 0) && 
//	    (lpChildObj->rtAnchor.nRight < 0) && (lpChildObj->rtAnchor.nBottom < 0) ) return atFALSE;

	hParentWnd = atWINOBJ_GetParentObj(hChildObj);
	if( (lpChildObj->dwAttr & atOBJATTR_CHILD) && (hParentWnd != atNULL) ){
		atWINOBJ_GetClientRect( hParentWnd, &parentrect );
		atUIAPI_MoveRect( &parentrect, &parentrect, -parentrect.nStartX, -parentrect.nStartY );
	} else {
//		atLPWINOBJ lpWinObj = atWINOBJ_GetPtr(hWinObj);
//		if( lpWinObj->hParentObj != atNULL ){
//			atWINOBJ_GetClientRect( lpWinObj->hParentObj, &parentrect );
//		} else {
			atREGION rgn;
			atDESKTOP_GetDesktopArea( atDESKTOP_AREA_CLIENT, &rgn );
			REGION2RECT( parentrect, rgn );
//		}
	}
	
	atWINOBJ_GetRect( hChildObj, &rect );
	anchor = &lpChildObj->rtAnchor;

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
 @brief ChildObj에 Anchor가 적용된 크기를 구한다.
 
 ChildObj에 Anchor가 적용된 크기를 구한다.

 @param hWinObj [in] : ChildObjMng 핸들
 @param 
 @return 없음
*/
atBOOL atWINOBJ_GetAnchorAdjRegion( atHWINOBJ hWinObj, atHWINOBJ hChildObj, atREGION *pRegion )
{
	atRECT rect;

	REGION2RECT( rect, *pRegion );	
	
	if( atWINOBJ_GetAnchorAdjRect( hWinObj, hChildObj, &rect ) ){
		RECT2REGION( *pRegion, rect );
		return atTRUE;
	} else 
		return atFALSE;
}


/** 
 @brief 윈도우의 레이아웃을 재조정
 
 Anchor를 적용하여 윈도우의 레이아웃을 재조정 한다.

 @param hWinObj [in] : ChildObjMng 핸들
 @param hChildObj [in] : ChildObj 핸들
 @return 없음
*/
void atWINOBJ_AdjustChildObjArea( atHWINOBJ hWinObj, atHWINOBJ hChildObj )
{
	int r;
	atREGION rgn;
	
	atWINOBJ_GetRegion( hChildObj, &rgn );
	r = atWINOBJ_GetAnchorAdjRegion( hWinObj, hChildObj, &rgn );
	if( r ){
		atWINOBJ_SetRegion( hChildObj, rgn.nX, rgn.nY, rgn.nWidth, rgn.nHeight, atFALSE );
	}
}


/** 
 @brief 모든 윈도우의 레이아웃을 재조정
 
 모든 윈도우의 레이아웃을 Anchor를 적용하여 재조정 한다.

 @param hWinObj [in] : ChildObjMng 핸들
 @return 없음
*/
void atWINOBJ_AdjustAllChildObjArea( atHWINOBJ hWinObj )
{
	atLPWINOBJ lpWinObj = atWINOBJ_GetPtr(hWinObj);
	atUILIST lst;
	int 	pos;
	atHWINOBJ hChildObj;

	if( lpWinObj == atNULL ) return;

	lst = lpWinObj->lpChildObjList;
	if( lst == atNULL ) return;
	
	hChildObj = atUIAPI_GetListHead(lst, &pos);

	while ( hChildObj != atNULL ){
		atWINOBJ_AdjustChildObjArea( hWinObj, hChildObj );

		hChildObj = (atHWINOBJ)atUIAPI_GetListNext(lst, &pos);
	}
}


/** 
 @brief 윈도우의 영역을 설정
 
 윈도우의 영역을 설정한다.

 @param hWinObj [in] : ChildObjMng 핸들
 @param hChildObj [in] : ChildObj 핸들
 @param nX, nY [in] : 윈도우의 위치
 @param nWidth, nHeight [in] : 윈도우의 크기
 @return 없음
*/
atVOID atWINOBJ_SetChildObjRegion( atHWINOBJ hWinObj, atHWINOBJ hChildObj, atINT nX, atINT nY, atINT nWidth, atINT nHeight )
{
	atLPWINOBJ		lpChildObj = atWINOBJ_GetPtr(hChildObj);
	
	lpChildObj->rect.nStartX = nX;
	lpChildObj->rect.nStartY = nY;
	lpChildObj->rect.nEndX = nX + nWidth - 1;
	lpChildObj->rect.nEndY = nY + nHeight - 1;
	
	atWINOBJ_AdjustChildObjArea( hWinObj, hChildObj );
}


/** 
 @brief ChildObj에 Message를 보낸다.
 
 ChildObj에 Message를 보낸다.

 @param hWinObj [in] : ChildObjMng 핸들
 @param nMsg [in] : Message
 @param lParam1, lParam2 [in] : 파라미터
 @return 윈도우 핸들러 호출 결과값
*/atVOID atWINOBJ_SendMessageAllChildObjs( atHWINOBJ hWinObj, int nMsg, long lParam1, long lParam2 )
{
	int 	pos;
	atUILIST lst;
	atHWINOBJ hChildObj;

	lst = atWINOBJ_GetChildObjList(hWinObj);
	if( lst == atNULL ) return;

	hChildObj = atUIAPI_GetListHead(lst, &pos);

	while ( hChildObj != atNULL ){
		atWINOBJ_ProcessObjProc( hChildObj, nMsg, lParam1, lParam2);
		hChildObj = (atHWINOBJ)atUIAPI_GetListNext(lst, &pos);
	}
}


/** 
 @brief ChildObj에 Message를 보낸다.
 
 ChildObj에 Message를 보낸다.

 @param hWinObj [in] : ChildObjMng 핸들
 @param nMsg [in] : Message
 @param lParam1, lParam2 [in] : 파라미터
 @return 윈도우 핸들러 호출 결과값
*/
atINT atWINOBJ_SendMessageToParentObj( atHWINOBJ hWinObj, int nMsg, long lParam1, long lParam2 )
{
	atLPWINOBJ lpWinObj = atWINOBJ_GetPtr(hWinObj);
	if( lpWinObj == atNULL ) return -1;
	if( lpWinObj->hParentObj == atNULL ) return 0;

	return atWINOBJ_ProcessObjProc( lpWinObj->hParentObj, nMsg, lParam1, lParam2 );		
}

