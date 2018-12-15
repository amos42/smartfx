#include "_QUEUE.h"
#include "_heap.h"

#ifdef __USES_A_UTIL_
#include "A_Queue.h"
#endif


/***************************

****************************/
BOOL_T   COMM_InitQueue( COMM_Q *lpQueue, void *lpBuf, INT_T nQueueSize )
{
#ifdef __USES_A_UTIL_
	atQUEUE_InitQueue( lpQueue, lpBuf, nQueueSize );
#else
    COMM_memset( lpQueue, 0, sizeof(COMM_Q) );

    lpQueue->lpBuf = (CHAR_T *)lpBuf;
    lpQueue->nQueueSize = nQueueSize;
    lpQueue->nEnqPtr = lpQueue->nDeqPtr = 0;
    lpQueue->nFillSize = 0;
    
    return TRUE;
#endif
}


/***************************

****************************/
void     COMM_ClearQueue( COMM_Q * lpQueue )
{
#ifdef __USES_A_UTIL_
	atQUEUE_ClearQueue( lpQueue );
#else
    if( lpQueue == NULL ) return;
    
    lpQueue->nEnqPtr = lpQueue->nDeqPtr = 0;
#endif
}


/***************************

****************************/
INT_T    COMM_GetQueueFillSize( COMM_Q * lpQueue )
{
#ifdef __USES_A_UTIL_
	atQUEUE_GetQueueFillSize( lpQueue );
#else
//    int ep, dp;
    
    if( lpQueue == NULL ) return 0;
    
    if( lpQueue->nQueueSize <= 0 ) return 0;
    
/*    
    ep = lpQueue->nEnqPtr;
    dp = lpQueue->nDeqPtr;
     
    if( ep == dp ) return 0;
    
    if( ep > dp ) 
    	return ep - dp;
    else 
    	return ep + (lpQueue->nQueueSize - dp);
*/

	  return lpQueue->nFillSize;    	
#endif
}


/***************************

****************************/
INT_T    COMM_GetQueueFreeSize( COMM_Q * lpQueue )
{
#ifdef __USES_A_UTIL_
	atQUEUE_GetQueueFreeSize( lpQueue );
#else
    if( lpQueue == NULL ) return 0;

    if( lpQueue->nQueueSize <= 0 ) return 0;

	return lpQueue->nQueueSize - COMM_GetQueueFillSize( lpQueue );
#endif
}

    
/***************************

****************************/
INT_T COMM_Enqueue( COMM_Q * lpQueue, void *pBuf, INT_T nSize, BOOL_T bParcely )
{
#ifdef __USES_A_UTIL_
	atQUEUE_Enqueue( lpQueue, pBuf, nSize, bParcely );
#else
    int ep, dp, ep2;
    int idx, nSize2;
    
    if( lpQueue == atNULL ) return 0;    
    if( lpQueue->nQueueSize <= 0 ) return 0;

		if( (nSize + lpQueue->nFillSize) > lpQueue->nQueueSize ){
			if( !bParcely ) return 0;				
			nSize = lpQueue->nQueueSize - lpQueue->nFillSize;
		}
		
    ep = lpQueue->nEnqPtr;
    dp = lpQueue->nDeqPtr;

	  idx = 0;
    ep2 = ep + nSize;
    if( ep2 >= lpQueue->nQueueSize ){
    	ep2 -= lpQueue->nQueueSize;
	  	nSize2 = lpQueue->nQueueSize - ep;
		  COMM_memcpy( &lpQueue->lpBuf[ep], pBuf, nSize2 );
		  ep = 0;
		  idx += nSize2;
		  nSize2 = nSize - nSize2;
	  } else {
		  nSize2 = nSize;
	  }
	  
	COMM_memcpy( &lpQueue->lpBuf[ep], (void *)((char *)pBuf + idx), nSize2 );
    
    lpQueue->nEnqPtr = ep2;    
    lpQueue->nFillSize += nSize;
    
    return nSize;
#endif
}

    
/***************************

****************************/
INT_T COMM_Dequeue( COMM_Q * lpQueue, void *pBuf, INT_T nSize, BOOL_T bParcely, BOOL_T bClear )
{
#ifdef __USES_A_UTIL_
	atQUEUE_Dequeue( lpQueue, pBuf,nSize, bParcely, bClear );
#else
    int ep, dp, dp2;
    int idx, nSize2;
    
    if( lpQueue == atNULL ) return 0;    
    if( lpQueue->nQueueSize <= 0 ) return 0;
    if( lpQueue->nFillSize <= 0 ) return 0;

		if( nSize > lpQueue->nFillSize ){
			if( !bParcely ) return 0;				
			nSize = lpQueue->nFillSize;
		}
		
    ep = lpQueue->nEnqPtr;
    dp = lpQueue->nDeqPtr;

    //if( ep == dp ) return 0;

	idx = 0;
    dp2 = dp + nSize;
    if( dp2 >= lpQueue->nQueueSize ){
    	dp2 -= lpQueue->nQueueSize;
	  	nSize2 = lpQueue->nQueueSize - dp;
		  if( pBuf ) COMM_memcpy( pBuf, &lpQueue->lpBuf[dp], nSize2 );
		  dp = 0;
		  idx += nSize2;
		  nSize2 = nSize - nSize2;
	  } else {
		  nSize2 = nSize;
	  }

		if( pBuf ) COMM_memcpy( (void *)((char *)pBuf + idx), &lpQueue->lpBuf[dp], nSize2 );
    
    if( bClear ){
    	lpQueue->nDeqPtr = dp2;
    	lpQueue->nFillSize -= nSize;
    }
    
    return nSize;
#endif
}


/***************************

****************************/
INT_T  COMM_Drop( COMM_Q * lpQueue, INT_T nDropCount )
{
#ifdef __USES_A_UTIL_
	return atQUEUE_Drop( lpQueue, nDropCount );
#else
	  return COMM_Dequeue( lpQueue, NULL, nDropCount, TRUE, TRUE );
#endif
}
