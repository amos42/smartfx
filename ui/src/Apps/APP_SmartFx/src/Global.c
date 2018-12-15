#include "Global.h"



GLOBALSETTING    g_GlobalSetting;
GLOBALIMAGE		g_GlobalImages; 

int APP_SCREEN_WIDTH = 320;
int APP_SCREEN_HEIGHT = 480;

int nDistDivid;
int nDistRecover;
int nDistPenDivid;
int nDistPenRecover;
int nDistLeft;
int nDistRight;
int nDistRead;
int nDistChart;
int nBettery;
atBOOL bPrintMsg;
atBOOL bScr1ResDist;
atBOOL bScr1ResPen;

SETTINGDEVICE defSetting;

/*
// 800
const int distTable_1[IDX_DIST_TABLE] = {
	2463, 2435, 2306, 2151, 2014, 1913, 1808, 1725, 1645, 1578, 
	1515, 1452, 1411, 1347, 1306, 1264, 1215, 1193, 1152, 1130, 
	1089, 1066, 1044, 1001,  979,  959,  937,  910,  895,  894,
	 875,  850,  829,  828,  828,  806,
};

// 750
const int distTable_2[IDX_DIST_TABLE] = {
	2454, 2380, 2205, 2086, 1938, 1819, 1716, 1644, 1560, 1498, 
	1434, 1368, 1304, 1260, 1218, 1175, 1133, 1089, 1068, 1046,
	1020,  998,  976,  954,  933,  866,  845,  822,  800,  779,
	 779,  757,  735,  735,  735,  691, 
};

// 700
const int distTable_3[IDX_DIST_TABLE] = {
	2449, 2445, 2345, 2187, 2065, 1943, 1838, 1734, 1650, 1568, 
	1517, 1455, 1389, 1346, 1283, 1240, 1193, 1149, 1127, 1084,
	1062, 1019,  998,  975,  954,  931,  909,  887,  866,  841,
	 818,  795,  795,  773,  751,  751,
};
*/

static void LoadGlobalImages(void)
{
}

static void LoadGlobalSettings(void)
{
}


void InitGlobalVar(void)
{
    g_GlobalSetting.bCaption = atTRUE;
	g_GlobalSetting.nOSDTrans = 3;   

	g_GlobalSetting.nTheme = 0;
	g_GlobalSetting.nBright = 5;
	g_GlobalSetting.nContrast = 5;
}

void FinalGlobalVar(void)
{
}

int EvaluateDistanceIndex(void)
{
	if (defSetting.nDist[2] > 800)
		return 1;
	
	if (defSetting.nDist[2] > 750)
		return 2;

	return 3;
}

void InitDefaultDeviceValues(void)
{
	SYSAPI_SysCommand(400, 3, 0);
}

void SetDeviceValues(SETTINGDEVICE st)
{
	SYSAPI_SysCommand(400, 0, (atLONG)&st);
}

SETTINGDEVICE *GetDeviceValues(void)
{
	return (SETTINGDEVICE *)SYSAPI_SysCommand(400, 1, 0);
}

int GetVolategeValue(void)
{
	return SYSAPI_SysCommand(100, 1, 0);
}

int GetDistanceValue(void)
{
	atINT value = SYSAPI_SysCommand(100, 1, 0);

	if (value <= defSetting.nDist[IDX_DIST_TABLE - 1])
		return 80;

	for (int i = 1; i<IDX_DIST_TABLE; i++) {
		if ((value < defSetting.nDist[i - 1]) && (value >= defSetting.nDist[i])) {
			return i + 9;
		}
	}

	/*atINT idx = EvaluateDistanceIndex();

	switch (idx) {
		case 1:
		{
			if (value <= distTable_1[IDX_DIST_TABLE - 1])
				return 45;

			for (int i = 1; i<IDX_DIST_TABLE; i++) {
				if ((value < distTable_1[i - 1]) && (value >= distTable_1[i])) {
					return i + 9;
				}
			}
		}
			break;

		case 2:
		{
			if (value <= distTable_2[IDX_DIST_TABLE - 1])
				return 45;

			for (int i = 1; i<IDX_DIST_TABLE; i++) {
				if ((value < distTable_2[i - 1]) && (value >= distTable_2[i])) {
					return i + 9;
				}
			}
		}
			break;

		case 3:
		{
			if (value <= distTable_3[IDX_DIST_TABLE - 1])
				return 45;

			for (int i = 1; i<IDX_DIST_TABLE; i++) {
				if ((value < distTable_3[i - 1]) && (value >= distTable_3[i])) {
					return i + 9;
				}
			}
		}
			break;
	}*/

	return 10;
}

int GetBatteryValue(void)
{
	return SYSAPI_SysCommand(100, 0, 0);;
}

void LedControl(atBOOL b)
{
	if(b)
		SYSAPI_SysCommand(300, 1, 0);
	else
		SYSAPI_SysCommand(300, 0, 0);
}

void SetBackLight(int val)
{
	SYSAPI_SysCommand(350, val, 0);
}


int GetDistanceValueTest(int idx)
{
	atINT value = SYSAPI_SysCommand(100, 1, 0);

	atU16 x = 1720;

	atDOUBLE d4 = 0.0;
	atDOUBLE d3 = 0.0;
	atDOUBLE d2 = 0.0;
	atDOUBLE d1 = 0.0;
	atDOUBLE d0 = 0.0;

	atU32 ltemp = 0;

	switch (idx) {
	case 1: // 800
	{
		d4 = 0.00000000001 * x;
		d4 *= x;
		d4 *= x;
		d4 *= x;

		d3 = -0.00000008 * x;
		d3 *= x;
		d3 *= x;

		d2 = 0.00002 * x;
		d2 *= x;

		d1 = -0.0685 * x;
		d0 = 87.316;

		d2 += (d1 + d0);
	}
	break;

	case 2: // 750
	{
		d4 = (6 / (pow(10, 12))) * pow(x, 4.0);
		d3 = -1 * (5 / (pow(10.0, 8.0))) * pow(x, 3.0);
		d2 = 0.00002 * pow(x, 2.0);

	}
	break;

	case 3: // 700
	{
		d4 = (4 / (pow(10.0, 12.0))) * pow(x, 4.0);
		d3 = -1 * (4 / (pow(10.0, 8.0))) * pow(x, 3.0);
		d2 = 0.0001 * pow(x, 2.0);
		d1 = -1 * 0.1711 * x;
		d0 = 120.16;
	}
	break;
	}

	return (int)d4;
}
