/*#############################################################################
-------------------------------------------------------------------------------

                                  A_List.c
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

#include <stdlib.h>
#include <string.h>

#include "AT_Types.h"
#include "A_List.h"



/*======= class atLIST ============*/

#ifdef atLIST_USE_OWNMEMORY
#ifdef USE_SECTIONMEM
#define ALLOCMEM(a)     ( atCALLFUNC(This->lstFuncs.fnAllocMem)((atHSECTIONMEM)This->lstFuncs.lpData, (a)) )
#define FREEMEM(a)      ( atCALLFUNC(This->lstFuncs.fnFreeMem)((atHSECTIONMEM)This->lstFuncs.lpData, (a)) )
#define REALLOCMEM(a,b) ( atCALLFUNC(This->lstFuncs.fnReallocMem)((atHSECTIONMEM)This->lstFuncs.lpData, (a), (b)) )
#else
#define ALLOCMEM(a)     ( atCALLFUNC(This->lstFuncs.fnAllocMem)(a) )
#define FREEMEM(a)      ( atCALLFUNC(This->lstFuncs.fnFreeMem)(a) )
#define REALLOCMEM(a,b) ( atCALLFUNC(This->lstFuncs.fnReallocMem)((a), (b)) )
#endif
#define SAFEALLOCMEM(a)  (((a)>0)? ALLOCMEM(a):atNULL)
#define SAFEFREEMEM(a)   {if(a) FREEMEM(a); (a)=atNULL;}
#endif


/***************************
  생성자
****************************/
#ifdef atLIST_USE_OWNMEMORY
atLIST * atLIST_Create( atLISTFUNCS *lpFuncs, atINT ItemSize, atINT iListSize )
#else
atBOOL   atLIST_Create( atLIST *This, atVOID *pBuf, atINT ItemSize, atINT iListSize )
#endif
{
#ifdef atLIST_USE_OWNMEMORY
    atLIST *This;
    
#ifdef USE_SECTIONMEM // 아직 This에 인스턴스가 없기 때문에 ALLOCMEM 매크로를 못 쓴다.
    This = (atLIST *)atCALLFUNC(lpFuncs->fnAllocMem)( atHSECTIONMEM)lpFuncs->lpData, sizeof(atLIST) );
#else
    This = (atLIST *)atCALLFUNC(lpFuncs->fnAllocMem)( sizeof(atLIST) );
#endif
#endif
#ifdef atLIST_USE_OWNMEMORY
    if( This == atNULL ) return atNULL;
#else        
    if( This == atNULL ) return atFALSE;
#endif
    memset( This, 0, sizeof(atLIST) );

#ifdef atLIST_USE_OWNMEMORY
    This->lstFuncs.lpData       = lpFuncs->lpData;
    This->lstFuncs.fnAllocMem   = lpFuncs->fnAllocMem;
    This->lstFuncs.fnFreeMem    = lpFuncs->fnFreeMem;
    This->lstFuncs.fnReallocMem = lpFuncs->fnReallocMem;
#endif

    This->iItemSize   = ItemSize;
    This->iListSize   = iListSize;

    This->iMaxCount   = 0;
    This->pItems      = (atVOID *)atNULL;
    This->iCount      = 0;
    This->iCurIndex   = 0;

    atLIST_SetCallbackFunc( This, atNULL, atNULL );
    atLIST_SetPreAllocSize( This, atLIST_DEF_PREALLOCSIZE );
    
#ifdef atLIST_USE_OWNMEMORY
    return( This );
#else
    return atTRUE;
#endif    
}


/***************************
  소멸자
****************************/
void atLIST_Destroy( atLIST *This )
{
    if( This == atNULL ) return;
    
    atLIST_Clear(This);

#ifdef atLIST_USE_OWNMEMORY
    SAFEFREEMEM( This->pItems );
    SAFEFREEMEM( This );
#else    
    This->pItems = atNULL;
#endif    
}


/***************************
 
****************************/
void atLIST_SetPreAllocSize( atLIST *This, atINT PreAllocSize )
{
    if( This == atNULL ) return;
    
    This->iPreAllocSize = PreAllocSize;
}


/***************************
  Callback 함수 지정
****************************/
void atLIST_SetCallbackFunc( atLIST *This, atLIST_INSERTFUNC *InsertFunc, atLIST_DELETEFUNC *DeleteFunc )
{
    if( This == atNULL ) return;
    
    This->pInsertFunc = InsertFunc;
    This->pDeleteFunc = DeleteFunc;
}


/***************************

****************************/
void atLIST_Clear( atLIST *This )
{
    int i;
    atVOID *e;
    
    if( This == atNULL ) return;
    
    if( This->pItems != atNULL ){
      if( This->iCount > 0 ){
 //       if( This->pDeleteFunc != atNULL ){
          for( i = This->iCount-1; i >= 0; i -- ){
            if( This->iItemSize > 0 ){
              e = (char *)This->pItems + (i * This->iItemSize);
              if( This->pDeleteFunc ) This->pDeleteFunc( e );
            } else {
              e = ((atVOID **)This->pItems)[i];
              if( This->pDeleteFunc ) This->pDeleteFunc( e );
            }  
          }
 //       } 
      }
      
#ifdef atLIST_USE_OWNMEMORY
//      if( This->iMaxCount != This->iPreAllocSize ){
      	SAFEFREEMEM( This->pItems );
        This->iMaxCount = 0;
//      }    
#endif
    }

    This->iCount = 0;
    This->iCurIndex = 0;
}


/***************************

****************************/
atINT atLIST_GetCount( atLIST *This )
{
    if( This == atNULL ) return( 0 );
    
    return( This->iCount );
}

    
/***************************

****************************/
atBOOL atLIST_SetCurIndex( atLIST *This, atINT Index )
{
    if( Index >= 0 && Index < This->iCount ){
      This->iCurIndex = Index;
      return( atTRUE );
    } else {
      return( atFALSE );
    }    
}

    
/***************************

****************************/
atINT atLIST_GetCurIndex( atLIST *This )
{
    if( This == atNULL ) return( -1 );
    
    return( This->iCurIndex );
}


/***************************

****************************/
atINT atLIST_MovePrev( atLIST *This )
{
    if( This == atNULL ) return( -1 );
    
    if( This->iCurIndex > 0 ) This->iCurIndex --;
    
    return( This->iCurIndex );
}
    
    
/***************************

****************************/
atINT atLIST_MoveNext( atLIST *This )
{
    if( This == atNULL ) return( -1 );
    
    if( This->iCurIndex < (This->iCount-1) ) This->iCurIndex ++;

    return( This->iCurIndex );
}


/***************************

****************************/
atBOOL atLIST_IsBOF( atLIST *This )
{
    if( This == atNULL ) return( atTRUE );
    
    return( This->iCurIndex <= 0 );
}


/***************************

****************************/
atBOOL atLIST_IsEOF( atLIST *This )
{
    if( This == atNULL ) return( atTRUE );
    
    return( This->iCurIndex >= (This->iCount-1) );
}


/***************************

****************************/
atBOOL atLIST_AddItem( atLIST *This, atVOID *pItem )
{
    int itmsz;
    atVOID *pp, *pp2;

    if( This == atNULL ) return( atFALSE );
    
    itmsz = This->iItemSize;
    if( itmsz <= 0 ) itmsz = sizeof(atVOID *);
    
    if( This->iCount >= This->iMaxCount ){
#ifdef atLIST_USE_OWNMEMORY
      //pp = (atVOID **)REALLOCMEM( This->pItems, (This->iMaxCount + This->iPreAllocSize) * sizeof(atVOID *) );
      pp2 = (atVOID *)SAFEALLOCMEM( (This->iMaxCount + This->iPreAllocSize) * itmsz );
      if( pp2 == atNULL ) return( atFALSE );

      if( This->pItems != atNULL ){
        memcpy( pp2, This->pItems, (This->iMaxCount) * itmsz );
        FREEMEM( This->pItems );
      }
      
      This->pItems = pp2;
      This->iMaxCount += This->iPreAllocSize;
#else
      return atFALSE;
#endif      
    }  
    pp = (char *)This->pItems + (This->iCount * itmsz);

    if( This->iItemSize > 0 ){
      if( This->pInsertFunc )
        This->pInsertFunc( pp, pItem );
      else
        memcpy( pp, pItem, itmsz );
    } else {
      *(void **)pp = pItem;
    }
    This->iCount ++;
    
    return( atTRUE );
}


/***************************

****************************/
atBOOL atLIST_InsertItem( atLIST *This, atINT Index, atVOID *pItem )
{
    int itmsz;
    atVOID *pp, *pp0, *pp2, *pp3;
    
    if( This == atNULL ) return( atFALSE );
    
    if( Index >= This->iCount ){
      return atLIST_AddItem( This, pItem );
    }
    
    if( Index < 0 ) Index = 0;
    This->iCurIndex = Index;

    itmsz = This->iItemSize; 
    if( itmsz <= 0 ) itmsz = sizeof(atVOID *);
    
    pp = (char *)This->pItems + (Index * itmsz);

    if( This->iCount < This->iMaxCount ){
#if 0 // gmju
	  memmove( (char *)pp + itmsz, pp, (This->iCount-Index) * itmsz );  // 중복된 영역이 있을 경우 사용하나, 플랫폼에 따라 오동작 여지 있음.
#else
	  int i;
	  for( i = This->iCount-Index-1; i >= 0; i -- ){
		memcpy( (char *)pp + (i+1) * itmsz, (char *)pp + i * itmsz, itmsz );
	  }
#endif
    } else {
#ifdef atLIST_USE_OWNMEMORY
      pp2 = (atVOID *)SAFEALLOCMEM( (This->iMaxCount + This->iPreAllocSize) * itmsz );
      if( pp2 == atNULL ) return( atFALSE );
      
      if( This->pItems != atNULL ){
        pp0 = This->pItems;
        memcpy( pp0, pp, (Index-1) * sizeof(atVOID *) );
        pp3 = (char *)pp0 + Index * itmsz;
        memcpy( (char *)pp + itmsz, pp3, (This->iCount-Index) * itmsz );      
        FREEMEM( pp0 );
      }

      This->pItems = pp2;
      This->iMaxCount += This->iPreAllocSize;
#else
      return atFALSE;
#endif      
    }  

    if( This->iItemSize > 0 ){
      if( This->pInsertFunc )
        This->pInsertFunc( pp, pItem );
      else
        memcpy( pp, pItem, itmsz );
    } else {
        *(void **)pp = pItem;
    }

    This->iCount ++;

    return( atTRUE );
}


/***************************

****************************/
atBOOL atLIST_DeleteItem( atLIST *This, atINT Index )
{
    int i, itmsz;
    atVOID *pp, *pp0, *pp2;
    
    if( This == atNULL ) return( atFALSE );
    
    if( This->pItems == atNULL ||
        Index < 0 || Index >= This->iCount ) return( atFALSE );

    itmsz = This->iItemSize;
    if( itmsz <= 0 ) itmsz = sizeof(atVOID *);
    
    pp = (char *)This->pItems + (Index * itmsz);

    if( This->iItemSize > 0 ){
      if( This->pDeleteFunc ) This->pDeleteFunc( pp );
    } else {
      if( This->pDeleteFunc ) This->pDeleteFunc( *(void **)pp );
    }

    // 남은 크기가 미리할당된 크기의 1.5배보다 크다면 할당된 크기를 줄인다.
    if( This->iCount > 1 ){
#ifdef atLIST_USE_OWNMEMORY
      if( (This->iMaxCount - This->iCount) < (This->iPreAllocSize+This->iPreAllocSize/2) ){
#endif      
		if( Index < This->iCount-1 ){
#if 0
		  memmove( pp, (char *)pp + itmsz, (This->iCount-Index-1) * itmsz );  // 중복된 영역이 있을 경우 사용하나, 플랫폼에 따라 오동작 여지 있음.
#else
		  for( i = 0; i < This->iCount-1 - Index; i ++ ){
			memcpy( (char *)pp + i * itmsz, (char *)pp + (i+1) * itmsz, itmsz );
		  }
#endif
		}
#ifdef atLIST_USE_OWNMEMORY
	  } else {  
////////////////// 나중에 Realloc을 이용해 수정할 예정. Realloc이 향상된 알고리즘으로 바뀐 후. //////////////////        
        i = This->iMaxCount - This->iPreAllocSize;
        if( i < This->iPreAllocSize ) i = This->iPreAllocSize;  
        pp2 = (atVOID *)SAFEALLOCMEM( i * itmsz );
        if( !pp2 ) return( atFALSE );
        pp0 = This->pItems;
        This->pItems = pp2;
        This->iMaxCount = i;
        if( Index > 0 ){
			memcpy( pp2, pp0, Index * itmsz );
		}
		pp = (char *)This->pItems + (Index * itmsz);
		if( Index < This->iCount-1 ){
			pp2 = (char *)pp0 + (Index+1) * itmsz;
			memcpy( pp, pp2, (This->iCount-1-Index) * itmsz );      
		}
		SAFEFREEMEM( pp0 );
      }
#endif      
    } else {
#ifdef atLIST_USE_OWNMEMORY
      SAFEFREEMEM( This->pItems );
      This->iMaxCount = 0;
#endif      
    }  

    This->iCount --;
//    This->iCurIndex = Index;
    if( This->iCurIndex >= This->iCount ) This->iCurIndex = This->iCount - 1;
    
    return( atTRUE );
}


/***************************

****************************/
atBOOL atLIST_RemoveItem( atLIST *This, atVOID *pItem )
{
/*
    int i, itmsz, Index;
    atVOID *pp, *pp0, *pp2;
    
    if( This == atNULL ) return( atFALSE );
    
    if( This->pItems == atNULL ) return( atFALSE );

    itmsz = This->iItemSize;
    if( itmsz <= 0 ) itmsz = sizeof(atVOID *);
    
    for( i = 0; i < This->iMaxCount; i ++ ){
      pp = (char *)This->pItems + (i * itmsz);
      if( 
    }
    if( pp == atNULL ) return atFALSE;
    
    if( This->iItemSize > 0 ){
      if( This->pDeleteFunc ) This->pDeleteFunc( pp );
    } else {
      if( This->pDeleteFunc )
        This->pDeleteFunc( *(void **)pp );
      else
        SAFEFREEMEM( *(void **)pp );
    }

    // 남은 크기가 미리할당된 크기의 1.5배보다 크다면 할당된 크기를 줄인다.
    if( This->iCount > 1 ){
      if( (This->iMaxCount - This->iCount) < (This->iPreAllocSize+This->iPreAllocSize/2) ){
        memmove( pp, (char *)pp + itmsz, (This->iCount-Index-1) * itmsz );
      } else {  
////////////////// 나중에 Realloc을 이용해 수정할 예정. Realloc이 향상된 알고리즘으로 바뀐 후. //////////////////        
        i = This->iMaxCount - This->iPreAllocSize;
        if( i < This->iPreAllocSize ) i = This->iPreAllocSize;  
        pp2 = (atVOID *)SAFEALLOCMEM( i * itmsz );
        if( !pp2 ) return( atFALSE );
        pp0 = This->pItems;
        This->pItems = pp2;
        This->iMaxCount -= This->iPreAllocSize;
        memcpy( pp2, pp0, Index * itmsz );
        pp2 = (char *)pp0 + (Index+1) * itmsz;
        memcpy( pp, pp2, (This->iCount-Index-1) * itmsz );      
        SAFEFREEMEM( pp0 );
      }
    } else {
      SAFEFREEMEM( This->pItems );
      This->iMaxCount = 0;
    }  

    This->iCount --;
    This->iCurIndex = Index;
    if( This->iCurIndex >= This->iCount ) This->iCurIndex = This->iCount - 1;

    return( atTRUE );
*/
    return( atFALSE );
}


/***************************

****************************/
atVOID * atLIST_GetItem( atLIST *This, atINT Index )
{
    int itmsz;
    atVOID *pp;

    if( This == atNULL ) return( atNULL );
    
    if( This->pItems == atNULL ||
        Index < 0 || Index >= This->iCount ) return( atNULL );
    
    itmsz = This->iItemSize;
    if( itmsz <= 0 ) itmsz = sizeof(atVOID *);
    
    pp = (char *)This->pItems + (Index * itmsz);

    if( This->iItemSize > 0 )
      return( pp );
    else
      return( *(void **)pp );
}    


/***************************

****************************/
atBOOL atLIST_SetItem( atLIST *This, atINT Index, atVOID *pItem )
{
    int itmsz;
    atVOID *pp;

    if( This == atNULL ) return( atFALSE );
    
    if( This->pItems == atNULL ||
        Index < 0 || Index >= This->iCount ) return( atFALSE );

    itmsz = This->iItemSize;
    if( itmsz <= 0 ) itmsz = sizeof(atVOID *);

    pp = (char *)This->pItems + (Index * itmsz);
    
    if( This->iItemSize > 0 ){
      if( This->pDeleteFunc )
        This->pDeleteFunc( pp );
       
      if( This->pInsertFunc )
        This->pInsertFunc( pp, pItem );
      else
        memcpy( pp, pItem, itmsz );
    } else {
      if( This->pDeleteFunc ) This->pDeleteFunc( pp );
      *(void **)pp = pItem;
    }
    
    memcpy( pp, pItem, itmsz );    
    
    return( atTRUE );
}


/***************************

****************************/
atVOID *atLIST_GetCurItem( atLIST *This )
{
    return atLIST_GetItem( This, atLIST_GetCurIndex(This) );
}


/***************************

****************************/
atBOOL atLIST_SetCurItem( atLIST *This, atVOID *pItem )
{
    return atLIST_SetItem( This, atLIST_GetCurIndex(This), pItem );
}


