#ifndef __SA_FILE_H_
#define __SA_FILE_H_


#ifndef __AT_TYPES_H_
#include "AT_Types.h"
#endif


#ifdef __cplusplus
extern "C" {
#endif


typedef atHANDLE    SYS_HFILE;


typedef enum {
	SYS_FILEOPEN_RDONLY = 0,
	SYS_FILEOPEN_WRONLY,
	SYS_FILEOPEN_RDWR,
	SYS_FILEOPEN_CREATE,
	SYS_FILEOPEN_APPEND
}  SYS_FILEOPEN;

typedef enum {
	SYS_FILESEEK_SET = 0,
	SYS_FILESEEK_CUR,
	SYS_FILESEEK_END
} SYS_FILESEEK;

#define SYS_FINDFILE_ATTR_VOLUME		0x8000
#define SYS_FINDFILE_ATTR_DIRECTORY		0x4000
#define SYS_FINDFILE_ATTR_READONLY		0x2000
#define SYS_FINDFILE_ATTR_HIDDEN		0x1000
#define SYS_FINDFILE_ATTR_SYSTEM		0x0800

typedef struct tagSYS_FINDFILEENTRY {
	atTCHAR     szPathName[256];
	atTCHAR     szFilter[256];
	atVOID *    hFindHandle;
	
	atDWORD 	dwAttr;
	atTCHAR 	szFileName[256]; 
	atLONG 		nFileSize;	
	atDATETIME	dDateTime;	
} SYS_FINDFILEENTRY;


atBOOL	SYSAPI_InitFile( atLONG lSysInst, atLONG lInitData1, atLONG lInitData2 );
void	SYSAPI_FinalFile( NOARGS );

atBOOL	SYSAPI_ChangeDir( atLPCTSTR szDirName );
void	SYSAPI_GetCurDir( atLPTSTR szDirName );
atBOOL	SYSAPI_CreateDir( atLPCTSTR szDirName );
atBOOL	SYSAPI_DeleteDir( atLPCTSTR szDirName );
atBOOL	SYSAPI_RenameDir( atLPCTSTR szSrcDir, atLPCTSTR szDesDir );

atBOOL 	SYSAPI_IsExistFile( atLPCTSTR pFileName );
long	SYSAPI_GetFileLength( SYS_HFILE hFile );
long 	SYSAPI_GetFileSize( atLPCTSTR szFileName );
atBOOL	SYSAPI_DeleteFile( atLPCTSTR szFileName );
atBOOL 	SYSAPI_RenameFile( atLPCTSTR szSrcName, atLPCTSTR szDesName );

SYS_HFILE SYSAPI_OpenFile( atLPCTSTR szFileName, SYS_FILEOPEN nOpenMode );
void 	SYSAPI_CloseFile( SYS_HFILE hFile );
long 	SYSAPI_ReadFile( SYS_HFILE hFile, void *pbOut, long nSize );
long 	SYSAPI_WriteFile( SYS_HFILE hFile, void *pbIn, long nSize );
void	SYSAPI_FlushFileBuffer( SYS_HFILE hFile);
long 	SYSAPI_SeekFile( SYS_HFILE hFile, long lPos, int nSeekMode );
long	SYSAPI_GetFilePos( SYS_HFILE hFile );
atBOOL	SYSAPI_IsEOF( SYS_HFILE hFile );

atBOOL  SYSAPI_FindFirstFile( const atLPTSTR szPathName, const atLPTSTR szFilter, SYS_FINDFILEENTRY *lpFindFileEntry );
atBOOL  SYSAPI_FindNextFile( SYS_FINDFILEENTRY *lpFindFileEntry );
atVOID  SYSAPI_CloseFindFile( SYS_FINDFILEENTRY *lpFindFileEntry );


#ifdef __cplusplus
}
#endif

#endif

