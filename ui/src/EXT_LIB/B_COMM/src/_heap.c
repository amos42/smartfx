#include "_heap.h"

#ifdef __USES_SYSAPI_
#include "SYSAPI.h"
#else
#include <malloc.h>
#include <string.h>
#include <tchar.h>
#endif

    
void *COMM_alloc( long size )
{  
#ifdef __USES_SYSAPI_
    return SYSAPI_AllocMem( size );
#else
    return (void *)malloc( size );
#endif
}

void COMM_free( void *ptr )
{
#ifdef __USES_SYSAPI_
    SYSAPI_FreeMem( ptr );
#else
    free( ptr );
#endif
}

void *COMM_realloc( void *ptr, long size )
{
#ifdef __USES_SYSAPI_
    return SYSAPI_ReallocMem( ptr, size );
#else
    return (void *)realloc( ptr, size );
#endif
}

void COMM_memset( void *ptr, unsigned char v, long size )
{
#ifdef __USES_SYSAPI_
	SYSAPI_memset( ptr, v, size );
#else
	memset( ptr, v, size );
#endif
}

void COMM_memcpy( void *des, void *src, long size )
{
#ifdef __USES_SYSAPI_
	SYSAPI_memcpy( des, src, size );
#else
	memcpy( des, src, size );
#endif
}

void COMM_strncpy( LPTSTR_T des, LPTSTR_T src, long size )
{
#ifdef __USES_SYSAPI_
	SYSAPI_tcsncpy( (atLPTSTR)des, (atLPTSTR)src, size );
#else
	_tcsncpy_s( (TCHAR *)des, size, (TCHAR *)src, size );
#endif
}

