#ifndef ___HEAP_H_
#define ___HEAP_H_


#include "BC_Def.h"


#ifdef __cplusplus
extern "C" {
#endif

    
void *COMM_alloc( long size );
void COMM_free( void *ptr );
void *COMM_realloc( void *ptr, long size );
void COMM_memset( void *ptr, unsigned char v, long size );
void COMM_memcpy( void *des, void *src, long size );
void COMM_strncpy( LPTSTR_T des, LPTSTR_T src, long size );


#ifdef __cplusplus
}
#endif


#endif
