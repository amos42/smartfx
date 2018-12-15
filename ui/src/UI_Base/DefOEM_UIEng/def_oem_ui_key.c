//
// Key.c
//
////////////////////////////////////////////////////////////////

#include "UI_MNG.h"
#include "UI_API/UI_API.h"
#include "OEM_UI_key.h"


atBOOL DEF_OEM_UI_InitKeypad(  atLONG lSysInst, atLONG lInitData1, atLONG lInitData2 )
{
	return atTRUE;
}


void DEF_OEM_UI_FinalKeypad(void)
{
}


int DEF_OEM_UI_ConvOEMKey2VKey( int OEMKey )
{
	return OEMKey; 
}


int DEF_OEM_UI_ConvVKey2OEMKey( int VKey )
{
    return VKey;
}
