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
