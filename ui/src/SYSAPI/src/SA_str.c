#include "SA_Types.h"
#include "SYSAPI.h"
//#include <stdio.h>
#include <stdarg.h>
//#include <string.h>
#include "OEM_SA_Str.h"
//#ifdef __USES_UNICODE_
//#include <wchar.h>
//#endif


#ifdef __USES_SYSAPI_STR_


static OEM_SYSAPI_STR_T tOemFn;


atVOID OEM_SYSAPI_STR_SetOemFn(OEM_SYSAPI_STR_T *fnOem)
{
	tOemFn = *fnOem;
}


atVOID OEM_SYSAPI_STR_GetOemFn(OEM_SYSAPI_STR_T *fnOem)
{
	*fnOem = tOemFn;
}


atVOID     SYSAPI_memset( atVOID *dst, atCHAR value, atLONG len )
{
	if( tOemFn.fnMemset != atNULL ){
		tOemFn.fnMemset( dst, value, len );
	}
}

atVOID     SYSAPI_memcpy( atVOID *dst, const atVOID *src, atLONG len )
{
	if( tOemFn.fnMemcpy != atNULL ){
		tOemFn.fnMemcpy( dst, src, len );
	}
}


SYS_STR SYSAPI_CreateString( atLPTSTR szStr, atINT nStrLen, atINT nCapacity, atDWORD dwAttr )
{
	if( tOemFn.fnCreateString != atNULL ){
		return (SYS_STR)tOemFn.fnCreateString( szStr, nStrLen, nCapacity, dwAttr );
	}

	return (SYS_STR)atNULL;
}

SYS_STR SYSAPI_AssignString( atLPVOID ptr, atINT nLen )
{
	if( tOemFn.fnAssignString != atNULL ){
		return (SYS_STR)tOemFn.fnAssignString( ptr, nLen );
	}

	return (SYS_STR)atNULL;
}

atVOID  SYSAPI_InitString( SYS_STR str, atLPTSTR szStr, atINT nStrLen, atINT nCapacity, atDWORD dwAttr )
{
	if( tOemFn.fnInitString != atNULL ){
		tOemFn.fnInitString( str, szStr, nStrLen, nCapacity, dwAttr );
	}
}

atVOID	SYSAPI_DestroyString( SYS_STR str )
{
	if( tOemFn.fnDestroyString != atNULL ){
		tOemFn.fnDestroyString( str );
	}
}

atLPTSTR	SYSAPI_GetString( SYS_STR str )
{
	if( tOemFn.fnGetString != atNULL ){
		return tOemFn.fnGetString( str );
	}

	return (atLPTSTR)atNULL;
}

atBOOL	SYSAPI_SetString( SYS_STR str, atLPTSTR szStr, atINT nStrLen )
{
	if( tOemFn.fnSetString != atNULL ){
		return tOemFn.fnSetString( str, szStr, nStrLen );
	}

	return atFALSE;
}


/////////////////////////////////////////////////////////////////

atINT     SYSAPI_strlen( atLPCSTR str )
{
	if( tOemFn.fnStrlen != atNULL ){
		return tOemFn.fnStrlen( str );
	}

	return 0;
}

atINT     SYSAPI_wcslen( atLPCWSTR str )
{
	if( tOemFn.fnWcslen != atNULL ){
		return tOemFn.fnWcslen( str );
	}

	return 0;
}

atINT     	SYSAPI_wcslenUCS2( const atUCS2_CHAR *str )
{
	if( tOemFn.fnWcslenUCS2 != atNULL ){
		return tOemFn.fnWcslenUCS2( str );
	}

	return 0;
}


atCHAR * SYSAPI_strcpy( atLPSTR dstr, atLPCSTR sstr )
{
	if( tOemFn.fnStrcpy != atNULL ){
		return tOemFn.fnStrcpy( dstr, sstr );
	}

	return (atCHAR *)atNULL;
}

atWCHAR * SYSAPI_wcscpy( atLPWSTR dstr, atLPCWSTR sstr )
{
	if( tOemFn.fnWcscpy != atNULL ){
		return tOemFn.fnWcscpy( dstr, sstr );
	}

	return (atWCHAR *)atNULL;
}


atCHAR * SYSAPI_strncpy( atLPSTR dstr, atLPCSTR sstr, atLONG len )
{
	if( tOemFn.fnStrncpy != atNULL ){
		return tOemFn.fnStrncpy( dstr, sstr, len );
	}

	return (atCHAR *)atNULL;
}

atWCHAR * SYSAPI_wcsncpy( atLPWSTR dstr, atLPCWSTR sstr, atLONG len )
{
	if( tOemFn.fnWcsncpy != atNULL ){
 		return tOemFn.fnWcsncpy( dstr, sstr, len );
	}

	return (atWCHAR *)atNULL;
}

	
atCHAR * SYSAPI_strcat( atLPSTR dstr, atLPCSTR sstr )
{
	if( tOemFn.fnStrcat != atNULL ){
		return tOemFn.fnStrcat( dstr, sstr );
	}

	return (atCHAR *)atNULL;
}

atWCHAR * SYSAPI_wcscat( atLPWSTR dstr, atLPCWSTR sstr )
{
	if( tOemFn.fnWcscat != atNULL ){
		return tOemFn.fnWcscat( dstr, sstr );
	}

	return (atWCHAR *)atNULL;
}


atINT     SYSAPI_strcmp( atLPCSTR str1, atLPCSTR str2 )
{
	if( tOemFn.fnStrcmp != atNULL ){
		return tOemFn.fnStrcmp( str1, str2 );
	}

	return 0;
}

atINT     SYSAPI_wcscmp( atLPCWSTR str1, atLPCWSTR str2 )
{
	if( tOemFn.fnWcscmp != atNULL ){
 		return tOemFn.fnWcscmp( str1, str2 );
	}

	return 0;
}


atINT	  SYSAPI_strncmp( atLPCSTR str1, atLPCSTR str2, atINT len )
{
	if( tOemFn.fnStrncmp != atNULL ){
		return tOemFn.fnStrncmp( str1, str2, len );
	}

	return 0;
}

atINT     SYSAPI_wcsncmp( atLPCWSTR str1, atLPCWSTR str2 , atINT len)
{
	if( tOemFn.fnWcsncmp != atNULL ){
 		return tOemFn.fnWcsncmp( str1, str2, len );
	}

	return 0;
}


atINT     SYSAPI_stricmp( atLPCSTR str1, atLPCSTR str2 )
{
	if( tOemFn.fnStricmp != atNULL ){
		return tOemFn.fnStricmp( str1, str2 );
	}

	return 0;
}

atINT     SYSAPI_wcsicmp( atLPCWSTR str1, atLPCWSTR str2 )
{
	if( tOemFn.fnWcsicmp != atNULL ){
		return tOemFn.fnWcsicmp( str1, str2 );
	}

	return 0;
}


atCHAR * SYSAPI_strstr( atLPSTR str1, atLPCSTR str2 )
{
	if( tOemFn.fnStrstr != atNULL ){
		return tOemFn.fnStrstr(str1, str2);
	}

	return (atCHAR *)atNULL;
}

atWCHAR * SYSAPI_wcsstr( atLPWSTR str1, atLPCWSTR str2 )
{
	if( tOemFn.fnWcsstr != atNULL ){
		return tOemFn.fnWcsstr(str1, str2);
	}

	return (atWCHAR *)atNULL;
}


atCHAR * SYSAPI_strchr( atLPCSTR str, atINT ch )
{
	if( tOemFn.fnStrchr != atNULL ){
		return tOemFn.fnStrchr( str, ch );
	}

	return (atCHAR *)atNULL;
}

atWCHAR * SYSAPI_wcschr( atLPCWSTR str, atINT ch )
{
	if( tOemFn.fnWcschr != atNULL ){
		return tOemFn.fnWcschr( str, ch );
	}

	return (atWCHAR *)atNULL;
}

atWCHAR * SYSAPI_wcsrchr( atLPCWSTR str, atINT ch )
{
	if( tOemFn.fnWcsrchr != atNULL ){
		return tOemFn.fnWcsrchr( str, ch );
	}

	return (atWCHAR *)atNULL;
}


atCHAR * SYSAPI_strtok( atLPSTR str, atLPSTR sapr )
{
	if( tOemFn.fnStrtok != atNULL ){
		return tOemFn.fnStrtok( str, sapr );
	}

	return (atCHAR *)atNULL;
}

atWCHAR * SYSAPI_wcstok( atLPWSTR str, atLPWSTR sapr )
{
	if( tOemFn.fnWcstok != atNULL ){
		return tOemFn.fnWcstok( str, sapr );
	}

	return (atWCHAR *)atNULL;
}


atCHAR * SYSAPI_strupr( atLPSTR str )
{
	if( tOemFn.fnStrupr != atNULL ){
		return tOemFn.fnStrupr( str );
	}

	return (atCHAR *)atNULL;
}

atWCHAR * SYSAPI_wcsupr( atLPWSTR str )
{
	if( tOemFn.fnWcsupr != atNULL ){
		return tOemFn.fnWcsupr( str );
	}

	return (atWCHAR *)atNULL;
}


atCHAR * SYSAPI_strlwr( atLPSTR str )
{
	if( tOemFn.fnStrlwr != atNULL ){
		return tOemFn.fnStrlwr( str );
	}

	return (atCHAR *)atNULL;
}

atWCHAR * SYSAPI_wcslwr( atLPWSTR str )
{
	if( tOemFn.fnWcslwr != atNULL ){
		return tOemFn.fnWcslwr( str );
	}

	return (atWCHAR *)atNULL;
}


atINT  SYSAPI_atoi( atLPCSTR szIn )
{
	if( tOemFn.fnAtoi != atNULL ){
		return tOemFn.fnAtoi( szIn );
	}

	return 0;
}

atINT  SYSAPI_wtoi( atLPCWSTR szIn )
{
	if( tOemFn.fnWtoi != atNULL ){
		return tOemFn.fnWtoi( szIn );
	}

	return 0;
}


atLPSTR  SYSAPI_itoa( atINT value, atLPSTR szOut, atINT radix )
{
	if( tOemFn.fnItoa != atNULL ){
		return tOemFn.fnItoa( value, szOut, radix );
	}

	return (atLPSTR)atNULL;
}

atLPWSTR  SYSAPI_itow( atINT value, atLPWSTR szOut, atINT radix )
{
	if( tOemFn.fnItow != atNULL ){
		return tOemFn.fnItow( value, szOut, radix );
	}

	return (atLPWSTR)atNULL;
}


atINT     SYSAPI_vsprintf( atLPSTR str, atLPCSTR fmt, va_list ap )
{
	if( tOemFn.fnVSnprintf != atNULL ){
		return tOemFn.fnVSnprintf( str, -1, fmt, ap );
	}

	return 0;
}

atINT     SYSAPI_vswprintf( atLPWSTR str, atLPCWSTR fmt, va_list ap )
{
	if( tOemFn.fnVSnwprintf != atNULL ){
		return tOemFn.fnVSnwprintf( str, -1, fmt, ap );
	}

	return 0;
}


atINT     SYSAPI_vsnprintf( atLPSTR str, atINT nLen, atLPCSTR fmt, va_list ap )
{
	if( tOemFn.fnVSnprintf != atNULL ){
		return tOemFn.fnVSnprintf( str, nLen, fmt, ap );
	}

	return 0;
}

atINT     SYSAPI_vsnwprintf( atLPWSTR str, atINT nLen, atLPCWSTR fmt, va_list ap )
{
	if( tOemFn.fnVSnwprintf != atNULL ){
		return tOemFn.fnVSnwprintf( str, nLen, fmt, ap );
	}

	return 0;
}


atINT  SYSAPI_sprintf( atLPSTR str, atLPCSTR fmt, ... )
{
    va_list ap;
    int len;

    va_start(ap, fmt);
    len = SYSAPI_vsnprintf( str, -1, fmt, ap );
    va_end(ap);

	return len;
}

atINT  SYSAPI_swprintf( atLPWSTR str, atLPCWSTR fmt, ... )
{
    va_list ap;
    int len;

    va_start(ap, fmt);
    len = SYSAPI_vsnwprintf( str, -1, fmt, ap );
    va_end(ap);

	return len;
}


atINT  SYSAPI_snprintf( atLPSTR str, atINT nLen, atLPCSTR fmt, ... )
{
    va_list ap;
    int len;

    va_start(ap, fmt);
    len = SYSAPI_vsnprintf( str, nLen, fmt, ap );
    va_end(ap);

	  return len;
}

atINT  SYSAPI_snwprintf( atLPWSTR str, atINT nLen, atLPCWSTR fmt, ... )
{
    va_list ap;
    int len;

    va_start(ap, fmt);
    len = SYSAPI_vsnwprintf( str, nLen, fmt, ap );
    va_end(ap);

	  return len;
}


atINT	  SYSAPI_vsscanf( atLPSTR dstr, atLPCSTR fmt, va_list ap )
{
	if( tOemFn.fnVSnscanf != atNULL ){
		return tOemFn.fnVSnscanf( dstr, -1, fmt, ap );
	}

	return 0;
}

atINT	  SYSAPI_vswscanf( atLPWSTR dstr, atLPCWSTR fmt, va_list ap )
{
	if( tOemFn.fnVSnwscanf != atNULL ){
		return tOemFn.fnVSnwscanf( dstr, -1, fmt, ap );
	}

	return 0;
}


atINT	  SYSAPI_vsnscanf( atLPSTR dstr, atINT nLen, atLPCSTR fmt, va_list ap )
{
	if( tOemFn.fnVSnscanf != atNULL ){
		return tOemFn.fnVSnscanf( dstr, nLen, fmt, ap );
	}

	return 0;
}

atINT	  SYSAPI_vsnwscanf( atLPWSTR dstr, atINT nLen, atLPCWSTR fmt, va_list ap )
{
	if( tOemFn.fnVSnwscanf != atNULL ){
		return tOemFn.fnVSnwscanf( dstr, nLen, fmt, ap );
	}

	return 0;
}


atINT	  SYSAPI_sscanf( atLPSTR dstr, atLPCSTR fmt, ... )
{
	va_list ap;
    int len;

    va_start(ap, fmt);
    len = SYSAPI_vsnscanf( dstr, -1, fmt, ap );
    va_end(ap);

	return len;
}

atINT	  SYSAPI_swscanf( atLPWSTR dstr, atLPCWSTR fmt, ... )
{
	va_list ap;
    int len;

    va_start(ap, fmt);
    len = SYSAPI_vsnwscanf( dstr, -1, fmt, ap );
    va_end(ap);

	return len;
}


atINT	  SYSAPI_snscanf( atLPSTR dstr, atINT nLen, atLPCSTR fmt, ... )
{
	va_list ap;
    int len;

    va_start(ap, fmt);
    len = SYSAPI_vsnscanf( dstr, nLen, fmt, ap );
    va_end(ap);

	return len;
}

atINT	  SYSAPI_snwscanf( atLPWSTR dstr, atINT nLen, atLPCWSTR fmt, ... )
{
	va_list ap;
    int len;

    va_start(ap, fmt);
    len = SYSAPI_vsnwscanf( dstr, nLen, fmt, ap );
    va_end(ap);

	return len;
}

atCHAR * SYSAPI_strins( atLPSTR dstr, atLPCSTR sstr, atINT nIndex, atINT nInsLen )
{
	int i;
	int srclen = nInsLen;
	int deslen = SYSAPI_strlen(dstr);
	if( srclen < 0 ) srclen = SYSAPI_strlen(sstr);

	if( srclen <= 0 ) return dstr;
	if( deslen <= 0 ){
		return SYSAPI_strncpy( dstr, sstr, srclen );
	}
	if( nIndex >= deslen ) nIndex = deslen; 

	for( i = deslen-nIndex-1; i >=0 ; i-- ){
		dstr[nIndex + srclen + i] = dstr[nIndex + i];
	}
	for( i = 0; i < srclen; i++ ){
		dstr[nIndex + i] = sstr[i];
	}
	dstr[deslen + srclen] = '\0';

	return dstr;
}

atWCHAR * SYSAPI_wcsins( atLPWSTR dstr, atLPCWSTR sstr, atINT nIndex, atINT nInsLen )
{
	int i;
	int srclen = nInsLen;
	int deslen = SYSAPI_wcslen(dstr);
	if( srclen < 0 ) srclen = SYSAPI_wcslen(sstr);

	if( srclen <= 0 ) return dstr;
	if( deslen <= 0 ){
		return SYSAPI_wcsncpy( dstr, sstr, srclen );
	}
	if( nIndex >= deslen ) nIndex = deslen; 

	for( i = deslen-nIndex-1; i >=0 ; i-- ){
		dstr[nIndex + srclen + i] = dstr[nIndex + i];
	}
	for( i = 0; i < srclen; i++ ){
		dstr[nIndex + i] = sstr[i];
	}
	dstr[deslen + srclen] = L'\0';

	return dstr;
}


atCHAR * SYSAPI_strdel( atLPSTR dstr, atINT nIndex, atINT nDelLen )
{
	int i, cnt;
	int len = SYSAPI_strlen(dstr);

	if( (len <= 0) || (nIndex >= len) || (nDelLen <= 0) ){
		return dstr;
	}

	cnt = len - nIndex - nDelLen;
	for( i = 0; i < cnt; i++ ){
		dstr[nIndex + i] = dstr[nIndex + nDelLen + i];
	}
	dstr[len - nDelLen] = '\0';

	return dstr;
}

atWCHAR * SYSAPI_wcsdel( atLPWSTR dstr, atINT nIndex, atINT nDelLen )
{
	int i, cnt;
	int len = SYSAPI_wcslen(dstr);

	if( (len <= 0) || (nIndex >= len) || (nDelLen <= 0) ){
		return dstr;
	}

	cnt = len - nIndex - nDelLen;
	for( i = 0; i < cnt; i++ ){
		dstr[nIndex + i] = dstr[nIndex + nDelLen + i];
	}
	dstr[len - nDelLen] = L'\0';

	return dstr;
}


atINT 	  SYSAPI_UCS2ANSI( _OUT_ atLPSTR szOut, _IN_ atLPCWSTR szIn, atINT nLen )
{
	if( tOemFn.fnWcslen != atNULL ){
		if( nLen < 0 ) nLen = tOemFn.fnWcslen((const atLPWSTR)szIn);
	}
	if( nLen <= 0 ) return 0;
	if( tOemFn.fnUCS2ANSI != atNULL ){
		return tOemFn.fnUCS2ANSI( szOut, szIn, nLen );
	}
	return 0;
}

atINT 	  SYSAPI_ANSI2UCS( _OUT_ atLPWSTR szOut, _IN_ atLPCSTR szIn, atINT nLen )
{
	if( tOemFn.fnStrlen != atNULL ){
		if( nLen < 0 ) nLen = tOemFn.fnStrlen((const atLPSTR)szIn);
	}
	if( nLen <= 0 ) return 0;
	if( tOemFn.fnANSI2UCS != atNULL ){
		return tOemFn.fnANSI2UCS( szOut, szIn, nLen );
	}
	return 0;
}

atINT 	  SYSAPI_ToANSI( _OUT_ atLPSTR szOut, _IN_ atLPCTSTR szIn, atINT nLen )
{
	if( nLen == 0 ) return 0;
	
#ifdef __USES_UNICODE_
 	return SYSAPI_UCS2ANSI( szOut, szIn, nLen ); // nLen < 0 일 경우, 전체
 #else
	if( nLen > 0 ){
		if( tOemFn.fnStrncpy != atNULL ){
			tOemFn.fnStrncpy( szOut, szIn, nLen );
		}
	} else if( nLen < 0 ){
		if( tOemFn.fnStrcpy != atNULL ){
			tOemFn.fnStrcpy( szOut, szIn );
		}
	}
	if( tOemFn.fnStrlen != atNULL ){
		return tOemFn.fnStrlen(szOut);
	}
	return 0;
#endif
}

atINT 	  SYSAPI_ToUCS(  _OUT_ atLPWSTR szOut, _IN_ atLPCTSTR szIn, atINT nLen )
{
	if( nLen == 0 ) return 0;
	
#ifdef __USES_UNICODE_
	if( nLen > 0 ){
		if( tOemFn.fnWcsncpy != atNULL ){
			tOemFn.fnWcsncpy( szOut, szIn, nLen );
		}
	} else if( nLen < 0 ){
		if( tOemFn.fnWcscpy != atNULL ){
			tOemFn.fnWcscpy( szOut, szIn );
		}
	}
	if( tOemFn.fnWcslen != atNULL ){
		return tOemFn.fnWcslen(szOut);
	}
	return 0;
#else
	return SYSAPI_ANSI2UCS( szOut, szIn, nLen ); // nLen < 0 일 경우, 전체
#endif
}

atINT 	  SYSAPI_ToUCS2(  _OUT_ atUCS2_CHAR *szOut, _IN_ atLPCTSTR szIn, atINT nLen )
{
	if( nLen == 0 ) return 0;
	
#ifdef __USES_UNICODE_
	if( sizeof(atWCHAR) == sizeof(atUCS2_CHAR) ){
		if( nLen > 0 ){
			if( tOemFn.fnWcsncpy != atNULL ){
				tOemFn.fnWcsncpy( szOut, szIn, nLen );
			}
		} else if( nLen < 0 ){
			if( tOemFn.fnWcscpy != atNULL ){
				tOemFn.fnWcscpy( szOut, szIn );
			}
		}
	} else if( sizeof(atWCHAR) == 4 ){
		if( tOemFn.fnUCS4_to_UCS2 != atNULL ){
			tOemFn.fnUCS4_to_UCS2( szOut, (const atUCS4_CHAR *)szIn, nLen );
		}
	} else {
		return 0;
	}
	if( tOemFn.fnWcslen != atNULL ){
		return tOemFn.fnWcslen(szOut);
	}
	return 0;
#else
	return SYSAPI_ANSI2UCS( szOut, szIn, nLen ); // nLen < 0 일 경우, 전체
#endif
}

atINT 	  SYSAPI_FromANSI( _OUT_ atLPTSTR szOut, _IN_ atLPCSTR szIn, atINT nLen )
{
	if( nLen == 0 ) return 0;
	
#ifdef __USES_UNICODE_
	return SYSAPI_ANSI2UCS( szOut, szIn, nLen ); // nLen < 0 일 경우, 전체
#else
	if( nLen > 0 ){
		if( tOemFn.fnStrncpy != atNULL ){
			tOemFn.fnStrncpy( szOut, szIn, nLen );
		}
	} else if( nLen < 0 ){
		if( tOemFn.fnStrcpy != atNULL ){
			tOemFn.fnStrcpy( szOut, szIn );
		}
	}
	if( tOemFn.fnStrlen != atNULL ){
		return tOemFn.fnStrlen(szOut);
	}
	return 0;
#endif
}

atINT 	  SYSAPI_FromUCS(  _OUT_ atLPTSTR szOut, _IN_ atLPCWSTR szIn, atINT nLen )
{
	if( nLen == 0 ) return 0;
	
#ifdef __USES_UNICODE_
	if( nLen > 0 ) {
		if( tOemFn.fnWcsncpy != atNULL ){
			tOemFn.fnWcsncpy((const atLPWSTR)szOut, (const atLPWSTR)szIn, nLen);
		}
	} else {
		if( tOemFn.fnWcscpy != atNULL ){
			tOemFn.fnWcscpy((const atLPWSTR)szOut, (const atLPWSTR)szIn);
		}
	}
	if( tOemFn.fnWcslen != atNULL ){
		return tOemFn.fnWcslen(szOut);
	}
	return 0;
#else
	return SYSAPI_UCS2ANSI( szOut, szIn, nLen ); // nLen < 0 일 경우, 전체
#endif
}

atINT 	  SYSAPI_FromUCS2(  _OUT_ atLPTSTR szOut, _IN_ const atUCS2_CHAR *szIn, atINT nLen )
{
	if( nLen == 0 ) return 0;
	
#ifdef __USES_UNICODE_
	if( sizeof(atWCHAR) == sizeof(atUCS2_CHAR) ){
		if( nLen > 0 ){
			if( tOemFn.fnWcsncpy != atNULL ){
				tOemFn.fnWcsncpy((const atLPWSTR)szOut, (const atLPWSTR)szIn, nLen);
			}
		} else {
			if( tOemFn.fnWcscpy != atNULL ){
				tOemFn.fnWcscpy((const atLPWSTR)szOut, (const atLPWSTR)szIn);
			}
		}
	} else if( sizeof(atWCHAR) == 4 ){
		if( tOemFn.fnUCS2_to_UCS4 != atNULL ){
			tOemFn.fnUCS2_to_UCS4( (atUCS4_CHAR *)szOut, szIn, nLen );
		}
	} else {
		return 0;
	}
	if( tOemFn.fnWcslen != atNULL ){
		return tOemFn.fnWcslen(szOut);	
	}
	return 0;
#else
	return SYSAPI_UCS2ANSI( szOut, szIn, nLen ); // nLen < 0 일 경우, 전체
#endif
}
	
#endif
