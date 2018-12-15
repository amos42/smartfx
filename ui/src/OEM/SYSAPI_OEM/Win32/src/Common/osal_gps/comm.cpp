//#include "gps/commLib.h"
#include "comm.h"
#include "gps/gpsinterface.h"

#if defined(_AFX) && defined(_DEBUG)
# define new DEBUG_NEW
# undef THIS_FILE
  static char THIS_FILE[] = __FILE__;
#endif

CComm::CComm ( void )
{
	memset ( &m_CommInfo,	0x00, sizeof(m_CommInfo) ) ;
	memset ( m_szBuff,		0x00, sizeof(m_szBuff) ) ;

	m_fGPSLogTmrOn	= FALSE ;

	m_bufCount = 0;

#if OVERLAPPED_MODE
	ZeroMemory(&m_stOverlap, sizeof(m_stOverlap));
#else

#endif // #if WIN_PC

	InitializeCriticalSection ( &m_cs ) ;

#if COMM_LOG
	m_hLog       = INVALID_HANDLE_VALUE ;
	m_pLogBuf    = NULL;
	m_nLogBuf    = 0;
	m_bLogEnable = FALSE;
#endif

	PORT(m_CommInfo)           = GPS_PORT ;
	BAUD(m_CommInfo)           = GPS_BAUD ;

	m_validCount = 0;

	m_bLogEnable = FALSE;

	m_pParser           = NULL;
	m_lpParserParameter = NULL;

	// Coverity Errors :: #804문제. 초기화.
#if COMM_LOG
	m_bLoging = FALSE;
#endif
	m_hReadGpsLog = INVALID_HANDLE_VALUE ;
}

CComm::~CComm ( void )
{
	if ( CONNECTED(m_CommInfo) )
	{
		if ( !closeComm() )
		{
			// error handling..
			//MessageBox ( g_hwndMain, _T("GPS Close Error.."), _T("Error"), MB_OK ) ;
		}
	}

	DeleteCriticalSection ( &m_cs ) ;
}

void CComm::setParser(PCOMM_PARSE_CALLBACK lpParser, LPVOID lpParam)
{
	m_pParser           = lpParser;
	m_lpParserParameter = lpParam;
}

/*-----------------------------------------------------------------------------

FUNCTION: SetupCommPort( void )

PURPOSE: Setup Communication Port with our settings

RETURN: 
    Handle of comm port is successful
    NULL is error occurs

HISTORY:   Date:      Author:     Comment:
           10/27/95   AllenD      Wrote it

-----------------------------------------------------------------------------*/
BOOL CComm::openComm (UINT port, UINT baudRate)
{
	ASSERT(port && baudRate);

	PORT(m_CommInfo)     = port;
	BAUD(m_CommInfo)     = baudRate;
	CONNECTED(m_CommInfo)= FALSE;
	
	if ( (COMDEV(m_CommInfo) != INVALID_HANDLE_VALUE) && (COMDEV(m_CommInfo) != NULL) )
	{
		return FALSE;
	}

	// get tty settings from settings dialog
	if ( !initCommInfo() )
	{
		// error handling ...
	}

	// open communication port handle
	TCHAR  szPort[32] ;

	_stprintf ( szPort, _T("COM%d:"), PORT(m_CommInfo) ) ;	// WinCE에서는 포트명 뒤에 : 을 붙인다.

#if OVERLAPPED_MODE
	COMDEV(m_CommInfo) = CreateFile ( szPort,  
									GENERIC_READ | GENERIC_WRITE, // GENERIC_READ,
									0, 
									NULL, 
									OPEN_EXISTING,
									FILE_FLAG_OVERLAPPED,	// Overlapped IO
									NULL ) ;

#else
	COMDEV(m_CommInfo) = CreateFile ( szPort,  
									GENERIC_READ | GENERIC_WRITE, // GENERIC_READ,
									0, 
									NULL, 
									OPEN_EXISTING,
									FILE_ATTRIBUTE_NORMAL,	// CE 환경에서는 Overlapped 를 지원하지 않는다..
									NULL ) ;
#endif

	if ( COMDEV(m_CommInfo) == INVALID_HANDLE_VALUE ) 
	{   
#ifdef _DEBUG
		// error handling ..
		TCHAR ss[100];
		_stprintf(ss, _T("GPS OPEN ERROR \nPORT=%d, BAUD=%d"), PORT(m_CommInfo), BAUD(m_CommInfo));
#endif

		return FALSE;
	}

	// Save original comm timeouts and set new ones
	if ( !GetCommTimeouts(COMDEV(m_CommInfo), &(TIMEOUTSORIG(m_CommInfo))) )
	{
		if ( COMDEV(m_CommInfo) != INVALID_HANDLE_VALUE )
		{
			CloseHandle ( COMDEV(m_CommInfo) ) ;
			COMDEV(m_CommInfo) = INVALID_HANDLE_VALUE ;
		}

		//MessageBox ( ghwndMain, _T("GetCommTimeouts() Fail"), _T("GetCommTimeouts"), MB_OK ) ;

		return FALSE;
	}

	// DCB Setting ..... (Set port state)
	if ( !setDCB() )
	{
		//MessageBox ( ghwndMain, _T("setDCB() Fail"), _T("setDCB"), MB_OK ) ;
	}

	// Timeouts Setting ..
	if ( !setTimeOuts () )
	{
		//MessageBox ( ghwndMain, _T("setTimeOuts() Fail"), _T("setTimeOuts"), MB_OK ) ;
	}

#if OVERLAPPED_MODE
   m_stOverlap.hEvent = CreateEvent(
        NULL,   // default security attributes 
        TRUE,   // manual-reset event 
        FALSE,  // not signaled 
        NULL    // no name
		);
    
    // Initialize the rest of the OVERLAPPED structure to zero.
    m_stOverlap.Internal = 0;
    m_stOverlap.InternalHigh = 0;
    m_stOverlap.Offset = 0;
    m_stOverlap.OffsetHigh = 0;
#endif // #if WIN_PC

	// set comm buffer sizes
	SetupComm ( COMDEV(m_CommInfo), MAX_READ_BUFFER, MAX_WRITE_BUFFER ) ;

	// set overall connect flag
	CONNECTED(m_CommInfo) = TRUE ;

	//__LOG(DZ_TEMP, "COMM OPEN OK");

	return TRUE;
}

/*
 *	COMM 로깅 파일 생성.
 *  
 */
BOOL CComm::createLogFile(const TCHAR* filename)
{
	ASSERT(m_hLog == INVALID_HANDLE_VALUE);

	m_hLog = CreateFile  ( filename,  
							GENERIC_WRITE, 
							FILE_SHARE_READ,
							NULL,			// secure mode : WinCE와의 호환성을 위해 0 으로 맞춰둔다.
							CREATE_ALWAYS,  // 이미 파일이 있으면 덮어쓴다..
							FILE_ATTRIBUTE_NORMAL,// | FILE_FLAG_OVERLAPPED,
							0 ) ;

	ASSERT(m_hLog != INVALID_HANDLE_VALUE);
	if(m_hLog != INVALID_HANDLE_VALUE)
	{
		if( !m_pLogBuf ) m_pLogBuf = new BYTE[MAX_LOGBUF];

		ASSERT(m_pLogBuf);
		if(!m_pLogBuf)
		{
			CloseHandle(m_hLog);
			m_hLog = INVALID_HANDLE_VALUE;
			return FALSE;
		}
		m_nLogBuf = 0;

		m_bLogEnable = TRUE;
		return TRUE;
	}

	return FALSE;
}

void CComm::shift (int _iMode, int _iSec)
{
	if ( m_hReadGpsLog != INVALID_HANDLE_VALUE ) 
	{
		if (_iMode == 1) // forword
		{
			long lSizeCur = SetFilePointer(m_hReadGpsLog, 0, NULL, FILE_CURRENT);
			lSizeCur += (200*_iSec);
			SetFilePointer (m_hReadGpsLog, lSizeCur, NULL, FILE_BEGIN );
		}
		else
		{
			long lSizeCur = SetFilePointer(m_hReadGpsLog, 0, NULL, FILE_CURRENT);
			lSizeCur -= (200*_iSec);
			if (lSizeCur<0)
				lSizeCur = 0;
			SetFilePointer (m_hReadGpsLog, lSizeCur, NULL, FILE_BEGIN );
		}
	}

}


#if COMM_LOG
#if 0
HANDLE CComm::openGpsLog ( TCHAR * _fileName )
{
	if (GPS_IsGPSFromFile())
	{
		if ( !_fileName )
			return INVALID_HANDLE_VALUE ;

		// terminate thread 
		if ( READSTATTHREAD(m_CommInfo) )
		{
			 TerminateThread ( READSTATTHREAD(m_CommInfo), 0x15 ) ;
			 CloseHandle ( READSTATTHREAD(m_CommInfo) ) ;

			 READSTATTHREAD(m_CommInfo) = INVALID_HANDLE_VALUE ;
		}

		// close handle
		closeComm () ;


		if ( m_hReadGpsLog != INVALID_HANDLE_VALUE )
			CloseHandle ( m_hReadGpsLog ) ;

		m_hReadGpsLog = INVALID_HANDLE_VALUE ;


		m_hReadGpsLog = CreateFile  ( _fileName,  
									GENERIC_READ, 
									FILE_SHARE_READ,
									NULL,			// secure mode : WinCE와의 호환성을 위해 0 으로 맞춰둔다.
									OPEN_EXISTING,  // 이미 파일이 있으면 덮어쓴다..
									FILE_ATTRIBUTE_NORMAL,// | FILE_FLAG_OVERLAPPED,
									0 ) ;


		// CFrame의 "IDM_OPEN_COMM_LOG"로 옮김 
	//	if ( m_hReadGpsLog != INVALID_HANDLE_VALUE )
	//	{
	//		SetTimer ( GD_GetFrameHwnd(), READ_COMM_LOG_TIMER, READ_COMM_LOG_INTERVAL, WndTimer ) ;
	//		m_fGPSLogTmrOn = TRUE ;
	//	}

		return m_hReadGpsLog ;
	}
	else
	{
		return INVALID_HANDLE_VALUE ;
	}

}
#endif
#endif


//#if COMM_LOG
//BOOL CComm::closeLog ()
//{
//	if ( m_hReadGpsLog != INVALID_HANDLE_VALUE )
//		CloseHandle ( m_hReadGpsLog ) ;
//
//	m_hReadGpsLog = INVALID_HANDLE_VALUE ;
//
//	KillTimer ( GD_GetFrameHwnd(), READ_COMM_LOG_TIMER ) ;
//
//	m_fGPSLogTmrOn = FALSE ;
//
//	return TRUE ;
//}
//#endif
//

#if 0
HANDLE CComm::openConnectTest ()
{
	if ( (COMDEV(m_CommInfo) != INVALID_HANDLE_VALUE) && (COMDEV(m_CommInfo) != NULL) )
	{
		closeComm ();
	}

	TCHAR  szPort[32] ;
	memset (szPort, 0x00, sizeof(szPort));
	_stprintf ( szPort, _T("COM%d:"), PORT(m_CommInfo) ) ;	// WinCE에서는 포트명 뒤에 : 을 붙인다.

	COMDEV(m_CommInfo) = CreateFile ( szPort,  
									GENERIC_READ | GENERIC_WRITE, // GENERIC_READ,
									0, 
									NULL, 
									OPEN_EXISTING,
									FILE_ATTRIBUTE_NORMAL,	// CE 환경에서는 Overlapped 를 지원하지 않는다..
									NULL ) ;

	TCHAR ss[100];
	memset (ss, 0x00, sizeof(ss));
	if ( COMDEV(m_CommInfo) == INVALID_HANDLE_VALUE ) 
	{   
		//_stprintf(ss, _T("GPS OPEN ERROR \nPORT=%d, BAUD=%d"), PORT(m_CommInfo), BAUD(m_CommInfo));
		//MessageBox ( GD_GetFrameHwnd(), ss, _T("GPS ERROR"), MB_OK | MB_ICONHAND) ;
		
		return COMDEV(m_CommInfo) ;
	}
	else
	{
		//_stprintf(ss, _T("GPS OPEN SUCCESS \nPORT=%d, BAUD=%d"), PORT(m_CommInfo), BAUD(m_CommInfo));
		//MessageBox ( GD_GetFrameHwnd(), ss, _T("GPS FIND OK"), MB_OK | MB_ICONHAND) ;

		closeComm ();
		return COMDEV(m_CommInfo) ;
	}

	// set overall connect flag
	//
	SYSAPI_Sleep (100);
	CONNECTED(m_CommInfo) = TRUE ;

	return COMDEV(m_CommInfo);
}

BOOL CComm::openTest ()
{
	DWORD ret = messageBox ( _T("Auto Searching..."), _T("Do you want to find a GPS Port and Baud"), MB_OKCANCEL | MB_ICONQUESTION ) ;
	if ( ret != MSG_ID_OK )
	{
		return TRUE;
	}

	closeComm ();

	int iPortAll [20] = { 1,2,3,4,5,6,7,8,9,10,11,12, -1, };
	int iBaudAll [100] = { CBR_1200, CBR_2400, CBR_4800, CBR_9600, CBR_14400,
		CBR_19200, CBR_38400, CBR_56000, CBR_57600, CBR_115200, CBR_128000, CBR_256000, -1};

	int iPortMin [20] = { 1,2,5,8,9, -1, };
	int iBaudMin [100] = { CBR_4800, CBR_9600, CBR_38400, CBR_57600, -1};
	
	for (int nn=0; iPortMin[nn]>0; nn++)
	{
		for (int kk=0; iBaudMin[kk]>0; kk++)
		{	
			setComPort ( iPortMin[nn], iBaudMin[kk] );
			HANDLE   hGPS = openConnectTest () ; // GPS Port Open 
			if ( (hGPS != NULL) && (hGPS != INVALID_HANDLE_VALUE) )
			{
				closeComm ();
				TCHAR  tpMsg[256];
				_stprintf (tpMsg, _T("Port(%d), Baud(%d)"), iPortMin[nn], iBaudMin[kk]);
				return TRUE;
			}
			SYSAPI_Sleep (100);
		}
	}

	return FALSE;
}
#endif

BOOL CComm::initCommInfo ()
{
	//memset ( &m_CommInfo, 0x00, sizeof(m_CommInfo) ) ;

	COMDEV(m_CommInfo)			= INVALID_HANDLE_VALUE ;
	READSTATTHREAD(m_CommInfo)	= INVALID_HANDLE_VALUE ;

	if(PORT(m_CommInfo) == 0)
	{
		PORT(m_CommInfo)           = GPS_PORT ;
		BAUD(m_CommInfo)           = GPS_BAUD ;
	}
	//PORT(m_CommInfo)           = GPS_PORT ;
	//BAUD(m_CommInfo)           = GPS_BAUD ;

	// Event Mask 는 readCommProc() 에서 저장함 ..

	// PC버젼의 경우에도 SYSTEM.INI변경하여 사용할 것.

//#if defined (WIN_PC)
//
//	HMENU	hMenu = GetMenu ( GD_GetFrameHwnd() ) ;
//
//	if ( (GetMenuState(hMenu, IDM_GPS_PORT_1, MF_BYCOMMAND)) & MF_CHECKED )	// check 가 되어 있으면 ..
//		PORT(m_CommInfo)  = 1 ;
//	else if ( (GetMenuState(hMenu, IDM_GPS_PORT_2, MF_BYCOMMAND)) & MF_CHECKED )	// check 가 되어 있으면 ..
//		PORT(m_CommInfo)  = 2 ;
//	else if ( (GetMenuState(hMenu, IDM_GPS_PORT_3, MF_BYCOMMAND)) & MF_CHECKED )	// check 가 되어 있으면 ..
//		PORT(m_CommInfo)  = 3 ;
//	else if ( (GetMenuState(hMenu, IDM_GPS_PORT_4, MF_BYCOMMAND)) & MF_CHECKED )	// check 가 되어 있으면 ..
//		PORT(m_CommInfo)  = 4 ;
//	else if ( (GetMenuState(hMenu, IDM_GPS_PORT_5, MF_BYCOMMAND)) & MF_CHECKED )	// check 가 되어 있으면 ..
//		PORT(m_CommInfo)  = 5 ;
//	else if ( (GetMenuState(hMenu, IDM_GPS_PORT_6, MF_BYCOMMAND)) & MF_CHECKED )	// check 가 되어 있으면 ..
//		PORT(m_CommInfo)  = 6 ;
//	else if ( (GetMenuState(hMenu, IDM_GPS_PORT_7, MF_BYCOMMAND)) & MF_CHECKED )	// check 가 되어 있으면 ..
//		PORT(m_CommInfo)  = 7 ;
//	else if ( (GetMenuState(hMenu, IDM_GPS_PORT_8, MF_BYCOMMAND)) & MF_CHECKED )	// check 가 되어 있으면 ..
//		PORT(m_CommInfo)  = 8 ;
//	else if ( (GetMenuState(hMenu, IDM_GPS_PORT_9, MF_BYCOMMAND)) & MF_CHECKED )	// check 가 되어 있으면 ..
//		PORT(m_CommInfo)  = 9 ;
//	else if ( (GetMenuState(hMenu, IDM_GPS_PORT_10, MF_BYCOMMAND)) & MF_CHECKED )	// check 가 되어 있으면 ..
//		PORT(m_CommInfo)  = 10 ;
//
//	if ( (GetMenuState(hMenu, IDM_GPS_BAUD_2400, MF_BYCOMMAND)) & MF_CHECKED )	// check 가 되어 있으면 ..
//		BAUD(m_CommInfo)	= CBR_2400 ;
//	else if ( (GetMenuState(hMenu, IDM_GPS_BAUD_4800, MF_BYCOMMAND)) & MF_CHECKED )	// check 가 되어 있으면 ..
//		BAUD(m_CommInfo)    = CBR_4800 ;
//	else if ( (GetMenuState(hMenu, IDM_GPS_BAUD_9600, MF_BYCOMMAND)) & MF_CHECKED )	// check 가 되어 있으면 ..
//		BAUD(m_CommInfo)    = CBR_9600 ;
//	else if ( (GetMenuState(hMenu, IDM_GPS_BAUD_14400, MF_BYCOMMAND)) & MF_CHECKED )	// check 가 되어 있으면 ..
//		BAUD(m_CommInfo)    = CBR_14400 ;
//	else if ( (GetMenuState(hMenu, IDM_GPS_BAUD_19200, MF_BYCOMMAND)) & MF_CHECKED )	// check 가 되어 있으면 ..
//		BAUD(m_CommInfo)    = CBR_19200 ;
//	else if ( (GetMenuState(hMenu, IDM_GPS_BAUD_38400, MF_BYCOMMAND)) & MF_CHECKED )	// check 가 되어 있으면 ..
//		BAUD(m_CommInfo)    = CBR_38400 ;
//	else if ( (GetMenuState(hMenu, IDM_GPS_BAUD_57600, MF_BYCOMMAND)) & MF_CHECKED )	// check 가 되어 있으면 ..
//		BAUD(m_CommInfo)    = CBR_57600 ;
//	else if ( (GetMenuState(hMenu, IDM_GPS_BAUD_115200, MF_BYCOMMAND)) & MF_CHECKED )	// check 가 되어 있으면 ..
//		BAUD(m_CommInfo)    = CBR_115200 ;
//	else if ( (GetMenuState(hMenu, IDM_GPS_BAUD_128000, MF_BYCOMMAND)) & MF_CHECKED )	// check 가 되어 있으면 ..
//		BAUD(m_CommInfo)    = CBR_128000 ;
//#endif
//
	CONNECTED(m_CommInfo)		= FALSE ;

	return TRUE ;
}

/*-----------------------------------------------------------------------------

FUNCTION: closeComm

PURPOSE: Closes a connection to a comm port

RETURN:
    TRUE  - successful breakdown of port
    FALSE - port isn't connected

COMMENTS: Waits for threads to exit,
          clears DTR, restores comm port timeouts, purges any i/o
          and closes all pertinent handles

HISTORY:   Date:      Author:     Comment:
           10/27/95   AllenD      Wrote it

-----------------------------------------------------------------------------*/
BOOL CComm::closeComm ()
{
    if (!CONNECTED(m_CommInfo))
        return TRUE ;

#if OVERLAPPED_MODE
	SetEvent(m_stOverlap.hEvent);
#endif
	
#if COMM_LOG
	if ( m_hLog != INVALID_HANDLE_VALUE )
	{
		//logGPS( 0, 0, true );
		if(m_bLogEnable)
			logData(NULL, 0, true);

		CloseHandle (m_hLog) ;
		m_hLog = INVALID_HANDLE_VALUE ;
		if(m_pLogBuf)
			delete [] m_pLogBuf;
		m_pLogBuf = NULL;
		m_nLogBuf = 0;
	}
#endif

    CONNECTED(m_CommInfo) = FALSE;

#if OVERLAPPED_MODE
    // restore original comm timeouts
    if (!SetCommTimeouts(COMDEV(m_CommInfo),  &(TIMEOUTSORIG(m_CommInfo))))
	{
		// error handling ..
	}

    // Purge reads/writes, input buffer and output buffer
    if (!PurgeComm(COMDEV(m_CommInfo), PURGE_FLAGS))
	{
		// error handling ..
	}

	CloseHandle ( COMDEV(m_CommInfo) ) ;
#endif

	CloseHandle ( READSTATTHREAD(m_CommInfo) ) ;

	COMDEV(m_CommInfo) = NULL;
	READSTATTHREAD(m_CommInfo) = INVALID_HANDLE_VALUE ;

    return TRUE;
}

/*-----------------------------------------------------------------------------

FUNCTION: setDCB ( void )

PURPOSE: Sets port state based on settings from the user

COMMENTS: Sets up DCB structure and calls SetCommState.
	  Sets up new timeouts by calling SetCommTimeouts.

HISTORY:   Date:      Author:     Comment:
	    1/ 9/96   AllenD      Wrote it

-----------------------------------------------------------------------------*/
BOOL CComm::setDCB ()
{
	// m_CommInfo 중 필요한 


    DCB dcb = {0};
    
    dcb.DCBlength = sizeof(dcb);

    //
    // get current DCB settings
    //
    if (!GetCommState(COMDEV(m_CommInfo), &dcb))
	{
		// error handling ..
	}


	dcb.BaudRate		= BAUD(m_CommInfo) ;
	dcb.fParity			= FALSE ;
	dcb.fNull			= FALSE ;
	dcb.StopBits		= ONESTOPBIT ;
	dcb.Parity			= NOPARITY ;
	dcb.ByteSize		= 8 ;
	dcb.fBinary			= TRUE ;	// 특히 Win32 환경에서는 반드시 TRUE로 ..

	if ( !SetCommState(COMDEV(m_CommInfo), &dcb) ) 
	{
		// error handling ...
		return FALSE ;
	}

    return TRUE;
}


BOOL	CComm::setTimeOuts ()
{
	COMMTIMEOUTS *	newTO = & (TIMEOUTSNEW(m_CommInfo)) ;


	// 타임아웃 설정. 읽기 타임아웃을 무한대로 설정 ...
	// 초기화에서 memset 을 했으므로.. 사실 아래 작업은 할 필요없다.. 
	// 무한정 기다리기 ... {
//	newTO->ReadIntervalTimeout			= 0 ;
//	newTO->ReadTotalTimeoutMultiplier	= 0 ;
//	newTO->ReadTotalTimeoutConstant		= 0 ;
//
//	newTO->WriteTotalTimeoutMultiplier	= 0 ;
//	newTO->WriteTotalTimeoutConstant	= 0 ;
	// } 무한정 기다리기 ..

	// ReadFile() 이 바로 리턴하게 만들기 {
	//newTO->ReadIntervalTimeout			= MAXDWORD ;

	newTO->ReadIntervalTimeout			= MAXDWORD ;
	newTO->ReadTotalTimeoutMultiplier	= 0 ;
	newTO->ReadTotalTimeoutConstant		= 0 ;
	// } ReadFile() 이 바로 리턴하게 만들기 


	newTO->WriteTotalTimeoutMultiplier	= 2 ;		// 보낸 문자열길이 * 2 = a
	newTO->WriteTotalTimeoutConstant	= 1000 ;	// a + 1초 가 타임아웃..
	

    if ( !SetCommTimeouts(COMDEV(m_CommInfo), &(TIMEOUTSNEW(m_CommInfo))) )
	{
		// error handling ....
		return FALSE ;
	}
	return TRUE ;
}

BOOL CComm::isConnected ( void )
{
	return CONNECTED(m_CommInfo) ;
}

/*-----------------------------------------------------------------------------

FUNCTION: getField ( char * _pBuff, DWORD _nFieldIdx, char * _pField, DWORD _nFieldLen )

PURPOSE: gps data parsing.. ( get Field )

PARMATERS:
	char * _pBuff		: 원래 데이터 포인터
	DWORD  _nFieldIdx	: 원하는 필드 index 
	char * _pField		: 필드 데이터 포인터
	DWORD  _nFieldLen	: 필드 데이터 길이

RETURN: TRUE or FALSE 

HISTORY:   Kevin Cho 2005.07.22		first coding ...

-----------------------------------------------------------------------------*/
// RDS/TMC $PLTTR
#if 0
BOOL    CComm::parseTTR( char* sMsg, DWORD nLen)
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

BOOL CComm::parse(const char* _msg, UINT _nLen)
{
	return TRUE;
}


BOOL CComm::process(const char* szData, UINT len)
{
	if(m_bLogEnable && m_hLog)
	{
		//! Save GPS Log data
		logData( (char*)szData, len) ;
	}

	if(len < 1) return FALSE;

	int idx =0;

#if !SHIPBUILD && 0
	char buff[512];

	int lll = len;

	if(len > 510) len = 510;
	memcpy(buff, szData, lll);

	buff[lll-1] = '\0';

	//__LOG(DZ_TEMP, "NMEA: %s" ,buff);
#endif

	return TRUE ;
}

BOOL	CComm::FlushLog()
{
	logData(NULL, 0, TRUE);
	return TRUE;
}

BOOL	CComm::logData(const char * _szLog, DWORD _nRead, int bForce )
{
	if (m_hLog == INVALID_HANDLE_VALUE) return FALSE;

	DWORD nWrite;

	if( bForce )
	{
		if( m_nLogBuf )
			WriteFile( m_hLog, m_pLogBuf, m_nLogBuf, &nWrite, 0 );
		if( _nRead )
			WriteFile( m_hLog, _szLog, _nRead, &nWrite, 0 );

		m_nLogBuf = 0;
		FlushFileBuffers(m_hLog);
		return TRUE;
	}

	if (!m_pLogBuf || !_szLog || !_nRead) return FALSE;

	if( m_nLogBuf + _nRead >= MAX_LOGBUF )
	{
		if (m_nLogBuf > 0)
		{
			WriteFile(m_hLog, m_pLogBuf, m_nLogBuf, &nWrite, 0);
			m_nLogBuf = 0;
		}
		if (_nRead)
		{
			WriteFile(m_hLog, _szLog, _nRead, &nWrite, 0);
		}
		FlushFileBuffers(m_hLog);
	}
	else
	{
		memcpy( m_pLogBuf+m_nLogBuf, _szLog, _nRead );
		m_nLogBuf += _nRead;
	}

	return TRUE;
}

BOOL CComm::writeComm (const char * _data , int len)
{
	if ( !_data )			return FALSE ;
	//if ( (*_data) != '$' )	return FALSE ;

	if ( !CONNECTED(m_CommInfo) )	return FALSE ;

	DWORD	nWrite;// nSend, ;
//	char	buff[MAX_WRITE_BUFFER + 10] ;

	//nSend = len;//strlen ( _data ) ;

	//__LOG(DZ_TEMP, "WRITE %s", _data);

	BOOL ret = WriteFile ( COMDEV(m_CommInfo), _data, len, &nWrite, NULL ) ;

	// Note that normally the code will not execute the following
	// because the driver caches write operations. Small I/O requests
	// (up to several thousand bytes) will normally be accepted
	// immediately and WriteFile will return true even though an
	// overlapped operation was specified

	if ( !ret )
	{
		//messageBox ( _T("Reset"), _T("GPS Reset Error."), MB_OK | MB_ICONQUESTION ) ;
		//__LOG(DZ_TEMP, "WRITE COMM ERROR!");

		if ( GetLastError() == ERROR_IO_PENDING )
		{
			// We should wait for the completion of the write operation
			// so we know if it worked or not

			// This is only one way to do this. It might be beneficial to
			// place the write operation in a separate thread
			// so that blocking on completion will not negatively
			// affect the responsiveness of the UI
	
			// If the write takes too long to complete, this
			// function will timeout according to the
			// CommTimeOuts.WriteTotalTimeoutMultiplier variable.
			// This code logs the timeout but does not retry
			// the write.
		}
		else
		{
			// some other error occurred

			DWORD		dwError ;
			COMSTAT		comStat ;

			BOOL	errRet = ClearCommError ( COMDEV(m_CommInfo), &dwError, &comStat ) ;

			return FALSE ;
		}
	}


	return TRUE ;
}

UINT CComm::getPort()
{
	return PORT(m_CommInfo);	
}

UINT CComm::getBaudrate()
{
	return BAUD(m_CommInfo);
}



