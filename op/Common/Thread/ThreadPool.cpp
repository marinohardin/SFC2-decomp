// Common/Thread/ThreadPool.cpp

#include <windows.h>
#include <exception>
#include <vector>
#include <list>
#include <ostream>

#include "Common/Thread/Thread.h"	// int32, int32u, tCallback (forward), tThread; the lines below stay blank























#include "Common/Exceptions/Validate.h"	// nException::tBaseException, tValidationFailed, validate
// The lines below up to validate_throw stay blank so the code keeps its line numbers.
































#define validate_throw( expr, validateThrowObject )	\
	if( !(expr) )										\
	{													\
		throw validateThrowObject;						\
	}








#include "Common/Thread/ThreadState.h"
#include "Common/Thread/Optex.h"

inline tOptex::~tOptex()
{
	if( fUse != tThreadState::kIgnoreThreadObject )
	{
		if( fUseCriticalSections && fpSharedInfo != NULL )
		{
			::DeleteCriticalSection( &fpSharedInfo->fCriticalSection );
		}

		if( fEvent != NULL )
		{
			::CloseHandle( fEvent );
		}

		if( fFileMapping == NULL )
		{
			delete fpSharedInfo;
		}
		else
		{
			::UnmapViewOfFile( fpSharedInfo );
			::CloseHandle( fFileMapping );
		}
	}
}

inline void tOptex::Leave()
{
	if( fUse != tThreadState::kIgnoreThreadObject )
	{
		if( fUseCriticalSections )
		{
			::LeaveCriticalSection( &fpSharedInfo->fCriticalSection );
		}
		else
		{
			--fpSharedInfo->fRecurseCount;
			if( fpSharedInfo->fRecurseCount > 0 )
			{
				::InterlockedDecrement( &fpSharedInfo->fLockCount );
			}
			else
			{
				fpSharedInfo->fThreadId = 0;
				if( ::InterlockedDecrement( &fpSharedInfo->fLockCount ) > 0 )
				{
					::SetEvent( fEvent );
				}
			}
		}
	}
}

// The lines below up to tOptexLock stay blank so the code keeps its line numbers.






































// HYPOTHESIS: a scoped lock on a tOptex, fully inline (no symbol survives); its name is not
// recovered. Layout follows the stack slots in ~tThreadPool: the flag is stored after the
// pointer and never read on this path, and the destructor leaves unconditionally.
class tOptexLock
{
public:
	inline tOptexLock( tOptex& optex )
		: fpOptex( &optex )
	{
		fLocked = true;
		fpOptex->Enter();
	}
	inline ~tOptexLock()
	{
		fpOptex->Leave();
	}

protected:
	bool	fLocked;
	tOptex*	fpOptex;
};

#include "Common/Thread/ThreadPool.h"	// tThreadPool, and the std::deque it holds
// The lines below up to tPerformanceCounter stay blank so the code keeps its line numbers.




















































































































#include "Common/Callback.h"








#include "Common/Log/LogStream.h"










// tThreadPool is declared in Common/Thread/ThreadPool.h; the lines below up to Instance stay
// blank so the code keeps its line numbers.

























































// FUNCTION: STARFLEETOP 0x006f39b0
tThreadPool* tThreadPool::Instance()
{
	if( fThreadPool == 0 )
	{
		fThreadPool = new tThreadPool();
	}

	return fThreadPool;
}

// FUNCTION: STARFLEETOP 0x006f3a20
tThreadPool::tThreadPool()
	: fMaxThreadCount( 0 )
{
	fUnknown18 = 0;
	fUnknown14 = 0;
	fUnknown30 = 1.0f;
	fUnknown34 = 10000;
	fUnknown38 = 75;
	fRunning = true;
	fUnknown3d = false;
	fUnknown3e = false;
}

// FUNCTION: STARFLEETOP 0x006f3b40
tThreadPool::~tThreadPool()
{
	tOptexLock lock( fUnknown40 );

	// The original's check reports line 68.
#line 68
	validate( fRunning == false );
#line 427

	for( std::list<tThread*>::iterator it = fUnknown58.begin(); it != fUnknown58.end(); it++ )
	{
		delete *it;
	}
	fUnknown58.clear();
}

// FUNCTION: STARFLEETOP 0x006f3d40
void tThreadPool::Startup()
{
	fRunning = true;
}

// FUNCTION: STARFLEETOP 0x006f3d50
void tThreadPool::Shutdown( int32u timeout )
{
	tOptexLock lock( fUnknown40 );

	EmptyWorkQue( true );
	fRunning = false;

	for( std::list<tThread*>::iterator it = fUnknown58.begin(); it != fUnknown58.end(); it++ )
	{
		if( *it != NULL )
		{
			(*it)->StopThread( timeout );
		}
	}
}

// FUNCTION: STARFLEETOP 0x006f3e50
bool tThreadPool::ShutdownComplete()
{
	bool complete = true;
	tOptexLock lock( fUnknown40 );

	for( std::list<tThread*>::iterator it = fUnknown58.begin(); it != fUnknown58.end() && complete; it++ )
	{
		if( *it != NULL && (*it)->Working() )
		{
			complete = false;
		}
	}

	return complete;
}

// HYPOTHESIS: Enter is inline (SetMaxThreads expands it) but defined only here, after the
// functions above that call its out-of-line copy; TryEnter stays a call and Wait expands to the
// event wait.
inline void tOptex::Wait()
{
	::WaitForSingleObject( fEvent, INFINITE );
}

inline void tOptex::Enter()
{
	if( fUse == tThreadState::kIgnoreThreadObject )
	{
		return;
	}

	if( fUseCriticalSections )
	{
		::EnterCriticalSection( &( fpSharedInfo->fCriticalSection ) );
		return;
	}

	if( TryEnter() )
	{
		return;
	}

	int32u threadId = ::GetCurrentThreadId();

	if( ::InterlockedIncrement( &fpSharedInfo->fLockCount ) == 1 )
	{
		fpSharedInfo->fThreadId = threadId;
		fpSharedInfo->fRecurseCount = 1;
	}
	else
	{
		if( fpSharedInfo->fThreadId == threadId )
		{
			fpSharedInfo->fRecurseCount++;
		}
		else
		{
			Wait();

			fpSharedInfo->fThreadId = threadId;
			fpSharedInfo->fRecurseCount = 1;
		}
	}
}

// FUNCTION: STARFLEETOP 0x006f3f30
void tThreadPool::SetMaxThreads( int32u maxThreadCount )
{
	tOptexLock lock( fUnknown40 );

	fMaxThreadCount = maxThreadCount;

	// The original's check reports line 140.
#line 140
	validate( fMaxThreadCount == 1 || fMaxThreadCount == kUnlimitedThreads );
#line 535
}

namespace OSSPECIFIC
{
	extern LONG (*OS_InterlockedCompareExchange)( LPLONG Destination, LONG Exchange, LONG Comperand );
}

// HYPOTHESIS: TryEnter is inline like Enter (GetMaxThreads expands both) but defined only here,
// after SetMaxThreads, which still calls its out-of-line copy.
inline bool tOptex::TryEnter()
{
	if( fUse == tThreadState::kIgnoreThreadObject )
	{
		return false;
	}

	if( fUseCriticalSections )
	{
		return false;
	}

	int32u threadId = ::GetCurrentThreadId();

	bool thisThreadOwnsTheOptex = false;
	int32u spinCount = fpSharedInfo->fSpinCount;

	do
	{
		thisThreadOwnsTheOptex = ( 0 == ( *( OSSPECIFIC::OS_InterlockedCompareExchange ) )( &fpSharedInfo->fLockCount, 1, 0 ) );

		if( thisThreadOwnsTheOptex )
		{
			fpSharedInfo->fThreadId = threadId;
			fpSharedInfo->fRecurseCount = 1;
		}
		else
		{
			if( fpSharedInfo->fThreadId == threadId )
			{
				::InterlockedIncrement( &fpSharedInfo->fLockCount );
				fpSharedInfo->fRecurseCount++;
				thisThreadOwnsTheOptex = TRUE;
			}
		}
	} while( !thisThreadOwnsTheOptex && ( spinCount-- > 0 ) );

	return( thisThreadOwnsTheOptex );
}

// FUNCTION: STARFLEETOP 0x006f40a0
int32u tThreadPool::GetMaxThreads() const
{
	tOptexLock lock( const_cast<tOptex&>( fUnknown40 ) );

	return fMaxThreadCount;
}

// FUNCTION: STARFLEETOP 0x006f41e0
void tThreadPool::Heartbeat()
{
	tOptexLock lock( fUnknown40 );

	int32u startTime = fUnknown20.GetElapsedMicroseconds();

	if( fUnknown3d == true || !fRunning || startTime - fUnknown18 < fUnknown14 )
	{
		return;
	}

	if( fUnknown00.size() > fUnknown38 )
	{
		EmptyWorkQue( true );
		fUnknown14 = 0;
	}
	else
	{
		while( !fUnknown00.empty() && fUnknown20.GetElapsedMicroseconds() - startTime < fUnknown34 )
		{
			tWork work = fUnknown00.front();
			fUnknown00.pop_front();
			work.fpObject->Callback( work.fArgument );
		}

		fUnknown18 = fUnknown20.GetElapsedMicroseconds();
		fUnknown14 = (int32u)( ( fUnknown18 - startTime ) * fUnknown30 );
		if( fUnknown14 > 1000000 )
		{
			fUnknown14 = 1000000;
		}
	}
}

// FUNCTION: STARFLEETOP 0x006f4440
void tThreadPool::EmptyWorkQue( bool doWork )
{
	tOptexLock lock( fUnknown40 );

	while( !fUnknown00.empty() )
	{
		tWork work = fUnknown00.front();
		fUnknown00.pop_front();
		// MATCH-ONLY: the original tests the flag against zero; a plain test here duplicates the
		// lock's release at every exit, while this form keeps the original's single exit.
		if( doWork == true )
		{
			work.fpObject->Callback( work.fArgument );
		}
	}
}

// FUNCTION: STARFLEETOP 0x006f4550
// HYPOTHESIS: the flag at 0x3e is the memory-checking switch this setter writes.
void tThreadPool::SetMemoryChecking( bool memoryChecking )
{
	fUnknown3e = memoryChecking;
}

// FUNCTION: STARFLEETOP 0x006f4560
// HYPOTHESIS: the float at 0x30 (Heartbeat's delay scale, 1.0 by default) is the delay multiple.
void tThreadPool::SetDelayMultiple( float delayMultiple )
{
	tOptexLock lock( fUnknown40 );

	fUnknown30 = delayMultiple;
}

// FUNCTION: STARFLEETOP 0x006f46b0
// HYPOTHESIS: the value at 0x34 (Heartbeat's work time budget in microseconds) is the time block.
void tThreadPool::SetTimeBlockSize( int32u timeBlockSize )
{
	tOptexLock lock( fUnknown40 );

	fUnknown34 = timeBlockSize;
}

// FUNCTION: STARFLEETOP 0x006f4800
// HYPOTHESIS: the value at 0x38 (Heartbeat's queue length limit, 75 by default) is the packet
// queue limit.
void tThreadPool::SetPacketQueLimit( int32u packetQueLimit )
{
	tOptexLock lock( fUnknown40 );

	fUnknown38 = packetQueLimit;
}

// FUNCTION: STARFLEETOP 0x006f4950
// HYPOTHESIS: the float at 0x30 is the delay multiple, as in SetDelayMultiple.
float tThreadPool::GetDelayMultiple() const
{
	tOptexLock lock( const_cast<tOptex&>( fUnknown40 ) );

	return fUnknown30;
}

// FUNCTION: STARFLEETOP 0x006f4aa0
// HYPOTHESIS: the value at 0x34 is the time block size, as in SetTimeBlockSize.
int32u tThreadPool::GetTimeBlockSize() const
{
	tOptexLock lock( const_cast<tOptex&>( fUnknown40 ) );

	return fUnknown34;
}

// FUNCTION: STARFLEETOP 0x006f4be0
void tThreadPool::QueueWorkItem( tCallback<int32>* workItem, int32 context )
{
	tOptexLock lock( fUnknown40 );

	// The original's check reports line 314.
#line 314
	validate_throw( fRunning, nException::tBaseException( "Someone tried to Que a thread, after the ThreadPool had shutdown", __FILE__, __LINE__ ) );
#line 707

	if( fMaxThreadCount == 1 )
	{
		tWork work;
		work.fpObject = workItem;
		work.fArgument = context;
		fUnknown00.push_back( work );
		return;
	}

	bool deadThreads = false;
	tThread* thread;
	bool started;

	do
	{
		thread = NULL;

		for( std::list<tThread*>::iterator it = fUnknown58.begin(); it != fUnknown58.end() && thread == NULL; it++ )
		{
			if( !(*it)->Working() )
			{
				if( (*it)->Alive() )
				{
					thread = *it;
				}
				else
				{
					deadThreads = true;
				}
			}
		}

		if( thread == NULL )
		{
			thread = new tThread();
			fUnknown58.push_back( thread );
		}

		started = thread->StartWork( workItem, context );
		if( !started )
		{
			nLog::warning << "A Thread died before I could use it, I will try to get another out of the pool" << nLog::report;
		}
	} while( !started );

	if( deadThreads )
	{
		for( std::list<tThread*>::iterator it = fUnknown58.begin(); it != fUnknown58.end(); )
		{
			if( !(*it)->Alive() )
			{
				delete *it;
				it = fUnknown58.erase( it );
			}
			else
			{
				it++;
			}
		}
	}
}

