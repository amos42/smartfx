#include "A_UIENG.h"
#include "OEM_UI_mem.h"
#include <malloc.h>

atBOOL DEF_OEM_UI_InitMem( atLONG lSysInst, atLONG lInitData1, atLONG lInitData2 )
{
	return atTRUE;
}


void DEF_OEM_UI_FinalMem(void)
{
}


void * DEF_OEM_UI_AllocMem( long size )
{
	return (void *)malloc(size);
}


void * DEF_OEM_UI_ReallocMem( void *ptr, long size )
{
	return (void *)realloc( ptr, size );
}


int DEF_OEM_UI_FreeMem( void *ptr )
{
	free( ptr );
	return 0;
}


atBOOL  DEF_OEM_UI_CheckValidateHeap( NOARGS )
{
	return atTRUE;
}


atVOID  DEF_OEM_UI_GetHeapInfo( atLPVOID lpParam, void (*pViewFunc)(atLPVOID lpParam, void *addr, int pos, int size, int type) )
{	
}


atLONG  DEF_OEM_UI_GetTotalLeftHeapSize( NOARGS )
{
	return 0;
}


atVOID  DEF_OEM_UI_CompactHeap( NOARGS )
{
}

