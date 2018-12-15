#include "A_UIENG.h"
#include "UI_API/UI_API.h"
#include "OEM_UI_Sound.h"


atBOOL				OEM_UI_InitSound( atLONG lSysInst, atLONG lInitData1, atLONG lInitData2 )
{
		return atTRUE;
}

atVOID				OEM_UI_FinalSound( NOARGS )
{
}

OEM_UI_SOUND	OEM_UI_PlaySound( atLPVOID lpSoundStream, atUI_SOUNDTYPE nSoundType, atDWORD dwFlags )
{
	return (OEM_UI_SOUND)atNULL;
}
	
atVOID	  		OEM_UI_StopSound( OEM_UI_SOUND hOemSound )
{
}
