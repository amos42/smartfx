#ifndef __bsp_uart_H
#define __bsp_uart_H

#ifdef __cplusplus
extern "C" {
#endif

#include "usart.h"
#include "threads.h"

HAL_StatusTypeDef bsp_uart_tx(UART_HandleTypeDef *huart, uint8_t *pData, uint16_t Size);
HAL_StatusTypeDef bsp_uart_interrupt_enable(UART_HandleTypeDef *huart);

void bsp_usart6_cbf_reg(BSP_CBF_TYPE cbf);	

#ifdef __cplusplus
}
#endif

#endif
