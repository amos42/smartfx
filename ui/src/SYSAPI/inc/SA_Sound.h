//
// Sound.h
//
////////////////////////////////////////////////////////////////

#ifndef __SA_SOUND_H_
#define __SA_SOUND_H_


#include "SA_Types.h"


#ifdef __cplusplus
extern "C" {
#endif


typedef atHANDLE SYS_SOUND;

typedef enum {
	SYS_SOUNDEVENT_NONE = 0,
	SYS_SOUNDEVENT_OPEN,
	SYS_SOUNDEVENT_CLOSE,
	SYS_SOUNDEVENT_DONE
} SYS_SOUNDEVENT;

typedef int (SYS_SOUND_PROC_FUNC)( SYS_SOUND sound_inst, int event, atLONG lParam1, atLONG lParam2 );

atBOOL      SYSAPI_InitSound( atLONG lSysInst, atLONG lInitData1, atLONG lInitData2 );
atVOID      SYSAPI_FinalSound(void);

SYS_SOUND SYSAPI_CreateSound( int nChannels, int nSamplingRate, int nBitsPerSample, SYS_SOUND_PROC_FUNC *fnNofifyFunc );
void        SYSAPI_DestroySound(SYS_SOUND hSound);

int		SYSAPI_SetSoundOEMParam(SYS_SOUND hSound, long lParam1, long lParam2 );

int 		SYSAPI_GetSoundParam( SYS_SOUND hSound, int *lpnChannels, int *lpnSamplingRate, int *lpnBitsPerSample );
long 	SYSAPI_CalcStreamSizeFromTime(SYS_SOUND hSound, long nSec);

int		SYSAPI_SetStreamBufferSize( SYS_SOUND hSound, long nBufferSize );
int		SYSAPI_PutSoundStream(SYS_SOUND hSound, void *lpStream, long nLen );
int         SYSAPI_PlaySound(SYS_SOUND hSound);
int	        SYSAPI_StopSound(SYS_SOUND hSound);
int	        SYSAPI_PauseSound(SYS_SOUND hSound);
int	        SYSAPI_ResumeSound(SYS_SOUND hSound);

int         SYSAPI_GetSoundVolume(SYS_SOUND hSound);
void        SYSAPI_SetSoundVolume(SYS_SOUND hSound, int nVolume);

void        SYSAPI_SetVibrator(atBOOL on_off, long timeout);


#ifdef __cplusplus
}
#endif


#endif //  __SOUND_H_
