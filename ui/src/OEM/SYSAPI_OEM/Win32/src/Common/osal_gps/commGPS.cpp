#include "inc/define.h"
#include "commGPS.h"
//#include "gps/commLib.h"
#include "gps/gpsinterface.h"
#include "gps/gpsemul.h"
#include "base/threadinterface.h"

#if defined(_AFX) && defined(_DEBUG)
# define new DEBUG_NEW
# undef THIS_FILE
  static char THIS_FILE[] = __FILE__;
#endif

DWORD readCommGPSProc (LPVOID pThread, LPVOID lpParameter);

CCommGPS::CCommGPS ( void )
{
	memset ( &m_CommInfo,	0x00, sizeof(m_CommInfo) ) ;
	memset ( m_szBuff,		0x00, sizeof(m_szBuff) ) ;

	m_fGPSLogTmrOn	= FALSE ;
	m_bufCount = 0;

#if USE_RECEIVE_COUNT
	m_nReceiveCount = 0;
#endif

#if defined (WIN_PC)
	m_hReadGpsLog = INVALID_HANDLE_VALUE ;
#endif

	PORT(m_CommInfo)           = GPS_PORT ;
	BAUD(m_CommInfo)           = GPS_BAUD ;

	m_validCount = 0;

#if PC_DBG
	m_bGPSMsgReceived = FALSE;
#endif
}

CCommGPS::~CCommGPS ( void )
{
	closeComm();
}

BOOL CCommGPS::parse(const char* _msg, UINT _nLen)
{
	return TRUE;
}

BOOL CCommGPS::process(const char* szData, UINT len)
{
	if(!szData || len < 1) return FALSE;

	//__LOG(DZ_TEMP, "LOGENABLE %d, HANDEL=%X", m_bLogEnable,m_hLog);
	if(m_bLogEnable && m_hLog)
	{
		//! Save GPS Log data
		logData( (char*)szData, len) ;
	}

	UINT idx =0;

#if !SHIPBUILD && 0
	char buff[512];

	int lll = len;

	if(len > 510) len = 510;
	memcpy(buff, szData, lll);

	buff[lll-1] = '\0';

	__LOG(DZ_TEMP, "NMEA: %s" ,buff);
#endif

	if(m_pParser)
	{
		while(idx < len){

			if(*szData ==  ASCII_CR || *szData == ASCII_LF){
				//! to-do checksum()
				if(m_bufCount > 20){
					m_pParser(m_szBuff, m_bufCount, m_lpParserParameter);
				}
				m_bufCount = 0;
			}
			else{
				if(*szData == ASCII_DOLLOR) m_bufCount = 0;
				m_szBuff[m_bufCount++] = *szData;
				if(m_bufCount >= sizeof(m_szBuff)){
					//! 일반적으로 NMEA데이타의 길이는 80자 정도이므로 
					//! 버퍼의 크기는 이보다는 커야한다.

					//! 이런 경우가 발생한다면 이것은 파싱오류또는 데이타 자체의 오류
					m_bufCount = 0;
				}
			}
			szData++;
			idx++;
		}
	}

	return TRUE ;
}

BOOL CCommGPS::openComm(UINT port, UINT baudRate)
{
	if (!CComm::openComm(port, baudRate)) return FALSE;

	// start threads and set initial thread state to not done
	if(!THREAD_Start(THREAD_INDEX_GPS, readCommGPSProc, this))
	{
		__LOG(DZ_TEMP, "readCommGPSProc createThreadMM fail!");
		return FALSE;
	}
	
	return TRUE;
}

BOOL CCommGPS::closeComm ()
{
#if OVERLAPPED_MODE
	SetEvent(m_stOverlap.hEvent);
#endif

	THREAD_Destroy(THREAD_INDEX_GPS);

	CComm::closeComm();

    return TRUE;
}

#if USE_RECEIVE_COUNT
void CCommGPS::SetReceiveCount(long nReceiveCount)
{
	m_nReceiveCount = nReceiveCount > 20000 ? 0 : nReceiveCount;
}

long CCommGPS::GetReceiveCount()
{
	return m_nReceiveCount;
}
#endif

DWORD readCommGPSProc (LPVOID pThread, LPVOID lpParameter)
{
	ASSERT(lpParameter);

	CCommGPS* pComm = (CCommGPS*)lpParameter;

	DWORD	dwMask, nRead ;
	DWORD	nLine = 0 ;
	BOOL	ret ;

	char	buff[MAX_READ_BUFFER + 10] ;
	char	line[MAX_READ_BUFFER + 10] ;

	memset ( line, 0x00, sizeof(line) ) ;

#if USE_RECEIVE_COUNT
	pComm->SetReceiveCount(0);
#endif

	EVTMASKNEW ( pComm->m_CommInfo) = EV_RXCHAR | EV_ERR | EV_BREAK ;
	//EVTMASKNEW ( pComm->m_CommInfo) = EV_RXCHAR | EV_ERR | EV_BREAK | EV_CTS | EV_DSR | EV_RLSD | EV_RXFLAG | EV_TXEMPTY ;

	// 기존의 Event Mask 를 저장 ...
	GetCommMask ( COMDEV(pComm->m_CommInfo), &(EVTMASKORIG(pComm->m_CommInfo)) ) ;

	SetCommMask ( COMDEV(pComm->m_CommInfo), EVTMASKNEW(pComm->m_CommInfo) ) ;

	__B_TRY

	while (1)
	{
		//아랫쪽의 Sleep(10)역활을 동시에 하면서 반응속도를 올린다. bruce
//		if (THREAD_WaitForStop(THREAD_INDEX_GPS))
		if (THREAD_IsStop(THREAD_INDEX_GPS))
		{
		#if USE_RECEIVE_COUNT
			pComm->SetReceiveCount(0);
		#endif
			break;
		}
		
        // check-me
        // PC버젼에서 OVERLAPPED모드로 정상작으로 작동되지 않음(FALSE반환)
        // 일단 NON OVERLAPPED모드로 동작하도록 변경함.
        // TEST요망
	#if OVERLAPPED_MODE
		ASSERT(pComm->m_stOverlap.hEvent);
		if (WaitCommEvent(COMDEV(pComm->m_CommInfo), &dwMask, &pComm->m_stOverlap))
	#else
		if (WaitCommEvent(COMDEV(pComm->m_CommInfo), &dwMask, NULL))
	#endif
		{
			if ((dwMask& EV_RXCHAR) == EV_RXCHAR)
			{
				ret = ReadFile ( COMDEV(pComm->m_CommInfo), buff, MAX_READ_BUFFER, &nRead, NULL ) ;
			#if GPS_SIMUL_ENABLE
				if (!GPS_IsLogOpend() && !GPS_IsSuspend())
			#endif
				if ( ret )
				{
					pComm->process(buff, nRead);
				#if USE_RECEIVE_COUNT
					pComm->SetReceiveCount(pComm->GetReceiveCount()+1);
				#endif
				}
			}
			else
			{
				switch ( dwMask )
				{
				//case EV_RXCHAR :	// A character was received and placed in the input buffer.
				//	break ;

				case EV_ERR :		// A line-status error occurred. Line-status errors are CE_FRAME, CE_OVERRUN, and CE_RXPARITY.
					{
						DWORD	error ;
						COMSTAT	stat ;
						ClearCommError ( COMDEV(pComm->m_CommInfo), &error, &stat ) ;
					}
					break ;

				case EV_BREAK :		// A break was detected on input.
					ClearCommBreak ( COMDEV(pComm->m_CommInfo) ) ;
					break ;

				//case EV_CTS :
				//case EV_DSR :
				//case EV_RLSD :
				//case EV_RXFLAG :
				//case EV_TXEMPTY :
				//default :
				//	break ;
				}
			}

		}//if (WaitCommEvent(COMDEV(pComm->m_CommInfo), &dwMask, NULL))

//		SYSAPI_Sleep(100);
	} ; // end of while 

	__B_EXCEPT(_T("readCommGPSProc"))
	
	SetCommMask ( COMDEV(pComm->m_CommInfo), EVTMASKORIG(pComm->m_CommInfo) ) ;

	return TRUE ;
}

