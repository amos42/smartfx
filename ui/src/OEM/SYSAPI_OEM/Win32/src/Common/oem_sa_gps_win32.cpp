#include "SYSAPI.h"
#include "OEM_SA_gps.h"
#include <windows.h>
#include <stdio.h>


#ifndef __NO_GPS_

#include "B_COMM.h"
#include "B_NMEA.h"

#define ASCII_DOLLOR	0x24		// '$'
#define ASCII_LF        0x0A		// '\n'
#define ASCII_CR        0x0D		// '\r'
#define	ASCII_ASTERISK	0x2A		// '*'
#define	ASCII_COMMA		0x2C		// ','

#define MAX_READ_BUFFER 1024


#ifndef MAX_SATELLITE_NUM
#define MAX_SATELLITE_NUM   (32)
#endif

typedef struct ___GPSSatellite {// GPS의 정보
	atDWORD	dwPRN ;		//	Satellite PRN number ( 1-32 )
	atDWORD	dwElev ;	//	Elevation, degrees	( 00-90 )
	atDWORD	dwAzi ;		//	Azimuth, degrees	( 000-359 )
	atDWORD	dwSNR ;		//	SNR - higher is better ( 00-99dB, null when not tracking ) 
	atBOOL	fUse ;		// 사용하고 있는지 어떤지 ..
} __GPSSatellite;


typedef struct ___GPSInfo {
	double	dblLon ;	// 경도		longitude	// + 면 동위(E), - 면 서위(W) ..
	double	dblLat ;	// 위도		latitude	// + 면 북위(N), - 면 남위(S) ..
	double	dblAlt ;	// 고도	(m)	altitude 
	
	SYS_DATETIME gps_time;
	
	atDWORD	dwSpeed ;	// 속도 (knot)			 ( 원래값 * 100 )
	atLONG	dwAngle ;	// Trace Angle (degree) 
	atWORD	nFixState;   	// GPS Quality
	atWORD	nQuality;   	// GPS Quality
	atWORD	n3DState;
	
	atWORD	wSV ;		// Number of Satellites in View 
	atWORD	wSU ;		// Number of Satellites in Using

	__GPSSatellite		satellite[MAX_SATELLITE_NUM] ;		// 위성정보

	atREAL	rHDOP ;	// Horizontal Dilution Of Precision
	atREAL	rVDOP ;	// Vertical   Dilution Of Precision
	atREAL	rPDOP ;	// Position   Dilution Of Precision
} __GPSInfo;


static atINT		OEM_SYSAPI_GPS_GetState( OEM_SYS_GPS hOemGPS );

class CMyGPS { ///: public CGPS {
public:
	__GPSInfo		m_GPSInfo ;								// GPS 정보
	TGPSInfo m_OrgGPSInfo;
	
//	SYS_TASK m_hProcHandle;
	SYS_GPS_PROC_FUNC *m_fnProc;
	atTCHAR port[32];
	int baudrate;
	COMM_T hComm;
	NMEA_PARSER_T hNMEAParser;
	
	atBOOL bConnected;
	char arPktBuf[MAX_READ_BUFFER+4];
	int nPktBufCnt;

	static int func( int pkt_type, TGPSInfo *lpGPSInfo, void *param )
	{
		CMyGPS *pGPS = (CMyGPS *)param;

		int isok;
		atREAL lon;
		atREAL lat;
		atREAL alt;
		long speed;
		long angle;

		isok = OEM_SYSAPI_GPS_GetState((OEM_SYS_GPS)pGPS) >= SYS_GPS_STATE_2D;
		lon = FLOATTOREAL(pGPS->m_GPSInfo.dblLon);
		lat = FLOATTOREAL(pGPS->m_GPSInfo.dblLat);
		alt = FLOATTOREAL(pGPS->m_GPSInfo.dblAlt);
		speed = pGPS->m_GPSInfo.dwSpeed;
		angle = pGPS->m_GPSInfo.dwAngle;
		
		if( pkt_type == NMEA_PKT_GPGGA ){
			pGPS->m_GPSInfo.nFixState = (lpGPSInfo->wGPSFix != 0)? 1 : 0;
			pGPS->m_GPSInfo.wSU = lpGPSInfo->wSU;
			pGPS->m_GPSInfo.rHDOP = FLOATTOREAL(lpGPSInfo->rHDOP);
		}

		if( pkt_type == NMEA_PKT_GPGSA ){
			pGPS->m_GPSInfo.n3DState = (lpGPSInfo->wFixMode1 == 'A')? lpGPSInfo->wFixMode2 : 2;	
//			pGPS->m_GPSInfo.rHDOP = FLOATTOREAL(lpGPSInfo->rHDOP);
			pGPS->m_GPSInfo.rVDOP = FLOATTOREAL(lpGPSInfo->rVDOP);
			pGPS->m_GPSInfo.rPDOP = FLOATTOREAL(lpGPSInfo->rPDOP);
		}
		
		if( pkt_type == NMEA_PKT_GPGSA )
		{
			int i;
			for( i = 0; i < MAX_SATELLITE_NUM; i++ ){
				pGPS->m_GPSInfo.satellite[i].dwPRN = lpGPSInfo->satellite[i].dwPRN;
				pGPS->m_GPSInfo.satellite[i].dwElev = lpGPSInfo->satellite[i].dwElev;
				pGPS->m_GPSInfo.satellite[i].dwAzi = lpGPSInfo->satellite[i].dwAzi;
				pGPS->m_GPSInfo.satellite[i].dwSNR = lpGPSInfo->satellite[i].dwSNR;
				pGPS->m_GPSInfo.satellite[i].fUse = lpGPSInfo->satellite[i].fUse;
			}
			pGPS->m_GPSInfo.wSV = lpGPSInfo->wSV;
		}
		
		if( pkt_type == NMEA_PKT_GPRMC ){
//			pGPS->m_GPSInfo.nFixState = (lpGPSInfo->wDataState == 'A')? 1 : 0;
			pGPS->m_GPSInfo.nQuality = (lpGPSInfo->wDataState == 'A')? 2 : 0;
			pGPS->m_GPSInfo.dblLon = lpGPSInfo->rLongitude;
			pGPS->m_GPSInfo.dblLat = lpGPSInfo->rLatitude;
			pGPS->m_GPSInfo.dblAlt = lpGPSInfo->rAltitude;
			SET_SYS_DATETIME( pGPS->m_GPSInfo.gps_time, 
								lpGPSInfo->gps_time.nYear, lpGPSInfo->gps_time.nMonth, lpGPSInfo->gps_time.nDay, 
								lpGPSInfo->gps_time.nHour, lpGPSInfo->gps_time.nMinute, lpGPSInfo->gps_time.nSec );
			pGPS->m_GPSInfo.dwSpeed = lpGPSInfo->dwSpeed;
			pGPS->m_GPSInfo.dwAngle = lpGPSInfo->dwAngle;
		}
		
		if( pkt_type == NMEA_PKT_BGPSPOS ){

#ifdef _DEBUG // check-me : fix-me
			//pGPS->m_GPSInfo.nFixState = 1;
			pGPS->m_GPSInfo.nFixState = 2;
			//pGPS->m_GPSInfo.n3DState = lpGPSInfo->wFixMode2;	
			pGPS->m_GPSInfo.n3DState = lpGPSInfo->wFixMode2;	
		//	pGPS->m_GPSInfo.rHDOP = FLOATTOREAL(lpGPSInfo->rHDOP);
			pGPS->m_GPSInfo.rVDOP = FLOATTOREAL(lpGPSInfo->rVDOP);
			pGPS->m_GPSInfo.rPDOP = FLOATTOREAL(lpGPSInfo->rPDOP);
			//pGPS->m_GPSInfo.nQuality = (lpGPSInfo->wDataState == 'A')? 2 : 0;
			pGPS->m_GPSInfo.nQuality = (lpGPSInfo->wDataState != 0)? 2 : 0;
			pGPS->m_GPSInfo.dblLon = lpGPSInfo->rLongitude;
			pGPS->m_GPSInfo.dblLat = lpGPSInfo->rLatitude;
			pGPS->m_GPSInfo.dblAlt = lpGPSInfo->rAltitude;
			SET_SYS_DATETIME( pGPS->m_GPSInfo.gps_time, 
								lpGPSInfo->gps_time.nYear, lpGPSInfo->gps_time.nMonth, lpGPSInfo->gps_time.nDay, 
								lpGPSInfo->gps_time.nHour, lpGPSInfo->gps_time.nMinute, lpGPSInfo->gps_time.nSec );
			pGPS->m_GPSInfo.dwSpeed = lpGPSInfo->dwSpeed;
			pGPS->m_GPSInfo.dwAngle = lpGPSInfo->dwAngle;
#else
			pGPS->m_GPSInfo.nFixState = 1;
			pGPS->m_GPSInfo.n3DState = lpGPSInfo->wFixMode2;	
			pGPS->m_GPSInfo.rVDOP = FLOATTOREAL(lpGPSInfo->rVDOP);
			pGPS->m_GPSInfo.rPDOP = FLOATTOREAL(lpGPSInfo->rPDOP);
			pGPS->m_GPSInfo.nQuality = (lpGPSInfo->wDataState == 'A')? 2 : 0;
			pGPS->m_GPSInfo.dblLon = lpGPSInfo->rLongitude;
			pGPS->m_GPSInfo.dblLat = lpGPSInfo->rLatitude;
			pGPS->m_GPSInfo.dblAlt = lpGPSInfo->rAltitude;
			SET_SYS_DATETIME( pGPS->m_GPSInfo.gps_time, 
								lpGPSInfo->gps_time.nYear, lpGPSInfo->gps_time.nMonth, lpGPSInfo->gps_time.nDay, 
								lpGPSInfo->gps_time.nHour, lpGPSInfo->gps_time.nMinute, lpGPSInfo->gps_time.nSec );
			pGPS->m_GPSInfo.dwSpeed = lpGPSInfo->dwSpeed;
			pGPS->m_GPSInfo.dwAngle = lpGPSInfo->dwAngle;
#endif
		}

//		pGPS->m_dwTick = SYSAPI_GetCurrTime();


		if( pGPS->m_fnProc ){
			pGPS->m_fnProc( (void *)pGPS, SYS_GPS_EVENT_PREPROCESS, isok, lon, lat, alt, speed, angle, &pGPS->m_GPSInfo.gps_time );
			isok = OEM_SYSAPI_GPS_GetState((OEM_SYS_GPS)pGPS) >= SYS_GPS_STATE_2D;
			lon = FLOATTOREAL(pGPS->m_GPSInfo.dblLon);
			lat = FLOATTOREAL(pGPS->m_GPSInfo.dblLat);
			alt = FLOATTOREAL(pGPS->m_GPSInfo.dblAlt);
			speed = pGPS->m_GPSInfo.dwSpeed;
			angle = pGPS->m_GPSInfo.dwAngle;
			pGPS->m_fnProc( (void *)pGPS, SYS_GPS_EVENT_POSTPROCESS, isok, lon, lat, alt, speed, angle, &pGPS->m_GPSInfo.gps_time );
		}							
		
		return 0;
	}

	CMyGPS(void) /// : CGPS()
	{	
		SYSAPI_memset( &m_GPSInfo, 0, sizeof(__GPSInfo) );
		hComm = atNULL;
//		m_hProcHandle = atNULL;
		m_fnProc = atNULL;
		bConnected = atFALSE;
		nPktBufCnt = 0;
		hNMEAParser = NMEA_CreateNMEAParser(&m_OrgGPSInfo, func, (void *)this);
	}

	~CMyGPS()
	{
		NMEA_DestroyNMEAParser(hNMEAParser);
	}	
};


static int _CommCallback( COMM_T hComm, int msg, long nParam1, long nParam2 )
{
	int r;
	int len;
	CMyGPS *pGPS = (CMyGPS *)COMM_GetUserTag( hComm );
	if( pGPS == atNULL ) return 0;

	static char	buff[MAX_READ_BUFFER + 10] ;
	
	switch( msg ){
		case COMM_MSG_INQUEUE:
			len = nParam1;
			if( len <= 0 ) return 0;
			if( len > MAX_READ_BUFFER) len = MAX_READ_BUFFER;
				
			if( len > 0 ){
				r = COMM_ReadComm( hComm, (void *)buff, len );
			}

			NMEA_ParseNMEAStream( pGPS->hNMEAParser, buff, len );

			break;
	}

	return 0;
}


static atBOOL		OEM_SYSAPI_InitGPS( atLONG lSysInst, atLONG lInitData1, atLONG lInitData2 )
{
	return atTRUE;
}

static atVOID		OEM_SYSAPI_FinalGPS( NOARGS )
{
}


static OEM_SYS_GPS 	OEM_SYSAPI_GPS_CreateGPS( void )
{
	CMyGPS *pGPS = new CMyGPS;

	pGPS->hComm = COMM_CreateComm();
	COMM_SetCallbackFunc( pGPS->hComm, _CommCallback );
	COMM_SetUserTag( pGPS->hComm, (DWORD_T)pGPS );

	return (OEM_SYS_GPS)pGPS;
}

static void 		OEM_SYSAPI_GPS_DestroyGPS( OEM_SYS_GPS hOemGPS )
{
	CMyGPS *pGPS = (CMyGPS *)hOemGPS;
	if( pGPS == atNULL ) return;
	
	COMM_DestroyComm( pGPS->hComm );

	delete pGPS;
}

static atBOOL 		OEM_SYSAPI_GPS_SetOEMParam( OEM_SYS_GPS hOemGPS, int nType, long lParam1, long lParam2 )
{
	CMyGPS *pGPS = (CMyGPS *)hOemGPS;
	if( pGPS == atNULL ) return atFALSE;

	COMM_SetCommType( pGPS->hComm, nType );

	if( nType == 0 ){
#ifdef _WIN32_WCE	
		SYSAPI_stprintf( pGPS->port, _AT("COM%d:"), lParam1 );
#else
		SYSAPI_stprintf( pGPS->port, _AT("COM%d"), lParam1 );
#endif
	} else if( nType == 1 ){
		SYSAPI_stprintf( pGPS->port, (atLPTSTR)lParam1 );
	}

    pGPS->baudrate = lParam2;
    
    return atTRUE;
}

static atBOOL 		OEM_SYSAPI_GPS_OpenGPS( OEM_SYS_GPS hOemGPS )
{
	CMyGPS *pGPS = (CMyGPS *)hOemGPS;
	if( pGPS == atNULL ) return atFALSE;

	atBOOL r = atTRUE;	
	if( !r ) return atFALSE;

	r = COMM_OpenComm( pGPS->hComm, (atLPTSTR)pGPS->port, pGPS->baudrate );
	if( r ){
		pGPS->bConnected = atTRUE;
	}

	return r;
}

static atBOOL OEM_SYSAPI_GPS_IsConnected(OEM_SYS_GPS hOemGPS)
{
	CMyGPS *pGPS = (CMyGPS *)hOemGPS;
	if( pGPS == atNULL ) return atFALSE;

	return pGPS->bConnected;
}

static atBOOL 		OEM_SYSAPI_GPS_CloseGPS( OEM_SYS_GPS hOemGPS )
{
	CMyGPS *pGPS = (CMyGPS *)hOemGPS;
	if( pGPS == atNULL ) return atFALSE;

	pGPS->bConnected = atFALSE;

	COMM_CloseComm( pGPS->hComm );

	/*
	if( pGPS->m_hProcHandle != atNULL ){
		SYSAPI_DestroyTask( (SYS_TASK)pGPS->m_hProcHandle );
		pGPS->m_hProcHandle = atNULL;
	}
	*/

	return atTRUE;	
//	return pGPS->close();	
}

static atINT		OEM_SYSAPI_GPS_GetState( OEM_SYS_GPS hOemGPS )
{
	CMyGPS *pGPS = (CMyGPS *)hOemGPS;
	if( pGPS == atNULL ) return SYS_GPS_STATE_INVALIDE;
	
//	if( !pGPS->bConnected ) return SYS_GPS_STATE_INVALIDE;
	
	int v = SYS_GPS_STATE_NOTFIXED;

	if( pGPS->m_GPSInfo.nFixState ){
		if( pGPS->m_GPSInfo.nQuality > 0 ){
			if( pGPS->m_GPSInfo.n3DState == 3 ){
				v = SYS_GPS_STATE_3D;
			} else if( pGPS->m_GPSInfo.n3DState == 2 ){
				v = SYS_GPS_STATE_2D;
			}
		}
	} else {
		if( !pGPS->bConnected ) return SYS_GPS_STATE_INVALIDE;
	}
	
	return v;
	
}

static atBOOL OEM_SYSAPI_GPS_SetProcessFunc( OEM_SYS_GPS hOemGPS, SYS_GPS_PROC_FUNC *fnProc )
{
	CMyGPS *pGPS = (CMyGPS *)hOemGPS;
	if( pGPS == atNULL ) return atFALSE;

	pGPS->m_fnProc = fnProc;
	
	return atTRUE;
}

static atBOOL 		OEM_SYSAPI_GPS_GetCoord( OEM_SYS_GPS hOemGPS, SYS_GPS_POSITION *lpGpsPos )
{
	CMyGPS *pGPS = (CMyGPS *)hOemGPS;
	if( pGPS == atNULL ) return atFALSE;
	
	double lon, lat, alt;
	lon = pGPS->m_GPSInfo.dblLon ;
	lat = pGPS->m_GPSInfo.dblLat ;
	alt = pGPS->m_GPSInfo.dblAlt ;

	lpGpsPos->rLongitude = FLOATTOREAL(lon);
	lpGpsPos->rLatitude = FLOATTOREAL(lat);
	lpGpsPos->rAltitude = FLOATTOREAL(alt);
	
	return atTRUE;
}

atBOOL 		OEM_SYSAPI_GPS_GetTime( OEM_SYS_GPS hOemGPS, SYS_DATETIME *datetime ) // 시간
{
	CMyGPS *pGPS = (CMyGPS *)hOemGPS;
	if( pGPS == atNULL ) return atFALSE;

	SET_SYS_DATETIME( *datetime,
		pGPS->m_GPSInfo.gps_time.nYear, pGPS->m_GPSInfo.gps_time.nMonth, pGPS->m_GPSInfo.gps_time.nDay,
		pGPS->m_GPSInfo.gps_time.nHour, pGPS->m_GPSInfo.gps_time.nMinute, pGPS->m_GPSInfo.gps_time.nSec );

	return atTRUE;
}

static double 		OEM_SYSAPI_GPS_GetSpeed ( OEM_SYS_GPS hOemGPS ) // 속도 (km/h)
{
	CMyGPS *pGPS = (CMyGPS *)hOemGPS;
	if( pGPS == atNULL ) return 0;
	
	return pGPS->m_GPSInfo.dwSpeed;	
}

static atLONG		OEM_SYSAPI_GPS_GetAngle( OEM_SYS_GPS hOemGPS ) // 진행 각도 (degree : 0 - 359)
{
	CMyGPS *pGPS = (CMyGPS *)hOemGPS;
	if( pGPS == atNULL ) return 0;
	
	return (long)pGPS->m_GPSInfo.dwAngle;
}

static atINT 		OEM_SYSAPI_GPS_GetNumSV ( OEM_SYS_GPS hOemGPS ) // 보이는 위성 갯수
{
	CMyGPS *pGPS = (CMyGPS *)hOemGPS;
	if( pGPS == atNULL ) return 0;
	
	return pGPS->m_GPSInfo.wSV;
}

static atINT 		OEM_SYSAPI_GPS_GetNumSU ( OEM_SYS_GPS hOemGPS ) // 사용중인 위성 갯수 
{
	CMyGPS *pGPS = (CMyGPS *)hOemGPS;
	if( pGPS == atNULL ) return 0;
	
	return pGPS->m_GPSInfo.wSU;
}

static atREAL 		OEM_SYSAPI_GPS_GetPDOP ( OEM_SYS_GPS hOemGPS ) // Position Dilution Of Precision
{
	CMyGPS *pGPS = (CMyGPS *)hOemGPS;
	if( pGPS == atNULL ) return 0;
	
	return pGPS->m_GPSInfo.rPDOP;
}

static atREAL 		OEM_SYSAPI_GPS_GetHDOP ( OEM_SYS_GPS hOemGPS ) // Horizontal Dilution Of Precision
{
	CMyGPS *pGPS = (CMyGPS *)hOemGPS;
	if( pGPS == atNULL ) return 0;
	
	return pGPS->m_GPSInfo.rHDOP;
}

static atREAL 		OEM_SYSAPI_GPS_GetVDOP ( OEM_SYS_GPS hOemGPS ) // Vertical Dilution Of Precision
{
	CMyGPS *pGPS = (CMyGPS *)hOemGPS;
	if( pGPS == atNULL ) return 0;
	
	return pGPS->m_GPSInfo.rVDOP;
}

static atBOOL 		OEM_SYSAPI_GPS_GetSatelliteInfo( OEM_SYS_GPS hOemGPS, atINT nIndex, SYS_GPS_SATELLITE *lpSatelliteEntry )
{
	CMyGPS *pGPS = (CMyGPS *)hOemGPS;
	__GPSSatellite	* p;
	if( pGPS == atNULL ) return atFALSE;
	
	if ( nIndex >= MAX_SATELLITE_NUM )
		return atFALSE ;

	if ( nIndex >= pGPS->m_GPSInfo.wSV )
		return atFALSE ;

	p = &(pGPS->m_GPSInfo.satellite[nIndex]) ;
	if( p->dwPRN == 0 ) return atFALSE;

	lpSatelliteEntry->nPRN = p->dwPRN;
	lpSatelliteEntry->nElevation = p->dwElev;
	lpSatelliteEntry->nAzimuth = p->dwAzi;
	lpSatelliteEntry->nSNR = p->dwSNR;
	lpSatelliteEntry->bUsed = p->fUse;
		
	return atTRUE;	
}

static atBOOL		OEM_SYSAPI_GPS_ResetSatelliteAll( OEM_SYS_GPS hOemGPS )
{
	CMyGPS *pGPS = (CMyGPS *)hOemGPS;
	if( pGPS == atNULL ) return atFALSE;

	SYSAPI_memset ( pGPS->m_GPSInfo.satellite, 0x00, sizeof(__GPSSatellite) * MAX_SATELLITE_NUM ) ;
	
	return atTRUE;
}

static atBOOL OEM_SYSAPI_GPS_EmulateGPSData( OEM_SYS_GPS hOemGPS, atINT nPktCat, SYS_GPS_INFO *lpGpsInfo )
{
	CMyGPS *pGPS = (CMyGPS *)hOemGPS;
	if( pGPS == atNULL ) return atFALSE;

	if( nPktCat == SYS_GPS_PKT_GPGGA )
	{
		pGPS->m_GPSInfo.nFixState = lpGpsInfo->nFixState;
		pGPS->m_GPSInfo.wSU       = lpGpsInfo->nSU;
		pGPS->m_GPSInfo.rHDOP     = lpGpsInfo->rHDOP;
	}
	else
	if( nPktCat == SYS_GPS_PKT_GPGSA )
	{
		pGPS->m_GPSInfo.nFixState = lpGpsInfo->nFixState;
		pGPS->m_GPSInfo.n3DState  = lpGpsInfo->n3DState;
//		pGPS->m_GPSInfo.rHDOP    = lpGpsInfo->rHDOP;
		pGPS->m_GPSInfo.rVDOP     = lpGpsInfo->rVDOP;
		pGPS->m_GPSInfo.rPDOP     = lpGpsInfo->rPDOP;
	}
	else
	if( nPktCat == SYS_GPS_PKT_GPGSV )
	{
		int i;
		for( i = 0; i < MAX_SATELLITE_NUM; i++ ){
			pGPS->m_GPSInfo.satellite[i].dwPRN = lpGpsInfo->satellite[i].nPRN;
			pGPS->m_GPSInfo.satellite[i].dwElev = lpGpsInfo->satellite[i].nElevation;
			pGPS->m_GPSInfo.satellite[i].dwAzi = lpGpsInfo->satellite[i].nAzimuth;
			pGPS->m_GPSInfo.satellite[i].dwSNR = lpGpsInfo->satellite[i].nSNR;
			pGPS->m_GPSInfo.satellite[i].fUse = lpGpsInfo->satellite[i].bUsed;
		}
		pGPS->m_GPSInfo.wSV = lpGpsInfo->nSV;
	}
	else
	if( nPktCat == SYS_GPS_PKT_GPRMC )
	{
//		pGPS->m_GPSInfo.nFixState = lpGpsInfo->nFixState;
		pGPS->m_GPSInfo.nQuality = lpGpsInfo->nQuality;
		pGPS->m_GPSInfo.dblLon = REALTOFLOAT(lpGpsInfo->pos.rLongitude);
		pGPS->m_GPSInfo.dblLat = REALTOFLOAT(lpGpsInfo->pos.rLatitude);
		pGPS->m_GPSInfo.dblAlt = REALTOFLOAT(lpGpsInfo->pos.rAltitude);
		pGPS->m_GPSInfo.dwSpeed = lpGpsInfo->nSpeed;
		pGPS->m_GPSInfo.dwAngle = lpGpsInfo->nAngle;
		pGPS->m_GPSInfo.gps_time = lpGpsInfo->gps_datetime;
	}
	else if(nPktCat == SYS_GPS_PKT_BGPS)
	{
		pGPS->m_GPSInfo.nFixState = lpGpsInfo->nFixState;
		pGPS->m_GPSInfo.wSU       = lpGpsInfo->nSU;
	
		pGPS->m_GPSInfo.nFixState = lpGpsInfo->nFixState;
		pGPS->m_GPSInfo.n3DState  = lpGpsInfo->n3DState;
		pGPS->m_GPSInfo.rHDOP     = lpGpsInfo->rHDOP;
		pGPS->m_GPSInfo.rVDOP     = lpGpsInfo->rVDOP;
		pGPS->m_GPSInfo.rPDOP     = lpGpsInfo->rPDOP;

		int i;
		for( i = 0; i < MAX_SATELLITE_NUM; i++ ){
			pGPS->m_GPSInfo.satellite[i].dwPRN = lpGpsInfo->satellite[i].nPRN;
			pGPS->m_GPSInfo.satellite[i].dwElev = lpGpsInfo->satellite[i].nElevation;
			pGPS->m_GPSInfo.satellite[i].dwAzi = lpGpsInfo->satellite[i].nAzimuth;
			pGPS->m_GPSInfo.satellite[i].dwSNR = lpGpsInfo->satellite[i].nSNR;
			pGPS->m_GPSInfo.satellite[i].fUse = lpGpsInfo->satellite[i].bUsed;
		}
		pGPS->m_GPSInfo.wSV = lpGpsInfo->nSV;
	
		pGPS->m_GPSInfo.nQuality = lpGpsInfo->nQuality;
		pGPS->m_GPSInfo.dblLon = REALTOFLOAT(lpGpsInfo->pos.rLongitude);
		pGPS->m_GPSInfo.dblLat = REALTOFLOAT(lpGpsInfo->pos.rLatitude);
		pGPS->m_GPSInfo.dblAlt = REALTOFLOAT(lpGpsInfo->pos.rAltitude);
		pGPS->m_GPSInfo.dwSpeed = lpGpsInfo->nSpeed;
		pGPS->m_GPSInfo.dwAngle = lpGpsInfo->nAngle;
		pGPS->m_GPSInfo.gps_time = lpGpsInfo->gps_datetime;
	}
	
	return atTRUE;
}


static atLONG OEM_SYSAPI_GPS_ExtGPSMethod( OEM_SYS_GPS hOemGPS, atINT func_no, atLONG param1, atLONG param2, atLONG param3, atLONG param4, atLONG param5, atLONG param6 )
{
	return 0;
}


// Support GPS Log File
static atBOOL OEM_SYSAPI_GPS_CreateLogFile(OEM_SYS_GPS hOemGPS, atTCHAR* filename)
{
	return atFALSE;
}

static atVOID OEM_SYSAPI_GPS_SetLogEnable(OEM_SYS_GPS hOemGPS, atBOOL bEnable)
{
	return ;
}

static atBOOL OEM_SYSAPI_GPS_IsLogEnable(OEM_SYS_GPS hOemGPS)
{
	return atFALSE;
}

static atVOID OEM_SYSAPI_GPS_SetLogStatus(OEM_SYS_GPS hOemGPS, atBOOL bEnable)
{
	return ;
}

static atBOOL OEM_SYSAPI_GPS_Loging(OEM_SYS_GPS hOemGPS, SYS_GPS_INFO lpGPSDat)
{
	return atFALSE;
}

static atBOOL OEM_SYSAPI_GPS_CloseLoging(OEM_SYS_GPS hOemGPS)
{
	return atFALSE;
}

#else


static atBOOL		OEM_SYSAPI_InitGPS( atLONG lSysInst, atLONG lInitData1, atLONG lInitData2 )
{
	return atTRUE;
}

static atVOID		OEM_SYSAPI_FinalGPS( NOARGS )
{
}


static OEM_SYS_GPS 	OEM_SYSAPI_GPS_CreateGPS( void )
{
	return atNULL;
}

static void 		OEM_SYSAPI_GPS_DestroyGPS( OEM_SYS_GPS hOemGPS )
{
}

static atBOOL 		OEM_SYSAPI_GPS_SetOEMParam( OEM_SYS_GPS hOemGPS, int nType, long lParam1, long lParam2 )
{
    return atTRUE;
}

static atBOOL 		OEM_SYSAPI_GPS_OpenGPS( OEM_SYS_GPS hOemGPS )
{
	return atFALSE;
}

static atBOOL OEM_SYSAPI_GPS_IsConnected(OEM_SYS_GPS hOemGPS)
{
	return atFALSE;
}

static atBOOL 		OEM_SYSAPI_GPS_CloseGPS( OEM_SYS_GPS hOemGPS )
{
	return 0;	
}

static atINT		OEM_SYSAPI_GPS_GetState( OEM_SYS_GPS hOemGPS )
{
	return 0;
}

static atBOOL OEM_SYSAPI_GPS_SetProcessFunc( OEM_SYS_GPS hOemGPS, SYS_GPS_PROC_FUNC *fnProc )
{
	return atTRUE;
}

static atBOOL 		OEM_SYSAPI_GPS_GetCoord( OEM_SYS_GPS hOemGPS, SYS_GPS_POSITION *lpGpsPos )
{
	return atTRUE;
}

static atBOOL 		OEM_SYSAPI_GPS_GetTime( OEM_SYS_GPS hOemGPS, SYS_DATETIME *datetime ) // 시간
{
	return atTRUE;
}

static double 		OEM_SYSAPI_GPS_GetSpeed ( OEM_SYS_GPS hOemGPS ) // 속도 (km/h)
{
	return 0;	
}

static atLONG		OEM_SYSAPI_GPS_GetAngle( OEM_SYS_GPS hOemGPS ) // 진행 각도 (degree : 0 - 359)
{
	return 0;
}

static atINT 		OEM_SYSAPI_GPS_GetNumSV ( OEM_SYS_GPS hOemGPS ) // 보이는 위성 갯수
{
	return 0;
}

static atINT 		OEM_SYSAPI_GPS_GetNumSU ( OEM_SYS_GPS hOemGPS ) // 사용중인 위성 갯수 
{
	return 0;
}

static atREAL 		OEM_SYSAPI_GPS_GetPDOP ( OEM_SYS_GPS hOemGPS ) // Position Dilution Of Precision
{
	return 0;
}

static atREAL 		OEM_SYSAPI_GPS_GetHDOP ( OEM_SYS_GPS hOemGPS ) // Horizontal Dilution Of Precision
{
	return 0;
}

static atREAL 		OEM_SYSAPI_GPS_GetVDOP ( OEM_SYS_GPS hOemGPS ) // Vertical Dilution Of Precision
{
	return 0;
}

static atBOOL 		OEM_SYSAPI_GPS_GetSatelliteInfo( OEM_SYS_GPS hOemGPS, atINT nIndex, SYS_GPS_SATELLITE *lpSatelliteEntry )
{
	return atTRUE;	
}

static atBOOL		OEM_SYSAPI_GPS_ResetSatelliteAll( OEM_SYS_GPS hOemGPS )
{
	return atTRUE;
}

static atBOOL OEM_SYSAPI_GPS_EmulateGPSData( OEM_SYS_GPS hOemGPS, atINT nPktCat, SYS_GPS_INFO *lpGpsInfo )
{
	return atTRUE;
}


static atLONG OEM_SYSAPI_GPS_ExtGPSMethod( OEM_SYS_GPS hOemGPS, atINT func_no, atLONG param1, atLONG param2, atLONG param3, atLONG param4, atLONG param5, atLONG param6 )
{
	return 0;
}

// Support GPS Log File
static atBOOL OEM_SYSAPI_GPS_CreateLogFile(OEM_SYS_GPS hOemGPS, atTCHAR* filename)
{
	return atFALSE;
}

static atVOID OEM_SYSAPI_GPS_SetLogEnable(OEM_SYS_GPS hOemGPS, atBOOL bEnable)
{
	return ;
}

static atBOOL OEM_SYSAPI_GPS_IsLogEnable(OEM_SYS_GPS hOemGPS)
{
	return atFALSE;
}

static atVOID OEM_SYSAPI_GPS_SetLogStatus(OEM_SYS_GPS hOemGPS, atBOOL bEnable)
{
	return ;
}

static atBOOL OEM_SYSAPI_GPS_Loging(OEM_SYS_GPS hOemGPS, SYS_GPS_INFO lpGPSDat)
{
	return atFALSE;
}

static atBOOL OEM_SYSAPI_GPS_CloseLoging(OEM_SYS_GPS hOemGPS)
{
	return atFALSE;
}

#endif


extern "C" void init_sysapi_gps_oem(void) 
{
	OEM_SYSAPI_GPS_T oem_funcs = {
		OEM_SYSAPI_InitGPS,
		OEM_SYSAPI_FinalGPS,

		OEM_SYSAPI_GPS_CreateGPS,
		OEM_SYSAPI_GPS_DestroyGPS,
		OEM_SYSAPI_GPS_SetOEMParam,
		OEM_SYSAPI_GPS_OpenGPS,
		OEM_SYSAPI_GPS_IsConnected,
		OEM_SYSAPI_GPS_CloseGPS,
		OEM_SYSAPI_GPS_GetState,
		OEM_SYSAPI_GPS_SetProcessFunc,
		OEM_SYSAPI_GPS_GetCoord,
		OEM_SYSAPI_GPS_GetTime,
		OEM_SYSAPI_GPS_GetSpeed,
		OEM_SYSAPI_GPS_GetAngle,
		OEM_SYSAPI_GPS_GetNumSV,
		OEM_SYSAPI_GPS_GetNumSU,
		OEM_SYSAPI_GPS_GetPDOP,
		OEM_SYSAPI_GPS_GetHDOP,
		OEM_SYSAPI_GPS_GetVDOP,
		OEM_SYSAPI_GPS_GetSatelliteInfo,
		OEM_SYSAPI_GPS_ResetSatelliteAll,
		OEM_SYSAPI_GPS_EmulateGPSData,
		OEM_SYSAPI_GPS_ExtGPSMethod,

		OEM_SYSAPI_GPS_CreateLogFile,
		OEM_SYSAPI_GPS_SetLogEnable,
		OEM_SYSAPI_GPS_IsLogEnable,
		OEM_SYSAPI_GPS_SetLogStatus,
		OEM_SYSAPI_GPS_Loging,
		OEM_SYSAPI_GPS_CloseLoging
	};

	OEM_SYSAPI_GPS_SetOemFn( &oem_funcs );
}
