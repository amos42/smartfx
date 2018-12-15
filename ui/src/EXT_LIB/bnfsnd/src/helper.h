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

	void Free();	// ����) Free()�Լ��� ���� ȣ���� Ư���� ������ ���ؼ��� ���Ǿ���. �Ϲ������� ���� ȣ������ ����. 
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
