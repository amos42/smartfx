#ifndef __HELPER_H__
#define __HELPER_H__

#include "AT_Types.h"

class CCritSec
{
protected:
	void* m_csec;
	int m_cRef;
	
public:
	CCritSec();
	~CCritSec();
	
	void Lock();
	void Unlock();

	void Free();	// 주의) Free()함수의 직접 호출은 특별한 목적에 의해서만 사용되어짐. 일반적으로 직접 호출하지 말것. 
};

class CAutoLock
{
protected:
	CCritSec* m_pcsec;

public:
	CAutoLock(CCritSec* pcsec);
	~CAutoLock();
};

#endif // __HELPER_H__
