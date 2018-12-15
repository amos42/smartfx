#include "Global.h"


GLOBALSETTING    g_GlobalSetting;
GLOBALIMAGE		g_GlobalImages;  


static void LoadGlobalImages(void)
{
}

static void LoadGlobalSettings(void)
{
}


void InitGlobalVar(void)
{
    g_GlobalSetting.bCaption = atTRUE;
	g_GlobalSetting.nOSDTrans = 3;   

	g_GlobalSetting.nTheme = 0;
	g_GlobalSetting.nBright = 5;
	g_GlobalSetting.nContrast = 5;
}

void FinalGlobalVar(void)
{
}


