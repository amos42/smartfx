//
// sound_wipic.c
//
////////////////////////////////////////////////////////////////

#include "OEM_SA_system.h"
 

atLONG OEM_SYSAPI_Initialize( SYSAPI_INIT_DATA *lpInitData )
{
	/// 여기에 메인 메세지 처리 proc 생성 및 등록
	return 0;
}


atVOID OEM_SYSAPI_Finalize( NOARGS )
{
}


void OEM_SYSAPI_GetSystemDateTime(SYS_DATETIME *date)
{
}

atLONG OEM_SYSAPI_GetAntennaInfo(void)
{
	return 0;
}

atLONG OEM_SYSAPI_GetBatteryInfo(void)
{
	return 50;
}

atINT OEM_SYSAPI_SystemOff(atINT ExitCode)
{

	return 0;
}

atINT OEM_SYSAPI_SysCommand(int cmdCode, atLONG param1, atLONG param2)
{
	switch(cmdCode)
	{
	case 100:
		// command for launching task swapper from options menu
//		if(g_appui) ((CAknAppUi*)g_appui)->ProcessCommandL(EAknCmdTaskSwapper);
//		if(g_appview) ((CAknView*)g_appview)->HandleCommandL( EAknCmdTaskSwapper );
		break;
	default :
		break;
	}
	return 0;
}
