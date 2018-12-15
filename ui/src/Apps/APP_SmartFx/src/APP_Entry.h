#ifndef __APP_ENTRY_H_
#define __APP_ENTRY_H_


#include "A_UIENG.h"


void APP_Entry_Init( void *hInst, void *hWnd, void *pHeap, long heap_size, int width, int height, void *pScreenBuffer, void *pResPtr, long nResSize );
void APP_Entry_Final(void);
void APP_Entry_Enter(void);
void APP_Entry_Exit(void);
void APP_Entry_Draw(void);
void APP_Entry_Debug(int param1, int param2);


#endif
