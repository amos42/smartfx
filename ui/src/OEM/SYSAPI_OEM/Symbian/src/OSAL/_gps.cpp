//#include "stdafx.h"
//#include "inc/tagdefine.h"
//#include "inc/OperateDefine.h"
//#include "gps/commLib.h"
#include "SYSAPI.h"

#include <stdlib.h>
#include <time.h>

#include "_gps.h"
//#include "base/token.h"

//#include "base/threadinterface.h"


#if defined(_AFX) && defined(_DEBUG)
# define new DEBUG_NEW
# undef THIS_FILE
  static char THIS_FILE[] = __FILE__;
#endif

#define		GPS_OFF_IS_MM_OFF		0
#define     GPS_FIX_BY_GSA_ONLY     0   // GSA::Mode2 정보만을 이용하여 
									    // GPS Fix결정 
//#define		AVRSPEED_KMH_IN_3SEC	3	// 차량의 3초간 평균시속

// Thread proc
//DWORD readGPSProc (LPVOID pThread, LPVOID lpParameter)
/*
static void readGPSProc(SYS_TASK hTask, void *lpParameter)
{
	ASSERT(lpParameter);

	CGPS* pGPS = (CGPS*)lpParameter;

	if (!pGPS->isConnected())
	{
		if(!pGPS->connect()) return;
	}

	stGPSInfo oGPSDat;

	while (SYSAPI_IsTaskActivate(hTask))
	{
//		if (SYSAPI_WaitEvent(hTask, 0x0010)) break;

//		_callback( pGPS->isGPSOK(), pGPS->getLon(), pGPS->getLat(), pGPS->getAlt(), pGPS->getSpeed_kmh(), pGPS->getAngle() );
		
//		if (!GPS_IsLogOpend())
		{
			if (pGPS->GetGPSData(&oGPSDat)){
//				GPS_PushRMCData(pGPS->isGPSOK(), pGPS->getLon(), pGPS->getLat(), pGPS->getAlt(), pGPS->getSpeed_kmh(), pGPS->getAngle());
				pGPS->callback( (void *)pGPS, pGPS->isGPSOK(), pGPS->getLon(), pGPS->getLat(), pGPS->getAlt(), pGPS->getSpeed_kmh(), pGPS->getAngle() );
			}
		}

		SYSAPI_Sleep(500);
	}

	pGPS->disconnect();

	return;
}
*/

//  5130.7106 -> 51도 30.7106분 -> 51.51184333.. 도 
inline double NMEA2LL(double nn)
{
	ASSERT(nn >= 0);

	atWORD	deg ;

	nn /= 100.0 ;
	deg = (atWORD) nn ;
	nn -= deg ; 
	return deg + ( (nn * 10.0)/6.0 ) ;
}

CGPS::CGPS ( void )
{
	memset ( &m_GPSInfo,	0x00, sizeof(m_GPSInfo) ) ;
	m_GPSInfo.dblHDOP = 100;
	m_GPSInfo.dblVDOP = 100;
	m_GPSInfo.dblPDOP = 100;

//	NMEA_ResetGPSInfo( &m_GPSInfo );

	m_bufCount = 0;

	//InitializeCriticalSection ( &m_cs ) ;
//	setPowerOn ( FALSE ) ;

	//PORT(m_CommInfo)           = GPS_PORT ;
	//BAUD(m_CommInfo)           = GPS_BAUD ;

	m_validCount = 0;

//	m_token.InitMaxCnt(20);

	m_iTimeCount = 0;	//Automatic TimeZone Setting을 위해 시간을 재는 변수
	bGPSFixed = FALSE;
	
//	m_avgSpeed_kmh_in_3sec(AVRSPEED_KMH_IN_3SEC)

	m_bConnected = FALSE;
	
	m_hLog = atNULL;
	m_pLogBuf = atNULL;
	m_nLogBuf = 0;
	m_bLogEnable = atFALSE;
	m_bLoging = atFALSE;
}

CGPS::~CGPS ( void )
{
	//DeleteCriticalSection ( &m_cs ) ;
//	setPowerOn ( atFALSE ) ;

	close();
}

atBOOL CGPS::open(atUINT port, atUINT baudRate)
{
//	setPowerOn ( atFALSE ) ;

	if(port & baudRate) // for compiler warning
	{
	  ;
	}

	// Thread Start
//	if(!THREAD_Start(THREAD_INDEX_GPS, readGPSProc, this))//GPS_GetInstance()))
//	m_hGPSThread = SYSAPI_CreateTask(_AT("GPSThread"), readGPSProc, this, atTRUE, 16*1024); //GPS_GetInstance()))
	
	return atTRUE;	
}

atBOOL CGPS::connect()
{
	if (m_bConnected) return TRUE;

	User::LeaveIfError(server.Connect());
	CleanupClosePushL(server);
	User::LeaveIfError(positioner.Open(server)); // use default positioning module
	CleanupClosePushL(positioner);

	_LIT(KRequestor,"LocationExample");
	User::LeaveIfError(positioner.SetRequestor( CRequestor::ERequestorService ,
												CRequestor::EFormatApplication , 
												KRequestor ) );
	
	// partial update(GPS신호중 시간 정보만이라도 일단 받아 들임) 설정
	TPositionUpdateOptions option;
	positioner.GetUpdateOptions(option);
	option.SetAcceptPartialUpdates(ETrue);
	option.SetUpdateInterval(GPS_DATA_INTERVAL * 100);
	option.SetUpdateTimeOut(0);
	positioner.SetUpdateOptions(option);

	_LIT(KCntPhone, "+358501234567");
	_LIT(KSrvName, "Tornado");
	phContact = CRequestor::NewLC(CRequestor::ERequestorContact,
								  CRequestor::EFormatTelephone,
								  KCntPhone);
	RequestStack.Append(phContact);

	phService = CRequestor::NewLC(CRequestor::ERequestorService,
								  CRequestor::EFormatApplication,
								  KSrvName);
	RequestStack.Append(phService);
	User::LeaveIfError(positioner.SetRequestor(RequestStack));
			
	server.GetDefaultModuleId(aModuleId);
			
	m_bConnected = TRUE;
	
	return m_bConnected;
}

atBOOL CGPS::disconnect()
{
	// Pop Clean Up RequestStack
	//RequestStack.Remove(phService);
	//RequestStack.Remove(phContact);

	RequestStack.Reset();//ResetAndDestroy();
	
	CleanupStack::PopAndDestroy(phService); // this will call Close() method
	CleanupStack::PopAndDestroy(phContact); // this will call Close() method	

	positioner.CancelRequest(EPositionerNotifyPositionUpdate);
	server.CancelRequest(EPositionerNotifyPositionUpdate);
	positioner.Close();
	server.Close();
	  
	CleanupStack::PopAndDestroy(&positioner); // this will call Close() method
  	CleanupStack::PopAndDestroy(&server); // this will call Close() method
	  
	m_bConnected = atFALSE;

	return atTRUE;
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
atBOOL CGPS::close()
{
//	setPowerOn ( atFALSE ) ;
	
	//THREAD_Stop(THREAD_INDEX_GPS); //Destroy 에 Stop이 존재함
	
//	THREAD_Destroy(THREAD_INDEX_GPS);
//	SYSAPI_DestroyTask( m_hGPSThread );

  return atTRUE;
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
/*
atDWORD	CGPS::parseNMEA (const char* data, atDWORD len )
{
	atDWORD dwRetVal = NMEA_ParseNMEA( &m_GPSInfo, data, len );

	if( dwRetVal == COMM_GPRMC )
	{
//		if( m_GPSInfo.dwSpeed != 0 )
//			m_avgSpeed_kmh_in_3sec.push_top( m_GPSInfo.dwSpeed ) ;
//		ApplyAutoTimeZone();
//#if GPS_SIMUL_ENABLE
//		if (!GPS_IsSuspend())
//#endif
//			GPS_PushRMCData(isGPSOK(), getLon(), getLat(), getAlt(), getSpeed_kmh(), getAngle());
	}

	return dwRetVal ;
}
*/

/*
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
*/

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
atBOOL	CGPS::addSatellite ( atDWORD _dwPRN, atDWORD _dwElev, atDWORD _dwAzi, atDWORD _dwSNR, atBOOL _fUse ) // 위성정보 추가
{
	if ( (_dwPRN < 1) || (_dwPRN > 32) )
		return atFALSE ;

	if ( m_GPSInfo.wSV >= MAX_SATELLITE_NUM )
		return atFALSE ;

	int	idx = m_GPSInfo.wSV ;
	if(idx >= MAX_SATELLITE_NUM ) return atFALSE;
	

	m_GPSInfo.satellite[idx].dwPRN  = _dwPRN ;
	m_GPSInfo.satellite[idx].dwElev = _dwElev ;
	m_GPSInfo.satellite[idx].dwAzi  = _dwAzi ;
	m_GPSInfo.satellite[idx].dwSNR  = _dwSNR ;
	m_GPSInfo.satellite[idx].fUse   = _fUse ;

	++m_GPSInfo.wSV ;


	return atTRUE ;
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
atBOOL	CGPS::setSatellite ( atDWORD _dwPRN, atDWORD _dwElev, atDWORD	_dwAzi, atDWORD _dwSNR )
{
	if ( (_dwPRN < 1) || (_dwPRN > 32) )
		return atFALSE ;


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
atBOOL	CGPS::setSatelliteUse ( atDWORD _dwPRN, atBOOL _fUse )
{
	if ( (_dwPRN < 1) || (_dwPRN > 32) )
		return atFALSE ;
	
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
atBOOL	CGPS::resetSatelliteAll ( void )
{
	memset ( m_GPSInfo.satellite, 0x00, sizeof(_GPSSatellite) * MAX_SATELLITE_NUM ) ;

	return atTRUE ;
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
atBOOL	CGPS::getCoord ( double & _lon, double & _lat, double & _alt ) // 좌표
{
	_lon = getLon () ;
	_lat = getLat () ;
	_alt = getAlt () ;
	
	return atTRUE ;
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
atBOOL	CGPS::getTime ( atWORD & _hh, atWORD & _mm, atWORD & _ss ) // 시간
{
	_hh = m_GPSInfo.gps_time.nHour ;
	_mm = m_GPSInfo.gps_time.nMinute ;
	_ss = m_GPSInfo.gps_time.nSec ;

	return atTRUE ;
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
atBOOL	CGPS::getDate ( atWORD & _yy, atWORD & _mm, atWORD & _dd ) // 날짜
{
	_yy = m_GPSInfo.gps_time.nYear ;
	_mm = m_GPSInfo.gps_time.nMonth ;
	_dd = m_GPSInfo.gps_time.nDay ;

	return atTRUE ;
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

	//double	dblSpeed = m_GPSInfo.dwSpeed / 100.0 ;	// 속도 (knot) ( 원래값 * 100 ) ;

	// 속도 변환 ( knot -> km/h )
	//1m/s -> 1.943846 knots
	//1knots ->1.851998 km/h

	//return dblSpeed * 1.851998 ;
	return m_GPSInfo.dwSpeed; 
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
atBOOL	CGPS::isGPSOK ( void ) // GPS 데이터가 믿을만 한지 ..
{
/*
	if (GPS_IsLogOpend()) // LOG로부터 읽어들일때는  NMEA field parsing값으로 판단하는 기존의 방식 
	{
		if( m_GPSInfo.wGPSFix != 0             //// GPS quality indicator (0=invalid; 1=GPS fix; 2=Diff. GPS fix)
			&& m_GPSInfo.wDataStat == 'A'		//// Data status (V=navigation receiver warning)
			&& m_GPSInfo.wFixMode > 1)			// 1 : Fix not available , 2: 2D,  3: 3D
			return atTRUE;
		else
			return atFALSE;
	}	
	else */ //symbian OS에서 gps data를 얻고 있을때는 아래와 같이 판단
	{
/*	
		if( bGPSFixed
			&& m_GPSInfo.wGPSFix >= 2
			&& m_GPSInfo.wDataStat =='A' )
*/
		if( bGPSFixed && m_GPSInfo.nFixState && m_GPSInfo.nQuality > 0 )
			return atTRUE;
	}
	
	return atFALSE;
//	return bGPSFixed;
}


/*-----------------------------------------------------------------------------

FUNCTION: getNumS? (void)

PURPOSE: 위성갯수 리턴 ( SV : Satellites in View, SU : Satellites in Using )

PARMATERS:	

RETURN: 위성갯수

HISTORY:   Kevin Cho	2005.07.21		처음으로 생성

-----------------------------------------------------------------------------*/
atWORD	CGPS::getNumSV ( void ) // 보이는 위성 갯수
{
	return m_GPSInfo.wSV ;
}


atWORD	CGPS::getNumSU ( void ) // 사용중인 위성 갯수 
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
	// Tornado 에서 셋팅이 불가하다.
	// 어차피 사용하지도 않는다.
	return 0;//( m_GPSInfo.dwPDOP / 100.0 ) ;
}


double	CGPS::getHDOP ( void ) // Horizontal Dilution Of Precision
{
	return m_GPSInfo.dblHDOP;
}

double	CGPS::getVDOP ( void ) // Vertical Dilution Of Precision
{
	return m_GPSInfo.dblVDOP;
}

double	CGPS::getElev ( void ) 		// Elevation
{
	int iMax = getNumSV();
	for (int nn = 0; nn<iMax ; nn++)
	{
		_GPSSatellite	* p = &(m_GPSInfo.satellite[nn]) ;

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
atBOOL	CGPS::getSatellite  ( atWORD _wIdx, atDWORD & _dwPRN, atDWORD & _dwElev, atDWORD & _dwAzi, atDWORD & _dwSNR, atBOOL & _fUse )
{
	//if ( (_wIdx < 0) || (_wIdx >= MAX_SATELLITE_NUM) )
	if ( _wIdx >= MAX_SATELLITE_NUM )
		return atFALSE ;

	if ( _wIdx >= getNumSV() )
		return atFALSE ;

	_GPSSatellite	* p = &(m_GPSInfo.satellite[_wIdx]) ;

	_dwPRN		= p->dwPRN ;
	_dwElev		= p->dwElev ;
	_dwAzi		= p->dwAzi ;
	_dwSNR		= p->dwSNR ;	// SNR - higher is better ( 00-99dB, null when not tracking ) 
	_fUse		= p->fUse ;

	return atTRUE ;
}

atDWORD CGPS::getGPSStat ()
{
#ifdef GPS_PORTING //별로 사용하는데가 없음
	if ( !isGPSOK() )		return GPS_STAT_BAD ;

	atWORD nSatellite = getNumSV () ;

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
#endif
	return 0 ;
}

/*
CBeagleTime CGPS::GetBeagleGpsTime ()
{
	CBeagleTime oTime;
	oTime.Set (m_GPSInfo.yy, m_GPSInfo.mon, m_GPSInfo.dd, m_GPSInfo.hh, m_GPSInfo.min, m_GPSInfo.ss);

	return oTime;
}
*/

/*-----------------------------------------------------------------------------

FUNCTION: readCommProc (LPVOID)

PURPOSE: Thread function controls comm port reading, comm port status 
         checking, and status messages.

PARMATERS:
    lpV - 4 byte value contains the tty child window handle

RETURN: always 1

COMMENTS: Waits on various events in the applications to handle
          port reading, status checking, status messages and modem status.

HISTORY:   Date:      Author:     Comment:
           10/27/95   AllenD      Wrote it
           11/21/95   AllenD      Incorporated status message heap

-----------------------------------------------------------------------------*/
atBOOL CGPS::chkeckValid()
{
	//CAutoLock lock(&m_cs_val);
	m_validCount--;
	if(m_validCount < 0) m_validCount = 0;

	if(m_validCount == 0){
		m_GPSInfo.dwSpeed = 0;
		m_GPSInfo.dblHDOP  = 99;
		m_GPSInfo.dblVDOP  = 99;
		m_GPSInfo.dblPDOP  = 99;
		m_GPSInfo.nFixState = 0;
		m_GPSInfo.nQuality = 0;
		m_GPSInfo.n3DState = 0;

		// Kevin Cho 2007.03.23 {
//#if (GPS_OFF_IS_MM_OFF)
//		// GPS 상태가 안좋아지면 MM 결과를 리셋한다..
//		setPowerOn ( FALSE ) ;
//		if ( stGlobalData::instance()->pMM )
//		{
//			stGlobalData::instance()->pMM->setOnRoad ( FALSE ) ;
//
//			stGlobalData::instance()->pMM->clearCandiOff () ;		// delete off-road candidates links
//			stGlobalData::instance()->pMM->clearCandiLink () ;		// delete on-road next candidates links
//			stGlobalData::instance()->pMM->clearPastLink  () ;		// delete on-road past links
//			stGlobalData::instance()->pMM->clearProhibitLink () ;	// delete on-road prohibited links ..
//
//			stGlobalData::instance()->pMM->resetMMResult () ;
//		}
//#endif
		// } Kevin Cho 2007.03.23 
	}

	return m_validCount != 0;
}


void CGPS::setValid()
{
	//CAutoLock lock(&m_cs_val);
	m_validCount = 30;
}

/*
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
*/

atBOOL CGPS::GetGPSData(_GPSInfo* pGPSDat)
{
	// log file에서 읽는 경우
	//if (m_hReadGpsLog) return FALSE;

	TPositionSatelliteInfo courseInfo;	
	positioner.NotifyPositionUpdate(courseInfo, status);
	User::WaitForRequest(status);	
	
	courseInfo.GetPosition(position);

	int stat = status.Int();
	bGPSFixed = (stat == KErrNone) ? TRUE: FALSE;
	
	if (!bGPSFixed)
	{
		memset(&m_GPSInfo, atNULL, sizeof(_GPSInfo));
		return FALSE;
	}

	TPositionModuleStatus aPosModuleStatus;
	server.GetModuleStatus(aPosModuleStatus,aModuleId);
	
	int DeviceStatus = aPosModuleStatus.DeviceStatus();
	int DataStatus = aPosModuleStatus.DataQualityStatus();	
	
//	m_GPSInfo.wGPSFix = 0;
	
	switch(DeviceStatus)
	{
	case TPositionModuleStatus::EDeviceActive:
		m_GPSInfo.nFixState = 1;
		m_GPSInfo.n3DState = 3;
		break;
	case TPositionModuleStatus::EDeviceReady:
		m_GPSInfo.nFixState = 1;
		m_GPSInfo.n3DState = 3;
		break;
	case TPositionModuleStatus::EDeviceInitialising:
	case TPositionModuleStatus::EDeviceStandBy:
		m_GPSInfo.nFixState = 0;
		m_GPSInfo.n3DState = 1;
		break;
	case TPositionModuleStatus::EDeviceUnknown:
	case TPositionModuleStatus::EDeviceError:
	case TPositionModuleStatus::EDeviceDisabled:
	case TPositionModuleStatus::EDeviceInactive:
	default:
		m_GPSInfo.nFixState = 0;
		break;
	}
	
//	m_GPSInfo.nQuality = 0;
		
	switch(DataStatus)
	{
	case TPositionModuleStatus::EDataQualityUnknown:
	case TPositionModuleStatus::EDataQualityLoss:
		m_GPSInfo.nQuality = 0;
		break;	
	case TPositionModuleStatus::EDataQualityPartial:
		m_GPSInfo.nQuality = 1;
		break;
	case TPositionModuleStatus::EDataQualityNormal:
		m_GPSInfo.nQuality = 2;
		break;
	default:
		m_GPSInfo.nQuality = 0;
		break;
	}

	TTime time = position.Time();
	TDateTime dtime = time.DateTime();
	
	m_GPSInfo.gps_time.nYear = dtime.Year();
	m_GPSInfo.gps_time.nMonth = dtime.Month();
	m_GPSInfo.gps_time.nDay = dtime.Day();
	m_GPSInfo.gps_time.nHour = dtime.Hour();
	m_GPSInfo.gps_time.nMinute = dtime.Minute();
	m_GPSInfo.gps_time.nSec = dtime.Second();
	 
	setValid();
		
	TCourse course;
	courseInfo.GetCourse(course);
	
	double dLat = position.Latitude();
	double dLon = position.Longitude();

	/*
	if (dLat != 0. || dLon != 0.)
	{
		m_GPSInfo.dblLonPrev = m_GPSInfo.dblLon;
		m_GPSInfo.dblLatPrev = m_GPSInfo.dblLat;
		m_GPSInfo.dblAltPrev = m_GPSInfo.dblAlt;
		m_GPSInfo.dwAnglePrev = m_GPSInfo.dwAngle;
	}
	m_GPSInfo.dwTick = SYSAPI_GetCurrTime();
	*/

	m_GPSInfo.dwSpeed = (atDWORD)(course.Speed() * 3600. / 1000.); // m/s 를 km/h로 전환
	m_GPSInfo.dwAngle = (atDWORD) course.Heading();
	m_GPSInfo.dblLat = dLat;
	m_GPSInfo.dblLon = dLon;
	m_GPSInfo.dblAlt = position.Altitude();
	
	//m_GPSInfo.wFixMode = 2;	// 2D
	//m_GPSInfo.wFixMode = 3;		// 3D
	
	m_GPSInfo.wSV = courseInfo.NumSatellitesInView();
	m_GPSInfo.wSU = courseInfo.NumSatellitesUsed();

	for (int i=0; i<m_GPSInfo.wSV; i++)
	{
		TSatelliteData SatData;
		courseInfo.GetSatelliteData(i, SatData);

		m_GPSInfo.satellite[i].dwAzi = (atDWORD)SatData.Azimuth();
		m_GPSInfo.satellite[i].dwElev = (atDWORD)SatData.Elevation();
		m_GPSInfo.satellite[i].fUse = SatData.IsUsed();
		m_GPSInfo.satellite[i].dwPRN = SatData.SatelliteId();
		m_GPSInfo.satellite[i].dwSNR = SatData.SignalStrength();		
	}	
	
	// Rainbow와 HDOP&VDOP 맞추기위해 10을 나눴지만 정확하지 않음. 튜닝이 필요함
	m_GPSInfo.dblHDOP = double(courseInfo.HorizontalDoP()/10);
	m_GPSInfo.dblVDOP = double(courseInfo.VerticalDoP()/10);
	//m_GPSInfo.dwPDOP = //심비안에서 Position Dilution of Precision 을 가져올 방법은... 
	m_GPSInfo.dblPDOP = 0;

	if (pGPSDat) memcpy(pGPSDat,&m_GPSInfo,sizeof(m_GPSInfo));

//	GPS_UpdatePowerOn();

	return atTRUE;
}

