#ifndef LOCK_TX_H_
#define LOCK_TX_H_

//#define DZ_LOCK	(LOG_X | _CL(a,0,0))

#include "PlatformDefine.h"


class CCritSec
{
public:
	CCritSec();
	
	~CCritSec();
	
	void Lock();
	        
	void Unlock();
	
protected:	
	SYS_CRSECTION m_crs;
	
};

class CAutoLock
{
public:
	CAutoLock(CCritSec* pcsec);
	
	~CAutoLock();
protected:
	CCritSec* m_pcsec;
};

#if 0
class TEvent
{
public:
	TEvent(BOOL fInitialState = TRUE)
	{
		m_csec.CreateLocal();
		if (fInitialState) {
			m_counter = 1; 	
			m_semaphore.CreateLocal(1);
		}
		else {
			m_counter = 0; 
			m_semaphore.CreateLocal(0);
		}
	}
	~TEvent()
	{
		m_csec.Close();
		m_semaphore.Close();
		
	}
	BOOL Set()
	{
		m_csec.Wait();
		if (m_counter != 0) {
			m_csec.Signal();
			return FALSE;
		}
		
		m_counter = 1;
		m_semaphore.Signal();
		m_csec.Signal();
		return TRUE;
	}
	BOOL Reset()
	{
		m_csec.Wait();
		if (m_counter != 1) { 
			m_csec.Signal();
			return FALSE;
		}
			
		m_counter = 0;
		m_semaphore.Wait();
		m_csec.Signal();
		return TRUE;
	}
	/*BOOL Pulse()
	{
		return PulseEvent(m_hEvent);
	}*/
	//dwTimeout is wait time in miliseconds...
	BOOL Wait(DWORD dwTimeOut = INFINITE)
	{	
		m_csec.Wait();
		
		if (m_counter == 1) {
			m_csec.Signal();
			return TRUE;
		}
		
		m_csec.Signal();
		
		//wait for some other thread to call Set Event...
		if (dwTimeOut == INFINITE) {
		   m_semaphore.Wait();
		   m_semaphore.Signal();
		   return TRUE;
		}
		else {
			TInt res = m_semaphore.Wait((TInt)dwTimeOut * 1000);
			if (res == KErrNone) {
				m_semaphore.Signal();
				return TRUE;
			}
			else {
				return FALSE;
			}
		}
	}
protected:
	RCriticalSection m_csec;
	RSemaphore		 m_semaphore;	
	TInt32			 m_counter;
};

#endif

#endif /*LOCK_TX_H_*/
