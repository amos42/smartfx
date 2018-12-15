/**
  @file UI_File.c
  @brief File에 관한 API
  @remark File에 관한 API 모음
  @author Amos
  @date 2010-04-19 오전 11:41:00
  @see UI_File.h
*/
#include "UI_API/UI_API.h"
#include "OEM_UI_File.h"


static OEM_UI_FILE_T tOemFn;

atVOID	OEM_UI_File_SetOemFn( OEM_UI_FILE_T *fnOem )
{
	tOemFn = *fnOem;
}


atVOID	OEM_UI_File_GetOemFn( OEM_UI_FILE_T *fnOem )
{
	*fnOem = tOemFn;
}


/** 
 @brief 파일 시스템을 초기화 한다. (내부용)
 
 파일 시스템을 초기화 한다.

 @param lSysInst [in] : UI엔진의 인스턴스 (OEM 전달용. 내부용)
 @param lInitData1, lInitData2 [in] : 초기화 파라미터 (OEM 전달용. 내부용)
 @return 성공하면 atTRUE, 실패하면 atFALSE
 @remark 내부용으로 사용된다.
*/
atBOOL atUIAPI_InitFile( atLONG lSysInst, atLONG lInitData1, atLONG lInitData2 )
{
	if( tOemFn.fnInitFile != atNULL ){
		return tOemFn.fnInitFile( lSysInst, lInitData1, lInitData2 );
	}

	return atFALSE;
}


/** 
 @brief 파일 시스템을 종료한다. (내부용)
 
 파일 시스템을 종료한다.

 @remark 내부용으로 사용된다.
*/
void atUIAPI_FinalFile(void)
{
	if( tOemFn.fnFinalFile != atNULL ){
		tOemFn.fnFinalFile();
	}
}


/** 
 @brief 파일이 존재하는가 여부를 판단
 
 파일이 존재하는가 여부를 판단

 @param szFileName [in] : 파일 이름
 @return 파일이 존재하면 atTRUE, 존재하지 않으면 atFALSE
*/
atBOOL atUIAPI_IsExistFile( atLPTSTR szFileName )
{
	if( tOemFn.fnIsExistFile != atNULL ){
		return tOemFn.fnIsExistFile( szFileName );
	}

	return atFALSE;
}


/** 
 @brief 파일의 크기를 얻는다.
 
 파일의 크기를 얻는다.

 @param szFileName [in] : 파일 이름
 @return 파일의 크기
*/
long atUIAPI_GetFileSize( atLPTSTR szFileName )
{
	if( tOemFn.fnGetFileSize != atNULL ){
		return tOemFn.fnGetFileSize( szFileName );
	}

	return 0;
}


/** 
 @brief 파일을 로드한다.
 
 파일을 로드한다.

 @param szFileName [in] : 파일 이름
 @param lpBuf [out] : Load한 데이터를 저장할 포인터
 @param nSize [in] : 읽을 크기
 @param nStartPos [in] : 읽기 시작할 파일상의 오프셋
 @return 읽어낸 데이터의 크기
*/
long atUIAPI_LoadFile( atLPTSTR szFileName, void *lpBuf, long nSize, long nStartPos )
{
	if( tOemFn.fnLoadFile != atNULL ){
		return tOemFn.fnLoadFile( szFileName, lpBuf, nSize, nStartPos );
	}

	return 0;
}


/** 
 @brief 파일을 저장한다.
 
 파일을 저장한다. 존재하지 않으면 Create 한다.

 @param szFileName [in] : 파일 이름
 @param lpBuf [out] : Save할 데이터의 포인터
 @param nSize [in] : 저장할 크기
 @param nStartPos [in] : 쓰기 시작할 파일상의 오프셋
 @return 저장한 데이터의 크기
*/
long atUIAPI_SaveFile( atLPTSTR szFileName, void *lpBuf, long nSize, long nStartPos )
{
	if( tOemFn.fnSaveFile != atNULL ){
		return tOemFn.fnSaveFile( szFileName, lpBuf, nSize, nStartPos );
	}

	return 0;
}
