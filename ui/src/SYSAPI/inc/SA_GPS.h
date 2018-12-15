#ifndef __SA_GPS_H_
#define __SA_GPS_H_

#include "SA_Types.h"
#include "AT_Real.h"


#ifdef __cplusplus
extern "C" {
#endif


typedef atHANDLE    SYS_HGPS;


#ifndef MAX_SATELLITE_NUM
#define MAX_SATELLITE_NUM (32)
#endif

typedef enum {
	SYS_GPS_STATE_NONE = 0,
	SYS_GPS_STATE_INVALIDE = SYS_GPS_STATE_NONE,
	SYS_GPS_STATE_NOTFIXED,
	SYS_GPS_STATE_2D,
	SYS_GPS_STATE_3D
} SYS_GPS_STATE;

typedef enum {
	SYS_GPS_EVENT_NONE,
	SYS_GPS_EVENT_PREPROCESS,
	SYS_GPS_EVENT_POSTPROCESS
} SYS_GPS_EVENT;

typedef enum {
	SYS_GPS_PKT_NONE = 0,
	SYS_GPS_PKT_GPRMC,
	SYS_GPS_PKT_GPGGA,
	SYS_GPS_PKT_GPGSV,
	SYS_GPS_PKT_GPGSA,
	SYS_GPS_PKT_PLTTS,
	SYS_GPS_PKT_PLTTF,
	SYS_GPS_PKT_PLTTQ,
	SYS_GPS_PKT_PLTCH,
	SYS_GPS_PKT_PLTMT,
	SYS_GPS_PKT_PLTRD,
	SYS_GPS_PKT_PLTTM,
	SYS_GPS_PKT_PLTTR,
	SYS_GPS_PKT_PLTFD,
	SYS_GPS_PKT_PLTCF,
	SYS_GPS_PKT_BGPS
} SYS_GPS_PKT;

typedef struct _SYS_GPS_SATELLITE {
	atINT	nPRN;			//	Satellite PRN number ( 1-32 )
	atINT	nElevation;	//	Elevation, degrees	( 00-90 )
	atINT	nAzimuth;		//	Azimuth, degrees	( 000-359 )
	atINT	nSNR;			// 	SNR - higher is better ( 00-99dB, null when not tracking ) 
	atBOOL	bUsed;			// 	사용하고 있는지 어떤지 ..
} SYS_GPS_SATELLITE;

typedef struct _SYS_GPS_POSITION {
	atREAL	rLongitude;
	atREAL	rLatitude;
	atREAL	rAltitude;
} SYS_GPS_POSITION;

//typedef struct _SYS_GPS_WRITE_LOG
//{
////#if GPS_LOG	// log writing
//	SYS_HFILE 	hWriteGpsLog;		// 저장할 파일 핸들
//	atBOOL		bGPSLogEnabled;     // GPS로깅 사용 여부 (default FALSE)
//	atBYTE* 	m_pLogBuf;
//	atINT 		m_nLogBuf;
////#endif
//} SYS_GPS_WRITE_LOG;

typedef struct _SYS_GPS_INFO {
	SYS_GPS_POSITION  pos;
	SYS_DATETIME gps_datetime;
	
	atANSI_CHAR	time_stamp[12] ;
	
	atDWORD nSpeed ;		// 속도 (knot)			 ( 원래값 * 100 )
	atLONG	nAngle ;		// Trace Angle (degree) 

//	atWORD	wGPSFix ;		// GPS Quality
//	atWORD	wDataStat ;	// Data Status
//	atWORD	wFixMode;   	// GSA::Mode2
//	atWORD	RESERVED;
	atWORD	nFixState;   	// GPS Quality
	atWORD	nQuality;   	// GPS Quality
	atWORD	n3DState;   	// GPS Quality
	
	atWORD	nSV ;			// Number of Satellites in View 
	atWORD	nSU ;			// Number of Satellites in Using

	SYS_GPS_SATELLITE		satellite[MAX_SATELLITE_NUM] ;		// 위성정보

	atREAL	rHDOP;			// Horizontal Dilution Of Precision
	atREAL	rVDOP;			// Vertical   Dilution Of Precision
	atREAL	rPDOP;			// Position   Dilution Of Precision
} SYS_GPS_INFO;

typedef int (SYS_GPS_PROC_FUNC)( void *gps_inst, int event, int gps_state, atREAL rLon, atREAL rLat, atREAL rAlt, long speed_kmh, int nAngle, SYS_DATETIME *datetime );


atBOOL SYSAPI_InitGPS( atLONG lSysInst, atLONG lInitData1, atLONG lInitData2 );
atVOID SYSAPI_FinalGPS( NOARGS );

SYS_HGPS 	SYSAPI_GPS_CreateGPS( void );
void 		SYSAPI_GPS_DestroyGPS( SYS_HGPS hGPS );
atBOOL 		SYSAPI_GPS_SetOEMParam( SYS_HGPS hGPS, int nType, long lParam1, long lParam2 );
atBOOL 		SYSAPI_GPS_OpenGPS( SYS_HGPS hGPS );
atBOOL 		SYSAPI_GPS_IsConnected( SYS_HGPS hGPS );
atBOOL 		SYSAPI_GPS_CloseGPS( SYS_HGPS hGPS );
atINT		SYSAPI_GPS_GetState( SYS_HGPS hGPS );
atBOOL 		SYSAPI_GPS_SetProcessFunc( SYS_HGPS hGPS, SYS_GPS_PROC_FUNC *fnProc );
atBOOL 		SYSAPI_GPS_GetCoord( SYS_HGPS hGPS, SYS_GPS_POSITION *lpGpsPos );
atBOOL 		SYSAPI_GPS_GetTime( SYS_HGPS hGPS, SYS_DATETIME *datetime ); // 시간
double 		SYSAPI_GPS_GetSpeed( SYS_HGPS hGPS ); // 속도 (km/h)
atLONG		SYSAPI_GPS_GetAngle( SYS_HGPS hGPS ); // 진행 각도 (degree : 0 - 359)
atINT 		SYSAPI_GPS_GetNumSV( SYS_HGPS hGPS ); // 보이는 위성 갯수
atINT 		SYSAPI_GPS_GetNumSU( SYS_HGPS hGPS ); // 사용중인 위성 갯수 
atREAL 		SYSAPI_GPS_GetPDOP( SYS_HGPS hGPS ); // Position Dilution Of Precision
atREAL 		SYSAPI_GPS_GetHDOP( SYS_HGPS hGPS ); // Horizontal Dilution Of Precision
atREAL 		SYSAPI_GPS_GetVDOP( SYS_HGPS hGPS ); // Vertical Dilution Of Precision
atBOOL 		SYSAPI_GPS_GetSatelliteInfo( SYS_HGPS hGPS, atINT nIndex, SYS_GPS_SATELLITE *lpSatelliteEntry );
atBOOL		SYSAPI_GPS_ResetSatelliteAll( SYS_HGPS hGPS );
atBOOL		SYSAPI_GPS_EmulateGPSData( SYS_HGPS hGPS, atINT nPktCat, SYS_GPS_INFO *lpGpsInfo );
atLONG		SYSAPI_GPS_ExtGPSMethod( SYS_HGPS hGPS, atINT func_no, atLONG param1, atLONG param2, atLONG param3, atLONG param4, atLONG param5, atLONG param6 );

// Support GPS Log File
atBOOL		SYSAPI_GPS_CreateLogFile(SYS_HGPS hGPS, atTCHAR* filename);
atVOID		SYSAPI_GPS_SetLogEnable(SYS_HGPS hGPS, atBOOL bEnable);
atBOOL		SYSAPI_GPS_IsLogEnable(SYS_HGPS hGPS);
atVOID		SYSAPI_GPS_SetLogStatus(SYS_HGPS hGPS, atBOOL bEnable);

#ifdef __cplusplus
}
#endif


#endif

