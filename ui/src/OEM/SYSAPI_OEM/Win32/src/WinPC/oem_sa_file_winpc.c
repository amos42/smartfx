#include "SYSAPI.h"
#include "OEM_SA_File.h"
#include <stdio.h>
#include <string.h>
#include <io.h>
#include <time.h>
#ifdef __USES_UNICODE_
#include <wchar.h>
#endif


static atBOOL OEM_SYSAPI_InitFile( atLONG lSysInst, atLONG lInitData1, atLONG lInitData2 )
{
	return atTRUE;
}


static void OEM_SYSAPI_FinalFile(void)
{
}


static atBOOL	OEM_SYSAPI_ChangeDir( atLPCTSTR szDirName )
{
#ifdef __USES_UNICODE_
		return (_wchdir(szDirName) == 0);
#else
		return (_chdir(szDirName) == 0);
#endif
	return atFALSE;
}

static void	OEM_SYSAPI_GetCurDir( atLPTSTR szDirName )
{
	SYSAPI_tcscpy( szDirName, _AT("X:/") );
}

static atBOOL	OEM_SYSAPI_CreateDir( atLPCTSTR szDirName  )
{
#ifdef __USES_UNICODE_
	if (_waccess (szDirName, 0) == -1){
		atINT iRet = _wmkdir (szDirName);
		if (iRet == -1)	return atFALSE;
		return atTRUE;
	}
#else
	if (access (szDirName, 0) == -1){
		atINT iRet = mkdir (szDirName);
		if (iRet == -1)	return atFALSE;
		return atTRUE;
	}
#endif
	return atFALSE;
}

static atBOOL	OEM_SYSAPI_DeleteDir( atLPCTSTR szDirName )
{
#ifdef __USES_UNICODE_
	if (_waccess (szDirName, 0) == 0){
		atINT iRet = _wrmdir (szDirName);
		if (iRet == -1)	return atFALSE;
		return atTRUE;
	}
#else
	if (access (szDirName, 0) == 0){
		atINT iRet = rmdir (szDirName);
		if (iRet == -1)	return atFALSE;
		return atTRUE;
	}
#endif
	return atFALSE;
}

static atBOOL	OEM_SYSAPI_RenameDir( atLPCTSTR szSrcDir, atLPCTSTR szDesDir )
{
	char s_filename[256], d_filename[256];
	SYSAPI_ToANSI(s_filename, (const atLPTSTR)szSrcDir, -1);	
	SYSAPI_ToANSI(d_filename, (const atLPTSTR)szDesDir, -1);	
	return (rename(s_filename, d_filename) == 0);
}

static atBOOL OEM_SYSAPI_IsExistFile( atLPCTSTR szFileName )
{
	struct _finddata_t stFileInfo;
	long hFile;
	int r;
	char filename[256];	

	SYSAPI_ToANSI(filename, (const atLPTSTR)szFileName, -1);	

	if( (hFile = _findfirst( filename, &stFileInfo)) != -1 )
		r = atTRUE; 
	else 
		r = atFALSE;

	_findclose( hFile );

	return r;
}

static OEM_SYSAPI_FILE OEM_SYSAPI_OpenFile( atLPCTSTR szFileName, int nOpenMode )
{
	//char filename[256];
	FILE *fp;
	//char *om;
	atTCHAR *om;
	
	if( nOpenMode == SYS_FILEOPEN_RDONLY )
		om = _AT("rb");
	else if( nOpenMode == SYS_FILEOPEN_WRONLY )
		om = _AT("b");
	else if( nOpenMode == SYS_FILEOPEN_RDWR )
		om = _AT("r+b");
	else if( nOpenMode == SYS_FILEOPEN_CREATE )
		om = _AT("w+b");
	else if( nOpenMode == SYS_FILEOPEN_APPEND )
		om = _AT("a+b");
	else
		return atNULL;

	//SYSAPI_ToANSI(filename, (const atLPTSTR)szFileName, -1);

   	//fp = fopen( filename, om );
#if defined(__USES_UNICODE_)
	fp = _wfopen( szFileName, om );
#else
	fp = fopen( szFileName, om );
#endif

	if( fp == NULL ) return (OEM_SYSAPI_FILE)atNULL;

	if( nOpenMode == SYS_FILEOPEN_APPEND )
		fseek( fp, 0, SEEK_END );

	return (OEM_SYSAPI_FILE)fp;
}

static void OEM_SYSAPI_CloseFile( OEM_SYSAPI_FILE hFile )
{
    fclose((FILE *)hFile);
}

static long OEM_SYSAPI_ReadFile(OEM_SYSAPI_FILE hFile, void *pbOut, long nSize)
{
    return fread( pbOut, 1, nSize, (FILE *)hFile );    
}

static long OEM_SYSAPI_WriteFile(OEM_SYSAPI_FILE hFile, void *pbIn, long nSize)
{
    return fwrite( pbIn, 1, nSize, (FILE *)hFile );    
}

static void	OEM_SYSAPI_FlushFileBuffer( OEM_SYSAPI_FILE hFile)
{
	fflush( (FILE *)hFile );
}

long OEM_SYSAPI_SeekFile(OEM_SYSAPI_FILE hFile, long lPos, int nSeekMode)
{
	int sm;

	if( nSeekMode == SYS_FILESEEK_SET )
		sm = SEEK_SET;
	else if( nSeekMode == SYS_FILESEEK_CUR )
		sm = SEEK_CUR;
	else if( nSeekMode == SYS_FILESEEK_END )
		sm = SEEK_END;

    return fseek( (FILE *)hFile, lPos, sm );    
}

static long	OEM_SYSAPI_GetFilePos ( OEM_SYSAPI_FILE hFile )
{
	return ftell( (FILE *)hFile);    
}

static atBOOL	OEM_SYSAPI_IsEOF( OEM_SYSAPI_FILE hFile )
{
	return feof( (FILE *)hFile);
}

static long OEM_SYSAPI_GetFileLength( OEM_SYSAPI_FILE hFile )
{
	return filelength( fileno((FILE *)hFile) );
}

static atBOOL OEM_SYSAPI_DeleteFile( atLPCTSTR szFileName )
{
	char filename[256];	
	SYSAPI_ToANSI(filename, (const atLPTSTR)szFileName, -1);	
	return (remove(filename) == 0);
}

static atBOOL OEM_SYSAPI_RenameFile( atLPCTSTR szSrcName, atLPCTSTR szDesName )
{
	char s_filename[256], d_filename[256];
	SYSAPI_ToANSI(s_filename, (const atLPTSTR)szSrcName, -1);	
	SYSAPI_ToANSI(d_filename, (const atLPTSTR)szDesName, -1);	
	return (rename(s_filename, d_filename) == 0);
}

static atBOOL   OEM_SYSAPI_FindFirstFile( atLPCTSTR szPathName, atLPCTSTR szFilter, SYS_FINDFILEENTRY *lpFindFileEntry )
{
	struct _finddata_t stFileInfo;
	long hFile;
	int len;
	char filename[256], temp[256];
	struct tm *newtime;

	SYSAPI_ToANSI( filename, (const atLPTSTR)szPathName, -1 );
	len = strlen(filename);
	if( len <= 0 ){
		strcpy( filename, ".\\" );
		len = 2;
	}
	if( (filename[len-1] != '\\') && (filename[len-1] != '/') ){
		strcat( filename, "\\" );
	}
	SYSAPI_ToANSI( temp, (const atLPTSTR)szFilter, -1 );
	strcat( filename, temp );

	if( (hFile = _findfirst( filename, &stFileInfo)) == -1 ){
		lpFindFileEntry->hFindHandle = atNULL;
		return atFALSE;
	}
	
	lpFindFileEntry->hFindHandle = (void *)hFile;
	lpFindFileEntry->dwAttr = 0;
	if( stFileInfo.attrib & _A_SUBDIR)
		lpFindFileEntry->dwAttr |= SYS_FINDFILE_ATTR_DIRECTORY;
	if( stFileInfo.attrib & _A_RDONLY)
		lpFindFileEntry->dwAttr |= SYS_FINDFILE_ATTR_READONLY;
	if( stFileInfo.attrib & _A_SYSTEM)
		lpFindFileEntry->dwAttr |= SYS_FINDFILE_ATTR_SYSTEM;
	//to-do

	SYSAPI_FromANSI( lpFindFileEntry->szFileName, stFileInfo.name, -1 );
	lpFindFileEntry->nFileSize = stFileInfo.size;

	newtime = localtime(&(stFileInfo.time_write));

	lpFindFileEntry->dDateTime.nYear	= newtime->tm_year + 1900;
	lpFindFileEntry->dDateTime.nMonth	= newtime->tm_mon + 1;
	lpFindFileEntry->dDateTime.nDay		= newtime->tm_mday;
	lpFindFileEntry->dDateTime.nHour	= newtime->tm_hour;
	lpFindFileEntry->dDateTime.nMinute	= newtime->tm_min;
	lpFindFileEntry->dDateTime.nSec		= newtime->tm_sec;
		
	return atTRUE;
}


static atBOOL   OEM_SYSAPI_FindNextFile( SYS_FINDFILEENTRY *lpFindFileEntry )
{
	struct _finddata_t stFileInfo;
	long hFile;
	struct tm *newtime;

	hFile = (long)lpFindFileEntry->hFindHandle;
	if( hFile == 0 ) return atFALSE;
	
	if( _findnext( hFile, &stFileInfo ) == -1 ){
		return atFALSE;
	}	
	
	lpFindFileEntry->dwAttr = 0;
	if( stFileInfo.attrib & _A_SUBDIR)
		lpFindFileEntry->dwAttr |= SYS_FINDFILE_ATTR_DIRECTORY;
	if( stFileInfo.attrib & _A_RDONLY)
		lpFindFileEntry->dwAttr |= SYS_FINDFILE_ATTR_READONLY;
	if( stFileInfo.attrib & _A_SYSTEM)
		lpFindFileEntry->dwAttr |= SYS_FINDFILE_ATTR_SYSTEM;
	//to-do

	SYSAPI_FromANSI( lpFindFileEntry->szFileName, stFileInfo.name, -1 );
	lpFindFileEntry->nFileSize = stFileInfo.size;

	newtime = localtime(&(stFileInfo.time_write));
	
	lpFindFileEntry->dDateTime.nYear	= newtime->tm_year + 1900;
	lpFindFileEntry->dDateTime.nMonth	= newtime->tm_mon + 1;
	lpFindFileEntry->dDateTime.nDay		= newtime->tm_mday;
	lpFindFileEntry->dDateTime.nHour	= newtime->tm_hour;
	lpFindFileEntry->dDateTime.nMinute	= newtime->tm_min;
	lpFindFileEntry->dDateTime.nSec		= newtime->tm_sec;

	return atTRUE;
}


static atVOID  OEM_SYSAPI_CloseFindFile( SYS_FINDFILEENTRY *lpFindFileEntry )
{
	long hFile;

	hFile = (long)lpFindFileEntry->hFindHandle;
	if( hFile == 0 ) return;
	
	_findclose( hFile );
}


void init_sysapi_file_oem() 
{
	OEM_SYSAPI_FILE_T oem_funcs = {
		OEM_SYSAPI_InitFile,
		OEM_SYSAPI_FinalFile,

		OEM_SYSAPI_ChangeDir,
		OEM_SYSAPI_GetCurDir,
		OEM_SYSAPI_CreateDir,
		OEM_SYSAPI_DeleteDir,
		OEM_SYSAPI_RenameDir,

		OEM_SYSAPI_OpenFile,
		OEM_SYSAPI_CloseFile,
		OEM_SYSAPI_ReadFile,
		OEM_SYSAPI_WriteFile,
		OEM_SYSAPI_FlushFileBuffer,
		OEM_SYSAPI_SeekFile,
		OEM_SYSAPI_GetFilePos,
		OEM_SYSAPI_IsEOF,
		OEM_SYSAPI_GetFileLength,

		OEM_SYSAPI_IsExistFile,
		OEM_SYSAPI_DeleteFile,
		OEM_SYSAPI_RenameFile,

		OEM_SYSAPI_FindFirstFile,
		OEM_SYSAPI_FindNextFile,
		OEM_SYSAPI_CloseFindFile
	};

	OEM_SYSAPI_FILE_SetOemFn( &oem_funcs );
}
