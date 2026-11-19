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

// FUNCTION: STARFLEETOP 0x006faa20
bool tDatabaseObject::operator==( const tDatabaseObject& obj )
{
	return GetID() == obj.GetID() ? true : false;
}

}

// HYPOTHESIS: Enter, TryEnter and Wait are inline (MakeReadOnly expands all three) but defined
// only here, after operator=, which still calls the out-of-line Enter.
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

namespace nDatabase
{

// FUNCTION: STARFLEETOP 0x006fac20
void tDatabaseObject::MakeReadOnly()
{
	tOptex::tGuard guard( &fAccess );

	fLockID = 0;
	if( fLockEvent != NULL )
	{
		delete fLockEvent;
		fLockEvent = NULL;
	}
}

// FUNCTION: STARFLEETOP 0x006fadc0
void tDatabaseObject::SetID( const tID& id )
{
	fID = id;
}

// FUNCTION: STARFLEETOP 0x006fade0
void tDatabaseObject::SetLockID( const int32u id )
{
	tOptex::tGuard guard( &fAccess );

	fLockID = id;
}

// The inlined basic_string::_Tidy of the name argument lacks the toolchain's _FROZEN test (the
// library difference noted at 0x00708820); everything after it lines up 4 bytes later.
// FUNCTION: STARFLEETOP 0x006faf30
void tDatabaseObject::AttachToDB( const tID& id )
{
	tOptex::tGuard guard( &fAccess );

	if( fLockEvent == NULL )
		fLockEvent = new tThreadEvent( true, false, "", tThreadState::kUseThreadObject );

	fID = id;
	fLockID = 0;
}

// FUNCTION: STARFLEETOP 0x006fb0f0
void tDatabaseObject::RemoveFromDB()
{
	tOptex::tGuard guard( &fAccess );

	fID = tID();
	fLockID = 0;
	if( fLockEvent != NULL )
		fLockEvent->Signal();
}

#line 213
// FUNCTION: STARFLEETOP 0x006fb3f0
void tDatabaseObject::UnLock()
{
	tOptex::tGuard guard( &fAccess );

	validate( fLockEvent );
	fLockID = 0;
	fLockEvent->Signal();
	RemoveReference();
}

// FUNCTION: STARFLEETOP 0x006fb560
int32u tDatabaseObject::GetLockID() const
{
	tOptex::tGuard guard( &fAccess );

	return fLockID;
}

// MATCH-ONLY: back to this file's own numbering; under the one above, this would share lines with MakeReadOnly.
#line 307
// FUNCTION: STARFLEETOP 0x006fb6a0
bool tDatabaseObject::IsLocked() const
{
	tOptex::tGuard guard( &fAccess );

	if( fLockID == 0 )
		return false;

	return true;
}

// FUNCTION: STARFLEETOP 0x006fb7f0
void tDatabaseObject::Kill()
{
	tOptex::tGuard guard( &fAccess );

	fAlive = false;
	guard.Leave();

	CheckForDeath();
}

}
