#if !defined (____GPS_H____) 
#define		____GPS_H____	

#include "base/token.h"
#include "gps/gpsinterface.h"
#include "commGPS.h"

#include "base/cirqueue.h"
#include "base/BeagleTime.h"

///////////////// ---------  GPS ����  --------- /////////////////
#define MAX_GPSLOGBUF			(1024*16)//16k�� ��Ƽ� ���Ϸ� ���´�.

class CGPS : public CCommGPS
{
public :
	stGPSInfo			m_GPSInfo ;								// GPS ����
	CRITICAL_SECTION	m_cs ;

private :
	char				m_szBuff[MAX_READ_BUFFER + 16] ;		// GPS ������ �޴� ����..
	int					m_bufCount ;

	BOOL			m_fPowerOn ;	// Power On �� ó������  GPS�� Ȱ��ȭ �Ǹ� TRUE �� �ȴ�..

	char	m_szBuffWrite[MAX_WRITE_BUFFER] ;

	bgl::cqueue<double>		m_avgSpeed_kmh_in_3sec ;	// �ֱ� 3�� ���� �ӵ� Queue  (km/h)

	//CCritSec		m_cs_dataQueue;	// GPS Data Queue�� Critical Section

	int             m_validCount;
	CToken          m_token;

public:
	int				m_iTimeCount;	//TimeZone Auto Setting �϶� �ð��� ��� ����
	
#if GPS_LOG
	BOOL    m_bLoging;				// GPS Logging ���� 
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
	double	getLon ( void ) ;		// �浵  (������) 
	double	getLat ( void ) ;		// ����  (������)
	double	getAlt ( void ) ;		// ��  (���� ����)
	BOOL	getCoord ( double & _lon, double & _lat, double & _alt ) ;	// ��ǥ

	BOOL	getTime ( WORD & _hh, WORD & _mm, WORD & _ss ) ;	// �ð�
	BOOL	getDate ( WORD & _yy, WORD & _mm, WORD & _dd ) ;	// ��¥

	double	getSpeed_kmh ( void ) ;		// �ӵ� ( km/h )
	long	getAngle ( void ) ;		// ���� ���� (degree : 0 - 359)

	BOOL	isGPSOK ( void ) ;		// GPS �����Ͱ� ������ ���� ..

	WORD	getNumSV ( void ) ;		// ���̴� ���� ����
	WORD	getNumSU ( void ) ;		// ������� ���� ���� 
	
	double	getPDOP ( void ) ;		// Position Dilution Of Precision
	double	getHDOP ( void ) ;		// Horizontal Dilution Of Precision
	double	getVDOP ( void ) ;		// Vertical Dilution Of Precision

	double	getElev ( void ) ;		// Elevation

	// ���� ���� �������� .. 
	BOOL	getSatellite  ( WORD _wIdx, DWORD & _dwPRN, DWORD & _dwElev, DWORD & _dwAzi, DWORD & _dwSNR, BOOL & _fUse ) ;
	BOOL	resetSatelliteAll ( void ) ;		// ��� ���� ������ �ʱ�ȭ

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

	// gps ���� ...
	BOOL	addSatellite ( DWORD _dwPRN, DWORD _dwElev, DWORD _dwAzi, DWORD _dwSNR = 0, BOOL _fUse = FALSE ) ;	// �������� �߰�
	BOOL	setSatellite ( DWORD _dwPRN, DWORD _dwElev, DWORD _dwAzi, DWORD _dwSNR = 0 ) ;	// �������� ����.. fUse ����
	BOOL	setSatelliteUse ( DWORD _dwPRN, BOOL _fUse = TRUE ) ;								// �������� ����.. fUse ���� ..

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
//	// GPS �α� ���� ����
//	BOOL createLogFile(const TCHAR* filename);
//	BOOL FlushLog();
#endif
};

//
//  Thread procedures
//
BOOL WINAPI readCommProc ( void ) ; //( LPVOID );

#endif	// ____GPS_H____
