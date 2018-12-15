/**
  @file UI_str.c
  @brief String에 관한 API
  @remark String에 관한 API 모음
  @author Amos
  @date 2010-04-19 오전 11:41:00
  @see UI_str.h
*/
#include "UI_Types.h"
#include "UI_API/UI_API.h"
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <wchar.h>
#include "OEM_UI_Str.h"


static OEM_UI_STR_T tOemFn;

atVOID	OEM_UI_Str_SetOemFn( OEM_UI_STR_T *fnOem )
{
	tOemFn = *fnOem;
}


atVOID	OEM_UI_Str_GetOemFn( OEM_UI_STR_T *fnOem )
{
	*fnOem = tOemFn;
}


/** 
 @brief 
 
 ......

 @param 
 @return 없음
*/
atVOID     atUIAPI_memset( atVOID *dst, atCHAR value, atLONG len )
{
	if( dst == atNULL ) return;
	tOemFn.fnMemset( dst, value, len );
}


/** 
 @brief 
 
 ......

 @param 
 @return 없음
*/
atVOID     atUIAPI_memcpy( atVOID *dst, const atVOID *src, atLONG len )
{
	if( (dst == atNULL) || (src == atNULL) ) return;
	tOemFn.fnMemcpy( dst, src, len );
}


/** 
 @brief 
 
 ......

 @param 
 @return 없음
*/
atINT     atUIAPI_strlen( atLPCTSTR str )
{
	return tOemFn.fnStrlen( str );
}


/** 
 @brief 
 
 ......

 @param 
 @return 없음
*/
atTCHAR * atUIAPI_strcpy( atLPTSTR dstr, atLPCTSTR sstr )
{
	return tOemFn.fnStrcpy( dstr, sstr );
}


/** 
 @brief 
 
 ......

 @param 
 @return 없음
*/
atTCHAR * atUIAPI_strncpy( atLPTSTR dstr, atLPCTSTR sstr, atLONG len )
{
	return tOemFn.fnStrncpy( dstr, sstr, len );
}
	

/** 
 @brief 
 
 ......

 @param 
 @return 없음
*/
atTCHAR * atUIAPI_strcat( atLPTSTR dstr, atLPCTSTR sstr )
{
	return tOemFn.fnStrcat( dstr, sstr );
}


/** 
 @brief 
 
 ......

 @param 
 @return 없음
*/
atINT     atUIAPI_strcmp( atLPCTSTR str1, atLPCTSTR str2 )
{
	return tOemFn.fnStrcmp( str1, str2 );
}


/** 
 @brief 
 
 ......

 @param 
 @return 없음
*/
atINT     atUIAPI_stricmp( atLPCTSTR str1, atLPCTSTR str2 )
{
	return tOemFn.fnStricmp( str1, str2 );
}


/** 
 @brief 
 
 ......

 @param 
 @return 없음
*/
atTCHAR * atUIAPI_strchr( atLPCTSTR str, atINT ch )
{
	return tOemFn.fnStrchr( str, ch );
}


/** 
 @brief 
 
 ......

 @param 
 @return 없음
*/
atINT     atUIAPI_vsprintf( atLPTSTR str, atLPCTSTR fmt, va_list ap )
{
	return tOemFn.fnVSPrintf( str, fmt, ap );
}


/** 
 @brief 
 
 ......

 @param 
 @return 없음
*/
atINT  atUIAPI_sprintf( atLPTSTR str, atLPCTSTR fmt, ... )
{
    va_list ap;
    int len;

    va_start(ap, fmt);
    len = atUIAPI_vsprintf( str, fmt, ap );
    va_end(ap);

	return len;
}


/** 
 @brief 
 
 ......

 @param 
 @return 없음
*/
atTCHAR * atUIAPI_strins( atLPTSTR dstr, atLPCTSTR sstr, atINT nIndex, atINT nInsLen )
{
	int i;
	int srclen = nInsLen;
	int deslen = atUIAPI_strlen(dstr);
	if( srclen < 0 ) srclen = atUIAPI_strlen(sstr);

	if( srclen <= 0 ) return dstr;
	if( deslen <= 0 ){
		return atUIAPI_strncpy( dstr, sstr, srclen );
	}
	if( nIndex >= deslen ) nIndex = deslen; 

	for( i = deslen-nIndex-1; i >=0 ; i-- ){
		dstr[nIndex + srclen + i] = dstr[nIndex + i];
	}
	for( i = 0; i < srclen; i++ ){
		dstr[nIndex + i] = sstr[i];
	}
	dstr[deslen + srclen] = _ATC('\0');

	return dstr;
}


/** 
 @brief 
 
 ......

 @param 
 @return 없음
*/
atTCHAR * atUIAPI_strdel( atLPTSTR dstr, atINT nIndex, atINT nDelLen )
{
	int i, cnt;
	int len = atUIAPI_strlen(dstr);

	if( (len <= 0) || (nIndex >= len) || (nDelLen <= 0) ){
		return dstr;
	}

	cnt = len - nIndex - nDelLen;
	for( i = 0; i < cnt; i++ ){
		dstr[nIndex + i] = dstr[nIndex + nDelLen + i];
	}
	dstr[len - nDelLen] = _ATC('\0');

	return dstr;
}

	
/** 
 @brief 
 
 ......

 @param 
 @return 없음
*/
atINT 	  atUIAPI_atoi( atLPCTSTR szIn )
{
	return tOemFn.fnAtoi( szIn );
}


/** 
 @brief 
 
 ......

 @param 
 @return 없음
*/
atLPTSTR   atUIAPI_itoa( atINT value, atLPTSTR szOut, atINT radix )
{
	return tOemFn.fnItoa( value, szOut, radix );
}


static atINT   _UI_wcslen( atLPCWSTR str )
{
	int len = 0;
	atWCHAR *ptr, ch;
	
	ptr = (atWCHAR *)str;
	while( (ch = *ptr++) != (atWCHAR)'\0' ) len++;
	
	return len;
}

/** 
 @brief 
 
 ......

 @param 
 @return 없음
*/
atINT 	  atUIAPI_UCS2ANSI( atLPSTR szOut, const atLPWSTR szIn, atINT nLen )
{
	if( nLen < 0 ) nLen = _UI_wcslen(szIn);
	if( nLen == 0 ){
		szOut[0] = _ATC('\0');
		return 0;
	}
	return tOemFn.fnUCS2ANSI( szOut, szIn, nLen );
}


/** 
 @brief 
 
 ......

 @param 
 @return 없음
*/
atINT 	  atUIAPI_ANSI2UCS( atLPWSTR szOut, const atLPSTR szIn, atINT nLen )
{
	if( nLen < 0 ) nLen = strlen(szIn);
	if( nLen == 0 ){
		szOut[0] = '\0';
		return 0;
	}
	return tOemFn.fnANSI2UCS( szOut, szIn, nLen );
}


/** 
 @brief 
 
 ......

 @param 
 @return 없음
*/
atINT 	  atUIAPI_ToANSI( _OUT_ atLPSTR szOut, _IN_ atLPCTSTR szIn, atINT nLen )
{
#ifdef __USES_UNICODE_
 	return atUIAPI_UCS2ANSI( szOut, szIn, nLen ); // nLen < 0 AI °æ¿i, AuA¼
 #else
	if( nLen > 0 ){
		OEM_UI_strncpy( szOut, szIn, nLen );
	} else if( nLen < 0 ){
		OEM_UI_strcpy( szOut, szIn );
	}
	return OEM_UI_strlen(szOut);
#endif
}


/** 
 @brief 
 
 ......

 @param 
 @return 없음
*/
atINT 	  atUIAPI_ToUCS(  _OUT_ atLPWSTR szOut, _IN_ atLPCTSTR szIn, atINT nLen )
{
#ifdef __USES_UNICODE_
	if( nLen > 0 ){
		tOemFn.fnStrncpy( szOut, szIn, nLen );
	} else if( nLen < 0 ){
		tOemFn.fnStrcpy( szOut, szIn );
	}
	return tOemFn.fnStrlen(szOut);
#else
	return atUIAPI_ANSI2UCS( szOut, szIn, nLen ); // nLen < 0 AI °æ¿i, AuA¼
#endif
}


/** 
 @brief 
 
 ......

 @param 
 @return 없음
*/
atINT 	  atUIAPI_FromANSI( _OUT_ atLPTSTR szOut, _IN_ const atLPSTR szIn, atINT nLen )
{
#ifdef __USES_UNICODE_
	return atUIAPI_ANSI2UCS( szOut, szIn, nLen ); // nLen < 0 AI °æ¿i, AuA¼
#else
	if( nLen > 0 ){
		tOemFn.fnStrncpy( szOut, szIn, nLen );
	} else if( nLen < 0 ){
		tOemFn.fnStrcpy( szOut, szIn );
	}
	return tOemFn.fnStrlen(szOut);
#endif
}


/** 
 @brief 
 
 ......

 @param 
 @return 없음
*/
atINT 	  atUIAPI_FromUCS(  _OUT_ atLPTSTR szOut, _IN_ const atLPWSTR szIn, atINT nLen )
{
#ifdef __USES_UNICODE_
	if( nLen > 0 )
		tOemFn.fnStrncpy((const atLPWSTR)szOut, (const atLPWSTR)szIn, nLen);
	else
		tOemFn.fnStrcpy((const atLPWSTR)szOut, (const atLPWSTR)szIn);
	return tOemFn.fnStrlen(szOut);
#else
	return atUIAPI_UCS2ANSI( szOut, szIn, nLen ); // nLen < 0 AI °æ¿i, AuA¼
#endif
}

