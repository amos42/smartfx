#include "inc/define.h"
//#include "gps/commLib.h"
#include "gps.h"
#include <time.h>
#include "base/token.h"
#include "base/CountryBndManager.h"
#include "base/API_Geometry.h"
#include "base/BeagleTime.h"
#include "base/HeapManager.h"
#include "base/LatLong-UTMconversion.h"
#include "base/settingmanager.h"
#include "netdb/timeZoneman.h"
#include "gps/gpsemul.h"

#if defined(_AFX) && defined(_DEBUG)
# define new DEBUG_NEW
# undef THIS_FILE
  static char THIS_FILE[] = __FILE__;
#endif

#define		GPS_OFF_IS_MM_OFF		0
#define     GPS_FIX_BY_GSA_ONLY     0   // GSA::Mode2 �������� �̿��Ͽ� 
									    // GPS Fix���� 
#define		AVRSPEED_KMH_IN_3SEC	3	// ������ 3�ʰ� ��սü�

//extern GPSPARAM g_objGPSParam;

//  5130.7106 -> 51�� 30.7106�� -> 51.51184333.. �� 
inline double NMEA2LL(double nn)
{
	ASSERT(nn >= 0);

	WORD	deg ;

	nn /= 100.0 ;
	deg = (WORD) nn ;
	nn -= deg ; 
	return deg + ( (nn * 10.0)/6.0 ) ;
}

DWORD WINAPI gpsParseProc(const char* stream, UINT len, LPVOID lpParam)
{
	ASSERT(lpParam);

	CGPS* pGPS = (CGPS*) lpParam;

	pGPS->parseNMEA (stream, len);

	return 0;
}

CGPS::CGPS ( void ) : m_avgSpeed_kmh_in_3sec(AVRSPEED_KMH_IN_3SEC)
{
	memset ( &m_GPSInfo, 0x00, sizeof(m_GPSInfo) ) ;
	m_GPSInfo.dwHDOP = 100 * 100 ;
	m_GPSInfo.dwVDOP = 100 * 100 ;
	m_GPSInfo.dwPDOP = 100 * 100 ;

	setParser(gpsParseProc, this);

	m_bufCount = 0;

	InitializeCriticalSection ( &m_cs ) ;

	setPowerOn ( FALSE ) ;

	m_validCount = 0;

#if PC_DBG
	m_bGPSMsgReceived = FALSE;
#endif

	m_token.InitMaxCnt(20);

	m_iTimeCount = 0;	//Automatic TimeZone Setting�� ���� �ð��� ��� ����
}



CGPS::~CGPS ( void )
{
	DeleteCriticalSection ( &m_cs ) ;

	setPowerOn ( FALSE ) ;
}

BOOL CGPS::open(UINT port, UINT baudRate)
{
	setPowerOn ( FALSE ) ;

	return openComm(port, baudRate);
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
BOOL CGPS::close()
{
	setPowerOn ( FALSE ) ;

	resetSatelliteAll();

	return closeComm();
}


/*-----------------------------------------------------------------------------

FUNCTION: setDCB ( void )

PURPOSE: Sets port state based on settings from the user

COMMENTS: Sets up DCB structure and calls SetCommState.
	  Sets up new timeouts by calling SetCommTimeouts.

HISTORY:   Date:      Author:     Comment:
	    1/ 9/96   AllenD      Wrote it

------------------------------------------------------------------------*/


/*------------------------------------------------------------------------

FUNCTION: createGpsThread

PURPOSE: Creates the Reader/Status and Writer threads

HISTORY:   Date:      Author:     Comment:
           10/27/95   AllenD      Wrote it

------------------------------------------------------------------------*/


/*------------------------------------------------------------------------

FUNCTION: getField ( char * _pBuff, DWORD _nFieldIdx, char * _pField, DWORD _nFieldLen )

PURPOSE: gps data parsing.. ( get Field )

PARMATERS:
	char * _pBuff		: ���� ������ ������
	DWORD  _nFieldIdx	: ���ϴ� �ʵ� index 
	char * _pField		: �ʵ� ������ ������
	DWORD  _nFieldLen	: �ʵ� ������ ����

RETURN: TRUE or FALSE 

HISTORY:   Kevin Cho 2005.07.22		first coding ...

------------------------------------------------------------------------*/
BOOL	CGPS::getField ( char * _pBuff, DWORD _nFieldIdx, char * _pField, DWORD _nFieldLen )
{
	if ( (_pBuff == NULL) || (_pField == NULL) || (_nFieldLen <= 0) )
	{
		return FALSE;
	}

	//
	// Go to the beginning of the selected field
	//
	DWORD	ii = 0 ;
	DWORD	dwFieldIdx = 0 ;

	while ( (dwFieldIdx != _nFieldIdx) && _pBuff[ii] )
	{
		if ( _pBuff[ii] == ASCII_COMMA )
		{
			++dwFieldIdx ;
		}

		++ii ;

		if ( !_pBuff[ii] )
		{
			_pField[0] = NULL ;
			return FALSE;
		}
	}

	if ( (_pBuff[ii] == ASCII_COMMA) || (_pBuff[ii] == ASCII_ASTERISK) )
	{
		_pField[0] = NULL ;
		return FALSE;
	}

	//
	// copy field from pData to Field
	//
	DWORD	jj = 0 ;
	while ( (_pBuff[ii] != ASCII_COMMA) && (_pBuff[ii] != ASCII_ASTERISK) && _pBuff[ii] )
	{
		_pField[jj] = _pBuff[ii];
		++jj ; ++ii ;

		//
		// check if field is too big to fit on passed parameter. If it is,
		// crop returned field to its max length.
		//
		if ( jj >= _nFieldLen )
		{
			break ;
		}
	}
	_pField[jj] = NULL ;


	return TRUE ;
}


/*-----------------------------------------------------------------------------

FUNCTION: parseNMEA ( DWORD _nRead )

PURPOSE: gps data parsing..

PARMATERS:
    _nRead		- DWORD ( 4bytes ) value contains the number of bytes read.
RETURN: TRUE or FALSE 

HISTORY:   Kevin Cho 2005.07.19		first coding ...
		   Jason	 2006.2.5       modify
			   - GPS ����Ÿ�� �Ķ���ͷ� �ѱ� 
			   - ���� NMEA����Ÿ ���� �Ѿ�� ���������� NMEA�� �и��ϴ� ��Ŀ���
			     �ܺο��� NMEA�������ݺ��� �и��Ͽ�, �ϳ��� �������ݸ��� �ѱ�.
			   - �̿����� GPS�α׸� �ܺο��� ����. 
			   - RMC������ M/M���� 
-----------------------------------------------------------------------------*/
DWORD CGPS::parseNMEA (const char* data, DWORD len )
{
	EnterCriticalSection ( &m_cs ) ;

	char buff[MAX_PARSE_BUFFER+10];
	memcpy(buff, data, len);
	buff[len] = '\0';

	// �������� ���´� ������ ����..
	// $GPRMC,154232,A,2758.612,N,08210.515,W,085.4,084.4,230394,003.1,W*43[CR][LF] 
#if defined(_DEBUG__0) && defined(_AFX)
	CString str = buff;
	TRACE(_T("%s\n"), str);
#endif

	DWORD dwRetVal = COMM_NONE;

	if ( !strncmp(buff, "$GPRMC", GPS_SENTENCE_ID_LEN) )
	{
		if (
		#if GPS_SIMUL_ENABLE
			!GPS_IsSuspend() &&
		#endif
			parseRMC ( buff, len))
			GPS_PushRMCData(isGPSOK(), getLon(), getLat(), getAlt(), getSpeed_kmh(), getAngle());
		dwRetVal = COMM_GPRMC;
	}
	else if ( !strncmp(buff, "$GPGGA", GPS_SENTENCE_ID_LEN) )
	{
		parseGGA ( buff, len) ;
		dwRetVal = COMM_GPGGA;
	}
	else if ( !strncmp(buff, "$GPGSV", GPS_SENTENCE_ID_LEN) )
	{
		parseGSV ( buff, len) ;
		dwRetVal = COMM_GPGSV;
	}
	else if ( !strncmp(buff, "$GPGSA", GPS_SENTENCE_ID_LEN) )
	{
		parseGSA ( buff, len) ;
		dwRetVal = COMM_GPGSA;
	}
	else if ( !strncmp(buff, "$PLTTS", GPS_SENTENCE_ID_LEN) )
	{
		// Tuner Status & Acknowledge
		dwRetVal = COMM_PLTTS;
	}
	else if ( !strncmp(buff, "$PLTTF", GPS_SENTENCE_ID_LEN) )
	{
		// Tunner Frequency (Philips model)
		dwRetVal = COMM_PLTTF;
	} 
	else if ( !strncmp(buff, "$PLTTQ", GPS_SENTENCE_ID_LEN) )
	{
		// Tunner Frequency (Open TMC 2.0 model)
		dwRetVal = COMM_PLTTQ;
	}
	else if ( !strncmp(buff, "$PLTCH", GPS_SENTENCE_ID_LEN) )
	{
		// Tunner Channel
		dwRetVal = COMM_PLTCH;
	}
	else if ( !strncmp(buff, "$PLTMT", GPS_SENTENCE_ID_LEN) )
	{
		// Meta data => RDS Country Code
		dwRetVal = COMM_PLTMT;
	}
	else if ( !strncmp(buff, "$PLTRD", GPS_SENTENCE_ID_LEN) )
	{
		// Row RDS Data (Not implemented by Amaryllo)
		dwRetVal = COMM_PLTRD;
	}
	else if ( !strncmp(buff, "$PLTTM", GPS_SENTENCE_ID_LEN) )
	{
		// TMC / Application data
		dwRetVal = COMM_PLTTM;
	}
	else if ( !strncmp(buff, "$PLTTR", GPS_SENTENCE_ID_LEN) )
	{
		//parseTTR( buff, len);
		dwRetVal = COMM_PLTTR;
	}
	else if ( !strncmp(buff, "$PLTFD", GPS_SENTENCE_ID_LEN) )
	{
		// RDS Filter
		dwRetVal = COMM_PLTFD;
	}
	else if ( !strncmp(buff, "$PLTCF", GPS_SENTENCE_ID_LEN) )
	{
		// Confirm message request
		dwRetVal = COMM_PLTCF;
	}

	LeaveCriticalSection ( &m_cs ) ;

	return dwRetVal ;
}

void CGPS::ApplyAutoTimeZone()
{
#if defined(WIN_CE) || (GPS_TIME_TEST) // ��Ʈ�� 2007.09.13 { TimeZone Automatic Setting
		
#if (GPS_TIME_TEST)
	#pragma message("�ӽ÷� PC�������� ���ư��� �� : PC�ð��� �ٲ����� �ſ� �����ؾ� ��.")
#endif

	//TimeZone ������ Auto �϶� �� �۵��Ѵ�.... 
	stSettingTimeZone settingTimeZone;
	GD_GetSettingTimeZone(&settingTimeZone);
	if (!settingTimeZone.bAutoSetting) return;
	
	if (m_iTimeCount < 0 || m_iTimeCount > 60 ) // [2008.8.27 Adolph] ī��Ʈ�� �ʱ�ȭ �Ǿ� �ٽ� ����ȭ ����������
		m_iTimeCount = 0;

	if (m_iTimeCount == 0) //ī��Ʈ �� �ð��� 1���϶�...(10�ʸ��� �ѹ��� �缳��)
	{
		BOOL fOldPosOk = FALSE;  // �������� ��ġ�� �����̳�?
		BOOL fLenOk = FALSE;	 // ������ 1�е��� ������ �Ÿ��� ����? 
		
		ipoint2 carPos = GD_GetCarPos();

		if((settingTimeZone.ptOldCarPos.x != 0 && settingTimeZone.ptOldCarPos.y != 0)) // ����������ġ�� �������ϸ� �Ÿ����
		{
			fOldPosOk = TRUE;
			
			if ((long)SightDistance( settingTimeZone.ptOldCarPos, carPos) > 500)
			{
				fLenOk = TRUE;
				settingTimeZone.ptOldCarPos.x = carPos.x;
				settingTimeZone.ptOldCarPos.y = carPos.y;
			}
		}
		else
		{
			// ������ġ�� ������ ���� ���ϸ� ��ġ�� �޴´�...
			settingTimeZone.ptOldCarPos.x = carPos.x;
			settingTimeZone.ptOldCarPos.y = carPos.y;
			
			fLenOk = TRUE;
			fOldPosOk = TRUE;
		}
		
		if( fOldPosOk && fLenOk )	
		{
			//TimeZone Sync
			SetBeagleAutoTimeZone(&settingTimeZone);
			setTimeZoneInformation(&settingTimeZone);			

			// config.cfg �� �����ϴ� �κ�.
			GD_SetSettingTimeZone(&settingTimeZone);
			SaveConfig_TimeZone(&settingTimeZone);
		
			GPS_UpdateLocalTime();
		}
	}

	m_iTimeCount++;
#endif
}

BOOL	CGPS::parseRMC ( char * _szRMC, DWORD _nLen ) 	// RMC  = Recommended Minimum Specific GPS/TRANSIT Data
{
//                1      2   3     4    5     6  7   8     9   10  11
//		$GPRMC,hhmmss.ss,A,llll.ll,a,yyyyy.yy,a,x.x,x.x,ddmmyy,x.x,a*hh
                   
//	       0      1       2     3     4     5      6     7    8   9   0 1 
//	    $GPRMC,074048.461,A,3733.2870,N,12655.1386,E,0.000000,,310106,,*14

//
//		RMC  = Recommended Minimum Specific GPS/TRANSIT Data
//
//		1    = UTC of position fix	(hhmmss.sss)
//		2    = Data status (V=navigation receiver warning)
//		3    = Latitude of fix		// 4807.038,N   Latitude 48 deg 07.038' N
//		4    = N or S
//		5    = Longitude of fix		//01131.000,E  Longitude 11 deg 31.000' E
//		6    = E or W
//		7    = Speed over ground in knots
//		8    = Track made good in degrees True
//		9    = UT date (DDMMYY) 
//		10   = Magnetic variation degrees (Easterly var. subtracts from true course)
//		11   = E or W
//		12   = Checksum

	if ( !checksumGPS(_szRMC, _nLen) ) 
		return FALSE ;

	char	szField[MAX_FIELD_BUFFER + 10] ;
	memset ( szField, 0x00, sizeof(szField) ) ;

	if ( !getField ( _szRMC, 0, szField ) )	// $GPXXX ���̵� �ʵ带 ������ �� ������ ���� ������ ..
		return FALSE ;

	m_token.Reset();
	int tok_count = m_token.Splite(_szRMC);

	if(tok_count < 12)
	{
		return FALSE;
	}

	if (m_GPSInfo.dblLon && m_GPSInfo.dblLat)
	{
		m_GPSInfo.dblLonPrev = m_GPSInfo.dblLon;
		m_GPSInfo.dblLatPrev = m_GPSInfo.dblLat;
		m_GPSInfo.dblAltPrev = m_GPSInfo.dblAlt;
		m_GPSInfo.dwAnglePrev = m_GPSInfo.dwAngle;
	}

	m_GPSInfo.dwTick = GetTickCount();

	//		1    = UTC of position fix  
	if ( getField(_szRMC, 1, szField) )
	{
		// time stamp save
		strcpyn ( m_GPSInfo.time_stamp, COUNTOF(m_GPSInfo.time_stamp), szField ) ;

		//m_GPSInfo.dwTime = atol ( szField ) ;		// (hhmmss.sss)
		char szTemp[4] ;
		memset ( szTemp, 0x00, sizeof(szTemp) ) ;
		
		char * pField = szField ;
		strncpy ( szTemp, pField, 2 ) ;
		m_GPSInfo.hh	= (char) atol (szTemp) ;

		pField += 2 ;
		strncpy ( szTemp, pField, 2 ) ;
		m_GPSInfo.min	= (char) atol (szTemp) ;

		pField += 2 ;
		strncpy ( szTemp, pField, 2 ) ;
		m_GPSInfo.ss	= (char) atol (szTemp) ;
	}
	else
	{
		//m_GPSInfo.dwTime = 0 ;
		m_GPSInfo.hh	= 0 ;
		m_GPSInfo.min	= 0 ;
		m_GPSInfo.ss	= 0 ;
	}

	//		2    = Data status (V=navigation receiver warning)
	if ( getField(_szRMC, 2, szField) )
	{
		m_GPSInfo.wDataStat = szField[0] ;
		if(m_GPSInfo.wDataStat == 'A'){
			//m_validCount = 30;
			setValid();
		}
		else{
			m_validCount = 0;
		}

	}
	else
	{
		m_GPSInfo.wDataStat = 0 ;
	}

	//		3    = Latitude of fix
	if ( getField(_szRMC, 3, szField) )
	{
		//m_GPSInfo.dblLat = atof ( szField ) ;
		m_GPSInfo.dblLat = NMEA2LL(atof ( szField ));	// �� ������ ���� 2006.5.15
		// 4807.038,N   Latitude 48 deg 07.038' N
	}
	else
	{
		m_GPSInfo.dblLat = 0 ;
	}

	//		4    = N or S
	if ( getField(_szRMC, 4, szField) )
	{
		// m_GPSInfo.dblLat ��  trans..
		if ( szField[0] == 'N' )
		{
		}
		else if ( szField[0] == 'S' )
		{
			m_GPSInfo.dblLat *= -1.0 ;
		}
		else
		{
			m_GPSInfo.dblLat = 0 ;
		}
	}
	else
	{
		m_GPSInfo.dblLat = 0 ;
	}


	//		5    = Longitude of fix
	if ( getField(_szRMC, 5, szField) )
	{
		//m_GPSInfo.dblLon = atof ( szField ) ;
		m_GPSInfo.dblLon = NMEA2LL(atof ( szField ));  // �� ������ ���� 

		//01131.000,E  Longitude 11 deg 31.000' E
	}
	else
	{
		m_GPSInfo.dblLon = 0 ;
	}


	//		6    = E or W
	if ( getField(_szRMC, 6, szField) )
	{
		// m_GPSInfo.dblLon ��  trans..
		if ( szField[0] == 'E' )
		{
		}
		else if ( szField[0] == 'W' )
		{
			m_GPSInfo.dblLon *= -1.0 ;
		}
		else
		{
			m_GPSInfo.dblLon = 0 ;
		}		
	}
	else
	{
		m_GPSInfo.dblLon = 0 ;
	}


	//		7    = Speed over ground in knots
	if ( getField(_szRMC, 7, szField) )
	{
		// knot : ���ء��װ��� �ӵ� ����. 1�ð��� 1�ظ�(����, 1852m)�� �ӵ��� 1��Ʈ�� �Ѵ�
		double knot = atof ( szField ) ;
		m_GPSInfo.dwSpeed = (DWORD) (knot * 100) ;	// double ���¸� DWORD�� �����Ѵ�.

		// getSpeed() �Լ����� knot�� km �� �ٲپ� �����ϱ� ������
		// ���⼭ knot->km �� �ʿ�� ����..

		//double kmh  = KNOTS_TO_KMH(knot) ;	// knot �� km/h �� ��ȯ
		//m_GPSInfo.dwSpeed = (DWORD) (kmh * 100) ;	// double ���¸� DWORD�� �����Ѵ�.


		// knot --> km/h --> save
		double km = knot * 1.851998 ;

		m_avgSpeed_kmh_in_3sec.push_top( km ) ;
	}
	else
	{
		m_GPSInfo.dwSpeed = 0 ;
	}

	//		8    = Track made good in degrees True
	if ( getField(_szRMC, 8, szField) )
	{
		m_GPSInfo.dwAngle = atol ( szField ) ;
	}
	else
	{
		m_GPSInfo.dwAngle = 0 ;
	}

	//		9    = UT date	(DDMMYY) 
	if ( getField(_szRMC, 9, szField) )		
	{
		//m_GPSInfo.dwDate = atol ( szField ) ;
		char szTemp[4] ;
		memset ( szTemp, 0x00, sizeof(szTemp) ) ;
		
		char * pField = szField ;
		strncpy ( szTemp, pField, 2 ) ;
		m_GPSInfo.dd	= (char) atol (szTemp) ;

		pField += 2 ;
		strncpy ( szTemp, pField, 2 ) ;
		m_GPSInfo.mon	= (char) atol (szTemp) ;

		pField += 2 ;
		strncpy ( szTemp, pField, 2 ) ;
		m_GPSInfo.yy	= (short) atol (szTemp) ;

		if ( m_GPSInfo.yy >= 80 )
			m_GPSInfo.yy += 1900 ;
		else
			m_GPSInfo.yy += 2000 ;
				}
	else
	{
		//m_GPSInfo.dwDate = 0 ;
		m_GPSInfo.yy	= 0 ;
		m_GPSInfo.mon	= 0 ;
		m_GPSInfo.dd	= 0 ;
	}

	//		10   = Magnetic variation degrees (Easterly var. subtracts from true course)
	//		11   = E or W

	ApplyAutoTimeZone();
				
	return TRUE;
}

BOOL	CGPS::parseGGA ( char * _szGGA, DWORD _nLen ) 	// GGA  = Global Positioning System Fix Data
{
	   //           1       2    3     4    5 6 7   8   9  0  1  2  3   4  
//		$GPGGA,hhmmss.ss,llll.ll,a,yyyyy.yy,a,x,xx,x.x,x.x,M,x.x,M,x.x,xxxx*hh
//
//		GGA  = Global Positioning System Fix Data
//
//		1    = UTC of Position
//		2    = Latitude (ddmm.mmmm)
//		3    = N or S
//		4    = Longitude (ddmm.mmmm)
//		5    = E or W
//		6    = GPS quality indicator (0=invalid; 1=GPS fix; 2=Diff. GPS fix)
//		7    = Number of satellites in use [not those in view]
//		8    = Horizontal dilution of position (HDOP)
//		9    = Antenna altitude above/below mean sea level (geoid)
//		10   = Meters  (Antenna height unit)
//		11   = Geoidal separation (Diff. between WGS-84 earth ellipsoid and
//			   mean sea level.  -=geoid is below WGS-84 ellipsoid)
//		12   = Meters  (Units of geoidal separation)
//		13   = Age in seconds since last update from diff. reference station
//		14   = Diff. reference station ID#
//		15   = Checksum

		
	if ( !checksumGPS(_szGGA, _nLen) ) 
		return FALSE ;

	char	szField[MAX_FIELD_BUFFER + 10] ;

	memset ( szField, 0x00, sizeof(szField) ) ;

	if ( !getField ( _szGGA, 0, szField ) )	// $GPXXX ���̵� �ʵ带 ������ �� ������ ���� ������ ..
		return FALSE ;

	m_token.Reset();
	int tok_count = m_token.Splite(_szGGA);

	if(tok_count < 15)
	{
		return FALSE;
	}


	//		1    = UTC of Position
	//		2    = Latitude
	//		3    = N or S
	//		4    = Longitude
	//		5    = E or W
	//		6    = GPS quality indicator (0=invalid; 1=GPS fix; 2=Diff. GPS fix)
	if ( getField(_szGGA, 6, szField) )
	{
		m_GPSInfo.wGPSFix = (WORD) atol ( szField ) ;
	}
	else
	{
		m_GPSInfo.wGPSFix = 0 ;
	}

	//		7    = Number of satellites in use [not those in view]
	if ( getField(_szGGA, 7, szField) )
	{
		m_GPSInfo.wSU = (WORD) atol ( szField ) ;
	}
	else
	{
		m_GPSInfo.wSU = 0 ;
	}

	//		8    = Horizontal dilution of position (HDOP)
	if ( getField(_szGGA, 8, szField) )
	{
		m_GPSInfo.dwHDOP = (DWORD) ( atof ( szField ) * 100 ) ;
	}
	else
	{
		m_GPSInfo.dwHDOP = 0 ;
	}

	//		9    = Antenna altitude above/below mean sea level (geoid)
	if ( getField(_szGGA, 9, szField) )
	{
		m_GPSInfo.dblAlt = atof ( szField ) ;
	}
	else
	{
		m_GPSInfo.dblAlt = 0 ;
	}

	//		10   = Meters  (Antenna height unit)
	if ( getField(_szGGA, 10, szField) )
	{
		// szField �� 'M'���� �Ѿ���� ���ʹ���..
		// do nothing..
	}

	//		11   = Geoidal separation (Diff. between WGS-84 earth ellipsoid and
	//			   mean sea level.  -=geoid is below WGS-84 ellipsoid)
	//		12   = Meters  (Units of geoidal separation)
	//		13   = Age in seconds since last update from diff. reference station
	//		14   = Diff. reference station ID#

	return TRUE ;
}

BOOL	CGPS::parseGSV ( char * _szGSV, DWORD _nLen ) 	// GSV  = Number of SVs(Satellites in View) in view, PRN numbers, elevation, azimuth & SNR values.
{
//		$GPGSV,4,1,13,02,02,213,,03,-3,000,,11,00,121,,14,13,172,05*67 
//		GSV  = Number of SVs(Satellites in View) in view, PRN numbers, elevation, azimuth & SNR values. 
//
//		1    = Total number of messages of this type in this cycle 
//		2    = Message number 
//		3    = Total number of SVs in view 
//		4    = SV PRN number 
//		5    = Elevation in degrees, 90 maximum 
//		6    = Azimuth, degrees from true north, 000 to 359 
//		7    = SNR, 00-99 dB (null when not tracking) 
//		8-11 = Information about second SV, same as field 4-7 
//		12-15= Information about third SV, same as field 4-7 
//		16-19= Information about fourth SV, same as field 4-7 
//
//		���� ����(SV)�� 4�� ����� �ƴ� ��� ������ �����ʹ� ª������.
//		ex) $GPGSV,4,4,13,14,13,172,05*67 


	if ( !checksumGPS(_szGSV, _nLen) ) 
		return FALSE ;

	DWORD	nTotalNumOfMsg, nMsgNum =0;
	char	szField[MAX_FIELD_BUFFER + 10] ;

	memset ( szField, 0x00, sizeof(szField) ) ;

	if ( !getField ( _szGSV, 0, szField ) )	// $GPXXX ���̵� �ʵ带 ������ �� ������ ���� ������ ..
		return FALSE ;

	if( !getField( _szGSV, 7, szField) )
	{
		return FALSE;
	}

	m_token.Reset();
	int tok_count = m_token.Splite(_szGSV);

	if(tok_count < 8)
	{
		return FALSE;
	}

	//		1    = Total number of messages of this type in this cycle 
	if ( getField(_szGSV, 1, szField) )	
	{
		nTotalNumOfMsg = atol ( szField ) ;

		//
		// Make sure that the nTotalNumOfMsg is valid. This is used to
		// calculate indexes into an array. I've seen corrept NMEA strings
		// with no checksum set this to large values.
		//
		if (nTotalNumOfMsg < 1 || nTotalNumOfMsg*4 > MAX_SATELLITE_NUM)
			return FALSE ; 
	}
	else
	{
		return FALSE ;
	}

	//		2    = Message number 
	if ( getField(_szGSV, 2, szField) )	
	{
		nMsgNum = atol ( szField ) ;
		//__LOG(DZ_TEMP, "%d", nMsgNum);

		//
		// Make sure that the message number is valid. This is used to
		// calculate indexes into an array
		//
		if ( nMsgNum < 1 || nMsgNum > nTotalNumOfMsg)
			return FALSE ; 

	}
	else
	{
		return FALSE ;
	}

	//		3    = Total number of SVs in view 
	if ( getField(_szGSV, 3, szField) )	
	{
		m_GPSInfo.wSV = (WORD) atol ( szField ) ;
	}
	else
	{
		m_GPSInfo.wSV = 0 ;
	}

	DWORD dwSatIndex;

	for ( int ii = 0 ; ii < 4 ; ++ii )
	{
		dwSatIndex = (nMsgNum-1)*4 + ii;

		//		4    = SV PRN number 
		if ( getField(_szGSV, 4 + (4*ii), szField) )	
		{
			m_GPSInfo.satellite[dwSatIndex].dwPRN = atol ( szField ) ;

			for (DWORD i=0; i<MAX_SATELLITE_NUM; i++)
			{
				if (i == dwSatIndex) continue;

				if (m_GPSInfo.satellite[i].dwPRN == m_GPSInfo.satellite[dwSatIndex].dwPRN)
				{
					memset(m_GPSInfo.satellite+i, NULL, sizeof(stSatellite));
					break;
				}
			}
		}
		else
		{
			m_GPSInfo.satellite[dwSatIndex].dwPRN = 0 ;
		}

		//		5    = Elevation in degrees, 90 maximum 
		if ( getField(_szGSV, 5 + (4*ii), szField) )	
		{
			m_GPSInfo.satellite[dwSatIndex].dwElev = atol ( szField ) ;
		}
		else
		{
			m_GPSInfo.satellite[dwSatIndex].dwElev = 0 ;
		}

		//		6    = Azimuth, degrees from true north, 000 to 359 
		if ( getField(_szGSV, 6 + (4*ii), szField) )
		{
			m_GPSInfo.satellite[dwSatIndex].dwAzi = atol ( szField ) ;
		}
		else
		{
			m_GPSInfo.satellite[dwSatIndex].dwAzi = 0 ;
		}

		//		7    = SNR, 00-99 dB (null when not tracking) 
		if ( getField(_szGSV, 7 + (4*ii), szField) )
		{
			m_GPSInfo.satellite[dwSatIndex].dwSNR = atol ( szField ) ;
		}
		else
		{
			m_GPSInfo.satellite[dwSatIndex].dwSNR = 0 ;
		}

		if (tok_count <= (4 + 4*(ii+1)) )
			break;
	}


	return TRUE ;
}


BOOL	CGPS::parseGSA ( char * _szGSA, DWORD _nLen ) 	// GSA  = GPS receiver operating mode, SVs(Satellites in View) used for navigation, and DOP values.
{
//		$GPGSA,A,3,19,28,14,18,27,22,31,39,,,,,1.7,1.0,1.3*35[CR][LF]
//
//		GSA  = GPS receiver operating mode, SVs(Satellites in View) used for navigation, and DOP values.
//
//		1    = Mode:
//			   M=Manual, forced to operate in 2D or 3D
//			   A=Automatic, 3D/2D
//		2    = Mode:
//			   1=Fix not available
//			   2=2D
//			   3=3D
//		3-14 = IDs of SVs used in position fix (null for unused fields)
//		15   = PDOP (Position   dilution of precision)
//		16   = HDOP (Horizontal dilution of precision)
//		17   = VDOP (Vertical   dilution of precision)


	if ( !checksumGPS(_szGSA, _nLen) ) 
	{
		//m_GPSInfo.wFixMode =0;
		return FALSE ;
	}

	char	szField[MAX_FIELD_BUFFER + 10] ;
	memset ( szField, 0x00, sizeof(szField) ) ;

	if ( !getField ( _szGSA, 0, szField ) )	// $GPXXX ���̵� �ʵ带 ������ �� ������ ���� ������ ..
	{
		//m_GPSInfo.wFixMode  =0;
		return FALSE ;
	}

	// Checksum���� �ʵ尡 �����ϴ��� üũ 
	if( !getField( _szGSA, 17, szField) )
	{
		return FALSE;
	}


	//		1    = Mode:
	//			   M=Manual, forced to operate in 2D or 3D
	//			   A=Automatic, 3D/2D
	if ( getField(_szGSA, 1, szField) )	
	{
		// do nothing ..
	}
	else
	{
		// do nothing ..
	}


	//		2    = Mode:
	//			   1=Fix not available
	//			   2=2D
	//			   3=3D
	if ( getField(_szGSA, 2, szField) )	
	{
		m_GPSInfo.wFixMode = atoi(szField);
	}
	else
	{
		// do nothing ..
	}
	

	//		3-14 = IDs of SVs used in position fix (null for unused fields)
	DWORD	dwPRN ;
	DWORD	ii, jj ;
	
	for ( jj = 0 ; jj < MAX_SATELLITE_NUM ; ++jj )
	{
		m_GPSInfo.satellite[jj].fUse = FALSE ;
	}


	for ( ii = 0 ; ii < 12 ; ++ii )
	{
		if ( getField(_szGSA, 3 + ii, szField) )	
		{		
			dwPRN = atol ( szField ) ;

			for ( jj = 0 ; jj < MAX_SATELLITE_NUM ; ++jj )
			{
				if ( dwPRN == m_GPSInfo.satellite[jj].dwPRN )
					m_GPSInfo.satellite[jj].fUse = TRUE ;
			}
		}
		else
		{
			// do nothing ...
		}
	}


	//		15   = PDOP (Position   dilution of precision)
	if ( getField(_szGSA, 15, szField) )	
	{
		m_GPSInfo.dwPDOP = (DWORD) ( atof ( szField ) * 100 ) ;
	}
	else
	{
		m_GPSInfo.dwPDOP = 0 ;
	}

	//		16   = HDOP (Horizontal dilution of precision)
	// HDOP�� GGA���� ....

	//		17   = VDOP (Vertical   dilution of precision)
	if ( getField(_szGSA, 17, szField) )	
	{
		m_GPSInfo.dwVDOP = (DWORD) ( atof ( szField ) * 100 ) ;
	}
	else
	{
		m_GPSInfo.dwVDOP = 0 ;
	}

	return TRUE ;
}




BOOL	CGPS::parseRMB ( char * _szRMB, DWORD _nLen ) 	// RMB  = Recommended Minimum Navigation Information
{
//		$GPRMB,A,x.x,a,c--c,d--d,llll.ll,e,yyyyy.yy,f,g.g,h.h,i.i,j*kk
//
//		RMB  = Recommended Minimum Navigation Information
//
//		1    = Data Status (V=navigation receiver warning)
//		2    = Crosstrack error in nautical miles
//		3    = Direction to steer (L or R) to correct error
//		4    = Origin waypoint ID#
//		5    = Destination waypoint ID#
//		6    = Destination waypoint latitude
//		7    = N or S
//		8    = Destination waypoint longitude
//		9    = E or W
//		10   = Range to destination in nautical miles
//		11   = Bearing to destination, degrees True
//		12   = Destination closing velocity in knots
//		13   = Arrival status; (A=entered or perpendicular passed)
//		14   = Checksum

	if ( !checksumGPS(_szRMB, _nLen) ) 
		return FALSE ;

	return TRUE ;
}


BOOL	CGPS::parseVTG ( char * _szVTG, DWORD _nLen ) 	// VTG  = Actual track made good and speed over ground
{
//		$GPVTG,t,T,,,s.ss,N,s.ss,K*hh
//
//		VTG  = Actual track made good and speed over ground
//
//		1    = Track made good
//		2    = Fixed text 'T' indicates that track made good is relative to true north
//		3    = not used
//		4    = not used
//		5    = Speed over ground in knots
//		6    = Fixed text 'N' indicates that speed over ground in in knots
//		7    = Speed over ground in kilometers/hour
//		8    = Fixed text 'K' indicates that speed over ground is in kilometers/hour
//		9    = Checksum

	if ( !checksumGPS(_szVTG, _nLen) ) 
		return FALSE ;

	return TRUE ;
}

BOOL	CGPS::parseRMA ( char * _szRMA, DWORD _nLen ) 	// RMA  = Navigation data from present position
{
//		$GPRMA,A,llll.ll,N,lllll.ll,W,,,ss.s,ccc,vv.v,W*hh
//
//		RMA  = Navigation data from present position
//
//		1    = Data status 
//		2    = Latitude
//		3    = N/S
//		4    = longitude
//		5    = W/E
//		6    = not used
//		7    = not used
//		8    = Speed over ground in knots
//		9    = Course over ground
//		10   = Variation
//		11   = Direction of variation E/W
//		12   = Checksum


	if ( !checksumGPS(_szRMA, _nLen) ) 
		return FALSE ;


	return TRUE ;
}



BOOL	CGPS::checksumGPS ( char * _szData, DWORD _nLen )
{
	// data format 
	// $GPRMC,154232,A,2758.612,N,08210.515,W,085.4,084.4,230394,003.1,W*43[CR][LF] 
	
	// checksum : * ���� ��. Hex format
	//			: $~* ������ ������ XOR ��..

	char	xor = 0x00 ;	// $~* ������ ������ XOR ���� �ϱ� ���� ����
	
	for ( DWORD ii = 0 ; ii < _nLen ; ++ii )
	{
		if ( _szData[ii] == ASCII_DOLLOR )
		{
			// do nothing..
		}
		else if ( _szData[ii] == ASCII_ASTERISK )
		{
			++ii ;
			break ;		// * �̸� ������ checksum ..
		}
		else 
		{
			xor ^= _szData[ii] ;
		}
	}

	char	checksum = 0x00 ;
	char	check ;


	check = _szData[ii++] ;

	if ( ('0' <= check) && (check <= '9') )
		check -= '0' ;
	else if ( ('A' <= check) && (check <= 'F') )
		check -= ('A' - 10) ;		// check = check - 'A' + 10 ;	// 10������ ����� ���ؼ� ....
	else 
		check = 0 ;


	checksum = check << 4 ;			// ���� 4 ��Ʈ�� ����Ʈ ..


	check = _szData[ii] ;

	if ( ('0' <= check) && (check <= '9') )
		check -= '0' ;
	else if ( ('A' <= check) && (check <= 'F') )
		check -= ('A' - 10) ;		// check = check - 'A' + 10 ;	// 10������ ����� ���ؼ� ....
	else 
		check = 0 ;


	checksum |= check ;				// ���� 4 ��Ʈ



	if ( checksum == xor )
		return TRUE ;

	return FALSE ;
}




/*-----------------------------------------------------------------------------

FUNCTION: addSatellite ( DWORD _dwPRN, DWORD _dwElev, DWORD _dwAzi, DWORD _dwSNR = 0, BOOL _fUse = FALSE )

PURPOSE: ���� ������ �߰���.

PARMATERS:
		DWORD _dwPRN  : 4����Ʈ. GPS ID == Satellite PRN number ( 1-32 )
		DWORD _dwElev : 4����Ʈ. ��(Elevation). ������ degrees	( 00-90 )
		DWORD _dwAzi  : 4����Ʈ. ������(Azimuth). ������ degrees	( 000-359 )
		DWORD _dwSNR  : 4����Ʈ. SNR - higher is better ( 00-99dB, null when not tracking ) 
		BOOL  _fUse   : 4����Ʈ. ���� ��� ����

RETURN: TRUE : �߰��� ����
		FALSE : �߰��� ����

HISTORY:   Kevin Cho	2005.07.21		ó������ ����

-----------------------------------------------------------------------------*/
BOOL	CGPS::addSatellite ( DWORD _dwPRN, DWORD _dwElev, DWORD _dwAzi, DWORD _dwSNR, BOOL _fUse ) // �������� �߰�
{
	if ( (_dwPRN < 1) || (_dwPRN > 32) )
		return FALSE ;

	if ( m_GPSInfo.wSV >= MAX_SATELLITE_NUM )
		return FALSE ;

	WORD	idx = m_GPSInfo.wSV ;
	if(idx >= MAX_SATELLITE_NUM ) return FALSE;
	

	m_GPSInfo.satellite[idx].dwPRN  = _dwPRN ;
	m_GPSInfo.satellite[idx].dwElev = _dwElev ;
	m_GPSInfo.satellite[idx].dwAzi  = _dwAzi ;
	m_GPSInfo.satellite[idx].dwSNR  = _dwSNR ;
	m_GPSInfo.satellite[idx].fUse   = _fUse ;

	++m_GPSInfo.wSV ;


	return TRUE ;
}

/*-----------------------------------------------------------------------------

FUNCTION: setSatellite ( DWORD _dwPRN, DWORD _dwElev, DWORD	_dwAzi, DWORD _dwSNR = 0 )

PURPOSE: ���� ������ ������. ��� ���θ� ����ϴ� fUse �� ����

PARMATERS:
		DWORD _dwPRN  : 4����Ʈ. GPS ID == Satellite PRN number ( 1-32 )
		DWORD _dwElev : 4����Ʈ. ��(Elevation). ������ degrees	( 00-90 )
		DWORD _dwAzi  : 4����Ʈ. ������(Azimuth). ������ degrees	( 000-359 )
		DWORD _dwSNR  : 4����Ʈ. SNR - higher is better ( 00-99dB, null when not tracking ) 

RETURN: TRUE : ���ÿ� ����
		FALSE : ���ÿ� ����

HISTORY:   Kevin Cho	2005.07.21		ó������ ����

-----------------------------------------------------------------------------*/
BOOL	CGPS::setSatellite ( DWORD _dwPRN, DWORD _dwElev, DWORD	_dwAzi, DWORD _dwSNR )
{
	if ( (_dwPRN < 1) || (_dwPRN > 32) )
		return FALSE ;


	for ( int ii = 0 ; ii < m_GPSInfo.wSV ; ++ii )
	{
		if ( m_GPSInfo.satellite[ii].dwPRN == _dwPRN )
		{
			m_GPSInfo.satellite[ii].dwElev = _dwElev ;
			m_GPSInfo.satellite[ii].dwAzi  = _dwAzi ;
			m_GPSInfo.satellite[ii].dwSNR  = _dwSNR ;

			return TRUE ;
		}
	}


	return FALSE ;
}


/*-----------------------------------------------------------------------------

FUNCTION: setSatelliteUse ( DWORD _dwPRN, BOOL _fUse )

PURPOSE: ���� ��� ���θ� ������.

PARMATERS:
		DWORD _dwPRN : 4����Ʈ. GPS ID == Satellite PRN number ( 1-32 )
		BOOL  _fUse  : 4����Ʈ. ���� ��뿩��

RETURN: TRUE : ���ÿ� ����
		FALSE : ���ÿ� ����

HISTORY:   Kevin Cho	2005.07.21		ó������ ����

-----------------------------------------------------------------------------*/
BOOL	CGPS::setSatelliteUse ( DWORD _dwPRN, BOOL _fUse )
{
	if ( (_dwPRN < 1) || (_dwPRN > 32) )
		return FALSE ;
	
	for ( int ii = 0 ; ii < m_GPSInfo.wSV ; ++ii )
	{
		if ( m_GPSInfo.satellite[ii].dwPRN == _dwPRN )
		{
			m_GPSInfo.satellite[ii].fUse = _fUse ;
			return TRUE ;
		}
	}

	return FALSE ;
}



/*-----------------------------------------------------------------------------

FUNCTION: resetSatelliteAll ( void )

PURPOSE: ��� ���� �����͸� �ʱ�ȭ.

PARMATERS:

RETURN: TRUE : �ʱ�ȭ�� ����.
		FALSE : �ʱ�ȭ�� ����

HISTORY:   Kevin Cho	2005.07.21		ó������ ����

-----------------------------------------------------------------------------*/
BOOL	CGPS::resetSatelliteAll ( void )
{
	memset ( m_GPSInfo.satellite, 0x00, sizeof(m_GPSInfo.satellite)) ;

	return TRUE ;
}




/*-----------------------------------------------------------------------------

FUNCTION: get??? (void)

PURPOSE: �浵/����/���� ������..

PARMATERS:

RETURN: �浵/����/��

HISTORY:   Kevin Cho	2005.07.21		ó������ ����

-----------------------------------------------------------------------------*/
double	CGPS::getLon ( void ) // �浵  (�� ����)
{
	return m_GPSInfo.dblLon ;
}


double	CGPS::getLat ( void )	// ���� ( �� ����)
{
	return m_GPSInfo.dblLat ;
}

double	CGPS::getAlt ( void ) // ��
{
	return m_GPSInfo.dblAlt ;
}


/*-----------------------------------------------------------------------------

FUNCTION: getCoord ( double & _lon, double & _lat, double & _alt )

PURPOSE: ��ǥ�� ����.

PARMATERS:
	double & _lon :	���۷���. �浵
	double & _lat : ���۷���. ���� 
	double & _alt : ���۷���. ��

RETURN: always TRUE 

HISTORY:   Kevin Cho	2005.07.21		ó������ ����

-----------------------------------------------------------------------------*/
BOOL	CGPS::getCoord ( double & _lon, double & _lat, double & _alt ) // ��ǥ
{
	_lon = getLon () ;
	_lat = getLat () ;
	_alt = getAlt () ;
	
	return TRUE ;
}



/*-----------------------------------------------------------------------------

FUNCTION: getTime (void)

PURPOSE: UTC �ð��� ����..

PARMATERS:

RETURN: �ð� (UTC)

HISTORY:   Kevin Cho	2005.07.21		ó������ ����

-----------------------------------------------------------------------------*/
//DWORD	CGPS::getTime ( void ) // �ð�
//{
//	return m_GPSInfo.dwTime ; 
//}


/*-----------------------------------------------------------------------------

FUNCTION: getDate (void)

PURPOSE: ��¥�� ����

PARMATERS:

RETURN: ��¥

HISTORY:   Kevin Cho	2005.07.21		ó������ ����

-----------------------------------------------------------------------------*/
//DWORD	CGPS::getDate ( void ) // ��¥
//{
//	return m_GPSInfo.dwDate ; 
//}


/*-----------------------------------------------------------------------------

FUNCTION: getTime ( WORD & _hh, WORD & _mm, WORD & _ss )

PURPOSE: �ð��� ��..

PARMATERS:	
	WORD & _hh : ���۷���. hour.
	WORD & _mm : ���۷���. min.
	WORD & _ss : ���۷���. sec.

RETURN: always TRUE

HISTORY:   Kevin Cho	2005.07.21		ó������ ����

-----------------------------------------------------------------------------*/
BOOL	CGPS::getTime ( WORD & _hh, WORD & _mm, WORD & _ss ) // �ð�
{
	_hh = m_GPSInfo.hh ;
	_mm = m_GPSInfo.min ;
	_ss = m_GPSInfo.ss ;

	return TRUE ;
}


/*-----------------------------------------------------------------------------

FUNCTION: getDate ( WORD & _yy, WORD & _mm, WORD & _dd )

PURPOSE: ��¥�� ��..

PARMATERS:	
	WORD & _yy : ���۷���. year.
	WORD & _mm : ���۷���. mon.
	WORD & _dd : ���۷���. day.

RETURN: always TRUE

HISTORY:   Kevin Cho	2005.07.21		ó������ ����

-----------------------------------------------------------------------------*/
BOOL	CGPS::getDate ( WORD & _yy, WORD & _mm, WORD & _dd ) // ��¥
{
	_yy = m_GPSInfo.yy ;
	_mm = m_GPSInfo.mon ;
	_dd = m_GPSInfo.dd ;

	return TRUE ;
}


/*-----------------------------------------------------------------------------

FUNCTION: getSpeed (void)

PURPOSE: �ӵ��� �� ..

PARMATERS:	

RETURN: 
	double : �ӵ�(km/h).
	-1 : ����.

HISTORY:   Kevin Cho	2005.07.21		ó������ ����

-----------------------------------------------------------------------------*/
double	CGPS::getSpeed_kmh ( void ) // �ӵ�
{
//for test GetAutoZoomScale()
//#if 1
//	int iSpeedMin = 0;
//	int iSpeedMax = 100;
//	int iDiv = 3;
//	static int s_iSpeed=0;
//	static int s_bSpeedInc=true;
//	if( s_bSpeedInc )
//	{
//		if( s_iSpeed/iDiv>iSpeedMax-iSpeedMin ) s_bSpeedInc = false;
//		else s_iSpeed++;
//	}else
//	{
//		if( s_iSpeed<=0 ) s_bSpeedInc = true;
//		else s_iSpeed--;
//	}
//	return iSpeedMin + MIN( s_iSpeed/iDiv, iSpeedMax-iSpeedMin );
//#endif

	double	dblSpeed = m_GPSInfo.dwSpeed / 100.0 ;	// �ӵ� (knot) ( ������ * 100 ) ;

	// �ӵ� ��ȯ ( knot -> km/h )
	//1m/s -> 1.943846 knots
	//1knots ->1.851998 km/h

	return dblSpeed * 1.851998 ;
}

/*-----------------------------------------------------------------------------

FUNCTION: getAngle(void)

PURPOSE: ���� ������ ����..

PARMATERS:	

RETURN: 
	DWORD dwAngle : Trace Angle. ( 0 - 359 )


HISTORY:   Kevin Cho	2005.07.21		ó������ ����

-----------------------------------------------------------------------------*/
long	CGPS::getAngle ( void ) // ���� ���� (degree : 0 - 359)
{
	return m_GPSInfo.dwAngle ;
}


/*-----------------------------------------------------------------------------

FUNCTION: isGPSOK (void)

PURPOSE: GPS �����Ͱ� ����������(Ȱ��ȭ �Ǿ�����) ���θ� ����...

PARMATERS:	

RETURN: 
	TRUE : GPS�� Ȱ��ȭ �Ǿ���.
	FALSE ; GPS�� Ȱ��ȭ ���� �ʾ���.

HISTORY:   Kevin Cho	2005.07.21		ó������ ����

      2007.05.23
	   * isGPSOK üũ �׸� ���� 
	     - HDOPüũ �κ��� �����ϰ�, 2D, 3D fixed�׸� �߰� 
		   (2D �̻� fix�Ǿ�� ��ȿ�Ѱ����� ����.)
		
-----------------------------------------------------------------------------*/
BOOL	CGPS::isGPSOK ( void ) // GPS �����Ͱ� ������ ���� ..
{
	if( m_GPSInfo.wGPSFix != 0             //// GPS quality indicator (0=invalid; 1=GPS fix; 2=Diff. GPS fix)
		&& m_GPSInfo.wDataStat == 'A')		//// Data status (V=navigation receiver warning)
		// GSA�� ������ �޴� �κ����� �����Ͽ��� ����
		//&& m_GPSInfo.wFixMode > 1)			// 1 : Fix not available , 2: 2D,  3: 3D
		return TRUE;
	else
		return FALSE;
}


/*-----------------------------------------------------------------------------

FUNCTION: getNumS? (void)

PURPOSE: �������� ���� ( SV : Satellites in View, SU : Satellites in Using )

PARMATERS:	

RETURN: ��������

HISTORY:   Kevin Cho	2005.07.21		ó������ ����

-----------------------------------------------------------------------------*/
WORD	CGPS::getNumSV ( void ) // ���̴� ���� ����
{
	return m_GPSInfo.wSV ;
}


WORD	CGPS::getNumSU ( void ) // ������� ���� ���� 
{
	return m_GPSInfo.wSU ;
}


/*-----------------------------------------------------------------------------

FUNCTION: get?DOP (void)

PURPOSE:DOP(Dilusion Of Precision)�� ����

PARMATERS:

RETURN: DOP

HISTORY:   Kevin Cho	2005.07.21		ó������ ����

-----------------------------------------------------------------------------*/
double	CGPS::getPDOP ( void ) // Position Dilution Of Precision
{
	return ( m_GPSInfo.dwPDOP / 100.0 ) ;
}


double	CGPS::getHDOP ( void ) // Horizontal Dilution Of Precision
{
	return ( m_GPSInfo.dwHDOP / 100.0 ) ;
}

double	CGPS::getVDOP ( void ) // Vertical Dilution Of Precision
{
	return ( m_GPSInfo.dwVDOP / 100.0 ) ;
}

double	CGPS::getElev ( void ) 		// Elevation
{
	int iMax = getNumSV();
	for (int nn = 0; nn<iMax ; nn++)
	{
		stSatellite	* p = &(m_GPSInfo.satellite[nn]) ;

		if (p->fUse)
			return p->dwElev;
	}

	return 0 ;
}

/*-----------------------------------------------------------------------------

FUNCTION: getSatellite  ( WORD _wIdx, DWORD & _dwPRN, DWORD & _dwElev, DWORD & _dwAzi, DWORD & _dwSNR, BOOL & _fUse )

PURPOSE: ���� ������ ������.

PARMATERS:	
	WORD _wIdx		: �迭 �ε���.
	DWORD & _dwPRN	: ���۷���. Satellite ID.
	DWORD & _dwElev : ���۷���. ��(Elevation). ������ degrees. ( 00-90 )
	DWORD & _dwAzi	: ���۷���. ������(Azimuth). ������ degrees.	( 000-359 )
	DWORD & _dwSNR	: SNR - higher is better ( 00-99dB, null when not tracking ) 
	BOOL & _fUse	: ������� ��������..

RETURN: 
	TRUE : ����
	FALSE : ����

HISTORY:   Kevin Cho	2005.07.21		ó������ ����

-----------------------------------------------------------------------------*/
BOOL	CGPS::getSatellite  ( WORD _wIdx, DWORD & _dwPRN, DWORD & _dwElev, DWORD & _dwAzi, DWORD & _dwSNR, BOOL & _fUse )
{
	//if ( (_wIdx < 0) || (_wIdx >= MAX_SATELLITE_NUM) || _wIdx >= getNumSV())
	if (_wIdx >= getNumSV())
	{
		_dwPRN = NULL;
		_dwSNR = NULL;
		_fUse = FALSE;
		
		return FALSE;
	}

	stSatellite* p = &(m_GPSInfo.satellite[_wIdx]) ;

	_dwPRN		= p->dwPRN ;
	_dwElev		= p->dwElev ;
	_dwAzi		= p->dwAzi ;
	_dwSNR		= p->dwSNR ;	// SNR - higher is better ( 00-99dB, null when not tracking ) 
	_fUse		= p->fUse ;

	return TRUE ;
}

DWORD CGPS::resetGPS ( DWORD _cmd )
{
	char cmd[1024];
	strcpy ( cmd, "" ) ;

	switch ( _cmd )
	{
	case GPS_RESET_COLD :
		strcpy(cmd, "$PSRF101,0,0,0,96000,0,0,12,4*1f\r\n");
		break ;
	case GPS_RESET_WARM :
		strcpy(cmd, "$PSRF101,0,0,0,96000,0,0,12,2*19\r\n");
		break ;
	case GPS_RESET_HOT :
		strcpy(cmd, "$PSRF101,0,0,0,96000,0,0,12,0*1b\r\n");
		break ;
	default :
		break ;
	}

	writeComm ( cmd, strlen(cmd) ) ;
	
	return 1 ;
}


DWORD CGPS::getGPSStat ()
{
	if ( !isConnected() )	return GPS_STAT_NO_SIG ;
	if ( !isGPSOK() )		return GPS_STAT_BAD ;

	WORD nSatellite = getNumSV () ;

	if ( nSatellite <= 3 )
	{
		return GPS_STAT_WEEK ;
	}
	else if ( nSatellite <= 5 )
	{
		return GPS_STAT_NORMAL ;
	}
	else if ( nSatellite >= 7 )
	{
		return GPS_STAT_GOOD ;
	}

	return -1 ;
}

BOOL CGPS::process(const char* szData, UINT len)
{
	if(!szData || len < 1) return FALSE;

#if GPS_LOG && COMM_LOG
	//! Save GPS Log data
	logData ( (char*)szData, len) ;
#endif

	BOOL bRetVal = FALSE;

	DWORD idx =0;

	while(idx < len)
	{
		if(*szData ==  ASCII_CR || *szData == ASCII_LF)
		{
			//! to-do checksum()
			if(m_bufCount > 20)
			{
				if (parseNMEA(m_szBuff, m_bufCount) == COMM_GPRMC)	//! ���������� RMC������ MM������ 
					bRetVal = TRUE;
			}
			
			m_bufCount = 0;
		}
		else
		{
			if(*szData == ASCII_DOLLOR) m_bufCount = 0;
			m_szBuff[m_bufCount++] = *szData;
			
			if(m_bufCount >= sizeof(m_szBuff))
			{
				//! �Ϲ������� NMEA����Ÿ�� ���̴� 80�� �����̹Ƿ� ������ ũ��� �̺��ٴ� Ŀ���Ѵ�.
				//! �̷� ��찡 �߻��Ѵٸ� �̰��� �Ľ̿����Ǵ� ����Ÿ ��ü�� ����
				m_bufCount = 0;
			}
		}
		
		szData++;
		idx++;
	}

	if (bRetVal)
		GPS_UpdatePowerOn();

	return bRetVal;
}

CBeagleTime CGPS::GetBeagleGpsTime ()
{
	CBeagleTime oTime;
	oTime.Set (m_GPSInfo.yy, m_GPSInfo.mon, m_GPSInfo.dd, m_GPSInfo.hh, m_GPSInfo.min, m_GPSInfo.ss);

	return oTime;
}

BOOL CGPS::chkeckValid()
{
	m_validCount--;
	if(m_validCount < 0) m_validCount = 0;

	if(m_validCount == 0)
	{
		m_GPSInfo.dwSpeed = 0;
		m_GPSInfo.dwHDOP  = 99 * 100 ;
		m_GPSInfo.dwVDOP  = 99 * 100 ;
		m_GPSInfo.dwPDOP  = 99 * 100 ;
		m_GPSInfo.wGPSFix = 0;
	}

	return m_validCount != 0;
}

void CGPS::setValid()
{
	m_validCount = 30;
}

BOOL CGPS::getPowerOn ()
{
	return m_fPowerOn ;
}

BOOL CGPS::setPowerOn ( BOOL _fPowerOnPos )
{
	m_fPowerOn = _fPowerOnPos ;

	return getPowerOn () ;
}

BOOL CGPS::writeComm ( char * _buff, int _len )
{
	if ( !_buff )						return FALSE ;
	if ( _len < 1 )						return FALSE ;
	if ( _len >= MAX_WRITE_BUFFER )		return FALSE ;

	memset ( m_szBuffWrite, 0x00, MAX_WRITE_BUFFER ) ; 
	memcpy ( m_szBuffWrite, _buff, _len ) ;

	CCommGPS::writeComm ( m_szBuffWrite, _len ) ;

	return FALSE ;
}

char * CGPS::getTimeStamp ()
{
	return m_GPSInfo.time_stamp	;
}

// Road Speed Limited(���Ӿ˸�)���� �ֱ� 3�ʵ����� ��� �ӵ��� �������� �Լ�
double CGPS::getAverageSpeed3Sec ()
{	
	int num =  m_avgSpeed_kmh_in_3sec.size() ;
	if( num < AVRSPEED_KMH_IN_3SEC)	//�ӵ��� 3���̻� ��������... ����ӵ�
		return getSpeed_kmh() ;
	
	double avrSpeed = 0 ;
	double * Speed ;

	//EnterCriticalSection ( &m_cs ) ;	//Lock

	for ( int ii = 0 ; ii < num ; ++ii)
	{
		Speed = m_avgSpeed_kmh_in_3sec.get(ii);
		avrSpeed += (*Speed) ;
	}

	//LeaveCriticalSection ( &m_cs ) ;	//unLock

	return avrSpeed / (double)num ;
}
