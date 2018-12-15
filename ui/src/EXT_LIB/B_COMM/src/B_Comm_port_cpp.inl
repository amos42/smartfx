#include "B_Comm.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <windows.h>


#include "COMM_Def.h"
#include "COMM_Data.h"

#include "_heap.h"
#include "_thread.h"


static BOOL_T COMM_SetDCB (COMM_T hComm);
static BOOL_T	COMM_SetTimeOuts (COMM_T hComm);
static DWORD_T readCommGPSProc (void *lpParam);



BOOL_T COMM_InitCommPort( _TComm *cmm )
{
	cmm->hCommDev = atNULL;
	cmm->lpFunc = atNULL;
	cmm->hCommThread = atNULL;

	cmm->bCommConnected = FALSE;

#if OVERLAPPED_MODE
	ZeroMemory(&cmm->m_stOverlap, sizeof(OVERLAPPED));
#endif 

#if USE_PREREAD_BUFFER
	cmm->hCSection = NULL;

	COMM_InitQueue( &cmm->tReadQueue, cmm->arReadBuffer, sizeof(cmm->arReadBuffer) );
#endif

	return TRUE;
}

/*-----------------------------------------------------------------------------

FUNCTION: SetupCommPort( void )

PURPOSE: Setup Communication Port with our settings

RETURN: 
    Handle of comm port is successful
    NULL is error occurs

HISTORY:   Date:      Author:     Comment:
           10/27/95   AllenD      Wrote it

-----------------------------------------------------------------------------*/
BOOL_T COMM_OpenCommPort( _TComm *cmm, LPTSTR_T szCommPort, UINT_T nBaudRate )
{
	if( cmm == atNULL ) return FALSE;
    if( cmm->bCommConnected ) return FALSE;

	COMM_strncpy(cmm->szCommPort, szCommPort, 32);
	cmm->nBaudRate     = nBaudRate;
	cmm->bCommConnected = FALSE;
	
	if ( (cmm->hCommDev != INVALID_HANDLE_VALUE) && (cmm->hCommDev != NULL) ){
		return FALSE;
	}

#if OVERLAPPED_MODE
	cmm->hCommDev = CreateFile ( (LPCTSTR)cmm->szCommPort,  
									GENERIC_READ | GENERIC_WRITE, // GENERIC_READ,
									0, 
									NULL, 
									OPEN_EXISTING,
									FILE_FLAG_OVERLAPPED,	// Overlapped IO
									NULL ) ;

#else
	cmm->hCommDev = CreateFile ( (LPCTSTR)cmm->szCommPort,  
									GENERIC_READ | GENERIC_WRITE, // GENERIC_READ,
									0, 
									NULL, 
									OPEN_EXISTING,
									FILE_ATTRIBUTE_NORMAL,	// CE 환경에서는 Overlapped 를 지원하지 않는다..
									NULL ) ;
#endif

	if ( cmm->hCommDev == INVALID_HANDLE_VALUE ){   
#ifdef _DEBUG
		atDWORD err_code = GetLastError();
#endif
		return FALSE;
	}

/*
	// Save original comm timeouts and set new ones
	if ( !GetCommTimeouts(cmm->hCommDev, &cmm->timeouts ) ){
		if ( cmm->hCommDev != INVALID_HANDLE_VALUE ){
			CloseHandle( cmm->hCommDev );
			cmm->hCommDev = INVALID_HANDLE_VALUE ;
		}

		return FALSE;
	}
*/

	// DCB Setting ..... (Set port state)
	if ( !COMM_SetDCB(cmm) )
	{
		//MessageBox ( ghwndMain, _T("setDCB() Fail"), _T("setDCB"), MB_OK ) ;
	}

	// Timeouts Setting ..
	if ( !COMM_SetTimeOuts (cmm) )
	{
		//MessageBox ( ghwndMain, _T("setTimeOuts() Fail"), _T("setTimeOuts"), MB_OK ) ;
	}

#if OVERLAPPED_MODE
   cmm->m_stOverlap.hEvent = CreateEvent(
        NULL,   // default security attributes 
        TRUE,   // manual-reset event 
        FALSE,  // not signaled 
        NULL    // no name
		);
    
    // Initialize the rest of the OVERLAPPED structure to zero.
    cmm->m_stOverlap.Internal = 0;
    cmm->m_stOverlap.InternalHigh = 0;
    cmm->m_stOverlap.Offset = 0;
    cmm->m_stOverlap.OffsetHigh = 0;
#endif // #if WIN_PC

	// set comm buffer sizes
	SetupComm ( cmm->hCommDev, MAX_READ_BUFFER, MAX_WRITE_BUFFER ) ;

#if USE_PREREAD_BUFFER
	cmm->hCSection = COMM_CreateCSection();
#endif

	cmm->hCommThread = COMM_CreateThread( readCommGPSProc, (void *)cmm );
	if( cmm->hCommThread == NULL ){

	#if USE_PREREAD_BUFFER
		COMM_DestroyCSection( cmm->hCSection );
	#endif

		return FALSE;
	}
	

	cmm->bCommConnected = TRUE;

	return TRUE;
}


/*-----------------------------------------------------------------------------

FUNCTION: closeComm

PURPOSE: Closes a connection to a comm port

RETURN:
    TRUE  - successful breakdown of port
    FALSE - port isn't connected

COMMENTS: Waits for threads to exit,
          clears DTR, restores comm port timeouts, purges any i/o
          and closes all pertinent handles

HISTORY:   Date:      Author:     Comment:
           10/27/95   AllenD      Wrote it

-----------------------------------------------------------------------------*/
BOOL_T COMM_CloseCommPort( _TComm *cmm )
{
	if( cmm == atNULL ) return FALSE;
	if( cmm->hCommDev == INVALID_HANDLE_VALUE ) return TRUE;

    if (!cmm->bCommConnected) return TRUE ;

#if OVERLAPPED_MODE
	SetEvent(cmm->m_stOverlap.hEvent);
#endif

    cmm->bConnected = FALSE;
    SetCommMask ( cmm->hCommDev, 0x0000 ) ;  // 블러킹 풀기 위해

#if OVERLAPPED_MODE
    // restore original comm timeouts
    if (!SetCommTimeouts(cmm->hCommDev,  &cmm->timeouts))
	{
		// error handling ..
	}

    // Purge reads/writes, input buffer and output buffer
    if (!PurgeComm(cmm->hCommDev, PURGE_TXABORT | PURGE_RXABORT | PURGE_TXCLEAR | PURGE_RXCLEAR))
	{
		// error handling ..
	}
#endif

	CloseHandle ( cmm->hCommDev ) ;
	cmm->hCommDev = NULL;
//	READSTATTHREAD(m_CommInfo) = INVALID_HANDLE_VALUE ;

	COMM_DestroyThread( cmm->hCommThread );
	
#if USE_PREREAD_BUFFER
	COMM_DestroyCSection( cmm->hCSection );
#endif

    return TRUE;
}

/*-----------------------------------------------------------------------------

FUNCTION: setDCB ( void )

PURPOSE: Sets port state based on settings from the user

COMMENTS: Sets up DCB structure and calls SetCommState.
	  Sets up new timeouts by calling SetCommTimeouts.

HISTORY:   Date:      Author:     Comment:
	    1/ 9/96   AllenD      Wrote it

-----------------------------------------------------------------------------*/
static BOOL_T COMM_SetDCB (COMM_T hComm)
{
	DCB dcb;
	_TComm *cmm = (_TComm *)hComm;
	if( cmm == atNULL ) return FALSE;

	dcb.DCBlength = sizeof(DCB);
	
	// get current DCB settings
	if (!GetCommState(cmm->hCommDev, &dcb))
	{
		// error handling ..
	}

	dcb.DCBlength = sizeof(DCB);
	dcb.BaudRate		= cmm->nBaudRate;
	dcb.fParity			= FALSE;
	dcb.fNull			= FALSE;
	dcb.StopBits		= ONESTOPBIT;
	dcb.Parity			= NOPARITY;
	dcb.ByteSize		= 8;
	dcb.fBinary			= TRUE;	// 특히 Win32 환경에서는 반드시 TRUE로 ..

//	dcb.fRtsControl = RTS_CONTROL_HANDSHAKE;
//	dcb.fDtrControl = DTR_CONTROL_HANDSHAKE;
//	dcb.fRtsControl = RTS_CONTROL_DISABLE;
//	dcb.fDtrControl = DTR_CONTROL_DISABLE;

	if ( !SetCommState(cmm->hCommDev, &dcb) ) 
	{
		// error handling ...
		return FALSE ;
	}

	return TRUE;
}


static BOOL_T	COMM_SetTimeOuts (COMM_T hComm)
{
	_TComm *cmm = (_TComm *)hComm;
	if( cmm == atNULL ) return FALSE;

	COMMTIMEOUTS *	timeouts = &cmm->timeouts;

	timeouts->ReadIntervalTimeout			= MAXDWORD ;
	timeouts->ReadTotalTimeoutMultiplier	= 0 ;
	timeouts->ReadTotalTimeoutConstant		= 0 ;
	timeouts->WriteTotalTimeoutMultiplier	= 2 ;		// 보낸 문자열길이 * 2 = a
	timeouts->WriteTotalTimeoutConstant	= 1000 ;	// a + 1초 가 타임아웃..

    if ( !SetCommTimeouts(cmm->hCommDev, &cmm->timeoutsnew ) )
	{
		// error handling ....
		return FALSE ;
	}
	return TRUE ;
}


DWORD_T COMM_ReadCommPort (_TComm *cmm, void * buf , DWORD_T len)
{
	DWORD_T readsize = 0;

	if( cmm == atNULL ) return 0;
	if( buf == atNULL ) return 0;
	if ( !cmm->bCommConnected )	return 0;

	if( len > MAX_READ_BUFFER ) len = MAX_READ_BUFFER;

#if USE_PREREAD_BUFFER
	if( COMM_GetQueueFillSize(&cmm->tReadQueue) > 0 ){
		COMM_EnterCSection( cmm->hCSection );
		readsize = COMM_Dequeue( &cmm->tReadQueue, buf, len, TRUE, TRUE );
		COMM_LeaveCSection( cmm->hCSection );

		if( readsize > 0 ){
			if( readsize >= len ) return readsize;
			buf = (void *)*((char *)buf + readsize);
			len -= readsize;
		}
	}
#endif

	BOOL_T ret = ReadFile ( cmm->hCommDev, buf, len, &len, NULL ) ;
	readsize += len;

	// Note that normally the code will not execute the following
	// because the driver caches write operations. Small I/O requests
	// (up to several thousand bytes) will normally be accepted
	// immediately and WriteFile will return true even though an
	// overlapped operation was specified

	if ( !ret )
	{
#if OVERLAPPED_MODE
		if ( GetLastError() == ERROR_IO_PENDING )
		{
			// We should wait for the completion of the write operation
			// so we know if it worked or not

			// This is only one way to do this. It might be beneficial to
			// place the write operation in a separate thread
			// so that blocking on completion will not negatively
			// affect the responsiveness of the UI
	
			// If the write takes too long to complete, this
			// function will timeout according to the
			// CommTimeOuts.WriteTotalTimeoutMultiplier variable.
			// This code logs the timeout but does not retry
			// the write.
			//--------- timeouts에 정해준 시간만큼 기다려준다.
			while (! GetOverlappedResult( cmm->hCommDev, &m_osRead, &dwRead, TRUE))
			{
				 dwError = GetLastError();
				 if (dwError != ERROR_IO_INCOMPLETE)
				 {
					  ClearCommError( cmm->hCommDev, &dwErrorFlags, &comstat);
					  break;
				 }
			}
		}
		else
#endif
		{
			// some other error occurred

			DWORD_T		dwError ;
			COMSTAT		comStat ;

			BOOL_T	errRet = ClearCommError ( cmm->hCommDev, &dwError, &comStat ) ;
		}

		return 0;
	}


	return readsize;
}


DWORD_T COMM_WriteCommPort (_TComm *cmm, const void * buf , int len)
{
	if( cmm == atNULL ) return 0;
	if( buf == atNULL ) return 0;
	if ( !cmm->bCommConnected )	return 0;

	DWORD_T	nWrite;// nSend, ;

	BOOL_T ret = WriteFile ( cmm->hCommDev, buf, len, &nWrite, NULL ) ;

	// Note that normally the code will not execute the following
	// because the driver caches write operations. Small I/O requests
	// (up to several thousand bytes) will normally be accepted
	// immediately and WriteFile will return true even though an
	// overlapped operation was specified

	if ( !ret )
	{
		if ( GetLastError() == ERROR_IO_PENDING )
		{
			// We should wait for the completion of the write operation
			// so we know if it worked or not

			// This is only one way to do this. It might be beneficial to
			// place the write operation in a separate thread
			// so that blocking on completion will not negatively
			// affect the responsiveness of the UI
	
			// If the write takes too long to complete, this
			// function will timeout according to the
			// CommTimeOuts.WriteTotalTimeoutMultiplier variable.
			// This code logs the timeout but does not retry
			// the write.
		}
		else
		{
			// some other error occurred

			DWORD_T		dwError ;
			COMSTAT		comStat ;

			BOOL_T	errRet = ClearCommError ( cmm->hCommDev, &dwError, &comStat ) ;

			return 0;
		}
	}


	return len;
}


//COMSTAT	stat ;

static DWORD_T readCommGPSProc (void *lpParam)
{
	COMM_T hComm = (COMM_T)lpParam;

	_TComm *cmm = (_TComm *)hComm;
	if( cmm == atNULL ) return 0;

	DWORD_T	dwMask;
	BOOL_T	ret ;
	DWORD	error ;
	COMSTAT	stat ;
#if USE_PREREAD_BUFFER
	CHAR_T tempbuf[MAX_READ_BUFFER];
#endif

//	cmm->dwEvtMaskNew = EV_RXCHAR | EV_ERR | EV_BREAK | EV_CTS | EV_DSR | EV_RLSD | EV_RXFLAG | EV_TXEMPTY ;
	cmm->dwEvtMaskNew = EV_RXCHAR | EV_ERR | EV_BREAK | EV_TXEMPTY;

	// 기존의 Event Mask 를 저장 ...
	GetCommMask ( cmm->hCommDev, &cmm->dwEvtMaskSig ) ;
	SetCommMask ( cmm->hCommDev, cmm->dwEvtMaskNew ) ;

	dwMask = cmm->dwEvtMaskNew;
//	dwMask = 0x00;

	while (cmm->bActivate && cmm->bConnected && (cmm->hCommDev != NULL) && (cmm->hCommDev != INVALID_HANDLE_VALUE) )
	{
        // check-me
        // PC버젼에서 OVERLAPPED모드로 정상작으로 작동되지 않음(FALSE반환)
        // 일단 NON OVERLAPPED모드로 동작하도록 변경함.
        // TEST요망

#if OVERLAPPED_MODE
		ASSERT(cmm->m_stOverlap.hEvent);
		ret = WaitCommEvent(cmm->hCommDev, &dwMask, &cmm->m_stOverlap);
#else
		ret = WaitCommEvent(cmm->hCommDev, &dwMask, NULL);
#endif
		if( !ret ) continue;

		if (dwMask & EV_RXCHAR )
		{
			dwMask &= ~EV_RXCHAR;
   			ret = ClearCommError( cmm->hCommDev, &error, &stat );	  

			/* check-me
				퀄컴GPS모듈을 사용하는 WIndows Mobile에서는 stat.cbInQue이 비정상적으로
				들어 온다.
			*/

			if( ret ){
#if USE_PREREAD_BUFFER

				DWORD_T readsize;
				do {
					if( COMM_GetQueueFreeSize(&cmm->tReadQueue) < MAX_READ_BUFFER ) break;
					readsize = MAX_READ_BUFFER;

					ret = ReadFile( cmm->hCommDev, tempbuf, readsize, &readsize, NULL );

					if( ret ){
						COMM_EnterCSection( cmm->hCSection );
						COMM_Enqueue( &cmm->tReadQueue, tempbuf, readsize, TRUE );
						COMM_LeaveCSection( cmm->hCSection );
						
						if( cmm->lpFunc ){
							cmm->lpFunc( hComm, COMM_MSG_INQUEUE, (long)COMM_GetQueueFillSize(&cmm->tReadQueue), 0 );
						}		
					} else {
						readsize = 0;
					}
				} while( readsize > 0 );

#else
				if( stat.cbInQue > 0 ){
					if( cmm->lpFunc ){
						cmm->lpFunc( hComm, COMM_MSG_INQUEUE, (long)stat.cbInQue, 0 );
					}		
				}
#endif
			}
		}

		if (dwMask & EV_BREAK )
		{
			dwMask &= ~EV_BREAK;
			ClearCommBreak ( cmm->hCommDev ) ;
		}

		if (dwMask & EV_ERR )
		{
			dwMask &= ~EV_ERR;
   			ret = ClearCommError( cmm->hCommDev, &error, &stat );	  

			/*
			if( error & CE_RXOVER ) SYSAPI_DebugPrint( _AT("CE_RXOVER,") );
			if( error & CE_OVERRUN ) SYSAPI_DebugPrint( _AT("CE_OVERRUN,") );
			if( error & CE_RXPARITY ) SYSAPI_DebugPrint( _AT("CE_RXPARITY,") );
			if( error & CE_FRAME ) SYSAPI_DebugPrint( _AT("CE_FRAME,") );
			if( error & CE_BREAK ) SYSAPI_DebugPrint( _AT("CE_BREAK,") );
			if( error & CE_TXFULL ) SYSAPI_DebugPrint( _AT("CE_TXFULL,") );
			if( error & CE_PTO ) SYSAPI_DebugPrint( _AT("CE_PTO,") );
			if( error & CE_IOE ) SYSAPI_DebugPrint( _AT("CE_IOE,") );
			if( error & CE_DNS ) SYSAPI_DebugPrint( _AT("CE_DNS,") );
			if( error & CE_OOP ) SYSAPI_DebugPrint( _AT("CE_OOP,") );
			if( error & CE_MODE ) SYSAPI_DebugPrint( _AT("CE_MODE,") );
			SYSAPI_DebugPrint( _AT("\r\n") );
			*/
		}
	}

	SetCommMask ( cmm->hCommDev, cmm->dwEvtMaskSig );

	return 0;
}

