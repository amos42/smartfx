/**
  @file OEM_UI_File.h
  @brief File에 관한 포팅 OEM API 선언
  @remark File에 관한 포팅 OEM API 선언
  @author Amos
  @date 2010-04-19 오전 11:41:00
  @see UI_File.h
*/
#ifndef __OEM_UI_FILE_H_
#define __OEM_UI_FILE_H_

#include "UI_Types.h"

#ifdef __cplusplus
extern "C" {
#endif


typedef void *OEM_UI_FILE;


typedef atBOOL	FN_OEM_UI_InitFile( atLONG lSysInst, atLONG lInitData1, atLONG lInitData2 );
typedef void	FN_OEM_UI_FinalFile(void);

typedef atBOOL 	FN_OEM_UI_IsExistFile( atLPTSTR pFileName );
typedef long 	FN_OEM_UI_GetFileSize( atLPTSTR szFileName );
typedef long	FN_OEM_UI_LoadFile( atLPTSTR szFileName, void *lpBuf, long nSize, long nStartPos );
typedef long	FN_OEM_UI_SaveFile( atLPTSTR szFileName, void *lpBuf, long nSize, long nStartPos );


typedef struct tagOEM_UI_FILE {
	FN_OEM_UI_InitFile		*fnInitFile;
	FN_OEM_UI_FinalFile		*fnFinalFile;

	FN_OEM_UI_IsExistFile	*fnIsExistFile;
	FN_OEM_UI_GetFileSize	*fnGetFileSize;
	FN_OEM_UI_LoadFile		*fnLoadFile;
	FN_OEM_UI_SaveFile		*fnSaveFile;
} OEM_UI_FILE_T;


atVOID	OEM_UI_File_SetOemFn( OEM_UI_FILE_T *fnOem );
atVOID	OEM_UI_File_GetOemFn( OEM_UI_FILE_T *fnOem );


#ifdef __cplusplus
}
#endif

#endif

