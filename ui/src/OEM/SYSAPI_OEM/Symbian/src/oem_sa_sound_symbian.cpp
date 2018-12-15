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

/*
#include <MdaAudioOutputStream.h>
#include <mda\common\audio.h>
//#include "AudioOutputCallback.h"

typedef void (*AudioOpenComplete)(void* Obj, void* AudioStream, TInt aError);
typedef void (*AudioBufferCopied)(void* Obj, void* AudioStream, TInt aError,
								  const TDesC8& aBuffer);
typedef void (*AudioPlayComplete)(void* Obj, void* AudioStream, TInt aError);

class CAudioOutput : public CBase, public MMdaAudioOutputStreamCallback
{
public:
	static CAudioOutput* NewL();
	static CAudioOutput* NewLC();
	
	~CAudioOutput();
	
	void SetCallback(void* aCallbackObj, AudioOpenComplete aAudioOpenCompleteCallback,
					 AudioBufferCopied aAudioBufferCopied, AudioPlayComplete aAudioPlayComplete);
	
	void SetAudioInfo(int aChannels, int aSamplingRate, int aBitsPerSample);
	
	void Start();
	void Pause(BOOL aPause);
	void Stop();
		
protected:		
	CMdaAudioOutputStream* iAudioStream; // handle to the stream
	TMdaAudioDataSettings  iAudioSettings; // stream settings
		
private:
	void ConstructL();
	CAudioOutput();
	
	virtual void MaoscOpenComplete(TInt aError);
	virtual void MaoscBufferCopied(TInt aError, const TDesC8& aBuffer);
	virtual void MaoscPlayComplete(TInt aError);
	
	void*	iCallbackObj;
	AudioOpenComplete iAudioOpenCompleteCallback;
	AudioBufferCopied iAudioBufferCopied;
	AudioPlayComplete iAudioPlayComplet;
						 
	int iChannels;
	int iSamplingRate;
	int iBitsPerSample;	
};


CAudioOutput* CAudioOutput::NewL()
{
	CAudioOutput* self = CAudioOutput::NewLC();
    CleanupStack::Pop(self);
	return self;
}

CAudioOutput* CAudioOutput::NewLC()
{
	CAudioOutput* self = new (ELeave) CAudioOutput();
	CleanupStack::PushL(self);
	self->ConstructL();
	return self;
}

void CAudioOutput::ConstructL()
{
	iAudioStream = CMdaAudioOutputStream::NewL(*this);
}

CAudioOutput::CAudioOutput()
{
	iCallbackObj = NULL;
	iAudioOpenCompleteCallback = NULL;
	iAudioBufferCopied = NULL;
	iAudioPlayComplet = NULL;
						 
	iChannels = 0;
	iSamplingRate = 0;
	iBitsPerSample = 0;	
}

CAudioOutput::~CAudioOutput()
{
	delete iAudioStream;
}

void CAudioOutput::SetCallback(void* aCallbackObj, 
							   AudioOpenComplete aAudioOpenCompleteCallback,
							   AudioBufferCopied aAudioBufferCopied, 
							   AudioPlayComplete aAudioPlayComplete)
{
	iCallbackObj = aCallbackObj;
	iAudioOpenCompleteCallback = aAudioOpenCompleteCallback;
	iAudioBufferCopied = aAudioBufferCopied;
	iAudioPlayComplet = aAudioPlayComplete;
}

void CAudioOutput::SetAudioInfo(int aChannels, int aSamplingRate, int aBitsPerSample)
{
	iChannels = aChannels;
	iSamplingRate = aSamplingRate;
	iBitsPerSample = aBitsPerSample;	
}

void CAudioOutput::Start()
{
	iAudioStream->Open(&iAudioSettings);
}

void CAudioOutput::Pause(BOOL fPause)
{
}

void CAudioOutput::Stop()
{
	iAudioStream->Stop();
}


//
// MaoscOpenComplete()
// This function will be called by audio driver as soon as audio device is opend...
void CAudioOutput::MaoscOpenComplete(TInt aError) 
{
	(*iAudioOpenCompleteCallback)(iCallbackObj, iAudioStream, aError);
}

//
// MaoscBufferCopied()
// This function will be called by audio device driver as soon as
// some audio data is played
//
void CAudioOutput::MaoscBufferCopied(TInt aError, const TDesC8& aBuffer)
{
	(*iAudioBufferCopied)(iCallbackObj, iAudioStream, aError, aBuffer);
}

//
// MaoscPlayComplete()
// This function will be called after Stream is stopped by Stop function or underflow
//
void CAudioOutput::MaoscPlayComplete(TInt aError)
{
	(*iAudioPlayComplet)(iCallbackObj, iAudioStream, aError);
}

#include "PlatformDefine.h"

#if _IS_SYMBIAN_
	#include "helper.h"
	#include <MdaAudioOutputStream.h>
#else
	#if _MSC_VER > 1000
		#pragma once
	#endif // _MSC_VER > 1000
#endif

#include "bnfsnd.h"
#include "CEffect.h"
#include "COutput.h"

#include <stdio.h>

#define FADE_BITS	16
#define FADE_TIME	750
#define FADE_BASE	0

COutput* COutput::NewL()
{
	COutput* self = COutput::NewLC();
    CleanupStack::Pop(self);
	return self;
}

COutput* COutput::NewLC()
{
	COutput* self = new (ELeave) COutput();
	CleanupStack::PushL(self);
	self->ConstructL();
	return self;
}

void COutput::ConstructL()
{
	iSoundBuf = new TPtr8(NULL, iOneBufferDataSize, iOneBufferDataSize);
	
	iBufferCriticalSec = new CCritSec;
		
	iBufferFillEvent = new TEvent;
	iFreeBufferEvent = new TEvent;
	
	iAudioDeviceEvent = new TEvent;
	
	iBufferFillEvent->Reset();
	iFreeBufferEvent->Set();
}

COutput::COutput()
{
	//printf("Start of COutput Constructor!!! \n");
	iBufferList = NULL;
	iDataBuffer = NULL;
	iDataFilledBufferNum = 0;
	iTotalWrittenStreamBlockNum = 0;
	iTotalBufferedBlockNum = 0;
	iCurrentWriteBufferIndex = 0;
	iCurrentReadBufferIndex = 0;

	iAudioOutputController = NULL;
	iOutputStopController = NULL;
	
	iBufferCriticalSec = NULL;
	iBufferFillEvent = NULL;
	iFreeBufferEvent = NULL;
	iAudioDeviceEvent = NULL;
	iSoundBuf = NULL;


	//default setting....
	iDefaultBufferLen = 500;
	iBufferNum = 8;
	iOneBufferDataSize = 16 * 1024;

	ifScanPeek = FALSE;
	iLPeek = 0;
	iRPeek = 0;
	
	ifFade = FALSE;
	iFadeSamples = 0;

	iTotalWrittenStreamBlockNum = 0;
	iTotalBufferedBlockNum = 0;
	ifPaused = FALSE;
	
	iChannels = 0;
	iSamplingRate = 0;
	iBitsPerSample = 0;
	ifPlaying = FALSE;
	
	//start thread here....
	iStopThread = FALSE;
	iThread.Close();
	iThreadStart();
	//printf("End of COutput Constructor!!! \n");
}

COutput::~COutput()
{
//	iThreadEnd();

//	Close();

	//delete buffer and objects now....

	if(iBufferFillEvent)
		delete iBufferFillEvent;
	iBufferFillEvent = NULL;

	if(iFreeBufferEvent)
		delete iFreeBufferEvent;
	iFreeBufferEvent = NULL;

	if(iAudioDeviceEvent)
		delete iAudioDeviceEvent;
	iAudioDeviceEvent = NULL;

	if(iBufferCriticalSec)
		delete iBufferCriticalSec;
	iBufferCriticalSec = NULL;

	if(iSoundBuf)
		delete iSoundBuf;
	iSoundBuf = NULL;
	
}

void COutput::iThreadStart()
{
	TInt res = iThread.Create(_L("ThreadAudioOutput"), AudioOutputThreadEntryPoint, KDefaultStackSize, 
		    			       1024*50, 1024*200, (TAny*)(this), EOwnerProcess);
	if (res == KErrNone) {
		TRequestStatus rendezvous_status;
		iThread.Rendezvous(rendezvous_status);
		iThread.Resume();
		User::WaitForRequest(rendezvous_status);
	}
}

void COutput::iThreadEnd()
{
//	iAudioOutputController->Stop(&iThread);
//	iAudioOutputController->Close(&iThread);

	//wait for thread to end...
	TRequestStatus bye_status;
	//wait for play thread to end...
	iThread.Logon(bye_status);

	
	TRequestStatus* status = &bye_status;
   	User::RequestComplete( status, KErrNone );

//	iOutputStopController->Stop(&iThread);

	//stop Thread now...
	User::WaitForRequest(bye_status);

//	User::After(2000);
//	iThread.Close();

}

TInt COutput::AudioOutputThreadEntryPoint(TAny* pParam)
{
	COutput* p_output = (COutput*)pParam;

	p_output->AudioOutputThreadProc();

	return 0;
}

//
// AudioOutputThreadProc()
//
void COutput::AudioOutputThreadProc()
{
	//printf("Start of COutput AudioOutputThreadProc!!! \n");

	// get clean-up stack
	CTrapCleanup* cleanup = CTrapCleanup::New();
	
	// create an active scheduler and server
	CActiveScheduler *active_schduler = new CActiveScheduler;
	
	CActiveScheduler::Install(active_schduler);
	
	//create an active object
	iAudioOutputController = new CAudioOutputController;
	iOutputStopController = new COutputStopController;
	
	//I don't know why NewL is failed...maybe Cleanup satck?
	iSoundFileOutput = new CSoundFileOutput;//CSoundFileOutput::NewL();
	
	RThread::Rendezvous(KErrNone);
	
	//printf("Start of COutput AudioOutputThreadProc Loop!!! \n");
	CActiveScheduler::Start();
	
	//end of thread...
	
	//delete object
	delete iSoundFileOutput;
	iSoundFileOutput = NULL;
	
	//delete active object;
	delete iAudioOutputController;
	iAudioOutputController = NULL;
	
	delete iOutputStopController;
	iOutputStopController = NULL;
		
	delete active_schduler;
	delete cleanup;
	
	//printf("End of COutput AudioOutputThreadProc !!! \n");
}

BOOL COutput::SetOutputParam(DWORD aDefaultBufferLen, BOOL aFade)
{
	if (aDefaultBufferLen < 10)
		return FALSE;

	iDefaultBufferLen = aDefaultBufferLen;
	ifFade = aFade;
	return TRUE;
}

void COutput::GetOutputParam(LPDWORD aDefaultBufferLen, BOOL* aFade)
{
	*aDefaultBufferLen = iDefaultBufferLen;
	*aFade = ifFade;
}

BOOL COutput::Open(int nChannels, int nSamplingRate, int nBitsPerSample)
{
	if (nChannels == iChannels &&
		nSamplingRate == iSamplingRate &&
		nBitsPerSample == iBitsPerSample)
	return TRUE;

	CloseAll();

	iOneBufferDataSize = BUFLEN_BASE;
	
	if (nSamplingRate > 11025)
		iOneBufferDataSize *= 2;
	if (nSamplingRate > 22050)
		iOneBufferDataSize *= 2;
	if (nChannels > 1)
		iOneBufferDataSize *= 2;
	if (nBitsPerSample > 8)
		iOneBufferDataSize *= 2;
	
	iChannels = nChannels;
	iSamplingRate = nSamplingRate;
	iBitsPerSample = nBitsPerSample;
	iBlockSize = nBitsPerSample * nChannels / 8;;
	
	ifPaused = FALSE;
	iTotalWrittenStreamBlockNum = 0;
	iTotalBufferedBlockNum = 0;

	if (!PrepareBuffer())
		goto fail;

	iLPeek = 0;
	iRPeek = 0;

	if (ifFade) {
		iFadeCurrent = FADE_BASE << FADE_BITS;
		iFadeSamples = nSamplingRate * FADE_TIME / 1000;
		iFadeRate = (int)((((double)1 - FADE_BASE) / iFadeSamples) * (1 << FADE_BITS));
		iFadeRate += 1;
	}
		
	return TRUE;

fail:
	CloseAll();
	return FALSE;
}

void COutput::Close()
{
	CloseAll();
}

WAVEHDR* COutput::GetBuffer()
{
	int i = 0;
	
	//printf("\n\r COutput::GetBuffer Now getting free buffer");
	
	while (1) {
		iFreeBufferEvent->Wait(10);
		 {
		 	CAutoLock lock(iBufferCriticalSec);
		 	if (iDataFilledBufferNum < iBufferNum)
		 		break;
		 }
		 i++;
		 //if (i > 1000)
		//	 printf("\n\r COutput::GetBuffer Some problem occurred!!!");
			
	}
	//printf("\n\r COutput::GetBuffer Now got free buffer(%d)", 
	//	   iCurrentWriteBufferIndex);
	
	return &iBufferList[iCurrentWriteBufferIndex];
}

//release current read buffer because driver says that it's finished...
void COutput::PutBuffer()
{
	static int index = 0;
	
	//printf("COutput::PutBuffer index(%d)\r\n", index);
	
	if (ifScanPeek)
		CheckPeek(&iBufferList[iCurrentReadBufferIndex]);
	
	
	{
		CAutoLock lock(iBufferCriticalSec);
		iFreeBufferEvent->Set();
		iDataFilledBufferNum--;
		iCurrentReadBufferIndex = (iCurrentReadBufferIndex + 1) % iBufferNum;
		if (iDataFilledBufferNum == 0)
			iBufferFillEvent->Reset();
	}
	//printf("COutput::PutBuffer End index(%d)\r\n", index);
	
	index++;
}

//application feeds sound data by this function...
void COutput::OutputBuffer(WAVEHDR* aCurrentFillDataItem)
{
	static int number_buffered = 0;
	
	CAutoLock lock(iBufferCriticalSec);
	FadeIn(aCurrentFillDataItem->lpData, aCurrentFillDataItem->dwBytesRecorded);
	
	//printf("COutput::OutputBuffer new PCM Data : %d\r\n", number_buffered);
	number_buffered++;
	
	iBufferFillEvent->Set();
	iDataFilledBufferNum++;
	iCurrentWriteBufferIndex = (iCurrentWriteBufferIndex + 1) % iBufferNum;
	iTotalBufferedBlockNum += aCurrentFillDataItem->dwBytesRecorded / iBlockSize;
	if (iDataFilledBufferNum >= iBufferNum)
		iFreeBufferEvent->Reset();
}

BOOL COutput::PrepareBuffer()
{
	if (iBufferList)
		return FALSE;

	iBufferNum = iDefaultBufferLen * (iBitsPerSample * iSamplingRate * iChannels / 8) / 
				 iOneBufferDataSize / 1000 + 1;
	
	if (iBufferNum < 4) 
		iBufferNum = 4;
	

	iBufferList = new WAVEHDR[iBufferNum];
	if (!iBufferList)
		return FALSE;

	iDataBuffer = new BYTE[iBufferNum * iOneBufferDataSize];
	if (!iDataBuffer)
		return FALSE;

	memset(iDataBuffer, 0, iBufferNum * iOneBufferDataSize);
	
	for (UINT i = 0; i < iBufferNum; i++) {
		memset(&iBufferList[i], 0, sizeof(WAVEHDR));
		iBufferList[i].lpData = iDataBuffer + (iOneBufferDataSize * i);
		iBufferList[i].dwBufferLength = iOneBufferDataSize;
	}
	iCurrentWriteBufferIndex = 0;
	iCurrentReadBufferIndex = 0;
	iDataFilledBufferNum = 0;
	
	iBufferFillEvent->Reset();
	iFreeBufferEvent->Set();
	
	return TRUE;
}

void COutput::FreeBuffer()
{
	CAutoLock lock(iBufferCriticalSec);

	if (iBufferList) {
		delete [] iBufferList;
		iBufferList =  NULL;
	}

	if (iDataBuffer) {
		delete [] iDataBuffer;
		iDataBuffer = NULL;
	}
	
	iCurrentWriteBufferIndex = 0;
	iCurrentReadBufferIndex = 0;
	iDataFilledBufferNum = 0;		
}

void COutput::Reset()
{
	CAutoLock lock(iBufferCriticalSec);	
	iDataFilledBufferNum = 0;
	iTotalWrittenStreamBlockNum = 0;
	iTotalBufferedBlockNum = 0;
	iCurrentWriteBufferIndex = 0;
	iCurrentReadBufferIndex = 0;
		
	iLPeek = 0;
	iRPeek = 0;
	
	if (ifFade) {
		iFadeCurrent = FADE_BASE << FADE_BITS;
		iFadeSamples = iSamplingRate * FADE_TIME / 1000;
		iFadeRate = (int)((((double)1 - FADE_BASE) / iFadeSamples) * (1 << FADE_BITS));
		iFadeRate += 1;
	}
	
	iBufferFillEvent->Reset();
	iFreeBufferEvent->Set();
}

void COutput::Start()
{
	if (!iAudioOutputController) return;

	iAudioDeviceEvent->Reset();
	
	//start audio output controller, set audio info here...
	iAudioOutputController->SetAudioInfo(iChannels, iSamplingRate, iBitsPerSample);
	iAudioOutputController->SetCallback(this, 
										COutput::AudioOpenCompleteCallback, 
										COutput::AudioBufferCopiedCallback, 
										COutput::AudioPlayCompleteCallback);
	ifPlaying = TRUE;
	iAudioOutputController->Open(&iThread);
	iAudioOutputController->Play(&iThread);
	
	iAudioDeviceEvent->Wait(INFINITE);
	
	//printf(" COuput::Finished initializing AudioOutputController!\n");
	
	//open audio file output
	iSoundFileOutput->Open();
	
	//printf(" COuput::Finished initializing SoundFileOutput!\n");
}

void COutput::Pause(BOOL fPause)
{
	iAudioOutputController->Pause(&iThread, fPause);
}

void COutput::Stop()
{
	{
		CAutoLock lock(iBufferCriticalSec);
		ifPlaying  = FALSE;
		//wake up the buffer copied callback in to stop properly.
		iBufferFillEvent->Set();
	}
	
//	iAudioDeviceEvent->Reset();
	iAudioOutputController->Stop(&iThread);
//	iAudioOutputController->Close(&iThread);
	//iAudioDeviceEvent->Wait(INFINITE);
	
	//close audio file output
	iSoundFileOutput->Close();
}

void COutput::PlaySoundFile(TCHAR* aSoundFileName)
{
	//start file play and wait for play to end...
	iSoundFileOutput->PlaySoundFile(aSoundFileName);
}

DWORD COutput::GetCurrent()
{
	return iTotalWrittenStreamBlockNum;
}

BOOL COutput::IsFlushed()
{
	return GetCurrent() >= iTotalBufferedBlockNum;
}

void COutput::GetBufferInfo(DWORD* aTotalDataBufferSize, DWORD* aBufferedDataSize)
{
	if (!iBufferList) {
		*aTotalDataBufferSize = 0;
		*aBufferedDataSize = 0;
		return;
	}

	*aTotalDataBufferSize = iBufferNum * iOneBufferDataSize; 
	*aBufferedDataSize = iDataFilledBufferNum * iOneBufferDataSize; 
}

void COutput::CheckPeek(WAVEHDR* aCurrentDataItem)
{
	int nSamples;
	if (iBitsPerSample == 16) {
		short* p = (short*)aCurrentDataItem->lpData;
		iLPeek = iRPeek = 0;
		nSamples = aCurrentDataItem->dwBytesRecorded / 2 / iChannels;

		if (iChannels == 2) {
			for (int i = 0; i < nSamples; i++) {
				iLPeek = iLPeek > *p ? iLPeek : *p;
				p++;
				iRPeek = iRPeek > *p ? iRPeek : *p;
				p++;
			}
			iLPeek = iLPeek * 100 / 0x7FFF;
			iRPeek = iRPeek * 100 / 0x7FFF;
		}
		else {
			for (int i = 0; i < nSamples; i++) {
				iLPeek = iLPeek > *p ? iLPeek : *p;
				p++;
			}
			iLPeek = iLPeek * 100 / 0x8FFF;
			iRPeek = iLPeek;
		}
	}
	else {
		unsigned char* p = (unsigned char*)aCurrentDataItem->lpData;
		iLPeek = iRPeek = 0x80;
		nSamples = aCurrentDataItem->dwBytesRecorded / iChannels;
		if (iChannels == 2) {
			for (int i = 0; i < nSamples; i++) {
				iLPeek = iLPeek > *p ? iLPeek : *p;
				p++;
				iRPeek = iRPeek > *p ? iRPeek : *p;
				p++;
			}
			iLPeek = (iLPeek - 0x80) * 100 / 0x7F;
			iRPeek = (iRPeek - 0x80) * 100 / 0x7F;
		}
		else {
			for (int i = 0; i < nSamples; i++) {
				iLPeek = iLPeek > *p ? iLPeek : *p;
				p++;
			}
			iLPeek = (iLPeek - 0x80) * 100 / 0x7F;
			iRPeek = iLPeek;
		}
	}
}

void COutput::FadeIn(LPBYTE aBuf, DWORD aBufNum)
{
	int n;
	DWORD index = 0;
	while ((aBufNum - index) && (iFadeSamples != 0)) {
		for (int nChannel = 0; nChannel < iChannels; nChannel++) {
			if (iBitsPerSample == 16) {
				n = ((int)*((short*)(aBuf + index))) * iFadeCurrent;
				*((short*)(aBuf + index)) = n >> FADE_BITS;
				index += 2;
			}
			else {
				n = (UINT8_TO_INT16((int)*(aBuf + index))) * iFadeCurrent;
				*(aBuf + index) = INT16_TO_UINT8(n >> FADE_BITS);
				index++;
			}
		}
		iFadeCurrent += iFadeRate;
		iFadeSamples--;

		if (iFadeCurrent >= (1 << FADE_BITS)) {
			iFadeSamples = 0;
			break;
		}
	}
}

short Clip16(int s);
unsigned char Clip8(int s);

void COutput::Preamp(LPBYTE aBuffer, DWORD aNumOfBuffer, int aRate)
{
	int n;

	if (aRate == PREAMP_FIXED_FLAT)
		return;

	if (iBitsPerSample == 16) {
		for (DWORD index = 0; index < aNumOfBuffer; ) {
			for (int nChannel = 0; nChannel < iChannels; nChannel++) {
				n = (int)*((short*)(aBuffer + index)) * aRate;
				*((short*)(aBuffer + index)) = Clip16(n >> PREAMP_FIXED_BITS);
				index += 2;
			}
		}
	}
	else {
		for (DWORD index = 0; index < aNumOfBuffer; ) {
			for (int nChannel = 0; nChannel < iChannels; nChannel++) {
				n = UINT8_TO_INT16((int)*(aBuffer + index)) * aRate;
				*(aBuffer + index) = Clip8(INT16_TO_UINT8(n >> PREAMP_FIXED_BITS));
				index++;
			}
		}
	}
}


#define ZERO_SAMPLE_MAX		16
DWORD COutput::ScanZeroSamples(BOOL aForward, LPBYTE aBuffer, DWORD aNumOfBuffer)
{
	int nZeroSamples = 0;
	int nBytesPerSample = iChannels * iBitsPerSample / 8;
	int nSamples = aNumOfBuffer / nBytesPerSample;

	if (iBitsPerSample == 16) {
		short* pSample;
		if (aForward) {
			pSample = (short*)aBuffer;
		}
		else {
			pSample = (short*)(aBuffer + aNumOfBuffer - nBytesPerSample);
			nBytesPerSample = -nBytesPerSample;
		}

		if (iChannels == 2) {
			for (int i = 0; i < nSamples; i++) {
				if ((*pSample > ZERO_SAMPLE_MAX) || (*pSample < -ZERO_SAMPLE_MAX) ||
					(*(pSample + 1) > ZERO_SAMPLE_MAX) || (*(pSample + 1) < -ZERO_SAMPLE_MAX))
					break;
				nZeroSamples++;
				pSample = (short*)((BYTE*)pSample + nBytesPerSample);
			}
		}
		else {
			for (int i = 0; i < nSamples; i++) {
				if ((*pSample > ZERO_SAMPLE_MAX) || (*pSample < -ZERO_SAMPLE_MAX))
					break;
				nZeroSamples++;
				pSample = (short*)((BYTE*)pSample + nBytesPerSample);
			}
		}
	}
	else {
		BYTE* pSample;
		if (aForward) {
			pSample = aBuffer;
		}
		else {
			pSample = aBuffer + aNumOfBuffer - nBytesPerSample;
			nBytesPerSample = -nBytesPerSample;
		}

		if (iChannels == 2) {
			for (int i = 0; i < nSamples; i++) {
				if (*pSample != 0x80 || *(pSample + 1) != 0x80)
					break;
				nZeroSamples++;
				pSample += nBytesPerSample;
			}
		}
		else {
			for (int i = 0; i < nSamples; i++) {
				if (*pSample != 0x80)
					break;
				nZeroSamples++;
				pSample += nBytesPerSample;
			}
		}
	}

	return nZeroSamples * iChannels * (iBitsPerSample / 8);
}

void COutput::CloseAll()
{	
	//close audio stream here...

	FreeBuffer();
	
	iChannels = 0;
	iSamplingRate = 0;
	iBitsPerSample = 0;
}

DWORD COutput::GetBufferingSamples()
{
	DWORD dwCurrent = GetCurrent();
	return iTotalBufferedBlockNum - dwCurrent;
}

void COutput::AudioOpenCompleteCallback(void* aObj, void* aAudioStream, TInt aError)
{
	COutput* output = (COutput*)aObj;
	
	return output->AudioOpenComplete(aAudioStream, aError);
}

void COutput::AudioBufferCopiedCallback(void* aObj, void* aAudioStream, 
								  		 TInt aError, const TDesC8& aBuffer)
{
	COutput* output = (COutput*)aObj;
	
	output->AudioBufferCopied(aAudioStream, aError, aBuffer);
}

void COutput::AudioPlayCompleteCallback(void* aObj, void* aAudioStream, TInt aError)
{
	COutput* output = (COutput*)aObj;
	
	output->AudioPlayComplete(aAudioStream, aError);
}

//
// MaoscOpenComplete()
// This function will be called by audio driver as soon as audio device is opend...
void COutput::AudioOpenComplete(void* aAudioStream, TInt aError) 
{
	CMdaAudioOutputStream* audio_stream = (CMdaAudioOutputStream*)aAudioStream;
	int audio_rate;
 	int audio_channel;
 		
 	if (aError==KErrNone) { 		
 		switch (iSamplingRate) {
 			case 8000:
 				audio_rate = TMdaAudioDataSettings::ESampleRate8000Hz;
 				break;
 			case 11025:
 				audio_rate = TMdaAudioDataSettings::ESampleRate11025Hz;
 				break;
 			case 12000:
 				audio_rate = TMdaAudioDataSettings::ESampleRate12000Hz;
 				break;
 			case 16000:
 				audio_rate = TMdaAudioDataSettings::ESampleRate16000Hz;
 				break; 			 
 			case 22050:
 				audio_rate = TMdaAudioDataSettings::ESampleRate22050Hz;
 				break;
 			case 24000:
 				audio_rate = TMdaAudioDataSettings::ESampleRate24000Hz;
 				break;
 			case 32000:
 				audio_rate = TMdaAudioDataSettings::ESampleRate32000Hz;
 				break;
 			case 44100:
 				audio_rate = TMdaAudioDataSettings::ESampleRate44100Hz;
 				break;
 			case 48000:
 				audio_rate =  TMdaAudioDataSettings::ESampleRate48000Hz;
 				break;
 			default:
 				return;
 		}
 		
 		if (iChannels == 1) {
 			audio_channel = TMdaAudioDataSettings::EChannelsMono;
 		} 
 		else if (iChannels == 2) {
 			audio_channel = TMdaAudioDataSettings::EChannelsStereo;
 		}
 		else {
 			return;
 		}
 		
 		// set stream properties to 16bit,16KHz mono
 		audio_stream->SetAudioPropertiesL(audio_rate, audio_channel);

 		// note that MaxVolume() is different in the emulator and the real device!
 		audio_stream->SetVolume(audio_stream->MaxVolume()/2);        
 		audio_stream->SetPriority(EPriorityMuchMore, EMdaPriorityPreferenceNone);
 		
 		iAudioDeviceEvent->Set();
 		
 		// Fill first buffer and write it to the stream
 		//if (iDataFilledBufferNum != 0)
 		WriteBufferToDevice(audio_stream);
 	}
}

//
// MaoscBufferCopied()
// This function will be called by audio device driver as soon as
// some audio data is played
//
void COutput::AudioBufferCopied(void* aAudioStream, TInt aError, 
								const TDesC8& aBuffer)
{
	CMdaAudioOutputStream* audio_stream = (CMdaAudioOutputStream*)aAudioStream;
	
	//printf("COutput::AudioBufferCopied Called!!!\r\n");
	
	//put current buffer
	PutBuffer();
	
	if (aError==KErrNone) {
		//if (iDataFilledBufferNum == 0)
		WriteBufferToDevice(audio_stream);
	}
}

//
// MaoscPlayComplete()
// This function will be called after Stream is stopped by Stop function or underflow
//
void COutput::AudioPlayComplete(void* aAudioStream, TInt aError)
{
	CMdaAudioOutputStream* audio_stream = (CMdaAudioOutputStream*)aAudioStream;
	
	iAudioDeviceEvent->Set();
	
	//printf("COutput::AudioPlayComplete Called!!!\r\n");
	
	// we only want to restart in case of an underflow
	// if aError!=KErrUnderflow the stream probably was stopped manually
 	if (aError==KErrUnderflow) {
 		WriteBufferToDevice(audio_stream);
 	}
}

//
// WriteBufferToDevice()
//
void COutput::WriteBufferToDevice(CMdaAudioOutputStream* audio_stream)
{
	//wait for data...
	while (1) {
		while (iDataFilledBufferNum == 0) {
			if (!ifPlaying)
				return;
			//printf("COutput::WriteBufferToDevice Wait for new PCM Data\r\n");
			iBufferFillEvent->Wait(INFINITE);
			//printf("COutput::WriteBufferToDevice Wakeup\r\n");
		}
	
		if (iBufferList[iCurrentReadBufferIndex].dwBytesRecorded == 0) {
			//put current buffer
			PutBuffer();
		}
		else
			break;
	}
		
	//there's data in current buffer item...
	{
		iSoundBuf->Set(iBufferList[iCurrentReadBufferIndex].lpData,
					   iBufferList[iCurrentReadBufferIndex].dwBytesRecorded,
					   iBufferList[iCurrentReadBufferIndex].dwBytesRecorded);
					   //iBufferList[iCurrentReadBufferIndex]->dwBytesRecorded,
					   //iBufferList[iCurrentReadBufferIndex]->dwBytesRecorded);
		
		// call WriteL with a descriptor pointing at iSoundData
		//printf("\r\n Now COutput::WriteBufferToDevice(%x %x %x %x)", 
		//	   iBufferList[iCurrentReadBufferIndex].lpData[0],
		//	   iBufferList[iCurrentReadBufferIndex].lpData[1],
		//	   iBufferList[iCurrentReadBufferIndex].lpData[2],
		//	   iBufferList[iCurrentReadBufferIndex].lpData[3]);
		
		audio_stream->WriteL(*iSoundBuf);
		iTotalWrittenStreamBlockNum += iBufferList[iCurrentReadBufferIndex].dwBytesRecorded / 
									   iBlockSize;
	}
}

*/



atBOOL OEM_SYSAPI_InitSound( atLONG lSysInst, atLONG lInitData1, atLONG lInitData2 )
{
	return atTRUE;
}

atVOID OEM_SYSAPI_FinalSound( NOARGS )
{
}

OEM_SYS_SOUND OEM_SYSAPI_CreateSound( int nChannels, int nSamplingRate, int nBitsPerSample, SYS_SOUND_PROC_FUNC *lpCallback )
{
	return atNULL;
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
