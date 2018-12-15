#include "inc/define.h"
//#include "commLib.h"
#include "commTMC.h"

#if defined(_AFX) && defined(_DEBUG)
# define new DEBUG_NEW
# undef THIS_FILE
  static char THIS_FILE[] = __FILE__;
#endif

DWORD readCommTMCProc(LPVOID pThread, LPVOID lpThreadParameter);

CCommTMC::CCommTMC ( void )
{
}

CCommTMC::~CCommTMC ( void )
{
}

BOOL CCommTMC::parse(const char* _msg, UINT _nLen)
{
	return TRUE;
}

BOOL CCommTMC::process(const char* szData, UINT len)
{
	if(m_bLogEnable && m_hLog && !THREAD_IsStop(THREAD_INDEX_TMC))
	{
		// Save Log data
		logData( szData, len) ;
	}

	if(len < 1) return FALSE;

	UINT idx =0;

#if !SHIPBUILD && 0
	char buff[512];

	int lll = len;

	if(len > 510) len = 510;
	memcpy(buff, szData, lll);

	buff[lll-1] = '\0';

	__LOG(DZ_TEMP, "RDS: %s" ,buff);
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

BOOL CCommTMC::openComm(UINT port, UINT baudRate)
{
	if (!CComm::openComm(port, baudRate)) return FALSE;

	// start threads and set initial thread state to not done
	if(!THREAD_Start(THREAD_INDEX_TMC, readCommTMCProc, this))
	{
		__LOG(DZ_TEMP, "readCommTMCProc createThreadMM fail!");
		return FALSE;
	}

	return TRUE;
}

BOOL CCommTMC::closeComm ()
{
    if (!CONNECTED(m_CommInfo)) return TRUE ;

#if OVERLAPPED_MODE
	SetEvent(m_stOverlap.hEvent);
	THREAD_Destroy(THREAD_INDEX_TMC);
#endif

	CComm::closeComm();

#if !OVERLAPPED_MODE
	THREAD_Destroy(THREAD_INDEX_TMC);
#endif

    return TRUE;
}

#if 0
BOOL  CCommTMC::parseTTR( char* sMsg, DWORD nLen)
{
	char buff[20];
	m_token.Reset();
	int tok_count = m_token.Splite(sMsg);

	if(tok_count < 6)
	{
		return FALSE;
	}

	memset(buff, 0, sizeof(buff));

	int error = atoi(m_token.Get(1));
	WORD blockA = hex2dec(m_token.Get(2));
	WORD blockB = hex2dec(m_token.Get(3));
	WORD blockC = hex2dec(m_token.Get(4));
	getField( m_token.Get(5), 0, buff);
	WORD blockD = hex2dec(buff);


	BYTE status = 0;

	status |=  SHL( RDS_BT_CTYPE, 2);

	m_rds.Process(blockA, blockB, blockC, blockD);
	
	/**
	m_rds.Process( status, HIBYTE(blockA), LOBYTE(blockA));
	m_rds.Process( status, HIBYTE(blockB), LOBYTE(blockB));
	m_rds.Process( status, HIBYTE(blockC), LOBYTE(blockC));
	m_rds.Process( status, HIBYTE(blockD), LOBYTE(blockD));
	**/


	return TRUE;
}
#endif


DWORD readCommTMCProc(LPVOID pThread, LPVOID lpThreadParameter)
{
	ASSERT(lpThreadParameter);

	CCommTMC * pComm = (CCommTMC *)lpThreadParameter;

	DWORD	dwMask, nRead ;
	DWORD	nLine = 0 ;
	BOOL	ret ;

	char	buff[MAX_READ_BUFFER + 12] ;
	char	line[MAX_READ_BUFFER + 12] ;

	memset ( line, 0x00, sizeof(line) ) ;


	EVTMASKNEW ( pComm->m_CommInfo) = EV_RXCHAR | EV_ERR | EV_BREAK ;
	//EVTMASKNEW ( pGPS->m_CommInfo) = EV_RXCHAR | EV_ERR | EV_BREAK | EV_CTS | EV_DSR | EV_RLSD | EV_RXFLAG | EV_TXEMPTY ;

	// 기존의 Event Mask 를 저장 ...
	GetCommMask ( COMDEV(pComm->m_CommInfo), &(EVTMASKORIG(pComm->m_CommInfo)) ) ;

	SetCommMask ( COMDEV(pComm->m_CommInfo), EVTMASKNEW(pComm->m_CommInfo) ) ;

	while (1)
	{
//		if (THREAD_WaitForStop(THREAD_INDEX_TMC)) break;
		if (THREAD_IsStop(THREAD_INDEX_TMC)) break;

#if OVERLAPPED_MODE
		if ( WaitCommEvent(COMDEV(pComm->m_CommInfo), &dwMask, &pComm->m_stOverlap) )
#else
		if ( WaitCommEvent(COMDEV(pComm->m_CommInfo), &dwMask, NULL) )
#endif

//		if ( WaitCommEvent(COMDEV(pComm->m_CommInfo), &dwMask, NULL) )
		{
			switch ( dwMask )
			{
			case EV_RXCHAR :	// A character was received and placed in the input buffer.
				{			
					ret = ReadFile ( COMDEV(pComm->m_CommInfo), buff, MAX_READ_BUFFER, &nRead, NULL ) ;

					if ( ret )
						pComm->process(buff, nRead) ;

					SYSAPI_Sleep(5);
				}
				break ;

			case EV_ERR :		// A line-status error occurred. Line-status errors are CE_FRAME, CE_OVERRUN, and CE_RXPARITY.
				{
					DWORD	error ;
					COMSTAT	stat ;
					ClearCommError ( COMDEV(pComm->m_CommInfo), &error, &stat ) ;
				}
				break ;

			case EV_BREAK :		// A break was detected on input.
				{
					ClearCommBreak ( COMDEV(pComm->m_CommInfo) ) ;
				}
				break ;

			case EV_CTS :
				{
				}
				break ;

			case EV_DSR :
				{
				}
				break ;

			case EV_RLSD :
				{
				}
				break ;

			case EV_RXFLAG :
				{
				}
				break ;

			case EV_TXEMPTY :
				{
				}
				break ;

			default :
				{
				}
				break ;
			}
		}
		else
		{	
			// error handling ..
		}
		
		SYSAPI_Sleep(0);
	} ; // end of while 
	
	SetCommMask ( COMDEV(pComm->m_CommInfo), EVTMASKORIG(pComm->m_CommInfo) ) ;

	return TRUE ;
}

