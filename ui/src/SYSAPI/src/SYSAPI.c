#include "SYSAPI.h"
#include "OEM_SA_System.h"


atBOOL SYSAPI_Initialize( SYSAPI_INIT_DATA *lpInitData, atDWORD dwInitMask )
{
	lpInitData->lOemInst = SYSAPI_InitSystem( lpInitData->lSysInst, lpInitData->lSysData1, lpInitData->lSysData2 );

#ifdef __USES_SYSAPI_SYSUTIL_
//	if( dwInitMask & SYS_INITMASK_SYSUTIL )
//		SYSAPI_InitSysUtil( lpInitData->lSysInst, lpInitData->lSysUtilData1, lpInitData->lSysUtilData2 );
#endif
#ifdef __USES_SYSAPI_MEMORY_
	if( dwInitMask & SYS_INITMASK_MEMORY )
		SYSAPI_InitMemory( lpInitData->lSysInst, lpInitData->lMemData1, lpInitData->lMemData2 );
#endif
#ifdef __USES_SYSAPI_FILE_
	if( dwInitMask & SYS_INITMASK_FILE )
		SYSAPI_InitFile( lpInitData->lSysInst, lpInitData->lFileData1, lpInitData->lFileData2 );
#endif
#ifdef __USES_SYSAPI_DEBUG_
	if( dwInitMask & SYS_INITMASK_DEBUG )
		SYSAPI_InitDebug( lpInitData->lSysInst, lpInitData->lDebugData1, lpInitData->lDebugData2 );
#endif
#ifdef __USES_SYSAPI_TASK_
	if( dwInitMask & SYS_INITMASK_TASK )
		SYSAPI_InitTask( lpInitData->lSysInst, lpInitData->lTaskData1, lpInitData->lTaskData2 );
#endif
#ifdef __USES_SYSAPI_TIMER_
	if( dwInitMask & SYS_INITMASK_TIMER )
		SYSAPI_InitTimer( lpInitData->lSysInst, lpInitData->lTimerData1, lpInitData->lTimerData2 );
#endif
#ifdef __USES_SYSAPI_DISPLAY_
	if( dwInitMask & SYS_INITMASK_DISPLAY )
		SYSAPI_InitDisplay( lpInitData->lSysInst, lpInitData->lScrData1, lpInitData->lScrData2 );
#endif
#ifdef __USES_SYSAPI_KEYPAD_
	if( dwInitMask & SYS_INITMASK_KEYPAD )
		SYSAPI_InitKeypad( lpInitData->lSysInst, lpInitData->lKeyData1, lpInitData->lKeyData2 );
#endif
#ifdef __USES_SYSAPI_TOUCH_
	if( dwInitMask & SYS_INITMASK_TOUCH )
		SYSAPI_InitTouchDev( lpInitData->lSysInst, lpInitData->lTouchData1, lpInitData->lTouchData2 );
#endif
#ifdef __USES_SYSAPI_SOUND_
	if( dwInitMask & SYS_INITMASK_SOUND )
		SYSAPI_InitSound( lpInitData->lSysInst, lpInitData->lSoundData1, lpInitData->lSoundData2 );
#endif
#ifdef __USES_SYSAPI_GPS_
	if( dwInitMask & SYS_INITMASK_GPS )
		SYSAPI_InitGPS( lpInitData->lSysInst, lpInitData->lGPSData1, lpInitData->lGPSData2 );
#endif

	SYSAPI_DebugPrint(_AT("SYSAPI Initialized.\r\n"));

	return atTRUE;
}


void SYSAPI_Finalize(void)
{
#ifdef __USES_SYSAPI_GPS_
    SYSAPI_FinalGPS();
#endif
#ifdef __USES_SYSAPI_RECORD_
    SYSAPI_FinalRecord();
#endif
#ifdef __USES_SYSAPI_SOUND_
    SYSAPI_FinalSound();
#endif
#ifdef __USES_SYSAPI_RECORD_
    SYSAPI_FinalRecord();
#endif
#ifdef __USES_SYSAPI_DEBUG_
    SYSAPI_FinalDebug();
#endif
#ifdef __USES_SYSAPI_TASK_
    SYSAPI_FinalTask();
#endif
#ifdef __USES_SYSAPI_KEYPAD_
	SYSAPI_FinalKeypad();
#endif
#ifdef __USES_SYSAPI_TOUCH_
	SYSAPI_FinalTouchDev();
#endif
#ifdef __USES_SYSAPI_DISPLAY_
    SYSAPI_FinalDisplay();
#endif
#ifdef __USES_SYSAPI_TIMER_
	SYSAPI_FinalTimer();
#endif
#ifdef __USES_SYSAPI_FILE_
	SYSAPI_FinalFile();
#endif
#ifdef __USES_SYSAPI_MEMORY_
	SYSAPI_FinalMemory();
#endif
#ifdef __USES_SYSAPI_SYSUTIL_
//    SYSAPI_FinalSysUtil();
#endif

	SYSAPI_FinalSystem();
}
