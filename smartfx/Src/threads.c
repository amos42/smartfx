#include "threads.h"
#include "bsp_usart.h"
#include "bsp_gpio.h"
#include "bsp_ble.h"
#include "bsp_key.h"
#include "bsp_sdram.h"
#include "bsp_quadspi.h"
#include "bsp_lcd.h"
#include "bsp_touch.h"
#include "bsp_adc.h"
#include "bsp_flash.h"
#include "bsp_dma2d.h"
#include "pkt.h"
#include "A_UIENG.h"

#if 1
void thread_0(void const *argument);
void thread_1(void const *argument);
void thread_2(void const *argument);

osThreadDef (thread_0, osPriorityNormal, 1, 0);
osThreadDef (thread_1, osPriorityNormal, 1, 0);
osThreadDef (thread_2, osPriorityNormal, 1, 0);

void timer_0(void const *argument);

osTimerDef (tim0, timer_0); 

APP_T gAppObj;


#define D_LOOP_MS										(200)											// 200 ms
#define POWER_OFF_TIME_MAN				(1000/D_LOOP_MS)			// 2s, manually off	

#define D_PWROFF_TIME_DEAULT			(60*10) 										// 기본단위 1s이고 기본은 10분

#define D__AUTO_OFF

extern void _keycall(int keycode, int status);

void thread_0(void const *argument) 
{
	int tcnt;
	uint8_t key_val;
	uint32_t key_cnt;
	volatile uint8_t g_state = 0, g_option = 0;	
	int8_t ble_check;

	for (;;) 
	{
		osDelay(D_LOOP_MS);	// 200ms

///////////////////////////////////////////////////////////////////		
		ble_check = ble_connect_check(); // 

		if (ble_check == 1) 
		{
			gBleObj.connected = 1;
			LED_BLE_ON();
			LED_IND_OFF();
		} else if (ble_check == 0) {
			gBleObj.connected = 0;
			LED_BLE_OFF();
			//LED_IND_ON();
		}
		
		if (gBleObj.connected == 0) {
			static uint8_t x = 3;
			
			x--; 
			if (x == 0) { LED_IND_TOG(); /*LED_NOR_TOG(); */x = 3; }
		} 



//////////////////////////////////////////////////////////////////////		
		
		key_val = key_pwr_key_get(); //key_st_get();
		key_cnt = key_pwr_key_count(); //key_st_count();
		
		//printf("kv:%d\r\n", key_val);

		if (g_state == 3) { // 제일 많이 실행하는 루틴을 앞으로...
			if (key_val == KEY_PUSH) {
				//printf("key push\r\n");
			} else if (key_val == KEY_RELEASE) {
				//printf("key released\r\n");

#ifdef D__AUTO_OFF
				gAppObj.f_update = 1;
#endif
			} else if (key_val == KEY_NONE)  {
#ifdef D__AUTO_OFF
				if (gAppObj.off_time == 0) {
					g_state = 4;
					g_option = 2;
				}
#endif
			} else if (key_val == KEY_HOLD && key_cnt > POWER_OFF_TIME_MAN) {						 
				g_state = 4;  	// 종료 state로 전환.
				g_option = 1;	// manual power off
			}
		} else if (g_state == 4) {
			if (g_option ==  1) { // manual power off.
				//printf("release the power key\r\n");
				LED_BLE_OFF();

				while (1) {
					LED_IND_ON();	
//					bsp_led_nor_on();
					osDelay(250);
					LED_IND_OFF();
//					bsp_led_nor_off();
					osDelay(250);
					
				
	
					key_val = key_pwr_key_get(); //key_st_get();
					key_cnt = key_pwr_key_count(); //key_st_count();
					if (key_val == KEY_RELEASE) {
						//printf("manual power off\r\n");
	
						PWR_HOLD_OFF();
						osDelay(4000);
						break;
					}
				}
			} else if (g_option == 2) {
				for (tcnt = 0; tcnt < 5; tcnt++) {
					LED_IND_ON();
					osDelay(250);						
					LED_IND_OFF();
					osDelay(250);
				}

				PWR_HOLD_OFF();
				osDelay(1000);
			}
			
			g_state = 2; 
		} else if (g_state == 0) {
//			printf("battery check\r\n");

			g_state = 1;
		} else if (g_state == 1) {
			g_state = 2;
		} else if (g_state == 2)	{
			g_state = 3;	
			LED_IND_ON();
		}
	}				
}

void thread_1(void const *argument) 
{
//	int i;
	osEvent evt;

	for (;;) 
	{
		evt = osSignalWait (0, 1000);
		
		if (evt.status == osEventSignal)  {
		//	printf("t1:evt.value.signals = 0x%x\r\n", evt.value.signals);
		
#ifdef D__AUTO_OFF
			gAppObj.f_update = 1;
#endif		
			
			if (evt.value.signals & D_EVT_PKT_0) {
				osSignalClear (D_PARSER_THREAD, D_EVT_PKT_0);
				printf("event 0\r\n");
			}
			
			if (evt.value.signals & D_EVT_PKT_1) {
				osSignalClear (D_PARSER_THREAD, D_EVT_PKT_1);
				printf("event 1\r\n");
			}
			
			if (evt.value.signals & D_EVT_PKT_2) {
				osSignalClear (D_PARSER_THREAD, D_EVT_PKT_2);
				printf("event 2\r\n");
			}
		}
  }	
}


extern void _touchcall(int up_down, int x, int y);

void thread_2(void const *argument) 
{
	osEvent evt;
	uint8_t key_st, key_re;

	for (;;) 
	{
		evt = osSignalWait (0, 150);
		
		if (evt.status == osEventSignal)  {
//			printf("t1:evt.value.signals = 0x%x\r\n", evt.value.signals);
			
			if (evt.value.signals & D_EVT_TOUCH) {
				osSignalClear (D_EVENT_THREAD, D_EVT_TOUCH);
				_touchcall(gAppObj.touch_sts, gAppObj.touch_xp, gAppObj.touch_yp);
#ifdef D__AUTO_OFF
				gAppObj.f_update = 1;
#endif				
				//printf("*******%d, %d,%d\r\n", gAppObj.touch_sts, gAppObj.touch_xp, gAppObj.touch_yp);
			}
			
//			if (evt.value.signals & 0x0002) {
//				osSignalClear (D_EVENT_THREAD, 0x0002);
//				printf("key_st_push\r\n");
//			}

//			if (evt.value.signals & 0x0004) {
//				osSignalClear (D_EVENT_THREAD, 0x0004);
//				printf("key_st_release\r\n");
//			}

//			if (evt.value.signals & 0x0008) {
//				osSignalClear (D_EVENT_THREAD, 0x0008);
//				printf("key_re_release\r\n");
//			}

//			if (evt.value.signals & 0x0010) {
//				osSignalClear (D_EVENT_THREAD, 0x0010);
//				printf("key_re_release\r\n");
//			}
		} else {
			key_st = key_st_get();
			key_re = key_re_get();

			if (key_st == KEY_PUSH) {
				_keycall(0, 0); //osSignalSet(D_EVENT_THREAD, 0x0002);
#ifdef D__AUTO_OFF
				gAppObj.f_update = 1;
#endif				
			} else if (key_st == KEY_RELEASE)	{
				_keycall(0, 1); //osSignalSet(D_EVENT_THREAD, 0x0004);
#ifdef D__AUTO_OFF
				gAppObj.f_update = 1;
#endif
			}
			
			if (key_re == KEY_PUSH) {
				_keycall(1, 0); //osSignalSet(D_EVENT_THREAD, 0x0008);
#ifdef D__AUTO_OFF
				gAppObj.f_update = 1;
#endif
			} else if (key_re == KEY_RELEASE)	{
				_keycall(1, 1); //osSignalSet(D_EVENT_THREAD, 0x0010);
#ifdef D__AUTO_OFF
				gAppObj.f_update = 1;
#endif
			}
		}
	}	
}


void timer_0(void const *arqument)
{
	static uint8_t cnt = 0;
	//printf("cnt=%d\r\n", cnt++);
}
#endif



void lcd_pattern_test(void)
{
#define BASE						(0xc0000000)

#define D_C_RED				(0x001f	<<	11)
#define D_C_GRN				(0x003f	<<	5)
#define D_C_BLU				(0x001f	<<	0)

	volatile int32_t x = 0;
	static volatile uint8_t idx = 0;
	uint16_t color[3];
	uint16_t r=0, g=0, b=0;
			
	if (idx == 0) {
		color[0] = D_C_RED; color[1] = D_C_GRN; color[2] = D_C_BLU + D_C_GRN; idx++;
	} else if (idx == 1) {
		color[0] = D_C_GRN; color[1] = D_C_BLU; color[2] = D_C_BLU + D_C_RED; idx++;
	} else if (idx == 2) {
		color[0] = D_C_BLU; color[1] = D_C_RED; color[2] = D_C_GRN + D_C_RED; idx=0;
	} 	
	
	x = 0;
	do {
		*(volatile uint16_t *)(BASE+x*2) = color[0]; x++;  
	} while (x < (320*100)); 

	do 
	{
		*(volatile uint16_t *)(BASE+x*2) = color[1]; x++;  
	} while (x < (320*200)); 

	do  
	{
		*(volatile uint16_t *)(BASE+x*2) = color[2]; x++;  
	}  while (x < (320*300));
	
	do  
	{
		*(volatile uint16_t *)(BASE+x*2) = color[0]; x++;  
	}  while (x < (320*400));			

	do  
	{
		*(volatile uint16_t *)(BASE+x*2) = color[1]; x++;  
	}  while (x < (320*480));			
}			


#include "ui_entryptr/entryptr.h"


//typedef struct {
//	uint16_t brBackLight;				// LCD Back Light 밝기
//	uint16_t distTable[10];
//	uint16_t cs;								// CheckSum 자기 자신 제외
//} FLASH_USER_DATA_T;

//FLASH_USER_DATA_T usrDataObj = { 5, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 0x55AA };


#include "A_Dalloc.h"
#include "A_RES.h"
#include "global.h"

const SETTINGDEVICE cSetDev = {
	D_SIGNITURE,			// signiture
	3, 												// back light brightness
	2454, 2380, 2205, 2086, 1938, 1819, 1716, 1644, 1560, 1498, 
	1434, 1368, 1304, 1260, 1218, 1175, 1133, 1089, 1068, 1046,
	1020,  998,  976,  954,  933,  866,  845,  822,  800,  779,
	779,  757,  735,  735,  735,  691,  691,  691,  691,  691,
	675,  675,  675,  675,  675,  675,  675,  675,  675,  675,
	665,  665,  665,  665,  665,  665,  665,  665,  665,  665,
	659,  659,  659,  659,  659,  659,  659,  659,  659,  659, 659,
	2080, // 3.4 Voltage value
	0,
	82,
	0 
};


SETTINGDEVICE gSetDev;
/*= {
	D_SIGNITURE,			// signiture
	3, 												// back light brightness
	2454, 2380, 2205, 2086, 1938, 1819, 1716, 1644, 1560, 1498, 
	1434, 1368, 1304, 1260, 1218, 1175, 1133, 1089, 1068, 1046,
	1020,  998,  976,  954,  933,  866,  845,  822,  800,  779,
	779,  757,  735,  735,  735,  691,  691,  691,  691,  691,
	675,  675,  675,  675,  675,  675,  675,  675,  675,  675,
	665,  665,  665,  665,  665,  665,  665,  665,  665,  665,
	659,  659,  659,  659,  659,  659,  659,  659,  659,  659, 659,
	2080, // 3.4 Voltage value
	0,
	82,
	0 
};
*/
void thread_start(void)
{
#define D_TIME_1SEC			10	
	uint8_t time = D_TIME_1SEC;
	uint8_t i=0;
	
 	bsp_sdram_test();
	
	memcpy(&gSetDev, &cSetDev, sizeof(SETTINGDEVICE));

	bsp_uart_interrupt_enable(&huart1);
	bsp_uart_interrupt_enable(&huart6);

	osKernelInitialize ();                    // initialize CMSIS-RTOS
	
	// create threads
	gAppObj.thx_id[0] = osThreadCreate(osThread(thread_0), NULL);
	gAppObj.thx_id[1] = osThreadCreate(osThread(thread_1), NULL);
	gAppObj.thx_id[2] = osThreadCreate(osThread(thread_2), NULL);
	
	gAppObj.f_update = 1;
	gAppObj.off_time = D_PWROFF_TIME_DEAULT;
	//gAppObj.tim_id[0] =  osTimerCreate (osTimer(tim0), osTimerPeriodic, NULL);

	printf("system start!\r\n");

	osKernelStart ();                         // start thread execution

	//osTimerStart (gAppObj.tim_id[0], 200);    // 200ms

	//osThreadTerminate (osThreadGetId ());     // terminate main thread
	
	LED_IND_OFF(); 
	LED_BLE_OFF(); 
	
	//osDelay(1000);
	
	//bsp_quadspi_test();
	BSP_QSPI_MemoryMappedMode();	

	osDelay(10);
	
	memset((void *)0xc0000000, 0, 8*1024*1024);
	
	{
	// internal flash memory test for user data.
	// sector 7:
		//bsp_flash_user_data_write((uint8_t *)&usrDataObj, sizeof(FLASH_USER_DATA_T));
		
		if (bsp_flash_user_data_valid() == 0) { // 한번이라도 writing 되었는가?
			bsp_flash_user_data_read((uint8_t *)&gSetDev, sizeof(SETTINGDEVICE));
		}
	}
	
	{
		//printf("SC620 BackLight Test... & LCD Reset\r\n");
		//lcd_pattern_test();
		UB_Graphic2D_CopyImgDMA2((unsigned short *)0x90000000,0,0, 320, 480);
		
	}

	bsp_ts_rst_enable();
	osDelay(2);
	bsp_ts_rst_disable();	
	osDelay(2);	
	UB_Touch_Init();	
	//UB_Touch_DisableIT();
	UB_Touch_EnableIT();

//	osDelay(200);
	bsp_bl_set(gSetDev.nBright);
	
	ble_adv_on_n_low_latency();
	bsp_adc_start(&hadc1, (uint32_t *)gAppObj.adc_val, D_ADC_CH_MAX);

//	atUIAPI_INIT_DATA x;
//	memset((void *)&x, 0, sizeof(atUIAPI_INIT_DATA));
//	atUIAPI_Initialize( &x );

	ENTRYPTR_Create(NULL, NULL, 320, 480);
	ENTRYPTR_Enter();
	ENTRYPTR_Resize(320, 480);
	
	for (;;)  
	{
		osDelay(100); // 100ms //loop time 바꾸지 말것
		
			
//		if (time % 2 == 0) 			printf("adc=%d,%d,%d,%4d\r\n",gAppObj.adc_val[0], gAppObj.adc_val[1], gAppObj.adc_val[2], gAppObj.adc_val[3]);
//		if (time % 2 == 0) 			printf("adc=%d,%d\r\n", gAppObj.adc_val[1], bsp_adc_get(1));


		if (time > 0) time--;

		if (time == 0) 
		{
		
			//lcd_pattern_test();
		
			time = D_TIME_1SEC;
			//VREFINT Internal reference voltage / 40 °C < TA < +85 °C/ min: 1.16, typical: 1.2, max: 1.24(1)
			//printf("Hello, %d\r\n", i++);
		
			if (gAppObj.f_update == 1) {
				gAppObj.f_update = 0;
				gAppObj.off_time = D_PWROFF_TIME_DEAULT;
			}
			
#ifdef D__AUTO_OFF		
			if (gAppObj.off_time > 0) gAppObj.off_time--;
#endif		
		}
	}	
}

