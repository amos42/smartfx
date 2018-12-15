#include "bsp_sdram.h"
#include "cmsis_os.h"
#include <string.h>

#if 1
/* Exported types ------------------------------------------------------------*/
typedef enum {PASSED = 0, FAILED = !PASSED} TestStatus_t;
/* Exported constants --------------------------------------------------------*/
#define SDRAM_BANK_ADDR                 ((uint32_t)0xc0000000)

/* #define SDRAM_MEMORY_WIDTH            FMC_SDRAM_MEM_BUS_WIDTH_8  */
#define SDRAM_MEMORY_WIDTH               FMC_SDRAM_MEM_BUS_WIDTH_16

#define SDCLOCK_PERIOD                   FMC_SDRAM_CLOCK_PERIOD_2
/* #define SDCLOCK_PERIOD                FMC_SDRAM_CLOCK_PERIOD_3 */

#define SDRAM_TIMEOUT     ((uint32_t)0xFFFF)

#define SDRAM_MODEREG_BURST_LENGTH_1             									((uint16_t)0x0000)
#define SDRAM_MODEREG_BURST_LENGTH_2             									((uint16_t)0x0001)
#define SDRAM_MODEREG_BURST_LENGTH_4             									((uint16_t)0x0002)
#define SDRAM_MODEREG_BURST_LENGTH_8             									((uint16_t)0x0004)
#define SDRAM_MODEREG_BURST_TYPE_SEQUENTIAL     							((uint16_t)0x0000)
#define SDRAM_MODEREG_BURST_TYPE_INTERLEAVED     						((uint16_t)0x0008)
#define SDRAM_MODEREG_CAS_LATENCY_2              									((uint16_t)0x0020)
#define SDRAM_MODEREG_CAS_LATENCY_3              									((uint16_t)0x0030)
#define SDRAM_MODEREG_OPERATING_MODE_STANDARD    					((uint16_t)0x0000)
#define SDRAM_MODEREG_WRITEBURST_MODE_PROGRAMMED 			((uint16_t)0x0000)
#define SDRAM_MODEREG_WRITEBURST_MODE_SINGLE     						((uint16_t)0x0200)

static void Fill_Buffer(uint32_t *pBuffer, uint32_t uwBufferLenght, uint32_t uwOffset);

FMC_SDRAM_CommandTypeDef command;

void bsp_sdram_init_seq(SDRAM_HandleTypeDef *hsdram, FMC_SDRAM_CommandTypeDef *Command)
{
  __IO uint32_t tmpmrd =0;
  /* Step 3:  Configure a clock configuration enable command */
  Command->CommandMode = FMC_SDRAM_CMD_CLK_ENABLE;
  Command->CommandTarget = FMC_SDRAM_CMD_TARGET_BANK1;
  Command->AutoRefreshNumber = 1;
  Command->ModeRegisterDefinition = 0;

  /* Send the command */
  HAL_SDRAM_SendCommand(hsdram, Command, SDRAM_TIMEOUT);

  /* Step 4: Insert 100 us minimum delay */
  /* Inserted delay is equal to 1 ms due to systick time base unit (ms) */
  HAL_Delay(1);

  /* Step 5: Configure a PALL (precharge all) command */
  Command->CommandMode = FMC_SDRAM_CMD_PALL;
  Command->CommandTarget = FMC_SDRAM_CMD_TARGET_BANK1;
  Command->AutoRefreshNumber = 1;
  Command->ModeRegisterDefinition = 0;

  /* Send the command */
  HAL_SDRAM_SendCommand(hsdram, Command, SDRAM_TIMEOUT);

  /* Step 6 : Configure a Auto-Refresh command */
  Command->CommandMode = FMC_SDRAM_CMD_AUTOREFRESH_MODE;
  Command->CommandTarget = FMC_SDRAM_CMD_TARGET_BANK1;
  Command->AutoRefreshNumber = 8;
  Command->ModeRegisterDefinition = 0;

  /* Send the command */
  HAL_SDRAM_SendCommand(hsdram, Command, SDRAM_TIMEOUT);

  /* Step 7: Program the external memory mode register */
  tmpmrd = (uint32_t)SDRAM_MODEREG_BURST_LENGTH_1          |
                     SDRAM_MODEREG_BURST_TYPE_SEQUENTIAL   |
                     SDRAM_MODEREG_CAS_LATENCY_2          |
                     SDRAM_MODEREG_OPERATING_MODE_STANDARD |
                     SDRAM_MODEREG_WRITEBURST_MODE_SINGLE;

  Command->CommandMode = FMC_SDRAM_CMD_LOAD_MODE;
  Command->CommandTarget = FMC_SDRAM_CMD_TARGET_BANK1;
  Command->AutoRefreshNumber = 1;
  Command->ModeRegisterDefinition = tmpmrd;

  /* Send the command */
  HAL_SDRAM_SendCommand(hsdram, Command, SDRAM_TIMEOUT);

  /* Step 8: Set the refresh rate counter */
  /* (15.62 us x Freq) - 20 */
  /* Set the device refresh counter */
  hsdram->Instance->SDRTR |= (uint32_t)2604; //jrlee @168MHz (uint32_t)3029; //jrlee @ 192MHz // ((uint32_t)(1476);//jrlee @96MHz

}


/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define BUFFER_SIZE         ((uint32_t)0x0100)
#define WRITE_READ_ADDR     ((uint32_t)0x50000)

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
//FMC_SDRAM_TimingTypeDef  SDRAM_Timing;


/* Read/Write Buffers */
uint32_t aTxBuffer[BUFFER_SIZE];
uint32_t aRxBuffer[BUFFER_SIZE];

/* Status variables */
__IO uint32_t uwWriteReadStatus = 0;

/* Counter index */
uint32_t uwIndex = 0;

void bsp_sdram_test(void)
{	
	uint8_t retry = 3;
	uint32_t x = 0;
	
	do 
	{
	
		bsp_sdram_init_seq(&hsdram1, &command);
		retry--;
	} 
	while ((retry != 0));
		

		
#if 0		
//		HAL_Delay(10);

//		/* Fill the buffer to write */
//		Fill_Buffer(aTxBuffer, BUFFER_SIZE, 0xA244250F);

//		/* Write data to the SDRAM memory */
//		for (uwIndex = 0; uwIndex < BUFFER_SIZE; uwIndex++)
//		{
//			*(__IO uint32_t*) (SDRAM_BANK_ADDR + WRITE_READ_ADDR + 4*uwIndex) = aTxBuffer[uwIndex];
//		}

//		/* Read back data from the SDRAM memory */
//		for (uwIndex = 0; uwIndex < BUFFER_SIZE; uwIndex++)
//		{
//			aRxBuffer[uwIndex] = *(__IO uint32_t*) (SDRAM_BANK_ADDR + WRITE_READ_ADDR + 4*uwIndex);
//		}

//		/*##-3- Checking data integrity ############################################*/

//		for (uwIndex = 0; (uwIndex < BUFFER_SIZE) && (uwWriteReadStatus == 0); uwIndex++)
//		{
//			if (aRxBuffer[uwIndex] != aTxBuffer[uwIndex])
//			{
//				uwWriteReadStatus++;
//			}
//		}
//		
//		if (uwWriteReadStatus != PASSED)
//		{
//			printf("SDRAM Test Failed!\r\n");
//		}
//		else
//		{
//			printf("SDRAM Test Passed!\r\n");
//		}	
//	
//		retry--;
#endif		
//	} 
//	while ((retry != 0) && (uwWriteReadStatus != PASSED));

	
	// frame buffer initialize
//	x = 0;
//	do {
//		*(volatile uint16_t *)(0xc0000000 + x*2) = 0x0000; x++;  
//	} while (x < (320*480)); 

}

static void Fill_Buffer(uint32_t *pBuffer, uint32_t uwBufferLenght, uint32_t uwOffset)
{
  uint32_t tmpIndex = 0;

  /* Put in global buffer different values */
  for (tmpIndex = 0; tmpIndex < uwBufferLenght; tmpIndex++ )
  {
    pBuffer[tmpIndex] = tmpIndex + uwOffset;
  }
}

#endif


