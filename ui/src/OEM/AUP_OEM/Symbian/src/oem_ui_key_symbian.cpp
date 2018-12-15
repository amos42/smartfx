//
// Key.c
//
////////////////////////////////////////////////////////////////
#include "UI_MNG.h"
#include "UI_API/UI_API.h"
#include "OEM_UI_key.h"
#include "SYSAPI.h"

//#include <w32std.h>
#include <e32keys.h>
#include <avkon.hrh>


atBOOL OEM_UI_InitInputDev(  atLONG lSysInst, atLONG lInitData1, atLONG lInitData2 )
{
// Memory, String, File, Debug, Task, Timer, Input, Sound, GPS는 시스템에서 초기화를 관리한다.
//	SYSAPI_InitInputDev( lSysInst, lInitData1, lInitData2 );
	
	return atTRUE;
}


atVOID OEM_UI_FinalInputDev( NOARGS )
{
// Memory, String, File, Debug, Task, Timer, Input, Sound, GPS는 시스템에서 초기화를 관리한다.
//	SYSAPI_FinalInputDev();
}
	

atINT OEM_UI_ConvOEMKey2VKey( atINT OEMKey )
{
    atINT vk = atVKEY_NULL;

    switch( OEMKey )
    {
        case EStdKeyDevice0			: vk = atVKEY_SOFT1;        break;
        case EStdKeyDevice1			: vk = atVKEY_SOFT2;        break;
        case EStdKeyDevice3			: vk = atVKEY_SELECT;       break;
        case EStdKeyMenu			: vk = atVKEY_MENU;       break;
        case EStdKeyUpArrow			: vk = atVKEY_UP;        break;
        case EStdKeyDownArrow		: vk = atVKEY_DOWN;        break;
        case EStdKeyLeftArrow		: vk = atVKEY_LEFT;        break;
        case EStdKeyRightArrow		: vk = atVKEY_RIGHT;        break;
        case EStdKeyIncVolume		: vk = atVKEY_VOLUP;        break;
        case EStdKeyDecVolume		: vk = atVKEY_VOLDOWN;        break;
        //case EStdKeyBackspace		: vk = atVKEY_CLEAR;        break;
        case EAknSoftkeyOptions		: vk = atVKEY_OPTIONS;		break;
        case EStdKeyNo				:	//종료 버튼 눌렀을때
        case EAknSoftkeyExit		: vk = atVKEY_EXIT;		break;
        case EStdKeyHash			: vk = atVKEY_POUND;		break;
        case EStdKeyNkpAsterisk		: vk = atVKEY_STAR;	break;
        case EAknSoftkeyOk			: vk = atVKEY_SOFT1;	break;
        case EAknSoftkeyCancel		: vk = atVKEY_SOFT2;	break;
        /*
        case EStdKeyDevice0			: vk = atVKEY_SOFT1;        break;
        case EStdKeyDevice1			: vk = atVKEY_SOFT2;        break;  
        */      
        case '1'					:
        case EStdKeyNkp1			: vk = atVKEY_1;        break;
        case '2'					:
        case EStdKeyNkp2			: vk = atVKEY_2;        break;
        case '3'					:
        case EStdKeyNkp3			: vk = atVKEY_3;        break;
        case '4'					:
        case EStdKeyNkp4			: vk = atVKEY_4;        break;
        case '5'					:
        case EStdKeyNkp5			: vk = atVKEY_5;        break;
        case '6'					:
        case EStdKeyNkp6			: vk = atVKEY_6;        break;
        case '7'					:
        case EStdKeyNkp7			: vk = atVKEY_7;        break;
        case '8'					:
        case EStdKeyNkp8 			: vk = atVKEY_8;        break;
        case '9'					:
        case EStdKeyNkp9			: vk = atVKEY_9;        break;
        case '0'					:
        case EStdKeyNkp0			: vk = atVKEY_0;        break;
        //case EStdKeyNkpAsterisk		: vk = atVKEY_STAR;        break;
        //case EStdKeyHash			: vk = atVKEY_POUND;        break;
        case EStdKeyBackspace		: vk = atVKEY_CLEAR;        break;
    }

    return vk;
}


atINT OEM_UI_ConvVKey2OEMKey( atINT VKey )
{
/*
    atINT ok = 0;
    
    switch( VKey )
    {
        case atVKEY_PLAY		: ok = KEY_PLAY;        break;
        case atVKEY_FF			: ok = KEY_FF;        break;
        case atVKEY_REW			: ok = KEY_REW;        break;
        case atVKEY_REC			: ok = KEY_REC;        break;
        case atVKEY_STOP		: ok = KEY_STOP;        break;
        case atVKEY_MENU		: ok = KEY_MENU;        break;
        case atVKEY_SELECT		: ok = KEY_EQ;        break;
        case atVKEY_HOLD		: ok = KEY_HOLD;        break;
    }

    return ok;
    */
	return 0;
}


atINT OEM_UI_ConvOEMPtr2VPtr( atINT *x, atINT *y )
{
	return 0;
}


atINT OEM_UI_ConvVPtr2OemPtr( atINT *x, atINT *y )
{
	return 0;
}

