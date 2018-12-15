#ifndef ___HEAP_H_
#define ___HEAP_H_


#ifdef __cplusplus
extern "C" {
#endif

    
void *NMEA_alloc( long size );
void NMEA_free( void *ptr );
void *NMEA_realloc( void *ptr, long size );

void NMEA_memset( void *ptr, char ch, long size );
void NMEA_memcpy( void *ptr, const void *src,  long size );

int NMEA_strlen(const char* str);
char* NMEA_strcpyn(char* dest, const char* src, int dest_size);
int NMEA_sprintf( char *str, const char *fmt, ... );


#ifdef __cplusplus
}
#endif


#endif
