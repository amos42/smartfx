#if !defined (____GPS_H____) 
#define		____GPS_H____	

#include "base/token.h"
#include "gps/gpsinterface.h"
#include "commGPS.h"

#include "base/cirqueue.h"
#include "base/BeagleTime.h"

///////////////// ---------  GPS 관련  --------- /////////////////
#define MAX_GPSLOGBUF			(1024*16)//16k씩 모아서 파일로 적는다.

class CGPS : public CCommGPS
{
public :
	stGPSInfo			m_GPSInfo ;								// GPS 정보
	CRITICAL_SECTION	m_cs ;

private :
	char				m_szBuff[MAX_READ_BUFFER + 16] ;		// GPS 데이터 받는 변수..
	int					m_bufCount ;

	BOOL			m_fPowerOn ;	// Power On 후 처음으로  GPS가 활성화 되면 TRUE 가 된다..

	char	m_szBuffWrite[MAX_WRITE_BUFFER] ;

	bgl::cqueue<double>		m_avgSpeed_kmh_in_3sec ;	// 최근 3초 간의 속도 Queue  (km/h)

	//CCritSec		m_cs_dataQueue;	// GPS Data Queue용 Critical Section

	int             m_validCount;
	CToken          m_token;

public:
	int				m_iTimeCount;	//TimeZone Auto Setting 일때 시간을 재는 변수
	
#if GPS_LOG
	BOOL    m_bLoging;				// GPS Logging 유무 
#endif

	CGPS  ( void ) ;
	virtual ~CGPS ( void ) ;

	BOOL	open(UINT port, UINT baudRate);
	BOOL	close( void ) ;

	//CCritSec* GetDataCS() {return &m_cs_dataQueue;}

	// NMEA Parser ..
	BOOL	process(const char* szData, UINT len);
	DWORD	parseNMEA (const char* data, DWORD _nRead ) ;
	//BOOL	isConnected(){ return m_comm.isConnected(); };

	// Get GPS Information ..
	double	getLon ( void ) ;		// 경도  (도단위) 
	double	getLat ( void ) ;		// 위도  (도단위)
	double	getAlt ( void ) ;		// 고도  (미터 단위)
	BOOL	getCoord ( double & _lon, double & _lat, double & _alt ) ;	// 좌표

	BOOL	getTime ( WORD & _hh, WORD & _mm, WORD & _ss ) ;	// 시간
	BOOL	getDate ( WORD & _yy, WORD & _mm, WORD & _dd ) ;	// 날짜

	double	getSpeed_kmh ( void ) ;		// 속도 ( km/h )
	long	getAngle ( void ) ;		// 진행 각도 (degree : 0 - 359)

	BOOL	isGPSOK ( void ) ;		// GPS 데이터가 믿을만 한지 ..

	WORD	getNumSV ( void ) ;		// 보이는 위성 갯수
	WORD	getNumSU ( void ) ;		// 사용중인 위성 갯수 
	
	double	getPDOP ( void ) ;		// Position Dilution Of Precision
	double	getHDOP ( void ) ;		// Horizontal Dilution Of Precision
	double	getVDOP ( void ) ;		// Vertical Dilution Of Precision

	double	getElev ( void ) ;		// Elevation

	// 위성 정보 가져오기 .. 
	BOOL	getSatellite  ( WORD _wIdx, DWORD & _dwPRN, DWORD & _dwElev, DWORD & _dwAzi, DWORD & _dwSNR, BOOL & _fUse ) ;
	BOOL	resetSatelliteAll ( void ) ;		// 모든 위성 정보를 초기화

	DWORD   resetGPS ( DWORD _cmd = GPS_RESET_HOT ) ;
	DWORD	getGPSStat () ;
	double	getAverageSpeed3Sec() ;	// Car 3 Sec Average Speed km/h


private :

	BOOL	setDCB ( void ) ;
	BOOL	setTimeOuts ( void ) ;

	BOOL	createGpsThread ( void );


	// NMEA parser ...
	BOOL	getField ( char * _pBuff, DWORD _nFieldIdx, char * _pField, DWORD _nFieldLen = MAX_FIELD_BUFFER ) ;
	BOOL	parseRMC ( char * _szRMC, DWORD _nLen ) ;	// RMC  = Recommended Minimum Specific GPS/TRANSIT Data
	BOOL	parseGGA ( char * _szGGA, DWORD _nLen ) ;	// GGA  = Global Positioning System Fix Data
	BOOL	parseGSV ( char * _szGSV, DWORD _nLen ) ;	// GSV  = Number of SVs(Satellites in View) in view, PRN numbers, elevation, azimuth & SNR values.
	BOOL	parseGSA ( char * _szGSA, DWORD _nLen ) ;	// GSA  = GPS receiver operating mode, SVs(Satellites in View) used for navigation, and DOP values.


	BOOL	parseRMB ( char * _szRMB, DWORD _nLen ) ;	// RMB  = Recommended Minimum Navigation Information
	BOOL	parseVTG ( char * _szVTG, DWORD _nLen ) ;	// VTG  = Actual track made good and speed over ground
	BOOL	parseRMA ( char * _szRMA, DWORD _nLen ) ;	// RMA  = Navigation data from present position
	
	BOOL	checksumGPS ( char * _szData, DWORD _nLen ) ;

	// gps 관련 ...
	BOOL	addSatellite ( DWORD _dwPRN, DWORD _dwElev, DWORD _dwAzi, DWORD _dwSNR = 0, BOOL _fUse = FALSE ) ;	// 위성정보 추가
	BOOL	setSatellite ( DWORD _dwPRN, DWORD _dwElev, DWORD _dwAzi, DWORD _dwSNR = 0 ) ;	// 위성정보 세팅.. fUse 제외
	BOOL	setSatelliteUse ( DWORD _dwPRN, BOOL _fUse = TRUE ) ;								// 위성정보 세팅.. fUse 세팅 ..

	friend BOOL WINAPI readCommProc (); //(LPVOID lpV)

public:
	void setPort(UINT nPort);
	void setBoadrate(UINT nRate);

	CBeagleTime GetBeagleGpsTime ();

	UINT getPort();
	UINT getBaudrate();

	BOOL chkeckValid();
	void setValid();

	BOOL getPowerOn () ;
	BOOL setPowerOn ( BOOL _fPowerOnPos ) ;

	BOOL writeComm ( char * _buff, int _len ) ;
	char * getTimeStamp () ;

	void ApplyAutoTimeZone();
	//long getReceiveCount()
	//{
	//#if USE_RECEIVE_COUNT
	//	return m_comm.GetReceiveCount();
	//#else
	//	return 0;
	//#endif
	//}

#if GPS_LOG
protected:
	BOOL logGPS( char * _szLog, DWORD _nRead, int bForce=false );

//public:
//	// GPS 로깅 파일 생성
//	BOOL createLogFile(const TCHAR* filename);
//	BOOL FlushLog();
#endif
};

//
//  Thread procedures
//
BOOL WINAPI readCommProc ( void ) ; //( LPVOID );

#endif	// ____GPS_H____
