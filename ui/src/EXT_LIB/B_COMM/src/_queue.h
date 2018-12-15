#ifndef ___QUEUE_H_
#define ___QUEUE_H_


#include "BC_Def.h"
#ifdef __USES_A_UTIL_
#include "A_Queue.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif
    

#ifdef __USES_A_UTIL_
typedef atQUEUE COMM_Q;
#else
typedef struct _tagCOMM_Q {
    CHAR_T * lpBuf;
    INT_T    nQueueSize;
    INT_T    nEnqPtr, nDeqPtr;
    INT_T		 nFillSize;
} COMM_Q;
#endif

BOOL_T   COMM_InitQueue( COMM_Q *lpQueue, void *lpBuf, INT_T nQueueSize );
void     COMM_ClearQueue( COMM_Q * lpQueue );
INT_T    COMM_GetQueueFillSize( COMM_Q * lpQueue );
INT_T    COMM_GetQueueFreeSize( COMM_Q * lpQueue );
INT_T    COMM_Enqueue( COMM_Q * lpQueue, void *pBuf, INT_T nSize, BOOL_T bParcely );
INT_T    COMM_Dequeue( COMM_Q * lpQueue, void *pBuf, INT_T nSize, BOOL_T bParcely, BOOL_T bClear );
INT_T    COMM_Drop( COMM_Q * lpQueue, INT_T nDropCount );


#ifdef __cplusplus
}
#endif
    

#endif
