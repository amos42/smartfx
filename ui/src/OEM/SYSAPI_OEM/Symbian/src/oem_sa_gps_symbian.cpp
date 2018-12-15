#include "SYSAPI.h"
#include "OEM_SA_gps.h"
#include "OEM_SA_str.h"
#include <string.h>
#include "OSAL/_gps.h"
#include "B_NMEA.h"



// GPS quality 필드 추가 후, 검증 전까지 임시로 유지할 매크로 (검증 후 삭제 요망. by amos)
#define __XXX_ (1)



#define MAX_GPS_LOGBUF (4*1024)

class CMyGPS : public CGPS {
public:
	atBOOL IsActivate;
	SYS_TASK m_hProcHandle;
	SYS_GPS_PROC_FUNC *m_fnProc;
	
	CMyGPS(void) : CGPS()
	{	
		m_hProcHandle = atNULL;
		m_fnProc = atNULL;
	}

	~CMyGPS()
	{
	}	
};

/*
class CSymbianGPS {
public:
	TPosition position;
	RPositionServer server;	
	RPositioner positioner;	
	TRequestStatus status;
	
	RRequestorStack RequestStack;
	CRequestor* phContact;
	CRequestor* phService;	
	TPositionModuleId aModuleId;
};

static atBOOL _connect() 
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
*/

// Thread proc
//DWORD readGPSProc (LPVOID pThread, LPVOID lpParameter)
static void readGPSProc(SYS_TASK hTask, void *lpParameter)
{
	ASSERT(lpParameter);

	CMyGPS* pGPS = (CMyGPS*)lpParameter;

	for( int i = 0; i < 1000; i++ )
	{
		if( !SYSAPI_IsTaskActivate(hTask) ) break;
		
		if (pGPS->isConnected()) break;
		
		if( pGPS->connect() ) break; 

		SYSAPI_Sleep(500);
	}

	if (!pGPS->isConnected()) return;

	_GPSInfo oGPSDat;
	int isok;
	atREAL lon;
	atREAL lat;
	atREAL alt;
	long speed;
	long angle;

	while (SYSAPI_IsTaskActivate(hTask))
	{
		if (pGPS->IsActivate )
		{
			// SYS_GPS_EVENT_PREPROCESS용 데이터
			if( pGPS->m_fnProc )
			{
				isok = pGPS->isGPSOK();
				lon = FLOATTOREAL(pGPS->getLon());
				lat = FLOATTOREAL(pGPS->getLat());
				alt = FLOATTOREAL(pGPS->getAlt());
				speed = pGPS->getSpeed_kmh();
				angle = pGPS->getAngle();
			}

			atBOOL bOK = pGPS->GetGPSData(&oGPSDat);

			// GPS Log 저장		
			SYS_GPS_INFO stGPSDat;
			stGPSDat.pos.rLatitude = FLOATTOREAL(oGPSDat.dblLat);
			stGPSDat.pos.rLongitude = FLOATTOREAL(oGPSDat.dblLon);
			stGPSDat.pos.rAltitude = FLOATTOREAL(oGPSDat.dblAlt);									
			stGPSDat.gps_datetime = oGPSDat.gps_time;
			stGPSDat.nSpeed = oGPSDat.dwSpeed;
			stGPSDat.nAngle = oGPSDat.dwAngle;
			stGPSDat.nFixState = oGPSDat.nFixState;
			stGPSDat.nQuality = oGPSDat.nQuality;
			stGPSDat.n3DState = oGPSDat.n3DState;
			stGPSDat.nSV = oGPSDat.wSV;
			stGPSDat.nSU = oGPSDat.wSU;
			stGPSDat.rHDOP = FLOATTOREAL(oGPSDat.dblHDOP);
			stGPSDat.rVDOP = FLOATTOREAL(oGPSDat.dblVDOP);
			stGPSDat.rPDOP = FLOATTOREAL(oGPSDat.dblPDOP);

			OEM_SYSAPI_GPS_Loging((OEM_SYS_GPS)lpParameter, stGPSDat);	// GPS Log 저장							
			
			if( pGPS->m_fnProc )
			{
				pGPS->m_fnProc( (void *)pGPS, SYS_GPS_EVENT_PREPROCESS, isok, lon, lat, alt, speed, angle, &pGPS->m_GPSInfo.gps_time );

				// SYS_GPS_EVENT_POSTPROCESS용 데이터
				isok = pGPS->isGPSOK();
				lon = FLOATTOREAL(pGPS->getLon());
				lat = FLOATTOREAL(pGPS->getLat());
				alt = FLOATTOREAL(pGPS->getAlt());
				speed = pGPS->getSpeed_kmh();
				angle = pGPS->getAngle();

				pGPS->m_fnProc( (void *)pGPS, SYS_GPS_EVENT_POSTPROCESS, isok, lon, lat, alt, speed, angle, &pGPS->m_GPSInfo.gps_time );
			}
		}

		SYSAPI_Sleep(100);
	}

	pGPS->disconnect();

	return;
}


// 51.51184333도  -> 51도  (0.51184333 * 60) 분 -> 5130.7106 
static double LL2NMEA(double nn)
{
	atINT deg = (atINT)(nn);
	atDOUBLE min = (nn - deg) * 60.0;
	
	return (deg * 100 + min);  // min unit * 100
}


atBOOL		OEM_SYSAPI_InitGPS( atLONG lSysInst, atLONG lInitData1, atLONG lInitData2 )
{
	return atTRUE;
}

atVOID		OEM_SYSAPI_FinalGPS( NOARGS )
{
}


OEM_SYS_GPS 	OEM_SYSAPI_GPS_CreateGPS( void )
{
	CMyGPS *pGPS = new CMyGPS;

	pGPS->IsActivate = atFALSE;
		
	return (OEM_SYS_GPS)pGPS;
}

void 		OEM_SYSAPI_GPS_DestroyGPS( OEM_SYS_GPS hOemGPS )
{
	CMyGPS *pGPS = (CMyGPS *)hOemGPS;
	if( pGPS == atNULL ) return;
	
	// GPS Log File Close
	OEM_SYSAPI_GPS_CloseLoging(hOemGPS);	
	
	delete pGPS;
}

atBOOL 		OEM_SYSAPI_GPS_SetOEMParam( OEM_SYS_GPS hOemGPS, int nType, long lParam1, long lParam2 )
{
	CMyGPS *pGPS = (CMyGPS *)hOemGPS;
	if( pGPS == atNULL ) return atFALSE;
	
    // Symbian 에선 별로 필요가 없다.	
    
    return atTRUE;
}

atBOOL 		OEM_SYSAPI_GPS_OpenGPS( OEM_SYS_GPS hOemGPS )
{
	CMyGPS *pGPS = (CMyGPS *)hOemGPS;
	if( pGPS == atNULL ) return atFALSE;

	atBOOL r = pGPS->open( 0, 0 );	
	if( !r ) return atFALSE;

//	if(!THREAD_Start(THREAD_INDEX_GPS, readGPSProc, this))//GPS_GetInstance()))
	pGPS->m_hProcHandle = (void *)SYSAPI_CreateTask(_AT("GPSThread"), readGPSProc, (void *)pGPS, atTRUE, 16*1024); //GPS_GetInstance()))	
	if( pGPS->m_hProcHandle == atNULL ) return atFALSE;

	pGPS->IsActivate = atTRUE;
	
	return atTRUE;
}

atBOOL 		OEM_SYSAPI_GPS_CloseGPS( OEM_SYS_GPS hOemGPS )
{
	CMyGPS *pGPS = (CMyGPS *)hOemGPS;
	if( pGPS == atNULL ) return atFALSE;
	
	// GPS Log File Close
	OEM_SYSAPI_GPS_CloseLoging(hOemGPS);

	pGPS->IsActivate = atFALSE;
	
	if( pGPS->m_hProcHandle != atNULL ){
		SYSAPI_DestroyTask( (SYS_TASK)pGPS->m_hProcHandle );
		pGPS->m_hProcHandle = atNULL;
	}
	
	return pGPS->close();	
}

atINT		OEM_SYSAPI_GPS_GetState( OEM_SYS_GPS hOemGPS )
{
	CMyGPS *pGPS = (CMyGPS *)hOemGPS;
	if( pGPS == atNULL ) return SYS_GPS_STATE_INVALIDE;
	
//	if( !pGPS->isConnected() ) return SYS_GPS_STATE_INVALIDE;
	
	int v = SYS_GPS_STATE_NOTFIXED;

	if( pGPS->m_GPSInfo.nFixState ){
#if __XXX_		
		if( pGPS->m_GPSInfo.nQuality > 0 ){
#endif			
			if( pGPS->m_GPSInfo.n3DState == 3 ){
				v = SYS_GPS_STATE_3D;
			} else if( pGPS->m_GPSInfo.n3DState == 2 ){
				v = SYS_GPS_STATE_2D;
			}
#if __XXX_		
		}
#endif		
	} else {
		if( !pGPS->isConnected() ) return SYS_GPS_STATE_INVALIDE;
	}
	
	return v;
}

atBOOL OEM_SYSAPI_GPS_SetProcessFunc( OEM_SYS_GPS hOemGPS, SYS_GPS_PROC_FUNC *fnProc )
{
	CMyGPS *pGPS = (CMyGPS *)hOemGPS;
	if( pGPS == atNULL ) return atFALSE;

	pGPS->m_fnProc = fnProc;
	
	return atTRUE;
}

atBOOL 		OEM_SYSAPI_GPS_GetCoord( OEM_SYS_GPS hOemGPS, SYS_GPS_POSITION *lpGpsPos )
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

	*datetime = pGPS->m_GPSInfo.gps_time;

	return atTRUE;
}

double 		OEM_SYSAPI_GPS_GetSpeed ( OEM_SYS_GPS hOemGPS ) // 속도 (km/h)
{
	CMyGPS *pGPS = (CMyGPS *)hOemGPS;
	if( pGPS == atNULL ) return 0;
	
	return pGPS->m_GPSInfo.dwSpeed;	
}

atLONG		OEM_SYSAPI_GPS_GetAngle( OEM_SYS_GPS hOemGPS ) // 진행 각도 (degree : 0 - 359)
{
	CMyGPS *pGPS = (CMyGPS *)hOemGPS;
	if( pGPS == atNULL ) return 0;
	
	return (long)pGPS->m_GPSInfo.dwAngle;
}

atINT 		OEM_SYSAPI_GPS_GetNumSV ( OEM_SYS_GPS hOemGPS ) // 보이는 위성 갯수
{
	CMyGPS *pGPS = (CMyGPS *)hOemGPS;
	if( pGPS == atNULL ) return 0;
	
	return pGPS->m_GPSInfo.wSV;
}

atINT 		OEM_SYSAPI_GPS_GetNumSU ( OEM_SYS_GPS hOemGPS ) // 사용중인 위성 갯수 
{
	CMyGPS *pGPS = (CMyGPS *)hOemGPS;
	if( pGPS == atNULL ) return 0;
	
	return pGPS->m_GPSInfo.wSU;
}

atREAL 		OEM_SYSAPI_GPS_GetPDOP ( OEM_SYS_GPS hOemGPS ) // Position Dilution Of Precision
{
	CMyGPS *pGPS = (CMyGPS *)hOemGPS;
	if( pGPS == atNULL ) return 0;
	
	return FLOATTOREAL(pGPS->m_GPSInfo.dblPDOP);
}

atREAL 		OEM_SYSAPI_GPS_GetHDOP ( OEM_SYS_GPS hOemGPS ) // Horizontal Dilution Of Precision
{
	CMyGPS *pGPS = (CMyGPS *)hOemGPS;
	if( pGPS == atNULL ) return 0;
	
	return FLOATTOREAL(pGPS->m_GPSInfo.dblHDOP);
}

atREAL 		OEM_SYSAPI_GPS_GetVDOP ( OEM_SYS_GPS hOemGPS ) // Vertical Dilution Of Precision
{
	CMyGPS *pGPS = (CMyGPS *)hOemGPS;
	if( pGPS == atNULL ) return 0;
	
	return FLOATTOREAL(pGPS->m_GPSInfo.dblVDOP);
}

atBOOL 		OEM_SYSAPI_GPS_GetSatelliteInfo( OEM_SYS_GPS hOemGPS, atINT nIndex, SYS_GPS_SATELLITE *lpSatelliteEntry )
{
	CMyGPS *pGPS = (CMyGPS *)hOemGPS;
	_GPSSatellite	* p;
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

atBOOL		OEM_SYSAPI_GPS_ResetSatelliteAll( OEM_SYS_GPS hOemGPS )
{
	CMyGPS *pGPS = (CMyGPS *)hOemGPS;
	if( pGPS == atNULL ) return atFALSE;

	SYSAPI_memset ( pGPS->m_GPSInfo.satellite, 0x00, sizeof(_GPSSatellite) * MAX_SATELLITE_NUM ) ;
	
	return atTRUE;
}

atBOOL OEM_SYSAPI_GPS_EmulateGPSData( OEM_SYS_GPS hOemGPS, atINT nPktCat, SYS_GPS_INFO *lpGpsInfo )
{
	CMyGPS *pGPS = (CMyGPS *)hOemGPS;
	if( pGPS == atNULL ) return atFALSE;

	if( nPktCat == SYS_GPS_PKT_GPGGA )
	{
		pGPS->m_GPSInfo.nFixState = lpGpsInfo->nFixState;
		pGPS->m_GPSInfo.wSU = lpGpsInfo->nSU;
		pGPS->m_GPSInfo.dblHDOP = REALTOFLOAT(lpGpsInfo->rHDOP);
	}
	
	if( nPktCat == SYS_GPS_PKT_GPGSA )
	{
		pGPS->m_GPSInfo.nFixState = lpGpsInfo->nFixState;
		pGPS->m_GPSInfo.n3DState = lpGpsInfo->n3DState;
//		pGPS->m_GPSInfo.dblHDOP = REALTOFLOAT(lpGpsInfo->rHDOP);
		pGPS->m_GPSInfo.dblVDOP = REALTOFLOAT(lpGpsInfo->rVDOP);
		pGPS->m_GPSInfo.dblPDOP = REALTOFLOAT(lpGpsInfo->rPDOP);
	}
	
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
	
	if( nPktCat == SYS_GPS_PKT_GPRMC )
	{
#if __XXX_		
//		pGPS->m_GPSInfo.nFixState = lpGpsInfo->nFixState;
		pGPS->m_GPSInfo.nQuality = lpGpsInfo->nQuality;
#else
		pGPS->m_GPSInfo.nFixState = lpGpsInfo->nFixState;
#endif
		pGPS->m_GPSInfo.dblLon = REALTOFLOAT(lpGpsInfo->pos.rLongitude);
		pGPS->m_GPSInfo.dblLat = REALTOFLOAT(lpGpsInfo->pos.rLatitude);
		pGPS->m_GPSInfo.dblAlt = REALTOFLOAT(lpGpsInfo->pos.rAltitude);
		pGPS->m_GPSInfo.dwSpeed = lpGpsInfo->nSpeed;
		pGPS->m_GPSInfo.dwAngle = lpGpsInfo->nAngle;
		pGPS->m_GPSInfo.gps_time = lpGpsInfo->gps_datetime;
	}
	
	return atTRUE;
}

atLONG OEM_SYSAPI_GPS_ExtGPSMethod( OEM_SYS_GPS hOemGPS, atINT func_no, atLONG param1, atLONG param2, atLONG param3, atLONG param4, atLONG param5, atLONG param6 )
{
	return 0;
}

////////////////////////////////////////////////////////////////////////////////
// Support GPS Loging
atBOOL OEM_SYSAPI_GPS_CreateLogFile(OEM_SYS_GPS hOemGPS, atTCHAR* filename)
{
	CMyGPS *pGPS = (CMyGPS *)hOemGPS;
	if( pGPS == atNULL ) return atFALSE;

//	CAutoLock gps_lock(&g_hGPSCS);
//	CAutoLock gps_lock(pGPS);

//	ASSERT(pGPS->m_hLog == atNULL);//INVALID_HANDLE_VALUE);
	
	pGPS->m_hLog = SYSAPI_OpenFile(filename, SYS_FILEOPEN_CREATE);

//	ASSERT(pGPS->m_hLog != atNULL);//INVALID_HANDLE_VALUE);
	
	if(pGPS->m_hLog != atNULL)//!= INVALID_HANDLE_VALUE)
	{
		if( !pGPS->m_pLogBuf ) pGPS->m_pLogBuf = new atBYTE[MAX_GPS_LOGBUF];

		ASSERT(pGPS->m_pLogBuf);
		if(!pGPS->m_pLogBuf)
		{
			SYSAPI_CloseFile(pGPS->m_hLog);
			pGPS->m_hLog = atNULL;//INVALID_HANDLE_VALUE;
			return atFALSE;
		}
		pGPS->m_nLogBuf = 0;

		pGPS->m_bLogEnable = atTRUE;
		return atTRUE;
	}

	return atFALSE;
}

atVOID OEM_SYSAPI_GPS_SetLogEnable(OEM_SYS_GPS hOemGPS, atBOOL bEnable)
{
	CMyGPS *pGPS = (CMyGPS *)hOemGPS;
	if( pGPS == atNULL ) return ;
	
	pGPS->m_bLogEnable = bEnable;
}

atBOOL OEM_SYSAPI_GPS_IsLogEnable(OEM_SYS_GPS hOemGPS)
{
	CMyGPS *pGPS = (CMyGPS *)hOemGPS;
	if( pGPS == atNULL ) return atFALSE;
	
	return pGPS->m_bLogEnable;
}

atVOID OEM_SYSAPI_GPS_SetLogStatus(OEM_SYS_GPS hOemGPS, atBOOL bEnable)
{
	CMyGPS *pGPS = (CMyGPS *)hOemGPS;
	if( pGPS == atNULL ) return ;
	
	pGPS->m_bLoging = bEnable;
}

static atBOOL _GPS_WriteLog(CMyGPS *pGPS, atCHAR * _szLog, atDWORD _nRead, atBOOL bForce )
{
	if ( pGPS->m_hLog == atNULL) return atFALSE;

	//atDWORD nWrite;
	
	if( bForce )
	{
		if( pGPS->m_nLogBuf )
			SYSAPI_WriteFile( pGPS->m_hLog, pGPS->m_pLogBuf, pGPS->m_nLogBuf);//, &nWrite, 0 );
		if( _nRead )
			SYSAPI_WriteFile( pGPS->m_hLog, _szLog, _nRead);//, &nWrite, 0 );

		pGPS->m_nLogBuf = 0;
		SYSAPI_FlushFileBuffer(pGPS->m_hLog);
		return TRUE;
	}

	if (!pGPS->m_pLogBuf || !_szLog || !_nRead) return atFALSE;

	if( pGPS->m_nLogBuf + _nRead >= MAX_GPS_LOGBUF )
	{
		if (pGPS->m_nLogBuf > 0)
		{
			SYSAPI_WriteFile(pGPS->m_hLog, pGPS->m_pLogBuf, pGPS->m_nLogBuf);//, &nWrite, 0);
			pGPS->m_nLogBuf = 0;
		}
		if (_nRead)
			SYSAPI_WriteFile(pGPS->m_hLog, _szLog, _nRead);//, &nWrite, 0);
		
		SYSAPI_FlushFileBuffer(pGPS->m_hLog);
	}
	else
	{
		SYSAPI_memcpy( pGPS->m_pLogBuf + pGPS->m_nLogBuf, _szLog, _nRead );
		pGPS->m_nLogBuf += _nRead;
	}

	return atTRUE;
}

#if 0	
static atUCHAR _GPS_GetGPSDataCheckSum(atCHAR* cpGPSData, atLONG lLength)
{
	if(!cpGPSData || !lLength) return 0;
	
	// '$'은 빼고 계산한다
	atUCHAR checksum = 0;
	
	for(atINT ii=0 ; ii<lLength ; ii++)
	{
		if(cpGPSData[ii] == '$') continue;
		if(cpGPSData[ii] == '*') break;
		checksum ^= cpGPSData[ii];
	}
	
	return checksum;
}
#endif

atBOOL OEM_SYSAPI_GPS_Loging(OEM_SYS_GPS hOemGPS, SYS_GPS_INFO stGPSDat)
{
	CMyGPS *pGPS = (CMyGPS *)hOemGPS;
	if( pGPS == atNULL) return atFALSE;
	
	if(!pGPS->m_bLogEnable || !pGPS->m_bLoging) return atFALSE;
	
//	_GPSInfo stGPSDat;
//	if( !pGPS->GetGPSData(&stGPSDat)) return atFALSE;

#if 0	
	atCHAR cpNMEA[128] = {0,};	
	atCHAR cpTemp[128] = {0,};
	atTCHAR stTemp[128], stNMEA[128];
	atCHAR szLat[20], szLon[20];
	atUCHAR checksum = 0;
	atLONG lLength = 0;
	
	atCHAR cpNS = 'N';
	atCHAR cpWE = 'E';	
	atDOUBLE dbLatitude = REALTOFLOAT(stGPSDat.pos.rLatitude);
	atDOUBLE dbLongitude = REALTOFLOAT(stGPSDat.pos.rLongitude);
	atDOUBLE dbAltitude = REALTOFLOAT(stGPSDat.pos.rAltitude);
	atDOUBLE dblHDOP = REALTOFLOAT(stGPSDat.rHDOP);
	atDOUBLE dblVDOP = REALTOFLOAT(stGPSDat.rVDOP);
	atDOUBLE dblPDOP = REALTOFLOAT(stGPSDat.rPDOP);	
	
	if(dbLatitude < 0 ) cpNS = 'S';
	if(dbLongitude < 0 ) cpWE = 'W';
	
	//NMEA Parsing
	SYSAPI_memset(cpTemp,0,sizeof(cpTemp));
	SYSAPI_memset(cpNMEA,0,sizeof(cpNMEA));
	SYSAPI_memset(stTemp,0,sizeof(stTemp));
	SYSAPI_memset(stNMEA,0,sizeof(stNMEA));
	//$GPGGA ///////////////////////////////////////////////////////////////
	
	   //           1       2    3     4    5 6 7   8   9  0  1  2  3   4  
//		$GPGGA,hhmmss.ss,llll.ll,a,yyyyy.yy,a,x,xx,x.x,x.x,M,x.x,M,x.x,xxxx*hh
	   //           1       2    3     4    5 6 7   8   9  0  1  2  3   4  

	// Symbian에서 sprintf의 %.4f 기능이 정상적으로 작동하지 않아, SYSAPI_sprintf로 대체
		
	SYSAPI_stprintf(stTemp, _AT("$GPGGA,%02d%02d%02d,%.4f,%c,%.4f,%c,%d,%02d,%02.1f,%.2f,%c,%.2f,%c,%.1f,%.1f"),
				stGPSDat.gps_datetime.nHour, stGPSDat.gps_datetime.nMinute, stGPSDat.gps_datetime.nSec,
				LL2NMEA(dbLatitude),cpNS, LL2NMEA(dbLongitude),cpWE,  // E,W,N,S 계산 추가해야함
				stGPSDat.nFixState,  /* 6*/
				stGPSDat.nSU,         
				dblHDOP,
				dbAltitude,  /* 9 */
				'M', 0.01, 'M', 0, 0);
	
	SYSAPI_ToANSI((atANSI_CHAR*)cpTemp,stTemp, -1); 

	checksum = _GPS_GetGPSDataCheckSum( cpTemp, strlen(cpTemp));	
	sprintf(cpNMEA, "%s*%02X\r\n", cpTemp,checksum);
	
	_GPS_WriteLog(hOemGPS, cpNMEA, strlen(cpNMEA), atFALSE);
	
	SYSAPI_memset(cpTemp,0,sizeof(cpTemp));
	
	//$GPGSA ///////////////////////////////////////////////////////////////
	SYSAPI_stprintf(stTemp, _AT("$GPGSA,A,%d,,,,,,,,,,,,,%.2f,%.2f,%.2f"),
			// GPS 품질에 관한 정보를 넣어줘야하는가?
			stGPSDat.n3DState,
			dblPDOP, dblHDOP, dblVDOP);

	SYSAPI_ToANSI((atANSI_CHAR*)cpTemp, stTemp, -1);
	checksum _GPS_GetGPSDataCheckSum( cpTemp, strlen(cpTemp) );	
	sprintf(cpNMEA, "%s*%02X\r\n", cpTemp, checksum);

	_GPS_WriteLog(hOemGPS, cpNMEA, strlen(cpNMEA), atFALSE);
	
	SYSAPI_memset(cpTemp,0,sizeof(cpTemp));
			
	//$GPRMC ///////////////////////////////////////////////////////////////
	
	//                1      2   3     4    5     6  7   8     9   10  11
	//		$GPRMC,hhmmss.ss,A,llll.ll,a,yyyyy.yy,a,x.x,x.x,ddmmyy,x.x,a*hh
	                   
	//	       0      1       2     3     4     5      6     7    8   9   0 1 
	//	    $GPRMC,074048.461,A,3733.2870,N,12655.1386,E,0.000000,,310106,,*14
	
	atDOUBLE fSpeed = (atDOUBLE)(stGPSDat.nSpeed / 1.852);
	atDOUBLE dAngle = (atDOUBLE)stGPSDat.nAngle;	
	
	SYSAPI_stprintf(stTemp, _AT("$GPRMC,%02d%02d%02d.%02d,%c,%.4f,%c,%.4f,%c,%.1f,%.1f,%02d%02d%02d,,,A"),
			stGPSDat.gps_datetime.nHour, stGPSDat.gps_datetime.nMinute, stGPSDat.gps_datetime.nSec,0,
#if __XXX_
			(pGPS->isGPSOK() && stGPSDat.nQuality > 0)? 'A' : 'V', // GPS 품질에 관한 정보를 넣어줘야하는가? <- 넣어 줘야 한다.
#else
			pGPS->isGPSOK() ? 'A' : 'V',
#endif
			LL2NMEA(dbLatitude),cpNS, LL2NMEA(dbLongitude),cpWE,  // E,W,N,S 계산 추가해야함
			fSpeed,
			dAngle,
			stGPSDat.gps_datetime.nDay, stGPSDat.gps_datetime.nMonth, stGPSDat.gps_datetime.nYear - 2000);

	SYSAPI_ToANSI((atANSI_CHAR*)cpTemp, stTemp, -1);
	checksum = _GPS_GetGPSDataCheckSum( cpTemp, strlen(cpTemp) );	
	sprintf(cpNMEA, "%s*%02X\r\n", cpTemp, checksum);
		
	// check-me:
	// 죽기 직전의 정보를 저장하기 위해 강제 저장 하도록 변경함
	//OEM_SYSAPI_GPS_WriteLog(hOemGPS, cpNMEA, strlen(cpNMEA), atFALSE);
	_GPS_WriteLog(hOemGPS, cpNMEA, strlen(cpNMEA), atTRUE);
	
	//$GPGSV ///////////////////////////////////////////////////////////////
	//정보부족으로 인해 만들어 내기 힘듬
#else
	int i;
	atCHAR cpNMEA[1024];	
	TGPSInfo gpsinfo;
	
	gpsinfo.dwAngle = stGPSDat.nAngle;
	gpsinfo.dwSpeed = (float)(stGPSDat.nSpeed / 1.852);
	gpsinfo.gps_time.nYear = stGPSDat.gps_datetime.nYear;
	gpsinfo.gps_time.nMonth = stGPSDat.gps_datetime.nMonth;
	gpsinfo.gps_time.nDay = stGPSDat.gps_datetime.nDay;
	gpsinfo.gps_time.nHour = stGPSDat.gps_datetime.nHour;
	gpsinfo.gps_time.nMinute = stGPSDat.gps_datetime.nMinute;
	gpsinfo.gps_time.nSec = stGPSDat.gps_datetime.nSec;
	gpsinfo.rLatitude = REALTOFLOAT(stGPSDat.pos.rLatitude);
	gpsinfo.rLongitude = REALTOFLOAT(stGPSDat.pos.rLongitude);
	gpsinfo.rAltitude = REALTOFLOAT(stGPSDat.pos.rAltitude);
	gpsinfo.rHDOP = REALTOFLOAT(stGPSDat.rHDOP);
	gpsinfo.rVDOP = REALTOFLOAT(stGPSDat.rVDOP);
	gpsinfo.rPDOP = REALTOFLOAT(stGPSDat.rPDOP);
	for( i = 0; i < 32; i++ ){
	 	gpsinfo.satellite[i].dwPRN = stGPSDat.satellite[i].nPRN;
	 	gpsinfo.satellite[i].dwElev = stGPSDat.satellite[i].nElevation;
	 	gpsinfo.satellite[i].dwAzi = stGPSDat.satellite[i].nAzimuth;
	 	gpsinfo.satellite[i].dwSNR = stGPSDat.satellite[i].nSNR;
	 	gpsinfo.satellite[i].fUse = stGPSDat.satellite[i].bUsed;
	}
	//WORD_T	wGPSFix;	// GPS quality indicator (0=invalid; 1=GPS fix; 2=Diff. GPS fix)
	//WORD_T	wFixMode1;   // GSA::Mode1 	(M=Manual, forced to operate in 2D or 3D; A=Automatic, 3D/2D)
	//WORD_T	wFixMode2;   // GSA::Mode2  (1=Fix not available; 2=2D; 3=3D)
	//WORD_T	wDataState;	// GPS Quality (A=Validate; V=navigation receiver warning)
	gpsinfo.wDataState = (1)? 'A' : 'V';
	gpsinfo.wFixMode1 = 'A';
	gpsinfo.wFixMode2 = stGPSDat.n3DState;
	gpsinfo.wGPSFix = (stGPSDat.nFixState && stGPSDat.nQuality > 0)? 1 : 0;
	gpsinfo.wSU = stGPSDat.nSU;
	gpsinfo.wSV = stGPSDat.nSV;
	
	int len = NMEA_BuildNMEAPacket( &gpsinfo, cpNMEA, sizeof(cpNMEA) );
	_GPS_WriteLog(pGPS, cpNMEA, len, atTRUE);
#endif

	return atTRUE;
}

atBOOL OEM_SYSAPI_GPS_CloseLoging(OEM_SYS_GPS hOemGPS)
{
	CMyGPS *pGPS = (CMyGPS *)hOemGPS;
	if( pGPS == atNULL ) return atFALSE;
		
	if(pGPS->m_bLogEnable && pGPS->m_hLog)
	{
		//OEM_SYSAPI_GPS_WriteLog(hOemGPS, atNULL, 0,atTRUE); // 버퍼의 내용을 flush시켜준다.
		SYSAPI_CloseFile(pGPS->m_hLog);
	}
	
	pGPS->m_hLog = atNULL;
	
	delete [] pGPS->m_pLogBuf ;
	pGPS->m_pLogBuf = atNULL;
	
	pGPS->m_nLogBuf = 0;
	                        
	return atTRUE;
}
