#ifndef __SA_STR_H_
#define __SA_STR_H_

#include "SA_Types.h"
#include <stdarg.h>


#ifdef __cplusplus
extern "C" {
#endif


typedef void * SYS_STR;


#define SYS_STRATTR_OWERMEM		0x8000
#define SYS_STRATTR_OWERSTRBUF	0x8000
#define SYS_STRATTR_AUTOEXTEND	0x4000


atVOID    SYSAPI_memset( atVOID *dst, atCHAR value, atLONG len );
atVOID    SYSAPI_memcpy( atVOID *dst, const atVOID *src, atLONG len );


SYS_STR SYSAPI_CreateString( atLPTSTR szStr, atINT nStrLen, atINT nCapacity, atDWORD dwAttr );
SYS_STR SYSAPI_AssignString( atLPVOID ptr, atINT nLen );
atVOID  SYSAPI_InitString( SYS_STR str, atLPTSTR szStr, atINT nStrLen, atINT nCapacity, atDWORD dwAttr );
atVOID	SYSAPI_DestroyString( SYS_STR str );
atLPTSTR	SYSAPI_GetString( SYS_STR str );
atBOOL	SYSAPI_SetString( SYS_STR str, atLPTSTR szStr, atINT nStrLen );


atINT    	SYSAPI_strlen( atLPCSTR str );
atINT     SYSAPI_wcslen( atLPCWSTR str );
atINT    	SYSAPI_wcslenUCS2( const atUCS2_CHAR *str );

atCHAR * 	SYSAPI_strcpy( atLPSTR dstr, atLPCSTR sstr );
atWCHAR * SYSAPI_wcscpy( atLPWSTR dstr, atLPCWSTR sstr );

atCHAR * 	SYSAPI_strncpy( atLPSTR dstr, atLPCSTR sstr, atLONG len );
atWCHAR * SYSAPI_wcsncpy( atLPWSTR dstr, atLPCWSTR sstr, atLONG len );

atCHAR *	SYSAPI_strcat( atLPSTR dstr, atLPCSTR sstr );
atWCHAR * SYSAPI_wcscat( atLPWSTR dstr, atLPCWSTR sstr );

atINT    	SYSAPI_strcmp( atLPCSTR str1, atLPCSTR str2 );
atINT     SYSAPI_wcscmp( atLPCWSTR str1, atLPCWSTR str2 );

atINT	 	  SYSAPI_strncmp( atLPCSTR str1, atLPCSTR str2, atINT len );
atINT	 	  SYSAPI_wcsncmp( atLPCWSTR str1, atLPCWSTR str2, atINT len );

atINT    	SYSAPI_stricmp( atLPCSTR str1, atLPCSTR str2 );
atINT     SYSAPI_wcsicmp( atLPCWSTR str1, atLPCWSTR str2 );

atCHAR * 	SYSAPI_strchr( atLPCSTR str, atINT ch );
atWCHAR * SYSAPI_wcschr( atLPCWSTR str, atINT ch );
atWCHAR * SYSAPI_wcsrchr( atLPCWSTR str, atINT ch );

atCHAR * 	SYSAPI_strstr( atLPSTR str1, atLPCSTR str2);
atWCHAR * SYSAPI_wcsstr( atLPWSTR str1, atLPCWSTR str2);

atCHAR * 	SYSAPI_strtok( atLPSTR str, atLPSTR sapr );
atWCHAR * SYSAPI_wcstok( atLPWSTR str, atLPWSTR sapr );

atCHAR *  SYSAPI_strupr( atLPSTR str );
atWCHAR * SYSAPI_wcsupr( atLPWSTR str );

atCHAR *  SYSAPI_strlwr( atLPSTR str );
atWCHAR * SYSAPI_wcslwr( atLPWSTR str );

atINT 	  SYSAPI_atoi( atLPCSTR szIn );
atINT 	  SYSAPI_wtoi( atLPCWSTR szIn );

atLPSTR   SYSAPI_itoa( atINT value, atLPSTR szOut, atINT radix );
atLPWSTR  SYSAPI_itow( atINT value, atLPWSTR szOut, atINT radix );

atCHAR *  SYSAPI_strins( atLPSTR dstr, atLPCSTR sstr, atINT nIndex, atINT nInsLen );
atWCHAR * SYSAPI_wcsins( atLPWSTR dstr, atLPCWSTR sstr, atINT nIndex, atINT nInsLen );

atCHAR *  SYSAPI_strdel( atLPSTR dstr, atINT nIndex, atINT nDelLen );
atWCHAR * SYSAPI_wcsdel( atLPWSTR dstr, atINT nIndex, atINT nDelLen );

atINT     SYSAPI_vsprintf( atLPSTR str, atLPCSTR fmt, va_list ap );
atINT     SYSAPI_vswprintf( atLPWSTR str, atLPCWSTR fmt, va_list ap );

atINT     SYSAPI_vsnprintf( atLPSTR str, atINT nLen, atLPCSTR fmt, va_list ap );
atINT     SYSAPI_vsnwprintf( atLPWSTR str, atINT nLen, atLPCWSTR fmt, va_list ap );

atINT     SYSAPI_sprintf( atLPSTR str, atLPCSTR fmt, ... );
atINT     SYSAPI_swprintf( atLPWSTR str, atLPCWSTR fmt, ... );

atINT     SYSAPI_snprintf( atLPSTR str, atINT nLen, atLPCSTR fmt, ... );
atINT     SYSAPI_snwprintf( atLPWSTR str, atINT nLen, atLPCWSTR fmt, ... );

atINT	  	SYSAPI_vsscanf( atLPSTR dstr, atLPCSTR fmt, va_list ap );
atINT	    SYSAPI_vswscanf( atLPWSTR dstr, atLPCWSTR fmt, va_list ap );

atINT	  	SYSAPI_vsnscanf( atLPSTR dstr, atINT nLen, atLPCSTR fmt, va_list ap );
atINT	    SYSAPI_vsnwscanf( atLPWSTR dstr, atINT nLen, atLPCWSTR fmt, va_list ap );

atINT	  	SYSAPI_sscanf( atLPSTR dstr, atLPCSTR fmt, ... );
atINT	    SYSAPI_wsscanf( atLPWSTR dstr, atLPCWSTR fmt, ... );

atINT	  	SYSAPI_snscanf( atLPSTR dstr, atINT nLen, atLPCSTR fmt, ... );
atINT	    SYSAPI_snwscanf( atLPWSTR dstr, atINT nLen, atLPCWSTR fmt, ... );


#if defined(__USES_UNICODE_)

#define SYSAPI_tcslen   SYSAPI_wcslen
#define SYSAPI_tcscpy	SYSAPI_wcscpy
#define SYSAPI_tcsncpy	SYSAPI_wcsncpy
#define SYSAPI_tcscat	SYSAPI_wcscat
#define SYSAPI_tcscmp	SYSAPI_wcscmp
#define SYSAPI_tcsncmp	SYSAPI_wcsncmp
#define SYSAPI_tcschr	SYSAPI_wcschr
#define SYSAPI_ttoi		SYSAPI_wtoi
#define SYSAPI_itot		SYSAPI_itow
#define SYSAPI_tcsupr	SYSAPI_wcsupr

#define SYSAPI_stprintf SYSAPI_swprintf
#define SYSAPI_vstprintf SYSAPI_vswprintf
#define SYSAPI_tcsstr 	SYSAPI_wcsstr
#define SYSAPI_tcsicmp	SYSAPI_wcsicmp

#else

#define SYSAPI_tcslen   SYSAPI_strlen
#define SYSAPI_tcscpy   SYSAPI_strcpy
#define SYSAPI_tcsncpy	SYSAPI_strncpy
#define SYSAPI_tcscat	SYSAPI_strcat
#define SYSAPI_tcscmp	SYSAPI_strcmp
#define SYSAPI_tcsncmp	SYSAPI_strncmp
#define SYSAPI_tcschr	SYSAPI_strchr

#define SYSAPI_ttoi		SYSAPI_atoi
#define SYSAPI_itot		SYSAPI_itoa
#define SYSAPI_tcsupr	SYSAPI_strupr

#define SYSAPI_stprintf SYSAPI_sprintf
#define SYSAPI_vstprintf SYSAPI_vsprintf
#define SYSAPI_tcsstr 	SYSAPI_strstr
#define SYSAPI_tcsicmp	SYSAPI_stricmp

#endif


atINT 	  SYSAPI_UCS2ANSI( _OUT_ atLPSTR szOut, _IN_ atLPCWSTR szIn, atINT nLen );
atINT 	  SYSAPI_ANSI2UCS( _OUT_ atLPWSTR szOut, _IN_ atLPCSTR szIn, atINT nLen );
atINT 	  SYSAPI_ToANSI( _OUT_ atLPSTR szOut, _IN_ atLPCTSTR szIn, atINT nLen );
atINT 	  SYSAPI_ToUCS(  _OUT_ atLPWSTR szOut, _IN_ atLPCTSTR szIn, atINT nLen );
atINT 	  SYSAPI_ToUCS2(  _OUT_ atUCS2_CHAR *szOut, _IN_ atLPCTSTR szIn, atINT nLen );
atINT 	  SYSAPI_FromANSI( _OUT_ atLPTSTR szOut, _IN_ atLPCSTR szIn, atINT nLen );
atINT 	  SYSAPI_FromUCS(  _OUT_ atLPTSTR szOut, _IN_ atLPCWSTR szIn, atINT nLen );
atINT 	  SYSAPI_FromUCS2(  _OUT_ atLPTSTR szOut, _IN_ const atUCS2_CHAR *szIn, atINT nLen );


#ifdef __cplusplus
}
#endif


#endif
