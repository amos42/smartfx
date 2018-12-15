#include "bsp_gpio.h"

void bsp_led_grn_on(void)
{
	HAL_GPIO_WritePin(D_LED_GRN_GPIO_Port, D_LED_GRN_Pin, GPIO_PIN_SET); 
}

void bsp_led_grn_off(void)
{
	HAL_GPIO_WritePin(D_LED_GRN_GPIO_Port, D_LED_GRN_Pin, GPIO_PIN_RESET); 
}

void bsp_led_grn_toggle(void)
{
		HAL_GPIO_TogglePin(D_LED_GRN_GPIO_Port, D_LED_GRN_Pin); 
}

void bsp_led_blue_on(void)
{
	HAL_GPIO_WritePin(D_LED_BLUE_GPIO_Port, D_LED_BLUE_Pin, GPIO_PIN_SET); 
}

void bsp_led_blue_off(void)
{
	HAL_GPIO_WritePin(D_LED_BLUE_GPIO_Port, D_LED_BLUE_Pin, GPIO_PIN_RESET); 
}

void bsp_led_blue_toggle(void)
{
		HAL_GPIO_TogglePin(D_LED_BLUE_GPIO_Port, D_LED_BLUE_Pin); 
}

void bsp_pwr_hold_on(void)
{
	HAL_GPIO_WritePin(D_PWR_HOLD_GPIO_Port, D_PWR_HOLD_Pin, GPIO_PIN_RESET); 
}

void bsp_pwr_hold_off(void)
{
	HAL_GPIO_WritePin(D_PWR_HOLD_GPIO_Port, D_PWR_HOLD_Pin, GPIO_PIN_RESET); 
}

void bsp_led_nor_on(void)
{
	HAL_GPIO_WritePin(D_LED_NOR_GPIO_Port, D_LED_NOR_Pin, GPIO_PIN_SET); 
}

void bsp_led_nor_off(void)
{
	HAL_GPIO_WritePin(D_LED_NOR_GPIO_Port, D_LED_NOR_Pin, GPIO_PIN_RESET); 
}

void bsp_led_nor_toggle(void)
{
		HAL_GPIO_TogglePin(D_LED_NOR_GPIO_Port, D_LED_NOR_Pin); 
}
