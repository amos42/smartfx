/**
  @file UI_Proc.c
  @brief Processor�� ���� API
  @remark Processor�� ���� API ����
  @author Amos
  @date 2010-04-19 ���� 11:41:00
  @see UI_Proc.h
*/
#include "UI_API/UI_API.h"
#include "OEM_UI_Proc.h"
#include "UI_Util.h"


static OEM_UI_PROC_T tOemFn;

atVOID	OEM_UI_Proc_SetOemFn( OEM_UI_PROC_T *fnOem )
{
	tOemFn = *fnOem;
}


atVOID	OEM_UI_Proc_GetOemFn( OEM_UI_PROC_T *fnOem )
{
	*fnOem = tOemFn;
}


/** 
 @brief 
 
 ......

 @param 
 @return ����
*/
atBOOL  	    atUIAPI_InitProcessor( atLONG lSysInst, atLONG lInitData1, atLONG lInitData2 )
{
    return tOemFn.fnInitProcessor( lSysInst, lInitData1, lInitData2 );
}


/** 
 @brief 
 
 ......

 @param 
 @return ����
*/
atVOID  	    atUIAPI_FinalProcessor(void)
{
    tOemFn.fnFinalProcessor();
}


/** 
 @brief 
 
 ......

 @param 
 @return ����
*/
atHPROCESSOR    atUIAPI_CreateProcessor( atLPTSTR szProcName, atINT nProcType, atBOOL (*callback)(atHPROCESSOR,void*), void *param )
{
    return (atHPROCESSOR)tOemFn.fnCreateProcessor( szProcName, nProcType, callback, param );
}


/** 
 @brief 
 
 ......

 @param 
 @return ����
*/
atVOID          atUIAPI_DestroyProcessor( atHPROCESSOR hProcessor )
{
    tOemFn.fnDestroyProcessor( (OEM_UI_PROCESSOR)hProcessor );
}


/** 
 @brief 
 
 ......

 @param 
 @return ����
*/
atVOID          atUIAPI_SuspendProcessor( atHPROCESSOR hProcessor )
{
    tOemFn.fnSuspendProcessor( (OEM_UI_PROCESSOR)hProcessor );
}


/** 
 @brief 
 
 ......

 @param 
 @return ����
*/
atVOID          atUIAPI_ResumeProcessor( atHPROCESSOR hProcessor )
{
    tOemFn.fnResumeProcessor( (OEM_UI_PROCESSOR)hProcessor );
}


/** 
 @brief 
 
 ......

 @param 
 @return ����
*/
atVOID          atUIAPI_WakeProcessor( atHPROCESSOR hProcessor )
{
    tOemFn.fnWakeProcessor( (OEM_UI_PROCESSOR)hProcessor );
}


/** 
 @brief 
 
 ......

 @param 
 @return ����
*/
atBOOL          atUIAPI_IsActiveProcessor( atHPROCESSOR hProcessor )
{
    return tOemFn.fnIsActiveProcessor( (OEM_UI_PROCESSOR)hProcessor );
}


/** 
 @brief 
 
 ......

 @param 
 @return ����
*/
atHCRSECTION    atUIAPI_CreateCriticalSection(void)
{
	return (atHCRSECTION)tOemFn.fnCreateCriticalSection();
}


/** 
 @brief 
 
 ......

 @param 
 @return ����
*/
atVOID          atUIAPI_DestroyCriticalSection( atHCRSECTION hCrSection )
{
	tOemFn.fnDestroyCriticalSection( (OEM_UI_CRSECTION)hCrSection );
}


/** 
 @brief 
 
 ......

 @param 
 @return ����
*/
atVOID          atUIAPI_EnterCriticalSection( atHCRSECTION hCrSection )
{
	tOemFn.fnEnterCriticalSection( (OEM_UI_CRSECTION)hCrSection );
}


/** 
 @brief 
 
 ......

 @param 
 @return ����
*/
atVOID          atUIAPI_LeaveCriticalSection( atHCRSECTION hCrSection )
{
	tOemFn.fnLeaveCriticalSection( (OEM_UI_CRSECTION)hCrSection );
}


atINT			atUIAPI_MessageLoop( atLONG lParam1, atLONG lParam2 )
{
	return tOemFn.fnMessageLoop( lParam1, lParam2 );
}
