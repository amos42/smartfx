/**
  @file UI_File.c
  @brief File�� ���� API
  @remark File�� ���� API ����
  @author Amos
  @date 2010-04-19 ���� 11:41:00
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
 @brief ���� �ý����� �ʱ�ȭ �Ѵ�. (���ο�)
 
 ���� �ý����� �ʱ�ȭ �Ѵ�.

 @param lSysInst [in] : UI������ �ν��Ͻ� (OEM ���޿�. ���ο�)
 @param lInitData1, lInitData2 [in] : �ʱ�ȭ �Ķ���� (OEM ���޿�. ���ο�)
 @return �����ϸ� atTRUE, �����ϸ� atFALSE
 @remark ���ο����� ���ȴ�.
*/
atBOOL atUIAPI_InitFile( atLONG lSysInst, atLONG lInitData1, atLONG lInitData2 )
{
	if( tOemFn.fnInitFile != atNULL ){
		return tOemFn.fnInitFile( lSysInst, lInitData1, lInitData2 );
	}

	return atFALSE;
}


/** 
 @brief ���� �ý����� �����Ѵ�. (���ο�)
 
 ���� �ý����� �����Ѵ�.

 @remark ���ο����� ���ȴ�.
*/
void atUIAPI_FinalFile(void)
{
	if( tOemFn.fnFinalFile != atNULL ){
		tOemFn.fnFinalFile();
	}
}


/** 
 @brief ������ �����ϴ°� ���θ� �Ǵ�
 
 ������ �����ϴ°� ���θ� �Ǵ�

 @param szFileName [in] : ���� �̸�
 @return ������ �����ϸ� atTRUE, �������� ������ atFALSE
*/
atBOOL atUIAPI_IsExistFile( atLPTSTR szFileName )
{
	if( tOemFn.fnIsExistFile != atNULL ){
		return tOemFn.fnIsExistFile( szFileName );
	}

	return atFALSE;
}


/** 
 @brief ������ ũ�⸦ ��´�.
 
 ������ ũ�⸦ ��´�.

 @param szFileName [in] : ���� �̸�
 @return ������ ũ��
*/
long atUIAPI_GetFileSize( atLPTSTR szFileName )
{
	if( tOemFn.fnGetFileSize != atNULL ){
		return tOemFn.fnGetFileSize( szFileName );
	}

	return 0;
}


/** 
 @brief ������ �ε��Ѵ�.
 
 ������ �ε��Ѵ�.

 @param szFileName [in] : ���� �̸�
 @param lpBuf [out] : Load�� �����͸� ������ ������
 @param nSize [in] : ���� ũ��
 @param nStartPos [in] : �б� ������ ���ϻ��� ������
 @return �о �������� ũ��
*/
long atUIAPI_LoadFile( atLPTSTR szFileName, void *lpBuf, long nSize, long nStartPos )
{
	if( tOemFn.fnLoadFile != atNULL ){
		return tOemFn.fnLoadFile( szFileName, lpBuf, nSize, nStartPos );
	}

	return 0;
}


/** 
 @brief ������ �����Ѵ�.
 
 ������ �����Ѵ�. �������� ������ Create �Ѵ�.

 @param szFileName [in] : ���� �̸�
 @param lpBuf [out] : Save�� �������� ������
 @param nSize [in] : ������ ũ��
 @param nStartPos [in] : ���� ������ ���ϻ��� ������
 @return ������ �������� ũ��
*/
long atUIAPI_SaveFile( atLPTSTR szFileName, void *lpBuf, long nSize, long nStartPos )
{
	if( tOemFn.fnSaveFile != atNULL ){
		return tOemFn.fnSaveFile( szFileName, lpBuf, nSize, nStartPos );
	}

	return 0;
}
