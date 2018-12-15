#include "UI_MNG.h"
#include "UI_API/UI_API.h"
#include "OEM_UI_Proc.h"



atBOOL  	    DEF_OEM_UI_InitProcessor( atLONG lSysInst, atLONG lInitData1, atLONG lInitData2 )
{
	return atTRUE;
}

atVOID  	    DEF_OEM_UI_FinalProcessor(void)
{
}

OEM_UI_PROCESSOR    DEF_OEM_UI_CreateProcessor( atLPTSTR szProcName, atINT nProcType, atBOOL (*callback)(OEM_UI_PROCESSOR, void*), void *param )
{
	return atNULL;
}

atVOID          DEF_OEM_UI_DestroyProcessor( OEM_UI_PROCESSOR OEM_Processor )
{
}

atVOID          DEF_OEM_UI_SuspendProcessor( OEM_UI_PROCESSOR OEM_Processor )
{
}

atVOID          DEF_OEM_UI_ResumeProcessor( OEM_UI_PROCESSOR OEM_Processor )
{
}

atVOID          DEF_OEM_UI_WakeProcessor( OEM_UI_PROCESSOR OEM_Processor )
{
}

atBOOL          DEF_OEM_UI_IsActiveProcessor( OEM_UI_PROCESSOR OEM_Processor )
{
	return atFALSE;
}

OEM_UI_CRSECTION    DEF_OEM_UI_CreateCriticalSection(void)
{
	return atNULL;
}

atVOID          DEF_OEM_UI_DestroyCriticalSection( OEM_UI_CRSECTION OEM_CrSection )
{
}

atVOID          DEF_OEM_UI_EnterCriticalSection( OEM_UI_CRSECTION OEM_CrSection )
{
}

atVOID          DEF_OEM_UI_LeaveCriticalSection( OEM_UI_CRSECTION OEM_CrSection )
{
}
