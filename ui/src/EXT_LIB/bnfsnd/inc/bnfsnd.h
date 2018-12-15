	/*
 *  GSPlayer - The audio player for WindowsCE
 *  Copyright (C) 2003  Y.Nagamidori
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
 */

/*
 *	 Modified by Jason 2006.3.1
 */

#if !defined(__BNF_SND_PLAY_H__)
#define __BNF_SND_PLAY_H__
//#include "PlatformDefine.h"

enum SND_FILE_TYPE
{
	SND_OPEN_NONE = 0, 
	SND_OPEN_MP3_FILE, 
	SND_OPEN_OGG_FILE, 
	SND_OPEN_WAV_FILE, 
	
};

typedef struct
{
	BOOL fEnable;
	int preamp;	  //set value 64 to 0 (default 31) updated 2.18
	int data[10]; //set value 64 to 0 (default 31) updated 2.18
}EQUALIZER;

#define EFFECT_REVERB		0
#define EFFECT_ECHO			1
#define EFFECT_SURROUND		2
#define EFFECT_3DCHORUS		3

typedef struct 
{
	BOOL fEnable;
	int nDelay;
	int nRate;
}EFFECT;

// nDelay
// EFFECT_REVERB: 0 - infinite
// EFFECT_ECHO: 0 - infinite
// EFFECT_SURROUND: ignore
// EFFECT_3DCHORUS: ignore
// nRate
// EFFECT_REVERB: 0 - infinite
// EFFECT_ECHO: 0 - infinite
// EFFECT_SURROUND: 0 - 100
// EFFECT_3DCHORUS: 0 - 100 (updated 2.16)

typedef struct
{
	int			nVersion;
	int			nLayer;
	int			nChannels;
	int			nSamplingRate;
	int			nBitRate;
	int			nDuration;
}SND_INFORMATION;

typedef struct
{
	int			nThreadPriority;
	int			nOutputBufferLen;		// buffer length (20 - 5000 ms)
	int			nOutputPrebuffer;		// prebuffer (0 - 100 percent)
	BOOL		fScanMpegCompletely;
	BOOL		fFadeIn;
	BOOL		fSuppressZeroSamples;
	BOOL		fAlwaysOpenDevice;
}SND_OPTIONS;

#define MAX_URL		MAX_PATH
typedef struct
{
	int nBuf;	// 1 buffer = 2048 bytes
	int nPreBuf;
	BOOL fUseProxy;
	TCHAR szProxy[MAX_URL];
	TCHAR szUserAgent[MAX_URL];
}SND_STREAMING_OPTIONS;

#define MAX_TAG_STR		255
typedef struct tID3Tag
{
	TCHAR szTrack[MAX_TAG_STR];
	TCHAR szArtist[MAX_TAG_STR];
	TCHAR szAlbum[MAX_TAG_STR];
	TCHAR szComment[MAX_TAG_STR];
	TCHAR szGenre[MAX_TAG_STR];
	int nYear;
	int nTrackNum;
}ID3TAGV1;

#ifndef MAXLONGLONG
//#define MAXLONGLONG                      (0x7fffffffffffffff)
#define MAXLONGLONG                      0x7FFFFFFF
#endif

#if _IS_SYMBIAN_
#else
#define SND_MSG_BASE		        (WM_USER + 9000)
#define SND_MSG_STATUS		        (SND_MSG_BASE + 1) // wParam == SND_STATUS, lParam = error (stop only)
#define SND_MSG_PEEK		        (SND_MSG_BASE + 2) // wParam == l lParam == r
#define SND_MSG_STREAM				(SND_MSG_BASE + 10000)
#define SND_MSG_STREAM_TITLE		(SND_MSG_STREAM + 1) // wParam = Stream Title (TCHAR[MAX_URL])
#endif

#ifndef __SND_STATUS_DEF_
#define __SND_STATUS_DEF_
enum SND_STATUS{SND_STATUS_STOP, SND_STATUS_PLAY, SND_STATUS_PAUSE, SND_STATUS_WAIT};
#endif

enum SND_STREAMING_STATUS{
	SND_STREAMING_DISABLED, SND_STREAMING_DISCONNECTED, SND_STREAMING_CONNECTING, 
	SND_STREAMING_BUFFERING, SND_STREAMING_CONNECTED
};

#define GUIDE_PROMPT_SND   1000

#ifdef __cplusplus
extern "C" {
#endif

void* SND_Initialize(void* hWnd = NULL);
void SND_Uninitialize();
BOOL SND_IsInitialized();
BOOL SND_Open(LPCTSTR pszFile);


/*
 *	음성코드를 Queue에 등록  
 *    도착지 음성코드일 경우, bDestination을 TRUE로 준다.
 */
BOOL SND_AddSnd(int snd_code);
BOOL SND_ValidCode(long code);

// Kyle Start [Add 2009/07/10-23:37]
BOOL BNFSND_IsPlayComplete();	// 사운드 구간 종료 플래그 확인함수
void BNFSND_SetPlayStandBy();	// 사운드 구간 종료 플래그 재설정함수
// Kyle [Add 2009/07/10-23:37] End

void SND_Close();
BOOL SND_Play();
void SND_Stop();
void SND_ClearSndBuffer();
void SND_Pause();
void SND_SetBassBoostLevel(int nLevel);
int SND_GetBassBoostLevel();
SND_STATUS SND_GetStatus();
void SND_AudioDeviceClose();
long SND_GetFileType();

//these are added because of symbian property....
BOOL SND_PlayAudioDevice();
BOOL SND_WaitAudioDeviceReady();
BOOL SND_WaitAudioDevicePlayEnd();
void SND_StopAudioDevice();

/*
 *	출력용 버퍼에 들어 있는 음성 코드의 수 반환 
 */
int SND_GetQueueCount();
BOOL SND_IsPlaying();

/*
 *	목적지 안내가 출력되었는지 여부
 */
BOOL SND_IsDestPlayed();
BOOL SND_Reset();
void SND_SetEffectSndFile(const TCHAR* filename);

BOOL SND_SetVolume(WORD _lVolume, WORD _rVolume);

//Button Effect Sound Type
#define	BTN_EFFECT_CLICK	0
#define	BTN_EFFECT_DISABLE	1

void SND_PlaySound(UINT nResId);
void SND_PlayClickEffect(DWORD dwEffectType = BTN_EFFECT_CLICK);

void SND_Suspend();
void SND_Resume();
void SND_Sleep(long a);

void SND_SetEqualizerPreAmp(int nPreAmp, BOOL bEnable);

#ifdef __cplusplus
};
#endif

#endif __BNF_SND_PLAY_H__
