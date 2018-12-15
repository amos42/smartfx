#include "bsp_flash.h"
#include "global.h"

// 7번 Sector 영역에 user data를 읽고 쓰는 루틴
// Sector Start Address :	0x080A0000
// Sector Size:							256 Kbytes

#define FLASH_USER_START_ADDR   			ADDR_FLASH_SECTOR_7  
#define FLASH_USER_END_ADDR     			(ADDR_FLASH_SECTOR_7  +	SIZE_FLASH_SECTOR_256K)

static uint32_t GetSector(uint32_t Address);


void bsp_flash_init(void)
{
}

uint8_t bsp_flash_user_data_valid(void)
{
	SETTINGDEVICE Dev;
	
	bsp_flash_user_data_read((uint8_t *)&Dev, sizeof(SETTINGDEVICE));
	
	if (Dev.sig == D_SIGNITURE) return 0;
	return (-1);
}

uint32_t bsp_flash_user_data_read(uint8_t *r_buf, uint32_t szr_buf)
{
	uint8_t *src_addr = (uint8_t *)FLASH_USER_START_ADDR;
	uint32_t i;
	
	if (szr_buf == 0) return 0;
	
	for (i = 0; i < szr_buf; i++) {
		r_buf[i] = src_addr[i];
	}
	
	return i;
}

uint32_t bsp_flash_user_data_write(uint8_t *w_buf, uint32_t szw_buf)
{
	FLASH_EraseInitTypeDef EraseInitStruct;	
	uint32_t FirstSector = 0, NbOfSectors = 0, err_code;
	uint32_t Address = 0, SECTORError = 0;
	uint32_t szWordLength, i ,ret = 0;
	uint32_t *pWordBuf = (uint32_t *)w_buf;
	
	if (szw_buf == 0) return 0;
	
	HAL_FLASH_Unlock();	

	FirstSector 		= GetSector(FLASH_USER_START_ADDR);
	NbOfSectors 	= GetSector(FLASH_USER_END_ADDR) - FirstSector + 1;
	
	EraseInitStruct.TypeErase     	= FLASH_TYPEERASE_SECTORS;
	EraseInitStruct.VoltageRange  	= FLASH_VOLTAGE_RANGE_3;
	EraseInitStruct.Sector        		= FirstSector;
	EraseInitStruct.NbSectors     	= NbOfSectors;
	
	if (HAL_FLASHEx_Erase(&EraseInitStruct, &SECTORError) != HAL_OK) {
		err_code = HAL_FLASH_GetError();
		HAL_FLASH_Lock();
		return err_code;
	}
	
	if ((szw_buf % 4) != 0) szw_buf += 3;
	szWordLength = szw_buf / 4;
	
	Address = FLASH_USER_START_ADDR;
	
	for (i = 0; i < szWordLength; i++, Address += 4) {
		if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, Address, pWordBuf[i]) != HAL_OK) {
			ret = 0xffffffff;
			break;
		}
	}

	HAL_FLASH_Lock();	
	
	return ret;
}


static uint32_t GetSector(uint32_t Address)
{
	uint32_t sector = 0;

	if ((Address < ADDR_FLASH_SECTOR_1) && (Address >= ADDR_FLASH_SECTOR_0)) {
		sector = FLASH_SECTOR_0;
	} else if ((Address < ADDR_FLASH_SECTOR_2) && (Address >= ADDR_FLASH_SECTOR_1)) {
		sector = FLASH_SECTOR_1;
	} else if ((Address < ADDR_FLASH_SECTOR_3) && (Address >= ADDR_FLASH_SECTOR_2)) {
		sector = FLASH_SECTOR_2; 
	} else if ((Address < ADDR_FLASH_SECTOR_4) && (Address >= ADDR_FLASH_SECTOR_3)) {
		sector = FLASH_SECTOR_3;
	} else if ((Address < ADDR_FLASH_SECTOR_5) && (Address >= ADDR_FLASH_SECTOR_4)) {
		sector = FLASH_SECTOR_4;
	} else if ((Address < ADDR_FLASH_SECTOR_6) && (Address >= ADDR_FLASH_SECTOR_5)) {
		sector = FLASH_SECTOR_5;
	} else if ((Address < ADDR_FLASH_SECTOR_7) && (Address >= ADDR_FLASH_SECTOR_6)) { 
		sector = FLASH_SECTOR_6;
	} else {
		sector = FLASH_SECTOR_7;
	}
	
	return sector;
}

