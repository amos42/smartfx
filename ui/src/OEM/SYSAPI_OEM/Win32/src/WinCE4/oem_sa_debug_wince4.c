#include "SYSAPI.h"
#include "OEM_SA_Debug.h"
#include <windows.h>
#if _MSC_VER==1201 // eVC ver4
#include "LogClientApp.h"
#endif

atBOOL  OEM_SYSAPI_InitDebug( atLONG lSysInst, atLONG lInitData1, atLONG lInitData2 )
{
	return atTRUE;
}

atVOID  OEM_SYSAPI_FinalDebug(void)
{
}

atVOID OEM_SYSAPI_Alert( const atLPTSTR szMsg )
{
	MessageBox( NULL, szMsg, _AT("Info"), MB_OK );
}

atBOOL OEM_SYSAPI_ActivateDebug( atBOOL bActivate )
{
	return atTRUE;
}

atINT  OEM_SYSAPI_DebugPrint( const atLPTSTR str )
{
#if _MSC_VER==1201 // eVC ver4
#ifdef __USES_UNICODE_
	SendTornado_LOG_MSGT( str );
#else
	SendTornado_LOG_MSG( str );
#endif
#endif

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
