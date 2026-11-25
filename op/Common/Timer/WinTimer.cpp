// Common/Timer/WinTimer.cpp

#include <windows.h>
#include <mmsystem.h>

#include "Common/Timer/WinTimer.h"
#include "unsorted/tWinClock.h"

// FUNCTION: STARFLEETOP 0x0070f630
tWinTimer::~tWinTimer()
{
	if (fTimerID != 0)
		timeKillEvent( fTimerID );
	delete fpClock;
	fpClock = 0;
}
