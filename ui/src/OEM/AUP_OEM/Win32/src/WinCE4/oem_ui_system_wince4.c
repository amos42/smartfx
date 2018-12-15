//
// sound_wipic.c
//
////////////////////////////////////////////////////////////////

#include "UI_MNG.h"
#include "OEM_UI_system.h"
#include <string.h>
#include <windows.h>
#include <time.h>
#include "SYSAPI.h"


HINSTANCE g_UI_hInstance;
HWND g_UI_hWindow;


static atLONG OEM_UI_Initialize( atUIAPI_INIT_DATA *lpInitData )
{
	SYSAPI_INIT_DATA sys_init;
	
	g_UI_hInstance = (HINSTANCE)lpInitData->lSysInst;
	g_UI_hWindow = (HWND)lpInitData->lSysData1;
	if( g_UI_hWindow == atNULL ) return -1;

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
					SYS_INITMASK_SYSUTIL | SYS_INITMASK_MEM | SYS_INITMASK_STR | 
					SYS_INITMASK_FILE | SYS_INITMASK_DEBUG | SYS_INITMASK_TASK |
					/*SYS_INITMASK_DISPLAY | */ SYS_INITMASK_TIMER | SYS_INITMASK_INPUT |
					SYS_INITMASK_SOUND | SYS_INITMASK_GPS );

	return 0;
}


static void OEM_UI_Finalize(void)
{
	SYSAPI_Finalize();

	g_UI_hInstance = atNULL;
	g_UI_hWindow = atNULL;
}


void init_uiapi_system_oem() 
{
	OEM_UI_SYSTEM_T oem_funcs = {
		OEM_UI_Initialize,
		OEM_UI_Finalize
	};

	OEM_UI_System_SetOemFn( &oem_funcs );
}
