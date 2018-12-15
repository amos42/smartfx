#ifndef __APP_ENTRY_H_
#define __APP_ENTRY_H_


void APP_Entry_Init( void *hInst, void *hWnd, int width, int height );
void APP_Entry_Final(void);
void APP_Entry_Enter(void);
void APP_Entry_Exit(void);
void APP_Entry_Timer(int id);
void APP_Entry_Draw( void *hDC );
void APP_Entry_Debug(int param1, int param2);


#endif
