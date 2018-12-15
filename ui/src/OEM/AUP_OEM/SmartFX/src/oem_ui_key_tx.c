//
// Key.c
//
////////////////////////////////////////////////////////////////

#include "UI_MNG.h"
#include "UI_API/UI_API.h"
#include "OEM_UI_key.h"
#include "SYSAPI.h"


static int syskey_to_vkey( int syskey )
{
    int vk = atVKEY_NULL;

    switch( syskey ){
			case SYS_VKEY_USERDEF + 100: vk = atVKEY_USERDEF + 100; break;			
    }

    return vk;
}


static int func( int msg, long param1, long param2 )
{
	int vkey;

	switch (msg) 
	{
		case SYS_MSG_KEYDOWN:
			vkey = syskey_to_vkey( param1 );
			atUIENG_OnKeyDown( vkey );
			break;

		case SYS_MSG_KEYUP:
			vkey = syskey_to_vkey( param1 );
			atUIENG_OnKeyUp( vkey );
			break;

		case SYS_MSG_CHAR:
			atUIENG_OnKeyChar( param1 );
			break;

//		case SYS_MSG_TOUCHDOWN:
// 			atUIENG_OnMouseDown( (param1)? 2 : 1, param2 >> 16, param2 & 0xffff );
//			break;

//		case SYS_MSG_TOUCHMOVE:
//			atUIENG_OnMouseMove( param1, param2 >> 16, param2 & 0xffff );
//			break;

//		case SYS_MSG_TOUCHUP:
//			atUIENG_OnMouseUp( param1, param2 >> 16, param2 & 0xffff );
//			break;
			
		case SYS_MSG_SCROLL:
			if( param1 > 0 )
				atUIENG_OnSysKeyDown( atVKEY_SCROLLDOWN );
			else if( param1 < 0 )
				atUIENG_OnSysKeyDown( atVKEY_SCROLLUP );
		   	break;
	}
	
	return 0;
}


atBOOL OEM_UI_InitKeypad(  atLONG lSysInst, atLONG lInitData1, atLONG lInitData2 )
{
// Memory, String, File, Debug, Task, Timer, Input, Sound, GPS는 시스템에서 초기화를 관리한다.
//	SYSAPI_InitInputDev( lSysInst, lInitData1, lInitData2 );

	SYSAPI_SetKeypadNotifyFunc( func );

	return atTRUE;
}


void OEM_UI_FinalKeypad(void)
{
// Memory, String, File, Debug, Task, Timer, Input, Sound, GPS는 시스템에서 초기화를 관리한다.
//	SYSAPI_FinalInputDev();
}


int OEM_UI_ConvOEMKey2VKey( int OEMKey )
{
	return syskey_to_vkey( SYSAPI_ConvOEMKey2VKey(OEMKey) );
}


int OEM_UI_ConvVKey2OEMKey( int VKey )
{
    return SYSAPI_ConvVKey2OEMKey( VKey );
}



void init_uiapi_key_oem() 
{
	OEM_UI_KEY_T oem_funcs = {
		OEM_UI_InitKeypad,
		OEM_UI_FinalKeypad,

		atNULL, //OEM_UI_GetMousePos,

		OEM_UI_ConvOEMKey2VKey,
		OEM_UI_ConvVKey2OEMKey,
		atNULL, //OEM_UI_ConvOEMPtr2VPtr,
		atNULL //OEM_UI_ConvVPtr2OemPtr
	};

	OEM_UI_Key_SetOemFn( &oem_funcs );
}
