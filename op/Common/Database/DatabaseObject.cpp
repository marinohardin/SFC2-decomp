// Common/Database/DatabaseObject.cpp

#include "Common/Database/DatabaseObject.h"

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

namespace nDatabase
{

// The inlined basic_string::_Tidy of the name argument lacks the toolchain's _FROZEN test (the
// library difference noted at 0x00708820); the rest of 0x006fa540 lines up.
// FUNCTION: STARFLEETOP 0x006fa540
tDatabaseObject::tDatabaseObject( iObjectVersion versionNumber )
	: tStreamable( versionNumber )
	, fAccess( tThreadState::GetCurThreadState()->DefaultThreadObjectUse() )
	, fAlive( true )
	, fReferenceCount( 0 )
	, fLockEvent( new tThreadEvent( true, false, "", tThreadState::kUseThreadObject ) )
	, fLockID( 0 )
{
}

// FUNCTION: STARFLEETOP 0x006fa6b0
tDatabaseObject::~tDatabaseObject()
{
	// HYPOTHESIS: the constructor allocates fLockEvent, so the destructor releases it.
	delete fLockEvent;
}

}
