// Common/Timer/Timer.cpp

#include "Common/Timer/Timer.h"

// FUNCTION: STARFLEETOP 0x0070fc80
tTimer::tTimer()
	: fCallbackType( kNoCallback )
	, fpTimerHandler( 0 )
	, fpCallbackFunc( 0 )
	, fContext( 0 )
	, fInitalTime( 0 )
	, fDueTime( 0 )
	, fPeriod( 0 )
{
}

// FUNCTION: STARFLEETOP 0x0070fd40
void tTimer::StartTimer( int32u dueTime, int32u period, iTimerCallback* pCallback, int32u context )
{
	fTimerHasStarted = 0;
	fDueTime = dueTime;
	fPeriod = period;
	fpTimerHandler = 0;
	fpCallbackFunc = 0;
	fCallbackType = kCallbackClass;
	fpCallback = pCallback;
	fContext = context;
}

// FUNCTION: STARFLEETOP 0x0070fda0
void tTimer::StopTimer()
{
	fDueTime = 0;
	fPeriod = 0;
}
