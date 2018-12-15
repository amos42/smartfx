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

// �ٸ� device������ comm.h�� ����Ǿ� �ִ� �͵�
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

///////////////// ---------  GPS ����  --------- /////////////////
#define MAX_GPSLOGBUF			(1024*16)//16k�� ��Ƽ� ���Ϸ� ���´�.
#ifndef MAX_FIELD_BUFFER
	#define MAX_FIELD_BUFFER		 25
#endif
#ifndef MAX_PARSE_BUFFER		
	#define MAX_PARSE_BUFFER		256
#endif
#ifndef MAX_LOG_READ_BUFFER
	#define MAX_LOG_READ_BUFFER		384//200//384		// GPS �α� �дµ� ����ϴ� ����.
#endif

#define GPS_DATA_INTERVAL	1000	//1000000 //GPS���� update�ֱ�. �ϴ� 1��

#define GPS_PORTING	1

#define MAX_LOGBUF		(1024*16)//16k�� ��Ƽ� ���Ϸ� ���´�.


#ifndef MAX_SATELLITE_NUM
#define MAX_SATELLITE_NUM   (32)
#endif


typedef struct __GPSSatellite {// GPS�� ����
	atDWORD	dwPRN ;		//	Satellite PRN number ( 1-32 )
	atDWORD	dwElev ;	//	Elevation, degrees	( 00-90 )
	atDWORD	dwAzi ;		//	Azimuth, degrees	( 000-359 )
	atDWORD	dwSNR ;		//	SNR - higher is better ( 00-99dB, null when not tracking ) 
	atBOOL	fUse ;		// ����ϰ� �ִ��� ��� ..
} _GPSSatellite;


typedef struct __GPSInfo {
	double	dblLon ;	// �浵		longitude	// + �� ����(E), - �� ����(W) ..
	double	dblLat ;	// ����		latitude	// + �� ����(N), - �� ����(S) ..
	double	dblAlt ;	// ��	(m)	altitude 
	
//	double	dblLonPrev ;	// �浵		longitude	// + �� ����(E), - �� ����(W) ..
//	double	dblLatPrev ;	// ����		latitude	// + �� ����(N), - �� ����(S) ..
//	double	dblAltPrev ;	// ��	(m)	altitude 

//	atDWORD	dwTick;
	
	SYS_DATETIME gps_time;
	
	atDWORD	dwSpeed ;	// �ӵ� (knot)			 ( ������ * 100 )
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

	_GPSSatellite		satellite[MAX_SATELLITE_NUM] ;		// ��������

	double	dblHDOP ;	// Horizontal Dilution Of Precision
	double	dblVDOP ;	// Vertical   Dilution Of Precision 
	double	dblPDOP ;	// Position   Dilution Of Precision 
} _GPSInfo;


class CGPS
{
public :
	_GPSInfo		m_GPSInfo ;								// GPS ����
private :
	int					m_bufCount ;

//	bgl::cqueue<double>		m_avgSpeed_kmh_in_3sec ;	// �ֱ� 3�� ���� �ӵ� Queue  (km/h)

	int             m_validCount;
//	CToken          m_token;
    /**
    * symbian GPS server������ ���� class
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
    * GPS�� ���������� ���ŵǰ� �ִ����� ��Ÿ���� flag
    */ 
	bool bGPSFixed;	

//	SYS_TASK m_hGPSThread;
//	void *m_hProcHandle;
	
	atBOOL m_bConnected;
public:
	int				m_iTimeCount;	//TimeZone Auto Setting �϶� �ð��� ��� ����

	
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
	atBOOL	process(const char* szData, atUINT len) { return atTRUE; }; //symbian������  �� �ǹ̾���..�׻� true
	atDWORD	parseNMEA (const char* data, atDWORD _nRead ) ;
	atBOOL	isConnected() { return  m_bConnected; }; //symbian������  �� �ǹ̾���..�׻� true

	// Get GPS Information ..
	double	getLon ( void ) ;		// �浵  (������) 
	double	getLat ( void ) ;		// ����  (������)
	double	getAlt ( void ) ;		// ��  (���� ����)
	atBOOL	getCoord ( double & _lon, double & _lat, double & _alt ) ;	// ��ǥ

	atBOOL	getTime ( atWORD & _hh, atWORD & _mm, atWORD & _ss ) ;	// �ð�
	atBOOL	getDate ( atWORD & _yy, atWORD & _mm, atWORD & _dd ) ;	// ��¥

	double	getSpeed_kmh ( void ) ;		// �ӵ� ( km/h )
	long	getAngle ( void ) ;		// ���� ���� (degree : 0 - 359)

	atBOOL	isGPSOK ( void ) ;		// GPS �����Ͱ� ������ ���� ..

	atWORD	getNumSV ( void ) ;		// ���̴� ���� ����
	atWORD	getNumSU ( void ) ;		// ������� ���� ���� 
	
	double	getPDOP ( void ) ;		// Position Dilution Of Precision
	double	getHDOP ( void ) ;		// Horizontal Dilution Of Precision
	double	getVDOP ( void ) ;		// Vertical Dilution Of Precision

	double	getElev ( void ) ;		// Elevation

	// ���� ���� �������� .. 
	atBOOL	getSatellite  ( atWORD _wIdx, atDWORD & _dwPRN, atDWORD & _dwElev, atDWORD & _dwAzi, atDWORD & _dwSNR, atBOOL & _fUse ) ;
	atBOOL	resetSatelliteAll ( void ) ;		// ��� ���� ������ �ʱ�ȭ

	atDWORD   resetGPS ( atDWORD _cmd = GPS_RESET_HOT ) ;
	atDWORD	getGPSStat () ;

	double getAverageSpeed3Sec() ;	// Car 3 Sec Average Speed km/h
	/**
     * @brief   GPS data�� ���´�. symbian�� ��� return�ϴµ� data�� ���������� �ð��� �ɸ� �� �ִ�.
     * @param   gps  gps data�� ����ü
     * @return  ���� TRUE ���� False
     */	
	
	atBOOL GetGPSData(_GPSInfo* gps);

private :
	atBOOL	setDCB ( void ) ;
	atBOOL	setTimeOuts ( void ) ;

	// gps ���� ...
	atBOOL	addSatellite ( atDWORD _dwPRN, atDWORD _dwElev, atDWORD _dwAzi, atDWORD _dwSNR = 0, atBOOL _fUse = FALSE ) ;	// �������� �߰�
	atBOOL	setSatellite ( atDWORD _dwPRN, atDWORD _dwElev, atDWORD _dwAzi, atDWORD _dwSNR = 0 ) ;	// �������� ����.. fUse ����
	atBOOL	setSatelliteUse ( atDWORD _dwPRN, atBOOL _fUse = atTRUE ) ;								// �������� ����.. fUse ���� ..

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
