#include "B_Comm.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <windows.h>


#include "COMM_Def.h"
#include "COMM_Data.h"

#include "B_Comm_port_cpp.inl"
#include "B_Comm_sim_cpp.inl"
#include "B_Comm_log_cpp.inl"

#include "_heap.h"
#include "_thread.h"


//static DWORD_T (*_CommProcess) (void *lpParam)
//static DWORD_T readCommGPSProc (void *lpParam);



COMM_T COMM_CreateComm( void )
{
	_TComm *cmm = (_TComm *)COMM_alloc( sizeof(_TComm) );
	COMM_memset( cmm, 0, sizeof(_TComm) );

	cmm->bActivate = atTRUE;
	cmm->bConnected = atFALSE;

	cmm->lpFunc = atNULL;

	COMM_InitCommPort( cmm );
	COMM_InitCommSimul( cmm );

	COMM_InitCommLog( cmm );

	return (COMM_T)cmm;
}

void COMM_DestroyComm( COMM_T hComm )
{
	_TComm *cmm = (_TComm *)hComm;
	if( cmm == atNULL ) return;

	cmm->bActivate = atFALSE;

	if( COMM_IsConnected(hComm) ){
		COMM_CloseComm(hComm);
	}

	if( COMM_IsLogging(cmm) ){
		COMM_StopCommLog(cmm);
	}
	
//	DeleteCriticalSection ( &cmm->m_cs ) ;

	COMM_free( cmm );
}

BOOL_T COMM_SetCallbackFunc( COMM_T hComm, COMM_CALLBACK_FUNC *lpFunc )
{
	_TComm *cmm = (_TComm *)hComm;
	if( cmm == atNULL ) return FALSE;

	cmm->lpFunc = lpFunc;

	return TRUE;
}


BOOL_T COMM_SetCommType( COMM_T hComm, UINT_T nCommType )
{
	_TComm *cmm = (_TComm *)hComm;
	if( cmm == atNULL ) return FALSE;

	if( nCommType == 0 ){
		cmm->nCommType = 0;
	} else if( nCommType == 1 ){
		cmm->nCommType = 1;
	} else {
		cmm->nCommType = 0;
		return FALSE;
	}
	
	return TRUE;
}


/*-----------------------------------------------------------------------------

FUNCTION: SetupCommPort( void )

PURPOSE: Setup Communication Port with our settings

RETURN: 
    Handle of comm port is successful
    NULL is error occurs

HISTORY:   Date:      Author:     Comment:
           10/27/95   AllenD      Wrote it

-----------------------------------------------------------------------------*/
BOOL_T COMM_OpenComm( COMM_T hComm, LPTSTR_T szCommPort, UINT_T nBaudRate )
{
	_TComm *cmm = (_TComm *)hComm;
	if( cmm == atNULL ) return FALSE;

    if (cmm->bConnected) return FALSE;

	COMM_strncpy(cmm->szCommPort, szCommPort, 32);
	cmm->nBaudRate     = nBaudRate;
	cmm->bConnected = FALSE;
	
	BOOL_T ret = FALSE;
	if( cmm->nCommType == 0 ){
		ret = COMM_OpenCommPort( cmm, szCommPort, nBaudRate );
	} else if( cmm->nCommType == 1 ){
		ret = COMM_StartSimulation( hComm, szCommPort, nBaudRate, 512 );
	}
	
	// set overall connect flag
	cmm->bConnected = ret;

//COMM_StartCommLog(cmm, _AT("\\test.log")); // 임시

	return ret;
}


/*-----------------------------------------------------------------------------

FUNCTION: closeComm

PURPOSE: Closes a connection to a comm port

RETURN:
    TRUE  - successful breakdown of port
    FALSE - port isn't connected

COMMENTS: Waits for threads to exit,
          clears DTR, restores comm port timeouts, purges any i/o
          and closes all pertinent handles

HISTORY:   Date:      Author:     Comment:
           10/27/95   AllenD      Wrote it

-----------------------------------------------------------------------------*/
BOOL_T COMM_CloseComm( COMM_T hComm )
{
	_TComm *cmm = (_TComm *)hComm;
	if( cmm == atNULL ) return FALSE;
	if( cmm->hCommDev == INVALID_HANDLE_VALUE ) return TRUE;

    if (!cmm->bConnected) return TRUE ;

//COMM_StopCommLog(cmm); // 임시

	if( cmm->nCommType == 0 ){
		COMM_CloseCommPort( cmm );
	} else if( cmm->nCommType == 1 ){
		COMM_StopSimulation( hComm );
	}

    return TRUE;
}


BOOL_T COMM_IsConnected ( COMM_T hComm )
{
	_TComm *cmm = (_TComm *)hComm;
	if( cmm == atNULL ) return FALSE;

	return cmm->bConnected;
}


void COMM_SetUserTag(COMM_T hComm, DWORD_T dwUserTag)
{
	_TComm *cmm = (_TComm *)hComm;
	if( cmm == atNULL ) return;

	cmm->dwUserTag = dwUserTag;
}

DWORD_T COMM_GetUserTag(COMM_T hComm)
{
	_TComm *cmm = (_TComm *)hComm;
	if( cmm == atNULL ) return 0;

	return cmm->dwUserTag;
}


DWORD_T COMM_ReadComm (COMM_T hComm, void * buf , DWORD_T len)
{
	_TComm *cmm = (_TComm *)hComm;
	if( cmm == atNULL ) return 0;
	if( buf == atNULL ) return 0;

	if ( !cmm->bConnected )	return 0;

	if( len <= 0 ) return 0;
	COMM_memset( buf, 0, len );

	int ret = 0;
	if( cmm->nCommType == 0 ){
		ret = COMM_ReadCommPort( cmm, buf, len );

		if( cmm->bProcessLogging ){
			COMM_WriteLogData( cmm, buf, ret );
		}
	} else if( cmm->nCommType == 1 ){
		ret = COMM_ReadCommSimul( cmm, buf, len );
	}

	return ret;
}


DWORD_T COMM_WriteComm (COMM_T hComm, const void * buf , int len)
{
	_TComm *cmm = (_TComm *)hComm;
	if( cmm == atNULL ) return 0;
	if( buf == atNULL ) return 0;

	if ( !cmm->bConnected )	return 0;

	int ret = 0;
	if( cmm->nCommType == 0 ){
		ret = COMM_WriteCommPort( cmm, buf, len );
	} else if( cmm->nCommType == 1 ){
//		ret = COMM_WriteCommSimul( cmm, buf, len );
	}

	return ret;
}

