#include "SYSAPI.h"
#include "OEM_SA_gps.h"
#include <windows.h>
#include <stdio.h>
#include <Gpsapi.h>

#include "base/TStream.h"



#define CURRENT_GPS_VERSION GPS_VERSION_1


class CMyGPS 
{ 
public:
	HANDLE hGPSDevice;

	HANDLE hThread;

	HANDLE hEventGPS[2];

	SYS_HFILE hLogFile;

	atBOOL bLogEnagled;

	GPS_POSITION gpos;

	atBOOL  terminated;

	TStream buffer;

	CMyGPS(void) /// : CGPS()
	{	
		hGPSDevice = atNULL;
		hThread    = NULL;
		hEventGPS[0] = NULL;
		hEventGPS[1] = NULL;
		hLogFile     = NULL;
		terminated = atFALSE;
		bLogEnagled = atFALSE;
		memset(&gpos, 0, sizeof(gpos));
	}

	~CMyGPS()
	{
	}	
};


#define LOG_BUFFER_SIZE  (32 * 1024 * 1024)

#define GPS_LOG_VER  1


static void _WriteGPSLOG(OEM_SYS_GPS hOemGPS)
{
	CMyGPS *pGPS = (CMyGPS *)hOemGPS;
	if( pGPS == atNULL ) return; 
	if(!pGPS->hGPSDevice) return;
	if(!pGPS->hLogFile) return;

	char temp[256];

	GPS_POSITION gpos = pGPS->gpos;

	// dwValidField
	// dwFlags
	// stUTCTime
	// lat
	// lon,
#if 0
	sprintf(temp, "$BGPS:%d,%d,%d,%d,%d,%d,%d,%d,%.6f,%.6f,%.2f,%.5f,%.5f,%.5f,%d,%d,%d,%.2f,%.2f,%.2f\r\n",
					gpos.dwValidFields, gpos.dwFlags,
					gpos.stUTCTime.wYear, gpos.stUTCTime.wMonth, gpos.stUTCTime.wDay,
					gpos.stUTCTime.wHour, gpos.stUTCTime.wMinute, gpos.stUTCTime.wSecond,
					gpos.dblLatitude, gpos.dblLongitude,
					gpos.flSpeed, gpos.flHeading,
					gpos.dblMagneticVariation,
					gpos.flAltitudeWRTSeaLevel, gpos.flAltitudeWRTEllipsoid,
					gpos.FixQuality,gpos.FixType,gpos.SelectionType,
					gpos.flPositionDilutionOfPrecision,	
					gpos.flHorizontalDilutionOfPrecision,
					gpos.flVerticalDilutionOfPrecision);
#else

	sprintf(temp, "$BGLPOS,%s,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%.6f,%.6f,%.2f,%.5f,%.5f,%.5f,%d,%d,%d,%.2f,%.2f,%.2f\r\n",
					"SRP",
					GPS_LOG_VER,
					0, 
					gpos.dwValidFields, gpos.dwFlags,
					gpos.stUTCTime.wYear, gpos.stUTCTime.wMonth, gpos.stUTCTime.wDay,
					gpos.stUTCTime.wHour, gpos.stUTCTime.wMinute, gpos.stUTCTime.wSecond,
					gpos.dblLatitude, gpos.dblLongitude,
					gpos.flSpeed, gpos.flHeading,
					gpos.dblMagneticVariation,
					gpos.flAltitudeWRTSeaLevel, gpos.flAltitudeWRTEllipsoid,
					gpos.FixQuality,gpos.FixType,gpos.SelectionType,
					gpos.flPositionDilutionOfPrecision,	
					gpos.flHorizontalDilutionOfPrecision,
					gpos.flVerticalDilutionOfPrecision);


#endif

	if(pGPS->buffer.size() + strlen(temp) > LOG_BUFFER_SIZE)
	{
		SYSAPI_WriteFile(pGPS->hLogFile, pGPS->buffer.begin(), pGPS->buffer.size());
		SYSAPI_FlushFileBuffer(pGPS->hLogFile);
		pGPS->buffer.clear();
	}

	pGPS->buffer.add(temp, strlen(temp));
}

static atBOOL _GetPositionWM( OEM_SYS_GPS hOemGPS )
{
	CMyGPS *pGPS = (CMyGPS *)hOemGPS;
	if( pGPS == atNULL ) return SYS_GPS_STATE_INVALIDE;

	if(!pGPS->hGPSDevice) return SYS_GPS_STATE_INVALIDE;

	GPS_POSITION gpos;
	memset(&gpos, 0, sizeof(gpos));
	gpos.dwVersion = CURRENT_GPS_VERSION;
	gpos.dwSize    = sizeof(gpos);

	if(GPSGetPosition(pGPS->hGPSDevice,&gpos, 1000, 0) != ERROR_SUCCESS)
		return atFALSE;

	pGPS->gpos = gpos;

	return atTRUE;
}


static DWORD gpsThreadProcWM(void *lpParam)
{
	DWORD res;
	CMyGPS* pGPS = (CMyGPS*)lpParam;

	while(pGPS->hThread && !pGPS->terminated)
	{
		if(pGPS->hEventGPS[0] == 0 || pGPS->hEventGPS[1] == 0) break;

		res = WaitForMultipleObjects(2, pGPS->hEventGPS, FALSE, 1000);

		if(pGPS->terminated) break;
		if(res == WAIT_FAILED) break;
		if(res == WAIT_OBJECT_0 || res == (WAIT_OBJECT_0 +1))
		{
			if(_GetPositionWM(pGPS))
			{
				_WriteGPSLOG(pGPS);
			}
		}
	}

	return 0;
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

	return (OEM_SYS_GPS)pGPS;
}

void 		OEM_SYSAPI_GPS_DestroyGPS( OEM_SYS_GPS hOemGPS )
{
	CMyGPS *pGPS = (CMyGPS *)hOemGPS;
	if( pGPS == atNULL ) return;

	OEM_SYSAPI_GPS_CloseGPS(hOemGPS);

	delete pGPS;
}

atBOOL 		OEM_SYSAPI_GPS_SetOEMParam( OEM_SYS_GPS hOemGPS, int nType, long lParam1, long lParam2 )
{
	CMyGPS *pGPS = (CMyGPS *)hOemGPS;
	if( pGPS == atNULL ) return atFALSE;

    return atTRUE;
}


atBOOL 		OEM_SYSAPI_GPS_OpenGPS( OEM_SYS_GPS hOemGPS )
{
	CMyGPS *pGPS = (CMyGPS *)hOemGPS;
	if( pGPS == atNULL ) return atFALSE;

	
	pGPS->hEventGPS[0] = CreateEvent(NULL, FALSE, FALSE,NULL );
	pGPS->hEventGPS[1] = CreateEvent(NULL, FALSE, FALSE,NULL );

	pGPS->hGPSDevice = GPSOpenDevice(pGPS->hEventGPS[0], pGPS->hEventGPS[1], NULL, 0);
	if(!pGPS->hGPSDevice) return atFALSE;

	DWORD ThreadId;
	pGPS->hThread = CreateThread( NULL, 0, (LPTHREAD_START_ROUTINE)gpsThreadProcWM, (void *)pGPS, 0, &ThreadId );
	if(!pGPS->hThread)
	{
		GPSCloseDevice(pGPS->hGPSDevice);
		pGPS->hGPSDevice = NULL;
	}

	return atTRUE;
}

atBOOL OEM_SYSAPI_GPS_IsConnected(OEM_SYS_GPS hOemGPS)
{
	CMyGPS *pGPS = (CMyGPS *)hOemGPS;
	if( pGPS == atNULL ) return atFALSE;

	return (pGPS->hGPSDevice != NULL);
}

atBOOL 		OEM_SYSAPI_GPS_CloseGPS( OEM_SYS_GPS hOemGPS )
{
	CMyGPS *pGPS = (CMyGPS *)hOemGPS;
	if( pGPS == atNULL ) return atFALSE;

	if(!pGPS->hGPSDevice) return atFALSE;

	int ret = GPSCloseDevice(pGPS->hGPSDevice);
	if(ret == ERROR_SUCCESS)
		pGPS->hGPSDevice = atNULL;

	SetEvent(pGPS->hEventGPS[0]);

	pGPS->terminated = atTRUE;

	if(pGPS->hThread)
	{
		CloseHandle(pGPS->hThread);
		pGPS->hThread = NULL;
	}

	if(pGPS->hEventGPS[0])
	{
		CloseHandle(pGPS->hEventGPS[0]);
		pGPS->hEventGPS[0] = NULL;
	}

	if(pGPS->hEventGPS[1])
	{
		CloseHandle(pGPS->hEventGPS[1]);
		pGPS->hEventGPS[1]  = NULL;
	}

	OEM_SYSAPI_GPS_CloseLoging(hOemGPS);

	return (ret == ERROR_SUCCESS);
}


atINT		OEM_SYSAPI_GPS_GetState( OEM_SYS_GPS hOemGPS )
{
	CMyGPS *pGPS = (CMyGPS *)hOemGPS;
	if( pGPS == atNULL ) return SYS_GPS_STATE_INVALIDE;

	if(!pGPS->hGPSDevice) return SYS_GPS_STATE_INVALIDE;

	switch(pGPS->gpos.FixType)
	{
		case GPS_FIX_UNKNOWN :
			return SYS_GPS_STATE_NOTFIXED;
		case GPS_FIX_2D :
			return SYS_GPS_STATE_2D;
		case GPS_FIX_3D :
			return SYS_GPS_STATE_3D;
		default:
			return SYS_GPS_STATE_INVALIDE;
	}

}

atBOOL OEM_SYSAPI_GPS_SetProcessFunc( OEM_SYS_GPS hOemGPS, SYS_GPS_PROC_FUNC *fnProc )
{
	return atTRUE;
}

atBOOL 		OEM_SYSAPI_GPS_GetCoord( OEM_SYS_GPS hOemGPS, SYS_GPS_POSITION *lpGpsPos )
{
	CMyGPS *pGPS = (CMyGPS *)hOemGPS;
	if( pGPS == atNULL ) return atFALSE;

	if(!pGPS->hGPSDevice) return atFALSE;

	lpGpsPos->rLongitude = FLOATTOREAL(pGPS->gpos.dblLongitude);
	lpGpsPos->rLatitude =  FLOATTOREAL(pGPS->gpos.dblLatitude);
	lpGpsPos->rAltitude =  FLOATTOREAL(pGPS->gpos.flAltitudeWRTSeaLevel);
	
	return atTRUE;
}

atBOOL 		OEM_SYSAPI_GPS_GetTime( OEM_SYS_GPS hOemGPS, SYS_DATETIME *datetime ) // 시간
{
	CMyGPS *pGPS = (CMyGPS *)hOemGPS;
	if( pGPS == atNULL ) return atFALSE;
	if(!pGPS->hGPSDevice) return atFALSE;

	SET_SYS_DATETIME( *datetime,
		pGPS->gpos.stUTCTime.wYear, pGPS->gpos.stUTCTime.wMonth,  pGPS->gpos.stUTCTime.wDay,
		pGPS->gpos.stUTCTime.wHour, pGPS->gpos.stUTCTime.wMinute, pGPS->gpos.stUTCTime.wSecond);

	return atTRUE;
}

double 		OEM_SYSAPI_GPS_GetSpeed ( OEM_SYS_GPS hOemGPS ) // 속도 (km/h)
{
	CMyGPS *pGPS = (CMyGPS *)hOemGPS;
	if( pGPS == atNULL ) return 0;
	if(!pGPS->hGPSDevice) return 0;

	return pGPS->gpos.flSpeed * 1.851998;   // mile -> km/h
}

atLONG		OEM_SYSAPI_GPS_GetAngle( OEM_SYS_GPS hOemGPS ) // 진행 각도 (degree : 0 - 359)
{
	CMyGPS *pGPS = (CMyGPS *)hOemGPS;
	if( pGPS == atNULL ) return 0;
	if(!pGPS->hGPSDevice) return 0;

	return pGPS->gpos.flHeading;
}

atINT 		OEM_SYSAPI_GPS_GetNumSV ( OEM_SYS_GPS hOemGPS ) // 보이는 위성 갯수
{
	CMyGPS *pGPS = (CMyGPS *)hOemGPS;
	if( pGPS == atNULL ) return 0;
	if(!pGPS->hGPSDevice) return 0;

	return pGPS->gpos.dwSatellitesInView;
}

atINT 		OEM_SYSAPI_GPS_GetNumSU ( OEM_SYS_GPS hOemGPS ) // 사용중인 위성 갯수 
{
	CMyGPS *pGPS = (CMyGPS *)hOemGPS;
	if( pGPS == atNULL ) return 0;
	if(!pGPS->hGPSDevice) return 0;

	return pGPS->gpos.dwSatelliteCount;
}

atREAL 		OEM_SYSAPI_GPS_GetPDOP ( OEM_SYS_GPS hOemGPS ) // Position Dilution Of Precision
{
	CMyGPS *pGPS = (CMyGPS *)hOemGPS;
	if( pGPS == atNULL ) return  0;
	if(!pGPS->hGPSDevice) return 0;

	return pGPS->gpos.flPositionDilutionOfPrecision;
}

atREAL 		OEM_SYSAPI_GPS_GetHDOP ( OEM_SYS_GPS hOemGPS ) // Horizontal Dilution Of Precision
{
	CMyGPS *pGPS = (CMyGPS *)hOemGPS;
	if( pGPS == atNULL ) return 0;
	if(!pGPS->hGPSDevice) return 0;

	return FLOATTOREAL(pGPS->gpos.flHorizontalDilutionOfPrecision);
}

atREAL 		OEM_SYSAPI_GPS_GetVDOP ( OEM_SYS_GPS hOemGPS ) // Vertical Dilution Of Precision
{
	CMyGPS *pGPS = (CMyGPS *)hOemGPS;
	if( pGPS == atNULL ) return 0;
	if(!pGPS->hGPSDevice) return 0;

	return pGPS->gpos.flVerticalDilutionOfPrecision;
}

atBOOL 		OEM_SYSAPI_GPS_GetSatelliteInfo( OEM_SYS_GPS hOemGPS, atINT nIndex, SYS_GPS_SATELLITE *lpSatelliteEntry )
{
	CMyGPS *pGPS = (CMyGPS *)hOemGPS;
	if( pGPS == atNULL ) return 0;
	if(!pGPS->hGPSDevice) return 0;

	if ( nIndex >= MAX_SATELLITE_NUM )
		return atFALSE ;

	if ( nIndex >= pGPS->gpos.dwSatellitesInView )
		return atFALSE ;

	lpSatelliteEntry->nPRN = pGPS->gpos.rgdwSatellitesInViewPRNs[nIndex];
	lpSatelliteEntry->nElevation = pGPS->gpos.rgdwSatellitesInViewElevation[nIndex];
	lpSatelliteEntry->nAzimuth = pGPS->gpos.rgdwSatellitesInViewAzimuth[nIndex];
	lpSatelliteEntry->nSNR = pGPS->gpos.rgdwSatellitesInViewSignalToNoiseRatio[nIndex];
	lpSatelliteEntry->bUsed = pGPS->gpos.rgdwSatellitesUsedPRNs[nIndex];
	
	return atTRUE;	
}

atBOOL		OEM_SYSAPI_GPS_ResetSatelliteAll( OEM_SYS_GPS hOemGPS )
{
	CMyGPS *pGPS = (CMyGPS *)hOemGPS;
	if( pGPS == atNULL ) return atFALSE;


	/*
	SYSAPI_memset ( pGPS->m_GPSInfo.satellite, 0x00, sizeof(__GPSSatellite) * MAX_SATELLITE_NUM ) ;
	*/
	return atTRUE;
}

atBOOL OEM_SYSAPI_GPS_EmulateGPSData( OEM_SYS_GPS hOemGPS, atINT nPktCat, SYS_GPS_INFO *lpGpsInfo )
{
	CMyGPS *pGPS = (CMyGPS *)hOemGPS;
	if( pGPS == atNULL ) return atFALSE;

#if 0 // to-do next

	if( nPktCat == SYS_GPS_PKT_GPGGA )
	{
		pGPS->m_GPSInfo.nFixState = lpGpsInfo->nFixState;
		pGPS->m_GPSInfo.wSU       = lpGpsInfo->nSU;
		pGPS->m_GPSInfo.rHDOP     = lpGpsInfo->rHDOP;
	}
	
	if( nPktCat == SYS_GPS_PKT_GPGSA )
	{
		pGPS->m_GPSInfo.nFixState = lpGpsInfo->nFixState;
		pGPS->m_GPSInfo.n3DState  = lpGpsInfo->n3DState;
//		pGPS->m_GPSInfo.rHDOP    = lpGpsInfo->rHDOP;
		pGPS->m_GPSInfo.rVDOP     = lpGpsInfo->rVDOP;
		pGPS->m_GPSInfo.rPDOP     = lpGpsInfo->rPDOP;
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
//		pGPS->m_GPSInfo.nFixState = lpGpsInfo->nFixState;
		pGPS->m_GPSInfo.nQuality = lpGpsInfo->nQuality;
		pGPS->m_GPSInfo.dblLon = REALTOFLOAT(lpGpsInfo->pos.rLongitude);
		pGPS->m_GPSInfo.dblLat = REALTOFLOAT(lpGpsInfo->pos.rLatitude);
		pGPS->m_GPSInfo.dblAlt = REALTOFLOAT(lpGpsInfo->pos.rAltitude);
		pGPS->m_GPSInfo.dwSpeed = lpGpsInfo->nSpeed;
		pGPS->m_GPSInfo.dwAngle = lpGpsInfo->nAngle;
		pGPS->m_GPSInfo.gps_time = lpGpsInfo->gps_datetime;
	}
#endif	
	return atTRUE;
}


atLONG OEM_SYSAPI_GPS_ExtGPSMethod( OEM_SYS_GPS hOemGPS, atINT func_no, atLONG param1, atLONG param2, atLONG param3, atLONG param4, atLONG param5, atLONG param6 )
{
	return 0;
}


// Support GPS Log File
atBOOL OEM_SYSAPI_GPS_CreateLogFile(OEM_SYS_GPS hOemGPS, atTCHAR* filename)
{
	CMyGPS *pGPS = (CMyGPS *)hOemGPS;
	if( pGPS == atNULL ) return atFALSE;

	pGPS->hLogFile = SYSAPI_OpenFile(filename, SYS_FILEOPEN_CREATE);

	if(pGPS->hLogFile)
	{
		char temp[256];
		strcpy(temp, "$BGLPOS,BEAGLE GPS LOG\r\n");
		SYSAPI_WriteFile(pGPS->hLogFile, temp,  strlen(temp));
	}


	return atFALSE;
}

atVOID OEM_SYSAPI_GPS_SetLogEnable(OEM_SYS_GPS hOemGPS, atBOOL bEnable)
{
	return ;
}

atBOOL OEM_SYSAPI_GPS_IsLogEnable(OEM_SYS_GPS hOemGPS)
{
	return atFALSE;
}

atVOID OEM_SYSAPI_GPS_SetLogStatus(OEM_SYS_GPS hOemGPS, atBOOL bEnable)
{
	return ;
}

atBOOL OEM_SYSAPI_GPS_Loging(OEM_SYS_GPS hOemGPS, SYS_GPS_INFO lpGPSDat)
{
	return atFALSE;
}

atBOOL OEM_SYSAPI_GPS_CloseLoging(OEM_SYS_GPS hOemGPS)
{
	CMyGPS *pGPS = (CMyGPS *)hOemGPS;
	if( pGPS == atNULL ) return atFALSE;

	if(pGPS->hLogFile)
	{
		if(pGPS->buffer.size())
		{
			SYSAPI_WriteFile(pGPS->hLogFile, pGPS->buffer.begin(), pGPS->buffer.size());
			pGPS->buffer.clear();
		}

		char temp[256];
		strcpy(temp, "$BGLPOS,BEAGLE GPS LOG - END\r\n");
		SYSAPI_WriteFile(pGPS->hLogFile, temp,  strlen(temp));
		SYSAPI_CloseFile(pGPS->hLogFile);
		pGPS->hLogFile = atNULL;
	}

	return atFALSE;
}


void init_sysapi_gps_oem() 
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
