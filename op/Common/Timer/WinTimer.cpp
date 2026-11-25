// Common/Timer/WinTimer.cpp

#include <windows.h>
#include <mmsystem.h>
#include "Common/Exceptions/Validate.h"

#include "Common/Timer/WinTimer.h"
#include "unsorted/tWinClock.h"

// FUNCTION: STARFLEETOP 0x0070f5a0
tWinTimer::tWinTimer()
	: fTimerHandle( 0 ), fpClock( new tWinClock )
{
}

// FUNCTION: STARFLEETOP 0x0070f630
tWinTimer::~tWinTimer()
{
	if (fTimerHandle != 0)
		timeKillEvent( fTimerHandle );
	delete fpClock;
	fpClock = 0;
}

#line 100
// FUNCTION: STARFLEETOP 0x0070f730
void tWinTimer::StartTimer( int32u dueTime, int32u period, iTimerCallbackFunc* pCallback, int32u context )
{
	if( fTimerHandle != 0 )
		StopTimer();
	if( dueTime == 0 )
		dueTime = 1;
	fInitalTime = fpClock->GetCurrentSystemTime();
	tTimer::StartTimer( dueTime, period, pCallback, context );
	fTimerHandle = timeSetEvent( fDueTime, 1, (LPTIMECALLBACK)WinTimerCallback, (DWORD)this, 0 );
#line 115
	validate( fTimerHandle );
}

// FUNCTION: STARFLEETOP 0x0070f7c0
void tWinTimer::StartTimer( int32u dueTime, int32u period, iTimerCallback* pCallback, int32u context )
{
	if( fTimerHandle != 0 )
		StopTimer();
	if( dueTime == 0 )
		dueTime = 1;
	fInitalTime = fpClock->GetCurrentSystemTime();
	tTimer::StartTimer( dueTime, period, pCallback, context );
	fTimerHandle = timeSetEvent( fDueTime, 1, (LPTIMECALLBACK)WinTimerCallback, (DWORD)this, 0 );
#line 141
	validate( fTimerHandle );
}

#line 190
// FUNCTION: STARFLEETOP 0x0070f940
void __stdcall tWinTimer::WinTimerCallback( unsigned int timerId, unsigned int msg, int32u user, int32u dw1, int32u dw2 )
{
	tWinTimer* winTimer = (tWinTimer*)user;
	winTimer->SignalTimerEvent();
	if( !winTimer->fTimerHasStarted )
	{
		timeKillEvent( winTimer->fTimerHandle );
		winTimer->fTimerHandle = 0;
		winTimer->fTimerHasStarted = true;
		if( winTimer->fPeriod > 0 )
		{
			winTimer->fTimerHandle = timeSetEvent( winTimer->fPeriod, 1, (LPTIMECALLBACK)WinTimerCallback, (DWORD)winTimer, 1 );
#line 210
			validate( winTimer->fTimerHandle );
		}
	}
}
