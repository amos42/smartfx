/**
  @file UI_Sound.c
  @brief Sound에 관한 API
  @remark Sound에 관한 API 모음
  @author Amos
  @date 2010-04-19 오전 11:41:00
  @see UI_Sound.h
*/
#include "UI_API/UI_API.h"
#include "OEM_UI_Sound.h"


static OEM_UI_SOUND_T tOemFn;

atVOID	OEM_UI_Sound_SetOemFn( OEM_UI_SOUND_T *fnOem )
{
	tOemFn = *fnOem;
}


atVOID	OEM_UI_Sound_GetOemFn( OEM_UI_SOUND_T *fnOem )
{
	*fnOem = tOemFn;
}


/** 
 @brief 
 
 ......

 @param 
 @return 없음
*/
atBOOL	atUIAPI_InitSound( atLONG lSysInst, atLONG lInitData1, atLONG lInitData2 )
{
    return tOemFn.fnInitSound( lSysInst, lInitData1, lInitData2 );
}


/** 
 @brief 
 
 ......

 @param 
 @return 없음
*/
atVOID	atUIAPI_FinalSound( NOARGS )
{
    tOemFn.fnFinalSound();
}


/** 
 @brief 
 
 ......

 @param 
 @return 없음
*/
atHSOUND	atUIAPI_PlaySound( atLPVOID lpSoundStream, atUI_SOUNDTYPE nSoundType, atDWORD dwFlags )
{
    return (atHSOUND)tOemFn.fnPlaySound( lpSoundStream, nSoundType, dwFlags );
}


/** 
 @brief 
 
 ......

 @param 
 @return 없음
*/
atVOID	  atUIAPI_StopSound( atHSOUND hSound )
{
	if( hSound == atNULL ) return;
    tOemFn.fnStopSound( (OEM_UI_SOUND)hSound );
}
