#include "SYSAPI.h"
#include "OEM_SA_Str.h"


atVOID     OEM_SYSAPI_memset( atVOID *dst, char value, atLONG len )
{
}

atVOID     OEM_SYSAPI_memcpy( atVOID *dst, const atVOID *src, atLONG len )
{
}


OEM_STR OEM_SYSAPI_CreateString( atLPTSTR szStr, atINT nStrLen, atINT nCapacity, atDWORD dwAttr )
{
	return (OEM_STR)atNULL;
}

OEM_STR OEM_SYSAPI_AssignString( atLPVOID ptr, atINT nLen )
{
	return (OEM_STR)atNULL;
}

atVOID  OEM_SYSAPI_InitString( OEM_STR oem_str, atLPTSTR szStr, atINT nStrLen, atINT nCapacity, atDWORD dwAttr )
{
}

atVOID	OEM_SYSAPI_DestroyString( OEM_STR oem_str )
{
}

atLPTSTR	OEM_SYSAPI_GetString( OEM_STR oem_str )
{
	return atNULL;
}

atBOOL	OEM_SYSAPI_SetString( OEM_STR oem_str, atLPTSTR szStr, atINT nStrLen )
{
	return atFALSE;
}



atINT   OEM_SYSAPI_strlen( const atLPSTR str )
{
  return 0;
}


atCHAR * OEM_SYSAPI_strcpy( atLPSTR dstr, const atLPSTR sstr )
{
  return atNULL;    
}


atCHAR * OEM_SYSAPI_strncpy( atLPSTR dstr, const atLPSTR sstr, atINT len )
{
  return atNULL;    
}

atCHAR  * OEM_SYSAPI_strcat( atLPSTR dstr, const atLPSTR sstr )
{
  return atNULL;    
}

atINT     OEM_SYSAPI_strcmp( const atLPSTR str1, const atLPSTR str2 )
{
  return 0;    
}

atINT     OEM_SYSAPI_stricmp( const atLPSTR str1, const atLPSTR str2 )
{
  return 0;    
}

atCHAR * OEM_SYSAPI_strchr( const atLPSTR str, atINT ch )
{
  return atNULL;    
}

atCHAR * OEM_SYSAPI_strtok( atLPSTR str, atLPSTR sapr )
{
  return atNULL;    
}

atINT OEM_SYSAPI_atoi( const atLPSTR szIn )
{
  return 0;    
}

atLPSTR OEM_SYSAPI_itoa( atINT value, atLPSTR szOut, atINT radix )
{
  return atNULL;    
}

atINT  OEM_SYSAPI_vsnprintf( atLPSTR str, atINT nLen, const atLPSTR fmt, va_list ap )
{
  return 0;    
}

int OEM_SYSAPI_vsnscanf( atLPSTR str, atINT nLen, const atLPSTR fmt, va_list ap )
{
  return 0;    
}


atINT   OEM_SYSAPI_wstrlen( const atLPWSTR str )
{
  return 0;    
}


atWCHAR * OEM_SYSAPI_wstrcpy( atLPWSTR dstr, const atLPWSTR sstr )
{
  return atNULL;    
}


atWCHAR * OEM_SYSAPI_wstrncpy( atLPWSTR dstr, const atLPWSTR sstr, atINT len )
{
  return atNULL;    
}

atWCHAR  * OEM_SYSAPI_wstrcat( atLPWSTR dstr, const atLPWSTR sstr )
{
  return atNULL;    
}

atINT     OEM_SYSAPI_wstrcmp( const atLPWSTR str1, const atLPWSTR str2 )
{
  return 0;    
}

atINT     OEM_SYSAPI_wstricmp( const atLPWSTR str1, const atLPWSTR str2 )
{
  return 0;    
}

atWCHAR * OEM_SYSAPI_wstrchr( const atLPWSTR str, atINT ch )
{
  return atNULL;
}

atWCHAR * OEM_SYSAPI_wstrtok( atLPWSTR str, atLPWSTR sapr )
{
	return atNULL;
}

atINT OEM_SYSAPI_watoi( const atLPWSTR szIn )
{
	return 0;
}

atLPWSTR OEM_SYSAPI_witoa( atINT value, atLPWSTR szOut, atINT radix )
{
	return atNULL;
}

atINT  OEM_SYSAPI_wvsnprintf( atLPWSTR str, atINT nLen, const atLPWSTR fmt, va_list ap )
{
	return 0;
}

int OEM_SYSAPI_wvsnscanf( atLPWSTR str, atINT nLen, const atLPWSTR fmt, va_list ap )
{
	return 0;
}

atINT OEM_SYSAPI_UCS2ANSI( atUCHAR *out, const atUSHORT *in, atINT nLen )
{
	return 0;
}

atINT OEM_SYSAPI_ANSI2UCS( atUSHORT *out, const atUCHAR *in, atINT nLen )
{
	return 0;
}
