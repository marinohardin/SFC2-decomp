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

// FUNCTION: STARFLEETOP 0x0070fca0
// FUNCTION: STARFLEETOP 0x0070fcc0
tTimer::~tTimer()
{
}

// FUNCTION: STARFLEETOP 0x0070fcd0
void tTimer::StartTimer( int32u dueTime, int32u period, tTimerHandler* pTimerHandler )
{
	fTimerHasStarted = false;
	fDueTime = dueTime;
	fPeriod = period;
	fCallbackType = kTimerHandler;
	fpTimerHandler = pTimerHandler;
	fpCallbackFunc = 0;
	fpCallback = 0;
	fContext = 0;
}

// FUNCTION: STARFLEETOP 0x0070fd00
void tTimer::StartTimer( int32u dueTime, int32u period, iTimerCallbackFunc* pCallback, int32u context )
{
	fTimerHasStarted = false;
	fDueTime = dueTime;
	fPeriod = period;
	fpTimerHandler = 0;
	fCallbackType = kTimerCallbackFuncton;
	fpCallbackFunc = pCallback;
	fpCallback = 0;
	fContext = context;
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

// FUNCTION: STARFLEETOP 0x0070fd80
void tTimer::ChangeTiming( int32u newDueTime, int32u newPeriod )
{
	StopTimer();
	fTimerHasStarted = false;
	fDueTime = newDueTime;
	fPeriod = newPeriod;
}

// FUNCTION: STARFLEETOP 0x0070fda0
void tTimer::StopTimer()
{
	fDueTime = 0;
	fPeriod = 0;
}

// FUNCTION: STARFLEETOP 0x0070fdb0
void tTimer::SignalTimerEvent()
{
	switch( fCallbackType )
	{
	case kTimerHandler:
		fpTimerHandler->HandlerTimerEvent( this );
		break;
	case kTimerCallbackFuncton:
		fpCallbackFunc( this, fContext );
		break;
	case kCallbackClass:
		fpCallback->Callback( fContext );
		break;
	default:
		throw;
	}
}
