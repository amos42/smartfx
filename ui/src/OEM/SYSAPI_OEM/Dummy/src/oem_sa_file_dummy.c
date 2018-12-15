#include "SYSAPI.h"
#include "OEM_SA_File.h"
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <dirent.h>
#include <unistd.h>
#ifdef __USES_UNICODE_
#include <wchar.h>
#endif
#include <fnmatch.h>


atBOOL OEM_SYSAPI_InitFile( atLONG lSysInst, atLONG lInitData1, atLONG lInitData2 )
{
	return atTRUE;
}


void OEM_SYSAPI_FinalFile(void)
{
}


atBOOL	OEM_SYSAPI_ChangeDir( atLPTSTR szDirName )
{
	return atFALSE;
}

void	OEM_SYSAPI_GetCurDir( atLPTSTR szDirName )
{
}

atBOOL	OEM_SYSAPI_CreateDir( atLPTSTR szDirName  )
{
	return atFALSE;
}

atBOOL OEM_SYSAPI_IsExistFile( atLPTSTR szFileName )
{
	return atFALSE;
}

OEM_SYSAPI_FILE OEM_SYSAPI_OpenFile( atLPTSTR szFileName, int nOpenMode )
{
	return (OEM_SYSAPI_FILE)atNULL;
}

void OEM_SYSAPI_CloseFile( OEM_SYSAPI_FILE hFile )
{
}

long OEM_SYSAPI_ReadFile(OEM_SYSAPI_FILE hFile, void *pbOut, long nSize)
{
	return 0;
}

long OEM_SYSAPI_WriteFile(OEM_SYSAPI_FILE hFile, void *pbIn, long nSize)
{
	return 0;
}

void	OEM_SYSAPI_FlushFileBuffer( OEM_SYSAPI_FILE hFile)
{
}

long OEM_SYSAPI_SeekFile(OEM_SYSAPI_FILE hFile, long lPos, int nSeekMode)
{
	return 0;
}

long	OEM_SYSAPI_GetFilePos (OEM_SYSAPI_FILE hFile)
{
	return 0;
}

atBOOL	OEM_SYSAPI_IsEOF( OEM_SYSAPI_FILE hFile )
{
	return atTRUE;
}

long OEM_SYSAPI_GetFileLength( OEM_SYSAPI_FILE hFile )
{
	return 0;
}

atBOOL OEM_SYSAPI_DeleteFile( atLPTSTR szFileName )
{
	return atFALSE;
}

atBOOL OEM_SYSAPI_RenameFile( atLPTSTR szSrcName, atLPTSTR szDesName )
{
	return atFALSE;
}


atBOOL   OEM_SYSAPI_FindFirstFile( const atLPTSTR szPathName, const atLPTSTR szFilter, SYS_FINDFILEENTRY *lpFindFileEntry )
{
	return atFALSE;
}


atBOOL   OEM_SYSAPI_FindNextFile( SYS_FINDFILEENTRY *lpFindFileEntry )
{
	return atFALSE;
}


atVOID  OEM_SYSAPI_CloseFindFile( SYS_FINDFILEENTRY *lpFindFileEntry )
{
}

