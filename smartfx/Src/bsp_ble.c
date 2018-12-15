#include "bsp_ble.h"
#include "threads.h"




BLE_T gBleObj;



uint32_t ble_usart_isr_cbf(uint32_t p0, void *p1)
{
	static uint8_t rxd;
	static uint8_t pkt_idx = 0, pkt_esc = 0;
	
	rxd = (uint8_t)p0;

	// D_BT_PKT_TIMEOVER 동안 USART(BT 연결) 인터럽트가 발생하지 않으면 frame(packet)을 받지 않았다는 것을 의미한다.
	// 따라서 timeover가 일었났다는 것을 이전 frame(packet)이 종료 되었고 새로운 frame(packet)이 들어 오지 않았다는 것이다.
	// 테스트를 통해서 D_BT_PKT_TIMEOVER값은 결정한다.
	
	// if ((rxd == 0xaa) && (gBleObj.timeover_count == D_BT_PKT_TIMEOVER)) gBleObj.sts = WAIT_FOR_START_DLE;	
		
	gBleObj.timeover_count = 0; // timer reset
	
	//putchar('*');

	if (rxd == D_FRM_STX)  
	{
		pkt_idx = 0;
		pkt_esc = 0;
		
		gBleObj.pp[pkt_idx++] = rxd;
	} 
	else if (rxd == D_FRM_ETX) 
	{
		gBleObj.pp[pkt_idx++] = rxd;

		*(gBleObj.pp_sz) = pkt_idx;
		
		osSignalSet(D_PARSER_THREAD, (D_EVT_PKT_0 << gBleObj.p_id));
		
		gBleObj.p_id++;
		if (gBleObj.p_id >= D_BT_P_NO) gBleObj.p_id = 0;
	
		gBleObj.pp = gBleObj.p[gBleObj.p_id];
		gBleObj.pp_sz = (uint8_t *)&gBleObj.p_sz[gBleObj.p_id];

	} else {
		if (pkt_idx < D_BT_P_SZ-1) {
			if (rxd == D_FRM_ESC) {
				pkt_esc = 1;
			} else {
				if (pkt_esc == 1) {
					rxd = (0xc0 | rxd);
					pkt_esc = 0;
				} 
			
				gBleObj.pp[pkt_idx++] = rxd;
			}
		}
	}

	return 0;
}


//-----------------------------------------------------------

int8_t ble_init(void)
{
	uint16_t i;
	
//	gBleObj.sts = WAIT_FOR_START_STX;
	
	gBleObj.f_sz = 0;

	for (i=0; i<D_BT_P_NO; i++) {
		gBleObj.p_sz[i] = 0;
	}	
	
	gBleObj.p_id = 0;

	gBleObj.pp = gBleObj.p[gBleObj.p_id];
	gBleObj.pp_sz = (uint8_t *)&gBleObj.p_sz[gBleObj.p_id];	
	
	gBleObj.timeover_count = 0;
	
	bsp_usart6_cbf_reg(ble_usart_isr_cbf);
	
	return 0;
	  
}


#define BLE_DELAY(t)							osDelay(t)

//------------------------------------------------------------------------
// BLE 연결 상태 체크
// high : connected, low : disconnect
// BLE module port/pin : P1.1
#define BLE_STS_GET()						HAL_GPIO_ReadPin(D_BLE_STS_GPIO_Port, D_BLE_STS_Pin)

// BLE UART sleep 상태 체크
// low : sleep, high : active
// BLE module port/pin : P1.0
#define BLE_SLEEP_GET()					HAL_GPIO_ReadPin(D_BLE_SLEEP_GPIO_Port, D_BLE_SLEEP_Pin)

// BLE UART : sleep -> wakeup
// high(default) -> low -> 2ms 이상 -> high
#define BLE_WAKEUP_SET()				HAL_GPIO_WritePin(D_BLE_WAKEUP_GPIO_Port, D_BLE_WAKEUP_Pin, GPIO_PIN_SET)
#define BLE_WAKEUP_RESET()		HAL_GPIO_WritePin(D_BLE_WAKEUP_GPIO_Port, D_BLE_WAKEUP_Pin, GPIO_PIN_RESET)

#define BLE_TX_N(d, s)						bsp_uart_tx(&huart6, d, s)				

//#define BLE_RST_SET()						HAL_GPIO_WritePin(D_BLE_RST_GPIO_Port, D_BLE_RST_Pin, GPIO_PIN_SET)
//#define BLE_RST_RESET()				HAL_GPIO_WritePin(D_BLE_RST_GPIO_Port, D_BLE_RST_Pin, GPIO_PIN_RESET)

//************************************************************************

// BLE UART가 sleep 상태 안되도록 

//void ble_reset(void)
//{
//	//BLE_AO_RESET();
//	GPIOE->BRR = GPIO_Pin_15;
//	BLE_DELAY(2);	// 20ms
//	GPIOE->BSRR = GPIO_Pin_15;
//	//BLE_AO_SET();
//}
void ble_wakeup(void)
{
	BLE_WAKEUP_RESET();
	BLE_DELAY(5);	// 10ms
	BLE_WAKEUP_SET();
	BLE_DELAY(5);
}

void ble_adv_on(void)
{
	const uint8_t  frm_adv_endless[4]	= { 0xc8, 0x02, 0x04, 0xc9};
	BLE_TX_N((uint8_t *)frm_adv_endless, 4);		
}

void ble_adv_endless(void)
{
	const uint8_t  frm_adv_endless[4]	= { 0xc8, 0x02, 0x18, 0xc9};
	BLE_TX_N((uint8_t *)frm_adv_endless, 4);		
}

void ble_adv_on_n_low_latency(void)
{
	//if (BLE_SLEEP_GET() == 0)  //  BLE usart sleep, BLE의 GPIO 읽음
	{
		
		{ 
//			const uint8_t  frm_nosleep[4]			= { 0xc8, 0x02, 0x11, 0xc9};
			const uint8_t  frm_adv_endless[4]	= { 0xc8, 0x02, 0x18, 0xc9};
			const uint8_t  frm_low_latency[7]	= { 0xc8, 0x05, 0x12, '3', '1', 0x00, 0xc9};
//			const uint8_t frm_adv_off[4] = { 0xc8, 2, 5, 0xc9};

//			printf("nosleep\r\n");
//			BLE_TX_N((uint8_t *)frm_nosleep, 4);		
//			BLE_DELAY(30);			
//			//printf("endless adv\r\n");
//			BLE_DELAY(10);			
//			BLE_TX_N((uint8_t *)frm_adv_endless, 4);		

			//printf("low latency\r\n");
//			BLE_DELAY(10);			
//			BLE_TX_N((uint8_t *)frm_low_latency,7);		
			
//			printf("adv off\r\n");
//			BLE_TX_N((uint8_t *)frm_adv_off, 4);		
//			os_dly_wait(5);						

//			BLE_TX_N((uint8_t *)frm_adv_endless, 4);		
//			BLE_DELAY(20);			
			ble_wakeup();  // IO를 이용해서 BLE 모듈 Wakeup
			printf("low latency\r\n");
			BLE_TX_N((uint8_t *)frm_low_latency,7);		
			BLE_DELAY(150);			

			ble_wakeup();  // IO를 이용해서 BLE 모듈 Wakeup
			printf("endless advertising\r\n");
			BLE_TX_N((uint8_t *)frm_adv_endless, 4);	
			BLE_DELAY(150);			
		}			
	}
}

void ble_low_latency(void)
{
	//uint8_t i;
	//if (BLE_SLEEP_GET() == 0)  //  BLE usart sleep
	{
		//for (i=0; i<2; i++) 
		{ 
			// Set endless advertising mode
			//const uint8_t  frm_nosleep[4]			= { 0xc8, 0x02, 0x11, 0xc9};
			const uint8_t  frm_low_latency[7]	= { 0xc8, 0x05, 0x12, '3', '1', 0x00, 0xc9};
			//const uint8_t  frm_spv[7]					= { 0xc8, 0x05, 0x13, '3', '1', 0x00, 0xc9};
			//const uint8_t  frm_con_tout[7]			= { 0xc8, 0x05, 0x17, '3', '1', 0x00, 0xc9};

			//BLE_DELAY(10);			
			//BLE_TX_N((uint8_t *)frm_nosleep, 4);		
			//BLE_DELAY(10);			
			//BLE_TX_N((uint8_t *)frm_adv_endless, 4);		
			//BLE_DELAY(10);			
			BLE_TX_N((uint8_t *)frm_low_latency,7);		
			BLE_DELAY(10);		
			//BLE_TX_N((uint8_t *)frm_spv,7);		
			//BLE_DELAY(10);			
			//BLE_TX_N((uint8_t *)frm_con_tout,7);		
		}			
	}
}

// 인터럽트를 사용하지 않기 때문에 
// 일정시간 간격으로 호출해서 BLE 연결 여부 체크
int8_t ble_connect_check(void)
{
	static uint8_t pre = 0, cur = 0;
	int8_t edge;
	
	cur = BLE_STS_GET();
		
	if (pre == 0 && cur == 1) 			{ edge = 1;	 	/*printf("Rising Edge\r\n");*/ } //gBleObj.connected = D_BSP_TRUE; 	}		// rising edge
	else if (pre == 1 && cur == 0)	{ edge =  0;	/*printf("Falling Edge\r\n");*/ } //gBleObj.connected = D_BSP_FALSE;	}		// falling edge
	else edge = -1;
	
	pre = cur;

	return edge;
}

