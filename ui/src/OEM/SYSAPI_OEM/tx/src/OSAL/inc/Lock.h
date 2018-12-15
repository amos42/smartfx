#ifndef _LOCK_H
#define _LOCK_H

#include "PlatformDefine.h"

/**
#if _IS_PC_
	#include "../WinPC/_Lock.h"
#elif _IS_WINCE_
	#include "../WinCE4/_Lock.h"
#elif _IS_WINMOBILE_
	#include "../WM6/_Lock.h"
#elif _IS_SYMBIAN_
	#include "../S60/_Lock.h"
#elif _IS_TX_
	#include "../tx/_lock.h"
#else
	#error 'platform type error'
#endif
**/

#include "../tx/_lock.h"


#endif // _LOCK_H
