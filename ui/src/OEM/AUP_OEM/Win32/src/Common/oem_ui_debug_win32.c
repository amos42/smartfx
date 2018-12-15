#include "A_UIENG.h"
#include "OEM_UI_Debug.h"
#include "SYSAPI.h"


static atBOOL  OEM_UI_InitDebug( atLONG lSysInst, atLONG lInitData1, atLONG lInitData2 )
{
// Memory, String, File, Debug, Task, Timer, Input, Sound, GPS�� �ý��ۿ��� �ʱ�ȭ�� �����Ѵ�.
//	return SYSAPI_InitDebug( lSysInst, lInitData1, lInitData2 );
	return atTRUE;
}

static atVOID  OEM_UI_FinalDebug(void)
{
// Memory, String, File, Debug, Task, Timer, Input, Sound, GPS�� �ý��ۿ��� �ʱ�ȭ�� �����Ѵ�.
//	SYSAPI_FinalDebug();
}

static atVOID OEM_UI_Alert( const atLPTSTR szMsg, atINT nType )
{
	SYSAPI_Alert( szMsg );
}

static atBOOL OEM_UI_ActivateDebug( atBOOL bActivate )
{
	return SYSAPI_ActivateDebug( bActivate );
}

static atINT  OEM_UI_DebugPrint( const atLPTSTR str )
{
	return SYSAPI_DebugPrint( str );
}



void init_uiapi_debug_oem() 
{
	OEM_UI_DEBUG_T oem_funcs = {
		OEM_UI_InitDebug,
		OEM_UI_FinalDebug,

		OEM_UI_ActivateDebug,
		OEM_UI_Alert,
		OEM_UI_DebugPrint
	};

	OEM_UI_Debug_SetOemFn( &oem_funcs );
}

