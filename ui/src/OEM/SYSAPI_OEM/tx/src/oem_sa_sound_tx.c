//
// sound_wipic.c
//
////////////////////////////////////////////////////////////////

#include "OEM_SA_sound.h"

typedef struct _tagmtSOUND {
	atINT size;
	atCHAR *buf;
	atVOID *lpMdaClip;
} mtSOUND, * atLPSOUND;

static atCHAR g_DeviceName[32];

atBOOL OEM_SYSAPI_InitSound( atLONG lSysInst, atLONG lInitData1, atLONG lInitData2 )
{
	return atTRUE;
}


OEM_SYS_SOUND OEM_SYSAPI_CreateSound( int nChannels, int nSamplingRate, int nBitsPerSample, SYS_SOUND_PROC_FUNC *fnNofifyFunc )
{
	return 0;
}


atVOID OEM_SYSAPI_FinalSound( NOARGS )
{
}


atVOID OEM_SYSAPI_DestroySound(OEM_SYS_SOUND hOemSound)
{
}

atINT OEM_SYSAPI_GetSoundVolume(OEM_SYS_SOUND hOemSound)
{
	return 0;
}

atVOID OEM_SYSAPI_SetSoundVolume(OEM_SYS_SOUND hOemSound, atINT nVolume)
{
}

atVOID OEM_SYSAPI_SetVibrator(atINT on_off, atINT timeout)
{
}

int OEM_SYSAPI_SetSoundOEMParam(OEM_SYS_SOUND hOemSound, long lParam1, long lParam2 )
{
	return 0;
}

int OEM_SYSAPI_SetStreamBufferSize( OEM_SYS_SOUND hOemSound, long nBufferSize )
{
	return 0;
}

int OEM_SYSAPI_PutSoundStream(OEM_SYS_SOUND hOemSound, void *lpStream, long nLen )
{
	return 0;
}

atINT OEM_SYSAPI_PlaySound(OEM_SYS_SOUND hOemSound)
{
	return 0;
}

atINT	OEM_SYSAPI_StopSound(OEM_SYS_SOUND hOemSound)
{
	return 0;
}

atINT	OEM_SYSAPI_PauseSound(OEM_SYS_SOUND hOemSound)
{
	return 0;
}

atINT	OEM_SYSAPI_ResumeSound(OEM_SYS_SOUND hOemSound)
{
	return 0;
}

