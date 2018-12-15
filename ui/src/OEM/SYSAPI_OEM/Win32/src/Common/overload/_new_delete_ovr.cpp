// Overload global new/delete

#define __OVERLOAD_NEW_DELETE_


#ifdef __OVERLOAD_NEW_DELETE_

#include <cstdio>
#include <cstdlib>
#include "SYSAPI.h"
#include <cstddef> // Size_t
#include <new>


using namespace std;


#ifndef _WIN32_WCE
void *operator new( size_t size )
{
//	return malloc( size );
  	return SYSAPI_AllocMem(size);
}
#endif

void *operator new[]( size_t size )
{
//	return malloc( size );
  	return SYSAPI_AllocMem(size);
}


void *operator new[]( size_t size, char* s, int n )
{
//	return malloc( size );
  	return SYSAPI_AllocMem(size);
}


#ifndef _WIN32_WCE
void operator delete( void *p )
{
//	free( p );
	SYSAPI_FreeMem( p );
}
#endif

void operator delete[]( void *p )
{
//	free( p );
	SYSAPI_FreeMem( p );
}

#endif
