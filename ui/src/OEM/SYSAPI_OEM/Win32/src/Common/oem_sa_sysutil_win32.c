#include "OEM_SA_sysutil.h"
#include <string.h>
#include <time.h>


static void OEM_SYSAPI_GetSystemDateTime(SYS_DATETIME *date)
{
	time_t t;
	struct tm *tmr;

	t = time(atNULL);
	tmr = localtime(&t);

	date->nYear		= tmr->tm_year + 1900;
	date->nMonth	= tmr->tm_mon + 1;
	date->nDay		= tmr->tm_mday;
	date->nHour		= tmr->tm_hour;
	date->nMinute	= tmr->tm_min;
	date->nSec		= tmr->tm_sec;
}


void init_sysapi_sysutil_oem(void) 
{
	OEM_SYSAPI_SYSUTIL_T oem_funcs = {
		OEM_SYSAPI_GetSystemDateTime
	};

	OEM_SYSAPI_SYSUTIL_SetOemFn( &oem_funcs );
}
