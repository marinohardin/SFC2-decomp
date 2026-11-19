// Common/Timer/HeartbeatTimer.cpp

#include "Common/Timer/HeartbeatTimer.h"
#include "unsorted/tWinClock.h"
#include "Common/Thread/ThreadState.h"
#include "Common/Exceptions/Validate.h"

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

// HYPOTHESIS: the SDK defines Enter, Wait, Leave and the guard's destructor inline; the
// constructor below expands them and calls TryEnter out of line.
inline void tOptex::Wait()
{
	::WaitForSingleObject( fEvent, INFINITE );
}

inline void tOptex::Enter()
{
	if( fUse == tThreadState::kIgnoreThreadObject )
	{
		return;
	}

	if( fUseCriticalSections )
	{
		::EnterCriticalSection( &( fpSharedInfo->fCriticalSection ) );
		return;
	}

	if( TryEnter() )
	{
		return;
	}

	int32u threadId = ::GetCurrentThreadId();

	if( ::InterlockedIncrement( &fpSharedInfo->fLockCount ) == 1 )
	{
		fpSharedInfo->fThreadId = threadId;
		fpSharedInfo->fRecurseCount = 1;
	}
	else
	{
		if( fpSharedInfo->fThreadId == threadId )
		{
			fpSharedInfo->fRecurseCount++;
		}
		else
		{
			Wait();

			fpSharedInfo->fThreadId = threadId;
			fpSharedInfo->fRecurseCount = 1;
		}
	}
}

inline void tOptex::Leave()
{
	if( fUse != tThreadState::kIgnoreThreadObject )
	{
		if( fUseCriticalSections )
		{
			::LeaveCriticalSection( &fpSharedInfo->fCriticalSection );
		}
		else
		{
			--fpSharedInfo->fRecurseCount;
			if( fpSharedInfo->fRecurseCount > 0 )
			{
				::InterlockedDecrement( &fpSharedInfo->fLockCount );
			}
			else
			{
				fpSharedInfo->fThreadId = 0;
				if( ::InterlockedDecrement( &fpSharedInfo->fLockCount ) > 0 )
				{
					::SetEvent( fEvent );
				}
			}
		}
	}
}

inline tOptex::tGuard::~tGuard()
{
	if( fNeedToLeave )
	{
		fOptex->Leave();
	}
}

// FUNCTION: STARFLEETOP 0x006f5b00
tHeartbeatTimer::tHeartbeatTimer() :
	fLastTime( 0 ),
	fInitalTime( 0 )
{
	fHBData = tHeartbeatTimerData::Instance();
	tOptex::tGuard guard( &fHBData->fAccessGlobalData );

	fTimerIndex = fHBData->fTimers.size();
	fHBData->fTimers.push_back( NULL );
}


// FUNCTION: STARFLEETOP 0x006f5c50
// FUNCTION: STARFLEETOP 0x006f5c70
tHeartbeatTimer::~tHeartbeatTimer()
{
	tOptex::tGuard guard( &fHBData->fAccessGlobalData );

	fHBData->fTimers[ fTimerIndex ] = NULL;
}

// FUNCTION: STARFLEETOP 0x006f5da0
void tHeartbeatTimer::StartTimer( int32u dueTime, int32u period, tTimerHandler* pTimerHandler )
{
	tOptex::tGuard guard( &fHBData->fAccessGlobalData );

	fLastTime = fInitalTime = fHBData->fpClock->GetCurrentSystemTime();
	tTimer::StartTimer( dueTime, period, pTimerHandler );
	fHBData->fTimers[ fTimerIndex ] = this;
}

// FUNCTION: STARFLEETOP 0x006f5ee0
void tHeartbeatTimer::StartTimer( int32u dueTime, int32u period, iTimerCallbackFunc* pCallback, int32u context )
{
	tOptex::tGuard guard( &fHBData->fAccessGlobalData );

	fLastTime = fInitalTime = fHBData->fpClock->GetCurrentSystemTime();
	tTimer::StartTimer( dueTime, period, pCallback, context );
	fHBData->fTimers[ fTimerIndex ] = this;
}

// FUNCTION: STARFLEETOP 0x006f6020
void tHeartbeatTimer::StartTimer( int32u dueTime, int32u period, iTimerCallback* pCallback, int32u context )
{
	tOptex::tGuard guard( &fHBData->fAccessGlobalData );

	fLastTime = fInitalTime = fHBData->fpClock->GetCurrentSystemTime();
	tTimer::StartTimer( dueTime, period, pCallback, context );
	fHBData->fTimers[ fTimerIndex ] = this;
}

// FUNCTION: STARFLEETOP 0x006f6160
void tHeartbeatTimer::ChangeTiming( int32u newDueTime, int32u newPeriod )
{
	tOptex::tGuard guard( &fHBData->fAccessGlobalData );

#line 123
	validate( fHBData->fTimers[fTimerIndex] );
	tTimer::ChangeTiming( newDueTime, newPeriod );
	fLastTime = fInitalTime = fHBData->fpClock->GetCurrentSystemTime();
}

// FUNCTION: STARFLEETOP 0x006f62c0
void tHeartbeatTimer::StopTimer()
{
	tOptex::tGuard guard( &fHBData->fAccessGlobalData );

	tTimer::StopTimer();
	fHBData->fTimers[ fTimerIndex ] = NULL;
	fLastTime = fInitalTime = 0;
}

// FUNCTION: STARFLEETOP 0x006f6580
void tHeartbeatTimer::OnAutoRunHB( int32 context )
{
	tHeartbeatTimerData* data = tHeartbeatTimerData::Instance();

	while ( data->fThread.Alive() && data->fThread.Working() )
	{
		Heartbeat();
		Sleep( context );
	}
}

// FUNCTION: STARFLEETOP 0x006f65d0
void tHeartbeatTimer::Heartbeat()
{
	tHeartbeatTimerData* data = tHeartbeatTimerData::Instance();
	tOptex::tGuard guard( &data->fAccessGlobalData );

	int32u currentTime = data->fpClock->GetCurrentSystemTime();

	for( int32u i = 0; i < data->fTimers.size(); i++ )
	{
		tHeartbeatTimer* pTimer = data->fTimers[i];
		if( pTimer )
		{
			int32u elapsed = currentTime - pTimer->fLastTime;
			bool signal = false;
			if( !pTimer->fTimerHasStarted )
			{
				if( elapsed > pTimer->fDueTime )
				{
					pTimer->fTimerHasStarted = true;
					signal = true;
				}
			}
			else if( elapsed > pTimer->fPeriod )
			{
				signal = true;
			}

			if( signal )
			{
				pTimer->SignalTimerEvent();
				if( pTimer->fPeriod > 0 )
				{
					pTimer->fLastTime = data->fpClock->GetCurrentSystemTime();
				}
				else
				{
					data->fTimers[i] = NULL;
				}
			}
		}
	}
}
