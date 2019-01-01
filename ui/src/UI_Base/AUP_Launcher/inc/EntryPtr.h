#ifndef __ENTRYPTR_H_
#define __ENTRYPTR_H_


#ifdef __cplusplus
extern "C" {
#endif


typedef int BOOL_T;


void   ENTRYPTR_Initialize(void);
void   ENTRYPTR_GetName( char *name );
BOOL_T ENTRYPTR_Create( void *inst, void *handle, int width, int height );
void   ENTRYPTR_Destroy(void);
void   ENTRYPTR_Enter(void);
void   ENTRYPTR_Exit(void);
void   ENTRYPTR_Draw(void);
void   ENTRYPTR_TickTimer(long millisec);
void   ENTRYPTR_Resize( int width, int height );
void   ENTRYPTR_ActivateDebug( BOOL_T bDebug );
int    ENTRYPTR_SysCommand( int code, int value );
int    ENTRYPTR_GetSystemVar( int code );
void   ENTRYPTR_TickMsgProcess(void);
void   ENTRYPTR_TickDispProcess(void);

#ifdef __cplusplus
}
#endif


#endif
