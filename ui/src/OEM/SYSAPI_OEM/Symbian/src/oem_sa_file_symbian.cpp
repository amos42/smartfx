#include "SYSAPI.h"
#include "OEM_SA_File.h"
//#include <stdio.h>
//#include <string.h>s
//#include <io.h>
#include "OSAL/_MPFile.h"


atBOOL OEM_SYSAPI_InitFile( atLONG lSysInst, atLONG lInitData1, atLONG lInitData2 )
{
	return atTRUE;
}


atVOID OEM_SYSAPI_FinalFile( NOARGS )
{
}

atBOOL	OEM_SYSAPI_ChangeDir( atLPTSTR szDirName )
{
	return atFALSE;
}

atVOID	OEM_SYSAPI_GetCurDir( atLPTSTR szDirName )
{
	SYSAPI_tcscpy( (atTCHAR*)szDirName, (const atLPTSTR)_AT("X:/") );
}

atBOOL	OEM_SYSAPI_CreateDir( atLPTSTR szDirName  )
{
#ifdef __USES_UNICODE_
	if ( OEM_SYSAPI_IsExistFile( szDirName ) == atFALSE)
#else
	if (access (szDirName, 0) == -1)
#endif
	{
		TBuf16<256> NameBuf = (const TUint16*)szDirName;
		RFs fSession;
		
		CleanupClosePushL(fSession);		
		
		if(fSession.Connect() == KErrNone)
		{
			if(fSession.MkDir(NameBuf) == KErrNone)
			{
				CleanupStack::Pop();
				fSession.Close();
				return atTRUE;
			}
		}
		
		CleanupStack::Pop();
		fSession.Close();
	}

	return atFALSE;
}

atBOOL OEM_SYSAPI_IsExistFile( atLPTSTR szFileName )
{
#if 0 
	struct _finddata_t stFileInfo;
	atLONG hFile;
	atINT r;
	atCHAR filename[256];	

	OEM_SYSAPI_UCS2ANSI(filename, szFileName);	

	if( (hFile = _findfirst( filename, &stFileInfo)) == -1 )
		r = atTRUE; 
	else 
		r = atFALSE;

	_findclose( hFile );

	return r;
#else
	FILE* pOpen = wfopen(szFileName, _AT("rb") );
	if(pOpen == NULL )
	    return atFALSE;
	    
	fclose(pOpen);
	return atTRUE;
		
#endif
}

OEM_SYSAPI_FILE OEM_SYSAPI_OpenFile( atLPTSTR szFileName, atINT nOpenMode )
{
#if 0
	atCHAR filename[256];
    FILE *fp;
	atCHAR *om;
	
	if( nOpenMode == SYS_FILEOPEN_RDONLY )
		om = "rb";
	else if( nOpenMode == SYS_FILEOPEN_WRONLY )
		om = "wb";
	else if( nOpenMode == SYS_FILEOPEN_WRTRUNC )
		om = "ab";
	else if( nOpenMode == SYS_FILEOPEN_RDWR )
		om = "r+b";
	else
		return atNULL;

	OEM_SYSAPI_UCS2ANSI(filename, szFileName);

    fp = fopen( filename, om );

    return (SYS_HFILE)fp;
    
#else
    CMPFile* file = new CMPFile;
    if (file == NULL)
    	return atNULL;
    
    atBOOL bRet = atFALSE;

    if( nOpenMode == SYS_FILEOPEN_CREATE )
    {
    	//file->CreateFile(szFileName, FILEMODEREAD | FILEMODEWRITE | FILEMODEBINARY );
    	file->CreateFile((TUint16 *)szFileName, FILEMODEWRITE | FILEMODEBINARY );
    } 
    else if( nOpenMode == SYS_FILEOPEN_APPEND )
    {
    	if (file->OpenFile((TUint16 *)szFileName, FILEMODEWRITE | FILEMODEBINARY))
    	{
    		file->Seek(EFileEnd, 0, EWriteStream);
    	}
    	else
    	{
       		//file->CreateFile(szFileName, FILEMODEREAD | FILEMODEWRITE | FILEMODEBINARY );	
    		file->CreateFile((TUint16 *)szFileName, FILEMODEWRITE | FILEMODEBINARY );
    	}
    }  
    else if( nOpenMode == SYS_FILEOPEN_RDONLY )
    {
		bRet = file->OpenFile((TUint16 *)szFileName, FILEMODEREAD | FILEMODEBINARY);
		if (bRet == atFALSE)
		{
			delete file;
			return atNULL;
		}
    }
	else if( nOpenMode == SYS_FILEOPEN_WRONLY )
	{
		bRet = file->OpenFile((TUint16 *)szFileName, FILEMODEWRITE | FILEMODEBINARY);
		if (bRet == atFALSE)
		{
			delete file;
			return atNULL;
		}
	}
	// SYS_FILEOPEN_RDWR 동작 안함
	/*else if( nOpenMode == SYS_FILEOPEN_RDWR )
	{
		bRet = file->OpenFile(szFileName, FILEMODEREAD | FILEMODEWRITE | FILEMODEBINARY);
		if (bRet == atFALSE)
		{
			delete file;
			return atNULL;
		}
	}*/
	else
		return atNULL;
    
    return (OEM_SYSAPI_FILE) file;    
    
#endif
}

atVOID OEM_SYSAPI_CloseFile( OEM_SYSAPI_FILE hFile )
{	
#if 0	
    fclose((FILE *)hFile);    
#else    
	if(hFile != NULL)
	{
		((CMPFile *)hFile)->CloseFile();
		
		delete (CMPFile*)hFile;
	}
    
#endif
}

atLONG OEM_SYSAPI_ReadFile(OEM_SYSAPI_FILE hFile, atVOID *pbOut, atLONG nSize)
{
#if 0
    return fread( pbOut, 1, nSize, (FILE *)hFile );
#else 
    int ret = ( (CMPFile*)hFile )->ReadFile(pbOut, nSize, nSize);
    if( ret < 0 ) return 0;
    return ret;
#endif
}

atLONG OEM_SYSAPI_WriteFile(OEM_SYSAPI_FILE hFile, atVOID *pbIn, atLONG nSize)
{
#if 0
    return fwrite( pbIn, 1, nSize, (FILE *)hFile );
#else
    int ret = ( (CMPFile*)hFile )->WriteFile(pbIn, nSize);
    if( ret < 0 ) return 0;
    return ret;
#endif   
}

void	OEM_SYSAPI_FlushFileBuffer( OEM_SYSAPI_FILE hFile)
{
}

atLONG OEM_SYSAPI_SeekFile(OEM_SYSAPI_FILE hFile, atLONG lPos, atINT nSeekMode)
{
#if 0
	atINT sm;

	if( nSeekMode == SYS_FILESEEK_SET )
		sm = SEEK_SET;
	else if( nSeekMode == SYS_FILESEEK_CUR )
		sm = SEEK_CUR;
	else if( nSeekMode == SYS_FILESEEK_END )
		sm = SEEK_END;
	else
		return 0;

    return fseek( (FILE *)hFile, lPos, sm );
#else
    TSeekPoint seekPoint;
    
    if( nSeekMode == SYS_FILESEEK_SET )
		seekPoint = EFileStart;
	else if( nSeekMode == SYS_FILESEEK_CUR )
		seekPoint = EFileCurrent;
	else if( nSeekMode == SYS_FILESEEK_END )
		seekPoint = EFileEnd;
	else
		return 0;

	( (CMPFile*)hFile )->Seek(seekPoint, lPos, EReadStream );
	( (CMPFile*)hFile )->Seek(seekPoint, lPos, EWriteStream );
   
	return ((CMPFile*)hFile)->GetPointer(EReadStream);
#endif
    
}

long	OEM_SYSAPI_GetFilePos (OEM_SYSAPI_FILE hFile)
{
#if 0
	return ftell( (FILE *)hFile);    
#else
	return ((CMPFile*)hFile)->GetPointer(EReadStream);
#endif
}

atBOOL	OEM_SYSAPI_IsEOF( OEM_SYSAPI_FILE hFile )
{
#if 0
	return feof( (FILE *)hFile);
#else
	return atFALSE;//((CMPFile*)hFile)->GetPointer(EReadStream);
#endif
}

long OEM_SYSAPI_GetFileLength( OEM_SYSAPI_FILE hFile )
{
#if 0
	return filelength(fileno((FILE*)hFile));
#else
	return ((CMPFile*)hFile)->GetSize();
#endif
}

atBOOL OEM_SYSAPI_DeleteFile( atLPTSTR szFileName )
{
#if 0
	atCHAR filename[256];	
	OEM_SYSAPI_UCS2ANSI(filename, szFileName);	
	return (remove(filename) == 0);
#else
	return CMPFile::fileDelete((TUint16 *)szFileName);	
#endif
}

atBOOL OEM_SYSAPI_RenameFile( atLPTSTR szSrcName, atLPTSTR szDesName )
{
#if 0
	char s_filename[256], d_filename[256];
	SYSAPI_ToANSI(s_filename, szSrcName, -1);	
	SYSAPI_ToANSI(d_filename, szDesName, -1);	
	return (rename(s_filename, d_filename) == 0);
#else
	//Zime 구현.. 심비안 Class RFs 참고...
	return CMPFile::fileRename((TUint16 *)szSrcName, (TUint16 *)szDesName);
#endif
}

atBOOL   OEM_SYSAPI_FindFirstFile( const atLPTSTR szPathName, const atLPTSTR szFilter, SYS_FINDFILEENTRY *lpFindFileEntry )
{
	atINT 	r;
	int len;	
	atTCHAR filename[256];	
	CMPFile *file;
	FIND_DATA findData;

	SYSAPI_memset(&findData, 0, sizeof(findData));
	SYSAPI_tcscpy( filename, szPathName );
	len = SYSAPI_tcslen(filename);
	if( len <= 0 ){
		SYSAPI_tcscpy( filename, _AT(".\\") );
		len = 2;
	}
	if( (filename[len-1] != _ATC('\\')) && (filename[len-1] != _ATC('/')) ){
		SYSAPI_tcscat( filename, _AT("\\") );
	}
	SYSAPI_tcscat( filename, szFilter );

    file = new CMPFile;	
	if( file->FindFirstFile((TUint16 *)filename, &findData) == atFALSE ){
		lpFindFileEntry->hFindHandle = (void *)atNULL;
		return atFALSE;
	}
	
	lpFindFileEntry->hFindHandle = (void *)file;
	lpFindFileEntry->dwAttr = 0;

	// attributes
	if(findData.att & FILE_ATT_HIDDEN)
		lpFindFileEntry->dwAttr |= SYS_FINDFILE_ATTR_HIDDEN;

	if(findData.att & FILE_ATT_SYSTEM)
		lpFindFileEntry->dwAttr |= SYS_FINDFILE_ATTR_SYSTEM;

	if(findData.att & FILE_ATT_DIRECTORY)
		lpFindFileEntry->dwAttr |= SYS_FINDFILE_ATTR_DIRECTORY;

	//if(findData.att & FILE_ATT_ARCHIVE)
	//	lpFindFileEntry->dwAttr |= SYS_FINDFILE_ATTR_ARCHIVE;

	if(findData.att & FILE_ATT_READONLY)
		lpFindFileEntry->dwAttr |= SYS_FINDFILE_ATTR_READONLY;

	SYSAPI_tcscpy( lpFindFileEntry->szFileName, (atLPTSTR)findData.name );
	lpFindFileEntry->nFileSize = findData.size;
		
	return atTRUE;
}


atBOOL   OEM_SYSAPI_FindNextFile( SYS_FINDFILEENTRY *lpFindFileEntry )
{
	FIND_DATA findData;
	CMPFile *file;

	file = (CMPFile *)lpFindFileEntry->hFindHandle;
	if( file == atNULL ) return atFALSE;

	if( file->FindNextFile( &findData ) == atFALSE ){
		return atFALSE;
	}	
	
	lpFindFileEntry->dwAttr = 0;
	// attributes
	if(findData.att & FILE_ATT_HIDDEN)
		lpFindFileEntry->dwAttr |= SYS_FINDFILE_ATTR_HIDDEN;

	if(findData.att & FILE_ATT_SYSTEM)
		lpFindFileEntry->dwAttr |= SYS_FINDFILE_ATTR_SYSTEM;

	if(findData.att & FILE_ATT_DIRECTORY)
		lpFindFileEntry->dwAttr |= SYS_FINDFILE_ATTR_DIRECTORY;

	//if(findData.att & FILE_ATT_ARCHIVE)
	//	lpFindFileEntry->dwAttr |= SYS_FINDFILE_ATTR_ARCHIVE;

	if(findData.att & FILE_ATT_READONLY)
		lpFindFileEntry->dwAttr |= SYS_FINDFILE_ATTR_READONLY;


	SYSAPI_tcscpy( lpFindFileEntry->szFileName, (atLPTSTR)findData.name );
	lpFindFileEntry->nFileSize = findData.size;

	return atTRUE;
}


atVOID  OEM_SYSAPI_CloseFindFile( SYS_FINDFILEENTRY *lpFindFileEntry )
{
	CMPFile *file;

	file = (CMPFile *)lpFindFileEntry->hFindHandle;
	if( file == atNULL ) return;
	
	file->FindFileColse();
}

