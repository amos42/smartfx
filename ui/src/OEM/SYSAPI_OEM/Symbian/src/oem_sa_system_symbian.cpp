//
// sound_wipic.c
//
////////////////////////////////////////////////////////////////

#include "OEM_SA_system.h"
#include <aknappui.h>
#include <aknview.h> 


CAknAppUi *g_SYS_appui   = atNULL;
CAknView *g_SYS_appview   = atNULL;
 

atLONG OEM_SYSAPI_Initialize( SYSAPI_INIT_DATA *lpInitData )
{
	g_SYS_appui   = (CAknAppUi *)lpInitData->lSysInst;
	g_SYS_appview   = (CAknView *)lpInitData->lSysData1;
	
	/// 여기에 메인 메세지 처리 proc 생성 및 등록
	return 0;
}


atVOID OEM_SYSAPI_Finalize( NOARGS )
{
	g_SYS_appview = atNULL;
	g_SYS_appui = atNULL;
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
	if( g_SYS_appui ) g_SYS_appui->Exit();
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
