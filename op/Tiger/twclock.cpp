//	twclock.cpp: twClock, the Windows clock part.

#include <windows.h>

#include "Tiger/tclock.h"

// HYPOTHESIS: one timer slot of mSetTimer and mTimerReceiver (20 bytes); field meanings unknown.
struct	twClockResponderDesc
{
	int32s	fUnknown_00;
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

		iTruth			fHasPerformanceCounter;
		int32s			fUnknown_10;
		double			fPerformanceFrequency;

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
