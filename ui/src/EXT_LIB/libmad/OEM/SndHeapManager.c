#include "SndHeapManager.h"

typedef void * HEAP_HANDLE;



#if 0

#include <windows.h>
#include "HeapManager.h"

typedef HANDLE HEAP_HANDLE;

static HEAP_HANDLE g_hSndHeap = NULL;
static DWORD g_dwHeapSemaphore = NULL;
static DWORD g_dwAllocSemaphore = NULL;

HEAP_HANDLE SndHeapCreate()
{
	if (!g_hSndHeap) g_hSndHeap = HeapCreate(HEAP_NO_SERIALIZE, 131072, NULL);	// 128KB
	g_dwHeapSemaphore++;

	return g_hSndHeap;
}

void SndHeapDestory()
{
	if (!g_hSndHeap) return;

	g_dwHeapSemaphore--;
	if (g_dwHeapSemaphore) return;

	HeapDestroy(g_hSndHeap);
	g_hSndHeap = NULL;
	g_dwAllocSemaphore = NULL;
}

HEAP_HANDLE SndHeapHandle()
{
	return g_hSndHeap;
}

LPVOID SndHeapAlloc(DWORD dwSize)
{
	if (!dwSize) return NULL;
	
	if (!g_hSndHeap)
	{
		if (!SndHeapCreate())	return NULL;
	}

	LPVOID pHeap = HeapAlloc(g_hSndHeap, HEAP_NO_SERIALIZE, WORD_ALIGN_LENGTH(dwSize));
	if (!pHeap) return FALSE;

	g_dwAllocSemaphore++;
	
	return pHeap;
}

LPVOID SndHeapRealloc(LPVOID lptr, DWORD dwSize)
{
	if (!g_hSndHeap || !lptr) return SndHeapAlloc(dwSize);
	
	if (!dwSize)
	{
		SndHeapFree(lptr);//, TRUE);
		return NULL;
	}

	LPVOID pHeap = HeapReAlloc(g_hSndHeap, HEAP_NO_SERIALIZE, lptr, WORD_ALIGN_LENGTH(dwSize));

	return pHeap;
}

void SndHeapFree(LPVOID lptr, BOOL bCompact)
{
	if (!g_hSndHeap || !lptr) return;
	
	HeapFree(g_hSndHeap, NULL, lptr);
	g_dwAllocSemaphore--;

	if (g_dwAllocSemaphore)
	{
		//if (bCompact)
			SndHeapCompact();
	}
	else
	{
		SndHeapDestory();
	}
}

void SndHeapCompact()
{
	if (g_hSndHeap) HeapCompact(g_hSndHeap, HEAP_NO_SERIALIZE);
}

DWORD SndHeapSize(LPVOID lptr)
{
	if (!g_hSndHeap || !lptr) return NULL;

	return HeapSize(g_hSndHeap, HEAP_NO_SERIALIZE, lptr);
}

#endif


#if 0

#include <windows.h>
#include "HeapManager.h"

typedef HANDLE HEAP_HANDLE;

static HEAP_HANDLE g_hSndHeap = NULL;
static DWORD g_dwHeapSemaphore = 0;
static DWORD g_dwAllocSemaphore = 0;

#ifndef WORD_ALIGN_LENGTH
#define WORD_ALIGN_LENGTH(a)	(((a)+3)&0xfffffffc)
#endif

HEAP_HANDLE SndHeapCreate()
{
	if (!g_hSndHeap) g_hSndHeap = HeapCreate(HEAP_NO_SERIALIZE, 131072, 0);	// 128KB
	g_dwHeapSemaphore++;

	return g_hSndHeap;
}

void SndHeapDestory()
{
	if (!g_hSndHeap) return;

	g_dwHeapSemaphore--;
	if (g_dwHeapSemaphore) return;

	HeapDestroy(g_hSndHeap);
	g_hSndHeap = NULL;
	g_dwAllocSemaphore = 0;
}

HEAP_HANDLE SndHeapHandle()
{
	return g_hSndHeap;
}

void* SndHeapAlloc(DWORD dwSize)
{
	if (!dwSize) return NULL;
	
	if (!g_hSndHeap)
	{
		if (!SndHeapCreate())	return NULL;
	}

	g_dwAllocSemaphore++;

	return HeapAlloc(g_hSndHeap, HEAP_NO_SERIALIZE, WORD_ALIGN_LENGTH(dwSize));
}

void* SndHeapRealloc(void* lptr, DWORD dwSize)
{
	if (!g_hSndHeap || !lptr) return SndHeapAlloc(dwSize);
	
	if (!dwSize)
	{
		SndHeapFree(lptr);//, TRUE);
		return NULL;
	}

	return HeapReAlloc(g_hSndHeap, HEAP_NO_SERIALIZE, lptr, WORD_ALIGN_LENGTH(dwSize));
}

void SndHeapFree(void* lptr)
{
	if (!g_hSndHeap || !lptr) return;
	
	HeapFree(g_hSndHeap, 0, lptr);
	g_dwAllocSemaphore--;

	if (g_dwAllocSemaphore)
	{
		//if (bCompact)
			SndHeapCompact();
	}
	else
	{
		SndHeapDestory();
	}
}

void SndHeapCompact()
{
#if !defined (_WIN32_WCE) || _WIN32_WCE > 420
	if (g_hSndHeap) HeapCompact(g_hSndHeap, HEAP_NO_SERIALIZE);
#endif
}

DWORD SndHeapSize(void* lptr)
{
	if (!g_hSndHeap || !lptr) return 0;

	return HeapSize(g_hSndHeap, HEAP_NO_SERIALIZE, lptr);
}

#endif



#include "SYSAPI.h"


void* SndHeapAlloc(atDWORD dwSize)
{
	return SYSAPI_AllocMem( dwSize );
}

void* SndHeapRealloc(void* lptr, unsigned long dwSize)
{
	return SYSAPI_ReallocMem( lptr, dwSize );
}

void SndHeapFree(void* lptr)
{
	SYSAPI_FreeMem( lptr );
}
