#include "SYSAPI.h"
#include "OEM_SA_Debug.h"
#include <stdio.h>
#include <fcntl.h>


static atBOOL g_bDebugActivate;


atBOOL  OEM_SYSAPI_InitDebug( atLONG lSysInst, atLONG lInitData1, atLONG lInitData2 )
{
	g_bDebugActivate = atFALSE;
	
	return atTRUE;
}

atVOID  OEM_SYSAPI_FinalDebug(void)
{
	if( g_bDebugActivate ){
		OEM_SYSAPI_ActivateDebug( atFALSE );
	}
}

atVOID OEM_SYSAPI_Alert( const atLPTSTR szMsg )
{
//	MessageBox( NULL, szMsg, _AT("Info"), MB_OK );
}

atBOOL OEM_SYSAPI_ActivateDebug( atBOOL bActivate )
{
	g_bDebugActivate = bActivate;
	
	return atTRUE;
}

atINT  OEM_SYSAPI_DebugPrint( const atLPTSTR str )
{
	char out_str[256];
	
	if( !g_bDebugActivate ) return 0;
		
//	SYSAPI_ToANSI( out_str, str, 256-1 );
	
//	return printf( out_str );
    return 0;
}

