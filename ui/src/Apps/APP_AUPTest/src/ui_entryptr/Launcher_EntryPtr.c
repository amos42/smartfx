#include "EntryPtr.h"

#include "A_UIEng.h"
#include "SYSAPI.h"
#include "APP_Entry.h"

#include "User_ResDump.h"


#define HEAP_SIZE (4*1024*1024)
static char heap[HEAP_SIZE + 1024];
//static char *heap = (char *)0xc0050000; //[50*1024]; //
//static short    pScreenBuffer[320*240];
static short    pScreenBuffer[320 * 480];
//static short *pScreenBuffer = atNULL;

extern void OEM_InitSYSAPIfunc(void);
extern void OEM_InitUIAPIfunc(void);


void ENTRYPTR_Initialize()
{
	OEM_InitSYSAPIfunc();
	OEM_InitUIAPIfunc();
}


void   ENTRYPTR_GetName( char *name )
{
	SYSAPI_strcpy( name, "SmartFX" );
}


BOOL_T ENTRYPTR_Create( void *inst, void *handle, int width, int height )
{
//   DebugSetOnwerWnd((HWND)handle);
//   DebugSetMsgFlag (1);

	APP_Entry_Init((void *)inst, (void *)handle, heap, HEAP_SIZE, width, height, (void *)pScreenBuffer, (void *)pUser_ResData, sizeof(pUser_ResData));
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


/*
void   ENTRYPTR_MouseDown( int btn, int x, int y )
{
		atUIENG_OnMouseDown( btn, x, y ); 
}


void   ENTRYPTR_MouseUp( int btn, int x, int y )
{
		atUIENG_OnMouseUp( btn, x, y ); 
}
*/


void   ENTRYPTR_ActivateDebug( BOOL_T bDebug )
{
	  atUIAPI_ActivateDebug( bDebug );
}

int   ENTRYPTR_SysCommand( int code, int value )
{
	  atUIENG_OnSystemNotify( code, value );
	  return 0;
}

int   ENTRYPTR_GetSystemVar( int code )
{
	  atUIENG_OnSystemNotify( code, 0 );
	  return 0;
}
