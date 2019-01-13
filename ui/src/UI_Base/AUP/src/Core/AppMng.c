/** 
  @file AppMng.c
  @brief Application Manager에 관한 API
  @remark Application Manager에 관한 API
  @author Amos
  @date 2010-04-19 오전 11:41:00
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
 @brief 현재 활성화된 Application을 얻는다.
 @return 현재 활성화된 Application의 핸들
*/
atHAPP atAPPMNG_GetCurApp(void)
{
	return g_hCurApp;
}


/** 
 @brief Application을 활성화 시킨다.
 @param hApp [in] : 활성화된 Application의 핸들
 @return 이전에 활성화 되어 있었던 Application의 핸들
*/
atHAPP atAPPMNG_SetCurApp( atHAPP hApp )
{
	atHAPP hOldApp = g_hCurApp;

	g_hCurApp = hApp;
	
	return hOldApp;
}


/** 
 @brief Application에 종료 코드를 전송한다.
 @param nExitCode [in] : 종료 코드
 @return 없음
*/
atVOID atAPPMNG_Exit( atINT nExitCode )
{
	atAPP_SendExitCode( g_hCurApp, nExitCode );
}

