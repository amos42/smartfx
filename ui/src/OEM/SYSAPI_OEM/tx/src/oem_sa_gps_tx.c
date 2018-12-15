#include "SYSAPI.h"
#include "OEM_SA_gps.h"


atBOOL		OEM_SYSAPI_InitGPS( atLONG lSysInst, atLONG lInitData1, atLONG lInitData2 )
{
	return atTRUE;
}

atVOID		OEM_SYSAPI_FinalGPS( NOARGS )
{
}


OEM_SYS_GPS 	OEM_SYSAPI_GPS_CreateGPS( void )
{
	return atNULL;
}

void 		OEM_SYSAPI_GPS_DestroyGPS( OEM_SYS_GPS hOemGPS )
{
}

atBOOL 		OEM_SYSAPI_GPS_SetOEMParam( OEM_SYS_GPS hOemGPS, int nType, long lParam1, long lParam2 )
{
    return atTRUE;
}

atBOOL 		OEM_SYSAPI_GPS_OpenGPS( OEM_SYS_GPS hOemGPS )
{
	return atTRUE;
}

atBOOL 		OEM_SYSAPI_GPS_CloseGPS( OEM_SYS_GPS hOemGPS )
{
	return 0;	
}

atINT		OEM_SYSAPI_GPS_GetState( OEM_SYS_GPS hOemGPS )
{
	return 0;
}

atBOOL OEM_SYSAPI_GPS_SetProcessFunc( OEM_SYS_GPS hOemGPS, SYS_GPS_PROC_FUNC *fnProc )
{
	return atTRUE;
}

atBOOL 		OEM_SYSAPI_GPS_GetCoord( OEM_SYS_GPS hOemGPS, SYS_GPS_POSITION *lpGpsPos )
{
	return atTRUE;
}

atBOOL 		OEM_SYSAPI_GPS_GetTime( OEM_SYS_GPS hOemGPS, SYS_DATETIME *datetime ) // 시간
{
	return atTRUE;
}

double 		OEM_SYSAPI_GPS_GetSpeed ( OEM_SYS_GPS hOemGPS ) // 속도 (km/h)
{
	return 0;	
}

atLONG		OEM_SYSAPI_GPS_GetAngle( OEM_SYS_GPS hOemGPS ) // 진행 각도 (degree : 0 - 359)
{
	return 0;
}

atINT 		OEM_SYSAPI_GPS_GetNumSV ( OEM_SYS_GPS hOemGPS ) // 보이는 위성 갯수
{
	return 0;
}

atINT 		OEM_SYSAPI_GPS_GetNumSU ( OEM_SYS_GPS hOemGPS ) // 사용중인 위성 갯수 
{
	return 0;
}

atREAL 		OEM_SYSAPI_GPS_GetPDOP ( OEM_SYS_GPS hOemGPS ) // Position Dilution Of Precision
{
	return 0;
}

atREAL 		OEM_SYSAPI_GPS_GetHDOP ( OEM_SYS_GPS hOemGPS ) // Horizontal Dilution Of Precision
{
	return 0;
}

atREAL 		OEM_SYSAPI_GPS_GetVDOP ( OEM_SYS_GPS hOemGPS ) // Vertical Dilution Of Precision
{
	return 0;
}

atBOOL 		OEM_SYSAPI_GPS_GetSatelliteInfo( OEM_SYS_GPS hOemGPS, atINT nIndex, SYS_GPS_SATELLITE *lpSatelliteEntry )
{
	return atTRUE;	
}

atBOOL		OEM_SYSAPI_GPS_ResetSatelliteAll( OEM_SYS_GPS hOemGPS )
{
	return atTRUE;
}

atBOOL OEM_SYSAPI_GPS_EmulateGPSData( OEM_SYS_GPS hOemGPS, atINT nPktCat, SYS_GPS_INFO *lpGpsInfo )
{
	return atTRUE;
}


atLONG OEM_SYSAPI_GPS_ExtGPSMethod( OEM_SYS_GPS hOemGPS, atINT func_no, atLONG param1, atLONG param2, atLONG param3, atLONG param4, atLONG param5, atLONG param6 )
{
	return 0;
}

// Support GPS Log File
atBOOL OEM_SYSAPI_GPS_CreateLogFile(OEM_SYS_GPS hOemGPS, atTCHAR* filename)
{
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
	return atFALSE;
}
