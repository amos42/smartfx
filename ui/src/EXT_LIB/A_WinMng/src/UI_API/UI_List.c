/**
  @file UI_List.c
  @brief List�� ���� API
  @remark List�� ���� API ����
  @author Amos
  @date 2010-04-19 ���� 11:41:00
  @see UI_List.h
*/
#include "UI_API/UI_API.h"
#include "A_List.h"
#include "UI_Util.h"


/** 
 @brief 
 
 ......

 @param 
 @return ����
*/
atUILIST atUIAPI_CreateList(void)
{
	atLISTFUNCS funcs;

	atUILIST lpList;

	funcs.lpData = atNULL;
	funcs.fnAllocMem = atUIAPI_AllocMem;
	
	funcs.fnFreeMem = (void*)atUIAPI_FreeMem;
	funcs.fnReallocMem = atUIAPI_ReallocMem;
	lpList = atLIST_Create( &funcs, sizeof(atUINODEITEM), 0 );  // ������ ���� ����� �����ϱ� ���� �Ϻη� ������ ũ�⸦ ����. 
	atLIST_SetPreAllocSize( lpList, 50 );

	return (atUILIST)lpList;
}


/** 
 @brief 
 
 ......

 @param 
 @return ����
*/
void atUIAPI_DestroyList(atUILIST lpList, atBOOL bDataDelete)
{
	if ( lpList == atNULL ) return;

	atUIAPI_RemoveListAll(lpList, bDataDelete);
	atLIST_Destroy( lpList );
}


/** 
 @brief 
 
 ......

 @param 
 @return ����
*/
atUINODEITEM atUIAPI_GetListHead(atUILIST lpList, atINT* lpPosition)
{
	atUINODEITEM *pitm = (atUINODEITEM *)atLIST_GetItem( lpList, 0 );
	if( pitm != atNULL ){
		if( lpPosition ) *lpPosition = 0;
		return *pitm;
	} else {
		if( lpPosition ) *lpPosition = -1;
		return (atUINODEITEM)atNULL;
	}
}


/** 
 @brief 
 
 ......

 @param 
 @return ����
*/
atUINODEITEM atUIAPI_GetListTail(atUILIST lpList, atINT* lpPosition)
{
	atUINODEITEM itm;
	int cnt;

	cnt = atUIAPI_GetListCount(lpList);
	if( cnt <= 0 ){
		if( lpPosition ) *lpPosition = -1;
		return atNULL;
	}
	
	itm = *(atUINODEITEM *)atLIST_GetItem( lpList, cnt-1 );
	if( lpPosition ) *lpPosition = cnt-1;
	
	return itm;
}


/** 
 @brief 
 
 ......

 @param 
 @return ����
*/
int atUIAPI_GetListCount(atUILIST lpList)
{
	return atLIST_GetCount(lpList);
}


/** 
 @brief 
 
 ......

 @param 
 @return ����
*/
// Return Value: Newly Nodehead.
atINT atUIAPI_AddListHead(atUILIST lpList, atUINODEITEM lpData)// const
{
	atLIST_InsertItem( lpList, 0, &lpData );
	return 0;
}


/** 
 @brief 
 
 ......

 @param 
 @return ����
*/
// Return Value: Newly NodeTail.
atINT atUIAPI_AddListTail(atUILIST lpList, atUINODEITEM lpData)// const
{
	atLIST_AddItem( lpList, &lpData );
	return atUIAPI_GetListCount(lpList) - 1;
}


/** 
 @brief 
 
 ......

 @param 
 @return ����
*/
void atUIAPI_RemoveListAll(atUILIST lpList, atBOOL bDataDelete)
{
	atUINODEITEM itm;
	int i, cnt;

	cnt = atUIAPI_GetListCount(lpList);
	if( cnt <= 0 ) return;
	
	if( bDataDelete ){
		for( i = 0; i < cnt; i ++ ){
			itm = *(atUINODEITEM *)atLIST_GetItem( lpList, i );
			if( itm ) UI_MEMFREE( itm );
		}
	}
	
	atLIST_Clear( lpList );
}


/** 
 @brief 
 
 ......

 @param 
 @return ����
*/
atINT atUIAPI_FindListItemIdx(atUILIST lpList, atUINODEITEM searchValue, atINT startAfter)
{
	atUINODEITEM itm;
	int i, cnt;

	cnt = atUIAPI_GetListCount(lpList);
	if( cnt <= 0 ) return -1;
	
	for( i = startAfter; i < cnt; i ++ ){
		itm = (atUINODEITEM)atUIAPI_GetListAt( lpList, i );
		if( itm == searchValue ) return i;
	}

	return -1;
}


/** 
 @brief 
 
 ......

 @param 
 @return ����
*/
atUINODEITEM atUIAPI_GetListAt(atUILIST lpList, atINT position)
{
	atUINODEITEM *pitm;

	if( position < 0 ) return atNULL;

	pitm = (atUINODEITEM *)atLIST_GetItem( lpList, position );
	return (pitm != atNULL)? *pitm : (atUINODEITEM)atNULL;
}


/** 
 @brief 
 
 ......

 @param 
 @return ����
*/
atUINODEITEM atUIAPI_GetListNext(atUILIST lpList, atINT* lpPosition) 
{
	int idx = * lpPosition;
	int cnt = atUIAPI_GetListCount(lpList);
	
	if( idx+1 >= cnt ){
		*lpPosition = -1;
		return atNULL;
	}
	
	*lpPosition = idx+1;
	return atUIAPI_GetListAt( lpList, idx+1 );
}


/** 
 @brief 
 
 ......

 @param 
 @return ����
*/
atUINODEITEM atUIAPI_GetListPrev(atUILIST lpList, atINT* lpPosition)
{
	int idx = *lpPosition;
	
	if( idx-1 < 0 ) {
		*lpPosition = -1;
		return atNULL;
	}

	if( idx >= atUIAPI_GetListCount(lpList) ) idx = atUIAPI_GetListCount(lpList)-1;
	
	*lpPosition = idx-1;
	return atUIAPI_GetListAt( lpList, idx-1 );
}


/** 
 @brief 
 
 ......

 @param 
 @return ����
*/
atINT atUIAPI_InsertListBefore(atUILIST lpList, atINT pos, atUINODEITEM newElement)
{
	atLIST_InsertItem( lpList, pos, &newElement );

	return pos;
}


/** 
 @brief 
 
 ......

 @param 
 @return ����
*/
atINT atUIAPI_InsertListAfter(atUILIST lpList, atINT pos, atUINODEITEM newElement)
{
	int cnt = atLIST_GetCount(lpList);
	
	if( ++pos >= cnt )
	{
		atLIST_AddItem( lpList, &newElement );
	}
	else
	{
		atLIST_InsertItem( lpList, pos, &newElement );
	}
	
	return pos;
}


/** 
 @brief 
 
 ......

 @param 
 @return ����
*/
void atUIAPI_SetListAt(atUILIST lpList, atINT position, atUINODEITEM newElement, atBOOL bDeleteData)
{
	void *itm;
	
	if( position < 0 ) return;
	
	if( bDeleteData )
	{
		itm = atUIAPI_GetListAt( lpList, position );
		if( itm ) UI_MEMFREE( itm );
	}
	atLIST_SetItem( lpList, position, &newElement );
}


/** 
 @brief 
 
 ......

 @param 
 @return ����
*/
void atUIAPI_RemoveListAt(atUILIST lpList, atINT position, atBOOL bDeleteData)
{
	void *itm;

	if( position < 0 ) return;
	
	if( bDeleteData )
	{
		itm = atUIAPI_GetListAt( lpList, position );
		if( itm ) UI_MEMFREE( itm );
	}
	atLIST_DeleteItem( lpList, position );
}


/** 
 @brief 
 
 ......

 @param 
 @return ����
*/
void atUIAPI_RemoveListItem(atUILIST lpList, atUINODEITEM item, atBOOL bDeleteData)
{
	int position = atUIAPI_FindListItemIdx( lpList, item, 0);
	atUIAPI_RemoveListAt( lpList, position, bDeleteData );
}


