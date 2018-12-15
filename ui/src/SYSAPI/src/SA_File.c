#include "SYSAPI.h"
#include "OEM_SA_File.h"


#ifdef __USES_SYSAPI_FILE_


#ifndef COUNTOF
#define COUNTOF(a)       (  sizeof(a)/sizeof(a[0]) )
#endif


static atBOOL g_bSysFileIntitialized = atFALSE;


static OEM_SYSAPI_FILE_T tOemFn;


atVOID OEM_SYSAPI_FILE_SetOemFn(OEM_SYSAPI_FILE_T *fnOem)
{
	tOemFn = *fnOem;
}


atVOID OEM_SYSAPI_FILE_GetOemFn(OEM_SYSAPI_FILE_T *fnOem)
{
	*fnOem = tOemFn;
}


atBOOL SYSAPI_InitFile( atLONG lSysInst, atLONG lInitData1, atLONG lInitData2 )
{
	if( g_bSysFileIntitialized ) return atTRUE;
	
	if( tOemFn.fnInitFile != atNULL ){
		g_bSysFileIntitialized = tOemFn.fnInitFile( lSysInst, lInitData1, lInitData2 );
	}

	return g_bSysFileIntitialized;
}

void SYSAPI_FinalFile(void)
{
	if( !g_bSysFileIntitialized ) return;
	
	if( tOemFn.fnFinalFile != atNULL ){
		tOemFn.fnFinalFile();
	}
	
	g_bSysFileIntitialized = atFALSE;
}


atBOOL	SYSAPI_ChangeDir( atLPCTSTR szDirName )
{
	if( tOemFn.fnChangeDir != atNULL ){
		return tOemFn.fnChangeDir( szDirName );
	}

	return atFALSE;
}

void	SYSAPI_GetCurDir( atLPTSTR szDirName )
{
	if( tOemFn.fnGetCurDir != atNULL ){
		tOemFn.fnGetCurDir( szDirName );
	}
}

atBOOL	SYSAPI_CreateDir( atLPCTSTR szDirName  )
{
	if( tOemFn.fnCreateDir != atNULL ){
		return tOemFn.fnCreateDir( szDirName );
	}

	return atFALSE;
}

atBOOL	SYSAPI_DeleteDir( atLPCTSTR szDirName )
{
	if( tOemFn.fnDeleteDir != atNULL ){
		return tOemFn.fnDeleteDir( szDirName );
	}

	return atFALSE;
}

atBOOL	SYSAPI_RenameDir( atLPCTSTR szSrcDir, atLPCTSTR szDesDir )
{
	if( tOemFn.fnRenameDir != atNULL ){
		return tOemFn.fnRenameDir( szSrcDir, szDesDir );
	}

	return atFALSE;
}

atBOOL SYSAPI_IsExistFile( atLPCTSTR szFileName )
{
	if( tOemFn.fnIsExistFile != atNULL ){
		return tOemFn.fnIsExistFile( szFileName );
	}

	return atFALSE;
}

long SYSAPI_GetFileLength( SYS_HFILE hFile )
{
	if( hFile == atNULL ) return 0;

	if( tOemFn.fnGetFileLength != atNULL ){
		return tOemFn.fnGetFileLength( (OEM_SYSAPI_FILE) hFile );
	}

	return 0;
}

long SYSAPI_GetFileSize( atLPCTSTR szFileName )
{
	long len = 0;
	OEM_SYSAPI_FILE hFile = atNULL;

	if( tOemFn.fnOpenFile != atNULL ){
		hFile = tOemFn.fnOpenFile( szFileName, SYS_FILEOPEN_RDONLY );
	}
	if( hFile == atNULL ) return 0;

	if( tOemFn.fnGetFileLength != atNULL ){
		len = tOemFn.fnGetFileLength( hFile );
	}
	if( tOemFn.fnCloseFile != atNULL ){
		tOemFn.fnCloseFile( hFile );
	}

	return len;
}

atBOOL SYSAPI_DeleteFile( atLPCTSTR szFileName )
{
	if( tOemFn.fnDeleteFile != atNULL ){
		return tOemFn.fnDeleteFile( szFileName );
	}

	return atFALSE;
}

atBOOL SYSAPI_RenameFile( atLPCTSTR szSrcName, atLPCTSTR szDesName )
{
	if( tOemFn.fnRenameFile != atNULL ){
		return tOemFn.fnRenameFile( szSrcName, szDesName );
	}

	return atFALSE;
}


SYS_HFILE SYSAPI_OpenFile( atLPCTSTR szFileName, SYS_FILEOPEN nOpenMode )
{
	if( tOemFn.fnOpenFile != atNULL ){
		return (SYS_HFILE)tOemFn.fnOpenFile( szFileName, (int)nOpenMode );
	}

	return (SYS_HFILE)atNULL;
}

void SYSAPI_CloseFile( SYS_HFILE hFile )
{
	if( hFile == atNULL ) return;

	if( tOemFn.fnCloseFile != atNULL ){
		tOemFn.fnCloseFile( (OEM_SYSAPI_FILE)hFile );
	}
}

long SYSAPI_ReadFile(SYS_HFILE hFile, void *pbOut, long nSize)
{
	if( hFile == atNULL ) return 0;

	if( tOemFn.fnReadFile != atNULL ){
		return tOemFn.fnReadFile( (OEM_SYSAPI_FILE)hFile, pbOut, nSize );
	}

	return 0;
}

long SYSAPI_WriteFile(SYS_HFILE hFile, void *pbIn, long nSize)
{
	if( hFile == atNULL ) return 0;

	if( tOemFn.fnWriteFile != atNULL ){
		return tOemFn.fnWriteFile( (OEM_SYSAPI_FILE)hFile, pbIn, nSize );
	}

	return 0;
}

void	SYSAPI_FlushFileBuffer( SYS_HFILE hFile)
{
	if( hFile == atNULL ) return;

	if( tOemFn.fnFlushFileBuffer != atNULL ){
		tOemFn.fnFlushFileBuffer((OEM_SYSAPI_FILE)hFile);
	}
}

long SYSAPI_SeekFile(SYS_HFILE hFile, long lPos, int nSeekMode)
{
	if( hFile == atNULL ) return 0;

	if( tOemFn.fnSeekFile != atNULL ){
		return tOemFn.fnSeekFile((OEM_SYSAPI_FILE)hFile, lPos, nSeekMode);
	}

	return 0;
}

long	SYSAPI_GetFilePos (SYS_HFILE hFile)
{
	if( hFile == atNULL ) return 0;

	if( tOemFn.fnGetFilePos != atNULL ){
		return tOemFn.fnGetFilePos((OEM_SYSAPI_FILE)hFile);
	}

	return 0;
}

atBOOL	SYSAPI_IsEOF( SYS_HFILE hFile )
{
	if( hFile == atNULL ) return atTRUE;

	if( tOemFn.fnIsEOF != atNULL ){
		return tOemFn.fnIsEOF((OEM_SYSAPI_FILE)hFile);
	}

	return atFALSE;
}

atBOOL   SYSAPI_FindFirstFile( const atLPTSTR szFileName, const atLPTSTR szFilter, SYS_FINDFILEENTRY *lpFindFileEntry )
{
	SYSAPI_tcsncpy( lpFindFileEntry->szPathName, szFileName, COUNTOF(lpFindFileEntry->szPathName) );
	SYSAPI_tcsncpy( lpFindFileEntry->szFilter, szFilter, COUNTOF(lpFindFileEntry->szFilter) );
		
	if( tOemFn.fnFindFirstFile != atNULL ){
		return tOemFn.fnFindFirstFile( szFileName, szFilter, lpFindFileEntry );
	}

	return atFALSE;
}

atBOOL   SYSAPI_FindNextFile( SYS_FINDFILEENTRY *lpFindFileEntry )
{
	if( tOemFn.fnFindNextFile != atNULL ){
		return tOemFn.fnFindNextFile( lpFindFileEntry );
	}

	return atFALSE;
}

atVOID   SYSAPI_CloseFindFile( SYS_FINDFILEENTRY *lpFindFileEntry )
{
	if( tOemFn.fnCloseFindFile != atNULL ){
		tOemFn.fnCloseFindFile( lpFindFileEntry );
	}
}


#endif

