#include "SA_Types.h"
#include "SYSAPI.h"
#include "OEM_SA_GPS.h"


#ifdef __USES_SYSAPI_GPS_


static atBOOL g_bSysGPSIntitialized = atFALSE;


static OEM_SYSAPI_GPS_T tOemFn;


atVOID OEM_SYSAPI_GPS_SetOemFn(OEM_SYSAPI_GPS_T *fnOem)
{
	tOemFn = *fnOem;
}


atVOID OEM_SYSAPI_GPS_GetOemFn(OEM_SYSAPI_GPS_T *fnOem)
{
	*fnOem = tOemFn;
}


atBOOL SYSAPI_InitGPS( atLONG lSysInst, atLONG lInitData1, atLONG lInitData2 )
{
	if( g_bSysGPSIntitialized ) return atTRUE;

	if(tOemFn.fnInitGPS != atNULL) {
		g_bSysGPSIntitialized = tOemFn.fnInitGPS( lSysInst, lInitData1, lInitData2 );
	}

	return g_bSysGPSIntitialized;
}

atVOID SYSAPI_FinalGPS( NOARGS )
{
	if( !g_bSysGPSIntitialized ) return;
	
	if( tOemFn.fnFinalGPS != atNULL ) {
		tOemFn.fnFinalGPS();
	}
	
	g_bSysGPSIntitialized = atFALSE;
}


SYS_HGPS SYSAPI_GPS_CreateGPS( NOARGS ) 
{
	if( tOemFn.fnGPS_CreateGPS != atNULL ) {
		return (SYS_HGPS)tOemFn.fnGPS_CreateGPS();
	}

	return (SYS_HGPS)atNULL;
}


void SYSAPI_GPS_DestroyGPS( SYS_HGPS hGPS )
{
	if( tOemFn.fnGPS_DestroyGPS != atNULL ) {
		tOemFn.fnGPS_DestroyGPS( (OEM_SYS_GPS)hGPS );
	}
}

atBOOL SYSAPI_GPS_SetOEMParam( SYS_HGPS hGPS, int nType, long lParam1, long lParam2 )
{
	if( tOemFn.fnGPS_SetOEMParam != atNULL ) {
		// UINT port, UINT baudRate
		return tOemFn.fnGPS_SetOEMParam( (OEM_SYS_GPS)hGPS, nType, lParam1, lParam2 );
	}

	return atFALSE;
}


atBOOL SYSAPI_GPS_OpenGPS( SYS_HGPS hGPS )
{
	if( tOemFn.fnGPS_OpenGPS != atNULL ) {
		return tOemFn.fnGPS_OpenGPS( (OEM_SYS_GPS)hGPS );
	}

	return atFALSE;
}


atBOOL SYSAPI_GPS_IsConnected(SYS_HGPS hGPS)
{
	if( tOemFn.fnGPS_IsConnected != atNULL ) {
		return tOemFn.fnGPS_IsConnected((OEM_SYS_GPS)hGPS);
	}

	return atFALSE;
}


atBOOL SYSAPI_GPS_CloseGPS( SYS_HGPS hGPS )
{
	if( tOemFn.fnGPS_CloseGPS != atNULL ) {
		return tOemFn.fnGPS_CloseGPS( (OEM_SYS_GPS)hGPS );
	}

	return atFALSE;
}


atINT SYSAPI_GPS_GetState( SYS_HGPS hGPS )
{
	if( tOemFn.fnGPS_GetState != atNULL ) {
		return tOemFn.fnGPS_GetState( (OEM_SYS_GPS)hGPS );
	}

	return 0;
}


atBOOL SYSAPI_GPS_SetProcessFunc( SYS_HGPS hGPS, SYS_GPS_PROC_FUNC *fnProc )
{
	if( tOemFn.fnGPS_SetProcessFunc != atNULL ) {
		return tOemFn.fnGPS_SetProcessFunc( (OEM_SYS_GPS)hGPS, fnProc );
	}

	return atFALSE;
}


 // 경도/위도/고도 (도 단위)
atBOOL SYSAPI_GPS_GetCoord( SYS_HGPS hGPS, SYS_GPS_POSITION *lpGpsPos )
{
	if( tOemFn.fnGPS_GetCoord != atNULL ) {
		return tOemFn.fnGPS_GetCoord( (OEM_SYS_GPS)hGPS, lpGpsPos );
	}

	return atFALSE;
}


atBOOL SYSAPI_GPS_GetTime( SYS_HGPS hGPS, SYS_DATETIME *datetime ) // 시간
{
	if( tOemFn.fnGPS_GetTime != atNULL ) {
		return tOemFn.fnGPS_GetTime( (OEM_SYS_GPS)hGPS, datetime );
	}

	return atFALSE;
}


double SYSAPI_GPS_GetSpeed ( SYS_HGPS hGPS ) // 속도 (km/h)
{
	if( tOemFn.fnGPS_GetSpeed != atNULL ) {
		return tOemFn.fnGPS_GetSpeed( (OEM_SYS_GPS)hGPS );
	}

	return 0;
}


atLONG	SYSAPI_GPS_GetAngle( SYS_HGPS hGPS ) // 진행 각도 (degree : 0 - 359)
{
	if( tOemFn.fnGPS_GetAngle != atNULL ) {
		return tOemFn.fnGPS_GetAngle( (OEM_SYS_GPS)hGPS );
	}

	return 0;
}


atINT SYSAPI_GPS_GetNumSV ( SYS_HGPS hGPS ) // 보이는 위성 갯수
{
	if( tOemFn.fnGPS_GetNumSV != atNULL ) {
		return tOemFn.fnGPS_GetNumSV( (OEM_SYS_GPS)hGPS );
	}

	return 0;
}


atINT SYSAPI_GPS_GetNumSU ( SYS_HGPS hGPS ) // 사용중인 위성 갯수 
{
	if( tOemFn.fnGPS_GetNumSU != atNULL ) {
		return tOemFn.fnGPS_GetNumSU( (OEM_SYS_GPS)hGPS );
	}

	return 0;
}


atREAL SYSAPI_GPS_GetPDOP ( SYS_HGPS hGPS ) // Position Dilution Of Precision
{
	if( tOemFn.fnGPS_GetPDOP != atNULL ) {
		return tOemFn.fnGPS_GetPDOP( (OEM_SYS_GPS)hGPS );
	}

	return 0;
}


atREAL SYSAPI_GPS_GetHDOP ( SYS_HGPS hGPS ) // Horizontal Dilution Of Precision
{
	if( tOemFn.fnGPS_GetHDOP != atNULL ) {
		return tOemFn.fnGPS_GetHDOP( (OEM_SYS_GPS)hGPS );
	}

	return 0;
}


atREAL SYSAPI_GPS_GetVDOP ( SYS_HGPS hGPS ) // Vertical Dilution Of Precision
{
	if( tOemFn.fnGPS_GetVDOP != atNULL ) {
		return tOemFn.fnGPS_GetVDOP( (OEM_SYS_GPS)hGPS );
	}

	return 0;
}


atBOOL SYSAPI_GPS_GetSatelliteInfo( SYS_HGPS hGPS, atINT nIndex, SYS_GPS_SATELLITE *lpSatelliteEntry )
{
	if( tOemFn.fnGPS_GetSatelliteInfo != atNULL ) {
		return tOemFn.fnGPS_GetSatelliteInfo( (OEM_SYS_GPS)hGPS, nIndex, lpSatelliteEntry );
	}

	return atFALSE;
}


atBOOL SYSAPI_GPS_ResetSatelliteAll( SYS_HGPS hGPS )
{
	if( tOemFn.fnGPS_ResetSatelliteAll != atNULL ) {
		return tOemFn.fnGPS_ResetSatelliteAll( (OEM_SYS_GPS)hGPS );
	}

	return atFALSE;
}


atBOOL SYSAPI_GPS_EmulateGPSData( SYS_HGPS hGPS, atINT nPktCat, SYS_GPS_INFO *lpGpsInfo )
{
	if( tOemFn.fnGPS_EmulateGPSData != atNULL ) {
		return tOemFn.fnGPS_EmulateGPSData( (OEM_SYS_GPS)hGPS, nPktCat, lpGpsInfo );
	}

	return atFALSE;
}


atLONG SYSAPI_GPS_ExtGPSMethod( SYS_HGPS hGPS, atINT func_no, atLONG param1, atLONG param2, atLONG param3, atLONG param4, atLONG param5, atLONG param6 )
{
	if( tOemFn.fnGPS_ExtGPSMethod != atNULL ) {
		return tOemFn.fnGPS_ExtGPSMethod( (OEM_SYS_GPS)hGPS, func_no, param1, param2, param3, param4, param5, param6 );
	}

	return 0;
}


////////////////////////////////////////////////////////////////////////
// Support GPS Log Data
atBOOL SYSAPI_GPS_CreateLogFile(SYS_HGPS hGPS, atTCHAR* filename)
{
	if( tOemFn.fnGPS_CreateLogFile != atNULL ) {
		return tOemFn.fnGPS_CreateLogFile((OEM_SYS_GPS)hGPS, filename);
	}

	return atFALSE;
}

atVOID SYSAPI_GPS_SetLogEnable(SYS_HGPS hGPS, atBOOL bEnable)
{
	if( tOemFn.fnGPS_SetLogEnable != atNULL ) {
		tOemFn.fnGPS_SetLogEnable((OEM_SYS_GPS)hGPS, bEnable);
	}
}

atBOOL SYSAPI_GPS_IsLogEnable(SYS_HGPS hGPS)
{
	if( tOemFn.fnGPS_IsLogEnable != atNULL ) {
		return tOemFn.fnGPS_IsLogEnable((OEM_SYS_GPS)hGPS);
	}

	return atFALSE;
}

atVOID SYSAPI_GPS_SetLogStatus(SYS_HGPS hGPS, atBOOL bEnable)
{
	if( tOemFn.fnGPS_SetLogStatus != atNULL ) {
		tOemFn.fnGPS_SetLogStatus((OEM_SYS_GPS)hGPS, bEnable);
	}
}

#endif

