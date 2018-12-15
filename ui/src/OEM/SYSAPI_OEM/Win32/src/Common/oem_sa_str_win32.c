#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
//#include <mbstring.h>
#include "SYSAPI.h"
#include "OEM_SA_str.h"
//#include <malloc.h>
//#ifdef __USES_UNICODE_
#include <wchar.h>
//#endif
#include <windows.h>


typedef struct __tagSTR {
	atDWORD dwAttr;
	atSHORT  nCapacity, nStrLen;
	atTCHAR *lpszStr;
} _STR;


static atVOID     OEM_SYSAPI_memset( atVOID *dst, char value, atLONG len )
{
	memset( dst, value, len );
}

static atVOID     OEM_SYSAPI_memcpy( atVOID *dst, const atVOID *src, atLONG len )
{
	memcpy( dst, src, len );
}


static atVOID  OEM_SYSAPI_InitString( OEM_STR oem_str, atLPTSTR szStr, atINT nStrLen, atINT nCapacity, atDWORD dwAttr )
{
	_STR *str;
	if( oem_str == atNULL ) return;
	
	str = (_STR *)oem_str;
	str->nCapacity = nCapacity;
	str->dwAttr = dwAttr;

	OEM_SYSAPI_SetString( oem_str, szStr, nStrLen );
}

static OEM_STR OEM_SYSAPI_AssignString( atLPVOID ptr, atINT nLen )
{
	_STR *str;
	
	if( ptr == atNULL ) return (OEM_STR)atNULL;
	str = (_STR *)ptr;
	str->lpszStr = (atLPTSTR)((long)ptr + sizeof(_STR));

	OEM_SYSAPI_InitString( str, atNULL, 0, nLen - sizeof(_STR), 0x0000 );

	return (OEM_STR)str;
}

static OEM_STR OEM_SYSAPI_CreateString( atLPTSTR szStr, atINT nStrLen, atINT nCapacity, atDWORD dwAttr )
{
	_STR *str;

	str = (_STR *)SYSAPI_AllocMem( sizeof(_STR) + nCapacity );
	OEM_SYSAPI_AssignString( (atLPVOID)str,  sizeof(_STR) + nCapacity );
	str->dwAttr = SYS_STRATTR_OWERMEM;

	return (OEM_STR)str;
}

static atVOID	OEM_SYSAPI_DestroyString( OEM_STR oem_str )
{
	_STR *str;
	if( oem_str == atNULL ) return;
	str = (_STR *)oem_str;
	
	if( str->dwAttr & SYS_STRATTR_OWERSTRBUF ){
		SYSAPI_FreeMem( str->lpszStr );
		str->lpszStr = atNULL;
	}
	
	if( str->dwAttr & SYS_STRATTR_OWERMEM ){
		SYSAPI_FreeMem( str );
	} else {
		str->dwAttr = 0x0000;
	}
}

static atLPTSTR	OEM_SYSAPI_GetString( OEM_STR oem_str )
{
	if( oem_str == atNULL ) return (atLPTSTR)atNULL;
	
	return ((_STR *)oem_str)->lpszStr;
}

static atBOOL	OEM_SYSAPI_SetString( OEM_STR oem_str, atLPTSTR szStr, atINT nStrLen )
{
	_STR *str;
	if( oem_str == atNULL ) return atFALSE;
	str = (_STR *)oem_str;

	if( szStr != atNULL ){
		if( nStrLen == -1 ) nStrLen = SYSAPI_tcslen( szStr );
		if( nStrLen > str->nCapacity ){
			if( str->dwAttr & SYS_STRATTR_OWERMEM ){
				SYSAPI_FreeMem( str->lpszStr );
				str->lpszStr = (atLPTSTR)SYSAPI_AllocMem( nStrLen + 1 );
			} else {
				str->lpszStr = (atLPTSTR)SYSAPI_AllocMem( nStrLen + 1 );
				str->dwAttr |= SYS_STRATTR_OWERMEM;
			}
			str->nCapacity = (atSHORT)nStrLen;
		}
		SYSAPI_tcsncpy( str->lpszStr, szStr, nStrLen );
	} else {
		str->lpszStr[0] = _ATC('\0');
		str->nStrLen = 0;
	}

	return atTRUE;
}



static atINT   OEM_SYSAPI_strlen( atLPCSTR str )
{
    return strlen( str );
}

static atINT   OEM_SYSAPI_wcslen( atLPCWSTR str )
{
    return wcslen( str );
}

static atINT  OEM_SYSAPI_wcslenUCS2( const atUCS2_CHAR *str )
{
	int len = 0;
	atUCS2_CHAR *ptr, ch;
	
	ptr = (atUCS2_CHAR *)str;
	while( (ch = *ptr++) != (atUCS2_CHAR)'\0' ) len++;
	
	return len;
}


static atCHAR * OEM_SYSAPI_strcpy( atLPSTR dstr, atLPCSTR sstr )
{
	return strcpy( dstr, sstr );
}

static atWCHAR * OEM_SYSAPI_wcscpy( atLPWSTR dstr, atLPCWSTR sstr )
{
    return wcscpy( dstr, sstr );
}


static atCHAR * OEM_SYSAPI_strncpy( atLPSTR dstr, atLPCSTR sstr, atINT len )
{
    atCHAR *s = strncpy( dstr, sstr, len );
    int len0 = strlen( sstr );
    dstr[MIN(len,len0)] = '\0';
    return dstr;
}

static atWCHAR * OEM_SYSAPI_wcsncpy( atLPWSTR dstr, atLPCWSTR sstr, atINT len )
{
    atWCHAR *s = wcsncpy( dstr, sstr, len );
    int len0 = wcslen( sstr );
    dstr[MIN(len,len0)] = L'\0';
    return dstr;
}


static atCHAR  * OEM_SYSAPI_strcat( atLPSTR dstr, atLPCSTR sstr )
{
	return strcat( dstr, sstr );
}

static atWCHAR  * OEM_SYSAPI_wcscat( atLPWSTR dstr, atLPCWSTR sstr )
{
    return wcscat( dstr, sstr );
}


static atINT     OEM_SYSAPI_strcmp( atLPCSTR str1, atLPCSTR str2 )
{
	return strcmp( str1, str2 );
}

static atINT     OEM_SYSAPI_wcscmp( atLPCWSTR str1, atLPCWSTR str2 )
{
    return wcscmp( str1, str2 );
}


static atINT     OEM_SYSAPI_strncmp( atLPCSTR str1, atLPCSTR str2, atINT len )
{
	return strncmp( str1, str2, len );
}

static atINT     OEM_SYSAPI_wcsncmp( atLPCWSTR str1, atLPCWSTR str2, atINT len )
{
    return wcsncmp( str1, str2, len );
}


static atINT     OEM_SYSAPI_stricmp( atLPCSTR str1, atLPCSTR str2 )
{
	atINT   diff;
	atCHAR* pTmp1, *pTmp2;

	pTmp1 = (atCHAR*)str1;
	pTmp2 = (atCHAR*)str2;
	
	while(pTmp1 && pTmp2)
	{
		diff = tolower(*pTmp1) - tolower(*pTmp2);
		if(diff) return diff;

		pTmp1++;
		pTmp2++;
	}

	return (*pTmp1 - *pTmp2);
	
	// fix-me: stricmp 사용시 CE에서 컴파일 에러남...
	//return stricmp( str1, str2 );
	//return _stricmp( str1, str2 );
}

static atWCHAR SA_towlower( atWCHAR c )
{
	return ( ((unsigned int)c >= (unsigned int)L'A') && ((unsigned int)c <= (unsigned int)L'Z') )?	
		(atWCHAR)((unsigned int)L'a' + ((unsigned int)c - (unsigned int)L'A')) : c;
}

static atINT     OEM_SYSAPI_wcsicmp( atLPCWSTR str1, atLPCWSTR str2 )
{
	atINT diff;
	atWCHAR *s1 = (atWCHAR *)str1;
	atWCHAR *s2 = (atWCHAR *)str2;

	while(*s1 && *s2)
	{
		diff = SA_towlower(*s1) - SA_towlower(*s2);
		if(diff) return diff;

		s1++;
		s2++;
	}

	return (*s1 - *s2);		
//    return wcsicmp( str1, str2 );
}


static atCHAR * OEM_SYSAPI_strchr( atLPCSTR str, atINT ch )
{
	return strchr( str, ch);
}

static atWCHAR * OEM_SYSAPI_wcschr( atLPCWSTR str, atINT ch )
{
    return wcschr( str, (wchar_t)ch );
}

static atWCHAR * OEM_SYSAPI_wcsrchr( atLPCWSTR str, atINT ch )
{
    return wcsrchr( str, (atWCHAR)ch );
}


static atCHAR * OEM_SYSAPI_strstr( atLPSTR str1, atLPCSTR str2 )
{
	return strstr(str1, str2);
}

static atWCHAR * OEM_SYSPAI_wcsstr( atLPWSTR str1, atLPCWSTR str2 )
{
	return wcsstr(str1, str2);
}


static atCHAR * OEM_SYSAPI_strtok( atLPSTR str, atLPSTR sapr )
{
	return strtok( str, sapr );
}

static atWCHAR * OEM_SYSAPI_wcstok( atLPWSTR str, atLPWSTR sapr )
{
	return (atWCHAR *)wcstok( str, (const wchar_t *)sapr, NULL );
}


static atINT OEM_SYSAPI_atoi( atLPCSTR szIn )
{
	return atoi( szIn );
}

static atINT OEM_SYSAPI_wtoi( atLPCWSTR szIn )
{
	int num = 0;
	atWCHAR *ptr, ch;
	
	ptr = (atWCHAR *)szIn;
	while( (ch = *ptr++) != L'\0' ){
	 	num = (num*10) + (ch - L'0');
	}
	
	return num;
}


static atLPSTR OEM_SYSAPI_itoa( atINT value, atLPSTR szOut, atINT radix )
{
	return _itoa( value, szOut, radix );
}

static atLPWSTR OEM_SYSAPI_itow( atINT value, atLPWSTR szOut, atINT radix )
{
	static const atWCHAR digit[] = L"0123456789ABCDEF";
    unsigned char buf[34], *bufp, c;
	atWCHAR* out;
	int quotient = value; 

	if( (radix < 2) || (radix > 16) ) return atNULL;
	
	out = szOut; 
	if( value < 0 ){
		*out++ = L'-';
		value = -value;
	}

	bufp = buf;
    do {
        *bufp++ = (unsigned char)(value % radix);
		value /= radix;
    } while( value != 0 );

    while ( bufp != buf ){
	  c = *--bufp;
      *out++ = digit[c];
    }
	
	*out = L'\0'; 
	
	return szOut; 
}


static atCHAR* OEM_SYSAPI_strupr( atLPSTR str )
{
	atCHAR* pTmp = str;
	while (*pTmp != '\0')
	{
		if(*pTmp >= 'a' && *pTmp <= 'z')
		{
			*pTmp = *pTmp - ('a' - 'A');
		}
		pTmp++;
	}
	return str;
}

static atWCHAR * OEM_SYSAPI_wcsupr( atLPWSTR str )
{
	atWCHAR* pTmp = str;
	while (*pTmp != L'\0')
	{
		if(*pTmp >= L'a' && *pTmp <= L'z')
		{
			*pTmp = *pTmp - (L'a' - L'A');
		}
		pTmp++;
	}
	return str;
}


static atCHAR * OEM_SYSAPI_strlwr( atLPSTR str )
{
	atCHAR* pTmp= (atCHAR *)str;
	while (*pTmp)
	{
		*pTmp= tolower(*pTmp);
		pTmp++;
	}
	return str;
}

static atWCHAR * OEM_SYSAPI_wcslwr( atLPWSTR str )
{
	atWCHAR* pTmp = (atWCHAR *)str;
	while (*pTmp)
	{
		*pTmp= tolower(*pTmp);
		pTmp++;
	}
	return str;
}


//#define __USES_FORCE_UNICODE_
#include "oem_sa_str_vsnprintf.inl"
static atINT  OEM_SYSAPI_vsnprintf( atLPSTR str, atINT nLen, atLPCSTR fmt, va_list ap )
{
    return _GLOBAL_FUNC_(my_vsnprintf)( (char *)str, nLen, (const char *)fmt, ap );
}
//#undef __USES_FORCE_UNICODE_

#define __USES_FORCE_UNICODE_
#include "oem_sa_str_vsnprintf.inl"
static atINT  OEM_SYSAPI_vsnwprintf( atLPWSTR str, atINT nLen, atLPCWSTR fmt, va_list ap )
{
	return _GLOBAL_FUNC_(my_vsnprintf)( str, nLen, fmt, ap );
}
#undef __USES_FORCE_UNICODE_


//#define __USES_FORCE_UNICODE_
#include "oem_sa_str_vsnscanf.inl"
static int OEM_SYSAPI_vsnscanf( atLPSTR str, atINT nLen, atLPCSTR fmt, va_list ap )
{
	return _GLOBAL_FUNC_(my_vsnscanf)( (char *)str, nLen, (atLPCSTR )fmt, ap );
}
//#undef __USES_FORCE_UNICODE_

#define __USES_FORCE_UNICODE_
#include "oem_sa_str_vsnscanf.inl"
static int OEM_SYSAPI_vsnwscanf( atLPWSTR str, atINT nLen, atLPCWSTR fmt, va_list ap )
{
	return _GLOBAL_FUNC_(my_vsnscanf)( str, nLen, fmt, ap );
}
#undef __USES_FORCE_UNICODE_


static atINT  OEM_SYSAPI_UCS2ANSI( atLPSTR szOut, atLPCWSTR szIn, atINT nLen )
{
/*	
	int cnt = 0;
	atUCS_CHAR *lpIn = szIn;
	atANSI_CHAR *lpOut = szOut;
	atUCS_CHAR ch;
	
	if( nLen <= 0 ) return 0;
	
	while( (ch = *lpIn++) != L'\0' ){
		*lpOut++ = (unsigned char)((ch) & 0xff);
		cnt++;
	}
	
	*lpOut = '\0';

	return cnt;
*/	
	int cnt;
	if( nLen <= 0 ) return 0;
	cnt = WideCharToMultiByte(CP_OEMCP, 0, szIn, -1, szOut, 0, NULL, NULL);
	cnt = WideCharToMultiByte(CP_OEMCP, 0, szIn, -1, szOut, cnt, NULL, NULL);
	szOut[cnt] = '\0';
	return cnt;
}

static atINT  OEM_SYSAPI_ANSI2UCS( atLPWSTR szOut, atLPCSTR szIn, atINT nLen )
{
/*	
	int cnt = 0;
	atANSI_CHAR *lpIn = szIn;
	atUCS_CHAR *lpOut = szOut;

	if( nLen <= 0 ) return 0;
	
	while ( (*lpIn) != 0x00 && (*lpIn) != '\0' && nLen)
	{
		*lpOut = (atUCS_CHAR)(*lpIn);

		lpIn++;	nLen--;
		lpOut++;	cnt++;
	}

	*lpOut = L'\0';

	return cnt;
*/	
	int cnt;
	if( nLen <= 0 ) return 0;
	cnt = MultiByteToWideChar(CP_OEMCP, MB_PRECOMPOSED, szIn, nLen, szOut, nLen);
	szOut[cnt] = L'\0';
	return cnt;
}

static atINT  OEM_SYSAPI_UCS2_to_UCS4( atUCS4_CHAR *szOut, const atUCS2_CHAR *szIn, atINT nLen )
{
	int cnt = 0;
	atUCS2_CHAR *lpIn = (atUCS2_CHAR *)szIn;
	atUCS4_CHAR *lpOut = szOut;

	if( nLen <= 0 ) return 0;
	
	while ( (*lpIn) != 0x00 && (*lpIn) != (atUCS2_CHAR)'\0' && nLen)
	{
		*lpOut = (atUCS4_CHAR)(*lpIn);

		lpIn++;	nLen--;
		lpOut++;	cnt++;
	}

	*lpOut = (atUCS4_CHAR)'\0';

	return cnt;
}

static atINT  OEM_SYSAPI_UCS4_to_UCS2( atUCS2_CHAR *szOut, const atUCS4_CHAR *szIn, atINT nLen )
{
	int cnt = 0;
	atUCS4_CHAR *lpIn = (atUCS4_CHAR *)szIn;
	atUCS2_CHAR *lpOut = szOut;

	if( nLen <= 0 ) return 0;
	
	while ( (*lpIn) != 0x00 && (*lpIn) != (atUCS4_CHAR)'\0' && nLen)
	{
		*lpOut = (atUCS2_CHAR)(*lpIn);

		lpIn++;	nLen--;
		lpOut++;	cnt++;
	}

	*lpOut = (atUCS2_CHAR)'\0';

	return cnt;
}



void init_sysapi_str_oem() 
{
	OEM_SYSAPI_STR_T oem_funcs = {
		OEM_SYSAPI_memset,
		OEM_SYSAPI_memcpy,

		OEM_SYSAPI_CreateString,
		OEM_SYSAPI_AssignString,
		OEM_SYSAPI_InitString,
		OEM_SYSAPI_DestroyString,
		OEM_SYSAPI_GetString,
		OEM_SYSAPI_SetString,

		OEM_SYSAPI_strlen,
		OEM_SYSAPI_strcpy,
		OEM_SYSAPI_strncpy,
		OEM_SYSAPI_strcat,
		OEM_SYSAPI_strcmp,
		OEM_SYSAPI_strncmp,
		OEM_SYSAPI_stricmp,
		OEM_SYSAPI_strchr,
		OEM_SYSAPI_strstr,
		OEM_SYSAPI_strtok,
		OEM_SYSAPI_strupr,
		OEM_SYSAPI_strlwr,
		OEM_SYSAPI_atoi,
		OEM_SYSAPI_itoa,
		OEM_SYSAPI_vsnprintf,
		OEM_SYSAPI_vsnscanf,

		OEM_SYSAPI_wcslen,
		OEM_SYSAPI_wcslenUCS2,
		OEM_SYSAPI_wcscpy,
		OEM_SYSAPI_wcsncpy,
		OEM_SYSAPI_wcscat,
		OEM_SYSAPI_wcscmp,
		OEM_SYSAPI_wcsncmp,
		OEM_SYSAPI_wcsicmp,
		OEM_SYSAPI_wcschr,
		OEM_SYSAPI_wcsrchr,
		OEM_SYSPAI_wcsstr,
		OEM_SYSAPI_wcstok,
		OEM_SYSAPI_wcsupr,
		OEM_SYSAPI_wcslwr,
		OEM_SYSAPI_wtoi,
		OEM_SYSAPI_itow,
		OEM_SYSAPI_vsnwprintf,
		OEM_SYSAPI_vsnwscanf,

		OEM_SYSAPI_UCS2ANSI,
		OEM_SYSAPI_ANSI2UCS,
		OEM_SYSAPI_UCS2_to_UCS4,
		OEM_SYSAPI_UCS4_to_UCS2
	};

	OEM_SYSAPI_STR_SetOemFn( &oem_funcs );
}
