#include "pkt.h"
#include "threads.h"

#define PACKED	__packed

#define D_FRM_REQ_TX											0x01
#define D_FRM_IND_RX											0x01
#define D_FRM_RSP_ACK										0x03
#define D_FRM_REQ_ADV_ON								0x04
#define D_FRM_REQ_ADV_OFF							0x05
#define D_FRM_IND_CONNECT							0x06
#define D_FRM_REQ_SET_DEV_NAME				0x07
#define D_FRM_IND_DISCONNECT						0x08
#define D_FRM_REQ_REBOOT								0x09
#define D_FRM_IND_BOOT										0x0a
#define D_FRM_REQ_GET_DEV_NAME				0x0b
#define D_FRM_RSP_MY_DEV_NAME				0x0c
#define D_FRM_REQ_DISCONNECT					0x0f
#define D_FRM_REQ_PWRSAVE							0x11
#define D_FRM_REQ_CON_TIME_MAX				0x12
#define D_FRM_REQ_SET_SPV_TIME				0x13
#define D_FRM_REQ_GET_VER							0x14
#define D_FRM_RSP_VER										0x15
#define D_FRM_REQ_CON_TIME_MIN				0x17
#define D_FRM_REQ_ADV_ON_ALL					0x18
#define D_FRM_REQ_SET_ADV_INTDUR			0x19

typedef PACKED union {
	uint8_t buf[35];
	PACKED struct { uint8_t stx, len, cls, next[1];		} frm, chk, rx;
	PACKED struct { uint8_t len, cls, next[1];				} tx;
	PACKED struct { uint8_t stx, len, cls, etx;				} ack, adv_on, adv_off, conn, disconn, boot;
} FRM_T; // frame

typedef PACKED union {
	uint8_t buf[35];
	PACKED struct { uint8_t sop, len, cls, next[1]; 											} pkt, chk;

	PACKED struct { uint8_t sop, len, cls, data[2], eop;									} ping;
	PACKED struct { uint8_t sop, len, cls ; uint16_t data; uint8_t  eop;		} bat;			// battery check : original
	PACKED struct { uint8_t sop, len, cls, id; uint16_t data; uint8_t  eop;	} adc;
	PACKED struct { uint8_t sop, len, cls, id, lumi, time, eop;						} led;
	PACKED struct { uint8_t sop, len, cls, time, eop; 										} off;
} PKT_T; // packet

#define D_PKT_SOP				0xf2
#define D_PKT_EOP				0xf3
#define D_PKT_DLE				0xf0
#define D_PKT_MSK				0x0f


#define D_PKT_PING						'P'		//0x50
#define D_PKT_BAT						'B'		//0x42

#define D_PKT_ADC						'A'		//0x41 
#define D_ADC_BAT						0			// <-- adc packet의 id
#define D_ADC_TEMP					1
#define D_ADC_VREF					2

#define D_PKT_LS_G					'G'		//0x47

#define D_PKT_LED						'L'		//0x4C
#define D_PKT_OFF						'O'		//0x4F

const uint8_t pkt_err[7] 				= {D_PKT_SOP, 0x05, 'R', 'E', 'R', 'R', D_PKT_EOP};
const uint8_t pkt_unknown[7]	= {D_PKT_SOP, 0x05, 'R', 'U', 'N', 'K', D_PKT_EOP};




#define PKT_TX_N(d, s)		bsp_uart_tx(&huart2, d, s)

int8_t frm_parser(uint8_t *pFrmBuf, uint8_t szFrm)
{
	//static uint8_t sts_boot = 0;
//	uint16_t i;
	uint8_t cls, frm_ctx_len, frm_pl_len;
//	uint8_t *pPkt;
	
	FRM_T *pFrm	= (FRM_T *)pFrmBuf;
	frm_ctx_len = szFrm - 2; // stx, ext 빼고 frame의 길이
	
	if (pFrm->chk.len != frm_ctx_len) return -1; // 길이가 같은가?
	if ((pFrmBuf[0] != D_FRM_STX) && (pFrmBuf[szFrm-1] != D_FRM_ETX)) return -2;	 // 제일 앞과 끝이 STX, ETX인가?
	
	cls = pFrm->chk.cls;
	
	//printf("[cls=0x%02x,len=%d]", cls, szFrmLen);
	
	switch (cls)  {
		case D_FRM_RSP_ACK: {
			//putchar('A');

		} break;
		
		case D_FRM_IND_RX: {

			frm_pl_len = frm_ctx_len - 2; // frame의 payload 부분(len과 cls 길이 빼고) packet(SOP부터 EOP까지)의 길이
				
//			{
//				uint8_t i;
//				printf("[rx]");
//				for (i=0; i<frm_pl_len;i++) printf("%02x ", pFrm->rx.next[i]);
//				printf("\r\n");
//			}

			pkt_merge(pFrm->rx.next, frm_pl_len);
			
		} break;
		
		case D_FRM_IND_CONNECT: {
			//osSignalSet (D_PARSER_THREAD, 0x00000002); 
		} break;

		case D_FRM_IND_DISCONNECT: {	
		//	ble_adv_on_n_low_latency();
				
		} break;
		
		case D_FRM_IND_BOOT: {

		} break;
		
/*
		default: {
			printf("[");
			for (i=0; i<pkt_len; i++) { printf("%02x ", pFrm->buf[i]); }
			printf("]\r\n");
		}
*/
	}
	
	return 0;
}

#if 1

void pkt_escaping(uint8_t *pInPtr, uint8_t szInPtr, uint8_t *pOutPtr, uint8_t *pszOutPtr)
{
	uint8_t i,j;
	
	j = 0;
	pOutPtr[j++] = pInPtr[0];
	
	for (i=1; i<szInPtr-1; i++)
	{
		if ((pInPtr[i] == D_PKT_SOP) || (pInPtr[i] == D_PKT_EOP) || (pInPtr[i] == D_PKT_DLE)) {
			pOutPtr[j++] = D_PKT_DLE;
			pOutPtr[j++] = pInPtr[i] & D_PKT_MSK;
		} else {
			pOutPtr[j++] = pInPtr[i];
		}
	}
	
	pOutPtr[j++] = pInPtr[i++];
	
	*pszOutPtr = j;
}

//void pkt_deescaping(uint8_t *pInPtr, uint8_t szInPtr, uint8_t *pOutPtr, uint8_t *pszOutPtr)
//{
//	uint8_t i,j;
//	uint8_t f_dle = 0;
//	
//	j = 0;
//	pOutPtr[j++] = pInPtr[0];
//	
//	for (i=1; i<szInPtr-2; i++)
//	{
//		if (f_dle == 0) {
//			if (pInPtr[i] == D_PKT_DLE) {
//				f_dle = 1;
//			} else {
//				pOutPtr[j++] = pInPtr[i];
//			}
//		} else {
//			pOutPtr[j++] = D_PKT_DLE | pInPtr[i];
//			f_dle = 0;
//		}
//	}
//	
//	pOutPtr[j++] = pInPtr[i++];

//	*pszOutPtr = j;
//}


#define D_MAX_MERG		60
#define D_LIM_MERG		58

int8_t pkt_merge(uint8_t *pPktBuf, uint8_t szPkt)
{
	static uint8_t mBuf[D_MAX_MERG];
	static uint8_t curIdx = 0;
	static uint8_t f_dle = 0;
	uint8_t i;
	
//	{
//		uint8_t t;
//		printf("[rx]");
//		for (t=0; t<szPkt;t++) printf("%02x ", pPktBuf[t]);
//		printf("\r\n");
//	}

	for (i = 0; i < szPkt; i++) {
		if (curIdx > D_LIM_MERG) { f_dle = 0; curIdx = 0; }
		
		if (pPktBuf[i] == D_PKT_SOP) {
			f_dle = 0;
			curIdx = 0;
			mBuf[curIdx++] = pPktBuf[i];
			
		} else if (pPktBuf[i] == D_PKT_EOP) {
			mBuf[curIdx++] = pPktBuf[i];
			
			pkt_parser(mBuf, curIdx);
		} else {
			if (f_dle == 0) {
				if (pPktBuf[i] == D_PKT_DLE) {
					f_dle = 1;
				} else {
					mBuf[curIdx++] = pPktBuf[i];
				}
			} else {
				mBuf[curIdx++] = D_PKT_DLE | pPktBuf[i];
				f_dle = 0;
			}
		}		
	}
	
	return 0;
}


//static uint8_t EscBuf[60], szEscBuf;
//static uint8_t DeEscBuf[60], szDeEscBuf;


#define D_LED_LUMI_MAX			100

int8_t pkt_parser(uint8_t *pPktBuf, uint8_t szPkt)
{
//	uint16_t i;
	uint8_t cls, pkt_ctx_len; //,pkt_pl_len;
//	uint8_t buf[35];
	
	PKT_T *pPkt	= (PKT_T *)pPktBuf;
	pkt_ctx_len = szPkt - 2;			// sop, eop를 제외한 packet 길이
	//pkt_pl_len = pkt_ctx_len - 2;	// len, cls 제외한 payload 길이
	
	if (pPkt->chk.len != pkt_ctx_len) return -1;
	if ((pPktBuf[0] != D_PKT_SOP) && (pPktBuf[szPkt-1] != D_PKT_EOP)) return -2;
	
	cls = pPkt->chk.cls;
	
//	printf("pkt.cls=%02x\r\n", cls);
	
	switch (cls)  {
		case D_PKT_PING: {
//			printf("[ping]");
//			printf("\r\n");			
			static uint8_t ping = 0;			
			
//			if (HAL_GPIO_ReadPin(D_LED_EXT_DET_GPIO_Port, D_LED_EXT_DET_Pin) == 0) { // 외부 Flash LED 모듈 장착 여부
//				pPkt->ping.data[0] = 1;
//			} else {
//				pPkt->ping.data[0] = 0;
//			}
			
			pPkt->ping.data[0] = 1;
			pPkt->ping.data[1] = ping++;
			
			frm_tx_send(pPktBuf, szPkt);
			gAppObj.f_update = 1;
		} break;
		
		case D_PKT_BAT: {
			//printf("[bat]");
			//for (i=0; i<szPkt; i++) printf("%02x ", pPktBuf[i]);
			//printf("\r\n");		
			
			pPkt->bat.data = gAppObj.adc_val[0];

			//for (i=0; i<szPkt; i++) printf("%02x ", pPktBuf[i]);
			//printf("\r\n");		
			frm_tx_send(pPktBuf, szPkt);
			gAppObj.f_update = 1;
		} break;

		case D_PKT_ADC: 
		{
			if (pPkt->adc.id < D_ADC_CH_MAX) {
			
				pPkt->adc.data = gAppObj.adc_val[pPkt->adc.id];

				frm_tx_send(pPktBuf, szPkt);
			} else {
				frm_tx_send((uint8_t *)pkt_unknown, 7);
			}
			
			gAppObj.f_update = 1;
		} break;
		
		case D_PKT_LS_G: 	// Light Source Gain 
		{ 
			bsp_ls_g_set(pPkt->ls_g.data);			
			frm_tx_send(pPktBuf, szPkt);
		
			gAppObj.f_update = 1;
		} break;

		case D_PKT_LED: {
			
			
			if (pPkt->led.id == 0) {
			
				gAppObj.led0_time = pPkt->led.time;  

				if (pPkt->led.lumi == 0) {
					bsp_flash_stop(0);
				} else {
					if (pPkt->led.lumi > D_LED_LUMI_MAX) bsp_flash_start(0, D_LED_LUMI_MAX);
					else bsp_flash_start(0, pPkt->led.lumi);
				}				
				
				frm_tx_send(pPktBuf, szPkt);
			} else if (pPkt->led.id == 1) {
				gAppObj.led1_time = pPkt->led.time;

				if (pPkt->led.lumi == 0) {
					bsp_flash_stop(1);
				} else {
					if (pPkt->led.lumi > D_LED_LUMI_MAX) bsp_flash_start(1, D_LED_LUMI_MAX);
					else bsp_flash_start(1, pPkt->led.lumi);
				}

				frm_tx_send(pPktBuf, szPkt);
				
			} else {
				frm_tx_send((uint8_t *)pkt_unknown, 7);
			}
			
			gAppObj.f_update = 1;
		} break;
		
		case D_PKT_OFF: {
			gAppObj.off_time = pPkt->off.time;
			frm_tx_send(pPktBuf, szPkt);
			gAppObj.f_update = 0;
		} break;
		
		default: {
			//printf("[");
			//for (i=0; i<szPkt; i++) { printf("%02x ", pPktBuf[i]); }
			//printf("]\r\n");
			frm_tx_send((uint8_t *)pkt_unknown, 7);
			gAppObj.f_update = 1;

		}
	}
	
	return 0;	
}

#endif

#define BLE_Tx(ch)	HAL_UART_Transmit(&huart2, (uint8_t *)&ch, 1, 100)

// in, in
int8_t frm_tx_send(uint8_t *pPktBuf, uint8_t szPkt)
{
	uint8_t c, i;
	FRM_T *pFrm	= (FRM_T *)gBleObj.f;
	
	while (gBleObj.f_sz != 0);	// frame 크기가.. 0인가? frame buffer 비어있나?
	gBleObj.f_sz = 1; // buffers for frame is occupied 
	
	pFrm->tx.len = szPkt + 2;  // len, cls 길이.
	pFrm->tx.cls = D_FRM_REQ_TX;
	
	for (i=0; i<szPkt; i++) pFrm->tx.next[i] = pPktBuf[i];
	
	c = D_FRM_STX;
	BLE_Tx(c);
	
	//printf("[tx]");
	
	for (i=0; i<pFrm->tx.len; i++) {
		c = pFrm->buf[i];

		//printf("%02x ", c); 

		if ((c >= 0xc0) && (c <= 0xcf)) { //(c == D_FRM_STX) || (c == D_FRM_ESC) || (c == D_FRM_ETX)) {
			c = D_FRM_ESC;
			BLE_Tx(c);
			c &= 0x0f;
			BLE_Tx(c);
		} else {
			BLE_Tx(c);
		}
	}
	
	//printf("\r\n");

	c = D_FRM_ETX;
	BLE_Tx(c);
	
	gBleObj.f_sz = 0;
	
	return 0;
}
