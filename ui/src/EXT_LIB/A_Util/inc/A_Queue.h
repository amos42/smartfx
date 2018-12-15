/*#############################################################################
-------------------------------------------------------------------------------

                                  Queue.h
                           (* =================== *)


 Version           : 1.0.0
 Dependent Modules :
 Author            : Ju, Gyeong-min ( Amos )
 Create            : 2001/04/25    01:47:09am
 Update            : 2003/04/29    00:00:00am
 C Compiler        : ADS 1.01, SDT 2.51, Borland C/C++ 5.0, Visual C/C++ 6.0
 Assembler         : ADS 1.01, SDT 2.51, Borland TASM 3.1, MASM 6.0
 Description       : 원형 큐 관련 함수들

-------------------------------------------------------------------------------
#############################################################################*/
#ifndef __A_QUEUE_H_
#define __A_QUEUE_H_


#include "AT_Types.h"


#ifndef USE_SECTIONMEM
//#define USE_SECTIONMEM  /// Section 메모리 사용 여부
#endif


#ifdef __cplusplus
extern "C" {
#endif
    

typedef struct _tagatQUEUEFUNCS {
	atLPVOID			lpData;
	atLPCALLBACKALLOC   fnAllocMem;
	atLPCALLBACKFREE    fnFreeMem;
//	void *              (*fnReallocMem)(void *, long);
} atQUEUEFUNCS;


typedef atVOID * (atQUEUE_INSERTFUNC)( atVOID * pDes, atVOID * pSrc );
typedef atINT    (atQUEUE_DELETEFUNC)( atVOID * pPtr );

typedef struct tagatQUEUE {
	atQUEUEFUNCS        lstFuncs;
    atVOID             *pItems;
    atINT               iQueueSize;
    atINT               iItemSize;
    atINT               iEnqPtr, iDeqPtr;
    atQUEUE_INSERTFUNC *pInsertFunc;
    atQUEUE_DELETEFUNC *pDeleteFunc;
} atQUEUE;
    

atQUEUE *       atQUEUE_Create( atQUEUEFUNCS *lpFuncs, atINT QueueSize, atINT ItemSize );
atVOID          atQUEUE_Destroy( atQUEUE * This );
atVOID          atQUEUE_SetCallbackFunc( atQUEUE * This, atQUEUE_INSERTFUNC *InsertFunc,
                                                         atQUEUE_DELETEFUNC *DeleteFunc );
atVOID          atQUEUE_Clear( atQUEUE * This );
atINT           atQUEUE_GetCount( atQUEUE * This );
atBOOL          atQUEUE_Enqueue( atQUEUE * This, atVOID *pItem, atBOOL bIsForce );
atBOOL          atQUEUE_EnqueueFirst( atQUEUE * This, atVOID *pItem );
atVOID *        atQUEUE_Dequeue( atQUEUE * This, atVOID *pDes );
atVOID *        atQUEUE_Peek( atQUEUE * This, atINT iIndex );
atVOID          atQUEUE_Drop( atQUEUE * This, atINT DropCount );
atBOOL          atQUEUE_Remove( atQUEUE * This, atINT iIndex );


#ifdef __cplusplus
};
#endif
    

#endif
