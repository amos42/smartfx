#ifndef ___FILE_H_
#define ___FILE_H_


#include "BC_Def.h"


#ifdef __cplusplus
extern "C" {
#endif


typedef HANDLE_T FILE_T;


#define COMM_FILEOPEN_NONE      0
#define COMM_FILEOPEN_READONLY  1
#define COMM_FILEOPEN_CREATE    2
#define COMM_FILEOPEN_APPEND    3


FILE_T 	COMM_OpenFile( LPTSTR_T szFileName, DWORD_T dwOpt );
void 	COMM_CloseFile( FILE_T hFile );
long 	COMM_GetFileSize( FILE_T hFile );
long 	COMM_GetFilePos( FILE_T hFile );
long 	COMM_ReadFile( FILE_T hFile, void *buf, long len );
long 	COMM_WriteFile( FILE_T hFile, void *buf, long len );


#ifdef __cplusplus
}
#endif


#endif
