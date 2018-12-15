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
#define     GPS_FIX_BY_GSA_ONLY     0   // GSA::Mode2 정보만을 이용하여 
									    // GPS Fix결정 
#define		AVRSPEED_KMH_IN_3SEC	3	// 차량의 3초간 평균시속

//extern GPSPARAM g_objGPSParam;

//  5130.7106 -> 51도 30.7106분 -> 51.51184333.. 도 
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

	m_iTimeCount = 0;	//Automatic TimeZone Setting을 위해 시간을 재는 변수
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
	char * _pBuff		: 원래 데이터 포인터
	DWORD  _nFieldIdx	: 원하는 필드 index 
	char * _pField		: 필드 데이터 포인터
	DWORD  _nFieldLen	: 필드 데이터 길이

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
			   - GPS 데이타를 파라미터로 넘김 
			   - 여러 NMEA데이타 군이 넘어와 내부적으로 NMEA를 분리하는 방식에서
			     외부에서 NMEA프로토콜별로 분리하여, 하나의 프로토콜만을 넘김.
			   - 이에따라 GPS로그를 외부에서 저장. 
			   - RMC에서만 M/M수행 
-----------------------------------------------------------------------------*/
DWORD CGPS::parseNMEA (const char* data, DWORD len )
{
	EnterCriticalSection ( &m_cs ) ;

	char buff[MAX_PARSE_BUFFER+10];
	memcpy(buff, data, len);
	buff[len] = '\0';

	// 데이터의 형태는 다음과 같다..
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
#if defined(WIN_CE) || (GPS_TIME_TEST) // 패트릭 2007.09.13 { TimeZone Automatic Setting
		
#if (GPS_TIME_TEST)
	#pragma message("임시로 PC버전에서 돌아가게 함 : PC시간이 바뀜으로 매우 주의해야 함.")
#endif

	//TimeZone 설정이 Auto 일때 만 작동한다.... 
	stSettingTimeZone settingTimeZone;
	GD_GetSettingTimeZone(&settingTimeZone);
	if (!settingTimeZone.bAutoSetting) return;
	
	if (m_iTimeCount < 0 || m_iTimeCount > 60 ) // [2008.8.27 Adolph] 카운트가 초기화 되야 다시 동기화 가능해진다
		m_iTimeCount = 0;

	if (m_iTimeCount == 0) //카운트 된 시간이 1초일때...(10초마다 한번씩 재설정)
	{
		BOOL fOldPosOk = FALSE;  // 예전차량 위치가 정상값이냐?
		BOOL fLenOk = FALSE;	 // 차량이 1분동안 정해진 거리를 갔나? 
		
		ipoint2 carPos = GD_GetCarPos();

		if((settingTimeZone.ptOldCarPos.x != 0 && settingTimeZone.ptOldCarPos.y != 0)) // 예전차량위치가 믿을만하면 거리계산
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
			// 차량위치가 믿을만 하지 못하면 위치를 받는다...
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

			// config.cfg 에 저장하는 부분.
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

	if ( !getField ( _szRMC, 0, szField ) )	// $GPXXX 아이디 필드를 가져올 수 없으면 깨진 데이터 ..
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
		m_GPSInfo.dblLat = NMEA2LL(atof ( szField ));	// 도 단위로 변경 2006.5.15
		// 4807.038,N   Latitude 48 deg 07.038' N
	}
	else
	{
		m_GPSInfo.dblLat = 0 ;
	}

	//		4    = N or S
	if ( getField(_szRMC, 4, szField) )
	{
		// m_GPSInfo.dblLat 를  trans..
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
		m_GPSInfo.dblLon = NMEA2LL(atof ( szField ));  // 도 단위로 변경 

		//01131.000,E  Longitude 11 deg 31.000' E
	}
	else
	{
		m_GPSInfo.dblLon = 0 ;
	}


	//		6    = E or W
	if ( getField(_szRMC, 6, szField) )
	{
		// m_GPSInfo.dblLon 를  trans..
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
		// knot : 항해·항공용 속도 단위. 1시간당 1해리(海里, 1852m)의 속도를 1노트라 한다
		double knot = atof ( szField ) ;
		m_GPSInfo.dwSpeed = (DWORD) (knot * 100) ;	// double 형태를 DWORD로 저장한다.

		// getSpeed() 함수에서 knot를 km 로 바꾸어 리턴하기 때문에
		// 여기서 knot->km 할 필요는 없다..

		//double kmh  = KNOTS_TO_KMH(knot) ;	// knot 를 km/h 로 변환
		//m_GPSInfo.dwSpeed = (DWORD) (kmh * 100) ;	// double 형태를 DWORD로 저장한다.


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

	if ( !getField ( _szGGA, 0, szField ) )	// $GPXXX 아이디 필드를 가져올 수 없으면 깨진 데이터 ..
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
		// szField 가 'M'으로 넘어오면 미터단위..
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
//		위성 갯수(SV)가 4의 배수가 아닌 경우 마지막 데이터는 짧아진다.
//		ex) $GPGSV,4,4,13,14,13,172,05*67 


	if ( !checksumGPS(_szGSV, _nLen) ) 
		return FALSE ;

	DWORD	nTotalNumOfMsg, nMsgNum =0;
	char	szField[MAX_FIELD_BUFFER + 10] ;

	memset ( szField, 0x00, sizeof(szField) ) ;

	if ( !getField ( _szGSV, 0, szField ) )	// $GPXXX 아이디 필드를 가져올 수 없으면 깨진 데이터 ..
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

	if ( !getField ( _szGSA, 0, szField ) )	// $GPXXX 아이디 필드를 가져올 수 없으면 깨진 데이터 ..
	{
		//m_GPSInfo.wFixMode  =0;
		return FALSE ;
	}

	// Checksum까지 필드가 존재하는지 체크 
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
	// HDOP는 GGA에서 ....

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
	
	// checksum : * 뒤의 값. Hex format
	//			: $~* 사이의 값들의 XOR 값..

	char	xor = 0x00 ;	// $~* 사이의 값들을 XOR 연산 하기 위한 변수
	
	for ( DWORD ii = 0 ; ii < _nLen ; ++ii )
	{
		if ( _szData[ii] == ASCII_DOLLOR )
		{
			// do nothing..
		}
		else if ( _szData[ii] == ASCII_ASTERISK )
		{
			++ii ;
			break ;		// * 이면 다음은 checksum ..
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
		check -= ('A' - 10) ;		// check = check - 'A' + 10 ;	// 10진수로 만들기 위해서 ....
	else 
		check = 0 ;


	checksum = check << 4 ;			// 상위 4 비트로 시프트 ..


	check = _szData[ii] ;

	if ( ('0' <= check) && (check <= '9') )
		check -= '0' ;
	else if ( ('A' <= check) && (check <= 'F') )
		check -= ('A' - 10) ;		// check = check - 'A' + 10 ;	// 10진수로 만들기 위해서 ....
	else 
		check = 0 ;


	checksum |= check ;				// 하위 4 비트



	if ( checksum == xor )
		return TRUE ;

	return FALSE ;
}




/*-----------------------------------------------------------------------------

FUNCTION: addSatellite ( DWORD _dwPRN, DWORD _dwElev, DWORD _dwAzi, DWORD _dwSNR = 0, BOOL _fUse = FALSE )

PURPOSE: 위성 정보를 추가함.

PARMATERS:
		DWORD _dwPRN  : 4바이트. GPS ID == Satellite PRN number ( 1-32 )
		DWORD _dwElev : 4바이트. 고도(Elevation). 단위는 degrees	( 00-90 )
		DWORD _dwAzi  : 4바이트. 방위각(Azimuth). 단위는 degrees	( 000-359 )
		DWORD _dwSNR  : 4바이트. SNR - higher is better ( 00-99dB, null when not tracking ) 
		BOOL  _fUse   : 4바이트. 위성 사용 여부

RETURN: TRUE : 추가에 성공
		FALSE : 추가에 실패

HISTORY:   Kevin Cho	2005.07.21		처음으로 생성

-----------------------------------------------------------------------------*/
BOOL	CGPS::addSatellite ( DWORD _dwPRN, DWORD _dwElev, DWORD _dwAzi, DWORD _dwSNR, BOOL _fUse ) // 위성정보 추가
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

PURPOSE: 위성 정보를 세팅함. 사용 여부를 사용하는 fUse 는 제외

PARMATERS:
		DWORD _dwPRN  : 4바이트. GPS ID == Satellite PRN number ( 1-32 )
		DWORD _dwElev : 4바이트. 고도(Elevation). 단위는 degrees	( 00-90 )
		DWORD _dwAzi  : 4바이트. 방위각(Azimuth). 단위는 degrees	( 000-359 )
		DWORD _dwSNR  : 4바이트. SNR - higher is better ( 00-99dB, null when not tracking ) 

RETURN: TRUE : 세팅에 성공
		FALSE : 세팅에 실패

HISTORY:   Kevin Cho	2005.07.21		처음으로 생성

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

PURPOSE: 위성 사용 여부를 세팅함.

PARMATERS:
		DWORD _dwPRN : 4바이트. GPS ID == Satellite PRN number ( 1-32 )
		BOOL  _fUse  : 4바이트. 위성 사용여부

RETURN: TRUE : 세팅에 성공
		FALSE : 세팅에 실패

HISTORY:   Kevin Cho	2005.07.21		처음으로 생성

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

PURPOSE: 모든 위성 데이터를 초기화.

PARMATERS:

RETURN: TRUE : 초기화에 성공.
		FALSE : 초기화에 실패

HISTORY:   Kevin Cho	2005.07.21		처음으로 생성

-----------------------------------------------------------------------------*/
BOOL	CGPS::resetSatelliteAll ( void )
{
	memset ( m_GPSInfo.satellite, 0x00, sizeof(m_GPSInfo.satellite)) ;

	return TRUE ;
}




/*-----------------------------------------------------------------------------

FUNCTION: get??? (void)

PURPOSE: 경도/위도/고도를 리턴함..

PARMATERS:

RETURN: 경도/위도/고도

HISTORY:   Kevin Cho	2005.07.21		처음으로 생성

-----------------------------------------------------------------------------*/
double	CGPS::getLon ( void ) // 경도  (도 단위)
{
	return m_GPSInfo.dblLon ;
}


double	CGPS::getLat ( void )	// 위도 ( 도 단위)
{
	return m_GPSInfo.dblLat ;
}

double	CGPS::getAlt ( void ) // 고도
{
	return m_GPSInfo.dblAlt ;
}


/*-----------------------------------------------------------------------------

FUNCTION: getCoord ( double & _lon, double & _lat, double & _alt )

PURPOSE: 좌표를 리턴.

PARMATERS:
	double & _lon :	레퍼런스. 경도
	double & _lat : 레퍼런스. 위도 
	double & _alt : 레퍼런스. 고도

RETURN: always TRUE 

HISTORY:   Kevin Cho	2005.07.21		처음으로 생성

-----------------------------------------------------------------------------*/
BOOL	CGPS::getCoord ( double & _lon, double & _lat, double & _alt ) // 좌표
{
	_lon = getLon () ;
	_lat = getLat () ;
	_alt = getAlt () ;
	
	return TRUE ;
}



/*-----------------------------------------------------------------------------

FUNCTION: getTime (void)

PURPOSE: UTC 시간을 리턴..

PARMATERS:

RETURN: 시간 (UTC)

HISTORY:   Kevin Cho	2005.07.21		처음으로 생성

-----------------------------------------------------------------------------*/
//DWORD	CGPS::getTime ( void ) // 시간
//{
//	return m_GPSInfo.dwTime ; 
//}


/*-----------------------------------------------------------------------------

FUNCTION: getDate (void)

PURPOSE: 날짜를 리턴

PARMATERS:

RETURN: 날짜

HISTORY:   Kevin Cho	2005.07.21		처음으로 생성

-----------------------------------------------------------------------------*/
//DWORD	CGPS::getDate ( void ) // 날짜
//{
//	return m_GPSInfo.dwDate ; 
//}


/*-----------------------------------------------------------------------------

FUNCTION: getTime ( WORD & _hh, WORD & _mm, WORD & _ss )

PURPOSE: 시간을 겟..

PARMATERS:	
	WORD & _hh : 레퍼런스. hour.
	WORD & _mm : 레퍼런스. min.
	WORD & _ss : 레퍼런스. sec.

RETURN: always TRUE

HISTORY:   Kevin Cho	2005.07.21		처음으로 생성

-----------------------------------------------------------------------------*/
BOOL	CGPS::getTime ( WORD & _hh, WORD & _mm, WORD & _ss ) // 시간
{
	_hh = m_GPSInfo.hh ;
	_mm = m_GPSInfo.min ;
	_ss = m_GPSInfo.ss ;

	return TRUE ;
}


/*-----------------------------------------------------------------------------

FUNCTION: getDate ( WORD & _yy, WORD & _mm, WORD & _dd )

PURPOSE: 날짜를 겟..

PARMATERS:	
	WORD & _yy : 레퍼런스. year.
	WORD & _mm : 레퍼런스. mon.
	WORD & _dd : 레퍼런스. day.

RETURN: always TRUE

HISTORY:   Kevin Cho	2005.07.21		처음으로 생성

-----------------------------------------------------------------------------*/
BOOL	CGPS::getDate ( WORD & _yy, WORD & _mm, WORD & _dd ) // 날짜
{
	_yy = m_GPSInfo.yy ;
	_mm = m_GPSInfo.mon ;
	_dd = m_GPSInfo.dd ;

	return TRUE ;
}


/*-----------------------------------------------------------------------------

FUNCTION: getSpeed (void)

PURPOSE: 속도를 겟 ..

PARMATERS:	

RETURN: 
	double : 속도(km/h).
	-1 : 실패.

HISTORY:   Kevin Cho	2005.07.21		처음으로 생성

-----------------------------------------------------------------------------*/
double	CGPS::getSpeed_kmh ( void ) // 속도
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

	double	dblSpeed = m_GPSInfo.dwSpeed / 100.0 ;	// 속도 (knot) ( 원래값 * 100 ) ;

	// 속도 변환 ( knot -> km/h )
	//1m/s -> 1.943846 knots
	//1knots ->1.851998 km/h

	return dblSpeed * 1.851998 ;
}

/*-----------------------------------------------------------------------------

FUNCTION: getAngle(void)

PURPOSE: 진행 각도를 리턴..

PARMATERS:	

RETURN: 
	DWORD dwAngle : Trace Angle. ( 0 - 359 )


HISTORY:   Kevin Cho	2005.07.21		처음으로 생성

-----------------------------------------------------------------------------*/
long	CGPS::getAngle ( void ) // 진행 각도 (degree : 0 - 359)
{
	return m_GPSInfo.dwAngle ;
}


/*-----------------------------------------------------------------------------

FUNCTION: isGPSOK (void)

PURPOSE: GPS 데이터가 믿을만한지(활성화 되었는지) 여부를 리턴...

PARMATERS:	

RETURN: 
	TRUE : GPS가 활성화 되었음.
	FALSE ; GPS가 활성화 되지 않았음.

HISTORY:   Kevin Cho	2005.07.21		처음으로 생성

      2007.05.23
	   * isGPSOK 체크 항목 변경 
	     - HDOP체크 부분을 제거하고, 2D, 3D fixed항목 추가 
		   (2D 이상 fix되어야 유효한것으로 본다.)
		
-----------------------------------------------------------------------------*/
BOOL	CGPS::isGPSOK ( void ) // GPS 데이터가 믿을만 한지 ..
{
	if( m_GPSInfo.wGPSFix != 0             //// GPS quality indicator (0=invalid; 1=GPS fix; 2=Diff. GPS fix)
		&& m_GPSInfo.wDataStat == 'A')		//// Data status (V=navigation receiver warning)
		// GSA의 영향을 받는 부분으로 무시하여도 좋음
		//&& m_GPSInfo.wFixMode > 1)			// 1 : Fix not available , 2: 2D,  3: 3D
		return TRUE;
	else
		return FALSE;
}


/*-----------------------------------------------------------------------------

FUNCTION: getNumS? (void)

PURPOSE: 위성갯수 리턴 ( SV : Satellites in View, SU : Satellites in Using )

PARMATERS:	

RETURN: 위성갯수

HISTORY:   Kevin Cho	2005.07.21		처음으로 생성

-----------------------------------------------------------------------------*/
WORD	CGPS::getNumSV ( void ) // 보이는 위성 갯수
{
	return m_GPSInfo.wSV ;
}


WORD	CGPS::getNumSU ( void ) // 사용중인 위성 갯수 
{
	return m_GPSInfo.wSU ;
}


/*-----------------------------------------------------------------------------

FUNCTION: get?DOP (void)

PURPOSE:DOP(Dilusion Of Precision)값 리턴

PARMATERS:

RETURN: DOP

HISTORY:   Kevin Cho	2005.07.21		처음으로 생성

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

PURPOSE: 위성 정보를 가져옴.

PARMATERS:	
	WORD _wIdx		: 배열 인덱스.
	DWORD & _dwPRN	: 레퍼런스. Satellite ID.
	DWORD & _dwElev : 레퍼런스. 고도(Elevation). 단위는 degrees. ( 00-90 )
	DWORD & _dwAzi	: 레퍼런스. 방위각(Azimuth). 단위는 degrees.	( 000-359 )
	DWORD & _dwSNR	: SNR - higher is better ( 00-99dB, null when not tracking ) 
	BOOL & _fUse	: 사용중인 위성인지..

RETURN: 
	TRUE : 성공
	FALSE : 실패

HISTORY:   Kevin Cho	2005.07.21		처음으로 생성

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
				if (parseNMEA(m_szBuff, m_bufCount) == COMM_GPRMC)	//! 내부적으로 RMC에서만 MM수행함 
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
				//! 일반적으로 NMEA데이타의 길이는 80자 정도이므로 버퍼의 크기는 이보다는 커야한다.
				//! 이런 경우가 발생한다면 이것은 파싱오류또는 데이타 자체의 오류
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

// Road Speed Limited(과속알림)에서 최근 3초동안의 평균 속도를 가져오는 함수
double CGPS::getAverageSpeed3Sec ()
{	
	int num =  m_avgSpeed_kmh_in_3sec.size() ;
	if( num < AVRSPEED_KMH_IN_3SEC)	//속도를 3번이상 못받으면... 현재속도
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
