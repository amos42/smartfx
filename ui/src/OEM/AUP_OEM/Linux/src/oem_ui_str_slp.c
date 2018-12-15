#include "A_UIENG.h"
//#include <stdio.h>
#include <stdarg.h>
//#include <string.h>
//#include <wchar.h>
#include "OEM_UI_Str.h"
#include "SYSAPI.h"


atVOID     OEM_UI_memset( atVOID *dst, char value, atLONG len )
{
	SYSAPI_memset( dst, value, len );
}

atVOID     OEM_UI_memcpy( atVOID *dst, const atVOID *src, atLONG len )
{
	SYSAPI_memcpy( dst, src, len );
}

atINT   OEM_UI_strlen( const atLPTSTR str )
{
	return SYSAPI_tcslen( str );
}


atTCHAR * OEM_UI_strcpy( atLPTSTR dstr, const atLPTSTR sstr )
{
	return SYSAPI_tcscpy( dstr, sstr );
}


atTCHAR * OEM_UI_strncpy( atLPTSTR dstr, const atLPTSTR sstr, atINT len )
{
	return SYSAPI_tcsncpy( dstr, sstr, len );
}



atTCHAR  * OEM_UI_strcat( atLPTSTR dstr, const atLPTSTR sstr )
{
	return SYSAPI_tcscat( dstr, sstr );
}


atINT     OEM_UI_strcmp( const atLPTSTR str1, const atLPTSTR str2 )
{
	return SYSAPI_tcscmp( str1, str2 );
}

atTCHAR * OEM_UI_strchr( const atLPTSTR str, atINT ch )
{
	return SYSAPI_tcschr( str, ch );
}


atINT  OEM_UI_vsprintf( atLPTSTR str, const atLPTSTR fmt, va_list ap )
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


atINT OEM_UI_atoi( const atLPTSTR szIn )
{
	return SYSAPI_ttoi( szIn );
}


atLPTSTR OEM_UI_itoa( atINT value, atLPTSTR szOut, atINT radix )
{
	return SYSAPI_itot( value, szOut, radix );
}
