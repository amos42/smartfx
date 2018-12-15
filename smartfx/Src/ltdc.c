/**
  ******************************************************************************
  * File Name          : LTDC.c
  * Description        : This file provides code for the configuration
  *                      of the LTDC instances.
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
#include "ltdc.h"

#include "gpio.h"

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

LTDC_HandleTypeDef hltdc;
LTDC_LayerCfgTypeDef pLayerCfg;

/* LTDC init function */
void MX_LTDC_Init(void)
{
#if 0


  hltdc.Instance = LTDC;
  hltdc.Init.HSPolarity = LTDC_HSPOLARITY_AL;
  hltdc.Init.VSPolarity = LTDC_VSPOLARITY_AL;
  hltdc.Init.DEPolarity = LTDC_DEPOLARITY_AL;
  hltdc.Init.PCPolarity = LTDC_PCPOLARITY_IPC;

#define  LCD320480_WIDTH    ((uint16_t)320)          /* LCD PIXEL WIDTH            */
#define  LCD320480_HEIGHT   ((uint16_t)480)          /* LCD PIXEL HEIGHT           */


#define  LCD320480_HSYNC            ((uint16_t)41)   /* Horizontal synchronization */
#define  LCD320480_HBP              ((uint16_t)13)    /* Horizontal back porch      */ 
#define  LCD320480_HFP              ((uint16_t)32)    /* Horizontal front porch     */
#define  LCD320480_VSYNC            ((uint16_t)10)   /* Vertical synchronization   */
#define  LCD320480_VBP              ((uint16_t)2)    /* Vertical back porch        */
#define  LCD320480_VFP              ((uint16_t)2)    /* Vertical front porch       */

	hltdc.Init.HorizontalSync = (LCD320480_HSYNC - 1);
	hltdc.Init.VerticalSync = (LCD320480_VSYNC - 1);
	hltdc.Init.AccumulatedHBP = (LCD320480_HSYNC + LCD320480_HBP - 1);
	hltdc.Init.AccumulatedVBP = (LCD320480_VSYNC + LCD320480_VBP - 1);  
	hltdc.Init.AccumulatedActiveH = (LCD320480_HEIGHT + LCD320480_VSYNC + LCD320480_VBP - 1);
	hltdc.Init.AccumulatedActiveW = (LCD320480_WIDTH + LCD320480_HSYNC + LCD320480_HBP - 1);
	hltdc.Init.TotalHeigh = (LCD320480_HEIGHT + LCD320480_VSYNC + LCD320480_VBP + LCD320480_VFP - 1);
	hltdc.Init.TotalWidth = (LCD320480_WIDTH + LCD320480_HSYNC + LCD320480_HBP + LCD320480_HFP - 1);   
	
	/* Initialize the LCD pixel width and pixel height */
	hltdc.LayerCfg->ImageWidth  = LCD320480_WIDTH;
	hltdc.LayerCfg->ImageHeight = LCD320480_HEIGHT;	
	
	hltdc.Init.Backcolor.Blue = 0;
	hltdc.Init.Backcolor.Green = 0;
	hltdc.Init.Backcolor.Red = 0;

	/* Configure the LTDC */  
	HAL_LTDC_Init(&hltdc);

	pLayerCfg.WindowX0 = 0;
	pLayerCfg.WindowX1 = LCD320480_WIDTH-1;
	pLayerCfg.WindowY0 = 0;
	pLayerCfg.WindowY1 = LCD320480_HEIGHT-1;
  
	pLayerCfg.PixelFormat = LTDC_PIXEL_FORMAT_RGB565;
	pLayerCfg.FBStartAdress = (uint32_t )0xc0000000;
	
	pLayerCfg.Alpha = 255;
	pLayerCfg.Alpha0 = 0; /* fully transparent */
	pLayerCfg.Backcolor.Blue = 0;
	pLayerCfg.Backcolor.Green = 0;
	pLayerCfg.Backcolor.Red = 0;

	/* Configure blending factors */
	pLayerCfg.BlendingFactor1 = LTDC_BLENDING_FACTOR1_CA;
	pLayerCfg.BlendingFactor2 = LTDC_BLENDING_FACTOR2_CA;

	/* Configure the number of lines and number of pixels per line */
	pLayerCfg.ImageWidth  = LCD320480_WIDTH;
	pLayerCfg.ImageHeight = LCD320480_HEIGHT;
    
  HAL_LTDC_ConfigLayer(&hltdc, &pLayerCfg, 1);

#else
  LTDC_LayerCfgTypeDef pLayerCfg;
  LTDC_LayerCfgTypeDef pLayerCfg1;

  hltdc.Instance = LTDC;
  
  hltdc.Init.HSPolarity = LTDC_HSPOLARITY_AL;
  hltdc.Init.VSPolarity = LTDC_VSPOLARITY_AL;
  hltdc.Init.DEPolarity = LTDC_DEPOLARITY_AL;
  hltdc.Init.PCPolarity = LTDC_PCPOLARITY_IPC;
  hltdc.Init.HorizontalSync = 40;
  hltdc.Init.VerticalSync = 9;						// 10 - 1
  hltdc.Init.AccumulatedHBP = 53;			// 41 + 13 - 1
  hltdc.Init.AccumulatedVBP = 11;			// 12 - 1
  hltdc.Init.AccumulatedActiveW = 373;	// 320 + 41 + 13 - 1
  hltdc.Init.AccumulatedActiveH = 491;
  hltdc.Init.TotalWidth = 405;
  hltdc.Init.TotalHeigh = 493;
  
  hltdc.Init.Backcolor.Blue = 0;
  hltdc.Init.Backcolor.Green = 0;
  hltdc.Init.Backcolor.Red = 0;
//	/* Initialize the LCD pixel width and pixel height */
//	hltdc.LayerCfg->ImageWidth  = 320;
//	hltdc.LayerCfg->ImageHeight = 480;	  
  HAL_LTDC_Init(&hltdc);

  pLayerCfg.WindowX0 = 0;
  pLayerCfg.WindowX1 = 320;
  pLayerCfg.WindowY0 = 0;
  pLayerCfg.WindowY1 = 480;
  pLayerCfg.PixelFormat = LTDC_PIXEL_FORMAT_RGB565;
  pLayerCfg.Alpha = 255;
  pLayerCfg.Alpha0 = 0;
  pLayerCfg.BlendingFactor1 = LTDC_BLENDING_FACTOR1_CA;
  pLayerCfg.BlendingFactor2 = LTDC_BLENDING_FACTOR2_CA;
  pLayerCfg.FBStartAdress = 0xc0000000;
  pLayerCfg.ImageWidth = 320;
  pLayerCfg.ImageHeight = 480;
  pLayerCfg.Backcolor.Blue = 0;
  pLayerCfg.Backcolor.Green = 0;
  pLayerCfg.Backcolor.Red = 0;
  HAL_LTDC_ConfigLayer(&hltdc, &pLayerCfg, 0);  
  HAL_LTDC_ConfigLayer(&hltdc, &pLayerCfg, 1);

//  pLayerCfg1.WindowX0 = 0;
//  pLayerCfg1.WindowX1 = 320;
//  pLayerCfg1.WindowY0 = 0;
//  pLayerCfg1.WindowY1 = 480;
//  pLayerCfg1.PixelFormat = LTDC_PIXEL_FORMAT_RGB565;
//  pLayerCfg1.Alpha = 255;
//  pLayerCfg1.Alpha0 = 0;
//  pLayerCfg1.BlendingFactor1 = LTDC_BLENDING_FACTOR1_CA;
//  pLayerCfg1.BlendingFactor2 = LTDC_BLENDING_FACTOR2_CA;
//  pLayerCfg1.FBStartAdress = 0xc0070800;
//  pLayerCfg1.ImageWidth = 320;
//  pLayerCfg1.ImageHeight = 480;
//  pLayerCfg1.Backcolor.Blue = 0;
//  pLayerCfg1.Backcolor.Green = 0xff;
//  pLayerCfg1.Backcolor.Red = 0;
//  HAL_LTDC_ConfigLayer(&hltdc, &pLayerCfg1, 0);
#endif  

}

void HAL_LTDC_MspInit(LTDC_HandleTypeDef* hltdc)
{

  GPIO_InitTypeDef GPIO_InitStruct;
  if(hltdc->Instance==LTDC)
  {
  /* USER CODE BEGIN LTDC_MspInit 0 */

  /* USER CODE END LTDC_MspInit 0 */
    /* Peripheral clock enable */
    __HAL_RCC_LTDC_CLK_ENABLE();
  
    /**LTDC GPIO Configuration    
    PK7     ------> LTDC_DE
    PK6     ------> LTDC_B7
    PK5     ------> LTDC_B6
    PG12     ------> LTDC_B4
    PJ14     ------> LTDC_B2
    PI10     ------> LTDC_HSYNC
    PK4     ------> LTDC_B5
    PJ15     ------> LTDC_B3
    PI9     ------> LTDC_VSYNC
    PK1     ------> LTDC_G6
    PK2     ------> LTDC_G7
    PJ11     ------> LTDC_G4
    PK0     ------> LTDC_G5
    PI14     ------> LTDC_CLK
    PJ10     ------> LTDC_G3
    PJ9     ------> LTDC_G2
    PJ6     ------> LTDC_R7
    PJ4     ------> LTDC_R5
    PJ5     ------> LTDC_R6
    PJ3     ------> LTDC_R4
    PJ2     ------> LTDC_R3
    PJ1     ------> LTDC_R2 
    */
    GPIO_InitStruct.Pin = LCD_DE_Pin|LCD_B7_Pin|LCD_B6_Pin|LCD_B5_Pin 
                          |LCD_G6_Pin|LCD_G7_Pin|LCD_G5_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF14_LTDC;
    HAL_GPIO_Init(GPIOK, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = LCD_B4_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF9_LTDC;
    HAL_GPIO_Init(LCD_B4_GPIO_Port, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = LCD_B2_Pin|LCD_B3_Pin|LCD_G4_Pin|LCD_G3_Pin 
                          |LCD_G2_Pin|LCD_R7_Pin|LCD_R5_Pin|LCD_R6_Pin 
                          |LCD_R4_Pin|LCD_R3_Pin|LCD_R2_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF14_LTDC;
    HAL_GPIO_Init(GPIOJ, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = LCD_HSYNC_Pin|LCD_VSYNC_Pin|LCD_CLK_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF14_LTDC;
    HAL_GPIO_Init(GPIOI, &GPIO_InitStruct);

    /* Peripheral interrupt init */
    HAL_NVIC_SetPriority(LTDC_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(LTDC_IRQn);
  /* USER CODE BEGIN LTDC_MspInit 1 */

  /* USER CODE END LTDC_MspInit 1 */
  }
}

void HAL_LTDC_MspDeInit(LTDC_HandleTypeDef* hltdc)
{

  if(hltdc->Instance==LTDC)
  {
  /* USER CODE BEGIN LTDC_MspDeInit 0 */

  /* USER CODE END LTDC_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_LTDC_CLK_DISABLE();
  
    /**LTDC GPIO Configuration    
    PK7     ------> LTDC_DE
    PK6     ------> LTDC_B7
    PK5     ------> LTDC_B6
    PG12     ------> LTDC_B4
    PJ14     ------> LTDC_B2
    PI10     ------> LTDC_HSYNC
    PK4     ------> LTDC_B5
    PJ15     ------> LTDC_B3
    PI9     ------> LTDC_VSYNC
    PK1     ------> LTDC_G6
    PK2     ------> LTDC_G7
    PJ11     ------> LTDC_G4
    PK0     ------> LTDC_G5
    PI14     ------> LTDC_CLK
    PJ10     ------> LTDC_G3
    PJ9     ------> LTDC_G2
    PJ6     ------> LTDC_R7
    PJ4     ------> LTDC_R5
    PJ5     ------> LTDC_R6
    PJ3     ------> LTDC_R4
    PJ2     ------> LTDC_R3
    PJ1     ------> LTDC_R2 
    */
    HAL_GPIO_DeInit(GPIOK, LCD_DE_Pin|LCD_B7_Pin|LCD_B6_Pin|LCD_B5_Pin 
                          |LCD_G6_Pin|LCD_G7_Pin|LCD_G5_Pin);

    HAL_GPIO_DeInit(LCD_B4_GPIO_Port, LCD_B4_Pin);

    HAL_GPIO_DeInit(GPIOJ, LCD_B2_Pin|LCD_B3_Pin|LCD_G4_Pin|LCD_G3_Pin 
                          |LCD_G2_Pin|LCD_R7_Pin|LCD_R5_Pin|LCD_R6_Pin 
                          |LCD_R4_Pin|LCD_R3_Pin|LCD_R2_Pin);

    HAL_GPIO_DeInit(GPIOI, LCD_HSYNC_Pin|LCD_VSYNC_Pin|LCD_CLK_Pin);

    /* Peripheral interrupt Deinit*/
    HAL_NVIC_DisableIRQ(LTDC_IRQn);

  }
  /* USER CODE BEGIN LTDC_MspDeInit 1 */

  /* USER CODE END LTDC_MspDeInit 1 */
} 

/* USER CODE BEGIN 1 */

/* USER CODE END 1 */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
