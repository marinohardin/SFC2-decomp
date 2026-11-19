// Common/Database/DatabaseObject.cpp

#include "Common/Database/DatabaseObject.h"
#include "Common/Exceptions/Validate.h"
#include "Common/OSVersion.h"
#include "Common/Thread/Optex.inl"
#include "Common/Log/LogStream.h"

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

inline tOptex::tGuard::~tGuard()
{
	if( fNeedToLeave )
	{
		fOptex->Leave();
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

// FUNCTION: STARFLEETOP 0x006fa6d0
tDatabaseObject::tDatabaseObject( const tDatabaseObject& copy )
	: fAlive( true )
	, fReferenceCount( 0 )
	, fLockEvent( NULL )
{
	*this = copy;
}

// FUNCTION: STARFLEETOP 0x006fa6b0
tDatabaseObject::~tDatabaseObject()
{
	// HYPOTHESIS: the constructor allocates fLockEvent, so the destructor releases it.
	delete fLockEvent;
}

// FUNCTION: STARFLEETOP 0x006fa870
tDatabaseObject& tDatabaseObject::operator=( const tDatabaseObject& other )
{
	tAllocatedClass::operator=( other );
	tStreamable::operator=( other );

	if( this != &other )
	{
		tOptex::tGuard guard( &fAccess );
		tOptex::tGuard otherGuard( &other.fAccess );

		delete fLockEvent;

		if( !other.fAlive )
		{
			nLog::error << "A dead database object was copyied" << nLog::report;
		}

		fID = other.fID;
		fLockEvent = NULL;
		fLockID = other.fLockID;
	}

	return *this;
}

// FUNCTION: STARFLEETOP 0x006fa9f0
bool tDatabaseObject::operator<( const tDatabaseObject& obj )
{
	return GetID() < obj.GetID() ? true : false;
}

}
