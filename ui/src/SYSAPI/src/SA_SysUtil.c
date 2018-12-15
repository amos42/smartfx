#include "AT_Types.h"
#include "SYSAPI.h"
#include "OEM_SA_SysUtil.h"


static OEM_SYSAPI_SYSUTIL_T tOemFn;


atVOID OEM_SYSAPI_SYSUTIL_SetOemFn(OEM_SYSAPI_SYSUTIL_T *fnOem)
{
	tOemFn = *fnOem;
}


atVOID OEM_SYSAPI_SYSUTIL_GetOemFn(OEM_SYSAPI_SYSUTIL_T *fnOem)
{
	*fnOem = tOemFn;
}


void SYSAPI_GetSystemDateTime(SYS_DATETIME *date)
{
	if( tOemFn.fnGetSystemDateTime != atNULL ){
		tOemFn.fnGetSystemDateTime(date);
	}
}
