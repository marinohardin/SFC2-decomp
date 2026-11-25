// Common/Timer/Timer.h

#if !defined( Timer_h )
#define Timer_h

#include "Common/BaseTypes.h"
#include "Common/Callback.h"

class tTimer;
class tClock;

// CLASS PURPOSE: A interface class for a timer callback
// HYPOTHESIS: the destructor is declared and not defined, as the SDK defines it inline; slot 1
// is what tTimer::SignalTimerEvent (0x0070fdb0) calls through.
class tTimerHandler
{
public:
//Setup
	virtual ~tTimerHandler();

//Operations
	virtual void HandlerTimerEvent( tTimer* pTimer ) = 0;
};

class tTimer
{
public:
//Types
	typedef void (iTimerCallbackFunc)( tTimer* pTimer, int32u context );
	typedef tCallback<int32u> iTimerCallback;

//Setup
	tTimer();
	virtual ~tTimer();

//Operations
	virtual void StartTimer( int32u dueTime, int32u period, tTimerHandler* pTimerHandler ) = 0;
	virtual void StartTimer( int32u dueTime, int32u period, iTimerCallbackFunc* pCallback, int32u context ) = 0;
	virtual void StartTimer( int32u dueTime, int32u period, iTimerCallback* pCallback, int32u context ) = 0;
	virtual void ChangeTiming( int32u newDueTime, int32u newPeriod ) = 0;
	virtual void StopTimer() = 0;

//Accessors
	virtual int32u TimePassedSinceStart() = 0;

protected:
//Types
	enum eCallbackType
	{
		kNoCallback,
		kTimerHandler,
		kTimerCallbackFuncton,
		kCallbackClass
	};

// Operations
	virtual void SignalTimerEvent();

// Fields
	eCallbackType		fCallbackType;
	tTimerHandler*		fpTimerHandler;
	iTimerCallbackFunc*	fpCallbackFunc;
	iTimerCallback*		fpCallback;
	int32u				fContext;

	int32u	fInitalTime;
	bool	fTimerHasStarted;
	int32u	fDueTime;
	int32u	fPeriod;
};

#endif
