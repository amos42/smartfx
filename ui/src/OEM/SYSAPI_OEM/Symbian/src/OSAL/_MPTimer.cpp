/*
 ============================================================================
 Name		: MTimer.cpp
 Author	  : 
 Version	 : 1.0
 Copyright   : Your copyright notice
 Description : CNModel implementation
 ============================================================================
 */

#include "_MPTimer.h"


///////////////////////////////////////////////////////////////////////////////////
// constructor/destructor

CMTimer::CMTimer() 
    : CTimer( CActive::EPriorityStandard )
{  

    ipfnCallBack   = NULL;
    iElapseTime    = 0;
    ipParam        = NULL;
    iRepeat        = EFalse;
    imyRepeat      = EFalse;
    
    ibProcStatus = EFalse;
    ibDeleteThis = EFalse;
    ibTimerStatus = EFalse;
    
	CActiveScheduler::Add( this );
}

CMTimer::~CMTimer()
{
    Cancel();   
}

///////////////////////////////////////////////////////////////////////////////////
// 2nd phase constructor.

void CMTimer::ConstructL( atLONG atInterval, atVOID (*apfnCallBack)( void*, atVOID* ), atVOID* param, atBOOL bRepeat )
{
	CTimer::ConstructL();

    ipfnCallBack = apfnCallBack;
    iElapseTime  = atInterval * 1000;
    ipParam      = param;
    iRepeat      = bRepeat;
    imyRepeat    = iRepeat; 
}
  
///////////////////////////////////////////////////////////////////////////////////
// Two-phased constructor.

CMTimer* CMTimer::NewL( atLONG atInterval, atVOID (*apfnCallBack)( void*, atVOID* ), atVOID* param, atBOOL bRepeat )
{
	CMTimer* self = new ( ELeave ) CMTimer();
	CleanupStack::PushL( self );
	self->ConstructL( atInterval, apfnCallBack, param, bRepeat );
	CleanupStack::Pop( self );
	return self;    
}


void CMTimer::Delete( CMTimer* apThis )
{
	apThis->ibDeleteThis = ETrue;
	apThis->ibTimerStatus = EFalse;
	
	//apThis->Cancel();
	
	//if(apThis->IsAdded())
	//	apThis->Deque();
	
	//if( !apThis->ibProcStatus )
		delete apThis;

}


///////////////////////////////////////////////////////////////////////////////////
// After completed timer period, execution comes to RunL.
void CMTimer::RunL()
{
	if( !ipfnCallBack || (iElapseTime <= 0) )
	{
		return;
	}

	ibProcStatus = ETrue;
	ipfnCallBack( (void*)this, ipParam );
	ibProcStatus = EFalse;

	if( ibDeleteThis )
	{
		//delete this;
		return;
	}
	else if( imyRepeat )
		After( iElapseTime );
	else
		ibTimerStatus = EFalse;
	

}



///////////////////////////////////////////////////////////////////////////////////
// Enable timer - 타이머를 enable/disable 시킴

void CMTimer::EnableTimer( atBOOL aEnableTimer )
{
	if( aEnableTimer )
	{
		if( IsActive() )
			return;
			//Cancel();
	
		imyRepeat     = iRepeat;
		ibTimerStatus = ETrue;

		if( !ibProcStatus && IsAdded())
		{
			After( iElapseTime );
		}
	}
	else
	{
		imyRepeat     = EFalse;
		ibTimerStatus = EFalse;

		if( IsActive() )
			Cancel();
	}
	
}


///////////////////////////////////////////////////////////////////////////////////
// Start timer - 타이머 동작중일 때 시간을 0으로 되돌림. 비동작중일 때는 무시

void CMTimer::ResetTimer()
{
	if( ibTimerStatus )
	{
		EnableTimer( ETrue );
	}
}


///////////////////////////////////////////////////////////////////////////////////
// Start timer

void CMTimer::StartTimer( atLONG aElapseTime )
{
	if( aElapseTime <= 0 )
		return;

	iElapseTime = aElapseTime * 1000;
	
	EnableTimer( ETrue );

}

///////////////////////////////////////////////////////////////////////////////////
// Stop timer

void CMTimer::StopTimer()
{
	EnableTimer( EFalse );

}

///////////////////////////////////////////////////////////////////////////////////
// Set Parameter

void CMTimer::SetParam( atVOID *param )
{
    ipParam = param;
}


	
