//
// sound_wipic.c
//
////////////////////////////////////////////////////////////////

#include "OEM_SA_sound.h"
#include "SYSAPI.h"
#include <windows.h>
#include <mmsystem.h>


#ifndef _WIN32_WCE
#pragma comment(lib, "winmm.lib")
//#pragma comment(lib, "Wavelib.lib")
#endif


/*
#define PREAMP_FIXED_BITS		8
#define PREAMP_FIXED_FLAT		(1 << PREAMP_FIXED_BITS)
#define BUFLEN_BASE				1024
#define SUBBUF_SIZE				512
*/

/*
#define FADE_BITS	16
#define FADE_TIME	750
#define FADE_BASE	0
*/

/*
#define UCHAR_MID	0x80
#define UINT8_TO_INT16(x) (((x) - UCHAR_MID) << 8)
#define INT16_TO_UINT8(x) (((x) >> 8) + UCHAR_MID)
*/

typedef struct __TSound {
	BOOL		m_bActivate;
	
	int size;
	char *buf;
	SYS_SOUND_PROC_FUNC *fnNofifyFunc;

//	int			m_nLPeek;
//	int			m_nRPeek;

	HWAVEOUT	m_hwo;
	union {
		PCMWAVEFORMAT m_pcm;
		WAVEFORMATEX m_pcm2;
	};

	DWORD		m_dwBufLen;
	BOOL		m_fScanPeek;
//	HANDLE		m_hEvent;
	WAVEHDR	m_WavHdr;
	DWORD		m_cBuf;
	DWORD		m_cWritten;
	DWORD		m_nCurrent;
	LPBYTE		m_pbBuf;
//	DWORD		m_cbBuf;
	DWORD		m_dwWritten;

//	BOOL	m_fFade;
//	int		m_nFadeRate;
//	int		m_nFadeCurrent;
//	int		m_nFadeSamples;

	BOOL		m_fPaused;
	DWORD		m_dwTotalSamples;
//	HANDLE		m_hEventThread;
//	HANDLE		m_hThread;
//	DWORD		m_dwThreadId;
//	DWORD		m_nWriteCur;
//	int			m_nHdrOut;
} _TSound;


static DWORD WINAPI WaveOutThread( void *Param );

static void Reset(_TSound *snd)
{
//	int i;
	
	if (!snd->m_hwo)
		return;

//		CAutoLock lock(&m_csecDevice);
		waveOutPause(snd->m_hwo);
		waveOutReset(snd->m_hwo);

/*
		SetEvent(snd->m_hEvent);
		snd->m_cWritten = 0;
		snd->m_dwWritten = 0;
		snd->m_nCurrent = 0;
//		snd->m_nWriteCur = 0;
		waveOutPause(snd->m_hwo);
*/

//	snd->m_nLPeek = 0;
//	snd->m_nRPeek = 0;
/*
	if (snd->m_fFade) {
		snd->m_nFadeCurrent = FADE_BASE << FADE_BITS;
		snd->m_nFadeSamples = snd->m_pcm.wf.nSamplesPerSec * FADE_TIME / 1000;
		snd->m_nFadeRate = (int)((((double)1 - FADE_BASE) / snd->m_nFadeSamples) * (1 << FADE_BITS));
		snd->m_nFadeRate += 1;
	}
*/
}

/*
static void CheckPeek(_TSound *snd, WAVEHDR* pHdr)
{
	int i;

	int nSamples;
	if (snd->m_pcm.wBitsPerSample == 16) {
		short* p = (short*)pHdr->lpData;
		snd->m_nLPeek = snd->m_nRPeek = 0;
		nSamples = pHdr->dwBufferLength / 2 / snd->m_pcm.wf.nChannels;

		if (snd->m_pcm.wf.nChannels == 2) {
			for ( i = 0; i < nSamples; i++) {
				snd->m_nLPeek = snd->m_nLPeek > *p ? snd->m_nLPeek : *p;
				p++;
				snd->m_nRPeek = snd->m_nRPeek > *p ? snd->m_nRPeek : *p;
				p++;
			}
			snd->m_nLPeek = snd->m_nLPeek * 100 / 0x7FFF;
			snd->m_nRPeek = snd->m_nRPeek * 100 / 0x7FFF;
		}
		else {
			for ( i = 0; i < nSamples; i++) {
				snd->m_nLPeek = snd->m_nLPeek > *p ? snd->m_nLPeek : *p;
				p++;
			}
			snd->m_nLPeek = snd->m_nLPeek * 100 / 0x8FFF;
			snd->m_nRPeek = snd->m_nLPeek;
		}
	}
	else {
		unsigned char* p = (unsigned char*)pHdr->lpData;
		snd->m_nLPeek = snd->m_nRPeek = 0x80;
		nSamples = pHdr->dwBufferLength / snd->m_pcm.wf.nChannels;
		if (snd->m_pcm.wf.nChannels == 2) {
			for ( i = 0; i < nSamples; i++) {
				snd->m_nLPeek = snd->m_nLPeek > *p ? snd->m_nLPeek : *p;
				p++;
				snd->m_nRPeek = snd->m_nRPeek > *p ? snd->m_nRPeek : *p;
				p++;
			}
			snd->m_nLPeek = (snd->m_nLPeek - 0x80) * 100 / 0x7F;
			snd->m_nRPeek = (snd->m_nRPeek - 0x80) * 100 / 0x7F;
		}
		else {
			for ( i = 0; i < nSamples; i++) {
				snd->m_nLPeek = snd->m_nLPeek > *p ? snd->m_nLPeek : *p;
				p++;
			}
			snd->m_nLPeek = (snd->m_nLPeek - 0x80) * 100 / 0x7F;
			snd->m_nRPeek = snd->m_nLPeek;
		}
	}
}
*/

/*
static void FadeIn(_TSound *snd, LPBYTE pbBuf, DWORD cbBuf)
{
	int n, nChannel;
	DWORD cb = 0;
	while ((cbBuf - cb) && snd->m_nFadeSamples) {
		for ( nChannel = 0; nChannel < snd->m_pcm.wf.nChannels; nChannel++) {
			if (snd->m_pcm.wBitsPerSample == 16) {
				n = ((int)*((short*)(pbBuf + cb))) * snd->m_nFadeCurrent;
				*((short*)(pbBuf + cb)) = n >> FADE_BITS;
				cb += 2;
			}
			else {
				n = (UINT8_TO_INT16((int)*(pbBuf + cb))) * snd->m_nFadeCurrent;
				*(pbBuf + cb) = INT16_TO_UINT8(n >> FADE_BITS);
				cb++;
			}
		}
		snd->m_nFadeCurrent += snd->m_nFadeRate;
		snd->m_nFadeSamples--;

		if (snd->m_nFadeCurrent >= (1 << FADE_BITS)) {
			snd->m_nFadeSamples = 0;
			break;
		}
	}
}
*/

static BOOL PrepareBuffer( _TSound *snd, PCMWAVEFORMAT *m_pcm )
{
//	int i;
//	CAutoLock lock(&m_csecBuff);

//	if (snd->m_pHdr)
//		return FALSE;

//	snd->m_cBuf = snd->m_dwBufLen * snd->m_pcm.wf.nAvgBytesPerSec / snd->m_cbBuf / 1000 + 1;
//	if (snd->m_cBuf < 2) snd->m_cBuf = 2;

//	snd->m_pbBuf = (BYTE*)SYSAPI_AllocMem(snd->m_cbBuf);
//	if (!snd->m_pbBuf) return FALSE;
//	memset(snd->m_pbBuf, 0, snd->m_cBuf * snd->m_cbBuf);
	
	memset(&snd->m_WavHdr, 0, sizeof(WAVEHDR));
	snd->m_WavHdr.lpData = snd->m_pbBuf;
//	snd->m_WavHdr.dwBufferLength = snd->m_cbBuf;
	snd->m_WavHdr.dwUser = (DWORD)snd;
	waveOutPrepareHeader(snd->m_hwo, &snd->m_WavHdr, sizeof(WAVEHDR));

	snd->m_nCurrent = 0;
	snd->m_cWritten = 0;
//	snd->m_hEvent = CreateEvent(NULL, TRUE, TRUE, NULL);
	return TRUE;
}

static void FreeBuffer( _TSound *snd )
{
//	CAutoLock lock(&m_csecBuff);

	if (snd->m_pbBuf) {
		SYSAPI_FreeMem(snd->m_pbBuf);
		snd->m_pbBuf = NULL;
	}

/*
	if (snd->m_hEvent) {
		CloseHandle(snd->m_hEvent);
		snd->m_hEvent = NULL;
	}
	snd->m_nCurrent = 0;
*/
}

static void CloseAll( _TSound *snd )
{
//	int i;

/*
	if (snd->m_hThread) {
		PostThreadMessage(snd->m_dwThreadId, WM_QUIT, 0, 0);
		WaitForSingleObject(snd->m_hThread, INFINITE);
		CloseHandle(snd->m_hThread);
		snd->m_hThread = NULL;
	}
*/
	snd->m_bActivate = FALSE;

	if (snd->m_hwo) {
		Reset(snd);

//		CAutoLock lockDev(&m_csecDevice);
//			CAutoLock lock(&m_csecBuff);
		waveOutUnprepareHeader(snd->m_hwo, &snd->m_WavHdr, sizeof(WAVEHDR));

		waveOutClose(snd->m_hwo);
		snd->m_hwo = NULL;
	}

/*
	if (snd->m_hEventThread) {
		CloseHandle(snd->m_hEventThread);
		snd->m_hEventThread = NULL;
	}
*/

	FreeBuffer(snd);
	memset(&snd->m_pcm, 0, sizeof(snd->m_pcm));
}

static void CALLBACK fnWaveOutCallback( HWAVE hWave, UINT uMsg, DWORD dwInstance, DWORD dwParam1, DWORD dwParam2 )
{
//	_TSound* snd = (_TSound*)((WAVEHDR*)dwInstance)->dwUser;
	_TSound* snd = (_TSound*)dwInstance;
	
	switch(uMsg){
		case WOM_OPEN:
			if( snd->fnNofifyFunc ) snd->fnNofifyFunc( (SYS_SOUND)snd, SYS_SOUNDEVENT_OPEN, 0, 0 );
			break;
		case WOM_CLOSE:
			if( snd->fnNofifyFunc ) snd->fnNofifyFunc( (SYS_SOUND)snd, SYS_SOUNDEVENT_CLOSE, 0, 0 );
			break;
		case WOM_DONE:
			if( snd->fnNofifyFunc ) snd->fnNofifyFunc( (SYS_SOUND)snd, SYS_SOUNDEVENT_DONE, 0, 0 );
//			if( snd->m_bActivate && snd->m_bRepeat && snd->m_hwo ) waveOutWrite(snd->m_hwo, &snd->m_WavHdr, sizeof(WAVEHDR));
			break;
		
//		pOutput->PutBuffer((WAVEHDR*)dwParam1);
//		OEM_SYSAPI_PutSoundStream( (OEM_SYS_SOUND)snd, (WAVEHDR*)dwParam1, snd->m_nLen );
	}
}

/*
static void CALLBACK fnWaveOutCallback2(HWAVE hWave, UINT uMsg, DWORD dwInstance, DWORD dwParam1, DWORD dwParam2)
{
//	_TSound* snd = (_TSound*)((WAVEHDR*)dwInstance)->dwUser;
	_TSound* snd = (_TSound*)dwInstance;
	
	if (uMsg == WOM_DONE) {
		if (snd->m_fScanPeek)
			CheckPeek(snd, (WAVEHDR*)dwParam1);
		InterlockedIncrement((long*)&snd->m_nSubBuf);
		SetEvent(snd->m_hEventThread);
	}
}
*/

/*
static DWORD WINAPI WaveOutThread( void *Param )
{
	_TSound* snd = (_TSound*)Param;

	DWORD cb;
	while (1){
		WaitForSingleObject( snd->m_hEventThread, INFINITE );
		
		InterlockedDecrement((long*)&snd->m_nSubBuf);
		if (!snd->m_nSubBuf) ResetEvent(snd->m_hEventThread);
//		CAutoLock lockDev(&m_csecDevice);
		snd->m_HdrOut[snd->m_nHdrOut].dwBytesRecorded = 0;
		while (snd->m_HdrOut[snd->m_nHdrOut].dwBytesRecorded < snd->m_HdrOut[snd->m_nHdrOut].dwBufferLength) {
			if (!snd->m_cWritten || snd->m_fPaused) {
				if (!snd->m_HdrClear[snd->m_nHdrOut]) {
					if (!snd->m_HdrOut[snd->m_nHdrOut].dwBytesRecorded)
						snd->m_HdrClear[snd->m_nHdrOut] = 1;

					memset(snd->m_HdrOut[snd->m_nHdrOut].lpData + snd->m_HdrOut[snd->m_nHdrOut].dwBytesRecorded, 0, 
						snd->m_HdrOut[snd->m_nHdrOut].dwBufferLength - snd->m_HdrOut[snd->m_nHdrOut].dwBytesRecorded);
					snd->m_HdrOut[snd->m_nHdrOut].dwBytesRecorded = snd->m_HdrOut[snd->m_nHdrOut].dwBufferLength;
				}
				break;
			}
			
//			CAutoLock lockBuf(&m_csecBuff);
			cb = min(snd->m_HdrOut[snd->m_nHdrOut].dwBufferLength - snd->m_HdrOut[snd->m_nHdrOut].dwBytesRecorded, 
					snd->m_WavHdr.dwBytesRecorded - snd->m_WavHdr.dwUser);
			memcpy(snd->m_HdrOut[snd->m_nHdrOut].lpData + snd->m_HdrOut[snd->m_nHdrOut].dwBytesRecorded, 
				snd->m_WavHdr.lpData + snd->m_WavHdr.dwUser, cb);
	
			snd->m_WavHdr.dwUser += cb;
			snd->m_HdrOut[snd->m_nHdrOut].dwBytesRecorded += cb;
			snd->m_HdrClear[snd->m_nHdrOut] = 0;
			snd->m_dwTotalSamples += cb / snd->m_pcm.wf.nBlockAlign;
			if (snd->m_WavHdr.dwUser == snd->m_WavHdr.dwBytesRecorded) { 
				snd->m_WavHdr.dwUser = 0;
				snd->m_nWriteCur = (snd->m_nWriteCur + 1) % snd->m_cBuf;
				snd->m_cWritten--;
				SetEvent(snd->m_hEvent);
			}
		}

		waveOutWrite(snd->m_hwo, &snd->m_HdrOut[snd->m_nHdrOut], sizeof(WAVEHDR));
		snd->m_nHdrOut = (snd->m_nHdrOut + 1) % SUBBUF_COUNT;
	}
}
*/

	

static atBOOL OEM_SYSAPI_InitSound( atLONG lSysInst, atLONG lInitData1, atLONG lInitData2 )
{
	return atTRUE;
}

static atVOID OEM_SYSAPI_FinalSound(void)
{
}

static OEM_SYS_SOUND OEM_SYSAPI_CreateSound( int nChannels, int nSamplingRate, int nBitsPerSample, SYS_SOUND_PROC_FUNC *fnNofifyFunc )
{
	MMRESULT mmr;
	int i, nCount = 0;
	_TSound *snd;
	
//	CAutoLock lock(&m_csecDevice);

	if ( waveOutGetNumDevs() <= 0 ) return atNULL;

	snd = (_TSound *)SYSAPI_AllocMem(sizeof(_TSound));
	if( snd == atNULL ) return atNULL;
	SYSAPI_memset( snd, 0, sizeof(_TSound) );
	
	snd->fnNofifyFunc = fnNofifyFunc;

/*	
	snd->m_cbBuf = BUFLEN_BASE;
	if (nSamplingRate > 11025)
		snd->m_cbBuf *= 2;
	if (nSamplingRate > 22050)
		snd->m_cbBuf *= 2;
	if (nChannels > 1)
		snd->m_cbBuf *= 2;
	if (nBitsPerSample > 8)
		snd->m_cbBuf *= 2;
*/

	snd->m_pcm.wf.wFormatTag = WAVE_FORMAT_PCM;
	snd->m_pcm.wf.nChannels = nChannels;
	snd->m_pcm.wf.nSamplesPerSec = nSamplingRate;
	snd->m_pcm.wf.nAvgBytesPerSec = nBitsPerSample * nSamplingRate * nChannels / 8;
	snd->m_pcm.wf.nBlockAlign = nBitsPerSample * nChannels / 8;
	snd->m_pcm.wBitsPerSample = nBitsPerSample;

	for (i = 0; i < 10; i++) {
		mmr = waveOutOpen(&snd->m_hwo, WAVE_MAPPER, (LPWAVEFORMATEX)&snd->m_pcm, (DWORD)fnWaveOutCallback, (DWORD)snd, CALLBACK_FUNCTION);

		if (mmr == MMSYSERR_NOERROR)
			break;
		else if (mmr != MMSYSERR_ALLOCATED)
			goto fail;
		
		SYSAPI_Sleep(50);
	}
	snd->m_fPaused = FALSE;
	snd->m_dwWritten = 0;

	if (!PrepareBuffer(snd, &snd->m_pcm))
		goto fail;

//	snd->m_nLPeek = 0;
//	snd->m_nRPeek = 0;

/*
	if (snd->m_fFade) {
		snd->m_nFadeCurrent = FADE_BASE << FADE_BITS;
		snd->m_nFadeSamples = snd->m_pcm.wf.nSamplesPerSec * FADE_TIME / 1000;
		snd->m_nFadeRate = (int)((((double)1 - FADE_BASE) / snd->m_nFadeSamples) * (1 << FADE_BITS));
		snd->m_nFadeRate += 1;
	}
*/

	snd->m_bActivate = TRUE;

	return (OEM_SYS_SOUND)snd;

fail:
	CloseAll(snd);

	return atNULL;
}

static void OEM_SYSAPI_DestroySound(OEM_SYS_SOUND hOemSound)
{
	_TSound *snd = (_TSound *)hOemSound;
	if( snd == atNULL ) return;
	
//	CAutoLock lock(&m_csecDevice);

//	if (snd->m_fDoubleBuf) {
		CloseAll(snd);
		return;
//	}

	Reset(snd);
}

static int OEM_SYSAPI_GetSoundVolume(OEM_SYS_SOUND hOemSound)
{
	MMRESULT r;
	DWORD volume;
	_TSound *snd = (_TSound *)hOemSound;
	if( snd == atNULL ) return 0;

	r = waveOutGetVolume( snd->m_hwo, &volume );
	if( r == MMSYSERR_NOERROR ) return (int)(volume * 100 / (DWORD)0xFFFF);
	
	return 0;
}

static void OEM_SYSAPI_SetSoundVolume(OEM_SYS_SOUND hOemSound, int nVolume)
{
	_TSound *snd = (_TSound *)hOemSound;
	if( snd == atNULL ) return;

 	waveOutSetVolume( snd->m_hwo, (DWORD)nVolume * (DWORD)0xFFFF / 100 );
}

static int OEM_SYSAPI_SetSoundOEMParam(OEM_SYS_SOUND hOemSound, long lParam1, long lParam2 )
{
	_TSound *snd = (_TSound *)hOemSound;
	if( snd == atNULL ) return 0;
	
//	CAutoLock lock(&m_csecDevice);
	if (snd->m_hwo)
		return FALSE;

//	snd->m_fFade = lParam1;
	return TRUE;
}

static int OEM_SYSAPI_SetStreamBufferSize( OEM_SYS_SOUND hOemSound, long nBufferSize )
{
	_TSound *snd = (_TSound *)hOemSound;
	if( snd == atNULL ) return 0;
	
//	CAutoLock lock(&m_csecDevice);
	if (snd->m_hwo)
		return FALSE;

	if (nBufferSize < 10)
		return FALSE;

	snd->m_dwBufLen = nBufferSize;
	
	return TRUE;
}

static int OEM_SYSAPI_PutSoundStream(OEM_SYS_SOUND hOemSound, void *lpStream, long nLen )
{
//	WAVEHDR WaveHdr;
//	MMRESULT r;
	_TSound *snd = (_TSound *)hOemSound;
	if( snd == atNULL ) return 0;

	snd->m_WavHdr.lpData = lpStream;
	snd->m_WavHdr.dwBufferLength = nLen;

	if (nLen <= 0) {
//		CAutoLock lock(&m_csecBuff);
//		SetEvent(snd->m_hEvent);
		return 0;
	}

//		CAutoLock lockDev(&m_csecDevice);
//		FadeIn(snd, (LPBYTE)lpStream, nLen);
//		r = waveOutWrite(snd->m_hwo, &snd->m_WavHdr, sizeof(WAVEHDR));
//		if( r != MMSYSERR_NOERROR ) return;

//		CAutoLock lockBuf(&m_csecBuff);
//		snd->m_cWritten++;
//		if (snd->m_cWritten >= snd->m_cBuf)
//			ResetEvent(snd->m_hEvent);

/*
		snd->m_nCurrent = (snd->m_nCurrent + 1) % snd->m_cBuf;
		snd->m_dwWritten += nLen / snd->m_pcm.wf.nBlockAlign;
*/

	return 0;
}

static int OEM_SYSAPI_PlaySound(OEM_SYS_SOUND hOemSound)
{
	MMRESULT r;
	_TSound *snd = (_TSound *)hOemSound;
	if( snd == atNULL ) return -1;

	r = waveOutWrite(snd->m_hwo, &snd->m_WavHdr, sizeof(WAVEHDR));
	if( r != MMSYSERR_NOERROR ) return -1;
	
	return 0;
}

static int	OEM_SYSAPI_StopSound(OEM_SYS_SOUND hOemSound)
{
	_TSound *snd = (_TSound *)hOemSound;
	if( snd == atNULL ) return -1;

	waveOutReset( snd->m_hwo );

	return 0;
}

static int	OEM_SYSAPI_PauseSound(OEM_SYS_SOUND hOemSound)
{
	_TSound *snd = (_TSound *)hOemSound;
	if( snd == atNULL ) return -1;
	
	if (!snd->m_hwo)
		return 0;

//		CAutoLock lock(&m_csecDevice);
		waveOutPause(snd->m_hwo);

	return 0;
}

static int	OEM_SYSAPI_ResumeSound(OEM_SYS_SOUND hOemSound)
{
	_TSound *snd = (_TSound *)hOemSound;
	if( snd == atNULL ) return -1;
	
	if (!snd->m_hwo)
		return 0;

//		CAutoLock lock(&m_csecDevice);
		waveOutRestart(snd->m_hwo);

	return 0;
}

static void OEM_SYSAPI_SetVibrator(int on_off, int timeout)
{
/*
	_TSound *snd = (_TSound *)hOemSound;
	if( snd == atNULL ) return;
*/
	
//	MC_mdaVibrator(on_off, timeout);
}


void init_sysapi_sound_oem() 
{
	OEM_SYSAPI_SOUND_T oem_funcs = {
		OEM_SYSAPI_InitSound,
		OEM_SYSAPI_FinalSound,

		OEM_SYSAPI_CreateSound,
		OEM_SYSAPI_DestroySound,

		OEM_SYSAPI_SetSoundOEMParam,

		OEM_SYSAPI_SetStreamBufferSize,
		OEM_SYSAPI_PutSoundStream,

		OEM_SYSAPI_PlaySound,
		OEM_SYSAPI_StopSound,
		OEM_SYSAPI_PauseSound,
		OEM_SYSAPI_ResumeSound,

		OEM_SYSAPI_GetSoundVolume,
		OEM_SYSAPI_SetSoundVolume,

		OEM_SYSAPI_SetVibrator
	};

	OEM_SYSAPI_SOUND_SetOemFn( &oem_funcs );
}

