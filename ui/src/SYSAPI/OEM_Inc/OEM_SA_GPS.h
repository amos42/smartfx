#ifndef __OEM_SA_GPS_H_
#define __OEM_SA_GPS_H_


#include "SA_Types.h"
#include "SA_GPS.h"

#ifdef __cplusplus
extern "C" {
#endif


typedef atHANDLE OEM_SYS_GPS;


typedef atBOOL		FN_OEM_SYSAPI_InitGPS( atLONG lSysInst, atLONG lInitData1, atLONG lInitData2 );
typedef atVOID		FN_OEM_SYSAPI_FinalGPS( NOARGS );

typedef OEM_SYS_GPS FN_OEM_SYSAPI_GPS_CreateGPS( void );
typedef void 		FN_OEM_SYSAPI_GPS_DestroyGPS( OEM_SYS_GPS hOemGPS );
typedef atBOOL 		FN_OEM_SYSAPI_GPS_SetOEMParam( OEM_SYS_GPS hOemGPS, int nType, long lParam1, long lParam2 );
typedef atBOOL 		FN_OEM_SYSAPI_GPS_OpenGPS( OEM_SYS_GPS hOemGPS );
typedef atBOOL 		FN_OEM_SYSAPI_GPS_IsConnected( OEM_SYS_GPS hOemGPS);
typedef atBOOL 		FN_OEM_SYSAPI_GPS_CloseGPS( OEM_SYS_GPS hOemGPS );
typedef atINT		FN_OEM_SYSAPI_GPS_GetState( OEM_SYS_GPS hOemGPS );
typedef atBOOL 		FN_OEM_SYSAPI_GPS_SetProcessFunc( OEM_SYS_GPS hOemGPS, SYS_GPS_PROC_FUNC *fnProc );
typedef atBOOL 		FN_OEM_SYSAPI_GPS_GetCoord( OEM_SYS_GPS hOemGPS, SYS_GPS_POSITION *lpGpsPos );
typedef atBOOL 		FN_OEM_SYSAPI_GPS_GetTime( OEM_SYS_GPS hOemGPS, SYS_DATETIME *datetime ); // 시간
typedef double 		FN_OEM_SYSAPI_GPS_GetSpeed ( OEM_SYS_GPS hOemGPS ); // 속도 (km/h)
typedef atLONG		FN_OEM_SYSAPI_GPS_GetAngle( OEM_SYS_GPS hOemGPS ); // 진행 각도 (degree : 0 - 359)
typedef atINT 		FN_OEM_SYSAPI_GPS_GetNumSV ( OEM_SYS_GPS hOemGPS ); // 보이는 위성 갯수
typedef atINT 		FN_OEM_SYSAPI_GPS_GetNumSU ( OEM_SYS_GPS hOemGPS ); // 사용중인 위성 갯수 
typedef atREAL 		FN_OEM_SYSAPI_GPS_GetPDOP ( OEM_SYS_GPS hOemGPS ); // Position Dilution Of Precision
typedef atREAL 		FN_OEM_SYSAPI_GPS_GetHDOP ( OEM_SYS_GPS hOemGPS ); // Horizontal Dilution Of Precision
typedef atREAL 		FN_OEM_SYSAPI_GPS_GetVDOP ( OEM_SYS_GPS hOemGPS ); // Vertical Dilution Of Precision
typedef atBOOL 		FN_OEM_SYSAPI_GPS_GetSatelliteInfo( OEM_SYS_GPS hOemGPS, atINT nIndex, SYS_GPS_SATELLITE *lpSatelliteEntry );
typedef atBOOL		FN_OEM_SYSAPI_GPS_ResetSatelliteAll( OEM_SYS_GPS hOemGPS );
typedef atBOOL 		FN_OEM_SYSAPI_GPS_EmulateGPSData( OEM_SYS_GPS hOemGPS, atINT nPktCat, SYS_GPS_INFO *lpGpsInfo );
typedef atLONG 		FN_OEM_SYSAPI_GPS_ExtGPSMethod( OEM_SYS_GPS hOemGPS, atINT func_no, atLONG param1, atLONG param2, atLONG param3, atLONG param4, atLONG param5, atLONG param6 );

// Support GPS Log File
typedef atBOOL		FN_OEM_SYSAPI_GPS_CreateLogFile(OEM_SYS_GPS hOemGPS, atTCHAR* filename);
typedef atVOID		FN_OEM_SYSAPI_GPS_SetLogEnable(OEM_SYS_GPS hOemGPS, atBOOL bEnable);
typedef atBOOL		FN_OEM_SYSAPI_GPS_IsLogEnable(OEM_SYS_GPS hOemGPS);
typedef atVOID		FN_OEM_SYSAPI_GPS_SetLogStatus(OEM_SYS_GPS hOemGPS, atBOOL bEnable);
typedef atBOOL 		FN_OEM_SYSAPI_GPS_Loging(OEM_SYS_GPS hOemGPS, SYS_GPS_INFO stGPSDat);
typedef atBOOL		FN_OEM_SYSAPI_GPS_CloseLoging(OEM_SYS_GPS hOemGPS);


typedef struct tagOEM_SYSAPI_GPS_T {
	FN_OEM_SYSAPI_InitGPS				*fnInitGPS;
	FN_OEM_SYSAPI_FinalGPS				*fnFinalGPS;

	FN_OEM_SYSAPI_GPS_CreateGPS			*fnGPS_CreateGPS;
	FN_OEM_SYSAPI_GPS_DestroyGPS		*fnGPS_DestroyGPS;
	FN_OEM_SYSAPI_GPS_SetOEMParam		*fnGPS_SetOEMParam;
	FN_OEM_SYSAPI_GPS_OpenGPS			*fnGPS_OpenGPS;
	FN_OEM_SYSAPI_GPS_IsConnected		*fnGPS_IsConnected;
	FN_OEM_SYSAPI_GPS_CloseGPS			*fnGPS_CloseGPS;
	FN_OEM_SYSAPI_GPS_GetState			*fnGPS_GetState;
	FN_OEM_SYSAPI_GPS_SetProcessFunc	*fnGPS_SetProcessFunc;
	FN_OEM_SYSAPI_GPS_GetCoord			*fnGPS_GetCoord;
	FN_OEM_SYSAPI_GPS_GetTime			*fnGPS_GetTime;
	FN_OEM_SYSAPI_GPS_GetSpeed			*fnGPS_GetSpeed;
	FN_OEM_SYSAPI_GPS_GetAngle			*fnGPS_GetAngle;
	FN_OEM_SYSAPI_GPS_GetNumSV			*fnGPS_GetNumSV;
	FN_OEM_SYSAPI_GPS_GetNumSU			*fnGPS_GetNumSU;
	FN_OEM_SYSAPI_GPS_GetPDOP			*fnGPS_GetPDOP;
	FN_OEM_SYSAPI_GPS_GetHDOP			*fnGPS_GetHDOP;
	FN_OEM_SYSAPI_GPS_GetVDOP			*fnGPS_GetVDOP;
	FN_OEM_SYSAPI_GPS_GetSatelliteInfo	*fnGPS_GetSatelliteInfo;
	FN_OEM_SYSAPI_GPS_ResetSatelliteAll	*fnGPS_ResetSatelliteAll;
	FN_OEM_SYSAPI_GPS_EmulateGPSData	*fnGPS_EmulateGPSData;
	FN_OEM_SYSAPI_GPS_ExtGPSMethod		*fnGPS_ExtGPSMethod;

	FN_OEM_SYSAPI_GPS_CreateLogFile		*fnGPS_CreateLogFile;
	FN_OEM_SYSAPI_GPS_SetLogEnable		*fnGPS_SetLogEnable;
	FN_OEM_SYSAPI_GPS_IsLogEnable		*fnGPS_IsLogEnable;
	FN_OEM_SYSAPI_GPS_SetLogStatus		*fnGPS_SetLogStatus;
	FN_OEM_SYSAPI_GPS_Loging			*fnGPS_Loging;
	FN_OEM_SYSAPI_GPS_CloseLoging		*fnGPS_CloseLoging;
} OEM_SYSAPI_GPS_T;


atVOID	OEM_SYSAPI_GPS_SetOemFn( OEM_SYSAPI_GPS_T *fnOem );
atVOID	OEM_SYSAPI_GPS_GetOemFn( OEM_SYSAPI_GPS_T *fnOem );


#ifdef __cplusplus
}
#endif


#endif 

