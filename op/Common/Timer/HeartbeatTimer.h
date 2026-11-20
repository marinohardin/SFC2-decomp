// Common/Timer/HeartbeatTimer.h

#if !defined( HeartbeatTimer_h )
#define HeartbeatTimer_h

#include "Common/AutoPtr.h"
#include "Common/BaseTypes.h"
#include "Common/Thread/Optex.h"
#include "Common/Thread/Thread.h"
#include "Common/Timer/Timer.h"

#include <vector>
#include "Common/FunctionCallback.h"

class tClock;

class tHeartbeatTimer : public tTimer
{
public:
// Setup
	tHeartbeatTimer();
	virtual ~tHeartbeatTimer();

// Operations
	virtual void StartTimer( int32u dueTime, int32u period, tTimerHandler* pTimerHandler );
	virtual void StartTimer( int32u dueTime, int32u period, iTimerCallbackFunc* pCallback, int32u context );
	virtual void StartTimer( int32u dueTime, int32u period, iTimerCallback* pCallback, int32u context );
	virtual void ChangeTiming( int32u newDueTime, int32u newPeriod );
	virtual void StopTimer();

	static void OnAutoRunHB( int32 context );

	static void Heartbeat();

//Accessors
	virtual int32u TimePassedSinceStart();

// Types
	typedef std::vector<tHeartbeatTimer*> iTimers;
	typedef tFunctionCallback< int32 > iThreadCallback;

	class tHeartbeatTimerData
	{
	public:
		static tHeartbeatTimerData* Instance();

		tOptex			fAccessGlobalData;
		tClock*			fpClock;
		iTimers			fTimers;
		tThread			fThread;
		iThreadCallback	fThreadCB;
		int32u			fAutoRunFrequency;

		typedef std::auto_ptr< tHeartbeatTimerData > iPtr;
	private:
		friend iPtr;

		tHeartbeatTimerData();
		~tHeartbeatTimerData();

		static iPtr fHeartbeatTimerData;
	};

public:
// Fields
	int32u					fLastTime;
	int32u					fInitalTime;
	int32					fTimerIndex;
	tHeartbeatTimerData*	fHBData;
};

#endif // !defined( HeartbeatTimer_h )
