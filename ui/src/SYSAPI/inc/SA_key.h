//
// SYSUTIL.h
//
////////////////////////////////////////////////////////////////

#ifndef __SA_KEY_H_
#define __SA_KEY_H_

#include "SA_Types.h"


#ifdef __cplusplus
extern "C" {
#endif


typedef enum {
	SYS_MSG_KEYDOWN,
	SYS_MSG_KEYUP,
	SYS_MSG_CHAR,
	SYS_MSG_SCROLL
} SYS_MSG_KEY;

typedef int (SYS_FUNC_KEYPADNOTIFY)( int msg, long lParam1, long lParam2 );


typedef enum
{
    SYS_VKEY_NULL = 0,

	// ASCII 영역 내의 키코드
	SYS_VKEY_ASCIICODE = 0x00001,

    
	// 이후부터는 확장된 키코드
	SYS_VKEY_EXTENDED = 0x10000,

	SYS_VKEY_0,
	SYS_VKEY_1,
	SYS_VKEY_2,
	SYS_VKEY_3,
	SYS_VKEY_4,
	SYS_VKEY_5,
	SYS_VKEY_6,
	SYS_VKEY_7,
	SYS_VKEY_8,
	SYS_VKEY_9,
	SYS_VKEY_STAR,
	SYS_VKEY_POUND,

	SYS_VKEY_SOFT1,
	SYS_VKEY_SOFT2,
	SYS_VKEY_SOFT3,

	SYS_VKEY_SIDE1,
	SYS_VKEY_SIDE2,
	SYS_VKEY_SIDE3,

	SYS_VKEY_LEFT,
	SYS_VKEY_RIGHT,
	SYS_VKEY_UP,
	SYS_VKEY_DOWN,

	SYS_VKEY_SELECT,
	SYS_VKEY_CLEAR,
	SYS_VKEY_SEND,
	SYS_VKEY_POWER,
	SYS_VKEY_END,

	SYS_VKEY_PLAY,
	SYS_VKEY_STOP,
	SYS_VKEY_FF,
	SYS_VKEY_REW,
	SYS_VKEY_REC,
	SYS_VKEY_HOLD,
	SYS_VKEY_MODE,
	SYS_VKEY_MODE2,
	SYS_VKEY_MODE3,
	SYS_VKEY_MENU,
	SYS_VKEY_VOLUP,
	SYS_VKEY_VOLDOWN,
	SYS_VKEY_OPTIONS,	
	SYS_VKEY_EXIT,	
	
	SYS_VKEY_HASH,
	SYS_VKEY_ASTER, 

	// Debug용 가상 키
	SYS_VKEY_DEBUG1,
	SYS_VKEY_DEBUG2,
	SYS_VKEY_DEBUG3,
	
	// 사용자 정의 키코드 영역
	SYS_VKEY_USERDEF_START = 0x20000,
	SYS_VKEY_USERDEF = SYS_VKEY_USERDEF_START,
	SYS_VKEY_USERDEF_END   = 0x2FFFF,

	// 키코드 끝.
	SYS_VKEY_MAX
} SYS_VKEY;


atBOOL 	SYSAPI_InitKeypad( atLONG lSysInst, atLONG lInitData1, atLONG lInitData2 );
void 	SYSAPI_FinalKeypad(void);

void		SYSAPI_SetKeypadNotifyFunc( SYS_FUNC_KEYPADNOTIFY *fnInputFunc );

int 	SYSAPI_ConvOEMKey2VKey( int OEMKey );
int 	SYSAPI_ConvVKey2OEMKey( int VKey );



#ifdef __cplusplus
}
#endif


#endif

