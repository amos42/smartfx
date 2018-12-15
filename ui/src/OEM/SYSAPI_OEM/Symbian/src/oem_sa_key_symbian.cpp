//
// Key.c
//
////////////////////////////////////////////////////////////////
#include "SYSAPI.h"
#include "OEM_SA_key.h"
//#include "AEEEmulAPI.h"
//#include <w32std.h>
#include <e32keys.h>
#include <avkon.hrh>


static SYS_FUNC_INPUTNOTIFY *g_fnInputFunc;


static atVOID key_func( atINT key_event, atINT key_code )
{
/*
	if( key_event == 6 )
		atUIENG_OnKeyDown( key_code );
	else if( key_event == 7 )
		atUIENG_OnLongKeyDown( key_code );
	else if( key_event == 8 )
		atUIENG_OnKeyUp( key_code );
		*/
}

static atVOID mouse_func( atINT mouse_event, atINT btn, atINT x, atINT y )
{
/*
	if( mouse_event == 9 )
		atUIENG_OnMouseDown( btn, x, y );
	else if( mouse_event == 10 )
		atUIENG_OnMouseMove( btn, x, y );
	else if( mouse_event == 11 )
		atUIENG_OnMouseUp( btn, x, y );
		*/
}

atBOOL OEM_SYSAPI_InitInputDev(  atLONG lSysInst, atLONG lInitData1, atLONG lInitData2 )
{
	g_fnInputFunc = atNULL;

	return atTRUE;
}


atVOID OEM_SYSAPI_FinalInputDev( NOARGS )
{
	g_fnInputFunc = atNULL;
}
	

void	OEM_SYSAPI_SetKeypadNotifyFunc( SYS_FUNC_INPUTNOTIFY *fnInputFunc )
{
	g_fnInputFunc	= fnInputFunc;
}
	

atINT OEM_SYSAPI_ConvOEMKey2VKey( atINT OEMKey )
{
    atINT vk = SYS_VKEY_NULL;

    switch( OEMKey )
    {
        case EStdKeyDevice0			: vk = SYS_VKEY_SOFT1;        break;
        case EStdKeyDevice1			: vk = SYS_VKEY_SOFT2;        break;
        case EStdKeyDevice3			: vk = SYS_VKEY_SELECT;       break;
        case EStdKeyMenu			: vk = SYS_VKEY_MENU;       break;
        case EStdKeyUpArrow			: vk = SYS_VKEY_UP;        break;
        case EStdKeyDownArrow		: vk = SYS_VKEY_DOWN;        break;
        case EStdKeyLeftArrow		: vk = SYS_VKEY_LEFT;        break;
        case EStdKeyRightArrow		: vk = SYS_VKEY_RIGHT;        break;
        case EStdKeyIncVolume		: vk = SYS_VKEY_VOLUP;        break;
        case EStdKeyDecVolume		: vk = SYS_VKEY_VOLDOWN;        break;
        //case EStdKeyBackspace		: vk = SYS_VKEY_CLEAR;        break;
        case EAknSoftkeyOptions		: vk = SYS_VKEY_OPTIONS;		break;
        case EStdKeyNo				:	//A¨ú¡¤a ©öo¨¡¡Æ ¢¥¡©¡¤¢ÒA¡í¢Ò¡×
        case EAknSoftkeyExit		: vk = SYS_VKEY_EXIT;		break;
        case EStdKeyHash			: vk = SYS_VKEY_POUND;		break;
        case EStdKeyNkpAsterisk		: vk = SYS_VKEY_STAR;	break;
        case EAknSoftkeyOk			: vk = SYS_VKEY_SOFT1;	break;
        case EAknSoftkeyCancel		: vk = SYS_VKEY_SOFT2;	break;
        /*
        case EStdKeyDevice0			: vk = SYS_VKEY_SOFT1;        break;
        case EStdKeyDevice1			: vk = SYS_VKEY_SOFT2;        break;  
        */      
        case '1'					:
        case EStdKeyNkp1			: vk = SYS_VKEY_1;        break;
        case '2'					:
        case EStdKeyNkp2			: vk = SYS_VKEY_2;        break;
        case '3'					:
        case EStdKeyNkp3			: vk = SYS_VKEY_3;        break;
        case '4'					:
        case EStdKeyNkp4			: vk = SYS_VKEY_4;        break;
        case '5'					:
        case EStdKeyNkp5			: vk = SYS_VKEY_5;        break;
        case '6'					:
        case EStdKeyNkp6			: vk = SYS_VKEY_6;        break;
        case '7'					:
        case EStdKeyNkp7			: vk = SYS_VKEY_7;        break;
        case '8'					:
        case EStdKeyNkp8 			: vk = SYS_VKEY_8;        break;
        case '9'					:
        case EStdKeyNkp9			: vk = SYS_VKEY_9;        break;
        case '0'					:
        case EStdKeyNkp0			: vk = SYS_VKEY_0;        break;
        //case EStdKeyNkpAsterisk		: vk = SYS_VKEY_STAR;        break;
        //case EStdKeyHash			: vk = SYS_VKEY_POUND;        break;
        case EStdKeyBackspace		: vk = SYS_VKEY_CLEAR;        break;
    }

    return vk;
}


atINT OEM_SYSAPI_ConvVKey2OEMKey( atINT VKey )
{
/*
    atINT ok = 0;
    
    switch( VKey )
    {
        case SYS_VKEY_PLAY		: ok = KEY_PLAY;        break;
        case SYS_VKEY_FF			: ok = KEY_FF;        break;
        case SYS_VKEY_REW			: ok = KEY_REW;        break;
        case SYS_VKEY_REC			: ok = KEY_REC;        break;
        case SYS_VKEY_STOP		: ok = KEY_STOP;        break;
        case SYS_VKEY_MENU		: ok = KEY_MENU;        break;
        case SYS_VKEY_SELECT		: ok = KEY_EQ;        break;
        case SYS_VKEY_HOLD		: ok = KEY_HOLD;        break;
    }

    return ok;
    */
	return 0;
}


atINT OEM_SYSAPI_ConvOEMPtr2VPtr( atINT *x, atINT *y )
{
	return 0;
}


atINT OEM_SYSAPI_ConvVPtr2OemPtr( atINT *x, atINT *y )
{
	return 0;
}

