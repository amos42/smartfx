//
// sound_wipi.h
//
////////////////////////////////////////////////////////////////

#ifndef __OEM_SA_SOUND_H_
#define __OEM_SA_SOUND_H_


#include "SA_Types.h"
#include "SA_Sound.h"


#ifdef __cplusplus
extern "C" {
#endif


typedef void * OEM_SYS_SOUND;


typedef atBOOL			FN_OEM_SYSAPI_InitSound( atLONG lSysInst, atLONG lInitData1, atLONG lInitData2 );
typedef atVOID			FN_OEM_SYSAPI_FinalSound(void);

typedef OEM_SYS_SOUND	FN_OEM_SYSAPI_CreateSound( int nChannels, int nSamplingRate, int nBitsPerSample, SYS_SOUND_PROC_FUNC *lpCallback );
typedef void			FN_OEM_SYSAPI_DestroySound(OEM_SYS_SOUND hOemSound);

typedef int				FN_OEM_SYSAPI_SetSoundOEMParam(OEM_SYS_SOUND hOemSound, long lParam1, long lParam2 );

typedef int				FN_OEM_SYSAPI_SetStreamBufferSize( OEM_SYS_SOUND hOemSound, long nBufferSize );
typedef int				FN_OEM_SYSAPI_PutSoundStream(OEM_SYS_SOUND hOemSound, void *lpStream, long nLen );

typedef int				FN_OEM_SYSAPI_PlaySound(OEM_SYS_SOUND hOemSound);
typedef int				FN_OEM_SYSAPI_StopSound(OEM_SYS_SOUND hOemSound);
typedef int				FN_OEM_SYSAPI_PauseSound(OEM_SYS_SOUND hOemSound);
typedef int				FN_OEM_SYSAPI_ResumeSound(OEM_SYS_SOUND hOemSound);

typedef int 			FN_OEM_SYSAPI_GetSoundVolume(OEM_SYS_SOUND hOemSound);
typedef void			FN_OEM_SYSAPI_SetSoundVolume(OEM_SYS_SOUND hOemSound, atINT nVolume);

typedef void			FN_OEM_SYSAPI_SetVibrator(int on_off, int timeout);


typedef struct tagOEM_SYSAPI_SOUND_T {
	FN_OEM_SYSAPI_InitSound				*fnInitSound;
 	FN_OEM_SYSAPI_FinalSound			*fnFinalSound;

	FN_OEM_SYSAPI_CreateSound			*fnCreateSound;
	FN_OEM_SYSAPI_DestroySound			*fnDestroySound;

	FN_OEM_SYSAPI_SetSoundOEMParam		*fnSetSoundOEMParam;

	FN_OEM_SYSAPI_SetStreamBufferSize	*fnSetStreamBufferSize;
	FN_OEM_SYSAPI_PutSoundStream		*fnPutSoundStream;

	FN_OEM_SYSAPI_PlaySound				*fnPlaySound;
	FN_OEM_SYSAPI_StopSound				*fnStopSound;
	FN_OEM_SYSAPI_PauseSound			*fnPauseSound;
	FN_OEM_SYSAPI_ResumeSound			*fnResumeSound;

	FN_OEM_SYSAPI_GetSoundVolume		*fnGetSoundVolume;
	FN_OEM_SYSAPI_SetSoundVolume		*fnSetSoundVolume;

	FN_OEM_SYSAPI_SetVibrator			*fnSetVibrator;
} OEM_SYSAPI_SOUND_T;


atVOID	OEM_SYSAPI_SOUND_SetOemFn( OEM_SYSAPI_SOUND_T *fnOem );
atVOID	OEM_SYSAPI_SOUND_GetOemFn( OEM_SYSAPI_SOUND_T *fnOem );


#ifdef __cplusplus
}
#endif

#endif // _SOUND_WIPI_C_H_INCLUDED
