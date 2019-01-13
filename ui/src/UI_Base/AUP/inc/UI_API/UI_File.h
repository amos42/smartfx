/**
  @file UI_File.h
  @brief File에 관한 API
  @remark File에 관한 API 모음
  @author Amos
  @date 2010-04-19 오전 11:41:00
  @see UI_File.c
*/
#ifndef __UI_FILE_H_
#define __UI_FILE_H_


#ifndef __AT_TYPES_H_
#include "AT_Types.h"
#endif


#ifdef __cplusplus
extern "C" {
#endif


typedef atHANDLE    atUI_HFILE;

typedef enum {
	atUI_FILEOPEN_RDONLY = 0,
	atUI_FILEOPEN_WRONLY,
	atUI_FILEOPEN_RDWR,
	atUI_FILEOPEN_CREATE,
	atUI_FILEOPEN_APPEND
} atUI_FILEOPEN;

typedef enum {
	atUI_FILESEEK_SET = 0,
	atUI_FILESEEK_CUR,
	atUI_FILESEEK_END
} atUI_FILESEEK;


atBOOL	atUIAPI_InitFile( atLONG lSysInst, atLONG lInitData1, atLONG lInitData2 );
void	atUIAPI_FinalFile(void);

atBOOL 	atUIAPI_IsExistFile( atLPTSTR pFileName );
long 	atUIAPI_GetFileSize( atLPTSTR szFileName );

long 	atUIAPI_LoadFile( atLPTSTR szFileName, void *lpBuf, long nSize, long nStartPos );
long 	atUIAPI_SaveFile( atLPTSTR szFileName, void *lpBuf, long nSize, long nStartPos );


#ifdef __cplusplus
}
#endif

#endif

