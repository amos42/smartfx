#include "MPThread.h"
#include "SymThread.h"


CSymThread::CSymThread(const TCHAR* strThreadName)
{
	m_pThread_name=strThreadName;
	m_Status=KRequestPending;
	
	m_bStartSleep=FALSE;
	m_pUserData=NULL;
	m_lpCallBack=NULL;
	m_dwThreadStat=MPTHREADSTAT_STOPPED;
			
}

CSymThread::~CSymThread()
{
	Stop();
	m_rThread.Close();
}

DWORD CSymThread::Start(void* 	lpThreadAttributes,//not used in Symbian OS
   						DWORD dwStackSize, 
   						LPFN_MPTHREAD_CALLBACK lpCallBack, 
   						LPVOID lpParameter, 
   						DWORD dwCreationFlags, //not used in Symbian OS
   						int bStartSleep
   						)
{
	if(!lpCallBack || !isStop())
		return FALSE;

	TInt result;
	
	m_lpCallBack=lpCallBack;
	m_pUserData=lpParameter;
	
	TPtrC ThreadName((TUint16*)m_pThread_name);
	
	m_event.ResetEvent(EVENT_STOP);
	m_event.ResetEvent(EVENT_SLEEP);
	m_event.ResetEvent(EVENT_WAKEUP);
	
	result=m_rThread.Create(ThreadName,_procThread,(TInt)dwStackSize , NULL, this, EOwnerProcess);
	if(result==KErrNone)
		{
		m_thread_id=m_rThread.Id();
		m_dwThreadStat= MPTHREADSTAT_RUNNING;
		//m_rThread.Logon(m_Status);//Thread����� event ������.//stop()���� �̵� 
		m_rThread.Resume();// Thread������ �ٷ� �����ϵ��� ����.
		return TRUE;
		}
	else
		return FALSE;
}

TInt CSymThread::_procThread(TAny* pData)
	{
		CTrapCleanup* cleanup = CTrapCleanup::New();
		CActiveScheduler *active_scheduler=new CActiveScheduler();
		CActiveScheduler::Install(active_scheduler);
		
		CSymThread* This=(CSymThread*)pData;
	    CActiveController* ActiveCon= new(ELeave) CActiveController((CSymThread*)pData);
	    
		if(!This->m_bStartSleep)
			{
	    	ActiveCon->Start();
	    	CActiveScheduler::Start();
			}
		
		This->m_dwThreadStat=MPTHREADSTAT_STOPPED;
		
		delete ActiveCon;
		delete active_scheduler;
		delete cleanup;
		
		return TRUE;
	}

DWORD CSymThread::WaitForControlEvent(DWORD dwWaitTimeOut,int bWaitSleep, int bWaitStop, int bWaitUser)
	{//int bWaitSleep, int bWaitStop, int bWaitUser->������, octhread�� ���߷��� ����
		
		m_dwRet_WaitForControlEvent = m_event.WaitForMultiEvent(dwWaitTimeOut);
		return IsControlEvent_Exist();
	}

DWORD CSymThread::Stop(DWORD dwWaitTimeOut)
	{
		m_event.SetEvent(EVENT_STOP);
		
		if(m_dwThreadStat!=MPTHREADSTAT_STOPPED)
		{
			m_rThread.Logon(m_Status);//Thread����� event ������.
			User::WaitForRequest(m_Status);
			return TRUE;
		}
		return FALSE;
	}

DWORD CSymThread::Sleep(DWORD dwWaitTimeOut)//dwWaitTimeOut�� ������ ����. ��۰� ���߱�����
	{
		return m_event.SetEvent(EVENT_SLEEP);
	}

DWORD CSymThread::Wakeup(DWORD dwWaitTimeOut)//dwWaitTimeOut�� ������ ����. ��۰� ���߱�����
	{
		return m_event.SetEvent(EVENT_WAKEUP);
	}

DWORD CSymThread::WaitForStop(DWORD dwTimeOut)
	{
		if(m_dwThreadStat!=MPTHREADSTAT_STOPPED)
			return m_event.WaitForSingleEvent(EVENT_STOP,dwTimeOut);
		else
			return FALSE;
	}

DWORD CSymThread::WaitForSleep(DWORD dwTimeOut)//beagle src���� ������ ����.
	{
		return m_event.WaitForSingleEvent(EVENT_SLEEP,dwTimeOut);
	}
DWORD CSymThread::WaitForWakeup(DWORD dwTimeOut)
	{
		return m_event.WaitForSingleEvent(EVENT_WAKEUP,dwTimeOut);
	}

DWORD CSymThread::isStop()
	{
		return (m_dwThreadStat==MPTHREADSTAT_STOPPED);
	}

DWORD CSymThread::isSleep()
	{
		return WaitForSleep(0);
	}
DWORD CSymThread::isWakeup()
	{
		return WaitForWakeup(0);
	}


void CActiveController::Start()
{
	TRequestStatus* request_status=&iStatus;
	User::RequestComplete(request_status,KErrNone);
}

void CActiveController::RunL()
	{
		m_curThread->m_lpCallBack(m_curThread,m_curThread->m_pUserData);
		CActiveScheduler::Stop();
	}
