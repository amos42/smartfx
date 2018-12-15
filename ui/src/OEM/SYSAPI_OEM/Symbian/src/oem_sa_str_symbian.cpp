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


typedef struct __tagSTR {
	atDWORD dwAttr;
	atSHORT  nCapacity, nStrLen;
	atTCHAR *lpszStr;
} _STR;


atVOID     OEM_SYSAPI_memset( atVOID *dst, char value, atLONG len )
{
	memset( dst, value, len );
}

atVOID     OEM_SYSAPI_memcpy( atVOID *dst, const atVOID *src, atLONG len )
{
	memcpy( dst, src, len );
}


OEM_STR OEM_SYSAPI_CreateString( atLPTSTR szStr, atINT nStrLen, atINT nCapacity, atDWORD dwAttr )
{
	_STR *str;

	str = (_STR *)SYSAPI_AllocMem( sizeof(_STR) + nCapacity );
	OEM_SYSAPI_AssignString( (atLPVOID)str,  sizeof(_STR) + nCapacity );
	str->dwAttr = SYS_STRATTR_OWERMEM;

	return (OEM_STR)str;
}

OEM_STR OEM_SYSAPI_AssignString( atLPVOID ptr, atINT nLen )
{
	_STR *str;
	
	if( ptr == atNULL ) return (OEM_STR)atNULL;
	str = (_STR *)ptr;
	str->lpszStr = (atLPTSTR)((long)ptr + sizeof(_STR));

	OEM_SYSAPI_InitString( str, atNULL, 0, nLen - sizeof(_STR), 0x0000 );

	return (OEM_STR)str;
}

atVOID  OEM_SYSAPI_InitString( OEM_STR oem_str, atLPTSTR szStr, atINT nStrLen, atINT nCapacity, atDWORD dwAttr )
{
	_STR *str;
	if( oem_str == atNULL ) return;
	
	str = (_STR *)oem_str;
	str->nCapacity = nCapacity;
	str->dwAttr = dwAttr;

	OEM_SYSAPI_SetString( oem_str, szStr, nStrLen );
}

atVOID	OEM_SYSAPI_DestroyString( OEM_STR oem_str )
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

atLPTSTR	OEM_SYSAPI_GetString( OEM_STR oem_str )
{
	if( oem_str == atNULL ) return (atLPTSTR)atNULL;
	
	return ((_STR *)oem_str)->lpszStr;
}

atBOOL	OEM_SYSAPI_SetString( OEM_STR oem_str, atLPTSTR szStr, atINT nStrLen )
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



atINT   OEM_SYSAPI_strlen( const atLPSTR str )
{
    return strlen( str );
}


atCHAR * OEM_SYSAPI_strcpy( atLPSTR dstr, const atLPSTR sstr )
{
	return strcpy( dstr, sstr );
}


atCHAR * OEM_SYSAPI_strncpy( atLPSTR dstr, const atLPSTR sstr, atINT len )
{
    atCHAR *s = strncpy( dstr, sstr, len );
    int len0 = strlen( sstr );
    dstr[MIN(len,len0)] = '\0';
    return dstr;
}



atCHAR  * OEM_SYSAPI_strcat( atLPSTR dstr, const atLPSTR sstr )
{
	return strcat( dstr, sstr );
}


atINT     OEM_SYSAPI_strcmp( const atLPSTR str1, const atLPSTR str2 )
{
	return strcmp( str1, str2 );
}

atINT     OEM_SYSAPI_stricmp( const atLPSTR str1, const atLPSTR str2 )
{
	// fix-me: stricmp 사용시 컴파일 에러남...
	//return stricmp( str1, str2 );
	return strcasecmp( str1, str2 );
}

atCHAR * OEM_SYSAPI_strchr( const atLPSTR str, atINT ch )
{
	return strchr( str, ch);
}

atCHAR * OEM_SYSAPI_strtok( atLPSTR str, atLPSTR sapr )
{
	return strtok( str, sapr );
}

atINT OEM_SYSAPI_atoi( const atLPSTR szIn )
{
	return atoi( szIn );
}


atLPSTR OEM_SYSAPI_itoa( atINT value, atLPSTR szOut, atINT radix )
{
	static const atCHAR digit[] = "0123456789ABCDEF";
	unsigned char buf[34], *bufp, c;
	atCHAR* out;
	int quotient = value; 
	
	if( (radix < 2) || (radix > 16) ) return atNULL;
	
	out = szOut; 
	if( value < 0 ){
		*out++ = '-';
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
	
	*out = '\0'; 
	
	return szOut; 
}

//#define __USES_FORCE_UNICODE_
#include "oem_sa_str_vsnprintf.inl"
atINT  OEM_SYSAPI_vsnprintf( atLPSTR str, atINT nLen, const atLPSTR fmt, va_list ap )
{
    return _GLOBAL_FUNC_(my_vsnprintf)( (_TCHAR *)str, nLen, (const _TCHAR *)fmt, ap );
}
//#undef __USES_FORCE_UNICODE_

//#define __USES_FORCE_UNICODE_
#include "oem_sa_str_vsnscanf.inl"
int OEM_SYSAPI_vsnscanf( atLPSTR str, atINT nLen, const atLPSTR fmt, va_list ap )
{
/*
	// fonction qui curieusement n'existe pas chez Microsoft
	atCHAR *nformat, *format;
	int i, n = 0, convtot = 0;

	nformat = _strdup(fmt);

	format = nformat;
	for(i = 0 ; format[i] ; i++)
	{
		if(format[i] == '%')
		{
			if(format[i+1] == '%' || format[i+1] == '*' || format[i+1] == '\0')
			{
				i++; // pas comptC) comme variable C_ allouer, je saute
			}
			else	// c'est une variable
			{ 
				if(n==0)	// premiC(re variable: OK
				{
					n++; 
				}
				else		// deuxiC(me variable: on coupe juste avant
				{ 
					atCHAR f[4] = {0, };
					int lu = -1, conv;
					void *p;
					
					f[1] = format[i+1];
					f[2] = format[i+2];
					format[i+1] = 'n';
					format[i+2] = '\0';	// %n = nombre de caractC(res lus dans l'entrC)e
					
					p = va_arg(args, void *);
					conv = sscanf(buf, format, p, &lu);
					if(lu == -1)	// C)chec C_ la conversion
					{ 
						convtot += conv;	// peut-C*tre converti quand mC*me une variable?
						break;				// mais ensuite on arrC*te
					}

					convtot++;	// une variable lue
					buf += lu;	// avancer dans l'entrC)e
					format[i+1] = f[1];
					format[i+2] = f[2]; // rC)tablir la chaC.ne de format
					format += i;
					i = -1;		// avancer dans le format
					n = 0;		// on repart avec 0 variable dC)tectC)e dans la nouvelle chaC.ne
				}
			}
		}
	}

	if(format[i] == '\0' && n > 0)
	{ 
		// on est arrivC) au bout, et il reste une variable C_ convertir
		void *p = va_arg(args, void *);
		int conv;
		conv = sscanf(buf, format, p);
		convtot += conv;
	}

	//OEM_SYSAPI_FreeMem(nformat);
	free(nformat);
	return convtot;
*/
	return _GLOBAL_FUNC_(my_vsnscanf)( (_TCHAR *)str, nLen, (const _TCHAR *)fmt, ap );
}
//#undef __USES_FORCE_UNICODE_


atINT   OEM_SYSAPI_wcslen( const atLPWSTR str )
{
    return wcslen( str );
}


atWCHAR * OEM_SYSAPI_wcscpy( atLPWSTR dstr, const atLPWSTR sstr )
{
    return wcscpy( dstr, sstr );
}


atWCHAR * OEM_SYSAPI_wcsncpy( atLPWSTR dstr, const atLPWSTR sstr, atINT len )
{
    atWCHAR *s = wcsncpy( dstr, sstr, len );
    int len0 = wcslen( sstr );
    dstr[MIN(len,len0)] = L'\0';
    return dstr;
}



atWCHAR  * OEM_SYSAPI_wcscat( atLPWSTR dstr, const atLPWSTR sstr )
{
    return wcscat( dstr, sstr );
}


atINT     OEM_SYSAPI_wcscmp( const atLPWSTR str1, const atLPWSTR str2 )
{
    return wcscmp( str1, str2 );
}

atINT     OEM_SYSAPI_wcsicmp( const atLPWSTR str1, const atLPWSTR str2 )
{
    return wcsicmp( str1, str2 );
}

atWCHAR * OEM_SYSAPI_wcschr( const atLPWSTR str, atINT ch )
{
    return wcschr( str, (wchar_t)ch );
}

atWCHAR * OEM_SYSAPI_wcstok( atLPWSTR str, atLPWSTR sapr )
{
	return (atWCHAR *)atNULL; // 일단 막자...
}

atINT OEM_SYSAPI_watoi( const atLPWSTR szIn )
{
	int num = 0;
	atWCHAR *ptr, ch;
	
	ptr = (atWCHAR *)szIn;
	while( (ch = *ptr++) != L'\0' ){
	 	num = (num*10) + (ch - L'0');
	}
	
	return num;
}

atLPWSTR OEM_SYSAPI_witoa( atINT value, atLPWSTR szOut, atINT radix )
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

#define __USES_FORCE_UNICODE_
#include "oem_sa_str_vsnprintf.inl"
atINT  OEM_SYSAPI_wvsnprintf( atLPWSTR str, atINT nLen, const atLPWSTR fmt, va_list ap )
{
/*
	#if _MSC_VER>1200 //MSVS2008 ver
		atWCHAR tpTmp[1024];
		int iRet = _vsnwprintf( tpTmp, 1024, fmt, ap );
		SYSAPI_wcscpy (str, tpTmp);
		return iRet;
	#else
		//MSVS 6.0 ver
		return vswprintf( str, fmt, ap );
	#endif
*/
	return _GLOBAL_FUNC_(my_vsnprintf)( str, nLen, fmt, ap );
}
#undef __USES_FORCE_UNICODE_

#define __USES_FORCE_UNICODE_
#include "oem_sa_str_vsnscanf.inl"
int OEM_SYSAPI_wvsnscanf( atLPWSTR str, atINT nLen, const atLPWSTR fmt, va_list ap )
{
/*
	// fonction qui curieusement n'existe pas chez Microsoft
	atWCHAR *nformat, *format;
	int i, n = 0, convtot = 0;

	nformat = wcsdup(fmt);

	format = nformat;
	for(i = 0 ; format[i] ; i++)
	{
		if(format[i] == L'%')
		{
			if(format[i+1] == L'%' || format[i+1] == L'*' || format[i+1] == L'\0')
			{
				i++; // pas comptC) comme variable C_ allouer, je saute
			}
			else	// c'est une variable
			{ 
				if(n==0)	// premiC(re variable: OK
				{
					n++; 
				}
				else		// deuxiC(me variable: on coupe juste avant
				{ 
					atWCHAR f[4] = {0, };
					int lu = -1, conv;
					void *p;
					
					f[1] = format[i+1];
					f[2] = format[i+2];
					format[i+1] = L'n';
					format[i+2] = L'\0';	// %n = nombre de caractC(res lus dans l'entrC)e
					
					p = va_arg(args, void *);
					conv = swscanf(buf, format, p, &lu);
					if(lu == -1)	// C)chec C_ la conversion
					{ 
						convtot += conv;	// peut-C*tre converti quand mC*me une variable?
						break;				// mais ensuite on arrC*te
					}

					convtot++;	// une variable lue
					buf += lu;	// avancer dans l'entrC)e
					format[i+1] = f[1];
					format[i+2] = f[2]; // rC)tablir la chaC.ne de format
					format += i;
					i = -1;		// avancer dans le format
					n = 0;		// on repart avec 0 variable dC)tectC)e dans la nouvelle chaC.ne
				}
			}
		}
	}

	if(format[i] == L'\0' && n > 0)
	{ 
		// on est arrivC) au bout, et il reste une variable C_ convertir
		void *p = va_arg(args, void *);
		int conv;
		conv = swscanf(buf, format, p);
		convtot += conv;
	}

	//OEM_SYSAPI_FreeMem(nformat);
	free(nformat);
	return convtot;
*/
	return _GLOBAL_FUNC_(my_vsnscanf)( str, nLen, fmt, ap );
}
#undef __USES_FORCE_UNICODE_


atINT  OEM_SYSAPI_UCS2ANSI( atLPSTR szOut, const atLPWSTR szIn, atINT nLen )
{
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
}

atINT  OEM_SYSAPI_ANSI2UCS( atLPWSTR szOut, const atLPSTR szIn, atINT nLen )
{
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
}

