#include "A_UIENG.h"
//#include <stdio.h>
#include <stdarg.h>
//#include <string.h>
#include <wchar.h>
#include "OEM_UI_str.h"


atVOID     DEF_OEM_UI_memset( atVOID *dst, char value, atLONG len )
{
	memset( dst, value, len );
}

atVOID     DEF_OEM_UI_memcpy( atVOID *dst, const atVOID *src, atLONG len )
{
	memcpy( dst, src, len );
}

atINT   DEF_OEM_UI_strlen( atLPCTSTR str )
{
	return tcslen( str );
}


atTCHAR * DEF_OEM_UI_strcpy( atLPTSTR dstr, atLPCTSTR sstr )
{
	return tcscpy(dstr, sstr);
}


atTCHAR * DEF_OEM_UI_strncpy( atLPTSTR dstr, atLPCTSTR sstr, atINT len )
{
	return tcsncpy( dstr, sstr, len );
}



atTCHAR  * DEF_OEM_UI_strcat( atLPTSTR dstr, atLPCTSTR sstr )
{
	return tcscat( dstr, sstr );
}


atINT     DEF_OEM_UI_strcmp( atLPCTSTR str1, atLPCTSTR str2 )
{
	return tcscmp( str1, str2 );
}

atINT     DEF_OEM_UI_stricmp( atLPCTSTR str1, atLPCTSTR str2 )
{
	return tcsicmp( str1, str2 );
}

atTCHAR * DEF_OEM_UI_strchr( atLPCTSTR str, atINT ch )
{
	return tcschr( str, ch );
}


atINT  DEF_OEM_UI_vsprintf( atLPTSTR str, atLPCTSTR fmt, va_list ap )
{
	return vstprintf( str, fmt, ap );
}


atINT DEF_OEM_UI_UCS2ANSI( atLPSTR out, const atLPWSTR in, atINT nLen )
{
	return SYSAPI_UCS2ANSI( out, in, nLen );
}


atINT DEF_OEM_UI_ANSI2UCS( atLPWSTR out, const atLPSTR in, atINT nLen )
{
	return SYSAPI_ANSI2UCS( out, in, nLen );
}


atINT DEF_OEM_UI_atoi( atLPCTSTR szIn )
{
	return _ttoi( szIn );
}


atLPTSTR DEF_OEM_UI_itoa( atINT value, atLPTSTR szOut, atINT radix )
{
	return _itot( value, szOut, radix );
}
