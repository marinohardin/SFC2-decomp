// unsorted/tWinClock.h

#if !defined( tWinClock_h )
#define tWinClock_h

#include "Common/BaseTypes.h"

// HYPOTHESIS: the original header of tClock and tWinClock is not known. The members are the
// SDK library symbols matched to 0x006fe030-0x006fe0a0 (a 4-byte object, its vtable alone);
// the order of the virtuals after the destructor is a guess.
class tClock
{
public:
	virtual ~tClock();

	virtual int32u GetCurrentSystemTime() const = 0;
	virtual double GetCurrentSystemTimeInSeconds() const = 0;
};

class tWinClock : public tClock
{
public:
	tWinClock();
	virtual ~tWinClock();

	virtual int32u GetCurrentSystemTime() const;
	virtual double GetCurrentSystemTimeInSeconds() const;
};

#endif // !defined( tWinClock_h )
