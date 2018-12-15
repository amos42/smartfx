#include "UI_API/UI_API.h"
#include "OEM_UI_File.h"
#include "SYSAPI.h"


atBOOL OEM_UI_InitFile( atLONG lSysInst, atLONG lInitData1, atLONG lInitData2 )
{
	return SYSAPI_InitFile( lSysInst, lInitData1, lInitData2 );
}

void OEM_UI_FinalFile(void)
{
	SYSAPI_FinalFile();
}


atBOOL OEM_UI_IsExistFile( atLPTSTR szFileName )
{
	return SYSAPI_IsExistFile( szFileName );
}

long OEM_UI_GetFileSize( atLPTSTR szFileName )
{
	return SYSAPI_GetFileSize( szFileName );
}


OEM_UI_FILE OEM_UI_OpenFile( atLPTSTR szFileName, int nOpenMode )
{
	SYS_FILEOPEN fm;
	
	if( nOpenMode == atUI_FILEOPEN_RDONLY )
		fm = SYS_FILEOPEN_RDONLY;
	else if( nOpenMode == atUI_FILEOPEN_WRONLY )
		fm = SYS_FILEOPEN_WRONLY;
	else if( nOpenMode == atUI_FILEOPEN_RDWR )
		fm = SYS_FILEOPEN_RDWR;
	else if( nOpenMode == atUI_FILEOPEN_CREATE )
		fm = SYS_FILEOPEN_CREATE;
	else if( nOpenMode == atUI_FILEOPEN_APPEND )
		fm = SYS_FILEOPEN_APPEND;
	else
		return atNULL;

    return (OEM_UI_FILE)SYSAPI_OpenFile( szFileName, fm );
}

void OEM_UI_CloseFile( OEM_UI_FILE hFile )
{
    SYSAPI_CloseFile( (SYS_HFILE)hFile );
}

long OEM_UI_ReadFile(OEM_UI_FILE hFile, void *pbOut, long nSize)
{
    return SYSAPI_ReadFile( (SYS_HFILE)hFile, pbOut, nSize );
}

long OEM_UI_WriteFile(OEM_UI_FILE hFile, void *pbIn, long nSize)
{
    return SYSAPI_WriteFile( (SYS_HFILE)hFile, pbIn, nSize );
}

long OEM_UI_SeekFile(OEM_UI_FILE hFile, long lPos, int nSeekMode)
{
	int sm;

	if( nSeekMode == atUI_FILESEEK_SET )
		sm = SYS_FILESEEK_SET;
	else if( nSeekMode == atUI_FILESEEK_CUR )
		sm = SYS_FILESEEK_CUR;
	else if( nSeekMode == atUI_FILESEEK_END )
		sm = SYS_FILESEEK_END;
	else return -1;

    return SYSAPI_SeekFile( hFile, lPos, sm );    
}

long OEM_UI_LoadFile( atLPTSTR szFileName, void *lpBuf, long nSize, long nStartPos )
{
	long ret;
	SYS_HFILE hFile = SYSAPI_OpenFile( szFileName, SYS_FILEOPEN_RDONLY );
	SYSAPI_SeekFile( hFile, nStartPos, SYS_FILESEEK_SET );
	ret = SYSAPI_ReadFile( hFile, lpBuf, nSize );
	SYSAPI_CloseFile( hFile );
	return ret;
}

long OEM_UI_SaveFile( atLPTSTR szFileName, void *lpBuf, long nSize, long nStartPos )
{
	return 0;
}
