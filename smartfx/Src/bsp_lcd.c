#include "bsp_lcd.h"
#include "cmsis_os.h"

#define csb_0			HAL_GPIO_WritePin(D_LCD_SPI_CS_GPIO_Port, D_LCD_SPI_CS_Pin, GPIO_PIN_RESET)
#define csb_1			HAL_GPIO_WritePin(D_LCD_SPI_CS_GPIO_Port, D_LCD_SPI_CS_Pin, GPIO_PIN_SET)

#define sclb_0			HAL_GPIO_WritePin(D_LCD_SPI_CLK_GPIO_Port, D_LCD_SPI_CLK_Pin, GPIO_PIN_RESET)
#define sclb_1			HAL_GPIO_WritePin(D_LCD_SPI_CLK_GPIO_Port, D_LCD_SPI_CLK_Pin, GPIO_PIN_SET)

#define sdi_0			HAL_GPIO_WritePin(D_LCD_SPI_DIO_GPIO_Port, D_LCD_SPI_DIO_Pin, GPIO_PIN_RESET)
#define sdi_1			HAL_GPIO_WritePin(D_LCD_SPI_DIO_GPIO_Port, D_LCD_SPI_DIO_Pin, GPIO_PIN_SET)

#define res_0			HAL_GPIO_WritePin(D_LCD_RST_N_GPIO_Port, D_LCD_RST_N_Pin, GPIO_PIN_RESET)
#define res_1			HAL_GPIO_WritePin(D_LCD_RST_N_GPIO_Port, D_LCD_RST_N_Pin, GPIO_PIN_SET)

#define delay(x)		osDelay(x)


static void write_command (unsigned char y) // (uchar y,uchar x)
{
	unsigned char i;
	
	csb_0;
	sclb_0;
	sdi_0;
	sclb_1;
	
	for(i=0;i<8;i++)
	{
		sclb_0;
		if (y&0x80) sdi_1;
		else sdi_0;
		sclb_1;
		y=y<<1;
	}
	csb_1;
}

//***************************************************************
static void write_data(unsigned char w) // (uchar w, uchar v)
{
	unsigned char i;

	csb_0;
	sclb_0;
	sdi_1;
	sclb_1;

	for(i=0;i<8;i++)
	{
		sclb_0;
		if (w&0x80)
		sdi_1;
		else
		sdi_0;
		sclb_1;
		w=w<<1;
	}
	csb_1;
}

/*************************************************/
void bsp_lcd_init(void)
{
	res_1;
	delay(1);
	res_0;
	delay(10);
	res_1;
	delay(120);
/***************************************/ 
	write_command(0xC0);	// Power Control 1
	write_data(0x08);
	write_data(0x06);

	write_command(0xC1);	// Power Control 2
	write_data(0x43);

	write_command(0XC5);	// VCOM Control
	write_data(0x00);
	write_data(0x40);

	write_command(0xC2);	// Power Control 3 (For Normal Mode)
	write_data(0x33);

	write_command(0xB1);	// Frame Rate Control (In Normal Mode/Full Colors)
	write_data(0x10); 	//B0); //30 fps, fosc
	write_data(0x11);		// 	

	write_command(0xB4);	// Display Inversion Control (B4h)
	write_data(0x02);

	write_command(0xB7);	// Entry Mode Set (B7h) 
	write_data(0x86);

	write_command(0xE0);		// PGAMCTRL(Positive Gamma Control)
	write_data(0x0F);
	write_data(0x29);
	write_data(0x25);
	write_data(0x0B);
	write_data(0x0E);
	write_data(0x07);
	write_data(0x42);
	write_data(0x87);
	write_data(0x2C);
	write_data(0x06);
	write_data(0x0F);
	write_data(0x02);
	write_data(0x0B);
	write_data(0x07);
	write_data(0x00);

	write_command(0XE1);	// NGAMCTRL (Negative Gamma Correction)
	write_data(0x0F);
	write_data(0x38);
	write_data(0x34);
	write_data(0x0D);
	write_data(0x10);
	write_data(0x09);
	write_data(0x53);
	write_data(0x87);
	write_data(0x3D);
	write_data(0x08);
	write_data(0x11);
	write_data(0x04);
	write_data(0x1A);
	write_data(0x16);
	write_data(0x00);

	write_command(0XF2);
	write_data(0x1E);
	write_data(0xA3);
	write_data(0x32);
	write_data(0x02);
	write_data(0xB2);
	write_data(0x52);
	write_data(0xFF);
	write_data(0x10);
	write_data(0x00);

	write_command(0XF8);
	write_data(0x21);
	write_data(0x04);

	write_command(0x36);		// Memory Access Control
	write_data(0x08);//48

	write_command(0x3A);		// Interface Pixel Format
	write_data(0x66); //55-16BIT

	write_command(0xF9);  
	write_data(0x00);  
	write_data(0x08); 

	write_command(0XF1);       
	write_data(0x36); 
	write_data(0x04); 
	write_data(0x00); 
	write_data(0x3C); 
	write_data(0X0F); 
	write_data(0x8F);   
	/**********set rgb interface mode******************/
	write_command(0xB6); 	// Display Function Control
	write_data(0x30); //30 set rgb
	write_data(0x22); //GS,SS 02£¬42£¬62
	write_data(0x3B);
	 /**************************************************/ 
	write_command(0x2A); //Frame rate control
	write_data(0x00);
	write_data(0x00);
	write_data(0x01);
	write_data(0x3F);

	write_command(0x2B); //Display function control
	write_data(0x00);
	write_data(0x00);
	write_data(0x01);
	write_data(0xDF);

	write_command(0x21);

	write_command(0x11);
	delay(120);
	write_command(0x29); //display on
	write_command(0x2c); 

}

//******************************************
void bsp_lcd_enter_standby(void)
{
	write_command(0x28);//display off
	delay(10);
	write_command(0x10);
	//delay(120);
	//RGB_OFF();
}
	
void bsp_lcd_exit_standby (void)
{
	//RGB_ON();
	//delay(10);
	write_command(0x11);// Exit Sleep/ Standby mode
	delay(120);
	write_command(0x29);
}

void bsp_lcd_rst_enable(void)
{
	HAL_GPIO_WritePin(D_LCD_RST_N_GPIO_Port, D_LCD_RST_N_Pin, GPIO_PIN_RESET); 	
}

void bsp_lcd_rst_disable(void)
{
	HAL_GPIO_WritePin(D_LCD_RST_N_GPIO_Port, D_LCD_RST_N_Pin, GPIO_PIN_SET); 	
}


void bsp_bl_enable(void)
{
	HAL_GPIO_WritePin(D_LCD_BL_EN_GPIO_Port, D_LCD_BL_EN_Pin, GPIO_PIN_SET); 	
}

void bsp_bl_disable(void)
{
	HAL_GPIO_WritePin(D_LCD_BL_EN_GPIO_Port, D_LCD_BL_EN_Pin, GPIO_PIN_RESET); 	
}


#define D_DEVADDR_SC620				0xe0				// back light

int8_t bsp_bl_i2c_tx(uint8_t *pData, uint16_t Size)
{
	while (HAL_I2C_Master_Transmit(&hi2c1, (uint16_t)D_DEVADDR_SC620, pData, Size, 3000)!= HAL_OK) 
	{
		if (HAL_I2C_GetError(&hi2c1) != HAL_I2C_ERROR_AF)
		{
			return -1;
		}
		return 0;
	}
	return -1;
}

int8_t bsp_bl_i2c_rx(uint8_t *pData, uint16_t Size)
{
	while(HAL_I2C_Master_Receive(&hi2c1, (uint16_t)D_DEVADDR_SC620, (uint8_t *)pData, Size, 3000) != HAL_OK)
	{
		if (HAL_I2C_GetError(&hi2c1) != HAL_I2C_ERROR_AF)
		{
			return -1;
		}
		return 0;
	 }
	 return -1;
}


void bsp_bl_set(uint8_t tbr_idx)
{
	const uint8_t table[5] = { 12, 24, 36, 48, 63 }; 
	uint8_t Buffer_Tx1[5]; //, Buffer_Rx1[15];
	HAL_StatusTypeDef sts1;
	int i;

	if (tbr_idx == 0 || tbr_idx > 5) return;
	
	Buffer_Tx1[0] = 0;
	Buffer_Tx1[1] = 0xff;
	sts1 = HAL_I2C_Master_Transmit(&hi2c1, 0xe0, Buffer_Tx1, 2, 5000);
	printf("\r\nsts = %d, ", sts1);

	for (i=1; i<=8; i++) {
		Buffer_Tx1[0] = i;
		Buffer_Tx1[1] = table[tbr_idx - 1];
		sts1 = HAL_I2C_Master_Transmit(&hi2c1, 0xe0, Buffer_Tx1, 2, 5000);
		printf("%d, ", sts1);
	}
	
	Buffer_Tx1[0] = 9;
	Buffer_Tx1[1] = 0x07;
	sts1 = HAL_I2C_Master_Transmit(&hi2c1, 0xe0, Buffer_Tx1, 2, 5000);
	printf("sts = %d\r\n", sts1);

}

void I2Cx_EV_IRQHandler(void)
{
  HAL_I2C_EV_IRQHandler(&hi2c1);
}

/**
  * @brief  This function handles I2C error interrupt request.
  * @param  None
  * @retval None
  * @Note   This function is redefined in "main.h" and related to I2C error
  */
void I2Cx_ER_IRQHandler(void)
{
  HAL_I2C_ER_IRQHandler(&hi2c1);
}


