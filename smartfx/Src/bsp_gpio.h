#ifndef __bsp_gpio_H
#define __bsp_gpio_H

#ifdef __cplusplus
extern "C" {
#endif

#include "gpio.h"

void bsp_led_grn_on(void);
void bsp_led_grn_off(void);
void bsp_led_grn_toggle(void);
void bsp_led_blue_on(void);
void bsp_led_blue_off(void);
void bsp_led_blue_toggle(void);
void bsp_pwr_hold_on(void);
void bsp_pwr_hold_off(void);
void bsp_led_nor_on(void);
void bsp_led_nor_off(void);
void bsp_led_nor_toggle(void);


#define LED_BLE_ON()							bsp_led_blue_on()
#define LED_BLE_OFF()						bsp_led_blue_off()
#define LED_BLE_TOG()						bsp_led_blue_toggle()

#define LED_IND_ON()							bsp_led_grn_on()
#define LED_IND_OFF()						bsp_led_grn_off()
#define LED_IND_TOG()						bsp_led_grn_toggle()

#define PWR_HOLD_ON()					bsp_pwr_hold_on()
#define PWR_HOLD_OFF()					bsp_pwr_hold_off()

#define LED_NOR_ON()						bsp_led_nor_on()
#define LED_NOR_OFF()						bsp_led_nor_off()
#define LED_NOR_TOG()					bsp_led_nor_toggle()

#ifdef __cplusplus
}
#endif

#endif
