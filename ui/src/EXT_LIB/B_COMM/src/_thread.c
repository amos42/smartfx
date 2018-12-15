#include "_thread.h"
#include "_heap.h"

#ifdef __USES_SYSAPI_
#include "SYSAPI.h"
#else
#include <windows.h>
#endif

    
THREAD_T COMM_CreateThread( COMM_THREAD_FUNC *lpFunc, void *lpParam )
{  
#ifdef __USES_SYSAPI_
    return (THREAD_T)SYSAPI_CreateTask( _AT("CommThread"), (SYS_TASK_FUNC *)lpFunc, lpParam, atTRUE, 0 );
#else
	DWORD ThreadId;
	return (THREAD_T)CreateThread( NULL, 0, (LPTHREAD_START_ROUTINE)lpFunc, (void *)lpParam, 0, &ThreadId );
#endif
}

void COMM_DestroyThread( THREAD_T hThread )
{
#ifdef __USES_SYSAPI_
    SYSAPI_DestroyTask( (SYS_TASK)hThread );
#else
	CloseHandle( (HANDLE)hThread );
#endif
}

void COMM_Sleep(long millisec)
{
#ifdef __USES_SYSAPI_
	SYSAPI_Sleep( millisec );
#else
	Sleep(millisec);
#endif
}

CSECTION_T COMM_CreateCSection(void)
{
#ifdef __USES_SYSAPI_
	return (CSECTION_T)SYSAPI_CreateCriticalSection();
#else	
	LPCRITICAL_SECTION cr;
	cr = (LPCRITICAL_SECTION)COMM_alloc( sizeof(CRITICAL_SECTION) );
	
	InitializeCriticalSection( cr );

	return (CSECTION_T)cr;
#endif
}

void COMM_DestroyCSection( CSECTION_T hCSection )
{
#ifdef __USES_SYSAPI_
	SYSAPI_DestoryCriticalSection( (SYS_CRSECTION)hCSection );
#else
	DeleteCriticalSection( (LPCRITICAL_SECTION)hCSection );
	COMM_free( (void *)hCSection );
#endif
}

void COMM_EnterCSection( CSECTION_T hCSection )
{
#ifdef __USES_SYSAPI_
	SYSAPI_EnterCriticalSection( (SYS_CRSECTION)hCSection );
#else
	EnterCriticalSection( (LPCRITICAL_SECTION)hCSection );
#endif
}

void COMM_LeaveCSection( CSECTION_T hCSection )
{
#ifdef __USES_SYSAPI_
	SYSAPI_LeaveCriticalSection( (SYS_CRSECTION)hCSection );
#else
	LeaveCriticalSection( (LPCRITICAL_SECTION)hCSection );
#endif
}

