#include "bsp_adc.h"

HAL_StatusTypeDef bsp_adc_start(ADC_HandleTypeDef* hadc, uint32_t* pData, uint32_t Length)
{
	return HAL_ADC_Start_DMA(hadc, pData, Length);
}

HAL_StatusTypeDef bsp_adc_stop(ADC_HandleTypeDef* hadc)
{
	return HAL_ADC_Stop_DMA(hadc);
}

uint16_t bsp_adc_get(uint8_t ch)
{
	uint8_t i;
	uint32_t adc_avg = 0;
	
	if (ch > D_ADC_CH_MAX) return 0;
	
//	for(i = 0; i < D_ADC_SAMPLE_NO; i++) {
//		printf("ch:%2d:%5d\r\b\n", ch, adc_arr[ch][i]);
//		adc_avg += (uint32_t)adc_arr[ch][i];
//	}
//	
	return (uint16_t)gAppObj.adc_val[ch];//(adc_avg/D_ADC_SAMPLE_NO);
}

uint16_t  adc_arr[D_ADC_CH_MAX][D_ADC_SAMPLE_NO];

static uint8_t adc_arr_no = 0;
	
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc)
{

	
	adc_arr[0][adc_arr_no] = gAppObj.adc_val[0];
	adc_arr[1][adc_arr_no] = gAppObj.adc_val[1];
	adc_arr[2][adc_arr_no] = gAppObj.adc_val[2];
	adc_arr[3][adc_arr_no] = gAppObj.adc_val[3];
	
	adc_arr_no++;
	if (adc_arr_no > D_ADC_SAMPLE_NO) adc_arr_no = 0;
	
}
