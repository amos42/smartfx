#include "SYSAPI.h"
#include "OEM_SA_Debug.h"

#include <stdio.h>


atBOOL  OEM_SYSAPI_InitDebug( atLONG lSysInst, atLONG lInitData1, atLONG lInitData2 )
{
	return atTRUE;
}

atVOID  OEM_SYSAPI_FinalDebug(void)
{
}

atVOID OEM_SYSAPI_Alert( const atLPTSTR szMsg )
{
	return atTRUE;
}

atBOOL OEM_SYSAPI_ActivateDebug( atBOOL bActivate )
{
}

atINT  OEM_SYSAPI_DebugPrint( const atLPTSTR str )
{
	char ss[1024];
	SYSAPI_ToANSI(ss, str, -1);
	return fprintf(stderr, ss);
}

