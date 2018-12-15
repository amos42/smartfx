
#if !defined (____COMM_GPS_H____) 
#define		____COMM_GPS_H____	

#include "inc/define.h"
#include "comm.h"

#define MAX_LOGBUF				(1024*16)//16k씩 모아서 파일로 적는다.

#define USE_RECEIVE_COUNT		DEBUG_COMPATIBLE

class CCommGPS : public CComm
{
public:
	CCommGPS ( void ) ;
	virtual ~CCommGPS( void ) ;

private:
	int		m_validCount;
	
#if USE_RECEIVE_COUNT
	long	m_nReceiveCount;
#endif

public:
	virtual BOOL process(const char* szData, UINT len);
	virtual BOOL parse(const char* _msg, UINT _nLen);
	virtual BOOL openComm(UINT port, UINT baudRate);
	virtual BOOL closeComm(void);

#if USE_RECEIVE_COUNT
	void SetReceiveCount(long nReceiveCount);
	long GetReceiveCount();
#endif
};

#endif	// ____COMM_GPS_H____
