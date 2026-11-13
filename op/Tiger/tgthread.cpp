//	tgthread.cpp: tCoopThreadPart, cooperative threads scheduled on the clock part.

#include "Tiger/tclock.h"
#include "Tiger/tgthread.h"

// std::find, for mFreeCoopThread.
#include <algorithm>

// FUNCTION: STARFLEETOP 0x0063edd0
double	tCoopThreadPart::mSetTime( void )
{
	double	now = aClockPart->mGetMicroseconds();

	if (now > fUnknown_1c80)
		fUnknown_1c80 = now;

	return fUnknown_1c80;
}

// FUNCTION: STARFLEETOP 0x0063ee80
double	tCoopThreadPart::mGetTime( void )
{
	return fUnknown_1c80;
}

// FUNCTION: STARFLEETOP 0x0063ee90
iTruth	tCoopThreadPart::mIsReady( void )
{
	return TestPart(tClockPart) && TestPart(tDebugPart);
}

// FUNCTION: STARFLEETOP 0x0063eeb0
void	tCoopThreadPart::mActivate( void )
{
	mSetTime();

	fUnknown_1c30 = 0;
	fUnknown_1c78 = 0;
	fUnknown_1c7c = 0;

	fUnknown_1c34.reserve( 100 );
	fUnknown_1c44.reserve( 100 );
	fUnknown_1c54.reserve( 100 );

	for (int i = 0; i < 100; i++)
	{
		fUnknown_0010[ i ].fn_00435265();
		fUnknown_1c34.push_back( &fUnknown_0010[ i ] );
	}

	fUnknown_1c64 = mAllocCoopThread();
	fUnknown_1c64->fUnknown_08 = 0;
	fUnknown_1c64->fUnknown_0c = mNewMainThread();
	fUnknown_1c64->fUnknown_18 = 0;
	fUnknown_1c64->fUnknown_20 = 0.0;
	fUnknown_1c64->fUnknown_38 = 0.0;
	fUnknown_1c64->fUnknown_40 = 0.0;

	fUnknown_1c70 = 0;
	fUnknown_1c68 = fUnknown_1c64;
	fUnknown_1c6c = 0;
	fUnknown_1c74 = 0;

	mSetMainThreadPriority( 100 );
}

// FUNCTION: STARFLEETOP 0x0063f110
void	tCoopThreadPart::mDeactivate( void )
{
	for (std::vector<tCoopThread*>::iterator it = fUnknown_1c44.begin(); it != fUnknown_1c44.end(); it++)
	{
		tCoopThread*	thread = *it;

		if (!thread->fUnknown_10 && thread != fUnknown_1c64)
			mDelThread( thread->fUnknown_08 );
	}

	if (fUnknown_1c64)
	{
		delete fUnknown_1c64->fUnknown_0c;
		fUnknown_1c64->fUnknown_0c = NULL;
	}
}

// FUNCTION: STARFLEETOP 0x0063f180
void	tCoopThreadPart::mNewThread( tClientThread* thread, long freq )
{
	if (mFindCoopThread( thread ))
	{
		// HYPOTHESIS: a deliberate fault on a thread added twice; the read of 0xffffffff is
		// the original's, kept in a function-local static named t.
		static volatile char	t = *(volatile char*) 0xffffffff;
	}

	thread->mUnknown_04();

	tCoopThread*	coop = mAllocCoopThread();
	LogThreadEvent( "New Thread Created.", 0 );

	coop->fUnknown_08 = thread;
	coop->fUnknown_0c = mNewNativeThread( thread, freq );
	LogThreadEvent( "( New Native Thread Created. )", 0 );

	coop->fUnknown_18 = 50;
	coop->fUnknown_1c = 0;
	coop->fUnknown_20 = 20000.0;
	coop->fUnknown_28 = mGetTime();

	{
		static double	targetJitter;

		targetJitter += 10000.0;
		while (targetJitter >= 100000.0)
			targetJitter -= 100000.0;

		coop->fUnknown_28 += targetJitter;
	}

	coop->fUnknown_38 = 0.0;
	coop->fUnknown_40 = 0.0;
	LogThreadEvent( "( New Thread Complete. )", 0 );
}

// FUNCTION: STARFLEETOP 0x00640220
void	tCoopThreadPart::mDoDelThread( tClientThread* thread )
{
	// HYPOTHESIS: each block below is the deliberate fault of mNewThread, used here as a check
	// on the thread being removed.
	tCoopThread*	coop = mFindCoopThread( thread );

	if (!coop)						{ static volatile char t = *(volatile char*) 0xffffffff; }
	if (coop == fUnknown_1c68)		{ static volatile char t = *(volatile char*) 0xffffffff; }
	if (coop == fUnknown_1c64)		{ static volatile char t = *(volatile char*) 0xffffffff; }
	if (!coop->fUnknown_0c)			{ static volatile char t = *(volatile char*) 0xffffffff; }

	delete coop->fUnknown_0c;
	coop->fUnknown_0c = NULL;

	mFreeCoopThread( coop );
	thread->mUnknown_08();
}

// FUNCTION: STARFLEETOP 0x006402e0
void	tCoopThreadPart::mDelThread( tClientThread* thread )
{
	// HYPOTHESIS: the original looks the thread up through the part binding, not this.
	tCoopThread*	coop = aCoopThreadPart->mFindCoopThread( thread );

	if (coop)
	{
		if (coop == fUnknown_1c64)		{ static volatile char t = *(volatile char*) 0xffffffff; }

		// HYPOTHESIS: the flag marks the entry dead; mDoSleep frees it on the next switch.
		coop->fUnknown_10 = 1;

		std::vector<tCoopThread*>::iterator	it = std::find( fUnknown_1c44.begin(), fUnknown_1c44.end(), coop );

		if (it != fUnknown_1c44.end())
		{
			fUnknown_1c44.erase( it );
			fUnknown_1c54.push_back( coop );
			mRest();
		}
	}
}

// FUNCTION: STARFLEETOP 0x0063f370
void	tCoopThreadPart::mResetPriorities( void )
{
	for (std::vector<tCoopThread*>::iterator it = fUnknown_1c44.begin(); it != fUnknown_1c44.end(); it++)
	{
		tCoopThread*	thread = *it;

		if (thread->fUnknown_1c > 0 && thread->fUnknown_28 <= 1.0 && !thread->fUnknown_10)
			thread->fUnknown_28 = 1.0;
	}
}

// FUNCTION: STARFLEETOP 0x0063f280
void	tCoopThreadPart::mSetThreadFreq( tClientThread* thread, long freq )
{
	tCoopThread*	coop = mFindCoopThread( thread );

	if (coop && freq > 0)
	{
		if (coop->fUnknown_1c > 0)
		{
			coop->fUnknown_1c = 0;
			coop->fUnknown_18 = freq;
			coop->fUnknown_20 = 1000000.0 / freq;
			coop->fUnknown_28 = mGetTime();
		}
		else
		{
			double	oldPeriod = coop->fUnknown_20;

			coop->fUnknown_18 = freq;
			coop->fUnknown_20 = 1000000.0 / freq;
			coop->fUnknown_28 -= oldPeriod - coop->fUnknown_20;
		}
	}
}

// FUNCTION: STARFLEETOP 0x0063f2f0
void	tCoopThreadPart::mSetMainThreadFreq( long freq )
{
	if (fUnknown_1c64 && freq > 0)
	{
		if (fUnknown_1c64->fUnknown_1c > 0)
		{
			fUnknown_1c64->fUnknown_1c = 0;
			fUnknown_1c64->fUnknown_18 = freq;
			fUnknown_1c64->fUnknown_20 = 1000000.0 / freq;
			fUnknown_1c64->fUnknown_28 = mGetTime();
		}
		else
		{
			double	oldPeriod = fUnknown_1c64->fUnknown_20;

			fUnknown_1c64->fUnknown_18 = freq;
			fUnknown_1c64->fUnknown_20 = 1000000.0 / freq;
			fUnknown_1c64->fUnknown_28 -= oldPeriod - fUnknown_1c64->fUnknown_20;
		}
	}
}

// FUNCTION: STARFLEETOP 0x0063f3c0
void	tCoopThreadPart::mSetThreadPriority( tClientThread* thread, long priority )
{
	tCoopThread*	coop = mFindCoopThread( thread );

	if (coop && priority > 0)
	{
		coop->fUnknown_1c = priority;
		coop->fUnknown_30 = 1.0 / (priority * 10);
		coop->fUnknown_18 = 0;
		coop->fUnknown_20 = 0.0;
		coop->fUnknown_28 = 1.0;

		mResetPriorities();
	}
}

// FUNCTION: STARFLEETOP 0x0063f420
void	tCoopThreadPart::mSetMainThreadPriority( long priority )
{
	if (fUnknown_1c64 && priority > 0)
	{
		fUnknown_1c64->fUnknown_18 = 0;
		fUnknown_1c64->fUnknown_20 = 0.0;
		fUnknown_1c64->fUnknown_1c = priority;
		fUnknown_1c64->fUnknown_30 = 1.0 / (priority * 10);
		fUnknown_1c64->fUnknown_28 = 1.0;

		mResetPriorities();
	}
}

// FUNCTION: STARFLEETOP 0x0063f490
void	tCoopThreadPart::mRest( void )
{
	double	now = mSetTime();
	LogThreadEvent( "Resting", 0 );

	fUnknown_1c68->fUnknown_40 = now;
	mDoSleep( now, fUnknown_1c68->fUnknown_0c );
}

// FUNCTION: STARFLEETOP 0x0063f4f0
void	tCoopThreadPart::mSleepFor( double time )
{
	if (time > 1.0)
	{
		double	now = mSetTime();

		fUnknown_1c68->fUnknown_28 = now + time;
		mDoSleep( now, fUnknown_1c68->fUnknown_0c );
	}
}

// FUNCTION: STARFLEETOP 0x0063f540
void	tCoopThreadPart::mSleepUntil( double time )
{
	double	now = mSetTime();

	if (time > now + 1.0)
	{
		fUnknown_1c68->fUnknown_28 = time;
		mDoSleep( now, fUnknown_1c68->fUnknown_0c );
	}
}

// FUNCTION: STARFLEETOP 0x0063f5a0
void	tCoopThreadPart::mSwitchTo( tClientThread* thread )
{
	try
	{
		tCoopThread*	coop = mFindCoopThread( thread );

		if (coop && !coop->fUnknown_10)
		{
			tCoopThread*	current = fUnknown_1c68;

			fUnknown_1c70 = 0;
			fUnknown_1c6c = coop;
			fUnknown_1c74 = 0;

			if (coop != current)
			{
				fUnknown_1c78++;
				double	now = mGetTime();

				if (fUnknown_1c6c->fUnknown_1c > 0 && fUnknown_1c6c->fUnknown_28 <= 1.0)
					fUnknown_1c6c->fUnknown_28 -= fUnknown_1c6c->fUnknown_30;
				else if (fUnknown_1c6c->fUnknown_1c > 0)
				{
					mResetPriorities();
					fUnknown_1c6c->fUnknown_28 -= fUnknown_1c6c->fUnknown_30;
				}
				else
				{
					tCoopThread*	next = fUnknown_1c6c;
					double			due = next->fUnknown_28 + next->fUnknown_20;

					if (now < due)
						next->fUnknown_28 = due;
					else
						next->fUnknown_28 = now + next->fUnknown_20;
				}

				current->fUnknown_0c->mGotoThread( fUnknown_1c6c->fUnknown_0c );
				fUnknown_1c7c++;
			}

			fUnknown_1c68 = current;
		}
	}
	catch (...)
	{
	}
}

// FUNCTION: STARFLEETOP 0x0063f6e0
tCoopThread*	tCoopThreadPart::mAllocCoopThread( void )
{
	tCoopThread*	thread = NULL;

	if (fUnknown_1c34.size() <= 0)
	{
		// HYPOTHESIS: the same deliberate fault as mNewThread's, here when no free entry is left.
		static volatile char	t = *(volatile char*) 0xffffffff;
	}

	thread = fUnknown_1c34.back();
	fUnknown_1c34.pop_back();
	fUnknown_1c44.push_back( thread );

	thread->fn_00435265();
	thread->fUnknown_14 = fUnknown_1c30++;

	return thread;
}

// FUNCTION: STARFLEETOP 0x0063f8f0
void	tCoopThreadPart::mFreeCoopThread( tCoopThread* thread )
{
	std::vector<tCoopThread*>::iterator	it;

	it = std::find( fUnknown_1c44.begin(), fUnknown_1c44.end(), thread );
	if (it != fUnknown_1c44.end())
		fUnknown_1c44.erase( it );

	it = std::find( fUnknown_1c54.begin(), fUnknown_1c54.end(), thread );
	if (it != fUnknown_1c54.end())
		fUnknown_1c54.erase( it );

	fUnknown_1c34.push_back( thread );

	thread->fn_00435265();
}

// FUNCTION: STARFLEETOP 0x0063fb10
tCoopThread*	tCoopThreadPart::mFindCoopThread( tClientThread* thread )
{
	tCoopThread*	found = NULL;
	std::vector<tCoopThread*>::iterator	it;

	for (it = fUnknown_1c44.begin(); !found && it != fUnknown_1c44.end(); it++)
	{
		if ((*it)->fUnknown_08 == thread)
			found = *it;
	}

	if (!found)
	{
		for (it = fUnknown_1c54.begin(); !found && it != fUnknown_1c54.end(); it++)
		{
			if ((*it)->fUnknown_08 == thread)
				found = *it;
		}
	}

	return found;
}

// FUNCTION: STARFLEETOP 0x0063fb70
tCoopThread*	tCoopThreadPart::mFindCoopThread( tNativeThread* thread )
{
	tCoopThread*	found = NULL;
	std::vector<tCoopThread*>::iterator	it;

	for (it = fUnknown_1c44.begin(); !found && it != fUnknown_1c44.end(); it++)
	{
		if ((*it)->fUnknown_0c == thread)
			found = *it;
	}

	if (!found)
	{
		for (it = fUnknown_1c54.begin(); !found && it != fUnknown_1c54.end(); it++)
		{
			if ((*it)->fUnknown_0c == thread)
				found = *it;
		}
	}

	return found;
}

// FUNCTION: STARFLEETOP 0x0063fbd0
// HYPOTHESIS: neither parameter is read in this build.
tCoopThread*	tCoopThreadPart::mGetBestNextThread( tCoopThread* thread, double time )
{
	tCoopThread*	result = NULL;

	while (!result)
	{
		// The most overdue timed thread, and the highest-weighted prioritised one.
		tCoopThread*	bestTimed = NULL;
		double			bestLate = -9999999.0;
		tCoopThread*	bestPriority = NULL;
		double			bestWeight = 0.0;
		long			runnable = 0;

		for (std::vector<tCoopThread*>::iterator it = fUnknown_1c44.begin(); it != fUnknown_1c44.end(); it++)
		{
			tCoopThread*	coop = *it;

			if (!coop->fUnknown_10)
				runnable++;

			if (coop->fUnknown_1c > 0 && coop->fUnknown_28 <= 1.0 && !coop->fUnknown_10)
			{
				if (coop->fUnknown_28 > bestWeight)
				{
					bestWeight = coop->fUnknown_28;
					bestPriority = coop;
				}
			}
			else
			{
				double	late = mGetTime() - (*it)->fUnknown_28;

				if (late > bestLate && !(*it)->fUnknown_10)
				{
					bestLate = late;
					bestTimed = *it;
				}
			}
		}

		if (bestLate >= 0.0)
			result = bestTimed;
		else if (bestWeight > 0.0)
			result = bestPriority;
		else
		{
			if (!runnable)
				return NULL;

			mResetPriorities();
			mSetTime();
		}
	}

	return result;
}

// FUNCTION: STARFLEETOP 0x0063fd10
void	tCoopThreadPart::mDoSleep( double time, tNativeThread* thread )
{
	// HYPOTHESIS: each block below is the deliberate fault of mNewThread, used here as a check
	// on the scheduler's state.
	tCoopThread*	current = fUnknown_1c68;
	tNativeThread*	native = current->fUnknown_0c;

	if (native != thread)		{ static volatile char t = *(volatile char*) 0xffffffff; }
	if (fUnknown_1c68 != current)			{ static volatile char t = *(volatile char*) 0xffffffff; }
	if (!fUnknown_1c68->fUnknown_0c)		{ static volatile char t = *(volatile char*) 0xffffffff; }

	LogThreadEvent( "S1", 0 );

	if (fUnknown_1c68->fUnknown_10 || !fUnknown_1c6c || fUnknown_1c6c == fUnknown_1c68)
	{
		bool	sleep = true;

		do
		{
			fUnknown_1c6c = mGetBestNextThread( fUnknown_1c68, time );

			if (!fUnknown_1c6c)
			{
				if (fUnknown_1c44.size() == 1 && fUnknown_1c68 == fUnknown_1c64)
				{
					LogThreadEvent( "S2", 0 );
					sleep = false;
				}

				fUnknown_1c6c = fUnknown_1c64;
				// HYPOTHESIS: 0x1c70 holds a double; see the member's note.
				*(double*) &fUnknown_1c70 = fUnknown_1c64->fUnknown_28 = time;
			}
			else
				*(double*) &fUnknown_1c70 = fUnknown_1c6c->fUnknown_28;

			if (!sleep)
				return;
		}
		while (!fUnknown_1c6c);

		if (!sleep)
			return;

		if (fUnknown_1c68 != current)				{ static volatile char t = *(volatile char*) 0xffffffff; }
		if (!fUnknown_1c68->fUnknown_0c)			{ static volatile char t = *(volatile char*) 0xffffffff; }
		if (fUnknown_1c68->fUnknown_0c != thread)	{ static volatile char t = *(volatile char*) 0xffffffff; }
		if (!fUnknown_1c6c)							{ static volatile char t = *(volatile char*) 0xffffffff; }
		if (!fUnknown_1c6c->fUnknown_0c)			{ static volatile char t = *(volatile char*) 0xffffffff; }

		LogThreadEvent( "S3", fUnknown_1c6c->fUnknown_0c->f14 );

		if (fUnknown_1c6c->fUnknown_10)				{ static volatile char t = *(volatile char*) 0xffffffff; }

		fUnknown_1c6c->fUnknown_38 = time;

		if (fUnknown_1c68 != current)				{ static volatile char t = *(volatile char*) 0xffffffff; }
		if (!fUnknown_1c68->fUnknown_0c)			{ static volatile char t = *(volatile char*) 0xffffffff; }
		if (fUnknown_1c68->fUnknown_0c != thread)	{ static volatile char t = *(volatile char*) 0xffffffff; }

		fUnknown_1c78++;
		LogThreadEvent( "S4", 0 );

		// The original reuses the parameter for the current time.
		time = mGetTime();

		if (fUnknown_1c6c->fUnknown_1c > 0 && fUnknown_1c6c->fUnknown_28 <= 1.0)
			fUnknown_1c6c->fUnknown_28 -= fUnknown_1c6c->fUnknown_30;
		else if (fUnknown_1c6c->fUnknown_1c > 0)
		{
			mResetPriorities();
			fUnknown_1c6c->fUnknown_28 -= fUnknown_1c6c->fUnknown_30;
		}
		else
		{
			tCoopThread*	next = fUnknown_1c6c;
			double			due = next->fUnknown_28 + next->fUnknown_20;

			if (time < due)
				next->fUnknown_28 = due;
			else
				next->fUnknown_28 = time + next->fUnknown_20;
		}

		native->mGotoThread( fUnknown_1c6c->fUnknown_0c );
		LogThreadEvent( "S5", 0 );
		fUnknown_1c7c++;

		if (current->fUnknown_0c != thread)			{ static volatile char t = *(volatile char*) 0xffffffff; }
		if (current->fUnknown_10)					{ static volatile char t = *(volatile char*) 0xffffffff; }
		if (!current->fUnknown_0c)					{ static volatile char t = *(volatile char*) 0xffffffff; }

		LogThreadEvent( "S6", 0 );

		fUnknown_1c70 = 0;
		fUnknown_1c68 = current;
		fUnknown_1c6c = NULL;
		fUnknown_1c74 = 0;

		if (current->fUnknown_10)					{ static volatile char t = *(volatile char*) 0xffffffff; }

		int		count = fUnknown_1c54.size();

		if (count > 0)
		{
			static tCoopThread*	dead;

			dead = NULL;
			dead = fUnknown_1c54[ 0 ];

			if (!dead->fUnknown_10)					{ static volatile char t = *(volatile char*) 0xffffffff; }

			if (dead != fUnknown_1c68)
			{
				LogThreadEvent( "S7", dead->fUnknown_0c->f14 );
				mDoDelThread( dead->fUnknown_08 );

				if (count != fUnknown_1c54.size() + 1)	{ static volatile char t = *(volatile char*) 0xffffffff; }
			}
		}

		LogThreadEvent( "S8", 0 );
	}
}
