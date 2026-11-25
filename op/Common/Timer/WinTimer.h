// Common/Timer/WinTimer.h

#if !defined( WinTimer_h )
#define WinTimer_h

#include "Common/BaseTypes.h"
#include "Common/Timer/Timer.h"

class tClock;

// HYPOTHESIS: the original header is not known. The virtuals follow the vtable at
// 0x007bb5b0 and the symbols; the fields are read from the constructor, the destructor and
// StartTimer (the object is 0x30 bytes).
class tWinTimer : public tTimer
{
public:
// Setup
	tWinTimer();
	virtual ~tWinTimer();

// Operations
	virtual void StartTimer( int32u dueTime, int32u period, tTimerHandler* pTimerHandler );
	virtual void StartTimer( int32u dueTime, int32u period, iTimerCallbackFunc* pCallback, int32u context );
	virtual void StartTimer( int32u dueTime, int32u period, iTimerCallback* pCallback, int32u context );
	virtual void ChangeTiming( int32u newDueTime, int32u newPeriod );
	virtual void StopTimer();

//Accessors
	virtual int32u TimePassedSinceStart();

protected:
	static void __stdcall WinTimerCallback( unsigned int timerId, unsigned int msg, int32u user, int32u dw1, int32u dw2 );

// Fields
	unsigned int	fTimerID;	// 0x28, the multimedia timer's id, 0 when none
	tClock*			fpClock;	// 0x2c, a tWinClock the timer owns
};

#endif
