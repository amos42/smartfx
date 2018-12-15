#include "B_Comm.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <windows.h>


#include "COMM_Def.h"
#include "COMM_Data.h"

#include "_file.h"
#include "_heap.h"
#include "_thread.h"


static DWORD_T readSimulProc (void *lpParam);



BOOL_T COMM_InitCommSimul( _TComm *cmm )
{
//	cmm->bProcessSimulation = FALSE;
	cmm->hSimulFile = NULL;
	cmm->hSimulThread = NULL;
	cmm->nSimulInterval = 0;
	cmm->nSimulBlkSize = 0;
	cmm->nSimulFileSize = 0;

	return TRUE;
}

BOOL_T COMM_StartSimulation(COMM_T hComm, LPTSTR_T szFileName, UINT_T nInterval, UINT_T nBlkSize )
{
	_TComm *cmm = (_TComm *)hComm;
	if( cmm == atNULL ) return FALSE;

	cmm->hSimulFile = COMM_OpenFile( szFileName, COMM_FILEOPEN_READONLY );
	if( cmm->hSimulFile == NULL ) return FALSE;

	cmm->nSimulFileSize = COMM_GetFileSize( cmm->hSimulFile );
	cmm->nSimulInterval = nInterval;
	cmm->nSimulBlkSize = nBlkSize;

	cmm->hSimulThread = COMM_CreateThread( readSimulProc, (void *)cmm );
	if( cmm->hSimulThread == NULL ){
		COMM_CloseFile( cmm->hSimulFile );
		cmm->hSimulFile = NULL;
		return FALSE;
	}
	
//	cmm->bProcessSimulation = TRUE;

	return atTRUE;
}

BOOL_T COMM_StopSimulation(COMM_T hComm)
{
	_TComm *cmm = (_TComm *)hComm;
	if( cmm == NULL ) return FALSE;

	if( cmm->hSimulFile ){
		COMM_CloseFile( cmm->hSimulFile );
		cmm->hSimulFile = NULL;
	}

	if(cmm->hSimulThread)
	{
		COMM_DestroyThread( cmm->hSimulThread );
		cmm->hSimulThread = NULL;
	}
	
//	cmm->bProcessSimulation = FALSE;
	
	return TRUE;
}

BOOL_T COMM_IsSimulation(_TComm *cmm)
{
	if( cmm == atNULL ) return 0;

	return (cmm->hSimulFile != NULL)? TRUE : FALSE;
}

DWORD_T COMM_ReadCommSimul (_TComm *cmm, void * buf , DWORD_T len)
{
	if( cmm == atNULL ) return 0;

	return COMM_ReadFile( cmm->hSimulFile, buf, len );
}


static DWORD_T readSimulProc (void *lpParam)
{
	COMM_T hComm = (COMM_T)lpParam;

	_TComm *cmm = (_TComm *)hComm;
	if( cmm == atNULL ) return 0;

	long pos, sz;

	while( cmm->bActivate && cmm->bConnected && cmm->hSimulFile )
	{
//		ret = WaitForSingleObject(event, cmm->interval);
//		if( !ret ) continue;
		Sleep( cmm->nSimulInterval );

		pos = COMM_GetFilePos( cmm->hSimulFile );
		if( pos < cmm->nSimulFileSize ){
			sz = cmm->nSimulFileSize - pos;
			if( sz > cmm->nSimulBlkSize ) sz = cmm->nSimulBlkSize;
			if( cmm->lpFunc ){
				cmm->lpFunc( hComm, COMM_MSG_INQUEUE, (long)sz, 0 );
			}		
		}
	}

	return 0;
}

