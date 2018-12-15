#ifndef __OEM_SA_STR_H_
#define __OEM_SA_STR_H_

#include "AT_Types.h"
#include <stdarg.h>


#ifdef __cplusplus
extern "C" {
#endif


typedef void * OEM_STR;


typedef atVOID		FN_OEM_SYSAPI_memset( atVOID *dst, atCHAR value, atLONG len );
typedef atVOID		FN_OEM_SYSAPI_memcpy( atVOID *dst, const atVOID *src, atLONG len );

typedef OEM_STR		FN_OEM_SYSAPI_CreateString( atLPTSTR szStr, atINT nStrLen, atINT nCapacity, atDWORD dwAttr );
typedef OEM_STR		FN_OEM_SYSAPI_AssignString( atLPVOID ptr, atINT nLen );
typedef atVOID		FN_OEM_SYSAPI_InitString( OEM_STR str, atLPTSTR szStr, atINT nStrLen, atINT nCapacity, atDWORD dwAttr );
typedef atVOID		FN_OEM_SYSAPI_DestroyString( OEM_STR str );
typedef atLPTSTR	FN_OEM_SYSAPI_GetString( OEM_STR str );
typedef atBOOL		FN_OEM_SYSAPI_SetString( OEM_STR str, atLPTSTR szStr, atINT nStrLen );

typedef atINT		FN_OEM_SYSAPI_strlen( atLPCSTR str );
typedef atCHAR *	FN_OEM_SYSAPI_strcpy( atLPSTR dstr, atLPCSTR sstr );
typedef atCHAR *	FN_OEM_SYSAPI_strncpy( atLPSTR dstr, atLPCSTR sstr, atINT len );
typedef atCHAR *	FN_OEM_SYSAPI_strcat( atLPSTR dstr, atLPCSTR sstr );
typedef atINT		FN_OEM_SYSAPI_strcmp( atLPCSTR str1, atLPCSTR str2 );
typedef atINT		FN_OEM_SYSAPI_strncmp( atLPCSTR str1, atLPCSTR str2, atINT len );
typedef atINT		FN_OEM_SYSAPI_stricmp( atLPCSTR str1, atLPCSTR str2 );
typedef atCHAR *	FN_OEM_SYSAPI_strchr( atLPCSTR str, atINT ch );
typedef atCHAR *	FN_OEM_SYSAPI_strstr( atLPSTR str1, atLPCSTR str2 );
typedef atCHAR *	FN_OEM_SYSAPI_strtok( atLPSTR str, atLPSTR sapr );
typedef atCHAR *	FN_OEM_SYSAPI_strupr( atLPSTR str );
typedef atCHAR *	FN_OEM_SYSAPI_strlwr( atLPSTR str );
typedef atINT 		FN_OEM_SYSAPI_atoi( atLPCSTR szIn );
typedef atLPSTR		FN_OEM_SYSAPI_itoa( atINT value, atLPSTR szOut, atINT radix );
typedef atINT		FN_OEM_SYSAPI_vsnprintf( atLPSTR str, atINT nLen, atLPCSTR fmt, va_list ap );
typedef atINT		FN_OEM_SYSAPI_vsnscanf( atLPSTR buf, atINT nLen, atLPCSTR fmt, va_list args );

typedef atINT		FN_OEM_SYSAPI_wcslen( atLPCWSTR str );
typedef atINT     	FN_OEM_SYSAPI_wcslenUCS2( const atUCS2_CHAR *str );
typedef atWCHAR *	FN_OEM_SYSAPI_wcscpy( atLPWSTR dstr, atLPCWSTR sstr );
typedef atWCHAR *	FN_OEM_SYSAPI_wcsncpy( atLPWSTR dstr, atLPCWSTR sstr, atINT len );
typedef atWCHAR *	FN_OEM_SYSAPI_wcscat( atLPWSTR dstr, atLPCWSTR sstr );
typedef atINT		FN_OEM_SYSAPI_wcscmp( atLPCWSTR str1, atLPCWSTR str2 );
typedef atINT		FN_OEM_SYSAPI_wcsncmp(atLPCWSTR str1, atLPCWSTR str2, atINT len);
typedef atINT		FN_OEM_SYSAPI_wcsicmp( atLPCWSTR str1, atLPCWSTR str2 );
typedef atWCHAR *	FN_OEM_SYSAPI_wcschr( atLPCWSTR str, atINT ch );
typedef atWCHAR *	FN_OEM_SYSAPI_wcsrchr( atLPCWSTR str, atINT ch );
typedef atWCHAR *	FN_OEM_SYSPAI_wcsstr( atLPWSTR str1, atLPCWSTR str2 );
typedef atWCHAR *	FN_OEM_SYSAPI_wcstok( atLPWSTR str, atLPWSTR sapr );
typedef atWCHAR *	FN_OEM_SYSAPI_wcsupr( atLPWSTR str );
typedef atWCHAR *	FN_OEM_SYSAPI_wcslwr( atLPWSTR str );
typedef atINT 		FN_OEM_SYSAPI_wtoi( atLPCWSTR szIn );
typedef atLPWSTR	FN_OEM_SYSAPI_itow( atINT value, atLPWSTR szOut, atINT radix );
typedef atINT		FN_OEM_SYSAPI_vsnwprintf( atLPWSTR str, atINT nLen, atLPCWSTR fmt, va_list ap );
typedef atINT		FN_OEM_SYSAPI_vsnwscanf( atLPWSTR buf, atINT nLen, atLPCWSTR fmt, va_list args );

typedef atINT 		FN_OEM_SYSAPI_UCS2ANSI( atLPSTR szOut, atLPCWSTR szIn, atINT nLen );
typedef atINT 		FN_OEM_SYSAPI_ANSI2UCS( atLPWSTR szOut, atLPCSTR szIn, atINT nLen );
typedef atINT  		FN_OEM_SYSAPI_UCS2_to_UCS4( atUCS4_CHAR *szOut, const atUCS2_CHAR *szIn, atINT nLen );
typedef atINT		FN_OEM_SYSAPI_UCS4_to_UCS2( atUCS2_CHAR *szOut, const atUCS4_CHAR *szIn, atINT nLen );


typedef struct tagOEM_SYSAPI_STR_T {
	FN_OEM_SYSAPI_memset			*fnMemset;
	FN_OEM_SYSAPI_memcpy			*fnMemcpy;

	FN_OEM_SYSAPI_CreateString		*fnCreateString;
	FN_OEM_SYSAPI_AssignString		*fnAssignString;
	FN_OEM_SYSAPI_InitString		*fnInitString;
	FN_OEM_SYSAPI_DestroyString		*fnDestroyString;
	FN_OEM_SYSAPI_GetString			*fnGetString;
	FN_OEM_SYSAPI_SetString			*fnSetString;

	FN_OEM_SYSAPI_strlen			*fnStrlen;
	FN_OEM_SYSAPI_strcpy			*fnStrcpy;
	FN_OEM_SYSAPI_strncpy			*fnStrncpy;
	FN_OEM_SYSAPI_strcat			*fnStrcat;
	FN_OEM_SYSAPI_strcmp			*fnStrcmp;
	FN_OEM_SYSAPI_strncmp			*fnStrncmp;
	FN_OEM_SYSAPI_stricmp			*fnStricmp;
	FN_OEM_SYSAPI_strchr			*fnStrchr;
	FN_OEM_SYSAPI_strstr			*fnStrstr;
	FN_OEM_SYSAPI_strtok			*fnStrtok;
	FN_OEM_SYSAPI_strupr			*fnStrupr;
	FN_OEM_SYSAPI_strlwr			*fnStrlwr;
	FN_OEM_SYSAPI_atoi				*fnAtoi;
	FN_OEM_SYSAPI_itoa				*fnItoa;
	FN_OEM_SYSAPI_vsnprintf			*fnVSnprintf;
	FN_OEM_SYSAPI_vsnscanf			*fnVSnscanf;

	FN_OEM_SYSAPI_wcslen			*fnWcslen;
	FN_OEM_SYSAPI_wcslenUCS2		*fnWcslenUCS2;
	FN_OEM_SYSAPI_wcscpy			*fnWcscpy;
	FN_OEM_SYSAPI_wcsncpy			*fnWcsncpy;
	FN_OEM_SYSAPI_wcscat			*fnWcscat;
	FN_OEM_SYSAPI_wcscmp			*fnWcscmp;
	FN_OEM_SYSAPI_wcsncmp			*fnWcsncmp;
	FN_OEM_SYSAPI_wcsicmp			*fnWcsicmp;
	FN_OEM_SYSAPI_wcschr			*fnWcschr;
	FN_OEM_SYSAPI_wcsrchr			*fnWcsrchr;
	FN_OEM_SYSPAI_wcsstr			*fnWcsstr;
	FN_OEM_SYSAPI_wcstok			*fnWcstok;
	FN_OEM_SYSAPI_wcsupr			*fnWcsupr;
	FN_OEM_SYSAPI_wcslwr			*fnWcslwr;
	FN_OEM_SYSAPI_wtoi				*fnWtoi;
	FN_OEM_SYSAPI_itow				*fnItow;
	FN_OEM_SYSAPI_vsnwprintf		*fnVSnwprintf;
	FN_OEM_SYSAPI_vsnwscanf			*fnVSnwscanf;

	FN_OEM_SYSAPI_UCS2ANSI			*fnUCS2ANSI;
	FN_OEM_SYSAPI_ANSI2UCS			*fnANSI2UCS;
	FN_OEM_SYSAPI_UCS2_to_UCS4		*fnUCS2_to_UCS4;
	FN_OEM_SYSAPI_UCS4_to_UCS2		*fnUCS4_to_UCS2;
} OEM_SYSAPI_STR_T;


atVOID	OEM_SYSAPI_STR_SetOemFn( OEM_SYSAPI_STR_T *fnOem );
atVOID	OEM_SYSAPI_STR_GetOemFn( OEM_SYSAPI_STR_T *fnOem );


#ifdef __cplusplus
}
#endif


#endif
