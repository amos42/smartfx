#ifndef __GLOBAL_H_
#define __GLOBAL_H_


#include "A_UIENG.h"

#define IDX_DIST_TABLE  71
#define USERKEY_1 (atVKEY_USERDEF + 100)

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


#define D_SIGNITURE	0xAABBCCDE

typedef struct _SETTINGDEVICE {
	atU32	sig;	// signiture   // 0xAABBCCDD
	atU16	nBright;	// 1-5,  // default : 1   
	atU16   nDist[71];	//   // default 0, 0, 750
	atU16	nBattery;	// Battery value;
	atBOOL	bPrintMsg;	// Print message at calibrate window
	atU16	nKindLang;	// kr:82  us:60  jn:81
	atU16   nChecksum;  // check sum.
} SETTINGDEVICE;

extern GLOBALSETTING    g_GlobalSetting;
extern GLOBALIMAGE		g_GlobalImages;  

    
    
void InitGlobalVar(void);
void FinalGlobalVar(void);
int EvaluateDistanceIndex(void);
int GetVolategeValue(void);
int GetDistanceValue(void);
int GetBatteryValue(void);
void LedControl(atBOOL b);
void SetBackLight(int val);

int GetDistanceValueTest(int idx);
void InitDefaultDeviceValues(void);
void SetDeviceValues(SETTINGDEVICE st);
SETTINGDEVICE *GetDeviceValues(void);

extern int APP_SCREEN_WIDTH;
extern int APP_SCREEN_HEIGHT;


extern int nDistDivid;
extern int nDistRecover;
extern int nDistPenDivid;
extern int nDistPenRecover;
extern int nDistLeft;
extern int nDistRight;
extern int nDistRead;
extern int nDistChart;
extern int nBettery;
extern atBOOL bPrintMsg;

extern atBOOL bScr1ResDist;
extern atBOOL bScr1ResPen;

extern SETTINGDEVICE defSetting;
    
//extern const int distTable_1[IDX_DIST_TABLE];
//extern const int distTable_2[IDX_DIST_TABLE];
//extern const int distTable_3[IDX_DIST_TABLE];

#define USERKEY_1 (atVKEY_USERDEF + 100)

// DEF Control
#define DEF_PEN_DIVID_LIMIT 12
#define DEF_PEN_RECOVER_LIMIT 18

#ifdef __cplusplus
};
#endif


#endif
