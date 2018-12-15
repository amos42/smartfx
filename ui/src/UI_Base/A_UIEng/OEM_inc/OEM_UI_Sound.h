/**
  @file OEM_UI_Sound.h
  @brief Sound에 관한 포팅 OEM API 선언
  @remark Sound에 관한 포팅 OEM API 선언
  @author Amos
  @date 2010-04-19 오전 11:41:00
  @see UI_Sound.h
*/
#ifndef __OEM_UI_SOUND_H_
#define __OEM_UI_SOUND_H_

#include "UI_Types.h"

#ifdef __cplusplus
extern "C" {
#endif


typedef void * OEM_UI_SOUND;


typedef atBOOL			FN_OEM_UI_InitSound( atLONG lSysInst, atLONG lInitData1, atLONG lInitData2 );
typedef atVOID			FN_OEM_UI_FinalSound( NOARGS );

typedef OEM_UI_SOUND	FN_OEM_UI_PlaySound( atLPVOID lpSoundStream, atUI_SOUNDTYPE nSoundType, atDWORD dwFlags );
typedef atVOID	  		FN_OEM_UI_StopSound( OEM_UI_SOUND hOemSound );


typedef struct tagOEM_UI_SOUND {
	FN_OEM_UI_InitSound		*fnInitSound;
	FN_OEM_UI_FinalSound	*fnFinalSound;

	FN_OEM_UI_PlaySound		*fnPlaySound;
	FN_OEM_UI_StopSound		*fnStopSound;
} OEM_UI_SOUND_T;


atVOID	OEM_UI_Sound_SetOemFn( OEM_UI_SOUND_T *fnOem );
atVOID	OEM_UI_Sound_GetOemFn( OEM_UI_SOUND_T *fnOem );


#ifdef __cplusplus
}
#endif

#endif

