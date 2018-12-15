#ifndef __bsp_ble_H
#define __bsp_ble_H

#ifdef __cplusplus
extern "C" {
#endif

#include "bsp_usart.h"
#include "bsp_gpio.h"
	
//#define D_EVT_PKT_0									0x0001
//#define D_EVT_PKT_1									0x0002
//#define D_EVT_PKT_2									0x0004

#define D_BT_PKT_TIMEOVER				100			// 100 ms

#define D_BT_F_SZ									80			// frame 크기

#define D_BT_P_NO									3
#define D_BT_P_SZ									60			// packet 크기

typedef struct 
{
	uint8_t 		f[D_BT_F_SZ]; // Tx 프레임 버퍼, 
	uint16_t 	f_sz;	// Tx 프레임 길이(0일 때만 프레임 버퍼 사용 가능)

	uint8_t 		p[D_BT_P_NO+1][D_BT_P_SZ];	// Rx 패킷 버퍼
	uint8_t 		p_sz[D_BT_P_NO];		// 패킷 코드, 패킷 크기
	
	uint8_t		p_id;
	uint8_t 		*pp, *pp_sz;
	
	uint32_t 	timeover_count;
	uint8_t		connected;
} BLE_T;


#define D_FRM_STX												0xc8
#define D_FRM_ESC												0xc7
#define D_FRM_ETX												0xc9

// os_mbx_declare(gBleMbx, 3);
extern uint32_t gBleMbx[];	
extern BLE_T gBleObj;

void ble_proc(void);
int8_t ble_init(void);
void ble_wakeup(void);
void ble_adv_on(void);
void ble_adv_endless(void);
void ble_low_latency(void);
void ble_adv_on_n_low_latency(void);
int8_t ble_connect_check(void);
void ble_reset(void);

#ifdef __cplusplus
}
#endif

#endif // __BLE_H__
