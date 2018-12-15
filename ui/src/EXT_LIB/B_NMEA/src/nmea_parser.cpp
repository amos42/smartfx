#include "GPS_Info.h"
#include "B_NMEA.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "_token.h"
#include "_heap.h"


#ifndef MAX_FIELD_BUFFER
	#define MAX_FIELD_BUFFER		 25
#endif
#ifndef MAX_PARSE_BUFFER		
	#define MAX_PARSE_BUFFER		256
#endif
#ifndef GPS_SENTENCE_ID_LEN
	#define	GPS_SENTENCE_ID_LEN		6
#endif


// ascii definitions
#define ASCII_DOLLOR	0x24		// '$'
#define ASCII_LF        0x0A		// '\n'
#define ASCII_CR        0x0D		// '\r'
#define	ASCII_ASTERISK	0x2A		// '*'
#define	ASCII_COMMA		0x2C		// ','


#ifndef COUNTOF
#define COUNTOF(OBJ)       (  sizeof(OBJ)/sizeof(OBJ[0]) )
#endif





//  5130.7106 -> 51도 30.7106분 -> 51.51184333.. 도 
static double NMEA2LL(double nn)
{
//	ASSERT(nn >= 0);

	WORD_T	deg ;

	nn /= 100.0 ;
	deg = (WORD_T) nn ;
	nn -= deg ; 
	return deg + ( (nn * 10.0)/6.0 ) ;
}

static BOOL_T	checksumGPS ( char * _szData, DWORD_T _nLen )
{
	// data format 
	// $GPRMC,154232,A,2758.612,N,08210.515,W,085.4,084.4,230394,003.1,W*43[CR][LF] 
	
	// checksum : * 뒤의 값. Hex format
	//			: $~* 사이의 값들의 XOR 값..

	char	exor = 0x00 ;	// $~* 사이의 값들을 XOR 연산 하기 위한 변수
	DWORD_T ii;
	for ( ii = 0 ; ii < _nLen ; ++ii )
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
			exor ^= _szData[ii] ;
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



	if ( checksum == exor )
		return TRUE ;

	return FALSE ;
}


static BOOL_T	getField ( char * _pBuff, DWORD_T _nFieldIdx, char * _pField, DWORD_T _nFieldLen = MAX_FIELD_BUFFER )
{
	if ( (_pBuff == NULL) || (_pField == NULL) || (_nFieldLen <= 0) )
	{
		return FALSE;
	}

	//
	// Go to the beginning of the selected field
	//
	DWORD_T	ii = 0 ;
	DWORD_T	dwFieldIdx = 0 ;

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
	DWORD_T	jj = 0 ;
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


static BOOL_T	parseRMC ( TGPSInfo *lpGPSInfo, char * _szRMC, DWORD_T _nLen ) 	// RMC  = Recommended Minimum Specific GPS/TRANSIT Data
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

	xCToken m_token;
	m_token.Reset();
	int tok_count = m_token.Splite(_szRMC);

	if(tok_count < 12)
	{
		return FALSE;
	}

/*	
	if (lpGPSInfo->dblLon && lpGPSInfo->dblLat)
	{
		lpGPSInfo->dblLonPrev = lpGPSInfo->dblLon;
		lpGPSInfo->dblLatPrev = lpGPSInfo->dblLat;
		lpGPSInfo->dblAltPrev = lpGPSInfo->dblAlt;
		lpGPSInfo->dwAnglePrev = lpGPSInfo->dwAngle;
	}
*/
	
	//		1    = UTC of position fix  
	if ( getField(_szRMC, 1, szField) )
	{
		// time stamp save
		//NMEA_strcpyn ( lpGPSInfo->time_stamp, szField, COUNTOF(lpGPSInfo->time_stamp) ) ;

		//lpGPSInfo->dwTime = atol ( szField ) ;		// (hhmmss.sss)
		char szTemp[4] ;
		memset ( szTemp, 0x00, sizeof(szTemp) ) ;
		
		char * pField = szField ;
		strncpy ( szTemp, pField, 2 ) ;
		lpGPSInfo->gps_time.nHour	= (char) atol (szTemp) ;

		pField += 2 ;
		strncpy ( szTemp, pField, 2 ) ;
		lpGPSInfo->gps_time.nMinute	= (char) atol (szTemp) ;

		pField += 2 ;
		strncpy ( szTemp, pField, 2 ) ;
		lpGPSInfo->gps_time.nSec	= (char) atol (szTemp) ;
	}
	else
	{
		//lpGPSInfo->dwTime = 0 ;
		lpGPSInfo->gps_time.nHour	= 0 ;
		lpGPSInfo->gps_time.nMinute	= 0 ;
		lpGPSInfo->gps_time.nSec	= 0 ;
	}

	//		2    = Data status (V=navigation receiver warning)
	if ( getField(_szRMC, 2, szField) )
	{
		lpGPSInfo->wDataState = szField[0] ;
/*
		if(lpGPSInfo->wDataState == 'A'){
			//m_validCount = 30;
			//setValid();
		}
		else{
			//m_validCount = 0;
		}
*/
	}
	else
	{
		lpGPSInfo->wDataState = 0 ;
	}

	//		3    = Latitude of fix
	if ( getField(_szRMC, 3, szField) )
	{
		//lpGPSInfo->dblLat = atof ( szField ) ;
		lpGPSInfo->rLatitude = NMEA2LL(atof ( szField ));	// 도 단위로 변경 2006.5.15
		// 4807.038,N   Latitude 48 deg 07.038' N
	}
	else
	{
		lpGPSInfo->rLatitude = 0 ;
	}

	//		4    = N or S
	if ( getField(_szRMC, 4, szField) )
	{
		// lpGPSInfo->dblLat 를  trans..
		if ( szField[0] == 'N' )
		{
		}
		else if ( szField[0] == 'S' )
		{
			lpGPSInfo->rLatitude *= -1.0 ;
		}
		else
		{
			lpGPSInfo->rLatitude = 0 ;
		}
	}
	else
	{
		lpGPSInfo->rLatitude = 0 ;
	}


	//		5    = Longitude of fix
	if ( getField(_szRMC, 5, szField) )
	{
		//lpGPSInfo->dblLon = atof ( szField ) ;
		lpGPSInfo->rLongitude = NMEA2LL(atof ( szField ));  // 도 단위로 변경 

		//01131.000,E  Longitude 11 deg 31.000' E
	}
	else
	{
		lpGPSInfo->rLongitude = 0 ;
	}


	//		6    = E or W
	if ( getField(_szRMC, 6, szField) )
	{
		// lpGPSInfo->dblLon 를  trans..
		if ( szField[0] == 'E' )
		{
		}
		else if ( szField[0] == 'W' )
		{
			lpGPSInfo->rLongitude *= -1.0 ;
		}
		else
		{
			lpGPSInfo->rLongitude = 0 ;
		}		
	}
	else
	{
		lpGPSInfo->rLongitude = 0 ;
	}


	//		7    = Speed over ground in knots
	if ( getField(_szRMC, 7, szField) )
	{
		// knot : 항해·항공용 속도 단위. 1시간당 1해리(海里, 1852m)의 속도를 1노트라 한다
		double knot = atof ( szField ) ;
		//lpGPSInfo->dwSpeed = (DWORD_T) (knot * 100) ;	// double 형태를 DWORD로 저장한다.

		// getSpeed() 함수에서 knot를 km 로 바꾸어 리턴하기 때문에
		// 여기서 knot->km 할 필요는 없다..

		//double kmh  = KNOTS_TO_KMH(knot) ;	// knot 를 km/h 로 변환
		//lpGPSInfo->dwSpeed = (DWORD_T) (kmh * 100) ;	// double 형태를 DWORD로 저장한다.


		// knot --> km/h --> save
		double km = knot * 1.851998 ;
		lpGPSInfo->dwSpeed = (DWORD_T)km;
//		m_avgSpeed_kmh_in_3sec.push_top( km ) ;
		
	}
	else
	{
		lpGPSInfo->dwSpeed = 0 ;
	}

	//		8    = Track made good in degrees True
	if ( getField(_szRMC, 8, szField) )
	{
		lpGPSInfo->dwAngle = atol ( szField ) ;
	}
	else
	{
		lpGPSInfo->dwAngle = 0 ;
	}

	//		9    = UT date	(DDMMYY) 
	if ( getField(_szRMC, 9, szField) )		
	{
		//lpGPSInfo->dwDate = atol ( szField ) ;
		char szTemp[4] ;
		memset ( szTemp, 0x00, sizeof(szTemp) ) ;
		
		char * pField = szField ;
		strncpy ( szTemp, pField, 2 ) ;
		lpGPSInfo->gps_time.nDay	= (char) atol (szTemp) ;

		pField += 2 ;
		strncpy ( szTemp, pField, 2 ) ;
		lpGPSInfo->gps_time.nMonth	= (char) atol (szTemp) ;

		pField += 2 ;
		strncpy ( szTemp, pField, 2 ) ;
		lpGPSInfo->gps_time.nYear	= (short) atol (szTemp) ;

		if ( lpGPSInfo->gps_time.nYear >= 80 )
			lpGPSInfo->gps_time.nYear += 1900 ;
		else
			lpGPSInfo->gps_time.nYear += 2000 ;
				}
	else
	{
		//lpGPSInfo->dwDate = 0 ;
		lpGPSInfo->gps_time.nYear	= 0 ;
		lpGPSInfo->gps_time.nMonth	= 0 ;
		lpGPSInfo->gps_time.nDay	= 0 ;
	}

	//		10   = Magnetic variation degrees (Easterly var. subtracts from true course)
	//		11   = E or W


	//ApplyAutoTimeZone();

	return TRUE;
}

static BOOL_T	parseGGA ( TGPSInfo *lpGPSInfo, char * _szGGA, DWORD_T _nLen ) 	// GGA  = Global Positioning System Fix Data
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

	xCToken m_token;
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
		lpGPSInfo->wGPSFix = (WORD_T) atol ( szField ) ;
	}
	else
	{
		lpGPSInfo->wGPSFix = 0 ;
	}

	//		7    = Number of satellites in use [not those in view]
	if ( getField(_szGGA, 7, szField) )
	{
		lpGPSInfo->wSU = (WORD_T) atol ( szField ) ;
	}
	else
	{
		lpGPSInfo->wSU = 0 ;
	}

	//		8    = Horizontal dilution of position (HDOP)
	if ( getField(_szGGA, 8, szField) )
	{
		lpGPSInfo->rHDOP = (REAL_T)atof( szField );
	}
	else
	{
		lpGPSInfo->rHDOP = 0 ;
	}


	//		9    = Antenna altitude above/below mean sea level (geoid)
	if ( getField(_szGGA, 9, szField) )
	{
		lpGPSInfo->rAltitude = atof ( szField ) ;
	}
	else
	{
		lpGPSInfo->rAltitude = 0 ;
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

static BOOL_T	parseGSV ( TGPSInfo *lpGPSInfo, char * _szGSV, DWORD_T _nLen ) 	// GSV  = Number of SVs(Satellites in View) in view, PRN numbers, elevation, azimuth & SNR values.
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

	DWORD_T	nTotalNumOfMsg, nMsgNum =0;
	char	szField[MAX_FIELD_BUFFER + 10] ;

	memset ( szField, 0x00, sizeof(szField) ) ;

	if ( !getField ( _szGSV, 0, szField ) )	// $GPXXX 아이디 필드를 가져올 수 없으면 깨진 데이터 ..
		return FALSE ;

	if( !getField( _szGSV, 7, szField) )
	{
		return FALSE;
	}

	xCToken m_token;
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
		if ( (nTotalNumOfMsg > 9) || (nTotalNumOfMsg < 0) )
			return FALSE ; 


		if ( (nTotalNumOfMsg < 1) || ((nTotalNumOfMsg*4) >= MAX_SATELLITE_NUM) )
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

		//
		// Make sure that the message number is valid. This is used to
		// calculate indexes into an array
		//
		if ( (nMsgNum > 9) || (nMsgNum < 0) )
			return FALSE ; 

	}
	else
	{
		return FALSE ;
	}


	//		3    = Total number of SVs in view 
	if ( getField(_szGSV, 3, szField) )	
	{
		lpGPSInfo->wSV = (WORD_T) atol ( szField ) ;
	}
	else
	{
		lpGPSInfo->wSV = 0 ;
	}



		
	for ( int ii = 0 ; ii < 4 ; ++ii )
	{
		//		4    = SV PRN number 
		if ( getField(_szGSV, 4 + (4*ii), szField) )	
		{
			lpGPSInfo->satellite[ii+(nMsgNum-1)*4].dwPRN = atol ( szField ) ;
		}
		else
		{
			lpGPSInfo->satellite[ii+(nMsgNum-1)*4].dwPRN = 0 ;
		}

		//		5    = Elevation in degrees, 90 maximum 
		if ( getField(_szGSV, 5 + (4*ii), szField) )	
		{
			lpGPSInfo->satellite[ii+(nMsgNum-1)*4].dwElev = atol ( szField ) ;
		}
		else
		{
			lpGPSInfo->satellite[ii+(nMsgNum-1)*4].dwElev = 0 ;
		}

		//		6    = Azimuth, degrees from true north, 000 to 359 
		if ( getField(_szGSV, 6 + (4*ii), szField) )
		{
			lpGPSInfo->satellite[ii+(nMsgNum-1)*4].dwAzi = atol ( szField ) ;
		}
		else
		{
			lpGPSInfo->satellite[ii+(nMsgNum-1)*4].dwAzi = 0 ;
		}

		//		7    = SNR, 00-99 dB (null when not tracking) 
		if ( getField(_szGSV, 7 + (4*ii), szField) )
		{
			lpGPSInfo->satellite[ii+(nMsgNum-1)*4].dwSNR = atol ( szField ) ;
		}
		else
		{
			lpGPSInfo->satellite[ii+(nMsgNum-1)*4].dwSNR = 0 ;
		}

		if (tok_count <= (4 + 4*(ii+1)) )
			break;
	}


	return TRUE ;
}


static BOOL_T	parseGSA ( TGPSInfo *lpGPSInfo, char * _szGSA, DWORD_T _nLen ) 	// GSA  = GPS receiver operating mode, SVs(Satellites in View) used for navigation, and DOP values.
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
		lpGPSInfo->wFixMode1 = 0;
		lpGPSInfo->wFixMode2 = 0;
		return FALSE ;
	}

	char	szField[MAX_FIELD_BUFFER + 10] ;
	memset ( szField, 0x00, sizeof(szField) ) ;

	if ( !getField ( _szGSA, 0, szField ) )	// $GPXXX 아이디 필드를 가져올 수 없으면 깨진 데이터 ..
	{
		lpGPSInfo->wFixMode1 = 0;
		lpGPSInfo->wFixMode2 = 0;
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
		lpGPSInfo->wFixMode1 = szField[0];
/*		
		if(lpGPSInfo->wDataStat == 'A'){
			//m_validCount = 30;
			//setValid();
		}
		else{
			//m_validCount = 0;
		}
*/		
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
		lpGPSInfo->wFixMode2 = atoi(szField);
	}
	else
	{
		// do nothing ..
	}
	

	//		3-14 = IDs of SVs used in position fix (null for unused fields)
	DWORD_T	dwPRN ;
	DWORD_T	ii, jj ;
	
	for ( jj = 0 ; jj < MAX_SATELLITE_NUM ; ++jj )
	{
		lpGPSInfo->satellite[jj].fUse = FALSE ;
	}


	for ( ii = 0 ; ii < 12 ; ++ii )
	{
		if ( getField(_szGSA, 3 + ii, szField) )	
		{		
			dwPRN = atol ( szField ) ;

			for ( jj = 0 ; jj < MAX_SATELLITE_NUM ; ++jj )
			{
				if ( dwPRN == lpGPSInfo->satellite[jj].dwPRN )
					lpGPSInfo->satellite[jj].fUse = TRUE ;
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
		lpGPSInfo->rPDOP = (REAL_T) atof ( szField ) ;
	}
	else
	{
		lpGPSInfo->rPDOP = 0 ;
	}

	//		16   = HDOP (Horizontal dilution of precision)
	// HDOP는 GGA에서 ....

	//		17   = VDOP (Vertical   dilution of precision)
	if ( getField(_szGSA, 17, szField) )	
	{
		lpGPSInfo->rVDOP = (REAL_T) atof ( szField );
	}
	else
	{
		lpGPSInfo->rVDOP = 0 ;
	}

	return TRUE ;
}




static BOOL_T	parseRMB ( TGPSInfo *lpGPSInfo, char * _szRMB, DWORD_T _nLen ) 	// RMB  = Recommended Minimum Navigation Information
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


static  BOOL_T	parseVTG ( TGPSInfo *lpGPSInfo, char * _szVTG, DWORD_T _nLen ) 	// VTG  = Actual track made good and speed over ground
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

static BOOL_T	parseRMA ( TGPSInfo *lpGPSInfo, char * _szRMA, DWORD_T _nLen ) 	// RMA  = Navigation data from present position
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




/*
static BOOL_T	parseTTR( TGPSInfo *lpGPSInfo, char* sMsg, DWORD_T nLen)
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
	
//	m_rds.Process( status, HIBYTE(blockA), LOBYTE(blockA));
//	m_rds.Process( status, HIBYTE(blockB), LOBYTE(blockB));
//	m_rds.Process( status, HIBYTE(blockC), LOBYTE(blockC));
//	m_rds.Process( status, HIBYTE(blockD), LOBYTE(blockD));

	return TRUE;
}
*/


/*
	Parse BEAGLE GPS LOG
*/
static BOOL_T	parsePOS( TGPSInfo *lpGPSInfo, char * _szPOS, DWORD_T _nLen ) 	// Beagle GPS Log
{

	/*
		$BGPSPOS,(1),(2),(3),(4),(5),(6),(7),(8),

		(1) "SRP"
		(2)  Version : 1
		(3)  Not used: 0
		(4)  Valid Fields
		(5)  Flags
		(6)  UTC Time (year)
		(7)  UTC Time (month)
		(8)  UTC TIme (day)
		(9)  UTC Time (hour)
		(10) UTC TIme (minute)
		(11) UTC Time (second)
		(12) Latitude,
		(13) Longitude
		(14) speed (knot)
		(15) heading
		(16) MagneticVariation
		(17) AltitudeWRTSeaLevel
		(18) AltitudeWRTEllipsoid
		(19) FixQuality
		(20) FixType
		(21) SelectionType
		(22) PDOP
		(23) HDOP
		(24) VDOP

	*/
	char	szField[MAX_FIELD_BUFFER + 10] ;

	memset ( szField, 0x00, sizeof(szField) ) ;

	if ( !getField ( _szPOS, 0, szField ) )	// $GPXXX 아이디 필드를 가져올 수 없으면 깨진 데이터 ..
		return FALSE ;

	xCToken m_token;
	m_token.Reset();
	int tok_count = m_token.Splite(_szPOS);

	if(tok_count < 20)
		return FALSE;

	char* pField = NULL;

	pField = m_token.Get(1);		 // 'SRP'
	if(strcmp(pField, "SRP")) return FALSE;

	pField = m_token.Get(2);		 // Version : 1
	if( atoi(pField) != 1) return FALSE;

	// UTC Time 
	pField = m_token.Get(6);
	lpGPSInfo->gps_time.nYear  = atoi(pField);

	pField = m_token.Get(7);
	lpGPSInfo->gps_time.nMonth = atoi(pField);

	pField = m_token.Get(8);
	lpGPSInfo->gps_time.nDay   = atoi(pField);

	pField = m_token.Get(9);
	lpGPSInfo->gps_time.nHour   = atoi(pField);

	pField = m_token.Get(10);
	lpGPSInfo->gps_time.nMinute = atoi(pField);

	pField = m_token.Get(11);
	lpGPSInfo->gps_time.nSec    = atoi(pField);

	// Latitude 
	pField = m_token.Get(12);
	lpGPSInfo->rLatitude  = atof(pField);  

	// Longutude
	pField = m_token.Get(13);
	lpGPSInfo->rLongitude  = atof(pField);  

	// Speed 
	pField = m_token.Get(14);
	double knot = atof(pField) ;
	lpGPSInfo->dwSpeed = knot * 1.851998; 

	// Heading
	pField = m_token.Get(15);
	lpGPSInfo->dwAngle = atoi(pField);

	// Altitude
	pField = m_token.Get(17);
	lpGPSInfo->rAltitude = atof(pField);

	pField = m_token.Get(19);
#ifdef _DEBUG   // fix-me : check-me
	lpGPSInfo->wFixMode2 = 2;//atoi(pField);
#else
	lpGPSInfo->wFixMode2 = atoi(pField);
#endif
	

	pField = m_token.Get(22);
	lpGPSInfo->rPDOP = atof(pField);

	pField = m_token.Get(23);
	lpGPSInfo->rHDOP = atof(pField);

	pField = m_token.Get(24);
	lpGPSInfo->rVDOP = atof(pField);

	return TRUE;
}


/*-----------------------------------------------------------------------------

FUNCTION: NMEA_ParseNMEA ( DWORD_T _nRead )

PURPOSE: gps data parsing..

PARMATERS:
    _nRead		- DWORD_T ( 4bytes ) value contains the number of bytes read.
RETURN: TRUE or FALSE 

HISTORY:   Kevin Cho 2005.07.19		first coding ...
		   Jason	 2006.2.5       modify
			   - GPS 데이타를 파라미터로 넘김 
			   - 여러 NMEA데이타 군이 넘어와 내부적으로 NMEA를 분리하는 방식에서
			     외부에서 NMEA프로토콜별로 분리하여, 하나의 프로토콜만을 넘김.
			   - 이에따라 GPS로그를 외부에서 저장. 
			   - RMC에서만 M/M수행 
-----------------------------------------------------------------------------*/
int	NMEA_ParseNMEAPacket( TGPSInfo *lpGPSInfo, const char* data, long len )
{
	DWORD_T dwRetVal = NMEA_PKT_NONE;

	char	buff[MAX_PARSE_BUFFER + 10] ;

	if( len < 0 ){
		len = NMEA_strlen( data );
		if( len <= 0 ) return NMEA_PKT_NONE;
	}

	if(len >= sizeof(buff))
		len = sizeof(buff)-1;
	
	NMEA_memcpy(buff, data, len);
	buff[len] = '\0';

	// 데이터의 형태는 다음과 같다..
	// $GPRMC,154232,A,2758.612,N,08210.515,W,085.4,084.4,230394,003.1,W*43[CR][LF] 

	if ( !strncmp(buff, "$GPRMC", GPS_SENTENCE_ID_LEN) )
	{
		if (parseRMC ( lpGPSInfo, buff, len))
		    dwRetVal = NMEA_PKT_GPRMC;
	}
	else if ( !strncmp(buff, "$GPGGA", GPS_SENTENCE_ID_LEN) )
	{
		if( parseGGA ( lpGPSInfo, buff, len) )
		    dwRetVal = NMEA_PKT_GPGGA;
	}
	else if ( !strncmp(buff, "$GPGSV", GPS_SENTENCE_ID_LEN) )
	{
		if( parseGSV ( lpGPSInfo, buff, len) )
		    dwRetVal = NMEA_PKT_GPGSV;
	}
	else if ( !strncmp(buff, "$GPGSA", GPS_SENTENCE_ID_LEN) )
	{
		if( parseGSA ( lpGPSInfo, buff, len) )
		    dwRetVal = NMEA_PKT_GPGSA;
	}
	else if ( !strncmp(buff, "$PLTTS", GPS_SENTENCE_ID_LEN) )
	{
		// Tuner Status & Acknowledge
		dwRetVal = NMEA_PKT_PLTTS;
	}
	else if ( !strncmp(buff, "$PLTTF", GPS_SENTENCE_ID_LEN) )
	{
		// Tunner Frequency (Philips model)
		dwRetVal = NMEA_PKT_PLTTF;
	} 
	else if ( !strncmp(buff, "$PLTTQ", GPS_SENTENCE_ID_LEN) )
	{
		// Tunner Frequency (Open TMC 2.0 model)
		dwRetVal = NMEA_PKT_PLTTQ;
	}
	else if ( !strncmp(buff, "$PLTCH", GPS_SENTENCE_ID_LEN) )
	{
		// Tunner Channel
		dwRetVal = NMEA_PKT_PLTCH;
	}
	else if ( !strncmp(buff, "$PLTMT", GPS_SENTENCE_ID_LEN) )
	{
		// Meta data => RDS Country Code
		dwRetVal = NMEA_PKT_PLTMT;
	}
	else if ( !strncmp(buff, "$PLTRD", GPS_SENTENCE_ID_LEN) )
	{
		// Row RDS Data (Not implemented by Amaryllo)
		dwRetVal = NMEA_PKT_PLTRD;
	}
	else if ( !strncmp(buff, "$PLTTM", GPS_SENTENCE_ID_LEN) )
	{
		// TMC / Application data
		dwRetVal = NMEA_PKT_PLTTM;
	}
	else if ( !strncmp(buff, "$PLTTR", GPS_SENTENCE_ID_LEN) )
	{
		//parseTTR( lpGPSInfo, buff, len);
		dwRetVal = NMEA_PKT_PLTTR;
	}
	else if ( !strncmp(buff, "$PLTFD", GPS_SENTENCE_ID_LEN) )
	{
		// RDS Filter
		dwRetVal = NMEA_PKT_PLTFD;
	}
	else if ( !strncmp(buff, "$PLTCF", GPS_SENTENCE_ID_LEN) )
	{
		// Confirm message request
		dwRetVal = NMEA_PKT_PLTCF;
	}
	else if (strncmp(buff, "$BGLPOS", 7) == 0)
	{
		// Beagle GPS Log
		if( parsePOS ( lpGPSInfo, buff, len) )
		    dwRetVal = NMEA_PKT_BGPSPOS;
	}

	return dwRetVal ;
}

