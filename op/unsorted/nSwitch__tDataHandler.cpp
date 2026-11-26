#include "Switch/DataHandler.h"
#include "Common/Thread/ThreadPool.h"
#include "Common/Log/LogStream.h"

// HYPOTHESIS: 0x00716040 writes an address to a stream, like an operator<<; not recovered.
std::ostream& __cdecl fn_00716040( std::ostream& stream, int32u* address );

// The destructor of f_04 is inlined into ~tDataHandler.
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

// The body lacks one call until nSwitch::tSwitch::fn_00713fb0 is declared in Switch.h.
// FUNCTION: STARFLEETOP 0x0071c8a0
// FUNCTION: STARFLEETOP 0x0071c8c0
nSwitch::tDataHandler::~tDataHandler()
{
	bool value = f_58;
	if( value )
	{
		// HYPOTHESIS: the original unregisters the handler here, nSwitch::tSwitch::fn_00713b20()
		// ->fn_00713fb0( this ); the call is missing (see the HEADER REQUEST).
	}
	delete f_78;
	if( f_b0 != NULL )
	{
		*f_b0 = true;
	}
}

// FUNCTION: STARFLEETOP 0x0071d010
void nSwitch::tDataHandler::fn_slot6( int32u arg )
{
	if( fn_slot1() )
	{
		// HYPOTHESIS: the argument is a reference-counted object passed on as a long.
		((tRefCount*)arg)->AddRef();
		tThreadPool::Instance()->QueueWorkItem( f_78, (int32)arg );
	}
	else
	{
		// HYPOTHESIS: the stream and the helper that prints the address are not recovered.
		fn_00716040( nLog::info, f_7c ) << " received packet " << *(int32*)( arg + 0x50 ) << " while waiting to be deleted" << nLog::report;
	}
}
