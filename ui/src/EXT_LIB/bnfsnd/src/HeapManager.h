#ifndef __HEAP_MANAGER_H__
#define __HEAP_MANAGER_H__

#if 1

HANDLE SndHeapCreate();
void SndHeapDestory();
HANDLE SndHeapHandle();

LPVOID SndHeapAlloc(DWORD dwSize);
LPVOID SndHeapRealloc(LPVOID lptr, DWORD dwSize);
void SndHeapFree(LPVOID lptr);
void SndHeapCompact();
DWORD SndHeapSize(LPVOID lptr);

#else

extern HANDLE BMHeapCreate(DWORD dwHeapIndex);
extern void BMHeapDestory(DWORD dwHeapIndex);
extern HANDLE BMHeapHandle(DWORD dwHeapIndex);

extern LPVOID BMHeapAlloc(DWORD dwSize, DWORD dwHeapIndex);
extern LPVOID BMHeapRealloc(LPVOID lptr, DWORD dwSize, DWORD dwHeapIndex);
extern void BMHeapFree(LPVOID lptr, BOOL bCompact, DWORD dwHeapIndex);
extern void BMHeapCompact(DWORD dwHeapIndex);
extern DWORD BMHeapSize(LPVOID lptr, DWORD dwHeapIndex);

#define SndHeapCreate()					BMHeapCreate(NULL)
#define SndHeapDestory()				BMHeapDestory(NULL)
#define SndHeapHandle()					BMHeapHandle(NULL)

#define SndHeapAlloc(dwSize)			BMHeapAlloc(dwSize, NULL)
#define SndHeapRealloc(lptr, dwSize)	BMHeapRealloc(lptr, dwSize, NULL)
#define SndHeapFree(lptr)				BMHeapFree(lptr, TRUE, NULL)
#define SndHeapClear(lptr)				BMHeapFree(lptr, FALSE, NULL)
#define SndHeapCompact()				BMHeapCompact(NULL)
#define SndHeapSize(lptr)				BMHeapSize(lptr, NULL)

#endif

#endif