#include "SYSAPI.h"
#include "OEM_SA_Debug.h"
#include <stdio.h>
#include <string.h>


#ifdef __USES_SYSAPI_DEBUG_


static atBOOL g_bSysDebugIntitialized = atFALSE;

SYS_HFILE g_hLogFile  = atNULL;
atLONG  g_nPrvTick  = 0;
atLONG  g_nPrvMemory = 0;


static OEM_SYSAPI_DEBUG_T tOemFn;


atVOID OEM_SYSAPI_DEBUG_SetOemFn(OEM_SYSAPI_DEBUG_T *fnOem)
{
	tOemFn = *fnOem;
}


atVOID OEM_SYSAPI_DEBUG_GetOemFn(OEM_SYSAPI_DEBUG_T *fnOem)
{
	*fnOem = tOemFn;
}


atBOOL  SYSAPI_InitDebug( atLONG lSysInst, atLONG lInitData1, atLONG lInitData2 )
{
	if( g_bSysDebugIntitialized ) return atTRUE;
	
	if( tOemFn.fnInitDebug != atNULL ){
		g_bSysDebugIntitialized = tOemFn.fnInitDebug( lSysInst, lInitData1, lInitData2 );
	}
	
	return g_bSysDebugIntitialized;
}

atVOID  SYSAPI_FinalDebug(void)
{
	if( !g_bSysDebugIntitialized ) return;
	
//	SYSAPI_LogClose();

	if( tOemFn.fnFinalDebug != atNULL ){
		tOemFn.fnFinalDebug();	
	}
	
	g_bSysDebugIntitialized = atFALSE;
}

atVOID SYSAPI_Alert( const atLPTSTR szMsg )
{
	if( !g_bSysDebugIntitialized ) return;
	
	if( tOemFn.fnAlert != atNULL ){
		tOemFn.fnAlert(szMsg);
	}
}

atVOID SYSAPI_Alertf( const atLPTSTR fmt, ... )
{
	atTCHAR str[128];
    va_list ap;
    int len;

	if( !g_bSysDebugIntitialized ) return;
	
    va_start(ap, fmt);
    len = SYSAPI_vstprintf( str, fmt, ap );
    va_end(ap);
	
	if( tOemFn.fnAlert != atNULL ){
		tOemFn.fnAlert(str);
	}
}

atBOOL  SYSAPI_ActivateDebug( atBOOL bActivate )
{
	if( tOemFn.fnActivateDebug != atNULL ){
		return tOemFn.fnActivateDebug( bActivate );
	}

	return atFALSE;
}

atINT  SYSAPI_DebugPrint( const atLPTSTR str )
{
	if( !g_bSysDebugIntitialized ) return 0;
	
	if( tOemFn.fnDebugPrint != atNULL ){
		return tOemFn.fnDebugPrint( str );
	}

	return 0;
}

atINT  SYSAPI_DebugPrintf( const atLPTSTR fmt, ... )
{
	atTCHAR str[128];
    va_list ap;
    int len;

	if( !g_bSysDebugIntitialized ) return 0;
	
    va_start(ap, fmt);
    len = SYSAPI_vstprintf( str, fmt, ap );
    va_end(ap);

	return SYSAPI_DebugPrint( str );
}

atVOID  SYSAPI_DebugDump( const atLPBYTE ptr, int len )
{
    int i, j;
	unsigned char ch;
	atTCHAR str[128];
	int idx = 0;

	if( !g_bSysDebugIntitialized ) return;
	
	SYSAPI_DebugPrint( _AT("") );
	SYSAPI_DebugPrintf( _AT("* Addr:0x%x, Size=%d"), ptr, len );
	
	for( i = 0; i < len; i += 16 ){
		idx = 0;
		idx += SYSAPI_stprintf( &str[idx], _AT("%04X: "), i );
		
		for( j = 0; j < 16; j ++ ){
			if( i+j < len ){
				ch = ptr[i+j];
				idx += SYSAPI_stprintf( &str[idx], _AT("%02X "), (int)ch );
			} else {
				idx += SYSAPI_stprintf( &str[idx], _AT("   ") );           
			}        
		}      
		idx += SYSAPI_stprintf( &str[idx], _AT(" ") );      
		for( j = 0; j < 16; j ++ ){        
			if( i+j >= len ) break;        
			ch = ptr[i+j];        
			if( (ch < _ATC(' ')) || (ch > _ATC('~')) ) ch = _ATC('.');        
			str[idx++] = ch;
		}      

		str[idx] = _ATC('\0');
		SYSAPI_DebugPrint( str );     
	}
}


/*
atINT SYSAPI_LogInit(atUINT mode, const atLPTSTR fileName)
{
	if( !g_bSysDebugIntitialized ) return 0;
	
	g_hLogFile = SYSAPI_OpenFile(fileName, SYS_FILEOPEN_CREATE);
	
	g_nPrvTick   = SYSAPI_GetCurrTime();
	g_nPrvMemory = SYSAPI_GetTotalLeftHeapSize();
	
	SYSAPI_LogPrintf(0, "##############################################\n");
	SYSAPI_LogPrintf(0, "#             Beagle System Log               \n");
	SYSAPI_LogPrintf(0, "#   \n");
	SYSAPI_LogPrintf(0, "#  Free  Memory(Heap) bytes %d\n", g_nPrvMemory);
	SYSAPI_LogPrintf(0, "#  Total Memory(Ram)  bytes %d\n", SYSAPI_GetTotalHeapSize());
	SYSAPI_LogPrintf(0, "##############################################\n");

	return 0;
}


atINT SYSAPI_LogClose()
{
	if( !g_bSysDebugIntitialized ) return 0;
	
	if(g_hLogFile)
	{
		SYSAPI_LogPrintf(0, "##############################################\n");
		SYSAPI_LogPrintf(0, "#  Free  Memory(Heap) bytes %d\n", SYSAPI_GetTotalLeftHeapSize());
		SYSAPI_LogPrintf(0, "#  Total Memory(Ram)  bytes %d\n", SYSAPI_GetTotalHeapSize());
		SYSAPI_LogPrintf(0, "#-------------------  EOF --------------------\n");
		SYSAPI_LogPrintf(0, "##############################################\n");
		
		SYSAPI_CloseFile(g_hLogFile);
		g_hLogFile = atNULL;
	}
	return 0;
}

atINT SYSAPI_LogPrintf(atUINT mode, atLPCSTR fmt, ...)
{
	atTCHAR str[256];
	atTCHAR buff[256];
	//atINT   len;
	va_list args;
	atULONG  tick;

	if( !g_bSysDebugIntitialized ) return 0;	
	if(!g_hLogFile) return 0;

	va_start(args, fmt);
	SYSAPI_vsprintf(str, fmt, args);
	va_end(args);
	
	tick = SYSAPI_GetCurrTime();

	SYSAPI_sprintf(buff, _AT("%d, %s <+%d>\n"), SYSAPI_GetCurrTime(), str, tick - g_nPrvTick);
	
	g_nPrvTick = tick;

	//len = atUIAPI_vsprintf(str, fmt, ap);
	SYSAPI_WriteFile(g_hLogFile, buff, strlen(buff) +1);

	return 1;
}

atINT SYSAPI_LogMemoryStatus(const atCHAR* fmt, ...)
{
	atCHAR str[256];
	atCHAR buff[256];
	//atINT   len;
	va_list args;
	atULONG  tick;
	atLONG nFreeMem;
	atLONG nTotalRam;
	
	if( !g_bSysDebugIntitialized ) return 0;
	if(!g_hLogFile) return 0;

	va_start(args, fmt);
	vsprintf(str, fmt, args);
	va_end(args);
	
	tick = SYSAPI_GetCurrTime();
	nFreeMem  = SYSAPI_GetTotalLeftHeapSize();
	nTotalRam = SYSAPI_GetTotalHeapSize();

	sprintf(buff, "[MEM] %d, %s <+%d>, Heap %d/%dbytes, Mem Inc %dbytes\n", 
				SYSAPI_GetCurrTime(), str, tick - g_nPrvTick, nFreeMem, nTotalRam, nFreeMem - g_nPrvMemory);
	
	g_nPrvTick   = tick;
	g_nPrvMemory = nFreeMem; 

	//len = atUIAPI_vsprintf(str, fmt, ap);
	SYSAPI_WriteFile(g_hLogFile, buff, strlen(buff) +1);

	return 1;
}


atINT	SYSAPI_LogPrintfW(atUINT mode, const atLPTSTR fmt, ...)
{
	atTCHAR str[256];
	atTCHAR buff[256];

//	atINT   len;
	va_list args;

	if( !g_bSysDebugIntitialized ) return 0;
	if(!g_hLogFile) return 0;

	va_start(args, fmt);
	SYSAPI_vstprintf(str, fmt, args);
	va_end(args);

	SYSAPI_stprintf(buff, _AT("[%ud], %d, %s \n"), mode, SYSAPI_GetCurrTime(), str);

	SYSAPI_WriteFile(g_hLogFile, buff, (SYSAPI_tcslen(buff)+1) * sizeof(atTCHAR) );

	return 1;
}


atINT	SYSAPI_LogMemoryStatusW(const atLPTSTR fmt, ...)
{
	atTCHAR str[256];
	atTCHAR buff[256];

//	atINT   len;
	va_list args;

	if( !g_bSysDebugIntitialized ) return 0;
	if(!g_hLogFile) return 0;

	va_start(args, fmt);
	SYSAPI_vstprintf(str, fmt, args);
	va_end(args);

	SYSAPI_stprintf(buff, _AT("%d, %s \n"), SYSAPI_GetCurrTime(), str);

	SYSAPI_WriteFile(g_hLogFile, buff, (SYSAPI_tcslen(buff)+1) * sizeof(atTCHAR) );

	return 1;
}
*/


#endif

