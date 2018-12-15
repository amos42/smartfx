#ifndef LOCK_SYMBIAN_H_
#define LOCK_SYMBIAN_H_

//#define DZ_LOCK	(LOG_X | _CL(a,0,0))

#include "PlatformDefine.h"
#include "Symbian2Win32.h"

#include <e32cmn.h>

class CCritSec
{
public:
	CCritSec()
	{
		m_csec.CreateLocal();
		m_curId = 0;
		m_cnt = 0;
	}
	
	~CCritSec()
	{
		m_csec.Close();
	}
	
	void Lock()
	{
        if (m_curId != RThread().Id())
        {
            m_csec.Wait();
            m_curId = RThread().Id();            
            m_cnt = 1;
        }
        else 
        {
           m_cnt++;
        }
	} 	
	        
	void Unlock()
	{		
       if(m_curId == RThread().Id())
       {
            if (m_cnt == 1)
            {
               m_cnt = 0;
               m_curId = 0;
               m_csec.Signal();
            }
            else
            {
               m_cnt--;
            }
        }
	}

	
	/*
	void Lock()
	{
        if(m_curId != RThread().Id() && m_cnt >= 0)
        {
            m_csec.Wait();
            m_curId = RThread().Id();            
            m_cnt++;
        }		
        
        // original code
		m_csec.Wait();
		//for debug - ybh
		m_cnt++;
		
	}
	void Unlock()
	{		
	    if(m_curId == RThread().Id() && m_cnt > 0)
	    {
	        m_cnt--;
	        m_csec.Signal();
	        
	    }
	    // original code
		if (m_csec.IsBlocked()) {
			m_csec.Signal();
			//for debug - ybh
			m_cnt--;
		}
		
	}
	*/
	
	//protected:
	RCriticalSection m_csec;
	TInt m_cnt;
	TThreadId m_curId;
	
};

class CAutoLock
{
public:
	CAutoLock(CCritSec* pcsec)
	{
	     
        m_pcsec = pcsec;
        m_pcsec->Lock();
        
	    /*
		if(m_curId != RThread().Id())
		{
		    m_pcsec = pcsec;
		    m_pcsec->Lock();
		    m_curId = RThread().Id();
		}
		*/
		
		//m_curId = RThread().Id();
		
	}
	
	~CAutoLock()
	{
		m_pcsec->Unlock();
	}
protected:
	CCritSec* m_pcsec;
	//TThreadId m_curId;
};

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

#endif /*LOCK_SYMBIAN_H_*/
