//
// oem_ui_system.c
//
////////////////////////////////////////////////////////////////

#include "UI_API/UI_API.h"
#include "OEM_UI_System.h"
#include "SYSAPI.h"
#include <string.h>


atLONG OEM_UI_Initialize( atUIAPI_INIT_DATA *lpInitData )
{
	SYSAPI_INIT_DATA sys_init;
	
	SYSAPI_memset(&sys_init, 0, sizeof(sys_init));
	
	sys_init.lSysInst   = lpInitData->lSysInst;
	sys_init.lSysData1  = lpInitData->lSysData1;
	sys_init.lSysData2  = lpInitData->lSysData2;
	sys_init.lOemInst   = lpInitData->lOemInst;
	sys_init.lCmdInst   = lpInitData->lCmdInst;

	sys_init.lMemData1   = lpInitData->lMemData1;
	sys_init.lMemData2   = lpInitData->lMemData2;
	sys_init.lFileData1  = lpInitData->lFileData1;
	sys_init.lFileData2  = lpInitData->lFileData2;
	sys_init.lDebugData1 = lpInitData->lDebugData1;
	sys_init.lDebugData2 = lpInitData->lDebugData2;
	sys_init.lTimerData1 = lpInitData->lTimerData1;
	sys_init.lTimerData2 = lpInitData->lTimerData2;
	sys_init.lTaskData1  = lpInitData->lProcData1;
	sys_init.lTaskData2  = lpInitData->lProcData2;
	sys_init.lKeyData1 = lpInitData->lKeyData1;
	sys_init.lKeyData2 = lpInitData->lKeyData2;
	sys_init.lTouchData1 = lpInitData->lTouchData1;
	sys_init.lTouchData2 = lpInitData->lTouchData2;

	SYSAPI_Initialize(&sys_init,
					SYS_INITMASK_SYSUTIL | SYS_INITMASK_MEMORY | SYS_INITMASK_STR | 
					SYS_INITMASK_FILE | SYS_INITMASK_DEBUG | SYS_INITMASK_TASK |
					/*SYS_INITMASK_DISPLAY | */ SYS_INITMASK_TIMER | SYS_INITMASK_KEYPAD | SYS_INITMASK_TOUCH |
					SYS_INITMASK_SOUND | SYS_INITMASK_GPS );

	return 0;
}


void OEM_UI_Finalize(void)
{
	SYSAPI_Finalize();
}

void OEM_UI_GetSystemDateTime(atDATETIME *date)
{
	SYS_DATETIME sdate;
	SYSAPI_GetSystemDateTime(&sdate);

	date->nYear		= sdate.nYear;
	date->nMonth	= sdate.nMonth;
	date->nDay		= sdate.nDay;
	date->nHour		= sdate.nHour;
	date->nMinute	= sdate.nMinute;
	date->nSec		= sdate.nSec;
}

atLONG OEM_UI_GetAntennaInfo(void)
{
	return SYSAPI_GetAntennaInfo();
}

atLONG OEM_UI_GetBatteryInfo(void)
{
	return SYSAPI_GetBatteryInfo();
}

atINT OEM_UI_SystemOff(atINT ExitCode)
{
	return SYSAPI_SystemOff(ExitCode);
}

