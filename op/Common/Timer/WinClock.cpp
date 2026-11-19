#include "unsorted/tWinClock.h"

// FUNCTION: STARFLEETOP 0x006fe0a0
double tWinClock::GetCurrentSystemTimeInSeconds() const
{
	return (double)GetCurrentSystemTime() * 0.001;
}
