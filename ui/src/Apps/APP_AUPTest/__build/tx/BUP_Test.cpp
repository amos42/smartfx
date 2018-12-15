/*
 *
 * ColumbusApplication.cpp : Defines the entry point for the application.
 * 
 *   Copyright 2009 Beagle Co., LTD.
 *	 All rights reserved.
 *
 */


#include "navi.h"
#include <stdio.h>
#include "BUP_Test.h"

#include "APP_Entry.h"
#include "GDI.h"


class CBUP_Test
{
public:
	
	CBUP_Test();
	~CBUP_Test();	

private:
	UINT m_hInstance;

};


CBUP_Test::CBUP_Test()
{
	m_hInstance = 0;
}


CBUP_Test::~CBUP_Test()
{
}



#define _SCR_RESOLUTION_WIDTH   480
#define _SCR_RESOLUTION_HEIGHT  272

HDC g_hDC = NULL;


HBUP_Test BeaglemapCreate(HDC hDC)
{
	fprintf(stderr, "[Navi] BeaglemapCreate\n");
	fprintf(stderr, "[Navi] %s\n", __TIME__);


 	CBUP_Test* pApp = new CBUP_Test();
	if(!pApp)
	{
		fprintf(stderr, "[Navi] BeaglemapCreate - failed!!\n");		
		return 0;
	}

	if(hDC)
	{
		g_hDC = hDC;
		fprintf(stderr, "[Navi] HDC = %x, Address of top left %x\n", g_hDC, g_hDC->pbyAddr);
	}
	else
	{
		fprintf(stderr, "[Navi] Error  hDC is NULL!!!\n");
	}


	APP_Entry_Init( (void*)pApp, (void*)pApp, _SCR_RESOLUTION_WIDTH, _SCR_RESOLUTION_HEIGHT);

	return (HBUP_Test)pApp;
}


void BeaglemapActive(HBUP_Test  hApp, UINT bActive)
{
	fprintf(stderr, "[Navi] BeaglemapActive\n");

	CBUP_Test* pApp = (CBUP_Test*)hApp;
	if(!pApp) return;

	APP_Entry_Enter();

	APP_Entry_Draw();

	// to-do

}


void BeaglemapSuspend(HBUP_Test hApp)
{
	CBUP_Test* pApp = (CBUP_Test*)hApp;
	if(!pApp) return;

	// to-do
}


UINT BeaglemapStatus(HBUP_Test hApp)
{
	CBUP_Test* pApp = (CBUP_Test*)hApp;
	if(!pApp) return 0;

	// to-do

	return 0;
}


int BeaglemapTerminate(HBUP_Test hApp)
{
	fprintf(stderr, "[Navi] BeaglemapTerminate\n");

	CBUP_Test* pApp = (CBUP_Test*)hApp;
	if(!pApp) return 0;

	APP_Entry_Exit();
	
	// to-do

	delete pApp;

	return 0;
}



int BeaglemapKeyEvent(HBUP_Test hApp, HDC hDC, UINT key)
{

/**
	CBUP_Test* pApp = (CBUP_Test*)hApp;
	if(!pApp) return 0;
**/

	if(!hDC) return 0;

	if(!hDC->pbyAddr) return 0;

	fprintf( stderr, "HDC %x, Address of top left = %x\n", hDC, hDC->pbyAddr);

	switch( key ) {
    case EVENT_UP_KEY:
        fprintf( stderr, "EVENT_UP_KEY\n" );
		memset(hDC->pbyAddr +100, 0x3f, 1000);
		break;
    case EVENT_DOWN_KEY:
        fprintf( stderr, "EVENT_DOWN_KEY Start \n" );
		memset(hDC->pbyAddr +100, 0x5f, 1000);
        break;
    case EVENT_LEFT_KEY:
        fprintf( stderr, "EVENT_LEFT_KEY\n" );
		memset(hDC->pbyAddr +100, 0xf0, 1000);
		break;
    case EVENT_RIGHT_KEY:
		fprintf( stderr, "EVENT_RIGHT_KEY\n" );
		memset(hDC->pbyAddr +100, 0xf7, 1000);
        break;
    case EVENT_JUMP_LEFT_KEY:
        fprintf( stderr,  "EVENT_JUMP_LEFT_KEY\n" );
        break;
    case EVENT_JUMP_RIGHT_KEY:
        fprintf( stderr,  "EVENT_JUMP_RIGHT_KEY\n" );
		break;
    case EVENT_TELE_KEY:
        fprintf( stderr,  "EVENT_TELE_KEY\n" );
        break;
    case EVENT_WIDE_KEY:
        fprintf( stderr, "EVENT_WIDE_KEY\n" );
        break;
    case EVENT_EFFECT_KEY:
        fprintf( stderr, "EVENT_EFFECT_KEY\n" );
        break;
    case EVENT_ENTER_KEY:
        fprintf( stderr, "EVENT_ENTER_KEY\n" );
        break;
    case EVENT_ADJDEL_KEY:
        fprintf( stderr,  "EVENT_ADJDEL_KEY\n" );
        break;
    default:
        break;
    }

	return 0;

}


