#ifndef _LOCK_H
#define _LOCK_H

#include "PlatformDefine.h"

#if _IS_PC_
	#include "../WinPC/_Lock.h"
#elif _IS_WINCE_
	#include "../WinCE4/_Lock.h"
#elif _IS_WINMOBILE_
	#include "../WM6/_Lock.h"
#elif _IS_SYMBIAN_
	#include "../S60/_Lock.h"
#else
	#error 'platform type error'
#endif

#endif // _LOCK_H
