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

// HYPOTHESIS: the object at +0x7c is a class not recovered; this reduced declaration carries
// only the member 0x00715500 that 0x0071d090 forwards to.
class fn_00715500_Object
{
public:
	void fn_00715500( int32u arg );
};

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

// FUNCTION: STARFLEETOP 0x0071d090
void nSwitch::tDataHandler::fn_0071d090( int32u arg )
{
	((fn_00715500_Object*)f_7c)->fn_00715500( arg );
}

// HYPOTHESIS: sets the interlocked flag at +0x58, then replaces the smart pointer at +0xac.
// FUNCTION: STARFLEETOP 0x0071d0a0
void nSwitch::tDataHandler::fn_0071d0a0( tSmartPtr<tAddress> address )
{
	f_58 = true;
	// HYPOTHESIS: the original inlined the smart pointer's pointer assignment here; SmartPtr.h
	// declares it without a body, so the build calls it out of line.
	f_ac = address.Get();
}
