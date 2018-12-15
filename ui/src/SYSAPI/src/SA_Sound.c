#include "AT_Types.h"
#include "SYSAPI.h"
#include "OEM_SA_Sound.h"


#ifdef __USES_SYSAPI_SOUND_


static atBOOL g_bSysSoundIntitialized = atFALSE;

typedef struct tagTSysSound {
	OEM_SYS_SOUND hOemSound;
	int nChannels, nSamplingRate, nBitsPerSample;
//	void (*callback)(SYS_Sound,void*);
	void *param;
} TSysSound;


static OEM_SYSAPI_SOUND_T tOemFn;


atVOID OEM_SYSAPI_SOUND_SetOemFn(OEM_SYSAPI_SOUND_T *fnOem)
{
	tOemFn = *fnOem;
}


atVOID OEM_SYSAPI_SOUND_GetOemFn(OEM_SYSAPI_SOUND_T *fnOem)
{
	*fnOem = tOemFn;
}


atBOOL SYSAPI_InitSound( atLONG lSysInst, atLONG lInitData1, atLONG lInitData2 )
{
	if( g_bSysSoundIntitialized ) return atTRUE;
	
	if( tOemFn.fnInitSound != atNULL ){
		g_bSysSoundIntitialized = tOemFn.fnInitSound( lSysInst, lInitData1, lInitData2 );
	}
	
	return g_bSysSoundIntitialized;
}

atVOID SYSAPI_FinalSound(void)
{
	if( !g_bSysSoundIntitialized ) return;
	
	if( tOemFn.fnFinalSound != atNULL ){
		tOemFn.fnFinalSound();
	}
	
	g_bSysSoundIntitialized = atFALSE;
}

SYS_SOUND SYSAPI_CreateSound( int nChannels, int nSamplingRate, int nBitsPerSample, SYS_SOUND_PROC_FUNC *fnNofifyFunc )
{
	TSysSound *p = SYSAPI_AllocMem(sizeof(TSysSound));

	p->nChannels = nChannels;
	p->nSamplingRate = nSamplingRate;
	p->nBitsPerSample = nBitsPerSample;

	if( tOemFn.fnCreateSound != atNULL ){
		p->hOemSound = (void *)tOemFn.fnCreateSound( nChannels, nSamplingRate, nBitsPerSample, fnNofifyFunc );
	}

   	return (SYS_SOUND)p;
}

void SYSAPI_DestroySound(SYS_SOUND hSound)
{
	TSysSound *p = (TSysSound *)hSound;
	if( p == atNULL ) return;
		
	if( tOemFn.fnDestroySound != atNULL ){
   		tOemFn.fnDestroySound(p->hOemSound);
	}

	SYSAPI_FreeMem( p );
}

int SYSAPI_SetSoundOEMParam(SYS_SOUND hSound, long lParam1, long lParam2 )
{
	TSysSound *p = (TSysSound *)hSound;
	if( p == atNULL ) return 0;
		
	if( tOemFn.fnSetSoundOEMParam != atNULL ){
   		return (int)tOemFn.fnSetSoundOEMParam( p->hOemSound, lParam1, lParam2);
	}

	return 0;
}

int SYSAPI_GetSoundParam( SYS_SOUND hSound, int *lpnChannels, int *lpnSamplingRate, int *lpnBitsPerSample )
{
	TSysSound *p = (TSysSound *)hSound;
	if( p == atNULL ) return 0;

	if( lpnChannels ) *lpnChannels = p->nChannels;
	if( lpnSamplingRate ) *lpnSamplingRate = p->nSamplingRate;
	if( lpnBitsPerSample ) *lpnBitsPerSample = p->nBitsPerSample;

	return 0;
}

long SYSAPI_CalcStreamSizeFromTime(SYS_SOUND hSound, long nSec)
{
	TSysSound *p = (TSysSound *)hSound;
	if( p == atNULL ) return 0;

	return p->nBitsPerSample * p->nSamplingRate * p->nChannels / 8;

	return 0;
}

int SYSAPI_SetStreamBufferSize( SYS_SOUND hSound, long nBufferSize )
{
	TSysSound *p = (TSysSound *)hSound;
	if( p == atNULL ) return 0;
		
	if( tOemFn.fnSetStreamBufferSize != atNULL ){
		return (int)tOemFn.fnSetStreamBufferSize( p->hOemSound, nBufferSize );
	}

	return 0;
}

int SYSAPI_PutSoundStream(SYS_SOUND hSound, void *lpStream, long nLen )
{
	TSysSound *p = (TSysSound *)hSound;
	if( p == atNULL ) return 0;
		
	if( tOemFn.fnPutSoundStream != atNULL ){
		return (int)tOemFn.fnPutSoundStream( p->hOemSound, lpStream, nLen );
	}

	return 0;
}

int SYSAPI_PlaySound(SYS_SOUND hSound)
{
	TSysSound *p = (TSysSound *)hSound;
	if( p == atNULL ) return 0;
		
	if( tOemFn.fnPlaySound != atNULL ){
   		return (int)tOemFn.fnPlaySound(p->hOemSound);
	}

	return 0;
}

int	SYSAPI_StopSound(SYS_SOUND hSound)
{
	TSysSound *p = (TSysSound *)hSound;
	if( p == atNULL ) return 0;
		
	if( tOemFn.fnStopSound != atNULL ){
   		return (int)tOemFn.fnStopSound(p->hOemSound);
	}

	return 0;
}

int	SYSAPI_PauseSound(SYS_SOUND hSound)
{
	TSysSound *p = (TSysSound *)hSound;
	if( p == atNULL ) return 0;
		
	if( tOemFn.fnPauseSound != atNULL ){
    	return (int)tOemFn.fnPauseSound(p->hOemSound);
	}

	return 0;
}

int	SYSAPI_ResumeSound(SYS_SOUND hSound)
{
	TSysSound *p = (TSysSound *)hSound;
	if( p == atNULL ) return 0;
		
	if( tOemFn.fnResumeSound != atNULL ){
    	return (int)tOemFn.fnResumeSound(p->hOemSound);
	}

	return 0;
}

int SYSAPI_GetSoundVolume(SYS_SOUND hSound)
{
	TSysSound *p = (TSysSound *)hSound;
	if( p == atNULL ) return 0;
		
	if( tOemFn.fnGetSoundVolume != atNULL ){
		return tOemFn.fnGetSoundVolume(p->hOemSound);
	}

	return 0;
}

void SYSAPI_SetSoundVolume(SYS_SOUND hSound, int nVolume)
{
	TSysSound *p = (TSysSound *)hSound;
	if( p == atNULL ) return;
		
	if( tOemFn.fnSetSoundVolume != atNULL ){
		tOemFn.fnSetSoundVolume(p->hOemSound, nVolume);
	}
}

void SYSAPI_SetVibrator(atBOOL on_off, long timeout)
{
//	TSysSound *p = (TSysSound *)hSound;
//	if( p == atNULL ) return 0;
		
	if( tOemFn.fnSetVibrator != atNULL ){
    	tOemFn.fnSetVibrator(on_off, timeout);
	}
}


#endif
