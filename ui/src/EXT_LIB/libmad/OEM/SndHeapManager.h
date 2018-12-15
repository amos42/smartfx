#ifndef __SNDHEAPMANAGER_H__
#define __SNDHEAPMANAGER_H__


#ifdef __cplusplus
extern "C" {
#endif


void* SndHeapAlloc(unsigned long dwSize);
void* SndHeapRealloc(void* lptr, unsigned long dwSize);
void  SndHeapFree(void* lptr);


#ifdef __cplusplus
}
#endif


#endif