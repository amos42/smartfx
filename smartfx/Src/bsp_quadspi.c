#include "bsp_quadspi.h"
#include "bsp_gpio.h"


#define BSP_LED_On(LED1)						LED_IND_ON()
#define BSP_LED_Toggle(LED1)				LED_IND_TOG()


/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Definition for QSPI clock resources */
#define QSPI_CLK_ENABLE()          __HAL_RCC_QSPI_CLK_ENABLE()
#define QSPI_CLK_DISABLE()         __HAL_RCC_QSPI_CLK_DISABLE()
#define QSPI_CS_GPIO_CLK_ENABLE()  __HAL_RCC_GPIOB_CLK_ENABLE()
#define QSPI_CLK_GPIO_CLK_ENABLE() __HAL_RCC_GPIOB_CLK_ENABLE()
#define QSPI_D0_GPIO_CLK_ENABLE()  __HAL_RCC_GPIOD_CLK_ENABLE()
#define QSPI_D1_GPIO_CLK_ENABLE()  __HAL_RCC_GPIOD_CLK_ENABLE()
#define QSPI_D2_GPIO_CLK_ENABLE()  __HAL_RCC_GPIOE_CLK_ENABLE()
#define QSPI_D3_GPIO_CLK_ENABLE()  __HAL_RCC_GPIOD_CLK_ENABLE()

#define QSPI_FORCE_RESET()         __HAL_RCC_QSPI_FORCE_RESET()
#define QSPI_RELEASE_RESET()       __HAL_RCC_QSPI_RELEASE_RESET()


/* N25Q512A Micron memory */
/* Size of the flash */
#define QSPI_FLASH_SIZE                      23
#define QSPI_PAGE_SIZE                       256

/* Reset Operations */
#define RESET_ENABLE_CMD                     0x66
#define RESET_MEMORY_CMD                     0x99

/* Identification Operations */
#define READ_ID_CMD                          0x9E
#define READ_ID_CMD2                         0x9F
#define MULTIPLE_IO_READ_ID_CMD              0xAF
#define READ_SERIAL_FLASH_DISCO_PARAM_CMD    0x5A

/* Read Operations */
#define READ_CMD                             0x03
#define READ_4_BYTE_ADDR_CMD                 0x13

#define FAST_READ_CMD                        0x0B
#define FAST_READ_DTR_CMD                    0x0D
#define FAST_READ_4_BYTE_ADDR_CMD            0x0C

#define DUAL_OUT_FAST_READ_CMD               0x3B
#define DUAL_OUT_FAST_READ_DTR_CMD           0x3D
#define DUAL_OUT_FAST_READ_4_BYTE_ADDR_CMD   0x3C

#define DUAL_INOUT_FAST_READ_CMD             0xBB
#define DUAL_INOUT_FAST_READ_DTR_CMD         0xBD
#define DUAL_INOUT_FAST_READ_4_BYTE_ADDR_CMD 0xBC

#define QUAD_OUT_FAST_READ_CMD               0x6B
#define QUAD_OUT_FAST_READ_DTR_CMD           0x6D
#define QUAD_OUT_FAST_READ_4_BYTE_ADDR_CMD   0x6C

#define QUAD_INOUT_FAST_READ_CMD             0xEB
#define QUAD_INOUT_FAST_READ_DTR_CMD         0xED
#define QUAD_INOUT_FAST_READ_4_BYTE_ADDR_CMD 0xEC

/* Write Operations */
#define WRITE_ENABLE_CMD                     0x06
#define WRITE_DISABLE_CMD                    0x04

/* Register Operations */
#define READ_STATUS_REG_CMD                  0x05
#define WRITE_STATUS_REG_CMD                 0x01

#define READ_LOCK_REG_CMD                    0xE8
#define WRITE_LOCK_REG_CMD                   0xE5

#define READ_FLAG_STATUS_REG_CMD             0x70
#define CLEAR_FLAG_STATUS_REG_CMD            0x50

#define READ_NONVOL_CFG_REG_CMD              0xB5
#define WRITE_NONVOL_CFG_REG_CMD             0xB1

#define READ_VOL_CFG_REG_CMD                 0x85
#define WRITE_VOL_CFG_REG_CMD                0x81

#define READ_ENHANCED_VOL_CFG_REG_CMD        0x65
#define WRITE_ENHANCED_VOL_CFG_REG_CMD       0x61

#define READ_EXT_ADDR_REG_CMD                0xC8
#define WRITE_EXT_ADDR_REG_CMD               0xC5

/* Program Operations */
#define PAGE_PROG_CMD                        0x02
#define PAGE_PROG_4_BYTE_ADDR_CMD            0x12

#define DUAL_IN_FAST_PROG_CMD                0xA2
#define EXT_DUAL_IN_FAST_PROG_CMD            0xD2

#define QUAD_IN_FAST_PROG_CMD                0x32
#define EXT_QUAD_IN_FAST_PROG_CMD            0x12 /*0x38*/
#define QUAD_IN_FAST_PROG_4_BYTE_ADDR_CMD    0x34

/* Erase Operations */
#define SUBSECTOR_ERASE_CMD                  0x20
#define SUBSECTOR_ERASE_4_BYTE_ADDR_CMD      0x21

#define SECTOR_ERASE_CMD                     0xD8
#define SECTOR_ERASE_4_BYTE_ADDR_CMD         0xDC

#define BULK_ERASE_CMD                       0xC7

#define PROG_ERASE_RESUME_CMD                0x7A
#define PROG_ERASE_SUSPEND_CMD               0x75

/* One-Time Programmable Operations */
#define READ_OTP_ARRAY_CMD                   0x4B
#define PROG_OTP_ARRAY_CMD                   0x42

/* 4-byte Address Mode Operations */
#define ENTER_4_BYTE_ADDR_MODE_CMD           0xB7
#define EXIT_4_BYTE_ADDR_MODE_CMD            0xE9

/* Quad Operations */
#define ENTER_QUAD_CMD                       0x35
#define EXIT_QUAD_CMD                        0xF5

/* Default dummy clocks cycles */
#define DUMMY_CLOCK_CYCLES_READ              8
#define DUMMY_CLOCK_CYCLES_READ_QUAD         10

#define DUMMY_CLOCK_CYCLES_READ_DTR          6
#define DUMMY_CLOCK_CYCLES_READ_QUAD_DTR     8

/* End address of the QSPI memory */
#define QSPI_END_ADDR              (1 << QSPI_FLASH_SIZE)

/* Size of buffers */
#define BUFFERSIZE                 (COUNTOF(aTxBuffer) - 1)

/* Exported macro ------------------------------------------------------------*/
#define COUNTOF(__BUFFER__)        (sizeof(__BUFFER__) / sizeof(*(__BUFFER__)))




//QSPI_HandleTypeDef hqspi;
__IO uint8_t CmdCplt, RxCplt, TxCplt, StatusMatch, TimeOut;

/* Buffer used for transmission */
static uint8_t aTxBuffer[] = " ****QSPI communication based on IT****  ****QSPI communication based on IT****  ****QSPI communication based on IT****  ****QSPI communication based on IT****  ****QSPI communication based on IT****  ****QSPI communication based on IT**** ";

/* Buffer used for reception */
static uint8_t aRxBuffer[BUFFERSIZE];

static void Error_Handler(void);
static void QSPI_WriteEnable(QSPI_HandleTypeDef *hqspi);
static void QSPI_AutoPollingMemReady(QSPI_HandleTypeDef *hqspi);
static void QSPI_DummyCyclesCfg(QSPI_HandleTypeDef *hqspi);

void bsp_quadspi_test(void)
{
  QSPI_CommandTypeDef sCommand;
  uint32_t address = 0;
  uint16_t index;
  volatile uint16_t err_cnt = 0;
  
 
 
  /* Initialize QuadSPI ------------------------------------------------------ */
//  hqspi.Instance = QUADSPI;
//  HAL_QSPI_DeInit(&hqspi);
//        
//  /* ClockPrescaler set to 2, so QSPI clock = 216MHz / (2+1) = 72MHz */
//  hqspi.Init.ClockPrescaler     = 2;
//  hqspi.Init.FifoThreshold      = 4;
//  hqspi.Init.SampleShifting     = QSPI_SAMPLE_SHIFTING_HALFCYCLE;
//  hqspi.Init.FlashSize          = POSITION_VAL(0x1000000) - 1;
//  hqspi.Init.ChipSelectHighTime = QSPI_CS_HIGH_TIME_2_CYCLE;
//  hqspi.Init.ClockMode          = QSPI_CLOCK_MODE_0;
//  hqspi.Init.FlashID            = QSPI_FLASH_ID_1;
//  hqspi.Init.DualFlash          = QSPI_DUALFLASH_DISABLE;
//  
//  if (HAL_QSPI_Init(&hqspi) != HAL_OK)
//  {
//    Error_Handler();
//  }

// memory mapped mode 설정에 이미 되어 있음.
	sCommand.InstructionMode   = QSPI_INSTRUCTION_1_LINE;
	sCommand.AddressSize       = QSPI_ADDRESS_24_BITS;
	sCommand.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
	sCommand.DdrMode           = QSPI_DDR_MODE_DISABLE;
	sCommand.DdrHoldHalfCycle  = QSPI_DDR_HHC_ANALOG_DELAY;
	sCommand.SIOOMode          = QSPI_SIOO_INST_EVERY_CMD;

	CmdCplt = 0;

	/* Initialize Reception buffer --------------------------------------- */
	for (index = 0; index < BUFFERSIZE; index++)
	{
		aRxBuffer[index] = 0;
	}

	/* Enable write operations ------------------------------------------- */
	QSPI_WriteEnable(&hqspi);

	/* Erasing Sequence -------------------------------------------------- */
	sCommand.Instruction = SECTOR_ERASE_CMD;
	sCommand.AddressMode = QSPI_ADDRESS_1_LINE;
	sCommand.Address     = address;
	sCommand.DataMode    = QSPI_DATA_NONE;
	sCommand.DummyCycles = 0;

	if (HAL_QSPI_Command_IT(&hqspi, &sCommand) != HAL_OK)
	{
		Error_Handler();
	}

//	

	while (CmdCplt == 0);

	CmdCplt = 0;
	StatusMatch = 0;

	/* Configure automatic polling mode to wait for end of erase ------- */  
	QSPI_AutoPollingMemReady(&hqspi);
	
	
//
	while (StatusMatch == 0);
	
	StatusMatch = 0;
	TxCplt = 0;

	/* Enable write operations ----------------------------------------- */
	QSPI_WriteEnable(&hqspi);

	/* Writing Sequence ------------------------------------------------ */
	sCommand.Instruction = QUAD_IN_FAST_PROG_CMD;
	sCommand.AddressMode = QSPI_ADDRESS_1_LINE;
	sCommand.DataMode    = QSPI_DATA_4_LINES;
	sCommand.NbData      = BUFFERSIZE;

	if (HAL_QSPI_Command(&hqspi, &sCommand, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
	{
		Error_Handler();
	}

	if (HAL_QSPI_Transmit_IT(&hqspi, aTxBuffer) != HAL_OK)
	{
		Error_Handler();
	}
//
  while (TxCplt == 0);

	TxCplt = 0;
	StatusMatch = 0;

	/* Configure automatic polling mode to wait for end of program ----- */  
	QSPI_AutoPollingMemReady(&hqspi);

//
	while (StatusMatch == 0);
 
	StatusMatch = 0;
	RxCplt = 0;

	/* Configure Volatile Configuration register (with new dummy cycles) */
	QSPI_DummyCyclesCfg(&hqspi);

	/* Reading Sequence ------------------------------------------------ */
	sCommand.Instruction = QUAD_OUT_FAST_READ_CMD;
	sCommand.DummyCycles = DUMMY_CLOCK_CYCLES_READ_QUAD;

	if (HAL_QSPI_Command(&hqspi, &sCommand, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
	{
		Error_Handler();
	}

	if (HAL_QSPI_Receive_IT(&hqspi, aRxBuffer) != HAL_OK)
	{
		Error_Handler();
	}
//
	while (RxCplt == 0);

	RxCplt = 0;

	/* Result comparison ----------------------------------------------- */
	for (index = 0; index < BUFFERSIZE; index++)
	{
		if (aRxBuffer[index] != aTxBuffer[index])
		{
			err_cnt++;
		}
	}

	if (err_cnt != 0) printf("QuadSPI Test Error!!\r\n");
	else printf("QuadSPI Test OK!!\r\n");



//
}

/**
  * @brief  Command completed callbacks.
  * @param  hqspi: QSPI handle
  * @retval None
  */
void HAL_QSPI_CmdCpltCallback(QSPI_HandleTypeDef *hqspi)
{
  CmdCplt++;
}

/**
  * @brief  Rx Transfer completed callbacks.
  * @param  hqspi: QSPI handle
  * @retval None
  */
void HAL_QSPI_RxCpltCallback(QSPI_HandleTypeDef *hqspi)
{
  RxCplt++;
}

/**
  * @brief  Tx Transfer completed callbacks.
  * @param  hqspi: QSPI handle
  * @retval None
  */
void HAL_QSPI_TxCpltCallback(QSPI_HandleTypeDef *hqspi)
{
  TxCplt++; 
}

/**
  * @brief  Status Match callbacks
  * @param  hqspi: QSPI handle
  * @retval None
  */
void HAL_QSPI_StatusMatchCallback(QSPI_HandleTypeDef *hqspi)
{
  StatusMatch++;
}

static void QSPI_WriteEnable(QSPI_HandleTypeDef *hqspi)
{
  QSPI_CommandTypeDef     sCommand;
  QSPI_AutoPollingTypeDef sConfig;

  /* Enable write operations ------------------------------------------ */
  sCommand.InstructionMode   = QSPI_INSTRUCTION_1_LINE;
  sCommand.Instruction       = WRITE_ENABLE_CMD;
  sCommand.AddressMode       = QSPI_ADDRESS_NONE;
  sCommand.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
  sCommand.DataMode          = QSPI_DATA_NONE;
  sCommand.DummyCycles       = 0;
  sCommand.DdrMode           = QSPI_DDR_MODE_DISABLE;
  sCommand.DdrHoldHalfCycle  = QSPI_DDR_HHC_ANALOG_DELAY;
  sCommand.SIOOMode          = QSPI_SIOO_INST_EVERY_CMD;

  if (HAL_QSPI_Command(hqspi, &sCommand, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    Error_Handler();
  }
  
  /* Configure automatic polling mode to wait for write enabling ---- */  
  sConfig.Match           = 0x02;
  sConfig.Mask            = 0x02;
  sConfig.MatchMode       = QSPI_MATCH_MODE_AND;
  sConfig.StatusBytesSize = 1;
  sConfig.Interval        = 0x10;
  sConfig.AutomaticStop   = QSPI_AUTOMATIC_STOP_ENABLE;

  sCommand.Instruction    = READ_STATUS_REG_CMD;
  sCommand.DataMode       = QSPI_DATA_1_LINE;

  if (HAL_QSPI_AutoPolling(hqspi, &sCommand, &sConfig, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief  This function reads the SR of the memory and awaits the EOP.
  * @param  hqspi: QSPI handle
  * @retval None
  */
static void QSPI_AutoPollingMemReady(QSPI_HandleTypeDef *hqspi)
{
  QSPI_CommandTypeDef     sCommand;
  QSPI_AutoPollingTypeDef sConfig;

  /* Configure automatic polling mode to wait for memory ready ------ */  
  sCommand.InstructionMode   = QSPI_INSTRUCTION_1_LINE;
  sCommand.Instruction       = READ_STATUS_REG_CMD;
  sCommand.AddressMode       = QSPI_ADDRESS_NONE;
  sCommand.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
  sCommand.DataMode          = QSPI_DATA_1_LINE;
  sCommand.DummyCycles       = 0;
  sCommand.DdrMode           = QSPI_DDR_MODE_DISABLE;
  sCommand.DdrHoldHalfCycle  = QSPI_DDR_HHC_ANALOG_DELAY;
  sCommand.SIOOMode         = QSPI_SIOO_INST_EVERY_CMD;

  sConfig.Match           = 0x00;
  sConfig.Mask            = 0x01;
  sConfig.MatchMode       = QSPI_MATCH_MODE_AND;
  sConfig.StatusBytesSize = 1;
  sConfig.Interval        = 0x10;
  sConfig.AutomaticStop   = QSPI_AUTOMATIC_STOP_ENABLE;

  if (HAL_QSPI_AutoPolling_IT(hqspi, &sCommand, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief  This function configures the dummy cycles on memory side.
  * @param  hqspi: QSPI handle
  * @retval None
  */
static void QSPI_DummyCyclesCfg(QSPI_HandleTypeDef *hqspi)
{
  QSPI_CommandTypeDef sCommand;
  uint8_t reg;

  /* Read Volatile Configuration register --------------------------- */
  sCommand.InstructionMode   = QSPI_INSTRUCTION_1_LINE;
  sCommand.Instruction       = READ_VOL_CFG_REG_CMD;
  sCommand.AddressMode       = QSPI_ADDRESS_NONE;
  sCommand.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
  sCommand.DataMode          = QSPI_DATA_1_LINE;
  sCommand.DummyCycles       = 0;
  sCommand.DdrMode           = QSPI_DDR_MODE_DISABLE;
  sCommand.DdrHoldHalfCycle  = QSPI_DDR_HHC_ANALOG_DELAY;
  sCommand.SIOOMode         = QSPI_SIOO_INST_EVERY_CMD;
  sCommand.NbData            = 1;

  if (HAL_QSPI_Command(hqspi, &sCommand, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    Error_Handler();
  }

  if (HAL_QSPI_Receive(hqspi, &reg, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    Error_Handler();
  }

  /* Enable write operations ---------------------------------------- */
  QSPI_WriteEnable(hqspi);

  /* Write Volatile Configuration register (with new dummy cycles) -- */  
  sCommand.Instruction = WRITE_VOL_CFG_REG_CMD;
  MODIFY_REG(reg, 0xF0, (DUMMY_CLOCK_CYCLES_READ_QUAD << POSITION_VAL(0xF0)));
      
  if (HAL_QSPI_Command(hqspi, &sCommand, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    Error_Handler();
  }

  if (HAL_QSPI_Transmit(hqspi, &reg, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
static void Error_Handler(void)
{
  /* User may add here some code to deal with this error */
    printf("QuadSpi Error!\r\n");
}

///**
//  * @brief  This function reset the QSPI memory.
//  * @param  hqspi: QSPI handle
//  * @retval None
//  */
//static uint8_t QSPI_ResetMemory(QSPI_HandleTypeDef *hqspi)
//{
//  QSPI_CommandTypeDef s_command;

//  /* Initialize the reset enable command */
//  s_command.InstructionMode   = QSPI_INSTRUCTION_1_LINE;
//  s_command.Instruction       = RESET_ENABLE_CMD;
//  s_command.AddressMode       = QSPI_ADDRESS_NONE;
//  s_command.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
//  s_command.DataMode          = QSPI_DATA_NONE;
//  s_command.DummyCycles       = 0;
//  s_command.DdrMode           = QSPI_DDR_MODE_DISABLE;
//  s_command.DdrHoldHalfCycle  = QSPI_DDR_HHC_ANALOG_DELAY;
//  s_command.SIOOMode          = QSPI_SIOO_INST_EVERY_CMD;

//  /* Send the command */
//  if (HAL_QSPI_Command(hqspi, &s_command, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
//  {
//    return QSPI_ERROR;
//  }

//  /* Send the reset memory command */
//  s_command.Instruction = RESET_MEMORY_CMD;
//  if (HAL_QSPI_Command(hqspi, &s_command, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
//  {
//    return QSPI_ERROR;
//  }

//  /* Configure automatic polling mode to wait the memory is ready */  
//  if (QSPI_AutoPollingMemReady(hqspi, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != QSPI_OK)
//  {
//    return QSPI_ERROR;
//  }

//  return QSPI_OK;
//}


uint8_t BSP_QSPI_Init(void)
{ 

//  /* QSPI memory reset */
//  if (QSPI_ResetMemory(&hqspi) != QSPI_OK)
//  {
//    return QSPI_NOT_SUPPORTED;
//  }
// 
//  /* Configuration of the dummy cycles on QSPI memory side */
//  if (QSPI_DummyCyclesCfg(&hqspi) != QSPI_OK)
//  {
//    return QSPI_NOT_SUPPORTED;
//  }
//  
  return QSPI_OK;
}



uint8_t BSP_QSPI_MemoryMappedMode(void)
{
  QSPI_CommandTypeDef      s_command;
  QSPI_MemoryMappedTypeDef s_mem_mapped_cfg;

  /* Configure the command for the read instruction */
  s_command.InstructionMode   = QSPI_INSTRUCTION_1_LINE;
  s_command.Instruction       = QUAD_INOUT_FAST_READ_CMD;
  s_command.AddressMode       = QSPI_ADDRESS_4_LINES;
  s_command.AddressSize       = QSPI_ADDRESS_24_BITS;
  s_command.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
  s_command.DataMode          = QSPI_DATA_4_LINES;
  s_command.DummyCycles       = N25Q128A_DUMMY_CYCLES_READ_QUAD;
  s_command.DdrMode           = QSPI_DDR_MODE_DISABLE;
  s_command.DdrHoldHalfCycle  = QSPI_DDR_HHC_ANALOG_DELAY;
  s_command.SIOOMode          = QSPI_SIOO_INST_EVERY_CMD;
  
  /* Configure the memory mapped mode */
  s_mem_mapped_cfg.TimeOutActivation = QSPI_TIMEOUT_COUNTER_ENABLE;
  s_mem_mapped_cfg.TimeOutPeriod     = 1;
  
  if (HAL_QSPI_MemoryMapped(&hqspi, &s_command, &s_mem_mapped_cfg) != HAL_OK)
  {
    return QSPI_ERROR;
  }

  return QSPI_OK;
}

