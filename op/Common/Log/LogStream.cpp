// Common/Log/LogStream.cpp

#include "Common/BaseTypes.h"
#include "Common/Thread/ThreadState.h"

#include "Common/Log/LogStream.h"

// HYPOTHESIS: the SDK defines this destructor inline in its header; the original inlines it
// into ~tStreamBuf.
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

namespace nLog
{

// FUNCTION: STARFLEETOP 0x006fe4f0
tLogStream::tStreamBuf::tStreamBuf( tLog* eventLog )
	: fLog( eventLog )
	, fEventType( tLog::kErrorEvent )
	, fLockType( false )
	, fAccess( tThreadState::GetCurThreadState()->DefaultThreadObjectUse() )
{}

// The two inlined basic_string::_Tidy calls of 0x006fe620 lack the toolchain's _FROZEN test
// (the library difference noted at 0x00708820); the rest lines up.
// FUNCTION: STARFLEETOP 0x006fe600
// FUNCTION: STARFLEETOP 0x006fe620
tLogStream::tStreamBuf::~tStreamBuf()
{}

};
