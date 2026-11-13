#include <windows.h>
#include <stdio.h>

#include "Tiger/tsay.h"

#include <vector>
// std::find, for the tail of twCoopThreadPart::mCoopRun.
#include <algorithm>

// HYPOTHESIS: field names are ours; the layout (16 bytes: id, thread, text,
// param) comes from the two format strings DumpEventLog prints them with.
struct twThreadEvent
{
	long        id;
	long        thread;
	const char* text;
	long        param;
};

// HYPOTHESIS: array length unknown.
CRITICAL_SECTION twEventCrit;
long             twEventCount;
twThreadEvent    twEvents[1024];

// FUNCTION: STARFLEETOP 0x00640700
void DumpEventLog()
{
	InitializeCriticalSection( &twEventCrit );
	EnterCriticalSection( &twEventCrit );

	FILE* f = fopen( "eventlog.txt", "wt" );
	if( f )
	{
		for( long i = 0; i < twEventCount; i++ )
		{
			if( twEvents[i].param )
				fprintf( f, "\n {%d/%8lx} %s [param = %d]", twEvents[i].id, twEvents[i].thread, twEvents[i].text, twEvents[i].param );
			else
				fprintf( f, "\n {%d/%8lx} %s ", twEvents[i].id, twEvents[i].thread, twEvents[i].text );
		}
		fclose( f );
	}

	LeaveCriticalSection( &twEventCrit );
	DeleteCriticalSection( &twEventCrit );
}

// FUNCTION: STARFLEETOP 0x006407b0
unsigned char MyReleaseSemaphore( void* sem )
{
	LONG prev = -1;
	unsigned char ok = ReleaseSemaphore( sem, 1, &prev );
	if( !ok )
	{
		GetLastError();
		return 0;
	}
	if( prev )
	{
		// HYPOTHESIS: a deliberate fault (read of address -1) when the semaphore
		// was already signalled; local names are ours.
		static char fault = *(char*)-1;
		return 0;
	}
	return 1;
}

// FUNCTION: STARFLEETOP 0x00640810
void MyWaitForSingleObject( void* handle )
{
	// HYPOTHESIS: deliberate faults (read of address -1) on an abandoned wait
	// or a timeout, as in MyReleaseSemaphore; local names are ours.
	switch( WaitForSingleObject( handle, 3600000000 ) )
	{
	case WAIT_ABANDONED:
		{
			static char abandoned = *(char*)-1;
		}
		break;
	case WAIT_TIMEOUT:
		{
			static char timedOut = *(char*)-1;
		}
		break;
	}
}

#include "Tiger/tgthread.h"

class twNativeThread : public tNativeThread
{
	public:
		twNativeThread( void );
		twNativeThread( class tClientThread* thread, long freq );
		virtual ~twNativeThread( void );
		static unsigned long twNativeThreadEntryProc( tNativeThread* thread );
		virtual void mGotoThread( tNativeThread* thread );

		// HYPOTHESIS: names are offsets; the value is a semaphore, from the call that fills it. The
		// thread handle and id before it (f10, f14) are tNativeThread's.
		HANDLE			f18;
		unsigned char	f1c;
		unsigned char	f1d;
};

// FUNCTION: STARFLEETOP 0x00640900
twNativeThread::twNativeThread( void )
{
	f1c = 0;
	f10 = GetCurrentThread();
	f1d = 1;
	f14 = GetCurrentThreadId();
	f18 = CreateSemaphoreA( NULL, 0, 100, NULL );
	if( f18 == NULL )
	{
		DWORD error = GetLastError();
		if( error == ERROR_CALL_NOT_IMPLEMENTED )
			f18 = CreateSemaphoreA( NULL, 0, 100, NULL );
		else
			throw error;
	}
}

// FUNCTION: STARFLEETOP 0x006409d0
twNativeThread::twNativeThread( class tClientThread* thread, long freq )
	: tNativeThread( thread, freq )
{
	// HYPOTHESIS: freq is also the stack size given to CreateThread.
	assert( freq > 0x2000 );
	f18 = CreateSemaphoreA( NULL, 0, 100, NULL );
	f1d = 0;
	if( f18 == NULL )
	{
		DWORD error = GetLastError();
		if( error == ERROR_CALL_NOT_IMPLEMENTED )
			f18 = CreateSemaphoreA( NULL, 0, 100, NULL );
		else
			throw error;
	}
	f10 = CreateThread( NULL, freq, (LPTHREAD_START_ROUTINE)twNativeThreadEntryProc, this, 0, &f14 );
	assert( f10 );
	assert( f14 );
}

// FUNCTION: STARFLEETOP 0x00640870
unsigned long twNativeThread::twNativeThreadEntryProc( tNativeThread* thread )
{
	aCoopThreadPart->mCoopRun( thread );
	return 0;
}

// The compiler generates twNativeThread's scalar deleting destructor from the virtual destructor;
// it has no source line of its own and is claimed here, on the destructor.
// FUNCTION: STARFLEETOP 0x006409b0
// FUNCTION: STARFLEETOP 0x00640ae0
twNativeThread::~twNativeThread( void )
{
	if( f18 )
	{
		CloseHandle( f18 );
		f18 = NULL;
	}
	if( !f1d )
	{
		if( !CloseHandle( f10 ) )
			GetLastError();
	}
	f14 = 0;
	if( !f1d )
		ExitThread( 0 );
}

// FUNCTION: STARFLEETOP 0x00640b40
void twNativeThread::mGotoThread( tNativeThread* thread )
{
	// HYPOTHESIS: the structure is read from the listing; the log texts and the helpers named
	// below are matched by call order only.
	twNativeThread* target = dynamic_cast<twNativeThread*>( thread );
	assert( target );
	assert( target->f18 );

	if( target->f0c )
	{
		target->f0d = 1;

		if( !MyReleaseSemaphore( target->f18 ) )
		{
			aCoopThreadPart->mRest();
			return;
		}
		MyWaitForSingleObject( f18 );
		if( f1c != 1 && this )
			delete this;
		if( !f0d )
		{
			DumpEventLog();
			static char dumped = *(char*)-1;
			return;
		}
		f0d = 0;
	}
	else
	{
		target->f0c = 1;
		target->f0d = 1;
		LogThreadEvent( "DoSleep: GotoThread-1", (long)target->f18 );
		if( !MyReleaseSemaphore( target->f18 ) )
		{
			aCoopThreadPart->mRest();
			return;
		}
		LogThreadEvent( "DoSleep: GotoThread-0", (long)f18 );
		MyWaitForSingleObject( f18 );
		if( f1c != 1 && this )
			delete this;
		if( !f0d )
		{
			DumpEventLog();
			static char dumped2 = *(char*)-1;
			return;
		}
		f0d = 0;
	}
}

class twCoopThreadPart : public tCoopThreadPart
{
	public:
		virtual void mCoopRun( tNativeThread* thread );
		// HYPOTHESIS: the tail of mCoopRun after its catch( ... ), built on its own until mCoopRun
		// is scored as a whole; see below.

	protected:
		virtual void mDoDelThread( class tClientThread* thread );
	private:
		virtual tNativeThread* mNewMainThread( void );
		virtual tNativeThread* mNewNativeThread( class tClientThread* thread, long freq );
		virtual void mDelNativeThread( tNativeThread* thread );
};

// FUNCTION: STARFLEETOP 0x00640db0
void twCoopThreadPart::mCoopRun( tNativeThread* thread )
{
	assert( thread );
	twNativeThread* target = dynamic_cast<twNativeThread*>( thread );
	assert( target );

	MyWaitForSingleObject( target->f18 );
	if( !target->f0d )
	{
		DumpEventLog();
		static char dumped = *(char*)-1;
	}
	else
		target->f0d = 0;

	tCoopThread* coop = mFindCoopThread( thread );
	assert( coop );

	fUnknown_1c70 = 0;
	fUnknown_1c68 = coop;
	fUnknown_1c6c = 0;
	fUnknown_1c74 = 0;

	twNativeThread* native = static_cast<twNativeThread*>( coop->fUnknown_0c );
	assert( native );
	if( native->f14 != GetCurrentThreadId() )
	{
		static char wrongThread = *(char*)-1;
	}

	try
	{
		aCoopThreadPart->mRest();
		coop->fUnknown_08->mUnknown_10();
	}
	catch( ... )
	{
	}
}

// 0x00640fa3 is where twCoopThreadPart::mCoopRun resumes after its catch( ... ) (the funclet
// returns it), running in mCoopRun's frame; it is scored with mCoopRun, not claimed apart.

































// FUNCTION: STARFLEETOP 0x00641100
tNativeThread* twCoopThreadPart::mNewMainThread( void )
{
	return new twNativeThread;
}

// FUNCTION: STARFLEETOP 0x00641160
tNativeThread* twCoopThreadPart::mNewNativeThread( class tClientThread* thread, long freq )
{
	return new twNativeThread( thread, freq );
}

// FUNCTION: STARFLEETOP 0x006411d0
void twCoopThreadPart::mDelNativeThread( tNativeThread* thread )
{
	delete thread;
}

// FUNCTION: STARFLEETOP 0x006411f0
void twCoopThreadPart::mDoDelThread( class tClientThread* thread )
{
	tCoopThread* coop = mFindCoopThread( thread );
	assert( coop );
	assert( coop != fUnknown_1c68 );
	assert( coop != fUnknown_1c64 );
	assert( coop->fUnknown_0c );
	twNativeThread* native = static_cast<twNativeThread*>( coop->fUnknown_0c );

	mFreeCoopThread( coop );
	thread->mUnknown_08();

	// HYPOTHESIS: the native thread is not deleted here; it is flagged and woken so that it
	// deletes itself on return from its wait (see mGotoThread). The original tests native for
	// NULL again here; this build drops that test after the assert.
	if( native && native->f18 )
	{
		native->f1c = 1;
		// HYPOTHESIS: MyReleaseSemaphore written out in place; local names are ours.
		LONG prev = -1;
		unsigned char ok = ReleaseSemaphore( native->f18, 1, &prev );
		if( !ok )
		{
			GetLastError();
			return;
		}
		if( prev )
		{
			static char fault = *(char*)-1;
		}
	}
}
