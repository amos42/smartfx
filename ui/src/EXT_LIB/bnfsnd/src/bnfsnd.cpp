#include <windows.h>
#include "bnfsnd.h"
#include "Player.h"

#if !_IS_SYMBIAN_
	#if defined(MAPLAY_EXPORTS) && !defined(_WIN32_WCE)
		BOOL __declspec(dllexport) APIENTRY DllMain( HANDLE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
		{
    		return TRUE;
		}
	#endif
#endif

CBnfPlayer *g_pPlayer = NULL;

void* SND_Initialize(void* hWnd/*= NULL*/)
{
	CBnfPlayer* pPlayer = new CBnfPlayer();
	if(!pPlayer) return NULL;

	g_pPlayer = pPlayer;
#if !_IS_SYMBIAN_
	g_pPlayer->SetMessageWindow((HWND)hWnd);
#endif

	return pPlayer;
}

void SND_Uninitialize()
{
	if(g_pPlayer){
		delete g_pPlayer;
		g_pPlayer = NULL;
	}
}

BOOL SND_IsInitialized()
{
	return g_pPlayer ? TRUE : FALSE;
}

BOOL SND_Open(LPCTSTR pszFile)
{
	if(!g_pPlayer) return FALSE;
	return g_pPlayer->OpenFile(pszFile);
}

void SND_Close()
{
	if(g_pPlayer)
		g_pPlayer->Close();
}

void SND_Suspend()
{
#if _IS_SYMBIAN_
	if(g_pPlayer && !g_pPlayer->IsSuspend()){
		g_pPlayer->Suspend();
	}
#endif
}

void SND_Resume()
{
#if _IS_SYMBIAN_
	if(g_pPlayer && g_pPlayer->IsSuspend()){
		g_pPlayer->Resume();
	}
#endif
}

void SND_Sleep(long a)
{
#if _IS_SYMBIAN_
		if(g_pPlayer){
			g_pPlayer->Sleep(a);
		}
#endif
}

int SND_AddSnd(int snd_code)
{
	if(!g_pPlayer || snd_code == -1) return FALSE;

#if _IS_SYMBIAN_
	if(g_pPlayer->IsSuspend()) return FALSE;
#endif
	
	SND_Play();
	SND_PlayAudioDevice();

	g_pPlayer->AddBnfSnd(snd_code);
	//g_pPlayer->Play();
	
	return g_pPlayer->BnfQueueCount();
}

BOOL SND_ValidCode(long code)
{
	if (!g_pPlayer) return FALSE;

	return g_pPlayer->GetSndIndex(code) ? TRUE : FALSE;
}

BOOL SND_Play()
{
	if(!g_pPlayer) return FALSE;

#if _IS_SYMBIAN_
	if(g_pPlayer->IsSuspend())
		return FALSE;
#endif

	return g_pPlayer->Play();
}

BOOL SND_PlayAudioDevice()
{
	if(!g_pPlayer) return FALSE;

#if _IS_SYMBIAN_
	if (!g_pPlayer->IsPlaying())
		g_pPlayer->PlayAudioDevice();
	return TRUE;
#else
	return FALSE;
#endif
}

BOOL SND_WaitAudioDeviceReady()
{
#if _IS_SYMBIAN_
	if(!g_pPlayer) 
		return FALSE;
	
	g_pPlayer->WaitAudioDeviceReady();
	return TRUE;
#else
	return FALSE;
#endif
}

BOOL SND_WaitAudioDevicePlayEnd()
{
#if _IS_SYMBIAN_
	if(!g_pPlayer) 
		return FALSE;
	
	g_pPlayer->WaitAudioDevicePlayEnd();
	return TRUE;
#else
	return FALSE;
#endif
}


void SND_StopAudioDevice()
{
#if _IS_SYMBIAN_
	if(g_pPlayer){
	g_pPlayer->StopAudioDevice();
	}
#endif
}

void SND_Stop()
{
	// fix-me:
	// 마지막 음성이 끊기는 관계로 대체하였으나
	// 그로 인해 thread가 프로그램 종료전 까지 계속 동작하는 문제가 있다.
	if(g_pPlayer)
	{
		g_pPlayer->ClearSndBuffer();
		//while (g_pPlayer->GetStatus() == SND_STATUS_PLAY) Sleep(1);
		//g_pPlayer->Stop();
	}
}

void SND_ClearSndBuffer()
{
	if(g_pPlayer)
		g_pPlayer->ClearSndBuffer();
}

/*
 *	출력용 버퍼에 들어 있는 음성 코드의 수 반환 
 */
int SND_GetQueueCount()
{
	return g_pPlayer ? g_pPlayer->BnfQueueCount() : 0;
}

BOOL SND_IsPlaying()
{
	return SND_GetQueueCount() || SND_GetStatus()==SND_STATUS_PLAY;
}

//BOOL SND_Ff(long lSkip)
//{
//	if(!g_pPlayer) return FALSE;
//	return g_pPlayer->Ff(lSkip);
//}
//
//BOOL SND_Rew(long lSkip)
//{
//	if(!g_pPlayer) return FALSE;
//	return g_pPlayer->Rew(lSkip);
//}
//
//BOOL SND_Seek(long lTime)
//{
//	if(!g_pPlayer) return FALSE;
//	return g_pPlayer->Seek(lTime);
//}
//
//long SND_GetDuration()
//{
//	if(!g_pPlayer) return 0;
//	return g_pPlayer->GetDuration();
//}
//

void SND_SetBassBoostLevel(int nLevel)
{
	if(g_pPlayer){
		g_pPlayer->SetBassBoostLevel(nLevel);
	}
}

int SND_GetBassBoostLevel()
{
	if(!g_pPlayer) return -1;
	return g_pPlayer->GetBassBoostLevel();
}

SND_STATUS SND_GetStatus()
{
	if(!g_pPlayer) return SND_STATUS_STOP;
	return g_pPlayer->GetStatus();
}


void SND_AudioDeviceClose()
{
	if(g_pPlayer){
		g_pPlayer->AudioDeviceClose();
	}
}

long SND_GetFileType()
{
	if (!g_pPlayer) return SND_OPEN_NONE;

// Toranado에서는 아직 미지원
//	return g_pPlayer->GetFileType();
	return SND_OPEN_MP3_FILE;
}

/*
 *	목적지 안내가 나왔는지 여부 
 */
BOOL SND_IsDestPlayed()
{
	if(g_pPlayer)
		return g_pPlayer->DestPlayed();

	return FALSE;
}

BOOL SND_Reset()
{
	if(g_pPlayer)
		return g_pPlayer->Reset();

	return FALSE;
}


void SND_SetEffectSndFile(const TCHAR* filename)
{
	if(g_pPlayer){
		g_pPlayer->SetEffectSndFile(filename);
	}
}

BOOL SND_SetVolume(WORD _lVolume, WORD _rVolume)
{
	// Toranado에서는 아직 미지원
	/*if(g_pPlayer)
	{
		HWAVEOUT hwo = g_pPlayer->GetWaveoutHandle();
		if (hwo)
			return waveOutSetVolume(hwo, (DWORD)MAKELONG(_lVolume, _rVolume));
			//MMSYSERR_INVALHANDLE
	}*/

	return FALSE;
}

void SND_SetEqualizerPreAmp(int nPreAmp, BOOL bEnable)
{
	// limit : master equalizer preamp 만 설정한다. 

	// Toranado에서는 아직 미지원
	/*if(g_pPlayer)
	{
		EQUALIZER equ;
		memset(&equ, 0, sizeof(equ));
		g_pPlayer->GetEqualizer(&equ);

		equ.fEnable = bEnable;
		equ.preamp  = nPreAmp > 20 ? nPreAmp : 20;
		g_pPlayer->SetEqualizer(&equ);
	}*/
}

#if _IS_SYMBIAN_
// Kyle Start [Add 2009/07/10-23:37]
BOOL BNFSND_IsPlayComplete()
{
	if(g_pPlayer)
		return g_pPlayer->IsPlayComplete();
	
	return FALSE;
}

void BNFSND_SetPlayStandBy()
{
	if(g_pPlayer)
		g_pPlayer->SetPlayStandBy();
}
// Kyle Start [Add 2009/07/10-23:37]
#endif
