#include "_file.h"

#ifdef __USES_SYSAPI_
#include "SYSAPI.h"
#else
#include <stdio.h>
#include <tchar.h>
#endif

    
FILE_T COMM_OpenFile( LPTSTR_T szFileName, DWORD_T dwOpt )
{  
#ifdef __USES_SYSAPI_
	int mode;
	  
	if( dwOpt == COMM_FILEOPEN_NONE ) return NULL;

	switch( dwOpt ){
	  	case COMM_FILEOPEN_READONLY : mode = SYS_FILEOPEN_RDONLY; break;
	  	case COMM_FILEOPEN_CREATE   : mode = SYS_FILEOPEN_CREATE; break;
	  	case COMM_FILEOPEN_APPEND   : mode = SYS_FILEOPEN_APPEND; break;
	}
	  
	return (FILE_T)SYSAPI_OpenFile( szFileName, mode );
#else
	char *mode;
	FILE *fp;
	errno_t r;
	  
	if( dwOpt == COMM_FILEOPEN_NONE ) return NULL;

	switch( dwOpt ){
	  	case COMM_FILEOPEN_READONLY : mode = "rb"; break;
	  	case COMM_FILEOPEN_CREATE   : mode = "wb"; break;
	  	case COMM_FILEOPEN_APPEND   : mode = "r+b"; break;
	}

	r = _tfopen_s( &fp, (const TCHAR *)szFileName, mode );
	if(r != 0){
		return (FILE_T)fp;
	} else {
		return (FILE_T)NULL;
	}
#endif
}


void COMM_CloseFile( FILE_T hFile )
{
#ifdef __USES_SYSAPI_
    SYSAPI_CloseFile( (SYS_HFILE)hFile );
#else
	fclose( (FILE *)hFile );
#endif
}


long COMM_GetFileSize( FILE_T hFile )
{  
#ifdef __USES_SYSAPI_
	long len;
	long cur_pos = SYSAPI_GetFilePos( (SYS_HFILE)hFile );
	SYSAPI_SeekFile( (SYS_HFILE)hFile, 0, SYS_FILESEEK_END );
	len = SYSAPI_GetFilePos( (SYS_HFILE)hFile );
	SYSAPI_SeekFile( (SYS_HFILE)hFile, cur_pos, SYS_FILESEEK_SET );	
	return len;
#else
	long len;
	long cur_pos = ftell( (FILE *)hFile );
	fseek( (FILE *)hFile, 0, SEEK_END );
	len = ftell( (FILE *)hFile );
	fseek( (FILE *)hFile, cur_pos, SEEK_SET );	
	return len;
#endif
}


long COMM_GetFilePos( FILE_T hFile )
{  
#ifdef __USES_SYSAPI_
	return SYSAPI_GetFilePos( (SYS_HFILE)hFile );
#else
	return ftell( (FILE *)hFile );
#endif
}


long COMM_ReadFile( FILE_T hFile, void *buf, long len )
{
#ifdef __USES_SYSAPI_
	  return SYSAPI_ReadFile( (SYS_HFILE)hFile, buf, len );
#else
	  return fread( buf, len, 1, (FILE *)hFile );
#endif
}


long COMM_WriteFile( FILE_T hFile, void *buf, long len )
{
#ifdef __USES_SYSAPI_
	  return SYSAPI_WriteFile( (SYS_HFILE)hFile, buf, len );
#else
	  return fwrite( buf, len, 1, (FILE *)hFile );
#endif
}
