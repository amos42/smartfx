#include "EntryPtr.h"

#include "A_UIEng.h"
#include "SYSAPI.h"
#include "APP_Entry.h"


#define HEAP_SIZE (7*1024*1024)
static char *heap = (char *)0xc0050000; 
//static short    pScreenBuffer[320 * 480];
static const short *pScreenBuffer = atNULL;
static const unsigned char *pUser_ImgData = (const unsigned char *)0x90000000;
static const unsigned char *pUser_ResData = (const unsigned char *)0x90100000;

void   ENTRYPTR_GetName( char *name )
{
	SYSAPI_strcpy( name, "SmartFX" );
}


BOOL_T ENTRYPTR_Create( void *inst, void *handle, int width, int height )
{
	APP_Entry_Init((void *)inst, (void *)handle, heap, HEAP_SIZE, width, height, (void *)pScreenBuffer, (void *)pUser_ResData, 0);
	return atTRUE;
}


void   ENTRYPTR_Destroy(void)
{
	  APP_Entry_Final();
}


void   ENTRYPTR_Enter(void)
{
			APP_Entry_Enter();
}


void   ENTRYPTR_Exit(void)
{
			APP_Entry_Exit();
}


void   ENTRYPTR_Draw(void)
{
			APP_Entry_Draw();
}


void   ENTRYPTR_Resize( int width, int height )
{
		atUIENG_OnResize( width, height ); 
}


void   ENTRYPTR_ActivateDebug( BOOL_T bDebug )
{
	  atUIAPI_ActivateDebug( bDebug );
}
