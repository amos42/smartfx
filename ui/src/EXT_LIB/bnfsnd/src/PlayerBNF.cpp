#include <windows.h>
#include <TCHAR.H>

#include "bnfsnd.h"
//#include "helper.h"
#include "player.h"
#include "HeapManager.h"

//#include "stl/map"
#include "map"

BOOL CBnfPlayer::BnfOpenFile(LPCTSTR pszFile)
{
	BOOL fRet = FALSE;
	DWORD     dwRead;

//	BnfClose();
	
	//m_Reader.Close();

	if (!m_Reader.Open(pszFile))
		return FALSE;

	if(m_Reader.Read((unsigned char*)&m_sndHeader, sizeof(m_sndHeader), &dwRead)){
		//assert(sizeof(m_sndHeader) == dwRead);
		if(m_sndHeader.rec_cnt){

			if(m_pSndIdx)
				SndHeapFree(m_pSndIdx);
			m_pSndIdx = (TBNF_SndIdx*)SndHeapAlloc(sizeof(TBNF_SndIdx)*m_sndHeader.rec_cnt);
			if(!m_pSndIdx)
			{
				BnfClose();
				return FALSE;
			}
			memset(m_pSndIdx, NULL, sizeof(TBNF_SndIdx)*m_sndHeader.rec_cnt);
			m_Reader.Read( (unsigned char*)m_pSndIdx, m_sndHeader.rec_cnt * sizeof(TBNF_SndIdx), &dwRead);

			TBNF_SndIdx* pIter = m_pSndIdx;
			long IterSize = 0;
			IterSize = m_sndIndexMap.size();
			if(IterSize !=0 )
				m_sndIndexMap.clear();

			
			for(int idx=0; idx < m_sndHeader.rec_cnt; idx++, pIter++){
				m_sndIndexMap.insert( TSndIndexMap::value_type(pIter->code, pIter) );
			}
		}
	}

	m_bnfOffset = sizeof(m_sndHeader.hdr_size) + sizeof(TBNF_SndIdx) * m_sndHeader.rec_cnt;
	
	int nBuff = sizeof(m_szFile)/sizeof(m_szFile[0]);
	_tcsncpy(m_szFile, pszFile, nBuff-1);
	m_szFile[nBuff-1] = _T('\0');
	if (!BnfScanFile()) {
		m_Reader.Close();
		return FALSE;
	}

	m_fOpen = OPEN_BNF_FILE;

	if(!m_hSndAddEvent){
		m_hSndAddEvent = CreateEvent( NULL, FALSE, FALSE, NULL);
	}

	return TRUE;
}


BOOL CBnfPlayer::BnfScanFile()
{
	return BnfScanFileNormally();
}


BOOL CBnfPlayer::BnfScanFileNormally()
{
	// 벫믦궻댧뭫궻긲깒??궻귒귩긚긌긿깛궥귡
	DWORD cbRead;
	LONGLONG llCur, llSize;
	MPEG_AUDIO_INFO infoCur, infoNext;
	BYTE bBuff[MPG_SCAN_BUFF_LEN];
	int nFrameCount, nSkip;

	memset(&m_Info, 0, sizeof(MPEG_AUDIO_INFO));
	memset(&infoCur, 0, sizeof(MPEG_AUDIO_INFO));
	memset(&infoNext, 0, sizeof(MPEG_AUDIO_INFO));

	m_Reader.SetPointer(m_bnfOffset, FILE_BEGIN);
	llCur = m_Reader.ScanAudioHeader();
	if (llCur == MAXLONGLONG)
		return FALSE;
		
	while (TRUE) {
		if (!m_Reader.Read(bBuff, MPG_SCAN_BUFF_LEN, &cbRead))
			return FALSE;

		if (cbRead < 4)
			return FALSE;

		if (!ParseHeader(bBuff, &infoCur)) {
			m_Reader.SetPointer(llCur + 1, FILE_BEGIN);
			llCur = m_Reader.ScanAudioHeader();
			if (llCur == MAXLONGLONG)
				return FALSE;

			continue;
		}

		if (m_Reader.SetPointer(llCur + infoCur.nFrameSize, FILE_BEGIN) == MAXLONGLONG)
			return FALSE;

		if (!m_Reader.Read(bBuff, MPG_SCAN_BUFF_LEN, &cbRead))
			return FALSE;

		if (cbRead < 4)
			return FALSE;

		if (!ParseHeader(bBuff, &infoNext)) {
			m_Reader.SetPointer(llCur + 1, FILE_BEGIN);
			llCur = m_Reader.ScanAudioHeader();
			if (llCur == MAXLONGLONG)
				return FALSE;

			continue;
		}

		if (infoCur.nVersion != infoNext.nVersion ||
			infoCur.nLayer != infoNext.nLayer ||
			infoCur.nSamplingRate != infoNext.nSamplingRate) {
			m_Reader.SetPointer(llCur + 1, FILE_BEGIN);
			llCur = m_Reader.ScanAudioHeader();
			if (llCur == MAXLONGLONG)
				return FALSE;

			continue;
		}

		m_Info = infoCur;

		if (m_Info.nVersion == 2)
			nSkip = m_Info.nChannels == 2 ? 17 : 9;
		else
			nSkip = m_Info.nChannels == 2 ? 32 : 17;

		if (m_Reader.SetPointer(llCur + nSkip + 4, FILE_BEGIN) == MAXLONGLONG)
			return FALSE;

		if (!m_Reader.Read(bBuff, MPG_SCAN_BUFF_LEN, &cbRead))
			return FALSE;

		if (cbRead < 4)
			return FALSE;

		// Xing header
		if (bBuff[0] == 'X' && bBuff[1] == 'i' && bBuff[2] == 'n' && bBuff[3] == 'g') {
			if (m_Reader.SetPointer(4, FILE_CURRENT) == MAXLONGLONG)
				return FALSE;
			
			if (!m_Reader.Read(bBuff, MPG_SCAN_BUFF_LEN, &cbRead))
				return FALSE;

			if (cbRead < 4)
				return FALSE;

			nFrameCount = bBuff[0] << 24 | bBuff[1] << 16 | bBuff[2] << 8 | bBuff[3];
			llSize =  m_Reader.GetSize();
			m_nDuration = (DWORD)m_Info.nSamplesPerFrame * nFrameCount;

			m_Info.nFrameSize = (int)(llSize / nFrameCount);
			m_Info.nBitRate = (int)((llSize * 8/ (m_nDuration / m_Info.nSamplingRate) + 500) / 1000);
		}
		// VBRI header
		else if (bBuff[0] == 'V' && bBuff[1] == 'B' && bBuff[2] == 'R' && bBuff[3] == 'I') {
			if (m_Reader.SetPointer(10, FILE_CURRENT) == MAXLONGLONG)
				return FALSE;
			
			if (!m_Reader.Read(bBuff, MPG_SCAN_BUFF_LEN, &cbRead))
				return FALSE;

			if (cbRead < 4)
				return FALSE;

			nFrameCount = bBuff[0] << 24 | bBuff[1] << 16 | bBuff[2] << 8 | bBuff[3];
			llSize =  m_Reader.GetSize();
			m_nDuration = (DWORD)m_Info.nSamplesPerFrame * nFrameCount;

			m_Info.nFrameSize = (int)(llSize / nFrameCount);
			m_Info.nBitRate = (int)((llSize * 8/ (m_nDuration / m_Info.nSamplingRate) + 500) / 1000);
		}
		else {
			// CBR
			llSize =  m_Reader.GetSize();
			nFrameCount = int(llSize / m_Info.nFrameSize);
			m_nDuration = (DWORD)m_Info.nSamplesPerFrame * nFrameCount;
		}
		m_Reader.SetPointer(m_bnfOffset, FILE_BEGIN);
		return TRUE;
	}
}


int CBnfPlayer::BnfRender(LPBYTE pbInBuf, DWORD cbInBuf, LPDWORD pcbProceed)
{
	int nRet;
	BOOL fNeedOutput = FALSE;
	DWORD cbInput, cbOutput;
	*pcbProceed = 0;
	do {
		// 믠?긲깋긐궻?긃긞긏
		if (m_fStop)
			return MAD_OK;

		// 뢯쀍긫긞긲?궻둴뺎
		if (!m_pOutHdr || !m_cbOutBufLeft) {
			if (m_pOutHdr) {
				OutputBuffer(m_pOutHdr, m_cbOutBuf - m_cbOutBufLeft);
				m_cbOutBufLeft = 0;
				m_pOutHdr = NULL;
			}
			m_pOutHdr = m_Output.GetBuffer();
			m_cbOutBufLeft = m_cbOutBuf;
			if (m_fSeek)
				return MAD_OK;
			if (m_fStop)
				return MAD_OK;
		}

		// 긢??궻?긃긞긏
		if (!fNeedOutput && !CheckAudioHeader(pbInBuf + *pcbProceed)) {
			while (++*pcbProceed < cbInBuf - 4) {
				MPEG_AUDIO_INFO info;
				if (ParseHeader(pbInBuf + *pcbProceed, &info) &&
					info.nVersion == m_Info.nVersion &&
					info.nLayer == m_Info.nLayer && 
					info.nSamplingRate == m_Info.nSamplingRate)
					break;
			}

			if (*pcbProceed > cbInBuf - 4)
				return MAD_NEED_MORE_INPUT;
		}	

		// 긢긓?긤 (1긲깒??)
		nRet = m_Decoder.Decode(pbInBuf + *pcbProceed, fNeedOutput ? 0 : cbInBuf - *pcbProceed, 
			(LPBYTE)m_pOutHdr->lpData + (m_cbOutBuf - m_cbOutBufLeft), 
			m_cbOutBufLeft, &cbOutput, &cbInput, 16, FALSE);
		switch (nRet) {
		case MAD_FATAL_ERR:
			// 깏긜긞긣궥귡
			m_Decoder.Reset();
		case MAD_ERR:
			*pcbProceed += 1; // 1긫귽긣궬궚릋귕귡
			break;
		case MAD_NEED_MORE_OUTPUT:
			fNeedOutput = TRUE;
			*pcbProceed += cbInput;
			m_cbOutBufLeft -= cbOutput;
			break;
		case MAD_OK:
			fNeedOutput = FALSE;
			*pcbProceed += cbInput;
			m_cbOutBufLeft -= cbOutput;
			break;
		}
	}
	while (nRet != MAD_NEED_MORE_INPUT || fNeedOutput);
	return nRet;
}


DWORD CBnfPlayer::BnfPlayerThread()
{
	#define MPG_FILE_READ_SIZE	(MPG_FILE_BUFF_LEN * 8)
	
	BOOL fRet;
	BOOL fFlush = FALSE;
	DWORD cbBufSize = 0 , cbInBuf = 0, cbInBufLeft = 0;
	DWORD state = 0;
	DWORD offset =0;
	DWORD snd_size = 0;
	DWORD read_size =0;
	BOOL  bNext = TRUE;

	TBNF_SndIdx* pIndex = NULL;
	int snd_code;

	//m_isRunning = TRUE;

	cbBufSize = MPG_FILE_READ_SIZE;
	LPBYTE pbRead = (BYTE*)SndHeapAlloc(MPG_FILE_READ_SIZE);

	if (!pbRead)
	{
		pbRead = (BYTE*)SndHeapAlloc(MPG_FILE_BUFF_LEN);
		cbBufSize = MPG_FILE_BUFF_LEN;
		if (!pbRead){
			//m_isRunning = FALSE;
			return RET_ERROR;
		}
	}

	memset(pbRead, 0, cbBufSize);

	while (TRUE) 
	{
		if (m_fStop && !BnfQueueCount() && m_Status!=SND_STATUS_PLAY)
		{
			SndHeapFree(pbRead);
			UpdateStatus(SND_STATUS_STOP);
			return RET_STOP;
		}

		if( BnfQueueCount() == 0)
		{
			UpdateStatus(SND_STATUS_WAIT);
			state = WaitForSingleObject(m_hSndAddEvent, INFINITE);
			if(state != WAIT_OBJECT_0) continue;
			//if( BnfQueueCount() == 0) continue;//break;
		}
		
		//m_Reverb.Reset();
		//m_Echo.Reset();
		//m_BassBoost.Reset();
		//m_3DChorus.Reset();
		//m_Output.Reset();
		//m_Decoder.Reset();
		cbInBufLeft = 0;
		m_cbOutBufLeft =0;
		m_pOutHdr = NULL;
		m_fFileBegin = TRUE;
		m_nWritten = 0;
	
		{
		CAutoLock lock(&m_csecSndBuffer);

		snd_code = PopBnfSnd();

		if(snd_code == GUIDE_PROMPT_SND && m_sEffectSndFilename[0])
		{
			PlaySound(m_sEffectSndFilename, NULL, SND_SYNC | SND_FILENAME );
			continue;
		}
		pIndex = GetSndIndex(snd_code);

		if(m_destSndCode == snd_code){
			m_bDestSndPlayed = TRUE;
		}

		if(pIndex == NULL){
			continue;
		}

		UpdateStatus(SND_STATUS_PLAY);
		// 디비깅 용으로 메시지 보냄 
		// PostCallbackMessage(SND_MSG_BASE +100, snd_code, 0);
		snd_size = pIndex->size;
		// 벶귒뜛귒
		m_Reader.SetPointer(m_bnfOffset + pIndex->offset, FILE_BEGIN);

		read_size = 0;
		}

		while(read_size < snd_size)
		{
			if(m_fStop) break;

			// Critical Section궻긜긞긣
			CAutoLock lock(&m_csecThread);
			if (m_fSeek) {
				if (m_Status == SND_STATUS_PLAY)
					m_fPlay = TRUE;

				m_Reverb.Reset();
				//m_Echo.Reset();
				m_BassBoost.Reset();
				//m_3DChorus.Reset();
				m_Output.Reset();
				m_Decoder.Reset();
				
				cbInBufLeft = 0;
				m_fSeek = FALSE;
				m_pOutHdr = NULL;
				break;
			}

			fRet = m_Reader.Read(pbRead + cbInBufLeft, cbBufSize - cbInBufLeft, &cbInBuf);
			if (!fRet || !cbInBuf) 
			{
				if (!fRet && GetLastError() != ERROR_SUCCESS) 
				{
					SndHeapFree(pbRead);
					//m_isRunning = FALSE;
					return RET_ERROR;
				}
				fFlush = TRUE;
			}
			
			if(read_size + cbInBuf > snd_size)
			{
				cbInBuf = snd_size - read_size;
				read_size = snd_size;
			}
			else read_size += cbInBuf;

			cbInBufLeft += cbInBuf;
			cbInBuf = 0;

			if (fFlush) 
			{
				if (!UnpreparePlayback(TRUE)) fFlush = FALSE;
				SndHeapFree(pbRead);
				//m_isRunning = FALSE;
				return RET_EOF;
			}

			if (BnfRender(pbRead, cbInBufLeft, &cbInBuf) == MAD_FATAL_ERR) 
			{
				SndHeapFree(pbRead);
				//m_isRunning = FALSE;
				return RET_ERROR;
			}
			if (m_fSuppress) 
			{
				SndHeapFree(pbRead);
				return RET_EOF;
			}
			memmove(pbRead, pbRead + cbInBuf, cbInBufLeft - cbInBuf);
			cbInBufLeft -= cbInBuf;

			Sleep(0);
		}

		if (m_pOutHdr) 
		{
			OutputBuffer(m_pOutHdr, m_cbOutBuf - m_cbOutBufLeft);
			m_pOutHdr = NULL;
			m_cbOutBufLeft = 0;
		}

		while (!m_Output.IsFlushed()) 
		{
			if (m_fPlay && GetStatus() == SND_STATUS_PLAY) 
			{
				m_Output.Pause(FALSE);
				m_fPlay = FALSE;
			}
			Sleep(1);
			if (m_fSeek)
			{
				SndHeapFree(pbRead);
				return RET_EOF;//FALSE;
			}
			if (m_fStop) break;
			UpdatePeek();
		}

		Sleep(1);

		//UpdateStatus(SND_STATUS_PAUSE);
		//m_Output.Pause(TRUE);
		//m_fPlay = FALSE;
	}
	
	if(pbRead) SndHeapFree(pbRead);

	return RET_EOF;
}


void CBnfPlayer::BnfStop()
{
	if(m_hSndAddEvent){
		m_fStop = TRUE;
		SetEvent(m_hSndAddEvent);	// Thread waiting을 멈추기 위해
	}
	m_Reader.SetPointer(m_bnfOffset, FILE_BEGIN);
}


void CBnfPlayer::BnfClose()
{
	BnfStop();
	m_Reader.Close();

	if(m_pSndIdx){
		SndHeapFree(m_pSndIdx);
		m_pSndIdx = NULL;
	}
	m_bnfOffset = 0;

	if(m_hSndAddEvent){
		CloseHandle(m_hSndAddEvent);
		m_hSndAddEvent = NULL;
	}

	while(m_bnfQueue.size()){
		m_bnfQueue.pop();
	}
}


int cmp_sndcode(const void* rec1, const void* rec2)
{
	return ((TBNF_SndIdx*)rec1)->code -  ((TBNF_SndIdx*)rec2)->code;
}


TBNF_SndIdx*CBnfPlayer::GetSndIndex(int code)
{
	CAutoLock lock(&m_csecSndBuffer);

	if(m_pSndIdx == NULL) return NULL;

	TSndIndexMap::iterator iter = m_sndIndexMap.find(code);
	if( iter == m_sndIndexMap.end()) return NULL;

	return (*iter).second;
	//return iter.second;

	//TBNF_SndIdx key;
	//TBNF_SndIdx* find;
	//key.code = code;
	//find = (TBNF_SndIdx*)bsearch(&key, m_pSndIdx, m_sndHeader.rec_cnt, sizeof(key), cmp_sndcode);
	//return find;

}


int CBnfPlayer::PopBnfSnd()
{
	CAutoLock lock(&m_csecSndBuffer);

	int snd_code;
	if(m_bnfQueue.empty()) return -1;

	snd_code = m_bnfQueue.front();

	m_bnfQueue.pop();

	return snd_code;
}


int CBnfPlayer::AddBnfSnd(int code, BOOL bDest /*= FALSE*/)
{
	CAutoLock lock(&m_csecSndBuffer);

	// 목적지 안내가 이미 등록된 경우 또 넣지 않음. 
	if(m_bDestAdded) return m_bnfQueue.size();

	m_bnfQueue.push(code);

	if(bDest){
		m_destSndCode = code;
		m_bDestSndPlayed = FALSE;
		m_bDestAdded = TRUE;
	}
	else{
		m_bDestAdded = FALSE;
	}


	if(GetStatus() != SND_STATUS_WAIT){
		Play();
	}
	if(m_hSndAddEvent){
		SetEvent(m_hSndAddEvent);
	}

	return m_bnfQueue.size();
}


void CBnfPlayer::ClearSndBuffer()
{
	CAutoLock lock(&m_csecSndBuffer);

	while(!m_bnfQueue.empty()){
		m_bnfQueue.pop();
	}

	m_destSndCode   = 0;
	m_bDestSndPlayed = FALSE;	
	m_bDestAdded     = FALSE;
}


int CBnfPlayer::BnfQueueCount()
{
	CAutoLock lock(&m_csecSndBuffer);		
	return m_bnfQueue.size();
}

/*
 *	목적지 안내가 출력되었는지 여부
 */
BOOL CBnfPlayer::DestPlayed()
{
	CAutoLock lock(&m_csecSndBuffer);		

	return m_bDestSndPlayed;
}


BOOL CBnfPlayer::BnfSeekFile(long lTime)
{

	BOOL fRet = FALSE;
	BOOL fPause = FALSE;

	// 렄듩궻빾듂
	int nNew = (int)((double)m_Info.nSamplingRate * lTime / 1000);
	int nFrames = nNew / m_Info.nSamplesPerFrame;

	LONGLONG llOld;
	if (m_Options.fScanMpegCompletely) {
		llOld = m_Reader.SetPointer(0, FILE_BEGIN);
		if (m_Reader.ScanAudioHeader(m_Info.nVersion, m_Info.nLayer) != MAXLONGLONG) {
			int nCount = 0;
			BYTE bBuff[4];
			DWORD dwRead;
			while (m_Reader.Read(bBuff, sizeof(bBuff), &dwRead) && dwRead == 4)	{
				// scan header
				MPEG_AUDIO_INFO info;
				if (ParseHeader(bBuff, &info)) {
					if (++nCount >= nFrames) {
						m_Reader.SetPointer(-4, FILE_CURRENT);
						fRet = TRUE;
						break;
					}
					else {
						m_Reader.SetPointer(info.nFrameSize - 4, FILE_CURRENT);
					}
				}
				else {
					m_Reader.SetPointer(-3, FILE_CURRENT);
					if (m_Reader.ScanAudioHeader(m_Info.nVersion, m_Info.nLayer) == MAXLONGLONG)
						break;
				}
			}
		}
	}
	else {
		LONGLONG llOffset = m_Info.nFrameSize * nFrames;

		// seek the offset
		llOld = m_Reader.SetPointer(llOffset, FILE_BEGIN);
		if (llOld != MAXLONGLONG) {
			if (m_Reader.ScanAudioHeader(m_Info.nVersion, m_Info.nLayer) != MAXLONGLONG)
				fRet = TRUE;
		}
	}

	if (fRet) {
		// 긘?긏뚣궻뚣룉뿚
		m_fSeek = TRUE;
		m_nSeek = nNew;

		m_Output.Reset(); // 긖긂깛긤긫긞긲?궼긏깏귺궥귡
	}
	else {
		m_Reader.SetPointer(llOld, FILE_BEGIN);
	}
	return fRet;
}


BOOL CBnfPlayer::Reset()
{
	m_Reverb.Reset();
	//m_Echo.Reset();
	m_BassBoost.Reset();
	m_3DChorus.Reset();
	m_Output.Reset();
	m_Decoder.Reset();

	return BnfScanFile();
}

void CBnfPlayer::SetEffectSndFile(const TCHAR* filename)
{
	_tcsncpy(m_sEffectSndFilename, filename, sizeof(m_sEffectSndFilename)/sizeof(TCHAR) -1);
	m_sEffectSndFilename[sizeof(m_sEffectSndFilename)/sizeof(TCHAR) -1] = _T('\0');
}