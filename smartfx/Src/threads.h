#ifndef __threads_H
#define __threads_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f7xx_hal.h"
#include "cmsis_os.h"

typedef uint32_t (*BSP_CBF_TYPE)(uint32_t, void *);	 

#define D_ADC_CH_MAX						4

typedef struct {
	osThreadId	thx_id[5];				// thread ID
	osTimerId		tim_id[2];		// timer ID			
	uint8_t t;
	uint8_t f_update;					// 키가 눌러 지거나 packet이 도착하면 자동 꺼지는 시간 초기화함.
	uint16_t off_time;					// 설정시 꺼지는 시간.
	uint16_t adc_val[D_ADC_CH_MAX];
	uint16_t touch_sts, touch_xp, touch_yp;
} APP_T;

extern APP_T gAppObj;


#define D_PARSER_THREAD						(gAppObj.thx_id[1]) 	
#define D_EVT_PKT_0									0x0001
#define D_EVT_PKT_1									0x0002
#define D_EVT_PKT_2									0x0004

#define D_EVENT_THREAD							(gAppObj.thx_id[2])
#define D_EVT_TOUCH								0x0001

#define D_BSP_TRUE									1
#define D_BSP_FALSE									0

void thread_start(void);
	 
#ifdef __cplusplus
}
#endif

#endif
