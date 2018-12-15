#ifndef __pkt_H
#define __pkt_H

#ifdef __cplusplus
extern "C" {
#endif

#include "bsp_ble.h"
#include "bsp_gpio.h"
	
int8_t pkt_merge(uint8_t *pPktBuf, uint8_t szPkt);
int8_t frm_parser(uint8_t *pFrmBuf, uint8_t szFrm);
void pkt_escaping(uint8_t *pInPtr, uint8_t szInPtr, uint8_t *pOutPtr, uint8_t *pszOutPtr);
//void pkt_deescaping(uint8_t *pInPtr, uint8_t szInPtr, uint8_t *pOutPtr, uint8_t *pszOutPtr);	
int8_t pkt_parser(uint8_t *pPktBuf, uint8_t szPkt);
int8_t frm_tx_send(uint8_t *pPktBuf, uint8_t szPkt);


#ifdef __cplusplus
}
#endif

#endif // __pkt_H
