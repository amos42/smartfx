#ifndef __B_NMEA_H_
#define __B_NMEA_H_


#include "GPS_Info.h"


#ifdef __cplusplus
extern "C" {
#endif

typedef void *  NMEA_PARSER_T;

enum {
	NMEA_PKT_NONE = 0,
	NMEA_PKT_GPRMC,
	NMEA_PKT_GPGGA,
	NMEA_PKT_GPGSV,
	NMEA_PKT_GPGSA,
	NMEA_PKT_PLTTS,
	NMEA_PKT_PLTTF,
	NMEA_PKT_PLTTQ,
	NMEA_PKT_PLTCH,
	NMEA_PKT_PLTMT,
	NMEA_PKT_PLTRD,
	NMEA_PKT_PLTTM,
	NMEA_PKT_PLTTR,
	NMEA_PKT_PLTFD,
	NMEA_PKT_PLTCF,
	NMEA_PKT_BGPSPOS,
};

typedef int (NMEA_CallbackFunc)( int pkt_type, TGPSInfo *lpGPSInfo, void *lpParam );


void				NMEA_ResetGPSInfo( TGPSInfo *lpGPSInfo );
int				  NMEA_ParseNMEAPacket( TGPSInfo *lpGPSInfo, const char* data, long len );

NMEA_PARSER_T	NMEA_CreateNMEAParser( TGPSInfo *lpGPSInfo, NMEA_CallbackFunc *lpFunc, void *lpParam );
void				  NMEA_DestroyNMEAParser( NMEA_PARSER_T hNMEAParser );
int					  NMEA_ParseNMEAStream( NMEA_PARSER_T hNMEAParser, const char* data, long len );
TGPSInfo *	  NMEA_GetGPSInfo( NMEA_PARSER_T hNMEAParser );

int		  NMEA_BuildNMEAPacket( TGPSInfo *lpGPSInfo, char* data, long len );

	
#ifdef __cplusplus
}
#endif


#endif
