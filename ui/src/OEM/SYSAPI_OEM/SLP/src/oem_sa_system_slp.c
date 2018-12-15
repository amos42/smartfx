//
// sound_wipic.c
//
////////////////////////////////////////////////////////////////

#include "OEM_SA_System.h"
#include <string.h>
#include <time.h>
#include <gtk/gtk.h>


void * g_SYS_hInstance;
void * g_SYS_hWindow;


atLONG OEM_SYSAPI_Initialize( SYSAPI_INIT_DATA *lpInitData )
{
	g_SYS_hInstance = (void *)lpInitData->lSysInst;
	g_SYS_hWindow = (void *)lpInitData->lSysData1;
	if( g_SYS_hWindow == atNULL ) return -1;


	return 0;
}


void OEM_SYSAPI_Finalize(void)
{
	g_SYS_hInstance = atNULL;
	g_SYS_hWindow = atNULL;
}

void OEM_SYSAPI_GetSystemDateTime(SYS_DATETIME *date)
{
	time_t t;
	struct tm *tmr;

	t = time(atNULL);
	tmr = localtime(&t);

	date->nYear		= tmr->tm_year + 1900;
	date->nMonth	= tmr->tm_mon + 1;
	date->nDay		= tmr->tm_mday;
	date->nHour		= tmr->tm_hour;
	date->nMinute	= tmr->tm_min;
	date->nSec		= tmr->tm_sec;
}

atLONG OEM_SYSAPI_GetAntennaInfo(void)
{
	return 0;
}

atLONG OEM_SYSAPI_GetBatteryInfo(void)
{
	return 50;
}

gboolean _SystemOff(gpointer data)
{
	gdk_threads_enter();
	gtk_main_quit();
	gdk_flush();
	gdk_threads_leave();
	return 0;
}

atINT OEM_SYSAPI_SystemOff(atINT ExitCode)
{
	g_idle_add((GSourceFunc)_SystemOff, NULL);
	//gdk_threads_enter();
	//gtk_main_quit();
	//gdk_threads_leave();
	return 1;//ExitCode;
}


atINT OEM_SYSAPI_SysCommand(int cmdCode, atLONG param1, atLONG param2)
{
	return 0;
}


