#if !defined (____GPS_H____) 
#define		____GPS_H____	

#include "AT_Types.h"
//#include "B_NMEA.h"

#include <lbs.h>
#include <LbsSatellite.h>
#include <LbsCommon.h>

//#include "symbian2win32.h"

//#include "base/token.h"
//#include "gps/gpsinterface.h"

//#include "base/cirqueue.h"
//#include "base/BeagleTime.h"
//#include "MPFile.h"

// 다른 device에서는 comm.h에 선언되어 있는 것들
#define GPS_RESET_COLD			1
#define GPS_RESET_WARM			2
#define GPS_RESET_HOT			3

#ifndef GPS_STAT
#define GPS_STAT
#define	GPS_STAT_NO_SIG			0
#define	GPS_STAT_BAD			1
#define	GPS_STAT_WEEK			2
#define	GPS_STAT_NORMAL			3
#define	GPS_STAT_GOOD			4
#endif

///////////////// ---------  GPS 관련  --------- /////////////////
#define MAX_GPSLOGBUF			(1024*16)//16k씩 모아서 파일로 적는다.
#ifndef MAX_FIELD_BUFFER
	#define MAX_FIELD_BUFFER		 25
#endif
#ifndef MAX_PARSE_BUFFER		
	#define MAX_PARSE_BUFFER		256
#endif
#ifndef MAX_LOG_READ_BUFFER
	#define MAX_LOG_READ_BUFFER		384//200//384		// GPS 로그 읽는데 사용하는 버퍼.
#endif

#define GPS_DATA_INTERVAL	1000	//1000000 //GPS정보 update주기. 일단 1초

#define GPS_PORTING	1

#define MAX_LOGBUF		(1024*16)//16k씩 모아서 파일로 적는다.


#ifndef MAX_SATELLITE_NUM
#define MAX_SATELLITE_NUM   (32)
#endif


typedef struct __GPSSatellite {// GPS의 정보
	atDWORD	dwPRN ;		//	Satellite PRN number ( 1-32 )
	atDWORD	dwElev ;	//	Elevation, degrees	( 00-90 )
	atDWORD	dwAzi ;		//	Azimuth, degrees	( 000-359 )
	atDWORD	dwSNR ;		//	SNR - higher is better ( 00-99dB, null when not tracking ) 
	atBOOL	fUse ;		// 사용하고 있는지 어떤지 ..
} _GPSSatellite;


typedef struct __GPSInfo {
	double	dblLon ;	// 경도		longitude	// + 면 동위(E), - 면 서위(W) ..
	double	dblLat ;	// 위도		latitude	// + 면 북위(N), - 면 남위(S) ..
	double	dblAlt ;	// 고도	(m)	altitude 
	
//	double	dblLonPrev ;	// 경도		longitude	// + 면 동위(E), - 면 서위(W) ..
//	double	dblLatPrev ;	// 위도		latitude	// + 면 북위(N), - 면 남위(S) ..
//	double	dblAltPrev ;	// 고도	(m)	altitude 

//	atDWORD	dwTick;
	
	SYS_DATETIME gps_time;
	
	atDWORD	dwSpeed ;	// 속도 (knot)			 ( 원래값 * 100 )
	atLONG	dwAngle ;	// Trace Angle (degree) 
//	atLONG	dwAnglePrev ;	// Trace Angle (degree) 

//	atWORD	wGPSFix ;	// GPS Quality
//	atWORD	wDataStat ;	// Data Status
//	atWORD	wFixMode;   // GSA::Mode2
//	atWORD	RESERVED;
	atWORD	nFixState;   	// GPS Quality
	atWORD	nQuality;   	// GPS Quality
	atWORD	n3DState;   	// GPS Quality
	
	atWORD	wSV ;		// Number of Satellites in View 
	atWORD	wSU ;		// Number of Satellites in Using

	_GPSSatellite		satellite[MAX_SATELLITE_NUM] ;		// 위성정보

	double	dblHDOP ;	// Horizontal Dilution Of Precision
	double	dblVDOP ;	// Vertical   Dilution Of Precision 
	double	dblPDOP ;	// Position   Dilution Of Precision 
} _GPSInfo;


class CGPS
{
public :
	_GPSInfo		m_GPSInfo ;								// GPS 정보
private :
	int					m_bufCount ;

//	bgl::cqueue<double>		m_avgSpeed_kmh_in_3sec ;	// 최근 3초 간의 속도 Queue  (km/h)

	int             m_validCount;
//	CToken          m_token;
    /**
    * symbian GPS server접속을 위한 class
    */	
	TPosition position;
	RPositionServer server;	
	RPositioner positioner;	
	TRequestStatus status;
	
	RRequestorStack RequestStack;
	CRequestor* phContact;
	CRequestor* phService;	
	TPositionModuleId aModuleId;

    /**
    * GPS가 정상적으로 수신되고 있는지를 나타내는 flag
    */ 
	bool bGPSFixed;	

//	SYS_TASK m_hGPSThread;
//	void *m_hProcHandle;
	
	atBOOL m_bConnected;
public:
	int				m_iTimeCount;	//TimeZone Auto Setting 일때 시간을 재는 변수

	
//protected:
	SYS_HFILE	m_hLog;
	atBYTE*		m_pLogBuf;
	atINT		m_nLogBuf;
	atBOOL		m_bLogEnable;
	atBOOL		m_bLoging;

public:	
	CGPS  ( void ) ;
	virtual ~CGPS ( void ) ;

	atBOOL	open(atUINT port, atUINT baudRate);
	atBOOL	close( void ) ;

	atBOOL    connect();     // connnect to server
	atBOOL    disconnect();  // disconnect from server

	// NMEA Parser ..
	atBOOL	process(const char* szData, atUINT len) { return atTRUE; }; //symbian에서는  별 의미없음..항상 true
	atDWORD	parseNMEA (const char* data, atDWORD _nRead ) ;
	atBOOL	isConnected() { return  m_bConnected; }; //symbian에서는  별 의미없음..항상 true

	// Get GPS Information ..
	double	getLon ( void ) ;		// 경도  (도단위) 
	double	getLat ( void ) ;		// 위도  (도단위)
	double	getAlt ( void ) ;		// 고도  (미터 단위)
	atBOOL	getCoord ( double & _lon, double & _lat, double & _alt ) ;	// 좌표

	atBOOL	getTime ( atWORD & _hh, atWORD & _mm, atWORD & _ss ) ;	// 시간
	atBOOL	getDate ( atWORD & _yy, atWORD & _mm, atWORD & _dd ) ;	// 날짜

	double	getSpeed_kmh ( void ) ;		// 속도 ( km/h )
	long	getAngle ( void ) ;		// 진행 각도 (degree : 0 - 359)

	atBOOL	isGPSOK ( void ) ;		// GPS 데이터가 믿을만 한지 ..

	atWORD	getNumSV ( void ) ;		// 보이는 위성 갯수
	atWORD	getNumSU ( void ) ;		// 사용중인 위성 갯수 
	
	double	getPDOP ( void ) ;		// Position Dilution Of Precision
	double	getHDOP ( void ) ;		// Horizontal Dilution Of Precision
	double	getVDOP ( void ) ;		// Vertical Dilution Of Precision

	double	getElev ( void ) ;		// Elevation

	// 위성 정보 가져오기 .. 
	atBOOL	getSatellite  ( atWORD _wIdx, atDWORD & _dwPRN, atDWORD & _dwElev, atDWORD & _dwAzi, atDWORD & _dwSNR, atBOOL & _fUse ) ;
	atBOOL	resetSatelliteAll ( void ) ;		// 모든 위성 정보를 초기화

	atDWORD   resetGPS ( atDWORD _cmd = GPS_RESET_HOT ) ;
	atDWORD	getGPSStat () ;

	double getAverageSpeed3Sec() ;	// Car 3 Sec Average Speed km/h
	/**
     * @brief   GPS data를 얻어온다. symbian의 경우 return하는데 data를 얻을때까지 시간이 걸릴 수 있다.
     * @param   gps  gps data의 구조체
     * @return  성공 TRUE 실패 False
     */	
	
	atBOOL GetGPSData(_GPSInfo* gps);

private :
	atBOOL	setDCB ( void ) ;
	atBOOL	setTimeOuts ( void ) ;

	// gps 관련 ...
	atBOOL	addSatellite ( atDWORD _dwPRN, atDWORD _dwElev, atDWORD _dwAzi, atDWORD _dwSNR = 0, atBOOL _fUse = FALSE ) ;	// 위성정보 추가
	atBOOL	setSatellite ( atDWORD _dwPRN, atDWORD _dwElev, atDWORD _dwAzi, atDWORD _dwSNR = 0 ) ;	// 위성정보 세팅.. fUse 제외
	atBOOL	setSatelliteUse ( atDWORD _dwPRN, atBOOL _fUse = atTRUE ) ;								// 위성정보 세팅.. fUse 세팅 ..

public:
	void setPort(atUINT nPort);
	atUINT getPort();
	
	void setBoudrate(atUINT nRate);
	atUINT getBaudrate();

//	CBeagleTime GetBeagleGpsTime ();


	atBOOL chkeckValid();
	void setValid();

	atBOOL getPowerOn () ;
	atBOOL setPowerOn ( atBOOL _fPowerOnPos ) ;

	atBOOL writeComm ( char * _buff, int _len ) {return atTRUE;}
	char * getTimeStamp () ;

	//void ApplyAutoTimeZone();
	//long getReceiveCount()
	//{
	//#if USE_RECEIVE_COUNT
	//	return m_comm.GetReceiveCount();
	//#else
	//	return 0;
	//#endif
	//}
};

#endif	// ____GPS_H____
