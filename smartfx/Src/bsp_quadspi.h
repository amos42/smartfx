#ifndef __bsp_quadspi_H
#define __bsp_quadspi_H

#ifdef __cplusplus
extern "C" {
#endif

#include "quadspi.h"
#include "n25q128a.h"

/* QSPI Error codes */
#define QSPI_OK            ((uint8_t)0x00)
#define QSPI_ERROR         ((uint8_t)0x01)
#define QSPI_BUSY          ((uint8_t)0x02)
#define QSPI_NOT_SUPPORTED ((uint8_t)0x04)
#define QSPI_SUSPENDED     ((uint8_t)0x08)


#define HAL_QPSI_TIMEOUT_DEFAULT_VALUE ((uint32_t)5000)/* 5 s */

void bsp_quadspi_test(void);
uint8_t BSP_QSPI_MemoryMappedMode(void);

#ifdef __cplusplus
}
#endif

#endif
