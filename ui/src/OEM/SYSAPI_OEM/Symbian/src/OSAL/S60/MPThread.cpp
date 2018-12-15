#include "MPThread.h"

#if _IS_SYMBIAN_
	#include "SymThread.h"
#elif _IS_PC_
	#include "OSAL/winPC/OCThread.h"
#elif _IS_WINCE_
	#include "OSAL/WinCE4/OCThread.h"
#elif _IS_WINMOBILE_
	#include "OSAL/wm6/OCThread.h"
#else
	#include "OSAL/WM6/OCThread.h"
#endif

MPThread* MPThread::CreateInstance(const TCHAR* strThreadName) //Factory Function
{
	MPThread* pThread=NULL;
#if _IS_SYMBIAN_
	pThread=(MPThread*)new(ELeave) CSymThread(strThreadName);
	if(pThread)
		return pThread;
#else
	pThread=(MPThread*)new OCThread;
#endif
	if(pThread)
		return pThread;
	else 
		return NULL;
}
