// Common/Thread/Thread.cpp

#include <process.h>

#include "Common/Thread/Thread.h"

// The inlined basic_string::_Tidy of the second event's name lacks the toolchain's _FROZEN
// test (the library difference noted at 0x00708820); the rest of 0x006fefd0 lines up.
// FUNCTION: STARFLEETOP 0x006fefd0
tThread::tThread() :
	fWorking( false ),
	fAlive( true ),
	fCallStackOfCallingThread( false )
{
	fThread = _beginthreadex( NULL, 0, ThreadProc, this, 0, &fThreadID );
}

// The destructor of fStartUpGuard is inlined into ~tThread; the guards inside fWorking and
// fAlive call the out-of-line copy.
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

// FUNCTION: STARFLEETOP 0x006ff170
tThread::~tThread()
{
	StopThread( 60000 );
}

#include "Common/Thread/WaitForObject.h"
#include "Common/Exceptions/Validate.h"

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

inline tOptex::tGuard::~tGuard()
{
	if( fNeedToLeave )
	{
		fOptex->Leave();
	}
}

// The validate below reports line 0x73 of the original file.
#line 103
// FUNCTION: STARFLEETOP 0x006ff4f0
void tThread::StopThread( int32u waitTime )
{
	tOptex::tGuard guard( &fStartUpGuard );
	if( !fAlive )
		return;

	fAlive = false;
	fWorktimeEvent.Signal();
	if( waitTime != 0 )
	{
		tWaitForObject::tResult result = tWaitForObject::Wait( fShutdownCompleteEvent, waitTime );
		validate( result.Ready() );
	}
}

// FUNCTION: STARFLEETOP 0x006ff6f0
bool tThread::Working() const
{
	return fWorking;
}

#include "Common/Thread/WaitForObject.h"

// HYPOTHESIS: names of the two statics at 0x00c7fc48 and 0x00c7fc46.
extern bool g_00c7fc48;
extern bool g_00c7fc46;

// FUNCTION: STARFLEETOP 0x006ff710
void tThread::fn_006ff710()
{
	while( fAlive )
	{
		tWaitForObject::tResult result = tWaitForObject::Wait( fWorktimeEvent, 60000 );
		if( result.TimedOut() )
		{
			StopThread( 0 );
		}
		else if( fAlive )
		{
			if( g_00c7fc48 )
			{
				try
				{
					fWorkItem->Callback( fContext );
				}
				catch( ... )
				{
					// HYPOTHESIS: the original's handler formats a message into a 256 byte buffer
					// (its frame has one); its content is not recovered.
					char message[ 256 ];
					message[ 0 ] = '\0';
					OutputDebugStringA( message );
				}
			}
			else
			{
				fWorkItem->Callback( fContext );
			}
		}
		if( g_00c7fc46 )
		{
			fCallStackOfCallingThread.ClearStackInfo();
		}
		fWorking = false;
	}
	fShutdownCompleteEvent.Signal();
}
