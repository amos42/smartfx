#include "A_UIENG.h"
#include "OEM_UI_Debug.h"
#include "SYSAPI.h"


atBOOL  OEM_UI_InitDebug( atLONG lSysInst, atLONG lInitData1, atLONG lInitData2 )
{
// Memory, String, File, Debug, Task, Timer, Input, Sound, GPS�� �ý��ۿ��� �ʱ�ȭ�� �����Ѵ�.
//	return SYSAPI_InitDebug( lSysInst, lInitData1, lInitData2 );
	return atTRUE;
}

atVOID  OEM_UI_FinalDebug(void)
{
// Memory, String, File, Debug, Task, Timer, Input, Sound, GPS�� �ý��ۿ��� �ʱ�ȭ�� �����Ѵ�.
//	SYSAPI_FinalDebug();
}

atVOID OEM_UI_Alert( const atLPTSTR szMsg, atINT nType )
{
//	MessageBox (NULL, szMsg, _AT("Info"), MB_OK);
}

atBOOL OEM_UI_ActivateDebug( atBOOL bActivate )
{
	return SYSAPI_ActivateDebug( bActivate );
}

atINT  OEM_UI_DebugPrint( const atLPTSTR str )
{
	return SYSAPI_DebugPrint( str );
}


