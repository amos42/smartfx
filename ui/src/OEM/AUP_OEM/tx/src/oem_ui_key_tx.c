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
        case SYS_VKEY_SELECT			: vk = atVKEY_SELECT;        break;
		case SYS_VKEY_CLEAR			: vk = atVKEY_CLEAR;		break;
        case SYS_VKEY_UP				: vk = atVKEY_UP;        	break;
        case SYS_VKEY_DOWN			: vk = atVKEY_DOWN;        	break;
        case SYS_VKEY_LEFT			: vk = atVKEY_LEFT;        	break;
        case SYS_VKEY_RIGHT			: vk = atVKEY_RIGHT;        	break;
        case SYS_VKEY_SEND			: vk = atVKEY_SEND;        	break;
        case SYS_VKEY_END			: vk = atVKEY_END;        	break;
        case SYS_VKEY_SOFT1			: vk = atVKEY_SOFT1;        	break;
        case SYS_VKEY_SOFT2			: vk = atVKEY_SOFT2;        	break;
        case SYS_VKEY_0		: vk = atVKEY_0;        		break;
        case SYS_VKEY_1		: vk = atVKEY_1;        		break;
        case SYS_VKEY_2		: vk = atVKEY_2;        		break;
        case SYS_VKEY_3		: vk = atVKEY_3;        		break;
        case SYS_VKEY_4		: vk = atVKEY_4;        		break;
        case SYS_VKEY_5		: vk = atVKEY_5;        		break;
        case SYS_VKEY_6		: vk = atVKEY_6;        		break;
        case SYS_VKEY_7		: vk = atVKEY_7;        		break;
        case SYS_VKEY_8		: vk = atVKEY_8;        		break;
        case SYS_VKEY_9		: vk = atVKEY_9;        		break;
		case SYS_VKEY_STAR				: vk = atVKEY_STAR;		break;
		case SYS_VKEY_POUND		: vk = atVKEY_POUND;	   	break;
		
		case SYS_VKEY_DEBUG1				: vk = atVKEY_DEBUG1;	   	break;
    }

    return vk;
}


static void func( int msg, long param1, long param2 )
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

		case SYS_MSG_TOUCHDOWN:
 			atUIENG_OnMouseDown( (param1)? 2 : 1, param2 >> 16, param2 & 0xffff );
			break;

		case SYS_MSG_TOUCHMOVE:
			atUIENG_OnMouseMove( param1, param2 >> 16, param2 & 0xffff );
			break;

		case SYS_MSG_TOUCHUP:
			atUIENG_OnMouseUp( param1, param2 >> 16, param2 & 0xffff );
			break;
			
		case SYS_MSG_SCROLL:
			if( param1 > 0 )
				atUIENG_OnSysKeyDown( atVKEY_SCROLLDOWN );
			else if( param1 < 0 )
				atUIENG_OnSysKeyDown( atVKEY_SCROLLUP );
		   	break;
	}
}


atBOOL OEM_UI_InitInputDev(  atLONG lSysInst, atLONG lInitData1, atLONG lInitData2 )
{
	SYSAPI_InitInputDev( lSysInst, lInitData1, lInitData2 );
	SYSAPI_SetInputNotifyFunc( func );

	return atTRUE;
}


void OEM_UI_FinalInputDev(void)
{
	SYSAPI_FinalInputDev();
}
	

int OEM_UI_ConvOEMKey2VKey( int OEMKey )
{
	return syskey_to_vkey( SYSAPI_ConvOEMKey2VKey(OEMKey) );
}


int OEM_UI_ConvVKey2OEMKey( int VKey )
{
    return SYSAPI_ConvVKey2OEMKey( VKey );
}


int OEM_UI_ConvOEMPtr2VPtr( int *x, int *y )
{
	return SYSAPI_ConvOEMPtr2VPtr( x, y );
}


int OEM_UI_ConvVPtr2OemPtr( int *x, int *y )
{
	return SYSAPI_ConvVPtr2OemPtr( x, y );
}

