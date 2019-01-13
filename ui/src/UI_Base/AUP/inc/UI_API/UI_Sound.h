/**
  @file UI_Sound.h
  @brief Sound에 관한 API
  @remark Sound에 관한 API 모음
  @author Amos
  @date 2010-04-19 오전 11:41:00
  @see UI_Sound.c
*/
#ifndef __UI_SOUND_H_
#define __UI_SOUND_H_

#include "UI_Types.h"


#ifdef __cplusplus
extern "C" {
#endif


typedef atHANDLE  atHSOUND;


typedef enum {
	atUI_SOUNDTYPE_NONE,
	atUI_SOUNDTYPE_WAV,
	atUI_SOUNDTYPE_MP3
} atUI_SOUNDTYPE;	

#define atUI_SOUNDFLAGS_MASK		0xFFFF0000
#define atUI_SOUNDFLAGS_NORMAL		0x00000000
#define atUI_SOUNDFLAGS_REPEAT		0x10000000


atBOOL		atUIAPI_InitSound( atLONG lSysInst, atLONG lInitData1, atLONG lInitData2 );
atVOID		atUIAPI_FinalSound( NOARGS );

atHSOUND	atUIAPI_PlaySound( atLPVOID lpSoundStream, atUI_SOUNDTYPE nSoundType, atDWORD dwFlags );
atVOID	  atUIAPI_StopSound( atHSOUND hSound );


#ifdef __cplusplus
}
#endif

#endif

