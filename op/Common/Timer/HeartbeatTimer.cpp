// Common/Timer/HeartbeatTimer.cpp

#include "Common/Timer/HeartbeatTimer.h"
#include "unsorted/tWinClock.h"

tHeartbeatTimer::tHeartbeatTimerData::iPtr tHeartbeatTimer::tHeartbeatTimerData::fHeartbeatTimerData;

// FUNCTION: STARFLEETOP 0x006f5860
tHeartbeatTimer::tHeartbeatTimerData* tHeartbeatTimer::tHeartbeatTimerData::Instance()
{
	if( fHeartbeatTimerData.get() == 0 )
	{
		fHeartbeatTimerData = iPtr( new tHeartbeatTimerData );
	}
	return fHeartbeatTimerData.get();
}

// FUNCTION: STARFLEETOP 0x006f5950
tHeartbeatTimer::tHeartbeatTimerData::tHeartbeatTimerData() :
	fThreadCB( OnAutoRunHB ),
	fAutoRunFrequency( 0 )
{
	fpClock = new tWinClock;
}
