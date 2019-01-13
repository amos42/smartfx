/**
  @file UI_OIC.c
  @brief OIC (OEM Input Control) 제어 API
  @remark OIC (OEM Input Control) 제어 API 모음
  @author Amos
  @date 2010-04-19 오전 11:41:00
  @see UI_OIC.h
*/
#include "UI_API/UI_API.h"
#include "OEM_UI_OIC.h"



#ifdef __USES_UI_OIC_


static OEM_UI_OIC_T tOemFn;


atVOID	OEM_UI_OIC_SetOemFn( OEM_UI_OIC_T *fnOem )
{
	tOemFn = *fnOem;
}


atVOID	OEM_UI_OIC_GetOemFn( OEM_UI_OIC_T *fnOem )
{
	*fnOem = tOemFn;
}


/** 
 @brief OIC 초기화
 
 ......

 @param 
 @return 성공시엔 atTRUE, 실패시엔 atFALSE
*/
atBOOL atUIAPI_InitOIC( atLONG lSysInst, atLONG lInitData1, atLONG lInitData2 )
{
	tOemFn.fnInitOIC( lSysInst, lInitData1, lInitData2 );
	
	return atTRUE;
}


/** 
 @brief OIC 종료
 
 ......

 @param 
 @return 없음
*/
atVOID atUIAPI_FinalOIC( NOARGS )
{
	tOemFn.fnFinalOIC();
}


/** 
 @brief 
 
 ......

 @param 
 @return 없음
*/
atBOOL atUIAPI_IsSupportedOIC( NOARGS )
{
	return tOemFn.fnIsSupportedOIC();
}


/** 
 @brief 
 
 ......

 @param 
 @return 없음
*/
atUI_OIC atUIAPI_CreateOIC( int x, int y, int width, int height, atUI_FUNC_OIC_NOTIFY *fnFunc, void *param )
{
	return (atUI_OIC)tOemFn.fnCreateOIC( x, y, width, height, fnFunc, param );
}


/** 
 @brief 
 
 ......

 @param 
 @return 없음
*/
void atUIAPI_DestroyOIC( atUI_OIC hOIC )
{
	tOemFn.fnDestroyOIC( (OEM_UI_OIC)hOIC );
}


/** 
 @brief 
 
 ......

 @param 
 @return 없음
*/
void atUIAPI_SetOICFont( atUI_OIC hOIC, int font_size )
{
	tOemFn.fnSetOICFont( (OEM_UI_OIC)hOIC, font_size );
}


/** 
 @brief 
 
 ......

 @param 
 @return 없음
*/
void atUIAPI_GetOICPos( atUI_OIC hOIC, int *x, int *y )
{
	tOemFn.fnGetOICPos( (OEM_UI_OIC)hOIC, x, y );
}


/** 
 @brief 
 
 ......

 @param 
 @return 없음
*/
void atUIAPI_GetOICSize( atUI_OIC hOIC, int *width, int *height )
{
	tOemFn.fnGetOICSize( (OEM_UI_OIC)hOIC, width, height );
}


/** 
 @brief 
 
 ......

 @param 
 @return 없음
*/
void atUIAPI_SetOICSize( atUI_OIC hOIC, int width, int height )
{
	tOemFn.fnSetOICSize( (OEM_UI_OIC)hOIC, width, height );
}


/** 
 @brief 
 
 ......

 @param 
 @return 없음
*/
void atUIAPI_MoveOIC( atUI_OIC hOIC, int x, int y )
{
	tOemFn.fnMoveOIC( (OEM_UI_OIC)hOIC, x, y );
}


/** 
 @brief 
 
 ......

 @param 
 @return 없음
*/
void atUIAPI_ShowAndSetFocusOIC( atUI_OIC hOIC )
{
	tOemFn.fnShowAndSetFocusOIC( (OEM_UI_OIC)hOIC );
}


/** 
 @brief 
 
 ......

 @param 
 @return 없음
*/
void atUIAPI_HideAndLostFocusOIC( atUI_OIC hOIC )
{
	tOemFn.fnHideAndLostFocusOIC( (OEM_UI_OIC)hOIC );
}


/** 
 @brief 
 
 ......

 @param 
 @return 없음
*/
void atUIAPI_RefreshOIC( atUI_OIC hOIC )
{
	tOemFn.fnRefreshOIC( (OEM_UI_OIC)hOIC );
}


/** 
 @brief 
 
 ......

 @param 
 @return 없음
*/
void atUIAPI_SetOICText( atUI_OIC hOIC, atLPCTSTR szText, atINT nLen )
{
	tOemFn.fnSetOICText( (OEM_UI_OIC)hOIC, szText, nLen );
}


/** 
 @brief 
 
 ......

 @param 
 @return 없음
*/
void atUIAPI_GetOICText( atUI_OIC hOIC, atLPTSTR szText, atINT nLen )
{
	tOemFn.fnGetOICText( (OEM_UI_OIC)hOIC, szText, nLen );
}


#endif
