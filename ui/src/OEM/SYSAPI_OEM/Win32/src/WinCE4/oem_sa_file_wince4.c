#include "SYSAPI.h"
#include "OEM_SA_File.h"
#include <windows.h>
#include <string.h>

// CreateFile 사용할 경우 1로 fopen을 사용할 경우 0으로 설정한다.
#define		_USE_FILE_WINAPI_		1	 		


#define ADD_SLASH(str, idx) { if (str[idx] != L'\\' || str[idx] != L'/') wcscat(str, L"\\"); }


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
	if (GetFileAttributes(szDirName) == -1L)
	{
		atBOOL bRet = (atBOOL) CreateDirectory (szDirName, NULL);
		return bRet;
	}

	return atTRUE;
}

atBOOL	OEM_SYSAPI_DeleteDir( atLPCTSTR szDirName )
{
	return atFALSE;
}

atBOOL	OEM_SYSAPI_RenameDir( atLPCTSTR szSrcDir, atLPCTSTR szDesDir)
{
	return atFALSE;
}

atBOOL OEM_SYSAPI_IsExistFile( atLPTSTR szFileName )
{
	if (GetFileAttributes(szFileName) == -1L)
	{
		return atFALSE;
	}
	return atTRUE;
}

OEM_SYSAPI_FILE OEM_SYSAPI_OpenFile( atLPTSTR szFileName, int nOpenMode )
{
#if (_USE_FILE_WINAPI_)

	// default open mode is read only.
	DWORD dwAccesMode = GENERIC_READ; 
	DWORD dwShareMode = FILE_SHARE_READ; 
	DWORD dwCreateMode = OPEN_EXISTING; 
	HANDLE hFile = NULL;

	if ( nOpenMode == SYS_FILEOPEN_WRONLY )
	{
		dwAccesMode = GENERIC_WRITE;
		dwShareMode = FILE_SHARE_WRITE;
		dwCreateMode = OPEN_EXISTING;
	}
	else if ( nOpenMode == SYS_FILEOPEN_RDONLY )
	{
		dwAccesMode = GENERIC_READ;
		dwShareMode = FILE_SHARE_READ;
		dwCreateMode = OPEN_EXISTING;
	}
	else if ( nOpenMode == SYS_FILEOPEN_RDWR )
	{
		dwAccesMode = GENERIC_READ | GENERIC_WRITE;
		dwShareMode = FILE_SHARE_READ | FILE_SHARE_WRITE;
		dwCreateMode = OPEN_EXISTING;
	}
	else if ( nOpenMode == SYS_FILEOPEN_CREATE )
	{
		dwAccesMode = GENERIC_READ | GENERIC_WRITE;
		dwShareMode = FILE_SHARE_READ | FILE_SHARE_WRITE;
		dwCreateMode = CREATE_ALWAYS;
	}
	else if ( nOpenMode == SYS_FILEOPEN_APPEND )
	{
		dwAccesMode = GENERIC_READ | GENERIC_WRITE;
		dwShareMode = FILE_SHARE_READ | FILE_SHARE_WRITE;
		dwCreateMode = OPEN_ALWAYS;
	}
	
	hFile = CreateFile (szFileName, dwAccesMode, dwShareMode, NULL, dwCreateMode, FILE_ATTRIBUTE_NORMAL, NULL);
	if( hFile == INVALID_HANDLE_VALUE ) return (OEM_SYSAPI_FILE)atNULL;

	if ( nOpenMode == SYS_FILEOPEN_APPEND )
		SetFilePointer( hFile, 0, NULL, FILE_END );

	return (OEM_SYSAPI_FILE)hFile;

#else

	atUCS_CHAR filename[256];
	FILE *fp;
	atTCHAR *om;
	
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

	SYSAPI_ToUCS(filename, szFileName, -1);

	fp = _wfopen ( filename, om );
	if( fp == NULL ) return (OEM_SYSAPI_FILE)atNULL;

	if( nOpenMode == SYS_FILEOPEN_APPEND )
		fseek( fp, 0, SEEK_END );

	return (OEM_SYSAPI_FILE)fp;
	
#endif
}

void OEM_SYSAPI_CloseFile( OEM_SYSAPI_FILE hFile )
{
#if (_USE_FILE_WINAPI_)
	if( hFile && ((HANDLE)hFile != INVALID_HANDLE_VALUE) )
		CloseHandle(hFile);
#else
	fclose((FILE *)hFile);
#endif
}

long OEM_SYSAPI_ReadFile(OEM_SYSAPI_FILE hFile, void *pbOut, long nSize)
{
#if (_USE_FILE_WINAPI_)
	DWORD iReadCnt = 0;
	ReadFile ((HANDLE)hFile, pbOut, nSize, &iReadCnt, NULL);
	return iReadCnt;
#else
	return fread( pbOut, 1, nSize, (FILE *)hFile );    
#endif
}

long OEM_SYSAPI_WriteFile(OEM_SYSAPI_FILE hFile, void *pbIn, long nSize)
{
#if (_USE_FILE_WINAPI_)
	DWORD iWriteCnt = 0;
	WriteFile ((HANDLE)hFile, pbIn, nSize, &iWriteCnt, NULL);
	return iWriteCnt;
#else
    return fwrite( pbIn, 1, nSize, (FILE *)hFile );    
#endif
}

void	OEM_SYSAPI_FlushFileBuffer( OEM_SYSAPI_FILE hFile)
{
#if (_USE_FILE_WINAPI_)
	FlushFileBuffers( (HANDLE)hFile );
#else
	fflush( (FILE *)hFile );
#endif
}

long OEM_SYSAPI_SeekFile(OEM_SYSAPI_FILE hFile, long lPos, int nSeekMode)
{
#if (_USE_FILE_WINAPI_)
	DWORD dwMoveMethod = FILE_BEGIN;

	if     ( nSeekMode == SYS_FILESEEK_SET ) dwMoveMethod = FILE_BEGIN;
	else if( nSeekMode == SYS_FILESEEK_CUR ) dwMoveMethod = FILE_CURRENT;
	else if( nSeekMode == SYS_FILESEEK_END ) dwMoveMethod = FILE_END;

	return SetFilePointer((HANDLE)hFile, lPos, NULL, dwMoveMethod);

#else

	int sm;

	if     ( nSeekMode == SYS_FILESEEK_SET )	sm = SEEK_SET;
	else if( nSeekMode == SYS_FILESEEK_CUR )	sm = SEEK_CUR;
	else if( nSeekMode == SYS_FILESEEK_END )	sm = SEEK_END;

    return fseek( (FILE *)hFile, lPos, sm );

#endif
}

long	OEM_SYSAPI_GetFilePos (OEM_SYSAPI_FILE hFile)
{
#if (_USE_FILE_WINAPI_)
	return SetFilePointer(hFile, 0, NULL, FILE_CURRENT);
#else
	return ftell( (FILE *)hFile);    
#endif
}

atBOOL	OEM_SYSAPI_IsEOF( OEM_SYSAPI_FILE hFile )
{
#if (_USE_FILE_WINAPI_)
//	DWORD r = SetFilePointer(hFile, 0, NULL, FILE_CURRENT);
//	if( (r == 0xFFFFFFFF) && (GetLastError() == ERROR_HANDLE_EOF) ) return atTRUE;
	return atFALSE;
	//return EOF( hFile );
#else
	return feof( (FILE *)hFile);
#endif
}

long OEM_SYSAPI_GetFileLength( OEM_SYSAPI_FILE hFile )
{
#if (_USE_FILE_WINAPI_)
	long lSizeLow;
	DWORD dwHighPart = 0;
	lSizeLow = GetFileSize ((HANDLE)hFile, &dwHighPart);
	return lSizeLow;
#else
	return filelength( fileno((FILE *)hFile) );
#endif
}

atBOOL OEM_SYSAPI_DeleteFile( atLPTSTR szFileName )
{
	atTCHAR filename[256];	
	SYSAPI_ToUCS(filename, szFileName, -1);	
	return (DeleteFile(filename) == TRUE);
}

atBOOL OEM_SYSAPI_RenameFile( atLPTSTR szSrcName, atLPTSTR szDesName )
{
	char s_filename[256], d_filename[256];
	SYSAPI_ToANSI(s_filename, szSrcName, -1);	
	SYSAPI_ToANSI(d_filename, szDesName, -1);	

	return (atBOOL)DeleteAndRenameFile((LPCWSTR)s_filename, (LPCWSTR)d_filename);
}



atBOOL   OEM_SYSAPI_FindFirstFile( const atLPTSTR lpszPath, const atLPTSTR lpszName, SYS_FINDFILEENTRY *lpFindFileEntry )
{
	DWORD dwCount = 0;
	WCHAR szPath[_MAX_PATH] = L".\\";
	WCHAR szName[_MAX_PATH] = L"*.*";
	WIN32_FIND_DATA		findata;
	HANDLE				hFind;
	WCHAR				lpszFullPath[MAX_PATH];
	WCHAR				lpszSubPath[MAX_PATH];

	if(lpszPath && lpszPath[0])
	{
		SYSAPI_tcscpy (szPath, lpszPath);
		//wcscpyn(szPath, _MAX_PATH, lpszPath);
		ADD_SLASH(szPath, wcslen(lpszPath)-1);
	}
	
	if(lpszName)
	{
		SYSAPI_tcscpy (szName, lpszName);
		//wcscpyn(szName, _MAX_PATH, lpszName);
	}

	swprintf(lpszFullPath, L"%s%s", szPath, szName);

	hFind = FindFirstFile(lpszFullPath, &findata);

	if(hFind == INVALID_HANDLE_VALUE) 
		return atFALSE;

	lpFindFileEntry->hFindHandle = (void *)hFind;
	lpFindFileEntry->dwAttr  = 0;
	if(findata.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		lpFindFileEntry->dwAttr |= SYS_FINDFILE_ATTR_DIRECTORY;
	if(findata.dwFileAttributes & FILE_ATTRIBUTE_READONLY)
		lpFindFileEntry->dwAttr |= SYS_FINDFILE_ATTR_READONLY;
	if(findata.dwFileAttributes & FILE_ATTRIBUTE_SYSTEM)
		lpFindFileEntry->dwAttr |= SYS_FINDFILE_ATTR_SYSTEM;
	// to-do

	//SYSAPI_FromANSI( lpFindFileEntry->szFileName, findata.cFileName, -1 );
	SYSAPI_tcscpy (lpFindFileEntry->szFileName, findata.cFileName);
	lpFindFileEntry->nFileSize   = findata.nFileSizeLow;

	return atTRUE;

/////////////////////////////////////////////////////////////
/*
	//struct _finddata_t stFileInfo;
	WIN32_FIND_DATA stFileInfo;
	long hFile;
	int len;
	char filename[256], temp[256];	

	SYSAPI_ToANSI( filename, szPathName, -1 );
	len = strlen(filename);
	if( len <= 0 ){
		strcpy( filename, ".\\" );
		len = 2;
	}
	if( (filename[len-1] != '\\') && (filename[len-1] != '/') ){
		strcat( filename, "\\" );
	}
	SYSAPI_ToANSI( temp, szFilter, -1 );
	strcat( filename, temp );

	if( (hFile = _findfirst( filename, &stFileInfo)) == -1 ){
		lpFindFileEntry->hFindHandle = (void *)-1;
		return atFALSE;
	}
	
	lpFindFileEntry->hFindHandle = (void *)hFile;
	lpFindFileEntry->dwAttr = stFileInfo.dwFileAttributes;
	SYSAPI_FromANSI( lpFindFileEntry->szFileName, stFileInfo.cFileName, -1 );
	lpFindFileEntry->nFileSize = stFileInfo.nFileSizeLow;
		
	return atTRUE;
*/
}


atBOOL   OEM_SYSAPI_FindNextFile( SYS_FINDFILEENTRY *lpFindFileEntry )
{
	WIN32_FIND_DATA		findata;
	atBOOL bRet = FindNextFile(lpFindFileEntry->hFindHandle, &findata);
	if (bRet == atTRUE)
	{
		lpFindFileEntry->dwAttr  = 0;
		if(findata.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			lpFindFileEntry->dwAttr |= SYS_FINDFILE_ATTR_DIRECTORY;
		if(findata.dwFileAttributes & FILE_ATTRIBUTE_READONLY)
			lpFindFileEntry->dwAttr |= SYS_FINDFILE_ATTR_READONLY;
		if(findata.dwFileAttributes & FILE_ATTRIBUTE_SYSTEM)
			lpFindFileEntry->dwAttr |= SYS_FINDFILE_ATTR_SYSTEM;

		//SYSAPI_FromANSI( lpFindFileEntry->szFileName, findata.cFileName, -1 );
		SYSAPI_tcscpy (lpFindFileEntry->szFileName, findata.cFileName);
		lpFindFileEntry->nFileSize   = findata.nFileSizeLow;
	}

	return bRet;


	/*
	WIN32_FIND_DATA stFileInfo;
	long hFile;

	hFile = (long)lpFindFileEntry->hFindHandle;
	if( hFile == -1 ) return atFALSE;
	
	if( _findnext( hFile, &stFileInfo ) == -1 ){
		return atFALSE;
	}	
	
	lpFindFileEntry->dwAttr = stFileInfo.dwFileAttributes;
	SYSAPI_FromANSI( lpFindFileEntry->szFileName, stFileInfo.cFileName, -1 );
	lpFindFileEntry->nFileSize = stFileInfo.nFileSizeLow;

	return atTRUE;
	*/
}


atVOID  OEM_SYSAPI_CloseFindFile( SYS_FINDFILEENTRY *lpFindFileEntry )
{
	long hFile;

	hFile = (long)lpFindFileEntry->hFindHandle;
	if( (hFile == NULL) || ((HANDLE)hFile == INVALID_HANDLE_VALUE) ) return;
	
	FindClose( lpFindFileEntry->hFindHandle );
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
