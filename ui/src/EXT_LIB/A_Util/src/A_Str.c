/*#############################################################################
-------------------------------------------------------------------------------

                                   Resource.c
                           (* =================== *)


 Version           : 1.0.0
 Dependent Modules : A_VM, A_Utils
 Author            : Ju, Gyeong-min ( Amos )
 Create            : 2001/04/25    01:47:09am
 Update            : 2003/04/29    00:00:00am
 C Compiler        : ADS 1.01, SDT 2.51, Borland C/C++ 5.0, Visual C/C++ 6.0
 Assembler         : ADS 1.01, SDT 2.51, Borland TASM 3.1, MASM 6.0
 Description       : Resource Manager related function definition
 
-------------------------------------------------------------------------------
#############################################################################*/

#include "AT_Types.h"
#include "A_Str.h"


/****************************
  배열을 채운다.
*****************************/
atVOID atSTR_memset( atLPVOID des, atCHAR ch, atLONG len )
{
	atCHAR *dptr = (atCHAR *)des;

	//if(len < 4) {
		while( len-- ) {
		    *dptr++ = ch;
		}
	//} else {
	//	long len2 = 4 - (len & 0x3);
	//	if(len2 > 0){
	//		len -= len2;
	//		while( len2-- ) {
	//		  *dptr++ = ch;
	//		}
	//	}
	//	len2 = len >> 2;
	//	if(len2 > 0){
	//		unsigned long *dptr2 = (unsigned long *)dptr;
	//		unsigned long v = ch;
	//		v |= (v << 24) | (v << 16) | (v << 8);
	//		while( len2-- ) {
	//		  *dptr2++ = v;
	//		}
	//		dptr = (atCHAR *)dptr2;
	//	}
	//	len &= 0x3;
	//	while( len-- > 0) {
	//	  *dptr++ = ch;
	//	}
	//}
}

/****************************
  문자열을 대문자로 바꾼다.
*****************************/
atLPSTR atSTR_strupr( atLPSTR str )
{
    atCHAR ch;
	int delta = (int)('a' - 'A');
    
    while( (ch = *str) != (atCHAR)'\0' ) {
      if( ch >= (atCHAR)'a' && ch <= (atCHAR)'z' ) *str = (atCHAR)(ch - delta);
      str++;
    }
    
    return( str );
}

/****************************
  문자열을 복사한다.
*****************************/
atLPSTR atSTR_strncpy( atLPSTR des, atLPCSTR src, atLONG len )
{
	atCHAR *dptr, *sptr, ch;
	
	dptr = (atCHAR *)des;
	sptr = (atCHAR *)src;
	if((dptr == atNULL) || (sptr == atNULL) || (len <= 0)) return (atCHAR *)atNULL;
	while( ((ch = *sptr++) != (atCHAR)'\0') && (len-- > 0)) {
	  *dptr++ = ch;
	}
	*dptr = (atCHAR)'\0' ;
	
	return (atCHAR *)des;
}

/****************************
  문자열을 비교한다.
*****************************/
atINT atSTR_strcmp( atLPCSTR str1, atLPCSTR str2 )
{
	atUCHAR *ptr1, *ptr2, ch1, ch2;
	
	ptr1 = (atUCHAR *)str1;
	ptr2 = (atUCHAR *)str2;
	if((ptr1 == atNULL) || (ptr2 == atNULL)) return 0;
	do {
	  ch1 = *ptr1++;
	  ch2 = *ptr2++;
	} while((ch1 == ch2) && (ch1 != (atCHAR)'\0'));
	
	return (atINT)ch1 - (atINT)ch2;
}

/****************************
  문자열을 복사한다.
*****************************/
atLPWSTR atSTR_wcsncpy( atLPWSTR des, atLPCWSTR src, atLONG len )
{
	atWCHAR *dptr, *sptr, ch;
	
	dptr = (atWCHAR *)des;
	sptr = (atWCHAR *)src;
	if((dptr == atNULL) || (sptr == atNULL) || (len <= 0)) return (atWCHAR *)atNULL;
	while( ((ch = *sptr++) != (atWCHAR)L'\0') && (len-- > 0)) {
	  *dptr++ = ch;
	}
	*dptr = (atWCHAR)L'\0';
	
	return des;
}

/****************************
  문자열을 복사한다.
*****************************/
atLPTSTR atSTR_tcsncpy( atLPTSTR des, atLPCTSTR src, atLONG len )
{
	atTCHAR *dptr, *sptr, ch;
	
	dptr = (atTCHAR *)des;
	sptr = (atTCHAR *)src;
	if((dptr == atNULL) || (sptr == atNULL) || (len <= 0)) return (atTCHAR *)atNULL;
	while( ((ch = *sptr++) != _AT('\0')) && (len-- > 0)) {
	  *dptr++ = ch;
	}
	*dptr = _AT('\0');
	
	return des;
}


//#define __USES_FORCE_UNICODE_
#include "_vsnprintf.inl"
atINT  atSTR_vsnprintf( atLPSTR str, atINT nLen, atLPCSTR fmt, va_list ap )
{
	return _GLOBAL_FUNC_(_astr_vsnprintf)( str, nLen, fmt, ap );
}


#include "_vsnscanf.inl"
atINT atSTR_vsnscanf( atLPSTR str, atINT nLen, atLPCSTR fmt, va_list ap )
{
	return _GLOBAL_FUNC_(_astr_vsnscanf)( (char *)str, nLen, (atLPCSTR )fmt, ap );
}
//#undef __USES_FORCE_UNICODE_


#define __USES_FORCE_UNICODE_

#include "_vsnprintf.inl"
atINT  atSTR_vsnwprintf( atLPWSTR str, atINT nLen, atLPCWSTR fmt, va_list ap )
{
	return _GLOBAL_FUNC_(_astr_vsnprintf)( str, nLen, fmt, ap );
}

#include "_vsnscanf.inl"
atINT atSTR_vsnwscanf( atLPWSTR str, atINT nLen, atLPCWSTR fmt, va_list ap )
{
	return _GLOBAL_FUNC_(_astr_vsnscanf)( str, nLen, fmt, ap );
}

#undef __USES_FORCE_UNICODE_


atCHAR * atSTR_strins(atLPSTR dstr, atLPCSTR sstr, atINT nIndex, atINT nInsLen)
{
	int i;
	int srclen = nInsLen;
	int deslen = SYSAPI_strlen(dstr);
	if (srclen < 0) srclen = SYSAPI_strlen(sstr);

	if (srclen <= 0) return dstr;
	if (deslen <= 0) {
		return SYSAPI_strncpy(dstr, sstr, srclen);
	}
	if (nIndex >= deslen) nIndex = deslen;

	for (i = deslen - nIndex - 1; i >= 0; i--) {
		dstr[nIndex + srclen + i] = dstr[nIndex + i];
	}
	for (i = 0; i < srclen; i++) {
		dstr[nIndex + i] = sstr[i];
	}
	dstr[deslen + srclen] = '\0';

	return dstr;
}

atWCHAR * atSTR_wcsins(atLPWSTR dstr, atLPCWSTR sstr, atINT nIndex, atINT nInsLen)
{
	int i;
	int srclen = nInsLen;
	int deslen = SYSAPI_wcslen(dstr);
	if (srclen < 0) srclen = SYSAPI_wcslen(sstr);

	if (srclen <= 0) return dstr;
	if (deslen <= 0) {
		return SYSAPI_wcsncpy(dstr, sstr, srclen);
	}
	if (nIndex >= deslen) nIndex = deslen;

	for (i = deslen - nIndex - 1; i >= 0; i--) {
		dstr[nIndex + srclen + i] = dstr[nIndex + i];
	}
	for (i = 0; i < srclen; i++) {
		dstr[nIndex + i] = sstr[i];
	}
	dstr[deslen + srclen] = L'\0';

	return dstr;
}


atCHAR * atSTR_strdel(atLPSTR dstr, atINT nIndex, atINT nDelLen)
{
	int i, cnt;
	int len = SYSAPI_strlen(dstr);

	if ((len <= 0) || (nIndex >= len) || (nDelLen <= 0)) {
		return dstr;
	}

	cnt = len - nIndex - nDelLen;
	for (i = 0; i < cnt; i++) {
		dstr[nIndex + i] = dstr[nIndex + nDelLen + i];
	}
	dstr[len - nDelLen] = '\0';

	return dstr;
}

atWCHAR * atSTR_wcsdel(atLPWSTR dstr, atINT nIndex, atINT nDelLen)
{
	int i, cnt;
	int len = SYSAPI_wcslen(dstr);

	if ((len <= 0) || (nIndex >= len) || (nDelLen <= 0)) {
		return dstr;
	}

	cnt = len - nIndex - nDelLen;
	for (i = 0; i < cnt; i++) {
		dstr[nIndex + i] = dstr[nIndex + nDelLen + i];
	}
	dstr[len - nDelLen] = L'\0';

	return dstr;
}



#if 0

atINT atSTR_UCS2ANSI(_OUT_ atLPSTR szOut, _IN_ atLPCWSTR szIn, atINT nLen)
{
	if (nLen < 0) nLen = atSTR_wcslen((const atLPWSTR)szIn);
	if (nLen <= 0) return 0;

	int cnt;
	if (nLen <= 0) return 0;
	cnt = WideCharToMultiByte(CP_OEMCP, 0, szIn, -1, szOut, 0, NULL, NULL);
	cnt = WideCharToMultiByte(CP_OEMCP, 0, szIn, -1, szOut, cnt, NULL, NULL);
	szOut[cnt] = '\0';
	return cnt;
}

atINT atSTR_ANSI2UCS(_OUT_ atLPWSTR szOut, _IN_ atLPCSTR szIn, atINT nLen)
{
	if (nLen < 0) nLen = atSTR_strlen((const atLPSTR)szIn);
	if (nLen <= 0) return 0;

	int cnt;
	if (nLen <= 0) return 0;
	cnt = MultiByteToWideChar(CP_OEMCP, MB_PRECOMPOSED, szIn, nLen, szOut, nLen);
	szOut[cnt] = L'\0';
	return cnt;
}

atINT  atSTR_UCS2_to_UCS4(atUCS4_CHAR *szOut, const atUCS2_CHAR *szIn, atINT nLen)
{
	int cnt = 0;
	atUCS2_CHAR *lpIn = (atUCS2_CHAR *)szIn;
	atUCS4_CHAR *lpOut = szOut;

	if (nLen <= 0) return 0;

	while ((*lpIn) != 0x00 && (*lpIn) != (atUCS2_CHAR)'\0' && nLen)
	{
		*lpOut = (atUCS4_CHAR)(*lpIn);

		lpIn++;	nLen--;
		lpOut++;	cnt++;
	}

	*lpOut = (atUCS4_CHAR)'\0';

	return cnt;
}

atINT  atSTR_UCS4_to_UCS2(atUCS2_CHAR *szOut, const atUCS4_CHAR *szIn, atINT nLen)
{
	int cnt = 0;
	atUCS4_CHAR *lpIn = (atUCS4_CHAR *)szIn;
	atUCS2_CHAR *lpOut = szOut;

	if (nLen <= 0) return 0;

	while ((*lpIn) != 0x00 && (*lpIn) != (atUCS4_CHAR)'\0' && nLen)
	{
		*lpOut = (atUCS2_CHAR)(*lpIn);

		lpIn++;	nLen--;
		lpOut++;	cnt++;
	}

	*lpOut = (atUCS2_CHAR)'\0';

	return cnt;
}


atINT atSTR_ToANSI(_OUT_ atLPSTR szOut, _IN_ atLPCTSTR szIn, atINT nLen)
{
	if (nLen == 0) return 0;

#ifdef __USES_UNICODE_
	return atSTR_UCS2ANSI(szOut, szIn, nLen); // nLen < 0 일 경우, 전체
#else
	if (nLen > 0) {
		atSTR_strncpy(szOut, szIn, nLen);
	}
	else if (nLen < 0) {
		atSTR_strcpy(szOut, szIn);
	}
	return atSTR_strlen(szOut);
#endif
}

atINT atSTR_ToUCS(_OUT_ atLPWSTR szOut, _IN_ atLPCTSTR szIn, atINT nLen)
{
	if (nLen == 0) return 0;

#ifdef __USES_UNICODE_
	if (nLen > 0) {
		atSTR_wcsncpy(szOut, szIn, nLen);
	}
	else if (nLen < 0) {
		atSTR_wcscpy(szOut, szIn);
	}
	return atSTR_wcslen(szOut);
#else
	return SYSAPI_ANSI2UCS(szOut, szIn, nLen); // nLen < 0 일 경우, 전체
#endif
}

atINT atSTR_ToUCS2(_OUT_ atUCS2_CHAR *szOut, _IN_ atLPCTSTR szIn, atINT nLen)
{
	if (nLen == 0) return 0;

#ifdef __USES_UNICODE_
	if (sizeof(atWCHAR) == sizeof(atUCS2_CHAR)) {
		if (nLen > 0) {
			atSTR_wcsncpy(szOut, szIn, nLen);
		}
		else if (nLen < 0) {
			atSTR_wcscpy(szOut, szIn);
		}
	}
	else if (sizeof(atWCHAR) == 4) {
		atSTR_UCS4_to_UCS2(szOut, (const atUCS4_CHAR *)szIn, nLen);
	}
	else {
		return 0;
	}
	return atSTR_wcslen(szOut);
#else
	return atSTR_ANSI2UCS(szOut, szIn, nLen); // nLen < 0 일 경우, 전체
#endif
}

atINT atSTR_FromANSI(_OUT_ atLPTSTR szOut, _IN_ atLPCSTR szIn, atINT nLen)
{
	if (nLen == 0) return 0;

#ifdef __USES_UNICODE_
	return atSTR_ANSI2UCS(szOut, szIn, nLen); // nLen < 0 일 경우, 전체
#else
	if (nLen > 0) {
		atSTR_strncpy(szOut, szIn, nLen);
	}
	else if (nLen < 0) {
		atSTR_strcpy(szOut, szIn);
	}
	return atSTR_strlen(szOut);
#endif
}

atINT atSTR_FromUCS(_OUT_ atLPTSTR szOut, _IN_ atLPCWSTR szIn, atINT nLen)
{
	if (nLen == 0) return 0;

#ifdef __USES_UNICODE_
	if (nLen > 0) {
		atSTR_wcsncpy((const atLPWSTR)szOut, (const atLPWSTR)szIn, nLen);
	}
	else {
		atSTR_wcscpy((const atLPWSTR)szOut, (const atLPWSTR)szIn);
	}
	return atSTR_wcslen(szOut);
#else
	return SYSAPI_UCS2ANSI(szOut, szIn, nLen); // nLen < 0 일 경우, 전체
#endif
}

atINT atSTR_FromUCS2(_OUT_ atLPTSTR szOut, _IN_ const atUCS2_CHAR *szIn, atINT nLen)
{
	if (nLen == 0) return 0;

#ifdef __USES_UNICODE_
	if (sizeof(atWCHAR) == sizeof(atUCS2_CHAR)) {
		if (nLen > 0) {
			atSTR_wcsncpy((const atLPWSTR)szOut, (const atLPWSTR)szIn, nLen);
		}
		else {
			atSTR_wcscpy((const atLPWSTR)szOut, (const atLPWSTR)szIn);
		}
	}
	else if (sizeof(atWCHAR) == 4) {
		atSTR_UCS2_to_UCS4((atUCS4_CHAR *)szOut, szIn, nLen);
	}
	else {
		return 0;
	}
	return atSTR_wcslen(szOut);
#else
	return atSTR_UCS2ANSI(szOut, szIn, nLen); // nLen < 0 일 경우, 전체
#endif
}

#endif

