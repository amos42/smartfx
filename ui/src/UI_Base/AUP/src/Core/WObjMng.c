/**
  @file WObjMng.c
  @brief WinObjMng ���� API
  @remark WinObjMng�� ���� API ����
  @author Amos
  @date 2010-04-19 ���� 11:41:00
  @see WObjMng.h WinObj.h
*/
#include "UI_MNG.h"
#include "WinObj.h"
#include "WObjMng.h"



/** 
 @brief WinObjMng ��ü�� �����͸� ��´�. (���ο�)
 
 WinObjMng ��ü�� �����͸� ��´�. (���ο�)

 @param hWinObjMng [in] : WinObjMng �ڵ�
 @return WinObjMng ��ü�� ������
 @remark ���ο�
*/
atLPWINOBJMNG atWINOBJMNG_GetPtr(atHWINOBJMNG hWinObjMng)
{
	return (atLPWINOBJMNG)hWinObjMng;
}


/**
 @brief WinObjMng ��ü�� �ڵ��� ��´�. (���ο�)
 
 WinObjMng ��ü�� �ڵ��� ��´�. (���ο�)

 @param lpApp [in] : WinObjMng ��ü�� ������
 @return WinObjMng�� �ڵ�
 @remark ���ο�
*/
atHWINOBJMNG atWINOBJMNG_GetHandle(atLPWINOBJMNG lpWinObjMng)
{
	return (atHWINOBJMNG)lpWinObjMng;
}


/** 
 @brief WinObjMng�� �����Ѵ�.
 
 WinObjMng�� �����Ѵ�.

 @param lHeapSize [in] : ���ø����̼��� ����ϴ� Heap ������
 @return ������ WinObjMng�� �ڵ�
 @remark ���� Heap ũ��� ������ �ʴ´�.
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
 @brief WinObjMng�� �����Ѵ�.
 
 WinObjMng�� �����Ѵ�.

 @param hParentWinObj [in] : WinObjMng�� ������ �θ� WinObj�� �ڵ�
 @param ExtObjMngSize [in] : WinObjMng�� ���������� ����� �߰� �������� ũ��
 @return ������ WinObjMng�� �ڵ�
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
 @brief WinObjMng�� �ı��Ѵ�.
 
 WinObjMng�� �ı��Ѵ�.

 @param hWinObjMng [in] : WinObjMng �ڵ�
 @return ����
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
 @brief WinObj�� �ı�
 
 WinObj�� �ı��Ѵ�.

 @param hWinObjMng [in] : WinObjMng �ڵ�
 @param hWinObj [in] : �ı��� WinObj �ڵ�
 @return ����
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
 @brief ��� WinObj�� �ı�
 
 ���� WinObjMng�� ��ϵ� ��� WinObj�� �ı��Ѵ�.

 @param hWinObjMng [in] : WinObjMng �ڵ�
 @return ����
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
 @brief ��� WinObj�� �ı� (���� �Ǵ� �ڵ鷯)
 
 ���� WinObjMng�� ��ϵ� ��� WinObj��, ���ǿ� �´� �����츸 ��� �ı��Ѵ�.\n
 �̶�, �ı��� ���� �Ǵ� �ڵ鷯�� �Ķ���ͷ� �����Ѵ�.

 @param hWinObjMng [in] : WinObjMng �ڵ�
 @param fnIsDel [in] : ���� �Ǵ� �ڵ鷯.
 @return ����
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
 @brief WinObj�� ������ ��´�.
 
 ���� WinObjMng�� ��ϵ� WinObj�� ������ ��´�.

 @param hWinObjMng [in] : WinObjMng �ڵ�
 @return WinObj�� ����
*/
atINT atWINOBJMNG_GetWinObjCount( atHWINOBJMNG hWinObjMng )
{
	atLPWINOBJMNG lpWinObjMng = atWINOBJMNG_GetPtr(hWinObjMng);
	if( lpWinObjMng == atNULL ) return 0;
	
	return atUIAPI_GetListCount(lpWinObjMng->lpWinObjList);
}


/** 
 @brief WinObjMng�� ��ϵ� WinObj�� ����Ʈ�� ��´�. (���ο�)
 
 WinObjMng�� ��ϵ� WinObj�� ����Ʈ�� ��´�.

 @param hWinObjMng [in] : WinObjMng �ڵ�
 @return WinObjMng�� ��ϵ� WinObj�� ����Ʈ
 @remark ���ο�
*/
atUILIST atWINOBJMNG_GetWinObjList( atHWINOBJMNG hWinObjMng )
{
	atLPWINOBJMNG lpWinObjMng = atWINOBJMNG_GetPtr(hWinObjMng);
	if( lpWinObjMng == atNULL ) return atNULL;
	
	return lpWinObjMng->lpWinObjList;
}


/** 
 @brief Ư�� �ε����� WinObj�� ��´�.(���ο�)
 
 Ư�� �ε����� WinObj�� ��´�.

 @param hWinObjMng [in] : WinObjMng �ڵ�
 @param nIndex [in] : WinObj Index
 @return WinObj �ڵ�
 @remark WinObj�� Index�� ���÷� ���ϱ� ������ Unique���� �ʴ�. ���ο����� �ַ� ���ȴ�.
*/
atHWINOBJ atWINOBJMNG_GetWinObjByIndex( atHWINOBJMNG hWinObjMng, atINT nIndex )
{
	atLPWINOBJMNG lpWinObjMng = atWINOBJMNG_GetPtr(hWinObjMng);
	if( lpWinObjMng == atNULL ) return atNULL;
	
	return (atHWINOBJ)atUIAPI_GetListAt(lpWinObjMng->lpWinObjList, nIndex);
}


/** 
 @brief WinObj�� ����Ʈ�� �ε����� ��´�. (���ο�)
 
 WinObj�� ����Ʈ�� �ε����� ��´�.

 @param hWinObjMng [in] : WinObjMng �ڵ�
 @param hWinObj [in] : WinObj �ڵ�
 @return ����
 @remark WinObj�� Index�� ���÷� ���ϱ� ������ Unique���� �ʴ�. ���ο����� �ַ� ���ȴ�.
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
 @brief WinObj�� ���
 
 WinObjMng�� WinObj�� ����Ѵ�.

 @param hWinObjMng [in] : WinObjMng �ڵ�
 @param hWinObj [in] : WinObj �ڵ�
 @return ������ atTRUE, ���н� atFALSE
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
 @brief ��ϵ� WinObj ����
 
 WinObjMng�� ��ϵ� WinObj�� �����Ѵ�.\n
 ��ϵǾ��ٰ� ������ ������� �ڵ����� �ı��ȴ�. 

 @param hWinObjMng [in] : WinObjMng �ڵ�
 @param hWinObj [in] : WinObj �ڵ�
 @return ������ atTRUE, ���н� atFALSE
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
 @brief WinObj�� ��Ŀ���� �̵���Ų��.
 
 WinObj�� ��Ŀ���� �̵���Ų��.

 @param hWinObjMng [in] : WinObjMng �ڵ�
 @param hWinObj [in] : WinObj �ڵ�
 @return ������ ��Ŀ���� ���� WinObj�� �ڵ�. ���� ��, atNULL
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

//  Recusive�� �Ͼ ���ɼ��� Ŀ�� �ϴ� ����...
//	atWINOBJMNG_SendMessageToParentWinObj( hWinObjMng, OBJMSG_CHANGECHILDOBJFOCUS, (long)hWinObj, (long)hOldWinObj );

	return hWinObj;
}


/** 
 @brief ���� ��Ŀ���� ���� WinObj�� ��´�.
 
 ���� ��Ŀ���� ���� WinObj�� ��´�.

 @param hWinObjMng [in] : WinObjMng �ڵ�
 @return ��Ŀ���� ���� WinObj�� �ڵ�. ���� ��, atNULL
*/
atHWINOBJ atWINOBJMNG_GetCurWinObj( atHWINOBJMNG hWinObjMng )
{
	atLPWINOBJMNG lpWinObjMng = atWINOBJMNG_GetPtr(hWinObjMng);
	if( lpWinObjMng == atNULL ) return atNULL;

	return lpWinObjMng->hCurWinObj;
}


/** 
 @brief ���� ��Ŀ���� ���� WinObj�� �ε����� ��´�. (���ο�)
 
 ���� ��Ŀ���� ���� WinObj�� �ε����� ��´�.

 @param hWinObjMng [in] : WinObjMng �ڵ�
 @return ���� ��Ŀ���� ���� WinObj�� �ε���
 @remark WinObj�� Index�� ���÷� ���ϱ� ������ Unique���� �ʴ�. ���ο����� �ַ� ���ȴ�.
*/
atINT atWINOBJMNG_GetCurWinObjOrder( atHWINOBJMNG hWinObjMng )
{
	atLPWINOBJMNG lpWinObjMng = atWINOBJMNG_GetPtr(hWinObjMng);
	if( lpWinObjMng == atNULL ) return -1;

	return lpWinObjMng->nCurWinObjOrder;
}


/** 
 @brief WinObj�� ���� ������ �Ű� �´�.
 
 WinObj�� ���� ������ �Ű� �´�.

 @param hWinObjMng [in] : WinObjMng �ڵ�
 @param hWinObj [in] : WinObj �ڵ�
 @return ������ ���� �������� �Ű��� �������� �ڵ�
 @remark StayOnTop �Ӽ��� ���� ���� �������� �ű��� ���� ���� �ִ�.
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
			// �̹� ž ������Ƿ�... ����...
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
 @brief WinObj�� ���� �������� �ű��.
 
 WinObj�� ���� �������� �ű��.

 @param hWinObjMng [in] : WinObjMng �ڵ�
 @param hWinObj [in] : WinObj �ڵ�
 @return ������ ���� ���ʿ� �Ű��� �������� �ڵ�
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
 @brief ���� ������ Ȱ��ȭ ������ WinObj�� ��´�. (���ο�)
 
 ���� ������ Ȱ��ȭ ������ WinObj�� ��´�.\n
 �ַ� ��ü���� ���� �Ҹ� �������� ������ ��Ŀ���� ���� �ĺ��� ã�� �� ����.

 @param hWinObjMng [in] : WinObjMng �ڵ�
 @return Ȱ��ȭ ������ WinObj�� �ڵ�
*/
atHWINOBJ	atWINOBJMNG_FindTopActiveWinObj( atHWINOBJMNG hWinObjMng )
{
	return atWINOBJMNG_FindTopActiveWinObjEx( hWinObjMng, atNULL );
}


/** 
 @brief ���� ������ Ȱ��ȭ ������ WinObj�� ��´�. (���ο�)
 
 ���� ������ Ȱ��ȭ ������ WinObj�� ��´�.\n
 �ַ� ��ü���� ���� �Ҹ� �������� ������ ��Ŀ���� ���� �ĺ��� ã�� �� ����.

 @param hWinObjMng [in] : WinObjMng �ڵ�
 @param hExcludeObj [in] : ������ WinObj �ڵ� 
 @return Ȱ��ȭ ������ WinObj�� �ڵ�
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
 @brief �ش� ID�� �����츦 ã�´�.
 
 �ش� ID�� �����츦 ã�´�.

 @param hWinObjMng [in] : WinObjMng �ڵ�
 @param nWinObjID [in] : WinObj�� ID
 @return ã�� WinObj�� �ڵ�. ���� �� atNULL
 @remark ID�� �ߺ��� ���Ǳ� ������ ���� ID�� �����찡 2�� �̻��� ��, ù��° �͸� ã�� �� �ִ�.
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
 @brief �ش� �̸��� �����츦 ã�´�.
 
 �ش� ID�� �����츦 ã�´�.

 @param hWinObjMng [in] : WinObjMng �ڵ�
 @param szWinObjText [in] : WinObj�� �̸�
 @return ã�� WinObj�� �ڵ�. ���� �� atNULL
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
 @brief ȭ����� �ش� ������ �ֻ�� �����츦 ã�´�.
 
 ȭ����� �ش� ������ �ֻ�� �����츦 ã�´�.

 @param hWinObjMng [in] : WinObjMng �ڵ�
 @param nX, nY [in] : ������ �Ǵ� ȭ����� ��ġ
 @return ã�� WinObj�� �ڵ�. ���� �� atNULL
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
 @brief WinObjMng�� Draw�� ���߰ų� �簳�Ѵ�.
 
 WinObjMng�� Draw�� ���߰ų� �簳�Ѵ�.

 @param hWinObjMng [in] : WinObjMng �ڵ�
 @param IsDraw [in] : Draw ���� ����
 @return ����
*/
void atWINOBJMNG_SetDrawFlag( atHWINOBJMNG hWinObjMng,  atBOOL IsDraw )
{
	atLPWINOBJMNG lpWinObjMng = atWINOBJMNG_GetPtr(hWinObjMng);
	if( lpWinObjMng == atNULL ) return;
	
	lpWinObjMng->bIsDraw = IsDraw;
}


/** 
 @brief WinObj�� Show/Hide ��Ų��.
 
 WinObj�� Show/Hide ��Ų��.

 @param hWinObjMng [in] : WinObjMng �ڵ�
 @param hWinObj [in] : WinObj �ڵ�
 @param bShow [in] : Show/Hide
 @return ����
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

	// ���� �ٴڿ� �ִ� ��������� ���ʷ� �׷� �ö�´�.
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
	// ��ȿ�� ȭ�� ������ ���Ѵ�.
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

	// 0, 0 ���̽��� �̵�
	atUIAPI_MoveRect( &invalide_rect, parent_rect, -parent_rect->nStartX, -parent_rect->nStartY );

	// ��ü ȭ���� �����츦 ã�´�.
	hWinObj = (atHWINOBJ)atUIAPI_GetListTail(lpWinObjMng->lpWinObjList, &pos);

      if( pbIsFull ) *pbIsFull = atFALSE;
	  
	// ���� �ٴڿ� �ش��ϴ� �����츦 ã�´�.
	while( hWinObj != atNULL ){
		if( atWINOBJ_IsVisible(hWinObj) ){
			lstWinObj[draw_cnt] = hWinObj;
			draw_cnt ++;

			// ȭ�� ��ü�� ������ �����찡 ������ ���Ŀ� �׸��� �ʴ´�.
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
			// ���� �� �ٴ� �����찡 ��ü ȭ���� �ƴ϶�� ����� �׸���.
			if( !atUIAPI_IsEmbededRect(&invalide_rect, &rect) ){
				atDESKTOP_DrawDesktop(); // �ϴ� �ӽ÷� ����.
			}
			hWinObj = (atHWINOBJ)atUIAPI_GetListHead(lpWinObjMng->lpWinObjList, &pos);
		} else {
			atDESKTOP_DrawDesktop();
		}
	}
*/

	// ���� �ٴڿ� �ִ� ��������� ���ʷ� �׷� �ö�´�.
	_RedrawWinObjList( /*lpWinObjMng, */ lstWinObj, draw_cnt, atFALSE, atFALSE, hFocusedObjs );
	
//	atUIAPI_SetScreenClipRect2( &clip );

	return draw_cnt;
}


/** 
 @brief WinObj�� ��� �ٽ� �׸���.
 
 Ư�� ���̽� �����츦 �������� WinObjMng�� ���Ե� WinObj�� ��� �ٽ� �׸���.

 @param hWinObjMng [in] : WinObjMng �ڵ�
 @param hWinObjBase [in] : ������ �Ǵ� WinObj �ڵ�
 @return �׷��� �������� ����
*/
int atWINOBJMNG_RedrawAllWinObjEx( atHWINOBJMNG hWinObjMng, atHWINOBJ hWinObjBase, atBOOL bFocused )
{
	int base_pos, draw_cnt;
	atRECT parent_rect;
	atLPWINOBJMNG lpWinObjMng = atWINOBJMNG_GetPtr(hWinObjMng);
	if( lpWinObjMng == atNULL ) return -1;

	if( lpWinObjMng->hParentWinObj && !atWINOBJ_IsValidate(lpWinObjMng->hParentWinObj) ) return -1;
	if( !lpWinObjMng->bIsDraw ) return -1;
		
	// ��ȿ�� ȭ�� ������ ���Ѵ�.
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
 @brief WinObj�� ��� �ٽ� �׸���.
 
 WinObjMng�� ���Ե� WinObj�� ��� �ٽ� �׸���.

 @param hWinObjMng [in] : WinObjMng �ڵ�
 @return �׷��� �������� ����
*/
int atWINOBJMNG_RedrawAllWinObj( atHWINOBJMNG hWinObjMng, atBOOL bFocused )
{
	int draw_cnt;
	atRECT parent_rect;
	atLPWINOBJMNG lpWinObjMng = atWINOBJMNG_GetPtr(hWinObjMng);
	if( lpWinObjMng == atNULL ) return -1;

	if( (lpWinObjMng->hParentWinObj == atNULL) && (atWINOBJMNG_GetWinObjCount(hWinObjMng)<= 0) ){
		// �����찡 ���� ���� ����.
		atDESKTOP_DrawDesktop();
		return 0;
	} 
	
	if( lpWinObjMng->hParentWinObj && !atWINOBJ_IsValidate(lpWinObjMng->hParentWinObj) ) return -1;
	if( !lpWinObjMng->bIsDraw ) return -1;
		
	// ��ȿ�� ȭ�� ������ ���Ѵ�.
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
 @brief Ư�� ������ ��ģ WinObj�� ��� �ٽ� �׸���.
 
 Ư�� ������ ��ģ WinObj�� ��� �ٽ� �׸���.

 @param hWinObjMng [in] : WinObjMng �ڵ�
 @param rtWinRect [in] : �׸� ����
 @return �׷��� �������� ����
 @todo �� �� ȿ�������� ó���ϱ� ���� ���� �Լ��� ��Ȳ�� ���� ������ �����Ѽ�, �ϴ� Ŭ���� ������ ��ü�� �������� �Ͽ���.
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
		// �����찡 ���� ���� ����.
		atDESKTOP_DrawDesktop();
		return 0;
	} 
	
	if( lpWinObjMng->hParentWinObj && !atWINOBJ_IsValidate(lpWinObjMng->hParentWinObj) ) return -1;
	if( !lpWinObjMng->bIsDraw ) return -1;

	// ������ ȭ�� ������ �θ� ȭ�� ���� ��, ��ȿ�� �������� ����.
//	if( !atUIAPI_IntersectRect( &invalid_rect, rtWinRect, &parent_rect ) ) return 0;
// �� �� ȿ�������� ó���ϱ� ���� ���� �Լ��� ��Ȳ�� ���� ������ �����Ѽ�, �ϴ� ������ ��ü�� �����ϵ��� �Ͽ���.
parent_rect = invalid_rect = *rtWinRect;
is_first = atTRUE;
SET_RECT( max_rect, 0, 0, 0, 0 );

	// ��ü ȭ���� �����츦 ã�´�.
	hWinObj = (atHWINOBJ)atUIAPI_GetListTail(lpWinObjMng->lpWinObjList, &pos);

	// ���� �ٴڿ� �ش��ϴ� �����츦 ã�´�.
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
						// ȭ�� ��ü�� ������ �����찡 ������ ���Ŀ� �׸��� �ʴ´�.
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

	// ���� �ٴ��� �����찡 �����߰ų�, ȭ���� ���� ä���� �ʾ����� desktop ������ �߰� (��ǻ� ����ƮŰ�� ���׳� ���� ������ �׻� ����)
	if( (lastattr & atOBJATTR_TRANSPARENT) || !atUIAPI_IsEmbededRect(&invalid_rect, &max_rect) ){
		lstWinObj[draw_cnt++] = atNULL;
	}


	// ȭ�� Ŭ���� ScreenClipRect������ �����Ѵ�.
//	clip = *atUIAPI_GetScreenClipRect();
//	atUIAPI_MoveRect( &invalid_rect, &invalid_rect, parent_rect.nStartX, parent_rect.nStartY );
//	atUIAPI_SetScreenClipRect2( &invalid_rect ); // ȭ�� �Ϻ� ���ſ� ������ ���� �ϴ� ���´�.

	// �θ� �����찡 ���� �ֻ��� ������ �Ŵ����� ��쿣
/*
	if( lpWinObjMng->hParentWinObj == atNULL ){
		if( hWinObj != atNULL ){
			atWINOBJ_GetRect( hWinObj, &rect );
			// ���� �� �ٴ� �����찡 ��ü ȭ���� �ƴ϶�� ����� �׸���.
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

	// ���� �ٴڿ� �ִ� ��������� ���ʷ� �׷� �ö�´�.
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
 @brief ������ WinObj�� ���� ������ Ȱ��ȭ ������ ��ü�� ã�´�. (���ο�)
 
 ���� ��Ŀ�� ���� WinObj�� ���� ������ Ȱ��ȭ ������ ��ü�� ã�´�.\n
 �� ������ ã�Ƶ� ������ ó������ ���ư��� ��� ã�� �����Ѵ�.

 @param hWinObjMng [in] : WinObjMng �ڵ�
 @param hWinObj [in] : ���� WinObj �ڵ�
 @return ã�� WinObj �ڵ�. �� ã���� ��� atNULL
 @remark Ű����� ��Ʈ���� ��Ŀ���� �̵���ų �� ���ο����� �ַ� ��.
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
 @brief ������ WinObj�� ���� ������ Ȱ��ȭ ������ ��ü�� ã�´�. (���ο�)
 
 ���� ��Ŀ�� ���� WinObj�� ���� ������ Ȱ��ȭ ������ ��ü�� ã�´�.\n
 ó������ ã�Ƶ� ������ �� ������ ���ư��� ��� ã�� �����Ѵ�.

 @param hWinObjMng [in] : WinObjMng �ڵ�
 @param hWinObj [in] : ���� WinObj �ڵ�
 @return ã�� WinObj �ڵ�. �� ã���� ��� atNULL
 @remark Ű����� ��Ʈ���� ��Ŀ���� �̵���ų �� ���ο����� �ַ� ��.
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
 @brief WinObj�� Anchor�� �����Ͽ� �θ� WinObj�� �°� ������ ũ�⸦ ��´�.
 
 WinObj�� Anchor�� �����Ͽ� �θ� WinObj�� �°� ������ ũ�⸦ ��´�.

 @param hWinObjMng [in] : WinObjMng �ڵ�
 @param hWinObj [in] : WinObj �ڵ�
 @param pRect [out] : �������� ���� ����
 @return ������ atTRUE, ���н� atFALSE
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
 @brief WinObj�� Anchor�� ����� ũ�⸦ ���Ѵ�.
 
 WinObj�� Anchor�� ����� ũ�⸦ ���Ѵ�.

 @param hWinObjMng [in] : WinObjMng �ڵ�
 @param 
 @return ����
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
 @brief �������� ���̾ƿ��� ������
 
 Anchor�� �����Ͽ� �������� ���̾ƿ��� ������ �Ѵ�.

 @param hWinObjMng [in] : WinObjMng �ڵ�
 @param hWinObj [in] : WinObj �ڵ�
 @return ����
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
 @brief ��� �������� ���̾ƿ��� ������
 
 ��� �������� ���̾ƿ��� Anchor�� �����Ͽ� ������ �Ѵ�.

 @param hWinObjMng [in] : WinObjMng �ڵ�
 @return ����
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
 @brief �������� ������ ����
 
 �������� ������ �����Ѵ�.

 @param hWinObjMng [in] : WinObjMng �ڵ�
 @param hWinObj [in] : WinObj �ڵ�
 @param nX, nY [in] : �������� ��ġ
 @param nWidth, nHeight [in] : �������� ũ��
 @return ����
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
 @brief WinObj�� Message�� ������.
 
 WinObj�� Message�� ������.

 @param hWinObjMng [in] : WinObjMng �ڵ�
 @param nMsg [in] : Message
 @param lParam1, lParam2 [in] : �Ķ����
 @return ������ �ڵ鷯 ȣ�� �����
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
 @brief WinObj�� Message�� ������.
 
 WinObj�� Message�� ������.

 @param hWinObjMng [in] : WinObjMng �ڵ�
 @param nMsg [in] : Message
 @param lParam1, lParam2 [in] : �Ķ����
 @return ������ �ڵ鷯 ȣ�� �����
*/
atINT atWINOBJMNG_SendMessageToParentWinObj( atHWINOBJMNG hWinObjMng, int nMsg, long lParam1, long lParam2 )
{
	atLPWINOBJMNG lpWinObjMng = atWINOBJMNG_GetPtr(hWinObjMng);
	if( lpWinObjMng == atNULL ) return -1;
	if( lpWinObjMng->hParentWinObj == atNULL ) return 0;

	return atWINOBJ_ProcessObjProc( lpWinObjMng->hParentWinObj, nMsg, lParam1, lParam2 );		
}

