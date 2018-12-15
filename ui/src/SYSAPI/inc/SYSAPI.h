#ifndef __SYSAPI_H_
#define __SYSAPI_H_


#include "SA_Types.h"
#include "SA_Cfg.h"


#define SYS_INITMASK_NONE (0x0)
#define SYS_INITMASK_ALL (0xFFFF)


#include "SA_System.h"

#ifdef __USES_SYSAPI_SYSUTIL_
#include "SA_SysUtil.h"
#define SYS_INITMASK_SYSUTIL (0x1 << 0)
#else
#define SYS_INITMASK_SYSUTIL (0x0)
#endif

#ifdef __USES_SYSAPI_MEMORY_
#include "SA_Mem.h"
#define SYS_INITMASK_MEMORY (0x1 << 1)
#else
#define SYS_INITMASK_MEMORY (0x0)
#endif

#ifdef __USES_SYSAPI_STR_
#include "SA_Str.h"
#define SYS_INITMASK_STR (0x1 << 2)
#else
#define SYS_INITMASK_STR (0x0)
#endif

#ifdef __USES_SYSAPI_FILE_
#include "SA_File.h"
#define SYS_INITMASK_FILE (0x1 << 3)
#else
#define SYS_INITMASK_FILE (0x0)
#endif

#ifdef __USES_SYSAPI_DEBUG_
#include "SA_Debug.h"
#define SYS_INITMASK_DEBUG (0x1 << 4)
#else
#define SYS_INITMASK_DEBUG (0x0)
#endif

#ifdef __USES_SYSAPI_TASK_
#include "SA_Task.h"
#define SYS_INITMASK_TASK (0x1 << 5)
#else
#define SYS_INITMASK_TASK (0x0)
#endif

#ifdef __USES_SYSAPI_DISPLAY_
#include "SA_Scr.h"
#define SYS_INITMASK_DISPLAY (0x1 << 6)
#else
#define SYS_INITMASK_DISPLAY (0x0)
#endif

#ifdef __USES_SYSAPI_TIMER_
#include "SA_Timer.h"
#define SYS_INITMASK_TIMER (0x1 << 7)
#else
#define SYS_INITMASK_TIMER (0x0)
#endif

#ifdef __USES_SYSAPI_KEYPAD_
#include "SA_Key.h"
#define SYS_INITMASK_KEYPAD (0x1 << 8)
#else
#define SYS_INITMASK_KEYPAD (0x0)
#endif

#ifdef __USES_SYSAPI_TOUCH_
#include "SA_Touch.h"
#define SYS_INITMASK_TOUCH (0x1 << 9)
#else
#define SYS_INITMASK_TOUCH (0x0)
#endif

#ifdef __USES_SYSAPI_SOUND_
#include "SA_Sound.h"
#define SYS_INITMASK_SOUND (0x1 << 10)
#else
#define SYS_INITMASK_SOUND (0x0)
#endif

#ifdef __USES_SYSAPI_GPS_
#include "SA_GPS.h"
#define SYS_INITMASK_GPS   (0x1 << 11)
#else
#define SYS_INITMASK_GPS (0x0)
#endif


#ifdef __cplusplus
extern "C" {
#endif

typedef struct tagSYSAPI_INIT_DATA {
	atLONG lSysInst;
	atLONG lSysData1;
	atLONG lSysData2;
	
	atLONG lOemInst;
	atLONG lCmdInst;

//#ifdef __USES_SYSAPI_SYSUTIL_
	atLONG lSysUtilData1;
	atLONG lSysUtilData2;
//#endif

//#ifdef __USES_SYSAPI_MEMORY_
	atLONG lMemData1;
	atLONG lMemData2;
//#endif

//#ifdef __USES_SYSAPI_FILE_
	atLONG lFileData1;
	atLONG lFileData2;
//#endif

//#ifdef __USES_SYSAPI_DEBUG_
	atLONG lDebugData1;
	atLONG lDebugData2;
//#endif

//#ifdef __USES_SYSAPI_TASK_
	atLONG lTaskData1;
	atLONG lTaskData2;
//#endif

//#ifdef __USES_SYSAPI_DISPLAY_
	atLONG lScrData1;
	atLONG lScrData2;
//#endif

//#ifdef __USES_SYSAPI_TIMER_
	atLONG lTimerData1;
	atLONG lTimerData2;
//#endif

//#ifdef __USES_SYSAPI_KEYPAD_
	atLONG lKeyData1;
	atLONG lKeyData2;
//#endif

//#ifdef __USES_SYSAPI_TOUCH_
	atLONG lTouchData1;
	atLONG lTouchData2;
//#endif

//#ifdef __USES_SYSAPI_SOUND_
	atLONG lSoundData1;
	atLONG lSoundData2;
//#endif

//#ifdef __USES_SYSAPI_GPS_
	atLONG lGPSData1;
	atLONG lGPSData2;
//#endif
} SYSAPI_INIT_DATA;


atBOOL	SYSAPI_Initialize( SYSAPI_INIT_DATA *lpInitData, atDWORD dwInitMask );
void 	SYSAPI_Finalize(void);


#ifdef __cplusplus
}
#endif


#endif
