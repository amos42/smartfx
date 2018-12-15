#include "GPS_Info.h"
#include "B_NMEA.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "_heap.h"


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
#define COUNTOF(a)       (  sizeof(a)/sizeof(a[0]) )
#endif



// 51.51184333도  -> 51도  (0.51184333 * 60) 분 -> 5130.7106 
static REAL_T LL2NMEA(REAL_T nn)
{
	INT_T deg = (INT_T)(nn);
	REAL_T min = (nn - deg) * 60.0;
	
	return (deg * 100 + min);  // min unit * 100
}


static BYTE_T makeChecksumGPS(CHAR_T* cpGPSData, int lLength)
{
	if(!cpGPSData || !lLength) return 0;
	
	// '$'은 빼고 계산한다
	BYTE_T checksum = 0;
	
	for(INT_T ii=0 ; ii<lLength ; ii++)
	{
		if(cpGPSData[ii] == ASCII_DOLLOR) continue;
		if(cpGPSData[ii] == ASCII_ASTERISK) break;
		checksum ^= cpGPSData[ii];
	}
	
	return checksum;
}


static DWORD_T buildGGA( TGPSInfo *lpGPSInfo, char* data, long len )
{
	char stTemp[MAX_PARSE_BUFFER];
	unsigned char checksum = 0;
	int len2;
	
	char cpNS = 'N';
	char cpWE = 'E';	
	if(lpGPSInfo->rLatitude < 0 ) cpNS = 'S';
	if(lpGPSInfo->rLongitude < 0 ) cpWE = 'W';
	
 	   //           1       2    3     4    5 6 7   8   9  0  1  2  3   4  
//		$GPGGA,hhmmss.ss,llll.ll,a,yyyyy.yy,a,x,xx,x.x,x.x,M,x.x,M,x.x,xxxx*hh
	   //           1       2    3     4    5 6 7   8   9  0  1  2  3   4  

	len2 = NMEA_sprintf(stTemp, "$GPGGA,%02d%02d%02d,%.4f,%c,%.4f,%c,%d,%02d,%02.1f,%.2f,%c,%.2f,%c,%.1f,%.1f",
				lpGPSInfo->gps_time.nHour, lpGPSInfo->gps_time.nMinute, lpGPSInfo->gps_time.nSec,
				LL2NMEA(lpGPSInfo->rLatitude),cpNS, LL2NMEA(lpGPSInfo->rLongitude),cpWE,  // E,W,N,S 계산 추가해야함
				lpGPSInfo->wGPSFix,  /* 6*/
				lpGPSInfo->wSU,         
				lpGPSInfo->rHDOP,
				lpGPSInfo->rAltitude,  /* 9 */
				'M', 0.01, 'M', 0, 0);
	   
	checksum = makeChecksumGPS( stTemp, len2 );	
	len2 = NMEA_sprintf( &stTemp[len2], "*%02X\r\n", checksum );
	
	if( len2 > len ) return 0;

	NMEA_strcpyn( data, stTemp, len2 );
	
	return len2;
}


static DWORD_T buildGSA( TGPSInfo *lpGPSInfo, char* data, long len )
{
	char stTemp[MAX_PARSE_BUFFER];
	unsigned char checksum = 0;
	int len2;
	
	//$GPGGA ///////////////////////////////////////////////////////////////
	   //           1       2    3     4    5 6 7   8   9  0  1  2  3   4  
//		$GPGGA,hhmmss.ss,llll.ll,a,yyyyy.yy,a,x,xx,x.x,x.x,M,x.x,M,x.x,xxxx*hh
	   //           1       2    3     4    5 6 7   8   9  0  1  2  3   4  

	//$GPGSA ///////////////////////////////////////////////////////////////
	len2 = NMEA_sprintf( stTemp, "$GPGSA,%c,%d,,,,,,,,,,,,,%.2f,%.2f,%.2f",
			lpGPSInfo->wFixMode1, lpGPSInfo->wFixMode2,
			lpGPSInfo->rPDOP, lpGPSInfo->rHDOP, lpGPSInfo->rVDOP);

	checksum = makeChecksumGPS( stTemp, len2 );	
	len2 = NMEA_sprintf( &stTemp[len2], "*%02X\r\n", checksum );

	if( len2 > len ) return 0;

	NMEA_strcpyn( data, stTemp, len2 );
	
	return len2;
}


static DWORD_T buildGSV( TGPSInfo *lpGPSInfo, const char* data, long len )
{
	//$GPGSV ///////////////////////////////////////////////////////////////
	//정보부족으로 인해 만들어 내기 힘듬. 차후 추가 예정.
	
	return 0;
}


static DWORD_T buildRMC( TGPSInfo *lpGPSInfo, char* data, long len )
{
	char stTemp[MAX_PARSE_BUFFER];
	unsigned char checksum = 0;
	int len2;
	
	CHAR_T cpNS = 'N';
	CHAR_T cpWE = 'E';	
	
	if(lpGPSInfo->rLatitude < 0 ) cpNS = 'S';
	if(lpGPSInfo->rLongitude < 0 ) cpWE = 'W';
	
	//$GPRMC ///////////////////////////////////////////////////////////////
	
	//                1      2   3     4    5     6  7   8     9   10  11
	//		$GPRMC,hhmmss.ss,A,llll.ll,a,yyyyy.yy,a,x.x,x.x,ddmmyy,x.x,a*hh
	                   
	//	       0      1       2     3     4     5      6     7    8   9   0 1 
	//	    $GPRMC,074048.461,A,3733.2870,N,12655.1386,E,0.000000,,310106,,*14
	
	REAL_T fSpeed = (REAL_T)(lpGPSInfo->dwSpeed / 1.852);
	REAL_T dAngle = (REAL_T)lpGPSInfo->dwAngle;	
	
	len2 = NMEA_sprintf( stTemp, "$GPRMC,%02d%02d%02d.%02d,%c,%.4f,%c,%.4f,%c,%.1f,%.1f,%02d%02d%02d,,,A",
			lpGPSInfo->gps_time.nHour, lpGPSInfo->gps_time.nMinute, lpGPSInfo->gps_time.nSec,0,
			lpGPSInfo->wDataState,
			LL2NMEA(lpGPSInfo->rLatitude),cpNS, LL2NMEA(lpGPSInfo->rLongitude),cpWE,  // E,W,N,S 계산 추가해야함
			fSpeed,
			dAngle,
			lpGPSInfo->gps_time.nDay, lpGPSInfo->gps_time.nMonth, lpGPSInfo->gps_time.nYear - 2000);

	checksum = makeChecksumGPS( stTemp, len2 );	
	len2 = NMEA_sprintf( &stTemp[len2], "*%02X\r\n", checksum );
	
	return len2;
}


int NMEA_BuildNMEAPacket( TGPSInfo *lpGPSInfo, char* data, long len )
{
	int buf_len = 0;

	if( data == NULL ) return 0;
	if( len <= 0 ) return 0;
	
	buf_len += buildGGA( lpGPSInfo, &data[buf_len], len );
	if( len < buf_len ) return buf_len;
	
	buf_len += buildGSA( lpGPSInfo, &data[buf_len], len - buf_len );
	if( len < buf_len ) return buf_len;
	
	buf_len += buildGSV( lpGPSInfo, &data[buf_len], len - buf_len );
	if( len < buf_len ) return buf_len;
	
	buf_len += buildRMC( lpGPSInfo, &data[buf_len], len - buf_len );
 	
	return buf_len;
}


