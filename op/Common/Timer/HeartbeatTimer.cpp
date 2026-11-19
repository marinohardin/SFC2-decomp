// Common/Timer/HeartbeatTimer.cpp

#include "Common/Timer/HeartbeatTimer.h"
#include "unsorted/tWinClock.h"
#include "Common/Thread/ThreadState.h"

// HYPOTHESIS: the SDK defines these destructors inline in its headers; the original inlines
// all three into ~tHeartbeatTimerData.
template< typename tnResult >
inline tCallback<tnResult>::~tCallback()
{}

template< typename tnResult >
inline tFunctionCallback<tnResult>::~tFunctionCallback()
{}

inline tOptex::~tOptex()
{
	if( fUse != tThreadState::kIgnoreThreadObject )
	{
		if( fUseCriticalSections && fpSharedInfo != NULL )
		{
			::DeleteCriticalSection( &fpSharedInfo->fCriticalSection );
		}

		if( fEvent != NULL )
		{
			::CloseHandle( fEvent );
		}

		if( fFileMapping == NULL )
		{
			delete fpSharedInfo;
		}
		else
		{
			::UnmapViewOfFile( fpSharedInfo );
			::CloseHandle( fFileMapping );
		}
	}
}

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

// FUNCTION: STARFLEETOP 0x006f5a10
tHeartbeatTimer::tHeartbeatTimerData::~tHeartbeatTimerData()
{
	delete fpClock;
}
