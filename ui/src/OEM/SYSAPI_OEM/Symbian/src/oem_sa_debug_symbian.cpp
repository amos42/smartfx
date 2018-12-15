#include "SYSAPI.h"
#include <stdio.h>
#include <string.h>
#include <wchar.h>
#include "OEM_SA_Debug.h"

#include "EIKDEF.H"
#include "EIKENV.H"
#include "e32svr.h"


atBOOL  OEM_SYSAPI_InitDebug( atLONG lSysInst, atLONG lInitData1, atLONG lInitData2 )
{
	return atTRUE;
}

atVOID  OEM_SYSAPI_FinalDebug(void)
{
}

atVOID OEM_SYSAPI_Alert( const atLPTSTR szMsg )
{
	//CEikonEnv::Static()->AlertWin( szMsg );
}

atBOOL OEM_SYSAPI_ActivateDebug( atBOOL bActivate )
{
	return atTRUE;
}

atINT  OEM_SYSAPI_DebugPrint( const atLPTSTR str )
{
	//RDebug::Print( str ); 	
}

