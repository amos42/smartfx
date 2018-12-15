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
