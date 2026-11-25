// Common/Thread/Thread.h

#if !defined( Thread_h )
#define Thread_h

#include "Common/BaseTypes.h"
#include "Common/Callback.h"
#include "Common/Thread/Optex.h"
#include "Common/Thread/ThreadEvent.h"

// CLASS PURPOSE: A class that encapsulates threading
class tThread
{
public:
// Types
	typedef tCallback< int32 > iThreadCallback;

// Setup
	tThread();
	~tThread();

// Operations
	bool StartWork( iThreadCallback* workItem, int32 context );
	void StopThread( int32u waitTime = 0 );

	// HYPOTHESIS: the enumerators are the SDK's; the original's names and values are not
	// recovered (the symbol of 0x006ff6d0 only names the type).
	enum ThreadPriority
	{
		kThreadPriorityIdle = 0,
		kThreadPriorityLow,
		kThreadPriorityBelowNormal,
		kThreadPriorityNormal,
		kThreadPriorityAboveNormal,
		kThreadPriorityHigh,
		kThreadPriorityRealTime
	};

	void mSetPriority( ThreadPriority priority );

// Accessors
	bool Working() const;
	bool Alive() const;

protected:
// Operations
	// HYPOTHESIS: the thread's work loop (the SDK's Work), 0x006ff710; no symbol names it.
	void fn_006ff710();
	static unsigned int __stdcall ThreadProc( void* param );

// Fields
	// HYPOTHESIS: fThread is the thread handle; the SDK also has fWaitingForThreadID and
	// fWaitingForOptex here, this build's layout has no room for them.
	int32u			fThread;			// 0x00
	tThreadEvent	fWorktimeEvent;		// 0x04
	tThreadEvent	fShutdownCompleteEvent;	// 0x10
	unsigned int	fThreadID;			// 0x1c

	// HYPOTHESIS: two tInterlockedScalar<bool> (vtable at 0x00, a tResourceGuard at 0x04, the
	// value at 0x1c), 0x20 bytes each. Working() reads fWorking, Alive() fAlive. Opaque until
	// the interlocked types exist as headers.
	char			fWorking[0x20];		// 0x20
	char			fAlive[0x20];		// 0x40
	iThreadCallback* fWorkItem;			// 0x60
	int32			fContext;			// 0x64
	tOptex			fStartUpGuard;		// 0x68, built with (kUseThreadObject, 4000)

	// HYPOTHESIS: the SDK's tCallStack fCallStackOfCallingThread, 0xc90 bytes; opaque until
	// Common/Debug/CallStack.h has content.
	char			fCallStackOfCallingThread[0xc90];	// 0x7c

private:
//Hidden
	tThread( const tThread& copy );
	tThread& operator=( const tThread& copy );
};

typedef char tThread_size_check[ sizeof( tThread ) == 0xd0c ? 1 : -1 ];

#endif
