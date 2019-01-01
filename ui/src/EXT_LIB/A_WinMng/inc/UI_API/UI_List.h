/**
  @file UI_List.h
  @brief List API
  @remark List API ����
  @author Amos
  @date 2010-04-19 ���� 11:41:00
  @see UI_List.c
*/
#ifndef __UI_LIST_H_
#define __UI_LIST_H_

#include "UI_Types.h"


#ifdef __cplusplus
extern "C" {
#endif


//typedef atLIST * atUILIST;
typedef atVOID * atUILIST;
typedef atVOID * atUINODEITEM;


atUILIST		atUIAPI_CreateList(void);											// ����Ʈ ����
void			atUIAPI_DestroyList(atUILIST lpList, atBOOL bDataDelete);		// ����Ʈ ����

int				atUIAPI_GetListCount(atUILIST lpList);							// ����Ʈ�� ī��Ʈ
atBOOL			atUIAPI_IsListEmpty(atUILIST lpList);							//

atUINODEITEM	atUIAPI_GetListHead(atUILIST lpList, atINT* lpPosition);
atUINODEITEM	atUIAPI_GetListTail(atUILIST lpList, atINT* lpPosition);

atINT			atUIAPI_AddListHead(atUILIST lpList, atUINODEITEM lpNodeData);
atINT			atUIAPI_AddListTail(atUILIST lpList, atUINODEITEM lpNodeData);

atUINODEITEM	atUIAPI_GetListNext(atUILIST lpList, atINT* lpPosition);
atUINODEITEM	atUIAPI_GetListPrev(atUILIST lpList, atINT* lpPosition);
atUINODEITEM	atUIAPI_GetListAt(atUILIST lpList, atINT position);

void			atUIAPI_SetListAt(atUILIST lpList, atINT pos, atUINODEITEM newElement, atBOOL bDeleteData);
void			atUIAPI_RemoveListAt(atUILIST lpList, atINT position, atBOOL bDeleteData);
void 			atUIAPI_RemoveListItem(atUILIST lpList, atUINODEITEM item, atBOOL bDeleteData);

atINT			atUIAPI_InsertListBefore(atUILIST lpList, atINT pos, atUINODEITEM newElement);
atINT			atUIAPI_InsertListAfter(atUILIST lpList, atINT pos, atUINODEITEM newElement);

atINT			atUIAPI_FindListItemIdx(atUILIST lpList, atUINODEITEM searchValue, atINT startAfter);

// bDataDelete = FALSE�̸� ����Ʈ���� �����͵��� �ݵ�� delete������� �Ѵ�.
void			atUIAPI_RemoveListAll(atUILIST lpList, atBOOL bDataDelete);


#ifdef __cplusplus
}
#endif


#endif

