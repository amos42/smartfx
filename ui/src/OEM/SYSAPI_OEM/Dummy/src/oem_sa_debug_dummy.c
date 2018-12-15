#include "SYSAPI.h"
#include "OEM_SA_Debug.h"
#include <stdio.h>
#include <fcntl.h>


atBOOL  OEM_SYSAPI_InitDebug( atLONG lSysInst, atLONG lInitData1, atLONG lInitData2 )
{
	return atTRUE;
}

atVOID  OEM_SYSAPI_FinalDebug(void)
{
}

atVOID OEM_SYSAPI_Alert( const atLPTSTR szMsg )
{
}

atBOOL OEM_SYSAPI_ActivateDebug( atBOOL bActivate )
{
	return atTRUE;
}

atINT  OEM_SYSAPI_DebugPrint( const atLPTSTR str )
{
    return 0;
}

