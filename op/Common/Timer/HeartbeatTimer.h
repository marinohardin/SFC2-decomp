// Common/Timer/HeartbeatTimer.h

#if !defined( HeartbeatTimer_h )
#define HeartbeatTimer_h

#include "Common/AutoPtr.h"
#include "Common/BaseTypes.h"
#include "Common/Thread/Optex.h"
#include "Common/Thread/Thread.h"

class tClock;

// HYPOTHESIS: the SDK derives tHeartbeatTimer from tTimer, which is not declared in op/ yet;
// the base and the timer members follow when the first of them is matched.
class tHeartbeatTimer
{
public:
	static void Heartbeat();

	class tHeartbeatTimerData
	{
	public:
		static tHeartbeatTimerData* Instance();

		tOptex			fAccessGlobalData;
		tClock*			fpClock;
		// HYPOTHESIS: the SDK's iTimers, a std::vector< tHeartbeatTimer* > (16 bytes); the
		// compiler's own <vector> brings its <memory>, whose auto_ptr is not the game's.
		char			fTimers[16];
		tThread			fThread;
		// HYPOTHESIS: the SDK's tFunctionCallback< int32 >, not declared in op/ yet; 8 bytes
		// from the allocation size in Instance (0xd40).
		char			fThreadCB[8];
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
