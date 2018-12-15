#ifndef __OEM_SA_FILE_H_
#define __OEM_SA_FILE_H_


#include "SA_Types.h"
#include "SA_File.h"


#ifdef __cplusplus
extern "C" {
#endif


typedef void *OEM_SYSAPI_FILE;


typedef atBOOL 	FN_OEM_SYSAPI_InitFile( atLONG lSysInst, atLONG lInitData1, atLONG lInitData2 );
typedef void 	FN_OEM_SYSAPI_FinalFile( NOARGS );

typedef atBOOL	FN_OEM_SYSAPI_ChangeDir( atLPCTSTR szDirName );
typedef void	FN_OEM_SYSAPI_GetCurDir( atLPTSTR szDirName );
typedef atBOOL	FN_OEM_SYSAPI_CreateDir( atLPCTSTR szDirName );
typedef atBOOL	FN_OEM_SYSAPI_DeleteDir( atLPCTSTR szDirName );
typedef atBOOL	FN_OEM_SYSAPI_RenameDir( atLPCTSTR szSrcDir, atLPCTSTR szDesDir);

typedef OEM_SYSAPI_FILE FN_OEM_SYSAPI_OpenFile( atLPCTSTR szFileName, int nOpenMode );
typedef void 	FN_OEM_SYSAPI_CloseFile( OEM_SYSAPI_FILE hFile );
typedef long 	FN_OEM_SYSAPI_ReadFile( OEM_SYSAPI_FILE hFile, void *pbOut, long nSize );
typedef long 	FN_OEM_SYSAPI_WriteFile( OEM_SYSAPI_FILE hFile, void *pbIn, long nSize );
typedef void	FN_OEM_SYSAPI_FlushFileBuffer( OEM_SYSAPI_FILE hFile);
typedef long 	FN_OEM_SYSAPI_SeekFile( OEM_SYSAPI_FILE hFile, long lPos, int nSeekMode );
typedef long	FN_OEM_SYSAPI_GetFilePos( OEM_SYSAPI_FILE hFile );
typedef atBOOL	FN_OEM_SYSAPI_IsEOF( OEM_SYSAPI_FILE hFile );
typedef long	FN_OEM_SYSAPI_GetFileLength( OEM_SYSAPI_FILE hFile );

typedef atBOOL 	FN_OEM_SYSAPI_IsExistFile( atLPCTSTR pFileName );
typedef atBOOL	FN_OEM_SYSAPI_DeleteFile( atLPCTSTR szFileName );
typedef atBOOL	FN_OEM_SYSAPI_RenameFile( atLPCTSTR szSrcName, atLPCTSTR szDesName );

typedef atBOOL  FN_OEM_SYSAPI_FindFirstFile( atLPCTSTR szPathName, atLPCTSTR szFilter, SYS_FINDFILEENTRY *lpFindFileEntry );
typedef atBOOL  FN_OEM_SYSAPI_FindNextFile( SYS_FINDFILEENTRY *lpFindFileEntry );
typedef atVOID  FN_OEM_SYSAPI_CloseFindFile( SYS_FINDFILEENTRY *lpFindFileEntry );


typedef struct tagOEM_SYSAPI_FILE_T {
	FN_OEM_SYSAPI_InitFile			*fnInitFile;
	FN_OEM_SYSAPI_FinalFile			*fnFinalFile;

	FN_OEM_SYSAPI_ChangeDir			*fnChangeDir;
	FN_OEM_SYSAPI_GetCurDir			*fnGetCurDir;
	FN_OEM_SYSAPI_CreateDir			*fnCreateDir;
	FN_OEM_SYSAPI_DeleteDir			*fnDeleteDir;
	FN_OEM_SYSAPI_RenameDir			*fnRenameDir;

	FN_OEM_SYSAPI_OpenFile			*fnOpenFile;
	FN_OEM_SYSAPI_CloseFile			*fnCloseFile;
	FN_OEM_SYSAPI_ReadFile			*fnReadFile;
	FN_OEM_SYSAPI_WriteFile			*fnWriteFile;
	FN_OEM_SYSAPI_FlushFileBuffer	*fnFlushFileBuffer;
	FN_OEM_SYSAPI_SeekFile			*fnSeekFile;
	FN_OEM_SYSAPI_GetFilePos		*fnGetFilePos;
	FN_OEM_SYSAPI_IsEOF				*fnIsEOF;
	FN_OEM_SYSAPI_GetFileLength		*fnGetFileLength;

	FN_OEM_SYSAPI_IsExistFile		*fnIsExistFile;
	FN_OEM_SYSAPI_DeleteFile		*fnDeleteFile;
	FN_OEM_SYSAPI_RenameFile		*fnRenameFile;

	FN_OEM_SYSAPI_FindFirstFile		*fnFindFirstFile;
	FN_OEM_SYSAPI_FindNextFile		*fnFindNextFile;
	FN_OEM_SYSAPI_CloseFindFile		*fnCloseFindFile;
} OEM_SYSAPI_FILE_T;


atVOID	OEM_SYSAPI_FILE_SetOemFn( OEM_SYSAPI_FILE_T *fnOem );
atVOID	OEM_SYSAPI_FILE_GetOemFn( OEM_SYSAPI_FILE_T *fnOem );


#ifdef __cplusplus
}
#endif

#endif

