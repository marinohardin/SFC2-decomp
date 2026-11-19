#include "unsorted/tWinClock.h"
#include "unsorted/tWinClockGlobal.h"

// FUNCTION: STARFLEETOP 0x006fe0a0
double tWinClock::GetCurrentSystemTimeInSeconds() const
{
	return (double)GetCurrentSystemTime() * 0.001;
}

// FUNCTION: STARFLEETOP 0x006fe0f0
// FUNCTION: STARFLEETOP 0x006fe0e0
tWinClockGlobal::iAutoWinClockGlobalPtr tWinClockGlobal::fpAutoWinClockGlobalPtr;

// FUNCTION: STARFLEETOP 0x006fe140
tWinClockGlobal* tWinClockGlobal::Instance()
{
	if( fpAutoWinClockGlobalPtr.get() == 0 )
		fpAutoWinClockGlobalPtr = iAutoWinClockGlobalPtr( new tWinClockGlobal );
	return fpAutoWinClockGlobalPtr.get();
}
