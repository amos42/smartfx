#ifndef __bsp_adc_H
#define __bsp_adc_H

#ifdef __cplusplus
extern "C" {
#endif

#include "adc.h"
#include "threads.h"

#define D_ADC_SAMPLE_NO	5
extern uint16_t  adc_arr[D_ADC_CH_MAX][D_ADC_SAMPLE_NO];

HAL_StatusTypeDef bsp_adc_start(ADC_HandleTypeDef* hadc, uint32_t* pData, uint32_t Length);
HAL_StatusTypeDef bsp_adc_stop(ADC_HandleTypeDef* hadc);

#ifdef __cplusplus
}
#endif

#endif // __bsp_adc_H
