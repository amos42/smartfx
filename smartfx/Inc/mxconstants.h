/**
  ******************************************************************************
  * File Name          : mxconstants.h
  * Description        : This file contains the common defines of the application
  ******************************************************************************
  *
  * COPYRIGHT(c) 2016 STMicroelectronics
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private define ------------------------------------------------------------*/

#define D_LED_GRN_Pin GPIO_PIN_4
#define D_LED_GRN_GPIO_Port GPIOE
#define QSPI_D2_Pin GPIO_PIN_2
#define QSPI_D2_GPIO_Port GPIOE
#define D_LCD_BL_EN_Pin GPIO_PIN_14
#define D_LCD_BL_EN_GPIO_Port GPIOG
#define D_LCD_BL_SCL_Pin GPIO_PIN_8
#define D_LCD_BL_SCL_GPIO_Port GPIOB
#define D_LED_NOR_Pin GPIO_PIN_5
#define D_LED_NOR_GPIO_Port GPIOB
#define D_LED_HIGH_Pin GPIO_PIN_4
#define D_LED_HIGH_GPIO_Port GPIOB
#define SWO_Pin GPIO_PIN_3
#define SWO_GPIO_Port GPIOB
#define D_uSD_CLK_Pin GPIO_PIN_12
#define D_uSD_CLK_GPIO_Port GPIOC
#define SWCLK_Pin GPIO_PIN_14
#define SWCLK_GPIO_Port GPIOA
#define SWDIO_Pin GPIO_PIN_13
#define SWDIO_GPIO_Port GPIOA
#define D_LED_BLUE_Pin GPIO_PIN_5
#define D_LED_BLUE_GPIO_Port GPIOE
#define D_LCD_BL_SDA_Pin GPIO_PIN_9
#define D_LCD_BL_SDA_GPIO_Port GPIOB
#define D_VCP_RX_Pin GPIO_PIN_7
#define D_VCP_RX_GPIO_Port GPIOB
#define QSPI_NCS_Pin GPIO_PIN_6
#define QSPI_NCS_GPIO_Port GPIOB
#define D_PWR_KEY_Pin GPIO_PIN_11
#define D_PWR_KEY_GPIO_Port GPIOG
#define D_PWR_HOLD_Pin GPIO_PIN_13
#define D_PWR_HOLD_GPIO_Port GPIOJ
#define OTG_FS_VBUS_Pin GPIO_PIN_12
#define OTG_FS_VBUS_GPIO_Port GPIOJ
#define FMC_D2_Pin GPIO_PIN_0
#define FMC_D2_GPIO_Port GPIOD
#define D_uSD_D3_Pin GPIO_PIN_11
#define D_uSD_D3_GPIO_Port GPIOC
#define D_uSD_D2_Pin GPIO_PIN_10
#define D_uSD_D2_GPIO_Port GPIOC
#define OTG_FS_P_Pin GPIO_PIN_12
#define OTG_FS_P_GPIO_Port GPIOA
#define LCD_DE_Pin GPIO_PIN_7
#define LCD_DE_GPIO_Port GPIOK
#define LCD_B7_Pin GPIO_PIN_6
#define LCD_B7_GPIO_Port GPIOK
#define LCD_B6_Pin GPIO_PIN_5
#define LCD_B6_GPIO_Port GPIOK
#define LCD_B4_Pin GPIO_PIN_12
#define LCD_B4_GPIO_Port GPIOG
#define LCD_B2_Pin GPIO_PIN_14
#define LCD_B2_GPIO_Port GPIOJ
#define D_OTG_FS_PowerSwitchOn_Pin GPIO_PIN_5
#define D_OTG_FS_PowerSwitchOn_GPIO_Port GPIOD
#define FMC_D3_Pin GPIO_PIN_1
#define FMC_D3_GPIO_Port GPIOD
#define OTG_FS_N_Pin GPIO_PIN_11
#define OTG_FS_N_GPIO_Port GPIOA
#define LCD_HSYNC_Pin GPIO_PIN_10
#define LCD_HSYNC_GPIO_Port GPIOI
#define LCD_B5_Pin GPIO_PIN_4
#define LCD_B5_GPIO_Port GPIOK
#define LCD_B3_Pin GPIO_PIN_15
#define LCD_B3_GPIO_Port GPIOJ
#define D_uSD_CMD_Pin GPIO_PIN_2
#define D_uSD_CMD_GPIO_Port GPIOD
#define D_LCD_SPI_CLK_Pin GPIO_PIN_1
#define D_LCD_SPI_CLK_GPIO_Port GPIOI
#define OTG_FS_ID_Pin GPIO_PIN_10
#define OTG_FS_ID_GPIO_Port GPIOA
#define LCD_VSYNC_Pin GPIO_PIN_9
#define LCD_VSYNC_GPIO_Port GPIOI
#define D_BLE_STS_Pin GPIO_PIN_0
#define D_BLE_STS_GPIO_Port GPIOI
#define D_VCP_TX_Pin GPIO_PIN_9
#define D_VCP_TX_GPIO_Port GPIOA
#define D_LCD_RST_N_Pin GPIO_PIN_11
#define D_LCD_RST_N_GPIO_Port GPIOI
#define LCD_G6_Pin GPIO_PIN_1
#define LCD_G6_GPIO_Port GPIOK
#define LCD_G7_Pin GPIO_PIN_2
#define LCD_G7_GPIO_Port GPIOK
#define D_uSD_D1_Pin GPIO_PIN_9
#define D_uSD_D1_GPIO_Port GPIOC
#define D_BLE_SLEEP_Pin GPIO_PIN_8
#define D_BLE_SLEEP_GPIO_Port GPIOA
#define OSC_25M_Pin GPIO_PIN_0
#define OSC_25M_GPIO_Port GPIOH
#define LCD_G4_Pin GPIO_PIN_11
#define LCD_G4_GPIO_Port GPIOJ
#define LCD_G5_Pin GPIO_PIN_0
#define LCD_G5_GPIO_Port GPIOK
#define D_uSD_D0_Pin GPIO_PIN_8
#define D_uSD_D0_GPIO_Port GPIOC
#define D_BLE_UART_RX_Pin GPIO_PIN_7
#define D_BLE_UART_RX_GPIO_Port GPIOC
#define LCD_CLK_Pin GPIO_PIN_14
#define LCD_CLK_GPIO_Port GPIOI
#define LCD_G3_Pin GPIO_PIN_10
#define LCD_G3_GPIO_Port GPIOJ
#define D_BLE_UART_TX_Pin GPIO_PIN_6
#define D_BLE_UART_TX_GPIO_Port GPIOC
#define FMC_SDNME_Pin GPIO_PIN_5
#define FMC_SDNME_GPIO_Port GPIOH
#define LCD_G2_Pin GPIO_PIN_9
#define LCD_G2_GPIO_Port GPIOJ
#define D_BLE_WAKEUP_Pin GPIO_PIN_7
#define D_BLE_WAKEUP_GPIO_Port GPIOG
#define D_K_DN_Pin GPIO_PIN_7
#define D_K_DN_GPIO_Port GPIOF
#define D_K_UP_Pin GPIO_PIN_6
#define D_K_UP_GPIO_Port GPIOF
#define LCD_R7_Pin GPIO_PIN_6
#define LCD_R7_GPIO_Port GPIOJ
#define FMC_D1_Pin GPIO_PIN_15
#define FMC_D1_GPIO_Port GPIOD
#define FMC_D15_Pin GPIO_PIN_10
#define FMC_D15_GPIO_Port GPIOD
#define D_K_ST_Pin GPIO_PIN_10
#define D_K_ST_GPIO_Port GPIOF
#define D_K_RT_Pin GPIO_PIN_9
#define D_K_RT_GPIO_Port GPIOF
#define D_K_LT_Pin GPIO_PIN_8
#define D_K_LT_GPIO_Port GPIOF
#define FMC_D0_Pin GPIO_PIN_14
#define FMC_D0_GPIO_Port GPIOD
#define FMC_D14_Pin GPIO_PIN_9
#define FMC_D14_GPIO_Port GPIOD
#define FMC_D13_Pin GPIO_PIN_8
#define FMC_D13_GPIO_Port GPIOD
#define D_K_RE_Pin GPIO_PIN_0
#define D_K_RE_GPIO_Port GPIOC
#define QSPI_CLK_Pin GPIO_PIN_2
#define QSPI_CLK_GPIO_Port GPIOB
#define LCD_R5_Pin GPIO_PIN_4
#define LCD_R5_GPIO_Port GPIOJ
#define QSPI_D1_Pin GPIO_PIN_12
#define QSPI_D1_GPIO_Port GPIOD
#define QSPI_D3_Pin GPIO_PIN_13
#define QSPI_D3_GPIO_Port GPIOD
#define D_LCD_TS_INT_N_Pin GPIO_PIN_2
#define D_LCD_TS_INT_N_GPIO_Port GPIOG
#define LCD_R6_Pin GPIO_PIN_5
#define LCD_R6_GPIO_Port GPIOJ
#define D_ADC_SEN_Pin GPIO_PIN_1
#define D_ADC_SEN_GPIO_Port GPIOA
#define D_ADC_BAT_Pin GPIO_PIN_0
#define D_ADC_BAT_GPIO_Port GPIOA
#define D_SPK_OUT_Pin GPIO_PIN_4
#define D_SPK_OUT_GPIO_Port GPIOA
#define D_SPK_ONOFF_Pin GPIO_PIN_4
#define D_SPK_ONOFF_GPIO_Port GPIOC
#define LCD_R4_Pin GPIO_PIN_3
#define LCD_R4_GPIO_Port GPIOJ
#define FMC_D5_Pin GPIO_PIN_8
#define FMC_D5_GPIO_Port GPIOE
#define QSPI_D0_Pin GPIO_PIN_11
#define QSPI_D0_GPIO_Port GPIOD
#define FMC_BA1_Pin GPIO_PIN_5
#define FMC_BA1_GPIO_Port GPIOG
#define FMC_BA0_Pin GPIO_PIN_4
#define FMC_BA0_GPIO_Port GPIOG
#define D_LCD_TS_SCL_Pin GPIO_PIN_7
#define D_LCD_TS_SCL_GPIO_Port GPIOH
#define D_LCD_TS_RST_N_Pin GPIO_PIN_9
#define D_LCD_TS_RST_N_GPIO_Port GPIOH
#define D_LCD_SPI_DIR_Pin GPIO_PIN_11
#define D_LCD_SPI_DIR_GPIO_Port GPIOH
#define LCD_R3_Pin GPIO_PIN_2
#define LCD_R3_GPIO_Port GPIOJ
#define FMC_D6_Pin GPIO_PIN_9
#define FMC_D6_GPIO_Port GPIOE
#define FMC_D8_Pin GPIO_PIN_11
#define FMC_D8_GPIO_Port GPIOE
#define FMC_D11_Pin GPIO_PIN_14
#define FMC_D11_GPIO_Port GPIOE
#define D_LCD_TS_SDA_Pin GPIO_PIN_8
#define D_LCD_TS_SDA_GPIO_Port GPIOH
#define D_LCD_SPI_CS_Pin GPIO_PIN_10
#define D_LCD_SPI_CS_GPIO_Port GPIOH
#define LCD_R2_Pin GPIO_PIN_1
#define LCD_R2_GPIO_Port GPIOJ
#define FMC_D4_Pin GPIO_PIN_7
#define FMC_D4_GPIO_Port GPIOE
#define FMC_D7_Pin GPIO_PIN_10
#define FMC_D7_GPIO_Port GPIOE
#define FMC_D9_Pin GPIO_PIN_12
#define FMC_D9_GPIO_Port GPIOE
#define FMC_D12_Pin GPIO_PIN_15
#define FMC_D12_GPIO_Port GPIOE
#define FMC_D10_Pin GPIO_PIN_13
#define FMC_D10_GPIO_Port GPIOE
#define D_LCD_SPI_DIO2_Pin GPIO_PIN_14
#define D_LCD_SPI_DIO2_GPIO_Port GPIOB
#define D_LCD_SPI_DIO_Pin GPIO_PIN_15
#define D_LCD_SPI_DIO_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

/**
  * @}
  */ 

/**
  * @}
*/ 

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
