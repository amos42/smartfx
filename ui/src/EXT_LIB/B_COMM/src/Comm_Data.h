#ifndef __COMM_DATA_H_
#define __COMM_DATA_H_


#include "BC_Types.h"
#include "COMM_Def.h"
#include <windows.h>

#if USE_PREREAD_BUFFER
#include "_queue.h"
#endif
#include "_thread.h"


#ifdef __cplusplus
extern "C" {
#endif
	

struct stCommInfo {
	HANDLE	hCommPort, hReaderStatus ;
	DWORD_T	dwPort, dwBaudRate ;
	
	DWORD_T	dwEvtMaskOrig, dwEvtMaskNew ;

    COMMTIMEOUTS timeoutsorig ;
    COMMTIMEOUTS timeoutsnew ;

	BOOL_T	fConnected, fRsv0, fRsv1, fRsv2  ;
	
	DWORD_T   dwPortStatus;
} ;


typedef struct __TComm {
	UINT_T		nCommType;
	
	TCHAR_T		szCommPort[32];
	UINT_T		nBaudRate;

	BOOL_T		bActivate;
	BOOL_T		bConnected;

	COMM_CALLBACK_FUNC *lpFunc;

	HANDLE		hCommDev;
	BOOL_T		bCommConnected;
	THREAD_T		hCommThread;
	DWORD_T		dwEvtMaskSig, dwEvtMaskNew ;
    COMMTIMEOUTS timeouts, timeoutsnew ;
#if OVERLAPPED_MODE
	// In PC, use overlapped IO. 
	OVERLAPPED		m_stOverlap;
#endif

#if USE_PREREAD_BUFFER
	COMM_Q      tReadQueue;
	CHAR_T		arReadBuffer[MAX_READ_BUFFER * 4];
	CSECTION_T  hCSection;
#endif

	HANDLE		hSimulFile;
	THREAD_T	hSimulThread;
	long 		nSimulInterval;
	long 		nSimulBlkSize;
	long			nSimulFileSize;

	HANDLE		hLogFile;
	BOOL_T		bProcessLogging;

	DWORD_T		dwUserTag;
} _TComm;


#ifdef __cplusplus
}
#endif


#endif
