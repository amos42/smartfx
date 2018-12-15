#ifndef __B_COMM_H_
#define __B_COMM_H_


#include "BC_Def.h"


#ifdef __cplusplus
extern "C" {
#endif
    

typedef void *  COMM_T;

typedef int (COMM_CALLBACK_FUNC)( COMM_T hComm, int msg, long nParam1, long nParam2 );


enum {
	COMM_MSG_NONE = 0,

	COMM_MSG_INQUEUE,
	COMM_MSG_OUT_COMPLETE,

	COMM_MSG_MAX
};


COMM_T	COMM_CreateComm(void);
void	COMM_DestroyComm( COMM_T hComm );

BOOL_T COMM_SetCommType( COMM_T hComm, UINT_T nCommType );
BOOL_T	COMM_SetCallbackFunc( COMM_T hComm, COMM_CALLBACK_FUNC *lpFunc );
BOOL_T	COMM_OpenComm( COMM_T hComm, LPTSTR_T szCommPort, UINT_T nBaudRate );
BOOL_T	COMM_CloseComm( COMM_T hComm );
BOOL_T	COMM_IsConnected( COMM_T hComm );

void 	COMM_SetUserTag(COMM_T hComm, DWORD_T dwUserTag);
DWORD_T COMM_GetUserTag(COMM_T hComm);

DWORD_T COMM_ReadComm (COMM_T hComm, void * buf , DWORD_T len);
DWORD_T COMM_WriteComm (COMM_T hComm, const void * buf , DWORD_T len);

BOOL_T	COMM_StartSimulation(COMM_T hComm, LPTSTR_T szFileName, UINT_T nInterval, UINT_T nBlkSize );
BOOL_T	COMM_StopSimulation(COMM_T hComm);

BOOL_T	COMM_StartCommLog(COMM_T hComm, LPTSTR_T szLogFileName);
void	COMM_StopCommLog(COMM_T hComm);


#ifdef __cplusplus
}
#endif


#endif
