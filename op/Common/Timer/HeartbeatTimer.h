// Common/Timer/HeartbeatTimer.h

#if !defined( HeartbeatTimer_h )
#define HeartbeatTimer_h

#include "Common/AutoPtr.h"
#include "Common/BaseTypes.h"
#include "Common/Thread/Optex.h"
#include "Common/Thread/Thread.h"

#include <vector>
#include "Common/FunctionCallback.h"

class tClock;

// HYPOTHESIS: the SDK derives tHeartbeatTimer from tTimer, which is not declared in op/ yet;
// the base and the timer members follow when the first of them is matched.
class tHeartbeatTimer
{
public:
	static void OnAutoRunHB( int32 context );

	static void Heartbeat();

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
};

#endif // !defined( HeartbeatTimer_h )
