#ifndef __bsp_lcd_H
#define __bsp_lcd_H

#ifdef __cplusplus
extern "C" {
#endif

#include "i2c.h"
#include "bsp_gpio.h"

void bsp_lcd_init(void);
void bsp_lcd_enter_standby(void);
void bsp_lcd_exit_standby (void);	

void bsp_lcd_rst_enable(void);
void bsp_lcd_rst_disable(void);

void bsp_bl_enable(void);
void bsp_bl_disable(void);
int8_t bsp_bl_i2c_tx(uint8_t *pData, uint16_t Size);
int8_t bsp_bl_i2c_rx(uint8_t *pData, uint16_t Size);
void bsp_bl_set(uint8_t tbr_idx);


void bsp_ts_rst_enable(void);
void bsp_ts_rst_disable(void);


#ifdef __cplusplus
}
#endif

#endif // __BLE_H__
