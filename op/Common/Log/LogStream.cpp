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

// MATCH-ONLY: the destructor itself is 0x006fe360; this is the compiler's deleting form.
// FUNCTION: STARFLEETOP 0x006feed0
tLogStream::~tLogStream()
{}

// FUNCTION: STARFLEETOP 0x006fe4f0
tLogStream::tStreamBuf::tStreamBuf( tLog* eventLog )
	: fLog( eventLog )
	, fEventType( tLog::kErrorEvent )
	, fLockType( false )
	, fAccess( tThreadState::GetCurThreadState()->DefaultThreadObjectUse() )
{}

// MATCH-ONLY: the destructor (0x006fe620) is implicit; its two inlined basic_string::_Tidy calls
// lack the toolchain's _FROZEN test (0x00708820), so it is unclaimed.
// FUNCTION: STARFLEETOP 0x006fe600
tLogStream::tStreamBuf::~tStreamBuf()
{}

// The inlined basic_string::_Grow calls of assign and append lack the toolchain's _FROZEN test
// (the library difference noted at 0x00708820), and the original's assign clears the length
// before growing.
// FUNCTION: STARFLEETOP 0x006fe720
std::streamsize tLogStream::tStreamBuf::xsputn( const char* text, std::streamsize n )
{
	LockAccess();

	if( n == 0 )
		return 0;

	if( text[0] == '%' && text[n - 1] == '%' )
	{
		fCommand.assign( text, n );
		RunCommand();
		return n;
	}

	fEventBuffer.append( text, n );
	return n;
}

// FUNCTION: STARFLEETOP 0x006fe880
int tLogStream::tStreamBuf::overflow( int ch )
{
	LockAccess();
	return xsputn( (const char*)&ch, 1 );
}

// Unclaimed (0x006fe8a0): the original shares one return-0 body between unrelated classes.
int tLogStream::tStreamBuf::underflow()
{
	return 0;
}

// FUNCTION: STARFLEETOP 0x006fe8b0
void tLogStream::tStreamBuf::Report()
{
	LockAccess( true );
	fLog->LogReport( fEventType, fEventBuffer.c_str(), 0, 0x400f0001, NULL );
	fEventBuffer.erase();
	LockAccess( false );
}

// FUNCTION: STARFLEETOP 0x006feda0
void tLogStream::tStreamBuf::SetEventType( tLog::tEventType eventType )
{
	fEventType = eventType;
	if ( eventType != 0 )
		fLockType = true;
	else
		fLockType = false;
}

// FUNCTION: STARFLEETOP 0x006fedc0
tLogStream::tLogStream( tLog::tEventType eventType )
	: std::basic_ostream< char, std::char_traits< char > >( 0 )
	, fStreamBuf( tLog::Instance() )
{
	fStreamBuf.SetEventType( eventType );
	init( &fStreamBuf );
}

};
