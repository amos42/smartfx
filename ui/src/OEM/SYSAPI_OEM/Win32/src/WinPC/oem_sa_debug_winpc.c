#include "SYSAPI.h"
#include "OEM_SA_Debug.h"
#include <windows.h>
#include <stdio.h>
#include <fcntl.h>
#include <io.h>

//#define _WIN32_WINNT 0x500
//#include <wincon.h>


static atBOOL g_bDebugActivate;


static atBOOL  OEM_SYSAPI_InitDebug( atLONG lSysInst, atLONG lInitData1, atLONG lInitData2 )
{
	g_bDebugActivate = atFALSE;
	
	return atTRUE;
}

static atVOID  OEM_SYSAPI_FinalDebug(void)
{
	if( g_bDebugActivate ){
		OEM_SYSAPI_ActivateDebug( atFALSE );
	}
}

static atVOID OEM_SYSAPI_Alert( const atLPTSTR szMsg )
{
	MessageBox( NULL, szMsg, _AT("Info"), MB_OK );
}

static atBOOL OEM_SYSAPI_ActivateDebug( atBOOL bActivate )
{
	int hCrt;
//	FILE *hf;
	int i;
	static FILE *g_console_file = NULL;

	if( bActivate ){	 
		AllocConsole();
		//hCrt = _open_osfhandle( (long) GetStdHandle(STD_OUTPUT_HANDLE), _O_TEXT );
		//g_console_file = _fdopen( hCrt, "w" );
		//*stdout = *g_console_file;
		//i = setvbuf( stdout, NULL, _IONBF, 0 );
		g_console_file = freopen("CONOUT$", "w", stdout); 

//		CONSOLE_SCREEN_BUFFER_INFO csbi;
//		GetConsoleScreenBufferInfo( g_console_file, &csbi )';
	} else {
//		_fcloseall();
		fclose( g_console_file );
		FreeConsole();	
		g_console_file = NULL;
	}

	g_bDebugActivate = bActivate;
	
	return atTRUE;
}

static atINT  OEM_SYSAPI_DebugPrint( const atLPTSTR str )
{
	char out_str[128];
	
	if( !g_bDebugActivate ) return 0;
		
	SYSAPI_ToANSI( out_str, str, 256-1 );
	
	return printf( out_str );
}



void init_sysapi_debug_oem() 
{
	OEM_SYSAPI_DEBUG_T oem_funcs = {
		OEM_SYSAPI_InitDebug,
		OEM_SYSAPI_FinalDebug,

		OEM_SYSAPI_Alert,

		OEM_SYSAPI_ActivateDebug,

		OEM_SYSAPI_DebugPrint
	};

	OEM_SYSAPI_DEBUG_SetOemFn( &oem_funcs );
}

