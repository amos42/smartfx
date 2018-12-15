/*
 * Purpose: Thread for Symbian
 * include : Symbian2Win32.h
 * library : None 
 * name    : Taejun Park
 * Remark  : The name and role of member function are based on OCThread of 'Beagle'
 * History : 2008-08-11
 */

#ifndef _SYM_THREAD_H_
#define _SYM_THREAD_H_

#include <e32svr.h>
#include <e32uid.h>

#include "Symbian2Win32.h"
#include "MPThread.h"
//#include "lock_symbian.h"

#if defined __cplusplus
extern "C" {
#endif

/*
 * TEventHandler: MPThread용 Event handler
 * Event: stop/sleep/wakeup
 * 생성자에서는 window api인 CreateEvent와 동일한 역할을 함. 
 *  (여기서는 Manual Reset:true, initial state: false와 동일하게 설정됨)
 */

enum { 
	EVENT_STOP=0,
	EVENT_SLEEP,
	EVENT_WAKEUP,
	EVENT_USER,
	EVENT_NONE,
	EVENT_TIMEOUT,
	EVENT_ERROR
};

enum {
	UNSIGNALED=0,
	SIGNALED,
};

class TEventHandler
{
public:
	TEventHandler()//BOOL fInitialState = TRUE)
	{
		m_csec.CreateLocal();
		m_counter = 1; 	
		m_semaphore.CreateLocal(1);
		m_semaphore_count=1;
		
		m_bEventState[EVENT_STOP]=UNSIGNALED;
		m_bEventState[EVENT_SLEEP]=UNSIGNALED;
		m_bEventState[EVENT_WAKEUP]=UNSIGNALED;
		m_bEventState[EVENT_USER]=UNSIGNALED;
		
	}
	~TEventHandler()
	{
		m_csec.Close();
		m_semaphore.Close();
	}
	
	//return TRUE	: 해당 event를 signal상태로 변경함.
	//		 FALSE	: SetEvent 불가상태		 
	BOOL SetEvent(DWORD aSignaledEvent)
	{
		m_csec.Wait();
		if (m_counter == 1) {
			m_bEventState[aSignaledEvent]=SIGNALED;
			m_csec.Signal();
			return FALSE;
		}
		
		m_bEventState[aSignaledEvent]=SIGNALED;
		m_counter = 1;
		m_semaphore.Signal();
		m_semaphore_count++;
		m_csec.Signal();
		return TRUE;
	}
	
	//return TRUE	: 해당 event를 unsignal상태로 변경함.
	//		 FALSE	: ResetEvent 불가상태
	BOOL ResetEvent(DWORD aUnSignaledEvent)
	{
		m_csec.Wait();
		if (m_counter == 0 || m_semaphore_count==0) { 
			m_csec.Signal();
			m_bEventState[aUnSignaledEvent]=UNSIGNALED;
			return FALSE;
		}
			
		m_bEventState[aUnSignaledEvent]=UNSIGNALED;
		m_counter = 0;
		m_semaphore_count--;
		m_semaphore.Wait();
		m_csec.Signal();
		return TRUE;
	}
	
	//return type
	//TRUE:  대기중인 event가 signaled 상태로 변경되었음.
	//FALSE: semaphore wait중 error발생 혹은 timeout까지 해당 이벤트 signal되지 않은 경우.
	BOOL WaitForSingleEvent(DWORD aWaitingEvent, DWORD dwTimeOut = INFINITE)
	{	
		m_csec.Wait();
		
		TUint32 uStartCount=0;//시간계산용 start tick count
		TUint32 uEndCount=0;//시간계산용 start tick count
		TUint32 uTime=0;//경과시간
		TUint32 uWaitTime;
		
		if(dwTimeOut == INFINITE)
			uWaitTime=INFINITE;
		else
			uWaitTime=dwTimeOut*1000;//micro seconds
		
		m_csec.Signal();
		
		if(uWaitTime==0){
			if(m_bEventState[aWaitingEvent]==SIGNALED)
				return TRUE;
			else
				return FALSE;
		}
		
		if(m_bEventState[aWaitingEvent]==SIGNALED){
			return TRUE;
		}
		else{
			if (uWaitTime == INFINITE) {
				while(m_bEventState[aWaitingEvent]==UNSIGNALED){
				   m_semaphore.Wait();
				   m_semaphore.Signal();
				   m_semaphore_count++;
				}
				
				return TRUE;
			}
			else{
				while(m_bEventState[aWaitingEvent]==UNSIGNALED){
					uStartCount=User::TickCount();
						m_semaphore_count--;
					TInt res = m_semaphore.Wait((TInt)uWaitTime);
					if(res!=KErrTimedOut)
						m_semaphore.Signal();
					m_semaphore_count++;
					uEndCount=User::TickCount();
					uTime=(uEndCount-uStartCount)*15625;// 1000*1000/64;//micro seconds
					uWaitTime=uWaitTime-uTime;			
					if (res != KErrNone)// || res==KErrTimedOut)
						return FALSE;
				}
				return TRUE;
			}
		}
	}
	
	//return type
	//EVENT_XXXX(stop,sleep,wakeup):signaled event
	//EVENT_ERROR: timeout or error
	DWORD WaitForMultiEvent(DWORD dwTimeOut = INFINITE)//msec
	{	
		m_csec.Wait();
		DWORD ret;    
        m_csec.Signal();
        
        if(dwTimeOut==0){
        	ret=CheckSignaledEvent();
        	if(ret<=EVENT_USER)
        		return ret;
        	else
        		return EVENT_NONE;
        }
        
		ret=CheckSignaledEvent();
		if(ret<=EVENT_USER)
			return ret;
		else{
			if (dwTimeOut == INFINITE){
				m_semaphore.Wait();
				m_semaphore.Signal();
				m_semaphore_count++;
				ret=CheckSignaledEvent();
				if(ret<=EVENT_USER)
					return ret;
			}
			else{
				m_semaphore_count--;
				TInt res = m_semaphore.Wait((TInt)dwTimeOut * 1000);
				if(res!=KErrTimedOut)
				{
					m_semaphore.Signal();
					m_semaphore_count++;		
					if(res==KErrNone)
					{
						ret=CheckSignaledEvent();
						if(ret<=EVENT_USER)
							return ret;
					}
					else
						return EVENT_ERROR;
				}
				else
					return EVENT_NONE;
			}
		}
		return EVENT_NONE;
	}
	
public:
	bool 					m_bEventState[3];//0:stop,1:sleep,wakeup
protected:	
	RCriticalSection m_csec;
	RSemaphore		 m_semaphore;	
	TInt32			 m_counter;
	TInt32			 m_semaphore_count;
private:
	DWORD CheckSignaledEvent(){
		for(int i=EVENT_STOP; i<=EVENT_USER;i++){
		if(m_bEventState[i]==SIGNALED)
			return i;
		}
		return EVENT_NONE;
	}
};

class CSymThread : public MPThread
	{
	public:
		LPFN_MPTHREAD_CALLBACK 	m_lpCallBack;
		void* 					m_pUserData;
	private:
		const TCHAR* 			m_pThread_name;
		TThreadId				m_thread_id;
		RThread					m_rThread;
		TRequestStatus			m_Status;		
		bool 					m_bStartSleep;
		DWORD					m_dwThreadStat;	
		TEventHandler			m_event;
		DWORD					m_dwRet_WaitForControlEvent;
			
	public:
		CSymThread(const TCHAR* strThreadName);
		virtual ~CSymThread();

		DWORD Start(void* lpThreadAttributes,
					DWORD dwStackSize,
					LPFN_MPTHREAD_CALLBACK lpCallBack, 
   					LPVOID lpParameter = 0,
   					DWORD dwCreationFlags = 0, 
   					int bStartSleep = false);
		
		DWORD isStop();//현재 thread가 stop상태인지 확인하는 함수.
		DWORD isSleep();//not used in beagle src
		DWORD isWakeup();//not used in beagle src
		/////////////////////////////////////////////////////////
		//WaitForWantStop) Return Values
		//WAIT_FAILED			: when thread is not running
		//WAIT_TIMEOUT			: timeouted
		//otherwise				: stop signal activated
		DWORD WaitForStop(DWORD dwTimeOut=INFINITE);//stop event 대기함수
		DWORD WaitForSleep(DWORD dwTimeOut=INFINITE);//sleep event 대기함수
		DWORD WaitForWakeup(DWORD dwTimeOut=INFINITE);//wakeup event 대기함수
		//DWORD WaitForWantSleep(DWORD dwTimeOut=INFINITE);
		//DWORD WaitForWantStop(DWORD dwTimeOut=INFINITE);
   			
		DWORD Stop(DWORD dwWaitTimeOut=INFINITE);//change 'stop event' to 'singaled' state
		DWORD Sleep(DWORD dwWaitTimeOut=INFINITE);//change 'sleep event' to 'singaled' state
		DWORD Wakeup(DWORD dwWaitTimeOut=INFINITE);//change 'wakeup event' to 'singaled' state
		DWORD WaitForControlEvent(DWORD dwWaitTimeOut=INFINITE, int bWaitSleep=true, int bWaitStop=true, int bWaitUser=true);//wait for multiple event(sleep, stop,wakeup)
		
		DWORD IsControlEvent_Stop() { return (m_dwRet_WaitForControlEvent==EVENT_STOP); }
		DWORD IsControlEvent_Sleep() { return (m_dwRet_WaitForControlEvent==EVENT_SLEEP); }
		DWORD IsControlEvent_Wakeup() { return (m_dwRet_WaitForControlEvent==EVENT_WAKEUP); }
		DWORD IsControlEvent_User() { return (m_dwRet_WaitForControlEvent==EVENT_USER); }
		
		DWORD ResetEvent(DWORD aEvent){	return m_event.ResetEvent(aEvent);}
		
	private:
		static TInt _procThread( void* pVoid );
		DWORD IsControlEvent_Exist() { return (EVENT_STOP<=m_dwRet_WaitForControlEvent && m_dwRet_WaitForControlEvent<=EVENT_USER); }


		};
#if defined __cplusplus
}
#endif

class CActiveController : public CActive
{
public:
	CSymThread* m_curThread;
	CActiveController(CSymThread* aSymThread) :
	 CActive(EPriorityStandard)
	{
		m_curThread=aSymThread;
		iStatus=KRequestPending;
		CActiveScheduler::Add(this);
		SetActive();
	}
	~CActiveController(){};
	void Start();
	
private:
	void DoCancel(){};
	void RunL();
};

#endif
