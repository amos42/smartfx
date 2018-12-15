/*#############################################################################
-------------------------------------------------------------------------------

                                  A_List.h
                           (* =================== *)


 Version           : 1.0.0
 Dependent Modules :
 Author            : Ju, Gyeong-min ( Amos )
 Create            : 2001/04/25    01:47:09am
 Update            : 2003/04/29    00:00:00am
 C Compiler        : ADS 1.01, SDT 2.51, Borland C/C++ 5.0, Visual C/C++ 6.0
 Assembler         : ADS 1.01, SDT 2.51, Borland TASM 3.1, MASM 6.0
 Description       : 리스트 관련 함수들

-------------------------------------------------------------------------------
#############################################################################*/
#ifndef __A_LIST_H_
#define __A_LIST_H_


#include "AT_Types.h"


#define atLIST_USE_OWNMEMORY   /// 자체 할당 메모리 사용 여부
//#define atLIST_USE_SECTIONMEM  /// Section 메모리 사용 여부


#ifdef __cplusplus
extern "C" {
#endif
    

#define atLIST_DEF_PREALLOCSIZE  100

typedef struct _tagatLISTFUNCS {
	atLPVOID			lpData;
	atLPCALLBACKALLOC   fnAllocMem;
	atLPCALLBACKFREE    fnFreeMem;
	void *              (*fnReallocMem)(void *, long);
} atLISTFUNCS;


typedef void * (atLIST_INSERTFUNC)( atVOID *pDes, atVOID *pSrc );
typedef int    (atLIST_DELETEFUNC)( atVOID *pPtr );

typedef struct tagatLIST {
	atLISTFUNCS         lstFuncs;
    atVOID             *pItems;
    atINT               iListSize;
    atINT               iItemSize;
    atINT               iMaxCount;
    atINT               iCount;
    atINT               iCurIndex;
    atINT               iPreAllocSize;
    atLIST_INSERTFUNC  *pInsertFunc;
    atLIST_DELETEFUNC  *pDeleteFunc;
} atLIST;
    

#ifdef atLIST_USE_OWNMEMORY
atLIST *        atLIST_Create( atLISTFUNCS *lpFuncs, atINT ItemSize, atINT iListSize );
#else
atBOOL          atLIST_Create( atLIST *This, atVOID *pBuf, atINT ItemSize, atINT iListSize );
#endif
atVOID          atLIST_Destroy( atLIST *This );
atVOID          atLIST_SetCallbackFunc( atLIST *This, atLIST_INSERTFUNC *InsertFunc,
                                                      atLIST_DELETEFUNC *DeleteFunc );
atVOID          atLIST_SetPreAllocSize( atLIST *This, atINT PreAllocSize );
atVOID          atLIST_Clear( atLIST *This );
atINT           atLIST_GetCount( atLIST *This );
atBOOL 			atLIST_SetItem( atLIST *This, atINT Index, atVOID *pItem );
atBOOL          atLIST_AddItem( atLIST *This, atVOID *Item );
atBOOL          atLIST_InsertItem( atLIST *This, atINT Index, atVOID *pItem );
atBOOL          atLIST_DeleteItem( atLIST *This, atINT Index );
atBOOL          atLIST_RemoveItem( atLIST *This, atVOID *pItem );
//atBOOL          atLIST_RemoveItemAt( atLIST *This, atVOID *pItem );
atVOID *        atLIST_GetItem( atLIST *This, atINT Index );
atINT           atLIST_GetCurIndex( atLIST *This );
atBOOL          atLIST_MoveIndex( atLIST *This, atINT Index );
atBOOL          atLIST_MoveFirst( atLIST *This );
atBOOL          atLIST_MovePrev( atLIST *This );
atBOOL          atLIST_MoveNext( atLIST *This );
atBOOL          atLIST_MoveLast( atLIST *This );
atBOOL          atLIST_IsBOF( atLIST *This );
atBOOL          atLIST_IsEOF( atLIST *This );


#ifdef __cplusplus
};
#endif
    

#endif

