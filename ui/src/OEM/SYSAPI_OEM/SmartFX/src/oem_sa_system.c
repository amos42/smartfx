//
// sound_wipic.c
//
////////////////////////////////////////////////////////////////

#include "OEM_SA_system.h"
 

static atLONG OEM_SYSAPI_InitSystem( atLONG lSysInst, atLONG lInitData1, atLONG lInitData2 )
{
	/// 여기에 메인 메세지 처리 proc 생성 및 등록
	return 0;
}


static atVOID OEM_SYSAPI_FinalSystem( NOARGS )
{
}


static atLONG OEM_SYSAPI_GetSystemVar(SYSAPI_SYSVAR sysvar)
{
	long ret = 0;

	switch(sysvar) {
	case SYSAPI_SYSVAR_BACKLIGHT :
		ret = 100;
		break;
	case SYSAPI_SYSVAR_BATTERY :
		ret = 100;
		break;
	}

	return ret;
}


static atLONG OEM_SYSAPI_SetSystemVar(SYSAPI_SYSVAR sysvar, atLONG value)
{
	return 0;
}


static atINT OEM_SYSAPI_SystemOff(atINT ExitCode)
{

	return 0;
}


static atINT  OEM_SYSAPI_InvokeSysGUI( SYS_FUNC_INVOKE_SYSGUI *fnFunc, atLONG lParam )
{
	return 0;
}


extern atUINT16 bsp_adc_get(atUINT8 ch);
extern void bsp_led_nor_on(void);
extern void bsp_led_nor_off(void);

static atINT OEM_SYSAPI_SysCommand(SYSAPI_SYSCMD cmdCode, atLONG param1, atLONG param2)
{
	atINT ret = 0;
	switch(cmdCode)
	{
	case 100:
		ret = (atINT)bsp_adc_get((atUINT8)param1);
		// command for launching task swapper from options menu
//		if(g_appui) ((CAknAppUi*)g_appui)->ProcessCommandL(EAknCmdTaskSwapper);
//		if(g_appview) ((CAknView*)g_appview)->HandleCommandL( EAknCmdTaskSwapper );
		break;
		
	case 300:
		if (param1 == 0)  bsp_led_nor_off();
		else  bsp_led_nor_on();
		break;
		
	default :
		break;
	}
	return ret;
}



void init_sysapi_system_oem(void) 
{
	OEM_SYSAPI_SYSTEM_T oem_funcs = {
		OEM_SYSAPI_InitSystem,
		OEM_SYSAPI_FinalSystem,

		OEM_SYSAPI_GetSystemVar,
		OEM_SYSAPI_SetSystemVar,

		OEM_SYSAPI_SystemOff,

		OEM_SYSAPI_InvokeSysGUI,

		OEM_SYSAPI_SysCommand
	};

	OEM_SYSAPI_SYSTEM_SetOemFn( &oem_funcs );
}
