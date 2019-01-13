/**
  @file WinObj2.c
  @brief ChildObj ���� API
  @remark ChildObj�� ���� API ����
  @author Amos
  @date 2010-04-19 ���� 11:41:00
  @see WObjMng.h ChildObj.h
*/
#include "AT_Types.h"
#include "UI_MNG.h"
#include "WinObj.h"


/** 
 @brief ChildObj�� �ı�
 
 ChildObj�� �ı��Ѵ�.

 @param hWinObj [in] : ChildObjMng �ڵ�
 @param hChildObj [in] : �ı��� ChildObj �ڵ�
 @return ����
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
 @brief ��� ChildObj�� �ı�
 
 ���� ChildObjMng�� ��ϵ� ��� ChildObj�� �ı��Ѵ�.

 @param hWinObj [in] : ChildObjMng �ڵ�
 @return ����
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
 @brief ��� ChildObj�� �ı� (���� �Ǵ� �ڵ鷯)
 
 ���� ChildObjMng�� ��ϵ� ��� ChildObj��, ���ǿ� �´� �����츸 ��� �ı��Ѵ�.\n
 �̶�, �ı��� ���� �Ǵ� �ڵ鷯�� �Ķ���ͷ� �����Ѵ�.

 @param hWinObj [in] : ChildObjMng �ڵ�
 @param fnIsDel [in] : ���� �Ǵ� �ڵ鷯.
 @return ����
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
 @brief ChildObj�� ������ ��´�.
 
 ���� ChildObjMng�� ��ϵ� ChildObj�� ������ ��´�.

 @param hWinObj [in] : ChildObjMng �ڵ�
 @return ChildObj�� ����
*/
atINT atWINOBJ_GetChildObjCount( atHWINOBJ hWinObj )
{
	atLPWINOBJ lpWinObj = atWINOBJ_GetPtr(hWinObj);
	if( lpWinObj == atNULL ) return 0;
	
	return atUIAPI_GetListCount(lpWinObj->lpChildObjList);
}


/** 
 @brief ChildObjMng�� ��ϵ� ChildObj�� ����Ʈ�� ��´�. (���ο�)
 
 ChildObjMng�� ��ϵ� ChildObj�� ����Ʈ�� ��´�.

 @param hWinObj [in] : ChildObjMng �ڵ�
 @return ChildObjMng�� ��ϵ� ChildObj�� ����Ʈ
 @remark ���ο�
*/
atUILIST atWINOBJ_GetChildObjList( atHWINOBJ hWinObj )
{
	atLPWINOBJ lpWinObj = atWINOBJ_GetPtr(hWinObj);
	if( lpWinObj == atNULL ) return atNULL;
	
	return lpWinObj->lpChildObjList;
}


/** 
 @brief Ư�� �ε����� ChildObj�� ��´�.(���ο�)
 
 Ư�� �ε����� ChildObj�� ��´�.

 @param hWinObj [in] : ChildObjMng �ڵ�
 @param nIndex [in] : ChildObj Index
 @return ChildObj �ڵ�
 @remark ChildObj�� Index�� ���÷� ���ϱ� ������ Unique���� �ʴ�. ���ο����� �ַ� ���ȴ�.
*/
atHWINOBJ atWINOBJ_GetChildObjByIndex( atHWINOBJ hWinObj, atINT nIndex )
{
	atLPWINOBJ lpWinObj = atWINOBJ_GetPtr(hWinObj);
	if( lpWinObj == atNULL ) return atNULL;
	
	return (atHWINOBJ)atUIAPI_GetListAt(lpWinObj->lpChildObjList, nIndex);
}


/** 
 @brief ChildObj�� ����Ʈ�� �ε����� ��´�. (���ο�)
 
 ChildObj�� ����Ʈ�� �ε����� ��´�.

 @param hWinObj [in] : ChildObjMng �ڵ�
 @param hChildObj [in] : ChildObj �ڵ�
 @return ����
 @remark ChildObj�� Index�� ���÷� ���ϱ� ������ Unique���� �ʴ�. ���ο����� �ַ� ���ȴ�.
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
 @brief ChildObj�� ���
 
 ChildObjMng�� ChildObj�� ����Ѵ�.

 @param hWinObj [in] : ChildObjMng �ڵ�
 @param hChildObj [in] : ChildObj �ڵ�
 @return ������ atTRUE, ���н� atFALSE
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
 @brief ��ϵ� ChildObj ����
 
 ChildObjMng�� ��ϵ� ChildObj�� �����Ѵ�.\n
 ��ϵǾ��ٰ� ������ ������� �ڵ����� �ı��ȴ�. 

 @param hWinObj [in] : ChildObjMng �ڵ�
 @param hChildObj [in] : ChildObj �ڵ�
 @return ������ atTRUE, ���н� atFALSE
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
 @brief ChildObj�� ��Ŀ���� �̵���Ų��.
 
 ChildObj�� ��Ŀ���� �̵���Ų��.

 @param hWinObj [in] : ChildObjMng �ڵ�
 @param hChildObj [in] : ChildObj �ڵ�
 @return ������ ��Ŀ���� ���� ChildObj�� �ڵ�. ���� ��, atNULL
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

//  Recusive�� �Ͼ ���ɼ��� Ŀ�� �ϴ� ����...
//	atWINOBJ_SendMessageToParentObj( hWinObj, OBJMSG_CHANGECHILDOBJFOCUS, (long)hChildObj, (long)hOldChildObj );

	return hChildObj;
}


/** 
 @brief ���� ��Ŀ���� ���� ChildObj�� ��´�.
 
 ���� ��Ŀ���� ���� ChildObj�� ��´�.

 @param hWinObj [in] : ChildObjMng �ڵ�
 @return ��Ŀ���� ���� ChildObj�� �ڵ�. ���� ��, atNULL
*/
atHWINOBJ atWINOBJ_GetCurChildObj( atHWINOBJ hWinObj )
{
	atLPWINOBJ lpWinObj = atWINOBJ_GetPtr(hWinObj);
	if( lpWinObj == atNULL ) return atNULL;

	return lpWinObj->hCurChildObj;
}


/** 
 @brief ���� ��Ŀ���� ���� ChildObj�� �ε����� ��´�. (���ο�)
 
 ���� ��Ŀ���� ���� ChildObj�� �ε����� ��´�.

 @param hWinObj [in] : ChildObjMng �ڵ�
 @return ���� ��Ŀ���� ���� ChildObj�� �ε���
 @remark ChildObj�� Index�� ���÷� ���ϱ� ������ Unique���� �ʴ�. ���ο����� �ַ� ���ȴ�.
*/
atINT atWINOBJ_GetCurChildObjOrder( atHWINOBJ hWinObj )
{
	atLPWINOBJ lpWinObj = atWINOBJ_GetPtr(hWinObj);
	if( lpWinObj == atNULL ) return -1;

	return lpWinObj->nCurChildObjOrder;
}


/** 
 @brief ChildObj�� ���� ������ �Ű� �´�.
 
 ChildObj�� ���� ������ �Ű� �´�.

 @param hWinObj [in] : ChildObjMng �ڵ�
 @param hChildObj [in] : ChildObj �ڵ�
 @return ������ ���� �������� �Ű��� �������� �ڵ�
 @remark StayOnTop �Ӽ��� ���� ���� �������� �ű��� ���� ���� �ִ�.
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
			// �̹� ž ������Ƿ�... ����...
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
 @brief ChildObj�� ���� �������� �ű��.
 
 ChildObj�� ���� �������� �ű��.

 @param hWinObj [in] : ChildObjMng �ڵ�
 @param hChildObj [in] : ChildObj �ڵ�
 @return ������ ���� ���ʿ� �Ű��� �������� �ڵ�
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
 @brief ���� ������ Ȱ��ȭ ������ ChildObj�� ��´�. (���ο�)
 
 ���� ������ Ȱ��ȭ ������ ChildObj�� ��´�.\n
 �ַ� ��ü���� ���� �Ҹ� �������� ������ ��Ŀ���� ���� �ĺ��� ã�� �� ����.

 @param hWinObj [in] : ChildObjMng �ڵ�
 @return Ȱ��ȭ ������ ChildObj�� �ڵ�
*/
atHWINOBJ	atWINOBJ_FindTopActiveChildObj( atHWINOBJ hWinObj )
{
	return atWINOBJ_FindTopActiveChildObjEx( hWinObj, atNULL );
}


/** 
 @brief ���� ������ Ȱ��ȭ ������ ChildObj�� ��´�. (���ο�)
 
 ���� ������ Ȱ��ȭ ������ ChildObj�� ��´�.\n
 �ַ� ��ü���� ���� �Ҹ� �������� ������ ��Ŀ���� ���� �ĺ��� ã�� �� ����.

 @param hWinObj [in] : ChildObjMng �ڵ�
 @param hExcludeObj [in] : ������ ChildObj �ڵ� 
 @return Ȱ��ȭ ������ ChildObj�� �ڵ�
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
 @brief �ش� ID�� �����츦 ã�´�.
 
 �ش� ID�� �����츦 ã�´�.

 @param hWinObj [in] : ChildObjMng �ڵ�
 @param nChildObjID [in] : ChildObj�� ID
 @return ã�� ChildObj�� �ڵ�. ���� �� atNULL
 @remark ID�� �ߺ��� ���Ǳ� ������ ���� ID�� �����찡 2�� �̻��� ��, ù��° �͸� ã�� �� �ִ�.
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
 @brief �ش� �̸��� �����츦 ã�´�.
 
 �ش� ID�� �����츦 ã�´�.

 @param hWinObj [in] : ChildObjMng �ڵ�
 @param szChildObjText [in] : ChildObj�� �̸�
 @return ã�� ChildObj�� �ڵ�. ���� �� atNULL
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
 @brief ȭ����� �ش� ������ �ֻ�� �����츦 ã�´�.
 
 ȭ����� �ش� ������ �ֻ�� �����츦 ã�´�.

 @param hWinObj [in] : ChildObjMng �ڵ�
 @param nX, nY [in] : ������ �Ǵ� ȭ����� ��ġ
 @return ã�� ChildObj�� �ڵ�. ���� �� atNULL
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
 @brief ChildObjMng�� Draw�� ���߰ų� �簳�Ѵ�.
 
 ChildObjMng�� Draw�� ���߰ų� �簳�Ѵ�.

 @param hWinObj [in] : ChildObjMng �ڵ�
 @param IsDraw [in] : Draw ���� ����
 @return ����
*/
void atWINOBJ_SetDrawFlag( atHWINOBJ hWinObj,  atBOOL IsDraw )
{
	atLPWINOBJ lpWinObj = atWINOBJ_GetPtr(hWinObj);
	if( lpWinObj == atNULL ) return;
	
	lpWinObj->bIsDraw = IsDraw;
}


/** 
 @brief ChildObj�� Show/Hide ��Ų��.
 
 ChildObj�� Show/Hide ��Ų��.

 @param hWinObj [in] : ChildObjMng �ڵ�
 @param hChildObj [in] : ChildObj �ڵ�
 @param bShow [in] : Show/Hide
 @return ����
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

	// ���� �ٴڿ� �ִ� ��������� ���ʷ� �׷� �ö�´�.
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
	// ��ȿ�� ȭ�� ������ ���Ѵ�.
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

	// 0, 0 ���̽��� �̵�
	atUIAPI_MoveRect( &invalide_rect, parent_rect, -parent_rect->nStartX, -parent_rect->nStartY );

	// ��ü ȭ���� �����츦 ã�´�.
	hChildObj = (atHWINOBJ)atUIAPI_GetListTail(lpWinObj->lpChildObjList, &pos);

      if( pbIsFull ) *pbIsFull = atFALSE;
	  
	// ���� �ٴڿ� �ش��ϴ� �����츦 ã�´�.
	while( hChildObj != atNULL ){
		if( atWINOBJ_IsVisible(hChildObj) ){
			lstChildObj[draw_cnt] = hChildObj;
			draw_cnt ++;

			// ȭ�� ��ü�� ������ �����찡 ������ ���Ŀ� �׸��� �ʴ´�.
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
			// ���� �� �ٴ� �����찡 ��ü ȭ���� �ƴ϶�� ����� �׸���.
			if( !atUIAPI_IsEmbededRect(&invalide_rect, &rect) ){
				atDESKTOP_DrawDesktop(); // �ϴ� �ӽ÷� ����.
			}
			hChildObj = (atHWINOBJ)atUIAPI_GetListHead(lpWinObj->lpChildObjList, &pos);
		} else {
			atDESKTOP_DrawDesktop();
		}
	}
*/

	// ���� �ٴڿ� �ִ� ��������� ���ʷ� �׷� �ö�´�.
	_RedrawChildObjList( /*lpChildObjMng, */ lstChildObj, draw_cnt, atFALSE, atFALSE, hFocusedObjs );
	
//	atUIAPI_SetScreenClipRect2( &clip );

	return draw_cnt;
}


/** 
 @brief ChildObj�� ��� �ٽ� �׸���.
 
 Ư�� ���̽� �����츦 �������� ChildObjMng�� ���Ե� ChildObj�� ��� �ٽ� �׸���.

 @param hWinObj [in] : ChildObjMng �ڵ�
 @param hChildObjBase [in] : ������ �Ǵ� ChildObj �ڵ�
 @return �׷��� �������� ����
*/
int atWINOBJ_RedrawAllChildObjEx( atHWINOBJ hWinObj, atHWINOBJ hChildObjBase, atBOOL bFocused )
{
	int base_pos, draw_cnt;
	atRECT parent_rect;
	atLPWINOBJ lpWinObj = atWINOBJ_GetPtr(hWinObj);
	if( lpWinObj == atNULL ) return -1;

	if( lpWinObj->hParentObj && !atWINOBJ_IsValidate(lpWinObj->hParentObj) ) return -1;
	if( !lpWinObj->bIsDraw ) return -1;
		
	// ��ȿ�� ȭ�� ������ ���Ѵ�.
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
		// �����찡 ���� ���� ����.
		atDESKTOP_DrawDesktop();
	} else {
		atANNBAR_DrawAnnBar();
		atSOFTKEY_DrawSoftKey();
	}

//	if( bFlush ) atUIAPI_FlushScreen();

	return draw_cnt;
}


/** 
 @brief ChildObj�� ��� �ٽ� �׸���.
 
 ChildObjMng�� ���Ե� ChildObj�� ��� �ٽ� �׸���.

 @param hWinObj [in] : ChildObjMng �ڵ�
 @return �׷��� �������� ����
*/
int atWINOBJ_RedrawAllChildObj( atHWINOBJ hWinObj, atBOOL bFocused )
{
	int draw_cnt;
	atRECT parent_rect;
	atLPWINOBJ lpWinObj = atWINOBJ_GetPtr(hWinObj);
	if( lpWinObj == atNULL ) return -1;

	if( (lpWinObj->hParentObj == atNULL) && (atWINOBJ_GetChildObjCount(hWinObj)<= 0) ){
		// �����찡 ���� ���� ����.
		atDESKTOP_DrawDesktop();
		return 0;
	} 
	
	if( lpWinObj->hParentObj && !atWINOBJ_IsValidate(lpWinObj->hParentObj) ) return -1;
	if( !lpWinObj->bIsDraw ) return -1;
		
	// ��ȿ�� ȭ�� ������ ���Ѵ�.
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
 @brief Ư�� ������ ��ģ ChildObj�� ��� �ٽ� �׸���.
 
 Ư�� ������ ��ģ ChildObj�� ��� �ٽ� �׸���.

 @param hWinObj [in] : ChildObjMng �ڵ�
 @param rtWinRect [in] : �׸� ����
 @return �׷��� �������� ����
 @todo �� �� ȿ�������� ó���ϱ� ���� ���� �Լ��� ��Ȳ�� ���� ������ �����Ѽ�, �ϴ� Ŭ���� ������ ��ü�� �������� �Ͽ���.
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
		// �����찡 ���� ���� ����.
		atDESKTOP_DrawDesktop();
		return 0;
	} 
	
	if( lpWinObj->hParentObj && !atWINOBJ_IsValidate(lpWinObj->hParentObj) ) return -1;
	if( !lpWinObj->bIsDraw ) return -1;

	// ������ ȭ�� ������ �θ� ȭ�� ���� ��, ��ȿ�� �������� ����.
//	if( !atUIAPI_IntersectRect( &invalid_rect, rtWinRect, &parent_rect ) ) return 0;
// �� �� ȿ�������� ó���ϱ� ���� ���� �Լ��� ��Ȳ�� ���� ������ �����Ѽ�, �ϴ� ������ ��ü�� �����ϵ��� �Ͽ���.
parent_rect = invalid_rect = *rtWinRect;
is_first = atTRUE;
SET_RECT( max_rect, 0, 0, 0, 0 );

	// ��ü ȭ���� �����츦 ã�´�.
	hChildObj = (atHWINOBJ)atUIAPI_GetListTail(lpWinObj->lpChildObjList, &pos);

	// ���� �ٴڿ� �ش��ϴ� �����츦 ã�´�.
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
						// ȭ�� ��ü�� ������ �����찡 ������ ���Ŀ� �׸��� �ʴ´�.
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

	// ���� �ٴ��� �����찡 �����߰ų�, ȭ���� ���� ä���� �ʾ����� desktop ������ �߰� (��ǻ� ����ƮŰ�� ���׳� ���� ������ �׻� ����)
	if( (lastattr & atOBJATTR_TRANSPARENT) || !atUIAPI_IsEmbededRect(&invalid_rect, &max_rect) ){
		lstChildObj[draw_cnt++] = atNULL;
	}


	// ȭ�� Ŭ���� ScreenClipRect������ �����Ѵ�.
//	clip = *atUIAPI_GetScreenClipRect();
//	atUIAPI_MoveRect( &invalid_rect, &invalid_rect, parent_rect.nStartX, parent_rect.nStartY );
//	atUIAPI_SetScreenClipRect2( &invalid_rect ); // ȭ�� �Ϻ� ���ſ� ������ ���� �ϴ� ���´�.

	// �θ� �����찡 ���� �ֻ��� ������ �Ŵ����� ��쿣
/*
	if( lpWinObj->hParentObj == atNULL ){
		if( hChildObj != atNULL ){
			atWINOBJ_GetRect( hChildObj, &rect );
			// ���� �� �ٴ� �����찡 ��ü ȭ���� �ƴ϶�� ����� �׸���.
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

	// ���� �ٴڿ� �ִ� ��������� ���ʷ� �׷� �ö�´�.
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
 @brief ������ ChildObj�� ���� ������ Ȱ��ȭ ������ ��ü�� ã�´�. (���ο�)
 
 ���� ��Ŀ�� ���� ChildObj�� ���� ������ Ȱ��ȭ ������ ��ü�� ã�´�.\n
 �� ������ ã�Ƶ� ������ ó������ ���ư��� ��� ã�� �����Ѵ�.

 @param hWinObj [in] : ChildObjMng �ڵ�
 @param hChildObj [in] : ���� ChildObj �ڵ�
 @return ã�� ChildObj �ڵ�. �� ã���� ��� atNULL
 @remark Ű����� ��Ʈ���� ��Ŀ���� �̵���ų �� ���ο����� �ַ� ��.
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
 @brief ������ ChildObj�� ���� ������ Ȱ��ȭ ������ ��ü�� ã�´�. (���ο�)
 
 ���� ��Ŀ�� ���� ChildObj�� ���� ������ Ȱ��ȭ ������ ��ü�� ã�´�.\n
 ó������ ã�Ƶ� ������ �� ������ ���ư��� ��� ã�� �����Ѵ�.

 @param hWinObj [in] : ChildObjMng �ڵ�
 @param hChildObj [in] : ���� ChildObj �ڵ�
 @return ã�� ChildObj �ڵ�. �� ã���� ��� atNULL
 @remark Ű����� ��Ʈ���� ��Ŀ���� �̵���ų �� ���ο����� �ַ� ��.
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
 @brief ChildObj�� Anchor�� �����Ͽ� �θ� ChildObj�� �°� ������ ũ�⸦ ��´�.
 
 ChildObj�� Anchor�� �����Ͽ� �θ� ChildObj�� �°� ������ ũ�⸦ ��´�.

 @param hWinObj [in] : ChildObjMng �ڵ�
 @param hChildObj [in] : ChildObj �ڵ�
 @param pRect [out] : �������� ���� ����
 @return ������ atTRUE, ���н� atFALSE
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
 @brief ChildObj�� Anchor�� ����� ũ�⸦ ���Ѵ�.
 
 ChildObj�� Anchor�� ����� ũ�⸦ ���Ѵ�.

 @param hWinObj [in] : ChildObjMng �ڵ�
 @param 
 @return ����
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
 @brief �������� ���̾ƿ��� ������
 
 Anchor�� �����Ͽ� �������� ���̾ƿ��� ������ �Ѵ�.

 @param hWinObj [in] : ChildObjMng �ڵ�
 @param hChildObj [in] : ChildObj �ڵ�
 @return ����
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
 @brief ��� �������� ���̾ƿ��� ������
 
 ��� �������� ���̾ƿ��� Anchor�� �����Ͽ� ������ �Ѵ�.

 @param hWinObj [in] : ChildObjMng �ڵ�
 @return ����
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
 @brief �������� ������ ����
 
 �������� ������ �����Ѵ�.

 @param hWinObj [in] : ChildObjMng �ڵ�
 @param hChildObj [in] : ChildObj �ڵ�
 @param nX, nY [in] : �������� ��ġ
 @param nWidth, nHeight [in] : �������� ũ��
 @return ����
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
 @brief ChildObj�� Message�� ������.
 
 ChildObj�� Message�� ������.

 @param hWinObj [in] : ChildObjMng �ڵ�
 @param nMsg [in] : Message
 @param lParam1, lParam2 [in] : �Ķ����
 @return ������ �ڵ鷯 ȣ�� �����
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
 @brief ChildObj�� Message�� ������.
 
 ChildObj�� Message�� ������.

 @param hWinObj [in] : ChildObjMng �ڵ�
 @param nMsg [in] : Message
 @param lParam1, lParam2 [in] : �Ķ����
 @return ������ �ڵ鷯 ȣ�� �����
*/
atINT atWINOBJ_SendMessageToParentObj( atHWINOBJ hWinObj, int nMsg, long lParam1, long lParam2 )
{
	atLPWINOBJ lpWinObj = atWINOBJ_GetPtr(hWinObj);
	if( lpWinObj == atNULL ) return -1;
	if( lpWinObj->hParentObj == atNULL ) return 0;

	return atWINOBJ_ProcessObjProc( lpWinObj->hParentObj, nMsg, lParam1, lParam2 );		
}

