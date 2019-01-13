/** 
  @file AppMng.c
  @brief Application Manager�� ���� API
  @remark Application Manager�� ���� API
  @author Amos
  @date 2010-04-19 ���� 11:41:00
  @see AppMng.h App.h
*/
#include "UI_Types.h"
#include "Window.h"
#include "Control.h"
#include "AppMng.h"
#include "UI_API/UI_API.h"
#include "UI_Util.h"
#include <string.h>



atHAPP g_hCurApp;


/** 
 @brief ���� Ȱ��ȭ�� Application�� ��´�.
 @return ���� Ȱ��ȭ�� Application�� �ڵ�
*/
atHAPP atAPPMNG_GetCurApp(void)
{
	return g_hCurApp;
}


/** 
 @brief Application�� Ȱ��ȭ ��Ų��.
 @param hApp [in] : Ȱ��ȭ�� Application�� �ڵ�
 @return ������ Ȱ��ȭ �Ǿ� �־��� Application�� �ڵ�
*/
atHAPP atAPPMNG_SetCurApp( atHAPP hApp )
{
	atHAPP hOldApp = g_hCurApp;

	g_hCurApp = hApp;
	
	return hOldApp;
}


/** 
 @brief Application�� ���� �ڵ带 �����Ѵ�.
 @param nExitCode [in] : ���� �ڵ�
 @return ����
*/
atVOID atAPPMNG_Exit( atINT nExitCode )
{
	atAPP_SendExitCode( g_hCurApp, nExitCode );
}

