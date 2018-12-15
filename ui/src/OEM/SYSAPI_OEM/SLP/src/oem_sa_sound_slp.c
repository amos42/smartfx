//
// sound_wipic.c
//
////////////////////////////////////////////////////////////////

#include "OEM_SA_Sound.h"
#include "SYSAPI.h"
	

atBOOL OEM_SYSAPI_InitSound( atLONG lSysInst, atLONG lInitData1, atLONG lInitData2 )
{
	return atTRUE;
}

atVOID OEM_SYSAPI_FinalSound(void)
{
}

OEM_SYS_SOUND OEM_SYSAPI_CreateSound( int nChannels, int nSamplingRate, int nBitsPerSample, SYS_SOUND_PROC_FUNC *fnNofifyFunc )
{
	return atNULL;
}

void OEM_SYSAPI_DestroySound(OEM_SYS_SOUND hOemSound)
{
}

int OEM_SYSAPI_GetSoundVolume(OEM_SYS_SOUND hOemSound)
{
	return 0;
}

void OEM_SYSAPI_SetSoundVolume(OEM_SYS_SOUND hOemSound, int nVolume)
{
}

int OEM_SYSAPI_SetSoundOEMParam(OEM_SYS_SOUND hOemSound, long lParam1, long lParam2 )
{
	return atTRUE;
}

int OEM_SYSAPI_SetStreamBufferSize( OEM_SYS_SOUND hOemSound, long nBufferSize )
{
	return atTRUE;
}

int OEM_SYSAPI_PutSoundStream(OEM_SYS_SOUND hOemSound, void *lpStream, long nLen )
{
	return 0;
}

int OEM_SYSAPI_PlaySound(OEM_SYS_SOUND hOemSound)
{
	return 0;
}

int	OEM_SYSAPI_StopSound(OEM_SYS_SOUND hOemSound)
{
	return 0;
}

int	OEM_SYSAPI_PauseSound(OEM_SYS_SOUND hOemSound)
{
	return 0;
}

int	OEM_SYSAPI_ResumeSound(OEM_SYS_SOUND hOemSound)
{
	return 0;
}

void OEM_SYSAPI_SetVibrator(int on_off, int timeout)
{
}

