//	twclock.cpp: twClock, the Windows clock part.

#include <windows.h>

#include "Tiger/tclock.h"

#include "Tiger/tTimerResponse.h"

// HYPOTHESIS: one timer slot of mSetTimer and mTimerReceiver (20 bytes); the response at 0x00,
// the timer id at 0x04; the meanings of the others are unknown.
struct	twClockResponderDesc
{
	tTimerResponse	*fUnknown_00;
	int32s	fUnknown_04;
	int32s	fUnknown_08;
	int32s	fUnknown_0c;
	int32s	fUnknown_10;
};

// HYPOTHESIS: the slot count; mActivate clears 100 slots, 0x00c3cdf8 to 0x00c3d5c8.
#define	kClockResponderMax	100

class	twClock : public tClockPart
{
	public:
		virtual		iTruth	mIsValid	( void );
		virtual		iTruth	mIsReady	( void );
		virtual		void	mActivate	( void );
		virtual		void	mDeactivate	( void );
		virtual		double	mGetMicroseconds	( void );
		virtual		void	mSetTimer	( long, long, tTimerResponse *, long );
		virtual		void	mSetTimer	( long, long, tTimerResponse * );
		virtual		void	mForgetTimer	( tTimerResponse * );

		iTruth			fHasPerformanceCounter;
		int32u			fUnknown_10;
		double			fPerformanceFrequency;

		static	void	CALLBACK	mTimerReceiver	( UINT id, UINT msg, DWORD user, DWORD dw1, DWORD dw2 );

	private:
		static	twClockResponderDesc	fResponseList[ kClockResponderMax ];
		static	int16s					fNumberOfRunningTimers;
};

twClockResponderDesc	twClock::fResponseList[ kClockResponderMax ];
int16s					twClock::fNumberOfRunningTimers;

// FUNCTION: STARFLEETOP 0x00637820
void	twClock::mActivate( void )
{
	LARGE_INTEGER	freq;

	fUnknown_10 = 1000000;
	fHasPerformanceCounter = QueryPerformanceFrequency( &freq );
	fPerformanceFrequency = (double) freq.LowPart + (double) freq.HighPart * 4294967296.0;
	if (fPerformanceFrequency == 0.0)
		fHasPerformanceCounter = kFalse;

	fNumberOfRunningTimers = 0;
	for( iIndex idx = 0; idx < kClockResponderMax; idx++ )
	{
		fResponseList[idx].fUnknown_00 = 0;
		fResponseList[idx].fUnknown_04 = 0;
		fResponseList[idx].fUnknown_08 = 0;
	}
}

// FUNCTION: STARFLEETOP 0x00637890
void	twClock::mDeactivate( void )
{
	for( iIndex idx = 0; idx < kClockResponderMax; idx++ )
	{
		if (fResponseList[idx].fUnknown_04 != 0)
		{
			if (timeKillEvent( fResponseList[idx].fUnknown_04 ) != TIMERR_NOERROR || fNumberOfRunningTimers < 0)
				throw TimerException();
			fResponseList[idx].fUnknown_04 = 0;
		}
	}
	fNumberOfRunningTimers = 0;
}

// FUNCTION: STARFLEETOP 0x006378f0
double	twClock::mGetMicroseconds( void )
{
	if (fHasPerformanceCounter != kTrue)
		return timeGetTime() * 1000;

	LARGE_INTEGER	count;

	QueryPerformanceCounter( &count );
	return ((double) count.LowPart + (double) count.HighPart * 4294967296.0) * fUnknown_10 / fPerformanceFrequency;
}

// FUNCTION: STARFLEETOP 0x00637970
void	twClock::mSetTimer( long param_1, long param_2, tTimerResponse *param_3, long param_4 )
{
	if (fNumberOfRunningTimers < kClockResponderMax)
	{
		fResponseList[fNumberOfRunningTimers].fUnknown_00 = param_3;
		fResponseList[fNumberOfRunningTimers].fUnknown_08 = param_2;
		fResponseList[fNumberOfRunningTimers].fUnknown_0c = param_1;
		fResponseList[fNumberOfRunningTimers].fUnknown_10 = param_4;

		MMRESULT	timerID = timeSetEvent( param_1 >= 1000 ? param_1 / 1000 : 1, param_4, mTimerReceiver, 0, TIME_PERIODIC );
		if (timerID == 0)
			throw TimerException();

		param_3->mSetIdentity( timerID );
		fResponseList[fNumberOfRunningTimers++].fUnknown_04 = timerID;
	}
	else
		throw TimerException();
}

// FUNCTION: STARFLEETOP 0x00637a50
void	twClock::mSetTimer( long param_1, long param_2, tTimerResponse *param_3 )
{
	if (fNumberOfRunningTimers < kClockResponderMax)
	{
		fResponseList[fNumberOfRunningTimers].fUnknown_00 = param_3;
		fResponseList[fNumberOfRunningTimers].fUnknown_08 = param_2;
		fResponseList[fNumberOfRunningTimers].fUnknown_0c = param_1;
		fResponseList[fNumberOfRunningTimers].fUnknown_10 = 0;

		long	delay = param_1 / 1000;
		if (delay == 0)
			delay = 1;

		MMRESULT	timerID = timeSetEvent( delay, 0, mTimerReceiver, 0, TIME_PERIODIC );
		if (timerID == 0)
			throw TimerException();

		param_3->mSetIdentity( timerID );
		fResponseList[fNumberOfRunningTimers++].fUnknown_04 = timerID;
	}
	else
		throw TimerException();
}

// FUNCTION: STARFLEETOP 0x00637b20
void	twClock::mForgetTimer( tTimerResponse *param_1 )
{
	unsigned long	timerID = param_1->mGetIdentity();

	// The slot is read before the bound is tested, so slot 100 is read past the end.
	int32u	idx = 0;
	while (fResponseList[idx].fUnknown_04 != timerID && idx < kClockResponderMax)
		idx++;

	if (idx < kClockResponderMax)
	{
		fNumberOfRunningTimers--;
		if (timeKillEvent( timerID ) != TIMERR_NOERROR || fNumberOfRunningTimers < 0)
			throw TimerException();
		fResponseList[idx].fUnknown_04 = 0;
	}
}
