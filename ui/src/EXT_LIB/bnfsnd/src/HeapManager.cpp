#include <windows.h>
#include "HeapManager.h"

static HANDLE g_hSndHeap = NULL;
static DWORD g_dwHeapSemaphore = NULL;
static DWORD g_dwAllocSemaphore = NULL;

#define WORD_ALIGN_LENGTH(a)	(((a)+3)&0xfffffffc)

HANDLE SndHeapCreate()
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

HANDLE SndHeapHandle()
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

	LPVOID pHeap = HeapAlloc(g_hSndHeap, HEAP_ZERO_MEMORY, WORD_ALIGN_LENGTH(dwSize));
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

	LPVOID pHeap = HeapReAlloc(g_hSndHeap, NULL, lptr, WORD_ALIGN_LENGTH(dwSize));

	return pHeap;
}

void SndHeapFree(LPVOID lptr)
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
#if !defined (_WIN32_WCE) || _WIN32_WCE > 420
	if (g_hSndHeap) HeapCompact(g_hSndHeap, NULL);
#endif
}

DWORD SndHeapSize(LPVOID lptr)
{
	if (!g_hSndHeap || !lptr) return NULL;

	return HeapSize(g_hSndHeap, NULL, lptr);
}
