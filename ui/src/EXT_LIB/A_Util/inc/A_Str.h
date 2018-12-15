#ifndef __STRFUNC_H_
#define __STRFUNC_H_


#include "AT_Types.h"
#include <stdarg.h>


#ifdef __cplusplus
extern "C" {
#endif


atVOID    atSTR_memset( atLPVOID des, atCHAR ch, atLONG len );

atLPSTR   atSTR_strupr( atLPSTR str );
atLPSTR   atSTR_strncpy( atLPSTR des, atLPCSTR src, atLONG len );
atINT     atSTR_strcmp( atLPCSTR str1, atLPCSTR str2 );

atLPTSTR  atSTR_wcsncpy( atLPWSTR des, atLPCWSTR src, atLONG len );

atLPTSTR  atSTR_tcsncpy( atLPTSTR des, atLPCTSTR src, atLONG len );

atINT	atSTR_vsnprintf( atLPSTR str, atINT nLen, atLPCSTR fmt, va_list ap );
atINT	atSTR_vsnscanf( atLPSTR str, atINT nLen, atLPCSTR fmt, va_list ap );
atINT	atSTR_vsnwprintf( atLPWSTR str, atINT nLen, atLPCWSTR fmt, va_list ap );
atINT	atSTR_vsnwscanf( atLPWSTR str, atINT nLen, atLPCWSTR fmt, va_list ap );


#ifdef __cplusplus
}
#endif


#endif
