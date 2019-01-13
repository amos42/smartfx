/**
  @file OEM_UI_str.h
  @brief String에 관한 포팅 OEM API 선언
  @remark String에 관한 포팅 OEM API 선언
  @author Amos
  @date 2010-04-19 오전 11:41:00
  @see UI_str.h
*/
#ifndef __OEM_UI_STR_H_
#define __OEM_UI_STR_H_

#include "AT_Types.h"
#include <stdarg.h>


#ifdef __cplusplus
extern "C" {
#endif


typedef atVOID		FN_OEM_UI_memset( atVOID *dst, atCHAR value, atLONG len );
typedef atVOID		FN_OEM_UI_memcpy( atVOID *dst, const atVOID *src, atLONG len );
typedef atINT		FN_OEM_UI_strlen( atLPCTSTR str );
typedef atTCHAR *	FN_OEM_UI_strcpy( atLPTSTR dstr, atLPCTSTR sstr );
typedef atTCHAR *	FN_OEM_UI_strncpy( atLPTSTR dstr, atLPCTSTR sstr, atINT len );
typedef atTCHAR *	FN_OEM_UI_strcat( atLPTSTR dstr, atLPCTSTR sstr );
typedef atINT		FN_OEM_UI_strcmp( atLPCTSTR str1, atLPCTSTR str2 );
typedef atINT		FN_OEM_UI_stricmp( atLPCTSTR str1, atLPCTSTR str2 );
typedef atTCHAR *	FN_OEM_UI_strchr( atLPCTSTR str, atINT ch );
typedef atINT		FN_OEM_UI_vsprintf( atLPTSTR str, atLPCTSTR fmt, va_list ap );
typedef atINT 		FN_OEM_UI_atoi( atLPCTSTR szIn );
typedef atLPTSTR	FN_OEM_UI_itoa( atINT value, atLPTSTR szOut, atINT radix );
typedef atINT 		FN_OEM_UI_UCS2ANSI( atLPSTR szOut, const atLPWSTR szIn, atINT nLen );
typedef atINT 		FN_OEM_UI_ANSI2UCS( atLPWSTR szOut, const atLPSTR szIn, atINT nLen );


typedef struct tagOEM_UI_STR {
	FN_OEM_UI_memset	*fnMemset;
	FN_OEM_UI_memcpy	*fnMemcpy;
	FN_OEM_UI_strlen	*fnStrlen;
	FN_OEM_UI_strcpy	*fnStrcpy;
	FN_OEM_UI_strncpy	*fnStrncpy;
	FN_OEM_UI_strcat	*fnStrcat;
	FN_OEM_UI_strcmp	*fnStrcmp;
	FN_OEM_UI_stricmp	*fnStricmp;
	FN_OEM_UI_strchr	*fnStrchr;
	FN_OEM_UI_vsprintf	*fnVSPrintf;
	FN_OEM_UI_atoi		*fnAtoi;
	FN_OEM_UI_itoa		*fnItoa;
	FN_OEM_UI_UCS2ANSI	*fnUCS2ANSI;
	FN_OEM_UI_ANSI2UCS	*fnANSI2UCS;
} OEM_UI_STR_T;


atVOID	OEM_UI_Str_SetOemFn( OEM_UI_STR_T *fnOem );
atVOID	OEM_UI_Str_GetOemFn( OEM_UI_STR_T *fnOem );


#ifdef __cplusplus
}
#endif


#endif
