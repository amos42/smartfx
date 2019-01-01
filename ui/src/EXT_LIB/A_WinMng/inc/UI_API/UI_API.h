/**
  @file UI_API.h
  @brief UI API 공통
  @remark UI API 공통
  @author Amos
  @date 2010-04-19 11:41:00
  @see UI_APIs.c
*/
#ifndef __UI_APIS_H_
#define __UI_APIS_H_

#include "UI_API/UI_Cfg.h"

#ifdef __USES_UI_MEM_
#include "UI_API/UI_Mem.h"
#endif
#ifdef __USES_UI_FONT_
#include "UI_API/UI_Font.h"
#endif
#ifdef __USES_UI_SCR_
#include "UI_API/UI_GDI.h"
#endif
#ifdef __USES_UI_TIMER_
#include "UI_API/UI_Timer.h"
#endif
#ifdef __USES_UI_KEYPAD_
#include "UI_API/UI_Key.h"
#endif
#ifdef __USES_UI_TOUCH_
#include "UI_API/UI_Touch.h"
#endif
#ifdef __USES_UI_LIST_
#include "UI_API/UI_List.h"
#endif
#ifdef __USES_UI_STR_
#include "UI_API/UI_Str.h"
#endif
#ifdef __USES_UI_DEBUG_
#include "UI_API/UI_Debug.h"
#endif


#ifdef __cplusplus
extern "C" {
#endif

typedef struct tagatUIAPI_INIT_DATA {
	atLONG lSysInst;
	atLONG lSysData1;
	atLONG lSysData2;
	
	atLONG lOemInst;
	atLONG lCmdInst;
	
//#ifdef __USES_UI_MEMORY_
	atLONG lMemData1;
	atLONG lMemData2;
//#endif

//#ifdef __USES_UI_SCR_
	atLONG lScrData1;
	atLONG lScrData2;
//#endif

//#ifdef __USES_UI_FONT_
	atLONG lFontData1;
	atLONG lFontData2;
//#endif

//#ifdef __USES_UI_TIMER_
	atLONG lTimerData1;
	atLONG lTimerData2;
//#endif

//#ifdef __USES_UI_KEYPAD_
	atLONG lKeyData1;
	atLONG lKeyData2;
//#endif

//#ifdef __USES_UI_TOUCH_
	atLONG lTouchData1;
	atLONG lTouchData2;
//#endif

//#ifdef __USES_UI_DEBUG_
	atLONG lDebugData1;
	atLONG lDebugData2;
//#endif
} atUIAPI_INIT_DATA;


atBOOL	atUIAPI_Initialize( atUIAPI_INIT_DATA *lpInitData );
void 	atUIAPI_Finalize(void);


#ifdef __cplusplus
}
#endif


#endif
