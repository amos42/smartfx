/**
  @file UI_str.h
  @brief String에 관한 API
  @remark String에 관한 API 모음
  @author Amos
  @date 2010-04-19 오전 11:41:00
  @see UI_str.c
*/
#ifndef __UI_STR_H_
#define __UI_STR_H_

#include "UI_Types.h"
#include <stdarg.h>


#ifdef __cplusplus
extern "C" {
#endif

atVOID    atUIAPI_memset( atVOID *dst, atCHAR value, atLONG len );
atVOID    atUIAPI_memcpy( atVOID *dst, const atVOID *src, atLONG len );
atINT     atUIAPI_strlen( atLPCTSTR str );
atTCHAR * atUIAPI_strcpy( atLPTSTR dstr, atLPCTSTR sstr );
atTCHAR * atUIAPI_strncpy( atLPTSTR dstr, atLPCTSTR sstr, atLONG len );
atTCHAR * atUIAPI_strcat( atLPTSTR dstr, atLPCTSTR sstr );
atINT     atUIAPI_strcmp( atLPCTSTR str1, atLPCTSTR str2 );
atINT     atUIAPI_stricmp( atLPCTSTR str1, atLPCTSTR str2 );
atTCHAR * atUIAPI_strchr( atLPCTSTR str, atINT ch );

atINT     atUIAPI_vsprintf( atLPTSTR str, atLPCTSTR fmt, va_list ap );
atINT     atUIAPI_sprintf( atLPTSTR str, atLPCTSTR fmt, ... );
atTCHAR * atUIAPI_strins( atLPTSTR dstr, atLPCTSTR sstr, atINT nIndex, atINT nInsLen );
atTCHAR * atUIAPI_strdel( atLPTSTR dstr, atINT nIndex, atINT nDelLen );
atINT 	  atUIAPI_atoi( atLPCTSTR szIn );
atLPTSTR   atUIAPI_itoa( atINT _value, atLPTSTR szOut, atINT radix );

atINT 	  atUIAPI_UCS2ANSI( atLPSTR szOut, const atLPWSTR szIn, atINT nLen );
atINT 	  atUIAPI_ANSI2UCS( atLPWSTR szOut, const atLPSTR szIn, atINT nLen );
atINT 	  atUIAPI_ToANSI( _OUT_ atLPSTR szOut, _IN_ atLPCTSTR szIn, atINT nLen );
atINT 	  atUIAPI_ToUCS(  _OUT_ atLPWSTR szOut, _IN_ atLPCTSTR szIn, atINT nLen );
atINT 	  atUIAPI_FromANSI( _OUT_ atLPTSTR szOut, _IN_ const atLPSTR szIn, atINT nLen );
atINT 	  atUIAPI_FromUCS(  _OUT_ atLPTSTR szOut, _IN_ const atLPWSTR szIn, atINT nLen );


#ifdef __cplusplus
}
#endif


#endif
