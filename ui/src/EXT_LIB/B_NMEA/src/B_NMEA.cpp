#include "GPS_Info.h"
#include "B_NMEA.h"
#include "_heap.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>


#ifndef MAX_FIELD_BUFFER
#define MAX_FIELD_BUFFER		 25
#endif
#ifndef MAX_PARSE_BUFFER		
#define MAX_PARSE_BUFFER		256
#endif
#ifndef GPS_SENTENCE_ID_LEN
#define	GPS_SENTENCE_ID_LEN		6
#endif

#ifndef MAX_READ_BUFFER
#define MAX_READ_BUFFER 1024
#endif

#ifndef MAX_PACKET_SIZE
#define MAX_PACKET_SIZE 80
#endif


// ascii definitions
#define ASCII_DOLLOR	0x24		// '$'
#define ASCII_LF        0x0A		// '\n'
#define ASCII_CR        0x0D		// '\r'
#define	ASCII_ASTERISK	0x2A		// '*'
#define	ASCII_COMMA		0x2C		// ','


#ifndef COUNTOF
#define COUNTOF(OBJ)       (  sizeof(OBJ)/sizeof(OBJ[0]) )
#endif


typedef struct _tagTNMEAParser {
	TGPSInfo *lpGPSInfo;
	NMEA_CallbackFunc *lpCallbackFunc;
	void *lpParam;

	int state;
	char buf[MAX_READ_BUFFER+4];
	int buf_cnt;
	int cur_idx;
} TNMEAParser;


NMEA_PARSER_T	NMEA_CreateNMEAParser( TGPSInfo *lpGPSInfo, NMEA_CallbackFunc *lpFunc, void *lpParam )
{
    TNMEAParser *nmea = (TNMEAParser *)NMEA_alloc( sizeof(TNMEAParser) );
    if( nmea == NULL ) return NULL;
    	
	nmea->lpGPSInfo = lpGPSInfo;
	nmea->lpCallbackFunc = lpFunc;
	nmea->lpParam = lpParam;

	nmea->state = 0;
	nmea->buf_cnt = 0;
	nmea->cur_idx = 0;
	
	return (NMEA_PARSER_T)nmea;
}


void	NMEA_DestroyNMEAParser( NMEA_PARSER_T hNMEAParser )
{
	TNMEAParser *nmea = (TNMEAParser *)hNMEAParser;
	if( nmea == NULL ) return;
		
	NMEA_free( (void *)nmea );
}


int	NMEA_ParseNMEAStream( NMEA_PARSER_T hNMEAParser, const char* data, long len )
{
	TNMEAParser *nmea = (TNMEAParser *)hNMEAParser;
	if( nmea == NULL ) return 0;

	if( len <= 0 ) return 0;

	char *tp = &nmea->buf[nmea->buf_cnt];
	char *p = (char *)data;
	char ch;
	
	while(len--){
		ch = *p++;
		
		switch( nmea->state ){
			case 0 : 
				  if( ch == ASCII_DOLLOR ){
					  *tp++ = ch;
					  nmea->buf_cnt++;
						nmea->state ++;
					}
					break;
					
			case 1 : 
				  *tp++ = ch;
				  nmea->buf_cnt++;
				  if( nmea->buf_cnt >= MAX_READ_BUFFER ){
					  nmea->buf_cnt = 0;
					  nmea->state = 0;
					  tp = nmea->buf;
					  break;
				  }

				  if( ch == ASCII_CR ){
						nmea->state ++;
					}
					break;
					
			case 2 : 
				  if( ch == ASCII_LF ){						  
						DWORD_T dwRetVal = NMEA_ParseNMEAPacket( nmea->lpGPSInfo, nmea->buf, nmea->buf_cnt );
						if( nmea->lpCallbackFunc ) nmea->lpCallbackFunc( dwRetVal, nmea->lpGPSInfo, nmea->lpParam );
					} else {
						p--;
						len++;
					}
					
				  nmea->buf_cnt = 0;
			    tp = nmea->buf;
					nmea->state = 0;								
					break;
			default:
					break;
		}
	}
		
	return 0;	
}

TGPSInfo *	  NMEA_GetGPSInfo( NMEA_PARSER_T hNMEAParser )
{
	TNMEAParser *nmea = (TNMEAParser *)hNMEAParser;
	if( nmea == NULL ) return NULL;
		
	return nmea->lpGPSInfo;	
}

void	NMEA_ResetGPSInfo( TGPSInfo *lpGPSInfo )
{
	memset( lpGPSInfo, 0, sizeof(TGPSInfo) );
	
	lpGPSInfo->rHDOP = 100.;
	lpGPSInfo->rVDOP = 100.;
	lpGPSInfo->rPDOP = 100.;	
}

