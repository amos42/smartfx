/**
  @file UI_Key.h
  @brief Key 입력에 관한 API
  @remark Key 입력에 관한 API
  @author Amos
  @date 2010-04-19 오전 11:41:00
  @see UI_Key.c
*/
#ifndef __UI_KEY_H_
#define __UI_KEY_H_

#include "UI_Types.h"


#ifdef __cplusplus
extern "C" {
#endif


typedef enum
{
    atVKEY_NULL = 0,

	// ASCII 영역 내의 키코드
	atVKEY_ASCIICODE = 0x00001,

    
	// 이후부터는 확장된 키코드
	atVKEY_EXTENDED = 0x10000,

	atVKEY_0,
	atVKEY_1,
	atVKEY_2,
	atVKEY_3,
	atVKEY_4,
	atVKEY_5,
	atVKEY_6,
	atVKEY_7,
	atVKEY_8,
	atVKEY_9,
	atVKEY_STAR,
	atVKEY_POUND,

	atVKEY_SOFT1,
	atVKEY_SOFT2,
	atVKEY_SOFT3,

	atVKEY_SIDE1,
	atVKEY_SIDE2,
	atVKEY_SIDE3,

	atVKEY_LEFT,
	atVKEY_RIGHT,
	atVKEY_UP,
	atVKEY_DOWN,

	atVKEY_SELECT,
	atVKEY_CLEAR,
	atVKEY_SEND,
	atVKEY_POWER,
	atVKEY_END,

	atVKEY_PLAY,
	atVKEY_STOP,
	atVKEY_FF,
	atVKEY_REW,
	atVKEY_REC,
	atVKEY_HOLD,
	atVKEY_MODE,
	atVKEY_MODE2,
	atVKEY_MODE3,
	atVKEY_MENU,
	atVKEY_VOLUP,
	atVKEY_VOLDOWN,
	atVKEY_OPTIONS,	
	atVKEY_EXIT,	
	
	atVKEY_SCROLLUP,
	atVKEY_SCROLLDOWN,
	
	atVKEY_F1,
	atVKEY_F2,
	atVKEY_F3,
	atVKEY_F4,
	atVKEY_F5,
	atVKEY_F6,
	atVKEY_F7,
	atVKEY_F8,
	atVKEY_F9,
	atVKEY_F10,

	// Debug용 가상 키
	atVKEY_DEBUG1,
	atVKEY_DEBUG2,
	atVKEY_DEBUG3,

	// 사용자 정의 키코드 영역
	atVKEY_USERDEF_START = 0x20000,
	atVKEY_USERDEF = atVKEY_USERDEF_START,
	atVKEY_USERDEF_END   = 0x2FFFF,

	// 키코드 끝.
	atVKEY_MAX
} atVKEY;


atBOOL 	atUIAPI_InitKeypad( atLONG lSysInst, atLONG lInitData1, atLONG lInitData2 );
void 	atUIAPI_FinalKeypad(void);

void atUIAPI_UseKeyTimer( atBOOL bUseKeyTimer );
void atUIAPI_StartKeyTimer( int nVKey );
void atUIAPI_EndKeyTimer(void);

int 	atUIAPI_ConvOEMKey2VKey( int OEMKey );
int 	atUIAPI_ConvVKey2OEMKey( int VKey );


#ifdef __cplusplus
}
#endif


#endif 
