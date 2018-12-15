#include "threads.h"
#include "bsp_usart.h"


////////////////////////////////////////////////
// UART
////////////////////////////////////////////////

// Tx는 polling 방식으로 
// Rx는 interrupt 방식으로 처리


HAL_StatusTypeDef bsp_uart_tx(UART_HandleTypeDef *huart, uint8_t *pData, uint16_t Size)
{
//	return HAL_UART_Transmit(huart,pData, Size, 5000);
	return HAL_UART_Transmit_IT(huart, pData, Size);
}

HAL_StatusTypeDef bsp_uart_interrupt_enable(UART_HandleTypeDef *huart)
{
  uint32_t tmp_state = 0;
  
  tmp_state = huart->State;
  if((tmp_state == HAL_UART_STATE_READY) || (tmp_state == HAL_UART_STATE_BUSY_TX))
  {

    /* Process Locked */
    __HAL_LOCK(huart);

    huart->ErrorCode = HAL_UART_ERROR_NONE;
    /* Check if a transmit process is ongoing or not */
    if(huart->State == HAL_UART_STATE_BUSY_TX)
    {
      huart->State = HAL_UART_STATE_BUSY_TX_RX;
    }
    else
    {
      huart->State = HAL_UART_STATE_BUSY_RX;
    }

    /* Process Unlocked */
    __HAL_UNLOCK(huart);

    /* Enable the UART Parity Error Interrupt */
    __HAL_UART_ENABLE_IT(huart, UART_IT_PE);

    /* Enable the UART Error Interrupt: (Frame error, noise error, overrun error) */
    __HAL_UART_ENABLE_IT(huart, UART_IT_ERR);

    /* Enable the UART Data Register not empty Interrupt */
    __HAL_UART_ENABLE_IT(huart, UART_IT_RXNE);

    return HAL_OK;
  }
  else
  {
    return HAL_BUSY;
  }
}

static uint32_t bsp_usart_cbf_dummy(uint32_t p0, void *p1)
{
	return 0;
}

static BSP_CBF_TYPE USART6_cbfunc = bsp_usart_cbf_dummy;

void bsp_usart6_cbf_reg(BSP_CBF_TYPE cbf)
{
	USART6_cbfunc = cbf;
}

// interrutpt 처리 루틴
HAL_StatusTypeDef bsp_uart_rx_irq(UART_HandleTypeDef *huart)
{
	uint8_t rx;
	uint16_t uhMask = huart->Mask;
	
	rx = (uint8_t)(uint8_t)(huart->Instance->RDR & (uint8_t)uhMask); 

	if (huart == &huart1) 
	{
		putchar(rx);
	} else if (huart == &huart6) {
		USART6_cbfunc((uint32_t)rx, 0);
	}		
	return HAL_OK;
}

#ifdef __GNUC__
  /* With GCC/RAISONANCE, small printf (option LD Linker->Libraries->Small printf
     set to 'Yes') calls __io_putchar() */
  #define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
  #define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */


//-----------------------------------------]
// printf proto-type 정의
// 

/**
  * @brief  Retargets the C library printf function to the USART.
  * @param  None
  * @retval : None
  */
PUTCHAR_PROTOTYPE
{
	HAL_UART_Transmit(&huart1, (uint8_t *)&ch, 1, 100);

	return ch;
}

