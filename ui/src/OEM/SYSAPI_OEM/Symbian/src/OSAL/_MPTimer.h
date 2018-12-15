/*
 ============================================================================
 Name		: MTimer.h
 Author	  : 
 Version	 : 1.0
 Copyright   : Your copyright notice
 Description : CMTimer declaration
 ============================================================================
 */

#ifndef __MTIMER_H__
#define __MTIMER_H__

#include "AT_Types.h"
#include <e32base.h>


class CMTimer : public CTimer
{
public:
	
    static CMTimer* NewL( atLONG atInterval, atVOID (*apfnCallBack)( void*, atVOID* ), atVOID* param, atBOOL bRepeat );
    static void Delete( CMTimer* apThis );
    
    CMTimer();
    ~CMTimer();

public: // interface functions

	void EnableTimer( atBOOL aEnableTimer );
	void ResetTimer();
    TBool GetTimerStatus() { return ibTimerStatus; }
	
    void StartTimer( atLONG aElapseTime );
    void StopTimer();
    void SetParam( atVOID *param );
    
private:

    void ConstructL( atLONG atInterval, atVOID (*apfnCallBack)( void*, atVOID* ), atVOID* param, atBOOL bRepeat );


private: // From CActive

    void RunL();

private: //Data

	TInt iElapseTime;									// elapse time (miliseconds)
	void* ipParam;
	TBool iRepeat, imyRepeat;							// iRepeat�� ����� �������̰�, imyRepeat�� timer ��ü ���� ����. 
	TBool ibProcStatus;									// proc �Լ� ���� ���� ����.
	TBool ibDeleteThis;									// ��ü ���� ����
	TBool ibTimerStatus;								// Ÿ�̸� ���� ����
	
	void (*ipfnCallBack)( void* pParam1, void* pParam2 );
};

#endif // __NTIMER_H__
