/*#############################################################################
-------------------------------------------------------------------------------

                                  A_Queue.c
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
#include "A_Queue.h"



/*======= class A_atQUEUE ============*/


#ifdef USE_SECTIONMEM
#define ALLOCMEM(a)     ( atCALLFUNC(This->lstFuncs.fnAllocMem)((atHSECTIONMEM)This->lstFuncs.lpData, (a)) )
#define FREEMEM(a)      ( atCALLFUNC(This->lstFuncs.fnFreeMem)((atHSECTIONMEM)This->lstFuncs.lpData, (a)) )
//#define REALLOCMEM(a,b) ( atCALLFUNC(This->lstFuncs.fnReallocMem)((atHSECTIONMEM)This->lstFuncs.lpData, (a), (b)) )
#else
#define ALLOCMEM(a)     ( atCALLFUNC(This->lstFuncs.fnAllocMem)(a) )
#define FREEMEM(a)      ( atCALLFUNC(This->lstFuncs.fnFreeMem)(a) )
//#define REALLOCMEM(a,b) ( atCALLFUNC(This->lstFuncs.fnReallocMem)((a), (b)) )
#endif
#define SAFEALLOCMEM(a)  (((a)>0)? ALLOCMEM(a):atNULL)
#define SAFEFREEMEM(a)   {if(a) FREEMEM(a); (a)=atNULL;}


/***************************
  생성자
****************************/
atQUEUE * atQUEUE_Create( atQUEUEFUNCS *lpFuncs, atINT QueueSize, atINT ItemSize )
{
    atQUEUE *This;
    
#ifdef USE_SECTIONMEM // 아직 This에 인스턴스가 없기 때문에 ALLOCMEM 매크로를 못 쓴다.
    This = (atQUEUE *)atCALLFUNC(lpFuncs->fnAllocMem)( atHSECTIONMEM)lpFuncs->lpData, sizeof(atQUEUE) );
#else
    This = (atQUEUE *)atCALLFUNC(lpFuncs->fnAllocMem)( sizeof(atQUEUE) );
#endif
    if( This == atNULL ) return atNULL;
    memset( This, 0, sizeof(atQUEUE) );

    This->lstFuncs.lpData       = lpFuncs->lpData;
    This->lstFuncs.fnAllocMem   = lpFuncs->fnAllocMem;
    This->lstFuncs.fnFreeMem    = lpFuncs->fnFreeMem;
//    This->lstFuncs.fnReallocMem = lpFuncs->fnReallocMem;
    
    This->iItemSize = ItemSize;
    if( ItemSize <= 0 ) ItemSize = sizeof(void *);
    This->pItems = (void **)ALLOCMEM( QueueSize * ItemSize );
    This->iQueueSize = (This->pItems)? QueueSize : 0;
    This->iEnqPtr = This->iDeqPtr = 0;
    
    This->pInsertFunc = atNULL;
    This->pDeleteFunc = atNULL;
    
    return( This );
}


/***************************
  소멸자
****************************/
atVOID atQUEUE_Destroy( atQUEUE *This )
{
    if( This == atNULL ) return;
    
    atQUEUE_Clear( This );

    FREEMEM( This->pItems );
    
    FREEMEM( This );
}


/***************************
  Callback 함수 지정
****************************/
atVOID atQUEUE_SetCallbackFunc( atQUEUE *This, atQUEUE_INSERTFUNC *InsertFunc, atQUEUE_DELETEFUNC *DeleteFunc )
{
    if( This == atNULL ) return;
    
    This->pInsertFunc = InsertFunc;
    This->pDeleteFunc = DeleteFunc;
}


/***************************

****************************/
atVOID atQUEUE_Clear( atQUEUE *This )
{
    if( This == atNULL ) return;
    
    atQUEUE_Drop( This, atQUEUE_GetCount(This) );
    
    This->iEnqPtr = This->iDeqPtr = 0;
}


/***************************

****************************/
atINT atQUEUE_GetCount( atQUEUE *This )
{
    int ep, dp;
    
    if( This == atNULL ) return( 0 );
    
    if( This->iQueueSize <= 0 ) return( 0 );
    
    ep = This->iEnqPtr;
    dp = This->iDeqPtr;
     
    if( ep == dp ) return( 0 );
    
    if( ep > dp ) return( ep - dp );
    else return( ep + (This->iQueueSize - dp) );
}

    
/***************************

****************************/
atBOOL atQUEUE_Enqueue( atQUEUE *This, atVOID *pItem, atBOOL bIsForce )
{
    int itmsz;
    int ep, dp, ep2;
    void *pDes;
    
    if( This == atNULL ) return( atFALSE );
    if( This->iQueueSize <= 0 ) return( atFALSE );

    if( atQUEUE_GetCount(This)+1 >= This->iQueueSize ){
		if( bIsForce )
			atQUEUE_Drop( This, 1 );
		else
			return( atFALSE );
    }

    ep = This->iEnqPtr;
    dp = This->iDeqPtr;

    ep2 = ep + 1;
    if( ep2 >= This->iQueueSize ) ep2 = 0;
    if( ep2 == dp ) return( atFALSE );

    itmsz = This->iItemSize;
    if( itmsz > 0 ){
      pDes = (void *)((atUINTPTR)This->pItems + ep * itmsz);
      if( This->pInsertFunc )
        This->pInsertFunc( pDes, pItem );  
      else  
        memcpy( pDes, pItem, itmsz );
    } else {
      ((void **)This->pItems)[ep] = pItem;
    }
    
    This->iEnqPtr = ep2;
    
    return( atTRUE );
}

    
/***************************

****************************/
atBOOL atQUEUE_EnqueueFirst( atQUEUE *This, atVOID *pItem )
{
    int itmsz;
    int ep, dp, dp2;
    void *pDes;
    
    if( This == atNULL ) return( atFALSE );
    
    if( This->iQueueSize <= 0 ) return( atFALSE );

    ep = This->iEnqPtr;
    dp = This->iDeqPtr;

    dp2 = dp - 1;
    if( dp2 < 0 ) dp2 = This->iQueueSize - 1;
    if( dp2 == ep ) return( atFALSE );

    itmsz = This->iItemSize;
    if( itmsz > 0 ){
      pDes = (void *)((atUINTPTR)This->pItems + dp2 * itmsz);
      if( This->pInsertFunc )
        This->pInsertFunc( pDes, pItem );  
      else  
        memcpy( pDes, pItem, itmsz );
    } else {
      ((void **)This->pItems)[dp2] = pItem;
    }
    
    This->iDeqPtr = dp2;

    return( atTRUE );
}

 /***************************

****************************/
atVOID * atQUEUE_Dequeue( atQUEUE *This, atVOID * pDes )
{
    int itmsz;
    int ep, dp;
    void *pSrc;
    
    if( This == atNULL ) return( atNULL );
    
    if( This->iQueueSize <= 0 ) return( atNULL );

    ep = This->iEnqPtr;
    dp = This->iDeqPtr;

    if( ep == dp ) return( atNULL );

    itmsz = This->iItemSize;
    if( itmsz <= 0 ) itmsz = sizeof(atVOID *);


/* 이 부분 좀 불안함... --; gmju 2005.9.23 */
    
    pSrc = (void *)((atUINTPTR)This->pItems + dp * itmsz);

    if( This->iItemSize > 0 ){
      if( This->pInsertFunc ){
		if( pDes == atNULL ) pDes = (void *)ALLOCMEM( itmsz );
        This->pInsertFunc( pDes, pSrc );
      } else { 
		  if( pDes ){
			memcpy( pDes, pSrc, itmsz );
		  } else {
			pDes = pSrc;
		  }
	  }
      if( This->pDeleteFunc ) This->pDeleteFunc( pSrc );
    } else {
      if( pDes != atNULL )
        memcpy( pDes, pSrc, itmsz );
      else
        pDes = ((void **)This->pItems)[dp];
    }

    if( ++dp >= This->iQueueSize ) dp = 0;
    This->iDeqPtr = dp;

    return( pDes );
}


/***************************

****************************/
atVOID * atQUEUE_Peek( atQUEUE *This, atINT iIndex )
{
    int itmsz, cnt;
    int ep, dp;
    void *p;
    
    if( This == atNULL ) return( atNULL );
    
    if( This->iQueueSize <= 0 ) return( atNULL );

    ep = This->iEnqPtr;
    dp = This->iDeqPtr;

    if( ep == dp ) return( atNULL );
    cnt = atQUEUE_GetCount( This );
    if( iIndex >= cnt ) return( atNULL );
    dp += iIndex;
    if( dp >= This->iQueueSize ) dp -= This->iQueueSize;    

    itmsz = This->iItemSize;
    if( itmsz > 0 ){
      p = (char *)This->pItems + dp * itmsz;
    } else {
      p = ((void **)This->pItems)[dp];
    }

    return( p );
}
    
    
/***************************

****************************/
atVOID atQUEUE_Drop( atQUEUE *This, atINT iDropCount )
{
    int i, itmsz;
    int ep, dp;
    void *p;
    
    if( This == atNULL ) return;
    
    if( This->iQueueSize <= 0 ) return;

    ep = This->iEnqPtr;
    dp = This->iDeqPtr;

    itmsz = This->iItemSize;

    for( i = 0; i < iDropCount; i ++ ){
      if( ep == dp ) break;

      if( itmsz > 0 ){
        p = (char *)This->pItems + dp * itmsz;
        if( This->pDeleteFunc ) This->pDeleteFunc( p );
      } else {
        p = ((void **)This->pItems)[dp];
        if( This->pDeleteFunc )
          This->pDeleteFunc( p );
        else
          FREEMEM( p );
      }
        
      if( ++dp >= This->iQueueSize ) dp = 0;
    }
    
    This->iDeqPtr = dp;
}


/***************************

****************************/
atBOOL atQUEUE_Remove( atQUEUE *This, atINT iIndex )
{
    int itmsz, count;
    int ep, dp, dp0;
    void *p, *p0;
    
    if( This == atNULL ) return atFALSE;
    
    if( This->iQueueSize <= 0 ) return atFALSE;

    count = atQUEUE_GetCount(This);
    if( iIndex < 0 || iIndex >= count ) return atFALSE;

    ep = This->iEnqPtr;
    dp = This->iDeqPtr;

    dp += iIndex;
    if( dp >= This->iQueueSize ) dp -= This->iQueueSize;

    itmsz = This->iItemSize;
    
    if( itmsz > 0 ){
      p = (char *)This->pItems + dp * itmsz;
      if( This->pDeleteFunc ) This->pDeleteFunc( p );
    } else {
      p = ((void **)This->pItems)[dp];
      if( This->pDeleteFunc )
        This->pDeleteFunc( p );
      else
        FREEMEM( p );
    }
    while( dp != ep ){
      dp0 = dp;
      if( ++dp >= This->iQueueSize ) dp = 0;
      if( itmsz > 0 ){
        p0 = (char *)This->pItems + dp0 * itmsz;
        p = (char *)This->pItems + dp * itmsz;
        memcpy( p0, p, itmsz );
      } else {
        ((void **)This->pItems)[dp0] = ((void **)This->pItems)[dp];
      }
    }
    dp = dp0;
    
    This->iDeqPtr = dp;
    
    return atTRUE;
}
