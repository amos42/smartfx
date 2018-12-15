#if !defined (____COMM_H____) 
#define		____COMM_H____	

#include <windows.h>
#include <tchar.h>
#include "inc/define.h"
#include "base/lock.h"
#include "gps/struct.h"
#include "base/threadinterface.h"

#define COMM_LOG       1

#define COMDEV(x)			(x.hCommPort)
#define READSTATTHREAD(x)	(x.hReaderStatus)

#define PORT(x)				(x.dwPort)
#define BAUD(x)				(x.dwBaudRate)

#define	EVTMASKORIG(x)		(x.dwEvtMaskOrig)
#define	EVTMASKNEW(x)		(x.dwEvtMaskNew)

#define TIMEOUTSORIG(x)		(x.timeoutsorig)
#define TIMEOUTSNEW(x)		(x.timeoutsnew)

#define CONNECTED(x)		(x.fConnected)

///////////////// ---------  GPS 관련  --------- /////////////////

#define MAX_WRITE_BUFFER         64// 512
#define MAX_READ_BUFFER         1024

#define MAX_PARSE_BUFFER		256
#define MAX_FIELD_BUFFER		 25

#define PURGE_FLAGS             (PURGE_TXABORT | PURGE_TXCLEAR | PURGE_RXABORT | PURGE_RXCLEAR)

#define GPS_RESET_COLD			1
#define GPS_RESET_WARM			2
#define GPS_RESET_HOT			3

#define	GPS_STAT_NO_SIG			0
#define	GPS_STAT_BAD			1
#define	GPS_STAT_WEEK			2
#define	GPS_STAT_NORMAL			3
#define	GPS_STAT_GOOD			4

#define MAX_LOGBUF				(1024*16)//16k씩 모아서 파일로 적는다.

#define OVERLAPPED_MODE			(0 && WIN_CE)

typedef DWORD (WINAPI *PCOMM_PARSE_CALLBACK)(const char* stream, UINT len, LPVOID lpParam);

struct stCommInfo
{
	HANDLE	hCommPort, hReaderStatus ;
	DWORD	dwPort, dwBaudRate ;
	
	DWORD	dwEvtMaskOrig, dwEvtMaskNew ;

    COMMTIMEOUTS timeoutsorig ;
    COMMTIMEOUTS timeoutsnew ;

	BOOL	fConnected, fRsv0, fRsv1, fRsv2  ;
	
	DWORD   dwPortStatus;
} ;

class	CComm
{
public :
	CComm ( void ) ;
	virtual ~CComm( void ) ;

public :
	stCommInfo		m_CommInfo ;							// Comm Port 정보 ...	
	BOOL			m_fGPSLogTmrOn ;
	
	#if COMM_LOG
		BOOL    m_bLoging;				// GPS Logging 유무 
	#endif

	#if OVERLAPPED_MODE
		// In PC, use overlapped IO. 
		OVERLAPPED		m_stOverlap;
	#endif

	#if PC_DBG
		// PC Debug버젼에서 Window-Message로 
		// GPS Log가 들어 왔을 때 마치 GPS 포트가 
		// 열린 것처림 처리하기 위해
		BOOL			m_bGPSMsgReceived;
	#endif
	
protected :
	char				m_szBuff[MAX_READ_BUFFER + 16] ;		// GPS 데이터 받는 변수..
	int					m_bufCount ;
	CRITICAL_SECTION	m_cs ;

	PCOMM_PARSE_CALLBACK  m_pParser;
	LPVOID                m_lpParserParameter;
	HANDLE			m_hReadGpsLog ;

	CCritSec		m_cs_val;
	int             m_validCount;

	BOOL m_bLogEnable;
	
	#if COMM_LOG
		HANDLE			m_hLog;
		BYTE*			m_pLogBuf;
		int				m_nLogBuf;
	#endif

protected :
	BOOL	setDCB ( void ) ;
	BOOL	setTimeOuts ( void ) ;

	friend BOOL WINAPI readCommProc (); //(LPVOID lpV)

public :
	virtual BOOL process(const char* szData, UINT len);
	virtual BOOL parse(const char* _msg, UINT _nLen);
	void    setParser(PCOMM_PARSE_CALLBACK lpParser, LPVOID);

	virtual BOOL openComm(UINT port, UINT baudRate);
	virtual BOOL closeComm(void);
	BOOL	initCommInfo (void) ;
	void	shift (int _iMode, int _iSec);

	//BOOL	openTest ();
	//HANDLE	openConnectTest ();
	BOOL	isConnected ( void );

	BOOL writeComm (const char * _data, int len ) ;
	BOOL FlushLog();

	UINT getPort();
	UINT getBaudrate();
	BOOL chkeckValid();
	void setValid();

	// Get GPS Information ..
	DWORD	timer ( UINT _idEvent ) ;
	
	// COMM 로깅 파일 생성
	BOOL createLogFile(const TCHAR* filename);
	BOOL logData(const char * _szLog, DWORD _nRead, int bForce=false );
};

#endif	// ____COMM_H____
