#include "A_UIENG.h"
//#include <stdio.h>
#include <stdarg.h>
//#include <string.h>
//#include <wchar.h>
#include "OEM_UI_str.h"
#include "SYSAPI.h"


atVOID     OEM_UI_memset( atVOID *dst, char value, atLONG len )
{
	SYSAPI_memset( dst, value, len );
}

atVOID     OEM_UI_memcpy( atVOID *dst, const atVOID *src, atLONG len )
{
	SYSAPI_memcpy( dst, src, len );
}

atINT   OEM_UI_strlen( atLPCTSTR str )
{
	return SYSAPI_tcslen( str );
}


atTCHAR * OEM_UI_strcpy( atLPTSTR dstr, atLPCTSTR sstr )
{
	return SYSAPI_tcscpy( dstr, sstr );
}


atTCHAR * OEM_UI_strncpy( atLPTSTR dstr, atLPCTSTR sstr, atINT len )
{
	return SYSAPI_tcsncpy( dstr, sstr, len );
}



atTCHAR  * OEM_UI_strcat( atLPTSTR dstr, atLPCTSTR sstr )
{
	return SYSAPI_tcscat( dstr, sstr );
}


atINT     OEM_UI_strcmp( atLPCTSTR str1, atLPCTSTR str2 )
{
	return SYSAPI_tcscmp( str1, str2 );
}

atINT     OEM_UI_stricmp( atLPCTSTR str1, atLPCTSTR str2 )
{
	return SYSAPI_tcsicmp( str1, str2 );
}

atTCHAR * OEM_UI_strchr( atLPCTSTR str, atINT ch )
{
	return SYSAPI_tcschr( str, ch );
}


atINT  OEM_UI_vsprintf( atLPTSTR str, atLPCTSTR fmt, va_list ap )
{
	return SYSAPI_vstprintf( str, fmt, ap );
}


atINT OEM_UI_UCS2ANSI( atLPSTR out, const atLPWSTR in, atINT nLen )
{
	return SYSAPI_UCS2ANSI( out, in, nLen );
}


atINT OEM_UI_ANSI2UCS( atLPWSTR out, const atLPSTR in, atINT nLen )
{
	return SYSAPI_ANSI2UCS( out, in, nLen );
}


atINT OEM_UI_atoi( atLPCTSTR szIn )
{
	return SYSAPI_ttoi( szIn );
}


atLPTSTR OEM_UI_itoa( atINT value, atLPTSTR szOut, atINT radix )
{
	return SYSAPI_itot( value, szOut, radix );
}


void init_uiapi_str_oem() 
{
	OEM_UI_STR_T oem_funcs = {
		OEM_UI_memset,
		OEM_UI_memcpy,
		OEM_UI_strlen,
		OEM_UI_strcpy,
		OEM_UI_strncpy,
		OEM_UI_strcat,
		OEM_UI_strcmp,
		OEM_UI_stricmp,
		OEM_UI_strchr,
		OEM_UI_vsprintf,
		OEM_UI_atoi,
		OEM_UI_itoa,
		OEM_UI_UCS2ANSI,
		OEM_UI_ANSI2UCS
	};

	OEM_UI_Str_SetOemFn( &oem_funcs );
}
