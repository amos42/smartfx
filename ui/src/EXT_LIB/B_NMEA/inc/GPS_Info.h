#ifndef __GPS_INFO_H_
#define __GPS_INFO_H_


#ifdef __cplusplus
extern "C" {
#endif


#ifndef __SIMPLE_BASE_TYPE_
#define __SIMPLE_BASE_TYPE_
typedef char            CHAR_T;
typedef int             INT_T;
typedef short           SHORT_T;
typedef long            LONG_T;
typedef unsigned char   BYTE_T;
typedef unsigned short  WORD_T;
typedef unsigned long   DWORD_T;
typedef int             BOOL_T;
typedef double          REAL_T;
typedef unsigned int    UINT_T;
#endif

#ifndef TRUE
#define TRUE (1)
#endif
#ifndef FALSE
#define FALSE (0)
#endif
    

#ifndef MAX_SATELLITE_NUM
#define MAX_SATELLITE_NUM   (32)
#endif

// SYS_DATETIME�� ��ġ��Ų��.
typedef struct _TGPSTime {
	SHORT_T 	nYear;
	CHAR_T		nMonth;
	CHAR_T		nDay;
	CHAR_T		nHour;
	CHAR_T		nMinute;
	CHAR_T		nSec;
	CHAR_T		tDummy;
} TGPSTime;

typedef struct _TGPSSatellite {// GPS�� ����
	DWORD_T	dwPRN ;		//	Satellite PRN number ( 1-32 )
	DWORD_T	dwElev ;	//	Elevation, degrees	( 00-90 )
	DWORD_T	dwAzi ;		//	Azimuth, degrees	( 000-359 )
	DWORD_T	dwSNR ;		//	SNR - higher is better ( 00-99dB, null when not tracking ) 
	BOOL_T	fUse ;		// ����ϰ� �ִ��� ��� ..
} TGPSSatellite;


typedef struct _TGPSInfo {
	WORD_T	wGPSFix;	// GPS quality indicator (0=invalid; 1=GPS fix; 2=Diff. GPS fix)
	WORD_T	wFixMode1;   // GSA::Mode1 	(M=Manual, forced to operate in 2D or 3D; A=Automatic, 3D/2D)
	WORD_T	wFixMode2;   // GSA::Mode2  (1=Fix not available; 2=2D; 3=3D)
	WORD_T	wDataState;	// GPS Quality (A=Validate; V=navigation receiver warning)
	
	REAL_T	rLongitude;	// �浵		longitude	// + �� ����(E), - �� ����(W) ..
	REAL_T	rLatitude;	// ����		latitude	// + �� ����(N), - �� ����(S) ..
	REAL_T	rAltitude;	// ��	(m)	altitude 

	DWORD_T	dwSpeed;	// �ӵ� (knot)			 ( ������ * 100 )
	LONG_T	dwAngle;	// Trace Angle (degree) 

	WORD_T	wSV;		// Number of Satellites in View 
	WORD_T	wSU;		// Number of Satellites in Using

	TGPSSatellite	satellite[MAX_SATELLITE_NUM] ;		// ��������

	REAL_T	rHDOP;	// Horizontal Dilution Of Precision
	REAL_T	rVDOP;	// Vertical   Dilution Of Precision
	REAL_T	rPDOP;	// Position   Dilution Of Precision

	TGPSTime gps_time;

//	CHAR_T	time_stamp[12];	
} TGPSInfo;


#ifdef __cplusplus
}
#endif


#endif
