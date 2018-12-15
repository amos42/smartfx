#include "_heap.h"
#include "SYSAPI.h"
#include <stdarg.h>

    
void *NMEA_alloc( long size )
{  
    return SYSAPI_AllocMem( size );
}

void NMEA_free( void *ptr )
{
    SYSAPI_FreeMem( ptr );
}

void *NMEA_realloc( void *ptr, long size )
{
    return SYSAPI_ReallocMem( ptr, size );
}


void NMEA_memset( void *ptr, char ch, long size )
{
    SYSAPI_memset( ptr, ch, size );
}


void NMEA_memcpy( void *ptr, const void *src,  long size )
{
    SYSAPI_memcpy( ptr, src, size );
}


int NMEA_strlen(const char* str)
{
    return SYSAPI_strlen( (const atLPSTR)str );
}

char* NMEA_strcpyn(char* dest, const char* src, int dest_size)
{
	char* pRes;
	int len = SYSAPI_strlen((char *)src);
	
	if(len >= dest_size) len = dest_size-1;

	pRes = SYSAPI_strncpy(dest, (char*)src, len);
	dest[len] = '\0';

	return pRes;
}

int NMEA_sprintf( char *str, const char *fmt, ... )
{
	va_list ap;
	int len;

	va_start(ap, fmt);
	len = SYSAPI_vsprintf( str, (char*)fmt, ap );
	va_end(ap);

	return len;
}

