#include "A_UIENG.h"
#include "UI_API/UI_API.h"
#include "OEM_UI_Sound.h"
#include <windows.h>


#ifndef _WIN32_WCE
//#pragma comment(lib, "winmm.lib")
//#pragma comment(lib, "Wavelib.lib")
#endif



static atBOOL				OEM_UI_InitSound( atLONG lSysInst, atLONG lInitData1, atLONG lInitData2 )
{
		return atTRUE;
}

static atVOID				OEM_UI_FinalSound( NOARGS )
{
}

static OEM_UI_SOUND	OEM_UI_PlaySound( atLPVOID lpSoundStream, atUI_SOUNDTYPE nSoundType, atDWORD dwFlags )
{
	BOOL r;
	DWORD flags;
	
	if( nSoundType != atUI_SOUNDTYPE_WAV ) return (OEM_UI_SOUND)atNULL;
		
	flags = SND_MEMORY | SND_ASYNC;
	if( dwFlags & atUI_SOUNDFLAGS_REPEAT ) flags |= SND_LOOP;

	r = PlaySound( (LPCTSTR)lpSoundStream, NULL, flags );
	if( !r ) return (OEM_UI_SOUND)atNULL;
	
	return (OEM_UI_SOUND)lpSoundStream;
}
	
static atVOID	  		OEM_UI_StopSound( OEM_UI_SOUND hOemSound )
{
#ifdef _WIN32_WCE
	PlaySound( (LPCTSTR)NULL, NULL, SND_MEMORY );
#else
	PlaySound( (LPCTSTR)hOemSound, NULL, SND_PURGE );
#endif
}


void init_uiapi_sound_oem() 
{
	OEM_UI_SOUND_T oem_funcs = {
		OEM_UI_InitSound,
		OEM_UI_FinalSound,

		OEM_UI_PlaySound,
		OEM_UI_StopSound
	};

	OEM_UI_Sound_SetOemFn( &oem_funcs );
}
