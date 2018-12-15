#ifndef __bsp_key_H
#define __bsp_key_H

#ifdef __cplusplus
extern "C" {
#endif

#include "gpio.h"

#define KEY_NONE						0		
#define KEY_PUSH						1
#define KEY_HOLD						2	
#define KEY_RELEASE				4

uint8_t key_st_get(void);
uint32_t key_st_count(void);
uint8_t key_re_get(void);
uint32_t key_re_count(void);
uint8_t key_pwr_key_get(void);
uint32_t key_pwr_key_count(void);

#ifdef __cplusplus
}
#endif

#endif
