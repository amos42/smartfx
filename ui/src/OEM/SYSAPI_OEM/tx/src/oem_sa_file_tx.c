
#include "SYSAPI.h"
#include "fx_api.h"
#include "OEM_SA_File.h"
#include "OEM_SA_Str.h"

#include <HceDirect.h>

// for-test
#include <stdio.h>



#define HxFILE  ULONG

ULONG g_nOpenFileCount = 0;

atBOOL OEM_SYSAPI_InitFile( atLONG lSysInst, atLONG lInitData1, atLONG lInitData2 )
{
	return atTRUE;
}


atVOID OEM_SYSAPI_FinalFile( NOARGS )
{
}

atBOOL	OEM_SYSAPI_ChangeDir( atLPCTSTR szDirName )
{
	return atFALSE;
}

atVOID	OEM_SYSAPI_GetCurDir( atLPTSTR szDirName )
{
	OEM_SYSAPI_tcscpy( (atTCHAR*)szDirName, _AT("X:/") );
}

atBOOL	OEM_SYSAPI_CreateDir( atLPCTSTR szDirName  )
{
#if 1
	char dir_name[FX_MAXIMUM_PATH];

	SYSAPI_ToANSI((atANSI_CHAR*)dir_name, szDirName, -1);

	return (DirectFsDirectoryCreate(dir_name) == 1);

#else
	FX_MEDIA* pMedia = atNULL;
	char dir_name[FX_MAXIMUM_PATH];

	// to-do :  set media pointer

	SYSAPI_ToANSI((atANSI_CHAR*)dir_name, szDirName, -1);


	return (fx_directory_create(pMedia, dir_name) == FX_SUCCESS) ?  atTRUE : atFALSE;
#endif
}

atBOOL OEM_SYSAPI_IsExistFile( atLPCTSTR szFileName )
{
	return atFALSE;
}


OEM_SYSAPI_FILE OEM_SYSAPI_OpenFile( atLPCTSTR szFileName, atINT nOpenMode )
{
#if 1

	CHAR   file_name[FX_MAXIMUM_PATH];
	HxFILE hFile;
	UINT   open_type = 0;

	//SYSAPI_DebugPrint(szFileName);
	memset(file_name, 0, sizeof(file_name));

	SYSAPI_ToANSI((atANSI_CHAR*)file_name, szFileName , SYSAPI_tcslen(szFileName));

	fprintf(stderr, "OpenFile(%s)\n", file_name);

	//SYSAPI_DebugPrintf(_AT("OEM_SYSAPI_OpenFile(%s)\n"), szFileName);

/*
#define DIRECT_FS_OPEN_FOR_READ         0x00
#define DIRECT_FS_OPEN_FOR_WRITE        0x01
#define DIRECT_FS_OPEN_BY_NAME          0x02
#define DIRECT_FS_OPEN_FOR_WRITE_BURST  0x04
#define DIRECT_FS_OVERWRITE             0x08

*/
	open_type = DIRECT_FS_OPEN_BY_NAME;
	switch(nOpenMode)
	{
		case SYS_FILEOPEN_CREATE :
			open_type |= DIRECT_FS_OPEN_FOR_WRITE | DIRECT_FS_OVERWRITE;
			break;

		case SYS_FILEOPEN_APPEND :
			open_type |= DIRECT_FS_OPEN_FOR_WRITE;
			break;

		case SYS_FILEOPEN_RDONLY :
			open_type |= DIRECT_FS_OPEN_FOR_READ;
			break;

		case SYS_FILEOPEN_WRONLY :
			open_type |= DIRECT_FS_OPEN_FOR_WRITE | DIRECT_FS_OVERWRITE;
			break;

		default :
			return atNULL;
			break;
	}
	
	hFile = DirectFsFileOpen(file_name, open_type);
	if(!hFile)
	{
		SYSAPI_DebugPrintf(_AT("Error : DirectFsFileOpen(%s, %d)\n"), szFileName, nOpenMode);
		return atNULL;
	}
	/*
	g_nOpenFileCount++;

	SYSAPI_DebugPrintf(_AT("Open file count =%d\n"),g_nOpenFileCount);
	*/

	return 	(OEM_SYSAPI_FILE)hFile;

#else
	FX_MEDIA* pMedia = atNULL; // to-do
	FX_FILE*  pFile  = atNULL;
	CHAR file_name[FX_MAXIMUM_PATH];

	UINT open_type = 0;
	
	switch(nOpenMode)
	{
		case SYS_FILEOPEN_CREATE :
	    {
	    	open_type = FX_OPEN_FOR_WRITE;
			break;
	    } 
	    case SYS_FILEOPEN_APPEND :
	    {

			break;
	    }
		case SYS_FILEOPEN_RDONLY :
	    {
			open_type = FX_OPEN_FOR_READ;
			break;
	    }
		case SYS_FILEOPEN_WRONLY :
		{
			open_type = FX_OPEN_FOR_WRITE;
			break;
		}
		default :
			return atNULL;
	}

	SYSAPI_ToANSI((atANSI_CHAR*)file_name, szFileName , SYSAPI_tcslen(szFileName));

	UINT ret = fx_file_open(pMedia, pFile, file_name, open_type);


	// to-do : check result 

	return (OEM_SYSAPI_FILE)pFile;
#endif
}

atVOID OEM_SYSAPI_CloseFile( OEM_SYSAPI_FILE hFile )
{
#if 1
	HxFILE hxFile;

	hxFile = (HxFILE)hFile;
	if(!hxFile) return;

	DirectFsFileClose(hxFile);

/*
	g_nOpenFileCount--;

	SYSAPI_DebugPrintf(_AT("Open file count =%d\n"),g_nOpenFileCount);
*/	

#else
	FX_FILE* pFile = (FX_FILE*)hFile;
	
	if(pFile)
		fx_file_close(pFile);
#endif	
}


/*
       OEM_SYSAPI_ReadFile()
      
	RETURN :  
	       ok       :  read size (bytes)
	       fail      :  0 
*/
atLONG OEM_SYSAPI_ReadFile(OEM_SYSAPI_FILE hFile, atVOID *pbIn, atLONG nSize)
{
#if 1
	ULONG ret;
	HxFILE hxFile = (HxFILE)hFile;
	if(!hxFile || !pbIn) return 0;

	//SYSAPI_DebugPrintf(_AT("OEM_SYSAPI_ReadFile(%d)\n"), nSize);

	ret = DirectFsFileRead(hxFile, pbIn, nSize);
	if(	ret == 0)
	{
		SYSAPI_DebugPrintf(_AT("ERROR : DirectFsFileRead(%d)\n"),nSize);
		return 0;
	}
	else
		return ret;

#else
	FX_FILE* pFile = (FX_FILE*)hFile;
	
	atULONG  read_size = 0;

	if(!pFile) return 0;
	
	return ( fx_file_read(pFile, pbOut, nSize, &read_size) == FX_SUCCESS ) ?  read_size : 0;
#endif	
}


/*
	OEM_SYSAPI_WriteFile()
		
*/
atLONG OEM_SYSAPI_WriteFile(OEM_SYSAPI_FILE hFile, atVOID *pbIn, atLONG nSize)
{
#if 1
	HxFILE hxFile = (HxFILE)hFile;

	if(!hxFile) return 0;

	return DirectFsFileWrite(hxFile, pbIn, nSize);

#else
	FX_FILE* pFile = (FX_FILE*)hFile;

	if(!pFile) return 0;

	return ( fx_file_write(pFile, pbIn, nSize) == FX_SUCCESS) ?  nSize : 0;
#endif
}

void	OEM_SYSAPI_FlushFileBuffer( OEM_SYSAPI_FILE hFile)
{
	
}


/*
	OEM_SYSAPI_SeekFile

	RETURN :  1 : on sucesses,  0 : on failure
*/
atLONG OEM_SYSAPI_SeekFile(OEM_SYSAPI_FILE hFile, atLONG lPos, atINT nSeekMode)
{
#if 1
	UINT   seek_from = 0;
	HxFILE hxFile = (HxFILE)hFile;

	if(!hxFile) return 0;

/*
#define DIRECT_FS_SEEK_BEGIN            0x00
#define DIRECT_FS_SEEK_END              0x01
#define DIRECT_FS_SEEK_FORWARD          0x02
#define DIRECT_FS_SEEK_BACK             0x03

*/

	switch(nSeekMode)
	{
		case SYS_FILESEEK_SET:
			seek_from = DIRECT_FS_SEEK_BEGIN;
			break;
		case SYS_FILESEEK_CUR:
			seek_from = DIRECT_FS_SEEK_FORWARD;		// check-me
			break;
		case SYS_FILESEEK_END:
			seek_from = DIRECT_FS_SEEK_END;
			break;
		default :
			return 0;
	}

	return DirectFsFileRelSeek(hxFile, lPos, seek_from);

#else
	FX_FILE* pFile = (FX_FILE*)hFile;

	if(!pFile) return 0;

	UINT seek_from = 0;

	switch(nSeekMode)
	{
		case SYS_FILESEEK_SET:
			seek_from = FX_SEEK_BEGIN;
			break;
		case SYS_FILESEEK_CUR:
			seek_from = FX_SEEK_FORWARD;
			break;
		case SYS_FILESEEK_END:
			seek_from = FX_SEEK_END;
			break;
	}

	return fx_file_relative_seek(pFile, lPos, seek_from);
#endif
}

long	OEM_SYSAPI_GetFilePos (OEM_SYSAPI_FILE hFile)
{
#if 1
	ULONG loc;
	HxFILE hxFile = (HxFILE)hFile;
	if(!hxFile) return 0;

	loc = 0;

	//SYSAPI_DebugPrint(_AT("OEM_SYSAPI_GetFilePos \n"));

	if(DirectFsFileTell(hxFile, &loc) == 0)
	{
		SYSAPI_DebugPrint(_AT("ERROR : GetFilePos\n"));
	}

	//fprintf(stderr, "OEM_SYSAPI_GetFilePos (%u) = %u\n", hxFile, loc);

	return loc;

#else
	FX_FILE* pFile = (FX_FILE*)hFile;
	ULONG location = 0;

	if(!pFile) return 0;

	fx_file_tell(pFile, &location);

	return location;
#endif
}

long OEM_SYSAPI_GetFileLength( OEM_SYSAPI_FILE hFile )
{
	ULONG  location;

	DirectFsFileRelSeek((HxFILE)hxFile, 0, DIRECT_FS_SEEK_END);

	location = 0;
	DirectFsFileTell((HxFILE)hxFile, &location);

	return location;
}

atBOOL OEM_SYSAPI_DeleteFile( atLPCTSTR szFileName )
{
#if 1
	char file_name[FX_MAXIMUM_PATH];

	SYSAPI_ToANSI((atANSI_CHAR*)file_name, szFileName, SYSAPI_tcslen(szFileName));

	return (DirectFsDelete(file_name) == 1);

#else
	FX_MEDIA* pMedia = atNULL;

	char file_name[FX_MAXIMUM_PATH];
	// to-do :  set media pointer

	SYSAPI_ToANSI((atANSI_CHAR*)file_name, szFileName, SYSAPI_tcslen(szFileName));

	return (fx_file_delete(pMedia, file_name) == FX_SUCCESS) ? atTRUE : atFALSE;
#endif
}


/*
     @ szSrcName [in]: old file name
     @ szDesName [in] : new file name
*/
atBOOL OEM_SYSAPI_RenameFile( atLPCTSTR szSrcName, atLPCTSTR szDesName )
{
#if 1
	char old_filename[FX_MAXIMUM_PATH];
	char new_filename[FX_MAXIMUM_PATH];

	SYSAPI_ToANSI((atANSI_CHAR*)old_filename, szSrcName, -1);
	SYSAPI_ToANSI((atANSI_CHAR*)new_filename, szDesName, -1);

	return (DirectFsRename(old_filename, new_filename) == 1);

#else
	FX_MEDIA* pMedia = atNULL;
	char old_filename[FX_MAXIMUM_PATH];
	char new_filename[FX_MAXIMUM_PATH];

	// to-do : set media pointer

	SYSAPI_ToANSI((atANSI_CHAR*)old_filename, szSrcName, -1);
	SYSAPI_ToANSI((atANSI_CHAR*)new_filename, szDesName, -1);

	return (fx_file_rename(pMedia, old_filename, new_filename) == FX_SUCCESS) ?  atTRUE : atFALSE;
#endif
}

atBOOL   OEM_SYSAPI_FindFirstFile( const atLPTSTR szPathName, const atLPTSTR szFilter, SYS_FINDFILEENTRY *lpFindFileEntry )
{

#if 1
	// to-do 
	return atFALSE;	
#else

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
	if( file->FindFirstFile(filename, &findData) == atFALSE ){
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

	SYSAPI_tcscpy( lpFindFileEntry->szFileName, findData.name );
	lpFindFileEntry->nFileSize = findData.size;
		
	return atTRUE;
#endif

}


atBOOL   OEM_SYSAPI_FindNextFile( SYS_FINDFILEENTRY *lpFindFileEntry )
{
#if 0
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


	SYSAPI_tcscpy( lpFindFileEntry->szFileName, findData.name );
	lpFindFileEntry->nFileSize = findData.size;

	return atTRUE;
#endif

	return atFALSE;
}


atVOID  OEM_SYSAPI_CloseFindFile( SYS_FINDFILEENTRY *lpFindFileEntry )
{
#if 0
	CMPFile *file;

	file = (CMPFile *)lpFindFileEntry->hFindHandle;
	if( file == atNULL ) return;
	
	file->FindFileColse();
#endif 
}


atBOOL	OEM_SYSAPI_IsEOF( OEM_SYSAPI_FILE hFile )
{
	return atFALSE;
}


