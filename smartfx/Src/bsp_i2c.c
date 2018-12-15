#include <string.h>
//#include "stm32f1xx_hal.h"
#include "i2c.h"
#include "a_i2c.h"

uint8_t Buffer_Rx1[30];
uint8_t Buffer_Tx1[30];
  
// I2C test 
int i2c_loop(void)
{
	HAL_StatusTypeDef sts1, sts2;
	static uint8_t mpu9150_reg_addr = 0x0d;


	Buffer_Tx1[0] = mpu9150_reg_addr;
	
	sts1 = HAL_I2C_Master_Transmit(&hi2c1, 0x68 << 1, Buffer_Tx1, 1, 1000);
	sts2 = HAL_I2C_Master_Receive(&hi2c1, 0x68 << 1, Buffer_Rx1, 1, 1000);
	
 	printf("sts1:%d, sts2:%d, ra:%02x, da:%02x\r\n", sts1, sts2, mpu9150_reg_addr, Buffer_Rx1[0]);

	if (mpu9150_reg_addr == 0x10) mpu9150_reg_addr = 0x0d;
	else mpu9150_reg_addr++;

	return 0;
}
  
int8_t SENSOR9AXIS_Read_Bytes(uint8_t devAddr, uint8_t regAddr, uint8_t length, uint8_t *data)
{

	HAL_StatusTypeDef  i2cRW_status;

	Buffer_Tx1[0] = regAddr;

	i2cRW_status = HAL_I2C_Master_Transmit(&hi2c1, devAddr << 1, Buffer_Tx1, 1, 1000);
	if (i2cRW_status != HAL_OK)
	{
	  //DEBUG_MSG_FUNC_ERROR;
	  return 0;
	}

	i2cRW_status = HAL_I2C_Master_Receive(&hi2c1, devAddr << 1, data, length, 1000);	 
	if (i2cRW_status != HAL_OK)
	{
	//        DEBUG_MSG_FUNC_ERROR;
	  return 0;
	}

	return length;
}

int8_t SENSOR9AXIS_Write_Bytes(uint8_t devAddr, uint8_t regAddr, uint8_t length, uint8_t *data)
{
	HAL_StatusTypeDef  i2cRW_status;

	Buffer_Tx1[0] = regAddr;
	memcpy(&Buffer_Tx1[1], data, length);

	HAL_I2C_Master_Transmit(&hi2c1, devAddr << 1, Buffer_Tx1, length+1, 1000);
	if (i2cRW_status != HAL_OK)
	{
	  //DEBUG_MSG_FUNC_ERROR;
	  return 0;
	}

	return length;
}
