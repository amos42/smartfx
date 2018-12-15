#include "OEM_UI_Font.h"


atVOID		DEF_OEM_InitFontSystem( atLONG lSysInst, atLONG lInitData1, atLONG lInitData2 )
{
}


atVOID		DEF_OEM_CloseFontSystem( NOARGS )
{
}

atVOID 		DEF_OEM_SuspendFontSystem( NOARGS )
{
}

atVOID 		DEF_OEM_ResumeFontSystem( NOARGS )
{
}

OEM_FONT 	DEF_OEM_CreateFont( atLPTSTR font_face, atINT font_size, atINT font_style, atBOOL IsStock )
{
	return atNULL;
}

atVOID 		DEF_OEM_DestroyFont( OEM_FONT hFont )
{
}

atINT		DEF_OEM_GetStringWidth( OEM_FONT hFont, atLPTSTR str, atINT len )
{	
	return 0;
}

atINT		DEF_OEM_GetFontHeight( OEM_FONT hFont )
{
	return 0;
}

atINT		DEF_OEM_GetGlyph( OEM_FONT hFont )
{
	return 0;
}


