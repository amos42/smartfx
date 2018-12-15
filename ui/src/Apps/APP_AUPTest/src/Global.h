#ifndef __GLOBAL_H_
#define __GLOBAL_H_


#include "A_UIENG.h"


#ifdef __cplusplus
extern "C" {
#endif    
    

typedef struct _GLOBALSETTING {
    atBOOL      bCaption;
	atINT       nOSDTrans;
    atINT       nTheme;
	atINT       nBright;
	atINT       nContrast;
} GLOBALSETTING;


typedef struct _GLOBALIMAGE {
    atHBITMAP  hDummy;    
} GLOBALIMAGE;


extern GLOBALSETTING    g_GlobalSetting;
extern GLOBALIMAGE		g_GlobalImages;  
    
    
void InitGlobalVar(void);
void FinalGlobalVar(void);
    
    
#ifdef __cplusplus
};
#endif


#endif
