//
// OEM_SYSAPI_SYSUTIL.h
//
////////////////////////////////////////////////////////////////

#ifndef __OEM_SA_SYSUTIL_H_
#define __OEM_SA_SYSUTIL_H_


#include "SA_Types.h"
#include "SYSAPI.h"


#ifdef __cplusplus
extern "C" {
#endif


typedef void	FN_OEM_SYSAPI_GetSystemDateTime(SYS_DATETIME *date);


typedef struct tagOEM_SYSAPI_SYSUTIL_T {
	FN_OEM_SYSAPI_GetSystemDateTime		*fnGetSystemDateTime;
} OEM_SYSAPI_SYSUTIL_T;


atVOID	OEM_SYSAPI_SYSUTIL_SetOemFn( OEM_SYSAPI_SYSUTIL_T *fnOem );
atVOID	OEM_SYSAPI_SYSUTIL_GetOemFn( OEM_SYSAPI_SYSUTIL_T *fnOem );


#ifdef __cplusplus
}
#endif


#endif // __OEM_SA_SYSUTIL_H_
