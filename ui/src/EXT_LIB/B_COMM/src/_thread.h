#ifndef ___THREAD_H_
#define ___THREAD_H_


#include "BC_Def.h"


#ifdef __cplusplus
extern "C" {
#endif


typedef HANDLE_T THREAD_T;
typedef HANDLE_T CSECTION_T;


//typedef DWORD_T (COMM_THREAD_FUNC)( THREAD_T hThread, void *lpParam );
typedef DWORD_T (COMM_THREAD_FUNC)( void *lpParam );
    

THREAD_T COMM_CreateThread( COMM_THREAD_FUNC *lpFunc, void *lpParam );
void COMM_DestroyThread( THREAD_T hThread );

void COMM_Sleep( long millisec );

DWORD_T COMM_WaitThreadForSignal(void);

CSECTION_T COMM_CreateCSection(void);
void COMM_DestroyCSection( CSECTION_T hCSection );
void COMM_EnterCSection( CSECTION_T hCSection );
void COMM_LeaveCSection( CSECTION_T hCSection );



#ifdef __cplusplus
}
#endif


#endif
