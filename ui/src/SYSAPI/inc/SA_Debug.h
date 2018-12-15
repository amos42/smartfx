//
// Debug.h
//
////////////////////////////////////////////////////////////////

#ifndef __SA_DEBUG_H_
#define __SA_DEBUG_H_

#include "SA_Types.h"


#ifdef __cplusplus
extern "C" {
#endif


atBOOL  	    SYSAPI_InitDebug( atLONG lSysInst, atLONG lInitData1, atLONG lInitData2 );
atVOID  	    SYSAPI_FinalDebug(void);

atVOID        SYSAPI_Alert( const atLPTSTR szMsg );
atVOID        SYSAPI_Alertf( const atLPTSTR fmt, ... );

atBOOL        SYSAPI_ActivateDebug( atBOOL bActivate );

atINT           SYSAPI_DebugPrint( const atLPTSTR str );
atINT           SYSAPI_DebugPrintf( const atLPTSTR fmt, ... );
atVOID        SYSAPI_DebugDump( const atLPBYTE ptr, int len );



/*
atINT SYSAPI_LogInit(atUINT mode, const atLPTSTR fileName);
atINT SYSAPI_LogPrintf(atUINT mode, const atCHAR* fmt, ...);
atINT SYSAPI_LogClose();
atINT SYSAPI_LogPrintf(atUINT mode, const atCHAR* fmt, ...);
atINT SYSAPI_LogPrintfW(atUINT mode, const atLPTSTR fmt, ...);

atINT SYSAPI_LogMemoryStatus(const atCHAR* fmt, ...);
atINT SYSAPI_LogMemoryStatusW(const atLPTSTR fmt, ...);
*/


#ifdef __cplusplus
}
#endif


#endif //  __DEBUG_H_
