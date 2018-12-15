#include "B_Comm.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <windows.h>


#include "COMM_Def.h"
#include "COMM_Data.h"

#include "_heap.h"
#include "_file.h"


BOOL_T COMM_InitCommLog( _TComm *cmm )
{
	cmm->bProcessLogging = FALSE;
	cmm->hLogFile = NULL;

	return TRUE;
}

BOOL_T COMM_StartCommLog(_TComm *cmm, LPTSTR_T szLogFileName)
{
	cmm->hLogFile = COMM_OpenFile( szLogFileName, COMM_FILEOPEN_APPEND );
	if( cmm->hLogFile != NULL )
		cmm->bProcessLogging = TRUE;	

	return cmm->bProcessLogging;
}

void COMM_StopCommLog(_TComm *cmm)
{
	cmm->bProcessLogging = FALSE;	

	if( cmm->hLogFile != NULL ){
		COMM_CloseFile( cmm->hLogFile );
		cmm->hLogFile = NULL;
	}
}

DWORD_T COMM_WriteLogData(_TComm *cmm, const void * buf , int len)
{
	if( !cmm->bProcessLogging ) return 0;
	
	return COMM_WriteFile( cmm->hLogFile, (void *)buf, len );
}


BOOL_T COMM_IsLogging(_TComm *cmm)
{
	return cmm->bProcessLogging;
}

