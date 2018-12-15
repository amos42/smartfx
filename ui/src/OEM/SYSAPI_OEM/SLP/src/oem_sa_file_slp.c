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
	SYSAPI_tcscpy( szDirName, _AT("X:/") );
}

atBOOL	OEM_SYSAPI_CreateDir( atLPTSTR szDirName  )
{
	char filename[256];

	SYSAPI_ToANSI(filename, szDirName, -1);

	if (access (filename, 0) == -1){
		int iRet = mkdir( filename, S_IREAD | S_IWRITE );
		if (iRet == -1)	return atFALSE;
		return atTRUE;
	}

	return atFALSE;
}

atBOOL OEM_SYSAPI_IsExistFile( atLPTSTR szFileName )
{
	FILE *fp;
	int r;
	char filename[256];

	SYSAPI_ToANSI(filename, szFileName, -1);

  fp = fopen( filename, "rb" );
	if( fp != NULL )
		r = atTRUE;
	else
		r = atFALSE;

	if(fp) fclose( fp );

	return r;
}

OEM_SYSAPI_FILE OEM_SYSAPI_OpenFile( atLPTSTR szFileName, int nOpenMode )
{
	char filename[256];
	FILE *fp;
	char *om;

	if( nOpenMode == SYS_FILEOPEN_RDONLY )
		om = "rb";
	else if( nOpenMode == SYS_FILEOPEN_WRONLY )
		om = "b";
	else if( nOpenMode == SYS_FILEOPEN_RDWR )
		om = "r+b";
	else if( nOpenMode == SYS_FILEOPEN_CREATE )
		om = "w+b";
	else if( nOpenMode == SYS_FILEOPEN_APPEND )
		om = "a+b";
	else
		return atNULL;

	SYSAPI_ToANSI(filename, szFileName, -1);

    	fp = fopen( filename, om );
	if( fp == NULL ) return (OEM_SYSAPI_FILE)atNULL;

	if( nOpenMode == SYS_FILEOPEN_APPEND )
		fseek( fp, 0, SEEK_END );

	return (OEM_SYSAPI_FILE)fp;
}

void OEM_SYSAPI_CloseFile( OEM_SYSAPI_FILE hFile )
{
    fclose((FILE *)hFile);
}

long OEM_SYSAPI_ReadFile(OEM_SYSAPI_FILE hFile, void *pbOut, long nSize)
{
    return fread( pbOut, 1, nSize, (FILE *)hFile );
}

long OEM_SYSAPI_WriteFile(OEM_SYSAPI_FILE hFile, void *pbIn, long nSize)
{
    return fwrite( pbIn, 1, nSize, (FILE *)hFile );
}

void	OEM_SYSAPI_FlushFileBuffer( OEM_SYSAPI_FILE hFile)
{
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

long	OEM_SYSAPI_GetFilePos (OEM_SYSAPI_FILE hFile)
{
	return ftell( (FILE *)hFile);
}

atBOOL	OEM_SYSAPI_IsEOF( OEM_SYSAPI_FILE hFile )
{
	return feof( (FILE *)hFile);
}

long OEM_SYSAPI_GetFileLength( OEM_SYSAPI_FILE hFile )
{
	long size;
	long pos = ftell((FILE *)hFile);
	fseek( (FILE *)hFile, 0, SEEK_END );
	size = ftell((FILE *)hFile);
	fseek( (FILE *)hFile, pos, SEEK_SET );
	return size;
}

atBOOL OEM_SYSAPI_DeleteFile( atLPTSTR szFileName )
{
	char filename[256];
	SYSAPI_ToANSI(filename, szFileName, -1);
	return (remove(filename) == 0);
}

atBOOL OEM_SYSAPI_RenameFile( atLPTSTR szSrcName, atLPTSTR szDesName )
{
	char s_filename[256], d_filename[256];
	SYSAPI_ToANSI(s_filename, szSrcName, -1);
	SYSAPI_ToANSI(d_filename, szDesName, -1);
	return (rename(s_filename, d_filename) == 0);
}


atBOOL   OEM_SYSAPI_FindFirstFile( const atLPTSTR szPathName, const atLPTSTR szFilter, SYS_FINDFILEENTRY *lpFindFileEntry )
{
	int len;
	DIR *dp;
	struct dirent *ep;
  struct stat stt;
	char filename[256], temp[256];

	SYSAPI_ToANSI( filename, szPathName, -1 );
	len = strlen(filename);
	if( len <= 0 ){
		strcpy( filename, "./" );
		len = 2;
	}
	if( filename[len-1] != '/' ){
		strcat( filename, "/" );
	}

	SYSAPI_ToANSI( temp, szFilter, -1 );
//	strcat( filename, temp );

	dp = opendir ( filename );
  if( dp == atNULL ){
	  return atFALSE;
  }

  lpFindFileEntry->hFindHandle = (void *)dp;

  ep = readdir (dp);

  while(ep){
    if( !fnmatch ( temp, ep->d_name, 0 ) ){
      lstat( ep->d_name, &stt );

	    lpFindFileEntry->dwAttr = 0;
    	if( S_ISDIR(stt.st_mode) )
		    lpFindFileEntry->dwAttr |= SYS_FINDFILE_ATTR_DIRECTORY;

	    SYSAPI_FromANSI( lpFindFileEntry->szFileName, ep->d_name, -1 );
	    lpFindFileEntry->nFileSize = stt.st_size;

	    return atTRUE;
    }

    ep = readdir (dp);
  }

	return atFALSE;
}


atBOOL   OEM_SYSAPI_FindNextFile( SYS_FINDFILEENTRY *lpFindFileEntry )
{
	char temp[256];
	struct dirent *ep;
  struct stat stt;
	DIR *dp = (DIR *)lpFindFileEntry->hFindHandle;
	if( dp == NULL ) return atFALSE;

	SYSAPI_ToANSI( temp, lpFindFileEntry->szFilter, -1 );

  ep = readdir (dp);

  while(ep){
    if( !fnmatch ( temp, ep->d_name, 0 ) ){
      lstat( ep->d_name, &stt );

	    lpFindFileEntry->dwAttr = 0;
    	if( S_ISDIR(stt.st_mode) )
		    lpFindFileEntry->dwAttr |= SYS_FINDFILE_ATTR_DIRECTORY;

	    SYSAPI_FromANSI( lpFindFileEntry->szFileName, ep->d_name, -1 );
	    lpFindFileEntry->nFileSize = stt.st_size;

	    return atTRUE;
    }

    ep = readdir (dp);
  }

	return atFALSE;
}


atVOID  OEM_SYSAPI_CloseFindFile( SYS_FINDFILEENTRY *lpFindFileEntry )
{
	DIR *dp = (DIR *)lpFindFileEntry->hFindHandle;
	if( dp == NULL ) return;

	closedir( dp );
}

