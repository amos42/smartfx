#include "SYSAPI.h"
#include "OEM_SA_Debug.h"

#include <stdio.h>

static atBOOL bActiveDebug = atFALSE;

atBOOL  OEM_SYSAPI_InitDebug( atLONG lSysInst, atLONG lInitData1, atLONG lInitData2 )
{
	return atTRUE;
}

atVOID  OEM_SYSAPI_FinalDebug(void)
{
}

atVOID OEM_SYSAPI_Alert( const atLPTSTR szMsg )
{
	return;
}

atBOOL OEM_SYSAPI_ActivateDebug( atBOOL bActivate )
{
	bActiveDebug = bActivate;
	
	return bActiveDebug;
}

atINT  OEM_SYSAPI_DebugPrint( const atLPTSTR str )
{
	if(bActiveDebug) {
		char ss[1024];
		SYSAPI_ToANSI(ss, str, -1);
		return printf("%s", ss);
	}
	
	return 0;
}


void init_sysapi_debug_oem() 
{
	OEM_SYSAPI_DEBUG_T oem_funcs = {
		OEM_SYSAPI_InitDebug,
		OEM_SYSAPI_FinalDebug,

		OEM_SYSAPI_Alert,

		OEM_SYSAPI_ActivateDebug,

		OEM_SYSAPI_DebugPrint
	};

	OEM_SYSAPI_DEBUG_SetOemFn( &oem_funcs );
}
