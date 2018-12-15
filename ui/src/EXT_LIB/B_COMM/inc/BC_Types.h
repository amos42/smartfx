#ifndef __BC_TYPES_H_
#define __BC_TYPES_H_


#include "AT_Types.h"


#ifdef __cplusplus
extern "C" {
#endif
    

#ifndef __SIMPLE_BASE_TYPE_
#define __SIMPLE_BASE_TYPE_
typedef char            CHAR_T;
typedef int             INT_T;
typedef short           SHORT_T;
typedef long            LONG_T;
typedef unsigned short  WORD_T;
typedef unsigned long   DWORD_T;
typedef int             BOOL_T;
typedef double          REAL_T;
typedef unsigned int    UINT_T;
#endif

typedef atTCHAR  TCHAR_T;
typedef atLPTSTR LPTSTR_T;

typedef atHANDLE HANDLE_T;


#ifndef TRUE
#define TRUE (1)
#endif
#ifndef FALSE
#define FALSE (0)
#endif


#ifdef __cplusplus
}
#endif


#endif
