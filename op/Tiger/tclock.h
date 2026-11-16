//	tclock.h: tClockPart, the part that gives the time.

#ifndef	tclock_h
#define	tclock_h

// tsay.h carries tpart.h and the part binding macros.
#include "Tiger/tsay.h"

class	tClockPart : public tPart
{
	public:
		GenAbstractBinding( tClockPart );

		// RTTI .?AVTimerException@tClockPart@@: one catchable type, thrown as a 1-byte temporary.
		class	TimerException {};

		// HYPOTHESIS: the first virtual after tPart's is twClock::mGetMicroseconds (vtable +0x18).
		virtual		double	mGetMicroseconds	( void ) = 0;
};

#define	aClockPart	(BindPart(tClockPart))

#endif	// #ifndef tclock_h
