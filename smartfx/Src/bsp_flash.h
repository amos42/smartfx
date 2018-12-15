#ifndef __bsp_flash_H
#define __bsp_flash_H

#include "stm32f7xx_hal.h"

#define ADDR_FLASH_SECTOR_0     ((uint32_t)0x08000000) /* Base address of Sector 0, 32 Kbytes */
#define ADDR_FLASH_SECTOR_1     ((uint32_t)0x08008000) /* Base address of Sector 1, 32 Kbytes */
#define ADDR_FLASH_SECTOR_2     ((uint32_t)0x08010000) /* Base address of Sector 2, 32 Kbytes */
#define ADDR_FLASH_SECTOR_3     ((uint32_t)0x08018000) /* Base address of Sector 3, 32 Kbytes */
#define ADDR_FLASH_SECTOR_4     ((uint32_t)0x08020000) /* Base address of Sector 4, 128 Kbytes */
#define ADDR_FLASH_SECTOR_5     ((uint32_t)0x08040000) /* Base address of Sector 5, 256 Kbytes */
#define ADDR_FLASH_SECTOR_6     ((uint32_t)0x08080000) /* Base address of Sector 6, 256 Kbytes */
#define ADDR_FLASH_SECTOR_7     ((uint32_t)0x080C0000) /* Base address of Sector 7, 256 Kbytes */

#define SIZE_FLASH_SECTOR_32K	0x8000
#define SIZE_FLASH_SECTOR_128K	0x20000
#define SIZE_FLASH_SECTOR_256K	0x40000

uint8_t bsp_flash_user_data_valid(void);
uint32_t bsp_flash_user_data_read(uint8_t *r_buf, uint32_t szr_buf);
uint32_t bsp_flash_user_data_write(uint8_t *w_buf, uint32_t szw_buf);


#endif /* __bsp_flash_H */

